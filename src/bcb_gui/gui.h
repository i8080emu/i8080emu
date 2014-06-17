#ifndef _I8080_GUI_H
#define _I8080_GUI_H

#define CM_ISBREAK (WM_APP + 400)

/* Максимальное число шестнадцатиричных чисел для поиска и ввода */
#define MAX_HEX_SEQUENCE 10

/* The various severities of error level, increasing downwards */
typedef enum ui_error_level {
  UI_ERROR_INFO,		/* Informational message */
  UI_ERROR_ERROR,		/* An actual error */
} ui_error_level;

/* Список элементов у которых можно изменить цвет в опциях
   Он должен совпадать со списком в ElementListBox
   в диалоге выбора цветов.
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

/* число элементов в списке цветов */
#define GUI_NUMCOLORS 7
#define GUI_NUMSHEMES 4

/* Структура, определяющая цвет и фон элемента */
typedef struct {
  COLORREF ink;
  COLORREF paper;
} GUI_COLOR;

/* Текущие настройки цветовых схем */
extern GUI_COLOR gui_color[GUI_NUMSHEMES][GUI_NUMCOLORS];

/* Номер выбраной цветовой схемы */
extern int gui_sheme;

/* Шрифт и размер для главного окна */
extern char *gui_font_family;
extern int gui_font_size;


#ifdef __cplusplus
 extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

int ui_error( ui_error_level severity, const char *format, ... );

void gui_init(HWND main);

void gui_draw(HDC hdc, RECT *crect);  // рисование всей области окна
void gui_scroll(int diff);      // изменение полосы прокрутки
void gui_click(int y);          // щелчок по области дизасемблера

void gui_up(void);              // курсор на одну позицию вверх
void gui_down(void);            // курсор на одну позицию вниз
void gui_pageup(void);          // курсор на страницу вверх
void gui_pagedown(void);        // курсор на страницу вниз

/* Команды меню */
void gui_file_clearmem(void);
void gui_file_loadimage(char *filename);
void gui_file_saveimage(char *filename);
void gui_file_loadblock(char *filename, int addr);
void gui_file_loadbin(char *filename, int addr, int len);
void gui_file_saveblock(char *filename, int addr, int len);

void gui_view_from(WORD addr);  // просмотр с заданого адреса
void gui_view_find(BYTE *data, int len);  /* поиск последовательности */
void gui_view_put(BYTE *data, int len);  /* ввод последовательности */

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
