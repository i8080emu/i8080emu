// ��������-�������� i8080
// ��������� �������� ���� ���������
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

/* ��������� � ������� ��������� */
String font_family;
int font_height;
String color_sheme;

/* ��������� ��������� ����.
   ���� ������ ��, �� ��� ���������� � cur_sheme
   ���� ������, �� ��������� ������ �� ������.
 */
GUI_COLOR temp_shemes[GUI_NUMSHEMES][GUI_NUMCOLORS];

/* �������� ����� �� ��������� */
GUI_COLOR default_shemes[GUI_NUMSHEMES][GUI_NUMCOLORS] = {
      // �������� �����: �����
      {
         {RGB(0x00,0x00,0x00),RGB(0x00,0x00,0xcc)},  // �������
         {RGB(0xff,0xff,0xff),RGB(0x00,0x00,0xcc)},  // �����
         {RGB(0xff,0xff,0xff),RGB(0x00,0x00,0xcc)},  // ��� �������
         {RGB(0x88,0x33,0x00),RGB(0x00,0x00,0xcc)},  // ���������
         {RGB(0x00,0xcc,0xcc),RGB(0x00,0x00,0xcc)},  // ��������
         {RGB(0x00,0x00,0x00),RGB(0x00,0x77,0x77)},  // ������
         {RGB(0xff,0xff,0xff),RGB(0xff,0x00,0x00)},  // ����� ��������
      },
      // �������� �����: �����
      {
         {RGB(0x00,0x00,0x00),RGB(0xff,0xff,0xff)},  // �������
         {RGB(0x00,0x00,0xff),RGB(0xff,0xff,0xff)},  // �����
         {RGB(0x00,0x00,0xff),RGB(0xff,0xff,0xff)},  // ��� �������
         {RGB(0x00,0x00,0x00),RGB(0xff,0xff,0xff)},  // ���������
         {RGB(0x00,0x00,0x00),RGB(0xff,0xff,0xff)},  // ��������
         {RGB(0xff,0xff,0xff),RGB(0x00,0x55,0x99)},  // ������
         {RGB(0xff,0xff,0xff),RGB(0xff,0x00,0x00)},  // ����� ��������
      },
      // �������� �����: ������
      {
         {RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00)},  // �������
         {RGB(0xff,0xff,0xff),RGB(0x00,0x00,0x00)},  // �����
         {RGB(0xff,0xff,0xff),RGB(0x00,0x00,0x00)},  // ��� �������
         {RGB(0xff,0xff,0xff),RGB(0x00,0x00,0x00)},  // ���������
         {RGB(0xff,0xff,0xff),RGB(0x00,0x00,0x00)},  // ��������
         {RGB(0xff,0xff,0xff),RGB(0x00,0x77,0x77)},  // ������
         {RGB(0x00,0x00,0x00),RGB(0xff,0x00,0x00)},  // ����� ��������
      },
      // �������� �����: ����������������
      {
         {RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00)},  // �������
         {RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00)},  // �����
         {RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00)},  // ��� �������
         {RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00)},  // ���������
         {RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00)},  // ��������
         {RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00)},  // ������
         {RGB(0x00,0x00,0x00),RGB(0x00,0x00,0x00)},  // ����� ��������
      }
   };

//---------------------------------------------------------------------------
__fastcall TLookOptForm::TLookOptForm(TComponent* Owner)
        : TForm(Owner)
{
  try
  {
    // ��������� ����� �� �����
    TFileStream *fs = new TFileStream(ExtractFileDir(Application->ExeName) + "/color.shm",fmOpenRead);
    fs->ReadBuffer(temp_shemes,sizeof(temp_shemes));
    delete fs;
  }
  catch (Exception &exception)
  {
    // ���� �� ������� ���������, ���������� ����� �� ���������
    memcpy(temp_shemes, default_shemes, sizeof(default_shemes));
  }

  TIniFile *ini;
  ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ));

  font_height = ini->ReadInteger( "Look", "FontHeight", 10 );
  font_family = ini->ReadString( "Look", "FontFamily", "MS Sans Serif");
  color_sheme = ini->ReadString( "Look", "ColorSheme", "�����");

  delete ini;

  /* ��������� �������� ���������� */
  BitBtn1Click(NULL);
}

/* ������ ������ ����� �������� */
void __fastcall TLookOptForm::Button1Click(TObject *Sender)
{
  int i = ColorShemeComboBox->ItemIndex;        // ����� �����
  int e = ElementListBox->ItemIndex;            // ����� ��������

  // ������ �������� ���� �� ��������� �����
  if(ColorDialog1->Execute()) {
    temp_shemes[i][e].ink = ColorDialog1->Color;
    Shape1->Brush->Color = ColorDialog1->Color;
  }
}

/* ������ �� ������ �������� ����, ��� � �� ������ "�������" */
void __fastcall TLookOptForm::Shape1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (Button == mbLeft) Button1Click(NULL);
}

