/* ��������-�������� i8080
*  ���������� ������� ��������
*/
#include <types.h>
#include <string.h>

#include "breakpnt.h"

/* ������� ����� �������� ��� ������� ������ */
static int breaks[65536];

/* ������������� ������� ����� �������� */
void break_init()
{
  setmem(breaks, sizeof(breaks), 0);
}


/* ������������� ��� ������� ����� �������� � �������� ������
   ���������� �� ��������
 */
void break_toggle(WORD addr)
{
  if (breaks[addr] != 0)
    breaks[addr] = 0;
  else
    breaks[addr] = -1;  // ����������� ����� ������������
}


/* ��������� ����� �������� � ������� ������ ������������
   ���� �� ���� ����� ��� ����������� ����� ��������, �� �������� �������
   ����� ������������ � ���. ���� �����������, �� ������ �� ������.
 */
void break_setbreak(WORD addr, int count)
{
  if (breaks[addr] == (-1))
    return;
  else
    breaks[addr] += count;
}


/* ����������, ����� ��� ���������� ��������� �������� ��
 ��� ����� ��������. ������� ������ ����� ��, ���� � �����
 ��������� ����� ������������. ���� �����������, �� ������ �� ������.
 */
void break_catch(WORD addr)
{
  if (breaks[addr] != 0 && breaks[addr] != -1)
    breaks[addr]--;
}

// ���������� true ���� �� ���� ����� ����������� ����� ��������
int break_isbreak(WORD addr)
{
  return breaks[addr];
}


