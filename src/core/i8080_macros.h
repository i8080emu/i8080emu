#ifndef _I8080_MACROS_H
#define _I8080_MACROS_H

/* Macros used for accessing the registers */
#define A   i8080.af.b.h
#define F   i8080.af.b.l
#define AF  i8080.af.w

#define B   i8080.bc.b.h
#define C   i8080.bc.b.l
#define BC  i8080.bc.w

#define D   i8080.de.b.h
#define E   i8080.de.b.l
#define DE  i8080.de.w

#define H   i8080.hl.b.h
#define L   i8080.hl.b.l
#define HL  i8080.hl.w


#define SPH i8080.sp.b.h
#define SPL i8080.sp.b.l
#define SP  i8080.sp.w

#define PCH i8080.pc.b.h
#define PCL i8080.pc.b.l
#define PC  i8080.pc.w


/* Флаги */

#define FLAG_C	0x01
#define FLAG_1	0x02
#define FLAG_P	0x04
#define FLAG_3	0x08
#define FLAG_AC	0x10
#define FLAG_5	0x20
#define FLAG_Z	0x40
#define FLAG_S	0x80


/* Некоторые часто-используемые инструкции */

// логическое "и"
#define AND(value)\
{\
  A &= (value);\
  F = FLAG_AC | sz53p_table[A];\
}

#define ADC(value)\
{\
  WORD adctemp = A + (value) + ( F & FLAG_C );\
  BYTE lookup = ( (A & 0x88) >> 3 ) | ( ( (value) & 0x88 ) >> 2 ) |\
    ( (adctemp & 0x88) >> 1 );\
  A=adctemp;\
  F = ( adctemp & 0x100 ? FLAG_C : 0 ) |\
    halfcarry_add_table[lookup & 0x07] | overflow_add_table[lookup >> 4] |\
    sz53p_table[A];\
}

#define ADC16(value)\
{\
  DWORD add16temp= HL + (value) + ( F & FLAG_C );\
  BYTE lookup = ( ( HL & 0x8800 ) >> 11 ) |\
    ( ( (value) & 0x8800 ) >> 10 ) |\
    ( ( add16temp & 0x8800 ) >> 9 );\
  HL = add16temp;\
  F = ( add16temp & 0x10000 ? FLAG_C : 0 )|\
    overflow_add_table[lookup >> 4] |\
    ( H & ( FLAG_3 | FLAG_5 | FLAG_S ) ) |\
    halfcarry_add_table[lookup&0x07]|\
    ( HL ? 0 : FLAG_Z );\
}

#define ADD(value)\
{\
  WORD addtemp = A + (value);\
  BYTE lookup = ( (A & 0x88) >> 3 ) | ( ( (value) & 0x88 ) >> 2 ) |\
    ( (addtemp & 0x88) >> 1 );\
  A=addtemp;\
  F = ( addtemp & 0x100 ? FLAG_C : 0 ) |\
    halfcarry_add_table[lookup & 0x07] | overflow_add_table[lookup >> 4] |\
    sz53p_table[A];\
}

#define ADD16(value1,value2)\
{\
  DWORD add16temp= (value1) + (value2);\
  BYTE lookup = ( ( (value1) & 0x0800 ) >> 11 ) |\
    ( ( (value2) & 0x0800 ) >> 10 ) |\
    ( ( add16temp & 0x0800 ) >> 9 );\
  (value1) = add16temp;\
  F = ( F & ( FLAG_P | FLAG_Z | FLAG_S ) ) |\
    ( add16temp & 0x10000 ? FLAG_C : 0 )|\
    ( ( add16temp >> 8 ) & ( FLAG_3 | FLAG_5 ) ) |\
    halfcarry_add_table[lookup];\
}


#define CALL()\
{\
  BYTE calltempl, calltemph;\
  calltempl=readbyte(PC++);\
  calltemph=readbyte(PC++);\
  PUSH16(PCL,PCH);\
  PCL=calltempl; PCH=calltemph;\
}

#define CP(value)\
{\
  WORD cptemp = A - value;\
  BYTE lookup = ( (A & 0x88) >> 3 ) | ( ( (value) & 0x88 ) >> 2 ) |\
    ( (cptemp & 0x88) >> 1 );\
  F = ( cptemp & 0x100 ? FLAG_C : ( cptemp ? 0 : FLAG_Z ) ) | FLAG_1 |\
    halfcarry_sub_table[lookup & 0x07] |\
    overflow_sub_table[lookup >> 4] |\
    ( value & ( FLAG_3 | FLAG_5 ) ) |\
    ( cptemp & FLAG_S );\
}

// уменьшение регистра на единицу
#define DCR(value)\
{\
  F = ( F & FLAG_C ) | ( (value)&0x0f ? 0 : FLAG_AC ) | FLAG_1;\
  (value)--;\
  F |= ( (value)==0x79 ? FLAG_P : 0 ) | sz53p_table[value];\
}

