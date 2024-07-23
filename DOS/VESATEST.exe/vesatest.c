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
#include    <stdio.h>
#include    <stdlib.h>
#include    <conio.h>

#define TRUE        1
#define FALSE       0

typedef int             BOOL;

typedef unsigned char   BYTE, *LPBYTE, UCHAR, *PUCHAR;
typedef unsigned int    USHORT, *PUSHORT;
typedef unsigned long   ULONG, *PULONG;

typedef unsigned short  WORD, *PWORD;
typedef unsigned long   DWORD, *PDWORD;

typedef signed      char        INT8;
typedef unsigned    char        UINT8;
typedef signed      short       INT16;
typedef unsigned    short       UINT16;
typedef signed      int         INT32;
typedef unsigned    int         UINT32;

#pragma pack(1)

typedef struct _VESA_GENERAL_INFO
{
    UCHAR       szSignature[4];
    WORD        wVersion;
    UCHAR far * pszVendor;
    DWORD       dwCapabilities;
    WORD far *  pModeList;
    WORD        wTotalMemory;
    WORD        wOemSoftwareRev;
    UCHAR far*  pszVenderName;
    UCHAR far*  pszProductName;
    UCHAR far*  pszProductRev;
    UCHAR       ucReserved[222];
    UCHAR       ucOemData[256];
}   VESA_GENERAL_INFO;

typedef struct _VESA_MODE_INFO
{
    WORD    wModeAttributes;        /* Mode attributes                  */
    UCHAR   ucWindowAAttributes;    /* Window A attributes              */
    UCHAR   ucWindowBAttributes;    /* Window B attributes              */
    WORD    wWindowGranularity;     /* Window granularity in k          */
    WORD    wWindowSize;            /* Window size in k                 */
    WORD    wWindowASegment;        /* Window A segment                 */
    WORD    wWindowBSegment;        /* Window B segment                 */
    DWORD   pWindowSchemeFunction;  /* Pointer to window function       */
    WORD    wBytesPerScanLine;      /* Bytes per scanline               */
    WORD    wXResolution;           /* Horizontal resolution            */
    WORD    wYResolution;           /* Vertical resolution              */
    UCHAR   ucXCharSize;            /* Character cell width             */
    UCHAR   ucYCharSize;            /* Character cell height            */
    UCHAR   ucNumberOfPlanes;       /* Number of memory planes          */
    UCHAR   ucBitsPerPixel;         /* Bits per pixel                   */
    UCHAR   ucNumberOfBanks;        /* Number of CGA style banks        */
    UCHAR   ucMemoryModel;          /* Memory model type                */
    UCHAR   ucBankSize;             /* Size of CGA style banks          */
    UCHAR   ucNumberOfImagePages;   /* Number of images pages           */
    UCHAR   ucReserved1;            /* Reserved                         */
    UCHAR   ucRedMaskSize;          /* Size of direct color red mask    */
    UCHAR   ucRedFieldPosition;     /* Bit posn of lsb of red mask      */
    UCHAR   ucGreenMaskSize;        /* Size of direct color green mask  */
    UCHAR   ucGreenFieldPosition;   /* Bit posn of lsb of green mask    */
    UCHAR   ucBlueMaskSize;         /* Size of direct color blue mask   */
    UCHAR   ucBlueFieldPosition;    /* Bit posn of lsb of blue mask     */
    UCHAR   ucRsvdMaskSize;         /* Size of direct color res mask    */
    UCHAR   ucRsvdFieldPosition;    /* Bit posn of lsb of res mask      */
    UCHAR   ucDirectColorModeInfo;  /* Direct color mode attributes     */
    /* VESA 2.0 variables */
    DWORD   dwPhysBasePtr;          /* physical address for flat frame buffer */
    UCHAR   ucReserved2[212];       /* Pad to 256 byte block size       */
}   VESA_MODE_INFO;

#pragma pack()

int listedModes=0;
void    ListVideoModes(void)
{
    VESA_GENERAL_INFO   vesaInfo, far *pVesaInfo = &vesaInfo;
    WORD far *          ModePtr;
    int                 ValidModeCount = 0;

    vesaInfo.szSignature[0] = 'V';
    vesaInfo.szSignature[1] = 'B';
    vesaInfo.szSignature[2] = 'E';
    vesaInfo.szSignature[3] = '2';

    __asm
    {
        push    es
        mov     ax, 04F00h      ; Get VESA info
        les     di, pVesaInfo   ; Pointer to info buffer
        int     10h             ; check for VESA
        pop     es
    }

    //printf("Vesa BIOS Version: %d.%d\n", vesaInfo.wVersion >> 8, vesaInfo.wVersion & 0xff);

    ModePtr = vesaInfo.pModeList;

    while (*ModePtr != 0xffff)
    {
        VESA_MODE_INFO  modeInfo, far *pModeInfo = &modeInfo;
        WORD            modeNumber = *ModePtr++;

        __asm
        {
            push    es
            mov     cx, modeNumber  ; mode to request info on
            les     di, pModeInfo   ; pointer to info buffer
            mov     ax, 04f01h      ; get MODE info
            int     10h
            pop     es
        }

        if (modeInfo.ucNumberOfPlanes == 1 && 
			modeInfo.dwPhysBasePtr != 0 && 
			modeInfo.ucBitsPerPixel != 15)
        {
			if(!(modeInfo.ucBitsPerPixel == 16 &&
			   (modeInfo.ucRedMaskSize != 5 ||
				modeInfo.ucGreenMaskSize != 6 ||
				modeInfo.ucBlueMaskSize != 5)))
			{
				if ((ValidModeCount % 5) == 0)
				{
					//printf("\n");
				}
				/*printf("   %4dx%4dx%2d",
					   modeInfo.wXResolution,
					   modeInfo.wYResolution,
					   modeInfo.ucBitsPerPixel);*/
				ValidModeCount++;
			}
        }
    }

    //printf("\n\nNumber of valid FLAT driver modes: %d\n", ValidModeCount);
	listedModes=ValidModeCount;
}

int     main(int argc, char **argv)
{
    ListVideoModes();
	if(listedModes==0)
	{
		system("cls");
		printf("Your video card doesn't support VESA,please choose another one!\n");
		getch();
		while(1);
	}
	printf("VESATEST_201B103 - VESA check succesfuly\n");
}

