#ifndef _I8080_ASM_H
#define _I8080_ASM_H

#define FMAXLEN 63      // максимальное число символов в поле
#define MAXLABELCHARS 8 // максимальное число символов в имени метки

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
  BYTE *mem;      /* указатель на виртуальную память, в которую транслировать */
  WORD addr;      /* текущий адрес компиляции (он же $ в ассемблерном тексте) */
  int org;        /* начальный адрес компиляции */
  int mult_org;   /* флаг установлен, если больше чем одна директива org */
  int codegen;    /* 0-первый проход, 1-второй проход, генерация кода включена */
  int prep_only;  /* только препроцесс (для консольной версии */
  label_t *labels;  /* список меток */
} ASMSESSION;

#ifdef __cplusplus
 extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

/* Возвращает описание ошибки по ее коду */
char* asm_get_error_text (asmerr_t errcode);

/* Начало процесса ассемблирование, инициализация структуры */
void asm_start(ASMSESSION *asmsess, void *virtual_memory);

/* Обработка одной строки */
asmerr_t asm_process(ASMSESSION *asmsess, char *line);

/* Начало второго прохода (генерация кода) */
void asm_pass2(ASMSESSION *asmsess);

/* Конец ассемблирования */
void asm_end(ASMSESSION *asmsess);

#ifdef __cplusplus
 }
#endif  /* __cplusplus */

#endif