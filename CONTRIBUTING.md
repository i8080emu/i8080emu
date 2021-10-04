Currently Qt port is in progress.

Notes below are about old Borland C++ version and Microsoft Help file
compilation. In Qt Port plain HTML will be used.


The below is left here for historical reasons,
need to remove when we release Qt port.

Для компиляции программы и плагинов требуется Borland C++ Builder 6.
При компиляции программы нужно убедиться что:

* CodeGuard отключен (лишний размер)
* проверить что линковка компонент от Borland статическая

Для компиляции справочной системы нужен Microsoft Help Work Shop.

Найти его можно по адресу www.realsoft.ru/lib/ms_help_workshop.zip,
также он распространяется вместе с популярными средами программирования
под windows- вместе с Microsoft Visual Studio и Borland
(в Builder C++ он находится в каталоге $BCB\help\tools ).
Помимо этого потребуется текстовый процессор для создания файла в формате
RTF (rich-text format), Word от все той же Microsoft вполне подойдет.

Для компиляции файла справки откройте Microsoft Help Workshop и выберите
команду меню File->Open и выберите i8080.hpj
Далее выберите команду меню File->Compile и в появившемся диалоге нажмите
кнопку Compile. Будет показано окно с сообщениями, возникшими в процессе
компиляции и если ошибок нет, будет создан i8080.hlp

Исчерпывающая информация о создании файлов справки находится
в справке самого Help Workshop.

Также есть небольшая статья по этому поводу на русском языке в
газете "Компьютерные Вести" No.10, 2003 год.
( http://www.kv.minsk.by/index2003101103.htm )
