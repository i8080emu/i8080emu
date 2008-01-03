object SaveBlockForm: TSaveBlockForm
  Left = 401
  Top = 258
  BorderStyle = bsDialog
  Caption = #1057#1086#1093#1088#1072#1085#1077#1085#1080#1077' '#1073#1083#1086#1082#1072' '#1087#1072#1084#1103#1090#1080
  ClientHeight = 176
  ClientWidth = 255
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
    Left = 16
    Top = 80
    Width = 34
    Height = 13
    Caption = #1040#1076#1088#1077#1089':'
  end
  object Label2: TLabel
    Left = 16
    Top = 104
    Width = 42
    Height = 13
    Caption = #1044#1083#1080#1085#1085#1072':'
  end
  object Label3: TLabel
    Left = 8
    Top = 8
    Width = 241
    Height = 57
    AutoSize = False
    Caption = 
      #1044#1083#1103' '#1089#1086#1093#1088#1072#1085#1077#1085#1080#1103' '#1073#1083#1086#1082#1072' '#1085#1077#1086#1073#1093#1086#1076#1080#1084#1086' '#1091#1082#1072#1079#1072#1090#1100' '#1085#1072#1095#1072#1083#1100#1085#1099#1081' '#1072#1076#1088#1077#1089' '#1073#1083#1086#1082#1072' '#1074' ' +
      #1087#1072#1084#1103#1090#1080' '#1080' '#1076#1083#1080#1085#1091' '#1074' '#1073#1072#1081#1090#1072#1093' ('#1074' '#1096#1077#1089#1090#1085#1072#1076#1094#1072#1090#1080#1088#1080#1095#1085#1086#1081' '#1089#1080#1089#1090#1077#1084#1077' '#1089#1095#1080#1089#1083#1077#1085#1080#1103').'
    WordWrap = True
  end
  object BitBtn1: TBitBtn
    Left = 8
    Top = 144
    Width = 75
    Height = 25
    TabOrder = 0
    Kind = bkOK
  end
  object BitBtn2: TBitBtn
    Left = 88
    Top = 144
    Width = 75
    Height = 25
    Caption = #1054#1090#1084#1077#1085#1072
    TabOrder = 1
    Kind = bkCancel
  end
  object BitBtn3: TBitBtn
    Left = 168
    Top = 144
    Width = 81
    Height = 25
    HelpContext = 110
    Caption = '&'#1057#1087#1088#1072#1074#1082#1072
    TabOrder = 2
    Kind = bkHelp
  end
  object Edit1: TEdit
    Left = 88
    Top = 80
    Width = 89
    Height = 21
    CharCase = ecUpperCase
    MaxLength = 4
    TabOrder = 3
    Text = '0000'
    OnChange = Edit1Change
  end
  object Edit2: TEdit
    Left = 88
    Top = 104
    Width = 89
    Height = 21
    CharCase = ecUpperCase
    MaxLength = 4
    TabOrder = 4
    Text = '400'
    OnChange = Edit1Change
  end
end
