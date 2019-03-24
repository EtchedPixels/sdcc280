	.title	Test of Z280 assembler

	offset	=	0x55		;arbitrary constants
	n	=	0x20
	nn	=	0x0584

	sxoff	=	0x55		; SX offset (-128..127)
	lxoff	=	0x1122		;  X offset
	raoff	=	0x1234		; RA address/offset
	daddr	=	0x3344		; DA address
		.globl	ext		; DA external address
		.globl	subr		; external subroutine
		
ODDBALL_SHIFT_ROTATE	=	0	; prefer this set to 0

	.area	_code
	
;*******************************************************************	
;*******************************************************************
;	Start test of addressing syntax	
;*******************************************************************	
;*******************************************************************	
;*******************************************************************	
	
	.z280
	;***********************************************************
	; add with carry
	adc	a,(hl)			;8E
	adc	a,(hl+ix)		;DD 89
	adc	a,(hl+iy)		;DD 8A
	adc	a,(hl+lxoff)		;FD 8B 22 11
	adc	a,(hl+2)		;FD 8B 02 00
	adc	a,lxoff(hl)		;FD 8B 22 11
	adc	a,(ix+iy)		;DD 8B
	adc	a,(ix)			;DD 8E 00
	adc	a,(ix+sxoff)		;DD 8E 55
	adc	a,(ix-sxoff)		;DD 8E AB
	adc	a,sxoff(ix)		;DD 8E 55
	adc	a,-sxoff(ix)		;DD 8E AB
	adc	a,(iy)			;FD 8E 00
	adc	a,(iy+sxoff)		;FD 8E 55
	adc	a,.+raoff(pc)		;FD 88 30 12
	adc	a,(pc+.+raoff)		;FD 88 30 12
	adc	A,[.+sxoff]		;FD 88 51 00
	adc	a,sxoff+.(pc)		;FD 88 51 00
	adc	a,.+raoff(pc)		;FD 88 30 12
	ADC	a,[.+raoff]		;FD 88 30 12
	adc	a,[.]			;FD 88 FC FF
	adc	a,(sp+lxoff)		;DD 88 22 11
	adc	a,lxoff(sp)		;DD 88 22 11
	adc	a,(daddr)		;DD 8F 44 33
	adc	a,b			;88
	adc	a,c			;89
	adc	a,d			;8A
	adc	a,e			;8B
	adc	a,h			;8C
	adc	a,l			;8D
	adc	a,(hl)			;8E
	;***********************************************************
data8:	.db	8
data16:	.dw	1000
	;***********************************************************
	adc	a,a			;8F
	adc	a,ixh			;DD 8C
	adc	a,ixl			;DD 8D
	adc	a,iyh			;FD 8C
	adc	a,iyl			;FD 8D
	adc	a,#n			;CE 20
	adc	a, n			;CE 20
	adc	hl,bc			;ED 4A
	adc	hl,de			;ED 5A
	adc	hl,hl			;ED 6A
	adc	hl,sp			;ED 7A
	adc	ix,bc			;DD ED 4A
	adc	ix,de			;DD ED 5A
	adc	ix,ix			;DD ED 6A
	adc	ix,sp			;DD ED 7A
	adc	iy,bc			;FD ED 4A
	adc	iy,de			;FD ED 5A
	adc	iy,iy			;FD ED 6A
	adc	iy,sp			;FD ED 7A
	
	
;*******************************************************************	
;*******************************************************************
;	End test of addressing syntax	
;*******************************************************************	
;*******************************************************************	
;*******************************************************************	
	


	

	
	
	
	
	; notes:
	;	Leading 'a' operand is optional.
	;	If offset is ommitted 0 is assumed.
	.z80
	;***********************************************************
	; add with carry to 'a'
	adc	a,(hl)			;8E
	adc	a,offset(ix)		;DD 8E 55
	adc	a,offset(iy)		;FD 8E 55
	adc	a,(ix+offset)		;DD 8E 55
	adc	a,(iy+offset)		;FD 8E 55
	adc	a,(ix)			;DD 8E 00
	adc	a,(iy)			;FD 8E 00
	adc	a,a			;8F
	adc	a,b			;88
	adc	a,c			;89
	adc	a,d			;8A
	adc	a,e			;8B
	adc	a,h			;8C
	adc	a,l			;8D
	adc	a,#n			;CE 20
	adc	a, n			;CE 20
	;***********************************************************
	adc	(hl)			;8E
	adc	offset(ix)		;DD 8E 55
	adc	offset(iy)		;FD 8E 55
	adc	(ix+offset)		;DD 8E 55
	adc	(iy+offset)		;FD 8E 55
	adc	(ix)			;DD 8E 00
	adc	(iy)			;FD 8E 00
	adc	a			;8F
	adc	b			;88
	adc	c			;89
	adc	d			;8A
	adc	e			;8B
	adc	h			;8C
	adc	l			;8D
	adc	#n			;CE 20
	;***********************************************************
	; add with carry register pair to 'hl'
	adc	hl,bc			;ED 4A
	adc	hl,de			;ED 5A
	adc	hl,hl			;ED 6A
	adc	hl,sp			;ED 7A
	;***********************************************************
	; add with carry to accumulator (addressing mode check)
	.z280
	adc	a,(data)		;DD 8F 14 00
	adc	a,(nn+6)		;DD 8F 8A 05
;;;	adc	a, n			;DD 8F 20 00
	adc	a,(ix+nn)		;FD 89 84 05
	adc	a,(iy+nn)		;FD 8A 84 05
	adc	a,(hl+offset)		;FD 8B 55 00
	adc	a,(sp)			;DD 88 00 00
	adc	a,(sp+nn)		;DD 88 84 05
	adc	a,(sp+offset)		;DD 88 55 00
	adc	a,(hl+ix)		;DD 89
	adc	a,(hl+iy)		;DD 8A
	adc	a,(ix+iy)		;DD 8B
	adc	a,[.+32]		;FD 88 1C 00
	;***********************************************************
	; add with carry to register IX, IY
	adc	ix,bc			;DD ED 4A
	adc	ix,de			;DD ED 5A
	adc	ix,ix			;DD ED 6A
	adc	ix,sp			;DD ED 7A
	adc	iy,bc			;FD ED 4A
	adc	iy,de			;FD ED 5A
	adc	iy,iy			;FD ED 6A
	adc	iy,sp			;FD ED 7A
		
	.z80
	;***********************************************************
	; add operand to 'a'
	add	a,(hl)			;86
	add	a,offset(ix)		;DD 86 55
	add	a,offset(iy)		;FD 86 55
	add	a,(ix+offset)		;DD 86 55
	add	a,(iy+offset)		;FD 86 55
	add	a,a			;87
	add	a,b			;80
	add	a,c			;81
	add	a,d			;82
	add	a,e			;83
	add	a,h			;84
	add	a,l			;85
	add	a,#n			;C6 20
	add	a, n			;C6 20
	;***********************************************************
	; single argument add to AC	
	add	(hl)			;86	
	add	offset(ix)		;DD 86 55
	add	offset(iy)		;FD 86 55
	add	(ix+offset)		;DD 86 55
	add	(iy+offset)		;FD 86 55
	add	a			;87
	add	b			;80
	add	c			;81
	add	d			;82
	add	e			;83
	add	h			;84
	add	l			;85
	add	#n			;C6 20
	add	 n			;C6 20
	;***********************************************************
	; add register pair to 'hl'
	add	hl,bc			;09
	add	hl,de			;19
	add	hl,hl			;29
	add	hl,sp			;39
	;***********************************************************
	; add register pair to 'ix'
	add	ix,bc			;DD 09
	add	ix,de			;DD 19
	add	ix,ix			;DD 29
	add	ix,sp			;DD 39
	;***********************************************************
	; add register pair to 'iy'
	add	iy,bc			;FD 09
	add	iy,de			;FD 19
	add	iy,iy			;FD 29
	add	iy,sp			;FD 39
	;***********************************************************
	;  p. 5-16
	.z280
	add	a,ixh			;DD 84
	add	a,ixl			;DD 85
	add	a,iyh			;FD 84
	add	a,iyl			;FD 85
	add	a,(daddr)		;DD 87 44 33
	add	a,(hl+lxoff)		;FD 83 22 11
	add	a,(ix+lxoff)		;FD 81 22 11
	add	a,(iy+lxoff)		;FD 82 22 11
	add	a,[.+raoff]		;FD 80 30 12
	add	a,(sp)			;DD 80 00 00
	add	a,offset(sp)		;DD 80 55 00
	add	a,(hl+ix)		;DD 81
	add	a,(hl+iy)		;DD 82
	add	a,(ix+iy)		;DD 83
	;***********************************************************
	;  p. 5-18
	add	hl,#daddr		;FD ED F6 44 33
	add	hl,(daddr)		;DD ED D6 44 33
	add	hl,(ix+lxoff)		;FD ED C6 22 11
	add	hl,(iy+lxoff)		;FD ED D6 22 11
	add	hl,(hl)			;DD ED C6
	addw	hl,#daddr		;FD ED F6 44 33
	addw	hl,(daddr)		;DD ED D6 44 33
	addw	hl,(ix+lxoff)		;FD ED C6 22 11
	addw	hl,(iy+lxoff)		;FD ED D6 22 11
	ADDW	HL,(IX+3)		;FD ED C6 03 00
	ADDW	HL,(IY-4)		;FD ED D6 FC FF
	addw	hl,[.+n]		;DD ED F6 1B 00
	addw	hl,(hl)			;DD ED C6
	;***********************************************************
	.z80
	;***********************************************************
	; logical 'and' operand with 'a'
	and	a,(hl)			;A6
	and	a,offset(ix)		;DD A6 55
	and	a,offset(iy)		;FD A6 55
	and	a,(ix+offset)		;DD A6 55
	and	a,(iy+offset)		;FD A6 55
	and	a,a			;A7
	and	a,b			;A0
	and	a,c			;A1
	and	a,d			;A2
	and	a,e			;A3
	and	a,h			;A4
	and	a,l			;A5
	and	a,#n			;E6 20
	and	a, n			;E6 20
	;***********************************************************
	;  p. 5--19
	.z280
	and	a,ixh			;DD A4
	and	a,ixl			;DD A5
	and	a,iyh			;FD A4
	and	a,iyl			;FD A5
	and	a,(daddr)		;DD A7 44 33
	and	a,lxoff(ix)		;FD A1 22 11
	and	a,(iy+lxoff)		;FD A2 22 11
	and	a,lxoff(hl)		;FD A3 22 11
	and	a,[.+32]		;FD A0 1C 00
	and	a,(pc+(.+raoff))	;FD A0 30 12
	and	a,-n(sp)		;DD A0 E0 FF
	and	a,(hl+ix)		;DD A1
	and	a,(hl+iy)		;DD A2
	and	a,(ix+iy)		;DD A3
	.z80
	;***********************************************************
	; test bit of location or register
	bit	0,(hl)			;CB 46
	bit	0,offset(ix)		;DD CB 55 46
	bit	0,offset(iy)		;FD CB 55 46
	bit	0,(ix+offset)		;DD CB 55 46
	bit	0,(iy+offset)		;FD CB 55 46
	bit	0,a			;CB 47
	bit	0,b			;CB 40
	bit	0,c			;CB 41
	bit	0,d			;CB 42
	bit	0,e			;CB 43
	bit	0,h			;CB 44
	bit	0,l			;CB 45
	bit	1,(hl)			;CB 4E
	bit	1,offset(ix)		;DD CB 55 4E
	bit	1,offset(iy)		;FD CB 55 4E
	bit	1,(ix+offset)		;DD CB 55 4E
	bit	1,(iy+offset)		;FD CB 55 4E
	bit	1,a			;CB 4F
	bit	1,b			;CB 48
	bit	1,c			;CB 49
	bit	1,d			;CB 4A
	bit	1,e			;CB 4B
	bit	1,h			;CB 4C
	bit	1,l			;CB 4D
	bit	2,(hl)			;CB 56
	bit	2,offset(ix)		;DD CB 55 56
	bit	2,offset(iy)		;FD CB 55 56
	bit	2,(ix+offset)		;DD CB 55 56
	bit	2,(iy+offset)		;FD CB 55 56
	bit	2,a			;CB 57
	bit	2,b			;CB 50
	bit	2,c			;CB 51
	bit	2,d			;CB 52
	bit	2,e			;CB 53
	bit	2,h			;CB 54
	bit	2,l			;CB 55
	bit	3,(hl)			;CB 5E
	bit	3,offset(ix)		;DD CB 55 5E
	bit	3,offset(iy)		;FD CB 55 5E
	bit	3,(ix+offset)		;DD CB 55 5E
	bit	3,(iy+offset)		;FD CB 55 5E
	bit	3,a			;CB 5F
	bit	3,b			;CB 58
	bit	3,c			;CB 59
	bit	3,d			;CB 5A
	bit	3,e			;CB 5B
	bit	3,h			;CB 5C
	bit	3,l			;CB 5D
	bit	4,(hl)			;CB 66
	bit	4,offset(ix)		;DD CB 55 66
	bit	4,offset(iy)		;FD CB 55 66
	bit	4,(ix+offset)		;DD CB 55 66
	bit	4,(iy+offset)		;FD CB 55 66
	bit	4,a			;CB 67
	bit	4,b			;CB 60
	bit	4,c			;CB 61
	bit	4,d			;CB 62
	bit	4,e			;CB 63
	bit	4,h			;CB 64
	bit	4,l			;CB 65
	bit	5,(hl)			;CB 6E
	bit	5,offset(ix)		;DD CB 55 6E
	bit	5,offset(iy)		;FD CB 55 6E
	bit	5,(ix+offset)		;DD CB 55 6E
	bit	5,(iy+offset)		;FD CB 55 6E
	bit	5,a			;CB 6F
	bit	5,b			;CB 68
	bit	5,c			;CB 69
	bit	5,d			;CB 6A
	bit	5,e			;CB 6B
	bit	5,h			;CB 6C
	bit	5,l			;CB 6D
	bit	6,(hl)			;CB 76
	bit	6,offset(ix)		;DD CB 55 76
	bit	6,offset(iy)		;FD CB 55 76
	bit	6,(ix+offset)		;DD CB 55 76
	bit	6,(iy+offset)		;FD CB 55 76
	bit	6,a			;CB 77
	bit	6,b			;CB 70
	bit	6,c			;CB 71
	bit	6,d			;CB 72
	bit	6,e			;CB 73
	bit	6,h			;CB 74
	bit	6,l			;CB 75
	bit	7,(hl)			;CB 7E
	bit	7,offset(ix)		;DD CB 55 7E
	bit	7,offset(iy)		;FD CB 55 7E
	bit	7,(ix+offset)		;DD CB 55 7E
	bit	7,(iy+offset)		;FD CB 55 7E
	bit	7,a			;CB 7F
	bit	7,b			;CB 78
	bit	7,c			;CB 79
	bit	7,d			;CB 7A
	bit	7,e			;CB 7B
	bit	7,h			;CB 7C
	bit	7,l			;CB 7D
	;***********************************************************
	; call subroutine at nn if condition is true
	call	C,nn			;DC 84 05
	call	M,nn			;FC 84 05
	call	s,nn			;FC 84 05
	call	NC,nn			;D4 84 05
	call	NZ,nn			;C4 84 05
	call	P,nn			;F4 84 05
	CALL	NS,nn			;F4 84 05
	call	PE,nn			;EC 84 05
	call	V,nn			;EC 84 05
	call	PO,nn			;E4 84 05
	call	nv,nn			;E4 84 05
	call	Z,nn			;CC 84 05
	;***********************************************************
	; unconditional call to subroutine at nn
	call	nn			;CD 84 05
	call	(nn)			;CD 84 05
	call	gg			;CD 00 00
	call	#nn			;CD 84 05
	;***********************************************************
	;  p. 5-21
	.z280
	call	(hl)			;DD CD
	call	z,(hl)			;DD CC
	call	NV,(hl)			;DD E4
	call	[.+0x108]		;FD CD 04 01
	call	c,.+0x108(pc)		;FD DC 04 01
