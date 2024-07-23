//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//

/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:    main.c

Abstract:       This program, loadcepc.exe, is responsible for loading
                a binary image into memory and booting the image. It 
                processes the boot arguments, loads the image file, and 
                then executes it.

Functions:


Notes:          none

--*/

// Include Files

#include <conio.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "loadcepc.h"
#include "xmsapi.h"
#include "ethernet.h"
#include <bootarg.h>


// Externs

extern int NoPPFS;                      // parallel port not present flag


// Defines

#define CHUNKSIZE                   (32768)
#define COPY_FLAG_UPDATE_EXISTING   (0x0001)
#define COPY_FLAG_UPDATE_OR_ADD     (0x0002)


// Variables

// Boot Header

static const unsigned char BootHeader[] = 
{
    0xAA,                               // header = 4 bytes
    0x55,
    0x55,
    0xAA,
    0x00,                               // opcode = 2 bytes (0 = BOOT)
    0x00,
};

#define BOOT_HEADER_SIZE (sizeof(BootHeader) / sizeof(BootHeader[0]))


// Boot Trailer

static const unsigned char BootTail[] = 
{
    0x5A,                               // trail = 4 bytes
    0xA5,
    0x0A,
    0x1A
};

#define BOOT_TAIL_SIZE (sizeof(BootTail) / sizeof(BootTail[0]))

#define BOOT_TYPE (-4)                   // boot type for x86


// Baud Rate

struct
{
    ULONG   ulRate;
    UCHAR   ucDivisor;
}   
BaudTable[] =
{
    { 9600,  12},
    { 19200,  6},
    { 38400,  3},
    { 57600,  2},
    { 115200, 1}
};

#define NUM_BAUD_RATES  (sizeof(BaudTable) / sizeof(BaudTable[0]))


BOOT_ARGS   BootArgs;
USHORT      usBlockHandle;                  // XMS memory handle
BOOL        bSerialDownload=FALSE;


// Function Prototypes

// External Functions

void    OEMInitDebugSerial(void);
int     OEMReadDebugByte(void);
void    OEMReadDebugBytes(unsigned char * ucBuffer, int usReadSize);
void    OEMWriteDebugByte(unsigned char ch);
int     OEMParallelPortGetByte(void);
void    OEMParallelPortSendByte(UCHAR chData);
void    OEMParallelPortInit(void);
void    Launch( ULONG ulEntryPoint, ULONG ulArguments );

// in video.c

UCHAR   SetVideoMode( int iDesiredMode);
BOOL    ParseVESAOptions( char * params);

// in ethmain.c

BOOL    ParseEthernetOptions( char * argstr);
BOOL    ParseDeviceName( char * argstr);

// in ppfstool.c

void    DrawPercent( DWORD CurValue, DWORD MaxValue);
void    DisplayDirectory( char *pszPath);
void    GetFile( char *pszSource, char *pszDestination, USHORT usCopyFlags);

// Local Functions

static void         usage( char *pszProgramNameArg );
static void         ExitSigHandler( int sig );
static WORD         GetJumpFlags( void );
static UCHAR        GetPCIConfigMechanism();
static USHORT       DownloadRead( int     hImage, 
                                  UCHAR  *ucBuffer, 
                                  USHORT  usReadSize, 
                                  BOOL    bParallelDownload );
static int          LoadImage   ( BOOL bVerbose, 
                                  BOOL bParallelDownload, 
                                  char *pszFileName, 
                                  PULONG pulEntryAddress);
static void	DisplayCEOSVersion( char *pszFileName);


//------------------------------------------------------------------------------
//
//  Function Name:  main( int argc, char **argv )
//  Description..:  Main entry point. 
//  Inputs.......:  argc, argv      
//  Outputs......:  int             exit status
//
//------------------------------------------------------------------------------

