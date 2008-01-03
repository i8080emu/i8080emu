//---------------------------------------------------------------------------

#ifndef CfgFormH
#define CfgFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------

class TConfigForm : public TForm
{
__published:	// IDE-managed Components
        TBitBtn *BitBtn1;
        TEdit *Edit1;
        TLabel *Label1;
        TLabel *Label3;
        TBitBtn *BitBtn2;
        TLabel *Label5;
        TLabel *Label6;
  TLabel *Label2;
  TCheckBox *CheckBox1;
  TCheckBox *CheckBox2;
  TComboBox *PortComboBox;
        void __fastcall FormShow(TObject *Sender);
  void __fastcall BitBtn1Click(TObject *Sender);
  void __fastcall Edit1Change(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TConfigForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TConfigForm *ConfigForm;
//---------------------------------------------------------------------------
#endif
