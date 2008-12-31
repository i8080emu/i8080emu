/* ���������� ������� ����������������� ������������
*  ���������� � �������� ����.
*/
#include <windows.h>
#include <stdio.h>

#include "gui.h"
#include "disas.h"
#include "mpsys.h"
#include "i8080.h"
#include "breakpnt.h"
#include "event.h"

static HWND hmain;      // handle �������� ����

// �������� ����� ��� ���� ������������
static HFONT    hfont;
static HFONT    hfontOld;

static HBRUSH   hbrush_ws;
static HBRUSH   hbrush_pc;
static HBRUSH   hbrush_bp;
static HBRUSH   hbrushOld;

/* �������� ����� �������� ��� StepOver */
static int StopOverBreakAddr;

/* ������� ��������� �������� ���� */
GUI_COLOR gui_color[GUI_NUMSHEMES][GUI_NUMCOLORS];

/* ����� �������� �������� ����� */
int gui_sheme;

/* ����� � ������ ��� �������� ���� */
char   *gui_font_family;
int     gui_font_size;

static WORD top_addr = 0;    // ����� ������� ������� � ����
static WORD cur_addr=0;      // ����� ������� ��� ��������
static int num_lines;        // ������� ����� � ����
static int cur_line;         // ����� ������, � ������� ������
static int font_height;      // ������ ������ ������ � ��������
static int font_width;       // ������ �������

/* ������������ ������ ������ � ���������� �� ������ */
#define ERROR_MESSAGE_MAX_LENGTH 256

int ui_error( ui_error_level severity, const char *format, ... )
{
  va_list ap;
  char message[ ERROR_MESSAGE_MAX_LENGTH + 1 ];

  /* �������� ��������� �� �������� ���������� */
  va_start( ap, format );
  vsnprintf( message, ERROR_MESSAGE_MAX_LENGTH, format, ap );
  va_end( ap );

  /* ���������� ������ � ���������� */
  MessageBox(hmain,message,"I8080 Emulator error",MB_OK);

  return 0;
}


void gui_init(HWND main)
{
  // ���������� ��������� �������� ����
  hmain = main;

  /* ������������� ���� ������� */
  i8080_init();
  break_init();
  event_init();
  mpsys_init();

  /* ������������� ���������� ������ */
  StopOverBreakAddr = -1;
}


/* ������ ���������: ��������� DC, �������� ����������� �������� */
static void begin_draw(HDC hdc)
{
  TEXTMETRIC tm;

  /* �������� ������ ��� ������� �������� */
  hbrush_ws = CreateSolidBrush(gui_color[gui_sheme][GUI_WHTSPC].paper);
  hbrush_pc = CreateSolidBrush(gui_color[gui_sheme][GUI_CURPNT].paper);
  hbrush_bp = CreateSolidBrush(gui_color[gui_sheme][GUI_BRKPNT].paper);

  /* �������� ������ */
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
    ui_error(UI_ERROR_ERROR, "������: ���������� ������������ �������� �����.\n�������� ������� ���������->������� ���... � �������� ���� �� ��������� �������.");

  // ���������� ������, ������� ��� ������ �� ���������
  // � ������������ �������� �����
  if ((hfontOld = SelectObject(hdc,hfont)) == NULL)
    ui_error(UI_ERROR_ERROR, "������: SelectObject ������� NULL");

  // ����� ������ ������ � ��������
  GetTextMetrics(hdc, &tm);
  font_height = tm.tmHeight;  // ������ ������
  font_width = tm.tmAveCharWidth;  // ������ �������
}

/* ����� ���������, ������������ DC � ������ */
static void end_draw(HDC hdc)
{
  // �������������� ������� ������, ������� ��� ������� �� ���������
  SelectObject(hdc,hfontOld);

  // �������� ������
  DeleteObject(hbrush_ws);
  DeleteObject(hbrush_pc);
  DeleteObject(hbrush_bp);

  /* �������� ������ */
  DeleteObject(hfont);
}

#define SETCOLOR(x)\
  do {\
  if(bNeedSetColors) {\
    SetBkColor(hdc,gui_color[gui_sheme][x].paper);\
    SetTextColor(hdc,gui_color[gui_sheme][x].ink);\
  }\
} while(0)

