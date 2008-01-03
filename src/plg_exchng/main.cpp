/*   ��������-�������� i8080

  Exchanger plugin

���� ������ ������������� ��������� ������� �������� ������
����� ������� ����������� � ����������� �����������.
� ���������, ����������� ���������� ��������, �����������
� ������������� � �����������, ����������� ����� ����������.

*/
#include <vcl.h>
#include <windows.h>
#include <inifiles.hpp>
#pragma hdrstop
#include "../../kernel/plugin.h"
#include "CfgForm.h"

/* �������� ���������� */
char description[] = "Data exchanger (exchanger.dll)";

/* �������� �������� ������ ���������� (�������� ������ � ����� ������������) */
char DEVICE_NAME[] = "Plugin_exchanger";

/* ��������� �� ��������� plugin_device �������� ���������� */
struct plugin_device *plugin;

/* ���������� ������������ */

// ����� ����� (� �������� �� ��������� �� ��� ��������� � ����������)
int device_port = 0x20;

// ����� �������� ������
transfer_mode_t transfer_mode;

// ������ ������ ��� ��������
String data_string;

// � ����� �������� ���������� ������ ��� ���������� ��������, ��
WORD sync_freq = 3000;

// ��� ������� ������������ �� ���� ������ ��� ��������� ����������
Byte interrupt_op_code;



/* ���������� ���������� */

// ���� ����, ��� �������� ��������
int is_transfer;

// ����� ������� ������ �������� ��������� ���� ������
DWORD sync_tacts;

// �������� �������� ������ ������� � ������� ���������� ���������
DWORD last_tstates;

// ����������� ����� � ������������:
int asyn_int_byte_num;          // ����� �������� ����� � ������
Byte asyn_data;                 // ������� ���� ������, ������� ����� �������� ��� ��������� � �����
int asyn_handled;               // ���� ����, ��� ������� ���� ��� �������� � ��������� �������� ���������


#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
        return 1;
}
//---------------------------------------------------------------------------


/* ��������������� ������� */


/* ���������� ���� �� ������ ������ �� ��������� ��������.
   ���� �������� ������� �� ������� ������, ���������� ����
   �������� � ���������� ��� ������.
 */
Byte GetDataByte(int offset)
{
  if (offset > data_string.Length()) {
    is_transfer = 0;
    return ERROR_CODE;
  }

  return data_string[offset];
}

/* ������� ��������� ������, �������������� ������ ����������
   �� ���������� ������������� ���������� ������.
   ������������ ��� ������ � ������������.
 */
static void callback()
{
  DWORD tacts_diff;

  // ������ �� ��������?
  if (!is_transfer)
    return;

  if (transfer_mode == TM_ASYNC_INT) {
    tacts_diff = *(plugin->tstates) - last_tstates;

    // �� ������ �� ����� ���������� ��������� ����?
    if (tacts_diff > sync_tacts )
      asyn_data = GetDataByte(tacts_diff / sync_tacts);
      asyn_handled = false;
  }
}


/*    ������� ���������� � �������� ����������    */
/*    ������ ���� ��������� ��� � (� �� �++)      */

