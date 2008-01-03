// ��������-�������� i8080
// ��������� ��� ���������� ����� ������
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include <string.h>
#include "SaveBlock.h"
#include "Utils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSaveBlockForm *SaveBlockForm;
//---------------------------------------------------------------------------
__fastcall TSaveBlockForm::TSaveBlockForm(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TSaveBlockForm::FormCloseQuery(TObject *Sender,
      bool &CanClose)
{
  int res;

  res = sscanf(Edit1->Text.c_str(),"%X", &addr);
  if (res == 0 || res == EOF) {
    ShowMessage("������� ������������ �������� � ���� ������ �����");
    CanClose = false;
  }

  res = sscanf(Edit2->Text.c_str(),"%X", &len);
  if (res == 0 || res == EOF) {
    ShowMessage("������� ������������ �������� � ���� ����� �����");
    CanClose = false;
  }
}
//---------------------------------------------------------------------------


void __fastcall TSaveBlockForm::Edit1Change(TObject *Sender)
{
  TEdit *e = (TEdit*)Sender;
  e->Text = GetXdigitOnly(e->Text);
}
//---------------------------------------------------------------------------

