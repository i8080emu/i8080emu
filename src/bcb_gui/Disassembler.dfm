object DisasForm: TDisasForm
  Left = 233
  Top = 125
  BorderStyle = bsDialog
  Caption = #1058#1077#1082#1089#1090#1086#1074#1099#1081' '#1076#1080#1079#1072#1089#1089#1077#1084#1073#1083#1077#1088
  ClientHeight = 400
  ClientWidth = 289
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
  object RichEdit1: TRichEdit
    Left = 0
    Top = 0
    Width = 289
    Height = 241
    Lines.Strings = (
      'RichEdit1')
    PlainText = True
    ScrollBars = ssVertical
    TabOrder = 0
  end
  object BitBtn1: TBitBtn
    Left = 152
    Top = 368
    Width = 81
    Height = 25
    HelpContext = 120
    Caption = '&'#1057#1087#1088#1072#1074#1082#1072
    TabOrder = 1
    Kind = bkHelp
  end
  object Button1: TButton
    Left = 208
    Top = 296
    Width = 75
    Height = 25
    Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100'...'
    TabOrder = 2
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 208
    Top = 328
    Width = 75
    Height = 25
    Caption = #1055#1077#1095#1072#1090#1100
    TabOrder = 3
    OnClick = Button2Click
  end
  object GroupBox1: TGroupBox
    Left = 8
    Top = 248
    Width = 185
    Height = 105
    Caption = #1044#1080#1079#1072#1089#1089#1077#1084#1073#1083#1080#1088#1091#1077#1084#1099#1081' '#1091#1095#1072#1089#1090#1086#1082
    TabOrder = 4
    object Label1: TLabel
      Left = 24
      Top = 61
      Width = 40
      Height = 13
      Caption = #1053#1072#1095#1072#1083#1086':'
    end
    object Label2: TLabel
      Left = 24
      Top = 83
      Width = 34
      Height = 13
      Caption = #1050#1086#1085#1077#1094':'
    end
    object RadioButton1: TRadioButton
      Tag = 5
      Left = 8
      Top = 16
      Width = 113
      Height = 17
      Caption = #1040#1074#1090#1086#1086#1087#1088#1077#1076#1077#1083#1077#1085#1080#1077
      Checked = True
      TabOrder = 0
      TabStop = True
      OnClick = RadioButton1Click
    end
    object RadioButton2: TRadioButton
      Tag = 5
      Left = 8
      Top = 32
      Width = 113
      Height = 17
      Caption = #1059#1082#1072#1079#1072#1090#1100' '#1074#1088#1091#1095#1085#1091#1102
      TabOrder = 1
    end
    object Edit1: TEdit
      Left = 72
      Top = 56
      Width = 65
      Height = 21
      CharCase = ecUpperCase
      MaxLength = 4
      TabOrder = 2
      Text = 'EDIT1'
      OnChange = Edit1Change
    end
    object Edit2: TEdit
      Left = 72
      Top = 80
      Width = 65
      Height = 21
      CharCase = ecUpperCase
      MaxLength = 4
      TabOrder = 3
      Text = 'EDIT2'
      OnChange = Edit1Change
    end
  end
  object BitBtn2: TBitBtn
    Left = 56
    Top = 368
    Width = 81
    Height = 25
    Caption = '&'#1047#1072#1082#1088#1099#1090#1100
    TabOrder = 5
    OnClick = BitBtn2Click
    Kind = bkClose
  end
  object Button3: TButton
    Left = 208
    Top = 264
    Width = 75
    Height = 25
    Caption = #1050#1086#1087#1080#1088#1086#1074#1072#1090#1100
    TabOrder = 6
    OnClick = Button3Click
  end
  object PrintDialog1: TPrintDialog
    Left = 152
    Top = 72
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = 'asm'
    Filter = 'Assembler file (*.asm)|*.asm|All files (*.*)|*.*'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofNoChangeDir, ofEnableSizing]
    Title = #1059#1082#1072#1078#1080#1090#1077' '#1092#1072#1081#1083' '#1076#1083#1103' '#1089#1086#1093#1088#1072#1085#1077#1085#1080#1103' '#1088#1077#1079#1091#1083#1100#1090#1072#1090#1072' '#1076#1080#1079#1072#1089#1089#1077#1084#1073#1083#1080#1088#1086#1074#1072#1085#1080#1103
    Left = 120
    Top = 72
  end
  object PrinterSetupDialog1: TPrinterSetupDialog
    Left = 184
    Top = 72
  end
end
