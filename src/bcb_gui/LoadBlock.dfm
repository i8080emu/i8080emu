object LoadBlockForm: TLoadBlockForm
  Left = 302
  Top = 258
  BorderStyle = bsDialog
  Caption = #1047#1072#1075#1088#1091#1079#1082#1072' '#1073#1083#1086#1082#1072' '#1074' '#1087#1072#1084#1103#1090#1100
  ClientHeight = 198
  ClientWidth = 260
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCloseQuery = FormCloseQuery
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 241
    Height = 65
    AutoSize = False
    Caption = 
      #1042' '#1101#1090#1086#1084' '#1076#1080#1072#1083#1086#1075#1077' '#1085#1077#1086#1073#1093#1086#1076#1080#1084#1086' '#1091#1082#1072#1079#1072#1090#1100' '#1072#1076#1088#1077#1089', '#1087#1086' '#1082#1086#1090#1086#1088#1086#1084#1091' '#1074#1099' '#1093#1086#1090#1080#1090#1077' '#1079 +
      #1072#1075#1088#1091#1079#1080#1090#1100' '#1091#1082#1072#1079#1072#1085#1085#1099#1081' '#1073#1083#1086#1082' '#1082#1086#1076#1072'. '#1055#1086' '#1091#1084#1086#1083#1095#1072#1085#1080#1102' '#1087#1088#1077#1076#1083#1086#1078#1077#1085' '#1072#1076#1088#1077#1089', '#1091#1082#1072#1079 +
      #1072#1085#1099#1081' '#1087#1088#1080' '#1089#1086#1093#1088#1072#1085#1077#1085#1080#1080' '#1073#1083#1086#1082#1072'.'
    WordWrap = True
  end
  object Label2: TLabel
    Left = 16
    Top = 99
    Width = 34
    Height = 13
    Caption = #1040#1076#1088#1077#1089':'
  end
  object Label3: TLabel
    Left = 16
    Top = 131
    Width = 36
    Height = 13
    Caption = #1044#1083#1080#1085#1072':'
  end
  object Edit1: TEdit
    Left = 72
    Top = 96
    Width = 81
    Height = 21
    MaxLength = 4
    TabOrder = 0
    Text = '0000'
    OnChange = Edit1Change
  end
  object Edit2: TEdit
    Left = 72
    Top = 128
    Width = 81
    Height = 21
    Enabled = False
    MaxLength = 4
    TabOrder = 1
    Text = '0000'
    OnChange = Edit1Change
  end
  object BitBtn1: TBitBtn
    Left = 8
    Top = 168
    Width = 75
    Height = 25
    TabOrder = 2
    Kind = bkOK
  end
  object BitBtn2: TBitBtn
    Left = 88
    Top = 168
    Width = 81
    Height = 25
    Caption = #1054#1090#1084#1077#1085#1072
    TabOrder = 3
    Kind = bkCancel
  end
  object BitBtn3: TBitBtn
    Left = 176
    Top = 168
    Width = 81
    Height = 25
    HelpContext = 100
    Caption = '&'#1057#1087#1088#1072#1074#1082#1072
    TabOrder = 4
    Kind = bkHelp
  end
end
