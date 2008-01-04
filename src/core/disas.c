/* ���� ���� �������� ���������� ����������,
*  ������������ ��������� ���������������� ���� ������
*  ��������������� �580.
*/
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "disas.h"
#include "mpsys.h"
#include "gui.h"


// �������� ��������� (��� ������ � ������������ ����������)
char *reg_name[8] = { "B","C","D","E","H","L","M","A" };

// �������� ����������� ���
char *pair_name[4] = { "B","D","H","SP" };

// �������� ����������� ��� ��� push � pop
char *pair_pushpop[4] = { "B","D","H","PSW" };

// �������� �������
char *ccc_name[8] = {"NZ","Z","NC","C","PO","PE","P","M"};

typedef enum {
OP_NONE,      // ��� ��������
OP_REG,       // ����������� ������� (���� ��� op1 �� DDD ���� op2 �� SSS)
OP_REGPAIR,   // ����������� ���� ��� ���� ��������
OP_PUSHPOP,   // ����������� ���� ��� push � pop
OP_PAIRBD,    // ����������� ���� B ��� D
OP_CCC,       // �������
OP_RSTN,      // ����� �������� rst
OP_BYTE,      // ������������ �������
OP_WORD,      // �������-�����
OP_ADDR       // ����� (��� ������ �������� � ������)
}operand_t;

struct dte_tag {
BYTE mask;
BYTE code;
char *mnem;
operand_t op1;
operand_t op2;
};


/* ���� �580 � ������� ���������� 2 (�� ����������� HLT, ����� ��������
   ���������� MOV M,M
 */
struct dte_tag decode_tbl[] = {
  {0xff, 0x76, "HLT", OP_NONE, OP_NONE},

  {0xc0, 0x40, "MOV", OP_REG, OP_REG},

  {0xff, 0xe9, "PCHL", OP_NONE, OP_NONE},
  {0xff, 0xf9, "SPHL", OP_NONE, OP_NONE},
  {0xff, 0xeb, "XCHG", OP_NONE, OP_NONE},
  {0xff, 0xe3, "XTHL", OP_NONE, OP_NONE},

  {0xc7, 0x06, "MVI", OP_REG, OP_BYTE},
  {0xcf, 0x01, "LXI", OP_REGPAIR, OP_WORD},
  {0xcf, 0xc5, "PUSH", OP_REGPAIR, OP_NONE},
  {0xcf, 0xc1, "POP", OP_REGPAIR, OP_NONE},

  {0xff, 0x32, "STA", OP_NONE, OP_WORD},
  {0xff, 0x3a, "LDA", OP_NONE, OP_WORD},
  {0xef, 0x02, "STAX",OP_PAIRBD, OP_NONE},
  {0xef, 0x0a, "LDAX", OP_PAIRBD, OP_NONE},

  {0xff, 0x22, "SHLD", OP_NONE, OP_WORD},
  {0xff, 0x2a, "LHLD", OP_NONE, OP_WORD},

  {0xf8, 0x80, "ADD", OP_NONE, OP_REG},
  {0xff, 0xc6, "ADI", OP_NONE, OP_BYTE},
  {0xf8, 0x88, "ADC", OP_NONE, OP_REG},
  {0xff, 0xce, "ACI", OP_NONE, OP_BYTE},
  {0xf8, 0x90, "SUB", OP_NONE, OP_REG},
  {0xff, 0xd6, "SUI", OP_NONE, OP_BYTE},
  {0xf8, 0x98, "SBB", OP_NONE, OP_REG},
  {0xff, 0xde, "SBI", OP_NONE, OP_BYTE},

  {0xff, 0x27, "DAA", OP_NONE, OP_NONE},

  {0xf8, 0xb8, "CMP", OP_NONE, OP_REG},
  {0xff, 0xfe, "CPI", OP_NONE, OP_BYTE},

  {0xcf, 0x09, "DAD", OP_REGPAIR, OP_NONE},

  {0xf8, 0xa0, "ANA", OP_NONE, OP_REG},
  {0xff, 0xe6, "ANI", OP_NONE, OP_BYTE},

  {0xf8, 0xb0, "ORA", OP_NONE, OP_REG},
  {0xff, 0xf6, "ORI", OP_NONE, OP_BYTE},
  {0xf8, 0xa8, "XRA", OP_NONE, OP_REG},
  {0xff, 0xee, "XRI", OP_NONE, OP_BYTE},

