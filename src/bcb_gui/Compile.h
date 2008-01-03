//---------------------------------------------------------------------------

#ifndef CompileH
#define CompileH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>

#include "core/asm.h"
#include <Dialogs.hpp>
//---------------------------------------------------------------------------

typedef enum {
cmJustTest,
cmInMemory,
cmInFile
} TCompileMode;

class TCompileForm : public TForm
{
__published:	// IDE-managed Components
        TBitBtn *BitBtn1;
        TBitBtn *BitBtn2;
        TBitBtn *BitBtn3;
  TSaveDialog *SaveDialog1;
        TListBox *ListBox1;
        void __fastcall FormShow(TObject *Sender);
  void __fastcall BitBtn1Click(TObject *Sender);
  void __fastcall ListBox1DblClick(TObject *Sender);
        void __fastcall BitBtn2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TCompileForm(TComponent* Owner);
  void __fastcall DoPass(ASMSESSION *as);
  TCompileMode CompileMode;
  BYTE membuf[0x10000];
  WORD Origin, CodeLen;
  int err_count;
};
//---------------------------------------------------------------------------
extern PACKAGE TCompileForm *CompileForm;
//---------------------------------------------------------------------------
#endif
