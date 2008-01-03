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

extern int speaker_port;
extern int can_direct_io;

//---------------------------------------------------------------------------
__fastcall TConfigForm::TConfigForm(TComponent* Owner)
        : TForm(Owner)
{
  TIniFile *ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));

  speaker_port = ini->ReadInteger( "Plugin_Speaker", "Port", SPEAKER_PORT);
  can_direct_io  = ini->ReadInteger( "Plugin_Speaker", "DirectIO", 1);

  delete ini;
}
//---------------------------------------------------------------------------

void __fastcall TConfigForm::FormDestroy(TObject *Sender)
{
  TIniFile *ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));

  ini->WriteInteger(  "Plugin_Speaker", "Port", speaker_port);
  ini->WriteInteger(  "Plugin_Speaker", "DirectIO", can_direct_io);

  delete ini;
}
//---------------------------------------------------------------------------

void __fastcall TConfigForm::FormShow(TObject *Sender)
{
  Edit1->Text = IntToHex(speaker_port, 2);

  if (can_direct_io)
    RadioButton1->Checked = true;
  else
    RadioButton2->Checked = true;
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

  speaker_port = port;
}
//---------------------------------------------------------------------------

