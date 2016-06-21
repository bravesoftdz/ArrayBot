object MoveSequencerFrame: TMoveSequencerFrame
  Left = 0
  Top = 0
  Width = 393
  Height = 289
  AutoSize = True
  TabOrder = 0
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 393
    Height = 289
    Caption = 'Move Sequencer'
    TabOrder = 0
    object Label1: TLabel
      Left = 16
      Top = 26
      Width = 52
      Height = 13
      Caption = 'Sequences'
    end
    object GroupBox2: TGroupBox
      Left = 207
      Top = 26
      Width = 169
      Height = 207
      Caption = 'Move'
      TabOrder = 0
      object Label2: TLabel
        Left = 16
        Top = 20
        Width = 28
        Height = 13
        Caption = 'Motor'
      end
      object mMovePosE: TFloatLabeledEdit
        Left = 16
        Top = 79
        Width = 49
        Height = 21
        EditLabel.Width = 37
        EditLabel.Height = 13
        EditLabel.Caption = 'Position'
        TabOrder = 0
        Text = '0.0'
        OnKeyDown = moveParEdit
      end
      object mMaxVelE: TFloatLabeledEdit
        Left = 16
        Top = 127
        Width = 65
        Height = 21
        EditLabel.Width = 60
        EditLabel.Height = 13
        EditLabel.Caption = 'Max Velocity'
        TabOrder = 1
        Text = '0.0'
        OnKeyDown = moveParEdit
      end
      object mAccE: TFloatLabeledEdit
        Left = 87
        Top = 127
        Width = 65
        Height = 21
        EditLabel.Width = 59
        EditLabel.Height = 13
        EditLabel.Caption = 'Acceleration'
        TabOrder = 2
        Text = '0.0'
        OnKeyDown = moveParEdit
      end
      object mDwellTimeE: TFloatLabeledEdit
        Left = 15
        Top = 167
        Width = 65
        Height = 21
        EditLabel.Width = 74
        EditLabel.Height = 13
        EditLabel.Caption = 'Dwell Time (ms)'
        TabOrder = 3
        Text = '0.0'
        OnKeyDown = moveParEdit
      end
      object MotorsCB: TComboBox
        Left = 16
        Top = 39
        Width = 105
        Height = 21
        TabOrder = 4
        Text = 'Select Motor'
        OnChange = MotorsCBChange
      end
      object mMovePositionLabel: TSTDStringLabeledEdit
        Left = 71
        Top = 79
        Width = 90
        Height = 21
        EditLabel.Width = 67
        EditLabel.Height = 13
        EditLabel.Caption = 'Position Name'
        TabOrder = 5
        Text = 'PositionName'
        OnKeyDown = moveParEdit
        Value = 'PositionName'
      end
    end
    object mStartBtn: TButton
      Left = 126
      Top = 231
      Width = 75
      Height = 47
      Caption = 'Start'
      TabOrder = 1
      OnClick = mStartBtnClick
    end
    object mMovesLB: TListBox
      Left = 16
      Top = 105
      Width = 121
      Height = 97
      ItemHeight = 13
      TabOrder = 2
      OnClick = mMovesLBClick
    end
    object mSequencesCB: TComboBox
      Left = 16
      Top = 78
      Width = 121
      Height = 21
      Style = csDropDownList
      ItemIndex = 0
      TabOrder = 3
      Text = 'MoveSequence'
      OnChange = mSequencesCBChange
      Items.Strings = (
        'MoveSequence')
    end
    object mSaveSequenceBtn: TButton
      Left = 16
      Top = 208
      Width = 41
      Height = 25
      Caption = 'Save'
      TabOrder = 4
      OnClick = mSaveSequenceBtnClick
    end
    object mAddMoveBtn: TButton
      Left = 143
      Top = 111
      Width = 58
      Height = 25
      Caption = 'Add Move'
      TabOrder = 5
      OnClick = mAddMoveBtnClick
    end
    object mDeleteSequenceBtn: TButton
      Left = 63
      Top = 44
      Width = 41
      Height = 28
      Caption = 'Delete'
      TabOrder = 6
      OnClick = mDeleteSequenceBtnClick
    end
    object mAddSeqBtn: TButton
      Left = 16
      Top = 44
      Width = 41
      Height = 28
      Caption = 'Add'
      TabOrder = 7
      OnClick = mAddSeqBtnClick
    end
    object mDeleteMoveBtn: TButton
      Left = 143
      Top = 142
      Width = 58
      Height = 25
      Caption = 'Del Move'
      TabOrder = 8
      OnClick = mDeleteMoveBtnClick
    end
  end
  object mSequenceTimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = mSequenceTimerTimer
    Left = 160
    Top = 184
  end
end
