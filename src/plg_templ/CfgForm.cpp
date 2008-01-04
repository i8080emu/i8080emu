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
  Edit1->Text = IntToHex(device_port, 2);
}
//---------------------------------------------------------------------------


/* ������ �� ������ OK.
   ����� ���������� ������� �������� �� ������������ �������� ����������
   � ���� ��� ���������, �������� �� � ��������������� ����������
   � ���� ������������ ��������� (i8080emu.ini)
 */
void __fastcall TConfigForm::BitBtn1Click(TObject *Sender)
{
  int port, res;

  /* ���������� � �������� ������ ����� */
  res = sscanf(Edit1->Text.c_str(), "%x", &port);

  /* ���� ������� ������������ ��������, ���������� ��������� �� ������ */
  if (res != 1 || port<0 || port>255) {
    ShowMessage(" � ���� ������ ����� ������� ����������� ��������!");
    return;
  }

  /* ���� ����� �������� �������� ������� ����������,
     �� ��������� �� � ��������������� ����������.
   */
  device_port = port;

  /* � ����� � ini-���� ��������� */
  TIniFile *ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));
  ini->WriteInteger( DEVICE_NAME, "Port", device_port);
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

