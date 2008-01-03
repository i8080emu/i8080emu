// Эмулятор-отладчик i8080
// Окно трансляции программы при ассемблировании
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Compile.h"
#include "Assembler.h"
#include <stdio.h>
#include <string.h>

#include "core/mpsys.h"
#include "core/gui.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TCompileForm *CompileForm;
//---------------------------------------------------------------------------
__fastcall TCompileForm::TCompileForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------

// выполнение одного прохода компиляции
void __fastcall TCompileForm::DoPass(ASMSESSION *as)
{
  int n;
  char *line;
  asmerr_t asm_err;

  err_count = 0;

  // последовательно передаем каждую строку для обработки
  for( n=0; n<AsmForm->RichEdit->Lines->Count; n++) {
    // делаем копию строки
    line = strdup(AsmForm->RichEdit->Lines->Strings[n].c_str());
    asm_err = asm_process(as, line);

    if (asm_err != ASMERR_NOERROR) {
      String s;
      // генерируем строку вида line 12: Неверный аргумент
      s.printf("line %d: %s", n+1, asm_get_error_text(asm_err));
      ListBox1->Items->Add(s);
      err_count++;
    }
    free(line);
  }
}
//---------------------------------------------------------------------------

void __fastcall TCompileForm::FormShow(TObject *Sender)
{
  ASMSESSION as;
  String s;

  /*  Компиляция, подготовка */
  AsmForm->RichEdit->ReadOnly = true;    // запрещаем редактирование текста
  BitBtn1->Enabled = false;             // запрещаем кнопку ОК, разрешаем отмену
  ListBox1->Clear();                    // очишаем поле результатов

  // очищаем временый буфер памяти
  memset(membuf, 0, sizeof(membuf));

  // начинаем асемблирование, первый проход
  ListBox1->Items->Add("Проход 1, проверка синтаксиса...");

  // инициализация сессии асемблирования
  asm_start(&as, membuf);

  // делаем первый проход
  DoPass(&as);

  // если был задан только первый проход или если есть ошибки, заканчиваем
  if (CompileMode == cmJustTest || err_count > 0)
    goto end;

  // делаем второй проход
  ListBox1->Items->Add("Проход 2, генерация кода...");
  asm_pass2(&as);
  DoPass(&as);

end:
  // сохраняем данные сессии
  Origin = as.org;
  CodeLen = as.addr-as.org;

  // освобождаем данные сессии
  asm_end(&as);

  // если было несколько директив org предупреждаем об этом
  if (as.mult_org)
    ListBox1->Items->Add("Предупреждение: несколько ORG");

  // выводим результат
  if (err_count > 0)
    s.printf("Неудачное завершение, обнаружено %d ошибки(ок).", err_count);
  else
    s.printf("Готово, программа будет занимать %04X байт(а) с адреса %04X.", CodeLen, Origin);
  ListBox1->Items->Add(s);

  /* Конец ассемблирования */
  AsmForm->RichEdit->ReadOnly = false;  // разрешаем редактирование текста
  BitBtn1->Enabled = true;              // разрешаем кнопку ОК
}
//---------------------------------------------------------------------------

// Кнопка закрытия формы
void __fastcall TCompileForm::BitBtn1Click(TObject *Sender)
{
  // если есть ошибки, то ничего больше не делаем
  if (err_count > 0) {
    Close();
    return;
  }

  // если задана компиляция в память, то записываем временый буфер
  // в виртуальную память системы
  if (CompileMode == cmInMemory)
    memcpy(&MEM[Origin], &membuf[Origin], CodeLen);

  // если задана компиляция в файл, то записываем временый буфер
  // в запрошеный у пользователя файл блока кодов
  if (CompileMode == cmInFile) {
    if (SaveDialog1->Execute())
      gui_file_saveblock(SaveDialog1->FileName.c_str(), Origin, CodeLen);
  }

  Close();
}
//---------------------------------------------------------------------------

// при двойном щелчке на строку с сообщением об ошибке
// в редакторе ассемблера курсор должен установиться на эту строку
void __fastcall TCompileForm::ListBox1DblClick(TObject *Sender)
{
  int i, num, res;
  char *str, *p;

  if ((i = ListBox1->ItemIndex) < 0)
    return;

  String s = ListBox1->Items->Strings[i];
  str = strdup(s.c_str());

  if (strncmp("line ",str, 5) == 0) {
    for( p = str + 5; !isdigit(*p); p++ )
      ;
    *p = '\0';
    res = sscanf(str+5, "%d", &num);
    if (res == 1) {
      // устанавливаем курсор на линию num
      AsmForm->RichEdit->CaretPos.y = num;
      AsmForm->RichEdit->CaretPos.x = 0;
    }
  }

  free(str);
}
//---------------------------------------------------------------------------

void __fastcall TCompileForm::BitBtn2Click(TObject *Sender)
{
  Close();
}
//---------------------------------------------------------------------------

