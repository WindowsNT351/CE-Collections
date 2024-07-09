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
#include "windows.h"
#include <string.h>

#define LOCKFLAG_WRITE      1

#define int     long

#define ZONE_DEBUG          1

#define DEBUGMSG(a, b)  printf b

int OEMParallelPortGetByte(void);
VOID OEMParallelPortSendByte(BYTE chData);

#define LockPages(a, b, c, d)
#define UnlockPages(a, b)

#define KUnicodeToAscii(a, b, c)    strncpy(a, b, c)
