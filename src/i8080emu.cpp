// Эмулятор-отладчик i8080
// Файл описания проекта
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
USEFORM("bcb_gui\Main.cpp", MainForm);
USEFORM("bcb_gui\ViewFrom.cpp", ViewFromForm);
USEFORM("bcb_gui\About.cpp", AboutForm);
USEFORM("bcb_gui\Assembler.cpp", AsmForm);
USEFORM("bcb_gui\SaveBlock.cpp", SaveBlockForm);
USEFORM("bcb_gui\Compile.cpp", CompileForm);
USEFORM("bcb_gui\Disassembler.cpp", DisasForm);
USEFORM("bcb_gui\FindPut.cpp", FindPutForm);
USEFORM("bcb_gui\Goto.cpp", GotoForm);
USEFORM("bcb_gui\LoadBin.cpp", LoadBinForm);
USEFORM("bcb_gui\LoadBlock.cpp", LoadBlockForm);
USEFORM("bcb_gui\LookOpt.cpp", LookOptForm);
USEFORM("bcb_gui\Memory.cpp", MemoryForm);
USEFORM("bcb_gui\Modules.cpp", ModulesForm);
USEFORM("bcb_gui\MpsysOpt.cpp", MPsysOptForm);
USEFORM("bcb_gui\RegFlag.cpp", RegFlagForm);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
        try
        {
                 Application->Initialize();
                 Application->Title = "i8080 emulator";
                 Application->HelpFile = "i8080emu.hlp";
                 Application->CreateForm(__classid(TMainForm), &MainForm);
                 Application->CreateForm(__classid(TViewFromForm), &ViewFromForm);
                 Application->CreateForm(__classid(TAboutForm), &AboutForm);
                 Application->CreateForm(__classid(TAsmForm), &AsmForm);
                 Application->CreateForm(__classid(TSaveBlockForm), &SaveBlockForm);
                 Application->CreateForm(__classid(TCompileForm), &CompileForm);
                 Application->CreateForm(__classid(TDisasForm), &DisasForm);
                 Application->CreateForm(__classid(TFindPutForm), &FindPutForm);
                 Application->CreateForm(__classid(TGotoForm), &GotoForm);
                 Application->CreateForm(__classid(TLoadBinForm), &LoadBinForm);
                 Application->CreateForm(__classid(TLoadBlockForm), &LoadBlockForm);
                 Application->CreateForm(__classid(TLookOptForm), &LookOptForm);
                 Application->CreateForm(__classid(TMemoryForm), &MemoryForm);
                 Application->CreateForm(__classid(TModulesForm), &ModulesForm);
                 Application->CreateForm(__classid(TMPsysOptForm), &MPsysOptForm);
                 Application->CreateForm(__classid(TRegFlagForm), &RegFlagForm);
                 Application->Run();
        }
        catch (Exception &exception)
        {
                 Application->ShowException(&exception);
        }
        return 0;
}
//---------------------------------------------------------------------------
