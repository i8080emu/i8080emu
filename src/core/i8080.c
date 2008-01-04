/* Эмулятор-отладчик i8080
*  Ядро эмулятора- исполнение инструкций.
*  За основу взято ядро популярного эмулятора Z80 fuse-emul
*/
#include <windows.h>

#include "i8080.h"
#include "i8080_macros.h"
#include "mpsys.h"
#include "event.h"


/* Whether a half carry occured or not can be determined by looking at
   the 3rd bit of the two arguments and the result; these are hashed
   into this table in the form r12, where r is the 3rd bit of the
   result, 1 is the 3rd bit of the 1st argument and 2 is the
   third bit of the 2nd argument; the tables differ for add and subtract
   operations */
BYTE halfcarry_add_table[] = { 0, FLAG_AC, FLAG_AC, FLAG_AC, 0, 0, 0, FLAG_AC };
BYTE halfcarry_sub_table[] = { 0, 0, FLAG_AC, 0, FLAG_AC, 0, FLAG_AC, FLAG_AC };

/* Similarly, overflow can be determined by looking at the 7th bits; again
   the hash into this table is r12 */
BYTE overflow_add_table[] = { 0, 0, 0, FLAG_P, FLAG_P, 0, 0, 0 };
BYTE overflow_sub_table[] = { 0, FLAG_P, 0, 0, 0, 0, FLAG_P, 0 };

/* Some more tables; initialised in z80_init_tables() */

BYTE sz53_table[0x100]; /* The S, Z, 5 and 3 bits of the lookup value */
BYTE parity_table[0x100]; /* The parity of the lookup value */
BYTE sz53p_table[0x100]; /* OR the above two tables together */



/* Здесь будет происходить все действие */
processor i8080;

/* Максимальное число зарегистрированых функций обратного вызова */
#define MAX_CALLBACKS 16

/* Массив указателей на зарегистрированые функции обратного вызова */
static i8080step_callback_t callback [MAX_CALLBACKS];

/* Число зарегистрированых функций обратного вызова */
static int num_callbacks = 0;

static void i8080_init_tables(void);

/* Начальная инициализация модуля эмуляции процессора (при старте программы) */
void i8080_init(void)
{
  i8080_init_tables();
}


/* Инициализация таблиц используемых для установки флагов */
static void i8080_init_tables(void)
{
  int i,j,k;
  BYTE parity;

  for(i=0;i<0x100;i++) {
    sz53_table[i]= i & ( FLAG_3 | FLAG_5 | FLAG_S );
    j=i; parity=0;
    for(k=0;k<8;k++) { parity ^= j & 1; j >>=1; }
    parity_table[i]= ( parity ? 0 : FLAG_P );
    sz53p_table[i] = sz53_table[i] | parity_table[i];
  }

  sz53_table[0]  |= FLAG_Z;
  sz53p_table[0] |= FLAG_Z;
}


/* Регистрация функции обратного вызова */
int i8080_register_callback(i8080step_callback_t cb)
{
  if (num_callbacks < MAX_CALLBACKS) {
    callback [num_callbacks] = cb;
    num_callbacks++;
    return 1;
  }
  else {
    MessageBox(NULL, "Невозможно зарегистрировать функцию обратного вызова- \
    превышено максимально допустимое количество зарегистрированых функций обратного вызова.",
    "Ошибка", MB_OK | MB_ICONEXCLAMATION);
    return 0;
  }
}

/* Сброс i8080 */
void i8080_reset()
{
  AF = BC = DE = HL = 0;
  SP = PC = 0;

  i8080.inte = 1;  /* прерывания разрешены */
  i8080.halted = 0;
}


/* Обработка маскируемых прерываний i8080 */
void i8080_interrupt()
{
  i8080.halted = 0;

  /* если разрешена обработка прерывания */
  if (i8080.inte)
  {
    /* запрещаем прерывания */
    i8080.inte = 0;

    /* помещаем в стек адрес возврата из прерывания */
    writebyte( --SP, PCH ); writebyte( --SP, PCL );

    /* выполняем команду RST 7 (ее код FF) */
    PC = 0x0038;
    tstates+=12;
  }
}


