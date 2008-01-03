// Ёмул€тор-отладчик i8080
// ќкно текстового дизассемблера
//---------------------------------------------------------------------------
#include <vcl.h>
#include <Printers.hpp>
#pragma hdrstop

#include "Disassembler.h"

#include "Utils.h"
#include "core/mpsys.h"
#include "core/disas.h"
#include <stdio.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDisasForm *DisasForm;


/* —писок меток. ¬ообще класс предназначен дл€ хранени€
   указателей. ” каждого указател€ есть свой пор€дковый
   номер, но т.к. нам нужно хранить только адрес, который
   нуждаетс€ в метке и хранить его номер, то объект нам не
   нужен- вместо указател€ на объект будем просто передавать
   адрес метки, а генерировать им€ метки исход€ из пор€дкового
   номера.
 */
TList *Labels;

//---------------------------------------------------------------------------
__fastcall TDisasForm::TDisasForm(TComponent* Owner)
  : TForm(Owner)
{
  Labels = new TList;
}
//---------------------------------------------------------------------------

// √енерируем листинг кода
void TDisasForm::GenerateDisass()
{
  WORD addr, start, end;
  DISASRESULT dr;
  int n, res;

  RichEdit1->Clear();
  Labels->Clear();

  // ≈сли выбрано автоматическое определение границ,
  if (RadioButton1->Checked) {
    // ищем первый ненулевой байт с начала и с конца
    for (n = 0; n < 0x10000; n++)
      if (MEM[n] != 0)
        break;
    start = n;

    // заносим адрес начала в поле ввода
    Edit1->Text = IntToHex(start, 4);

    // если вс€ пам€ть пуста, выход
    if (n == 0x10000) {
      RichEdit1->Lines->Add("; ѕам€ть виртуальной системы пуста!");
      RichEdit1->Lines->Add("; Ќечего дизассемблировать.");
      Edit2->Text = "0000";
      return;
    }

    // определ€ем адрес конца
    for (n = 0xffff; n > start; n--)
      if (MEM[n] != 0)
        break;
    end = n;

    // заносим адрес конца в поле ввода
    Edit2->Text = IntToHex(end, 4);
  }
  else {
    // »наче границы введены вручную,
    // берем границы из полей ввода
    int tmp;
    sscanf(Edit1->Text.c_str(),"%X", &tmp);
    start = tmp;
    sscanf(Edit2->Text.c_str(),"%X", &tmp);
    end = tmp;

    // провер€ем чтоб начало не превышало конец
    if (start > end) {
      RichEdit1->Lines->Add("; Ќеверно указаны границы пам€ти!");
      RichEdit1->Lines->Add("; јдрес начала больше адреса конца.");
      return;
    }
  }

  // если начинаем не с нулевого адреса, вставить
  // директиву ORG
  if (start != 0) {
    String s;
    s.printf("\tORG\t%04X", start);
    RichEdit1->Lines->Add(s);
  }

  // первый проход дизассемблирование- сбор меток
  addr = start;
  do {
    res = disas_addr(addr, &dr);
    /* если это была команда перехода или вызова */
    if (dr.needloop) {
      // добавл€ем адрес в список меток, если его там еще нет
      if (Labels->IndexOf((void *)dr.loopaddr) == -1)
        Labels->Add((void *)dr.loopaddr);
    }
    addr += res;
  } while(addr <= end);

  // формируем листинг
  addr = start;
  do {
    String l,s;

    res = disas_addr(addr, &dr);

    // если это команда перехода или вызова
    // провер€ем, не ссылаетс€ ли она на адрес
    // имеющийс€ в таблице меток
    if (dr.needloop) {
      int ind = Labels->IndexOf((void *)dr.loopaddr);
      if (ind != -1)
        sprintf(dr.ops, "M%03d", ind);
    }

    // если на этот адрес есть ссылки, генерируем метку
    int ind = Labels->IndexOf((void *)addr);
    if (ind != -1)
      l.printf("M%03d:", ind);
    else
      l = "";

    s.printf("\t%s\t%s", &dr.mnem, &dr.ops);
    s = l + s;
    RichEdit1->Lines->Add(s);
    addr += res;
  } while(addr <= end);
}
//---------------------------------------------------------------------------

void __fastcall TDisasForm::BitBtn2Click(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------
void __fastcall TDisasForm::Button1Click(TObject *Sender)
{
  if (SaveDialog1->Execute()) {
    // сохран€ем содержимое RichEdit1
    RichEdit1->Lines->SaveToFile(SaveDialog1->FileName);
  }
}
//---------------------------------------------------------------------------
void __fastcall TDisasForm::RadioButton1Click(TObject *Sender)
{
  Edit1->Enabled = RadioButton2->Checked;
  Edit2->Enabled = RadioButton2->Checked;
  GenerateDisass();
}
//---------------------------------------------------------------------------
void __fastcall TDisasForm::FormShow(TObject *Sender)
{
  GenerateDisass();
}
//---------------------------------------------------------------------------
void __fastcall TDisasForm::Button2Click(TObject *Sender)
{
  if (MessageBox(Handle, "¬ы действительно хотите распечатать весь текст?","ѕодтверждение",MB_YESNO) == IDYES) {
    TPrinter *Prntr = Printer();
    TRect r = Rect(200, 200, Prntr->PageWidth - 200, Prntr->PageHeight - 200);
    Prntr->BeginDoc();
    Prntr->Canvas->TextRect(r, 200, 200, RichEdit1->Lines->Text);
    Prntr->EndDoc();
  }
}
//---------------------------------------------------------------------------

void __fastcall TDisasForm::Button3Click(TObject *Sender)
{
  RichEdit1->SelectAll();
  RichEdit1->CopyToClipboard();
  RichEdit1->SelLength = 0;
}
//---------------------------------------------------------------------------

void __fastcall TDisasForm::Edit1Change(TObject *Sender)
{
  TEdit *e = (TEdit*)Sender;
  e->Text = GetXdigitOnly(e->Text);
}
//---------------------------------------------------------------------------

