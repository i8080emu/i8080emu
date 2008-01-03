object LookOptForm: TLookOptForm
  Left = 281
  Top = 173
  BorderStyle = bsDialog
  Caption = #1053#1072#1089#1090#1088#1086#1081#1082#1072' '#1074#1085#1077#1096#1085#1077#1075#1086' '#1074#1080#1076#1072
  ClientHeight = 378
  ClientWidth = 437
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
  object TShape
    Left = 312
    Top = 128
    Width = 33
    Height = 33
  end
  object PageControl: TPageControl
    Left = 8
    Top = 8
    Width = 425
    Height = 313
    ActivePage = TabSheetFont
    TabIndex = 1
    TabOrder = 0
    object TabSheetColors: TTabSheet
      Caption = #1062#1074#1077#1090#1072
      object Label1: TLabel
        Left = 9
        Top = 22
        Width = 90
        Height = 13
        Caption = #1062#1074#1077#1090#1086#1074#1099#1077' '#1089#1093#1077#1084#1099':'
      end
      object Label2: TLabel
        Left = 16
        Top = 64
        Width = 47
        Height = 13
        Caption = #1069#1083#1077#1084#1077#1085#1090':'
      end
      object Shape1: TShape
        Left = 168
        Top = 96
        Width = 33
        Height = 33
        OnMouseDown = Shape1MouseDown
      end
      object Label3: TLabel
        Left = 168
        Top = 80
        Width = 28
        Height = 13
        Caption = #1062#1074#1077#1090':'
      end
      object Label4: TLabel
        Left = 288
        Top = 80
        Width = 26
        Height = 13
        Caption = #1060#1086#1085':'
      end
      object Shape2: TShape
        Left = 288
        Top = 96
        Width = 33
        Height = 33
        OnMouseDown = Shape2MouseDown
      end
      object ElementListBox: TListBox
        Left = 16
        Top = 80
        Width = 137
        Height = 177
        ItemHeight = 13
        Items.Strings = (
          #1087#1088#1086#1073#1077#1083#1099
          #1072#1076#1088#1077#1089
          #1082#1086#1076' '#1082#1086#1084#1072#1085#1076#1099
          #1084#1085#1077#1084#1086#1085#1080#1082#1072
          #1086#1087#1077#1088#1072#1085#1076#1099
          #1090#1086#1095#1082#1072' '#1074#1099#1087#1086#1083#1085#1077#1085#1080#1103
          #1090#1086#1095#1082#1072' '#1086#1089#1090#1072#1085#1086#1074#1072)
        TabOrder = 0
        OnClick = ElementListBoxClick
      end
      object ColorShemeComboBox: TComboBox
        Left = 113
        Top = 19
        Width = 145
        Height = 19
        Style = csOwnerDrawFixed
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'MS Sans Serif'
        Font.Pitch = fpFixed
        Font.Style = []
        ItemHeight = 13
        ParentFont = False
        TabOrder = 1
        OnChange = ColorShemeComboBoxChange
        Items.Strings = (
          #1089#1080#1085#1103#1103
          #1073#1077#1083#1072#1103
          #1095#1077#1088#1085#1072#1103
          #1087#1086#1083#1100#1079#1086#1074#1072#1090#1077#1083#1100#1089#1082#1072#1103)
      end
      object Button1: TButton
        Left = 208
        Top = 104
        Width = 73
        Height = 25
        Caption = #1042#1099#1073#1088#1072#1090#1100'...'
        TabOrder = 2
        OnClick = Button1Click
      end
      object Button2: TButton
        Left = 328
        Top = 104
        Width = 75
        Height = 25
        Caption = #1042#1099#1073#1088#1072#1090#1100'...'
        TabOrder = 3
        OnClick = Button2Click
      end
      object Button4: TButton
        Left = 200
        Top = 232
        Width = 89
        Height = 25
        Caption = #1055#1086' '#1091#1084#1086#1083#1095#1072#1085#1080#1102
        TabOrder = 4
        OnClick = Button4Click
      end
      object Button5: TButton
        Left = 304
        Top = 232
        Width = 89
        Height = 25
        Caption = #1057#1086#1093#1088#1072#1085#1080#1090#1100
        TabOrder = 5
        OnClick = Button5Click
      end
    end
    object TabSheetFont: TTabSheet
      Caption = #1064#1088#1080#1092#1090
      ImageIndex = 1
      object Label5: TLabel
        Left = 8
        Top = 16
        Width = 128
        Height = 13
        Caption = #1059#1089#1090#1072#1085#1086#1074#1083#1077#1085#1085#1099#1077' '#1096#1088#1080#1092#1090#1099':'
      end
      object Label6: TLabel
        Left = 176
        Top = 32
        Width = 42
        Height = 13
        Caption = #1056#1072#1079#1084#1077#1088':'
      end
      object Label7: TLabel
        Left = 176
        Top = 80
        Width = 47
        Height = 13
        Caption = #1054#1073#1088#1072#1079#1077#1094':'
      end
      object ExemplLabel: TLabel
        Left = 160
        Top = 112
        Width = 249
        Height = 89
        Alignment = taCenter
        AutoSize = False
        Caption = 'AaBbCc123'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'MS Sans Serif'
        Font.Style = []
        ParentFont = False
      end
      object FontListBox: TListBox
        Left = 8
        Top = 32
        Width = 145
        Height = 245
        ItemHeight = 13
        Sorted = True
        TabOrder = 0
        OnClick = FontListBoxClick
      end
      object FontSizeComboBox: TComboBox
        Left = 229
        Top = 28
        Width = 52
        Height = 19
        Style = csOwnerDrawFixed
        ItemHeight = 13
        TabOrder = 1
        OnChange = FontListBoxClick
        Items.Strings = (
          '7'
          '8'
          '9'
          '10'
          '11'
          '12'
          '13'
          '14'
          '15'
          '16'
          '17'
          '18'
          '19'
          '20'
          '21'
          '22'
          '23'
          '24'
          '25'
          '26'
          '27'
          '28'
          '29'
          '30'
          '31'
          '32'
          '33'
          '34'
          '35'
          '36')
      end
    end
  end
  object BitBtn1: TBitBtn
    Left = 96
    Top = 344
    Width = 89
    Height = 25
    TabOrder = 1
    OnClick = BitBtn1Click
    Kind = bkOK
  end
  object BitBtn2: TBitBtn
    Left = 200
    Top = 344
    Width = 105
    Height = 25
    Caption = #1054#1090#1084#1077#1085#1072
    TabOrder = 2
    Kind = bkCancel
  end
  object BitBtn3: TBitBtn
    Left = 320
    Top = 344
    Width = 97
    Height = 25
    HelpContext = 170
    Caption = '&'#1057#1087#1088#1072#1074#1082#1072
    TabOrder = 3
    Kind = bkHelp
  end
  object ColorDialog1: TColorDialog
    Ctl3D = True
    Left = 308
    Top = 8
  end
  object FontDialog: TFontDialog
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    MinFontSize = 0
    MaxFontSize = 0
    Left = 384
    Top = 8
  end
  object ColorDialog2: TColorDialog
    Ctl3D = True
    Left = 344
    Top = 8
  end
end
