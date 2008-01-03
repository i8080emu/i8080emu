#include <vcl.h>
#include <windows.h>
#pragma hdrstop
#include "CfgForm.h"

#include "core/plugin.h"

/* �������� ���������� */
char description[] = "������ (������ ���������� ��������) (speaker.dll)";

int speaker_port;
int can_direct_io;

#pragma argsused
int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
        return 1;
}
//---------------------------------------------------------------------------


/* �������� ��� ��������� ��������� ������� */
void SetSpeakerState(int state)
{
  if (can_direct_io) {

/*  � - ���������� ������������� ����:
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



//  ������� ���������� � �������� ����������
//   ������ ���� ��������� ��� � (� �� �++)

/* ����� ���������� (����������, ����� ������������ ����������������� ������� */
extern "C" void reset()
{
  /* ��������� ������ */
  SetSpeakerState(0);
}

/* ���������� ���� ������� ���������� */
extern "C" void config()
{
  TConfigForm *ConfigForm = new TConfigForm(NULL);
  ConfigForm->ShowModal();
  delete ConfigForm;
}

/* ������ ����� �� ����� */
extern "C" BYTE read(WORD por)
{
  return 0xff;
}

/* ������ ����� � ���� */
extern "C" void write(WORD port, BYTE data)
{
  int state = data & SPEAKER_MASK;
  SetSpeakerState(state);
}


/* �������������� �� dll �������, ������� ������ ����������������
   ���������� � ��������� ��������� �� ������� � ������� 1,
   ��� ���� ������������� ���������� ������ ��������, ������� 0
 */
extern "C" int __export DeviceInit(struct plugin_device *dev)
{
//  OSVERSIONINFO osver;
  DWORD dwVersion;

  /* �������� ���������� */
  dev->descr = description;

  /* �� ������ �������� ���� ����, ������� ����� 255 (����������� ��� ����) */
  dev->mask = 0xff;
  dev->port = speaker_port;

  /* ��������� ��������� �� ������� ���������� */
  dev->read = read;
  dev->write = write;
  dev->reset = reset;
  dev->config = config;

  /* ������ ������ windows */
  dwVersion = GetVersion();
  if (dwVersion == 0) {
    MessageBox(NULL, "��������� ������ ��� ������ GetVersion � ������ speaker.dll","speaker_init", MB_OK);
    return FALSE;
  }

  /* ���� ����� ���� � ��������� NT
    ��������� ������ ������ � ������
   */
  if (dwVersion < 0x80000000)
    can_direct_io = false;

  /* ���������� ���� ������� ������������� */
  return 1;
}

