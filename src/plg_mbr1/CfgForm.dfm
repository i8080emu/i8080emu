object ConfigForm: TConfigForm
  Left = 364
  Top = 204
  BorderStyle = bsDialog
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1091#1089#1090#1088#1086#1081#1089#1090#1074#1072
  ClientHeight = 218
  ClientWidth = 362
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
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
    Top = 120
    Width = 66
    Height = 13
    Caption = #1040#1076#1088#1077#1089' '#1087#1086#1088#1090#1072':'
  end
  object Label3: TLabel
    Left = 8
    Top = 64
    Width = 337
    Height = 26
    Caption = 
      #1052#1085#1086#1075#1086#1088#1077#1078#1080#1084#1085#1099#1081' '#1073#1091#1092#1077#1088#1085#1099#1081' '#1088#1077#1075#1080#1089#1090#1088' ('#1052#1041#1056'), '#1088#1072#1073#1086#1090#1072#1102#1097#1080#1081' '#1074' '#1088#1077#1078#1080#1084#1077' '#1074#1074#1086#1076#1072',' +
      ' '#1087#1086#1076#1082#1083#1102#1095#1077#1085' '#1082' '#1089#1077#1084#1080#1089#1077#1075#1084#1077#1085#1090#1085#1086#1084#1091' '#1094#1080#1092#1088#1086#1074#1086#1084#1091' '#1080#1085#1076#1080#1082#1072#1090#1086#1088#1091'.'
    WordWrap = True
  end
  object Label4: TLabel
    Left = 16
    Top = 32
    Width = 297
    Height = 24
    Caption = 'c '#1089#1077#1084#1080#1089#1077#1075#1084#1077#1085#1090#1085#1099#1084' '#1080#1085#1076#1080#1082#1072#1090#1086#1088#1086#1084
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object BitBtn1: TBitBtn
    Left = 96
    Top = 184
    Width = 75
    Height = 25
    TabOrder = 0
    OnClick = BitBtn1Click
    Kind = bkOK
  end
  object Edit1: TEdit
    Left = 104
    Top = 119
    Width = 49
    Height = 21
    TabOrder = 1
    Text = '50'
  end
  object CheckBox1: TCheckBox
    Left = 8
    Top = 160
    Width = 225
    Height = 17
    Caption = #1055#1086#1082#1072#1079#1099#1074#1072#1090#1100' '#1088#1072#1073#1086#1095#1077#1077' '#1086#1082#1085#1086' '#1087#1088#1080' '#1089#1090#1072#1088#1090#1077
    Checked = True
    State = cbChecked
    TabOrder = 2
  end
end
