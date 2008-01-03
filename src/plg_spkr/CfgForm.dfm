object ConfigForm: TConfigForm
  Left = 359
  Top = 224
  BorderStyle = bsDialog
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1091#1089#1090#1088#1086#1081#1089#1090#1074#1072
  ClientHeight = 273
  ClientWidth = 340
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCloseQuery = FormCloseQuery
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 96
    Width = 111
    Height = 13
    Caption = #1040#1076#1088#1077#1089' '#1087#1086#1088#1090#1072' '#1089#1087#1080#1082#1077#1088#1072':'
  end
  object Label3: TLabel
    Left = 48
    Top = 8
    Width = 66
    Height = 24
    Caption = #1057#1087#1080#1082#1077#1088
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 24
    Top = 144
    Width = 221
    Height = 13
    Caption = '('#1101#1090#1086' '#1073#1091#1076#1077#1090' '#1088#1072#1073#1086#1090#1072#1090#1100' '#1074' '#1089#1080#1089#1090#1077#1084#1072#1093' Windows9x)'
  end
  object Label4: TLabel
    Left = 24
    Top = 184
    Width = 132
    Height = 13
    Caption = '('#1076#1083#1103' '#1089#1080#1089#1090#1077#1084' NT, 2000, XP)'
    WordWrap = True
  end
  object Label5: TLabel
    Left = 8
    Top = 40
    Width = 3
    Height = 13
    WordWrap = True
  end
  object Label6: TLabel
    Left = 8
    Top = 40
    Width = 301
    Height = 39
    Caption = 
      #1044#1072#1085#1085#1086#1077' '#1091#1089#1090#1088#1086#1081#1089#1090#1074#1086'  '#1087#1088#1077#1076#1085#1072#1079#1085#1072#1095#1077#1085#1086' '#1076#1083#1103'  '#1087#1086#1076#1072#1095#1080' '#1079#1074#1091#1082#1086#1074#1099#1093'  '#1089#1080#1075#1085#1072#1083#1086#1074',' +
      ' '#1082#1086#1090#1086#1088#1099#1081' '#1092#1086#1088#1084#1080#1088#1091#1077#1090#1089#1103' '#1087#1091#1090#1077#1084' '#1091#1089#1090#1072#1085#1086#1074#1082#1080' '#1080' '#1089#1073#1088#1086#1089#1072' '#1085#1091#1083#1077#1074#1086#1075#1086' '#1073#1080#1090#1072' '#1087#1086#1088#1090 +
      #1072'.'
    WordWrap = True
  end
  object Label7: TLabel
    Left = 24
    Top = 200
    Width = 243
    Height = 13
    Caption = '('#1090#1088#1077#1073#1091#1077#1090' '#1087#1088#1077#1076#1074#1072#1088#1080#1090#1077#1083#1100#1085#1086#1081' '#1091#1089#1090#1072#1085#1086#1074#1082#1080' '#1076#1088#1072#1081#1074#1077#1088#1072')'
    WordWrap = True
  end
  object BitBtn1: TBitBtn
    Left = 32
    Top = 240
    Width = 75
    Height = 25
    TabOrder = 0
    Kind = bkOK
  end
  object Edit1: TEdit
    Left = 136
    Top = 92
    Width = 49
    Height = 21
    CharCase = ecUpperCase
    ReadOnly = True
    TabOrder = 1
    Text = 'F4'
  end
  object BitBtn2: TBitBtn
    Left = 136
    Top = 240
    Width = 75
    Height = 25
    TabOrder = 2
    Kind = bkCancel
  end
  object RadioButton1: TRadioButton
    Left = 8
    Top = 128
    Width = 265
    Height = 17
    Caption = #1048#1089#1087#1086#1083#1100#1079#1086#1074#1072#1090#1100' '#1087#1088#1103#1084#1086#1081' '#1074#1099#1074#1086#1076' '#1074' '#1087#1086#1088#1090' 0x61'
    Checked = True
    TabOrder = 3
    TabStop = True
  end
  object RadioButton2: TRadioButton
    Left = 8
    Top = 168
    Width = 257
    Height = 17
    Caption = #1054#1089#1091#1097#1077#1089#1090#1074#1083#1103#1090#1100' '#1074#1099#1074#1086#1076' '#1095#1077#1088#1077#1079' '#1076#1088#1072#1081#1074#1077#1088
    TabOrder = 4
  end
  object BitBtn3: TBitBtn
    Left = 240
    Top = 240
    Width = 75
    Height = 25
    HelpType = htKeyword
    HelpKeyword = #1074#1085#1077#1096#1085#1080#1077' '#1084#1086#1076#1091#1083#1080'- '#1089#1087#1080#1082#1077#1088
    TabOrder = 5
    Kind = bkHelp
  end
end
