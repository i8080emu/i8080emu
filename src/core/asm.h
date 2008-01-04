#ifndef _I8080_ASM_H
#define _I8080_ASM_H

#define FMAXLEN 63      // ������������ ����� �������� � ����
#define MAXLABELCHARS 8 // ������������ ����� �������� � ����� �����

typedef unsigned char	BYTE;
typedef unsigned short	WORD;

typedef enum {
ASMERR_NOERROR,
ASMERR_LABEL,
ASMERR_MNEM,
ASMERR_BADARG,
ASMERR_ORG,
ASMERR_ONEPRINTSYM,
ASMERR_BAD_DB,
ASMERR_DUPLABEL,
ASMERR_BADSYM,
ASMERR_BADLABEL,
ASMERR_TOOBIG,
ASMERR_EXTRAARG,
ASMERR_BADSYNTAX,
ASMERR_NOT_CLOSED_QUOTE
} asmerr_t;

typedef struct tag_label label_t;
typedef struct tag_label {
  char name[MAXLABELCHARS+1];
  WORD value;
  label_t *next;
} label_t;

typedef struct {
  BYTE *mem;      /* ��������� �� ����������� ������, � ������� ������������� */
  WORD addr;      /* ������� ����� ���������� (�� �� $ � ������������ ������) */
  int org;        /* ��������� ����� ���������� */
  int mult_org;   /* ���� ����������, ���� ������ ��� ���� ��������� org */
  int codegen;    /* 0-������ ������, 1-������ ������, ��������� ���� �������� */
  int prep_only;  /* ������ ���������� (��� ���������� ������ */
  label_t *labels;  /* ������ ����� */
} ASMSESSION;

#ifdef __cplusplus
 extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

/* ���������� �������� ������ �� �� ���� */
char* asm_get_error_text (asmerr_t errcode);

/* ������ �������� ���������������, ������������� ��������� */
void asm_start(ASMSESSION *asmsess, void *virtual_memory);

/* ��������� ����� ������ */
asmerr_t asm_process(ASMSESSION *asmsess, char *line);

/* ������ ������� ������� (��������� ����) */
void asm_pass2(ASMSESSION *asmsess);

/* ����� ��������������� */
void asm_end(ASMSESSION *asmsess);

#ifdef __cplusplus
 }
#endif  /* __cplusplus */

#endif