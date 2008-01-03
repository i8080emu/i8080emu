//---------------------------------------------------------------------------

#ifndef MemoryH
#define MemoryH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TMemoryForm : public TForm
{
__published:	// IDE-managed Components
        TScrollBar *ScrollBar1;
        TPaintBox *PaintBox1;
        void __fastcall FormPaint(TObject *Sender);
        void __fastcall ScrollBar1Scroll(TObject *Sender,
          TScrollCode ScrollCode, int &ScrollPos);
private:	// User declarations
public:		// User declarations
        __fastcall TMemoryForm(TComponent* Owner);
        WORD mem_top;   // адрес, с которого отображается

};
//---------------------------------------------------------------------------
extern PACKAGE TMemoryForm *MemoryForm;
//---------------------------------------------------------------------------
#endif
