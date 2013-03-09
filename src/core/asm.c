#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>

#include "asm.h"


/* 0 - no debug, 3 - max level */
extern int DEBUG_LEVEL;

/* Размер буфера для считывания чисел и меток */
const int BUFSIZE = 16;


/* Look also asmerr_t in asm.h */
static char *error_text[] = {
  "Нет ошибки, все ОК",
  "Недопустимая метка",
  "Несуществующая мнемоника",
  "Недопустимый аргумент",
  "Директива ORG не может быть использована здесь",
  "В одинарных кавычках должен быть записан один печатный символ",
  "Неверные аргументы директивы DB",
  "Метка с таким именем уже использована",
  "Встречены недопустимые символы",
  "Метка не найдена",
  "Значение превышает 255",
  "Лишний аргумент",
  "Синтаксическая ошибка",
  "Двойная ковычка не закрыта"
};


/* ======== i8080 INSTRUCTIONS ========= */

typedef enum
{
  OP_NONE,
  OP_BYTE,
  OP_WORD,
  OP_REG,
  OP_REGPAIR,
  OP_REGPAIR2,
  OP_CCC,
  OP_BD,
  OP_RSTN,
}
operand_t;

char *op_regs[] = { "B", "C", "D", "E", "H", "L", "M", "A", NULL };
char *op_regpairs[] = { "B", "D", "H", "SP", NULL };
char *op_regpairs2[] = { "B", "D", "H", "PSW", NULL };
char *op_bd[] = { "B", "D", NULL };
char *op_CCC[] = { "NZ", "Z", "NC", "C", "PO", "PE", "P", "M", NULL };
char *op_rstn[] = { "0", "1", "2", "3", "4", "5", "6", "7", NULL };

typedef struct
{
  char *mnem;			/* мнемоника */
  int len;			/* длинна команды (0 для директив асемблера) */
  operand_t op1;		/* тип первого операнда */
  operand_t op2;		/* тип второго операнд */
  BYTE opcode;			/* код команды (если в коде есть биты,
				   обозначающие операнды, то они должны быть равны нулю */
}
OPS;

/* полный перечень команд К580 и директивы ассемблера */
OPS comands[] = {

  /* директивы. Их порядок используется в asm_process(),
     все аргументы из таблицы игнорируются) */

  {"ORG", 0, OP_NONE, OP_NONE, 0x00},
  {"EQU", 0, OP_NONE, OP_NONE, 0x00},
  {"DB", 0, OP_NONE, OP_NONE, 0x00},
  {"DW", 0, OP_NONE, OP_NONE, 0x00},

  /* коды К580, в порядке приложения 2 методического пособия
     "Основы микропроцессорной техники"
     Примечание: если OP_REG- первый аргумент, это DDD (куда)
     если второй- SSS (откуда) */

  {"MOV", 1, OP_REG, OP_REG, 0x40},
  {"PCHL", 1, OP_NONE, OP_NONE, 0xe9},
  {"SPHL", 1, OP_NONE, OP_NONE, 0xf9},
  {"XCHG", 1, OP_NONE, OP_NONE, 0xeb},
  {"XTHL", 1, OP_NONE, OP_NONE, 0xe3},
  {"MVI", 2, OP_REG, OP_BYTE, 0x06},
  {"LXI", 3, OP_REGPAIR, OP_WORD, 0x01},
  {"PUSH", 1, OP_REGPAIR2, OP_NONE, 0xc5},
  {"POP", 1, OP_REGPAIR2, OP_NONE, 0xc1},
  {"STA", 3, OP_NONE, OP_WORD, 0x32},
  {"LDA", 3, OP_NONE, OP_WORD, 0x3a},
  {"STAX", 1, OP_BD, OP_NONE, 0x02},
  {"LDAX", 1, OP_BD, OP_NONE, 0x0a},
  {"SHLD", 3, OP_NONE, OP_WORD, 0x22},
  {"LHLD", 3, OP_NONE, OP_WORD, 0x2a},
  {"ADD", 1, OP_NONE, OP_REG, 0x80},
  {"ADI", 2, OP_NONE, OP_BYTE, 0xc6},
  {"ADC", 1, OP_NONE, OP_REG, 0x88},
  {"ACI", 2, OP_NONE, OP_BYTE, 0xce},
  {"SUB", 1, OP_NONE, OP_REG, 0x90},
  {"SUI", 2, OP_NONE, OP_BYTE, 0xd6},
  {"SBB", 1, OP_NONE, OP_REG, 0x98},
  {"SBI", 2, OP_NONE, OP_BYTE, 0xde},
  {"DAA", 1, OP_NONE, OP_NONE, 0x27},
  {"CMP", 1, OP_NONE, OP_REG, 0xb8},
  {"CPI", 2, OP_NONE, OP_BYTE, 0xfe},
  {"DAD", 1, OP_REGPAIR, OP_NONE, 0x09},
  {"ANA", 1, OP_NONE, OP_REG, 0xa0},
  {"ANI", 2, OP_NONE, OP_BYTE, 0xe6},
  {"ORA", 1, OP_NONE, OP_REG, 0xb0},
  {"ORI", 2, OP_NONE, OP_BYTE, 0xf6},
  {"XRA", 1, OP_NONE, OP_REG, 0xa8},
  {"XRI", 2, OP_NONE, OP_BYTE, 0xee},
  {"RLC", 1, OP_NONE, OP_NONE, 0x07},
  {"RRC", 1, OP_NONE, OP_NONE, 0x0f},
  {"RAL", 1, OP_NONE, OP_NONE, 0x17},
  {"RAR", 1, OP_NONE, OP_NONE, 0x1f},
  {"CMA", 1, OP_NONE, OP_NONE, 0x2f},
  {"CMC", 1, OP_NONE, OP_NONE, 0x3f},
  {"STC", 1, OP_NONE, OP_NONE, 0x37},
  {"INR", 1, OP_REG, OP_NONE, 0x04},
  {"INX", 1, OP_REGPAIR, OP_NONE, 0x03},
  {"DCR", 1, OP_REG, OP_NONE, 0x05},
  {"DCX", 1, OP_REGPAIR, OP_NONE, 0x0b},
  {"JMP", 3, OP_NONE, OP_WORD, 0xc3},
  {"J", 3, OP_CCC, OP_WORD, 0xc2},
  {"CALL", 3, OP_NONE, OP_WORD, 0xcd},
  {"C", 3, OP_CCC, OP_WORD, 0xc4},
  {"RET", 1, OP_NONE, OP_NONE, 0xc9},
  {"R", 1, OP_CCC, OP_NONE, 0xc0},
  {"IN", 2, OP_NONE, OP_BYTE, 0xdb},
  {"OUT", 2, OP_NONE, OP_BYTE, 0xd3},
  {"NOP", 1, OP_NONE, OP_NONE, 0x00},
  {"EI", 1, OP_NONE, OP_NONE, 0xfb},
  {"DI", 1, OP_NONE, OP_NONE, 0xf3},
  {"HLT", 1, OP_NONE, OP_NONE, 0x76},
  {"RST", 1, OP_RSTN, OP_NONE, 0xc7},
  {NULL, 0, OP_NONE, OP_NONE, 0x00}
};