/* ����� ����� �����
   ���� ������ ���������� � ����������� �� ����,
   ��� �� ����� �������� �� ������� �����,
   �� ��������� �� �� ��� PC
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

  // �������������� ������� �� ������ addr
  res = disas_addr(addr, &dr);

  paper = gui_color[gui_sheme][GUI_WHTSPC].paper;

  // ���� ����� ����� ��������, �� �� �����
  if (break_isbreak(addr)) {
    ink = gui_color[gui_sheme][GUI_BRKPNT].ink;
    paper = gui_color[gui_sheme][GUI_BRKPNT].paper;
    bNeedSetColors = 0;
  }

  // ���� �� ���� ������ ����� ������, �� ������������ ��� �����
  if (addr == cur_addr) {
    ink ^= gui_color[gui_sheme][GUI_CURPNT].ink;
    paper ^= gui_color[gui_sheme][GUI_CURPNT].paper;
    bNeedSetColors = 0;
  }

  // �������� ������ ������ �����
  tempBrush = CreateSolidBrush(paper);
  FillRect(hdc, r, tempBrush);
  DeleteObject(tempBrush);

  // ������������� ����� ��� ������ ������
  SetTextColor(hdc, ink);
  SetBkColor(hdc, paper);

  // ������� ���� ����� (� ��� ������ ����� �������)
  r->left = 5;
  SETCOLOR(GUI_ADDR);
  DrawText(hdc,dr.addr,-1,r,DT_LEFT);

  // ������� ����� ����������, ���� ��� �����
  if (addr == i8080.pc.w) {
    r->left = font_width * 7;
    DrawText(hdc,">",-1,r,DT_LEFT);
  }

  // ������� ���� ����� �������
  r->left = font_width * 9;
  p = buf;
  for(n=0; n<res; n++)
    p+=sprintf(p,"%02X",MEM[addr++]);
  SETCOLOR(GUI_CODE);
  DrawText(hdc,buf,-1,r,DT_LEFT);

  // ������� ���� ��������
  r->left = font_width * 20;
  SETCOLOR(GUI_MNEM);
  DrawText(hdc,dr.mnem,-1,r,DT_LEFT);

  // ������� ���� ���������
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
  int line;  // ������� ������, �����
  WORD addr;
  RECT rect;

  // ������ ���������
  begin_draw(hdc);

  // ��������� ������� ����� ���������� � ���� �� ������
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

  /* ����� �������� ��������� */
  rect.left = crect->left + font_width * 50 + 3;
  rect.right = crect->right;
  rect.top = crect->top;
  rect.bottom = font_height * 6;
  draw_regs(hdc, &rect);

  /* ����� �������� ������ */
  rect.top = crect->top + font_height * 6 + 3;
  rect.bottom = crect->bottom;
  draw_flags(hdc, &rect);

  // ����� ���������
  end_draw(hdc);
}


/* ������� ������, ���� ������� ����� ��� ����� �� ������.
   ���� �� � ��� �����, ������ �� ������.
 */
