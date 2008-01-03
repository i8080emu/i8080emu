//---------------------------------------------------------------------------

#ifndef MpsysOptH
#define MpsysOptH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include "CSPIN.h"
//---------------------------------------------------------------------------
class TMPsysOptForm : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TGroupBox *GroupBox1;
        TRadioButton *AutoRadioButton;
        TRadioButton *ManualRadioButton;
        TRadioButton *RadioButton1;
        TLabel *Label2;
        TTrackBar *DelayTrackBar;
        TLabel *Label3;
        TLabel *Label4;
        TBitBtn *BitBtn1;
        TBitBtn *BitBtn2;
        TBitBtn *BitBtn3;
        TCSpinEdit *CSpinEdit1;
        void __fastcall ManualRadioButtonClick(TObject *Sender);
        void __fastcall AutoRadioButtonClick(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TMPsysOptForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMPsysOptForm *MPsysOptForm;
//---------------------------------------------------------------------------
#endif
