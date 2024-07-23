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

   mdppfs.c

Abstract:

   This file implements the NK kernel ppfs client side interface

Notes:

--*/

#include "windows.h"
#include "wdm.h"
#include "pc.h"

#define PAR_PORT_BASE       0x0378
#define PAR_PORT_DATA       0
#define PAR_PORT_STAT       1
#define PAR_PORT_CTRL       2

#define PAR_STAT_NBUSY      0x80
#define PAR_STAT_NACK       0x40
#define PAR_STAT_PE         0x20
#define PAR_STAT_SLCT       0x10
#define PAR_STAT_NERR       0x08    // This isn't connected

#define PAR_CTRL_READ       0x20
#define PAR_CTRL_IRQE       0x10
#define PAR_CTRL_SLCT_IN    0x08
#define PAR_CTRL_NINIT      0x04
#define PAR_CTRL_AUTOFEED   0x02
#define PAR_CTRL_STROBE     0x01

#define STATUS_IS_DISCONNECTED(a) \
    ((((a) & (PAR_STAT_PE | PAR_STAT_SLCT)) != PAR_STAT_SLCT) || \
     (((a) & (PAR_STAT_NBUSY | PAR_STAT_NACK)) == PAR_STAT_NACK))

//#define STATUS_IS_DISCONNECTED(a) (!((a) & PAR_STAT_SLCT))

#if DEBUG
#define LOG_ENTRY_TYPE_MASK 0xFF00
#define LOG_ENTRY_DATA_MASK 0x00FF

#define LOG_ENTRY_READ      ((USHORT)('R' << 8))
#define LOG_ENTRY_WRITE     ((USHORT)('W' << 8))
#define LOG_ENTRY_CONTROL   ((USHORT)('C' << 8))
#define LOG_ENTRY_STATUS    ((USHORT)('S' << 8))
#define LOG_ENTRY_DATA      ((USHORT)('D' << 8))
#define LOG_ENTRY_EXIT      ((USHORT)('E' << 8))
#define LOG_ENTRY_EVENT     ((USHORT)('V' << 8))

#define LOG_EVENT_SKIP_RECEIVE      ((USHORT)0)
#define LOG_EVENT_BAD_STATUS        ((USHORT)1)
#define LOG_EVENT_BAD_DISCONNECT    ((USHORT)2)

#define NUMBER_LOG_EVENTS   3

TCHAR   *EventDescriptions[NUMBER_LOG_EVENTS] = 
{
    TEXT("# Skipped Receives"),
    TEXT("# Bad Status"),
    TEXT("# Bad Disconnect"),
};

DWORD   dwEventCounters[NUMBER_LOG_EVENTS];

VOID    LogEntry(USHORT usEntry);
VOID    DumpLog(VOID);
VOID    DumpCounters(VOID);

#define LOG_ENTRY(a)    LogEntry(a)
#define DUMP_LOG()      DumpLog()
#else
#define LOG_ENTRY(a)
#define DUMP_LOG()
#endif

extern volatile DWORD *PtrCurMSec;      /* current millisecond counter */

#define IoPortBase  ((PUCHAR)PAR_PORT_BASE)

BOOL    NoPPFS;
BOOL    bLastOpWasWrite;

BOOL    __inline    VerifyDisconnect(VOID)
{
    int     i;
    USHORT  usStatus;

    for (i = 0; i < 3; i++)
    {
        usStatus = READ_PORT_UCHAR(IoPortBase + PAR_PORT_STAT);

        LOG_ENTRY((USHORT)(LOG_ENTRY_STATUS | usStatus));

        if (!STATUS_IS_DISCONNECTED(usStatus))
        {
            LOG_ENTRY(LOG_ENTRY_EVENT | LOG_EVENT_BAD_STATUS);

            return FALSE;
        }
    }
    return TRUE;
    
}

