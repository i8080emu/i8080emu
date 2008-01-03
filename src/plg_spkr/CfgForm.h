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

/* Адрес порта, к которому подключено устройство */
#define SPEAKER_PORT 0xf4

/* Маска бита, отвечающего за состояние спикера */
#define SPEAKER_MASK 0x01

class TConfigForm : public TForm
{
__published:	// IDE-managed Components
        TBitBtn *BitBtn1;
        TEdit *Edit1;
        TLabel *Label1;
        TLabel *Label3;
        TBitBtn *BitBtn2;
        TLabel *Label2;
        TRadioButton *RadioButton1;
        TRadioButton *RadioButton2;
        TLabel *Label4;
        TLabel *Label5;
        TLabel *Label6;
  TLabel *Label7;
        TBitBtn *BitBtn3;
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
private:	// User declarations
public:		// User declarations
        __fastcall TConfigForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TConfigForm *ConfigForm;
//---------------------------------------------------------------------------
#endif
