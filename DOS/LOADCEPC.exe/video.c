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

/*++

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

Module Name:    video.c

Abstract:

Functions:


Notes:

--*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "loadcepc.h"
#include "video.h"
#include "bootarg.h"

extern  BOOT_ARGS BootArgs;

static  BOOL    bNoModeSet = FALSE;


void mode320x200x256( void )
{
    // Put the display into 320x200x256 colour mode and clear it

    __asm
    {
        mov     ah, 0       ; Set video mode
        mov     al, 013h    ; 320x200x256 and clear screen
        int     10h         ; set mode 13
    }
}


int isVesaSupported( void )
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


int setVesaMode( int vesaMode, int scanLength )
{
	//printf( "setVesaMode( %d, %d\n", vesaMode, scanLength );

    __asm
    {
        mov     ax, 04F02h      ; VESA Set mode
        mov     bx, vesaMode    ; Set given VESA Mode
        int     10h             ;

        cmp     ax, 0004Fh      ; Was it successful?
        jne     failed          ;

        mov     cx, scanLength  ; Set so banks are an integral number lines
        jcxz    finished        ; if no scanLength requested, just exit
        mov     ax, 04F06h      ; VESA Set logical scan line length
        mov     bl, 0           ;
        int     10h             ; set VESA scan length

        cmp     ax, 0004Fh      ; Was it successful?
        jne     failed          ;
    }

finished:
    return TRUE;

failed:
    return FALSE;
}

/* 
** SetVideoMode: maps from the user input 0-X to a VESA mode
**
** User  VESA MODE
** Input
** 0     320x200x256    default mode, should work on any video card with 64K 
**                      memory.
** 1     480x240x256    non-standard VGA resolution, emulated in a 640x480 
**                      window.
** 2     640x480x256
** 3     800x600x256
** 4     1024x768x256
** 5     320x240x256    non-standard VGA resolution, emulated in a 640x480 
**                      window.
** 6     208x240x256
*/

UCHAR SetVideoMode( int desiredMode )
{
    int     vesaMode = 0;
    int     scanLength;

    // if user requested to skip the mode set, just pass bootargs

    if( bNoModeSet )                                
    {
        return( 0xFF );     // force caller to leave bootargs vars alone
    }

    // In the default mode, there is no need to bank switch the card 
    // since only 64K required VESA is NOT required for this mode.

    if( desiredMode == 0 )
    {
        mode320x200x256();
        return( 0 );
    }

    // If VESA is supported use VESA to put the 
    // video card into the appropriate mode.

    if( isVesaSupported() )
    {
        switch( desiredMode )
        {
        case 0x01:   // 480x240x256 is emulated in a partial 640x480x256 screen 
        case 0x02:
        case 0x05:   // 240x320x256 is emulated in a partial 640x480x256 screen 
        case 0x06:   // 208x240x256 is emulated in a partial 640x480x256 screen 

            scanLength = VESASCANLENGTH1024;    // scan length for this mode 
            vesaMode = VESAMODE640x480x256;     // VESA Mode 101 640x480x256 
            break;

        case 0x03:

            scanLength = VESASCANLENGTH1024;    // scan length for this mode 
            vesaMode = VESAMODE800x600x256;     // VESA Mode 103 800x600x256
            break;

        case 0x04:

            scanLength = VESASCANLENGTH1024;    // scan length for this mode 
            vesaMode = VESAMODE1024x768x256;    // VESA Mode 105 1024x768x256 
            break;

        default:    
        
            // VESA mode value was passed in - use directly 

            vesaMode = desiredMode;   /* use passed in mode value */
            vesaMode |= 0x4000;       /* ask for linear/flat framebuffer mode */
            scanLength = 0;           /* use mode default scan length */
            break;
        }

        // Setup the video card

        if( setVesaMode( vesaMode, scanLength ) )
        {
            // Indicate status

            return( (UCHAR)(desiredMode < 0x100 ? desiredMode : 0xFF) );   
        }
    }

    // Set default mode if we get here

    mode320x200x256();
    return( 0 );
}

//
// FindClosestVESAMode - Finds VESA mode with X and Y size closest to 
//  requested screen size.
//
// Returns flat framebuffer pointer, flat framebuffer stride, and physical X 
//  and Y size
//

void FindClosestVESAMode( void )
{
    VESA_GENERAL_INFO   vesaInfo, far *pVesaInfo = &vesaInfo;
    VESA_MODE_INFO      modeInfo, far *pModeInfo = &modeInfo;
    DWORD               deltaX, deltaY;
    DWORD               currError, error;
    WORD                vesaMode;
    int                 index;

    //printf( "FindClosestVESAMode\n\r" );

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

        cmp     ax, 0004Fh      ; Was it successful?
        je      itsok
        jmp     noVesa
    }

