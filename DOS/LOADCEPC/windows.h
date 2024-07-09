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
#include <stdio.h>
#include <string.h>

typedef struct _CRITICAL_SECTION
{
    int     iDummy;
}   CRITICAL_SECTION, *LPCRITICAL_SECTION;

typedef unsigned long   DWORD, *LPDWORD;
typedef unsigned long   BOOL;
typedef char            CHAR;
typedef unsigned char   BYTE, *LPBYTE, UCHAR, *PUCHAR;
typedef unsigned short  USHORT, *PUSHORT;

#define FALSE           0
#define TRUE            1

#define TEXT
#define VOID            void
#define WCHAR           char
#define TCHAR           char
#define PTCHAR          char *

#define strlenW         strlen

#define OutputDebugString   printf
#define NKDbgPrintfW        printf

#define EnterCriticalSection(a)
#define LeaveCriticalSection(a)
