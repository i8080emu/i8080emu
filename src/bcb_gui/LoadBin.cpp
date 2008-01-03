//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop
#include "Utils.h"

#include "LoadBin.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLoadBinForm *LoadBinForm;
//---------------------------------------------------------------------------
__fastcall TLoadBinForm::TLoadBinForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TLoadBinForm::BrowseButtonClick(TObject *Sender)
{
  if (OpenDialog1->Execute()) {
    FileEdit->Text = OpenDialog1->FileName;
  }
}
//---------------------------------------------------------------------------
void __fastcall TLoadBinForm::StartEditChange(TObject *Sender)
{
  TEdit *e = (TEdit*)Sender;
  e->Text = GetXdigitOnly(e->Text);
}
//---------------------------------------------------------------------------
void __fastcall TLoadBinForm::FormCloseQuery(TObject *Sender,
      bool &CanClose)
{
  int res;

  res = sscanf(StartEdit->Text.c_str(),"%X", &start);
  if (res == 0 || res == EOF) {
    ShowMessage("¬ведено некорректное значение в поле адреса блока");
    CanClose = false;
  }

  res = sscanf(LenEdit->Text.c_str(),"%X", &len);
  if (res == 0 || res == EOF) {
    ShowMessage("¬ведено некорректное значение в поле адреса блока");
    CanClose = false;
  }

  // load whole file
  if (RadioButton2->Checked)
    len = 0;
}
//---------------------------------------------------------------------------
void __fastcall TLoadBinForm::RadioButton1Click(TObject *Sender)
{
  LenEdit->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TLoadBinForm::RadioButton2Click(TObject *Sender)
{
  LenEdit->Enabled = true;
}
//---------------------------------------------------------------------------
