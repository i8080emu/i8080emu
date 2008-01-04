/* ���������� ���������� ����������������� �������
   (���������� ������, ������, ����/����� ����� �����,
   ����� � �������� ������������).
*/
#include <windows.h>

#include "mpsys.h"
#include "event.h"
#include "i8080.h"
#include "myglib.h"
#include "gui.h"
#include "breakpnt.h"
#include "plugin.h"

/* ������ ����������� ������� */
BYTE MEM[0x10000];

/* ������� ������ ����������� ������� */
DWORD tstates;

/* �������� ������� ����������� ������� */
DWORD mpsys_hz=2000000;

/* ���� ��������� �� HLT (�������� � main.cpp) */
extern BOOL bHltStop;

/* ����� ����� ���������� ���������� ��������� � �������� ��������
 (���� ���������� �������������� ����� �������� */
#define SYNC_TIME 50

/* ����� ������� ������ ������������ �������� */
#define SYNC_EVERY 500

/* ������������ ����� ������� ��������� ������ �� �������� ������ �
  ������ � ������ */
#define MAX_CALLBACKS 16

/* ���������� ������ �������� */
long mpsys_delay;

/* ����� ������ �������� �������� */
delaymode_t mpsys_delaymode;

/* ����: ����������� ����������� ������� ��� ��� */
static int going;

/* ������� ���������� ��������� �� ���� �������� ������� �����
   (miliseconds per loop)
 */
float mspl;

/* ������� ���������� �������� �� ���� ���� ����������� �������
   (miliseconds per tack)
 */
float mspt;

/* ������� �������� ����� ���������� ������� */
static DWORD lastticks;
/* ������� �������� �������� ������ */
static DWORD tlast;

/* ��������� �� ����� � ������������� ������,
  � ������� ����������� ���������� */
HANDLE hThread;
DWORD dwIDThread;

/* ��� �������� ���������� ��������� */
char *exe_dir_name;

/* ����������, ������������ � ������� */
struct plugin_device *devices = NULL;

/* ������� ��������� ������ �� ������ � ������ ������ */
static mpsys_read_callback_t    read_callback [MAX_CALLBACKS];
static mpsys_write_callback_t   write_callback [MAX_CALLBACKS];

/* ����� ������������ ������� ��������� ������ */
static int num_read_callbacks = 0;
static int num_write_callbacks = 0;


/* ����������� ������� ��������� ������ ������ ������ */
int i8080_register_read_callback(mpsys_read_callback_t cb)
{
  if (num_read_callbacks < MAX_CALLBACKS) {
    read_callback [num_read_callbacks] = cb;
    num_read_callbacks++;
    return 1;
  }
  else {
    MessageBox(NULL, "���������� ���������������� ������� ��������� ������ ������ ������- \
    ��������� ����������� ���������� ���������� ����������������� ������� ��������� ������.",
    "������", MB_OK | MB_ICONEXCLAMATION);
    return 0;
  }
}

/* ����������� ������� ��������� ������ ������ � ������ */
int i8080_register_write_callback(mpsys_write_callback_t cb)
{
  if (num_write_callbacks < MAX_CALLBACKS) {
    write_callback [num_write_callbacks] = cb;
    num_write_callbacks++;
    return 1;
  }
  else {
    MessageBox(NULL, "���������� ���������������� ������� ��������� ������ ������ � ������- \
    ��������� ����������� ���������� ���������� ����������������� ������� ��������� ������.",
    "������", MB_OK | MB_ICONEXCLAMATION);
    return 0;
  }
}






void mpsys_load_device(HWND hwnd, HINSTANCE hinst, char *filename)
{
  int (__stdcall *init)(struct plugin_device *);
  struct plugin_device *newdev;
  HINSTANCE dllp;
  int res;

  /* ������� ��������� ���������� ��� �������� ������� */
  newdev = (struct plugin_device *)malloc(sizeof(struct plugin_device));

  /* ��������� ���� ��������� ������� */
  newdev->event_add = event_add;
  newdev->register_callback = i8080_register_callback;
  newdev->register_read_callback = i8080_register_read_callback;
  newdev->register_write_callback = i8080_register_write_callback;
  newdev->mem = &MEM[0];
  newdev->tstates = &tstates;
  newdev->mpsys_hz = &mpsys_hz;
  newdev->hWnd = hwnd;
  newdev->hIsnst = hinst;

  /* �������� ��������� ������ */
  if ((dllp = LoadLibrary(filename)) == NULL)
    ui_error(0,"�� ������� ��������� ������ %s",filename);
  /* ���� ������ �������� ������, ����� ����� ���������������� ������� */
  else if ((init = (int(__stdcall *)(struct plugin_device *)) GetProcAddress(dllp, "_DeviceInit")) == NULL)
    ui_error(0,"� ������� %s ��� ������� _DeviceInit",filename);
  /* �������� ������� ������������� */
  else if (!init(newdev))
    ui_error(0,"������� ������������� ������� %s ������� ������",filename);
  /* ���� ������������� ������ ������, ��������� � ������ */
  else {
    newdev->next = devices;
    devices = newdev;
    return; /* �������, ����� ������������ ��������� */
  }

  /* ���� ����� �� ����, ������, ������������� ����������
    �� ������- ����������� ������ �� ��� ��������� */
  free(newdev);
}

