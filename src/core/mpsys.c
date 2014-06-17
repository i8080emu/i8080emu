/* Реализация интерфейса микропроцессорной системы
   (исполнение команд, память, ввод/вывод через порты,
   связь с внешними устройствами).
*/
#include <stdio.h>

#include "mpsys.h"
#include "event.h"
#include "i8080.h"
#include "myglib.h"
#include "breakpnt.h"
#include "plugin.h"

/* память виртуальной системы */
BYTE MEM[0x10000];

/* счетчик тактов виртуальной системы */
DWORD tstates;

/* тактовая частота виртуальной системы */
DWORD mpsys_hz=2000000;

/* флаг остановки по HLT (задается в main.cpp) */
extern BOOL bHltStop;

/* Через какое количество милисекунд сверяться с реальным временем
 (если установлен автоматический выбор задержки */
#define SYNC_TIME 50

/* Через сколько тактов осуществлять задержку */
#define SYNC_EVERY 500

/* Максимальное число функций обратного вызова на операции чтения и
  записи в память */
#define MAX_CALLBACKS 16

/* Количество циклов задержки */
long mpsys_delay;

/* Режим выбора задержки эмуляции */
delaymode_t mpsys_delaymode;

/* Флаг: выполняется виртуальная система или нет */
static int going;

/* Сколько милисекунд тратиться на одну итерацию пустого цикла
   (miliseconds per loop)
 */
float mspl;

/* Сколько милисекунд проходит за один такт виртуальной системы
   (miliseconds per tack)
 */
float mspt;

/* прошлое значение тиков системного таймера */
static DWORD lastticks;
/* прошлое значение счетчика тактов */
static DWORD tlast;

/* Указатель на поток и идентификатор потока,
  в котором запускается выполнение */
HANDLE hThread;
DWORD dwIDThread;

/* Имя корневой директории эмулятора */
char *exe_dir_name;

/* Устройства, подключенные к системе */
struct plugin_device *devices = NULL;

/* Функции обратного вызова на чтение и запись памяти */
static mpsys_read_callback_t    read_callback [MAX_CALLBACKS];
static mpsys_write_callback_t   write_callback [MAX_CALLBACKS];

/* Число установленых функций обратного вызова */
static int num_read_callbacks = 0;
static int num_write_callbacks = 0;


/* Регистрация функции обратного вызова чтения памяти */
int i8080_register_read_callback(mpsys_read_callback_t cb)
{
  if (num_read_callbacks < MAX_CALLBACKS) {
    read_callback [num_read_callbacks] = cb;
    num_read_callbacks++;
    return 1;
  }
  else {
    MessageBox(NULL, "Невозможно зарегистрировать функцию обратного вызова чтения памяти- \
    превышено максимально допустимое количество зарегистрированых функций обратного вызова.",
    "Ошибка", MB_OK | MB_ICONEXCLAMATION);
    return 0;
  }
}

/* Регистрация функции обратного вызова записи в память */
int i8080_register_write_callback(mpsys_write_callback_t cb)
{
  if (num_write_callbacks < MAX_CALLBACKS) {
    write_callback [num_write_callbacks] = cb;
    num_write_callbacks++;
    return 1;
  }
  else {
    MessageBox(NULL, "Невозможно зарегистрировать функцию обратного вызова записи в память- \
    превышено максимально допустимое количество зарегистрированых функций обратного вызова.",
    "Ошибка", MB_OK | MB_ICONEXCLAMATION);
    return 0;
  }
}






void mpsys_load_device(HWND hwnd, HINSTANCE hinst, char *filename)
{
  int (__stdcall *init)(struct plugin_device *);
  struct plugin_device *newdev;
  HINSTANCE dllp;
  int res;

  /* Готовим структуру устройства для передачи плагину */
  newdev = (struct plugin_device *)malloc(sizeof(struct plugin_device));

  /* Заполняем поля структуры плагина */
  newdev->event_add = event_add;
  newdev->register_callback = i8080_register_callback;
  newdev->register_read_callback = i8080_register_read_callback;
  newdev->register_write_callback = i8080_register_write_callback;
  newdev->mem = &MEM[0];
  newdev->tstates = &tstates;
  newdev->mpsys_hz = &mpsys_hz;
  newdev->hWnd = hwnd;
  newdev->hIsnst = hinst;

  /* Пытаемся загрузить плагин */
  if ((dllp = LoadLibrary(filename)) == NULL)
    ui_error(0,"Не удалось загрузить плагин %s",filename);
  /* Если плагин загружен удачно, берем адрес инициализирующей функции */
  else if ((init = (int(__stdcall *)(struct plugin_device *)) GetProcAddress(dllp, "_DeviceInit")) == NULL)
    ui_error(0,"В плагине %s нет функции _DeviceInit",filename);
  /* Вызываем функцию инициализации */
  else if (!init(newdev))
    ui_error(0,"Функция инициализации плагина %s вернула ошибку",filename);
  /* Если инициализация прошла удачно, добавляем в список */
  else {
    newdev->next = devices;
    devices = newdev;
    return; /* возврат, минуя освобождение структуры */
  }

  /* если дошли до сюда, значит, инициализация устройства
    не прошла- освобождаем память из под структуры */
  free(newdev);
}

