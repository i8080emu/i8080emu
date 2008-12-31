/* Реализация функций пользовательского графического
*  интерфейса в основном окне.
*/
#include <windows.h>
#include <stdio.h>

#include "gui.h"
#include "disas.h"
#include "mpsys.h"
#include "i8080.h"
#include "breakpnt.h"
#include "event.h"

static HWND hmain;      // handle главного окна

// выбраный шрифт для окна дизасемблера
static HFONT    hfont;
static HFONT    hfontOld;

static HBRUSH   hbrush_ws;
static HBRUSH   hbrush_pc;
static HBRUSH   hbrush_bp;
static HBRUSH   hbrushOld;

/* Временая точка останова для StepOver */
static int StopOverBreakAddr;

/* Текущие настройки цветовых схем */
GUI_COLOR gui_color[GUI_NUMSHEMES][GUI_NUMCOLORS];

/* Номер выбраной цветовой схемы */
int gui_sheme;

/* Шрифт и размер для главного окна */
char   *gui_font_family;
int     gui_font_size;

static WORD top_addr = 0;    // адрес верхней команды в окне
static WORD cur_addr=0;      // адрес команды под курсором
static int num_lines;        // сколько строк в окне
static int cur_line;         // номер строки, в которой курсор
static int font_height;      // высота строки текста в пикселах
static int font_width;       // ширина символа

/* Максимальная длинна строки с сообщением об ошибке */
#define ERROR_MESSAGE_MAX_LENGTH 256

int ui_error( ui_error_level severity, const char *format, ... )
{
  va_list ap;
  char message[ ERROR_MESSAGE_MAX_LENGTH + 1 ];

  /* Создание сообщения по принятым аргументам */
  va_start( ap, format );
  vsnprintf( message, ERROR_MESSAGE_MAX_LENGTH, format, ap );
  va_end( ap );

  /* Показываем окошко с сообщением */
  MessageBox(hmain,message,"I8080 Emulator error",MB_OK);

  return 0;
}


void gui_init(HWND main)
{
  // запоминаем указатель главного окна
  hmain = main;

  /* инициализация всех модулей */
  i8080_init();
  break_init();
  event_init();
  mpsys_init();

  /* Инициализация переменных модуля */
  StopOverBreakAddr = -1;
}


/* Начало рисования: получение DC, создание графических объектов */
static void begin_draw(HDC hdc)
{
  TEXTMETRIC tm;

  /* создание кистей для заливки областей */
  hbrush_ws = CreateSolidBrush(gui_color[gui_sheme][GUI_WHTSPC].paper);
  hbrush_pc = CreateSolidBrush(gui_color[gui_sheme][GUI_CURPNT].paper);
  hbrush_bp = CreateSolidBrush(gui_color[gui_sheme][GUI_BRKPNT].paper);

  /* Создание шрифта */
  hfont = CreateFont(
    gui_font_size,	// logical height of font
    0,	// logical average character width
    0,	// angle of escapement
    0,	// base-line orientation angle
    FW_NORMAL,	// font weight
    FALSE,	// italic attribute flag
    FALSE,	// underline attribute flag
    FALSE,	// strikeout attribute flag
    DEFAULT_CHARSET,	// character set identifier
    OUT_DEFAULT_PRECIS,	// output precision
    CLIP_DEFAULT_PRECIS,	// clipping precision
    DEFAULT_QUALITY,	// output quality
    DEFAULT_PITCH | FF_DONTCARE,	// pitch and family
    gui_font_family 	// pointer to typeface name string
   );

   if (hfont == NULL )
    ui_error(UI_ERROR_ERROR, "Ошибка: невозможно использовать указаный шрифт.\nВыберите команду Настройки->Внешний вид... и выберите один из доступных шрифтов.");

  // запоминаем объект, который был выбран до рисования
  // и активизируем созданый шрифт
  if ((hfontOld = SelectObject(hdc,hfont)) == NULL)
    ui_error(UI_ERROR_ERROR, "Ошибка: SelectObject вернула NULL");

  // берем высоту текста в пикселах
  GetTextMetrics(hdc, &tm);
  font_height = tm.tmHeight;  // высота шрифта
  font_width = tm.tmAveCharWidth;  // ширина символа
}

