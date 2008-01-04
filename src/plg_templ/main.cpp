/*  ��������-�������� i8080

  Template plugin


������ ������� ��� ���������

*/
#include <vcl.h>
#include <windows.h>
#include <inifiles.hpp>
#pragma hdrstop
#include "core/plugin.h"
#include "CfgForm.h"

/* �������� ���������� */
char description[] = "Template for plugin (templ.dll)";

/* �������� �������� ������ ���������� (�������� ������ � ����� ������������) */
char DEVICE_NAME[] = "Plugin_template";

struct plugin_device *plugin;

/* ����� ����� (� �������� �� ��������� �� ��� ��������� � ����������) */
int device_port = 0x00;

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
        return 1;
}
//---------------------------------------------------------------------------


/*    ������� ���������� � �������� ����������    */
/*    ������ ���� ��������� ��� � (� �� �++)      */

/* ����� ���������� (����������, ����� ������������ ����������������� ������� */
extern "C" void reset()
{
}

/* ���������� ���� ������� ���������� */
extern "C" void config()
{
  TConfigForm *ConfigForm = new TConfigForm(NULL);
  ConfigForm->ShowModal();
  delete ConfigForm;
}

/* ������ ����� �� ����� */
extern "C" BYTE read(WORD port)
{
  return 0xff;
}

/* ������ ����� � ���� */
extern "C" void write(WORD port, BYTE data)
{
}


/* int __export DeviceInit(struct plugin_device *dev)
   �������������� �� dll �������, ������� ������ ����������������
   ���������� � ��������� ��������� �� ������� � ������� 1,
   ��� ���� ������������� ���������� ������ ��������, ������� 0
 */
extern "C" int __export DeviceInit(struct plugin_device *dev)
{
  DWORD dwVersion;

  plugin = dev;

  /* ��������� ���� �������� ��������� */
  TIniFile *ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));

  /* ��������� ����� ����� �� ����� ��������, � ���� ��� �� ������, ����� �� ��������� */
  device_port = ini->ReadInteger( DEVICE_NAME, "Port", device_port);

  /* ��������� ��������� ��������� */




  /* ��������� ���� �������� */
  delete ini;

  /* ��������� ����������� ���� ��������� plugin */
  dev->descr = description;   // ��������
  dev->mask = 0xff; // ����� ����� ���������� (����������� � readme.txt)
  dev->port = device_port; // ����� ����� ����������
  dev->read = read; // ������� ������ �� �����
  dev->write = write; // ������� ������ � ����
  dev->reset = reset; // ������� ������ ����������
  dev->config = config;  /* ������� ������� �������� ��� NULL, ���� ���������� ��� �� ����� */

  /* ����� ������ ������� ���������������� ����������.
     � ������ ������� ������ ������� ��������� �� ������
     � ������� 0
   */




  /* ���������� ���� ������� ������������� */
  return 1;
}

