object RegFlagForm: TRegFlagForm
  Left = 358
  Top = 183
  BorderStyle = bsDialog
  Caption = #1048#1079#1084#1077#1085#1077#1085#1080#1077' '#1088#1077#1075#1080#1089#1090#1088#1086#1074' '#1080' '#1092#1083#1072#1075#1086#1074
  ClientHeight = 268
  ClientWidth = 321
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
    Left = 16
    Top = 232
    Width = 89
    Height = 25
    TabOrder = 0
    OnClick = BitBtn1Click
    Kind = bkOK
  end
  object BitBtn2: TBitBtn
    Left = 112
    Top = 232
    Width = 89
    Height = 25
    Caption = #1054#1090#1084#1077#1085#1072
    TabOrder = 1
    Kind = bkCancel
  end
  object BitBtn3: TBitBtn
    Left = 208
    Top = 232
    Width = 89
    Height = 25
    HelpContext = 130
    Caption = '&'#1057#1087#1088#1072#1074#1082#1072
    TabOrder = 2
    Kind = bkHelp
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 8
    Width = 305
    Height = 105
    Caption = #1056#1077#1075#1080#1089#1090#1088#1099
    TabOrder = 3
    object Label1: TLabel
      Left = 8
      Top = 44
      Width = 14
      Height = 13
      Caption = 'BC'
    end
    object Label2: TLabel
      Left = 8
      Top = 68
      Width = 15
      Height = 13
      Caption = 'DE'
    end
    object Label3: TLabel
      Left = 136
      Top = 43
      Width = 14
      Height = 13
      Caption = 'HL'
    end
    object Label4: TLabel
      Left = 136
      Top = 68
      Width = 14
      Height = 13
      Caption = 'SP'
    end
    object Label5: TLabel
      Left = 10
      Top = 21
      Width = 7
      Height = 13
      Caption = 'A'
    end
    object Edit1: TEdit
      Left = 32
      Top = 40
      Width = 49
      Height = 21
      CharCase = ecUpperCase
      MaxLength = 4
      TabOrder = 0
      Text = 'EDIT1'
      OnChange = Edit1Change
    end
    object Edit2: TEdit
      Left = 32
      Top = 64
      Width = 49
      Height = 21
      CharCase = ecUpperCase
      MaxLength = 4
      TabOrder = 1
      Text = 'EDIT2'
      OnChange = Edit1Change
    end
    object Edit3: TEdit
      Left = 160
      Top = 40
      Width = 49
      Height = 21
      CharCase = ecUpperCase
      MaxLength = 4
      TabOrder = 2
      Text = 'EDIT3'
      OnChange = Edit1Change
    end
    object Edit4: TEdit
      Left = 160
      Top = 64
      Width = 49
      Height = 21
      CharCase = ecUpperCase
      MaxLength = 4
      TabOrder = 3
      Text = 'EDIT4'
      OnChange = Edit1Change
    end
    object Edit5: TEdit
      Left = 32
      Top = 16
      Width = 25
      Height = 21
      CharCase = ecUpperCase
      MaxLength = 2
      TabOrder = 4
      Text = 'EDIT5'
      OnChange = Edit1Change
    end
  end
  object GroupBox2: TGroupBox
    Left = 8
    Top = 120
    Width = 305
    Height = 97
    Caption = #1060#1083#1072#1075#1080
    TabOrder = 4
    object CheckBox1: TCheckBox
      Left = 8
      Top = 24
      Width = 137
      Height = 17
      Caption = 'CY ('#1092#1083#1072#1075' '#1087#1077#1088#1077#1085#1086#1089#1072')'
      TabOrder = 0
    end
    object CheckBox2: TCheckBox
      Left = 8
      Top = 48
      Width = 121
      Height = 17
      Caption = 'P ('#1092#1083#1072#1075' '#1095#1077#1090#1085#1086#1089#1090#1080')'
      TabOrder = 1
    end
    object CheckBox3: TCheckBox
      Left = 8
      Top = 72
      Width = 225
      Height = 17
      Caption = 'AC ('#1092#1083#1072#1075' '#1074#1089#1087#1086#1084#1086#1075#1072#1090#1077#1083#1100#1085#1086#1075#1086' '#1087#1077#1088#1077#1085#1086#1089#1072')'
      TabOrder = 2
    end
    object CheckBox4: TCheckBox
      Left = 200
      Top = 24
      Width = 89
      Height = 17
      Caption = 'Z ('#1092#1083#1072#1075' '#1085#1091#1083#1103')'
      TabOrder = 3
    end
    object CheckBox5: TCheckBox
      Left = 200
      Top = 48
      Width = 97
      Height = 17
      Caption = 'S ('#1092#1083#1072#1075' '#1079#1085#1072#1082#1072')'
      TabOrder = 4
    end
  end
end