/* Конец рисования, освобождение DC и кистей */
static void end_draw(HDC hdc)
{
  // востанавливаем текущим объект, который был активен до рисования
  SelectObject(hdc,hfontOld);

  // удаление кистей
  DeleteObject(hbrush_ws);
  DeleteObject(hbrush_pc);
  DeleteObject(hbrush_bp);

  /* удаление шрифта */
  DeleteObject(hfont);
}

#define SETCOLOR(x)\
  do {\
  if(bNeedSetColors) {\
    SetBkColor(hdc,gui_color[gui_sheme][x].paper);\
    SetTextColor(hdc,gui_color[gui_sheme][x].ink);\
  }\
} while(0)

/* Вывод одной линии
   Цвет должен выбираться в зависимости от того,
   нет ли точки останова на текущей линии,
   не указывает ли на нее PC
 */
int draw_line(HDC hdc,RECT *r,WORD addr)
{
  int res;
  DISASRESULT dr;
  char n,*p,buf[50];
  COLORREF ink, paper;
  HBRUSH tempBrush;
  int bNeedSetColors;

  bNeedSetColors = 1;

  // дизасемблируем команду по адресу addr
  res = disas_addr(addr, &dr);

  paper = gui_color[gui_sheme][GUI_WHTSPC].paper;

  // если здесь точка останова, то ее цвета
  if (break_isbreak(addr)) {
    ink = gui_color[gui_sheme][GUI_BRKPNT].ink;
    paper = gui_color[gui_sheme][GUI_BRKPNT].paper;
    bNeedSetColors = 0;
  }

  // если на этом адресе стоит курсор, то использовать его цвета
  if (addr == cur_addr) {
    ink ^= gui_color[gui_sheme][GUI_CURPNT].ink;
    paper ^= gui_color[gui_sheme][GUI_CURPNT].paper;
    bNeedSetColors = 0;
  }

  // заливаем строку нужным фоном
  tempBrush = CreateSolidBrush(paper);
  FillRect(hdc, r, tempBrush);
  DeleteObject(tempBrush);

  // устанавливаем цвета для вывода текста
  SetTextColor(hdc, ink);
  SetBkColor(hdc, paper);

  // выводим поле меток (в нем обычно адрес команды)
  r->left = 5;
  SETCOLOR(GUI_ADDR);
  DrawText(hdc,dr.addr,-1,r,DT_LEFT);

  // выводим точку исполнения, если она здесь
  if (addr == i8080.pc.w) {
    r->left = font_width * 7;
    DrawText(hdc,">",-1,r,DT_LEFT);
  }

  // выводим поле кодов команды
  r->left = font_width * 9;
  p = buf;
  for(n=0; n<res; n++)
    p+=sprintf(p,"%02X",MEM[addr++]);
  SETCOLOR(GUI_CODE);
  DrawText(hdc,buf,-1,r,DT_LEFT);

  // выводим поле мнемоник
  r->left = font_width * 20;
  SETCOLOR(GUI_MNEM);
  DrawText(hdc,dr.mnem,-1,r,DT_LEFT);

  // выводим поле операндов
  r->left = font_width * 30;
  SETCOLOR(GUI_OPS);
  DrawText(hdc,dr.ops,-1,r,DT_LEFT);

  return res;
}


