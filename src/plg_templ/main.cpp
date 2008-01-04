/*  Эмулятор-отладчик i8080

  Template plugin


Шаблон плагина для эмулятора

*/
#include <vcl.h>
#include <windows.h>
#include <inifiles.hpp>
#pragma hdrstop
#include "core/plugin.h"
#include "CfgForm.h"

/* Описание устройства */
char description[] = "Template for plugin (templ.dll)";

/* Название внешнего модуля устройства (название секции в файле конфигурации) */
char DEVICE_NAME[] = "Plugin_template";

struct plugin_device *plugin;

/* Адрес порта (и значение по умолчанию до его изменения в настройках) */
int device_port = 0x00;

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
        return 1;
}
//---------------------------------------------------------------------------


/*    Функции интерфейса с основной программой    */
/*    должны быть объявлены как С (а не С++)      */

/* Сброс устройства (вызывается, когда сбрасывается микропроцессорная система */
extern "C" void reset()
{
}

/* Показывает окно свойств устройства */
extern "C" void config()
{
  TConfigForm *ConfigForm = new TConfigForm(NULL);
  ConfigForm->ShowModal();
  delete ConfigForm;
}

/* Чтение байта из порта */
extern "C" BYTE read(WORD port)
{
  return 0xff;
}

/* Запись байта в порт */
extern "C" void write(WORD port, BYTE data)
{
}


/* int __export DeviceInit(struct plugin_device *dev)
   Экспортируемая из dll функция, которая должна инициализировать
   устройство и заполнить указатели на функции и вернуть 1,
   или если инициализация устройства прошла неудачно, вернуть 0
 */
extern "C" int __export DeviceInit(struct plugin_device *dev)
{
  DWORD dwVersion;

  plugin = dev;

  /* Открываем файл настроек программы */
  TIniFile *ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));

  /* Считываем адрес порта из файла настроек, а если оно не задано, берем по умолчанию */
  device_port = ini->ReadInteger( DEVICE_NAME, "Port", device_port);

  /* Считываем остальные настройки */




  /* закрываем файл настроек */
  delete ini;

  /* Заполняем необходимые поля структуры plugin */
  dev->descr = description;   // описание
  dev->mask = 0xff; // Маска порта устройства (подробности в readme.txt)
  dev->port = device_port; // адрес порта устройства
  dev->read = read; // функция чтения из порта
  dev->write = write; // функция записи в порт
  dev->reset = reset; // функция сброса устройства
  dev->config = config;  /* функция диалога настроек или NULL, если устройство его не имеет */

  /* Здесь делаем попытку инициализировать устройство.
     В случае неудачи должны вывести сообщение об ошибке
     и вернуть 0
   */




  /* возвращаем флаг удачной инициализации */
  return 1;
}

