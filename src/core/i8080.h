#ifndef _I8080_I8080_H
#define _I8080_I8080_H

#include "plugin.h"

/* Объединение предоставляет доступ к регистровой паре как к слову или байтам */
typedef union {
  struct { BYTE l,h; } b;
  WORD w;
} regpair;

/* процессор */
typedef struct {
  /* регистры */
  regpair af,bc,de,hl;
  regpair sp,pc;

  /* состояние прерываний */
  BYTE inte;  /* тригер разрешения прерывания */
  int halted;  /* */
} processor;

extern processor i8080;

#ifdef __cplusplus
 extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

/* Начальная инициализация модуля эмуляции процессора (при старте программы) */
void i8080_init(void);

/* Сброс процессора */
void i8080_reset(void);

/* Обработка прерывания */
void i8080_interrupt(void);

/* Регистрация функции обратного вызова */
int i8080_register_callback(i8080step_callback_t cb);

/* Выполнение текущей команды */
void i8080_do_opcode(void);

#ifdef __cplusplus
 }
#endif  /* __cplusplus */

#endif

