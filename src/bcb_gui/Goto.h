//---------------------------------------------------------------------------

#ifndef GotoH
#define GotoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TGotoForm : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TLabel *Label2;
        TEdit *Edit1;
        TBitBtn *BitBtn1;
        TBitBtn *BitBtn2;
        void __fastcall Edit1Change(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        unsigned short int addr;
        __fastcall TGotoForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TGotoForm *GotoForm;
//---------------------------------------------------------------------------
#endif
