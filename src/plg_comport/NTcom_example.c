#include <stdio.h>
#include <conio.h>
#include "windows.h"

void main(int argc, char *argv[])
{
 char COM[16], FileName[128];
 DWORD BaudRate;
 BYTE Buff[10];
 DWORD nread,nwrite;
  // ����砥� ��ࠬ����
  strcpy(COM, "COM1");
  BaudRate = 1200;
  strcpy(FileName, "file");
  // ���뢠�� ����
  HANDLE port = CreateFile(COM, GENERIC_READ | GENERIC_WRITE,
      FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
  if (port==INVALID_HANDLE_VALUE) {
    printf("'%s' Open Error %d\n", COM, GetLastError()); 
    exit(-1);
  }
  // ����砥� ⥪�騥 ��ࠬ���� COM-����
  DCB dcb;
  if (GetCommState(port, &dcb)) {
     // ��⠭�������� �ॡ㥬� ��ࠬ����
     // ᪮����
     dcb.BaudRate = BaudRate;
     // �ଠ� �����
     dcb.ByteSize = 7; dcb.Parity = 0; dcb.StopBits = 0;
     // ��祥
     dcb.fBinary =  1;
     dcb.fOutxCtsFlow = 0;      // CTS output flow control
     dcb.fOutxDsrFlow = 0;      // DSR output flow control
     // � ��� ����� ����� ���ॡ������� �������� �� �㦭��
     dcb.fDtrControl  = 0;      // DTR flow control type
     //
     dcb.fDsrSensitivity = 0;   // DSR sensitivity
     dcb.fTXContinueOnXoff = 1; // XOFF continues Tx
     dcb.fOutX        = 0;      // XON/XOFF out flow control
     dcb.fInX         = 0;      // XON/XOFF in flow control
     dcb.fRtsControl  = 0;      // RTS flow control
     dcb.fAbortOnError= 0;      // abort reads/writes on error
     SetCommState(port, &dcb);
  }
  // ��⠭�������� ⠩�����
  COMMTIMEOUTS touts;
  if (GetCommTimeouts(port, &touts)) {
     touts.ReadIntervalTimeout = MAXDWORD;  // ���ࢠ� �� �ᯮ������
     touts.ReadTotalTimeoutMultiplier = 0;
     touts.ReadTotalTimeoutConstant = 0;    // ⠩���� �⥭�� �� �ᯮ������
                                            // (�⥭�� ��� ��������)
     SetCommTimeouts(port, &touts);
  }
  // ���⠢�塞 DTR � RTS
  EscapeCommFunction(port, SETDTR);
  EscapeCommFunction(port, SETRTS);

  for (;;) {
     nread=0; ReadFile(port, Buff, 1, &nread, NULL);
     if (nread==0) { // ��� ���⮢
       Sleep(20); // �������
     } else { // ����
       // ��堥� �� �࠭
       printf(" %02X", Buff[0]);
       // ��堥� � 䠩� 
       // ���뢠�� ����� ࠧ ������ - �� �� ���, �� �� :-)
       HANDLE file = CreateFile(FileName, GENERIC_WRITE,
           FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, NULL);
       SetFilePointer(file, 0, NULL, FILE_END);
       WriteFile(file, Buff, 1, &nwrite, NULL);
       CloseHandle(file);
     }
     // �஢��塞 �� ��室
     if (kbhit() && getch()==27) break;
  }
  CloseHandle(port);
}