/* ����� ���������� (����������, ����� ������������ ����������������� ������� */
extern "C" void reset()
{
  is_transfer = 0;
  sync_tacts = *(plugin->mpsys_hz) / sync_freq;

  // ��������� �� ������ ������������ ������ ������� ���
  // ����� ������ 10 ������ (����� ������� in)
  if (sync_tacts < 10 ) {
    ShowMessage("������� ����������� ������� �������� ������.");
  }
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
  DWORD tacts_diff;

  // ���� ��������� � ������ ���������� ������� �����
  if ((port & 0x01) == 0) {

    /* ������ �� ����� ��������� ���������� */

    // ������� ������ ������ � ���������� ������
    tacts_diff = *(plugin->tstates) - last_tstates;

    /* ���������� ���� ������� �� ������ */
    switch(transfer_mode) {
      case TM_SYNC:
        // � ���������� ������ ������ ������� �� ���������� ����� ������
        // ����� �������� � ������ ������ ����������.
        return GetDataByte(tacts_diff / sync_tacts);
      case TM_ASYNC_KVIT:
        break;
      case TM_ASYNC_INT:
        break;
      default:
        ShowMessage("�������� �������� ���������� ������ ��������!");
    }
  }
  else {

    /* ������ �� ����� ������ */

    // ���� ������ �� ���� ��������, ���������� ��� ������
    if (!is_transfer)
      return ERROR_CODE;

    /* ���������� ���� ������� �� ������ */
    switch(transfer_mode) {
      case TM_SYNC:
        break;
      case TM_ASYNC_KVIT:
        break;
      case TM_ASYNC_INT:
        break;
      default:
        ShowMessage("�������� �������� ���������� ������ ��������!");
    }
  }
  return ERROR_CODE;
}


/* ������ ����� � ���� */
extern "C" void write(WORD port, BYTE data)
{
  // ���� ��������� � ������ ���������� ������� �����
  if ((port & 0x01) == 0) {

    /* ������ � ���� ��������� ���������� */

    // ���� ����� ������ ������ ��������
    if (data == START_TRANSFER_CODE) {
      last_tstates = *(plugin->tstates);
      is_transfer = 1;
    }

    /* ������ � ���� ��������� ����� 0x0f �������� ������ � ������ �������� */
    switch(transfer_mode) {
      case TM_SYNC:
        break;
      case TM_ASYNC_KVIT:
        break;
      case TM_ASYNC_INT:
        asyn_int_byte_num = 0;
        break;
      default:
        ShowMessage("�������� �������� ���������� ������ ��������!");
    }
  }
  else {
    /* ������ � ���� ������ ������������ */
    return;
  }
}


/* int __export DeviceInit(struct plugin_device *dev)
   �������������� �� dll �������, ������� ������ ����������������
   ���������� � ��������� ��������� �� ������� � ������� 1,
   ��� ���� ������������� ���������� ������ ��������, ������� 0
 */
extern "C" int __export DeviceInit(struct plugin_device *dev)
{
  DWORD dwVersion;

  // ���������� ��������� �� ���� ������ (�� ����� ��� ��������� � ���������)
  plugin = dev;

  /* ��������� ���� �������� ��������� */
  TIniFile *ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));

  /* ��������� ����� ����� �� ����� ��������, � ���� ��� �� ������, ����� �� ��������� */
//  device_port = ini->ReadInteger( DEVICE_NAME, "Port", device_port);

  /* ��������� ��������� ��������� */
  data_string = ini->ReadString( DEVICE_NAME, "data_string", "This string is ex�mple capacity for work of data transfer mechanism.");
  transfer_mode = (transfer_mode_t)ini->ReadInteger( DEVICE_NAME, "transfer_mode", TM_SYNC);
  interrupt_op_code = ini->ReadInteger( DEVICE_NAME, "interrupt_op_code", 0xff);

  /* ��������� ���� �������� */
  delete ini;

  /* ��������� ����������� ���� ��������� plugin */
  dev->descr = description;
  dev->mask = 0xfe;
  dev->port = device_port;
  dev->read = read;
  dev->write = write;
  dev->reset = reset;
  dev->config = config;

  /* ������������ ������� ��������� ������ */
  if (!dev->register_callback(callback))
    ShowMessage("�� ������� ���������������� ������� ��������� ������-  \
    ������ �� ������ ������������ ���������� � ������������ ����� � ������������ \
    ����� �������� �����������! (��������� ����������� ���������� ����� ��������, \
    �������������� ������� ��������� ������. ���������� ��������� ��������� �������, \
    ������� �� ����������.)");

  /* ���������� ���� ������� ������������� */
  return 1;
}

