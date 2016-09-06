object Main: TMain
  Left = 0
  Top = 0
  Caption = 'Arduino Controller'
  ClientHeight = 714
  ClientWidth = 1006
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -19
  Font.Name = 'Tahoma'
  Font.Style = []
  KeyPreview = True
  OldCreateOrder = False
  Position = poDefault
  OnClose = FormClose
  OnCloseQuery = FormCloseQuery
  OnCreate = FormCreate
  OnKeyDown = FormKeyDown
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 23
  object mButtonPanel: TPanel
    Left = 823
    Top = 71
    Width = 183
    Height = 643
    Align = alRight
    TabOrder = 0
    object BottomBtnPanel: TPanel
      Left = 1
      Top = 515
      Width = 181
      Height = 127
      Align = alBottom
      AutoSize = True
      TabOrder = 0
      object Button5: TArrayBotButton
        Left = 1
        Top = 1
        Width = 179
        Height = 125
        Action = FileExit1
        Align = alBottom
        Caption = 'E&xit'
        ParentDoubleBuffered = True
        TabOrder = 0
        SoundID = 'BUTTON_CLICK_4'
      end
    end
    object Button1: TArrayBotButton
      Left = 1
      Top = 404
      Width = 181
      Height = 111
      Align = alBottom
      Caption = 'About'
      ParentDoubleBuffered = True
      TabOrder = 1
      OnClick = mAboutBtnClick
      SoundID = 'BUTTON_CLICK_4'
    end
  end
  object MainPanel: TPanel
    Left = 0
    Top = 71
    Width = 823
    Height = 643
    Align = alClient
    TabOrder = 1
    object Splitter1: TSplitter
      Left = 1
      Top = 473
      Width = 821
      Height = 3
      Cursor = crVSplit
      Align = alTop
      ExplicitLeft = 25
      ExplicitTop = 586
      ExplicitWidth = 902
    end
    object TopPanel: TPanel
      Left = 1
      Top = 1
      Width = 821
      Height = 472
      Align = alTop
      TabOrder = 0
      object PageControl1: TPageControl
        Left = 1
        Top = 1
        Width = 819
        Height = 470
        ActivePage = TabSheet2
        Align = alClient
        TabOrder = 0
        object TabSheet1: TTabSheet
          Caption = 'Ribbon Control'
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
          object GroupBox3: TGroupBox
            Left = 3
            Top = 85
            Width = 303
            Height = 156
            Caption = 'Ribbon Separation'
            TabOrder = 0
            object mAutoPuffCB: TPropertyCheckBox
              Left = 16
              Top = 39
              Width = 169
              Height = 17
              Caption = 'Enable Auto Puff'
              TabOrder = 0
              OnClick = mAutoPuffCBClick
            end
            object mEnablesoundsCB: TPropertyCheckBox
              Left = 16
              Top = 107
              Width = 257
              Height = 17
              Caption = 'Enable Notification Sounds'
              TabOrder = 1
            end
            object mEnableAutoZeroCutCB: TPropertyCheckBox
              Left = 16
              Top = 72
              Width = 169
              Height = 17
              Caption = 'Enable Zero Cut'
              TabOrder = 2
            end
          end
          object GroupBox6: TGroupBox
            Left = 327
            Top = 20
            Width = 346
            Height = 136
            Caption = 'Puffer'
            TabOrder = 1
            object mPuffNowBtn: TArrayBotButton
              Left = 23
              Top = 37
              Width = 150
              Height = 80
              Caption = 'Puff'
              ParentDoubleBuffered = True
              TabOrder = 0
              OnClick = mPuffRelatedBtnClick
              SoundID = 'short_beep_3'
            end
            object mEnablePuffBtn: TArrayBotButton
              Left = 179
              Top = 37
              Width = 150
              Height = 80
              Caption = 'Enable Puff'
              ParentDoubleBuffered = True
              TabOrder = 1
              OnClick = mPuffRelatedBtnClick
              SoundID = 'BUTTON_CLICK_4'
            end
          end
          object mDesiredRibbonLengthE: TIntegerLabeledEdit
            Left = 3
            Top = 42
            Width = 303
            Height = 31
            EditLabel.Width = 192
            EditLabel.Height = 23
            EditLabel.Caption = 'Desired Ribbon Length'
            TabOrder = 2
            Text = '-1'
            OnKeyDown = mDesiredRibbonLengthEKeyDown
            Value = -1
          end
          object GroupBox7: TGroupBox
            Left = 327
            Top = 168
            Width = 346
            Height = 249
            Caption = 'Leica UC7'
            TabOrder = 3
            object Label1: TLabel
              Left = 192
              Top = 27
              Width = 115
              Height = 23
              Caption = 'Cut Thickness'
            end
            object mSetZeroCutBtn: TArrayBotButton
              Left = 16
              Top = 102
              Width = 145
              Height = 80
              Caption = 'Set Zero Cut'
              TabOrder = 0
              SoundID = 'short_beep_3'
            end
            object mSetPresetCutBtn: TArrayBotButton
              Left = 190
              Top = 102
              Width = 145
              Height = 80
              Caption = 'Set Preset Cut'
              TabOrder = 1
              SoundID = 'short_beep_3'
            end
            object ComboBox1: TComboBox
              Left = 192
              Top = 56
              Width = 137
              Height = 31
              ItemIndex = 3
              TabOrder = 2
              Text = '45'
              Items.Strings = (
                '1000'
                '500'
                '100'
                '45')
            end
          end
        end
        object TabSheet3: TTabSheet
          Caption = 'Lights and Sensors'
          ImageIndex = 2
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
          object mFrontBackLEDBtn: TArrayBotButton
            Left = 18
            Top = 51
            Width = 167
            Height = 100
            Caption = 'Flip LEDs ON'
            ParentDoubleBuffered = True
            TabOrder = 0
            OnClick = LigthsBtnsClick
            SoundID = 'BUTTON_CLICK_4'
          end
          object mCoaxLightBtn: TArrayBotButton
            Left = 227
            Top = 51
            Width = 181
            Height = 100
            Caption = 'Flip Coax Light ON'
            ParentDoubleBuffered = True
            TabOrder = 1
            OnClick = LigthsBtnsClick
            SoundID = 'BUTTON_CLICK_4'
          end
        end
        object TabSheet2: TTabSheet
          Caption = 'Board Settings'
          ImageIndex = 1
          ExplicitLeft = 0
          ExplicitTop = 0
          ExplicitWidth = 0
          ExplicitHeight = 0
          object GroupBox1: TGroupBox
            Left = 0
            Top = 0
            Width = 257
            Height = 432
            Align = alLeft
            Caption = 'Arduino Server'
            TabOrder = 0
            object mArduinoServerPortE: TIntegerLabeledEdit
              Left = 16
              Top = 56
              Width = 121
              Height = 31
              EditLabel.Width = 108
              EditLabel.Height = 23
              EditLabel.Caption = 'Network Port'
              TabOrder = 0
              Text = '50000'
              Value = 50000
            end
            object mArduinoServerStartBtn: TButton
              Left = 16
              Top = 160
              Width = 200
              Height = 65
              Caption = 'Start/Stop'
              TabOrder = 1
              OnClick = mArduinoServerStartBtnClick
            end
          end
          object mArduinoSB: TScrollBox
            Left = 257
            Top = 0
            Width = 554
            Height = 432
            Align = alClient
            TabOrder = 1
          end
        end
      end
    end
    object BottomPanel: TPanel
      Left = 1
      Top = 476
      Width = 821
      Height = 166
      Align = alClient
      TabOrder = 1
      object infoMemo: TMemo
        Left = 1
        Top = 31
        Width = 819
        Height = 134
        Align = alClient
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ReadOnly = True
        ScrollBars = ssBoth
        TabOrder = 0
        WordWrap = False
      end
      object ToolBar1: TToolBar
        Left = 1
        Top = 1
        Width = 819
        Height = 30
        AutoSize = True
        ButtonHeight = 30
        Caption = 'ToolBar1'
        TabOrder = 1
        object mClearLogMemoBtn: TBitBtn
          Left = 0
          Top = 0
          Width = 75
          Height = 30
          Caption = 'Clear'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -16
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnClick = mClearLogMemoBtnClick
        end
        object LogLevelCB: TComboBox
          Left = 75
          Top = 0
          Width = 145
          Height = 31
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -19
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          Text = 'INFO'
          OnChange = LogLevelCBChange
          Items.Strings = (
            'INFO'
            'DEBUG'
            'DEBUG1'
            'DEBUG2'
            'DEBUG3'
            'DEBUG4'
            'DEBUG5')
        end
      end
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 1006
    Height = 71
    Align = alTop
    TabOrder = 2
    object GroupBox4: TGroupBox
      Left = 749
      Top = 1
      Width = 139
      Height = 69
      Align = alRight
      Caption = 'Temperature'
      TabOrder = 0
      object mTemperatureLbl: mtkFloatLabel
        Left = 40
        Top = 31
        Width = 43
        Height = 23
        Caption = '-1.00'
        ValueString = '-1.00'
        Value = -1.000000000000000000
        TheFont.Charset = DEFAULT_CHARSET
        TheFont.Color = clWindowText
        TheFont.Height = -19
        TheFont.Name = 'Tahoma'
        TheFont.Style = []
      end
    end
    object GroupBox5: TGroupBox
      Left = 888
      Top = 1
      Width = 117
      Height = 69
      Align = alRight
      Caption = 'Humidity'
      TabOrder = 1
      object mHumidityE: mtkFloatLabel
        Left = 30
        Top = 31
        Width = 43
        Height = 23
        Caption = '-1.00'
        ValueString = '-1.00'
        Value = -1.000000000000000000
        TheFont.Charset = DEFAULT_CHARSET
        TheFont.Color = clWindowText
        TheFont.Height = -19
        TheFont.Name = 'Tahoma'
        TheFont.Style = []
      end
    end
    object GroupBox2: TGroupBox
      Left = 1
      Top = 1
      Width = 152
      Height = 69
      Align = alLeft
      Caption = 'Section Count'
      TabOrder = 2
      object mSectionCount: TIntLabel
        Left = 40
        Top = 31
        Width = 17
        Height = 23
        Caption = '-1'
        ValueString = '-1'
        Value = -1
        TheFont.Charset = DEFAULT_CHARSET
        TheFont.Color = clWindowText
        TheFont.Height = -19
        TheFont.Name = 'Tahoma'
        TheFont.Style = []
      end
    end
    object mResetCounterBtn: TArrayBotButton
      Left = 153
      Top = 1
      Width = 160
      Height = 69
      Align = alLeft
      Caption = 'Reset Counter'
      ParentDoubleBuffered = True
      TabOrder = 3
      OnClick = mResetCounterBtnClick
      SoundID = 'BUTTON_CLICK_4'
    end
  end
  object ActionList1: TActionList
    Left = 164
    Top = 464
    object checkForDevices: TAction
      Caption = 'Devices Check'
    end
    object addDevicesToListBox: TAction
      Caption = 'Connect All'
    end
    object identifyCurrent: TAction
      Caption = 'Identify'
    end
    object homeDevice: TAction
      Caption = 'Home'
    end
    object jogForward: TAction
      Caption = 'Fwd'
    end
    object jogBackwards: TAction
      Caption = 'Rev'
    end
    object moveForward: TAction
      Caption = 'Forward'
    end
    object moveBackward: TAction
      Caption = 'Backward'
    end
    object stopAllA: TAction
      Caption = 'Stop All'
    end
    object reInitBotA: TAction
      Caption = 'Reinitialize Devices'
    end
    object ShutDownA: TAction
      Caption = 'Disconnect Devices'
    end
    object FileExit1: TFileExit
      Caption = 'E&xit'
      Hint = 'Exit|Quits the application'
      ImageIndex = 0
    end
    object abortLiftA: TAction
      Caption = 'Abort Lift'
    end
    object liftA: TAction
      Caption = 'Lift'
    end
  end
  object ShutDownTimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = ShutDownTimerTimer
    Left = 44
    Top = 384
  end
  object ApplicationEvents1: TApplicationEvents
    OnException = ApplicationEvents1Exception
    Left = 32
    Top = 464
  end
  object UIUpdateTimer: TTimer
    Enabled = False
    Interval = 300
    OnTimer = UIUpdateTimerTimer
    Left = 172
    Top = 384
  end
end