int     iDesiredMode = 0;
int main( int argc, char **argv )
{
    char *  pszFileName = "300STD.bin";
    ULONG   ulEntryPoint;
    ULONG   ulArguments;
    int     xmsError;
    BOOL    bEnabled;
    BOOL    bVerbose = FALSE;
    BOOL    bParallelDownload=FALSE;
    BOOL    bBackupImage=FALSE;
    ULONG   ulBaudRate;
    char    cOption;
    int     i = 0;
    int     ret = 0;
    int     iPpfstool=0;
    BOOL    bRNDISDownload=FALSE;
    BOOL    bEtherDownload=FALSE;

    // Setup signal handler for Ctrl+C/Ctrl+Break

    signal( SIGINT,  ExitSigHandler );
    signal( SIGTERM, ExitSigHandler );
    signal( SIGABRT, ExitSigHandler );

    // Initialize the boot args block

    memset( &BootArgs, 0, sizeof(BOOT_ARGS));
    BootArgs.ucVideoMode     = 0;
    BootArgs.ucComPort       = 1;
    BootArgs.ucBaudDivisor   = 3;           // Default 38400
    BootArgs.ucPCIConfigType = 0;
    BootArgs.dwSig           = BOOTARG_SIG;
    BootArgs.dwLen           = sizeof(BOOT_ARGS);
    BootArgs.dwVersionSig    = BOOT_ARG_VERSION_SIG;
    BootArgs.MajorVersion    = BOOT_ARG_MAJOR_VER;
    BootArgs.MinorVersion    = BOOT_ARG_MINOR_VER;

    usBlockHandle = 0;

    // Initialize parallel port

    OEMParallelPortInit();              

    // Process arguments 

    if( argc > 1 )
    {
        int iArgIndex;

        for( iArgIndex = 1; iArgIndex < argc; iArgIndex++ )
        {
            if( argv[iArgIndex][0] != '/' && argv[iArgIndex][0] != '-' )
            {
                break;
            }

            cOption = (char)tolower(argv[iArgIndex][1]);

            // Process according to option character

            switch( cOption )
            {
            case 'b':   /* Baud rate */

                if( argv[iArgIndex][2] != ':' ||
                    argv[iArgIndex][3] < '0' || argv[iArgIndex][3] > '9' )
                {
                    printf("Invalid option - %s\n", argv[iArgIndex]);
                    ret = 1;
                    goto main_error;
                }

                ulBaudRate = atol(&argv[iArgIndex][3]);

                for( i=0; i < NUM_BAUD_RATES; i++ )
                {
                    if( BaudTable[i].ulRate == ulBaudRate )
                    {
                        BootArgs.ucBaudDivisor = BaudTable[i].ucDivisor;
                        break;
                    }
                }

                if( i >= NUM_BAUD_RATES )
                {
                    printf( "Unsupported baud rate - %s\n", argv[iArgIndex] );
                    ret = 1;
                    goto main_error;
                }
                break;

            case 'c':

                // COM Port Parameter
                //
                // Allowable values for the COM ports 1-4. Zero (0) is 
                // allowed as a way to disable use of the COM port and is
                // allowed by ...\csp\i486\oal\debug.c.

                if( (argv[iArgIndex][2] == ':') &&
                    (argv[iArgIndex][3] >= '0') &&  
                    (argv[iArgIndex][3] <= '4') )
                {
                    BootArgs.ucComPort = (UCHAR)(argv[iArgIndex][3] - '0');
                }
                else
                {
                    printf( "Invalid option - %s\n", argv[iArgIndex] );
                    ret = 1;
                    goto main_error;
                }
                break;

            case 'd':   /* Display mode */

                if( argv[iArgIndex][2] != ':' ||
                    argv[iArgIndex][3] < '0' || argv[iArgIndex][3] > '6' )
                {
                    printf( "Invalid option - %s\n", argv[iArgIndex] );
                    ret = 1;
                    goto main_error;
                }
                iDesiredMode = (int)(argv[iArgIndex][3] - '0');
                break;

            case 'e':
                if( bRNDISDownload )
                {
                    printf( "The /E and /K options are mutually exclusive"
                            " - please choose one of them.\r\n" );

                    goto main_error;
                }
                if( !ParseEthernetOptions(argv[iArgIndex]) )
                {
                    printf("Invalid option - %s\n", argv[iArgIndex]);
                    ret = 1;
                    goto main_error;
                }
                bEtherDownload = TRUE;
                break;

            case 'k':   /* RNDIS flags */

                if( bEtherDownload )
                {
                    printf( "The /E and /K options are mutually exclusive"
                            "- please choose one of them.\r\n");
                    goto main_error;
                }
                if( !ParseEthernetOptions(argv[iArgIndex]) )
                {
                    printf("Invalid option - %s\n", argv[iArgIndex]);
                    ret = 1;
                    goto main_error;
                }
                bRNDISDownload = TRUE;
                break;

            case 'l':   

                // VESA VBE Linear/Flat Framebuffer display mode 

                if( !ParseVESAOptions( argv[iArgIndex] ) )
                {
                    printf( "Invalid option - %s\n", argv[iArgIndex] );
                    ret = 1;
                    goto main_error;
                }
                iDesiredMode = BootArgs.vesaMode;
#if 0

                // Video Mode Debug - display and hang

                printf( "VESA Boot Options\n" );
                printf( "vesaMode........: 0x%x\n", BootArgs.vesaMode );
                printf( "xDisplayScreen..: %d\n", BootArgs.cxDisplayScreen );
                printf( "YDisplayScreen..: %d\n", BootArgs.cyDisplayScreen );
                printf( "bppScreen.......: %d\n", BootArgs.bppScreen );
                printf( "xPhysScreen.....: %d\n", BootArgs.cxPhysicalScreen );  
                printf( "yPhysScreen.....: %d\n", BootArgs.cyPhysicalScreen );  

                while( 1 )
                {
                    ;
                }
#endif
                break;

            case 'n':   /* Device name specified */

                if( !ParseDeviceName(argv[iArgIndex]) )
                {
                    printf("Invalid option - %s\n", argv[iArgIndex]); 
                    ret = 1;
                    goto main_error;
                }
                break;

            case 'p':   /* Parallel port boot download */

                bParallelDownload=TRUE;
                pszFileName= NULL;
                break;

            case 'a':   /* Use permanent storage to store image */

                bBackupImage=TRUE;
                break;

            case 'q':   /* Serial port boot download */

                bSerialDownload=TRUE;
                pszFileName= NULL;
                break;

            case 'v':

                bVerbose = TRUE;
                break;

            case 's':
                {
                    char   *pszPath;

                    iPpfstool=1;

                    if( argv[iArgIndex][2] != '\0' )
                    {
                        pszPath = &argv[iArgIndex][2];
                        if( *pszPath == ':' )
                        {
                            pszPath++;
                        }
                    }
                    else if( (argc > (i+1) && argv[iArgIndex+1][0] != '-') && 
                             (argv[iArgIndex+1][0] != '/') )
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

            case 'g':
            case 'r':
            case 'u':
                {
                    char    *pszSource;
                    char    *pszDestination;

                    iPpfstool=1;
                    if( argv[iArgIndex][2] != '\0' )
                    {
                        pszSource = &argv[iArgIndex][2];
                        if( *pszSource == ':' )
                        {
                            pszSource++;
                        }
                    }
                    else if( argc > (iArgIndex+1) && argv[iArgIndex+1][0] != '-' && argv[iArgIndex+1][0] != '/' )
                    {
                        pszSource = argv[iArgIndex+1];
                        iArgIndex++;
                    }
                    else
                    {
                        printf("Error source path missing\n");
                        break;
                    }

                    if( argc > (iArgIndex+1) && argv[iArgIndex+1][0] != '-' && argv[iArgIndex+1][0] != '/' )
                    {
                        pszDestination = argv[iArgIndex+1];
                        i++;
                    }
                    else
                    {
                        pszDestination = NULL;
                    }

                    GetFile( pszSource, pszDestination,
                             cOption == 'r' ? COPY_FLAG_UPDATE_EXISTING :
                             cOption == 'u' ? COPY_FLAG_UPDATE_OR_ADD :
                             0 );
                }
                break;

            default:

                printf("Unrecognized option - %s\n", argv[iArgIndex]);

                // Fall through to display usage 

            case '?':
            case 'h':

                // Display usage data

                usage( argv[0] );
                ret = 1;
                goto main_error;
            }
        }

        if( iArgIndex == (argc - 1) )
        {
            pszFileName = argv[iArgIndex];
        }
    }

    if( iPpfstool )
    {
        ret = 0;
        goto main_error;
    }

    if( !XmsIsInstalled() )
    {
        printf( "HIMEM.SYS must be loaded\n" );
        ret = 2;
        goto main_error;
    }

    if( bVerbose )
    {
        if( (xmsError = XmsQueryA20(&bEnabled)) != XMS_SUCCESS )
        {
            printf( "Error querying A20 status - %s\n", 
                XmsErrorString(xmsError) );
            ret = 3;
            goto main_error;
        }

        if( bEnabled )
        {
            printf( "Warning A20 line already enabled\n" );
        }
    }

    // Check if PCI Bios is available, if so find out config mechanism

    BootArgs.ucPCIConfigType = GetPCIConfigMechanism();

    // Load the boot image

    if( !LoadImage( bVerbose, bParallelDownload, pszFileName, &ulEntryPoint) )
    {
        ret = 4;
        goto main_error;
    }

    // Enable A20 line

    if( (xmsError = XmsLocalEnableA20()) != XMS_SUCCESS )
    {
        printf("Can't enable A20 line, error = 0x%4.4X\n", xmsError);

        ret = 5;
        goto main_error;
    }

    // Set the video mode

    BootArgs.ucVideoMode = SetVideoMode( iDesiredMode );

    if( BootArgs.ucVideoMode == 0 )
    {
        // if using default mode set params for flat framebuffer driver.

        BootArgs.cxDisplayScreen     = 320; 
        BootArgs.cyDisplayScreen     = 200;
        BootArgs.cxPhysicalScreen    = 320; 
        BootArgs.cyPhysicalScreen    = 200;
        BootArgs.bppScreen           = 8;
        BootArgs.cbScanLineLength    = 320;
        BootArgs.pvFlatFrameBuffer   = 0x800A0000;
    }
    else if( BootArgs.ucVideoMode != 0xFF )
    {
        // if not using VESA mode clear these so flat 
        // framebuffer driver sees all zeroes and faults.

        BootArgs.cxDisplayScreen     = 0;   
        BootArgs.cyDisplayScreen     = 0;
        BootArgs.cxPhysicalScreen    = 0;   
        BootArgs.cyPhysicalScreen    = 0;
        BootArgs.bppScreen           = 0;
        BootArgs.cbScanLineLength    = 0;
        BootArgs.pvFlatFrameBuffer   = 0;
    }

    if( bSerialDownload )
    {
        BootArgs.ucBaudDivisor|=0x80;
    }

    if( bParallelDownload )
    {
        BootArgs.KitlTransport = GetJumpFlags();
    }

    if( bBackupImage )
    {
        BootArgs.ucLoaderFlags |= LDRFL_FLASH_BACKUP;
    }

    if( bVerbose )
    {
        printf( "Jumping to 0x%8.8lX\n", ulEntryPoint );
    }

    // Convert arguments to linear addres

    ulArguments = (((ULONG)(void far *)&BootArgs) >> 16) << 4;
    ulArguments += ((ULONG)(void far *)&BootArgs) & 0xFFFF;

    // Execute the image

    Launch( ulEntryPoint, ulArguments );

    // Launch should not return

    return( 0 );


    // Main Error Handler

main_error:

    // Make sure we unlock and free extended memory

    if( usBlockHandle )
    {
        XmsUnlockExtendedMemory(usBlockHandle);
        XmsFreeExtendedMemory(usBlockHandle);
    }

    //printf("\n\n               Press any key to cancel.\n");
	printf("\n\n             ==============Press any key to cancel==============\n ");
	__asm
    {
        push    dx
        xor     ax,ax
        int     0x16
        pop     dx
    }
    // Indicate status

    return( ret );
}

//------------------------------------------------------------------------------
//
//  Function Name:  usage( char *pszProgramNameArg )
//  Description..:  This function displays program usage information.
//  Inputs.......:  char *      ProgramName
//  Outputs......:  none
//
//------------------------------------------------------------------------------

static void usage( char *pszProgramNameArg ) 
{
    char *pszProgram;

    pszProgram = strrchr(pszProgramNameArg, '\\');

    if( pszProgram != NULL )
    {
        pszProgram++;
    }
    else
    {
        pszProgram = pszProgramNameArg;
    }

    printf(
          "\n"
          "%s: Loads a BIN image into memory and boots it.\n"
          "\n"
          "Usage:%s /B:baud /C:Port /D:display /P /H /V /N:name\n"
          "  (/E|/K):IO:IRQ[:<dotted IP>] /L:VXxVYxBPP[:PXxPY] <file name>\n"
          "\n"
          "  /B  Baud Rate [9600|19200|38400(default)|57600|115200]"
          "\n"
          "  /C  Communications Port [0=disable|1|2|3|4]"
          "\n"
          "  /D  Display Resolution (for VESA modes, use the /L option instead).\n"
          "      0 = 320x200x256 (default), 1 = 480x240x256,\n"
          "      2 = 640x480x256, 3 = 800x600x256, 4 = 1024x768x256\n"
          "      5 = 240x320x256, 6 = 208x240x256\n"
          "\n"
          "  /P  Parallel port boot download.\n"
          "\n"
          "  /A  Use permanent storage device to store NK image.\n"
          "\n"
          "  /H  Help - displays this message.\n"
          "\n"
          "  Default <file name> is NK.BIN except for /P option which uses\n"
          "  default on host unless <file name> specified.\n"
          "\n"
          "Press any key for more options...",

          pszProgram, pszProgram );

    __asm
    {
        push    dx
        xor     ax,ax
        int     0x16
        pop     dx
    }

    printf(
          "\n"
          "\n"
          "\n"
          "Usage:%s /B:baud /C:Port /D:display /P /H /V /N:name\n"
          "  (/E|/K):IO:IRQ[:<dotted IP>] /L:VXxVYxBPP[:PXxPY] <file name>\n"
          "\n"
          "  /V  Verbose - displays extra status information during boot.\n"
          "\n"
          "  /E:IO:IRQ[:<dotted IP>]  (Debug Ethernet Card parameters)\n"
          "      IO - IO Base Address (specified in hexadecimal)\n"
          "      IRQ - Interrupt (specified in decimal or hexadecimal)\n"
          "      Dotted IP - (optional) static IP address (No DHCP)\n"
          "    NOTE: The /E and /K options are mutually exclusive.\n"
          "\n"
          "  /K:IO:IRQ[:<dotted IP>]  (RNDIS Card parameters)\n"
          "      IO - IO Base Address (specified in hexadecimal)\n"
          "      IRQ - Interrupt (specified in decimal or hexadecimal)\n"
          "      Dotted IP - (optional) static IP address (No DHCP)\n"
          "    NOTE: The /E and /K options are mutually exclusive.\n"
          "\n"
          "  /L:DXxDYxBPP[:PXxPY] (Use VESA VBE Linear/Flat Framebuffer video mode)\n"
          "      DX, DY  - Display X and Y Sizes\n"
          "      BPP - Bits Per Pixel Color Depth (8, 16, 24, or 32)\n"
          "      PX, PY  - (optional) Physical Screen X and Y Sizes to simulate a\n"
          "                small device display (PX & PY should be greater than DX & DY)."
          "\n"
          " /N:name  Optionally specify a device name to be used when creating\n"
          "          the Ethernet debug name (for host-side connectivity).\n"
          "\n"
          "Press any key for more options...",

          pszProgram );

    __asm
    {
        push    dx
        xor     ax,ax
        int     0x16
        pop     dx
    }

    printf(
          "\n"
          "Usage: %s -s[:]<pattern> -(g|r|u)[[:]<pattern> [<destination>]\n"
          "-s    Show a directory of files matching <pattern>.\n"
          "\n"
          "-g    Get files matching <pattern> and copy them to the\n"
          "      optionally specified <destination>.  Any existing file with\n"
          "      the same name will be overwritten.\n"
          "\n"
          "-r    Refresh files matching <pattern> which already exist in <destination>\n"
          "      and have a timestamp newer than the one in <destination>.\n"
          "\n"
          "-u    Update files matching <pattern> which don't already exist in\n"
          "      <destination> or those that have a timestamp newer than those\n"
          "      in <destination>.\n"
          "\n"
          "<pattern> Windows filename path with optional wildcard characters.\n"
          "\n"
          "<destination> If not specified then the current directory is the default.\n"
          "      If specified and it doesn't exist and the last character is \\,\n"
          "      then the directory is created.  Otherwise <destination> is\n"
          "      treated as the name of the file.\n"
          "      It is an error for <pattern> to match multiple files\n"
          "      when <destination> specifies a file.\n"
		  "For CE Collections - Last built with ver.2.01-Beta1-03",

          pszProgram );
}

//------------------------------------------------------------------------------
//
//  Function Name:  ExitSigHandler( int sig )
//  Description..:  This function handles Ctrl_c/Ctrl+Break while downloading.
//                  XMS must be freed or unlocked or we can't execute loadcepc
//                  again without rebooting.
//  Inputs.......:  int     sig         exit signal number
//  Outputs......:  none
//
//------------------------------------------------------------------------------

static void ExitSigHandler( int sig )
{
    if( usBlockHandle )
    {
        XmsUnlockExtendedMemory(usBlockHandle);
        XmsFreeExtendedMemory(usBlockHandle);
    }
	
    exit (sig);
}

//------------------------------------------------------------------------------
//
//  Function Name:  DownloadRead( ... )
//  Description..:  This function is used to read download data. It 
//                  encapsulates the device we are reading from. 
//  Inputs.......:  int     hImage          handle to image, may be zero.
//                  UCHAR   *ucBuffer       pntr to dst buffer
//                  USHORT  usReadSize      number of bytes to read
//                  BOOL                    parallel port switch
//  Outputs......:  
//
//------------------------------------------------------------------------------

static USHORT DownloadRead( int     hImage, 
                            UCHAR  *ucBuffer, 
                            USHORT  usReadSize, 
                            BOOL    bParallelDownload )
{
    USHORT  loop;

    if( bParallelDownload )
    {
        // Parallel download 

        for( loop = 0; loop < usReadSize; loop++ )
        {
            ucBuffer[loop]= (UCHAR)OEMParallelPortGetByte();

            if( NoPPFS )
            {
                return 0;
            }
        }
        return usReadSize;
    }
    else if( bSerialDownload )
    {
        // Serial download

        OEMReadDebugBytes( ucBuffer,usReadSize );
        return usReadSize;
    }
    else
    {
        // Ethernet?

        return _read( hImage, ucBuffer, usReadSize );
    }
}

//------------------------------------------------------------------------------
//
//  Function Name:  LoadImage
//  Description..:  This function is responsible for getting and loading 
//                  the image into memory.
//  Inputs.......:  BOOL    Verbose                 set verbose tracing flag
//                  BOOL    ParallelDownLoad        use parallel port flag
//                  char *  FileName                image file name
//                  PULONG  EntryPoint              memory entry point
//  Outputs......:  TRUE on success, FALSE on failure.
//
//------------------------------------------------------------------------------

static int LoadImage( BOOL     bVerbose, 
                      BOOL     bParallelDownload, 
                      char    *pszFileName, 
                      PULONG   pulEntryPoint )
{
    int             hImage;
    long            lImageSize;
    int             xmsError;
    USHORT          usTotalFree;
    USHORT          usLargestBlock;
    ULONG           ulLinearAddress;
    ULONG           ulSectionAddress;
    ULONG           ulSectionSize;
    ULONG           ulSectionChecksum;
    ULONG           ulSectionOffset;
    ULONG           ulChecksum;
    USHORT          usReadSize;
    USHORT          usAmountRead;
    ULONG           ulReadTotal=0;
    ULONG           ulReadProgress=0;
    USHORT          usIndex;
    static UCHAR    ucBuffer[ CHUNKSIZE ];
    static UCHAR    ucSignature[] = { 'B', '0', '0', '0', 'F', 'F', '\n'};
    int             i;
    UCHAR           BootPacket[256];
    UCHAR           *pDestByte;
    UCHAR           *pTemp;
    unsigned int    chksum;
    long int        uiTemp;
    long int        bootType;
    unsigned        len;

	char cReturnStri[60]={0};
	strcpy(cReturnStri,"\r                                                                         \r\n        ");

    // Find the largest Extended Memory block and allocate it

    xmsError = XmsQueryFreeExtendedMemory(&usLargestBlock, &usTotalFree);

    if( xmsError != XMS_SUCCESS )
    { 
        printf("Error querying free extended memory - %s\n", XmsErrorString(xmsError));

        if( !bParallelDownload && !bSerialDownload )
        {
            _close(hImage);
        }

        return FALSE;
    }

	printf("               ImageName:......: %s\n",pszFileName );
	printf("               FLATMode........: %d\n",iDesiredMode);
    printf("               VESAMode........: 0x%x\n", BootArgs.vesaMode );
	printf("               FreeEMSMem......: %u\n",usTotalFree);
	printf("               LagestBlock.....: %u\n",usLargestBlock);
	printf("             ==========Booting - Press CTRL+C to cancel=========\n ");
	fprintf(stderr, "                            ");
	fprintf(stderr, "             ");
    if( bVerbose )
    {
		fprintf(stderr,cReturnStri);
        // Print info about memory available
        printf( "Total free extended memory = %u, largest block = %u\n",
                usTotalFree, usLargestBlock);
    }

    // Allocate the extended block

    xmsError = XmsAllocateExtendedMemory(usLargestBlock, &usBlockHandle);

    if( xmsError != XMS_SUCCESS )
    {
		fprintf(stderr,cReturnStri);
        printf( "Error allocating extended memory - %s\n", 
               XmsErrorString(xmsError));

        if( !bParallelDownload && !bSerialDownload )
        {
            _close(hImage);
        }

        return FALSE;
    }

    //printf("XMS usBlockHandle = 0x%x\n", usBlockHandle);

    // Lock the block and obtain its linear address.  
    // Then ensure that the image fits into it.

    xmsError = XmsLockExtendedMemory(usBlockHandle, &ulLinearAddress);

    if( xmsError != XMS_SUCCESS )
    {
        fprintf(stderr,cReturnStri);
        printf("Error locking extended memory - %s\n", XmsErrorString(xmsError));

        XmsFreeExtendedMemory(usBlockHandle);
        usBlockHandle = 0;

        if( !bParallelDownload && !bSerialDownload )
        {
            _close(hImage);
        }

        return FALSE;
    }

    // Print info about block allocated

    if( bVerbose )
    {
        fprintf(stderr,cReturnStri);
        printf("Block allocated at 0x%lX\n", ulLinearAddress);
    }

    // Zero upper memory in CHUNKSIZE chunks

    memset(ucBuffer, 0, sizeof(ucBuffer));
    ulSectionSize = (ULONG)usLargestBlock * 1024;

    for( ulSectionOffset = 0; ulSectionOffset < ulSectionSize; ulSectionOffset += usReadSize )
    {
        if( (ulSectionSize - ulSectionOffset) < CHUNKSIZE )
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

        if( xmsError != XMS_SUCCESS )
        {
            fprintf(stderr,cReturnStri);
            printf("Error zeroing extended memory - %s\n", XmsErrorString(xmsError));
            if( !bParallelDownload && !bSerialDownload )
                _close(hImage);

            return FALSE;
        }
    }

    // Open image file

    // If this is a serial download, initialize the serial port

    if( bSerialDownload )
    {
        OEMInitDebugSerial();
    }

    if( bParallelDownload || bSerialDownload )
    {
        // Prepare boot packet
        
        pDestByte = BootPacket;

        for( i = 0; i < BOOT_HEADER_SIZE; i++ )
        {
            *pDestByte++ = BootHeader[i];
        }

        chksum = 0;
        len = 9;

        if( pszFileName )
        {
			fprintf(stderr,cReturnStri);
            if( bSerialDownload )
                printf("Loading image %s via serial port.\r\n",pszFileName);
            else
                printf("Loading image %s via parallel port.\r\n",pszFileName);

            bootType = 1;              // The NULL byte

            for( pTemp = (UCHAR*)pszFileName; *pTemp; pTemp++ )
            {
                bootType++;
            }

            len += (unsigned)bootType;

        }
        else
        {
			fprintf(stderr,cReturnStri);
            if( bSerialDownload )
                printf("Loading host default image via serial port.\r\n");
            else
                printf("Loading host default image via parallel port.\r\n");

            bootType = BOOT_TYPE;
        }

        uiTemp = len;

        for( i = 0; i < 2; i++ )
        {
            *pDestByte++ = (unsigned char)(uiTemp & 0xFF);
            chksum += (unsigned int)(uiTemp & 0xFF);
            uiTemp >>= 8;
        }

        uiTemp = bootType;

        for( i = 0; i < 4; i++ )
        {
            *pDestByte++ = (unsigned char)(uiTemp & 0xFF);
            chksum += (unsigned int)(uiTemp & 0xFF);
            uiTemp >>= 8;
        }

        if( bootType > 0 )
        {
            for( pTemp = (UCHAR*)pszFileName; *pTemp; pTemp++ )
            {
                *pDestByte++ = *pTemp;
                chksum += *pTemp;
            }
            *pDestByte++ = 0;
        }

        *pDestByte++ = (unsigned char)((~chksum) & 0xFF);

        for( i = 0; i < BOOT_TAIL_SIZE; i++ )
        {
            *pDestByte++ = BootTail[i];
        }


        if( bVerbose ) 
		{
			fprintf(stderr,cReturnStri);
			printf("Sending boot packet: ");
		}

        usIndex=0;

        while( &BootPacket[usIndex] < pDestByte )
        {
            if( NoPPFS )
            {
                return FALSE;
            }

            if( bVerbose ) 
			{
				fprintf(stderr,cReturnStri);
				printf("%x ",BootPacket[usIndex]);
			}

            if( bSerialDownload )
            {
                OEMWriteDebugByte(BootPacket[usIndex]);
            }
            else
            {
                OEMParallelPortSendByte(BootPacket[usIndex]);
            }
            usIndex++;
        }

        if( bVerbose ) printf("Sent.\r\n");
    }
    else
    {
        hImage = _open(pszFileName, _O_BINARY | _O_RDONLY);

        if( hImage == -1 )
        {
			fprintf(stderr,cReturnStri);
            printf("%s: Error opening file - %s\n", pszFileName, _strerror(NULL));
            return FALSE;
        }

        if( (lImageSize = _filelength(hImage)) == -1 )
        {
			fprintf(stderr,cReturnStri);
            printf("%s: Error obtaining file size - %s\n", pszFileName, _strerror(NULL));
            if( !bParallelDownload && !bSerialDownload )
                _close(hImage);

            return FALSE;
        }

        if( bVerbose )
        {
			fprintf(stderr,cReturnStri);
            printf("Loading %s, size = %ld\n", pszFileName, lImageSize);
	        DisplayCEOSVersion( pszFileName );  // Extract Version Number from image if available
	    }
    }

    // Read initial signature and physical start and size

    usReadSize = sizeof(ucSignature) + 2 * sizeof(ULONG);

    if( DownloadRead( hImage, ucBuffer, usReadSize, bParallelDownload) != usReadSize )
    {
		fprintf(stderr,cReturnStri);
        printf("Error reading signature - %s\n", _strerror(NULL));
        if( !bParallelDownload && !bSerialDownload )
            _close(hImage);

        return FALSE;
    }

    if( memcmp(ucBuffer, ucSignature, sizeof(ucSignature)) != 0 )
    {
		fprintf(stderr,cReturnStri);
        printf("Error invalid signature\nData: ");
        for( i = 0; i < (int)usReadSize; i++ )
            printf("%x ",ucBuffer[i]);
        printf("\r\n");
        if( !bParallelDownload && !bSerialDownload )
            _close(hImage);

        return FALSE;
    }

    ulReadTotal=*(PULONG)&ucBuffer[sizeof(ucSignature) + sizeof(ULONG)];

    // Print Physical start and size

    if( bVerbose )
    {
		fprintf(stderr,cReturnStri);
        printf( "Image physical start = 0x%8.8lX, size = %ld\n",
              *(PULONG)&ucBuffer[sizeof(ucSignature)],
              *(PULONG)&ucBuffer[sizeof(ucSignature) + sizeof(ULONG)]);
    }

    // Initialize the percent thingie.

    DrawPercent( (DWORD)-1, (DWORD)"");

    // Copy file to upper memory in CHUNKSIZE chunks

    for(;;)
    {
        usAmountRead = DownloadRead(hImage, ucBuffer, 3 * sizeof(ULONG), bParallelDownload);
        ulReadProgress+=12;

        if( usAmountRead != 3 * sizeof(ULONG) )
        {
            fprintf(stderr,cReturnStri);
            printf("Error reading header - %s\n", XmsErrorString(xmsError));
            if( !bParallelDownload && !bSerialDownload )
                _close(hImage);

            return FALSE;
        }

        ulSectionAddress = *(PULONG)&ucBuffer[0];
        ulSectionSize = *(PULONG)&ucBuffer[4];
        ulSectionChecksum = *(PULONG)&ucBuffer[8];

        if( ulSectionAddress == 0 )
        {
            *pulEntryPoint = ulSectionSize;

            break;
        }

        if( ulSectionAddress < ulLinearAddress ||
            (ulSectionAddress + ulSectionSize) >
            (ulLinearAddress + (ULONG)usLargestBlock * 1024) )
        {
            fprintf(stderr,cReturnStri);
            printf(
                  "Error image section doesn't fit in allocated block\n"
                  "      Block allocated at 0x%lX, size = %ld\n"
                  "      Section physical start = 0x%8.8lX, size = %ld\n"
				  "      Memory too low in your VM might cause this.\n"
				  "\n"
				  "        >> Increase memory may fix this.\n",
                  ulLinearAddress, (ULONG)usLargestBlock * 1024,
                  ulSectionAddress, ulSectionSize);

            if( !bParallelDownload && !bSerialDownload )
                _close(hImage);

            return FALSE;
        }

        if( bVerbose )
        {
            fprintf(stderr,cReturnStri);
            printf(
                  "Section physical start = 0x%8.8lX, size = %ld\n",
                  ulSectionAddress, ulSectionSize);
        }

        ulChecksum = 0;

        for( ulSectionOffset = 0; ulSectionOffset < ulSectionSize; )
        {
            if( (ulSectionSize - ulSectionOffset) < CHUNKSIZE )
            {
                usReadSize = (USHORT)(ulSectionSize - ulSectionOffset);
            }
            else
            {
                usReadSize = CHUNKSIZE;
            }
            usAmountRead = DownloadRead(hImage, ucBuffer, usReadSize,bParallelDownload);

            if( usAmountRead != usReadSize )
            {
                fprintf(stderr,cReturnStri);
                printf("Error reading section - %s\n", XmsErrorString(xmsError));
                if( !bParallelDownload && !bSerialDownload )
                    _close(hImage);

                return FALSE;
            }
            ulReadProgress+= usReadSize;
            DrawPercent(ulReadProgress, ulReadTotal);

#if 0
            for( usIndex = 0; usIndex < usAmountRead; usIndex++ )
            {
                ulChecksum += ucBuffer[usIndex];
            }
#endif

#if 1
            xmsError = XmsMoveExtendedMemory(
                                            0, (ULONG)(UCHAR far *)ucBuffer,
                                            usBlockHandle, ulSectionAddress - ulLinearAddress + ulSectionOffset,
                                            (usAmountRead + 1) & ~1U);

            if( xmsError != XMS_SUCCESS )
            {
                fprintf(stderr,cReturnStri);
                printf("Error moving extended memory - %s\n", XmsErrorString(xmsError));
                if( !bParallelDownload && !bSerialDownload )
                    _close(hImage);

                return FALSE;
            }
#endif
            ulSectionOffset += usAmountRead;
        }
#if 0
        if( ulChecksum != ulSectionChecksum )
        {
            fprintf(stderr,cReturnStri);
            printf(
                  "Bad checksum 0x%8.8lX, expected 0x%8.8lX\n",
                  ulChecksum, ulSectionChecksum);

            if( !bParallelDownload && !bSerialDownload )
                _close(hImage);

            return FALSE;
        }
#endif
    }

    DrawPercent(ulReadTotal, ulReadTotal);

    if( !bParallelDownload && !bSerialDownload )
    {
        _close(hImage);
    }

    // Stop the floppy motor in case it contained the image file

    __asm
    {
        push    dx
        mov     dx, 03F2h   ; Floppy motor and DMA control
        in      al, dx
        and     al, 00Fh    ; Clear motor on bits
        out     dx, al
        pop     dx
    }
    fprintf(stderr,cReturnStri);

    // Indicate success

    return( TRUE );
}

//------------------------------------------------------------------------------
//
//  Function Name:  GetPCIConfigMechanism()
//  Description..:  Execute trap to obtain PCI configuration Mechanism.
//  Inputs.......:  none
//  Outputs......:  UCHAR  0 indicates no PCI bus, else PCI configuration mech.
//
//------------------------------------------------------------------------------

static UCHAR GetPCIConfigMechanism()
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

    // Indicate PCI bus info

    /*printf( "%d PCI bus%s (Version %X.%2.2X) using Configuration Mechanism # %d\n",
        ucPCIBus + 1, ucPCIBus == 0 ? "" : "ses",
        ucPCIVersionMajor, ucPCIVersionMinor, ucPCIConfigMech);*/


    // Indicate Configuration mechanism

    return( UCHAR)((ucPCIBus << 2) | ucPCIConfigMech );

    // Indicate no PCI bus

    noPCI: printf( "PCI bus not detected\n" );
    return( 0 );
}

