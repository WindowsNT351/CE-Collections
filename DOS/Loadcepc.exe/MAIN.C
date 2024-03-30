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

#include <conio.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "loadcepc.h"
#include "xmsapi.h"
#include "ethernet.h"
#include "..\inc\bootarg.h"

#pragma warning(disable: 4001)

#define CHUNKSIZE   32768
#define COPY_FLAG_UPDATE_EXISTING   0x0001
#define COPY_FLAG_UPDATE_OR_ADD     0x0002

int     LoadImage(BOOL bVerbose, BOOL bParallelDownload, char *pszFileName, PULONG pulEntryAddress);

int     ExecuteImage(BOOL bVerbose, ULONG ulEntryPoint);

void    Launch(ULONG ulEntryPoint, ULONG ulArguments);

UCHAR   SetVideoMode(UCHAR iDesiredMode);

UCHAR   GetPCIConfigMechanism();
BOOL ParseEthernetOptions(char * argstr);

extern int NoPPFS;						// parallel port not present flag
int OEMParallelPortGetByte(void);
void OEMParallelPortSendByte(UCHAR chData);
void OEMParallelPortInit(void);
void DisplayDirectory(char *pszPath);
void GetFile(char *pszSource, char *pszDestination, USHORT usCopyFlags);

BOOL SMCDetect(BYTE **ppbBaseAddress, USHORT *pwIrq);

const unsigned char   BootHeader[] = {
	0xAA,								// header = 4 bytes
	0x55,
	0x55,
	0xAA,
	0x00,								// opcode = 2 bytes (0 = BOOT)
	0x00,
	};
#define BOOT_HEADER_SIZE (sizeof(BootHeader) / sizeof(BootHeader[0]))

const unsigned char   BootTail[] = {
	0x5A,								// trail = 4 bytes
	0xA5,
	0x0A,
	0x1A
	};
#define BOOT_TAIL_SIZE (sizeof(BootTail) / sizeof(BootTail[0]))

#define BOOT_TYPE -4					// boot type for x86

struct  
{
    ULONG   ulRate;
    UCHAR   ucDivisor;
}   BaudTable[] = 
{
    { 9600, 12 },
    { 19200, 6 },
    { 38400, 3 },
    { 57600, 2 },
    { 115200, 1 }
};

#define NUM_BAUD_RATES  (sizeof(BaudTable) / sizeof(BaudTable[0]))

BOOT_ARGS BootArgs;

BOOL	bSerialDownload=FALSE;
extern int OEMReadDebugByte(void);
extern void OEMReadDebugBytes(unsigned char * ucBuffer, int usReadSize);
extern void OEMWriteDebugByte(unsigned char ch);

void
usage(char *pszProgramNameArg)
{
    char *  pszProgram;

    pszProgram = strrchr(pszProgramNameArg, '\\');

    if (pszProgram != NULL)
    {
        pszProgram++;
    }
    else
    {
        pszProgram = pszProgramNameArg;
    }

    printf(
        "%s: Loads an NK ROM image into memory and boots it\n\n"
        "Usage:%s [/B:baud] [/C:port] [/D:display] [/E:Card:IO:IRQ] [/P] [/Q]\n"
        "\t\t[/H] [/V] [<file name>]\n\n"
        "\t/B\tBaudrate - 9600, 19200, 38400, 57600, or 115200 (default 19200)\n"
        "\t/C\tCOM Port - 1 = COM1, 2 = COM2 (default COM1)\n"
        "\t/D\tDisplay Resolution - 0 = 320x200 (default), 1 = 480x240x256,\n"
        "\t\t\t2 = 640x480x256, 3 = 800x600x256, 4 = 1024x768x256\n"
        "\t\t\t5 = 240x320x256 \n"
		"\t/P\tParallel port boot download.\n"
		"\t/Q\tSerial port boot download.\n"
        "\t/E:Card:IO:IRQ\tCard 0 = SMC9000, 1 = NE2000 (For Debug Ethernet)\n"
        "\t\t\t(Other debug ethernet options in hex digits)\n"
        "\t/H\tHelp - displays this message\n"
        "\t/V\tVerbose - displays extra status information during boot\n"
        "\n\tDefault <file name> is NK.BIN except for /P option which uses\n"
	    "\tdefault on host unless <file name> specified.\n"
        "\nPress any key for more options...\n",
        pszProgram, pszProgram);
    __asm 
    {
        push    dx
		xor		ax,ax
		int		0x16
		pop		dx
    }
	printf(
        "usage: %s -s[:]<pattern> -(g|r|u)[[:]<pattern> [<destination>]\n"
        "-s    Show a directory of files matching <pattern>.\n\n"
        "-g    Get files matching <pattern> and copy them to the\n"
        "      optionally specified <destination>.  Any existing file with\n"
        "      the same name will be overwritten.\n\n"
        "-r    Refresh files matching <pattern> which already exist in <destination>\n"
        "      and have a timestamp newer than the one in <destination>.\n\n"
        "-u    Update files matching <pattern> which don't already exist in\n"
        "      <destination> or those that have a timestamp newer than those\n"
        "      in <destination>.\n\n"
        "<pattern> Windows filename path with optional wildcard characters.\n\n"
        "<destination> If not specified then the current directory is the\n"
        "      default.\n"
        "      If specified and it doesn't exist and the last character is \\,\n"
        "      then the directory is created.  Otherwise <destination> is\n"
        "      treated as the name of the file.\n"
        "      It is an error for <pattern> to match multiple files\n"
        "      when <destination> specifies a file.\n",
        pszProgram);

}