void draw_regs(HDC hdc,RECT *r)
{
  char buf[50];

  FillRect(hdc, r, hbrush_ws);

  SetBkColor(hdc,gui_color[gui_sheme][GUI_ADDR].paper);
  SetTextColor(hdc,gui_color[gui_sheme][GUI_ADDR].ink);

  sprintf(buf, " AF %04X", i8080.af.w);
  DrawText(hdc,buf,-1,r,DT_LEFT);

  r->top += font_height;
  sprintf(buf, " BC %04X", i8080.bc.w);
  DrawText(hdc,buf,-1,r,DT_LEFT);

  r->top += font_height;
  sprintf(buf, " DE %04X", i8080.de.w);
  DrawText(hdc,buf,-1,r,DT_LEFT);

  r->top += font_height;
  sprintf(buf, " HL %04X", i8080.hl.w);
  DrawText(hdc,buf,-1,r,DT_LEFT);

  r->top += font_height;
  sprintf(buf, " PC %04X", i8080.pc.w);
  DrawText(hdc,buf,-1,r,DT_LEFT);

  r->top += font_height;
  sprintf(buf, " SP %04X", i8080.sp.w);
  DrawText(hdc,buf,-1,r,DT_LEFT);
}


void draw_flags(HDC hdc,RECT *r)
{
  char buf[50];

  FillRect(hdc, r, hbrush_ws);

  SetBkColor(hdc,gui_color[gui_sheme][GUI_ADDR].paper);
  SetTextColor(hdc,gui_color[gui_sheme][GUI_ADDR].ink);

  sprintf(buf, " CY = %d", i8080.af.b.l & 0x01);
  DrawText(hdc,buf,-1,r,DT_LEFT);

  r->top += font_height;
  sprintf(buf, " P  = %d", (i8080.af.b.l & 0x04) >> 2);
  DrawText(hdc,buf,-1,r,DT_LEFT);

  r->top += font_height;
  sprintf(buf, " AC = %d", (i8080.af.b.l & 0x10) >> 4);
  DrawText(hdc,buf,-1,r,DT_LEFT);

  r->top += font_height;
  sprintf(buf, " Z  = %d", (i8080.af.b.l & 0x40) >> 6);
  DrawText(hdc,buf,-1,r,DT_LEFT);

  r->top += font_height;
  sprintf(buf, " S  = %d", (i8080.af.b.l & 0x80) >> 7);
  DrawText(hdc,buf,-1,r,DT_LEFT);
}


void gui_draw(HDC hdc,RECT *crect)
{
  int line;  // текущая строка, всего
  WORD addr;
  RECT rect;

  // начало рисования
  begin_draw(hdc);

  // вычисляем сколько строк поместится в окно по высоте
  num_lines = (crect->bottom - crect->top) / font_height + 1;

  addr = top_addr;

  for (line = 0; line < num_lines; line++) {
    RECT lr;
    lr.left = crect->left;
    lr.right = crect->left + font_width * 50;
    lr.top = line * font_height;
    lr.bottom = (line+1) * font_height;
    addr += draw_line(hdc, &lr, addr);
//    MessageBeep(0); Sleep(1000);
  }

  /* вывод значений регистров */
  rect.left = crect->left + font_width * 50 + 3;
  rect.right = crect->right;
  rect.top = crect->top;
  rect.bottom = font_height * 6;
  draw_regs(hdc, &rect);

  /* вывод значений флагов */
  rect.top = crect->top + font_height * 6 + 3;
  rect.bottom = crect->bottom;
  draw_flags(hdc, &rect);

  // конец рисования
  end_draw(hdc);
}


/* Функция следит, чтоб заданый адрес был виден на экране.
   Если он и так виден, ничего не делает.
 */
void show_addr(WORD addr)
{
  WORD a;
  int n,m;

  // проверяем, отображается ли сейчас данный адрес (с запасом в 2 строки)
  for(a=top_addr,n=0; n<(num_lines-2); n++,a+=disas_lenght(a))
    if(addr==a)
      return;

  /* если дошли до сюда,
     значит, этот адрес не отображается,
     поправляем это
   */
  for(n=1; n<(num_lines-2); n++,a+=disas_lenght(a)) {
    if(addr==a) {
      // то просто смещаем вид вниз
      for(m=0; m<n; m++) {
        top_addr += disas_lenght(top_addr);
      }
      return;
    }
  }

  top_addr = addr;
}


