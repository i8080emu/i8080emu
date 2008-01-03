//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "State.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMbrStateForm *MbrStateForm;
//---------------------------------------------------------------------------
__fastcall TMbrStateForm::TMbrStateForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMbrStateForm::FormPaint(TObject *Sender)
{
  Draw7();
}
//---------------------------------------------------------------------------

// рисование семисегментного индикатора
void TMbrStateForm::  Draw7()
{
  TColor col_on = clLime;
  TColor col_off = clGray;

  PaintBox1->Canvas->Pen->Width = 4;

  PaintBox1->Canvas->Pen->Color =
    (data & 0x01)? col_on : col_off;
  PaintBox1->Canvas->MoveTo(6,0);
  PaintBox1->Canvas->LineTo(15+6,0);

}
