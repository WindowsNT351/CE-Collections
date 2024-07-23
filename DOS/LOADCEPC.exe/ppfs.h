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
long rlseek(long fd, long off, long mode);
long rwrite(long fd, char *buf, long cnt);
long rread(long fd, char *buf, long cnt);
long rclose(long fd);
long ropen(WCHAR *name, long mode);

long rRegOpen(DWORD hKey, CHAR *szName, LPDWORD lphKey);
long rRegClose(DWORD hKey);
long rRegGet(DWORD hKey, CHAR *szName, LPDWORD lpdwType,
                LPBYTE lpbData, LPDWORD lpdwSize);
long rRegEnum(DWORD hKey, DWORD dwIndex, LPBYTE lpbData, LPDWORD lpdwSize);

long rfindfirst(DWORD hFind, WCHAR *pattern, struct _finddata_t *fd);
long rfindnext(DWORD hFind, struct _finddata_t *fd);