.if 0
	call	subr			;CD 00 00
	call	(subr)			;CD 00 00
	call	[subr]			;FD CD 00 00
	call	(0x100)			;CD 00 01	
	call	[0x100]			;FD CD 00 00
.endif
	.z80
	;***********************************************************
	; complement carry flag
	ccf				;3F
	;***********************************************************
	; compare operand with 'a'
	cp	a,(hl)			;BE
	cp	a,offset(ix)		;DD BE 55
	cp	a,offset(iy)		;FD BE 55
	cp	a,(ix+offset)		;DD BE 55
	cp	a,(iy+offset)		;FD BE 55
	cp	a,a			;BF
	cp	a,b			;B8
	cp	a,c			;B9
	cp	a,d			;BA
	cp	a,e			;BB
	cp	a,h			;BC
	cp	a,l			;BD
	cp	a,#n			;FE 20
	cp	a, n			;FE 20
	;***********************************************************
	;  p. 5-23
	.z280
	cp	a,ixh			;DD BC
	cp	a,ixl			;DD BD
	cp	a,iyh			;FD BC
	cp	a,iyl			;FD BD
	cp	a,(daddr)		;DD BF 44 33
	cp	a,(ix+lxoff)		;FD B9 22 11
	cp	a,(iy+lxoff)		;FD BA 22 11
	cp	a,(hl+lxoff)		;FD BB 22 11
	cp	a,[.+offset]		;FD B8 51 00
	cp	a,lxoff(sp)		;DD B8 22 11
	cp	a,(sp)			;DD B8 00 00
	cp	a,(hl+ix)		;DD B9
	cp	a,(hl+iy)		;DD BA
	cp	a,(ix+iy)		;DD BB
	;***********************************************************
	;  p. 5-29
	cpw	hl,bc			;ED C7
	cpw	hl,de			;ED D7
	cpw	hl,hl			;ED E7
	cpw	hl,sp			;ED F7
	CPW	HL,IX			;DD ED E7
	CPW	hl,iy			;FD ED E7
	cpw	hl,#nn			;FD ED F7 84 05
	cpw	hl,(daddr)		;DD ED D7 44 33
	cpw	hl,(ix+lxoff)		;FD ED C7 22 11
	cpw	hl,(iy+lxoff)		;FD ED D7 22 11
	cpw	hl,(ix)			;FD ED C7 00 00
	cpw	hl,(iy-4)		;FD ED D7 FC FF
	cpw	hl,[.+20]		;DD ED F7 0F 00
	cpw	hl,(hl)			;DD ED C7
	cp	hl,bc			;ED C7
	cp	hl,de			;ED D7
	cp	hl,hl			;ED E7
	cp	hl,sp			;ED F7
	CP	HL,IX			;DD ED E7
	CP	hl,iy			;FD ED E7
	cp	hl,#nn			;FD ED F7 84 05
	cp	hl,(daddr)		;DD ED D7 44 33
	cp	hl,(ix+lxoff)		;FD ED C7 22 11
	cp	hl,(iy+lxoff)		;FD ED D7 22 11
	cp	hl,(ix)			;FD ED C7 00 00
	cp	hl,(iy-4)		;FD ED D7 FC FF
	cp	hl,[.+20]		;DD ED F7 0F 00
	cp	hl,(hl)			;DD ED C7
	cpw	bc			;ED C7
	cpw	de			;ED D7
	cpw	hl			;ED E7
	cpw	sp			;ED F7
	CPW	IX			;DD ED E7
	CPW	iy			;FD ED E7
	cpw	#nn			;FD ED F7 84 05
	cpw	(daddr)			;DD ED D7 44 33
	cpw	(ix+lxoff)		;FD ED C7 22 11
	cpw	(iy+lxoff)		;FD ED D7 22 11
	cpw	(ix)			;FD ED C7 00 00
	cpw	(iy-4)			;FD ED D7 FC FF
	cpw	[.+20]			;DD ED F7 0F 00
	cpw	(hl)			;DD ED C7
	.z80
	;***********************************************************
	; compare location (hl) and 'a'
	; decrement 'hl' and 'bc'
	cpd				;ED A9
	;***********************************************************
	; compare location (hl) and 'a'
	; decrement 'hl' and 'bc'
	; repeat until 'bc' = 0
	cpdr				;ED B9
	;***********************************************************
	; compare location (hl) and 'a'
	; increment 'hl' and decrement 'bc'
	cpi				;ED A1
	;***********************************************************
	; compare location (hl) and 'a'
	; increment 'hl' and decrement 'bc'
	; repeat until 'bc' = 0
	cpir				;ED B1
	;***********************************************************
	; 1's complement of 'a'
	cpl				;2F
	;***********************************************************
	; decimal adjust 'a'
	daa				;27
	;***********************************************************
	; decrement operand
	dec	(hl)			;35
	dec	offset(ix)		;DD 35 55
	dec	offset(iy)		;FD 35 55
	dec	(ix+offset)		;DD 35 55
	dec	(iy+offset)		;FD 35 55
	dec	a			;3D
	dec	b			;05
	dec	bc			;0B
	dec	c			;0D
	dec	d			;15
	dec	de			;1B
	dec	e			;1D
	dec	h			;25
	dec	hl			;2B
	dec	ix			;DD 2B
	dec	iy			;FD 2B
	dec	l			;2D
	dec	sp			;3B
	;***********************************************************
	;  p. 5-32
	decb	(hl)			;35
	decb	offset(ix)		;DD 35 55
	decb	offset(iy)		;FD 35 55
	decb	(ix+offset)		;DD 35 55
	decb	(iy+offset)		;FD 35 55
	decw	bc			;0B
	decw	de			;1B
	decw	hl			;2B
	decw	sp			;3B
	decw	ix			;DD 2B
	decw	iy			;FD 2B
	.z280
	dec	ixh			;DD 25
	dec	ixl			;DD 2D
	dec	iyh			;FD 25
	dec	iyl			;FD 2D
	dec	(daddr)			;DD 3D 44 33
	dec	(ix+lxoff)		;FD 0D 22 11
	dec	(iy+lxoff)		;FD 15 22 11
	dec	(hl+lxoff)		;FD 1D 22 11
	dec	(hl+offset)		;FD 1D 55 00
	dec	[.+offset]		;FD 05 51 00
	dec	lxoff(sp)		;DD 05 22 11
	dec	(sp)			;DD 05 00 00
	dec	(hl+ix)			;DD 0D
	dec	(hl+iy)			;DD 15
	dec	(ix+iy)			;DD 1D
	decb	(daddr)			;DD 3D 44 33
	decb	(ix+lxoff)		;FD 0D 22 11
	decb	(iy+lxoff)		;FD 15 22 11
	decb	(hl+lxoff)		;FD 1D 22 11
	decb	(hl+offset)		;FD 1D 55 00
	decb	[.+offset]		;FD 05 51 00
	decb	lxoff(sp)		;DD 05 22 11
	decb	(sp)			;DD 05 00 00
	decb	(hl+ix)			;DD 0D
	decb	(hl+iy)			;DD 15
	decb	(ix+iy)			;DD 1D
	;***********************************************************
	;  p. 5-33
	decw	(hl)			;DD 0B
	decw	(daddr)			;DD 1B 44 33
	decw	(ix+lxoff)		;FD 0B 22 11
	decw	(iy+lxoff)		;FD 1B 22 11
	decw	.+lxoff(pc)		;DD 3B 1E 11
	decw	[.+offset]		;DD 3B 51 00
	.z80
	;***********************************************************
	;***********************************************************
	; disable interrupts
	di				;F3
;	di	#3			;	q
	.z180
	di				;F3
;	di	#3			;	q
	;***********************************************************
	;  p. 5-34
	.z280
;	di				;	q
;	di	#3			;	q
	.z280p
	di				;F3
	di	#3			;ED 77 03
	di	3			;ED 77 03
	;***********************************************************
	;  p. 5-35	DIV(byte)
	.z280
	div	hl,a			;ED FC
	div	hl,b			;ED C4
	div	hl,c			;ED CC
	div	hl,d			;ED D4
	div	hl,e			;ED DC
	div	hl,h			;ED E4
	div	hl,l			;ED EC
	div	hl,ixh			;DD ED E4
	div	hl,ixl			;DD ED EC
	div	hl,iyh			;FD ED E4
	div	hl,iyl			;FD ED EC
	div	hl,#10			;FD ED FC 0A
	div	hl,(daddr)		;DD ED FC 44 33
	div	hl,offset(ix)		;DD ED F4 55
	div	hl,(iy+offset)		;FD ED F4 55
	div	hl,(iy)			;FD ED F4 00
	div	hl,(ix)			;DD ED F4 00
	div	hl,lxoff(ix)		;FD ED CC 22 11
	div	hl,(iy+lxoff)		;FD ED D4 22 11
	div	hl,lxoff(hl)		;FD ED DC 22 11
	div	hl,[.+offset]		;FD ED C4 50 00
	div	hl,offset(sp)		;DD ED C4 55 00
	div	hl,(hl+ix)		;DD ED CC
	div	hl,(hl+iy)		;DD ED D4
	div	hl,(ix+iy)		;DD ED DC
