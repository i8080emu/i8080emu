; ���� ������ ������ ������������� �� ���������� ��������
; ���� �������� 440 �� (���� �� ������ ������)
; ������������� 3 ���.

	org	0000
	lxi	b,2640	; ����� ����������
	mvi	d,01		; ��� ������� ����������

loop:	; ������ �����
	mov	a,d
	out	0xf4		; ������������� ��� ������� (����� ����� ������� ������������� � ���� ���������->������ ������� ���������)
	xri	1		; ����������� ��������� ���� �������
	mov	d,a		; ���������� ��������� ����

	; ������� � ������ � ����� ����� �������� 
	; 5 + 10 + 7 + 5 +
	; 5 + 5 + 4 + 10 = 51 ����, ������ �������� ���
	; 2272 - 51 = 2221 ����� 

	; ������������ ��������
	mvi	h,82		; 7 ������ + 27*82 = 2221
loop1:	nop
	nop
	nop
	dcr	h
	jnz	loop1		; ������ �������� ����� 4*3 + 5 + 10 = 27 ������

	; ����� ��������

	; ��������� ������� �����
	dcx	b
	; ���������� � ����� (������� ���������� ����������� ���� �� ��������� �� �����
	; ������� ����������� ��������� �������� b � � �� �����������)
	mov	a,b
	ora	c
	jnz	loop
	nop
	hlt

; ������ ����� 1/440 = 2,272 �� ���
; 2000000 / 440 = 4545 ������ ���������� (��� ��� �������� ������� 2��)
; ���������� ����� ����� 4545/2 = 2272 �����.
; �� 3 ������� ������� 3*440 = 1320 �������� (6000000 ������).
; ��� 1320*2 = 2640 ������������
	