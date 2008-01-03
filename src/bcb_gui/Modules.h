//---------------------------------------------------------------------------

#ifndef ModulesH
#define ModulesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TModulesForm : public TForm
{
__published:	// IDE-managed Components
        TBitBtn *CloseBitBtn;
        TBitBtn *HelpBitBtn;
        TListBox *ListBox1;
        TButton *PropertyButton;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall PropertyButtonClick(TObject *Sender);
        void __fastcall ListBox1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TModulesForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TModulesForm *ModulesForm;
//---------------------------------------------------------------------------
#endif