void show_addr(WORD addr)
{
  WORD a;
  int n,m;

  // ���������, ������������ �� ������ ������ ����� (� ������� � 2 ������)
  for(a=top_addr,n=0; n<(num_lines-2); n++,a+=disas_lenght(a))
    if(addr==a)
      return;

  /* ���� ����� �� ����,
     ������, ���� ����� �� ������������,
     ���������� ���
   */
  for(n=1; n<(num_lines-2); n++,a+=disas_lenght(a)) {
    if(addr==a) {
      // �� ������ ������� ��� ����
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


/* �������, ���������� �� ������� ����� */

void gui_up(void)
{
  WORD l,c;

  // ���� ������ ������ �� � ������ ������ ������
  if (cur_addr != top_addr) {
    for(c=top_addr; c!=cur_addr; c+=disas_lenght(c)) l=c;
    cur_addr = l;
  }
  else {  // ����� ��������� c 50 ����� ����
    for(c=top_addr-50; c!=cur_addr; c+=disas_lenght(c)) l=c;
    cur_addr = l;
  }

  show_addr(cur_addr);          // �������� ������ �� ������� ������
}

void gui_run_goto(WORD addr)
{
  mpsys_stop();         // ��������� ����������, ���� ����
  gui_view_from(addr);     // �������� � ���������� ������
  i8080.pc.w = addr;
}

void gui_down(void)
{
  // ������� ������ �� ��������� �������
  cur_addr += disas_lenght(cur_addr);

  show_addr(cur_addr);          // �������� ������ �� ������� ������
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

  // ������ ����� � ���-�� ������, ��� � �� ������� pagedown
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

  /* ��������� ��������� ��������� */
  fread(&i8080, sizeof(processor), 1, fp);

  /* ��������� ���������� ������ */
  fread(&MEM, sizeof(MEM), 1, fp);

  fclose(fp);
}

void gui_file_saveimage(char *filename)
{
  FILE *fp;

  fp = fopen(filename, "wb");

  /* ��������� ��������� ��������� */
  fwrite(&i8080, sizeof(processor), 1, fp);

  /* ��������� ���������� ������ */
  fwrite(&MEM, sizeof(MEM), 1, fp);

  fclose(fp);
}

void gui_file_loadblock(char *filename, int addr)
{
  FILE *fp;
  WORD _addr, len;

  fp = fopen(filename, "rb");

  /* ��������� ����� � ������ ����� */
  fread(&_addr, sizeof(WORD), 1, fp);
  fread(&len,  sizeof(WORD), 1, fp);

  /* ���� ������ �����+����� ������� �� ������� ������, ������ */
  if ((addr + len) > 0xffff) {
    ui_error(0,"������: ���������� ��������� ���� �� ������ %X- ����� �� ������� ������ ����������� �������! (�����+�����=%X", addr, addr+len);
    return;
  }

  /* ��������� ������  */
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

  /* ���� ������ �����+����� ������� �� ������� ������, ������ */
  if ((addr + len) > 0xffff) {
    ui_error(0,"������: ���������� ��������� ���� �� ������ %X- ����� �� ������� ������ ����������� �������! (�����+�����=%X", addr, addr+len);
    return;
  }

  /* ��������� ������  */
  fread(&MEM[addr], len, 1, fp);

  fclose(fp);
}


void gui_file_saveblock(char *filename, int addr, int len)
{
  FILE *fp;
  WORD a,l;

  if (addr < 0 || addr > 0xffff) {
    ui_error(0,"������: ��������� ����� ����� ����� %X, ��� ����������� (����� ����� ���� ������ �� 0000 �� FFFF)", addr);
    return;
  }

  if (len < 1) {
    ui_error(0,"������: ������ ����� ����� %X, ��� ����������� (����� ����� ����� ���� ������ �� 0001 �� FFFF)", addr);
    return;
  }

  if ((addr + len) > 0xffff) {
    ui_error(0,"������: ������� ����� ������� �� ������� ������ ����������� �������! (�����+�����=%X", addr+len);
    return;
  }

  a = addr;
  l = len;

  fp = fopen(filename, "wb");

  /* ��������� ����� � ������ ����� */
  fwrite(&a, sizeof(WORD), 1, fp);
  fwrite(&l, sizeof(WORD), 1, fp);

  /* ��������� ������ ����� */
  fwrite(&MEM[addr], len, 1, fp);

  fclose(fp);
}


void gui_view_from(WORD addr)
{
  top_addr = addr;
  cur_addr = top_addr;  // ������ �������� �� ������� ������
}

/* ����� ������������������.
   ���� �������� ������������������ ������� �� ���������
   ����� ������� ������. ���� �� �������, ������� ���������
   �� ����, � ���� �������, �� ������������� �� ��� ������.
 */
void gui_view_find(BYTE *data, int len)
{
  WORD a = cur_addr;
  WORD n;

  if (len < 1 || len > MAX_HEX_SEQUENCE)
    return;

  /* �������� ����� �� ���������� ����� */
  a++;

  /* ��������� ����� ���� �� ������� ��� ������ */
  for ( ; a != cur_addr; a++) {
    /* �������� ������� ��������� */
    for (n = 0; n < len; n++) {
      if (MEM[a + n] != *(data+n))
        break;
    }
    /* ���� ���� ���������� �� �����, ������ ����� */
    if (n == len) {
      cur_addr = a;
      show_addr(a);
      return;
    }
  }
  /* ���� ����� �� ����, �� ����� */
  ui_error(0, "�������� �� ������");
}

/* ���� ������������������.
   ���������� �������� ������������������ ������� �
   ������ �������.
 */
void gui_view_put(BYTE *data, int len)
{
  int n;
  WORD a = cur_addr;

  // ���� len == -1 �� ������ ����������� �� �����
  for (n=0; n < len; n++)
    MEM[a++] = *(data+n);
}

/* �����, ��������� ������� */
void gui_run_reset()
{
  mpsys_stop();         // ��������� ����������, ���� ����
  mpsys_reset();        // ����� ����������������� �������
  gui_view_from(0);     // �������� � �������� ������

  /* ���� �� ����� ��������� StepOver �� �������� �������� ����� �������� */
  StepOverRemoveTempBreak();
}

/* ��������� ��������� ������� */
void gui_run_stepinto()
{
  i8080_do_opcode();     // ��������� ���� �������
  show_addr(i8080.pc.w); // ���������� ����� ����������
}

/* ��������� �� ��������� ������� */
void gui_run_stepover()
{
  WORD next;
  next = i8080.pc.w + disas_lenght(i8080.pc.w);
  // ������������� ����� �������� �� ���� ������������
  break_setbreak(next, 1);
  StopOverBreakAddr = next;

  gui_run_run();
}

/* ������ � �������� ����� */
void gui_run_run()
{
  mpsys_run(hmain);
}

/* ��������� ��������� ��������� */
void gui_run_stop()
{
  mpsys_stop();
  show_addr(i8080.pc.w);
  /* ���� �������� ��� ������� �� StepOver, ������� �������� ����� �������� */
  StepOverRemoveTempBreak();
}

void gui_run_togglebrkpnt()
{
  break_toggle(cur_addr);
}


