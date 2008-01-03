// Ёмул€тор-отладчик i8080
// ќкно редактора ассемблера
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Assembler.h"
#include "Compile.h"
#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAsmForm *AsmForm;


String FileName;  // им€ редактируемого файла

BOOL bSaveAsCanceled;

/* Level of debuging for assembler (asm.c)
   0 - no debug, 3 - max level */
int DEBUG_LEVEL = 0;

//---------------------------------------------------------------------------
__fastcall TAsmForm::TAsmForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAsmForm::FileNewExecute(TObject *Sender)
{
  if (RichEdit->Modified == true) {
    switch (MessageBox(Handle,"“екст не сохранен, сохранить?","ѕредупреждение",MB_YESNOCANCEL | MB_ICONWARNING)) {
      case IDYES:
        FileSaveExecute(Sender);
        break;
      case IDCANCEL:
        bSaveAsCanceled = true;
        return;
    }
  }

  RichEdit->Clear();
  FileName = "";
  StatusBar1->Panels->Items[1]->Text = FileName;
}
//---------------------------------------------------------------------------

void __fastcall TAsmForm::FileSaveExecute(TObject *Sender)
{
  if (FileName == "")
    FileSaveAsExecute(NULL);
  else {
    RichEdit->Lines->SaveToFile(FileName);
    RichEdit->Modified = false;
    StatusBar1->Panels->Items[1]->Text = "";
  }
}
//---------------------------------------------------------------------------

void __fastcall TAsmForm::FileSaveAsExecute(TObject *Sender)
{
  if(SaveDialog1->Execute()) {
    FileName = SaveDialog1->FileName;
    RichEdit->Lines->SaveToFile(FileName);
    RichEdit->Modified = false;
    StatusBar1->Panels->Items[1]->Text = "";
    AsmForm->Caption = "јссемблер - " + FileName;
  }
  else
    bSaveAsCanceled = true;
}
//---------------------------------------------------------------------------

void __fastcall TAsmForm::FileOpenExecute(TObject *Sender)
{
  bSaveAsCanceled == false;
  if(OpenDialog1->Execute()) {
    FileNewExecute(Sender);
    if (bSaveAsCanceled == true)
      return;
    FileName = OpenDialog1->FileName;
    RichEdit->Lines->LoadFromFile(FileName);
    RichEdit->Modified = false;
    StatusBar1->Panels->Items[1]->Text = "";
    AsmForm->Caption = "јссемблер - " + FileName;
  }
}
//---------------------------------------------------------------------------

void __fastcall TAsmForm::EditSelectAllExecute(TObject *Sender)
{
  RichEdit->SelectAll();
}
//---------------------------------------------------------------------------

void __fastcall TAsmForm::EditUndoExecute(TObject *Sender)
{
  RichEdit->Undo();
}
//---------------------------------------------------------------------------


void __fastcall TAsmForm::FileExitExecute(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

void __fastcall TAsmForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  bSaveAsCanceled = false;
  if (RichEdit->Modified == true) {
    switch (MessageBox(Handle,"“екст не сохранен.\n’отите сохранить его перед выходом?","ѕредупреждение",MB_YESNOCANCEL | MB_ICONWARNING)) {
      case IDYES:
        FileSaveExecute(Sender);
        if (bSaveAsCanceled == true)
          CanClose = false;
        break;
      case IDNO:
        RichEdit->Modified = false;
        break;
      case IDCANCEL:
        CanClose = false;
    }
  }
  MainForm->AsmFormCanClose = CanClose;
}
//---------------------------------------------------------------------------

