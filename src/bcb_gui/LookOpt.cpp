// Эмулятор-отладчик i8080
// Настройка внешнего вида программы
//---------------------------------------------------------------------------
#include <vcl.h>
#include <windows.h>
#include <inifiles.hpp>
#pragma hdrstop

#include "LookOpt.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLookOptForm *LookOptForm;

#include "gui.h"

/* Выбранные в диалоге параметры */
String font_family;
int font_height;
String color_sheme;

/* Временные настройки схем.
   Если нажата ОК, то они копируются в cur_sheme
   Если отмена, то изменения никуда не пойдут.
 */
GUI_COLOR temp_shemes[GUI_NUMSHEMES][GUI_NUMCOLORS];

/* Цветовые схемы по умолчанию */
GUI_COLOR default_shemes[GUI_NUMSHEMES][GUI_NUMCOLORS] = {
      // цветовые схемы: синяя
      {
         {RGB(0x00,0x00,0x00),RGB(0x00,0x00,0xcc)},  // пробелы
         {RGB(0xff,0xff,0xff),RGB(0x00,0x00,0xcc)},  // адрес
         {RGB(0xff,0xff,0xff),RGB(0x00,0x00,0xcc)},  // код команды
         {RGB(0x88,0x33,0x00),RGB(0x00,0x00,0xcc)},  // мнемоника
         {RGB(0x00,0xcc,0xcc),RGB(0x00,0x00,0xcc)},  // операнды
         {RGB(0x00,0x00,0x00),RGB(0x00,0x77,0x77)},  // курсор
         {RGB(0xff,0xff,0xff),RGB(0xff,0x00,0x00)},  // точка останова
      },
      // цветовые схемы: белая
      {
         {RGB(0x00,0x00,0x00),RGB(0xff,0xff,0xff)},  // пробелы
         {RGB(0x00,0x00,0xff),RGB(0xff,0xff,0xff)},  // адрес
         {RGB(0x00,0x00,0xff),RGB(0xff,0xff,0xff)},  // код команды
         {RGB(0x00,0x00,0x00),RGB(0xff,0xff,0xff)},  // мнемоника
         {RGB(0x00,0x00,0x00),RGB(0xff,0xff,0xff)},  // операнды
         {RGB(0xff,0xff,0xff),RGB(0x00,0x55,0x99)},  // курсор
         {RGB(0xff,0xff,0xff),RGB(0xff,0x00,0x00)},  // точка останова
      },
      // цветовые схемы: черная
      {
         {RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00)},  // пробелы
         {RGB(0xff,0xff,0xff),RGB(0x00,0x00,0x00)},  // адрес
         {RGB(0xff,0xff,0xff),RGB(0x00,0x00,0x00)},  // код команды
         {RGB(0xff,0xff,0xff),RGB(0x00,0x00,0x00)},  // мнемоника
         {RGB(0xff,0xff,0xff),RGB(0x00,0x00,0x00)},  // операнды
         {RGB(0xff,0xff,0xff),RGB(0x00,0x77,0x77)},  // курсор
         {RGB(0x00,0x00,0x00),RGB(0xff,0x00,0x00)},  // точка останова
      },
      // цветовые схемы: пользовательская
      {
         {RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00)},  // пробелы
         {RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00)},  // адрес
         {RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00)},  // код команды
         {RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00)},  // мнемоника
         {RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00)},  // операнды
         {RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00)},  // курсор
         {RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00)},  // точка останова
      }
   };

//---------------------------------------------------------------------------
__fastcall TLookOptForm::TLookOptForm(TComponent* Owner)
        : TForm(Owner)
{
  try
  {
    // загружаем схемы из файла
    TFileStream *fs = new TFileStream(ExtractFileDir(Application->ExeName) + "/color.shm",fmOpenRead);
    fs->ReadBuffer(temp_shemes,sizeof(temp_shemes));
    delete fs;
  }
  catch (Exception &exception)
  {
    // если не удалось загрузить, используем схемы по умолчанию
    memcpy(temp_shemes, default_shemes, sizeof(default_shemes));
  }

  TIniFile *ini;
  ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));

  font_height = ini->ReadInteger( "Look", "FontHeight", 10 );
  font_family = ini->ReadString( "Look", "FontFamily", "MS Sans Serif");
  color_sheme = ini->ReadString( "Look", "ColorSheme", "синяя");

  delete ini;

  /* активация выбраных параметров */
  BitBtn1Click(NULL);
}

/* Кнопка выбора цвета элемента */
void __fastcall TLookOptForm::Button1Click(TObject *Sender)
{
  int i = ColorShemeComboBox->ItemIndex;        // номер схемы
  int e = ElementListBox->ItemIndex;            // номер элемента

  // вносим выбраный цвет во временную схему
  if(ColorDialog1->Execute()) {
    temp_shemes[i][e].ink = ColorDialog1->Color;
    Shape1->Brush->Color = ColorDialog1->Color;
  }
}

/* Щелчок по фигуре квадрата тоже, что и по кнопке "Выбрать" */
void __fastcall TLookOptForm::Shape1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (Button == mbLeft) Button1Click(NULL);
}

