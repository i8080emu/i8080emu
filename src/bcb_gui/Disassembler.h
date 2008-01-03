//---------------------------------------------------------------------------

#ifndef DisassemblerH
#define DisassemblerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
//---------------------------------------------------------------------------
class TDisasForm : public TForm
{
__published:	// IDE-managed Components
  TRichEdit *RichEdit1;
  TBitBtn *BitBtn1;
  TButton *Button1;
  TButton *Button2;
  TGroupBox *GroupBox1;
  TRadioButton *RadioButton1;
  TRadioButton *RadioButton2;
  TLabel *Label1;
  TLabel *Label2;
  TEdit *Edit1;
  TEdit *Edit2;
  TBitBtn *BitBtn2;
  TPrintDialog *PrintDialog1;
  TSaveDialog *SaveDialog1;
  TPrinterSetupDialog *PrinterSetupDialog1;
  TButton *Button3;
  void __fastcall BitBtn2Click(TObject *Sender);
  void __fastcall Button1Click(TObject *Sender);
  void __fastcall RadioButton1Click(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall Button2Click(TObject *Sender);
  void __fastcall Button3Click(TObject *Sender);
  void __fastcall Edit1Change(TObject *Sender);
private:
  void GenerateDisass();	// User declarations
public:		// User declarations
  __fastcall TDisasForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDisasForm *DisasForm;
//---------------------------------------------------------------------------
#endif