  {0xff, 0x07, "RLC", OP_NONE, OP_NONE},
  {0xff, 0x0f, "RRC", OP_NONE, OP_NONE},
  {0xff, 0x17, "RAL", OP_NONE, OP_NONE},
  {0xff, 0x1f, "RAR", OP_NONE, OP_NONE},
  {0xff, 0x2f, "CMA", OP_NONE, OP_NONE},
  {0xff, 0x3f, "CMC", OP_NONE, OP_NONE},
  {0xff, 0x37, "STS", OP_NONE, OP_NONE},

  {0xc7, 0x04, "INR", OP_REG, OP_NONE},
  {0xcf, 0x03, "INX", OP_REGPAIR, OP_NONE},
  {0xc7, 0x05, "DCR", OP_REG, OP_NONE},
  {0xcf, 0x0b, "DCX", OP_REGPAIR, OP_NONE},

  {0xff, 0xc3, "JMP", OP_NONE, OP_ADDR},
  {0xc7, 0xc2, "J", OP_CCC, OP_ADDR},
  {0xff, 0xcd, "CALL", OP_NONE, OP_ADDR},
  {0xc7, 0xc4, "C", OP_CCC, OP_ADDR},
  {0xff, 0xc9, "RET", OP_NONE, OP_NONE},
  {0xc7, 0xc0, "R", OP_CCC, OP_NONE},

  {0xff, 0x00, "NOP", OP_NONE, OP_NONE},
  {0xff, 0xfb, "EI", OP_NONE, OP_NONE},
  {0xff, 0xf3, "DI", OP_NONE, OP_NONE},

  {0xff, 0xdb, "IN", OP_NONE, OP_BYTE},
  {0xff, 0xd3, "OUT", OP_NONE, OP_BYTE},

  {0xc7, 0xc7, "RST", OP_RSTN, OP_NONE},
  {0x00, 0x00, "", OP_NONE, OP_NONE},
 };


/* ����������������� ������� �� �������� ������.
   ���������� ������ ������� ������� � ������
   � ����� ����� ��������� �� ��������� lpdr.
 */
int disas_addr(WORD addr, DISASRESULT *lpdr)
{
  BYTE opcode;
  char *op;
  int i;

  // ����� ������
  sprintf(lpdr->addr,"%04X",addr);

  opcode = MEM[addr++];

  // �������� ������ � ���������� ��� ������
  lpdr->ops[0] = '\0';

  lpdr->needloop = 0;

  for (i=0; decode_tbl[i].mask != 0x00; i++) {

    if ((opcode & decode_tbl[i].mask) != decode_tbl[i].code)
      continue;

    // �������� ����� ���������
    strcpy(lpdr->mnem, decode_tbl[i].mnem);

    switch (decode_tbl[i].op1)
    {
      case OP_NONE:
        break;
      case OP_REG:
        // ������ ����������� ������� ���������� (DDD)
        strcpy(lpdr->ops, reg_name[(opcode & 0x38) >> 3]);
        break;
      case OP_REGPAIR:
        strcpy(lpdr->ops, pair_name[(opcode & 0x30) >> 4]);
        break;
      case OP_PUSHPOP:
        strcpy(lpdr->ops, pair_pushpop[(opcode & 0x30) >> 4]);
        break;
      case OP_PAIRBD:
        strcpy(lpdr->ops, pair_name[(opcode & 0x30) >> 4]);
        break;
      case OP_CCC:
        // �.�. ����� ��������� ��� ����������, �������� ������ �������� � ����� ����� �������
        strcpy(strchr(lpdr->mnem,0), ccc_name[(opcode & 0x38) >> 3]);
        break;
      case OP_RSTN:
        lpdr->ops[0] = '0' + ((opcode & 0x38) >> 3);
        lpdr->ops[1] = '\0';
        break;
      case OP_BYTE:
      case OP_WORD:
        ui_error(0,"������ � ������� decode_tbl: OP_WORD � OP_BYTE ����� ���� ������ ������ ���������!");
      break;
    }

    // ���� �� ����� ��� ���-�� �������� � ��������
    if (lpdr->ops[0] != '\0') {
      op = strchr(lpdr->ops,0);
    } else
      op = NULL;

    switch (decode_tbl[i].op2)
    {
      case OP_NONE:
        return 1;
      case OP_REG:
        if (op)
          *op++ = ',';
        else
          op = lpdr->ops;
        // ������ ����������� �������- �������� (SSS)
        strcpy(op, reg_name[opcode & 0x07]);
        return 1;
      case OP_BYTE:
        if (op)
          *op++ = ',';
        else
          op = lpdr->ops;
        {
          BYTE b;
          b = MEM[addr];
          sprintf(op,"%02X", (int)b);
        }
        return 2;
      case OP_WORD:
      case OP_ADDR:
        if (op)
          *op++ = ',';
        else
          op = lpdr->ops;
        {
          WORD w;
          w = MEM[addr++];
          w |= MEM[addr] << 8;
          sprintf(op,"%04X", (int)w);
          /* ���� ������� ������� �����, �� ������������� ���� */
          if (decode_tbl[i].op2 == OP_ADDR) {
            lpdr->needloop = 1;
            lpdr->loopaddr = w;
          }
        }
        return 3;

      case OP_REGPAIR:
      case OP_PAIRBD:
      case OP_CCC:
      case OP_RSTN:
      case OP_PUSHPOP:
      default:
        ui_error(0,"������ � ������� decode_tbl: \
        OP_REGPAIR, OP_PAIRBD, OP_CCC, OP_RSTN, OP_PUSPPOP �� ����� ���� ������ ����������! \
        (� ������ %d ������ �������� %d", i, decode_tbl[i].op2);
      return 1;
    }

  } /* for (i=0; ....*/

  // ���� �� ����� � �������
  strcpy(lpdr->mnem, "*NOP");
  return 1;
}






