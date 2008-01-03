object ConfigForm: TConfigForm
  Left = 297
  Top = 226
  BorderStyle = bsDialog
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1091#1089#1090#1088#1086#1081#1089#1090#1074#1072
  ClientHeight = 297
  ClientWidth = 362
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
    Left = 16
    Top = 8
    Width = 327
    Height = 24
    Caption = #1052#1085#1086#1075#1086#1088#1077#1078#1080#1084#1085#1099#1081' '#1073#1091#1092#1077#1088#1085#1099#1081' '#1088#1077#1075#1080#1089#1090#1088
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 8
    Top = 152
    Width = 66
    Height = 13
    Caption = #1040#1076#1088#1077#1089' '#1087#1086#1088#1090#1072':'
  end
  object Label3: TLabel
    Left = 8
    Top = 40
    Width = 215
    Height = 13
    Caption = #1052#1085#1086#1075#1086#1088#1077#1078#1080#1084#1085#1099#1081' '#1073#1091#1092#1077#1088#1085#1099#1081' '#1088#1077#1075#1080#1089#1090#1088' ('#1052#1041#1056')'
    WordWrap = True
  end
  object BitBtn1: TBitBtn
    Left = 152
    Top = 264
    Width = 75
    Height = 25
    TabOrder = 0
    Kind = bkOK
  end
  object Edit1: TEdit
    Left = 96
    Top = 151
    Width = 49
    Height = 21
    TabOrder = 1
    Text = '50'
  end
  object CheckBox1: TCheckBox
    Left = 8
    Top = 216
    Width = 225
    Height = 17
    Caption = #1055#1086#1082#1072#1079#1099#1074#1072#1090#1100' '#1088#1072#1073#1086#1095#1077#1077' '#1086#1082#1085#1086' '#1087#1088#1080' '#1089#1090#1072#1088#1090#1077
    TabOrder = 2
  end
  object CheckBox2: TCheckBox
    Left = 8
    Top = 240
    Width = 297
    Height = 17
    Caption = #1055#1086#1082#1072#1079#1099#1074#1072#1090#1100' '#1086#1082#1085#1086' '#1087#1088#1080' '#1086#1073#1088#1072#1097#1077#1085#1080#1103#1093' '#1082' '#1087#1086#1088#1090#1072#1084
    Checked = True
    State = cbChecked
    TabOrder = 3
  end
end
