// Эмулятор-отладчик i8080
// Диалог ввода шестнадцатиричной последовательности
// в виде мнемоники, кодов или текста.
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <string.h>

#include "FindPut.h"
#include "Main.h"
#include "Utils.h"
#include "core/asm.h"
#include "core/gui.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFindPutForm *FindPutForm;
//---------------------------------------------------------------------------
__fastcall TFindPutForm::TFindPutForm(TComponent* Owner)
        : TForm(Owner)
{
  // длина последовательности -1 значит, что ничего нет
  len = -1;
}
//---------------------------------------------------------------------------
void __fastcall TFindPutForm::Button3Click(TObject *Sender)
{
  Close();
}

BYTE get_xdigit_value(char xdigit)
{
  if (xdigit >= '0' && xdigit <= '9')
    return xdigit-'0';
  else
    return xdigit-'A'+0x0a;
}

//---------------------------------------------------------------------------
void __fastcall TFindPutForm::Edit1Change(TObject *Sender)
{
  RadioButton1->Checked = true;
}
//---------------------------------------------------------------------------
void __fastcall TFindPutForm::Edit2Change(TObject *Sender)
{
  RadioButton2->Checked = true;

  // оставляем в поле только шестнадцатиричные цифры
  TEdit *e = (TEdit*)Sender;
  e->Text = GetXdigitOnly(e->Text);
}
//---------------------------------------------------------------------------

void __fastcall TFindPutForm::Edit3Change(TObject *Sender)
{
  RadioButton3->Checked = true;
}
//---------------------------------------------------------------------------

void __fastcall TFindPutForm::FormKeyDown(TObject *Sender, WORD &Key,
      TShiftState Shift)
{
  if (Key == VK_RETURN)
    BitBtn2Click(Sender);

  if (Key == VK_ESCAPE)
    Close();
}
//---------------------------------------------------------------------------

/* Кнопка ОК. Преобразуем введеные данные в кодовую последовательность. */
void __fastcall TFindPutForm::BitBtn2Click(TObject *Sender)
{

  /* Если введена мнемоника */
  if (RadioButton1->Checked) {
    ASMSESSION as;
    asmerr_t asm_err;
    // буфер памяти для ассемблирования
    BYTE membuf[0x10000];

    // очищаем временый буфер памяти
    memset(membuf, 0, sizeof(membuf));

    // инициализируем сессию асемблирования
    asm_start(&as, membuf);

    // включаем генерацию кода
    as.codegen = 1;

    /* копируем введеную с мнемоникой строку */
    char *p = new char[Edit1->Text.Length() + 1];
    strcpy(p, Edit1->Text.c_str());

    // передаем строку на ассемблирование
    asm_err = asm_process(&as, p);

    // если произошла ошибка или введена пустая мнемоника (она будет считаться меткой)
    if (asm_err != ASMERR_NOERROR || as.addr == 0) {
      ShowMessage("Введена неверная мнемоника");
      MessageBeep(0xFFFFFFFF);
      len = -1;
    }
    else {
      // если ассемблирование прошло нормально, записываем результат в буфер
      memcpy(data, membuf, MAX_HEX_SEQUENCE);
      /* устанавливаем длину буфера
         т.к. ассемблирование по умолчанию начиналось с нулевого
         адреса, текущий адрес компиляции и будет являться длинной команды.
       */
      len = as.addr;

      // проверка на превышение максимального размера буфера
      if (len > MAX_HEX_SEQUENCE)
        len = MAX_HEX_SEQUENCE;
    }

    // освобождаем данные сессии
    asm_end(&as);

    // освобождаем копию строки
    delete p;
  }
  else if (RadioButton2->Checked){
    /* шестнадцатиричный поиск, считываение последовательности */
    char *sc;
    int first = 1;
    int n = 0;

    /* копируем строку */
    sc = new char[Edit2->Text.Length() + 1];
    strcpy(sc, Edit2->Text.c_str());

    /* Считываем шестнадцатиричные числа */
    for (char *p = sc; *p != '\0'; p++) {
      if (*p == ' ')
        continue;

      if (n > MAX_HEX_SEQUENCE) {
        ShowMessage("Введеная последовательность длиннее, чем можно использовать для поиска.");
        delete sc;
        return;
      }

      if (first)
        data[n] = get_xdigit_value(*p) << 4;
      else {
        data[n] |= get_xdigit_value(*p);
        n++;
      }

      first = !first;
    }
    // освобождаем память от копии строки
    delete sc;

    // число цифр должно быть четно
    if (!first && n>0) {
      ShowMessage("Число шестнадцатиричных цифр нечетно! Вероятно последнее число введено неполностью.");
      return;
    }

    // устанавливаем длинну введеной последовательности
    len = n;
  }
  else {
    // Введен текст
    /* копируем введеную строку текста */
    char *p = new char[Edit3->Text.Length() + 1];
    strcpy(p, Edit3->Text.c_str());
    int n;

    // в цикле переносим в буфер максимально разрешеное число символов
    for (n = 0; n < MAX_HEX_SEQUENCE; n++)
      if (*(p+n) != '\0')
        data[n] = *(p+n);
      else
        break;

    len = n; // устанавливаем длинну введеной последовательности
    delete p;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFindPutForm::FormShow(TObject *Sender)
{
  Edit1->Text = "";
  Edit2->Text = "";
  Edit3->Text = "";
}
//---------------------------------------------------------------------------