int
main(int argc, char **argv)
{

    char *  pszFileName = "nk.bin";
    ULONG   ulEntryPoint;
    ULONG   ulArguments;
    int     xmsError;
    BOOL    bEnabled;
    BOOL    bVerbose = FALSE;
	BOOL	bParallelDownload=FALSE;
    int     iDesiredMode = 0;
    ULONG   ulBaudRate;
    char    cOption;
	int		i;
	int		iPpfstool=0;


    /* Set up boot args struct */
    BootArgs.ucVideoMode     = 0;
    BootArgs.ucComPort       = 1;
    BootArgs.ucBaudDivisor   = 6;
    BootArgs.ucPCIConfigType = 0;
    
    BootArgs.dwSig = BOOTARG_SIG;
    BootArgs.dwLen = sizeof(BOOT_ARGS);
    
    OEMParallelPortInit();				// initialize parallel port
    /*
    ** Parse arguments
    */

    if (argc > 1)
    {
        int iArgIndex;

        for (iArgIndex = 1; iArgIndex < argc; iArgIndex++)
        {
            if (argv[iArgIndex][0] != '/' && argv[iArgIndex][0] != '-')
            {
                break;
            }
			cOption=tolower(argv[iArgIndex][1]);
            switch (argv[iArgIndex][1])
            {
            case 'b':   /* Baud rate */
            case 'B':
                if (argv[iArgIndex][2] != ':' ||
                    argv[iArgIndex][3] < '0' || argv[iArgIndex][3] > '9')
                {
                    printf("Invalid option - %s\n", argv[iArgIndex]);
                    usage(argv[0]);
                    return 1;
                }

                ulBaudRate = atol(&argv[iArgIndex][3]);

                for (i = 0; i < NUM_BAUD_RATES; i++)
                {
                    if (BaudTable[i].ulRate == ulBaudRate)
                    {
                        BootArgs.ucBaudDivisor = BaudTable[i].ucDivisor;
                        break;
                    }
                }

                if (i >= NUM_BAUD_RATES)
                {
                    printf("Unsupported baud rate - %s\n", argv[iArgIndex]);
                    usage(argv[0]);
                    return 1;
                }
                break;

            case 'c':   /* COM port */
            case 'C':
                if (argv[iArgIndex][2] != ':' ||
                    (argv[iArgIndex][3] != '1' && argv[iArgIndex][3] != '2' &&
                     argv[iArgIndex][3] != '3' && argv[iArgIndex][3] != '4'))
                {
                    printf("Invalid option - %s\n", argv[iArgIndex]);
                    usage(argv[0]);
                    return 1;
                }
                BootArgs.ucComPort = (UCHAR)(argv[iArgIndex][3] - '0');
                break;

            case 'd':   /* Display mode */
            case 'D':
                if (argv[iArgIndex][2] != ':' ||
                    argv[iArgIndex][3] < '0' || argv[iArgIndex][3] > '5')
                {
                    printf("Invalid option - %s\n", argv[iArgIndex]);
                    usage(argv[0]);
                    return 1;
                }
                iDesiredMode = argv[iArgIndex][3] - '0';
                break;

			case 'p':	/* Parallel port boot download */
			case 'P':
				bParallelDownload=TRUE;
				pszFileName= NULL;
				break;

			case 'q':	/* Serial port boot download */
			case 'Q':
				bSerialDownload=TRUE;
				pszFileName= NULL;
				break;

            case 'v':
            case 'V':
                bVerbose = TRUE;
                break;

			case 'S':
            case 's':
                {
                    char   *pszPath;
					iPpfstool=1;
                    if (argv[iArgIndex][2] != '\0')
                    {
                        pszPath = &argv[iArgIndex][2];
                        if (*pszPath == ':')
                        {
                            pszPath++;
                        }
                    }
                    else if (argc > (i+1) && argv[iArgIndex+1][0] != '-' && argv[iArgIndex+1][0] != '/')
                    {
                        pszPath = argv[iArgIndex+1];
                        i++;
                    }
                    else
                    {
                        pszPath = "*.*"; 
                    }
                    DisplayDirectory(pszPath);
                }
                break;

                case 'E':
                case 'e':
                    if (!ParseEthernetOptions(argv[iArgIndex])) {
                        printf("Invalid option - %s\n", argv[iArgIndex]);
                        usage(argv[0]);
                        return 1;
                    }
                    break;
                
                case 'G':
                case 'R':
                case 'U':
                case 'g':
                case 'r':
                case 'u':
                {
                    char    *pszSource;
                    char    *pszDestination;

					iPpfstool=1;
                    if (argv[iArgIndex][2] != '\0')
                    {
                        pszSource = &argv[iArgIndex][2];
                        if (*pszSource == ':')
                        {
                            pszSource++;
                        }
                    }
                    else if (argc > (iArgIndex+1) && argv[iArgIndex+1][0] != '-' && argv[iArgIndex+1][0] != '/')
                    {
                        pszSource = argv[iArgIndex+1];
                        iArgIndex++;
                    }
                    else
                    {
                        printf("Error source path missing\n");
                        break;
                    }

                    if (argc > (iArgIndex+1) && argv[iArgIndex+1][0] != '-' && argv[iArgIndex+1][0] != '/')
                    {
                        pszDestination = argv[iArgIndex+1];
                        i++;
                    }
                    else
                    {
                        pszDestination = NULL;
                    }

                    GetFile(
                        pszSource, pszDestination,
                        cOption == 'r' ? COPY_FLAG_UPDATE_EXISTING :
                        cOption == 'u' ? COPY_FLAG_UPDATE_OR_ADD :
                        0);
                }
                break;

                default:
                    printf("Unrecognized option - %s\n", argv[iArgIndex]);
                    /* FALL THROUGH */

                case '?':
                case 'h':
                case 'H':
                    usage(argv[0]);
                    return 1;
            }
        }

        if (iArgIndex == (argc - 1))
        {
            pszFileName = argv[iArgIndex];
        } 
    }

	if (iPpfstool)
		return 0;

    if (!XmsIsInstalled())
    {
        printf("HIMEM.SYS must be loaded\n");
        return 2;
    }

    if (bVerbose)
    {
        if ((xmsError = XmsQueryA20(&bEnabled)) != XMS_SUCCESS)
        {
            printf("Error querying A20 status - %s\n", XmsErrorString(xmsError));

            return 3;
        }

        if (bEnabled)
        {
            printf("Warning A20 line already enabled\n");
        }
    }

    /*
    ** Check if PCI Bios is available, if so find out config mechanism
    */

    BootArgs.ucPCIConfigType = GetPCIConfigMechanism();

    if (!LoadImage(bVerbose, bParallelDownload, pszFileName, &ulEntryPoint))
    {
        return 4;
    }

    /*
    ** Enable A20 line
    */

    if ((xmsError = XmsLocalEnableA20()) != XMS_SUCCESS)
    {
        printf("Can't enable A20 line, error = 0x%4.4X\n", xmsError);

        return 5;
    }

    BootArgs.ucVideoMode = SetVideoMode(iDesiredMode);

	if (bSerialDownload)
		BootArgs.ucBaudDivisor|=0x80;

    if (bVerbose)
    {
        printf("Jumping to 0x%8.8lX\n", ulEntryPoint);
    }

    /*
    ** Convert arguments to linear addres
    */
    
    ulArguments = (((ULONG)(void far *)&BootArgs) >> 16) << 4;
    ulArguments += ((ULONG)(void far *)&BootArgs) & 0xFFFF;

    Launch(ulEntryPoint, ulArguments);

    /*
    ** Launch shouldn't have returned
    */    
    return 0;
}

