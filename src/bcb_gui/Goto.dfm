object GotoForm: TGotoForm
  Left = 476
  Top = 516
  Width = 297
  Height = 147
  Caption = #1055#1077#1088#1077#1076#1090#1080' '#1082' '#1072#1076#1088#1077#1089#1091'...'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 273
    Height = 33
    Caption = 
      #1042#1074#1077#1076#1080#1090#1077' '#1074' '#1096#1077#1089#1090#1085#1072#1076#1094#1072#1090#1080#1088#1080#1095#1085#1086#1081' '#1089#1080#1089#1090#1077#1084#1077' '#1072#1076#1088#1077#1089', '#1082' '#1082#1086#1090#1086#1088#1086#1084#1091' '#1093#1086#1090#1080#1090#1077' '#1087#1077#1088 +
      #1077#1084#1077#1089#1090#1080#1090#1100' '#1090#1086#1095#1082#1091' '#1074#1099#1087#1086#1083#1085#1077#1085#1080#1103':'
    WordWrap = True
  end
  object Label2: TLabel
    Left = 8
    Top = 56
    Width = 34
    Height = 13
    Caption = #1040#1076#1088#1077#1089':'
  end
  object Edit1: TEdit
    Left = 48
    Top = 53
    Width = 49
    Height = 21
    TabStop = False
    CharCase = ecUpperCase
    HideSelection = False
    MaxLength = 4
    TabOrder = 1
    Text = '0000'
    OnChange = Edit1Change
  end
  object BitBtn1: TBitBtn
    Left = 48
    Top = 88
    Width = 75
    Height = 25
    TabOrder = 0
    OnClick = BitBtn1Click
    Kind = bkOK
  end
  object BitBtn2: TBitBtn
    Left = 136
    Top = 88
    Width = 81
    Height = 25
    Caption = #1054#1090#1084#1077#1085#1072
    TabOrder = 2
    Kind = bkCancel
  end
end
