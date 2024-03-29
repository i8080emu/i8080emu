#ifndef _I8080_BREAK_H
#define _I8080_BREAK_H

void break_init(void);

/* ������������� ��� ������� ����� �������� � �������� ������
   ���������� �� ��������
 */
void break_toggle(WORD addr);

/* ���������� ����� ��������� ����� �������� � �������� ������
   ������������, ��������, ��� StepOver �������� �����
   �������� � ������ ������������ ����.
 */
void break_setbreak(WORD addr, int count);

/* ����������, ����� ��� ���������� ��������� �������� ��
 ��� ����� ��������. ������� ������ ����� ��, ���� � �����
 ��������� ����� ������������
 */
void break_catch(WORD addr);

// ���������� true ���� �� ���� ����� ����������� ����� ��������
int break_isbreak(WORD addr);

#endif

