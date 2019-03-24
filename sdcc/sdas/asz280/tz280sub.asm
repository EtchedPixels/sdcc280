	.title	Externals for tz280.asm
	
	.area	_code
	.globl	ext
	
	.z280
	.even
subr::	lda	hl,(pc+ext)		;
	ret				;C9
