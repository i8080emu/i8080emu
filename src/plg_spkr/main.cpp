#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include "CfgForm.h"

#include "core/plugin.h"

/* Описание устройства */
char description[] = "Спикер (аналог системного динамика) (speaker.dll)";

int speaker_port;
int can_direct_io;

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
        return 1;
}
//---------------------------------------------------------------------------


/* Включает или выключает системный динамик */
void SetSpeakerState(int state)
{
  if (can_direct_io) {

/*  С - эквивалент ассемблерного кода:
    tmp = inportb(0x61);
    tmp = (spk_state)? (tmp & 0xfe) | 2 : tmp & 0xfd;
    outportb(0x61, tmp);
*/
    __asm {
      push  eax
      push  ebx
      in    al,61h
      mov bl,al
      mov eax,state
      and eax,eax
      jz  state0
      mov al,bl
      and al,0feH
      or  al,002H
      jmp outport
    state0:
      mov al,bl
      and al,0fcH
    outport:
      out 61h,al
      pop ebx
      pop eax
    }
  } else {


  }
}



//  Функции интерфейса с основной программой
//   должны быть объявлены как С (а не С++)

/* Сброс устройства (вызывается, когда сбрасывается микропроцессорная система */
extern "C" void reset()
{
  /* Выключаем спикер */
  SetSpeakerState(0);
}

/* Показывает окно свойств устройства */
extern "C" void config()
{
  TConfigForm *ConfigForm = new TConfigForm(NULL);
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
  int state = data & SPEAKER_MASK;
  SetSpeakerState(state);
}


/* Экспортируемая из dll функция, которая должна инициализировать
   устройство и заполнить указатели на функции и вернуть 1,
   или если инициализация устройства прошла неудачно, вернуть 0
 */
extern "C" int __export DeviceInit(struct plugin_device *dev)
{
//  OSVERSIONINFO osver;
  DWORD dwVersion;

  /* Описание устройства */
  dev->descr = description;

  /* за спикер отвечает один порт, поэтому маска 255 (учитываются все биты) */
  dev->mask = 0xff;
  dev->port = speaker_port;

  /* заполняем указатели на функции интерфейса */
  dev->read = read;
  dev->write = write;
  dev->reset = reset;
  dev->config = config;

  /* Узнаем версию windows */
  dwVersion = GetVersion();
  if (dwVersion == 0) {
    MessageBox(NULL, "Произошла ошибка при вызове GetVersion в модуле speaker.dll","speaker_init", MB_OK);
    return FALSE;
  }

  /* Если имеем дело с системами NT
    выключаем прямой доступ к портам
   */
  if (dwVersion < 0x80000000)
    can_direct_io = false;

  /* возвращаем флаг удачной инициализации */
  return 1;
}

