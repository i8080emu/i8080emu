//---------------------------------------------------------------------------

#ifndef LoadBinH
#define LoadBinH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TLoadBinForm : public TForm
{
__published:	// IDE-managed Components
        TLabel *Label1;
        TEdit *FileEdit;
        TButton *BrowseButton;
        TLabel *Label2;
        TEdit *StartEdit;
        TRadioButton *RadioButton1;
        TRadioButton *RadioButton2;
        TEdit *LenEdit;
        TLabel *Label3;
        TBitBtn *BitBtn1;
        TBitBtn *BitBtn2;
        TBitBtn *BitBtn3;
        TOpenDialog *OpenDialog1;
        void __fastcall BrowseButtonClick(TObject *Sender);
        void __fastcall StartEditChange(TObject *Sender);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall RadioButton1Click(TObject *Sender);
        void __fastcall RadioButton2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TLoadBinForm(TComponent* Owner);
  int start, len;
};
//---------------------------------------------------------------------------
extern PACKAGE TLoadBinForm *LoadBinForm;
//---------------------------------------------------------------------------
#endif
