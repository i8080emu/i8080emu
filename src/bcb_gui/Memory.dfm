object MemoryForm: TMemoryForm
  Left = 240
  Top = 239
  Width = 513
  Height = 300
  BorderStyle = bsSizeToolWin
  Caption = #1057#1086#1076#1077#1088#1078#1080#1084#1086#1077' '#1087#1072#1084#1103#1090#1080
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnPaint = FormPaint
  PixelsPerInch = 96
  TextHeight = 13
  object PaintBox1: TPaintBox
    Left = 0
    Top = 0
    Width = 488
    Height = 273
    Align = alClient
  end
  object ScrollBar1: TScrollBar
    Left = 488
    Top = 0
    Width = 17
    Height = 273
    Align = alRight
    Kind = sbVertical
    Max = 65536
    PageSize = 0
    TabOrder = 0
    OnScroll = ScrollBar1Scroll
  end
end