BOOL    WaitForStatus(USHORT usMask, USHORT usValue)
{
    USHORT  usStatus;
    DWORD   msecStart = *PtrCurMSec;
    int     tries = 0;

    do
    {
        if (*PtrCurMSec - msecStart >= 200)
        {
            if (++tries > 5)
            {
                NoPPFS = TRUE;

                printf("\r\nWaitForStatus: time out (1), status = %2.2X\r\n",
                    usStatus);

                LOG_ENTRY(LOG_ENTRY_EXIT | 2);

                DUMP_LOG();

                return FALSE;
            }
            msecStart = *PtrCurMSec;
        }

        usStatus = READ_PORT_UCHAR(IoPortBase + PAR_PORT_STAT);

        LOG_ENTRY((USHORT)(LOG_ENTRY_STATUS | usStatus));

        //
        // Use SELECTIN to identify the existence of ppsh
        //
        if (STATUS_IS_DISCONNECTED(usStatus))
        {
            if (VerifyDisconnect())
            {
                printf("\r\nWaitForStatus: PPSH disconnected\r\n");

                NoPPFS = TRUE;

                LOG_ENTRY(LOG_ENTRY_EXIT | 1);

                DUMP_LOG();

                return FALSE;
            }
            else
            {
              usStatus = READ_PORT_UCHAR(IoPortBase + PAR_PORT_STAT);
              LOG_ENTRY((USHORT)(LOG_ENTRY_STATUS | usStatus));
            }
        }

        if ((usStatus & usMask) == usValue)
        {
            int     i;

            for (i = 0; i < 2; i++)
            {
                usStatus = READ_PORT_UCHAR(IoPortBase + PAR_PORT_STAT);

                LOG_ENTRY((USHORT)(LOG_ENTRY_STATUS | usStatus));

                if ((usStatus & usMask) != usValue)
                {
                    LOG_ENTRY(LOG_ENTRY_EVENT | LOG_EVENT_BAD_STATUS);

                    break;
                }
            }
        }
    }
    while ((usStatus & usMask) != usValue);

    return TRUE;
}

int OEMParallelPortInit(void)
{
    LOG_ENTRY(LOG_ENTRY_CONTROL | PAR_CTRL_AUTOFEED | PAR_CTRL_STROBE);

    WRITE_PORT_UCHAR(
        IoPortBase + PAR_PORT_CTRL, PAR_CTRL_AUTOFEED | PAR_CTRL_STROBE);

    return TRUE;
}

int OEMParallelPortGetByte(void)
{
    BYTE    value;

    if (NoPPFS)
    {
        return -1;
    }

Retry:
    LOG_ENTRY(LOG_ENTRY_READ);

    LOG_ENTRY(LOG_ENTRY_CONTROL | PAR_CTRL_READ | PAR_CTRL_STROBE);

    WRITE_PORT_UCHAR(IoPortBase + PAR_PORT_CTRL, PAR_CTRL_READ | PAR_CTRL_STROBE);

    if (!WaitForStatus(PAR_STAT_NACK, PAR_STAT_NACK))
    {
        return -1;
    }

    value = READ_PORT_UCHAR(IoPortBase + PAR_PORT_DATA);

    LOG_ENTRY((USHORT)(LOG_ENTRY_DATA | value));

    LOG_ENTRY(LOG_ENTRY_CONTROL | PAR_CTRL_AUTOFEED | PAR_CTRL_STROBE);

    WRITE_PORT_UCHAR(IoPortBase + PAR_PORT_CTRL, PAR_CTRL_AUTOFEED | PAR_CTRL_STROBE);

    if (!WaitForStatus(PAR_STAT_NACK, 0))
    {
        return -1;
    }

    LOG_ENTRY(LOG_ENTRY_EXIT | 0);

    if (bLastOpWasWrite && value == 0x1A)
    {
        //
        // The problem is that periodically the first character we
        // receive after a write is the last byte sent of the previous write.
        //
        // For now we will ignore it
        //
        LOG_ENTRY(LOG_ENTRY_EVENT | LOG_EVENT_SKIP_RECEIVE);
        bLastOpWasWrite = FALSE;
        goto Retry;
    }

    bLastOpWasWrite = FALSE;

    return value;
}

