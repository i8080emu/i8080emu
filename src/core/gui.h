#ifndef _I8080_GUI_H
#define _I8080_GUI_H

#define CM_ISBREAK (WM_APP + 400)

/* ������������ ����� ����������������� ����� ��� ������ � ����� */
#define MAX_HEX_SEQUENCE 10

/* The various severities of error level, increasing downwards */
typedef enum ui_error_level {
  UI_ERROR_INFO,		/* Informational message */
  UI_ERROR_ERROR,		/* An actual error */
} ui_error_level;

/* ������ ��������� � ������� ����� �������� ���� � ������
   �� ������ ��������� �� ������� � ElementListBox
   � ������� ������ ������.
 */
enum gui_colors{
GUI_WHTSPC,
GUI_ADDR,
GUI_CODE,
GUI_MNEM,
GUI_OPS,
GUI_CURPNT,
GUI_BRKPNT,
};

/* ����� ��������� � ������ ������ */
#define GUI_NUMCOLORS 7
#define GUI_NUMSHEMES 4

/* ���������, ������������ ���� � ��� �������� */
typedef struct {
  COLORREF ink;
  COLORREF paper;
} GUI_COLOR;

/* ������� ��������� �������� ���� */
extern GUI_COLOR gui_color[GUI_NUMSHEMES][GUI_NUMCOLORS];

/* ����� �������� �������� ����� */
extern int gui_sheme;

/* ����� � ������ ��� �������� ���� */
extern char *gui_font_family;
extern int gui_font_size;


#ifdef __cplusplus
 extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

int ui_error( ui_error_level severity, const char *format, ... );

void gui_init(HWND main);

void gui_draw(HDC hdc, RECT *crect);  // ��������� ���� ������� ����
void gui_scroll(int diff);      // ��������� ������ ���������
void gui_click(int y);          // ������ �� ������� ������������

void gui_up(void);              // ������ �� ���� ������� �����
void gui_down(void);            // ������ �� ���� ������� ����
void gui_pageup(void);          // ������ �� �������� �����
void gui_pagedown(void);        // ������ �� �������� ����

/* ������� ���� */
void gui_file_clearmem(void);
void gui_file_loadimage(char *filename);
void gui_file_saveimage(char *filename);
void gui_file_loadblock(char *filename, int addr);
void gui_file_loadbin(char *filename, int addr, int len);
void gui_file_saveblock(char *filename, int addr, int len);

void gui_view_from(WORD addr);  // �������� � �������� ������
void gui_view_find(BYTE *data, int len);  /* ����� ������������������ */
void gui_view_put(BYTE *data, int len);  /* ���� ������������������ */

void gui_run_reset(void);
void gui_run_stepinto(void);
void gui_run_stepover(void);
void gui_run_run(void);
void gui_run_stop(void);
void gui_run_goto(WORD addr);
void gui_run_togglebrkpnt(void);


#ifdef __cplusplus
 }
#endif  /* __cplusplus */
//---------------------------------------------------------------------------
#endif
