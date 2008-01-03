//---------------------------------------------------------------------------
#include <vcl.h>
#include <inifiles.hpp>
#pragma hdrstop

#include <stdio.h>
#include "CfgForm.h"
#include "Utils.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TConfigForm *ConfigForm;
extern char DEVICE_NAME[];

/* здесь должны быть объявления на внешние переменые,
   значения которых можно настраивать */
extern int device_port;
extern int real_com_port_number;
extern BOOL can_read;
extern BOOL can_write;

//---------------------------------------------------------------------------
__fastcall TConfigForm::TConfigForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------


/* Открытие окна настроек устройства.
   Во все настраиваемые поля необходимо занести
   информацию о текущих настройках
  */
void __fastcall TConfigForm::FormShow(TObject *Sender)
{
  Edit1->Text = IntToHex(device_port, 2);

  PortComboBox->ItemIndex = real_com_port_number;

  CheckBox1->Enabled = can_read;
  CheckBox2->Enabled = can_write;
}
//---------------------------------------------------------------------------


/* Щелчок по кнопке OK.
   Здесь необходимо сделать проверку на допустимость введеных параметров
   и если все нормально, записать их в соответствующие переменные
   и файл конфигурации программы (i8080emu.ini)
 */
void __fastcall TConfigForm::BitBtn1Click(TObject *Sender)
{
  int port, res;

  /* считывание и проверка адреса порта */
  res = sscanf(Edit1->Text.c_str(), "%x", &port);

  /* если введено некорректное значение, показываем сообщение об ошибке */
  if (res != 1 || port<0 || port>255) {
    ShowMessage(" В поле адреса порта введено некоректное значение!");
    return;
  }

  /* если все хорошо, сохраняем новое значение адреса порта в
     переменной и файле настроек и выходим */
  device_port = port;

  if (PortComboBox->ItemIndex >= 0)
    real_com_port_number = PortComboBox->ItemIndex;

  can_read  = CheckBox1->Checked;
  can_write = CheckBox2->Checked;

  /* Записываем все настройки в ini-файл программы */
  TIniFile *ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));
  ini->WriteInteger( DEVICE_NAME, "Port", device_port);
  ini->WriteInteger( DEVICE_NAME, "RealCOM", real_com_port_number);
  ini->WriteInteger( DEVICE_NAME, "can_read", can_read);
  ini->WriteInteger( DEVICE_NAME, "can_write", can_write);
  delete ini;

  Close();
}
//---------------------------------------------------------------------------

void __fastcall TConfigForm::Edit1Change(TObject *Sender)
{
  // оставляем в поле только шестнадцатиричные цифры
  TEdit *e = (TEdit*)Sender;
  e->Text = GetXdigitOnly(e->Text);
}
//---------------------------------------------------------------------------