VOID OEMParallelPortSendByte(BYTE chData)
{
    if (NoPPFS)
        return;

    LOG_ENTRY(LOG_ENTRY_WRITE);

    if (!WaitForStatus(PAR_STAT_NBUSY, 0))
    {
        return;
    }

    LOG_ENTRY((USHORT)(LOG_ENTRY_DATA | chData));

    WRITE_PORT_UCHAR(IoPortBase + PAR_PORT_CTRL, PAR_CTRL_AUTOFEED | PAR_CTRL_STROBE);

    WRITE_PORT_UCHAR(IoPortBase + PAR_PORT_DATA, chData);

    LOG_ENTRY(LOG_ENTRY_CONTROL | PAR_CTRL_AUTOFEED);

    WRITE_PORT_UCHAR(IoPortBase + PAR_PORT_CTRL, PAR_CTRL_AUTOFEED);

    if (!WaitForStatus(PAR_STAT_NBUSY, PAR_STAT_NBUSY))
    {
        return;
    }

    LOG_ENTRY(LOG_ENTRY_CONTROL | PAR_CTRL_AUTOFEED | PAR_CTRL_STROBE);

    WRITE_PORT_UCHAR(IoPortBase + PAR_PORT_CTRL, PAR_CTRL_AUTOFEED | PAR_CTRL_STROBE);

    LOG_ENTRY(LOG_ENTRY_EXIT | 0);

    bLastOpWasWrite = TRUE;
}

#if DEBUG
#define LOG_SIZE        0x1000
#define RW_STACK_SIZE   10

WCHAR   wcHexDigits[16] = 
{
    TEXT('0'), TEXT('1'), TEXT('2'), TEXT('3'),
    TEXT('4'), TEXT('5'), TEXT('6'), TEXT('7'), 
    TEXT('8'), TEXT('9'), TEXT('A'), TEXT('B'),
    TEXT('C'), TEXT('D'), TEXT('E'), TEXT('F')
};

USHORT  usLogBuffer[LOG_SIZE];
int     iLogHead = 0;
int     iLogTail = 0;

VOID
LogEntry(USHORT usEntry)
{
    static  USHORT  usLastEntry;
    USHORT          usEntryType;
    USHORT          usEntryData;

    usEntryData = usEntry & LOG_ENTRY_DATA_MASK;
    usEntryType = usEntry & LOG_ENTRY_TYPE_MASK;

    switch (usEntryType)
    {
    case LOG_ENTRY_STATUS:
        if (usLastEntry == usEntry)
        {
            //
            // Don't log duplicate status
            //
            return;
        }
        break;
        
    case LOG_ENTRY_EVENT:
        if (usEntryData < NUMBER_LOG_EVENTS)
        {
            dwEventCounters[usEntryData]++;
        }
        break;
    }
        
    usLastEntry = usEntry;

    usLogBuffer[iLogTail++] = usEntry;
    iLogTail %= LOG_SIZE;

    if (iLogTail == iLogHead)
    {
        iLogHead++;
        iLogHead %= LOG_SIZE;
    }
}