void
DEBUG (int level, const char *format, ...)
{
  va_list p;
  va_start (p, format);

  if (DEBUG_LEVEL >= level)
    {
      fprintf (stderr, "i8080asm: DEBUG: ");
      vfprintf (stderr, format, p);
    }

  va_end (p);
}

int label_getvalue (ASMSESSION * asmsess, char *label);



/* ======== FUNCTIONS ========== */

static int scan_mnem (const char *s, int *ccc_index);

/* Get text of error */
char *
asm_get_error_text (asmerr_t errcode)
{
  return error_text[errcode];
}


/* Wrapper: put byte in virtual memory */
void
put_in_mem (ASMSESSION * asmsess, WORD addr, BYTE value)
{
  if (asmsess->codegen)
    asmsess->mem[addr] = value;
}


/* Функция prepare_line
   Предварительная подготовка строки перед ассемблированием.
   Изменяет исходную строку, делая следующие изменения:
     * удаляет лишние пробелы, заменяя их одним символом пробела где нужно и 
       удаляет пробелы где они не нужны
     * пробелы до и после запятой убираются
     * преобразует маленькие английские буквы в большие
     * отбрасывает пробелы и комментарии в конце строки
     * строки в двойных кавычках не затрагиваются
   Возвращает указатель на результирующую строку (тоже, что и на входе)
   Т.е. строки вида:
      loop:   mvi A,0xab    ; загружаем в рег. А число 171
      db 12, 0x12  ,  "Text ""text"" in quotes"  ; текст и коды
   будут преобразованы в строки:
      LOOP:     MVI     A,0XAB
      DB        12,0X12,"Text ""text"" in quotes"
 */

