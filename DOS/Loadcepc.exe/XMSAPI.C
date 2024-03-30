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

#include "xmsapi.h"

#pragma warning(disable:4704)

#pragma pack(1)

typedef struct  _EXTENDED_MEMORY_MOVE
{
    unsigned long   ulLength;
    unsigned int    uiSourceHandle;
    unsigned long   ulSourceOffset;
    unsigned int    uiDestinationHandle;
    unsigned long   ulDestinationOffset;

}   EXTENDED_MEMORY_MOVE, *PEXTENDED_MEMORY_MOVE;

#pragma pack()

int (far * XMSControl)() = 0;

struct 
{
    int     iErrorCode;
    char *  pszErrorString;
}
XmsErrorMessages[] = 
{
    { XMS_SUCCESS, "Success" },

    { XMS_E_NOT_IMPLEMENTED, "Not implemented" },
    { XMS_E_VDISK_DETECTED, "VDISK detected" },
    { XMS_E_A20_ERROR, "A20 error" },

    { XMS_W_NO_HMA, "No HMA" },
    { XMS_W_HMA_IN_USE, "HMA in use" },
    { XMS_W_HMA_DENIED, "HMA denied" },
    { XMS_W_HMA_NOT_ALLOCATED, "HMA not allocated" },
    { XMS_W_A20_STILL_ENABLED, "A20 still enabled" },

    { XMS_S_NO_FREE_EXTENDED_MEMORY, "No free extended memory" },
    { XMS_S_NO_FREE_HANDLES, "No free handles" },
    { XMS_S_INVALID_HANDLE, "Invalid handle" },
    { XMS_S_INVALID_SOURCE_HANDLE, "Invalid source handle" },
    { XMS_S_INVALID_SOURCE_OFFSET, "Invalid source offset" },
    { XMS_S_INVALID_DESTINATION_HANDLE, "Invalid destination handle" },
    { XMS_S_INVALID_DESTINATION_OFFSET, "Invalid destination offset" },
    { XMS_S_INVALID_LENGTH, "Invalid length" },
    { XMS_S_INVALID_OVERLAP, "Invalid overlap" },
    { XMS_S_PARITY_ERROR, "Parity error" },
    { XMS_S_BLOCK_LOCKED, "Block locked" },
    { XMS_S_HANDLE_LOCKED, "Handle locked" },
    { XMS_S_LOCK_COUNT_OVERFLOW, "Lock count overflow" },
    { XMS_S_LOCK_FAILED, "Lock failed" },

    { XMS_I_SMALLER_UMB_AVAILABLE, "Smaller UMB available" },
    { XMS_I_NO_UMBS_AVAILABLE, "No UMBs available" },
    { XMS_I_INVALID_UMB_SEGMENT, "Invalid UMB segment" }
};

#define N_XMS_ERROR_MESSAGES    (sizeof(XmsErrorMessages) / sizeof(XmsErrorMessages[0]))

int
XmsIsInstalled()
{
    int bInstalled = 0;

    if (XMSControl != 0)
    {
        return 1;
    }

    _asm
    {
        mov ax,4300h                        ; Check if XMS is present)
        int 2Fh
        xor ah,ah
        cmp al,80h
        jne done                            ; Nope - no point in continuing

        mov ax,4310h                        ; Get XMS entry point
        int 2Fh
        mov WORD PTR [XMSControl], bx       ; Save entry point
        mov WORD PTR [XMSControl + 2], es

        inc bInstalled                      ; Indicate installed
    }

done:
    return bInstalled;
}

int
XmsLocalEnableA20()
{
    int xmsError = XMS_SUCCESS;

    if (!XmsIsInstalled())
    {
        return XMS_E_NOT_IMPLEMENTED;
    }

    _asm
    {
        mov     ah, 05h
        call    [XMSControl]
        clc
        rcr     al, 1
        jc      done

        xor     bh, bh
        mov     xmsError, bx
    }

done:
    return xmsError;
}

int
XmsLocalDisableA20()
{
    int xmsError = XMS_SUCCESS;

    if (!XmsIsInstalled())
    {
        return XMS_E_NOT_IMPLEMENTED;
    }

    _asm
    {
        mov     ah, 06h
        call    [XMSControl]
        clc
        rcr     al, 1
        jc      done

        xor     bh, bh
        mov     xmsError, bx
    }

done:
    return xmsError;
}

int
XmsQueryA20(
    int *pbEnabled)
{
    int xmsError = XMS_SUCCESS;

    if (!XmsIsInstalled())
    {
        return XMS_E_NOT_IMPLEMENTED;
    }

    _asm
    {
        mov     ah, 07h
        call    [XMSControl]

        push    si
        mov     si, pbEnabled
        mov     WORD PTR [si], ax
        pop     si

        xor     bh, bh
        mov     xmsError, bx
    }

    return xmsError;
}

