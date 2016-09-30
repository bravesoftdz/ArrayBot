object ImagesAndMoviesDM: TImagesAndMoviesDM
  OldCreateOrder = False
  Height = 591
  Width = 604
  object images: TSQLDataSet
    CommandText = 'select * from abImage order by date desc'
    MaxBlobSize = -1
    Params = <>
    SQLConnection = SQLConnection1
    Left = 32
    Top = 112
  end
  object SQLConnection1: TSQLConnection
    ConnectionName = 'ATDBDebug'
    DriverName = 'DevartSQLite'
    LoginPrompt = False
    Params.Strings = (
      'DriverName=DevartSQLite'
      'DriverUnit=DbxDevartSQLite'
      
        'DriverPackageLoader=TDBXDynalinkDriverLoader,DBXCommonDriver170.' +
        'bpl'
      
        'MetaDataPackageLoader=TDBXDevartSQLiteMetaDataCommandFactory,Dbx' +
        'DevartSQLiteDriver170.bpl'
      'ProductName=DevartSQLite'
      'LibraryName=dbexpsqlite40.dll'
      'VendorLib=sqlite3.dll'
      'Database=C:\Users\matsk\AppData\Local\ArrayBot\atDB.db'
      'LocaleCode=0000'
      'IsolationLevel=ReadCommitted'
      'ASCIIDataBase=False'
      'BusyTimeout=0'
      'EnableSharedCache=False'
      'MaxBlobSize=-1'
      'FetchAll=True'
      'ForceCreateDatabase=False'
      'ForeignKeys=True'
      'UseUnicode=True'
      'EnableLoadExtension=False'
      'BlobSize=-1')
    Connected = True
    Left = 32
    Top = 32
  end
  object imageNote: TSQLQuery
    Active = True
    DataSource = image_note
    MaxBlobSize = -1
    Params = <>
    SQL.Strings = (
      'select * from abImage_note where image_id = 5')
    SQLConnection = SQLConnection1
    Left = 40
    Top = 184
  end
  object notesQ: TSQLQuery
    Active = True
    DataSource = imageNotes
    MaxBlobSize = -1
    Params = <>
    SQL.Strings = (
      
        ' SELECT * FROM note WHERE id IN (SELECT note_id FROM abImage_not' +
        'e WHERE image_id = "")')
    SQLConnection = SQLConnection1
    Left = 40
    Top = 256
  end
  object DataSetProvider2: TDataSetProvider
    DataSet = notesQ
    Left = 144
    Top = 256
  end
  object DataSetProvider1: TDataSetProvider
    DataSet = imageNote
    Left = 144
    Top = 184
  end
  object imagesProvider: TDataSetProvider
    DataSet = images
    Left = 144
    Top = 112
  end
  object imagesCDS: TClientDataSet
    Active = True
    Aggregates = <>
    Params = <>
    ProviderName = 'imagesProvider'
    AfterScroll = imagesCDSAfterScroll
    Left = 256
    Top = 112
  end
  object imagesDS: TDataSource
    DataSet = imagesCDS
    Left = 336
    Top = 112
  end
  object image_note: TDataSource
    DataSet = imageNoteCDS
    Left = 336
    Top = 184
  end
  object imageNoteCDS: TClientDataSet
    Active = True
    Aggregates = <>
    Params = <>
    ProviderName = 'DataSetProvider1'
    Left = 256
    Top = 184
  end
  object notesCDS: TClientDataSet
    Active = True
    Aggregates = <>
    Params = <>
    ProviderName = 'DataSetProvider2'
    Left = 256
    Top = 256
  end
  object imageNotes: TDataSource
    DataSet = notesCDS
    Left = 336
    Top = 256
  end
end