USHORT DownloadRead(int hImage, UCHAR* ucBuffer, USHORT usReadSize, BOOL bParallelDownload) {
int loop, loop2;

	if (bParallelDownload) {
		for (loop = 0; loop < usReadSize; loop++ ) {
			ucBuffer[loop]= (UCHAR)OEMParallelPortGetByte();
			if (NoPPFS)
				return 0;
		}
		return usReadSize;
	} else if (bSerialDownload) {
		OEMReadDebugBytes(ucBuffer,usReadSize);
		return usReadSize;
	} else
		return _read(hImage, ucBuffer, usReadSize);
}

int
LoadImage(BOOL bVerbose, BOOL bParallelDownload, char *pszFileName, PULONG pulEntryPoint)
{
    int             hImage;
    long            lImageSize;

    int             xmsError;

    USHORT          usTotalFree;
    USHORT          usLargestBlock;
    USHORT          usBlockHandle;

    ULONG           ulLinearAddress;
    
    ULONG           ulSectionAddress;
    ULONG           ulSectionSize;
    ULONG           ulSectionChecksum;

    ULONG           ulSectionOffset;
    ULONG           ulChecksum;
    
    USHORT          usReadSize;
    USHORT          usAmountRead;
	ULONG			ulReadTotal=0;
	ULONG			ulReadProgress=0;

    USHORT          usIndex;

    static UCHAR    ucBuffer[ CHUNKSIZE ];

    static UCHAR    ucSignature[] = { 'B', '0', '0', '0', 'F', 'F', '\n' };
	int i;
	UCHAR			BootPacket[256];
	UCHAR			*pDestByte;
	UCHAR			*pTemp;
	unsigned int	chksum;
	long int		uiTemp;
	long int	 	bootType;
	unsigned		len;

    /*
    ** Stop the floppy motor in case it contained the image file
    */

    __asm 
    {
        push    dx
        mov     dx, 03F2h   ; Floppy motor and DMA control
        in      al, dx
        and     al, 00Fh    ; Clear motor on bits
        out     dx, al
        pop     dx
    }
    /*
    ** Find the largest Extended Memory block and allocate it
    */

    xmsError = XmsQueryFreeExtendedMemory(&usLargestBlock, &usTotalFree);

    if (xmsError != XMS_SUCCESS)
    {
        printf("Error querying free extended memory - %s\n", XmsErrorString(xmsError));
		if (!bParallelDownload && !bSerialDownload)
	        _close(hImage);

        return FALSE;
    }

    if (bVerbose)
    {
        /*
        ** Print info about memory available
        */

        printf(
            "Total free extended memory = %u, largest block = %u\n",
            usTotalFree, usLargestBlock);
    }

    xmsError = XmsAllocateExtendedMemory(usLargestBlock, &usBlockHandle);

    if (xmsError != XMS_SUCCESS)
    {
        printf("Error allocating extended memory - %s\n", XmsErrorString(xmsError));
		if (!bParallelDownload && !bSerialDownload)
	        _close(hImage);

        return FALSE;
    }

    /*
    ** Lock the block and obtain its linear address.  Then ensure that the
    ** image fits into it.
    */

    xmsError = XmsLockExtendedMemory(usBlockHandle, &ulLinearAddress);

    if (xmsError != XMS_SUCCESS)
    {
	    fprintf(stderr, "\r                          \r");
        printf("Error locking extended memory - %s\n", XmsErrorString(xmsError));

        XmsFreeExtendedMemory(usBlockHandle);
		if (!bParallelDownload && !bSerialDownload)
	        _close(hImage);

        return FALSE;
    }

    if (bVerbose)
    {
        /*
        ** Print info about block allocated
        */

	    fprintf(stderr, "\r                          \r");
        printf("Block allocated at 0x%lX\n", ulLinearAddress);
    }

    /*
    ** Zero upper memory in CHUNKSIZE chunks
    */

	memset(ucBuffer, 0, sizeof(ucBuffer));
	ulSectionSize = (ULONG)usLargestBlock * 1024;
#if 1	
    for (ulSectionOffset = 0; ulSectionOffset < ulSectionSize; ulSectionOffset += usReadSize)
    {
        if ((ulSectionSize - ulSectionOffset) < CHUNKSIZE)
        {
            usReadSize = (USHORT)(ulSectionSize - ulSectionOffset);
        }
        else
        {
            usReadSize = CHUNKSIZE;
        }

        xmsError = XmsMoveExtendedMemory(
            0, (ULONG)(UCHAR far *)ucBuffer, 
            usBlockHandle, ulSectionOffset, usReadSize);

        if (xmsError != XMS_SUCCESS)
        {
		    fprintf(stderr, "\r                          \r");
            printf("Error zeroing extended memory - %s\n", XmsErrorString(xmsError));

            XmsUnlockExtendedMemory(usBlockHandle);

            XmsFreeExtendedMemory(usBlockHandle);
			if (!bParallelDownload && !bSerialDownload)
	            _close(hImage);

            return FALSE;
        }
    }
#endif

    /*
    ** Open image file
    */
	if (bSerialDownload)
		OEMInitDebugSerial();
	if (bParallelDownload || bSerialDownload) {
        //
        // Prepare boot packet
        //
        pDestByte = BootPacket;
        for (i = 0; i < BOOT_HEADER_SIZE; i++) {
            *pDestByte++ = BootHeader[i];
        }
        
        chksum = 0;
        len = 9;
        
        if (pszFileName) {
			if (bSerialDownload)
				printf("Loading image %s via serial port.\r\n",pszFileName);
			else
				printf("Loading image %s via parallel port.\r\n",pszFileName);
            bootType = 1;              // The NULL byte
            for (pTemp = pszFileName; *pTemp; pTemp++) {
                bootType++;
            }
            
            len += bootType;
            
        } else { 
			if (bSerialDownload)
				printf("Loading host default image via serial port.\r\n");
            else
				printf("Loading host default image via parallel port.\r\n");
            bootType = BOOT_TYPE; 
        }
        
        uiTemp = len;
        for (i = 0; i < 2; i++) {
            *pDestByte++ = (unsigned char)(uiTemp & 0xFF);
            chksum += (uiTemp & 0xFF);
            uiTemp >>= 8;
        }
        
        uiTemp = bootType;
        for (i = 0; i < 4; i++) {
            *pDestByte++ = (unsigned char)(uiTemp & 0xFF);
            chksum += (uiTemp & 0xFF);
            uiTemp >>= 8;
        }
        
        if (bootType > 0) {
            for (pTemp = pszFileName; *pTemp; pTemp++) {
                *pDestByte++ = *pTemp;
                chksum += *pTemp;
            }
            *pDestByte++ = 0;
        }
        
        *pDestByte++ = (unsigned char)((~chksum) & 0xFF);
        
        for (i = 0; i < BOOT_TAIL_SIZE; i++) {
            *pDestByte++ = BootTail[i];
        }
		if (bVerbose)
			printf("Sending boot packet: ");
		usIndex=0;
		while (&BootPacket[usIndex] < pDestByte) {
			if (NoPPFS)
				return FALSE;
			if (bVerbose)
				printf("%x ",BootPacket[usIndex]);
			if (bSerialDownload)
				OEMWriteDebugByte(BootPacket[usIndex]);
			else
				OEMParallelPortSendByte(BootPacket[usIndex]);
			usIndex++;
		}
		if (bVerbose)
			printf("Sent.\r\n");
	} else {  
	    hImage = _open(pszFileName, _O_BINARY | _O_RDONLY);

	    if (hImage == -1)
	    {
	        printf("%s: Error opening file - %s\n", pszFileName, _strerror(NULL));
            XmsUnlockExtendedMemory(usBlockHandle);
            XmsFreeExtendedMemory(usBlockHandle);

	        return FALSE;
	    }

	    if ((lImageSize = _filelength(hImage)) == -1)
	    {
	        printf("%s: Error obtaining file size - %s\n", pszFileName, _strerror(NULL));
			if (!bParallelDownload && !bSerialDownload)
		        _close(hImage);

	        return FALSE;
	    }
	    if (bVerbose)
	    {
	        printf("Loading %s, size = %ld\n", pszFileName, lImageSize);
	    }
	}

    /*
    ** Read initial signature and physical start and size
    */

    usReadSize = sizeof(ucSignature) + 2 * sizeof(ULONG);

    if (DownloadRead(hImage, &ucBuffer, usReadSize, bParallelDownload) != (int)usReadSize)
    {
        printf("Error reading signature - %s\n", _strerror(NULL));
		if (!bParallelDownload && !bSerialDownload)
	        _close(hImage);

        return FALSE;
    }

    if (memcmp(ucBuffer, ucSignature, sizeof(ucSignature)) != 0)
    {
        printf("Error invalid signature\nData: ");
		for (i=0; i<usReadSize; i++)
			printf("%x ",ucBuffer[i]);
		printf("\r\n");
		if (!bParallelDownload && !bSerialDownload)
	        _close(hImage);

        return FALSE;
    }

	ulReadTotal=*(PULONG)&ucBuffer[sizeof(ucSignature) + sizeof(ULONG)];
    if (bVerbose)
    {
        /*
        ** Print Physical start and size
        */

        printf(
            "Image physical start = 0x%8.8lX, size = %ld\n",
            *(PULONG)&ucBuffer[sizeof(ucSignature)],
            *(PULONG)&ucBuffer[sizeof(ucSignature) + sizeof(ULONG)]);
    }
	// Initialize the percent thingie.
	DrawPercent((DWORD)-1, (DWORD)"");

    /*
    ** Copy file to upper memory in CHUNKSIZE chunks
    */

    for ( ; ; )
    {
        usAmountRead = DownloadRead(hImage, ucBuffer, 3 * sizeof(ULONG), bParallelDownload);
		ulReadProgress+=12;
        if (usAmountRead != 3 * sizeof(ULONG))
        {
		    fprintf(stderr, "\r                          \r");
            printf("Error reading header - %s\n", XmsErrorString(xmsError));

            XmsUnlockExtendedMemory(usBlockHandle);

            XmsFreeExtendedMemory(usBlockHandle);
			if (!bParallelDownload && !bSerialDownload)
	            _close(hImage);

            return FALSE;
        }

        ulSectionAddress = *(PULONG)&ucBuffer[0];
        ulSectionSize = *(PULONG)&ucBuffer[4];
        ulSectionChecksum = *(PULONG)&ucBuffer[8];

        if (ulSectionAddress == 0)
        {
            *pulEntryPoint = ulSectionSize;

            break;
        }

        if (ulSectionAddress < ulLinearAddress ||
            (ulSectionAddress + ulSectionSize) >
                (ulLinearAddress + (ULONG)usLargestBlock * 1024))
        {
		    fprintf(stderr, "\r                          \r");
            printf(
                "Error image section doesn't fit in allocated block\n"
                "Block allocated at 0x%lX, size = %ld\n"
                "Section physical start = 0x%8.8lX, size = %ld\n",
                ulLinearAddress, (ULONG)usLargestBlock * 1024,
                ulSectionAddress, ulSectionSize);
            
            XmsUnlockExtendedMemory(usBlockHandle);

            XmsFreeExtendedMemory(usBlockHandle);
			if (!bParallelDownload && !bSerialDownload)
	            _close(hImage);

            return FALSE;
        }

        if (bVerbose)
        {
		    fprintf(stderr, "\r                          \r");
            printf(
                "Section physical start = 0x%8.8lX, size = %ld\n",
                ulSectionAddress, ulSectionSize);
        }

        ulChecksum = 0;

        for (ulSectionOffset = 0; ulSectionOffset < ulSectionSize; )
        {
            if ((ulSectionSize - ulSectionOffset) < CHUNKSIZE)
            {
                usReadSize = (USHORT)(ulSectionSize - ulSectionOffset);
            }
            else
            {
                usReadSize = CHUNKSIZE;
            }
            usAmountRead = DownloadRead(hImage, ucBuffer, usReadSize,bParallelDownload);

            if (usAmountRead != usReadSize)
            {
			    fprintf(stderr, "\r                          \r");
                printf("Error reading section - %s\n", XmsErrorString(xmsError));

                XmsUnlockExtendedMemory(usBlockHandle);

                XmsFreeExtendedMemory(usBlockHandle);
				if (!bParallelDownload && !bSerialDownload)
	                _close(hImage);

                return FALSE;
            }
			ulReadProgress+= usReadSize;
			DrawPercent(ulReadProgress, ulReadTotal);
#if 0

            for (usIndex = 0; usIndex < usAmountRead; usIndex++)
            {
                ulChecksum += ucBuffer[usIndex];
            }
#endif
#if 1
            xmsError = XmsMoveExtendedMemory(
                0, (ULONG)(UCHAR far *)ucBuffer,
                usBlockHandle, ulSectionAddress - ulLinearAddress + ulSectionOffset,
                (usAmountRead + 1) & ~1U);

            if (xmsError != XMS_SUCCESS)
            {
			    fprintf(stderr, "\r                          \r");
                printf("Error moving extended memory - %s\n", XmsErrorString(xmsError));

                XmsUnlockExtendedMemory(usBlockHandle);

                XmsFreeExtendedMemory(usBlockHandle);
				if (!bParallelDownload && !bSerialDownload)
	                _close(hImage);

                return FALSE;
            }
#endif
            ulSectionOffset += usAmountRead;
        }
#if 0
        if (ulChecksum != ulSectionChecksum)
        {
		    fprintf(stderr, "\r                          \r");
            printf(
                "Bad checksum 0x%8.8lX, expected 0x%8.8lX\n",
                ulChecksum, ulSectionChecksum);

            XmsUnlockExtendedMemory(usBlockHandle);

            XmsFreeExtendedMemory(usBlockHandle);
			if (!bParallelDownload && !bSerialDownload)
	            _close(hImage);

            return FALSE;
        }
#endif
    }

	DrawPercent(ulReadTotal, ulReadTotal);
	if (!bParallelDownload && !bSerialDownload)
	    _close(hImage);

    /*
    ** Stop the floppy motor in case it contained the image file
    */

    __asm 
    {
        push    dx
        mov     dx, 03F2h   ; Floppy motor and DMA control
        in      al, dx
        and     al, 00Fh    ; Clear motor on bits
        out     dx, al
        pop     dx
    }
    fprintf(stderr, "\r                          \r");

    return TRUE;
}

UCHAR
GetPCIConfigMechanism()
{
    UCHAR   ucPCIVersionMajor;
    UCHAR   ucPCIVersionMinor;
    UCHAR   ucPCIBus;
    UCHAR   ucPCIConfigMech;

    __asm
    {
        mov     ax, 0xB101
        int     0x1A

        jc      noPCI

        cmp     dx, 0x4350     ; 'CP'
        
        jne     noPCI

        or      ah, ah
        jnz     noPCI

        mov     ucPCIVersionMajor, bh
        mov     ucPCIVersionMinor, bl
        mov     ucPCIBus, cl

        and     al, 0x03
        mov     ucPCIConfigMech, al
    }

    printf(
        "%d PCI bus%s (Version %X.%2.2X) using Configuration Mechanism # %d\n",
        ucPCIBus + 1, ucPCIBus == 0 ? "" : "ses",
        ucPCIVersionMajor, ucPCIVersionMinor, ucPCIConfigMech);

    return (ucPCIBus << 2) | ucPCIConfigMech;

noPCI:
    printf("PCI bus not detected\n");

    return 0;
}
