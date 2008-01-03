// Эмулятор-отладчик i8080
// Изменение регистров и флагов
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "RegFlag.h"

#include <stdio.h>
#include <string.h>
#include "Utils.h"

#include "core/i8080.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TRegFlagForm *RegFlagForm;
//---------------------------------------------------------------------------
__fastcall TRegFlagForm::TRegFlagForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TRegFlagForm::FormShow(TObject *Sender)
{
  Edit1->Text = IntToHex(i8080.bc.w , 4);
  Edit2->Text = IntToHex(i8080.de.w , 4);
  Edit3->Text = IntToHex(i8080.hl.w , 4);
  Edit4->Text = IntToHex(i8080.sp.w , 4);
  Edit5->Text = IntToHex(i8080.af.b.h,2);

  CheckBox1->Checked = i8080.af.b.l & 0x01;
  CheckBox2->Checked = i8080.af.b.l & 0x04;
  CheckBox3->Checked = i8080.af.b.l & 0x10;
  CheckBox4->Checked = i8080.af.b.l & 0x40;
  CheckBox5->Checked = i8080.af.b.l & 0x80;
}
//---------------------------------------------------------------------------


void __fastcall TRegFlagForm::Edit1Change(TObject *Sender)
{
  TEdit *e = (TEdit*)Sender;
  e->Text = GetXdigitOnly(e->Text);
}
//---------------------------------------------------------------------------


void __fastcall TRegFlagForm::BitBtn1Click(TObject *Sender)
{
  /* Записываем в регистры данные из полей диалога */
  // т.к. в поля для редактирования невозможно ввести нешеснадцатиричное цисло,
  // необходимости проверять корректности ввода здесь нет.
  sscanf(Edit1->Text.c_str(),"%hx", &i8080.bc.w);
  sscanf(Edit2->Text.c_str(),"%hx", &i8080.de.w);
  sscanf(Edit3->Text.c_str(),"%hx", &i8080.hl.w);
  sscanf(Edit4->Text.c_str(),"%hx", &i8080.sp.w);
  WORD tmp;
  sscanf(Edit5->Text.c_str(),"%hx", &tmp);
  i8080.af.b.h = tmp;

  /* Устанавливаем флаги данными диалога */
  i8080.af.b.l = 0;  // сбрасываем все флаги
  if(CheckBox1->Checked) i8080.af.b.l |= 0x01;
  if(CheckBox2->Checked) i8080.af.b.l |= 0x04;
  if(CheckBox3->Checked) i8080.af.b.l |= 0x10;
  if(CheckBox4->Checked) i8080.af.b.l |= 0x40;
  if(CheckBox5->Checked) i8080.af.b.l |= 0x80;
}
//---------------------------------------------------------------------------

