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

#include <string.h>
#include "loadcepc.h"
#include "video.h"
  
#define VESAMODE480x200x256		0x101	/* VESA Mode 101 - 480x200x256 emulated in 640x480 */
#define VESAMODE640x480x256		0x101	/* VESA Mode 101 - 640x480x256 */
#define VESAMODE800x600x256		0x103	/* VESA Mode 103 - 800x600x256 */
#define VESAMODE1024x768x256	0x105	/* VESA Mode 105 - 1024x768x256 */

#define VESASCANLENGTH1024		1024	/* These VESA modes have the same scan length */

void mode320x200x256(void)
{
	/*
	** Put the display into 320x200x256 colour mode and clear it
	*/
	__asm
	{
		mov     ah, 0       ; Set video mode
		mov     al, 013h    ; 320x200x256 and clear screen
		int     10h         ; set mode 13
	} 
}


int isVesaSupported(void)
{
    VESA_GENERAL_INFO   vesaInfo, far *pVesaInfo = &vesaInfo;

	__asm
    {
        mov     ax, 04F00h      ; Get VESA info
        les     di, pVesaInfo   ; Pointer to info buffer
        int     10h             ; check for VESA

        cmp     ax, 0004Fh      ; Was it successful?
        jne     noVesa
    }
	return TRUE;

noVesa:
	return FALSE;
}

int setVesaMode(int vesaMode, int scanLength)
{
    __asm
    {
        mov     ax, 04F02h      ; VESA Set mode
        mov     bx, vesaMode    ; Set given VESA Mode
        int     10h             ; 

        cmp     ax, 0004Fh      ; Was it successful?
        jne     failed

        mov     ax, 04F06h      ; VESA Set logical scan line length
        mov     bl, 0
        mov     cx, scanLength  ; Set so banks are an integral number lines
        int     10h             ; set VESA scan length

        cmp     ax, 0004Fh      ; Was it successful?
        jne     failed
    }

    return TRUE;
failed:
	return FALSE;
}

/* SetVideoMode: maps from the user input 0-X to a VESA mode
** User  VESA MODE
** Input  
** 0	 320x200x256   default mode, should work with any video card with 64K memory
** 1     480x240x256   non-standard VGA resolution, emulated in a 640x480 window.
** 2	 640x480x256
** 3	 800x600x256
** 4	 1024x768x256
** 5	 320x240x256   non-standard VGA resolution, emulated in a 640x480 window.
*/
                         
UCHAR SetVideoMode(UCHAR desiredMode)
{
    int		vesaMode = 0;
	int		scanLength = VESASCANLENGTH1024;

    if (desiredMode == 0)
    {
		/* In the default mode, there is no need to bank switch the card since only 64K required */
		/* VESA is NOT required for this mode. */
		goto defaultMode;
	}

	if (isVesaSupported())
	{
		/* Use VESA to put the video card into the appropriate mode */
		switch (desiredMode)
		{
		case 0x01:								/* 480x240x256 is emulated in a partial 640x480x256 screen	*/
		case 0x02:
		case 0x05:								/* 240x320x256 is emulated in a partial 640x480x256 screen	*/
			scanLength = VESASCANLENGTH1024;	/* scan length for this mode	*/
			vesaMode = VESAMODE640x480x256;		/* VESA Mode 101 - 640x480x256	*/
			break;
		case 0x03:
			scanLength = VESASCANLENGTH1024;	/* scan length for this mode	*/
			vesaMode = VESAMODE800x600x256;		/* VESA Mode 103 - 800x600x256	*/
			break;
		case 0x04:
			scanLength = VESASCANLENGTH1024;	/* scan length for this mode	*/
			vesaMode = VESAMODE1024x768x256;	/* VESA Mode 105 - 1024x768x256 */
			break;
		default:
			goto defaultMode;
			break;
		}

		/* OK, setup the video card */
		if (setVesaMode(vesaMode, scanLength))
		{
			return desiredMode;					/* Successful setting of VESA mode */
		}
	}

defaultMode:
	mode320x200x256();
	return 0;
}

