// Эмулятор-отладчик i8080
// Дамп памяти
//---------------------------------------------------------------------------
#include <vcl.h>
#include "Memory.h"
#pragma hdrstop

#include "core/mpsys.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TMemoryForm *MemoryForm;

extern String font_family;
extern int font_height;


//---------------------------------------------------------------------------
__fastcall TMemoryForm::TMemoryForm(TComponent* Owner)
        : TForm(Owner)
{
  mem_top = 0;
}
//---------------------------------------------------------------------------
void __fastcall TMemoryForm::FormPaint(TObject *Sender)
{
  WORD addr;
  int y = 0;
  String s;

  PaintBox1->Canvas->Font->Height = font_height;
  PaintBox1->Canvas->Font->Name = font_family;

  for (addr=mem_top; y < PaintBox1->Height; addr += 8)
  {
    /* Формируем строку */
    s = IntToHex(addr,4) + "   ";       // адрес

    // по восемь кодов в строке
    for (int n=0; n<8; n++)
      s += " " + IntToHex( MEM[ (WORD)addr+n ], 2 );

    s += "    ";  // отступ

    // в символьном виде
    for (int n=0; n<8; n++) {
      char c = MEM[ (WORD)addr+n ];
      s += (isprint(c))? c: '?';
    }

    /* Вывод строки */
    PaintBox1->Canvas->TextOut(3, y, s);

    /* Переходим к следующей строке */
    y += PaintBox1->Canvas->Font->Height;

    if (y < 0) break;
  }
}
//---------------------------------------------------------------------------
void __fastcall TMemoryForm::ScrollBar1Scroll(TObject *Sender,
      TScrollCode ScrollCode, int &ScrollPos)
{
  mem_top = ScrollPos;
  MemoryForm->Repaint();
}
//---------------------------------------------------------------------------