int
XmsQueryFreeExtendedMemory(
    unsigned int *puiLargestFreeBlock,
    unsigned int *puiTotalFree)
{
    int xmsError = XMS_SUCCESS;

    if (!XmsIsInstalled())
    {
        return XMS_E_NOT_IMPLEMENTED;
    }

    _asm
    {
        mov     ah, 08h
        call    [XMSControl]

        push    si
        mov     si, puiLargestFreeBlock
        mov     WORD PTR [si], ax

        mov     si, puiTotalFree
        mov     WORD PTR [si], dx
        pop     si

        xor     bh, bh
        mov     xmsError, bx
    }

    return xmsError;
}

int
XmsAllocateExtendedMemory(
    unsigned int uiBlockSizeK,
    unsigned int * puiBlockHandle)
{
    int xmsError = XMS_SUCCESS;

    if (!XmsIsInstalled())
    {
        return XMS_E_NOT_IMPLEMENTED;
    }

    _asm
    {
        mov     ah, 09h
        mov     dx, uiBlockSizeK
        call    [XMSControl]

        push    si
        mov     si, puiBlockHandle
        mov     WORD PTR [si], dx
        pop     si

        clc
        rcr     al, 1
        jc      done

        xor     bh, bh
        mov     xmsError, bx
    }

done:
    return xmsError;
}

int
XmsFreeExtendedMemory(
    unsigned int uiBlockHandle)
{
    int xmsError = XMS_SUCCESS;

    if (!XmsIsInstalled())
    {
        return XMS_E_NOT_IMPLEMENTED;
    }

    _asm
    {
        mov     ah, 0Ah
        mov     dx, uiBlockHandle
        call    [XMSControl]

        clc
        rcr     al, 1
        jc      done

        xor     bh, bh
        mov     xmsError, bx
    }

done:
    return xmsError;
}

int
XmsMoveExtendedMemory(
    unsigned int    uiSourceHandle,
    unsigned long   ulSourceOffset,
    unsigned int    uiDestinationHandle,
    unsigned long   ulDestinationOffset,
    unsigned long   ulLength)
{
    EXTENDED_MEMORY_MOVE    moveInfo;

    int                     xmsError = XMS_SUCCESS;

    if (!XmsIsInstalled())
    {
        return XMS_E_NOT_IMPLEMENTED;
    }

    moveInfo.uiSourceHandle = uiSourceHandle;
    moveInfo.ulSourceOffset = ulSourceOffset;
    moveInfo.uiDestinationHandle = uiDestinationHandle;
    moveInfo.ulDestinationOffset = ulDestinationOffset;
    moveInfo.ulLength = ulLength;

    _asm
    {
        mov     ah, 0Bh

        push    si
        lea     si, moveInfo
        call    [XMSControl]
        pop     si

        clc
        rcr     al, 1
        jc      done

        xor     bh, bh
        mov     xmsError, bx
    }

done:
    return xmsError;
}

int
XmsLockExtendedMemory(
    unsigned int uiBlockHandle,
    unsigned long * pulLinearAddress)
{
    int xmsError = XMS_SUCCESS;

    if (!XmsIsInstalled())
    {
        return XMS_E_NOT_IMPLEMENTED;
    }

    _asm
    {
        mov     ah, 0Ch
        mov     dx, uiBlockHandle
        call    [XMSControl]

        push    si
        mov     si, pulLinearAddress
        mov     WORD PTR [si], bx
        mov     WORD PTR [si + 2], dx
        pop     si

        clc
        rcr     al, 1
        jc      done

        xor     bh, bh
        mov     xmsError, bx
    }

done:
    return xmsError;
}

int
XmsUnlockExtendedMemory(
    unsigned int uiBlockHandle)
{
    int xmsError = XMS_SUCCESS;

    if (!XmsIsInstalled())
    {
        return XMS_E_NOT_IMPLEMENTED;
    }

    _asm
    {
        mov     ah, 0Dh
        mov     dx, uiBlockHandle
        call    [XMSControl]

        clc
        rcr     al, 1
        jc      done

        xor     bh, bh
        mov     xmsError, bx
    }

done:
    return xmsError;
}

char *XmsErrorString(int xmsError)
{
    int     i;

    for (i = 0; i < N_XMS_ERROR_MESSAGES; i++)
    {
        if (XmsErrorMessages[i].iErrorCode == xmsError)
        {
            return XmsErrorMessages[i].pszErrorString;
        }
    }

    return "Unknown error";
}
