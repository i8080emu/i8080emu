/* Эмулятор-отладчик i8080
*  Управление точками останова
*/
#include <types.h>
#include <string.h>

#include "breakpnt.h"

/* таблица точек останова для каждого адреса */
static int breaks[65536];

/* инициализация таблицы точек останова */
void break_init()
{
  setmem(breaks, sizeof(breaks), 0);
}


/* устанавливает или снимает точку останова с заданого адреса
   вызывается из оболочки
 */
void break_toggle(WORD addr)
{
  if (breaks[addr] != 0)
    breaks[addr] = 0;
  else
    breaks[addr] = -1;  // бесконечное число срабатываний
}


/* Установка точки останова с заданым числом срабатываний
   Если на этот адрес уже установлена точка останова, то добавить заданое
   число срабатываний к ней. Если бесконечная, то ничего не делать.
 */
void break_setbreak(WORD addr, int count)
{
  if (breaks[addr] == (-1))
    return;
  else
    breaks[addr] += count;
}


/* вызывается, когда при выполнении программа попалась на
 эту точку останова. Функция должна снять ее, если у точки
 кончилось число срабатываний. Если бесконечная, то ничего не делать.
 */
void break_catch(WORD addr)
{
  if (breaks[addr] != 0 && breaks[addr] != -1)
    breaks[addr]--;
}

// возвращает true если на этот адрес установлена точка останова
int break_isbreak(WORD addr)
{
  return breaks[addr];
}


