/*
 * \file        CSV_Paser.c
 * \brief       Read one field of CSV file record.
 * \Author      Cedar
 * \date        2013-11-21
 * \version     V1.2
 * \github      https://github.com/cedar-renjun/CSV_Parser
 * \note        If you have any questions about it, please feel free to write me
 *              My Email is : xuesong5825718@gmail.com**
 *
 * \History
 *  ----------------------V1.0----------------------
 *  First Commit
 *
 *  ----------------------V1.1----------------------
 *  1: Format the source code, remove all tab
 *  2: Add MIT license
 *  3: Remove the back-end space of Record Field
 *
 *  ----------------------V1.2----------------------
 *  1: Add github address
 *  2: Update the version and date
 *
 * \license
 * Copyright (c) 2013 Cedar, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 * and associated documentation files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge, publish, distribute,
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

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
static char * TestStr = "  \"Hello   ,  world\"  ,    Jim   , xxxx, ssss, ";
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

    // Read Field then print them out.
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
    //! -# Remove the end space of double-quotes enclosed block. e.g: "ABCD"    , B, C,
    while(1)
    {

        //! Skip space
        if(*src == ' ')
        {
            src++;
            continue;
        }

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


