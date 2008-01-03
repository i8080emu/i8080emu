// ��������-�������� i8080
// ���� ���������� ��������� ��� ���������������
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Compile.h"
#include "Assembler.h"
#include <stdio.h>
#include <string.h>

#include "core/mpsys.h"
#include "core/gui.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCompileForm *CompileForm;
//---------------------------------------------------------------------------
__fastcall TCompileForm::TCompileForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

// ���������� ������ ������� ����������
void __fastcall TCompileForm::DoPass(ASMSESSION *as)
{
  int n;
  char *line;
  asmerr_t asm_err;

  err_count = 0;

  // ��������������� �������� ������ ������ ��� ���������
  for( n=0; n<AsmForm->RichEdit->Lines->Count; n++) {
    // ������ ����� ������
    line = strdup(AsmForm->RichEdit->Lines->Strings[n].c_str());
    asm_err = asm_process(as, line);

    if (asm_err != ASMERR_NOERROR) {
      String s;
      // ���������� ������ ���� line 12: �������� ��������
      s.printf("line %d: %s", n+1, asm_get_error_text(asm_err));
      ListBox1->Items->Add(s);
      err_count++;
    }
    free(line);
  }
}
//---------------------------------------------------------------------------

void __fastcall TCompileForm::FormShow(TObject *Sender)
{
  ASMSESSION as;
  String s;

  /*  ����������, ���������� */
  AsmForm->RichEdit->ReadOnly = true;    // ��������� �������������� ������
  BitBtn1->Enabled = false;             // ��������� ������ ��, ��������� ������
  ListBox1->Clear();                    // ������� ���� �����������

  // ������� �������� ����� ������
  memset(membuf, 0, sizeof(membuf));

  // �������� ��������������, ������ ������
  ListBox1->Items->Add("������ 1, �������� ����������...");

  // ������������� ������ ��������������
  asm_start(&as, membuf);

  // ������ ������ ������
  DoPass(&as);

  // ���� ��� ����� ������ ������ ������ ��� ���� ���� ������, �����������
  if (CompileMode == cmJustTest || err_count > 0)
    goto end;

  // ������ ������ ������
  ListBox1->Items->Add("������ 2, ��������� ����...");
  asm_pass2(&as);
  DoPass(&as);

end:
  // ��������� ������ ������
  Origin = as.org;
  CodeLen = as.addr-as.org;

  // ����������� ������ ������
  asm_end(&as);

  // ���� ���� ��������� �������� org ������������� �� ����
  if (as.mult_org)
    ListBox1->Items->Add("��������������: ��������� ORG");

  // ������� ���������
  if (err_count > 0)
    s.printf("��������� ����������, ���������� %d ������(��).", err_count);
  else
    s.printf("������, ��������� ����� �������� %04X ����(�) � ������ %04X.", CodeLen, Origin);
  ListBox1->Items->Add(s);

  /* ����� ��������������� */
  AsmForm->RichEdit->ReadOnly = false;  // ��������� �������������� ������
  BitBtn1->Enabled = true;              // ��������� ������ ��
}
//---------------------------------------------------------------------------

// ������ �������� �����
void __fastcall TCompileForm::BitBtn1Click(TObject *Sender)
{
  // ���� ���� ������, �� ������ ������ �� ������
  if (err_count > 0) {
    Close();
    return;
  }

  // ���� ������ ���������� � ������, �� ���������� �������� �����
  // � ����������� ������ �������
  if (CompileMode == cmInMemory)
    memcpy(&MEM[Origin], &membuf[Origin], CodeLen);

  // ���� ������ ���������� � ����, �� ���������� �������� �����
  // � ���������� � ������������ ���� ����� �����
  if (CompileMode == cmInFile) {
    if (SaveDialog1->Execute())
      gui_file_saveblock(SaveDialog1->FileName.c_str(), Origin, CodeLen);
  }

  Close();
}
//---------------------------------------------------------------------------

// ��� ������� ������ �� ������ � ���������� �� ������
// � ��������� ���������� ������ ������ ������������ �� ��� ������
void __fastcall TCompileForm::ListBox1DblClick(TObject *Sender)
{
  int i, num, res;
  char *str, *p;

  if ((i = ListBox1->ItemIndex) < 0)
    return;

  String s = ListBox1->Items->Strings[i];
  str = strdup(s.c_str());

  if (strncmp("line ",str, 5) == 0) {
    for( p = str + 5; !isdigit(*p); p++ )
      ;
    *p = '\0';
    res = sscanf(str+5, "%d", &num);
    if (res == 1) {
      // ������������� ������ �� ����� num
      AsmForm->RichEdit->CaretPos.y = num;
      AsmForm->RichEdit->CaretPos.x = 0;
    }
  }

  free(str);
}
//---------------------------------------------------------------------------

void __fastcall TCompileForm::BitBtn2Click(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

