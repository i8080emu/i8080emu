object MPsysOptForm: TMPsysOptForm
  Left = 303
  Top = 178
  BorderStyle = bsDialog
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1089#1082#1086#1088#1086#1089#1090#1080' '#1101#1084#1091#1083#1103#1094#1080#1080
  ClientHeight = 278
  ClientWidth = 382
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
    Left = 8
    Top = 8
    Width = 174
    Height = 13
    Caption = #1058#1072#1082#1090#1086#1074#1072#1103' '#1095#1072#1089#1090#1086#1090#1072' '#1087#1088#1086#1094#1077#1089#1089#1086#1088#1072', '#1043#1094':'
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 40
    Width = 369
    Height = 193
    Caption = #1047#1072#1076#1077#1088#1078#1082#1072' '#1089#1082#1086#1088#1086#1089#1090#1080' '#1101#1084#1091#1083#1103#1094#1080#1080
    TabOrder = 0
    object Label2: TLabel
      Left = 24
      Top = 152
      Width = 315
      Height = 26
      Caption = 
        '('#1074' '#1101#1090#1086#1084' '#1089#1083#1091#1095#1072#1077' '#1101#1084#1091#1083#1103#1094#1080#1103' '#1073#1091#1076#1077#1090' '#1087#1088#1086#1080#1089#1093#1086#1076#1080#1090#1100' '#1089' '#1084#1072#1082#1089#1080#1084#1072#1083#1100#1085#1086#1081' '#1089#1082#1086#1088#1086#1089#1090 +
        #1100#1102' '#1074' '#1079#1072#1074#1080#1089#1080#1084#1086#1089#1090#1080' '#1086#1090' '#1087#1088#1086#1080#1079#1074#1086#1076#1080#1090#1077#1083#1100#1085#1086#1089#1090#1080' '#1089#1080#1089#1090#1077#1084#1099')'
      WordWrap = True
    end
    object Label3: TLabel
      Left = 24
      Top = 104
      Width = 44
      Height = 13
      Caption = #1041#1099#1089#1090#1088#1077#1077
    end
    object Label4: TLabel
      Left = 296
      Top = 104
      Width = 57
      Height = 13
      Caption = #1052#1077#1076#1083#1077#1085#1085#1077#1077
    end
    object AutoRadioButton: TRadioButton
      Left = 8
      Top = 24
      Width = 273
      Height = 17
      Caption = #1040#1074#1090#1086#1084#1072#1090#1080#1095#1077#1089#1082#1080#1081' '#1074#1099#1073#1086#1088' ('#1088#1077#1082#1086#1084#1077#1085#1076#1091#1077#1090#1089#1103')'
      Checked = True
      TabOrder = 0
      TabStop = True
      OnClick = AutoRadioButtonClick
    end
    object ManualRadioButton: TRadioButton
      Left = 8
      Top = 48
      Width = 153
      Height = 17
      Caption = #1059#1089#1090#1072#1085#1086#1074#1082#1072' '#1074#1088#1091#1095#1085#1091#1102
      TabOrder = 1
      OnClick = ManualRadioButtonClick
    end
    object RadioButton1: TRadioButton
      Left = 8
      Top = 136
      Width = 113
      Height = 17
      Caption = #1053#1077#1090' '#1079#1072#1076#1077#1088#1078#1082#1080
      TabOrder = 2
      OnClick = AutoRadioButtonClick
    end
    object DelayTrackBar: TTrackBar
      Left = 24
      Top = 72
      Width = 337
      Height = 33
      Enabled = False
      Max = 1000
      Min = 1
      Orientation = trHorizontal
      Frequency = 1
      Position = 500
      SelEnd = 0
      SelStart = 0
      TabOrder = 3
      TickMarks = tmBottomRight
      TickStyle = tsAuto
    end
  end
  object BitBtn1: TBitBtn
    Left = 57
    Top = 244
    Width = 88
    Height = 25
    TabOrder = 1
    OnClick = BitBtn1Click
    Kind = bkOK
  end
  object BitBtn2: TBitBtn
    Left = 153
    Top = 244
    Width = 88
    Height = 25
    Caption = #1054#1090#1084#1077#1085#1072
    TabOrder = 2
    Kind = bkCancel
  end
  object BitBtn3: TBitBtn
    Left = 249
    Top = 244
    Width = 88
    Height = 25
    HelpContext = 160
    Caption = '&'#1057#1087#1088#1072#1074#1082#1072
    TabOrder = 3
    Kind = bkHelp
  end
  object CSpinEdit1: TCSpinEdit
    Left = 192
    Top = 5
    Width = 81
    Height = 22
    MaxValue = 10000000
    MinValue = 500000
    TabOrder = 4
    Value = 2000000
  end
end
