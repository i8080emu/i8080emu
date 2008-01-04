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


//---------------------------------------------------------------------------
__fastcall TConfigForm::TConfigForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TConfigForm::FormShow(TObject *Sender)
{
  /* Заносим в элементы диалога настройки
     информацию из переменных конфигурации
   */
  Edit1->Text = IntToHex(device_port, 2);
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

  /* Если новые значения настроек введены правильные,
     то сохраняем их в соответствующих переменных.
   */
  device_port = port;

  /* а также в ini-файл программы */
  TIniFile *ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));
  ini->WriteInteger( DEVICE_NAME, "Port", device_port);
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