asmerr_t
prepare_line (char *source_line)
{
  char *in, *out;
  int space, drop_spaces;
  in = out = source_line;

  space = 0;
  drop_spaces = 1;

  while (1)
    {

      // конец строки или комментарий завершают строку
      if (*in == '\n' || *in == ';' || *in == '\0')
	{
	  *out = '\0';
	  break;
	}

      // пропускаем пробелы
      if (isspace (*in))
	{
	  in++;
	  space = 1;
	  continue;
	}

      // если встречена запятая, ':' или знак арифметического действия, 
      // то пробелов до и после не должно быть
      if (strchr (":,+-*/%&|^", *in) != NULL)
	{
	  drop_spaces = 1;
	  *out++ = *in++;
	  while (isspace (*in))
	    in++;
	  continue;
	}

      /*если установлен флаг опбрасывания пробелов, делаем это */
      if (drop_spaces)
	{
	  drop_spaces = 0;
	  space = 0;
	}

      /* вставляем один пробел вместо многих (если были) */
      if (space)
	{
	  space = 0;
	  *out++ = ' ';
	}

      /* если встречена строка в двойных кавычках, то копируется без изменений */
      if (*in == '"')
	{
	  *out++ = *in++;	// копируем символ "

	  while (1)
	    {

	      // если встречен конец строки, это ошибка
	      if (*in == '\0')
		{
		  return ASMERR_NOT_CLOSED_QUOTE;
		}

	      // две двойные кавычки подряд обозначают ", пропускаем
	      if (*in == '"' && *(in + 1) == '"')
		{
		  *out++ = *in++;
		  *out++ = *in++;
		  continue;
		}

	      // одиночная " означает конец строки в кавычках
	      if (*in == '"')
		{
		  *out++ = *in++;
		  break;
		}

	      // все остальные символы копируются без изменений
	      *out++ = *in++;
	    }
	  continue;
	}			// enf if ""

      /* любой другой символ копируется с преобразованием в верхний регистр */
      if (!isascii (*in))
	{
	  DEBUG (3, "prepare_line badsymbol %d\n", (int) *in);
	  return ASMERR_BADSYM;
	}

      *out++ = toupper (*in++);
    }

  return ASMERR_NOERROR;
}



/* ======= LABELS =========== */

/* Проверка, может ли этот символ учавствовать в имени метки.
   Возвращает true если может, false если нет */
static int
is_valid_label_char (const char c)
{
  /* разрешены цифры */
  if (isdigit (c))
    return 1;
  /* буквы */
  if (isalpha (c))
    return 1;
  /* знак подчеркивания */
  if (c == '_')
    return 1;

  /* остальные - нет */
  return 0;
}


/* Проверяет строку на допустимость быть меткой.
   Если метка допустимая, возвращается 1, иначе 0

   Метка может состоять из букв, цифр и подчеркивания
   с необязательным символом ':' в конце, первая буква,
   не длиннее MAXLABELCHARS и не должна
   совпадать ни с одной из мнемоник.

 */
static int
is_valid_label (char *label)
{
  int n;

  DEBUG (2, "is_valid_label: check label=%s\n", label);

  if (label == NULL)
    return 0;

  if (!isalpha (label[0]))	// первой должна быть буква
    return 0;

  // пропускаем все следующие буквы, цифры и подчеркивания
  for (n = 0; is_valid_label_char (label[n]); n++)
    ;
  if (n > MAXLABELCHARS)	// если слишком длинная, нельзя
    return 0;
  if (label[n] != '\0')		// если на этом строка не завершилась, нельзя
    return 0;

  DEBUG (3, "is_valid_label: check chars OK, check for mnemonic...\n");

  /* проверяем, не является ли мнемоникой */
  if (scan_mnem (label, NULL) == (-1))
    {
      DEBUG (3, "is_valid_label: mnemonic not found, it valid label\n");
      return 1;			// если такой мнемоники нет, то может быть метко
    }
  else
    {
      DEBUG (3, "is_valid_label: mnemonic found, it not valid label\n");
      return 0;
    }
}


/* Add label in list.
   Если метка добавлена, возвращает 1
   В случае неудачи возвращает 0
 */
int
label_add (ASMSESSION * asmsess, char *label, WORD value)
{
  label_t *newl;

  DEBUG (1, "label_add: label=%s value=%d\n", label, (int) value);

  /* если метка уже есть, возвращаем 0 */
  if (label_getvalue (asmsess, label) != (-1))
    {
      DEBUG (2, "label_add: label already exist, return error code\n");
      return 0;
    }

  /* выделяем память и добавляем новый элемент в список */
  newl = (label_t *) malloc (sizeof (label_t));
  strncpy (newl->name, label, MAXLABELCHARS);
  newl->value = value;

  /* добавляем в начало списка (так быстрее, чем в конец- конец еще нужно найти) */
  newl->next = asmsess->labels;
  asmsess->labels = newl;

  DEBUG (3, "label_add: label sussefuly added\n");

  return 1;
}

/* Взять значение метки.
  Если метка найдена, то возвращается ее значение (от 0x0000 до 0xffff)
  Если не найдена, возвращает -1
 */
int
label_getvalue (ASMSESSION * asmsess, char *label)
{
  label_t *find;
//  char buf[MAXLABELCHARS + 1];
//  int n;

  if (label == NULL)
    {
      DEBUG (1, "label_getvalue: error: label=NULL\n");
      return -1;
    }

//  // copy text label
//  for (n=0; is_valid_label_char(*label) && n<MAXLABELCHARS; n++)
//    buf[n] = *label++;

  // terminator :-)
//  buf[n] = '\0';

  // find label in table
  for (find = asmsess->labels; find != NULL; find = find->next)
    {
      if (strncmp (label, find->name, MAXLABELCHARS) == 0)
	return find->value;
    }
  return -1;
}

/* Освобождение списка меток */
void
label_free_all (ASMSESSION * asmsess)
{
  label_t *next;

  for (; asmsess->labels != NULL; asmsess->labels = next)
    {
      next = asmsess->labels->next;
      free (asmsess->labels);
    }
}



