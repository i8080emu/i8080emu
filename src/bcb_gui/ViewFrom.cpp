// ��������-�������� i8080
// ������ ������� ���������� ������ �����������
//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "ViewFrom.h"
#include "Utils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TViewFromForm *ViewFromForm;
//---------------------------------------------------------------------------
__fastcall TViewFromForm::TViewFromForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TViewFromForm::BitBtn1Click(TObject *Sender)
{
  int i;
  sscanf(Edit1->Text.c_str(),"%x",&i);

  if(i<0 || i>65535) {
    ShowMessage("����� ������ ��������� � �������� �� 0 �� #FFFF (65535 ����������).");
    BitBtn1->Cancel = true;
  }
  else
    addr = i;
}
//---------------------------------------------------------------------------

void __fastcall TViewFromForm::Edit1Change(TObject *Sender)
{
  TEdit *e = (TEdit*)Sender;
  e->Text = GetXdigitOnly(e->Text);
}
//---------------------------------------------------------------------------

