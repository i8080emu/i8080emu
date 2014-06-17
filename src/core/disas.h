/*
  Объявление интерфейса для дизасемблирования кодов К580.

*/
#ifndef _I8080_DISAS_H
#define _I8080_DISAS_H
//-------------------------------------------------------------------------

typedef struct {
char addr[8];    // адрес в текстовом виде
char needloop;   // нужна ли метка (для команд перехода и вызова)
WORD loopaddr;   // адрес метки, если используется
char mnem[8];    // мнемоника
char ops[16];    // операнды
}DISASRESULT;

#ifdef __cplusplus
 extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

// дизасемблировать код
int disas_addr(WORD addr, DISASRESULT *lpdr);

// только узнать длинну
int disas_lenght(WORD a);

#ifdef __cplusplus
 }
#endif  /* __cplusplus */

//-------------------------------------------------------------------------
#endif