//------------------------------------------------------------------------------
//
//  Type Name..:    JUMP_PACKET    
//  Description:    Defines the JUMP_PACKET type. Type is byte aligned.
//
//------------------------------------------------------------------------------

#pragma pack(1)

typedef struct JUMP_PACKET 
{
    UCHAR   Header[BOOT_HEADER_SIZE];
    WORD    wLen;
    DWORD   dwTransport;
    UCHAR   ucChkSum;
    UCHAR   Tail[BOOT_TAIL_SIZE];
} 
JUMP_PACKET, *PJUMP_PACKET;

#pragma pack()


//------------------------------------------------------------------------------
//
//  Function Name:  GetJumpFlags(void)
//  Description..:  This function validates the JUMP_PACKET and if valid
//                  indicates the transport parameter.
//  Inputs.......:  none
//  Outputs......:  0 indicates JUMP_PACKET failure, else Transport parameter
//
//------------------------------------------------------------------------------

static WORD GetJumpFlags(void)
{
    JUMP_PACKET Packet;
    USHORT usRead;
    UCHAR *pData;
    UCHAR CheckSum;
    int i;

    usRead = DownloadRead( 0, (UCHAR *)&Packet, sizeof(Packet), TRUE );

    // check read size

    if( usRead != sizeof(Packet) )
    {
        printf( "Read Packet Failed\n" );
        return 0;       // KTS_DEFAULT
    }

    // check header/tail valid

    if( (memcmp(Packet.Header, BootHeader, 4) != 0) ||
        (memcmp(Packet.Tail,   BootTail,   4) != 0) )
    {
        printf( "Read Header/Tail failed\n");
        return 0;
    }

    // check command

    if( Packet.Header[4] != 0x00 || Packet.Header[5] != 0x00 )
    {
        printf("Invalid Command\n");
        return 0;
    }

    // check wlen

    if( Packet.wLen != 9 )
    {
        printf("Invalid Length\n");
        return 0;
    }

    // check checksum

    CheckSum = 0;

    for( i=0, pData = (UCHAR *)&Packet.dwTransport; i < sizeof(Packet.dwTransport); i++, pData++ )
    {
        CheckSum += *pData;
    }

    if( CheckSum != Packet.ucChkSum )
    {
        printf("Invalid checksum\n");
        return 0;
    }

    // Packet is valid - indicate Transport

    return( (WORD)Packet.dwTransport );
}