; misprint in the manual on p. 5-35 says the following should assemble as ED E4
;  Appendix C.  corrects this to ED F4
	div	hl,(hl)			;ED F4
	;***********************************************************
	;  p. 5-37	DIVU(byte)
	divu	hl,a			;ED FD
	divu	hl,b			;ED C5
	divu	hl,c			;ED CD
	divu	hl,d			;ED D5
	divu	hl,e			;ED DD
	divu	hl,h			;ED E5
	divu	hl,l			;ED ED
	divu	hl,ixh			;DD ED E5
	divu	hl,ixl			;DD ED ED
	divu	hl,iyh			;FD ED E5
	divu	hl,iyl			;FD ED ED
	divu	hl,#10			;FD ED FD 0A
	divu	hl,(daddr)		;DD ED FD 44 33
	divu	hl,offset(ix)		;DD ED F5 55
	divu	hl,(iy+offset)		;FD ED F5 55
	divu	hl,(iy)			;FD ED F5 00
	divu	hl,(ix)			;DD ED F5 00
	divu	hl,lxoff(ix)		;FD ED CD 22 11
	divu	hl,(iy+lxoff)		;FD ED D5 22 11
	divu	hl,lxoff(hl)		;FD ED DD 22 11
	divu	hl,[.+offset]		;FD ED C5 50 00
	divu	hl,offset(sp)		;DD ED C5 55 00
	divu	hl,(hl+ix)		;DD ED CD
	divu	hl,(hl+iy)		;DD ED D5
	divu	hl,(ix+iy)		;DD ED DD
	divu	hl,(hl)			;ED F5
	;***********************************************************
	;  p. 5-39	DIVUW(word)
	divuw	dehl,bc			;ED CB
	divuw	dehl,de			;ED DB
	divuw	dehl,hl			;ED EB
	divuw	dehl,sp			;ED FB
	divuw	dehl,ix			;DD ED EB
	divuw	dehl,iy			;FD ED EB
	divuw	dehl,(hl)		;DD ED CB
	divuw	dehl,(ix+lxoff)		;FD ED CB 22 11
	divuw	DEHL,(IX)		;FD ED CB 00 00
	divuw	dehl,(iy+lxoff)		;FD ED DB 22 11
	divuw	dehl,[.+offset]		;DD ED FB 50 00
	divuw	dehl,(daddr)		;DD ED DB 44 33
	divuw	dehl,#10		;FD ED FB 0A 00
	;***********************************************************
	;  p. 5-41	DIVW(word)
	divw	dehl,bc			;ED CA
	divw	dehl,de			;ED DA
	divw	dehl,hl			;ED EA
	divw	dehl,sp			;ED FA
	divw	dehl,ix			;DD ED EA
	divw	dehl,iy			;FD ED EA
	divw	dehl,(hl)		;DD ED CA
	divw	dehl,(ix+lxoff)		;FD ED CA 22 11
	DIVW	DEHL,(IX)		;FD ED CA 00 00
	divw	dehl,(iy+lxoff)		;FD ED DA 22 11
	divw	dehl,[.+offset]		;DD ED FA 50 00
	divw	dehl,(daddr)		;DD ED DA 44 33
	divw	dehl,#10		;FD ED FA 0A 00
	.z80
	;***********************************************************
	; decrement b and jump relative if b # 0
	djnz	.+0x12			;10 10
	;***********************************************************
	; enable interrupts
	ei				;FB
	.z180
	ei				;FB
	;***********************************************************
	;  p. 5-44
	.z280p
	ei				;FB
	ei	#0x37			;ED 7F 37
	.z80
	;***********************************************************
	; exchange location and (sp)
	ex	(sp),hl			;E3
	ex	(sp),ix			;DD E3
	ex	(sp),iy			;FD E3
	;***********************************************************
	; exchange af and af'
	ex	af,af'			;08
	;***********************************************************
	; exchange de and hl
	ex	de,hl			;EB
	;***********************************************************
	;  p. 5-47
	.z280
	ex	h,l			;ED EF
	;  p. 5-48
	ex	ix,hl			;DD EB
	ex	iy,hl			;FD EB
	ex	hl,iy			;FD EB
	ex	hl,ix			;DD EB
	; p. 5-49
	ex	a,a			;ED 3F
	ex	a,b			;ED 07
	ex	a,c			;ED 0F
	ex	a,d			;ED 17
	ex	a,e			;ED 1F
	ex	a,h			;ED 27
	ex	a,l			;ED 2F
	ex	a,ixh			;DD ED 27
	ex	a,ixl			;DD ED 2F
	ex	a,iyh			;FD ED 27
	ex	a,iyl			;FD ED 2F
	ex	a,(hl)			;ED 37
	ex	a,(daddr)		;DD ED 3F 44 33
	ex	a,(ix)			;DD ED 37 00
	ex	a,offset(ix)		;DD ED 37 55
	ex	a,offset(iy)		;FD ED 37 55
	ex	a,lxoff(ix)		;FD ED 0F 22 11
	ex	a,lxoff(iy)		;FD ED 17 22 11
	ex	a,(HL+offset)		;FD ED 1F 55 00
	ex	a,[.-lxoff]		;FD ED 07 D9 EE
	ex	a,offset(sp)		;DD ED 07 55 00
	ex	a,(hl+ix)		;DD ED 0F
	ex	a,(hl+iy)		;DD ED 17
	ex	a,(ix+iy)		;DD ED 1F
	;***********************************************************
	;  extend sign  a->hl,   hl->dehl
	;  pp. 5-50, 5-51
	exts				;ED 64
	exts	a			;ED 64
	exts	hl			;ED 6C
	.z80
	;***********************************************************
	; exchange:
	;	bc <-> bc'
	;	de <-> de'
	;	hl <-> hl'
	exx				;D9
	;***********************************************************
	; halt (wait for interrupt or reset)
	halt				;76
	;***********************************************************
	; set interrupt mode
	im	0			;ED 46
	im	1			;ED 56
	im	2			;ED 5E
	.z280p
	im	3			;ED 4E
	.z80
	;***********************************************************
	; load 'a' with input from device n
	in	a,(n)			;DB 20
	in	a,0x33			;DB 33
	;***********************************************************
	; load register with input from (c)
	in	a,(c)			;ED 78
	in	b,(c)			;ED 40
	in	c,(c)			;ED 48
	in	d,(c)			;ED 50
	in	e,(c)			;ED 58
	in	h,(c)			;ED 60
	in	l,(c)			;ED 68
	;***********************************************************
	;  p. 5-55
	.z280
	in	ixh,(c)			;DD ED 60
	in	ixl,(c)			;DD ED 68
	in	iyh,(c)			;FD ED 60
	in	iyl,(c)			;FD ED 68
	in	(daddr),(c)		;DD ED 78 44 33
	in	lxoff(ix),(c)		;FD ED 48 22 11
	in	lxoff(iy),(c)		;FD ED 50 22 11
	in	lxoff(hl),(c)		;FD ED 58 22 11
	in	[.+offset],(c)		;FD ED 40 50 00
	in	(sp+lxoff),(c)		;DD ED 40 22 11
	in	(ix+hl),(c)		;DD ED 48
	in	(hl+iy),(c)		;DD ED 50
	in	(iy+ix),(c)		;DD ED 58
	;***********************************************************
	;  p. 5-65
	in	hl,(c)			;ED B7
	inw	hl,(c)			;ED B7
	.z80
	;***********************************************************
	; increment operand
	inc	(hl)			;34
	inc	offset(ix)		;DD 34 55
	inc	offset(iy)		;FD 34 55
	inc	(ix+offset)		;DD 34 55
	inc	(iy+offset)		;FD 34 55
	inc	a			;3C
	inc	b			;04
	inc	bc			;03
	incw	bc			;03
	inc	c			;0C
	inc	d			;14
	inc	de			;13
	incw	de			;13
	inc	e			;1C
	inc	h			;24
	inc	hl			;23
	incw	hl			;23
	inc	ix			;DD 23
	incw	ix			;DD 23
	inc	iy			;FD 23
	incw	iy			;FD 23
	inc	l			;2C
	inc	sp			;33
	incw	sp			;33
	;  p. 5-57
	.z280
	inc	ixh			;DD 24
	inc	ixl			;DD 2C
	inc	iyh			;FD 24
	inc 	iyl			;FD 2C
	inc	(daddr)			;DD 3C 44 33
	inc	(ix+lxoff)		;FD 0C 22 11
	inc	(iy+lxoff)		;FD 14 22 11
	inc	(hl+offset)		;FD 1C 55 00
	inc	[.+offset]		;FD 04 51 00
	inc	lxoff(sp)		;DD 04 22 11
	inc	(hl+ix)			;DD 0C
	inc	(hl+iy)			;DD 14
	inc	(ix+iy)			;DD 1C
	incw	(hl)			;DD 03
	incw	(daddr)			;DD 13 44 33
	incw	(ix+offset)		;FD 03 55 00
	incw	(iy+lxoff)		;FD 13 22 11
	incw	[.+raoff]		;DD 33 30 12
	.z80
	;***********************************************************
	; load location (hl) with input
	; from port (c)
	; decrement 'hl' and 'b'
	ind				;ED AA
	;***********************************************************
	; load location (hl) with input
	; from port (c)
	; decrement 'hl' and 'b'
	; repeat until 'b' = 0
	indr				;ED BA
	;***********************************************************
	; load location (hl) with input
	; from port (c)
	; increment 'hl' and decrement 'b'
	ini				;ED A2
	;***********************************************************
	; load location (hl) with input
	; from port (c)
	; increment 'hl' and decrement 'b'
	; repeat until 'b' = 0
	inir				;ED B2
	;***********************************************************
	.z280
jmp1:	indw				;ED 8A
	indrw				;ED 9A
	iniw				;ED 82
	inirw				;ED 92
	;***********************************************************
 	; jump on auxiliary accumulator/flag (AF)
	;  p. 5-66
	jaf	jmp1			;DD 28 F5
	;***********************************************************
	; jump on auxiliary register set
	;  p. 5-67
	jar	jmp1			;DD 20 F2
	.z80
	;***********************************************************
	; unconditional jump to location nn
	jp	nn			;C3 84 05
	jp	(hl)			;E9
	jp	(ix)			;DD E9
	jp	(iy)			;FD E9
	;***********************************************************
	; jump to location if condition is true
	jp	C,nn			;DA 84 05
	jp	M,nn			;FA 84 05
	jp	NC,nn			;D2 84 05
	jp	NZ,nn			;C2 84 05
	jp	P,nn			;F2 84 05
	jp	PE,nn			;EA 84 05
	jp	PO,nn			;E2 84 05
	jp	Z,nn			;CA 84 05
	;***********************************************************
	;  p. 5-68
	.z280
	jp	nz,(hl)			;DD C2
	jp	z,(hl)			;DD CA
	jp	nc,(hl)			;DD D2
	jp	c,(hl)			;DD DA
	jp	nv,(hl)			;DD E2
	jp	v,(hl)			;DD EA
	jp	ns,(hl)			;DD F2
	jp	s,(hl)			;DD FA
	jp	po,(hl)			;DD E2
	jp	pe,(hl)			;DD EA
	jp	p,(hl)			;DD F2
	jp	m,(hl)			;DD FA
	; long relative jumps
	jp	[jmp1]			;FD C3 9E FF
	jp	nz,[jmp1]		;FD C2 9A FF
	jp	z,[jmp1]		;FD CA 96 FF
	jp	p,[jmp1]		;FD F2 92 FF
	jp	m,[jmp1]		;FD FA 8E FF
	.z80
	;***********************************************************
	; unconditional jump relative to PC+e
	jr	jr1+0x10		;18 10
	;***********************************************************
	; jump relative to PC+e if condition is true