itsok:

    // check for minimum compat version of VBE

    if (vesaInfo.wVersion < 0x0200)    
    {
        goto noVesa;
    }

    // Clear mode, set error to large value

    BootArgs.vesaMode = 0;
    currError = 0xFFFFFFFF;

    // Process the VESA mode list

    for( index=0; vesaInfo.pModeList[index] != -1; index++ )
    {
        // Set mode we are checking

        vesaMode = vesaInfo.pModeList[index];

        // Fetch the Mode Info

        __asm
        {
            push    es
            mov     cx, vesaMode
            les     di, pModeInfo   ; Pointer to info buffer
            mov     ax, 04F01h      ; Return VBE Mode Information
            int     10h
            pop     es
        }

        // Check Mode Info - the following are required:
        //
        // - planer mode
        // - flat frame buffer compat mode
        // - matching color depth
        // - same size or bigger than requested

        if( (modeInfo.ucNumberOfPlanes == 1)                        &&
            (modeInfo.dwPhysBasePtr    != 0)                        &&  
            (modeInfo.ucBitsPerPixel   == BootArgs.bppScreen)       && 
            (modeInfo.wXResolution     >= BootArgs.cxDisplayScreen) &&
            (modeInfo.wYResolution     >= BootArgs.cyDisplayScreen)  )
        {
            // only allow 565 16bpp modes through

            if( (modeInfo.ucBitsPerPixel == 16) &&              
                ((modeInfo.ucRedMaskSize   != 5) || 
                 (modeInfo.ucGreenMaskSize != 6) ||
                 (modeInfo.ucBlueMaskSize  != 5)) )
            {
                continue;
            }

            // Compute screen resolution error

            deltaX = (DWORD)modeInfo.wXResolution - 
                     (DWORD)BootArgs.cxDisplayScreen;
            deltaY = (DWORD)modeInfo.wYResolution - 
                     (DWORD)BootArgs.cyDisplayScreen;

            error = ((deltaX * deltaX) + (deltaY * deltaY));

            // If the error is diminishing save the mode data

            if( error < currError )
            {
                // Save the current error for next comparison

                currError = error;

                // Update the boot args with current data

                BootArgs.vesaMode          = vesaMode;
                BootArgs.pvFlatFrameBuffer = modeInfo.dwPhysBasePtr;
                BootArgs.cxPhysicalScreen  = modeInfo.wXResolution;
                BootArgs.cyPhysicalScreen  = modeInfo.wYResolution;
                BootArgs.cbScanLineLength  = modeInfo.wBytesPerScanLine;
                BootArgs.RedMaskSize       = modeInfo.ucRedMaskSize;
                BootArgs.RedMaskPosition   = modeInfo.ucRedFieldPosition;
                BootArgs.GreenMaskSize     = modeInfo.ucGreenMaskSize;
                BootArgs.GreenMaskPosition = modeInfo.ucGreenFieldPosition;
                BootArgs.BlueMaskSize      = modeInfo.ucBlueMaskSize;
                BootArgs.BlueMaskPosition  = modeInfo.ucBlueFieldPosition;
            }
        }
    }


    if( BootArgs.vesaMode == 0)
    {
        goto noVesa;
    }

    return;

noVesa: 

    // no matching VESA mode found, use 320x200x8 video mode as fallback

    BootArgs.vesaMode = 0;
}

//
// FindVESAMode - Finds VESA mode which exactly matches requested X and Y size
// Returns flat framebuffer pointer and flat framebuffer stride
//

