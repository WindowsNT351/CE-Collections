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

Functions:


Notes: 

--*/
#ifndef _ETHERNET_H
#define _ETHERNET_H 1


/*
 * Addressing info struct.  We don't ARP for unknown ethernet addresses, so
 * even the ethernet address must be configured.
 */
typedef struct _EDBG_ADDR {
    DWORD  dwIP;
    USHORT wMAC[3];
    USHORT wPort;
    
} EDBG_ADDR;


#define EDBG_SYSINTR_NOINTR  0xFFFFFFFF

// The following defs can be used for platforms which support multiple adapter types.
// Identifiers are provided here for drivers which are built in common\oak\drivers\ethdbg.
// Any platform specific adapter types can be identified based off of EDBG_ADAPTER_OEM.
#define EDBG_ADAPTER_SMC9000  0
#define EDBG_ADAPTER_NE2000   1

#define EDBG_ADAPTER_OEM      16


#endif // _ETHERNET_H