/* ������ ������ ���� �������� */
void __fastcall TLookOptForm::Button2Click(TObject *Sender)
{
  int i = ColorShemeComboBox->ItemIndex;        // ����� �����
  int e = ElementListBox->ItemIndex;            // ����� ��������

  if(ColorDialog2->Execute()) {
    temp_shemes[i][e].paper = ColorDialog2->Color;
    Shape2->Brush->Color = ColorDialog2->Color;
  }
}

/* ������ �� ������ �������� ����, ��� � �� ������ "�������" */
void __fastcall TLookOptForm::Shape2MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (Button == mbLeft) Button2Click(NULL);
}

/* ������� ��������� ������ ��� ������������ ������� */
int CALLBACK EnumFontFamProc(
    ENUMLOGFONT FAR *lpelf,	// pointer to logical-font data
    NEWTEXTMETRIC FAR *lpntm,	// pointer to physical-font data
    int FontType,	// type of font
    LPARAM lParam 	// address of application-defined data
   )
{
  String name = (char*)lpelf->elfLogFont.lfFaceName;
  LookOptForm->FontListBox->Items->Add(name);

  return 1; // ���������� ������������
}

/* ����� �����. ���������, ������������ � ���� ������
   ������� �� ������ gui, � �� ��, ��� ���������� ����
   ����������, �.�. ��� ���� ����� ������ ������, � �� ��
 */
void __fastcall TLookOptForm::FormShow(TObject *Sender)
{
  // ����������� �������� ���� (�� ������, ���� ����� ������ ������
  // � �� ���� ��������� ���������
  memcpy(temp_shemes, gui_color, sizeof(gui_color));

  /* ������� ������ ������� */
  FontListBox->Clear();

  /* ������ � ������� ���� ��������� ������� */
  EnumFontFamilies(
    (void*)(ExemplLabel->Canvas->Handle),
    NULL,	// pointer to family-name string
    (FONTENUMPROC)EnumFontFamProc,	// pointer to callback function
    NULL 	// address of application-supplied data
   );

  /* ��������� � ���� ���������� �� ������ gui */
  FontSizeComboBox->ItemIndex = gui_font_size - 7;   // 7 - ���������� �����, ��� �������� ������ � ������� FontSizeComboBox
  FontListBox->ItemIndex = FontListBox->Items->IndexOf(gui_font_family);
  ColorShemeComboBox->ItemIndex = gui_sheme;
  if (ElementListBox->ItemIndex <0 ) ElementListBox->ItemIndex = 0;

  /* ����������� �������� */
  FontListBoxClick(NULL);
  ElementListBoxClick(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TLookOptForm::FontListBoxClick(TObject *Sender)
{
  font_height = StrToInt(FontSizeComboBox->Text);

  // ���� ���-�� ������� � ������ ��������� �������
  if ( FontListBox->ItemIndex >= 0)
    font_family = FontListBox->Items->Strings[FontListBox->ItemIndex];

  ExemplLabel->Font->Size = font_height;
  ExemplLabel->Font->Name = font_family;
}
//---------------------------------------------------------------------------

/* ����� �������� ����� �� ����������� ������ */
void __fastcall TLookOptForm::ColorShemeComboBoxChange(TObject *Sender)
{
  int i = ColorShemeComboBox->ItemIndex;
  color_sheme = ColorShemeComboBox->Items->Strings[i];
  ElementListBoxClick(NULL);
}

/* ������ �� ������ ��������� (������, �����, ���������...)
   ��������� ����� ���������, ������������ ���� ������ � ����.
 */
void __fastcall TLookOptForm::ElementListBoxClick(TObject *Sender)
{
  int i = ColorShemeComboBox->ItemIndex;        // ����� �������� �����
  int e = ElementListBox->ItemIndex;            // ����� ��������� ��������

  if (i<0 || e<0) return;

  // ���������� �������� � ����� ��������� ��������
  Shape1->Brush->Color = (TColor)temp_shemes[i][e].ink;
  Shape2->Brush->Color = (TColor)temp_shemes[i][e].paper;
}

/* ������ ��, �������� �������� ��������� � ������ gui */
void __fastcall TLookOptForm::BitBtn1Click(TObject *Sender)
{
  int i = ColorShemeComboBox->Items->IndexOf(color_sheme);
  if (i<0) i=0;
  /* ��������� �������� ���������� */
  gui_font_size = font_height;
  gui_font_family = font_family.c_str();
  gui_sheme = i;

  // ���������� ��������� �������� �����
  memcpy(gui_color, temp_shemes, sizeof(gui_color));
}

/* �������� �����, ���������� ���������� */
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
  // �������� ����� �� ��������� � �������
  memcpy(temp_shemes, default_shemes, sizeof(default_shemes));
}
//---------------------------------------------------------------------------



