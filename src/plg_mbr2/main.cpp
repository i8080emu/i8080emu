/*
*  Многорежимный буферный регистр (МБР)
*/
#include <vcl.h>
#include <windows.h>
#pragma hdrstop

#include "CfgForm.h"
#include "State.h"

#include "../../kernel/plugin.h"

/* Описание устройства */
char description[] = "Многорежимный буферный регистр (mbr.dll)";

/* Адрес порта, к которому подключено устройство */
int mbr_port;

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
        return 1;
}
//---------------------------------------------------------------------------

int InitWindow()
{
  MbrStateForm = new TMbrStateForm(NULL);
  MbrStateForm->Show();

  return 1;
}


/* Функции интерфейса с основной программой
   должны быть объявлены как С (а не С++)
 */

/* Сброс устройства (вызывается, когда сбрасывается микропроцессорная система */
extern "C" void reset()
{
  MbrStateForm->Show();
}

/* Показывает окно свойств устройства */
extern "C" void config()
{
  ConfigForm = new TConfigForm(NULL);
  ConfigForm->ShowModal();
  delete ConfigForm;
}

/* Чтение байта из порта */
extern "C" BYTE read(WORD por)
{
  return 0xff;
}

/* Запись байта в порт */
extern "C" void write(WORD port, BYTE data)
{
}


/* Экспортируемая из dll функция, которая должна инициализировать
   устройство и заполнить указатели на функции и вернуть 1,
   или если инициализация устройства прошла неудачно, вернуть 0
 */
extern "C" int __export DeviceInit(struct plugin_device *dev)
{
  /* Описание устройства */
  dev->descr = description;

  /* Маска и адрес порта устройства */
  dev->mask = 0xff;
  dev->port = mbr_port;

  /* заполняем указатели на функции интерфейса */
  dev->read = read;
  dev->write = write;
  dev->reset = reset;
  dev->config = config;  /* или NULL, если устройство не настраивается */

  /* Пытаемся создать окно и запустить процедуру в отдельном потоке */
  if (!InitWindow())
    return 0;

  /* возвращаем флаг удачной инициализации */
  return 1;
}

