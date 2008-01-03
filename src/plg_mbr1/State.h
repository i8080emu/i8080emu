//---------------------------------------------------------------------------

#ifndef StateH
#define StateH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TMbrStateForm : public TForm
{
__published:	// IDE-managed Components
  TPaintBox *PaintBox1;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *ModeLabel;
  TLabel *PortLabel;
  void __fastcall FormPaint(TObject *Sender);
private:
  void   Draw7();	// User declarations
public:		// User declarations
        __fastcall TMbrStateForm(TComponent* Owner);
  BYTE data;
};
//---------------------------------------------------------------------------
extern PACKAGE TMbrStateForm *MbrStateForm;
//---------------------------------------------------------------------------
#endif
