// Algorithm.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/********************************************************************************
 *              Macro
*********************************************************************************/
#define      BUF_NUM       3
#define      BUF_SIZE      50


/********************************************************************************
 *              Internal data struct
*********************************************************************************/
static char * TestStr = "  \"Hello   ,  world\"  , Jim, xxxx, ssss, ";
static char TargetPool[BUF_NUM][BUF_SIZE];

/********************************************************************************
 *              Function prototype
*********************************************************************************/
char * CSVParser_ReadField(char * src, char * tar);

/********************************************************************************
 *              Demo Applicaton
*********************************************************************************/
int main(int argc, char* argv[])
{
    char * pAddr = TestStr;
    int i        = 0;

    //! Clear Buffer Memory
    memset(TargetPool, 0, BUF_SIZE*BUF_NUM);
    
    // Read Five Field then print them out.
    for(i = 0; i < BUF_NUM; i++)
    {
        pAddr = CSVParser_ReadField(pAddr, TargetPool[i]);
        printf("Field %d value :%s\r\n", i, TargetPool[i]);
    }

	return 0;
}


/*******************************************************************************
 *
 * \breif Read one field of CSV file record.
 *   You can use this function to read an field of record, the format of CSV must
 * follow the document:
 * <RFC4180 : Common Format and MIME Type for Comma-Separated Values (CSV) Files>
 *   This function follow the rule 1-7 of RFC4180, more information, please reference
 * to the linkage below:
 * http://www.rfc-editor.org/rfc/rfc4180.txt 
 *
 * \param  [in] src the begin address of Record.
 * \param  [in] tar the data buffer that store the field of Record read from src.
 * \ret         the begin address of next field.
 *
 * \Author      Cedar
 * \date        2013-11-20
 * \note        If you have any questions about it, please feel free to write me
 *              My Email is : xuesong5825718@gmail.com
 *
*******************************************************************************/
char * CSVParser_ReadField(char * src, char * tar)
{
#define CR              0x0D
#define LF              0x0A

	//! Skip the front space
	while(*src == ' ')
	{
		src++;
	}
	

    //! Double-quotes Enclosed Field ?
    if(*src == '"')
    {
        src++;
        while(1)
        {
            //! Handle double-quotes in enclosed block.
            if(*src == '"')
            {
                //! Find the end of enclosed block, now, need to handle tail space of block.
                //! space is indicated as 'x' in the string below:
                //! "ABCD"xxxx, B, C,
                if(*(++src) != '"')
                {
                    break;
                }
            }

            //! Handle CR/LF in enclosed block.
            if(*src == CR)
            {
                if(*(src+1) == LF)
                {
                    src += 2;
                }
                else
                {
                    src += 1;
                }
            }

            *tar++ = *src++;
        }
    }

    //! Handle
    //! -# General field. e.g: A, BB, CCC, ...
    //! -# The end space of double-quotes enclosed block. e.g: "ABCD"    , B, C,
    while(1)
	{
        //! End with CR/LF
        if((*src == CR) || (*src == LF))
        {
            break;
        }

        //! End with comma symbol
        if(*src != ',')
        {
            *tar++ = *src++;
        }
		else
		{
			break;
		}
	}

    //! return the begin address of next field.
    return (src + 1);
}

