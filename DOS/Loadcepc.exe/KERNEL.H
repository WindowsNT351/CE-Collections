#include "windows.h"
#include <string.h>

#define	LOCKFLAG_WRITE		1

#define	int		long

#define	ZONE_DEBUG			1

#define	DEBUGMSG(a, b)	printf b

int OEMParallelPortGetByte(void);
VOID OEMParallelPortSendByte(BYTE chData);

#define	LockPages(a, b, c, d)
#define	UnlockPages(a, b)

#define	KUnicodeToAscii(a, b, c)	strncpy(a, b, c)
