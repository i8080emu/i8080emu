#ifndef _I8080_I8080_H
#define _I8080_I8080_H

#include "plugin.h"

/* ����������� ������������� ������ � ����������� ���� ��� � ����� ��� ������ */
typedef union {
  struct { BYTE l,h; } b;
  WORD w;
} regpair;

/* ��������� */
typedef struct {
  /* �������� */
  regpair af,bc,de,hl;
  regpair sp,pc;

  /* ��������� ���������� */
  BYTE inte;  /* ������ ���������� ���������� */
  int halted;  /* */
} processor;

extern processor i8080;

#ifdef __cplusplus
 extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

/* ��������� ������������� ������ �������� ���������� (��� ������ ���������) */
void i8080_init(void);

/* ����� ���������� */
void i8080_reset(void);

/* ��������� ���������� */
void i8080_interrupt(void);

/* ����������� ������� ��������� ������ */
int i8080_register_callback(i8080step_callback_t cb);

/* ���������� ������� ������� */
void i8080_do_opcode(void);

#ifdef __cplusplus
 }
#endif  /* __cplusplus */

#endif