// увеличение регистра на единицу
#define INR(value)\
{\
  (value)++;\
  F = ( F & FLAG_C ) | ( (value)==0x80 ? FLAG_P : 0 ) |\
  ( (value)&0x0f ? 0 : FLAG_AC ) | ( (value) ? 0 : FLAG_Z ) |\
  sz53p_table[(value)];\
}

#define LD16_NNRR(regl,regh)\
do{\
  WORD ldtemp;\
  ldtemp=readbyte(PC++);\
  ldtemp|=readbyte(PC++) << 8;\
  writebyte(ldtemp++,(regl));\
  writebyte(ldtemp,(regh));\
  break;\
}while(0)

#define LD16_RRNN(regl,regh)\
do{\
  WORD ldtemp;\
  ldtemp=readbyte(PC++);\
  ldtemp|=readbyte(PC++) << 8;\
  (regl)=readbyte(ldtemp++);\
  (regh)=readbyte(ldtemp);\
  break;\
}while(0)

#define JP()\
{\
  WORD jptemp=PC;\
  PCL=readbyte(jptemp++);\
  PCH=readbyte(jptemp);\
}

#define OR(value)\
{\
  A |= (value);\
  F = sz53p_table[A];\
}

#define POP16(regl,regh)\
{\
  (regl)=readbyte(SP++);\
  (regh)=readbyte(SP++);\
}

#define PUSH16(regl,regh)\
{\
  SP--;\
  writebyte(SP,(regh));\
  SP--;\
  writebyte(SP,(regl));\
}

#define RET()\
{\
  POP16(PCL,PCH);\
}

#define RL(value)\
{\
  BYTE rltemp = (value);\
  (value) = ( (value)<<1 ) | ( F & FLAG_C );\
  F = ( rltemp >> 7 ) | sz53p_table[(value)];\
}

#define RLC(value)\
{\
  (value) = ( (value)<<1 ) | ( (value)>>7 );\
  F = ( (value) & FLAG_C ) | sz53p_table[(value)];\
}

#define RR(value)\
{\
  BYTE rrtemp = (value);\
  (value) = ( (value)>>1 ) | ( F << 7 );\
  F = ( rrtemp & FLAG_C ) | sz53p_table[(value)];\
}

#define RRC(value)\
{\
  F = (value) & FLAG_C;\
  (value) = ( (value)>>1 ) | ( (value)<<7 );\
  F |= sz53p_table[(value)];\
}

#define RST(value)\
{\
  PUSH16(PCL,PCH);\
  PC=(value);\
}

// вычитание с заемом из CY
#define SBB(value)\
{\
  WORD sbctemp = A - (value) - ( F & FLAG_C );\
  BYTE lookup = ( (A & 0x88) >> 3 ) | ( ( (value) & 0x88 ) >> 2 ) |\
    ( (sbctemp & 0x88) >> 1 );\
  A=sbctemp;\
  F = ( sbctemp & 0x100 ? FLAG_C : 0 ) | FLAG_1 |\
    halfcarry_sub_table[lookup & 0x07] | overflow_sub_table[lookup >> 4] |\
    sz53p_table[A];\
}

#define SBC16(value)\
{\
  DWORD sub16temp = HL - (value) - (F & FLAG_C);\
  BYTE lookup = ( ( HL & 0x8800 ) >> 11 ) |\
    ( ( (value) & 0x8800 ) >> 10 ) |\
    ( ( sub16temp & 0x8800 ) >> 9 );\
  HL = sub16temp;\
  F = ( sub16temp & 0x10000 ? FLAG_C : 0 ) |\
    FLAG_1 | overflow_sub_table[lookup >> 4] |\
    ( H & ( FLAG_3 | FLAG_5 | FLAG_S ) ) |\
    halfcarry_sub_table[lookup&0x07] |\
    ( HL ? 0 : FLAG_Z) ;\
}

#define SLA(value)\
{\
  F = (value) >> 7;\
  (value) <<= 1;\
  F |= sz53p_table[(value)];\
}

#define SLL(value)\
{\
  F = (value) >> 7;\
  (value) = ( (value) << 1 ) | 0x01;\
  F |= sz53p_table[(value)];\
}

#define SRA(value)\
{\
  F = (value) & FLAG_C;\
  (value) = ( (value) & 0x80 ) | ( (value) >> 1 );\
  F |= sz53p_table[(value)];\
}

#define SRL(value)\
{\
  F = (value) & FLAG_C;\
  (value) >>= 1;\
  F |= sz53p_table[(value)];\
}

#define SUB(value)\
{\
  WORD subtemp = A - (value);\
  BYTE lookup = ( (A & 0x88) >> 3 ) | ( ( (value) & 0x88 ) >> 2 ) |\
    ( (subtemp & 0x88) >> 1 );\
  A=subtemp;\
  F = ( subtemp & 0x100 ? FLAG_C : 0 ) | FLAG_1 |\
    halfcarry_sub_table[lookup & 0x07] | overflow_sub_table[lookup >> 4] |\
    sz53p_table[A];\
}

#define XOR(value)\
{\
  A ^= (value);\
  F = sz53p_table[A];\
}

#endif
