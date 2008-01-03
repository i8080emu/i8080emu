object LoadBinForm: TLoadBinForm
  Left = 258
  Top = 306
  Width = 408
  Height = 196
  Caption = #1047#1072#1075#1088#1091#1079#1082#1072' '#1092#1072#1081#1083#1072' '#1074' '#1087#1072#1084#1103#1090#1100
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCloseQuery = FormCloseQuery
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 25
    Width = 32
    Height = 13
    Caption = #1060#1072#1081#1083':'
  end
  object Label2: TLabel
    Left = 8
    Top = 52
    Width = 104
    Height = 13
    Caption = #1040#1076#1088#1077#1089' '#1076#1083#1103' '#1079#1072#1075#1088#1091#1079#1082#1080':'
  end
  object Label3: TLabel
    Left = 232
    Top = 96
    Width = 23
    Height = 13
    Caption = #1073#1072#1081#1090
  end
  object FileEdit: TEdit
    Left = 48
    Top = 19
    Width = 257
    Height = 21
    TabOrder = 0
  end
  object BrowseButton: TButton
    Left = 312
    Top = 16
    Width = 75
    Height = 25
    Caption = #1054#1073#1079#1086#1088'...'
    TabOrder = 1
    OnClick = BrowseButtonClick
  end
  object StartEdit: TEdit
    Left = 120
    Top = 48
    Width = 41
    Height = 21
    MaxLength = 4
    TabOrder = 2
    Text = '0000'
    OnChange = StartEditChange
  end
  object RadioButton1: TRadioButton
    Left = 8
    Top = 80
    Width = 153
    Height = 17
    Caption = #1047#1072#1075#1088#1091#1079#1080#1090#1100' '#1074#1077#1089#1100' '#1092#1072#1081#1083
    Checked = True
    TabOrder = 3
    TabStop = True
    OnClick = RadioButton1Click
  end
  object RadioButton2: TRadioButton
    Left = 8
    Top = 96
    Width = 161
    Height = 17
    Caption = #1047#1072#1075#1088#1091#1079#1080#1090#1100' '#1090#1086#1083#1100#1082#1086' '#1087#1077#1088#1074#1099#1077
    TabOrder = 4
    OnClick = RadioButton2Click
  end
  object LenEdit: TEdit
    Left = 176
    Top = 92
    Width = 49
    Height = 21
    Enabled = False
    MaxLength = 4
    TabOrder = 5
    Text = '0000'
    OnChange = StartEditChange
  end
  object BitBtn1: TBitBtn
    Left = 64
    Top = 136
    Width = 75
    Height = 25
    TabOrder = 6
    Kind = bkOK
  end
  object BitBtn2: TBitBtn
    Left = 160
    Top = 136
    Width = 75
    Height = 25
    TabOrder = 7
    Kind = bkCancel
  end
  object BitBtn3: TBitBtn
    Left = 256
    Top = 136
    Width = 75
    Height = 25
    HelpContext = 105
    TabOrder = 8
    Kind = bkHelp
  end
  object OpenDialog1: TOpenDialog
    DefaultExt = 'bin'
    Filter = 'All files (*.*)|*.*'
    InitialDir = '.'
    Options = [ofHideReadOnly, ofFileMustExist, ofEnableSizing]
    Left = 312
    Top = 56
  end
end
