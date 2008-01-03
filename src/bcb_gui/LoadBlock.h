//---------------------------------------------------------------------------

#ifndef LoadBlockH
#define LoadBlockH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TLoadBlockForm : public TForm
{
__published:	// IDE-managed Components
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
  TEdit *Edit1;
  TEdit *Edit2;
  TBitBtn *BitBtn1;
  TBitBtn *BitBtn2;
  TBitBtn *BitBtn3;
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall Edit1Change(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TLoadBlockForm(TComponent* Owner);
  String BlockFilename;
  int addr, len;
};
//---------------------------------------------------------------------------
extern PACKAGE TLoadBlockForm *LoadBlockForm;
//---------------------------------------------------------------------------
#endif