jr1:	jr	C,jr2+0x10		;38 10
jr2:	jr	NC,jr3+0x10		;30 10
jr3:	jr	NZ,jr4+0x10		;20 10
jr4:	jr	Z,jr5+0x10		;28 10
jr5:	
	jr	jr1			;18 F6
	;***********************************************************
	; load source to destination
	ld	a,(hl)			;7E
	ld	a,offset(ix)		;DD 7E 55
	ld	a,offset(iy)		;FD 7E 55
	ld	a,(ix+offset)		;DD 7E 55
	ld	a,(iy+offset)		;FD 7E 55
	ld	a,a			;7F
	ld	a,b			;78
	ld	a,c			;79
	ld	a,d			;7A
	ld	a,e			;7B
	ld	a,h			;7C
	ld	a,l			;7D
	ld	a,#n			;3E 20
	ld	a, n			;3E 20
	ld	b,(hl)			;46
	ld	b,offset(ix)		;DD 46 55
	ld	b,offset(iy)		;FD 46 55
	ld	b,(ix+offset)		;DD 46 55
	ld	b,(iy+offset)		;FD 46 55
	ld	b,a			;47
	ld	b,b			;40
	ld	b,c			;41
	ld	b,d			;42
	ld	b,e			;43
	ld	b,h			;44
	ld	b,l			;45
	ld	b,#n			;06 20
	ld	b, n			;06 20
	ld	c,(hl)			;4E
	ld	c,offset(ix)		;DD 4E 55
	ld	c,offset(iy)		;FD 4E 55
	ld	c,(ix+offset)		;DD 4E 55
	ld	c,(iy+offset)		;FD 4E 55
	ld	c,a			;4F
	ld	c,b			;48
	ld	c,c			;49
	ld	c,d			;4A
	ld	c,e			;4B
	ld	c,h			;4C
	ld	c,l			;4D
	ld	c,#n			;0E 20
	ld	c, n			;0E 20
	ld	d,(hl)			;56
	ld	d,offset(ix)		;DD 56 55
	ld	d,offset(iy)		;FD 56 55
	ld	d,(ix+offset)		;DD 56 55
	ld	d,(iy+offset)		;FD 56 55
	ld	d,a			;57
	ld	d,b			;50
	ld	d,c			;51
	ld	d,d			;52
	ld	d,e			;53
	ld	d,h			;54
	ld	d,l			;55
	ld	d,#n			;16 20
	ld	d, n			;16 20
	ld	e,(hl)			;5E
	ld	e,offset(ix)		;DD 5E 55
	ld	e,offset(iy)		;FD 5E 55
	ld	e,(ix+offset)		;DD 5E 55
	ld	e,(iy+offset)		;FD 5E 55
	ld	e,a			;5F
	ld	e,b			;58
	ld	e,c			;59
	ld	e,d			;5A
	ld	e,e			;5B
	ld	e,h			;5C
	ld	e,l			;5D
	ld	e,#n			;1E 20
	ld	e, n			;1E 20
	ld	h,(hl)			;66
	ld	h,offset(ix)		;DD 66 55
	ld	h,offset(iy)		;FD 66 55
	ld	h,(ix+offset)		;DD 66 55
	ld	h,(iy+offset)		;FD 66 55
	ld	h,a			;67
	ld	h,b			;60
	ld	h,c			;61
	ld	h,d			;62
	ld	h,e			;63
	ld	h,h			;64
	ld	h,l			;65
	ld	h,#n			;26 20
	ld	h, n			;26 20
	ld	l,(hl)			;6E
	ld	l,offset(ix)		;DD 6E 55
	ld	l,offset(iy)		;FD 6E 55
	ld	l,(ix+offset)		;DD 6E 55
	ld	l,(iy+offset)		;FD 6E 55
	ld	l,a			;6F
	ld	l,b			;68
	ld	l,c			;69
	ld	l,d			;6A
	ld	l,e			;6B
	ld	l,h			;6C
	ld	l,l			;6D
	ld	l,#n			;2E 20
	ld	l, n			;2E 20
	;***********************************************************
	ld	i,a			;ED 47
	ld	r,a			;ED 4F
	ld	a,i			;ED 57
	ld	a,r			;ED 5F
	;***********************************************************
	ld	(bc),a			;02
	ld	(de),a			;12
	ld	a,(bc)			;0A
	ld	a,(de)			;1A
	;***********************************************************
	ld	(hl),a			;77
	ld	(hl),b			;70
	ld	(hl),c			;71
	ld	(hl),d			;72
	ld	(hl),e			;73
	ld	(hl),h			;74
	ld	(hl),l			;75
	ld	(hl),#n			;36 20
	ld	(hl), n			;36 20
	;***********************************************************
	ld	offset(ix),a		;DD 77 55
	ld	offset(ix),b		;DD 70 55
	ld	offset(ix),c		;DD 71 55
	ld	offset(ix),d		;DD 72 55
	ld	offset(ix),e		;DD 73 55
	ld	offset(ix),h		;DD 74 55
	ld	offset(ix),l		;DD 75 55
	ld	offset(ix),#n		;DD 36 55 20
	ld	offset(ix), n		;DD 36 55 20
	;***********************************************************
	ld	(ix+offset),a		;DD 77 55
	ld	(ix+offset),b		;DD 70 55
	ld	(ix+offset),c		;DD 71 55
	ld	(ix+offset),d		;DD 72 55
	ld	(ix+offset),e		;DD 73 55
	ld	(ix+offset),h		;DD 74 55
	ld	(ix+offset),l		;DD 75 55
	ld	(ix+offset),#n		;DD 36 55 20
	ld	(ix+offset), n		;DD 36 55 20
	;***********************************************************
	ld	offset(iy),a		;FD 77 55
	ld	offset(iy),b		;FD 70 55
	ld	offset(iy),c		;FD 71 55
	ld	offset(iy),d		;FD 72 55
	ld	offset(iy),e		;FD 73 55
	ld	offset(iy),h		;FD 74 55
	ld	offset(iy),l		;FD 75 55
	ld	offset(iy),#n		;FD 36 55 20
	ld	offset(iy), n		;FD 36 55 20
	;***********************************************************
	ld	(iy+offset),a		;FD 77 55
	ld	(iy+offset),b		;FD 70 55
	ld	(iy+offset),c		;FD 71 55
	ld	(iy+offset),d		;FD 72 55
	ld	(iy+offset),e		;FD 73 55
	ld	(iy+offset),h		;FD 74 55
	ld	(iy+offset),l		;FD 75 55
	ld	(iy+offset),#n		;FD 36 55 20
	ld	(iy+offset), n		;FD 36 55 20
	;***********************************************************
	ld	(nn),a			;32 84 05
	ld	(nn),bc			;ED 43 84 05
	ld	(nn),de			;ED 53 84 05
	ld	(nn),hl			;22 84 05
	ld	(nn),sp			;ED 73 84 05
	ld	(nn),ix			;DD 22 84 05
	ld	(nn),iy			;FD 22 84 05
	;***********************************************************
	ld	a,(nn)			;3A 84 05
	ld	bc,(nn)			;ED 4B 84 05
	ld	de,(nn)			;ED 5B 84 05
	ld	hl,(nn)			;2A 84 05
	ld	sp,(nn)			;ED 7B 84 05
	ld	ix,(nn)			;DD 2A 84 05
	ld	iy,(nn)			;FD 2A 84 05
	;***********************************************************
	ld	bc,#nn			;01 84 05
	ld	bc, nn			;01 84 05
	ld	de,#nn			;11 84 05
	ld	de, nn			;11 84 05
	ld	hl,#nn			;21 84 05
	ld	hl, nn			;21 84 05
	ld	sp,#nn			;31 84 05
	ld	sp, nn			;31 84 05
	ld	ix,#nn			;DD 21 84 05
	ld	ix, nn			;DD 21 84 05
	ld	iy,#nn			;FD 21 84 05
	ld	iy, nn			;FD 21 84 05
	;***********************************************************
	ld	sp,hl			;F9
	ld	sp,ix			;DD F9
	ld	sp,iy			;FD F9
	;***********************************************************
	; p. 5-70
	.z280
	ld	a,ixh			;DD 7C
	ld	a,ixl			;DD 7D
	ld	a,iyh			;FD 7C
	ld	a,iyl			;FD 7D
	ld	a,(ix+lxoff)		;FD 79 22 11
	ld	a,(iy+lxoff)		;FD 7A 22 11
	ld	a,(hl+lxoff)		;FD 7B 22 11
	ld	a,[.+lxoff]		;FD 78 1E 11
	ld	a,(sp)			;DD 78 00 00
	ld	a,(sp+lxoff)		;DD 78 22 11
	ld	a,(hl+ix)		;DD 79
	ld	a,(hl+iy)		;DD 7A
	ld	a,(ix+iy)		;DD 7B
	;***********************************************************
	; p. 5-71
	ld	ixh,a			;DD 67
	ld	ixl,a			;DD 6F
	ld	iyh,a			;FD 67
	ld	iyl,a			;FD 6F
	ld	(ix+lxoff),a		;ED 2B 22 11
	ld	(iy+lxoff),a		;ED 33 22 11
	ld	(hl+lxoff),a		;ED 3B 22 11
	ld	[.-32],a		;ED 23 DC FF
	ld	(sp),a			;ED 03 00 00
	ld	(sp+6),a		;ED 03 06 00
	ld	(hl+ix),a		;ED 0B
	ld	(hl+iy),a		;ED 13
	ld	(iy+ix),a		;ED 1B
	;***********************************************************
	; p. 5-73
	ld	d,#n			;16 20
	ld	ixh,#n			;DD 26 20
	ld	ixl,#n			;DD 2E 20
	ld	iyh,#n			;FD 26 20
	ld	iyl,#n			;FD 2E 20
	ldb	(daddr),#n		;DD 3E 44 33 20
	ld	(daddr),#n		;DD 3E 44 33 20
	ldb	(ix+lxoff),#n		;FD 0E 20
	ld	(ix+lxoff),#n		;FD 0E 20
	ldb	(iy+lxoff),#n		;FD 16 20
	ld	(iy+lxoff),#n		;FD 16 20
	ldb	(hl+lxoff),#n		;FD 1E 20
	ld	(hl+lxoff),#n		;FD 1E 20
	ldb	[.+32],#n		;FD 06 1B 00 20
	ld	[.+32],#n		;FD 06 1B 00 20
	ldb	(sp),#n			;DD 06 00 00 20
	ld	(sp+lxoff),#n		;DD 06 22 11 20
	ldb	(hl+ix),#n		;DD 0E 20
	ld	(hl+iy),#n		;DD 16 20
	ldb	(ix+iy),#n		;DD 1E 20
	;***********************************************************
	; p. 5-74
	ld	a,ixh			;DD 7C
	ld	a,iyl			;FD 7D
	ld	b,ixh			;DD 44
	ld	b,ixl			;DD 45
	LD	C,IXH			;DD 4C
	LD	C,IXL			;DD 4D
	ld	d,iyh			;FD 54
	ld	d,iyl			;FD 55
	ld	e,iyh			;FD 5C
	ld	e,iyl			;FD 5D
	ld	ixh,ixh			;DD 64
	ld	ixh,ixl			;DD 65
	ld	ixl,ixh			;DD 6C
	ld	ixl,ixl			;DD 6D
	ld	iyh,iyh			;FD 64
	ld	iyh,iyl			;FD 65
	ld	iyl,iyh			;FD 6C
	ld	iyl,iyl			;FD 6D
	ld	h,d			;62
	ld	l,e			;6B
	ld	ixh,a			;DD 67
	ld	ixl,a			;DD 6F
	ld	iyh,a			;FD 67
	ld	iyl,a			;FD 6F
	ld	ixh,b			;DD 60
	ld	ixl,c			;DD 69
	ld	iyh,d			;FD 62
	ld	iyl,e			;FD 6B
	ld	ixh,#n			;DD 26 20
	ld	ixl,#n+3		;DD 2E 23
	ld	iyh,#n-1		;FD 26 1F
	ld	iyl,#n			;FD 2E 20
	;***********************************************************
	; p. 5-76
	lda	hl,(daddr)		;21 44 33
	lda	ix,(daddr)		;DD 21 44 33
	lda	iy,(daddr)		;FD 21 44 33
	lda	hl,(ix+sxoff)		;ED 2A 55 00
	lda	hl,(ix+lxoff)		;ED 2A 22 11
	lda	hl,(iy+sxoff)		;ED 32 55 00
	lda	hl,(iy+lxoff)		;ED 32 22 11
	lda	ix,(ix+sxoff)		;DD ED 2A 55 00
	lda	ix,(ix+lxoff)		;DD ED 2A 22 11
	lda	ix,(IY+sxoff)		;DD ED 32 55 00
	lda	ix,(iy+lxoff)		;DD ED 32 22 11
	lda	iy,(ix+sxoff)		;FD ED 2A 55 00
	lda	iy,(ix+lxoff)		;FD ED 2A 22 11
	lda	iy,(IY+sxoff)		;FD ED 32 55 00
	lda	iy,(iy+lxoff)		;FD ED 32 22 11
	LDA	IX,(IY)			;dd ed 32 00 00
	lda	hl,[.]			;ED 22 FC FF
	lda	ix,[.+n]		;DD ED 22 1B 00
	lda	iy,.+n(pc)		;FD ED 22 1B 00
	lda	hl,(sp)			;ED 02 00 00
	lda	ix,(sp+sxoff)		;DD ED 02 55 00
	lda	iy,(sp+lxoff)		;FD ED 02 22 11
	lda	hl,(ix+hl)		;ED 0A
	lda	hl,(hl+iy)		;ED 12
	lda	hl,(ix+iy)		;ED 1A
	lda	ix,(hl+ix)		;DD ED 0A
	lda	ix,(hl+iy)		;DD ED 12
	lda	ix,(ix+iy)		;DD ED 1A
	lda	iy,(hl+ix)		;FD ED 0A
	lda	iy,(hl+iy)		;FD ED 12
	lda	iy,(ix+iy)		;FD ED 1A
	;***********************************************************
	;  p. 5-77
	.z280p
	ldctl	hl,(c)			;ED 66
	ldctl	ix,(c)			;DD ED 66
	ldctl	iy,(c)			;FD ED 66
	ldctl	(c),hl			;ED 6E
	ldctl	(c),ix			;DD ED 6E
	ldctl	(c),iy			;FD ED 6E
	ldctl	hl,usp			;ED 87
	ldctl	ix,usp			;DD ED 87
	ldctl	iy,usp			;FD ED 87
	ldctl	usp,hl			;ED 8F
	ldctl	usp,ix			;DD ED 8F
	ldctl	usp,iy			;FD ED 8F
	;***********************************************************
	;  p. 5-84
	ldud	a,(hl)			;ED 86
	ldud	a,sxoff(ix)		;DD ED 86 55
	ldud	a,(iy+sxoff)		;FD ED 86 55
	ldud	(hl),a			;ED 8E
	ldud	sxoff(ix),a		;DD ED 8E 55
	ldud	(iy),a			;FD ED 8E 00
	;***********************************************************
	;  p. 5-86
	ldup	a,(hl)			;ED 96
	ldup	a,sxoff(ix)		;DD ED 96 55
	ldup	a,(iy+sxoff)		;FD ED 96 55
	ldup	(hl),a			;ED 9E
	ldup	sxoff(ix),a		;DD ED 9E 55
	ldup	(iy),a			;FD ED 9E 00
	;***********************************************************
	;  p. 5-88
	.z280
	ldw	bc,#nn			;01 84 05
	ldw	de,#nn			;11 84 05
	ldw	hl,#nn			;21 84 05
	ldw	sp,#nn			;31 84 05
	ldw	ix,#nn			;DD 21 84 05
	ldw	iy,#nn			;FD 21 84 05
	ldw	(hl),#nn		;DD 01 84 05
	ldw	(daddr),#nn		;DD 11 44 33 84 05
	ldw	[.+6],#nn		;DD 31 00 00 84 05
	;***********************************************************
	;  p. 5-89     two immediates handled above
	ldw	hl,(daddr)		;2A 44 33
	ldw	ix,(daddr)		;DD 2A 44 33
	ldw	iy,(daddr)		;FD 2A 44 33
	ldw	hl,(ix+lxoff)		;ED 2C 22 11
	ldw	hl,(iy+lxoff)		;ED 34 22 11
	ldw	hl,(hl+lxoff)		;ED 3C 22 11
	ldw	ix,(ix+lxoff)		;DD ED 2C 22 11
	ldw	ix,(iy+lxoff)		;DD ED 34 22 11
	ldw	ix,(hl+lxoff)		;DD ED 3C 22 11
	ldw	iy,(ix+lxoff)		;FD ED 2C 22 11
	ldw	iy,(iy+lxoff)		;FD ED 34 22 11
	ldw	iy,(hl+lxoff)		;FD ED 3C 22 11
	ldw	hl,[.+32]		;ED 24 1C 00
	ldw	ix,[.+32]		;DD ED 24 1B 00
	ldw	iy,[.+32]		;FD ED 24 1B 00
	ldw	hl,(sp+lxoff)		;ED 04 22 11
	ldw	ix,(sp)			;DD ED 04 00 00
	ldw	iy,(sp+nn)		;FD ED 04 84 05
	ldw	hl,(hl+ix)		;ED 0C
	ldw	hl,(hl+iy)		;ED 14
	ldw	hl,(ix+iy)		;ED 1C
	ldw	ix,(hl+ix)		;DD ED 0C
	ldw	ix,(hl+iy)		;DD ED 14
	ldw	ix,(ix+iy)		;DD ED 1C
	ldw	iy,(hl+ix)		;FD ED 0C
	ldw	iy,(hl+iy)		;FD ED 14
	ldw	iy,(ix+iy)		;FD ED 1C
	;***********************************************************
	;  p. 5-90
	ldw	(daddr),hl		;22 44 33
	ldw	lxoff(hl),hl		;ED 3D 22 11
	ldw	lxoff(ix),hl		;ED 2D 22 11
	ldw	lxoff(iy),hl		;ED 35 22 11
	ldw	lxoff(hl),ix		;DD ED 3D 22 11
	ldw	lxoff(ix),ix		;DD ED 2D 22 11
	ldw	lxoff(iy),ix		;DD ED 35 22 11
	ldw	lxoff(hl),iy		;FD ED 3D 22 11
	ldw	lxoff(ix),iy		;FD ED 2D 22 11
	ldw	lxoff(iy),iy		;FD ED 35 22 11
	ldw	.+sxoff(pc),hl		;ED 25 51 00
	ldw	[.+sxoff],ix		;DD ED 25 50 00
	ldw	[.+32],iy		;FD ED 25 1B 00
	ldw	(sp+lxoff),hl		;ED 05 22 11
	ldw	(sp+lxoff),ix		;DD ED 05 22 11
	ldw	(sp),iy			;FD ed 05 00 00
	ldw	(hl+ix),iy		;FD ED 0D
	ldw	(hl+iy),iy		;FD ED 15
	LDW	(IX+IY),iy		;FD ED 1D
	ldw	(hl+ix),ix		;DD ED 0D
	ldw	(hl+iy),ix		;DD ED 15
	LDW	(IX+IY),ix		;DD ED 1D
	ldw	(hl+ix),hl		;ED 0D
	ldw	(hl+iy),hl		;ED 15
	LDW	(IX+IY),hl		;ED 1D
	;***********************************************************
	;  p. 5-91
	ldw	bc,#nn			;01 84 05
	ldw	de,#nn			;11 84 05
	ldw	hl,#nn			;21 84 05
	ldw	sp,#nn			;31 84 05
	ldw	bc,(hl)			;ED 06
	ldw	de,(hl)			;ED 16
	ldw	hl,(hl)			;ED 26
	ldw	sp,(hl)			;ED 36
	ldw	bc,(daddr)		;ED 4B 44 33
	ldw	de,(daddr)		;ED 5B 44 33
	ldw	hl,(daddr)		;2A 44 33
	ldw	sp,(daddr)		;ED 7B 44 33
	ldw	bc,sxoff(ix)		;DD ED 06 55
	ldw	bc,sxoff(iy)		;FD ED 06 55
	ldw	de,sxoff(ix)		;DD ED 16 55
	ldw	de,sxoff(iy)		;FD ED 16 55
	ldw	hl,sxoff(ix)		;DD ED 26 55
	ldw	hl,sxoff(iy)		;FD ED 26 55
	ldw	sp,sxoff(ix)		;DD ED 36 55
	ldw	sp,sxoff(iy)		;FD ED 36 55
	ldw	(hl),bc			;ED 0E
	ldw	(hl),de			;ED 1E
	ldw	(hl),hl			;ED 2E
	ldw	(hl),sp			;ED 3E
	ldw	(daddr),bc		;ED 43 44 33
	ldw	(daddr),de		;ED 53 44 33
	ldw	(daddr),sp		;ED 73 44 33
	LDW	(daddr),HL		;22 44 33
	ldw	(ix+sxoff),bc		;DD ED 0E 55
	ldw	sxoff(iy),bc		;FD ED 0E 55
	ldw	(iy),de			;FD ED 1E 00
	ldw	(ix+sxoff),de		;DD ED 1E 55
	ldw	sxoff(ix),hl		;DD ED 2E 55
	ldw	sxoff(iy),hl		;FD ED 2E 55
	ldw	(ix),sp			;DD ED 3E 00
	ldw	(iy+sxoff),sp		;FD ED 3E 55
	;***********************************************************
	;  p. 5-92
	ldw	sp,hl			;F9
	ldw	sp,ix			;DD F9
	ldw	sp,iy			;FD F9
	ld	sp,hl			;F9
	ld	sp,ix			;DD F9
	ld	sp,iy			;FD F9
	ldw	sp,#nn			;31 84 05
	ld	sp,#nn			;31 84 05
	ldw	sp,(hl)			;ED 36
	ld	sp,(hl)			;ED 36
	ldw	sp,(daddr)		;ED 7B 44 33
	ld	sp,(daddr)		;ED 7B 44 33
	ldw	sp,(ix+sxoff)		;DD ED 36 55
	ld	sp,(ix+sxoff)		;DD ED 36 55
	ldw	sp,(iy+sxoff)		;FD ED 36 55
	ld	sp,(iy+sxoff)		;FD ED 36 55
	ldw	(hl),sp			;ED 3E
	ld	(hl),sp			;ED 3E
	ldw	(daddr),sp		;ED 73 44 33
	ld	(daddr),sp		;ED 73 44 33
	ldw	(ix+sxoff),sp		;DD ED 3E 55
	ld	(iy+sxoff),sp		;FD ed 3E 55
	.z80
	;***********************************************************
	; load location (hl)
	; with location (de)
	; decrement de, hl
	; decrement bc
	ldd				;ED A8
	;***********************************************************
	; load location (hl)
	; with location (de)
	; decrement de, hl
	; decrement bc
	; repeat until bc = 0
	lddr				;ED B8
	;***********************************************************
	; load location (hl)
	; with location (de)
	; increment de, hl
	; decrement bc
	ldi				;ED A0
	;***********************************************************
	; load location (hl)
	; with location (de)
	; increment de, hl
	; decrement bc
	; repeat until bc = 0
	ldir				;ED B0
	;***********************************************************
	; multiplication
	;  p. 5-93
	.z280
	mult	a,b			;ED C0
	mult	a,c			;ED C8
	mult	a,d			;ED D0
	mult	a,e			;ED D8
	mult	a,h			;ED E0
	mult	a,l			;ED E8
	mult	a,(hl)			;ED F0
	mult	a,a			;ED F8
	mult	a,ixh			;DD ED E0
	mult	a,ixl			;DD ED E8
	mult	a,iyh			;FD ED E0
	mult	a,iyl			;FD ED E8
	mult	a,#n			;FD ED F8 20
	mult	a,(daddr)		;DD ED F8 44 33
	mult	a,offset(ix)		;DD ED F0 55
	mult	a,offset(iy)		;FD ED F0 55
	mult	a,(ix)			;DD ED F0 00
	mult	a,(iy+n)		;FD ED F0 20
	mult	a,(ix+lxoff)		;FD ED C8 22 11
	mult	a,(iy+lxoff)		;FD ED D0 22 11
	mult	a,(hl+n)		;FD ED D8 20 00
	mult	a,[.-5]			;FD ED C0 F6 FF
	mult	a,lxoff(sp)		;DD ED C0 22 11
	mult	a,(hl+ix)		;DD ED C8
	mult	a,(hl+iy)		;DD ED D0
	mult	a,(ix+iy)		;DD ED D8
	multu	a,b			;ED C1
	multu	a,c			;ED C9
	multu	a,d			;ED D1
	multu	a,e			;ED D9
	multu	a,h			;ED E1
	multu	a,l			;ED E9
	multu	a,(hl)			;ED F1
	multu	a,a			;ED F9
	multu	a,ixh			;DD ED E1
	multu	a,ixl			;DD ED E9
	multu	a,iyh			;FD ED E1
	multu	a,iyl			;FD ED E9
	multu	a,#n			;FD ED F9 20
	multu	a,(daddr)		;DD ED F9 44 33
	multu	a,(ix+lxoff)		;FD ED C9 22 11
	multu	a,(iy+lxoff)		;FD ED D1 22 11
	multu	a,(hl+lxoff)		;FD ED D9 22 11
	multu	a,(ix)			;DD ED F1 00
	multu	a,offset(iy)		;FD ED F1 55
	multu	a,[.-5]			;FD ED C1 F6 FF
	multu	a,(sp)			;DD ED C1 00 00
	multu	a,lxoff(sp)		;DD ED C1 22 11
	multu	a,(hl+ix)		;DD ED C9
	multu	a,(hl+iy)		;DD ED D1
	multu	a,(ix+iy)		;DD ED D9
	multuw	hl,bc			;ED C3
	multuw	hl,de			;ED D3
	multuw	hl,hl			;ED E3
	multuw	hl,sp			;ED F3
	multuw	hl,ix			;DD ED E3
	multuw	hl,iy			;FD ED E3
	multuw	hl,#nn			;FD ED F3 84 05
	multuw	hl,(hl)			;DD ED C3
	multuw	hl,(ix+lxoff)		;FD ED C3 22 11
	multuw	hl,lxoff(iy)		;FD ED D3 22 11
	multuw	hl,(iy)			;FD ED D3 00 00
	multuw	hl,[.+nn]		;DD ED F3 7F 05
	multw	hl,(hl)			;DD ED C2
	multw	hl,lxoff(ix)		;FD ED C2 22 11
	multw	hl,lxoff(iy)		;FD ED D2 22 11
	multw	hl,nn+.(pc)		;DD ED F2 7F 05
	multw	hl,[nn+.]		;DD ED F2 7f 05
	multw	hl,(daddr)		;DD ED D2 44 33
	multw	hl,bc			;ED C2
	multw	hl,de			;ED D2
	multw	hl,hl			;ED E2
	multw	hl,sp			;ED F2
	multw	hl,ix			;DD ED E2
	multw	hl,iy			;FD ED E2
	multw	hl,#nn			;FD ED F2 84 05
	.z80
	;***********************************************************
	; 2's complement of 'a'
	neg				;ED 44
	neg	a			;ED 44
	.z280
	neg	hl			;ED 4C
	.z80
	;***********************************************************
	; no operation
	nop				;00
	;***********************************************************
	; logical 'or' operand with 'a'
	or	a,(hl)			;B6
	or	a,offset(ix)		;DD B6 55
	or	a,offset(iy)		;FD B6 55
	or	a,(ix+offset)		;DD B6 55
	or	a,(iy+offset)		;FD B6 55
	or	a,a			;B7
	or	a,b			;B0
	or	a,c			;B1
	or	a,d			;B2
	or	a,e			;B3
	or	a,h			;B4
	or	a,l			;B5
	or	a,#n			;F6 20
	or	a, n			;F6 20
	;***********************************************************
	;  p. 5-100
	.z280
	or	a,(hl+ix)		;DD B1
	or	a,(hl+iy)		;DD B2
	or	a,(hl+lxoff)		;FD B3 22 11
	or	a,(ix+iy)		;DD B3
	or	a,lxoff(iy)		;FD B2 22 11
	or	a,[.+lxoff]		;FD B0 1E 11
	or	a,lxoff(sp)		;DD B0 22 11
	or	a,(daddr)		;DD B7 44 33
	or	a,ixh			;DD B4
	or	a,ixl			;DD B5
	or	a,iyh			;FD B4
	or	a,iyl			;FD B5
	.z80
	;***********************************************************
	; load output port (c)
	; with location (hl)
	; decrement hl and decrement b
	; repeat until b = 0
	otdr				;ED BB
	;***********************************************************
	; load output port (c)
	; with location (hl)
	; increment hl and decrement b
	; repeat until b = 0
	otir				;ED B3
	;***********************************************************
	; load output port (c) with reg
	out	(c),a			;ED 79
	out	(c),b			;ED 41
	out	(c),c			;ED 49
	out	(c),d			;ED 51
	out	(c),e			;ED 59
	out	(c),h			;ED 61
	out	(c),l			;ED 69
	;***********************************************************
	;  p. 5-105
	.z280
	out	(c),(hl+ix)		;DD ED 49
	out	(c),(hl+iy)		;DD ED 51
	out	(c),(hl+lxoff)		;FD ED 59 22 11
	out	(c),(ix+iy)		;DD ED 59
	out	(c),(ix+lxoff)		;FD ED 49 22 11
	out	(c),(iy+lxoff)		;FD ED 51 22 11
	out	(c),[.+offset]		;FD ED 41 50 00
	out	(c),offset(sp)		;DD ED 41 55 00
	out	(c),(daddr)		;DD ED 79 44 33
	.z80
	;***********************************************************
	; load output port (n) with 'a'
	out	(n),a			;D3 20
	out	n,a			;D3 20
	;***********************************************************
	; load output port (c)
	; with location (hl)
	; decrement hl and decrement b
	outd				;ED AB
	;***********************************************************
	; load output port (c)
	; with location (hl)
	; increment hl and decrement b
	outi				;ED A3
	;***********************************************************
	.z280
	outdw				;ED 8B
	outiw				;ED 83
	otdrw				;ED 9B
	otirw				;ED 93
	;***********************************************************
	;  p. 5-110	output to word port
	out	(c),hl			;ED BF
	outw	(c),hl			;ED BF
	;***********************************************************
	;  p. 5-111    NOT privileged
	pcache				;ED 65
	.z80
	;***********************************************************
	; load destination with top of stack
	pop	af			;F1
	pop	bc			;C1
	pop	de			;D1
	pop	hl			;E1
	pop	ix			;DD E1
	pop	iy			;FD E1
	.z280
	pop	(hl)			;DD C1
	pop	[.+lxoff]		;DD F1 1E 11
	pop	(daddr)			;DD D1 44 33
	.z80
	;***********************************************************
	; put source on stack
	push	af			;F5
	push	bc			;C5
	push	de			;D5
	push	hl			;E5
	push	ix			;DD E5
	push	iy			;FD E5
	.z280
	push	(hl)			;DD C5
	push	[.+lxoff]		;DD F5 1E 11
	push	(daddr)			;DD D5 44 33
	push	#nn			;FD F5 84 05
	.z80
	;***********************************************************
	; reset bit of location or register
	res	0,(hl)			;CB 86
	res	0,offset(ix)		;DD CB 55 86
	res	0,offset(iy)		;FD CB 55 86
	res	0,(ix+offset)		;DD CB 55 86
	res	0,(iy+offset)		;FD CB 55 86
	res	0,a			;CB 87
	res	0,b			;CB 80
	res	0,c			;CB 81
	res	0,d			;CB 82
	res	0,e			;CB 83
	res	0,h			;CB 84
	res	0,l			;CB 85
	res	1,(hl)			;CB 8E
	res	1,offset(ix)		;DD CB 55 8E
	res	1,offset(iy)		;FD CB 55 8E
	res	1,(ix+offset)		;DD CB 55 8E
	res	1,(iy+offset)		;FD CB 55 8E
	res	1,a			;CB 8F
	res	1,b			;CB 88
	res	1,c			;CB 89
	res	1,d			;CB 8A
	res	1,e			;CB 8B
	res	1,h			;CB 8C
	res	1,l			;CB 8D
	res	2,(hl)			;CB 96
	res	2,offset(ix)		;DD CB 55 96
	res	2,offset(iy)		;FD CB 55 96
	res	2,(ix+offset)		;DD CB 55 96
	res	2,(iy+offset)		;FD CB 55 96
	res	2,a			;CB 97
	res	2,b			;CB 90
	res	2,c			;CB 91
	res	2,d			;CB 92
	res	2,e			;CB 93
	res	2,h			;CB 94
	res	2,l			;CB 95
	res	3,(hl)			;CB 9E
	res	3,offset(ix)		;DD CB 55 9E
	res	3,offset(iy)		;FD CB 55 9E
	res	3,(ix+offset)		;DD CB 55 9E
	res	3,(iy+offset)		;FD CB 55 9E
	res	3,a			;CB 9F
	res	3,b			;CB 98
	res	3,c			;CB 99
	res	3,d			;CB 9A
	res	3,e			;CB 9B
	res	3,h			;CB 9C
	res	3,l			;CB 9D
	res	4,(hl)			;CB A6
	res	4,offset(ix)		;DD CB 55 A6
	res	4,offset(iy)		;FD CB 55 A6
	res	4,(ix+offset)		;DD CB 55 A6
	res	4,(iy+offset)		;FD CB 55 A6
	res	4,a			;CB A7
	res	4,b			;CB A0
	res	4,c			;CB A1
	res	4,d			;CB A2
	res	4,e			;CB A3
	res	4,h			;CB A4
	res	4,l			;CB A5
	res	5,(hl)			;CB AE
	res	5,offset(ix)		;DD CB 55 AE
	res	5,offset(iy)		;FD CB 55 AE
	res	5,(ix+offset)		;DD CB 55 AE
	res	5,(iy+offset)		;FD CB 55 AE
	res	5,a			;CB AF
	res	5,b			;CB A8
	res	5,c			;CB A9
	res	5,d			;CB AA
	res	5,e			;CB AB
	res	5,h			;CB AC
	res	5,l			;CB AD
	res	6,(hl)			;CB B6
	res	6,offset(ix)		;DD CB 55 B6
	res	6,offset(iy)		;FD CB 55 B6
	res	6,(ix+offset)		;DD CB 55 B6
	res	6,(iy+offset)		;FD CB 55 B6
	res	6,a			;CB B7
	res	6,b			;CB B0
	res	6,c			;CB B1
	res	6,d			;CB B2
	res	6,e			;CB B3
	res	6,h			;CB B4
	res	6,l			;CB B5
	res	7,(hl)			;CB BE
	res	7,offset(ix)		;DD CB 55 BE
	res	7,offset(iy)		;FD CB 55 BE
	res	7,(ix+offset)		;DD CB 55 BE
	res	7,(iy+offset)		;FD CB 55 BE
	res	7,a			;CB BF
	res	7,b			;CB B8
	res	7,c			;CB B9
	res	7,d			;CB BA
	res	7,e			;CB BB
	res	7,h			;CB BC
	res	7,l			;CB BD
	;***********************************************************
	; return from subroutine
	ret				;C9
	;***********************************************************
	; return from subroutine if condition is true
	ret	C			;D8
	ret	M			;F8
	ret	NC			;D0
	ret	NZ			;C0
	ret	P			;F0
	ret	PE			;E8
	ret	PO			;E0
	ret	Z			;C8
	;***********************************************************
	; return from interrupt (privileged)
	reti				;ED 4D
	;***********************************************************
	; return from non-maskable interrupt (privileged)
	retn				;ED 45
	;***********************************************************
	; return from interrupt (long)
	.z280p
	retil				;ED 55
	.z80
	;***********************************************************
	; rotate left through carry
