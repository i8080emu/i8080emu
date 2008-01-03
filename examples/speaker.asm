	org 0000
; Speaker test
loop:	mvi	a,1
	out	0xf4
	nop
	nop
	nop
	nop
	nop
	nop
	mvi	a,0
	out 	0xf4
	dcr	b
	jnz	loop
	hlt
