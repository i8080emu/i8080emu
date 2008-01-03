/*   Эмулятор-отладчик i8080

  Exchanger plugin

Этот плагин демонстрирует различные способы передачи данных
между внешним устройством и центральным процессором.
В частности, эмулируется синхронная передача, асинхронная
с квитированием и асинхронная, управляемая через прерывания.

*/
#include <vcl.h>
#include <windows.h>
#include <inifiles.hpp>
#pragma hdrstop
#include "../../kernel/plugin.h"
#include "CfgForm.h"

/* Описание устройства */
char description[] = "Data exchanger (exchanger.dll)";

/* Название внешнего модуля устройства (название секции в файле конфигурации) */
char DEVICE_NAME[] = "Plugin_exchanger";

/* Указатель на структуру plugin_device текущего устройства */
struct plugin_device *plugin;

/* Переменные конфигурации */

// Адрес порта (и значение по умолчанию до его изменения в настройках)
int device_port = 0x20;

// Режим передачи данных
transfer_mode_t transfer_mode;

// Строка данных для передачи
String data_string;

// С какой частотой передаются данные при синхронной передаче, Гц
WORD sync_freq = 3000;

// Код команды выставляемый на шину данных при генерации прерывания
Byte interrupt_op_code;



/* Внутренние переменные */

// флаг того, что передача запущена
int is_transfer;

// через сколько тактов выдавать очередной байт данных
DWORD sync_tacts;

// значение счетчика тактов системы с момента последнего обращения
DWORD last_tstates;

// асинхронный обмен с прерываниями:
int asyn_int_byte_num;          // номер текущего байта в строке
Byte asyn_data;                 // текущий байт данных, который будет прочитан при обращении к порту
int asyn_handled;               // флаг того, что текущий байт был прочитан и требуется передать следующий


#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
        return 1;
}
//---------------------------------------------------------------------------


/* Вспомогательные функции */


/* Возвращает байт из строки данных по указаному смещению.
   Если смещение выходит за пределы строки, сбрасывает флаг
   передачи и возвращает код ошибки.
 */
Byte GetDataByte(int offset)
{
  if (offset > data_string.Length()) {
    is_transfer = 0;
    return ERROR_CODE;
  }

  return data_string[offset];
}

/* Функция обратного вызова, вырабатывающая сигнал прерывания
   по прошествию определенного количества тактов.
   Используется при обмене с прерываниями.
 */
static void callback()
{
  DWORD tacts_diff;

  // начата ли передача?
  if (!is_transfer)
    return;

  if (transfer_mode == TM_ASYNC_INT) {
    tacts_diff = *(plugin->tstates) - last_tstates;

    // не пришло ли время выставлять следующий байт?
    if (tacts_diff > sync_tacts )
      asyn_data = GetDataByte(tacts_diff / sync_tacts);
      asyn_handled = false;
  }
}


/*    Функции интерфейса с основной программой    */
/*    должны быть объявлены как С (а не С++)      */

/* Сброс устройства (вызывается, когда сбрасывается микропроцессорная система */
extern "C" void reset()
{
  is_transfer = 0;
  sync_tacts = *(plugin->mpsys_hz) / sync_freq;

  // процессор не сможет обрабатывать данные быстрее чем
  // через каждые 10 тактов (время команды in)
  if (sync_tacts < 10 ) {
    ShowMessage("Выбрана недопустимо высокая скорость обмена.");
  }
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
  DWORD tacts_diff;

  // порт состояния и данных отличается младшим битом
  if ((port & 0x01) == 0) {

    /* Чтение из порта состояния устройства */

    // сколько тактов прошло с последнего вызова
    tacts_diff = *(plugin->tstates) - last_tstates;

    /* прочитаный байт зависит от режима */
    switch(transfer_mode) {
      case TM_SYNC:
        // в синхронном режиме просто смотрим по прошедшему числу тактов
        // какое смещение в строке данных достигнуто.
        return GetDataByte(tacts_diff / sync_tacts);
      case TM_ASYNC_KVIT:
        break;
      case TM_ASYNC_INT:
        break;
      default:
        ShowMessage("Неверное значение переменной ражима передачи!");
    }
  }
  else {

    /* Чтение из порта данных */

    // если сейчас не идет передача, возвращаем код ошибки
    if (!is_transfer)
      return ERROR_CODE;

    /* прочитаный байт зависит от режима */
    switch(transfer_mode) {
      case TM_SYNC:
        break;
      case TM_ASYNC_KVIT:
        break;
      case TM_ASYNC_INT:
        break;
      default:
        ShowMessage("Неверное значение переменной ражима передачи!");
    }
  }
  return ERROR_CODE;
}


/* Запись байта в порт */
extern "C" void write(WORD port, BYTE data)
{
  // порт состояния и данных отличается младшим битом
  if ((port & 0x01) == 0) {

    /* Запись в порт состояния устройства */

    // если подан сигнал начала передачи
    if (data == START_TRANSFER_CODE) {
      last_tstates = *(plugin->tstates);
      is_transfer = 1;
    }

    /* запись в порт состояния числа 0x0f означает сигнал к началу передачи */
    switch(transfer_mode) {
      case TM_SYNC:
        break;
      case TM_ASYNC_KVIT:
        break;
      case TM_ASYNC_INT:
        asyn_int_byte_num = 0;
        break;
      default:
        ShowMessage("Неверное значение переменной ражима передачи!");
    }
  }
  else {
    /* Запись в порт данных игнорируется */
    return;
  }
}


/* int __export DeviceInit(struct plugin_device *dev)
   Экспортируемая из dll функция, которая должна инициализировать
   устройство и заполнить указатели на функции и вернуть 1,
   или если инициализация устройства прошла неудачно, вернуть 0
 */
extern "C" int __export DeviceInit(struct plugin_device *dev)
{
  DWORD dwVersion;

  // запоминаем указатель на этот плагин (он нужен для обращения к эмулятору)
  plugin = dev;

  /* Открываем файл настроек программы */
  TIniFile *ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));

  /* Считываем адрес порта из файла настроек, а если оно не задано, берем по умолчанию */
//  device_port = ini->ReadInteger( DEVICE_NAME, "Port", device_port);

  /* Считываем остальные настройки */
  data_string = ini->ReadString( DEVICE_NAME, "data_string", "This string is exаmple capacity for work of data transfer mechanism.");
  transfer_mode = (transfer_mode_t)ini->ReadInteger( DEVICE_NAME, "transfer_mode", TM_SYNC);
  interrupt_op_code = ini->ReadInteger( DEVICE_NAME, "interrupt_op_code", 0xff);

  /* закрываем файл настроек */
  delete ini;

  /* Заполняем необходимые поля структуры plugin */
  dev->descr = description;
  dev->mask = 0xfe;
  dev->port = device_port;
  dev->read = read;
  dev->write = write;
  dev->reset = reset;
  dev->config = config;

  /* Регистрируем функцию обратного вызова */
  if (!dev->register_callback(callback))
    ShowMessage("Не удалось зарегистрировать функцию обратного вызова-  \
    плагин не сможет генерировать прерывания и ассинхронный режим с прерываниями \
    будет работать неправильно! (превышено максимольно допустимое число плагинов, \
    регистрирующих функции обратного вызова. Попробуйте отключить некоторые плагины, \
    которые их используют.)");

  /* возвращаем флаг удачной инициализации */
  return 1;
}