/*  ====== ARGUMENTS ======= */


// Эта функция нужна !!! (в новой нотации)


/* Возвращает урезаную строку с полем.
   Переданый указатель на начало поля модифицирует,
   переводя его на конец или приравнивая NULL
   если достигнут конец строки.
 */
static char *
get_field (char **line, char *delimiter)
{
  char *field, *p;

  if (*line == NULL)
    return NULL;

  // должно начинаться сразу
  field = *line;

  // find end of field
  for (p = *line; *p != '\0' && is_valid_label_char (*p); p++)
    ;

  // запоминаем последний символ (делимитер)
  *delimiter = *p;

  // terminate field
  *p++ = '\0';

  // next field
  *line = (*delimiter == '\0') ? NULL : p;

  return field;
}



/* Ищет строку в таблице мнемоник и директив ассемблера.
 * Для мнемоник, содержащих условие CCC возвращает еще и его номер
 * через указатель ccc_index
 * Если строка не найдена, возвращает -1
 */
static int
scan_mnem (const char *s, int *ccc_index)
{
  int i;

  DEBUG (2, "scan_mnem: find '%s' in table\n", s);

  if (s == NULL)
    return -1;

  // search in mnemonic and directives table
  for (i = 0; comands[i].mnem != NULL; i++)
    {
      /* если это не условная команда (не содержит CCC в мнемонике) */
      if (comands[i].op1 != OP_CCC)
	{
	  if (strcmp (comands[i].mnem, s) == 0)	/* то просто делаем посимвольную проверку */
	    return i;
	}
      else
	{
	  /* если в мнемонике содержится условие CCC, то проверяем все возможные окончания */
	  int n;
	  char buf[BUFSIZE];
	  for (n = 0; op_CCC[n] != NULL; n++)
	    {
	      sprintf (buf, "%s%s", comands[i].mnem, op_CCC[n]);
	      if (strcmp (s, buf) == 0)
		{
		  if (ccc_index == NULL)
		    DEBUG (1, "scan_mnem: fatal error: *ccc_index=NULL\n");
		  else
		    {
		      if (ccc_index != NULL)
			*ccc_index = n;
		    }
		  return i;
		}
	    }
	}
    }

  return -1;
}




/* Считывание шестнадцатиричного или десятичного числа.
   Десятичные пишутся как есть, а шестнадцатиричные начиная с '0x' как в С.
   Возвращает число из диапазона 0...65535 или -1 в случае ошибки.
 */
int
scan_word (const char *cp)
{
  int ret;
  char *sc, *p;
  int res;


// TODO: rewrite with buf[BUFSIZE]

  if (cp == NULL)
    return -1;

  // делаем копию строки и ограничиваем ее концом числа
  sc = strdup (cp);

  /* если это шестнадцатиричное число */
  if (*sc == '0' && *(sc + 1) == 'X')
    {
      /* ограничиваем последней шестнадцатиричной цифрой */
      for (p = sc + 2; isxdigit (*p); p++);
      *p = '\0';

      /* считываем число из получившейся строки и освобождаем ее */
      res = sscanf (sc + 2, "%X", &ret);
    }
  /* иначе рассматриваем как десятичное */
  else
    {
      /* ограничиваем последней десятичной цифрой */
      for (p = sc; isdigit (*p); p++);
      *p = '\0';

      /* считываем число из получившейся строки и освобождаем ее */
      res = sscanf (sc, "%d", &ret);
    }

  free (sc);
  // если scanf вернула 0 или EOF, значит не удалось считать число
  if (res <= 0)
    return -1;
  // проверяем, что получилось
  if (ret >= 0 && ret <= 0xffff)
    return ret;
  return -1;
}


/* Вычисляет значение числового аргумета.
   Параметры:
     asmsess - текущая сессия (для извлечения значения меток)
     arg - указатель на переменную, куда записать результат
     line - указатель на указатель на текущее положение в строке
  Возвращает ASMERR_NOERROR если все хорошо или код ошибки.
  Функция модифицирует указатель на текущее место в строке, 
  перемещая его к началу следующего аргумента
  или приравнивая NULL если в строке больше нет аргументов.
  Предполагаем, что строка уже разобрана функцие prepare_line и не
  содержит лишних пробелов и комментарии уже отброшены.
 */
