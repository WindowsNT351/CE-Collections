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

#include <windows.h>

unsigned char __inline READ_PORT_UCHAR(unsigned char *port)
{
    return _inp((unsigned short)port);
}

void __inline WRITE_PORT_UCHAR(unsigned char * port, unsigned char value)
{
    _outp((unsigned short)port, (value));
}

#define LS_TSR_EMPTY        0x40
#define LS_THR_EMPTY        0x20
#define LS_RX_BREAK         0x10
#define LS_RX_FRAMING_ERR   0x08
#define LS_RX_PARITY_ERR    0x04
#define LS_RX_OVERRUN       0x02
#define LS_RX_DATA_READY    0x01

#define LS_RX_ERRORS        ( LS_RX_FRAMING_ERR | LS_RX_PARITY_ERR | LS_RX_OVERRUN )

#define COM1_BASE           0x03F8
#define COM2_BASE           0x02F8

#define comTxBuffer         0x00
#define comRxBuffer         0x00
#define comDivisorLow       0x00
#define comDivisorHigh      0x01
#define comIntEnable        0x01
#define comIntId            0x02
#define comFIFOControl      0x02
#define comLineControl      0x03
#define comModemControl     0x04
#define comLineStatus       0x05
#define comModemStatus      0x06

#define IoPortBase          ( (unsigned char *) COM1_BASE )

extern struct  _ARGUMENTS 
{
   unsigned char   ucVideoMode;
   unsigned char   ucComPort;
   unsigned char   ucBaudDivisor;
   unsigned char   ucPCIConfigType;
}   BootArgs;

//   14400 = 8
//   16457 = 7 +/-
//   19200 = 6
//   23040 = 5
//   28800 = 4
//   38400 = 3
//   57600 = 2
//  115200 = 1


void OEMInitDebugSerial(void)
{
    unsigned char   ucArgs[3];
    unsigned int   dwSize;

    dwSize = sizeof(ucArgs);

    WRITE_PORT_UCHAR(IoPortBase+comLineControl, 0x80);   // Access Baud Divisor
    WRITE_PORT_UCHAR(IoPortBase+comDivisorLow, BootArgs.ucBaudDivisor&0x7f);    // 19200
    WRITE_PORT_UCHAR(IoPortBase+comDivisorHigh, 0x00);
    WRITE_PORT_UCHAR(IoPortBase+comFIFOControl, 0x01);   // Enable FIFO if present
    WRITE_PORT_UCHAR(IoPortBase+comLineControl, 0x03);   // 8 bit, no parity

    WRITE_PORT_UCHAR(IoPortBase+comIntEnable, 0x00);     // No interrupts, polled

    WRITE_PORT_UCHAR(IoPortBase+comModemControl, 0x03);  // Assert DTR, RTS
}

void OEMWriteDebugString(unsigned short *str)
{
    while (*str)
    {
        while (!(READ_PORT_UCHAR(IoPortBase+comLineStatus) & LS_THR_EMPTY))
        {
            ;
        }

        WRITE_PORT_UCHAR(IoPortBase+comTxBuffer, (unsigned char)*str++);
    }
}

void OEMWriteDebugByte(BYTE ucChar)
{
    while (!(READ_PORT_UCHAR(IoPortBase+comLineStatus) & LS_THR_EMPTY))
    {
        ;
    }

    WRITE_PORT_UCHAR(IoPortBase+comTxBuffer, ucChar);
}

int iComTotal=0;
void OEMReadDebugBytes(unsigned char * ucBuffer, int usReadSize) {
	int i=0;
	unsigned char uStat;

	while (usReadSize--) {
		do {
			uStat=READ_PORT_UCHAR(IoPortBase+comLineStatus);
			if (uStat & LS_RX_ERRORS) {
				while (1)
					printf("port status error=%xh i=%u\r",uStat,iComTotal);
			}
		} while (!(uStat&LS_RX_DATA_READY));
   		ucBuffer[i++]=READ_PORT_UCHAR(IoPortBase+comRxBuffer);
		iComTotal++;
	}
}

int OEMReadDebugByte(void)
{
	int i;
	OEMReadDebugBytes((unsigned char *)&i,1);
    return i;
}


/*****************************************************************************
*
*
*   @func   void    |   OEMClearDebugComError | Clear a debug communications er
or
*
*/
void
OEMClearDebugCommError(
    void
    )
{
}
