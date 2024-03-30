main:
  mov AX, 4A32h      
  mov BL, byte 3              
  int 2fh

  mov AX, 4A32h      
  mov BL, byte 5              
  int 2fh

  mov ah,4ch
	int 21h