VOID
DumpLog(VOID)
{
    // R 00 W 4F
    TCHAR   szPrintLine[100];
    int     iLogCurrent;
    PTCHAR  pCurrentColumn;
    int     nDataItems;
    USHORT  usCurrentOp, usLastOp;
    int     iReadWriteStack[RW_STACK_SIZE];
    int     i;

    pCurrentColumn = szPrintLine;
    nDataItems = 0;

    usLastOp = 0;

    for (i = 0; i < RW_STACK_SIZE; i++)
    {
        iReadWriteStack[i] = iLogTail;
    }

    for (iLogCurrent = iLogHead; iLogCurrent != iLogTail;
        iLogCurrent++, iLogCurrent %= LOG_SIZE)
    {
        usCurrentOp = usLogBuffer[iLogCurrent] & LOG_ENTRY_TYPE_MASK;
        switch (usCurrentOp)
        {
        case LOG_ENTRY_READ:
        case LOG_ENTRY_WRITE:
            if (usLastOp != usCurrentOp)
            {
                if (pCurrentColumn != szPrintLine)
                {
                    memcpy(pCurrentColumn, TEXT("\r\n"), sizeof(TEXT("\r\n")));
                    OutputDebugString(szPrintLine);
                    pCurrentColumn = szPrintLine;
                }

                *pCurrentColumn++ = (TCHAR)(usLogBuffer[iLogCurrent] >> 8);
                *pCurrentColumn++ = TEXT(' ');
                usLastOp = usCurrentOp;
                nDataItems = 0;
            }

            for (i = 0; i < (RW_STACK_SIZE - 1); i++)
            {
                iReadWriteStack[i] = iReadWriteStack[i + 1];
            }

            iReadWriteStack[RW_STACK_SIZE - 1] = iLogCurrent;
            break;

        case LOG_ENTRY_EVENT:
        case LOG_ENTRY_DATA:
            if (nDataItems == 25)
            {
                memcpy(pCurrentColumn, TEXT("\r\n"), sizeof(TEXT("\r\n")));
                OutputDebugString(szPrintLine);
                pCurrentColumn = szPrintLine;
                *pCurrentColumn++ = TEXT(' ');
                *pCurrentColumn++ = TEXT(' ');
                nDataItems = 0;
            }
            *pCurrentColumn++ = wcHexDigits[(usLogBuffer[iLogCurrent] >> 4) & 0x0F];
            *pCurrentColumn++ = wcHexDigits[usLogBuffer[iLogCurrent] & 0x0F];
            *pCurrentColumn++ = usCurrentOp == LOG_ENTRY_DATA ? TEXT(' ') : TEXT('!');
            nDataItems++;
            break;
        }
    }

    if (pCurrentColumn != szPrintLine)
    {
        memcpy(pCurrentColumn, TEXT("\r\n"), sizeof(TEXT("\r\n")));
        OutputDebugString(szPrintLine);
        pCurrentColumn = szPrintLine;
    }

    nDataItems = 0;

    for (i = 0; i < RW_STACK_SIZE; i++)
    {
        if (iReadWriteStack[i] != iLogTail)
        {
            break;
        }
    }

    iLogCurrent = (i < RW_STACK_SIZE) ? iReadWriteStack[i] : iLogTail;

    for ( ; iLogCurrent != iLogTail; iLogCurrent++, iLogCurrent %= LOG_SIZE)
    {
        if (nDataItems == 16)
        {
            memcpy(pCurrentColumn, TEXT("\r\n"), sizeof(TEXT("\r\n")));
            OutputDebugString(szPrintLine);
            pCurrentColumn = szPrintLine;
            nDataItems = 0;
        }

        *pCurrentColumn++ = (TCHAR)(usLogBuffer[iLogCurrent] >> 8);

        *pCurrentColumn++ = TEXT(' ');

        *pCurrentColumn++ = wcHexDigits[(usLogBuffer[iLogCurrent] >> 4) & 0x0F];

        *pCurrentColumn++ = wcHexDigits[usLogBuffer[iLogCurrent] & 0x0F];

        *pCurrentColumn++ = TEXT(' ');

        nDataItems++;
    }

    if (pCurrentColumn != szPrintLine)
    {
        memcpy(pCurrentColumn, TEXT("\r\n"), sizeof(TEXT("\r\n")));
        OutputDebugString(szPrintLine);
        pCurrentColumn = szPrintLine;
    }

    DumpCounters();
}

VOID
DumpCounters(VOID)
{
    int     i;
    
    for (i = 0; i < NUMBER_LOG_EVENTS; i++)
    {
        if (dwEventCounters[i] != 0)
        {
            NKDbgPrintfW(
                TEXT("%s = %d\r\n"), EventDescriptions[i], dwEventCounters[i]);
        }
    }
}
#endif
