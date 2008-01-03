; Тестовый файл для проверки ассемблера

; тестовый 
	org	0x0010
num	equ	50
	
	; цикл
	mvi	b,num
loop:	dcr 	e	; в этой строке пробел и табуляция
	jnz	loop
	hlt

	; тест директивы db
	db	10,20,30,40
	db	0x10, 0x20, 0x30, 0x40
	db	"Some text"


; полный список кодов К580
	nop		; 00
	lxi	b,0000	; 01 00 00
	stax 	b	; 02
	inx	b	; 03
	inr	b	; 04
	dcr	b	; 05
	mvi	b,00	; 06 00
	rlc		; 07
;	*nop		  08
	dad	b	; 09
	ldax	b	; 0A
	dcx	b	; 0B
	inr	c	; 0C
	dcr	c	; 0D
	mvi	c,00	; 0E 00
	rrc		; 0F
;	*nop		  10
	lxi	d,0000	; 11 00 00
	stax	d	; 12
	inx	d	; 13
	inr	d	; 14
	dcr	d	; 15
	mvi	d,00	; 16 00
	ral		; 17
;	*nop		  18
	dad	d	; 19
	ldax	d	; 1A
	dcx	d	; 1B
	inr	e	; 1C
	dcr	e	; 1D
	mvi	e,00	; 1E 00
	rar		; 1F
;	*nop		  20
	lxi	h,0000	; 21 00 00
	shld	0000	; 22 00 00
	inx	h	; 23
	inr	h	; 24
	dcr	h	; 25
	mvi	h,00	; 26 00
	dad	h	; 29
	lhld	0000	; 2A 00 00
	dcx	h	; 2B
	inr	l	; 2C
	dcr	l	; 2D
	mvi	l,00	; 2E 00
	cma		; 2F
