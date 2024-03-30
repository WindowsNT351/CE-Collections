/*++
THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.
Copyright (c) 1995-1998  Microsoft Corporation

Module Name:  
    smchw.h
Abstract:  
    Definitions for the SMC9000 registers

Notes: 

--*/
#ifndef _SMCHW_H
#define _SMCHW_H


// These registers are in Bank 0 at the given offsets from the base address
// Since they are set up for 32-bit accesses, the offsets are multiplied by 2 from
//  the numbers given in the SMC91C94 spec.
#define TCR_REG			0
#define EPHSTATUS_REG	2
#define RCR_REG			4
#define COUNTER_REG		6
#define MIR_REG			8
#define MCR_REG			10
// The register at offset 24 is reserved
// The bank select register is the same for all 4 banks
#define BANKSEL_REG		14

// Bank 1 registers
#define CONFIG_REG		0
#define BASE_REG		2
#define MACADDR_REG0	4
#define MACADDR_REG1	6
#define MACADDR_REG2	8
#define GENERAL_REG		10
#define CONTROL_REG		12

// Bank 2 _REGisters
#define MMUCOMMAND_REG	0
#define PNR_ARR_REG		2
#define FIFOPORTS_REG	4
#define POINTER_REG		6
#define DATA_REG		8
#define AUXDATA_REG		10
#define INTERRUPT_REG	12
#define INTERRUPT_MASK_REG 13

// Bank 3 registers
#define MULTITAB_REG0	0
#define MULTITAB_REG1	2
#define MULTITAB_REG2	4
#define MULTITAB_REG3	6
#define MGMT_REG		8
#define REVISION_REG	10
#define ERCV_REG		12

// These values are written to the bank select register to change banks.
#define BANK0 0x3300
#define BANK1 0x3301
#define BANK2 0x3302
#define BANK3 0x3303

// EPH Status reg (also used for TX status word)
#define EPH_UNDERRUN            0x8000  //  Frame uderrun
#define EPH_LINKERROR           0x4000  //  10BASET link error condition
#define EPH_RXOVERRUN           0x2000  //  Receiver overrun
#define EPH_COUNTER             0x1000  //  Counter roll over
#define EPH_EXDEFER             0x0800  //  Excessive deferral
#define EPH_CARRIER             0x0400  //  Carrier not present
#define EPH_LATE                0x0200  //  Late collision
#define EPH_DEFER               0x0080  //  Frame was deferred
#define EPH_BCAST               0x0040  //  Last frame was broadcast
#define EPH_SQET                0x0020  //  Signal Quality Error
#define EPH_16COL               0x0010  //  Too many collisions
#define EPH_MCAST               0x0008  //  Last frame was multicast
#define EPH_MULTICOL            0x0004  //  Multiple collisions on last frame
#define EPH_1COL                0x0002  //  Single collision on last frame
#define EPH_TX_OK               0x0001  //  Frame successfully transmitted


// MMU Commands
#define CMD_NOP                 0       //  No-Op command
#define CMD_ALLOC               0x0020  //  Allocate memory
#define CMD_RESET               0x0040  //  Reset MMU to initial state
#define CMD_REM_TOP             0x0060  //  Remove frame from top of RX fifo
#define CMD_REM_REL_TOP         0x0080  //  Remove and release top of RX fifo
#define CMD_REL_SPEC            0x00a0  //  Release specific packet
#define CMD_ENQ_TX              0x00c0  //  Enqueue to xmit fifo
#define CMD_ENQ_RX              0x00e0  //  Reset xmit fifos (should only be done
                                        //  with transmitter disabled)
#define MMU_CMD_BUSY            0x0001  //  MMU busy, don't modify PNR

// Allocation Result Register (low byte of PNR_ARR_REG)
#define ARR_FAIL                0x80    //  Allocation failed
#define ARR_ALLOC_MSK           0x7f    //  Mask allocated packet number
                                        //  Actually 1fh but use full mask for
                                        //  upward compatibility.

// Pointer Register (POINTER_REG)
#define PTR_RCV                 0x8000  //  Access is to receive area
#define PTR_AUTO                0x4000  //  Auto-increment on access
#define PTR_READ                0x2000  //  =1 then read operation
                                        //  =0 then write operation
#define PTR_ETEN                0x1000  //  Detect early transmit underrun
#define PTR_NOT_EMPTY           0x0800  //  Write fifo not empty
#define PTR_OFFSET              0x03ff  //  Mask pointer value

