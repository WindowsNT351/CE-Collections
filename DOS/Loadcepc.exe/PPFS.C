/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.
Copyright (c) 1995-1998  Microsoft Corporation

Module Name:  
   ppfs.c
   
Abstract:  
   This file implements the NK kernel ppfs client side interface
   
Notes: 
--*/

#include "kernel.h"

/* All strings must be <= 128 bytes including the null termination */

/* Message format: (assume little endians, lower address first>
 * -4      0       4     N-1       N       N+4
 * AA5555AA<header><data><checksum>5AA50A1A
 *
 * Header format:
 * 0       2       4
 * <opcode><length>
 * lsb          msb
 *
 * Length is 16 bit value. It includes all fields (== N)
 * Opcode is 16 bit value.  Current values are:
 * 0x0000 - boot (AA5555AA00000500FA5AA50A1A)
 * 0x0001 - init (AA5555AA01000500F95AA50A1A)
 * 0x0002 - open
 * 0x0003 - close
 * 0x0004 - read
 * 0x0005 - write
 * 0x0006 - seek
 * 0x0007 - delete
 * 0x0008 - findfirst
 * 0x0009 - findnext
 * 0x000A - rRegGet
 * 0x000B - rRegOpen
 * 0x000C - rRegClose
 * 0x000D - rRegEnum
 *
 *
 * Reply to a message will have the same opcode.  Bit15 is set to
 * indicate transmission failure.
 *
 * Data is stream of bytes
 *
 * Checksum is ~(sum of preceding N-1 bytes)
 *
 * Length of data is <length> - 5
 *
 */

CRITICAL_SECTION ppfscs;
extern int NoPPFS;						// parallel port not present flag

int read_value(int *chksum) {
	int loop;
	int result;
	BYTE ch;
	for (loop = 0; loop < sizeof(int); loop++ ) {
		ch = (BYTE)OEMParallelPortGetByte();
		*chksum += ch;
		((LPBYTE)&result)[loop] = ch;
	}
	return result;
}

void write_value(int val, int *chksum) {
	int loop;
	BYTE ch;
	for (loop = 0; loop < 4; loop++) {
		ch = ((LPBYTE)&val)[loop];
		*chksum += ch;
		OEMParallelPortSendByte(ch);
	}
}

int read_header(int *chksum) {
	if (read_value(chksum) != 0xaa5555aa)
		return -1;
	*chksum = 0;
	return read_value(chksum);
}

void write_header(int cmd, int *chksum) {
	write_value(0xaa5555aa,chksum);
	*chksum = 0;
	write_value(cmd,chksum);
}

void read_data(LPBYTE buf, int cnt, int *chksum) {
	BYTE ch;
	while (cnt--) {
		ch = (BYTE)OEMParallelPortGetByte();
		*chksum += ch;
		*buf++ = ch;
	}
}

void write_data(LPBYTE buf, int cnt, int *chksum) {
	while (cnt--) {
		*chksum += *buf;
		OEMParallelPortSendByte(*buf++);
	}
}

/* returns TRUE if success, FALSE if failure */
BOOL read_end(int checksum) {
	BYTE b;
	int tmp;
	b = (BYTE)OEMParallelPortGetByte();
	if (((checksum & 0xff) != b) ||
		(read_value(&tmp) != 0x1a0aa55a))
		return FALSE;
	return TRUE;
}

void write_end(int checksum) {
	int tmp;
	BYTE ch = (BYTE)((checksum & 0xff) ^ 0xff);
	OEMParallelPortSendByte(ch);
	write_value(0x1a0aa55a,&tmp); /* Write out end of message signature */
}

int rlseek(int fd, int off, int mode) {
	int chksum, result;

	EnterCriticalSection(&ppfscs);
	if (NoPPFS)
	{
		LeaveCriticalSection(&ppfscs);
		return -1;
	}
	write_header(0x00110006,&chksum);	/* opcode = 0x0006, length = 17 */
	write_value(fd,&chksum);
	write_value(off,&chksum);
	write_value(mode,&chksum);
	write_end(chksum);
	if (read_header(&chksum) != 0x00090006) { /* opcode = 0x0006, length = 9 */
		LeaveCriticalSection(&ppfscs);
		return -1;
	}
	result = read_value(&chksum);
	if (!read_end(chksum)) {
		LeaveCriticalSection(&ppfscs);
		return -1;
	}
	LeaveCriticalSection(&ppfscs);
	return result;
}

int rwriteshort(int fd, char *buf, int cnt) {
	int chksum, result;

	write_header(0x000d0005+(cnt<<16), &chksum); /* opcode = 0x0005, length = 13 + cnt */
	write_value(fd,&chksum);
	write_value(cnt,&chksum);
	write_data(buf,cnt,&chksum);
	write_end(chksum);
	if (read_header(&chksum) != 0x00090005) /* opcode = 0x0005, length = 9        */
		return -1;
	result = read_value(&chksum);
	if (!read_end(chksum))
		return -1;
	return result;
}