void FindVESAMode( void )
{
    VESA_GENERAL_INFO   vesaInfo, far *pVesaInfo = &vesaInfo;
    VESA_MODE_INFO      modeInfo, far *pModeInfo = &modeInfo;
    WORD                vesaMode;
    int                 index;

	//printf( "FindVesaMode( void )\n" );

    vesaInfo.szSignature[0] = 'V';
    vesaInfo.szSignature[1] = 'B';
    vesaInfo.szSignature[2] = 'E';
    vesaInfo.szSignature[3] = '2';

    // Fetch the VESA info data

    __asm
    {
        push    es
        mov     ax, 04F00h      ; Get VESA info
        les     di, pVesaInfo   ; Pointer to info buffer
        int     10h             ; check for VESA
        pop     es

        cmp     ax, 0004Fh      ; Was it successful?
        je      itsok
        jmp     noVesa
    }

itsok:

    // Validate minimum compatibility version of VBE

    if( vesaInfo.wVersion < 0x0200 )     
    {
        goto noVesa;
    }

    // Clear mode

    BootArgs.vesaMode = 0;

    // Process the VESA ModeList

    for( index=0; vesaInfo.pModeList[index] != -1; index++ )
    {
        // Set mode we are checking

        vesaMode = vesaInfo.pModeList[index];

        // Fetch the Mode Info

        __asm
        {
            push    es
            mov     cx, vesaMode
            les     di, pModeInfo   ; Pointer to info buffer
            mov     ax, 04F01h      ; Return VBE Mode Information
            int     10h
            pop     es
        }

        // Check Mode Info - the following are required:
        //
        // - planer mode
        // - flat frame buffer compat mode
        // - matching color depth
        // - same size or bigger than requested

        if( (modeInfo.ucNumberOfPlanes == 1)                         &&     
            (modeInfo.dwPhysBasePtr    != 0)                         &&     
            (modeInfo.ucBitsPerPixel   == BootArgs.bppScreen)        &&    
            (modeInfo.wXResolution     == BootArgs.cxPhysicalScreen) &&  
            (modeInfo.wYResolution     == BootArgs.cyPhysicalScreen)  )
        {
            // Conditions are met. Set BootArgs with Mode data and return.

            BootArgs.vesaMode          = vesaMode;
            BootArgs.pvFlatFrameBuffer = modeInfo.dwPhysBasePtr;
            BootArgs.cbScanLineLength  = modeInfo.wBytesPerScanLine;
            BootArgs.RedMaskSize       = modeInfo.ucRedMaskSize;
            BootArgs.RedMaskPosition   = modeInfo.ucRedFieldPosition;
            BootArgs.GreenMaskSize     = modeInfo.ucGreenMaskSize;
            BootArgs.GreenMaskPosition = modeInfo.ucGreenFieldPosition;
            BootArgs.BlueMaskSize      = modeInfo.ucBlueMaskSize;
            BootArgs.BlueMaskPosition  = modeInfo.ucBlueFieldPosition;
            return;
        }
    }

noVesa: 

    // No matching VESA mode found, use 320x200x8 as fallback

    BootArgs.vesaMode = 0;
}

//
//  ParseVESAOptions
//

BOOL ParseVESAOptions( char *params )
{
    int cArgs;

	//printf( "ParseVESAOptions( 0x%x )\n", params );

    // Clear mode   

    BootArgs.vesaMode = 0;

    // Verify option format is /L:

    if (params[2] != ':')
    {
        return( FALSE );            // colon after /L missing, exit
    }

    // Check for the N format of the option, /L:N:xxx. This 
    // mode skips all the VESA mode processing and the mode set.

    if( (memcmp( &params[3],"N:",2) == 0) || (memcmp( &params[3],"n:",2) == 0) )
    {
        bNoModeSet = TRUE;

        cArgs = sscanf( &params[5], "%dx%dx%d:%dx%d",
                        &BootArgs.cxDisplayScreen,
                        &BootArgs.cyDisplayScreen,
                        &BootArgs.bppScreen,
                        &BootArgs.cxPhysicalScreen,
                        &BootArgs.cyPhysicalScreen );
        if(cArgs == 3)
        {
            BootArgs.cxPhysicalScreen = BootArgs.cxDisplayScreen;
            BootArgs.cyPhysicalScreen = BootArgs.cyDisplayScreen;
        }
        else if(cArgs == 5)
        {
            if (BootArgs.cxPhysicalScreen < BootArgs.cxDisplayScreen ||
                BootArgs.cyPhysicalScreen < BootArgs.cyDisplayScreen)
            {
                printf
                ( 
"ERROR: The physical screen dimensions (PX & PY) should be larger than the\n"
"       display resolution chosen. This option is used to simulate a \n"
"       smaller drawing area on a larger display.\n"
                );

                // The physical screen size must be >= display screen size

                return( FALSE );      
            }
        }
        BootArgs.vesaMode = 0xffff;     // just some really large value
    }
    else    
    {
        // No 'N' present in option. Process and set the VESA mode

        bNoModeSet = FALSE;

        cArgs = sscanf( &params[3], "%dx%dx%d:%dx%d",
                        &BootArgs.cxDisplayScreen,
                        &BootArgs.cyDisplayScreen,
                        &BootArgs.bppScreen,
                        &BootArgs.cxPhysicalScreen,
                        &BootArgs.cyPhysicalScreen );

        // Process according to # of args specified

        switch( cArgs )
        {
        case 3:

            // Explicit physical size not specified - find closest one

            FindClosestVESAMode();      // uses and fills in BootArgs values
            break;

        case 5:

            // Validate specified physical size

            if( BootArgs.cxPhysicalScreen < BootArgs.cxDisplayScreen ||
                BootArgs.cyPhysicalScreen < BootArgs.cyDisplayScreen )
            {
                printf
                (
"ERROR: The physical screen dimensions (PX & PY) should be larger than the\n"
"       display resolution chosen.  This option is used to simulate a \n"
"       smaller drawing area on a larger display.\n"
                );
                
                // The physical screen size must be >= display screen size

                return( FALSE );      
            }

            // Find the actual VESA mode

            FindVESAMode();             // uses and fills in BootArgs values
            break;

        default:

            // Invalid number of parameters

            return( FALSE );
            break;
        }   
    }

    // Indicate success

    return( TRUE );
}
