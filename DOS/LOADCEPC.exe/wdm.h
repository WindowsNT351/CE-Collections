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
int __cdecl _inp(unsigned);
int __cdecl _outp(unsigned, int);

#pragma intrinsic(_inp, _outp)

#define READ_PORT_UCHAR(a)      _inp((unsigned)a)
#define WRITE_PORT_UCHAR(a, b)  _outp((unsigned)a, (int)b)
