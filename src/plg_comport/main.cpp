#include <vcl.h>
#include <windows.h>
#include <inifiles.hpp>
#pragma hdrstop
#include <stdio.h>
#include "CfgForm.h"


#include "core/plugin.h"

/* Описание устройства */
char description[] = "Real COM port redirect (com_port.dll)";

/* Название внешнего модуля устройства (название секции в файле конфигурации) */
char DEVICE_NAME[] = "Plugin_comport";

/* Адрес порта (и значение по умолчанию до его изменения в настройках) */
int device_port = 0xc1;

/* Номер реального COM-порта */
int real_com_port_number;

/* Флаг разрешения чтения */
BOOL can_read;

/* Флаг разрешения записи */
BOOL can_write;

/* Флаг открытия COM-порта */
int COM_opened = 0;

/* Файловый дескриптор COM-порта */
HANDLE com_port;

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
        return 1;
}
//---------------------------------------------------------------------------

/* Попытка подключения к реальному COM-порту компьютера
   Возвращает 1 если успешно
   0 в случае ошибки
 */
int Init_Real_COM_port()
{
 char com_port_name[16];
 DWORD BaudRate;
 BYTE Buff[10];
 DWORD nread,nwrite;

  /* Формируем имя COM-порта */
  sprintf(com_port_name, "COM%d", real_com_port_number);

  BaudRate = 1200;

  /* Открываем порт */
  com_port = CreateFile(com_port_name, GENERIC_READ | GENERIC_WRITE,
      FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
  if (com_port==INVALID_HANDLE_VALUE) {
    char error_text[200];
    sprintf(error_text,"'%s' Open Error %d\n", com_port_name, GetLastError());
    ShowMessage(error_text);
    return 0;
  }
  // Получаем текущие параметры COM-порта
  DCB dcb;
  if (GetCommState(com_port, &dcb)) {
     // Устанавливаем требуемые параметры
     // скорость
     dcb.BaudRate = BaudRate;
     // формат линии
     dcb.ByteSize = 7; dcb.Parity = 0; dcb.StopBits = 0;
     // прочее
     dcb.fBinary =  1;
     dcb.fOutxCtsFlow = 0;      // CTS output flow control
     dcb.fOutxDsrFlow = 0;      // DSR output flow control
     // а вот здесь может потребоваться изменить на нужное
     dcb.fDtrControl  = 0;      // DTR flow control type
     //
     dcb.fDsrSensitivity = 0;   // DSR sensitivity
     dcb.fTXContinueOnXoff = 1; // XOFF continues Tx
     dcb.fOutX        = 0;      // XON/XOFF out flow control
     dcb.fInX         = 0;      // XON/XOFF in flow control
     dcb.fRtsControl  = 0;      // RTS flow control
     dcb.fAbortOnError= 0;      // abort reads/writes on error
     SetCommState(com_port, &dcb);
  }
  // устанавливаем таймауты
  COMMTIMEOUTS touts;
  if (GetCommTimeouts(com_port, &touts)) {
     touts.ReadIntervalTimeout = MAXDWORD;  // интервал не используется
     touts.ReadTotalTimeoutMultiplier = 0;
     touts.ReadTotalTimeoutConstant = 0;    // таймаут чтения не используется
                                            // (чтение без ожидания)
     SetCommTimeouts(com_port, &touts);
  }
  // Выставляем DTR и RTS
  EscapeCommFunction(com_port, SETDTR);
  EscapeCommFunction(com_port, SETRTS);

  // возвращаем флаг удачной инициализации
  return 1;
}


/*    Функции интерфейса с основной программой    */
/*    должны быть объявлены как С (а не С++)      */


/* Сброс устройства (вызывается, когда сбрасывается микропроцессорная система */
extern "C" void reset()
{
}


/* Показывает окно свойств устройства.
   Занасение в поля формы начальных данных
   и запись измененых настроек по кнопке ОК
   происходит в коде формы
 */
extern "C" void config()
{
  TConfigForm *ConfigForm = new TConfigForm(NULL);
  ConfigForm->ShowModal();
  delete ConfigForm;
}

/* Чтение байта из порта */
extern "C" BYTE read(WORD port)
{
  char byte;
  unsigned long nread;

  if ( !can_read )
    return 0xff;

  if ( !COM_opened ) {
    if(Init_Real_COM_port())
      COM_opened = 1;
    else
      return 0xff;
  }

  nread = 0;
  ReadFile(com_port, &byte, 1, &nread, NULL);

  /* можно сделать проверку */
//  if (nread == 0);

  return byte;
}

/* Запись байта в порт */
extern "C" void write(WORD port, BYTE data)
{
  unsigned long nwrite;

  if ( !can_write )
    return;

  if ( !COM_opened ) {
    if(Init_Real_COM_port())
      COM_opened = 1;
    else
      return;
  }

  nwrite = 0;
  WriteFile(com_port, &data, 1, &nwrite, NULL);

  /* можно сделать проверку */
//  if (nwrite == 0);
}


/* int __export DeviceInit(struct plugin_device *dev)
   Экспортируемая из dll функция, которая должна инициализировать
   устройство и заполнить указатели на функции и вернуть 1,
   или если инициализация устройства прошла неудачно, вернуть 0
 */
extern "C" int __export DeviceInit(struct plugin_device *dev)
{
  DWORD dwVersion;

  /* Открываем файл настроек программы */
  TIniFile *ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));

  /* Считываем адрес порта из файла настроек, а если оно не задано, берем по умолчанию */
  device_port = ini->ReadInteger( DEVICE_NAME, "Port", device_port);

  /* Считываем остальные настройки */
  real_com_port_number = ini->ReadInteger( DEVICE_NAME, "RealCOM", 2);
  can_read = ini->ReadInteger( DEVICE_NAME, "can_read", 1);
  can_write = ini->ReadInteger( DEVICE_NAME, "can_write", 1);

  /* закрываем файл настроек */
  delete ini;

  /* Заполняем необходимые поля структуры plagin */
  dev->descr = description;   // описание
  dev->mask = 0xff; // Маска порта устройства (подробности в readme.txt)
  dev->port = device_port; // адрес порта устройства
  dev->read = read; // функция чтения из порта
  dev->write = write; // функция записи в порт
  dev->reset = reset; // функция сброса устройства
  dev->config = config;  /* функция диалога настроек или NULL, если устройство его не имеет */

  /* возвращаем флаг удачной инициализации */
  return 1;

/* Возможность открытия реального COM-порта будет проверена
  при первом обращении к нему, иначе пользователь не имел бы
  доступа к диологу настроек
 */
}

