
     ������ ������� ��� Borland Builder C++ 6

��� �������� ����������� ������� ��������� ��� ���������-��������� i8080.



 ��������:

� ���� ����� ��������� ����� ������� ��� ����������, ������� �������� 
�������� ��� ������� ����������. ��� �������� ������������ ���������� 
�� ������ ����� ���� ��� ���������� ����� �������� ��� ������� �������������,
������ � ������ �� ������, ���� ��������, ���� �����.



��������� ����������:

1. �������� ����� ��� ������� ������ ���������� � �������� plugins
   (�.�. � ��� �� �����, ��� ���� ����� speaker, mbr, templ)

2. �������� ������ ������� ���������� (plugins/templ/templ.bpr)
   � Builder C++ 6 � �������� ������� File->Save Project As...
   ��������� ������ � ������ ��� �������� ����� ��� ������ ���������� ���
   ����� ������, ��� ����� ���������� ���� ���������� (��������, my_device).
   ��� ����� ������� ���������� ����������� ��������, ����� �������������� 
   ������ ��� ���������� ������� ��������� templ.dll.

3. ������������� �� main.cpp (���� �� ��� �� ������������) � �������� �������
   File->Save As...  � ������� ������ "���������", ��� ����� �������� main.cpp 
   � ����� ��� ������ �������.

4. ������������� �� CfgForm.cpp (���� View->Units...), ��������� �������
   File->Save As... � ������� "���������".

5. ��������� � main.cpp � �������� ������� ����� ������ � ������� ����� ����������,
   ������ � ��������� � ��������� ������ �� ����:

      char description[] = "Plugin template (templ.dll)";
      char DEVICE_NAME[] = "Plugin_template";
      int device_port = 0xc3;

6. ���������������� ��� ������ ���������� � ��������:

   reset() - ����� ���������� (���������� ��� ������ ����������� �������)

   BYTE read(WORD por) - ������ �� �����. � �������� ��������� port ���������� ����� �����,
      �� �������� ����������� ������ (���� �������� �������, ����� ���������� ����������
      ����� � ���������� ������, ���������� ������).
      �� ������ ������� ������ ���������� ���� ������, ���������� �� �����.

   void write(WORD port, BYTE data) - ������ � ����. �������� port �������� ����� �����,
      �� �������� ������������ ������ (���������� ������� read) � �������� data �������� ����
      ������, ������������ � ����.


   int __export DeviceInit(struct plugin_device *dev) - ������������� ����������.
      ��� ������� ��������� ��� ������ �� ������������� ���������� � ������ ������� TRUE
      (�� ����) ���� ������������� ������ ������� � FALSE (����), ���� ������������� ���������� 
      ����������. � �������� ��������� � ������� ���������� ����� ���������, ����������� �������
      ����������. ��� ������� ������ ����� ��������� ��������� ���� � ���� ���������:
  
      dev->descr - ��������� �� ������ � ��������� ���������� (��� ������ ������������ � ����
         ������ ���������);

      dev->mask - ����� �����. ���� ���������� ���������� � ������ �����, �� �������� ����� ������ 
         ���� 0xff (���������� 255, �������� 11111111), �.�. ��� ��������, ��������� �� ����� ����� 
         � ������� ����������, ����������� ����� ��� ����. 
         ����, ��������, ���������� ���������� ������ �� ���������� ��������� ������� (��������, �������
         ����� ������ ����� � �������� ���� (������� 6 ���) �����, ��������, 10110, � ������� ����� ���� 
         ������, �� ����� ������ ���� 11111000 (� �������� ����). �.�. ��� ��������� �������������� ������
         � ������� ����� ���������� ����������� ����� ������ ������� 5 �����, � ��� ������� ����� ��������
         � ����.

      dev->port - ����� ����� ����������.
         (���� � ���������� ��������� �������, ���������� ������, �� ��� ��������
         ������ ��������� ���� �� ����� ����������� �����, �.�. ��� ����������� �������
         ��� �������� ������ ���� 10110000

      dev->read - ��������� �� ������� ������ �� �����

      dev->write - ��������� �� ������� ������ � ����

      dev->reset - ��������� �� ������� ������ ����������

      dev->config - ��������� �� ������� ������ ���� �������� ����������
         (���� � ���������� ��� ���� ��������, �� ����� ��������� ����� ��������� NULL)


7. ����� ������� ���������� ������ ���� � ����� /plugins 
   ������ ��������� ���� my_device.dll (��� my_device-��������, ��� ������� �� ��������� ��� ������),
   ������� � �������� ���������� ������ ����������.

��� ������� ��������� ���� dll ����� ������������� ���������. ���� �������� ������� �������
(�.�. ���� ������� ������������� ������ true), ���� ���������� ��������� � ������ ��������� 
(������� ���� "���������->������ ������� ���������...")


2003 Sashnov Alexander
<asashnov@rambler.ru>
https://asashnov.github.io/