static asmerr_t
get_nummeric_arg (ASMSESSION * asmsess, int *arg, char **line)
{
  char *cp;
  int value, i;
  char buf[BUFSIZE];
  char action;			/* знак арифметического действия */
//  char d;

  /* При первом проходе возвращаем любое число */
  if (!asmsess->codegen)
    {
      DEBUG (3, "get_nummeric_arg: no codegen, *line=%s\n", *line);
      *arg = 0xfa;		/* any number < 255 */
      for (cp = *line; *cp != ',' && *cp != '\0'; cp++)
	;
      if (*cp == '\0')
	*line = NULL;
      else
	*line = ++cp;
      DEBUG (3,
	     "get_nummeric_arg: no codegen, return any number. Next arg pointer=%p\n",
	     *line);
      return ASMERR_NOERROR;
    }

  cp = *line;
  value = 0;
  action = ' ';

  /* считываем числа (метки, $) и арифметические действия до ',' или конца строки */
  while (1)
    {

      DEBUG (2, "get_nummeric_arg loop: action=%c value=%d cp=%s\n", action,
	     value, cp);

      /* считываем метку, число или $ в переменную i */
      if (*cp == '$')
	{
	  i = asmsess->addr;
	  cp++;
	}
      else if (isdigit (*cp))
	{			/* as number */
	  int n;
	  for (n = 0; (isxdigit (*cp) || *cp == 'X') && n < (BUFSIZE - 1);
	       n++)
	    buf[n] = *cp++;
	  buf[n] = '\0';
	  DEBUG (3, "get_nummeric_arg: loop: isxdigit buffer=%s\n", &buf[0]);
	  i = scan_word (&buf[0]);
	  if (i < 0)
	    return ASMERR_BADARG;
	}
      else
	{			/* as label */
	  int n;
	  for (n = 0; is_valid_label_char (*cp) && n < (BUFSIZE - 1);
	       buf[n++] = *cp++);
	  buf[n] = '\0';
	  DEBUG (3, "get_nummeric_arg: loop: label buffer=%s\n", &buf[0]);
	  i = label_getvalue (asmsess, &buf[0]);
	  if (i < 0)
	    return ASMERR_BADLABEL;
	}

      /* value = value 'action' i (делаем нужное действие) */
      switch (action)
	{
	case ' ':		/* если это число было первым */
	  value = i;
	  break;
	case '+':
	  value += i;
	  break;
	case '-':
	  value -= i;
	  break;
	case '*':
	  value *= i;
	  break;
	case '/':
	  value /= i;
	  break;
	case '%':
	  value %= i;
	  break;
	case '&':		/* logical AND */
	  value &= i;
	  break;
	case '|':		/* logical OR */
	  value |= i;
	  break;
	case '^':		/* logical XOR */
	  value ^= i;
	  break;
	default:
	  /* в переменную action было внесено неверное значение */
	  return ASMERR_BADSYNTAX;
	}

      /* если встречена , или конец строки, то выход */
      if (*cp == ',' || *cp == '\0')
	break;
      else
	/* иначе считаем это арифметическим знаком */
	action = *cp++;
    }

  /* если встречена запятая, значит есть следующий аргумент */
  if (*cp == ',')
    cp++;

  /* если встречено две запятых подряд- ошибка */
  if (*cp == ',')
    return ASMERR_BADARG;

  /* если аргументов больше нет, возвращаем NULL в качестве указателя на следующий */
  if (*cp == '\0')
    *line = NULL;
  else
    *line = cp;

  DEBUG (2, "get_nummeric_arg: exit: value=%d\n", value);

  *arg = value;

  return ASMERR_NOERROR;
}





/* ======== ASM PROCESS ========= */




/* Трансляция мнемоники и генерация кода.
   Если мнемоника удачно отранслирована, возвращает ASMERR_NOERROR
   Иначе возвращает код ошибки.
 */