void __fastcall TAsmForm::AsmCheckExecute(TObject *Sender)
{
  CompileForm->CompileMode = cmJustTest;
  CompileForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TAsmForm::AsmMemExecute(TObject *Sender)
{
  CompileForm->CompileMode = cmInMemory;
  CompileForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TAsmForm::AsmFileExecute(TObject *Sender)
{
  CompileForm->CompileMode = cmInFile;
  CompileForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TAsmForm::FormShow(TObject *Sender)
{
//  ѕри закрытии формы и открытию вновь текст не удал€етс€
//  RichEdit->Clear();
//  RichEdit->Modified = false;
}
//---------------------------------------------------------------------------


void __fastcall TAsmForm::RichEditChange(TObject *Sender)
{
  StatusBar1->Panels->Items[1]->Text = "изменен";
}
//--------------------------------------------------------------------------

void __fastcall TAsmForm::EditFindExecute(TObject *Sender)
{
  FindDialog1->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TAsmForm::FindDialog1Find(TObject *Sender)
{
  int FoundAt, StartPos, ToEnd;
  // begin the search after the current selection
  // if there is one
  // otherwise, begin at the start of the text
  if (RichEdit->SelLength)
    StartPos = RichEdit->SelStart + RichEdit->SelLength;
  else

    StartPos = 0;

  // ToEnd is the length from StartPos
  // to the end of the text in the rich edit control

  ToEnd = RichEdit->Text.Length() - StartPos;

  FoundAt = RichEdit->FindText(FindDialog1->FindText, StartPos, ToEnd, TSearchTypes()<< stMatchCase);
  if (FoundAt != -1)
  {
    RichEdit->SetFocus();
    RichEdit->SelStart = FoundAt;
    RichEdit->SelLength = FindDialog1->FindText.Length();
  }
}
//---------------------------------------------------------------------------

void __fastcall TAsmForm::EditNextExecute(TObject *Sender)
{
  FindDialog1Find(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TAsmForm::EditReplExecute(TObject *Sender)
{
  ReplaceDialog1->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TAsmForm::ReplaceDialog1Find(TObject *Sender)
{
  int FoundAt, StartPos, ToEnd;
  // begin the search after the current selection
  // if there is one
  // otherwise, begin at the start of the text
  if (RichEdit->SelLength)
    StartPos = RichEdit->SelStart + RichEdit->SelLength;
  else

    StartPos = 0;

  // ToEnd is the length from StartPos
  // to the end of the text in the rich edit control

  ToEnd = RichEdit->Text.Length() - StartPos;

  FoundAt = RichEdit->FindText(ReplaceDialog1->FindText, StartPos, ToEnd, TSearchTypes()<< stMatchCase);
  if (FoundAt != -1)
  {
    RichEdit->SetFocus();
    RichEdit->SelStart = FoundAt;
    RichEdit->SelLength = ReplaceDialog1->FindText.Length();
  }
}
//---------------------------------------------------------------------------

void __fastcall TAsmForm::ReplaceDialog1Replace(TObject *Sender)
{
 TReplaceDialog *dlg = (TReplaceDialog *)Sender;
  /* perform a global case-sensitive search for FindText in RichEdit */
  int SelPos = RichEdit->Lines->Text.Pos(dlg->FindText);
  if (SelPos > 0)
  {
    RichEdit->SelStart = SelPos - 1;
    RichEdit->SelLength = dlg->FindText.Length();
    // Replace selected text with ReplaceText
    RichEdit->SelText = dlg->ReplaceText;
  }
  else
    MessageBeep(0);
}
//---------------------------------------------------------------------------


void __fastcall TAsmForm::FilePrintExecute(TObject *Sender)
{
  if (MessageBox(Handle, "¬ы действительно хотите распечатать весь текст?","ѕодтверждение",MB_YESNO) == IDYES) {
    TPrinter *Prntr = Printer();
    TRect r = Rect(200, 200, Prntr->PageWidth - 200, Prntr->PageHeight - 200);
    Prntr->BeginDoc();
    Prntr->Canvas->TextRect(r, 200, 200, RichEdit->Lines->Text);
    Prntr->EndDoc();
  }
}
//---------------------------------------------------------------------------

void __fastcall TAsmForm::HelpContentsExecute(TObject *Sender)
{
  Application->HelpCommand(HELP_CONTEXT, 500);
}
//---------------------------------------------------------------------------

/* ѕри вызове контекстной справки в окне редактора беретс€ текст слова,
   на котором стоит курсор и открываетс€ справка, использу€
   это как ключевое слово. ≈сли соответствующа€ стать€
   будет найдена, она отобразитс€, иначе будет выведена
   справка по редактору ассемблера.
 */
void __fastcall TAsmForm::HelpKeyExecute(TObject *Sender)
{
  // узнаем текущее положение курсора в тексте
  int start = RichEdit->SelStart;

  // ищем начало слова
  while(isalpha(RichEdit->Lines->Text[start]) && start > 0)
    start--;
  if (start != 0)
    start++;

  int end = start;
  // ищем конец слова
  while(isalpha(RichEdit->Lines->Text[end]))
    end++;

  // берем найденое слово
  String word = RichEdit->Lines->Text.SubString(start, end-start);

  // вызываем WinHelp указав в качестве второго параметра адрес искомого слова
  Application->HelpCommand(HELP_KEY, (int)word.c_str());
}
//---------------------------------------------------------------------------


void __fastcall TAsmForm::RichEditMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  TPoint p = RichEdit->CaretPos;
  // +1 чтобы нумераци€ строк была не с нул€, а с еденицы
  StatusBar1->Panels->Items[0]->Text = IntToStr(p.y+1) + ":" + IntToStr(p.x+1);
}
//---------------------------------------------------------------------------
void __fastcall TAsmForm::RichEditKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  TPoint p = RichEdit->CaretPos;
  // +1 чтобы нумераци€ строк была не с нул€, а с еденицы
  StatusBar1->Panels->Items[0]->Text = IntToStr(p.y+1) + ":" + IntToStr(p.x+1);
}
//---------------------------------------------------------------------------