/* ����������, ������� ���������� ��������� ��
   ���� ������ ������� �����
 */
void Calculate_mspl(void)
{
  DWORD oldticks, timediff;
  long n, loops;

  loops = 100;

  do {

    oldticks = GetTickCount();

    for(n = 0; n < loops; n++)
      ; /* ������ ���� */

    timediff = GetTickCount() - oldticks;

    mspl = timediff / loops;

    loops *= 10;

  } while(timediff < 200);
}


int mpsys_init(void)
{
  /* ����������� �������������� ������� */
  Calculate_mspl();
  return 0;
}

int mpsys_reset(void)
{
  struct plugin_device *dev;

  /* ����� ���������� */
  i8080_reset();

  /* ����� ������� ��������� */
  for(dev = devices; dev != NULL; dev = dev->next )
    dev->reset();

  return 0;
}


/* ���� �������� ���������, �� ������ �� ������.
   ���� ����������� ������������ ��������, �� ������������ ��.
   ���� �������������, �� ������������ �� � ������������.
 */
static void SyncToRealtime(void)
{
  int n;
  float delaydiff, musttime;
  DWORD timediff;

  /* ���������� ������ ����� ������ �������� */
  for (n = 0; n < mpsys_delay; n++)
    ; /* ������ �� ������ */

  if (mpsys_delaymode == DELAY_AUTO) {
    /* ��������� �������� */
    timediff = GetTickCount() - lastticks;
    musttime = (tstates - tlast) * mspt;
    /* ������ �� ��������� ���������� � ��������� �������
       � �� ����, ������� ������ ���� ������, ������������
       �������� ��������
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

    // ��������� ��������� ��������
    i8080_do_opcode();

    // �� ���������� �� �� ����� ��������?
    if (break_isbreak(i8080.pc.w)) {
      // ��������� ����� ������������ �����
      break_catch(i8080.pc.w);
      // �������� ���������� �� ��������� ��������
      SendMessage(hwnd, CM_ISBREAK, NULL, NULL);
      // ���� ���������� ������
      while( going ) Sleep(5);
      break;
    }

    // �� ��������� �� ������� HLT
    if (i8080.halted) {
      // ���� �������� ��������� �� HLT
      if (bHltStop) {
        // �������� ���������� �� ��������� ��������
        SendMessage(hwnd, CM_ISBREAK, NULL, NULL);
        // ���� ���������� ������
        while( going ) Sleep(5);
        break;
      }
    }

    /* ���� ������� ���, �� ��������� ���������� */
    if (tstates < event_next_event)
      i8080_do_opcode();
    /* ����� ���������� ������� */
    else
      event_do_events();

    /* ������ �� ���, ���� �������� �� ���� ������� ������� */
    SyncToRealtime();
  }
}

void mpsys_run(HWND hWnd)
{
  if(going != 0)
    return;

  /* ������������� ���������� ��� ���������� �������� */
  tlast = tstates;
  lastticks = GetTickCount();
  mspt = 1000 / mpsys_hz;

  /* �������� ���������� ������ ��� ���������� ��������� � �������� ������� */
  hThread = CreateThread(NULL, 0,
    (LPTHREAD_START_ROUTINE)run_loop,
    (LPVOID)hWnd, 0, (LPVOID)&dwIDThread);

  if (hThread != NULL)
    going = 1;
  else
    ui_error(UI_ERROR_ERROR, "������ ��� �������� ������");
}

void mpsys_stop()
{
  if(going == 0)
    return;

  // ���������� ���� ���������� ������ �������
  going = 0;

  // ���������� ���������� ��������, ���� �������� 50 ����������
  WaitForSingleObject(hThread, 50);
}

int mpsys_isrun()
{
  return going;
}

BYTE readbyte(WORD addr)
{
  int n;

  // �������� ������������ ����������� �� ������ �� ������
  for (n = 0; n < num_read_callbacks; n++)
    read_callback[n](addr);

  return MEM[addr];
}

void writebyte(WORD addr, BYTE data)
{
  int n;

  // �������� ������������ ����������� �� ������ � ������
  for (n = 0; n < num_write_callbacks; n++)
    write_callback[n](addr, &data);

  MEM[addr] = data;
}

/* ������ ����� �� ����� */
BYTE readport(WORD port)
{
  struct plugin_device *dev;

  BYTE return_value = 0xff;

  /* �������� ������ ��������� �� ������� ����������� � ����� ����� */
  for(dev = devices; dev->next; dev = dev->next ) {
    if( ( port & dev->mask ) == dev->port ) {
      return_value &= dev->read(port);
    }
  }

  return return_value;
}

/* ����� ����� � ���� */
void writeport(WORD port, BYTE b)
{
  struct plugin_device *dev;

  for(dev = devices; dev != NULL; dev = dev->next ) {
    if( ( port & dev->mask ) == dev->port ) {
      dev->write(port, b);
    }
  }
}

