.DATA
    MSG     DB  "Hello, World!", 0Dh, 0Ah, '$'
.CODE

STARTUP PROC   ; Setup the starting address otherwise you'll see:
            ; LINK : warning L4055: start address not equal to 0x100 for /TINY
    MOV     AH, 09h
    MOV     DX, 10
    INT     21h
    MOV     AH, 4Ch
    INT     21h
STARTUP ENDP

END