.if ODDBALL_SHIFT_ROTATE
	rl	a,(hl)			;CB 16
	rl	a,offset(ix)		;DD CB 55 16
	rl	a,offset(iy)		;FD CB 55 16
	rl	a,(ix+offset)		;DD CB 55 16
	rl	a,(iy+offset)		;FD CB 55 16
	rl	a,a			;CB 17
	rl	a,b			;CB 10
	rl	a,c			;CB 11
	rl	a,d			;CB 12
	rl	a,e			;CB 13
	rl	a,h			;CB 14
	rl	a,l			;CB 15
.endif
	rl	(hl)			;CB 16
	rl	offset(ix)		;DD CB 55 16
	rl	offset(iy)		;FD CB 55 16
	rl	(ix+offset)		;DD CB 55 16
	rl	(iy+offset)		;FD CB 55 16
	rl	a			;CB 17
	rl	b			;CB 10
	rl	c			;CB 11
	rl	d			;CB 12
	rl	e			;CB 13
	rl	h			;CB 14
	rl	l			;CB 15
	;***********************************************************
	; rotate left 'a' with carry
	rla				;17
	;***********************************************************
	; rotate left circular
.if ODDBALL_SHIFT_ROTATE
	rlc	a,(hl)			;CB 06
	rlc	a,offset(ix)		;DD CB 55 06
	rlc	a,offset(iy)		;FD CB 55 06
	rlc	a,(ix+offset)		;DD CB 55 06
	rlc	a,(iy+offset)		;FD CB 55 06
	rlc	a,a			;CB 07
	rlc	a,b			;CB 00
	rlc	a,c			;CB 01
	rlc	a,d			;CB 02
	rlc	a,e			;CB 03
	rlc	a,h			;CB 04
	rlc	a,l			;CB 05