//------------------------------------------------------------------------------
//
//  Function Name:  DisplayCEOSVersion(void)
//  Description..:  This function searches through the Image binary (e.g. eboot.bin) looking for
//		    a Windows CE Version number, delimited by a unique string--> "$@( VER $@("
//  Inputs.......:  char *pszFileName, denotes the image being loaded by LoadImage
//  Outputs......:  none, simply prints debug info to the user
//
//------------------------------------------------------------------------------

static void DisplayCEOSVersion(char *pszFileName)
{

#define DELIM_LEN 3

    char delim[] = "$@(";		// To change delimiter, modify delim & DELIM_LEN!
    char *delim_ptr = delim;
    char *delim_end = delim + DELIM_LEN;
    int  align32bit = 4 - ( DELIM_LEN % 4 );

    FILE *lImageFile = NULL;	
    int  ver_lo,ver_hi;			
    int  done = 0;			// Loop exit condition, done = 1 denotes version found
    int  iterations = 0;		// Used to limit search length
    int  cnt;					

    lImageFile = fopen( pszFileName, "rb" );

    // Algorithm to search through image for 'delim' (version) 'delim'
    // If successful, sets done = 1.
    if( lImageFile ) {

        while( !feof(lImageFile)  &&  !done  &&  (iterations++ < 512) ) {

            if( fgetc(lImageFile) == *delim_ptr ) {
                delim_ptr++;
            } else {
                delim_ptr = delim;
            }
			
            if( delim_ptr == delim_end ) {

                // Initial delimiter string found, collect data:
                fseek(lImageFile, align32bit, SEEK_CUR);    // next 32-bit boundary	
		ver_lo = fgetc(lImageFile);                 // Get two bytes (in a DWORD)
                ver_hi = fgetc(lImageFile);					
                fseek(lImageFile, 2, SEEK_CUR);             // Skip two bytes to next 32-bit boundary

                delim_ptr = delim;
           
                for( cnt=0; cnt < DELIM_LEN; cnt++) {
                    if( fgetc(lImageFile) == *delim_ptr ) {
                        delim_ptr++;
                    }
                }

                if( delim_ptr == delim_end ) {
                    done = 1;
                }
            }
        }

        fclose( lImageFile );
    }

    if (done) {
        printf("Booting %s compiled with Windows CE Version %d\n", pszFileName, (ver_lo | (ver_hi << 8)) );
    } else { 
        printf("%s: Unknown or old version of bootloader\n", pszFileName);
    }

}