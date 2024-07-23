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
#define XMS_SUCCESS                         0x00

#define XMS_E_NOT_IMPLEMENTED               0x80
#define XMS_E_VDISK_DETECTED                0x81
#define XMS_E_A20_ERROR                     0x82

#define XMS_W_NO_HMA                        0x90
#define XMS_W_HMA_IN_USE                    0x91
#define XMS_W_HMA_DENIED                    0x92
#define XMS_W_HMA_NOT_ALLOCATED             0x93
#define XMS_W_A20_STILL_ENABLED             0x94

#define XMS_S_NO_FREE_EXTENDED_MEMORY       0xA0
#define XMS_S_NO_FREE_HANDLES               0xA1
#define XMS_S_INVALID_HANDLE                0xA2
#define XMS_S_INVALID_SOURCE_HANDLE         0xA3
#define XMS_S_INVALID_SOURCE_OFFSET         0xA4
#define XMS_S_INVALID_DESTINATION_HANDLE    0xA5
#define XMS_S_INVALID_DESTINATION_OFFSET    0xA6
#define XMS_S_INVALID_LENGTH                0xA7
#define XMS_S_INVALID_OVERLAP               0xA8
#define XMS_S_PARITY_ERROR                  0xA9
#define XMS_S_BLOCK_LOCKED                  0xAA
#define XMS_S_HANDLE_LOCKED                 0xAB
#define XMS_S_LOCK_COUNT_OVERFLOW           0xAC
#define XMS_S_LOCK_FAILED                   0xAD

#define XMS_I_SMALLER_UMB_AVAILABLE         0xB0
#define XMS_I_NO_UMBS_AVAILABLE             0xB1
#define XMS_I_INVALID_UMB_SEGMENT           0xB2

int XmsIsInstalled();

int XmsLocalEnableA20();

int XmsLocalDisableA20();

int XmsQueryA20(
        int *pbEnabled);

int XmsQueryFreeExtendedMemory(
        unsigned int *puiLargestFreeBlock, unsigned int *puiTotalFree);

int XmsAllocateExtendedMemory(
        unsigned int uiBlockSizeK, unsigned int * puiBlockHandle);

int XmsFreeExtendedMemory(
        unsigned int uiBlockHandle);

int XmsMoveExtendedMemory(
        unsigned int uiSourceHandle, unsigned long ulSourceOffset,
        unsigned int uiDestinationHandle, unsigned long ulDestinationOffset,
        unsigned long ulLength);

int XmsLockExtendedMemory(
        unsigned int uiBlockHandle,
        unsigned long * pulLinearAddress);

int XmsUnlockExtendedMemory(
        unsigned int uiBlockHandle);

char *XmsErrorString();