.endif
	rlc	(hl)			;CB 06
	rlc	offset(ix)		;DD CB 55 06
	rlc	offset(iy)		;FD CB 55 06
	rlc	(ix+offset)		;DD CB 55 06
	rlc	(iy+offset)		;FD CB 55 06
	rlc	a			;CB 07
	rlc	b			;CB 00
	rlc	c			;CB 01
	rlc	d			;CB 02
	rlc	e			;CB 03
	rlc	h			;CB 04
	rlc	l			;CB 05
	;***********************************************************
	; rotate left 'a' circular
	rlca				;07
	;***********************************************************
	; rotate digit left and right
	; between 'a' and location (hl)
	rld				;ED 6F
	;***********************************************************
	; rotate right through carry
.if ODDBALL_SHIFT_ROTATE
	rr	a,(hl)			;CB 1E
	rr	a,offset(ix)		;DD CB 55 1E
	rr	a,offset(iy)		;FD CB 55 1E
	rr	a,(ix+offset)		;DD CB 55 1E
	rr	a,(iy+offset)		;FD CB 55 1E
	rr	a,a			;CB 1F
	rr	a,b			;CB 18
	rr	a,c			;CB 19
	rr	a,d			;CB 1A
	rr	a,e			;CB 1B
	rr	a,h			;CB 1C
	rr	a,l			;CB 1D