#if 0

/* ����������������� ������� �� �������� ������.
   ���������� ������ ������� ������� � ������
   � ����� ����� ��������� �� ��������� lpdr.
 */
int disas_addr(WORD addr, DISASRESULT *lpdr)
{
  BYTE opcode;
  BYTE B2;
  BYTE B3;
  WORD B2B3;

  char *RR;
  char *SSS;
  char *DDD;
  char *CCC;

  // ����� ������
  sprintf(lpdr->loop,"%04X",addr);

  opcode = MEM[addr++];
  B2 = MEM[addr++];
  B3 = MEM[addr];
  B2B3 = B3 << 8 | B2;

  SSS = reg_name[(opcode >> 0) & 0x07];
  DDD = reg_name[(opcode >> 3) & 0x07];
  RR =  pair_name[(opcode >> 4) & 0x03];
  CCC = cond_name[(opcode >> 3) & 0x07];

  strcpy(lpdr->mnem,"UNKN");
  strcpy(lpdr->ops,"");

  // ���������� ������ �� ������ ���� �����
  switch(opcode&0xc0)
  {
    /*  00?? ???? */
    case 0x00:

      // �������� �������� ���������
      if ((opcode & 0xc7) == 0x06) {
        // 00 DDD 110   MVI rd,B2
        strcpy(lpdr->mnem, "MVI");
        sprintf(lpdr->ops,"%s,%X", DDD, B2);
        return 2;
      }

      // �������� ����������� ���� ���������
      else if((opcode & 0xcf) == 0x01) {
        // 00 RR 0001  LXI rp,B2B3
        strcpy(lpdr->mnem, "LXI");
        sprintf(lpdr->ops,"%s,%X", RR, B2B3);
        return 3;
      }

      // ��������� ��������
      else if((opcode & 0xc7) == 0x04) {
        // 00 DDD 100  INR rd
        strcpy(lpdr->mnem, "INR");
        sprintf(lpdr->ops,"%s", DDD);   // ����������� �������
        return 1;
      }

      // ��������� ��������
      else if((opcode & 0xc7) == 0x05) {
        // 00 DDD 101  DCR rd
        strcpy(lpdr->mnem, "DCR");
        sprintf(lpdr->ops,"%s",DDD);    // ����������� �������
        return 1;
      }

      // ��������� ����������� ����
      else if((opcode & 0xcf) == 0x03) {
        // 00 RR 0011  INX rp
        strcpy(lpdr->mnem, "INX");
        sprintf(lpdr->ops,"%s", RR);
        return 1;
      }

      // ��������� ����������� ����
      else if((opcode & 0xcf) == 0x0b) {
        // 00 RR 1011  DCX rp
        strcpy(lpdr->mnem, "DCX");
        sprintf(lpdr->ops,"%s", RR);
        return 1;
      }

      // �������� HL � ����������� �����
      else if((opcode & 0xcf) == 0x09) {
        // 00 RR 1001  DAD rp
        strcpy(lpdr->mnem, "DAD");
        sprintf(lpdr->ops,"%s", RR);
        return 1;
      }

      // ������ ������� �� 00?? ???? (��� ����������� ���������)
      else {
        switch (opcode)
        {
          case 0x00:
            // 0000 0000 NOP
            strcpy(lpdr->mnem, "NOP");
            return 1;
          case 0x32:
            // 0011 0010  STA ,Adr
            strcpy(lpdr->mnem, "STA");
            sprintf(lpdr->ops,"%X", B2B3);
            return 3;
          case 0x3a:
            // 0011 1010  LDA ,Adr
            strcpy(lpdr->mnem, "LDA");
            sprintf(lpdr->ops,"%X", B2B3);
            return 3;
          case 0x02:
            // 0000 0010 STAX B
            strcpy(lpdr->mnem, "STAX");
            strcpy(lpdr->ops,"B");
            return 1;
          case 0x12:
            // 0001 0010 STAX D
            strcpy(lpdr->mnem, "STAX");
            strcpy(lpdr->ops,"D");
            return 1;
          case 0x0a:
            // 0000 1010 LDAX B
            strcpy(lpdr->mnem, "LDAX");
            strcpy(lpdr->ops,"B");
            return 1;
          case 0x1a:
            // 0001 1010 LDAX D
            strcpy(lpdr->mnem, "LDAX");
            strcpy(lpdr->ops,"D");
            return 1;
          case 0x22:
            // 0010 0010 SHLD ,Adr
            strcpy(lpdr->mnem, "SHLD");
            sprintf(lpdr->ops,"%X", B2B3);
            return 3;
          case 0x2b:
            // 0010 1010 LHLD ,Adr
            strcpy(lpdr->mnem, "SHLD");
            sprintf(lpdr->ops,"%X", B2B3);
            return 3;
          case 0x27:
            // 0010 0111 DAA
            strcpy(lpdr->mnem, "DAA");
            return 1;
          case 0x07:
            // 0000 0111 RLC
            strcpy(lpdr->mnem, "RLC");
            return 1;
          case 0x0f:
            // 0000 1111 RRC
            strcpy(lpdr->mnem, "RRC");
            return 1;
          case 0x17:
            // 0001 0111 RAL
            strcpy(lpdr->mnem, "RAL");
            return 1;
          case 0x1f:
            // 0001 1111 RAR
            strcpy(lpdr->mnem, "RAR");
            return 1;
          case 0x2f:
            // 0010 1111 CMA
            strcpy(lpdr->mnem, "CMA");
            return 1;
          case 0x3f:
            // 0011 1111 CMC
            strcpy(lpdr->mnem, "CMC");
            return 1;
          case 0x37:
            // 0011 0111 STC
            strcpy(lpdr->mnem, "STC");
            return 1;
        }
      }
      break;

    /*  01?? ???? */
    case 0x40:

      // ��������� ����������
      if (opcode == 0x76) {
        // 0111 0110  HLT
        strcpy(lpdr->mnem, "HLT");
        return 1;
      }

      // 01 DDD SSS - �������������� ���������
      strcpy(lpdr->mnem, "mov");
      sprintf(lpdr->ops,"%s,%s", DDD, SSS);
      return 1;

    /*  10?? ???? */
    case 0x80:
      // ����������� �������� � ������������
      if ((opcode & 0xf8) == 0x80) {
        // 1000 0SSS   ADD rs
        strcpy(lpdr->mnem, "ADD");
        sprintf(lpdr->ops,"%s", SSS);
        return 1;
      }

      // ����������� �������� � ������������ � ���������
      if ((opcode & 0xf8) == 0x88) {
        // 1000 1SSS   AD� rs
        strcpy(lpdr->mnem, "AD�");
        sprintf(lpdr->ops,"%s", SSS);
        return 1;
      }

      // ��������� �������� �� ������������
      else if((opcode & 0xf8) == 0x90) {
        // 1001 0SSS   SUB rs
        strcpy(lpdr->mnem, "SUB");
        sprintf(lpdr->ops,"%s", SSS);
        return 1;
      }

      // ��������� �������� �� ������������ � ���������
      else if((opcode & 0xf8) == 0x98) {
        // 1001 1SSS   SBB rs
        strcpy(lpdr->mnem, "SBB");
        sprintf(lpdr->ops,"%s", SSS);
        return 1;
      }

      // ��������� �������� � �������������
      else if((opcode & 0xf8) == 0x90) {
        // 1011 1SSS   CMP rs
        strcpy(lpdr->mnem, "CMP");
        sprintf(lpdr->ops,"%s", SSS);
        return 1;
      }

      // ��������� "�" �������� � �������������
      else if((opcode & 0xf8) == 0xa0) {
        // 1010 0SSS   ANA rs
        strcpy(lpdr->mnem, "ANA");
        sprintf(lpdr->ops,"%s", SSS);
        return 1;
      }

      // ��������� "���" �������� � �������������
      else if((opcode & 0xf8) == 0xb0) {
        // 1011 0SSS   ORA rs
        strcpy(lpdr->mnem, "ORA");
        sprintf(lpdr->ops,"%s", SSS);
        return 1;
      }

      // ��������� "����������� ���" �������� � �������������
      else if((opcode & 0xf8) == 0xb8) {
        // 1010 1SSS   XRA rs
        strcpy(lpdr->mnem, "XRA");
        sprintf(lpdr->ops,"%s", SSS);
        return 1;
      }
      break;

    /*  11?? ???? */
    case 0xc0:

      // ��������� ����������� ���� � ����
      if ((opcode & 0xcf) == 0xc9) {
        // 11 RR 0101   PUSH rp
        strcpy(lpdr->mnem, "PUSH");
        sprintf(lpdr->ops,"%s", RR);
        return 1;
      }

      // ����� ����������� ���� � ������� �����
      else if((opcode & 0xcf) == 0xc1) {
        // 11 RR 0001   POP rp
        strcpy(lpdr->mnem, "POP");
        sprintf(lpdr->ops,"%s", RR);
        return 1;
      }

      // �������� �������
      else if((opcode & 0xc7) == 0xc2) {
        // 11 CCC 010   Jxx Adr
        sprintf(lpdr->mnem, "J%s", CCC);
        sprintf(lpdr->ops, "%X", B2B3);
        return 3;
      }

      // RST N
      else if((opcode & 0xc7) == 0xc7) {
        // 11 NNN 111
        strcpy(lpdr->mnem, "RST");
        sprintf(lpdr->ops, "%d", (opcode & 0xc7) >> 3);
        return 1;
      }

      // ��������� �������
      else {
        switch (opcode)
        {
          case 0xe9:
            // 1110 1001 PCHL
            strcpy(lpdr->mnem, "PCHL");
            return 1;
          case 0xf9:
            // 1111 1001 SPHL
            strcpy(lpdr->mnem, "SPHL");
            return 1;
          case 0xeb:
            // 1110 1011 XCHG
            strcpy(lpdr->mnem, "XCHG");
            return 1;
          case 0xe3:
            // 1110 0011 XTHL
            strcpy(lpdr->mnem, "XTHL");
            return 1;
          case 0xc6:
            // 1100 0110   ADI B2
            strcpy(lpdr->mnem, "ADI");
            sprintf(lpdr->ops,"%X", B2);
            return 2;
          case 0xd6:
            // 1101 0110   SUI B2
            strcpy(lpdr->mnem, "SUI");
            sprintf(lpdr->ops,"%X", B2);
            return 2;
          case 0xfe:
            // 1111 1110   CPI B2
            strcpy(lpdr->mnem, "CPI");
            sprintf(lpdr->ops,"%X", B2);
            return 2;
          case 0xe6:
            // 1110 0110   ANI B2
            strcpy(lpdr->mnem, "ANI");
            sprintf(lpdr->ops,"%X", B2);
            return 2;
          case 0xf6:
            // 1111 0110   ORI B2
            strcpy(lpdr->mnem, "ORI");
            sprintf(lpdr->ops,"%X", B2);
            return 2;
          case 0xee:
            // 1110 1110   XRI B2
            strcpy(lpdr->mnem, "XRI");
            sprintf(lpdr->ops,"%X", B2);
            return 2;
          case 0xc3:
            // 1100 0011 JMP Adr
            strcpy(lpdr->mnem, "JMP");
            sprintf(lpdr->ops,"%X", B2B3);
            return 3;
          case 0xcd:
            // 1100 1101 CALL Adr
            strcpy(lpdr->mnem, "CALL");
            sprintf(lpdr->ops,"%04X", B2B3);
            return 3;
          case 0xc9:
            // 1100 1001 RET
            strcpy(lpdr->mnem, "RET");
            return 1;
          case 0xdb:
            // 1101 1011 IN ADR
            strcpy(lpdr->mnem, "IN");
            sprintf(lpdr->ops,"%X", B2);
            return 2;
          case 0xd3:
            // 1101 0011 OUT ADR
            strcpy(lpdr->mnem, "OUT");
            sprintf(lpdr->ops,"%X", B2);
            return 2;
          case 0xfb:
            // 1111 1011 EI
            strcpy(lpdr->mnem, "EI");
            return 1;
          case 0xf3:
            // 1111 0011 DI
            strcpy(lpdr->mnem, "DI");
            return 1;
        }
      }
  }
  return 1;
}

#endif



int disas_lenght(WORD a)
{
  DISASRESULT dr;
  return disas_addr(a,&dr);
}



