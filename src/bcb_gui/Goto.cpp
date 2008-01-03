//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "Goto.h"
#include "Utils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TGotoForm *GotoForm;
//---------------------------------------------------------------------------
__fastcall TGotoForm::TGotoForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TGotoForm::Edit1Change(TObject *Sender)
{
  TEdit *e = (TEdit*)Sender;
  e->Text = GetXdigitOnly(e->Text);
}
//---------------------------------------------------------------------------
void __fastcall TGotoForm::BitBtn1Click(TObject *Sender)
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