// Fifo Ports Register (FIFOPORTS_REG)
#define FIFO_EMPTY              0x80    //  No packet at top of fifo
#define FIFO_TX_PKT_MASK        0x7f    //  Mask top packet number
                                        //  Actually 1fh but use full mask for
                                        //  upward compatibility.

// Interrupt flags that can be set/acknowledged
#define ERCV_INT	0x0040
#define EPH_INT		0x0020
#define RX_OVRN_INT	0x0010
#define ALLOC_INT	0x0008
#define TXEMPTY_INT	0x0004
#define TX_INT		0x0002
#define RCV_INT		0x0001

// Interrupt mask bits, if these are set the interrupt is enabled
#define ERCV_INTM		0x4000
#define EPH_INTM		0x2000
#define RX_OVRN_INTM	0x1000
#define ALLOC_INTM		0x0800
#define TXEMPTY_INTM	0x0400
#define TX_INTM			0x0200
#define RCV_INTM		0x0100

// This value is used to intialize the Control Register (Pg 49 of the SMC91C94 spec):
// Bit 14 - RCV_BAD - Don't receive Frames with bad CRC
// Bit 13 - PWRDN - Don't go into power down mode
// Bit 11 - AUTO RELEASE - Do use the auto memory release feature for successfully transmitted Frames
// Bit  7 - LE ENABLE - Don't generate interrupts for link errors (merged through EPH_INT)
// Bit  6 - CR ENABLE - Don't generate interrupts for counter roll over (merged through EPH_INT)
// Bit  5 - TE ENABLE - Do generate interrupts for transmit errors (merged through EPH_INT)
// Bit  2 - EEPROM SELECT - Don't select the EEPROM
// Bit  1 - RELOAD - Don't reload EEPROM data
// Bit  0 - STORE - Don't store EEPROM data
#define CONTROL_REG_INIT 0x0920

// This value is used to intialize the Transmit Control Register (Pg 35 of the SMC91C94 spec):
// Bit 13 - EPH LOOP - Don't do EPH Internal Loopback
// Bit 12 - STP SQET - Don't stop transmission on SQET error
// DANGER - May need FDUPLX for IPX
// Bit 11 - FDUPLX - Don't use full duplex operation so that Frames sourced by this card are not received by it
// Bit 10 - MON_CSN - Don't monitor carrier while transmitting.  Transmission will continue whether the card senses
//  it's own carrier after the preamble or not.
// Bit  8 - NOCRC - Do insert the CRC at the end of transmitted frames automatically
// Bit  7 - PAD_EN - Do pad frames shorter than the minimum of 64 bytes automatically
// Bit  2 - FORCOL - Don't force a collision
// Bit  1 - LOOP - Don't loop back frames internally without transmitting
// Bit  0 - TXENA - Do enable transmission
#define TCR_REG_INIT 0x0081

// This value is used to initialize the Memory Configuration Register (Pg 42 of the SMC91C94 spec):
// Bits 7-0 - MEMORY RESERVED FOR TRANSMIT - Reserve memory for transmit purposes only.  I need to
//  reserve enough memory for one maximum sized Frame for transmission purposes.  This will prevent
//  deadlock conditions in which Frames keep coming in, but can't be acknowledged because of a lack of memory.
// The amount of memory reserved is calculated as MCR_REG_INIT * 256 * M, where M is 1 for the 91C94
//  (pg 42 of the SMC91C94 spec.).  So, for a 1500 byte frame, I need 1500 / 256 = 6 memory pages.
#define MCR_REG_INIT 0x0006

// This value is used to initialize the Receive Control Register (Pg 39 of the SMC91C94 spec):
// Bit 15 - SOFT RST - Don't do a soft reset
// Bit 14 - FILT_CAR - Don't filter the carrier signal
// Bit  9 - STRIP CRC - Don't strip the CRC from the Frame
// Bit  8 - RXEN - Do enable the Frame receiver
// Bit  2 - ALMUL - Don't accept all multicast frames
// Bit  1 - PRMS - Don't go into promiscuous mode
// Bit  0 - RX_ABORT - Write the receive abort flag low
//	(set by a frame that was longer than 1532 bytes or out of buffer memory error)
#define RCR_REG_INIT 0x0100


#endif  // _SMCHW_H
