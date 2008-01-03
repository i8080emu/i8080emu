// Эмулятор-отладчик i8080
// Настройка скорости эмуляции
//---------------------------------------------------------------------------
#include <vcl.h>
#include <inifiles.hpp>
#pragma hdrstop

#include "MpsysOpt.h"

#include "core\mpsys.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TMPsysOptForm *MPsysOptForm;
//---------------------------------------------------------------------------
__fastcall TMPsysOptForm::TMPsysOptForm(TComponent* Owner)
        : TForm(Owner)
{
  TIniFile *ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));

  mpsys_hz = ini->ReadInteger( "Speed", "Processor_Hz", 2000000 );
  mpsys_delay = ini->ReadInteger( "Speed", "Delay", 500 );
  mpsys_delaymode = delaymode_t(ini->ReadInteger( "Speed", "DelayMode", DELAY_AUTO));

  delete ini;
}
//---------------------------------------------------------------------------
void __fastcall TMPsysOptForm::ManualRadioButtonClick(TObject *Sender)
{
  DelayTrackBar->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TMPsysOptForm::AutoRadioButtonClick(TObject *Sender)
{
  DelayTrackBar->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TMPsysOptForm::BitBtn1Click(TObject *Sender)
{
  mpsys_hz = CSpinEdit1->Value;

  if (AutoRadioButton->Checked) { /* Автоматический выбор задержки */
    mpsys_delay = 500;
    mpsys_delaymode = DELAY_AUTO;
  }
  else if(ManualRadioButton->Checked) { /* Ручная установка */
    mpsys_delay = DelayTrackBar->Position;
    mpsys_delaymode = DELAY_USER;
  }
  else { /* Без задержки */
    mpsys_delay = 0;
    mpsys_delaymode = DELAY_NO;
  }
}
//---------------------------------------------------------------------------
void __fastcall TMPsysOptForm::FormDestroy(TObject *Sender)
{
  TIniFile *ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));

  ini->WriteInteger( "Speed", "Processor_Hz", mpsys_hz);
  ini->WriteInteger( "Speed", "Delay", mpsys_delay);
  ini->WriteInteger( "Speed", "DelayMode", mpsys_delaymode);

  delete ini;
}
//---------------------------------------------------------------------------

void __fastcall TMPsysOptForm::FormShow(TObject *Sender)
{
  CSpinEdit1->Value = mpsys_hz;

  switch(mpsys_delaymode)
  {
    case DELAY_AUTO:
      AutoRadioButton->Checked = true;
      break;
    case DELAY_USER:
      ManualRadioButton->Checked = true;
      DelayTrackBar->Position = mpsys_delay;
      break;
    case DELAY_NO:
      RadioButton1->Checked = true;
  }
}
//---------------------------------------------------------------------------