static asmerr_t
parse_mnem (ASMSESSION * asmsess, int mnem_index, int ccc_index, char *line)
{
  int i;
  BYTE opcode;
  int val;
  WORD addr;
  asmerr_t ret;
  char *op, d;

  DEBUG (1, "parse_mnem(index=%d, ccc_index=%d, line=%s)\n", mnem_index,
	 ccc_index, line);

  ret = ASMERR_NOERROR;

  /* если ORG не было, то принимаем ее равной 0 */
  if (asmsess->org < 0)
    asmsess->org = 0;

  /* узнаем текущий адрес ассемблирования */
  addr = asmsess->addr;

  /* берем из таблицы код команды */
  opcode = comands[mnem_index].opcode;

  /* запись в память кода команды */
  put_in_mem (asmsess, addr, opcode);

  // обрабатываем первый аргумент
  DEBUG (3, "parse_mnem: parse arg1...\n");

  switch (comands[mnem_index].op1)
    {
    case OP_NONE:
      DEBUG (3, "parse_mnem: parse arg1: arg1 type NONE, passed\n");
      break;
    case OP_BYTE:
    case OP_WORD:
      fprintf (stderr,
	       "Ошибка в таблице команд ассемблера: строка %d (мнемоника %s): операнды OP_BYTE или OP_WORD не могут быть первым аргументом!",
	       mnem_index, comands[mnem_index].mnem);
      ret = ASMERR_BADARG;
      goto exit;
    case OP_REG:
      // берем первый операнд
      op = get_field (&line, &d);

      DEBUG (3, "parse_mnem: parse arg1: arg1 type REG=%s, next arg=%s\n", op,
	     line);

      for (i = 0;; i++)
	{
	  // find register operand in table
	  if (op_regs[i] == NULL || op == NULL)
	    {
	      ret = ASMERR_BADARG;
	      goto exit;
	    }
	  //
	  if (strcmp (op_regs[i], op) == 0)
	    {
	      opcode |= i << 3;	// ?? DDD ???
	      put_in_mem (asmsess, addr, opcode);
	      break;
	    }
	}
      break;
    case OP_REGPAIR:
      // берем первый операнд
      op = get_field (&line, &d);

      DEBUG (3, "parse_mnem: parse arg1: arg1 type REGPAIR=%s, next arg=%s\n",
	     op, line);

      for (i = 0;; i++)
	{
	  if (op_regpairs[i] == NULL || op == NULL)
	    {
	      ret = ASMERR_BADARG;
	      goto exit;
	    }
	  if (strcmp (op_regpairs[i], op) == 0)
	    {
	      opcode |= i << 4;	// ?? RR ????
	      put_in_mem (asmsess, addr, opcode);
	      break;
	    }
	}
      break;
    case OP_REGPAIR2:
      // берем первый операнд
      op = get_field (&line, &d);


      DEBUG (3,
	     "parse_mnem: parse arg1: arg1 type REGPAIR2=%s, next arg=%s\n",
	     op, line);

      for (i = 0;; i++)
	{
	  if (op_regpairs2[i] == NULL || op == NULL)
	    {
	      ret = ASMERR_BADARG;
	      goto exit;
	    }
	  if (strcmp (op_regpairs2[i], op) == 0)
	    {
	      opcode |= i << 4;	// ?? RR ????
	      put_in_mem (asmsess, addr, opcode);
	      break;
	    }
	}
      break;

    case OP_CCC:

      DEBUG (3, "parse_mnem: parse arg1: arg1 type CCC=%d, next arg=%s\n",
	     ccc_index, line);

      if (ccc_index > 7)
	{
	  ret = ASMERR_BADARG;
	  goto exit;
	}
      opcode |= ccc_index << 3;	// ?? CCC ???
      put_in_mem (asmsess, addr, opcode);
      break;

    case OP_BD:
      // берем первый операнд
      op = get_field (&line, &d);

      DEBUG (3, "parse_mnem: parse arg1: arg1 type DB=%s, next arg=%s\n", op,
	     line);

      for (i = 0;; i++)
	{
	  if (op_bd[i] == NULL || op == NULL)
	    {
	      ret = ASMERR_BADARG;
	      goto exit;
	    }
	  if (strcmp (op_bd[i], op) == 0)
	    {
	      opcode |= i << 4;	// ??? [DB] ????
	      put_in_mem (asmsess, addr, opcode);
	      break;
	    }
	}
      // помечаем, что операнд уже использован
      op = NULL;
      break;

    case OP_RSTN:
      // берем первый операнд
      op = get_field (&line, &d);

      DEBUG (3, "parse_mnem: parse arg1: arg1 type RST_N=%s, next arg=%s\n",
	     op, line);

      for (i = 0;; i++)
	{
	  if (op_rstn[i] == NULL || op == NULL)
	    {
	      ret = ASMERR_BADARG;
	      goto exit;
	    }
	  if (strcmp (op_rstn[i], op) == 0)
	    {
	      opcode |= i << 3;	// 11 NNN 111
	      put_in_mem (asmsess, addr, opcode);
	      break;
	    }
	}
    }

  // обрабатываем второй аргумент
  DEBUG (3, "parse_mnem: parse arg2....\n");
  switch (comands[mnem_index].op2)
    {
    case OP_NONE:
      DEBUG (3, "parse_mnem: parse arg2: arg2 type NONE, passed\n");
      break;

    case OP_BYTE:

      ret = get_nummeric_arg (asmsess, &val, &line);

      DEBUG (3, "parse_mnem: parse arg2: arg2 type BYTE=%d, next arg=%s\n",
	     val, line);

      if (ret != ASMERR_NOERROR
	  && (asmsess->codegen && ret == ASMERR_BADLABEL))
	goto exit;

      // значение для байта не должно превышать 255
      if (val > 255)
	{
	  ret = ASMERR_TOOBIG;
	  goto exit;
	}
      /* увеличиваем счетчик адреса */
      addr++;
      /* */
      if (asmsess->codegen)
	{
	  // записываем его в память, если второй проход
	  put_in_mem (asmsess, addr, (BYTE) val);
	}
      break;

    case OP_WORD:
      ret = get_nummeric_arg (asmsess, &val, &line);

      DEBUG (3, "parse_mnem: parse arg2: arg2 type WORD=%d, next arg=%s\n",
	     val, line);

      if (ret != ASMERR_NOERROR
	  && (asmsess->codegen && ret == ASMERR_BADLABEL))
	goto exit;

      /* записываем сначала младший байт, потом старший */
      put_in_mem (asmsess, ++addr, (WORD) val & 0x00ff);
      put_in_mem (asmsess, ++addr, (WORD) val >> 8);
      break;

    case OP_REG:

      op = get_field (&line, &d);
      DEBUG (3, "parse_mnem: parse arg2: arg2 type REG=%s, next arg=%s\n", op,
	     line);
      for (i = 0;; i++)
	{
	  if (op_regs[i] == NULL || op == NULL)
	    {
	      ret = ASMERR_BADARG;
	      goto exit;
	    }
	  if (strcmp (op_regs[i], op) == 0)
	    {
	      opcode |= i;	// SSS
	      put_in_mem (asmsess, addr, opcode);
	      break;
	    }
	}
      break;

    case OP_REGPAIR:		// вторым аргументом может быть только байт,
    case OP_REGPAIR2:		// регистр или слово.
    case OP_CCC:		// Если что-то другое, то это ошибка
    case OP_BD:		// в таблице команд
    case OP_RSTN:
      fprintf (stderr, "Ошибка в таблице команд ассемблера!");
      ret = ASMERR_BADARG;
    }

  /* если еще есть аргументы - они лишние */
  if (line != NULL)
    ret = ASMERR_EXTRAARG;

exit:
  /* увеличиваем счетчик адреса сессии на длину команды */
  asmsess->addr += comands[mnem_index].len;

  return ret;
}