int rwrite(int fd, char *buf, int cnt) {
	int csize, fullsize;
	int result, result2;
	char *buf2 = buf;
	if (NoPPFS)
		return -1;
	fullsize = cnt;
	LockPages(buf,fullsize,0,0);
	EnterCriticalSection(&ppfscs);
	result2 = 0;
	while (cnt) {
		csize = ( cnt > 32*1024L ? 32*1024L : cnt);
		if ((result = rwriteshort(fd,buf2,csize)) == -1) {
			result2 = -1;
			break;
		}
		result2 += result;
		cnt -= csize;
		buf2 += csize;
	}
	LeaveCriticalSection(&ppfscs);
	UnlockPages(buf,fullsize);
	return result2;
}

int rreadshort(int fd, char *buf, int cnt) {
	int chksum, result, size;
	write_header(0x000d0004, &chksum); /* opcode = 0x0004, length = 13 */
	write_value(fd, &chksum);
	write_value(cnt,&chksum);
	write_end(chksum);
	result = read_header(&chksum);
	if ((result & 0xffff) != 0x0004)
		return -1;
	size = ((result >> 16) & 0xffff) - 9; /* subtract header & chksum */
	result = read_value(&chksum);
	read_data(buf,size,&chksum);
	if (!read_end(chksum))
		return -1;
	return result;
}

int rread(int fd, char *buf, int cnt) {
	int csize, fullsize;
	int result, result2;
	char *buf2 = buf;
	if (NoPPFS)
		return -1;
	fullsize = cnt;
	LockPages(buf,fullsize,0,LOCKFLAG_WRITE);
	EnterCriticalSection(&ppfscs);
	result2 = 0;
	while (cnt) {
		csize = ( cnt > 32*1024L ? 32*1024L : cnt);
		if ((result = rreadshort(fd,buf2,csize)) == -1) {
			result2 = -1;
			break;
		}
		result2 += result;
		cnt -= csize;
		buf2 += csize;
    }
	LeaveCriticalSection(&ppfscs);
	UnlockPages(buf,fullsize);
	return result2;
}	

int rclose(int fd) {
	int chksum, result;

	EnterCriticalSection(&ppfscs);
	if (NoPPFS) {
		LeaveCriticalSection(&ppfscs);
		return -1;
	}
	write_header(0x00090003, &chksum); /* opcode = 0x0003, length = 9 */
	write_value(fd,&chksum);
	write_end(chksum);
	if (read_header(&chksum) != 0x00090003) {
		LeaveCriticalSection(&ppfscs);
		return -1;
	}
	result = read_value(&chksum);
	if (!read_end(chksum)) {
		LeaveCriticalSection(&ppfscs);
		return -1;
	}
	LeaveCriticalSection(&ppfscs);
	return result;
}

#define MAX_FILENAME_LEN 256

void SC_PPSHRestart(void) {
	NoPPFS = 0;
}

int ropen(WCHAR *name, int mode) {
	int chksum, result, len;
	char fname[MAX_FILENAME_LEN];
	len = strlenW(name)+1;
	KUnicodeToAscii(fname,name,MAX_FILENAME_LEN);
	if (NoPPFS)
		return -1;
	EnterCriticalSection(&ppfscs);
	write_header(0x00090002 + (len<<16), &chksum); /* opcode = 0x0002, length = 9 + strlen + 1 */
	write_value(mode,&chksum);
	write_data(fname,len,&chksum);
	write_end(chksum);
	if (read_header(&chksum) != 0x00090002) {
		LeaveCriticalSection(&ppfscs);
		return -1;
	}
	result = read_value(&chksum);
	if (!read_end(chksum)) {
		LeaveCriticalSection(&ppfscs);
		return -1;
	}
	LeaveCriticalSection(&ppfscs);
	return result;
}

int rfindfirst(DWORD hFind, WCHAR *pattern, struct _finddata_t *fd) {
	int chksum, result, len;
	char fname[MAX_FILENAME_LEN];

	if (NoPPFS)
		return -1;
    if (pattern != NULL) {
    	len = strlenW(pattern)+1;
    	KUnicodeToAscii(fname,pattern,MAX_FILENAME_LEN);
    } else {
        len = 0;
    }
	EnterCriticalSection(&ppfscs);
	write_header(0x00090008 + (len<<16), &chksum); /* opcode = 0x0002, length = 9 + strlen + 1 */
	write_value(hFind,&chksum);
    if (len != 0) {
    	write_data(fname,len,&chksum);
    }
	write_end(chksum);
	result = read_header(&chksum);
	if ((result & 0xffff) != 0x0008)
		return -1;
	len = ((result >> 16) & 0xffff) - 9; /* subtract header & chksum */
	result = read_value(&chksum);
    if (len != 0) {
    	read_data((LPBYTE)fd,len,&chksum);
    }
	if (!read_end(chksum)) {
		LeaveCriticalSection(&ppfscs);
		return -1;
	}
	LeaveCriticalSection(&ppfscs);
	return result;
}

