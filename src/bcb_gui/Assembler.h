//---------------------------------------------------------------------------

#ifndef AssemblerH
#define AssemblerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ActnList.hpp>
#include <ComCtrls.hpp>
#include <StdActns.hpp>
#include <ToolWin.hpp>
#include <Dialogs.hpp>
#include <ImgList.hpp>
//---------------------------------------------------------------------------
class TAsmForm : public TForm
{
__published:	// IDE-managed Components
        TRichEdit *RichEdit;
        TToolBar *ToolBar1;
        TActionList *ActionList1;
        TEditCopy *EditCopy;
        TEditCut *EditCut;
        TEditDelete *EditDelete;
        TEditPaste *EditPaste;
        TEditSelectAll *EditSelectAll;
        TEditUndo *EditUndo;
        THelpContents *HelpContents;
        TAction *FileNew;
        TAction *FileOpen;
        TAction *FileSave;
        TAction *FileSaveAs;
        TAction *FileExit;
        TMainMenu *MainMenu1;
        TMenuItem *N1;
        TMenuItem *FileNew1;
        TMenuItem *N2;
        TMenuItem *N3;
        TMenuItem *N4;
        TMenuItem *N5;
        TMenuItem *N6;
        TMenuItem *N7;
        TMenuItem *N8;
        TMenuItem *N9;
        TMenuItem *N10;
        TMenuItem *N11;
        TMenuItem *N12;
        TMenuItem *N13;
        TMenuItem *N14;
        TAction *EditFind;
        TAction *EditRepl;
        TAction *EditNext;
        TMenuItem *N15;
        TMenuItem *N16;
        TMenuItem *N17;
        TMenuItem *N18;
        TMenuItem *N19;
        TMenuItem *N20;
        TMenuItem *N21;
        TAction *AsmCheck;
        TAction *AsmMem;
        TAction *AsmFile;
        TMenuItem *N22;
        TMenuItem *N23;
        TMenuItem *N24;
        TMenuItem *N25;
        TOpenDialog *OpenDialog1;
        TSaveDialog *SaveDialog1;
        TStatusBar *StatusBar1;
        TImageList *ImageList1;
        TToolButton *ToolButton1;
        TToolButton *ToolButton2;
        TToolButton *ToolButton3;
        TToolButton *ToolButton4;
        TToolButton *ToolButton5;
        TToolButton *ToolButton6;
        TToolButton *ToolButton7;
        TToolButton *ToolButton8;
        TToolButton *ToolButton9;
        TToolButton *ToolButton10;
        TToolButton *ToolButton11;
        TToolButton *ToolButton12;
  TToolButton *ToolButton13;
  TToolButton *ToolButton14;
  TToolButton *ToolButton15;
        TFindDialog *FindDialog1;
        TReplaceDialog *ReplaceDialog1;
  TAction *FilePrint;
  TMenuItem *N26;
  TMenuItem *N27;
  TAction *HelpKey;
        void __fastcall FileNewExecute(TObject *Sender);
        void __fastcall FileSaveExecute(TObject *Sender);
        void __fastcall FileSaveAsExecute(TObject *Sender);
        void __fastcall FileOpenExecute(TObject *Sender);
        void __fastcall EditSelectAllExecute(TObject *Sender);
        void __fastcall EditUndoExecute(TObject *Sender);
        void __fastcall FileExitExecute(TObject *Sender);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall AsmCheckExecute(TObject *Sender);
        void __fastcall AsmMemExecute(TObject *Sender);
        void __fastcall AsmFileExecute(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall RichEditChange(TObject *Sender);
        void __fastcall EditFindExecute(TObject *Sender);
        void __fastcall FindDialog1Find(TObject *Sender);
        void __fastcall ReplaceDialog1Replace(TObject *Sender);
        void __fastcall ReplaceDialog1Find(TObject *Sender);
        void __fastcall EditReplExecute(TObject *Sender);
        void __fastcall EditNextExecute(TObject *Sender);
  void __fastcall FilePrintExecute(TObject *Sender);
  void __fastcall HelpContentsExecute(TObject *Sender);
  void __fastcall HelpKeyExecute(TObject *Sender);
        void __fastcall RichEditMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
        void __fastcall RichEditKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
private:	// User declarations
public:		// User declarations
        __fastcall TAsmForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAsmForm *AsmForm;
//---------------------------------------------------------------------------
#endif
