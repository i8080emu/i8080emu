object FindPutForm: TFindPutForm
  Left = 387
  Top = 271
  BorderStyle = bsDialog
  Caption = #1055#1086#1080#1089#1082'/'#1042#1074#1086#1076' '#1084#1085#1077#1084#1086#1085#1080#1082#1080' '#1080#1083#1080' '#1082#1086#1076#1086#1074
  ClientHeight = 130
  ClientWidth = 262
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object RadioButton1: TRadioButton
    Left = 8
    Top = 8
    Width = 113
    Height = 17
    Caption = #1052#1085#1077#1084#1086#1085#1080#1082#1072
    Checked = True
    TabOrder = 3
    TabStop = True
  end
  object RadioButton2: TRadioButton
    Left = 8
    Top = 48
    Width = 105
    Height = 17
    Caption = #1050#1086#1076#1099
    TabOrder = 1
  end
  object Edit1: TEdit
    Left = 24
    Top = 24
    Width = 121
    Height = 21
    TabOrder = 0
    OnChange = Edit1Change
    OnKeyDown = FormKeyDown
  end
  object Edit2: TEdit
    Left = 24
    Top = 64
    Width = 121
    Height = 21
    CharCase = ecUpperCase
    TabOrder = 2
    OnChange = Edit2Change
    OnKeyDown = FormKeyDown
  end
  object HelpBitBtn: TBitBtn
    Left = 176
    Top = 72
    Width = 81
    Height = 25
    HelpContext = 140
    Caption = '&'#1057#1087#1088#1072#1074#1082#1072
    TabOrder = 4
    Kind = bkHelp
  end
  object BitBtn2: TBitBtn
    Left = 176
    Top = 8
    Width = 81
    Height = 25
    TabOrder = 5
    OnClick = BitBtn2Click
    Kind = bkOK
  end
  object BitBtn3: TBitBtn
    Left = 176
    Top = 40
    Width = 81
    Height = 25
    Caption = '&'#1047#1072#1082#1088#1099#1090#1100
    TabOrder = 6
    Kind = bkClose
  end
  object RadioButton3: TRadioButton
    Left = 8
    Top = 88
    Width = 113
    Height = 17
    Caption = #1058#1077#1082#1089#1090
    TabOrder = 7
  end
  object Edit3: TEdit
    Left = 24
    Top = 104
    Width = 121
    Height = 21
    MaxLength = 10
    TabOrder = 8
    OnChange = Edit3Change
  end
end
