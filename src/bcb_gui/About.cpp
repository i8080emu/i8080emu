// Эмулятор-отладчик i8080
// Окно с информацией о программе
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "About.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAboutForm *AboutForm;
//---------------------------------------------------------------------------
__fastcall TAboutForm::TAboutForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAboutForm::Button1Click(TObject *Sender)
{
        Close();        
}
//---------------------------------------------------------------------------

void __fastcall TAboutForm::StaticText2Click(TObject *Sender)
{
//  Shell("start http://sashnov.nm.ru/i8080emu.html");
}
//---------------------------------------------------------------------------

