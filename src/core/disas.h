/*
  ���������� ���������� ��� ����������������� ����� �580.

*/
#ifndef _I8080_DISAS_H
#define _I8080_DISAS_H
//-------------------------------------------------------------------------

typedef struct {
char addr[8];    // ����� � ��������� ����
char needloop;   // ����� �� ����� (��� ������ �������� � ������)
WORD loopaddr;   // ����� �����, ���� ������������
char mnem[8];    // ���������
char ops[16];    // ��������
}DISASRESULT;

#ifdef __cplusplus
 extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

// ���������������� ���
int disas_addr(WORD addr, DISASRESULT *lpdr);

// ������ ������ ������
int disas_lenght(WORD a);

#ifdef __cplusplus
 }
#endif  /* __cplusplus */

//-------------------------------------------------------------------------
#endif