/* Определяет, сколько милисекунд тратиться на
   один проход пустого цикла
 */
void Calculate_mspl(void)
{
  DWORD oldticks, timediff;
  long n, loops;

  loops = 100;

  do {

    oldticks = GetTickCount();

    for(n = 0; n < loops; n++)
      ; /* пустой цикл */

    timediff = GetTickCount() - oldticks;

    mspl = timediff / loops;

    loops *= 10;

  } while(timediff < 200);
}


int mpsys_init(void)
{
  /* определения быстродействия системы */
  Calculate_mspl();
  return 0;
}

int mpsys_reset(void)
{
  struct plugin_device *dev;

  /* Сброс процессора */
  i8080_reset();

  /* Сброс внешних устройств */
  for(dev = devices; dev != NULL; dev = dev->next )
    dev->reset();

  return 0;
}


/* Если задержка отключена, то ничего не делает.
   Если установлена фиксированая задержка, то осуществляет ее.
   Если автоматически, то осуществляет ее и корректирует.
 */
static void SyncToRealtime(void)
{
  int n;
  float delaydiff, musttime;
  DWORD timediff;

  /* выполнение нужное число циклов задержки */
  for (n = 0; n < mpsys_delay; n++)
    ; /* ничего не делаем */

  if (mpsys_delaymode == DELAY_AUTO) {
    /* коррекция задержки */
    timediff = GetTickCount() - lastticks;
    musttime = (tstates - tlast) * mspt;
    /* сейчас по имеющейся информации о прощедшем времени
       и по тому, сколько должно было пройти, корректируем
       величину задержки
     */
    delaydiff = ((float)timediff - (float)musttime) * mspl;

    if (((float)mpsys_delay + delaydiff) < 0)
      mpsys_delay = 0;
    else
      mpsys_delay += delaydiff;
  }
}

void run_loop(void *hwnd)
{
  while( going ) {

    // выполняем очередную операцию
    i8080_do_opcode();

    // не наткнулись ли на точку останова?
    if (break_isbreak(i8080.pc.w)) {
      // уменьшаем число срабатываний точки
      break_catch(i8080.pc.w);
      // извещаем приложение об остановке процесса
      SendMessage(hwnd, CM_ISBREAK, NULL, NULL);
      // ждем завершения потока
      while( going ) Sleep(5);
      break;
    }

    // не встречена ли команда HLT
    if (i8080.halted) {
      // если включена остановка по HLT
      if (bHltStop) {
        // извещаем приложение об остановке процесса
        SendMessage(hwnd, CM_ISBREAK, NULL, NULL);
        // ждем завершения потока
        while( going ) Sleep(5);
        break;
      }
    }

    /* если событий нет, то выполнять инструкции */
    if (tstates < event_next_event)
      i8080_do_opcode();
    /* иначе обработать события */
    else
      event_do_events();

    /* Следим за тем, чтоб эмуляция не была слишком быстрой */
    SyncToRealtime();
  }
}

void mpsys_run(HWND hWnd)
{
  if(going != 0)
    return;

  /* инициализация переменных для автовыбора задержки */
  tlast = tstates;
  lastticks = GetTickCount();
  mspt = 1000 / mpsys_hz;

  /* создание отдельного потока для выполнения программы в реальном времени */
  hThread = CreateThread(NULL, 0,
    (LPTHREAD_START_ROUTINE)run_loop,
    (LPVOID)hWnd, 0, (LPVOID)&dwIDThread);

  if (hThread != NULL)
    going = 1;
  else
    ui_error(UI_ERROR_ERROR, "Ошибка при создании потока");
}

void mpsys_stop()
{
  if(going == 0)
    return;

  // сбрасываем флаг выполнения команд системы
  going = 0;

  // дожидаемся завершения процесса, ждем максимум 50 милисекунд
  WaitForSingleObject(hThread, 50);
}

int mpsys_isrun()
{
  return going;
}

BYTE readbyte(WORD addr)
{
  int n;

  // Вызываем установленые обработчики на чтение из памяти
  for (n = 0; n < num_read_callbacks; n++)
    read_callback[n](addr);

  return MEM[addr];
}

void writebyte(WORD addr, BYTE data)
{
  int n;

  // Вызываем установленые обработчики на запись в память
  for (n = 0; n < num_write_callbacks; n++)
    write_callback[n](addr, &data);

  MEM[addr] = data;
}

/* Чтение байта из порта */
BYTE readport(WORD port)
{
  struct plugin_device *dev;

  BYTE return_value = 0xff;

  /* Просмотр списка периферии на предмет подключения к этому порту */
  for(dev = devices; dev->next; dev = dev->next ) {
    if( ( port & dev->mask ) == dev->port ) {
      return_value &= dev->read(port);
    }
  }

  return return_value;
}

/* Вывод байта в порт */
void writeport(WORD port, BYTE b)
{
  struct plugin_device *dev;

  for(dev = devices; dev != NULL; dev = dev->next ) {
    if( ( port & dev->mask ) == dev->port ) {
      dev->write(port, b);
    }
  }
}

