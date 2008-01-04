//---------------------------------------------------------------------------
#include <vcl.h>
#include <inifiles.hpp>
#pragma hdrstop

#include <stdio.h>
#include "CfgForm.h"
#include "Utils.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TConfigForm *ConfigForm;
extern char DEVICE_NAME[];

/* ����� ������ ���� ���������� �� ������� ���������,
   �������� ������� ����� ����������� */
extern int device_port;
extern transfer_mode_t transfer_mode;
extern String data_string;
extern Byte interrupt_op_code;

static transfer_mode_t temp_mode;
//---------------------------------------------------------------------------
__fastcall TConfigForm::TConfigForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TConfigForm::FormShow(TObject *Sender)
{
  /* ������� � �������� ������� ���������
     ���������� �� ���������� ������������
   */
//  Edit1->Text = IntToHex(device_port, 2);
  DataEdit->Text = data_string;

  /* ������������� ������������� ������ �������� */
  if (transfer_mode == TM_SYNC)
    RadioButton1->Checked = true;
  else if (transfer_mode == TM_ASYNC_KVIT)
    RadioButton2->Checked = true;
  else
    RadioButton3->Checked = true;
  RadioButton1Click(Sender);

  Edit3->Text = IntToHex(interrupt_op_code, 2);
}
//---------------------------------------------------------------------------


/* ������ �� ������ OK.
   ����� ���������� ������� �������� �� ������������ �������� ����������
   � ���� ��� ���������, �������� �� � ��������������� ����������
   � ���� ������������ ��������� (i8080emu.ini)
 */
void __fastcall TConfigForm::BitBtn1Click(TObject *Sender)
{
  int port, code, res;
#if 0
������ ������ �����������
  /* ���������� � �������� ������ ����� */
  res = sscanf(Edit1->Text.c_str(), "%x", &port);

  /* ���� ������� ������������ ��������, ���������� ��������� �� ������ */
  if (res != 1 || port<0 || port>255) {
    ShowMessage(" � ���� ������ ����� ������� ����������� ��������!");
    return;
  }

  device_port = port;
#endif

  /* ���������� � �������� ���� �������� */
  res = sscanf(Edit3->Text.c_str(), "%x", &code);

  if (res != 1 || code<0 || code>255) {
    ShowMessage(" � ���� ���� ������� ������� ����������� ��������!");
    return;
  }

  transfer_mode = temp_mode;
  data_string = DataEdit->Text;
  interrupt_op_code = code;

  /* ���������� �������� �������� � ini ���� */
  TIniFile *ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));
  ini->WriteInteger( DEVICE_NAME, "Port", device_port);
  ini->WriteInteger( DEVICE_NAME, "transfer_mode", transfer_mode);
  ini->WriteString( DEVICE_NAME, "data_string", data_string);
  ini->WriteInteger( DEVICE_NAME, "interrupt_op_code", interrupt_op_code);
  delete ini;

  Close();
}
//---------------------------------------------------------------------------

void __fastcall TConfigForm::Edit1Change(TObject *Sender)
{
  // ��������� � ���� ������ ����������������� �����
  TEdit *e = (TEdit*)Sender;
  e->Text = GetXdigitOnly(e->Text);
}
//---------------------------------------------------------------------------

void __fastcall TConfigForm::RadioButton1Click(TObject *Sender)
{
  if (RadioButton1->Checked) {
    Edit3->Enabled = false;
    temp_mode = TM_SYNC;
  }
  else if (RadioButton2->Checked) {
    Edit3->Enabled = false;
    temp_mode = TM_ASYNC_KVIT;
  }
  else {
    Edit3->Enabled = true;
    temp_mode = TM_ASYNC_INT;
  }
}
//---------------------------------------------------------------------------