.endif
	rr	(hl)			;CB 1E
	rr	offset(ix)		;DD CB 55 1E
	rr	offset(iy)		;FD CB 55 1E
	rr	(ix+offset)		;DD CB 55 1E
	rr	(iy+offset)		;FD CB 55 1E
	rr	a			;CB 1F
	rr	b			;CB 18
	rr	c			;CB 19
	rr	d			;CB 1A
	rr	e			;CB 1B
	rr	h			;CB 1C
	rr	l			;CB 1D
	;***********************************************************
	; rotate 'a' right with carry
	rra				;1F
	;***********************************************************
	; rotate right circular
.if ODDBALL_SHIFT_ROTATE
	rrc	a,(hl)			;CB 0E
	rrc	a,offset(ix)		;DD CB 55 0E
	rrc	a,offset(iy)		;FD CB 55 0E
	rrc	a,(ix+offset)		;DD CB 55 0E
	rrc	a,(iy+offset)		;FD CB 55 0E
	rrc	a,a			;CB 0F
	rrc	a,b			;CB 08
	rrc	a,c			;CB 09
	rrc	a,d			;CB 0A
	rrc	a,e			;CB 0B
	rrc	a,h			;CB 0C
	rrc	a,l			;CB 0D
.endif
	rrc	(hl)			;CB 0E
	rrc	offset(ix)		;DD CB 55 0E
	rrc	offset(iy)		;FD CB 55 0E
	rrc	(ix+offset)		;DD CB 55 0E
	rrc	(iy+offset)		;FD CB 55 0E
	rrc	a			;CB 0F
	rrc	b			;CB 08
	rrc	c			;CB 09
	rrc	d			;CB 0A
	rrc	e			;CB 0B
	rrc	h			;CB 0C
	rrc	l			;CB 0D
	;***********************************************************
	; rotate 'a' right circular
	rrca				;0F
	;***********************************************************
	; rotate digit right and left
	; between 'a' and location (hl)
	rrd				;ED 67
	;***********************************************************
	; restart location
	rst	0x00			;C7
	rst	0x08			;CF
	rst	0x10			;D7
	rst	0x18			;DF
	rst	0x20			;E7
	rst	0x28			;EF
	rst	0x30			;F7
	rst	0x38			;FF
	;***********************************************************
	; subtract with carry to 'a'
	sbc	a,(hl)			;9E
	sbc	a,offset(ix)		;DD 9E 55
	sbc	a,offset(iy)		;FD 9E 55
	sbc	a,(ix+offset)		;DD 9E 55
	sbc	a,(iy+offset)		;FD 9E 55
	sbc	a,a			;9F
	sbc	a,b			;98
	sbc	a,c			;99
	sbc	a,d			;9A
	sbc	a,e			;9B
	sbc	a,h			;9C
	sbc	a,l			;9D
	sbc	a,#n			;DE 20
	sbc	a, n			;DE 20
	;***********************************************************
	; add with carry register pair to 'hl'
	sbc	hl,bc			;ED 42
	sbc	hl,de			;ED 52
	sbc	hl,hl			;ED 62
	sbc	hl,sp			;ED 72
	;***********************************************************
	;  p. 5-130
	.z280
	sbc	a,(hl+ix)		;DD 99
	sbc	a,(hl+iy)		;DD 9A
	sbc	a,(hl+lxoff)		;FD 9B 22 11
	sbc	a,(ix+iy)		;DD 9B
	sbc	a,(ix+lxoff)		;FD 99 22 11
	sbc	a,lxoff(iy)		;FD 9A 22 11
	sbc	a,[.-nn]		;FD 98 78 FA
	sbc	a,(sp)			;DD 98 00 00
	sbc	a,lxoff(sp)		;DD 98 22 11
	sbc	a,(sp+lxoff)		;DD 98 22 11
	sbc	a,(daddr)		;DD 9F 44 33
	sbc	a,ixh			;DD 9C
	sbc	a,ixl			;DD 9D
	sbc	a,iyh			;FD 9C
	sbc	a,iyl			;FD 9D
	;***********************************************************
	;  p. 5-131
	sbc	ix,bc			;DD ED 42
	sbc	ix,de			;DD ED 52
	sbc	ix,ix			;DD ED 62
	sbc	ix,sp			;DD ED 72
	sbc	iy,bc			;FD ED 42
	sbc	iy,de			;FD ED 52
	sbc	iy,iy			;FD ED 62
	sbc	iy,sp			;FD ED 72
	;***********************************************************
	; Z280 system call
	sc	0x4321			;ED 71 21 43
	sc	#0x4321			;ED 71 21 43
	sc	(0x4321)		;ED 71 21 43
	.z80
	;***********************************************************
	; set carry flag (C=1)
	scf				;37
	;***********************************************************
	; set bit of location or register
	set	0,(hl)			;CB C6
	set	0,offset(ix)		;DD CB 55 C6
	set	0,offset(iy)		;FD CB 55 C6
	set	0,(ix+offset)		;DD CB 55 C6
	set	0,(iy+offset)		;FD CB 55 C6
	set	0,a			;CB C7
	set	0,b			;CB C0
	set	0,c			;CB C1
	set	0,d			;CB C2
	set	0,e			;CB C3
	set	0,h			;CB C4
	set	0,l			;CB C5
	set	1,(hl)			;CB CE
	set	1,offset(ix)		;DD CB 55 CE
	set	1,offset(iy)		;FD CB 55 CE
	set	1,(ix+offset)		;DD CB 55 CE
	set	1,(iy+offset)		;FD CB 55 CE
	set	1,a			;CB CF
	set	1,b			;CB C8
	set	1,c			;CB C9
	set	1,d			;CB CA
	set	1,e			;CB CB
	set	1,h			;CB CC
	set	1,l			;CB CD
	set	2,(hl)			;CB D6
	set	2,offset(ix)		;DD CB 55 D6
	set	2,offset(iy)		;FD CB 55 D6
	set	2,(ix+offset)		;DD CB 55 D6
	set	2,(iy+offset)		;FD CB 55 D6
	set	2,a			;CB D7
	set	2,b			;CB D0
	set	2,c			;CB D1
	set	2,d			;CB D2
	set	2,e			;CB D3
	set	2,h			;CB D4
	set	2,l			;CB D5
	set	3,(hl)			;CB DE
	set	3,offset(ix)		;DD CB 55 DE
	set	3,offset(iy)		;FD CB 55 DE
	set	3,(ix+offset)		;DD CB 55 DE
	set	3,(iy+offset)		;FD CB 55 DE
	set	3,a			;CB DF
	set	3,b			;CB D8
	set	3,c			;CB D9
	set	3,d			;CB DA
	set	3,e			;CB DB
	set	3,h			;CB DC
	set	3,l			;CB DD
	set	4,(hl)			;CB E6
	set	4,offset(ix)		;DD CB 55 E6
	set	4,offset(iy)		;FD CB 55 E6
	set	4,(ix+offset)		;DD CB 55 E6
	set	4,(iy+offset)		;FD CB 55 E6
	set	4,a			;CB E7
	set	4,b			;CB E0
	set	4,c			;CB E1
	set	4,d			;CB E2
	set	4,e			;CB E3
	set	4,h			;CB E4
	set	4,l			;CB E5
	set	5,(hl)			;CB EE
	set	5,offset(ix)		;DD CB 55 EE
	set	5,offset(iy)		;FD CB 55 EE
	set	5,(ix+offset)		;DD CB 55 EE
	set	5,(iy+offset)		;FD CB 55 EE
	set	5,a			;CB EF
	set	5,b			;CB E8
	set	5,c			;CB E9
	set	5,d			;CB EA
	set	5,e			;CB EB
	set	5,h			;CB EC
	set	5,l			;CB ED
	set	6,(hl)			;CB F6
	set	6,offset(ix)		;DD CB 55 F6
	set	6,offset(iy)		;FD CB 55 F6
	set	6,(ix+offset)		;DD CB 55 F6
	set	6,(iy+offset)		;FD CB 55 F6
	set	6,a			;CB F7
	set	6,b			;CB F0
	set	6,c			;CB F1
	set	6,d			;CB F2
	set	6,e			;CB F3
	set	6,h			;CB F4
	set	6,l			;CB F5
	set	7,(hl)			;CB FE
	set	7,offset(ix)		;DD CB 55 FE
	set	7,offset(iy)		;FD CB 55 FE
	set	7,(ix+offset)		;DD CB 55 FE
	set	7,(iy+offset)		;FD CB 55 FE
	set	7,a			;CB FF
	set	7,b			;CB F8
	set	7,c			;CB F9
	set	7,d			;CB FA
	set	7,e			;CB FB
	set	7,h			;CB FC
	set	7,l			;CB FD
	;***********************************************************
	; shift operand left arithmetic
.if ODDBALL_SHIFT_ROTATE
	sla	a,(hl)			;CB 26
	sla	a,offset(ix)		;DD CB 55 26
	sla	a,offset(iy)		;FD CB 55 26
	sla	a,(ix+offset)		;DD CB 55 26
	sla	a,(iy+offset)		;FD CB 55 26
	sla	a,a			;CB 27
	sla	a,b			;CB 20
	sla	a,c			;CB 21
	sla	a,d			;CB 22
	sla	a,e			;CB 23
	sla	a,h			;CB 24
	sla	a,l			;CB 25
.endif
	sla	(hl)			;CB 26
	sla	offset(ix)		;DD CB 55 26
	sla	offset(iy)		;FD CB 55 26
	sla	(ix+offset)		;DD CB 55 26
	sla	(iy+offset)		;FD CB 55 26
	sla	a			;CB 27
	sla	b			;CB 20
	sla	c			;CB 21
	sla	d			;CB 22
	sla	e			;CB 23
	sla	h			;CB 24
	sla	l			;CB 25
	;***********************************************************
	; shift operand right arithmetic
.if ODDBALL_SHIFT_ROTATE
	sra	a,(hl)			;CB 2E
	sra	a,offset(ix)		;DD CB 55 2E
	sra	a,offset(iy)		;FD CB 55 2E
	sra	a,(ix+offset)		;DD CB 55 2E
	sra	a,(iy+offset)		;FD CB 55 2E
	sra	a,a			;CB 2F
	sra	a,b			;CB 28
	sra	a,c			;CB 29
	sra	a,d			;CB 2A
	sra	a,e			;CB 2B
	sra	a,h			;CB 2C
	sra	a,l			;CB 2D
.endif
	sra	(hl)			;CB 2E
	sra	offset(ix)		;DD CB 55 2E
	sra	offset(iy)		;FD CB 55 2E
	sra	(ix+offset)		;DD CB 55 2E
	sra	(iy+offset)		;FD CB 55 2E
	sra	a			;CB 2F
	sra	b			;CB 28
	sra	c			;CB 29
	sra	d			;CB 2A
	sra	e			;CB 2B
	sra	h			;CB 2C
	sra	l			;CB 2D
	;***********************************************************
	; shift operand right logical
.if ODDBALL_SHIFT_ROTATE
	srl	a,(hl)			;CB 3E
	srl	a,offset(ix)		;DD CB 55 3E
	srl	a,offset(iy)		;FD CB 55 3E
	srl	a,(ix+offset)		;DD CB 55 3E
	srl	a,(iy+offset)		;FD CB 55 3E
	srl	a,a			;CB 3F
	srl	a,b			;CB 38
	srl	a,c			;CB 39
	srl	a,d			;CB 3A
	srl	a,e			;CB 3B
	srl	a,h			;CB 3C
	srl	a,l			;CB 3D
.endif
	srl	(hl)			;CB 3E
	srl	offset(ix)		;DD CB 55 3E
	srl	offset(iy)		;FD CB 55 3E
	srl	(ix+offset)		;DD CB 55 3E
	srl	(iy+offset)		;FD CB 55 3E
	srl	a			;CB 3F
	srl	b			;CB 38
	srl	c			;CB 39
	srl	d			;CB 3A
	srl	e			;CB 3B
	srl	h			;CB 3C
	srl	l			;CB 3D
	;***********************************************************
	; subtract operand from 'a'
	sub	a,(hl)			;96
	sub	a,offset(ix)		;DD 96 55
	sub	a,offset(iy)		;FD 96 55
	sub	a,(ix+offset)		;DD 96 55
	sub	a,(iy+offset)		;FD 96 55
	sub	a,a			;97
	sub	a,b			;90
	sub	a,c			;91
	sub	a,d			;92
	sub	a,e			;93
	sub	a,h			;94
	sub	a,l			;95
	sub	a,#n			;D6 20
	sub	a, n			;D6 20
	;***********************************************************
	;  p. 5-139
	.z280
