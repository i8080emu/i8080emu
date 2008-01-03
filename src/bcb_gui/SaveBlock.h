//---------------------------------------------------------------------------

#ifndef SaveBlockH
#define SaveBlockH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TSaveBlockForm : public TForm
{
__published:	// IDE-managed Components
  TBitBtn *BitBtn1;
  TBitBtn *BitBtn2;
  TBitBtn *BitBtn3;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
  TEdit *Edit1;
  TEdit *Edit2;
  void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall Edit1Change(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TSaveBlockForm(TComponent* Owner);
  int addr, len;
};
//---------------------------------------------------------------------------
extern PACKAGE TSaveBlockForm *SaveBlockForm;
//---------------------------------------------------------------------------
#endif
