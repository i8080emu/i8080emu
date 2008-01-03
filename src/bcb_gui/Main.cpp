// Эмулятор-отладчик i8080
// Главная форма
//---------------------------------------------------------------------------
#include <vcl.h>
#include "Main.h"
#include "Assembler.h"
#include "About.h"
#include "LookOpt.h"
#include "ViewFrom.h"
#include "SaveBlock.h"
#include "LoadBlock.h"
#include "MpsysOpt.h"
#include "Modules.h"
#include "FindPut.h"
#include "Memory.h"
#include "RegFlag.h"
#include "Disassembler.h"
#include "LoadBin.h"
#include "Goto.h"
#pragma hdrstop

#include "core/mpsys.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TMainForm *MainForm;

/* Флаг того что перерисовки окна еще не было */
BOOL first_paint = true;

/* Флаг выполнения в реальном времени */
BOOL is_now_execute = false;

/* флаг остановки по HLT */
BOOL bHltStop;


extern "C" void mpsys_load_device(HWND hwnd, HINSTANCE hinst, char *filename);

//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FileExitExecute(TObject *Sender)
{
        Close();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FileAsmExecute(TObject *Sender)
{
        AsmForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::HelpAboutExecute(TObject *Sender)
{
        AboutForm->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormPaint(TObject *Sender)
{
  /* при первой перерисовке окна производим инициализацию */
  if (first_paint)
  {
    TSearchRec sr;

    first_paint = 0;

    /* Переходим в директорию плагинов */
    SetCurrentDir("plugins");

    /* Поиск всех модулей (динамических библиотек) */
    if (FindFirst("*.dll", faAnyFile, sr) == 0)
    {
      do
      {
        /* загрузка внешнего устройства */
        mpsys_load_device(Handle, MainInstance, sr.Name.c_str());
      } while (FindNext(sr) == 0);
      FindClose(sr);
    }

    /* переходим в корневую директорию эмулятора */
    SetCurrentDir("..");
  }

  /* В зависимости от режима работы выводим окно дизассемблера или пустой экран */
  if (!is_now_execute) {

    PaintBox->Hint = "Эмулятор находится в режиме пошагового исполнения";

/* экспериментальный код- перерисовка в памяти
Чтобы использовать обычный код для рисования на экране, замените 1 на 0 */
#if 1
    /* перерисовка клиентской области окна
       с использованием внеэкранной поверхности для рисования,
       чтобы избежать мельканий на экране
     */
    Graphics::TBitmap *bm = new Graphics::TBitmap();

    bm->Height = PaintBox->Height;
    bm->Width = PaintBox->Width;

    gui_draw(bm->Canvas->Handle,&(bm->Canvas->ClipRect));
    PaintBox->Canvas->CopyMode = cmSrcCopy;
    PaintBox->Canvas->Draw(0, 0, bm);

    delete bm;

#else
    /* перерисовка окна прямо на экране */
    gui_draw(PaintBox->Canvas->Handle,&(PaintBox->Canvas->ClipRect));

#endif

    /* также необходимо перерисовывать окно с содержимым памяти
       если оно видно на экране */
    if (MemoryForm->Visible)
      MemoryForm->Repaint();
  }
  else {
    PaintBox->Hint = "Эмулятор находится в режиме исполнения в реальном времени";

    /* Делаем рабочую область окна недоступной */
    PaintBox->Cursor = crHourGlass;

    PaintBox->Canvas->TextRect(PaintBox->ClientRect, 50,20, "Выполнение в реальном времени");
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormCreate(TObject *Sender)
{
  // инициализация графического интерфейса программы
  gui_init(Handle);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::OptLookExecute(TObject *Sender)
{
  LookOptForm->ShowModal();
  SendMessage(Handle,WM_PAINT,0,0);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FileLoadImageExecute(TObject *Sender)
{
  if (OpenDialog1->Execute()) {
    gui_file_loadimage(OpenDialog1->FileName.c_str());
    SendMessage(Handle,WM_PAINT,0,0);
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FileCleanExecute(TObject *Sender)
{
  if (MessageBox(Handle, "Выполнение этой команды очистит все содержимое памяти. Продолжить?", "Предупреждение", MB_YESNOCANCEL) == IDYES) {
    gui_file_clearmem();
    gui_run_reset();
    SendMessage(Handle,WM_PAINT,0,0);
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ScrollBar1Change(TObject *Sender)
{
  gui_view_from(ScrollBar1->Position);
  SendMessage(Handle,WM_PAINT,0,0);
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::RunResetExecute(TObject *Sender)
{
  gui_run_reset();
  SendMessage(Handle,WM_PAINT,0,0);
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::RunStepIntoExecute(TObject *Sender)
{
  gui_run_stepinto();
  SendMessage(Handle,WM_PAINT,0,0);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::RunStepOverExecute(TObject *Sender)
{
  RunStepInto->Enabled = true;
  RunStepOver->Enabled = true;
  RunRun->Enabled = true;
  PaintBox->Enabled = true;
  RunStop->Enabled = false;

  bHltStop = HLTStop->Checked;

  gui_run_stepover();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::RunBreakExecute(TObject *Sender)
{
  gui_run_togglebrkpnt();
  SendMessage(Handle,WM_PAINT,0,0);
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  switch (Key)
  {
    case VK_UP: gui_up();
    break;
    case VK_DOWN: gui_down();
    break;
    case VK_PRIOR: gui_pageup();
    break;
    case VK_NEXT: gui_pagedown();
    break;
    case VK_HOME: gui_view_from(0);
    break;
  }
  SendMessage(Handle,WM_PAINT,0,0);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ViewFromExecute(TObject *Sender)
{
  if(ViewFromForm->ShowModal()) {
    gui_view_from(ViewFromForm->addr);
    MainForm->Repaint();
  }
}

int   MouseCurY;
//---------------------------------------------------------------------------

void __fastcall TMainForm::PaintBoxMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
  MouseCurY = Y;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::PaintBoxClick(TObject *Sender)
{
  // по щелчку курсор ставится на выбраную линию
  gui_click(MouseCurY);
  MainForm->Repaint();
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::RunRunExecute(TObject *Sender)
{
  /* Запрещаем обработку событий */
  PaintBox->Enabled = false;

  /* Устанавливаем флаг исполнения програмы в реальном времени */
  is_now_execute = true;

  /* устанавливаем состояние флага остановки по HLT из опции меню */
  bHltStop = HLTStop->Checked;

  /* Пересчитываем разрешеность действий меню */
  SetActionEnableStates();

  gui_run_run();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::RunStopExecute(TObject *Sender)
{
  /* Включаем рабочую область окна */
  PaintBox->Enabled = true;
  PaintBox->Cursor = crDefault;

  gui_run_stop();

  /* Сбрасываем флаг выполнения в реальном времени */
  is_now_execute = false;

  // пересматриваем разрешеность действий
  SetActionEnableStates();

  SendMessage(Handle,WM_PAINT,0,0);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CMIsBreak(TMessage &Message)
{
  RunStopExecute(NULL);
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::FileSaveImageExecute(TObject *Sender)
{
  if (SaveDialog1->Execute()) {
    gui_file_saveimage(SaveDialog1->FileName.c_str());
    SendMessage(Handle,WM_PAINT,0,0);
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FileLoadBlockExecute(TObject *Sender)
{
  if (OpenDialog2->Execute()) {
    /* Сообщаем диалогу имя файла загружаемого блока */
    LoadBlockForm->BlockFilename = OpenDialog2->FileName;
    /* Показываем диалог адреса загрузки блока */
    if (LoadBlockForm->ShowModal() == mrOk) {
      gui_file_loadblock(OpenDialog2->FileName.c_str(), LoadBlockForm->addr);
      SendMessage(Handle,WM_PAINT,0,0);
    }
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FileSaveBlockExecute(TObject *Sender)
{
  if (SaveDialog2->Execute()) {
    if (SaveBlockForm->ShowModal() == mrOk) {
      gui_file_saveblock(SaveDialog2->FileName.c_str(), SaveBlockForm->addr, SaveBlockForm->len);
      SendMessage(Handle,WM_PAINT,0,0);
    }
  }
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::OptEmuSpeedExecute(TObject *Sender)
{
  MPsysOptForm->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::OptModulesExecute(TObject *Sender)
{
  ModulesForm->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ViewFindExecute(TObject *Sender)
{
  // т.к. диалог поиска и ввода мнемоник и кодов- одно и тоже,
  // а справка по каждому своя, необходимо переключить ссылку
  // по кнопке "Справка" на нужный раздел.
  FindPutForm->HelpBitBtn->HelpContext = 150;

  if (FindPutForm->ShowModal()) {
    gui_view_find(FindPutForm->data, FindPutForm->len);
    MainForm->Repaint();
  }
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::ViewMemExecute(TObject *Sender)
{
  MemoryForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ViewStatusExecute(TObject *Sender)
{
  ViewStatus->Checked = !ViewStatus->Checked;
  StatusBar1->Visible = ViewStatus->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ViewToolbarExecute(TObject *Sender)
{
  ViewToolbar->Checked = !ViewToolbar->Checked;
  ToolBar1->Visible = ViewToolbar->Checked;
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::ViewRegFlagExecute(TObject *Sender)
{
  RegFlagForm->ShowModal();
  MainForm->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ViewPutExecute(TObject *Sender)
{
  // т.к. диалог поиска и ввода мнемоник и кодов- одно и тоже,
  // а справка по каждому своя, необходимо переключить ссылку
  // по кнопке "Справка" на нужный раздел.
  FindPutForm->HelpBitBtn->HelpContext = 140;

  if (FindPutForm->ShowModal()) {
    // запись в память ввода
    gui_view_put(FindPutForm->data, FindPutForm->len);
    MainForm->Repaint();
  }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormMouseWheelDown(TObject *Sender,
      TShiftState Shift, TPoint &MousePos, bool &Handled)
{
  gui_scroll(5);
  MainForm->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormMouseWheelUp(TObject *Sender,
      TShiftState Shift, TPoint &MousePos, bool &Handled)
{
  gui_scroll(-5);
  MainForm->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  // при выходе завершаем поток виртуальной машины
  gui_run_stop();

  // и проверяем, сохранен ли текст в окне ассемблера
  AsmForm->Close();
  CanClose = AsmFormCanClose;
}
//---------------------------------------------------------------------------

/* Эта функция анализирует состояние программы и
   устанавливает, какие действия сейчас могут быть
   доступными, устанавливая или убирая у них атрибут
   Enabled.
 */
void __fastcall TMainForm::SetActionEnableStates()
{
  // если идет выполнение в реальном времени, ряд действий
  // должен быть запрещен
  if (is_now_execute) {

    RunStop->Enabled = true;

    RunReset->Enabled = false;
    RunRun->Enabled = false;
    RunStepInto->Enabled = false;
    RunStepOver->Enabled = false;
  }
  else {

    RunStop->Enabled = false;

    RunReset->Enabled = true;
    RunRun->Enabled = true;
    RunStepInto->Enabled = true;
    RunStepOver->Enabled = true;
    RunRun->Enabled = true;
    RunGoto->Enabled = true;
  }

  // если ничего не введено, найти далее недоступно
  ViewFindNext->Enabled = FindPutForm->len != -1;

}

void __fastcall TMainForm::ViewFindNextExecute(TObject *Sender)
{
    gui_view_find(FindPutForm->data, FindPutForm->len);
    MainForm->Repaint();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormShow(TObject *Sender)
{
  SetActionEnableStates();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FileDisasExecute(TObject *Sender)
{
  DisasForm->Show();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::HelpHelpExecute(TObject *Sender)
{
  Application->HelpCommand(HELP_FINDER, 0);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::HelpOnhelpExecute(TObject *Sender)
{
  Application->HelpCommand(HELP_HELPONHELP, 0);
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::FileLoadBinExecute(TObject *Sender)
{
  if (LoadBinForm->ShowModal() == mrOk) {
      gui_file_loadbin(LoadBinForm->FileEdit->Text.c_str(), LoadBinForm->start, LoadBinForm->len);
      SendMessage(Handle,WM_PAINT,0,0);
  }
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::RunGotoExecute(TObject *Sender)
{
if(GotoForm->ShowModal()) {
    gui_run_goto(GotoForm->addr);
    MainForm->Repaint();
  }
}
//---------------------------------------------------------------------------