int rfindnext(DWORD hFind, struct _finddata_t *fd) {
	int chksum, result, len;
	
	if (NoPPFS)
		return -1;
	EnterCriticalSection(&ppfscs);
	write_header(0x00090009, &chksum); /* opcode = 0x0009, length = 9 */
	write_value(hFind,&chksum);
	write_end(chksum);
	result = read_header(&chksum);
	if ((result & 0xffff) != 0x0009)
		return -1;
	len = ((result >> 16) & 0xffff) - 9; /* subtract header & chksum */
	result = read_value(&chksum);
    if (len != 0) {
    	read_data((LPBYTE)fd,len,&chksum);
    }
	if (!read_end(chksum)) {
		LeaveCriticalSection(&ppfscs);
		return -1;
	}
	LeaveCriticalSection(&ppfscs);
	return result;
}

// Registration database access functions for kernel debug support initialization
int rRegOpen(DWORD hKey, CHAR *szName, LPDWORD lphKey) {
	int chksum, len;
	if (NoPPFS)
		return -1;
	len = strlen(szName)+1;
	EnterCriticalSection(&ppfscs);
	write_header(0x0009000B + (len<<16), &chksum); /* opcode = 0x000B, length = 9 + strlen + 1 */
	write_value(hKey,&chksum);
	write_data(szName,len,&chksum);
	write_end(chksum);
	if (read_header(&chksum) != 0x0009000B) {
		LeaveCriticalSection(&ppfscs);
		return -1;
	}
	*lphKey = read_value(&chksum);
	if (!read_end(chksum)) {
		LeaveCriticalSection(&ppfscs);
		return -1;
	}
	LeaveCriticalSection(&ppfscs);
	return 0;
}

int rRegClose(DWORD hKey) {
	int chksum, result;
	if (NoPPFS)
		return -1;
	EnterCriticalSection(&ppfscs);
	write_header(0x0009000C, &chksum); /* opcode = 0x000C, length = 9 */
	write_value(hKey,&chksum);
	write_end(chksum);
	if (read_header(&chksum) != 0x0009000C) {
		LeaveCriticalSection(&ppfscs);
		return -1;
	}
	result = read_value(&chksum);
	if (!read_end(chksum)) {
		LeaveCriticalSection(&ppfscs);
		return -1;
	}
	LeaveCriticalSection(&ppfscs);
	return result;
}

int rRegGet(DWORD hKey, CHAR *szName, LPDWORD lpdwType,
                LPBYTE lpbData, LPDWORD lpdwSize) {
	int chksum, result, len;
	if (NoPPFS)
		return -1;
	len = strlen(szName)+1;
	EnterCriticalSection(&ppfscs);
	write_header(0x0009000A + (len<<16), &chksum); /* opcode = 0x000A, length = 9 + strlen + 1 */
	write_value(hKey,&chksum);
	write_data(szName,len,&chksum);
	write_end(chksum);
	DEBUGMSG(ZONE_DEBUG,(TEXT("NKDBG: RegGet. hKey=%lu, Name=%a\r\n"), hKey, szName));
	result = read_header(&chksum);
	if ((result & 0xffff) != 0x000A)
	{
    	LeaveCriticalSection(&ppfscs);
		return 0;
    }		
	len = ((result >> 16) & 0xffff) - 9; /* subtract header & chksum */
	*lpdwType = read_value(&chksum);
	*lpdwSize = len;
	DEBUGMSG(ZONE_DEBUG,(TEXT("NKDBG: RegGet. Type=%lu, Size=%lu\r\n"), *lpdwType, *lpdwSize));
	read_data(lpbData,len,&chksum);
	if (!read_end(chksum)) {
		LeaveCriticalSection(&ppfscs);
		return 0;
	}
	LeaveCriticalSection(&ppfscs);
	return 1;
}

int rRegEnum(DWORD hKey, DWORD dwIndex, LPBYTE lpbData, LPDWORD lpdwSize) {
	int chksum, result, len;
	if (NoPPFS)
		return -1;
	EnterCriticalSection(&ppfscs);
	write_header(0x000D000D, &chksum); /* opcode = 0x000A, length = 13 */
	write_value(hKey,&chksum);
	write_value(dwIndex,&chksum);
	write_end(chksum);
	DEBUGMSG(ZONE_DEBUG,(TEXT("NKDBG: RegEnum. hKey=%lu, Index=%u\r\n"), hKey, dwIndex));
	result = read_header(&chksum);
	if ((result & 0xffff) != 0x000D)
	{
    	LeaveCriticalSection(&ppfscs);
		return -1;
    }		
	len = ((result >> 16) & 0xffff) - 9; /* subtract header & chksum */
	result = read_value(&chksum);
	*lpdwSize = len;
	DEBUGMSG(ZONE_DEBUG,(TEXT("NKDBG: RegEnum. Return=%lu, Size=%lu\r\n"), result, *lpdwSize));
	read_data(lpbData,len,&chksum);
	if (!read_end(chksum)) {
		LeaveCriticalSection(&ppfscs);
		return -1;
	}
	LeaveCriticalSection(&ppfscs);
	return result;
}

