#include <stdio.h>
#include <string.h>

typedef struct _CRITICAL_SECTION
{
    int		iDummy;
}   CRITICAL_SECTION, *LPCRITICAL_SECTION;

typedef unsigned long   DWORD, *LPDWORD;
typedef unsigned long   BOOL;
typedef	char			CHAR;
typedef unsigned char   BYTE, *LPBYTE, UCHAR, *PUCHAR;
typedef unsigned short  USHORT, *PUSHORT;

#define FALSE           0
#define TRUE            1

#define TEXT
#define VOID            void
#define	WCHAR			char
#define	TCHAR			char
#define	PTCHAR			char *

#define	strlenW			strlen

#define	OutputDebugString	printf
#define	NKDbgPrintfW	    printf

#define	EnterCriticalSection(a)
#define	LeaveCriticalSection(a)
