;***********************************************************************************************************
;
;       Init.Asm - Boot Loader initialization code for x86 PC.
;
;       Author: RBN
;
;       Written 5/1/96
;
;***********************************************************************************************************

.486p

OpPrefix MACRO
    db      66h
    ENDM

_TEXT SEGMENT WORD PUBLIC USE16 'CODE'

FLAT_STACK_SIZE     EQU 4000h
        db      FLAT_STACK_SIZE dup ( 0 )
FLAT_STACK_START    equ     $        

;
;       This static GDT contains 2 selectors - A flat code selector, and a flat data selector.
;
GDT_Data LABEL DWORD
        db      0,    0,   0,   0,   0,         0,         0,   0 ; First GDT entry always unused
CS_FLAT_SEL EQU ($-GDT_Data)
        db      0FFh, 0FFh, 00h, 00h, 00h, 10011010b, 11001111b, 00h ; Code
DS_FLAT_SEL EQU ($-GDT_Data)
        db      0FFh, 0FFh, 00h, 00h, 00h, 10010010b, 11001111b, 00h ; Data
GDT_TABLE_SIZE = $ - OFFSET GDT_Data

;
;       Limit + Pointer to the GDT
;
GDTPtr LABEL FWORD
        dw      GDT_TABLE_SIZE - 1  ; Limit of 0 = 1 byte
        dd      OFFSET GDT_Data

PModeEntrySeg   dw      SEG InPModeNow

PModeEntryOff   dd      OFFSET InPModeNow

        

;******************************************************************************
;
;       Launch
;
;       At this point, we are running in a 16-bit code segment.  
;       Since this code is written in a 16-bit code segment, we need to put an
;       OpPrefix in front of all 32-bit instructions.
;                       
;       This function jumps to InPModeNow to force CS to be reloaded with a
;       valid PMode selector.
;
;******************************************************************************

Launch  PROC NEAR C PUBLIC

        cli                         ; Make sure we don't get any more interrupts

        mov     al, 0FFh            ; Disable all PIC interrupts
        out     021h, al

if 0        
        mov     dx, 003FBh
        mov     al, 080h            ; Access Baud Divisor
        out     dx, al

        dec     dx                  ; 3FAh
        dec     dx                  ; 3F9h

        dec     dx                  ; 3F8h
        mov     al, 002h            ; 57600 Baud
        out     dx, al

        inc     dx                  ; 3F9h
        xor     al, al
        out     dx, al

        inc     dx                  ; 3FAh

        inc     dx                  ; 3FBh
        mov     al, 003h            ; DLAB = 0, 8 bit, no parity
        out     dx, al

        dec     dx                  ; 3FAh

        dec     dx                  ; 3F9h
        xor     al, al              ; No interrupts, polled
        out     dx, al

        inc     dx                  ; 3FAh
        mov     al, 001h            ; Enable FIFO if present
        out     dx, al

        inc     dx                  ; 3FBh

        inc     dx                  ; 3FCh
        mov     al, 003h            ; Assert DTR, RTS
        out     dx, al

        mov     dx, 03FDh        
@@:
        in      al, dx               ; Get Modem status
        test    al, 020h
        jz      @B

        mov     al, 'A'
        mov     dx, 03F8h
        out     dx, al
endif

        ;
        ; Since we were loaded by the DOS 16bit real-mode loader we need to
        ; manually relocate all references to linear addresses before we switch
        ; to protect mode.
        ;

        xor     ebx, ebx            ; Clear upper word
        mov     bx, cs              ; Our segment 
        shl     ebx, 4              ; Convert segment to linear address

        mov     eax, DWORD PTR [GDTPtr + 2]
        add     eax, ebx
        mov     DWORD PTR [GDTPtr + 2], eax

        xor     eax, eax
        mov     ax, [PModeEntrySeg]
        shl     eax, 4
        add     eax, [PModeEntryOff]
        mov     [PModeLbl], eax

        pop     ax                  ; Remove return address from stack
        pop     edx                 ; Load entry point from arguments
        pop     esi                 ; Linear address of arguments

        OpPrefix
        lgdt    FWORD PTR [GDTPtr]  ; Load the GDTR

        mov     ecx, OFFSET FLAT_STACK_START
        add     ecx, ebx

    ;
    ;   Don't need OpPrefix on mov to/from CR0 -- It's always 32-bit
    ;
        mov     eax, cr0            ; Get the current CR0
        or      al, 1               ; Set the PE bit to enable protected mode
        mov     cr0, eax            ; NOW WE'RE IN PMODE!

        OpPrefix
        db      0EAh                ; Far jump forces a selector lookup
PModeLbl        dd      0
        dw      CS_FLAT_SEL

Launch  ENDP

_TEXT ENDS

_TEXT32 SEGMENT WORD PUBLIC USE32 'CODE32'

;***********************************************************************************************************
;
;       InPModeNow
;
;       This function is responsible for setting up the data selectors and the stack and then jumping to main.
;
;***********************************************************************************************************

InPModeNow PROC NEAR

        mov     eax, DS_FLAT_SEL
        mov     ds, eax
        mov     es, eax
        mov     fs, eax
        mov     gs, eax
        mov     ss, eax
        mov     esp, ecx

        push    edx

        mov     edx, 001FFFFCh
        mov     dword ptr [edx], esi ; Save linear ptr to args in known location

if 0
        mov     dx, 03FDh        
@@:
        in      al, dx               ; Get Modem status
        test    al, 020h
        jz      @B

        mov     al, 'B'
        mov     dx, 03F8h
        out     dx, al
endif

        ret                         ; Jump to entry point    

InPModeNow ENDP         

_TEXT32 ENDS

    END