/* Кнопка выбора фона элемента */
void __fastcall TLookOptForm::Button2Click(TObject *Sender)
{
  int i = ColorShemeComboBox->ItemIndex;        // номер схемы
  int e = ElementListBox->ItemIndex;            // номер элемента

  if(ColorDialog2->Execute()) {
    temp_shemes[i][e].paper = ColorDialog2->Color;
    Shape2->Brush->Color = ColorDialog2->Color;
  }
}

/* Щелчок по фигуре квадрата тоже, что и по кнопке "Выбрать" */
void __fastcall TLookOptForm::Shape2MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (Button == mbLeft) Button2Click(NULL);
}

/* функция обратного вызова для перечисления шрифтов */
int CALLBACK EnumFontFamProc(
    ENUMLOGFONT FAR *lpelf,	// pointer to logical-font data
    NEWTEXTMETRIC FAR *lpntm,	// pointer to physical-font data
    int FontType,	// type of font
    LPARAM lParam 	// address of application-defined data
   )
{
  String name = (char*)lpelf->elfLogFont.lfFaceName;
  LookOptForm->FontListBox->Items->Add(name);

  return 1; // продолжить перечисление
}

/* Показ формы. Параметры, отображаемые в окне должны
   браться из модуля gui, а не те, что последними были
   выставлены, т.к. при этом могли нажать Отмену, а не ОК
 */
void __fastcall TLookOptForm::FormShow(TObject *Sender)
{
  // копирование цветовых схем (на случай, если будет нажата Отмена
  // и на надо сохранять изменения
  memcpy(temp_shemes, gui_color, sizeof(gui_color));

  /* Очистка списка шрифтов */
  FontListBox->Clear();

  /* Запрос у системы всех возможных шрифтов */
  EnumFontFamilies(
    (void*)(ExemplLabel->Canvas->Handle),
    NULL,	// pointer to family-name string
    (FONTENUMPROC)EnumFontFamProc,	// pointer to callback function
    NULL 	// address of application-supplied data
   );

  /* установка в окне параметров из модуля gui */
  FontSizeComboBox->ItemIndex = gui_font_size - 7;   // 7 - наименьшее число, оно написано первым в строках FontSizeComboBox
  FontListBox->ItemIndex = FontListBox->Items->IndexOf(gui_font_family);
  ColorShemeComboBox->ItemIndex = gui_sheme;
  if (ElementListBox->ItemIndex <0 ) ElementListBox->ItemIndex = 0;

  /* Перерисовка образцов */
  FontListBoxClick(NULL);
  ElementListBoxClick(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TLookOptForm::FontListBoxClick(TObject *Sender)
{
  font_height = StrToInt(FontSizeComboBox->Text);

  // если что-то выбрано в списке доступных шрифтов
  if ( FontListBox->ItemIndex >= 0)
    font_family = FontListBox->Items->Strings[FontListBox->ItemIndex];

  ExemplLabel->Font->Size = font_height;
  ExemplLabel->Font->Name = font_family;
}
//---------------------------------------------------------------------------

/* Выбор цветовой схемы из выпадающего списка */
void __fastcall TLookOptForm::ColorShemeComboBoxChange(TObject *Sender)
{
  int i = ColorShemeComboBox->ItemIndex;
  color_sheme = ColorShemeComboBox->Items->Strings[i];
  ElementListBoxClick(NULL);
}

/* Щелчок по списку элементов (пробел, адрес, мнемоника...)
   Изменение цвета квадратов, отображающих цвет чернил и фона.
 */
void __fastcall TLookOptForm::ElementListBoxClick(TObject *Sender)
{
  int i = ColorShemeComboBox->ItemIndex;        // номер выбраной схемы
  int e = ElementListBox->ItemIndex;            // номер выбраного элемента

  if (i<0 || e<0) return;

  // окрашиваем квадраты в цвета выбраного элемента
  Shape1->Brush->Color = (TColor)temp_shemes[i][e].ink;
  Shape2->Brush->Color = (TColor)temp_shemes[i][e].paper;
}

/* Кнопка ОК, передаем выбраные параметры в модуль gui */
void __fastcall TLookOptForm::BitBtn1Click(TObject *Sender)
{
  int i = ColorShemeComboBox->Items->IndexOf(color_sheme);
  if (i<0) i=0;
  /* активация выбраных параметров */
  gui_font_size = font_height;
  gui_font_family = font_family.c_str();
  gui_sheme = i;

  // записываем изменения цветовой схемы
  memcpy(gui_color, temp_shemes, sizeof(gui_color));
}

/* Закрытие формы, сохранение параметров */
void __fastcall TLookOptForm::FormDestroy(TObject *Sender)
{
TIniFile *ini;
ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));
  ini->WriteInteger( "Look", "FontHeight", font_height);
  ini->WriteString( "Look", "FontFamily", font_family);
  ini->WriteString( "Look", "ColorSheme", color_sheme);
delete ini;
}
//---------------------------------------------------------------------------

void __fastcall TLookOptForm::Button5Click(TObject *Sender)
{
  TFileStream *fs = new TFileStream("color.shm",fmCreate);
  fs->WriteBuffer(temp_shemes,sizeof(temp_shemes));
  delete fs;
}
//---------------------------------------------------------------------------

void __fastcall TLookOptForm::Button4Click(TObject *Sender)
{
  // копируем схемы по умолчанию в текущие
  memcpy(temp_shemes, default_shemes, sizeof(default_shemes));
}
//---------------------------------------------------------------------------