/* Выполнение текущей команды i8080 */
void i8080_do_opcode(void)
{
  BYTE opcode;
  int n;

  /* Если К580 HALTed (остановлен), ничего не делать */
  if( i8080.halted )
    return;

  opcode = readbyte( PC++ );

  /* обработка кода операции */
  switch(opcode) {
    case 0x00:		/* NOP */
      tstates += 4;
      break;
    case 0x01:		/* LD BC,nnnn   LXI B,B2B3  */
      C=readbyte(PC++);
      B=readbyte(PC++);
      tstates += 10;
      break;
    case 0x02:		/* LD (BC),A    STAX B  */
      writebyte(BC,A);
      tstates += 7;
      break;
    case 0x03:		/* INC BC       INX B  */
      BC++;
      tstates += 5;
      break;
    case 0x04:		/* INC B        INR B  */
      INR(B);
      tstates += 5;
      break;
    case 0x05:		/* DEC B        DCR B  */
      DCR(B);
      tstates += 5;
      break;
    case 0x06:		/* LB B,nn      MVI B,B2  */
      B=readbyte(PC++);
      tstates += 7;
      break;
    case 0x07:		/* RLCA         RLC  */
      A = ( A << 1 ) | ( A >> 7 );
      F = ( F & ( FLAG_P | FLAG_Z | FLAG_S ) ) |
	( A & ( FLAG_C | FLAG_3 | FLAG_5 ) );
      tstates += 4;
      break;
    case 0x08:		/* EX AF,AF'    *NOP */
      break;
    case 0x09:		/* ADD HL,BC    DAD B  */
      ADD16(HL,BC);
      tstates += 10;
      break;
    case 0x0a:		/* LD A,(BC)    LDAX B  */
      A=readbyte(BC);
      tstates += 7;
      break;
    case 0x0b:		/* DEC BC       DCX B */
      BC--;
      tstates += 5;
      break;
    case 0x0c:		/* INC C        INR C */
      INR(C);
      tstates += 5;
      break;
    case 0x0d:		/* DEC C        DCR C */
      DCR(C);
      tstates += 5;
      break;
    case 0x0e:		/* LD C,nn      MVI C,B2*/
      C=readbyte(PC++);
      tstates += 7;
      break;
    case 0x0f:		/* RRCA         RRC */
      F = ( F & ( FLAG_P | FLAG_Z | FLAG_S ) ) | ( A & FLAG_C );
      A = ( A >> 1) | ( A << 7 );
      F |= ( A & ( FLAG_3 | FLAG_5 ) );
      tstates += 4;
      break;
    case 0x10:		/* DJNZ offset  *NOP */
      break;
    case 0x11:		/* LD DE,nnnn   LXI D,B2B3 */
      E=readbyte(PC++);
      D=readbyte(PC++);
      tstates += 10;
      break;
    case 0x12:		/* LD (DE),A    STAX D */
      writebyte(DE,A);
      tstates += 7;
      break;
    case 0x13:		/* INC DE       INX D */
      DE++;
      tstates += 5;
      break;
    case 0x14:		/* INC D        INR D */
      INR(D);
      tstates += 5;
      break;
    case 0x15:		/* DEC D        DCR D */
      DCR(D);
      tstates += 5;
      break;
    case 0x16:		/* LD D,nn      MVI D,B2*/
      D=readbyte(PC++);
      tstates += 7;
      break;
    case 0x17:		/* RLA  RAL */
      {
	BYTE bytetemp = A;
	A = ( A << 1 ) | ( F & FLAG_C );
	F = ( F & ( FLAG_P | FLAG_Z | FLAG_S ) ) |
	  ( A & ( FLAG_3 | FLAG_5 ) ) | ( bytetemp >> 7 );
      }
      tstates += 4;
      break;
    case 0x18:		/* JR offset *NOP */
      break;
    case 0x19:		/* ADD HL,DE    DAD D */
      ADD16(HL,DE);
      tstates += 10;
      break;
    case 0x1a:		/* LD A,(DE)    LDAX D */
      A=readbyte(DE);
      tstates += 7;
      break;
    case 0x1b:		/* DEC DE       DCX D */
      DE--;
      tstates += 5;
      break;
    case 0x1c:		/* INC E        INR E */
      INR(E);
      tstates += 5;
      break;
    case 0x1d:		/* DEC E        DCR E */
      DCR(E);
      tstates += 5;
      break;
    case 0x1e:		/* LD E,nn      MVI E,B2 */
      E=readbyte(PC++);
      tstates += 7;
      break;
    case 0x1f:		/* RRA  RAR */
      {
	BYTE bytetemp = A;
	A = ( A >> 1 ) | ( F << 7 );
	F = ( F & ( FLAG_P | FLAG_Z | FLAG_S ) ) |
	  ( A & ( FLAG_3 | FLAG_5 ) ) | ( bytetemp & FLAG_C ) ;
      }
      tstates += 4;
      break;
    case 0x20:		/* JR NZ,offset *NOP */
      break;
    case 0x21:		/* LD HL,nnnn   LXI H,B2B3 */
      L=readbyte(PC++);
      H=readbyte(PC++);
      tstates += 10;
      break;
    case 0x22:		/* LD (nnnn),HL         SHLD B2B3 */
      LD16_NNRR(L,H);
      tstates += 16;
      break;
    case 0x23:		/* INC HL       INX H */
      HL++;
      tstates += 5;
      break;
    case 0x24:		/* INC H        INR H */
      INR(H);
      tstates += 5;
      break;
    case 0x25:		/* DEC H        DCR H */
      DCR(H);
      tstates += 5;
      break;
    case 0x26:		/* LD H,nn      MVI H,B2*/
      H=readbyte(PC++);
      tstates += 7;
      break;
    case 0x27:		/* DAA          *NOP */
      break;
    case 0x28:		/* JR Z,offset  *NOP */
      break;
    case 0x29:		/* ADD HL,HL    DAD H */
      ADD16(HL,HL);
      tstates += 10;
      break;
    case 0x2a:		/* LD HL,(nnnn) LHLD B2B3 */
      LD16_RRNN(L,H);
      tstates += 16;
      break;
    case 0x2b:		/* DEC HL       DCX H */
      HL--;
      tstates += 5;
      break;
    case 0x2c:		/* INC L        INR L */
      INR(L);
      tstates += 5;
      break;
    case 0x2d:		/* DEC L        DCR L */
      DCR(L);
      tstates += 5;
      break;
    case 0x2e:		/* LD L,nn      MVI L,B2 */
      L=readbyte(PC++);
      tstates += 7;
      break;
    case 0x2f:		/* CPL          CMA */
      A ^= 0xff;
      F = ( F & ( FLAG_C | FLAG_P | FLAG_Z | FLAG_S ) ) |
	( A & ( FLAG_3 | FLAG_5 ) ) | ( FLAG_1 | FLAG_AC );
      tstates += 4;
      break;
    case 0x30:		/* JR NC,offset *NOP */
      break;
    case 0x31:		/* LD SP,nnnn   LXI SP,B2B3 */
      SPL=readbyte(PC++);
      SPH=readbyte(PC++);
      tstates += 10;
      break;
    case 0x32:		/* LD (nnnn),A  STA B2B3 */
      {
	WORD wordtemp=readbyte(PC++);
	wordtemp|=readbyte(PC++) << 8;
	writebyte(wordtemp,A);
      }
      tstates += 13;
      break;
    case 0x33:		/* INC SP       INX SP */
      SP++;
      tstates += 5;
      break;
    case 0x34:		/* INC (HL)     INR M */
      {
	BYTE bytetemp=readbyte(HL);
	INR(bytetemp);
	writebyte(HL,bytetemp);
      }
      tstates += 10;
      break;
    case 0x35:		/* DEC (HL)     DCR M */
      {
	BYTE bytetemp=readbyte(HL);
	DCR(bytetemp);
	writebyte(HL,bytetemp);
      }
      tstates += 10;
      break;
    case 0x36:		/* LD (HL),nn   MVI M,B2 */
      writebyte(HL,readbyte(PC++));
      tstates += 7;
      break;
    case 0x37:		/* SCF          STC */
      F = F | FLAG_C;
      tstates += 4;
      break;
    case 0x38:		/* JR C,offset *NOP */
      break;
    case 0x39:		/* ADD HL,SP    DAD SP */
      ADD16(HL,SP);
      tstates += 10;
      break;
    case 0x3a:		/* LD A,(nnnn)  LDA B2B3 */
      {
	WORD wordtemp;
	wordtemp = readbyte(PC++);
	wordtemp|= ( readbyte(PC++) << 8 );
	A=readbyte(wordtemp);
      }
      tstates += 13;
      break;
    case 0x3b:		/* DEC SP       DCX SP */
      SP--;
      tstates += 5;
      break;
    case 0x3c:		/* INC A        INR A */
      INR(A);
      tstates += 5;
      break;
    case 0x3d:		/* DEC A        DCR A */
      DCR(A);
      tstates += 5;
      break;
    case 0x3e:		/* LD A,nn      MVI A,B2 */
      A=readbyte(PC++);
      tstates += 7;
      break;
    case 0x3f:		/* CCF          CMC */
      F = ( F & ( FLAG_P | FLAG_Z | FLAG_S ) ) |
	( ( F & FLAG_C ) ? FLAG_AC : FLAG_C ) | ( A & ( FLAG_3 | FLAG_5 ) );
      tstates += 4;
      break;
    case 0x40:		/* LD B,B       MOV B,B */
      tstates += 5;
      break;
    case 0x41:		/* LD B,C       MOV B,C */
      B=C;
      tstates += 5;
      break;
    case 0x42:		/* LD B,D       MOV B,D */
      B=D;
      tstates += 5;
      break;
    case 0x43:		/* LD B,E       MOV B,E */
      B=E;
      tstates += 5;
      break;
    case 0x44:		/* LD B,H       MOV B,H */
      B=H;
      tstates += 5;
      break;
    case 0x45:		/* LD B,L       MOV B,L */
      B=L;
      tstates += 5;
      break;
    case 0x46:		/* LD B,(HL)    MOV B,M */
      B=readbyte(HL);
      tstates += 7;
      break;
    case 0x47:		/* LD B,A       MOV B,A */
      B=A;
      tstates += 5;
      break;
    case 0x48:		/* LD C,B       MOV C,B */
      C=B;
      tstates += 5;
      break;
    case 0x49:		/* LD C,C       MOV C,C */
      tstates += 5;
      break;
    case 0x4a:		/* LD C,D       MOV C,D */
      C=D;
      tstates += 5;
      break;
    case 0x4b:		/* LD C,E       MOV C,E */
      C=E;
      tstates += 5;
      break;
    case 0x4c:		/* LD C,H       MOV C,H */
      C=H;
      tstates += 5;
      break;
    case 0x4d:		/* LD C,L       MOV C,L */
      C=L;
      tstates += 5;
      break;
    case 0x4e:		/* LD C,(HL)    MOV C,M */
      C=readbyte(HL);
      tstates += 7;
      break;
    case 0x4f:		/* LD C,A       MOV C,A */
      C=A;
      tstates += 5;
      break;
    case 0x50:		/* LD D,B       MOV D,B */
      D=B;
      tstates += 5;
      break;
    case 0x51:		/* LD D,C       MOV D,C */
      D=C;
      tstates += 5;
      break;
    case 0x52:		/* LD D,D       MOV D,D */
      tstates += 5;
      break;
    case 0x53:		/* LD D,E       MOV D,E */
      D=E;
      tstates += 5;
      break;
    case 0x54:		/* LD D,H       MOV D,H */
      D=H;
      tstates += 5;
      break;
    case 0x55:		/* LD D,L       MOV D,L */
      D=L;
      tstates += 5;
      break;
    case 0x56:		/* LD D,(HL)    MOV D,M */
      D=readbyte(HL);
      tstates += 7;
      break;
    case 0x57:		/* LD D,A       MOV D,A */
      D=A;
      tstates += 5;
      break;
    case 0x58:		/* LD E,B       MOV E,B */
      E=B;
      tstates += 5;
      break;
    case 0x59:		/* LD E,C       MOV E,C */
      E=C;
      tstates += 5;
      break;
    case 0x5a:		/* LD E,D       MOV E,D */
      E=D;
      tstates += 5;
      break;
    case 0x5b:		/* LD E,E       MOV E,E */
      tstates += 5;
      break;
    case 0x5c:		/* LD E,H       MOV E,H */
      E=H;
      tstates += 5;
      break;
    case 0x5d:		/* LD E,L       MOV E,L */
      E=L;
      tstates += 5;
      break;
    case 0x5e:		/* LD E,(HL)    MOV E,M */
      E=readbyte(HL);
      tstates += 7;
      break;
    case 0x5f:		/* LD E,A       MOV E,A */
      E=A;
      tstates += 5;
      break;
    case 0x60:		/* LD H,B       MOV H,B */
      H=B;
      tstates += 5;
      break;
    case 0x61:		/* LD H,C       MOV H,C */
      H=C;
      tstates += 5;
      break;
    case 0x62:		/* LD H,D       MOV H,D */
      H=D;
      tstates += 5;
      break;
    case 0x63:		/* LD H,E       MOV H,E */
      H=E;
      tstates += 5;
      break;
    case 0x64:		/* LD H,H       MOV H,H */
      tstates += 5;
      break;
    case 0x65:		/* LD H,L       MOV H,L */
      H=L;
      tstates += 5;
      break;
    case 0x66:		/* LD H,(HL)    MOV H,M */
      H=readbyte(HL);
      tstates += 7;
      break;
    case 0x67:		/* LD H,A       MOV H,A */
      H=A;
      tstates += 5;
      break;
    case 0x68:		/* LD L,B       MOV L,B */
      L=B;
      tstates += 5;
      break;
    case 0x69:		/* LD L,C       MOV L,C */
      L=C;
      tstates += 5;
      break;
    case 0x6a:		/* LD L,D       MOV L,D */
      L=D;
      tstates += 5;
      break;
    case 0x6b:		/* LD L,E       MOV L,E */
      L=E;
      tstates += 5;
      break;
    case 0x6c:		/* LD L,H       MOV L,H */
      L=H;
      tstates += 5;
      break;
    case 0x6d:		/* LD L,L       MOV L,L */
      tstates += 5;
      break;
    case 0x6e:		/* LD L,(HL)    MOV L,M */
      L=readbyte(HL);
      tstates += 7;
      break;
    case 0x6f:		/* LD L,A       MOV L,A */
      L=A;
      tstates += 5;
      break;
    case 0x70:		/* LD (HL),B    MOV M,B */
      writebyte(HL,B);
      tstates += 7;
      break;
    case 0x71:		/* LD (HL),C    MOV M,C */
      writebyte(HL,C);
      tstates += 7;
      break;
    case 0x72:		/* LD (HL),D    MOV M,D */
      writebyte(HL,D);
      tstates += 7;
      break;
    case 0x73:		/* LD (HL),E    MOV M,E */
      writebyte(HL,E);
      tstates += 7;
      break;
    case 0x74:		/* LD (HL),H    MOV M,H */
      writebyte(HL,H);
      tstates += 7;
      break;
    case 0x75:		/* LD (HL),L    MOV M,L */
      writebyte(HL,L);
      tstates += 7;
      break;
    case 0x76:		/* HALT         HLT */
      i8080.halted=1;
      tstates += 4;
      break;
    case 0x77:		/* LD (HL),A    MOV M,A */
      writebyte(HL,A);
      tstates += 7;
      break;
    case 0x78:		/* LD A,B       MOV A,B */
      A=B;
      tstates += 5;
      break;
    case 0x79:		/* LD A,C       MOV A,C */
      A=C;
      tstates += 5;
      break;
    case 0x7a:		/* LD A,D       MOV A,D */
      A=D;
      tstates += 5;
      break;
    case 0x7b:		/* LD A,E       MOV A,E */
      A=E;
      tstates += 5;
      break;
    case 0x7c:		/* LD A,H       MOV A,H */
      A=H;
      tstates += 5;
      break;
    case 0x7d:		/* LD A,L       MOV A,L */
      A=L;
      tstates += 5;
      break;
    case 0x7e:		/* LD A,(HL)    MOV A,M */
      A=readbyte(HL);
      tstates += 7;
      break;
    case 0x7f:		/* LD A,A       MOV A,A */
      tstates += 5;
      break;
    case 0x80:		/* ADD A,B      ADD B */
      ADD(B);
      tstates += 4;
      break;
    case 0x81:		/* ADD A,C      ADD C */
      ADD(C);
      tstates += 4;
      break;
    case 0x82:		/* ADD A,D      ADD D */
      ADD(D);
      tstates += 4;
      break;
    case 0x83:		/* ADD A,E      ADD E */
      ADD(E);
      tstates += 4;
      break;
    case 0x84:		/* ADD A,H      ADD H */
      ADD(H);
      tstates += 4;
      break;
    case 0x85:		/* ADD A,L      ADD L */
      ADD(L);
      tstates += 4;
      break;
    case 0x86:		/* ADD A,(HL)   ADD M */
      {
	BYTE bytetemp=readbyte(HL);
	ADD(bytetemp);
      }
      tstates += 7;
      break;
    case 0x87:		/* ADD A,A      ADD A */
      ADD(A);
      tstates += 4;
      break;
    case 0x88:		/* ADC A,B      ADC B */
      ADC(B);
      tstates += 4;
      break;
    case 0x89:		/* ADC A,C      ADC C */
      ADC(C);
      tstates += 4;
      break;
    case 0x8a:		/* ADC A,D      ADC D */
      ADC(D);
      tstates += 4;
      break;
    case 0x8b:		/* ADC A,E      ADC E */
      ADC(E);
      tstates += 4;
      break;
    case 0x8c:		/* ADC A,H      ADC H */
      ADC(H);
      tstates += 4;
      break;
    case 0x8d:		/* ADC A,L      ADC L */
      ADC(L);
      tstates += 4;
      break;
    case 0x8e:		/* ADC A,(HL)   ADC M */
      {
	BYTE bytetemp=readbyte(HL);
	ADC(bytetemp);
      }
      tstates += 7;
      break;
    case 0x8f:		/* ADC A,A      ADC A */
      ADC(A);
      tstates += 4;
      break;
    case 0x90:		/* SUB A,B      SUB B */
      SUB(B);
      tstates += 4;
      break;
    case 0x91:		/* SUB A,C      SUB C */
      SUB(C);
      tstates += 4;
      break;
    case 0x92:		/* SUB A,D      SUB D */
      SUB(D);
      tstates += 4;
      break;
    case 0x93:		/* SUB A,E      SUB E */
      SUB(E);
      tstates += 4;
      break;
    case 0x94:		/* SUB A,H      SUB H */
      SUB(H);
      tstates += 4;
      break;
    case 0x95:		/* SUB A,L      SUB L */
      SUB(L);
      tstates += 4;
      break;
    case 0x96:		/* SUB A,(HL)   SUB M */
      {
	BYTE bytetemp=readbyte(HL);
	SUB(bytetemp);
      }
      tstates += 7;
      break;
    case 0x97:		/* SUB A,A      SUB A */
      SUB(A);
      tstates += 4;
      break;

/* вычитание с заемом- старший (9) бит находится с CY */

    case 0x98:		/* SBC A,B      SBB B */
      SBB(B);
      tstates += 4;
      break;
    case 0x99:		/* SBC A,C      SBB C */
      SBB(C);
      tstates += 4;
      break;
    case 0x9a:		/* SBC A,D      SBB D */
      SBB(D);
      tstates += 4;
      break;
    case 0x9b:		/* SBC A,E      SBB E */
      SBB(E);
      tstates += 4;
      break;
    case 0x9c:		/* SBC A,H      SBB H */
      SBB(H);
      tstates += 4;
      break;
    case 0x9d:		/* SBC A,L      SBB L */
      SBB(L);
      tstates += 4;
      break;
    case 0x9e:		/* SBC A,(HL)   SBB M */
      {
	BYTE bytetemp=readbyte(HL);
	SBB(bytetemp);
      }
      tstates += 7;
      break;
    case 0x9f:		/* SBC A,A      SBB A */
      SBB(A);
      tstates += 4;
      break;
    case 0xa0:		/* AND A,B      ANA B */
      AND(B);
      tstates += 4;
      break;
    case 0xa1:		/* AND A,C      ANA C */
      AND(C);
      tstates += 4;
      break;
    case 0xa2:		/* AND A,D      ANA D */
      AND(D);
      tstates += 4;
      break;
    case 0xa3:		/* AND A,E      ANA E */
      AND(E);
      tstates += 4;
      break;
    case 0xa4:		/* AND A,H      ANA H */
      AND(H);
      tstates += 4;
      break;
    case 0xa5:		/* AND A,L      ANA L */
      AND(L);
      tstates += 4;
      break;
    case 0xa6:		/* AND A,(HL)   ANA M */
      {
	BYTE bytetemp=readbyte(HL);
	AND(bytetemp);
      }
      tstates += 7;
      break;
    case 0xa7:		/* AND A,A      ANA A */
      AND(A);
      tstates += 4;
      break;
    case 0xa8:		/* XOR A,B      XRA B */
      XOR(B);
      tstates += 4;
      break;
    case 0xa9:		/* XOR A,C      XRA C */
      XOR(C);
      tstates += 4;
      break;
    case 0xaa:		/* XOR A,D      XRA D */
      XOR(D);
      tstates += 4;
      break;
    case 0xab:		/* XOR A,E      XRA E */
      XOR(E);
      tstates += 4;
      break;
    case 0xac:		/* XOR A,H      XRA H */
      XOR(H);
      tstates += 4;
      break;
    case 0xad:		/* XOR A,L      XRA L */
      XOR(L);
      tstates += 4;
      break;
    case 0xae:		/* XOR A,(HL)   XRA M */
      {
	BYTE bytetemp=readbyte(HL);
	XOR(bytetemp);
      }
      tstates += 7;
      break;
    case 0xaf:		/* XOR A,A      XRA A */
      XOR(A);
      tstates += 4;
      break;
    case 0xb0:		/* OR A,B       ORA B */
      OR(B);
      tstates += 4;
      break;
    case 0xb1:		/* OR A,C       ORA C */
      OR(C);
      tstates += 4;
      break;
    case 0xb2:		/* OR A,D       ORA D */
      OR(D);
      tstates += 4;
      break;
    case 0xb3:		/* OR A,E       ORA E */
      OR(E);
      tstates += 4;
      break;
    case 0xb4:		/* OR A,H       ORA H */
      OR(H);
      tstates += 4;
      break;
    case 0xb5:		/* OR A,L       ORA L */
      OR(L);
      tstates += 4;
      break;
    case 0xb6:		/* OR A,(HL)    ORA M */
      {
	BYTE bytetemp=readbyte(HL);
	OR(bytetemp);
      }
      tstates += 7;
      break;
    case 0xb7:		/* OR A,A       ORA A */
      OR(A);
      tstates += 4;
      break;
    case 0xb8:		/* CP B         CMP B */
      CP(B);
      tstates += 4;
      break;
    case 0xb9:		/* CP C         CMP C */
      CP(C);
      tstates += 4;
      break;
    case 0xba:		/* CP D         CMP D */
      CP(D);
      tstates += 4;
      break;
    case 0xbb:		/* CP E         CMP E */
      CP(E);
      tstates += 4;
      break;
    case 0xbc:		/* CP H         CMP H */
      CP(H);
      tstates += 4;
      break;
    case 0xbd:		/* CP L         CMP L */
      CP(L);
      tstates += 4;
      break;
    case 0xbe:		/* CP (HL)      CMP M */
      {
	BYTE bytetemp=readbyte(HL);
	CP(bytetemp);
      }
      tstates += 7;
      break;
    case 0xbf:		/* CP A         CMP A */
      CP(A);
      tstates += 4;
      break;
    case 0xc0:		/* RET NZ       *NOP */
      break;
    case 0xc1:		/* POP BC       POP B */
      POP16(C,B);
      tstates += 10;
      break;
    case 0xc2:		/* JP NZ,nnnn   JNZ B2B3 */
      if ( ! ( F & FLAG_Z ) ) { JP(); }
      else PC+=2;
      tstates += 10;
      break;
    case 0xc3:		/* JP nnnn      JMP B2B3 */
      JP();
      tstates += 10;
      break;
    case 0xc4:		/* CALL NZ,nnnn *NOP */
      break;
    case 0xc5:		/* PUSH BC      PUSH B */
      tstates++;
      PUSH16(C,B);
      tstates += 11;
      break;
    case 0xc6:		/* ADD A,nn     ADI B2 */
      {
	BYTE bytetemp=readbyte(PC++);
	ADD(bytetemp);
      }
      tstates += 7;
      break;
    case 0xc7:		/* RST 00       RST 0 */
      RST(0x00);
      tstates += 11;
      break;
    case 0xc8:		/* RET Z        *NOP */
      break;
    case 0xc9:		/* RET          RET */
      RET();
      tstates += 10;
      break;
    case 0xca:		/* JP Z,nnnn    JZ B2B3 */
      if ( F & FLAG_Z ) { JP(); }
      else PC+=2;
      tstates += 10;
      break;
    case 0xcb:		/* CBxx opcodes *NOP */
      break;
    case 0xcc:		/* CALL Z,nnnn  *NOP */
      break;
    case 0xcd:		/* CALL nnnn    CALL B2B3 */
      CALL();
      tstates += 17;
      break;
    case 0xce:		/* ADC A,nn     ACI B2 */
      {
	BYTE bytetemp=readbyte(PC++);
	ADC(bytetemp);
      }
      tstates += 7;
      break;
    case 0xcf:		/* RST 8        RST 1 */
      RST(0x08);
      tstates += 11;
      break;
    case 0xd0:		/* RET NC       *NOP */
      break;
    case 0xd1:		/* POP DE       POP D */
      POP16(E,D);
      tstates += 10;
      break;
    case 0xd2:		/* JP NC,nnnn   JNC B2B3 */
      if ( ! ( F & FLAG_C ) ) { JP(); }
      else PC+=2;
      tstates += 10;
      break;
    case 0xd3:		/* OUT (nn),A   OUT B2 */
      {
	WORD outtemp;
        outtemp = readbyte( PC++ );
	writeport( outtemp , A );
      }
      tstates += 10;
      break;
    case 0xd4:		/* CALL NC,nnnn *NOP */
      break;
    case 0xd5:		/* PUSH DE      PUSH D */
      PUSH16(E,D);
      tstates += 11;
      break;
    case 0xd6:		/* SUB nn       SUI B2 */
      {
	BYTE bytetemp=readbyte(PC++);
	SUB(bytetemp);
      }
      tstates += 7;
      break;
    case 0xd7:		/* RST 10       RST 2 */
      RST(0x10);
      tstates += 11;
      break;
    case 0xd8:		/* RET C        *NOP */
      break;
    case 0xd9:		/* EXX          *NOP */
      break;
    case 0xda:		/* JP C,nnnn    JC B2B3 */
      if ( F & FLAG_C ) { JP(); }
      else PC+=2;
      tstates += 10;
      break;
    case 0xdb:		/* IN A,(nn)    IN B2 */
      {
        WORD intemp;
	intemp = readbyte( PC++ );
        A = readport( intemp );
        F = ( F & FLAG_C) | sz53p_table[(A)];
      }
      tstates += 10;
      break;
    case 0xdc:		/* CALL C,nnnn  *NOP */
      break;
    case 0xdd:		/* DDxx opcodes *NOP */
      break;
    case 0xde:		/* SBC A,nn     SBI B2 */
      {
	BYTE bytetemp=readbyte(PC++);
	SBB(bytetemp);
      }
      tstates += 7;
      break;
    case 0xdf:		/* RST 18       RST 3 */
      RST(0x18);
      tstates += 11;
      break;
    case 0xe0:		/* RET PO       *NOP */
      break;
    case 0xe1:		/* POP HL       POP H */
      POP16(L,H);
      tstates += 10;
      break;
    case 0xe2:		/* JP PO,nnnn   JPO B2B3 */
      if ( ! ( F & FLAG_P ) ) { JP(); }
      else PC+=2;
      tstates += 10;
      break;
    case 0xe3:		/* EX (SP),HL   XTHL */
      {
	BYTE bytetempl=readbyte(SP), bytetemph=readbyte(SP+1);
	writebyte(SP,L); writebyte(SP+1,H);
	L=bytetempl; H=bytetemph;
      }
      tstates += 18;
      break;
    case 0xe4:		/* CALL PO,nnnn *NOP */
      break;
    case 0xe5:		/* PUSH HL      PUSH H */
      PUSH16(L,H);
      tstates += 11;
      break;
    case 0xe6:		/* AND nn       ANI B2 */
      {
	BYTE bytetemp=readbyte(PC++);
	AND(bytetemp);
      }
      tstates += 7;
      break;
    case 0xe7:		/* RST 20       RST 4 */
      RST(0x20);
      tstates += 11;
      break;
    case 0xe8:		/* RET PE       *NOP */
      break;
    case 0xe9:		/* JP HL        PCHL */
      PC=HL;		/* NB: NOT INDIRECT! */
      tstates += 5;
      break;
    case 0xea:		/* JP PE,nnnn   JPE B2B3 */
      if ( F & FLAG_P ) { JP(); }
      else PC+=2;
      tstates += 10;
      break;
    case 0xeb:		/* EX DE,HL     XCHG */
      {
	WORD wordtemp=DE; DE=HL; HL=wordtemp;
      }
      tstates += 4;
      break;
    case 0xec:		/* CALL PE,nnnn *NOP */
      break;
    case 0xed:		/* EDxx opcodes *NOP */
      break;
    case 0xee:		/* XOR A,nn     XRI B2 */
      {
	BYTE bytetemp=readbyte(PC++);
	XOR(bytetemp);
      }
      tstates += 7;
      break;
    case 0xef:		/* RST 28       RST 5 */
      RST(0x28);
      tstates += 11;
      break;
    case 0xf0:		/* RET P        *NOP */
      break;
    case 0xf1:		/* POP AF       POP PSW */
      POP16(F,A);
      tstates += 10;
      break;
    case 0xf2:		/* JP P,nnnn    JP B2B3 */
      if ( ! ( F & FLAG_S ) ) { JP(); }
      else PC+=2;
      tstates += 10;
      break;
    case 0xf3:		/* DI           DI */
      i8080.inte = 0;  /* запрещаем прерывания */
      tstates += 4;
      break;
    case 0xf4:		/* CALL P,nnnn *NOP */
      break;
    case 0xf5:		/* PUSH AF      PUSH PSW */
      PUSH16(F,A);
      tstates += 11;
      break;
    case 0xf6:		/* OR nn        ORI B2 */
      {
	BYTE bytetemp=readbyte(PC++);
	OR(bytetemp);
      }
      tstates += 7;
      break;
    case 0xf7:		/* RST 30       RST 6 */
      RST(0x30);
      tstates += 11;
      break;
    case 0xf8:		/* RET M        *NOP */
      break;
    case 0xf9:		/* LD SP,HL     SPHL */
      SP=HL;
      tstates += 5;
      break;
    case 0xfa:		/* JP M,nnnn    JM B2B3 */
      if ( F & FLAG_S ) { JP(); }
      else PC+=2;
      tstates += 10;
      break;
    case 0xfb:		/* EI           EI */
      i8080.inte = 1;  /* разрешаем прерывания */
      tstates += 4;
      break;
    case 0xfc:		/* CALL M,nnnn  *NOP */
      break;
    case 0xfd:		/* FDxx opcodes *NOP */
      break;
    case 0xfe:		/* CP nn        CPI B2 */
      {
	BYTE bytetemp=readbyte(PC++);
	CP(bytetemp);
      }
      tstates += 7;
      break;
    case 0xff:		/* RST 38       RST 7 */
      RST(0x38);
      tstates += 11;
      break;
    }	/* закрывающая switch(opcode) { */

  /* Вызываем зарегистрированые плагинами функции обраного вызова,
     информирующие их о выполнении процессором очередной команды
   */
  for (n = 0; n < num_callbacks; n++)
    callback[n]();
}


