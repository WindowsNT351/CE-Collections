/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.
Copyright (c) 1995-1998  Microsoft Corporation

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