/* Обработка аргументов директивы DB.
   Возвращает код ошибки.
 */
static asmerr_t
parse_db (ASMSESSION * asmsess, char *ops)
{
  WORD addr;
  int val;
  asmerr_t err;

  if (asmsess->org < 0)		// если ORG не было, то принимаем ее равной 0
    asmsess->org = 0;

  // берем текущий адрес ассемблирования
  addr = asmsess->addr;

  DEBUG (1, "parse_db addr=%04x ops=%s\n", addr, ops);

  /* разбираем аргументы */
  while (ops != NULL && *ops != '\0')
    {

      DEBUG (2, "parse_db loop: ops=%s\n", ops);

      /* text in double quotes */
      if (*ops == '"')
	{
	  DEBUG (3, "parse_db loop: enter to parse \"\n");

	  /* просматриваем текст до закрывающей кавычки */
	  for (ops++; *ops != '"' || (*ops == '"' && *(ops + 1) == '"');
	       ops++)
	    {
	      if (*ops == '"' && *(ops + 1) == '"')
		{		/* две "" подряд означают одну " */
		  put_in_mem (asmsess, addr++, '"');
		  ops++;
		}
	      else
		put_in_mem (asmsess, addr++, *ops);
	    }

	  ops++;		// *ops = ',' or \0
	  DEBUG (3, "parse_db loop: after " " parsing: ops=%s\n", ops);
	  continue;

	}
      else
	{
	  err = get_nummeric_arg (asmsess, &val, &ops);
	  if (err != ASMERR_NOERROR)
	    return err;
	  put_in_mem (asmsess, addr++, (BYTE) val);
	  DEBUG (3, "parse_db loop: after get_nummeric_arg: val=%d, ops=%p\n",
		 val, ops);
	}
    }

  /* сохраняем текущий адрес асемблирования 
   * (его нельзя менять во время разбора, т.к. значение $ (текущий адрес) берется
   * из этой переменной) */
  asmsess->addr = addr;

  DEBUG (3, "parse_db exit: addr=%p\n", addr);

  return ASMERR_NOERROR;
}


/* Обработка аргументов директивы DW
   Возвращает ASERR_NOERROR если все хорошо,
   при этом увеличивает текущий адрес ассемблирования
   на длинну аргументов
   При ошибке возвращает ее код
 */
static asmerr_t
parse_dw (ASMSESSION * asmsess, char *ops)
{
  WORD addr;
  int val;
  asmerr_t err;


  DEBUG (1, "parse_dw: ops=%s\n", ops);

  if (asmsess->org < 0)		// если ORG не было, то принимаем ее равной 0
    asmsess->org = 0;

  // берем текущий адрес ассемблирования
  addr = asmsess->addr;

  /* разбираем аргументы */
  while (ops != NULL && *ops != '\0')
    {
      DEBUG (3, "parse_db loop: ops=%s\n", ops);

      err = get_nummeric_arg (asmsess, &val, &ops);
      if (err != ASMERR_NOERROR)
	return err;
      put_in_mem (asmsess, addr++, (WORD) val & 0x00ff);
      put_in_mem (asmsess, addr++, (WORD) val >> 8);

      DEBUG (3, "parse_db loop: after get_nummeric_arg: val=%d, ops=%p\n",
	     val, ops);
    }

  /* сохраняем текущий адрес асемблирования */
  asmsess->addr = addr;

  DEBUG (3, "parse_dw exit: addr=%p\n", addr);

  return ASMERR_NOERROR;
}







/*   ----========= Assembler interface =====--------   */




/* Начало процесса ассемблирование, инициализация структуры */
void
asm_start (ASMSESSION * asmsess, void *virtual_memory)
{
  DEBUG (1, "asm_start()\n");

  // инициализация всех полей структуры
  asmsess->mem = virtual_memory;
  asmsess->addr = 0;
  asmsess->org = -1;
  asmsess->mult_org = 0;
  asmsess->codegen = 0;
  asmsess->prep_only = 0;
  asmsess->labels = NULL;
}



