
;        .globl offset
offset	=  -0x40

; produces an incorrect operand
        cp a,(hl)			;BE
	cp a,offset(iy)			;FD BE C0
        cp a,(iy+offset)		;FD BE C0
	cp a,#offset			;FE C0
	
; produces the correct operand
        cp (hl)				;BE
        cp offset(iy)			;FD BE C0
        cp (iy+offset)			;FD BE C0
        cp #offset			;FE C0
 
 	adc	a,offset(ix)		;DD 8E C0

