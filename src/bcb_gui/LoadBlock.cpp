// Ёмул€тор-отладчик i8080
// ƒиалог загрузки блока в пам€ть
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "LoadBlock.h"
#include "Utils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLoadBlockForm *LoadBlockForm;
//---------------------------------------------------------------------------
__fastcall TLoadBlockForm::TLoadBlockForm(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TLoadBlockForm::FormShow(TObject *Sender)
{
  FILE *fp;
  WORD _addr, _len;

  fp = fopen(BlockFilename.c_str(), "rb");

  /* —читываем адрес и длинну блока */
  fread(&_addr, sizeof(WORD), 1, fp);
  fread(&_len,  sizeof(WORD), 1, fp);

  /* ¬ыводим в пол€ считаные числа */
  Edit1->Text = IntToHex(_addr,4);
  Edit2->Text = IntToHex(_len,4);
}
//---------------------------------------------------------------------------
void __fastcall TLoadBlockForm::FormCloseQuery(TObject *Sender,
      bool &CanClose)
{
  int res;

  res = sscanf(Edit1->Text.c_str(),"%X", &addr);
  if (res == 0 || res == EOF) {
    ShowMessage("¬ведено некорректное значение в поле адреса блока");
    CanClose = false;
  }
}
//---------------------------------------------------------------------------
void __fastcall TLoadBlockForm::Edit1Change(TObject *Sender)
{
  TEdit *e = (TEdit*)Sender;
  e->Text = GetXdigitOnly(e->Text);
}
//---------------------------------------------------------------------------


