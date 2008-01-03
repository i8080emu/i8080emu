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

  mbr_port = ini->ReadInteger( "Plugin_Mbr", "Port", 0x50);

  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TConfigForm::FormDestroy(TObject *Sender)
{
  TIniFile *ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));

  ini->WriteInteger( "Plugin_Mbr", "Port", mbr_port);

  delete ini;
}
//---------------------------------------------------------------------------

void __fastcall TConfigForm::FormShow(TObject *Sender)
{
  Edit1->Text = IntToHex(mbr_port, 2);
}
//---------------------------------------------------------------------------

void __fastcall TConfigForm::FormCloseQuery(TObject *Sender,
      bool &CanClose)
{
  int port, res;

  res = sscanf(Edit1->Text.c_str(), "%x", &port);

  if (res != 1 || port<0 || port>255) {
    ShowMessage(" В поле адреса порта введено некоректное значение!");
    CanClose = false;
  }

  mbr_port = port;
}
//---------------------------------------------------------------------------

