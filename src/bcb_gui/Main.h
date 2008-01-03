//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <ActnList.hpp>
#include <StdActns.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <ToolWin.hpp>
#include <Dialogs.hpp>
#include "core/gui.h"
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
        TActionList *ActionList1;
        TAction *FileClean;
        TAction *FileLoadImage;
        TAction *FileSaveImage;
        TAction *FileAsm;
        TAction *FileExit;
        TAction *ViewMem;
        TAction *ViewStatus;
        TAction *RunReset;
        TAction *RunRun;
        TAction *RunStop;
        TAction *RunStepInto;
        TAction *RunBreak;
  TAction *OptEmuSpeed;
        TAction *HelpAbout;
        TMainMenu *MainMenu;
        TMenuItem *N27;
        TMenuItem *N28;
        TMenuItem *N29;
        TMenuItem *N30;
        TMenuItem *N31;
        TMenuItem *N1;
        TMenuItem *N4;
        TMenuItem *N5;
        TMenuItem *N6;
        TMenuItem *N7;
        TMenuItem *N9;
        TMenuItem *N10;
        TMenuItem *N11;
        TMenuItem *N12;
        TMenuItem *N13;
        TMenuItem *N15;
        TMenuItem *N16;
        TMenuItem *N17;
        TMenuItem *N19;
        TMenuItem *N20;
        TMenuItem *N21;
        TMenuItem *N22;
  THelpContents *HelpHelp;
        TAction *ViewFrom;
        TMenuItem *N23;
        TMenuItem *N24;
        TAction *OptLook;
        TMenuItem *N25;
        TPaintBox *PaintBox;
        TStatusBar *StatusBar1;
        TImageList *ImageList1;
        TToolBar *ToolBar1;
        TBevel *Bevel1;
        TOpenDialog *OpenDialog1;
        TSaveDialog *SaveDialog1;
        TOpenDialog *OpenDialog2;
        TSaveDialog *SaveDialog2;
        TAction *FileLoadBlock;
        TAction *FileSaveBlock;
        TMenuItem *N2;
        TMenuItem *N3;
        TMenuItem *N26;
        TMenuItem *N32;
        TToolButton *ToolButton1;
        TToolButton *ToolButton2;
        TToolButton *ToolButton3;
        TToolButton *ToolButton4;
        TToolButton *ToolButton5;
        TToolButton *ToolButton6;
        TToolButton *ToolButton7;
        TScrollBar *ScrollBar1;
        TAction *RunStepOver;
        TToolButton *ToolButton9;
        TToolButton *ToolButton10;
        TToolButton *ToolButton11;
        TToolButton *ToolButton12;
        TToolButton *ToolButton13;
  TMenuItem *N33;
  TMenuItem *N34;
  TMenuItem *N35;
        TAction *OptModules;
        TMenuItem *N18;
        TAction *ViewFind;
        TToolButton *ToolButton8;
        TAction *ViewToolbar;
        TMenuItem *N8;
        TToolButton *ToolButton14;
        TMenuItem *N36;
        TMenuItem *N37;
        TAction *ViewRegFlag;
        TToolButton *ToolButton15;
        TAction *ViewPut;
        TMenuItem *N39;
        TAction *ViewFindNext;
        TToolButton *ToolButton16;
        TMenuItem *N14;
  TAction *FileDisas;
  TMenuItem *N40;
  TAction *HelpOnhelp;
  TMenuItem *N43;
  TMenuItem *HLTStop;
        TAction *FileLoadBin;
        TMenuItem *Loadbinaryblock1;
        TMenuItem *N41;
        TMenuItem *N38;
        TAction *RunGoto;
        TMenuItem *N42;
        void __fastcall FileExitExecute(TObject *Sender);
        void __fastcall FileAsmExecute(TObject *Sender);
        void __fastcall HelpAboutExecute(TObject *Sender);
        void __fastcall FormPaint(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall OptLookExecute(TObject *Sender);
        void __fastcall FileLoadImageExecute(TObject *Sender);
        void __fastcall FileCleanExecute(TObject *Sender);
        void __fastcall ScrollBar1Change(TObject *Sender);
        void __fastcall RunResetExecute(TObject *Sender);
        void __fastcall RunStepIntoExecute(TObject *Sender);
        void __fastcall RunStepOverExecute(TObject *Sender);
        void __fastcall RunBreakExecute(TObject *Sender);
        void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
        void __fastcall ViewFromExecute(TObject *Sender);
        void __fastcall PaintBoxClick(TObject *Sender);
        void __fastcall PaintBoxMouseMove(TObject *Sender,
          TShiftState Shift, int X, int Y);
  void __fastcall RunRunExecute(TObject *Sender);
  void __fastcall RunStopExecute(TObject *Sender);
  void __fastcall FileSaveImageExecute(TObject *Sender);
  void __fastcall FileLoadBlockExecute(TObject *Sender);
  void __fastcall FileSaveBlockExecute(TObject *Sender);
        void __fastcall OptEmuSpeedExecute(TObject *Sender);
        void __fastcall OptModulesExecute(TObject *Sender);
        void __fastcall ViewFindExecute(TObject *Sender);
        void __fastcall ViewMemExecute(TObject *Sender);
        void __fastcall ViewStatusExecute(TObject *Sender);
        void __fastcall ViewToolbarExecute(TObject *Sender);
        void __fastcall ViewRegFlagExecute(TObject *Sender);
        void __fastcall ViewPutExecute(TObject *Sender);
        void __fastcall FormMouseWheelDown(TObject *Sender,
          TShiftState Shift, TPoint &MousePos, bool &Handled);
        void __fastcall FormMouseWheelUp(TObject *Sender,
          TShiftState Shift, TPoint &MousePos, bool &Handled);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall ViewFindNextExecute(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
  void __fastcall FileDisasExecute(TObject *Sender);
  void __fastcall HelpHelpExecute(TObject *Sender);
  void __fastcall HelpOnhelpExecute(TObject *Sender);
        void __fastcall FileLoadBinExecute(TObject *Sender);
        void __fastcall RunGotoExecute(TObject *Sender);
private:
        void __fastcall SetActionEnableStates();	// User declarations
public:		// User declarations
        __fastcall TMainForm(TComponent* Owner);
  bool AsmFormCanClose;
protected:

void __fastcall CMIsBreak(TMessage &Message);

BEGIN_MESSAGE_MAP
  VCL_MESSAGE_HANDLER(CM_ISBREAK, TMessage, CMIsBreak);
END_MESSAGE_MAP(TForm)
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
