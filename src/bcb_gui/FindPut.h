//---------------------------------------------------------------------------

#ifndef FindPutH
#define FindPutH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>

#include "core/gui.h"
//---------------------------------------------------------------------------
class TFindPutForm : public TForm
{
__published:	// IDE-managed Components
        TRadioButton *RadioButton1;
        TRadioButton *RadioButton2;
        TEdit *Edit1;
        TEdit *Edit2;
  TBitBtn *HelpBitBtn;
        TBitBtn *BitBtn2;
        TBitBtn *BitBtn3;
  TRadioButton *RadioButton3;
  TEdit *Edit3;
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Edit1Change(TObject *Sender);
        void __fastcall Edit2Change(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall BitBtn2Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
  void __fastcall Edit3Change(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TFindPutForm(TComponent* Owner);
        BYTE data[MAX_HEX_SEQUENCE];  // введеные данные (мнемоника или код)
        int len;  // длинна введеной последовательности
};
//---------------------------------------------------------------------------
extern PACKAGE TFindPutForm *FindPutForm;
//---------------------------------------------------------------------------
#endif
