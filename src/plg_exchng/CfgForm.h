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

// байт, сигнализирующий начало передачи
#define START_TRANSFER_CODE  0x0f

// байт готовности данных
#define DATA_READY_CODE  0x00

// байт неготовности данных или ошибка
#define ERROR_CODE  0xff


typedef enum {
TM_SYNC,
TM_ASYNC_KVIT,
TM_ASYNC_INT
} transfer_mode_t;


class TConfigForm : public TForm
{
__published:	// IDE-managed Components
        TBitBtn *BitBtn1;
        TLabel *Label1;
        TLabel *Label3;
        TBitBtn *BitBtn2;
        TLabel *Label5;
        TLabel *Label6;
  TLabel *Label4;
  TGroupBox *GroupBox1;
  TRadioButton *RadioButton1;
  TRadioButton *RadioButton2;
  TRadioButton *RadioButton3;
  TLabel *Label2;
  TEdit *Edit3;
  TLabel *Label7;
  TLabel *Label8;
  TEdit *DataEdit;
  TLabel *Label9;
        void __fastcall FormShow(TObject *Sender);
  void __fastcall BitBtn1Click(TObject *Sender);
  void __fastcall Edit1Change(TObject *Sender);
  void __fastcall RadioButton1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
        __fastcall TConfigForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TConfigForm *ConfigForm;
//---------------------------------------------------------------------------
#endif
