// ��������-�������� i8080
// ���� ���������� �������������
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


/* ������ �����. ������ ����� ������������ ��� ��������
   ����������. � ������� ��������� ���� ���� ����������
   �����, �� �.�. ��� ����� ������� ������ �����, �������
   ��������� � ����� � ������� ��� �����, �� ������ ��� ��
   �����- ������ ��������� �� ������ ����� ������ ����������
   ����� �����, � ������������ ��� ����� ������ �� �����������
   ������.
 */
TList *Labels;

//---------------------------------------------------------------------------
__fastcall TDisasForm::TDisasForm(TComponent* Owner)
  : TForm(Owner)
{
  Labels = new TList;
}
//---------------------------------------------------------------------------

// ���������� ������� ����
void TDisasForm::GenerateDisass()
{
  WORD addr, start, end;
  DISASRESULT dr;
  int n, res;

  RichEdit1->Clear();
  Labels->Clear();

  // ���� ������� �������������� ����������� ������,
  if (RadioButton1->Checked) {
    // ���� ������ ��������� ���� � ������ � � �����
    for (n = 0; n < 0x10000; n++)
      if (MEM[n] != 0)
        break;
    start = n;

    // ������� ����� ������ � ���� �����
    Edit1->Text = IntToHex(start, 4);

    // ���� ��� ������ �����, �����
    if (n == 0x10000) {
      RichEdit1->Lines->Add("; ������ ����������� ������� �����!");
      RichEdit1->Lines->Add("; ������ �����������������.");
      Edit2->Text = "0000";
      return;
    }

    // ���������� ����� �����
    for (n = 0xffff; n > start; n--)
      if (MEM[n] != 0)
        break;
    end = n;

    // ������� ����� ����� � ���� �����
    Edit2->Text = IntToHex(end, 4);
  }
  else {
    // ����� ������� ������� �������,
    // ����� ������� �� ����� �����
    int tmp;
    sscanf(Edit1->Text.c_str(),"%X", &tmp);
    start = tmp;
    sscanf(Edit2->Text.c_str(),"%X", &tmp);
    end = tmp;

    // ��������� ���� ������ �� ��������� �����
    if (start > end) {
      RichEdit1->Lines->Add("; ������� ������� ������� ������!");
      RichEdit1->Lines->Add("; ����� ������ ������ ������ �����.");
      return;
    }
  }

  // ���� �������� �� � �������� ������, ��������
  // ��������� ORG
  if (start != 0) {
    String s;
    s.printf("\tORG\t%04X", start);
    RichEdit1->Lines->Add(s);
  }

  // ������ ������ ������������������- ���� �����
  addr = start;
  do {
    res = disas_addr(addr, &dr);
    /* ���� ��� ���� ������� �������� ��� ������ */
    if (dr.needloop) {
      // ��������� ����� � ������ �����, ���� ��� ��� ��� ���
      if (Labels->IndexOf((void *)dr.loopaddr) == -1)
        Labels->Add((void *)dr.loopaddr);
    }
    addr += res;
  } while(addr <= end);

  // ��������� �������
  addr = start;
  do {
    String l,s;

    res = disas_addr(addr, &dr);

    // ���� ��� ������� �������� ��� ������
    // ���������, �� ��������� �� ��� �� �����
    // ��������� � ������� �����
    if (dr.needloop) {
      int ind = Labels->IndexOf((void *)dr.loopaddr);
      if (ind != -1)
        sprintf(dr.ops, "M%03d", ind);
    }

    // ���� �� ���� ����� ���� ������, ���������� �����
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
    // ��������� ���������� RichEdit1
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
  if (MessageBox(Handle, "�� ������������� ������ ����������� ���� �����?","�������������",MB_YESNO) == IDYES) {
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