/* Обработка одной строки (главная функция ассемблирования)
   Входные параметры:
     asmsess - указатель на структуру ASMSESSION
     line - указатель на строку исходного кода
   Возвращает значение ASMERR_NOERROR если все прошло удачно
     или код ошибки в противном случае.
*/
asmerr_t
asm_process (ASMSESSION * asmsess, char *line)
{
  asmerr_t err;
  char *label = NULL;		// label in this line (for EQU directive)
  WORD label_addr = 0;	// address of label in this line (for EQU directive)
  int i, ccc_i;
  char *field, d;

  DEBUG (1, "asm_process(%s)\n", line);

  err = ASMERR_NOERROR;

  /* предварительная обработка строки */
  if ((err = prepare_line (line)) != ASMERR_NOERROR)
    return err;

  DEBUG (2, "after prepare: %s\n", line);

  /* если только препроцессинг, то выводим строку и выходим */
  if (asmsess->prep_only)
    {
      puts (line);
      return ASMERR_NOERROR;
    }

  if (strlen(line) < 1) {
	  DEBUG (2, "line empty, return.\n");
	        err = ASMERR_NOERROR;
		      goto exit;
		          }



  field = get_field (&line, &d);
  DEBUG (3, "asm_process: field=%s line=%s\n", field, line);

  // TODO: check delimiter for space or :

  /* Если первое поле может быть меткой... */
  if (is_valid_label (field))
    {
      DEBUG (2, "asm_process: IT LABEL!\n");

      // запоминаем строку с меткой
      label = field;
      label_addr = asmsess->addr;

      // считываем следующее поле
      field = get_field (&line, &d);

      DEBUG (2, "asm_process: next field=%s\n", field);

      /* если поле пустое, заканчиваем обработку */
      if (strlen(field) < 1)
	{
	  err = ASMERR_NOERROR;
	  goto exit;
	}
    }


  /* в текущем поле должна быть мнемоника */
  if ((i = scan_mnem (field, &ccc_i)) == -1)
    {
      err = ASMERR_MNEM;
      goto exit;
    }
  else
    {
      // если команда является инструкцией микропроцессора К580
      if (comands[i].len > 0)
	{
	  // трансляция мнемоники и генерация кода (если второй проход)
	  err = parse_mnem (asmsess, i, ccc_i, line);
	  goto exit;
	}
      else
	{
	  // иначе это директива ассемблера
	  switch (i)
	    {
	    case 0:		/* ORG - меняет текущий адрес компиляции */
	      {
		int w;
		/* если org уже определена, устанавливаем флаг нескольких org */
		if (asmsess->org >= 0 && !asmsess->codegen)
		    asmsess->mult_org = 1;
		/* считываем аргумент */
		if ((w = scan_word (line)) == -1)
		  {
		    err = ASMERR_BADARG;
		    goto exit;
		  }
		/* если это первая org, то запоминаем адрес */
		if (!asmsess->mult_org)
		  asmsess->org = w;
		asmsess->addr = w;
	      }
	      break;
	    case 1:		/* EQU - определяет константу */
	      if (!asmsess->codegen)	/* обрабатывается только на первом проходе */
		{
		  int w;
                  /* WORKAROUND: We need turn off asmsess->codegen flag becouse
		   * get_nummeric_arg will calculate this value only in 1-th pass */
		  asmsess->codegen = 1;
		  err = get_nummeric_arg (asmsess, &w, &line);
		  asmsess->codegen = 0;
		  if (err != ASMERR_NOERROR)
		    {
		      label = NULL;
		      goto exit;
		    }
		  label_addr = w;
		  goto exit;
		}
	      break;
	    case 2:		/* DB - задает последовательность байтов */
	      err = parse_db (asmsess, line);
	      break;
	    case 3:		/* DW - задает последовательность слов (16 бит целые) */
	      err = parse_dw (asmsess, line);
	    }
	}
    }

exit:
  /* Если была встречена метка, то во время первого прохода добавляем ее.
     Добавление метки в таблицу происходит в конце на случай, если используется
     директива EQU- тогда метке необходимо присвоить не текущий адрес ассемблирования,
     а результат выражения после директивы.
   */
  if (!asmsess->codegen && label != NULL)
    {
      if (!label_add (asmsess, label, label_addr))
	{
	  err = ASMERR_DUPLABEL;
	}
    }

  return err;
}


/* Начало второго прохода (генерация кода) */
void
asm_pass2 (ASMSESSION * asmsess)
{
  DEBUG (1, "asm_pass2()\n");

  /* присваиваем текущему адресу компиляции первую org */
  asmsess->addr = asmsess->org;
  asmsess->codegen = 1;
}


/* Конец ассемблирования */
void
asm_end (ASMSESSION * asmsess)
{
  DEBUG (1, "asm_end()\n");

  label_free_all (asmsess);
}

