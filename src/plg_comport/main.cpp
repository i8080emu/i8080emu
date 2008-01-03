#include <vcl.h>
#include <windows.h>
#include <inifiles.hpp>
#pragma hdrstop
#include <stdio.h>
#include "CfgForm.h"


#include "core/plugin.h"

/* �������� ���������� */
char description[] = "Real COM port redirect (com_port.dll)";

/* �������� �������� ������ ���������� (�������� ������ � ����� ������������) */
char DEVICE_NAME[] = "Plugin_comport";

/* ����� ����� (� �������� �� ��������� �� ��� ��������� � ����������) */
int device_port = 0xc1;

/* ����� ��������� COM-����� */
int real_com_port_number;

/* ���� ���������� ������ */
BOOL can_read;

/* ���� ���������� ������ */
BOOL can_write;

/* ���� �������� COM-����� */
int COM_opened = 0;

/* �������� ���������� COM-����� */
HANDLE com_port;

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
        return 1;
}
//---------------------------------------------------------------------------

/* ������� ����������� � ��������� COM-����� ����������
   ���������� 1 ���� �������
   0 � ������ ������
 */
int Init_Real_COM_port()
{
 char com_port_name[16];
 DWORD BaudRate;
 BYTE Buff[10];
 DWORD nread,nwrite;

  /* ��������� ��� COM-����� */
  sprintf(com_port_name, "COM%d", real_com_port_number);

  BaudRate = 1200;

  /* ��������� ���� */
  com_port = CreateFile(com_port_name, GENERIC_READ | GENERIC_WRITE,
      FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
  if (com_port==INVALID_HANDLE_VALUE) {
    char error_text[200];
    sprintf(error_text,"'%s' Open Error %d\n", com_port_name, GetLastError());
    ShowMessage(error_text);
    return 0;
  }
  // �������� ������� ��������� COM-�����
  DCB dcb;
  if (GetCommState(com_port, &dcb)) {
     // ������������� ��������� ���������
     // ��������
     dcb.BaudRate = BaudRate;
     // ������ �����
     dcb.ByteSize = 7; dcb.Parity = 0; dcb.StopBits = 0;
     // ������
     dcb.fBinary =  1;
     dcb.fOutxCtsFlow = 0;      // CTS output flow control
     dcb.fOutxDsrFlow = 0;      // DSR output flow control
     // � ��� ����� ����� ������������� �������� �� ������
     dcb.fDtrControl  = 0;      // DTR flow control type
     //
     dcb.fDsrSensitivity = 0;   // DSR sensitivity
     dcb.fTXContinueOnXoff = 1; // XOFF continues Tx
     dcb.fOutX        = 0;      // XON/XOFF out flow control
     dcb.fInX         = 0;      // XON/XOFF in flow control
     dcb.fRtsControl  = 0;      // RTS flow control
     dcb.fAbortOnError= 0;      // abort reads/writes on error
     SetCommState(com_port, &dcb);
  }
  // ������������� ��������
  COMMTIMEOUTS touts;
  if (GetCommTimeouts(com_port, &touts)) {
     touts.ReadIntervalTimeout = MAXDWORD;  // �������� �� ������������
     touts.ReadTotalTimeoutMultiplier = 0;
     touts.ReadTotalTimeoutConstant = 0;    // ������� ������ �� ������������
                                            // (������ ��� ��������)
     SetCommTimeouts(com_port, &touts);
  }
  // ���������� DTR � RTS
  EscapeCommFunction(com_port, SETDTR);
  EscapeCommFunction(com_port, SETRTS);

  // ���������� ���� ������� �������������
  return 1;
}


/*    ������� ���������� � �������� ����������    */
/*    ������ ���� ��������� ��� � (� �� �++)      */


/* ����� ���������� (����������, ����� ������������ ����������������� ������� */
extern "C" void reset()
{
}


/* ���������� ���� ������� ����������.
   ��������� � ���� ����� ��������� ������
   � ������ ��������� �������� �� ������ ��
   ���������� � ���� �����
 */
extern "C" void config()
{
  TConfigForm *ConfigForm = new TConfigForm(NULL);
  ConfigForm->ShowModal();
  delete ConfigForm;
}

/* ������ ����� �� ����� */
extern "C" BYTE read(WORD port)
{
  char byte;
  unsigned long nread;

  if ( !can_read )
    return 0xff;

  if ( !COM_opened ) {
    if(Init_Real_COM_port())
      COM_opened = 1;
    else
      return 0xff;
  }

  nread = 0;
  ReadFile(com_port, &byte, 1, &nread, NULL);

  /* ����� ������� �������� */
//  if (nread == 0);

  return byte;
}

/* ������ ����� � ���� */
extern "C" void write(WORD port, BYTE data)
{
  unsigned long nwrite;

  if ( !can_write )
    return;

  if ( !COM_opened ) {
    if(Init_Real_COM_port())
      COM_opened = 1;
    else
      return;
  }

  nwrite = 0;
  WriteFile(com_port, &data, 1, &nwrite, NULL);

  /* ����� ������� �������� */
//  if (nwrite == 0);
}


/* int __export DeviceInit(struct plugin_device *dev)
   �������������� �� dll �������, ������� ������ ����������������
   ���������� � ��������� ��������� �� ������� � ������� 1,
   ��� ���� ������������� ���������� ������ ��������, ������� 0
 */
extern "C" int __export DeviceInit(struct plugin_device *dev)
{
  DWORD dwVersion;

  /* ��������� ���� �������� ��������� */
  TIniFile *ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));

  /* ��������� ����� ����� �� ����� ��������, � ���� ��� �� ������, ����� �� ��������� */
  device_port = ini->ReadInteger( DEVICE_NAME, "Port", device_port);

  /* ��������� ��������� ��������� */
  real_com_port_number = ini->ReadInteger( DEVICE_NAME, "RealCOM", 2);
  can_read = ini->ReadInteger( DEVICE_NAME, "can_read", 1);
  can_write = ini->ReadInteger( DEVICE_NAME, "can_write", 1);

  /* ��������� ���� �������� */
  delete ini;

  /* ��������� ����������� ���� ��������� plagin */
  dev->descr = description;   // ��������
  dev->mask = 0xff; // ����� ����� ���������� (����������� � readme.txt)
  dev->port = device_port; // ����� ����� ����������
  dev->read = read; // ������� ������ �� �����
  dev->write = write; // ������� ������ � ����
  dev->reset = reset; // ������� ������ ����������
  dev->config = config;  /* ������� ������� �������� ��� NULL, ���� ���������� ��� �� ����� */

  /* ���������� ���� ������� ������������� */
  return 1;

/* ����������� �������� ��������� COM-����� ����� ���������
  ��� ������ ��������� � ����, ����� ������������ �� ���� ��
  ������� � ������� ��������
 */
}

