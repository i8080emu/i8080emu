object CompileForm: TCompileForm
  Left = 369
  Top = 151
  BorderStyle = bsToolWindow
  Caption = #1040#1089#1089#1077#1084#1073#1083#1080#1088#1086#1074#1072#1085#1080#1077' '#1090#1077#1082#1089#1090#1072
  ClientHeight = 376
  ClientWidth = 431
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object BitBtn1: TBitBtn
    Left = 48
    Top = 344
    Width = 75
    Height = 25
    TabOrder = 0
    OnClick = BitBtn1Click
    Kind = bkOK
  end
  object BitBtn2: TBitBtn
    Left = 144
    Top = 344
    Width = 89
    Height = 25
    Caption = #1054#1090#1084#1077#1085#1072
    TabOrder = 1
    OnClick = BitBtn2Click
    Kind = bkCancel
  end
  object BitBtn3: TBitBtn
    Left = 248
    Top = 344
    Width = 89
    Height = 25
    Caption = '&'#1057#1087#1088#1072#1074#1082#1072
    TabOrder = 2
    Kind = bkHelp
  end
  object ListBox1: TListBox
    Left = 8
    Top = 8
    Width = 417
    Height = 321
    ItemHeight = 13
    TabOrder = 3
    OnDblClick = ListBox1DblClick
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = 'blk'
    FileName = 'untitled001'
    Filter = 'Block files (*.blk)|*.blk|All files (*.*)|*.*'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofNoChangeDir, ofEnableSizing]
    Left = 376
    Top = 336
  end
end
