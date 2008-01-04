#ifndef _I8080_MPSYS_H
#define _I8080_MPSYS_H

/* ����� ��������� �������� �������� */
typedef enum {
DELAY_NO = 0,  /* ��� �������� */
DELAY_AUTO,    /* ��������������� */
DELAY_USER     /* ������ ������������� */
} delaymode_t;

extern BYTE MEM[0x10000];

/* �������� ������� ������� */
extern DWORD mpsys_hz;

/* ���������� ������ �������� */
extern long mpsys_delay;

/* ����� ������ �������� �������� */
extern delaymode_t mpsys_delaymode;

extern DWORD tstates;

/* ����������, ������������ � ������� */
extern struct plugin_device *devices;

/* ��� �������� ���������� ��������� */
extern char *exe_dir_name;

BYTE readbyte(WORD addr);
void writebyte(WORD addr, BYTE data);

BYTE readport(WORD port);
void writeport(WORD port, BYTE b);

int mpsys_init(void);
int mpsys_reset(void);
int mpsys_interrupt(void);

void mpsys_run(HWND hWnd);
void mpsys_stop(void);
//int mpsys_isrun(void);

int mpsys_ShowConfigDialog();


#endif