void StepOverRemoveTempBreak(void)
{
  if (StopOverBreakAddr != -1) {
    break_catch(StopOverBreakAddr);
    StopOverBreakAddr = -1;
  }
}


/* Функции, вызываемые из главной формы */

void gui_up(void)
{
  WORD l,c;

  // если сейчас курсор не в первой сверху строке
  if (cur_addr != top_addr) {
    for(c=top_addr; c!=cur_addr; c+=disas_lenght(c)) l=c;
    cur_addr = l;
  }
  else {  // иначе сканируем c 50 байта выше
    for(c=top_addr-50; c!=cur_addr; c+=disas_lenght(c)) l=c;
    cur_addr = l;
  }

  show_addr(cur_addr);          // показать строку на которой курсор
}

void gui_run_goto(WORD addr)
{
  mpsys_stop();         // прерываем исполнение, если было
  gui_view_from(addr);     // просмотр с указанного адреса
  i8080.pc.w = addr;
}

void gui_down(void)
{
  // смещаем курсор на следующую команду
  cur_addr += disas_lenght(cur_addr);

  show_addr(cur_addr);          // показать строку на которой курсор
}

void gui_pageup(void)
{
  int n;
  for(n=1; n<num_lines; n++)
    gui_up();
}

void gui_pagedown(void)
{
  int n;
  for(n=1; n<num_lines; n++)
    top_addr += disas_lenght(top_addr);

  // курсор будет в той-же строке, что и до нажания pagedown
  cur_addr = top_addr;
  for(n=0; n<cur_line; n++)
    cur_addr += disas_lenght(cur_addr);
}


void gui_scroll(int diff)
{
  top_addr += diff;
}

void gui_click(int y)
{
  int n;
  cur_line = y / font_height;
  cur_addr = top_addr;
  for(n=0; n<cur_line; n++)
    cur_addr += disas_lenght(cur_addr);
}


void gui_file_clearmem()
{
  memset(MEM,0,sizeof(MEM));
}


void gui_file_loadimage(char *filename)
{
  FILE *fp;

  fp = fopen(filename, "rb");

  /* Считываем состояние эмулятора */
  fread(&i8080, sizeof(processor), 1, fp);

  /* Считываем содержимое памяти */
  fread(&MEM, sizeof(MEM), 1, fp);

  fclose(fp);
}

void gui_file_saveimage(char *filename)
{
  FILE *fp;

  fp = fopen(filename, "wb");

  /* Сохраняем состояние эмулятора */
  fwrite(&i8080, sizeof(processor), 1, fp);

  /* Сохраняем содержимое памяти */
  fwrite(&MEM, sizeof(MEM), 1, fp);

  fclose(fp);
}

void gui_file_loadblock(char *filename, int addr)
{
  FILE *fp;
  WORD _addr, len;

  fp = fopen(filename, "rb");

  /* Считываем адрес и длинну блока */
  fread(&_addr, sizeof(WORD), 1, fp);
  fread(&len,  sizeof(WORD), 1, fp);

  /* Если начало блока+длина выходят за пределы памяти, ошибка */
  if ((addr + len) > 0xffff) {
    ui_error(0,"Ошибка: Невозможно загрузить блок по адресу %X- выход за пределы памяти виртуальной системы! (адрес+длина=%X", addr, addr+len);
    return;
  }

  /* Считываем данные  */
  fread(&MEM[addr], len, 1, fp);

  fclose(fp);
}


void gui_file_loadbin(char *filename, int addr, int len)
{
  FILE *fp;

  fp = fopen(filename, "rb");

  if (len == 0) {
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
  }

  /* Если начало блока+длина выходят за пределы памяти, ошибка */
  if ((addr + len) > 0xffff) {
    ui_error(0,"Ошибка: Невозможно загрузить блок по адресу %X- выход за пределы памяти виртуальной системы! (адрес+длина=%X", addr, addr+len);
    return;
  }

  /* Считываем данные  */
  fread(&MEM[addr], len, 1, fp);

  fclose(fp);
}


