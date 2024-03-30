/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.
Copyright (c) 1995-1998  Microsoft Corporation

Module Name:  

Abstract:  
    Routines for the SMC9000 ethernet controller used for the debug
    ethernet services.  This file has been stripped of all but the
    detect and functionality

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
#include "..\inc\bootarg.h"

BOOT_ARGS BootArgs;

// The base address for the card
static BYTE volatile *pbEthernetBase;
#define ReadWord( wOffset ) _inpw((USHORT)(pbEthernetBase + wOffset))
#define WriteWord( wOffset, Value ) _outpw((USHORT)(pbEthernetBase + wOffset), (USHORT)Value)

typedef struct _SMC_CONFIG
{
    BYTE   *pbIoBase;
    USHORT wIRQ;
    
} SMC_CONFIG;

SMC_CONFIG Configs[] =
{
    {  0x300, 2 },
    {  0x320, 10},
    {  0x340, 11},
    {  0x360, 3}
};

#define NUM_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define NUM_SMC_CONFIGS (NUM_ELEMENTS(Configs))

BOOL
SMCDetect(BYTE **ppbBaseAddress, USHORT *pwIrq)
{
    int i;
    USHORT BSR;
    USHORT MyAddr[3];
    
    for (i=0;i<NUM_SMC_CONFIGS;i++) {
        *ppbBaseAddress = (BYTE *)pbEthernetBase = Configs[i].pbIoBase;
        *pwIrq          = Configs[i].wIRQ;
        // The upper byte of the BSR always reads 0x33, use this to verify the I/O base
        BSR = ReadWord(BANKSEL_REG);
        if ((BSR & 0xFF00) == 0x3300) {
            printf("SMCDetect: SMC board found at I/O base 0x%X\n",*ppbBaseAddress);
            
            // Verify that a valid Ethernet address is loaded
            WriteWord( BANKSEL_REG, BANK1 );
            MyAddr[0] = ReadWord( MACADDR_REG0 );
            MyAddr[1] = ReadWord( MACADDR_REG1 );
            MyAddr[2] = ReadWord( MACADDR_REG2 );

            if (MyAddr[0] || MyAddr[1] || MyAddr[2]) {
                printf( "MAC Address:%02X:%02X:%02X:%02X:%02X:%02X\r\n",
                        MyAddr[0] & 0x00FF, MyAddr[0] >> 8,
                        MyAddr[1] & 0x00FF, MyAddr[1] >> 8,
                        MyAddr[2] & 0x00FF, MyAddr[2] >> 8  );
                return TRUE;
            }
            else {
                printf("SMCDetect: Invalid Ethernet address (check switch settings)\n");
                return FALSE;
            }   
        }
    }
    printf("SMCDetect: SMC board not found\n");
    return FALSE;
}   // SMCDetect

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


//
// Function to extract the debug ethernet adapter options from the command line
//
// Expect argstr = "/E:<card>:<io addr>:<irq>:<debug zone mask>"
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
    char CardType;
    char * card;
    BYTE   *pBase;
    USHORT wIRQ;
    int a;  // argument counter 0 = CardType, 1 = I/O Base Address, 2 = IRQ

    bRet = FALSE;

    if (strlen(argstr) < 10) {       // "/E:1:123:1" at minimum
        goto peo_exit;
    }

    end = argstr + 2;

    //
    // Extract the CardType, I/O Base address and IRQ from the command line
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
        while ((*end) && (*end != ':')) {
            end++;
        }

        if (begin == end) {
            break;
        }

        endch = *end;
        *end = 0;       // temporarily zero terminate the option string

        switch (a) {
        case 0: // CardType
            CardType = *begin;
            break;

        case 1: // I/O base address
            pBase = (BYTE *)hex_atoi(begin);
            break;

        case 2: // IRQ
            wIRQ = hex_atoi(begin);
            break;

        case 3: // EDBG Debug zone mask (defined in oak\inc\ethdbg.h)
            BootArgs.dwEdbgDebugZone = (DWORD)hex_atoi(begin);
            BootArgs.dwEdbgDebugZone |= 0x10000;
            break;

        default:    // error!
            goto peo_exit;
        }

        *end = endch;   // un-zero-terminate if need be.
    }

    if (a < 3) {
        goto peo_exit;
    }

    switch (CardType) {
    case '0':
        // Search for a SMC9000 debug Ethernet card
        if (SMCDetect(&pBase, &wIRQ))
            BootArgs.ucEdbgAdapterType = EDBG_ADAPTER_SMC9000;
        else {
            printf("Could not detect SMC9000 debug Ethernet card\n");
            goto peo_exit;
        }
        card = "SMC9000";
        break;

    case '1':
        BootArgs.ucEdbgAdapterType = EDBG_ADAPTER_NE2000;
        card = "NE2000";
        break;

    default:
        goto peo_exit;
    }

    BootArgs.ucEdbgIRQ         = wIRQ;
    // WARNING - must mask off high bits
    BootArgs.dwEdbgBaseAddr    = (DWORD)pBase & 0xFFFF;
    BootArgs.ucLoaderFlags     |= LDRFL_USE_EDBG;
    printf("Debug network card: %s at I/O port 0x%x, IRQ 0x%x\n", card, pBase, wIRQ);
    bRet = TRUE;

peo_exit:
    return bRet;
}   // ParseEthernetOptions
