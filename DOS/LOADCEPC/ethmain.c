//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this sample source code is subject to the terms of the Microsoft
// license agreement under which you licensed this sample source code. If
// you did not accept the terms of the license agreement, you are not
// authorized to use this sample source code. For the terms of the license,
// please see the license agreement between you and Microsoft or, if applicable,
// see the LICENSE.RTF on your install media or the root of your tools installation.
// THE SAMPLE SOURCE CODE IS PROVIDED "AS IS", WITH NO WARRANTIES.
//
/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:

Abstract:
    Loadcepc Ethernet routines.

Functions:


Notes:

--*/


#include <conio.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "loadcepc.h"
#include "smchw.h"
#include "wdm.h"

#include "ethernet.h"
#include "bootarg.h"

BOOT_ARGS BootArgs;

// The base address for the card
static BYTE volatile *pbEthernetBase;
#define ReadWord( wOffset ) _inpw((USHORT)(pbEthernetBase + wOffset))
#define WriteWord( wOffset, Value ) _outpw((USHORT)(pbEthernetBase + wOffset), (USHORT)Value)


WORD
hex_atoi(char * hexstr)
{
    WORD wRet = 0;
    while (*hexstr) {
        wRet <<= 4;
        switch (*hexstr) {
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            wRet += (WORD)(*hexstr - '0');
            break;

        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            wRet += (WORD)(*hexstr - 'a');
            wRet += 10;
            break;

        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            wRet += (WORD)(*hexstr - 'A');
            wRet += 10;
            break;
        }
        hexstr++;
    }
    return wRet;
}   // hex_atoi


// This routine will take a dotted decimal IP address as represent here and return a binary version of it
DWORD inet_addr( char *pszDottedD ) {

    DWORD dwIP = 0;
    DWORD cBytes;
    char *pszLastNum;

    // Replace the dots with NULL terminators
    pszLastNum = pszDottedD;
    for( cBytes = 0; cBytes < 4; cBytes++ ) {
        while(*pszDottedD != '.' && *pszDottedD != '\0')
            pszDottedD++;
        if (*pszDottedD == '\0' && cBytes != 3)
            return 0;
        *pszDottedD = '\0';
        dwIP |= (atol(pszLastNum) & 0xFF) << (8*cBytes);
        pszLastNum = ++pszDottedD;
    }

    return dwIP;

} // inet_ntoa()


//
// Function to extract the device name from the command line.
//
// Expect argstr = "/N:name
//
// Return FALSE for failure
//
BOOL
ParseDeviceName(char * argstr)
{
    UCHAR *pStart = NULL;
    UCHAR *pEnd   = NULL;

    if (argstr == NULL)
        return(FALSE);

    pStart = (argstr + 2);
    while ((*pStart == ':') || (*pStart == '"'))
    {
        if (*pStart == '\0')
            return(FALSE);

        ++pStart;
    }

    pEnd = pStart;
    while ((*pEnd != '"') && (*pEnd != '\0'))
    {
        ++pEnd;
    } 
    *pEnd = '\0';

    if (pStart == pEnd)
        return(FALSE);
  
    // Leave enough room for two character Ethernet controller type and
    // a numeric value derived from the MAC address, hence the 7.
    // 
    strncpy(BootArgs.szDeviceNameRoot, pStart, (MAX_DEV_NAMELEN - 7));

    return(TRUE);
}


//
// Function to extract the debug ethernet adapter options from the command line
//
// Expect argstr = "/E:<io addr>:<irq>:<debug zone mask>"
//
// Return FALSE for failure
//
BOOL
ParseEthernetOptions(char * argstr)
{
    BOOL bRet;
    char * begin;
    char * end;
    char endch;
    BYTE   *pBase = NULL;
    UCHAR ucIRQ = 0;
    int a;  // argument counter 0 = I/O Base Address, 1 = IRQ, etc.

    bRet = FALSE;

    if (strlen(argstr) < 6) {       // "/E:0:1" or "/K:0:1" at minimum.
        goto peo_exit;
    }

    // If the user wishes to use an RNDIS adapter, check for that here.
    // otherwise, just set it to default and let xBoot.bin change it if necessary.
    if (tolower(*(argstr + 1)) == 'k')
        BootArgs.ucEdbgAdapterType = 'R';
    else
        BootArgs.ucEdbgAdapterType = EDBG_ADAPTER_DEFAULT;

    end = argstr + 2;

    //
    // Extract the I/O Base address and IRQ from the command line
    // (and super secret EDBG debug zone mask)
    //
    for (a = 0; a < 4; a++) {
        begin = end;
        //
        // Colon delimited list
        //
        if (*begin != ':') {
            break;
        }
        begin++;    // skip colon
        end = begin;
        while ((*end) && (*end != ':') && (*end != ' ')) {
            end++;
        }

        if (begin == end) {
            break;
        }

        endch = *end;
        *end = 0;       // temporarily zero terminate the option string

        switch (a) {
        case 0: // I/O base address
            pBase = (BYTE *)hex_atoi(begin);
            break;

        case 1: // IRQ
            ucIRQ = (UCHAR)hex_atoi(begin);
            if (ucIRQ >= 0x10)       // check for decimal value instead of hex value
            {
                ucIRQ -= 6;
            }
            break;

        case 2: // IP address
            if ((BootArgs.EdbgAddr.dwIP = inet_addr(begin)) != 0) {
                printf("Using IP address %u.%u.%u.%u\n",
                       (BYTE)(BootArgs.EdbgAddr.dwIP),
                       (BYTE)(BootArgs.EdbgAddr.dwIP >> 8),
                       (BYTE)(BootArgs.EdbgAddr.dwIP >> 16),
                       (BYTE)(BootArgs.EdbgAddr.dwIP >> 24));
            }
            else
                printf("Invalid IP address %s\n",begin);
            break;

        case 3: // EDBG Debug zone mask (defined in oak\inc\ethdbg.h)
            BootArgs.dwEdbgDebugZone = (DWORD)hex_atoi(begin);
            BootArgs.dwEdbgDebugZone |= 0x10000; // Add DHCP zones
            printf("Using debug zone mask 0x%X\n",BootArgs.dwEdbgDebugZone);
            break;

        default:    // error!
            goto peo_exit;
        }

        *end = endch;   // un-zero-terminate if need be.
    }

    if (a < 2) {
        goto peo_exit;
    }

    BootArgs.ucEdbgIRQ         = ucIRQ;
    // WARNING - must mask off high bits
    BootArgs.dwEdbgBaseAddr    = (DWORD)pBase & 0xFFFF;
    BootArgs.ucLoaderFlags     |= LDRFL_USE_EDBG;
    printf("Debug network card at I/O port 0x%x, IRQ 0x%x\n", pBase, ucIRQ);
    bRet = TRUE;

peo_exit:
    return bRet;
}   // ParseEthernetOptions
