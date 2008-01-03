#include <stdio.h>
#include <conio.h>
#include "windows.h"

void main(int argc, char *argv[])
{
 char COM[16], FileName[128];
 DWORD BaudRate;
 BYTE Buff[10];
 DWORD nread,nwrite;
  // Получаем параметры
  strcpy(COM, "COM1");
  BaudRate = 1200;
  strcpy(FileName, "file");
  // Открываем порт
  HANDLE port = CreateFile(COM, GENERIC_READ | GENERIC_WRITE,
      FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
  if (port==INVALID_HANDLE_VALUE) {
    printf("'%s' Open Error %d\n", COM, GetLastError()); 
    exit(-1);
  }
  // Получаем текущие параметры COM-порта
  DCB dcb;
  if (GetCommState(port, &dcb)) {
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
     SetCommState(port, &dcb);
  }
  // устанавливаем таймауты
  COMMTIMEOUTS touts;
  if (GetCommTimeouts(port, &touts)) {
     touts.ReadIntervalTimeout = MAXDWORD;  // интервал не используется
     touts.ReadTotalTimeoutMultiplier = 0;
     touts.ReadTotalTimeoutConstant = 0;    // таймаут чтения не используется
                                            // (чтение без ожидания)
     SetCommTimeouts(port, &touts);
  }
  // Выставляем DTR и RTS
  EscapeCommFunction(port, SETDTR);
  EscapeCommFunction(port, SETRTS);

  for (;;) {
     nread=0; ReadFile(port, Buff, 1, &nread, NULL);
     if (nread==0) { // нет байтов
       Sleep(20); // задежка
     } else { // есть
       // пихаем на экран
       printf(" %02X", Buff[0]);
       // пихаем в файл 
       // открываем каждый раз заново - это не глюк, это фича :-)
       HANDLE file = CreateFile(FileName, GENERIC_WRITE,
           FILE_SHARE_READ, NULL, OPEN_ALWAYS, 0, NULL);
       SetFilePointer(file, 0, NULL, FILE_END);
       WriteFile(file, Buff, 1, &nwrite, NULL);
       CloseHandle(file);
     }
     // проверяем на выход
     if (kbhit() && getch()==27) break;
  }
  CloseHandle(port);
}
