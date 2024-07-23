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
#define VESAMODE480x200x256     0x101   /* VESA Mode 101 - 480x200x256 emulated in 640x480 */
#define VESAMODE640x480x256     0x101   /* VESA Mode 101 - 640x480x256 */
#define VESAMODE800x600x256     0x103   /* VESA Mode 103 - 800x600x256 */
#define VESAMODE1024x768x256    0x105   /* VESA Mode 105 - 1024x768x256 */

#define VESASCANLENGTH1024      1024    /* These VESA modes have the same scan length */

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
