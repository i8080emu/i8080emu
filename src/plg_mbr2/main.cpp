/*
*  ������������� �������� ������� (���)
*/
#include <vcl.h>
#include <windows.h>
#pragma hdrstop

#include "CfgForm.h"
#include "State.h"

#include "../../kernel/plugin.h"

/* �������� ���������� */
char description[] = "������������� �������� ������� (mbr.dll)";

/* ����� �����, � �������� ���������� ���������� */
int mbr_port;

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
        return 1;
}
//---------------------------------------------------------------------------

int InitWindow()
{
  MbrStateForm = new TMbrStateForm(NULL);
  MbrStateForm->Show();

  return 1;
}


/* ������� ���������� � �������� ����������
   ������ ���� ��������� ��� � (� �� �++)
 */

/* ����� ���������� (����������, ����� ������������ ����������������� ������� */
extern "C" void reset()
{
  MbrStateForm->Show();
}

/* ���������� ���� ������� ���������� */
extern "C" void config()
{
  ConfigForm = new TConfigForm(NULL);
  ConfigForm->ShowModal();
  delete ConfigForm;
}

/* ������ ����� �� ����� */
extern "C" BYTE read(WORD por)
{
  return 0xff;
}

/* ������ ����� � ���� */
extern "C" void write(WORD port, BYTE data)
{
}


/* �������������� �� dll �������, ������� ������ ����������������
   ���������� � ��������� ��������� �� ������� � ������� 1,
   ��� ���� ������������� ���������� ������ ��������, ������� 0
 */
extern "C" int __export DeviceInit(struct plugin_device *dev)
{
  /* �������� ���������� */
  dev->descr = description;

  /* ����� � ����� ����� ���������� */
  dev->mask = 0xff;
  dev->port = mbr_port;

  /* ��������� ��������� �� ������� ���������� */
  dev->read = read;
  dev->write = write;
  dev->reset = reset;
  dev->config = config;  /* ��� NULL, ���� ���������� �� ������������� */

  /* �������� ������� ���� � ��������� ��������� � ��������� ������ */
  if (!InitWindow())
    return 0;

  /* ���������� ���� ������� ������������� */
  return 1;
}

