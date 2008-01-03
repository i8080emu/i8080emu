//---------------------------------------------------------------------------

#include <vcl.h>
#include <inifiles.hpp>
#pragma hdrstop

#include <stdio.h>
#include "CfgForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TConfigForm *ConfigForm;

extern int mbr_port;


//---------------------------------------------------------------------------
__fastcall TConfigForm::TConfigForm(TComponent* Owner)
        : TForm(Owner)
{
  TIniFile *ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));

  mbr_port = ini->ReadInteger( "Plugin_Mbr1", "Port", 0x50);
  bShowOnStart = ini->ReadInteger( "Plugin_Mbr1", "ShowOnStart", true);


  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TConfigForm::FormDestroy(TObject *Sender)
{
  TIniFile *ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));

  ini->WriteInteger( "Plugin_Mbr1", "Port", mbr_port);
  ini->WriteInteger( "Plugin_Mbr1", "ShowOnStart", bShowOnStart);

  delete ini;
}
//---------------------------------------------------------------------------

void __fastcall TConfigForm::FormShow(TObject *Sender)
{
  Edit1->Text = IntToHex(mbr_port, 2);
  CheckBox1->Checked = bShowOnStart;
}
//---------------------------------------------------------------------------


void __fastcall TConfigForm::BitBtn1Click(TObject *Sender)
{
  int port, res;

  res = sscanf(Edit1->Text.c_str(), "%x", &port);
  if (res != 1 || port<0 || port>255) {
    ShowMessage(" В поле адреса порта введено некоректное значение!");
  }
  mbr_port = port;

  bShowOnStart = CheckBox1->Checked;
}
//---------------------------------------------------------------------------

