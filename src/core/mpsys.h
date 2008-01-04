#ifndef _I8080_MPSYS_H
#define _I8080_MPSYS_H

/* режим установки задержки эмуляции */
typedef enum {
DELAY_NO = 0,  /* нет задержки */
DELAY_AUTO,    /* автоопределение */
DELAY_USER     /* задано пользователем */
} delaymode_t;

extern BYTE MEM[0x10000];

/* тактовая частота системы */
extern DWORD mpsys_hz;

/* Количество циклов задержки */
extern long mpsys_delay;

/* Режим выбора задержки эмуляции */
extern delaymode_t mpsys_delaymode;

extern DWORD tstates;

/* Устройства, подключенные к системе */
extern struct plugin_device *devices;

/* Имя корневой директории эмулятора */
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

