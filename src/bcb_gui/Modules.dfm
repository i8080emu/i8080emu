object ModulesForm: TModulesForm
  Left = 305
  Top = 230
  BorderStyle = bsDialog
  Caption = #1047#1072#1075#1088#1091#1078#1077#1085#1085#1099#1077' '#1084#1086#1076#1091#1083#1080' '#1101#1084#1091#1083#1103#1094#1080#1080' '#1074#1085#1077#1096#1085#1080#1093' '#1091#1089#1090#1088#1086#1081#1089#1090#1074
  ClientHeight = 201
  ClientWidth = 384
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
  object CloseBitBtn: TBitBtn
    Left = 144
    Top = 168
    Width = 81
    Height = 25
    Caption = '&'#1047#1072#1082#1088#1099#1090#1100
    TabOrder = 0
    Kind = bkClose
  end
  object HelpBitBtn: TBitBtn
    Left = 240
    Top = 168
    Width = 81
    Height = 25
    HelpContext = 180
    Caption = '&'#1057#1087#1088#1072#1074#1082#1072
    TabOrder = 1
    Kind = bkHelp
  end
  object ListBox1: TListBox
    Left = 8
    Top = 8
    Width = 369
    Height = 145
    ItemHeight = 13
    TabOrder = 2
    OnClick = ListBox1Click
  end
  object PropertyButton: TButton
    Left = 48
    Top = 168
    Width = 81
    Height = 25
    Caption = #1057#1074#1086#1081#1089#1090#1074#1072'...'
    TabOrder = 3
    OnClick = PropertyButtonClick
  end
end