sublbl:	sub	a,(hl+ix)		;DD 91
	sub	a,(hl+iy)		;DD 92
	sub	a,lxoff(hl)		;FD 93 22 11
	sub	a,(ix+iy)		;DD 93
	sub	a,lxoff(ix)		;FD 91 22 11
	sub	a,(ix+lxoff)		;FD 91 22 11
	sub	a,lxoff(iy)		;FD 92 22 11
	sub	a,(iy+lxoff)		;FD 92 22 11
	sub	a,[sublbl]		;FD 90 E2 FF
	sub	a,lxoff(sp)		;DD 90 22 11
	sub	a,(sp+lxoff)		;DD 90 22 11
	sub	a,(sp)			;DD 90 00 00
	sub	a,(daddr)		;DD 97 44 33
	sub	a,ixh			;DD 94
	sub	a,ixl			;DD 95
	sub	a,iyh			;FD 94
	sub	a,iyl			;FD 95
	;***********************************************************
	;  p. 5-140
	sub	hl,(hl)			;DD ED CE
	sub	hl,(ix+lxoff)		;FD ED CE 22 11
	sub	hl,(ix+sxoff)		;FD ED CE 55 00
	sub	hl,(iy+lxoff)		;FD ED DE 22 11
	sub	hl,(iy)			;FD ED DE 00 00
	sub	hl,[.+n]		;DD ED FE 1B 00
	sub	hl,(daddr)		;DD ED DE 44 33
	sub	hl,bc			;ED CE
	sub	hl,de			;ED DE
	sub	hl,hl			;ED EE
	sub	hl,ix			;DD ED EE
	sub	hl,iy			;FD ED EE
	sub	hl,sp			;ED FE
	sub	hl,#nn			;FD ED FE 84 05
	subw	hl,(hl)			;DD ED CE
	subw	hl,(ix+lxoff)		;FD ED CE 22 11
	subw	hl,(ix+sxoff)		;FD ED CE 55 00
	subw	hl,(iy+lxoff)		;FD ED DE 22 11
	subw	hl,(iy)			;FD ED DE 00 00
	subw	hl,[.+n]		;DD ED FE 1B 00
	subw	hl,(daddr)		;DD ED DE 44 33
	subw	hl,bc			;ED CE
	subw	hl,de			;ED DE
	subw	hl,hl			;ED EE
	subw	hl,ix			;DD ED EE
	subw	hl,iy			;FD ED EE
	subw	hl,sp			;ED FE
	subw	hl,#nn			;FD ED FE 84 05
	;***********************************************************
	; Z280 test and set
	;  p. 5-141
	tset	b			;CB 30
	tset	c			;CB 31
	tset	d			;CB 32
	tset	e			;CB 33
	tset	h			;CB 34
	tset	l			;CB 35
	tset	(hl)			;CB 36
	tset	a			;CB 37
	tset	(ix+sxoff)		;DD CB 55 36
	tset	(iy+sxoff)		;FD CB 55 36
	;***********************************************************
	; Z280 test input
	tsti	(c)			;ED 70
	.z80
	;***********************************************************
	; logical 'xor' operand with 'a'
	xor	a,(hl)			;AE
	xor	a,offset(ix)		;DD AE 55
	xor	a,offset(iy)		;FD AE 55
	xor	a,(ix+offset)		;DD AE 55
	xor	a,(iy+offset)		;FD AE 55
	xor	a,a			;AF
	xor	a,b			;A8
	xor	a,c			;A9
	xor	a,d			;AA
	xor	a,e			;AB
	xor	a,h			;AC
	xor	a,l			;AD
	xor	a,#n			;EE 20
	xor	a, n			;EE 20
	;***********************************************************
	;  p. 5-143
	.z280
	xor	a,(hl+ix)		;DD A9
	xor	a,(hl+iy)		;DD AA
	xor	a,(ix+iy)		;DD AB
	xor	a,lxoff(ix)		;FD A9 22 11
	xor	a,lxoff(iy)		;FD AA 22 11
	xor	a,[.+n]			;FD A8 1C 00
	xor	a,lxoff(sp)		;DD A8 22 11
	xor	a,(daddr)		;DD AF 44 33
	xor	a,ixh			;DD AC
	xor	a,ixl			;DD AD
	xor	a,iyh			;FD AC
	xor	a,iyl			;FD AD
	.page
	;***********************************************************
	; Hitachi HD64180 Codes
	;***********************************************************

	.hd64

	;***********************************************************
	; start of the Z180 section
	;***********************************************************
	
	;***********************************************************
	; load register with input from port (n)
	in0	a,(n)			;ED 38 20
	in0	b,(n)			;ED 00 20
	in0	c,(n)			;ED 08 20
	in0	d,(n)			;ED 10 20
	in0	e,(n)			;ED 18 20
	in0	h,(n)			;ED 20 20
	in0	l,(n)			;ED 28 20
	;***********************************************************
	; multiplication of each half
	; of the specified register pair
	; with the 16-bit result going to
	; the specified register pair
	mlt	bc			;ED 4C
	mlt	de			;ED 5C
	mlt	hl			;ED 6C
	mlt	sp			;ED 7C
	;***********************************************************
	; load output port (c) with
	; location (hl),
	; decrement hl and b
	; decrement c
	otdm				;ED 8B
	;***********************************************************
	; load output port (c) with
	; location (hl),
	; decrement hl and c
	; decrement b
	; repeat until b = 0
	otdmr				;ED 9B
	;***********************************************************
	; load output port (c) with
	; location (hl),
	; increment hl and b
	; decrement c
	otim				;ED 83
	;***********************************************************
	; load output port (c) with
	; location (hl),
	; increment hl and c
	; decrement b
	; repeat until b = 0
	otimr				;ED 93
	;***********************************************************
	; load output port (n) from register
	out0	(n),a			;ED 39 20
	out0	(n),b			;ED 01 20
	out0	(n),c			;ED 09 20
	out0	(n),d			;ED 11 20
	out0	(n),e			;ED 19 20
	out0	(n),h			;ED 21 20
	out0	(n),l			;ED 29 20
	;***********************************************************
	; enter sleep mode
	slp				;ED 76
	;***********************************************************
	; non-destructive'and' with accumulator and specified operand
	tst	a,a			;ED 3C
	tst	a,b			;ED 04
	tst	a,c			;ED 0C
	tst	a,d			;ED 14
	tst	a,e			;ED 1C
	tst	a,h			;ED 24
	tst	a,l			;ED 2C
	tst	a,#n			;ED 64 20
	tst	a, n			;ED 64 20
	tst	a,(hl)			;ED 34
	tst	a			;ED 3C
	tst	b			;ED 04
	tst	c			;ED 0C
	tst	d			;ED 14
	tst	e			;ED 1C
	tst	h			;ED 24
	tst	l			;ED 2C
	tst	#n			;ED 64 20
	tst	 n			;ED 64 20
	tst	(hl)			;ED 34
	;***********************************************************
	; non-destructive 'and' of n and the contents of port (c)
	tstio	#n,(c)			;ED 74 20
	tstio	(c),#n			;ED 74 20
	tstio	#n			;ED 74 20
	tstio	 n			;ED 74 20
	;***********************************************************
	; end of the Z180 section
	;***********************************************************
	
	;***********************************************************
	; start of the Z80 undocumented section
	;***********************************************************
	.z80u
	adc	a,ixh			;DD 8C
	adc	a,ixl			;DD 8D
	adc	a,iyh			;FD 8C
	adc	a,iyl			;FD 8D
	add	a,ixh			;DD 84
	add	a,ixl			;DD 85
	add	a,iyh			;FD 84
	add	a,iyl			;FD 85
	and	a,ixh			;DD A4
	and	a,ixl			;DD A5
	and	a,iyh			;FD A4
	and	a,iyl			;FD A5
	cp	a,ixh			;DD BC
	cp	a,ixl			;DD BD
	cp	a,iyh			;FD BC
	cp	a,iyl			;FD BD
	dec	ixh			;DD 25
	dec	ixl			;DD 2D
	dec	iyh			;FD 25
	dec	iyl			;FD 2D
	in	ixh,(c)			;DD ED 60
	in	ixl,(c)			;DD ED 68
	in	iyh,(c)			;FD ED 60
	in	iyl,(c)			;FD ED 68
	inc	ixh			;DD 24
	inc	ixl			;DD 2C
	inc	iyh			;FD 24
	inc 	iyl			;FD 2C
	ld	a,ixh			;DD 7C
	ld	a,ixl			;DD 7D
	ld	a,iyh			;FD 7C
	ld	a,iyl			;FD 7D
	ld	ixh,a			;DD 67
	ld	ixl,a			;DD 6F
	ld	iyh,a			;FD 67
	ld	iyl,a			;FD 6F
	ld	ixh,#n			;DD 26 20
	ld	ixl,#n			;DD 2E 20
	ld	iyh,#n			;FD 26 20
	ld	iyl,#n			;FD 2E 20
	ld	a,ixh			;DD 7C
	ld	a,iyl			;FD 7D
	ld	b,ixh			;DD 44
	ld	b,ixl			;DD 45
	LD	C,IXH			;DD 4C
	LD	C,IXL			;DD 4D
	ld	d,iyh			;FD 54
	ld	d,iyl			;FD 55
	ld	e,iyh			;FD 5C
	ld	e,iyl			;FD 5D
.if 1
	ld	ixh,ixh			;DD 64
	ld	ixh,ixl			;DD 65
	ld	ixl,ixh			;DD 6C
	ld	ixl,ixl			;DD 6D
	ld	iyh,iyh			;FD 64
	ld	iyh,iyl			;FD 65
	ld	iyl,iyh			;FD 6C
	ld	iyl,iyl			;FD 6D
.endif
	ld	ixh,a			;DD 67
	ld	ixl,a			;DD 6F
	ld	iyh,a			;FD 67
	ld	iyl,a			;FD 6F
	ld	ixh,b			;DD 60
	ld	ixl,c			;DD 69
	ld	iyh,d			;FD 62
	ld	iyl,e			;FD 6B
	ld	ixh,#n			;DD 26 20
	ld	ixl,#n+3		;DD 2E 23
	ld	iyh,#n-1		;FD 26 1F
	ld	iyl,#n			;FD 2E 20
	or	a,ixh			;DD B4
	or	a,ixl			;DD B5
	or	a,iyh			;FD B4
	or	a,iyl			;FD B5
	sbc	a,ixh			;DD 9C
	sbc	a,ixl			;DD 9D
	sbc	a,iyh			;FD 9C
	sbc	a,iyl			;FD 9D
	sub	a,ixh			;DD 94
	sub	a,ixl			;DD 95
	sub	a,iyh			;FD 94
	sub	a,iyl			;FD 95
	xor	a,ixh			;DD AC
	xor	a,ixl			;DD AD
	xor	a,iyh			;FD AC
	xor	a,iyl			;FD AD
	;***********************************************************
	; c.f. 'tset' on Z280 (see above)
	; shift left (like SLA), but shift a '1' into bit 0 !!!
	sll	(hl)			;CB 36
	sll	offset(ix)		;DD CB 55 36
	sll	offset(iy)		;FD CB 55 36
	sll	(ix+offset)		;DD CB 55 36
	sll	(iy+offset)		;FD CB 55 36
	sll	a			;CB 37
	sll	b			;CB 30
	sll	c			;CB 31
	sll	d			;CB 32
	sll	e			;CB 33
	sll	h			;CB 34
	sll	l			;CB 35
	;  sll a  ==  tset a	may be used to distinguish Z80 from Z280 with Areg = 0x41
	;***********************************************************
	; end of the Z80 undocumented section
	;***********************************************************
	

	.z280
	.globl	gg
.if 0
	adc	a,(ix+offset)
	adc	(ix+offset)
	
	ld	a,(nn+4+iy)	; 'u' error
	add	a,sp		; 'q' error
	add	hl,(hl+lxoff)		;error
	mlt	bc		; illegal on the Z80
	exx;xxxxxxxx
	exx			;kkkkkk
	halt	a,a
	im	2
	exx	af
	ld	a,nn+4(iy)
	ld	a,(ix+nn+4)
	ld	a,(iy+offset+5)
	ld	bc,(gg+0x1234)
	ld	d,# >(gg+0x1234)
	ld	e,# <(gg+0x1234)
	out	( c ) , a	; ED 79
	out	c,a		; 'a' error
	reti			;privileged
	retn			;privileged
	adc	a,(pc+raoff)		;FD 88 32 12  pq
	adc	a,lxoff(pc)		;FD 88 20 11  pq
	adc	a,sxoff(pc)		;FD 88 53 00  pq
	adc	a,(ext)			;DD 8F 00 00  rs
	adc	a,(ext+n)		;DD 8F 20 00  rs
.endif

	.area	_data
gg:	.ascii	"The quick brown fox "
data:	
	.asciz	"jumped over the lazy dogs."

ext:	.dw	0

;	.end	
