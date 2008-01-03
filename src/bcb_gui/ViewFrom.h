//---------------------------------------------------------------------------

#ifndef ViewFromH
#define ViewFromH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TViewFromForm : public TForm
{
__published:	// IDE-managed Components
        TBitBtn *BitBtn1;
        TBitBtn *BitBtn2;
        TEdit *Edit1;
        TLabel *Label1;
        TLabel *Label2;
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall Edit1Change(TObject *Sender);
private:
public:		// User declarations
        unsigned short int addr;
        __fastcall TViewFromForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TViewFromForm *ViewFromForm;
//---------------------------------------------------------------------------
#endif
