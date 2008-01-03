//---------------------------------------------------------------------------

#ifndef LookOptH
#define LookOptH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TLookOptForm : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl;
        TBitBtn *BitBtn1;
        TBitBtn *BitBtn2;
        TBitBtn *BitBtn3;
        TTabSheet *TabSheetColors;
        TTabSheet *TabSheetFont;
        TColorDialog *ColorDialog1;
        TFontDialog *FontDialog;
        TListBox *ElementListBox;
        TComboBox *ColorShemeComboBox;
        TLabel *Label1;
        TLabel *Label2;
        TShape *Shape1;
        TLabel *Label3;
        TButton *Button1;
        TLabel *Label4;
        TShape *Shape2;
        TButton *Button2;
        TListBox *FontListBox;
        TLabel *Label5;
        TLabel *Label6;
        TComboBox *FontSizeComboBox;
        TLabel *Label7;
        TLabel *ExemplLabel;
        TColorDialog *ColorDialog2;
        TButton *Button4;
        TButton *Button5;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FontListBoxClick(TObject *Sender);
        void __fastcall BitBtn1Click(TObject *Sender);
        void __fastcall ColorShemeComboBoxChange(TObject *Sender);
        void __fastcall ElementListBoxClick(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall Shape1MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Shape2MouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TLookOptForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TLookOptForm *LookOptForm;
//---------------------------------------------------------------------------
#endif
