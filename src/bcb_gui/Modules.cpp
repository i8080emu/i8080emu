// ��������-�������� i8080
// ����������� ���������� ������� ������� ���������
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "core/plugin.h"
#include "core/mpsys.h"

#include "Modules.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TModulesForm *ModulesForm;
//---------------------------------------------------------------------------
__fastcall TModulesForm::TModulesForm(TComponent* Owner)
        : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TModulesForm::FormShow(TObject *Sender)
{
  struct plugin_device *dev;

  /* ��������� ������ ������� ��������� */
  ListBox1->Clear();


  for(dev = devices; dev != NULL; dev = dev->next ) {
    ListBox1->Items->AddObject(dev->descr, (System::TObject*)dev);
  }

  /* ��������� ������ �������� */
  PropertyButton->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TModulesForm::PropertyButtonClick(TObject *Sender)
{
  int i;
  struct plugin_device *dev;

  if ((i = ListBox1->ItemIndex) >= 0) {
    /* �������� ��������� �� �������� ���������� */
    dev = (struct plugin_device *)
          ListBox1->Items->Objects[i];
    /* ����� ���� ������� ���������� */
    if (dev->config == NULL)
      ShowMessage("���������� �� ����� ���� �������.");
    else
      dev->config();
  }
}
//---------------------------------------------------------------------------

void __fastcall TModulesForm::ListBox1Click(TObject *Sender)
{
  PropertyButton->Enabled = ListBox1->ItemIndex >= 0;
}
//---------------------------------------------------------------------------