void gui_file_saveblock(char *filename, int addr, int len)
{
  FILE *fp;
  WORD a,l;

  if (addr < 0 || addr > 0xffff) {
    ui_error(0,"Ошибка: начальный адрес блока равен %X, что недопустимо (адрес может быть числом от 0000 до FFFF)", addr);
    return;
  }

  if (len < 1) {
    ui_error(0,"Ошибка: длинна блока равна %X, что недопустимо (длина блока может быть числом от 0001 до FFFF)", addr);
    return;
  }

  if ((addr + len) > 0xffff) {
    ui_error(0,"Ошибка: размеры блока выходят за пределы памяти виртуальной системы! (адрес+длина=%X", addr+len);
    return;
  }

  a = addr;
  l = len;

  fp = fopen(filename, "wb");

  /* Сохраняем адрес и длинну блока */
  fwrite(&a, sizeof(WORD), 1, fp);
  fwrite(&l, sizeof(WORD), 1, fp);

  /* Сохраняем данные блока */
  fwrite(&MEM[addr], len, 1, fp);

  fclose(fp);
}


void gui_view_from(WORD addr)
{
  top_addr = addr;
  cur_addr = top_addr;  // курсор помещаем на верхнюю строку
}

/* Поиск последовательности.
   Ищет указаную последовательность начиная со следующей
   после курсора строки. Если не находит, выводит сообщение
   об этом, а если находит, то устанавливает на нее курсор.
 */
void gui_view_find(BYTE *data, int len)
{
  WORD a = cur_addr;
  WORD n;

  if (len < 1 || len > MAX_HEX_SEQUENCE)
    return;

  /* начинаем поиск со следующего байта */
  a++;

  /* выполняем поиск пока не обойдем всю память */
  for ( ; a != cur_addr; a++) {
    /* побайтно сверяем фрагменты */
    for (n = 0; n < len; n++) {
      if (MEM[a + n] != *(data+n))
        break;
    }
    /* Если цикл выполнился до конца, значит нашли */
    if (n == len) {
      cur_addr = a;
      show_addr(a);
      return;
    }
  }
  /* если дошли до сюда, не нашли */
  ui_error(0, "Фрагмент не найден");
}

/* ввод последовательности.
   Записывает указаную последовательность начиная с
   адреса курсора.
 */
void gui_view_put(BYTE *data, int len)
{
  int n;
  WORD a = cur_addr;

  // если len == -1 то ничего выполняться не будет
  for (n=0; n < len; n++)
    MEM[a++] = *(data+n);
}

/* Сброс, выполнять сначала */
void gui_run_reset()
{
  mpsys_stop();         // прерываем исполнение, если было
  mpsys_reset();        // сброс микропроцессорной системы
  gui_view_from(0);     // просмотр с нулевого адреса

  /* Если до этого выполняли StepOver то сбросить временую точку останова */
  StepOverRemoveTempBreak();
}

/* Выполнить одиночную команду */
void gui_run_stepinto()
{
  i8080_do_opcode();     // выполняем одну команду
  show_addr(i8080.pc.w); // показываем точку выполнения
}

/* Выполнить до следующей команды */
void gui_run_stepover()
{
  WORD next;
  next = i8080.pc.w + disas_lenght(i8080.pc.w);
  // устанавливаем точку останова на одно срабатывание
  break_setbreak(next, 1);
  StopOverBreakAddr = next;

  gui_run_run();
}

/* Запуск с текущего места */
void gui_run_run()
{
  mpsys_run(hmain);
}

/* остановка запущеной программы */
void gui_run_stop()
{
  mpsys_stop();
  show_addr(i8080.pc.w);
  /* Если эмулятор был запущен по StepOver, удалить временую точку останова */
  StepOverRemoveTempBreak();
}

void gui_run_togglebrkpnt()
{
  break_toggle(cur_addr);
}