;	*nop		  30
	lxi	sp,0000	; 31 00 00  дашьше не проверял!!!!!!!!!!!!!!!
	sta	0000	; 32 00 00
	inx	sp	; 33
	inr	m	; 34
	dcr	m	; 35
	mvi	m,00	; 36 00
	stc		; 37
	dad	sp	; 39
	lda	0000	; 3A 00 00
	dcx	sp	; 3B
	inr	a	; 3C
	dcr	a	; 3D
	mvi	a,00	; 3E 00
	cmc		; 3F
	mov	b,b	; 40
	mov	b,c	; 41
	mov	b,d	; 42
	mov	b,e	; 43
	mov	b,h	; 44
	mov	b,l	; 45
	mov	b,m	; 46
	mov	b,a	; 47
	mov	c,b	; 48
	mov	c,c	; 49
	mov	c,d	; 4A
	mov	c,e	; 4B
	mov	c,h	; 4C
	mov	c,l	; 4D
	mov	c,m	; 4E
	mov	c,a	; 4F
	mov	d,b	; 50
	mov	d,c	; 51
	mov	d,d	; 52
	mov	d,e	; 53
	mov	d,h	; 54
	mov	d,l	; 55
	mov	d,m	; 56
	mov	d,a	; 57
	mov	e,b	; 58
	mov	e,c	; 59
	mov	e,d	; 5A
	mov	e,e	; 5B
	mov	e,h	; 5C
	mov	e,l	; 5D
	mov	e,m	; 5E
	mov	e,a	; 5F
	mov	h,b	; 60
	mov	h,c	; 61
	mov	h,d	; 62
	mov	h,e	; 63
	mov	h,h	; 64
	mov	h,l	; 65
	mov	h,m	; 66
	mov	h,a	; 67
	mov	l,b	; 68
	mov	l,c	; 69
	mov	l,d	; 6A
	mov	l,e	; 6B
	mov	l,h	; 6C
	mov	l,l	; 6D
	mov	l,m	; 6E
	mov	l,a	; 6F
	mov	m,b	; 70
	mov	m,c	; 71
	mov	m,d	; 72
	mov	m,e	; 73
	mov	m,h	; 74
	mov	m,l	; 75
	hlt		; 76
	mov	m,a	; 77
	mov	a,b	; 78
	mov	a,c	; 79
	mov	a,d	; 7A
	mov	a,e	; 7B
	mov	a,h	; 7C
	mov	a,l	; 7D
	mov	a,m	; 7E
	mov	a,a	; 7F
	add	b	; 80
	add	c	; 81
	add	d	; 82
	add	e	; 83
	add	h	; 84
	add	l	; 85
	add	m	; 86
	add	a	; 87
	adc	b	; 88
	adc	c	; 89
	adc	d	; 8A
	adc	e	; 8B
	adc	h	; 8C
	adc	l	; 8D
	adc	m	; 8E
	adc	a	; 8F
	sub	b	; 90
	sub	c	; 91
	sub	d	; 92
	sub	e	; 93
	sub	h	; 94
	sub	l	; 95
	sub	m	; 96
	sub	a	; 97
	sbb	b	; 98
	sbb	c	; 99
	sbb	d	; 9A
	sbb	e	; 9B
	sbb	h	; 9C
	sbb	l	; 9D
	sbb	m	; 9E
	sbb	a	; 9F
	ana	b	; A0
	ana	c	; A1
	ana	d	; A2
	ana	e	; A3
	ana	h	; A4
	ana	l	; A5
	ana	m	; A6
	ana	a	; A7
	xra	b	; A8
	xra	c	; A9
	xra	d	; AA
	xra	e	; AB
	xra	h	; AC
	xra	l	; AD
	xra	m	; AE
	xra	a	; AF
	ora	b	; B0
	ora	c	; B1
	ora	d	; B2
	ora	e	; B3
	ora	h	; B4
	ora	l	; B5
	ora	m	; B6
	ora	a	; B7
	cmp	b	; B8
	cmp	c	; B9
	cmp	d	; BA
	cmp	e	; BB
	cmp	h	; BC
	cmp	l	; BD
	cmp	m	; BE
	cmp	a	; BF
;	*nop		  C0
	pop	b	; C1
	jnz	08af	; C2 AF 08
	jmp	08af	; C3 AF 08
;	*nop		  C4
	push	b	; C5
	adi	a4	; C6
	rst	0	; C7
;	*nop		  C8
	ret		; C9
	jz	08af	; CA AF 08
;	*nop		  CB
;	*nop		  CC
	call	f512	; CD 12 F5
	aci	11	; CE 11
	rst	1	; CF
;	*nop		  D0
	pop	d	; D1
	jnc	0000	; D2 00 00
	out	12	; D3 12
;	*nop		  D4
	push	d	; D5
	sui	12	; D6 12
	rst	2	; D7
;	*nop		  D8
;	*nop		  D9
	jc	1234	; DA 34 12
	in	f5	; DB F5
;	*nop		  DC
;	*nop		  DD
	sbi	00	; DE 00
	rst	3	; DF
;	*nop		  E0
	pop	h	; E1
	jpo	0000	; E2 00 00
	xthl		; E3
;	*nop		  E4
	push	h	; E5
	ani	00	; E6 00
	rst	4	; E7
;	*nop		  E8
	pchl		; E9
	jpe	8899	; EA 99 88
	xchg		; EB
;	*nop		  EC
;	*nop		  ED
	xri	00	; EE 00
	rst	5	; EF
;	*nop		  F0
	pop	psw	; F1
	jp	fdec	; F2 EC FD
	di		; F3
;	*nop		  F4
	push	psw	; F5
	ori	00	; F6 00
	rst	6	; F7
;	*nop		  F8
	sphl		; F9
	jm	9876	; FA 76 98
	ei		; FB
;	*nop		  FC
;	*nop		  FD
	cpi	34	; FE
	rst	7	; FF

