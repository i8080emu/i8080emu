// ��������-�������� i8080
// ������ ����� ����������������� ������������������
// � ���� ���������, ����� ��� ������.
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <string.h>

#include "FindPut.h"
#include "Main.h"
#include "Utils.h"
#include "core/asm.h"
#include "core/gui.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFindPutForm *FindPutForm;
//---------------------------------------------------------------------------
__fastcall TFindPutForm::TFindPutForm(TComponent* Owner)
        : TForm(Owner)
{
  // ����� ������������������ -1 ������, ��� ������ ���
  len = -1;
}
//---------------------------------------------------------------------------
void __fastcall TFindPutForm::Button3Click(TObject *Sender)
{
  Close();
}

BYTE get_xdigit_value(char xdigit)
{
  if (xdigit >= '0' && xdigit <= '9')
    return xdigit-'0';
  else
    return xdigit-'A'+0x0a;
}

//---------------------------------------------------------------------------
void __fastcall TFindPutForm::Edit1Change(TObject *Sender)
{
  RadioButton1->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TFindPutForm::Edit2Change(TObject *Sender)
{
  RadioButton2->Checked = true;

  // ��������� � ���� ������ ����������������� �����
  TEdit *e = (TEdit*)Sender;
  e->Text = GetXdigitOnly(e->Text);
}
//---------------------------------------------------------------------------

void __fastcall TFindPutForm::Edit3Change(TObject *Sender)
{
  RadioButton3->Checked = true;
}
//---------------------------------------------------------------------------

void __fastcall TFindPutForm::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (Key == VK_RETURN)
    BitBtn2Click(Sender);

  if (Key == VK_ESCAPE)
    Close();
}
//---------------------------------------------------------------------------

/* ������ ��. ����������� �������� ������ � ������� ������������������. */
void __fastcall TFindPutForm::BitBtn2Click(TObject *Sender)
{

  /* ���� ������� ��������� */
  if (RadioButton1->Checked) {
    ASMSESSION as;
    asmerr_t asm_err;
    // ����� ������ ��� ���������������
    BYTE membuf[0x10000];

    // ������� �������� ����� ������
    memset(membuf, 0, sizeof(membuf));

    // �������������� ������ ��������������
    asm_start(&as, membuf);

    // �������� ��������� ����
    as.codegen = 1;

    /* �������� �������� � ���������� ������ */
    char *p = new char[Edit1->Text.Length() + 1];
    strcpy(p, Edit1->Text.c_str());

    // �������� ������ �� ���������������
    asm_err = asm_process(&as, p);

    // ���� ��������� ������ ��� ������� ������ ��������� (��� ����� ��������� ������)
    if (asm_err != ASMERR_NOERROR || as.addr == 0) {
      ShowMessage("������� �������� ���������");
      MessageBeep(0xFFFFFFFF);
      len = -1;
    }
    else {
      // ���� ��������������� ������ ���������, ���������� ��������� � �����
      memcpy(data, membuf, MAX_HEX_SEQUENCE);
      /* ������������� ����� ������
         �.�. ��������������� �� ��������� ���������� � ��������
         ������, ������� ����� ���������� � ����� �������� ������� �������.
       */
      len = as.addr;

      // �������� �� ���������� ������������� ������� ������
      if (len > MAX_HEX_SEQUENCE)
        len = MAX_HEX_SEQUENCE;
    }

    // ����������� ������ ������
    asm_end(&as);

    // ����������� ����� ������
    delete p;
  }
  else if (RadioButton2->Checked){
    /* ����������������� �����, ����������� ������������������ */
    char *sc;
    int first = 1;
    int n = 0;

    /* �������� ������ */
    sc = new char[Edit2->Text.Length() + 1];
    strcpy(sc, Edit2->Text.c_str());

    /* ��������� ����������������� ����� */
    for (char *p = sc; *p != '\0'; p++) {
      if (*p == ' ')
        continue;

      if (n > MAX_HEX_SEQUENCE) {
        ShowMessage("�������� ������������������ �������, ��� ����� ������������ ��� ������.");
        delete sc;
        return;
      }

      if (first)
        data[n] = get_xdigit_value(*p) << 4;
      else {
        data[n] |= get_xdigit_value(*p);
        n++;
      }

      first = !first;
    }
    // ����������� ������ �� ����� ������
    delete sc;

    // ����� ���� ������ ���� �����
    if (!first && n>0) {
      ShowMessage("����� ����������������� ���� �������! �������� ��������� ����� ������� �����������.");
      return;
    }

    // ������������� ������ �������� ������������������
    len = n;
  }
  else {
    // ������ �����
    /* �������� �������� ������ ������ */
    char *p = new char[Edit3->Text.Length() + 1];
    strcpy(p, Edit3->Text.c_str());
    int n;

    // � ����� ��������� � ����� ����������� ���������� ����� ��������
    for (n = 0; n < MAX_HEX_SEQUENCE; n++)
      if (*(p+n) != '\0')
        data[n] = *(p+n);
      else
        break;

    len = n; // ������������� ������ �������� ������������������
    delete p;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFindPutForm::FormShow(TObject *Sender)
{
  Edit1->Text = "";
  Edit2->Text = "";
  Edit3->Text = "";
}
//---------------------------------------------------------------------------


