#include <vcl.h>
#pragma hdrstop
#include "TMainForm.h"
#include "abVCLUtils.h"
#include "mtkLogger.h"
#include "mtkVCLUtils.h"
#include "mtkWin32Utils.h"
#include "mtkUtils.h"
#include "camera/uc480_tools.h"
#include "TSettingsForm.h"
#include "abDBUtils.h"
#include "abVCLUtils.h"
using namespace mtk;
using namespace ab;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TPropertyCheckBox"
#pragma link "mtkFloatLabel"
#pragma link "TArrayBotBtn"
#pragma resource "*.dfm"
TMainForm *MainForm;

extern string gLogFileName;
extern string gApplicationRegistryRoot;
extern string gLogFileLocation;
extern string gLogFileName;
extern string gAppDataFolder;
extern bool   gAppIsStartingUp;
extern bool   gAppIsClosing;

//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TRegistryForm(gApplicationRegistryRoot, "MainForm", Owner),
    	mLogFileReader(joinPath(getSpecialFolder(CSIDL_LOCAL_APPDATA), "ArrayBot", gLogFileName), &logMsg),
        mCaptureVideo(false),
        mSettingsForm(NULL),
        mAVIID(0),
    	mIniFile(joinPath(gAppDataFolder, "ArrayCam.ini"), true, true),
    	mLogLevel(lAny),
        mAutoGain(false),
        mAutoExposure(false),
        mVerticalMirror(false),
        mHorizontalMirror(false),
        mPairLEDs(false),
		mGetReadyForZeroCutSound("SHORT_BEEP_2", 10, 500),
		mSetZeroCutSound("SHORT_BEEP_2", 25, 150),
		mRestoreFromZeroCutSound("CLOSING_DOWN_1", 15, 350),
        mSnapShotFolder(""),
        mMoviesFolder(""),
        mLocalDBFile(""),
        mClientDBSession("")
{
   	mLogFileReader.start(true);

	//Setup UI/INI properties
    mProperties.setSection("GENERAL");
	mProperties.setIniFile(&mIniFile);
	mProperties.add((BaseProperty*)  &mLogLevel.setup( 	    	"LOG_LEVEL",    		lAny));
	mProperties.add((BaseProperty*)  &mAutoGain.setup(			"AUTO_GAIN",    		false));
	mProperties.add((BaseProperty*)  &mAutoExposure.setup( 		"AUTO_EXPOSURE",    	false));
	mProperties.add((BaseProperty*)  &mVerticalMirror.setup(	"VERTICAL_MIRROR",    	false));
	mProperties.add((BaseProperty*)  &mHorizontalMirror.setup(	"HORIZONTAL_MIRROR",    false));
	mProperties.add((BaseProperty*)  &mHorizontalMirror.setup(	"HORIZONTAL_MIRROR",    false));
	mProperties.add((BaseProperty*)  &mPairLEDs.setup(			"PAIR_LEDS",    		true));
    mProperties.add((BaseProperty*)  &mSnapShotFolder.setup(	"SNAP_SHOT_FOLDER",     "C:\\Temp"	));
	mProperties.add((BaseProperty*)  &mMoviesFolder.setup(		"MOVIES_FOLDER",   		"C:\\Temp"	));
	mProperties.add((BaseProperty*)  &mLocalDBFile.setup(		"LOCAL_DB",   			joinPath(getSpecialFolder(CSIDL_LOCAL_APPDATA), "ArrayBot", "atdb.db")	));

    mProperties.read();

    mClientDBSession.connect(mLocalDBFile);

	//Camera rendering mode
    mRenderMode = IS_RENDER_FIT_TO_WINDOW;

	mLightsArduinoClient.assignOnMessageReceivedCallBack(onLightsArduinoMessageReceived);
    mSensorsArduinoClient.assignOnMessageReceivedCallBack(onSensorsArduinoMessageReceived);

    mLightsArduinoClient.onConnected 		= onArduinoClientConnected;
	mLightsArduinoClient.onDisconnected 	= onArduinoClientDisconnected;

    gLogger.setLogLevel(mLogLevel);
}

__fastcall TMainForm::~TMainForm()
{
	mProperties.write();
    mIniFile.save();
}

//This one is called from the reader thread
void __fastcall TMainForm::logMsg()
{
    infoMemo->Lines->Insert(0, (vclstr(mLogFileReader.getData())));
    mLogFileReader.purge();
}

//Callback from socket client class
void TMainForm::onArduinoClientConnected()
{
    Log(lDebug) << "ArduinoClient was connected..";

    //Send message to update UI
    mLightsArduinoClient.getBoardStatus();
    enableDisableClientControls(true);
}

void TMainForm::onArduinoClientDisconnected()
{
    Log(lDebug) << "Arduino Client was disconnected..";

	//Don't worry if we are closing down..
    if(gAppIsClosing != true)
    {
    	enableDisableClientControls(false);
    }
}

void TMainForm::enableDisableClientControls(bool enable)
{
	//Disable client related components..
    if(mSettingsForm)
    {
    	enableDisableGroupBox(mSettingsForm->LightIntensitiesGB, enable);
    }
	mToggleCoaxBtn->Enabled = enable;
    mFrontBackLEDBtn->Enabled = enable;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(Key == vkEscape)
    {
		mCamera.exitCamera();
    	Close();
    }
}

void __fastcall TMainForm::mMainPanelResize(TObject *Sender)
{
//	mFitToScreenButtonClick(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mToggleLogPanelClick(TObject *Sender)
{
	mBottomPanel->Visible = !mBottomPanel->Visible;
	mToggleLogPanelBtn->Caption =  (mBottomPanel->Visible) ? "Hide Bottom Panel" : "Show Bottom Panel";
	mFitToScreenButtonClick(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::ClearLogMemo(TObject *Sender)
{
	infoMemo->Clear();
}


void __fastcall TMainForm::mCameraStreamPanelDblClick(TObject *Sender)
{
//	this->BorderStyle = (this->BorderStyle == bsNone) ? bsSingle : bsNone;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Button2Click(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mSettingsBtnClick(TObject *Sender)
{
	//Open settings form
    if(!mSettingsForm)
    {
		mSettingsForm = new TSettingsForm(*this);
    }

	this->Visible = true;
    mSettingsForm->Show();

}



//---------------------------------------------------------------------------
void __fastcall TMainForm::mFrontBackLEDBtnClick(TObject *Sender)
{
	TArrayBotButton* b = dynamic_cast<TArrayBotButton*>(Sender);
    if(b == mFrontBackLEDBtn)
    {
    	mLightsArduinoClient.toggleLED();
    }

    if(b == mToggleCoaxBtn)
    {
    	mLightsArduinoClient.toggleCoax();
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::LogLevelCBChange(TObject *Sender)
{
	if(LogLevelCB->ItemIndex == -1)
    {
    	return;
    }

    string lvl = stdstr(LogLevelCB->Items->Strings[LogLevelCB->ItemIndex]);
    mLogLevel = toLogLevel(lvl);
    gLogger.setLogLevel(mLogLevel);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mAddImageFileBtnClick(TObject *Sender)
{
	TButton* b = dynamic_cast<TButton*>(Sender);

    if(b == mAddImageFileBtn)
    {
		//Browse for file
        string f = browseForFile(mSnapShotFolder);
        if(!f.size())
        {
            return;
        }

        //Add this file to DB and open a metadata entry form for the user
        try
        {
            mClientDBSession.insertImageFile(getFileNameNoPath(f), getCurrentUserID(mUsersCB), "Note..");
            DBNavigator1->BtnClick(nbRefresh);
            DBNavigator1->BtnClick(nbRefresh);
        }
        catch(...)
        {
            handleSQLiteException();
        }
    }
}

void TMainForm::stopSounds()
{
    mGetReadyForZeroCutSound.stop();
    mRestoreFromZeroCutSound.stop();
    mSetZeroCutSound.stop();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FormShow(TObject *Sender)
{
	string dBase(mLocalDBFile);
    if(!fileExists(dBase))
    {
	  	Log(lError) << "The db file: "<<dBase<<" do not exist!";
    }
    else
    {
        if (ImagesAndMoviesDM->Connect(dBase))
        {
           // Connection successfull
            Log(lInfo) << "DataModule connected to the database: "<<dBase;
            populateUsersCB(mUsersCB, mClientDBSession);
        }
        else
        {
            Log(lInfo) << "Datamodule failed to connect to database: "<<dBase;
        }
    }
    gAppIsStartingUp = false;
}

void __fastcall TMainForm::DBMemo1Change(TObject *Sender)
{
	//Enable update button
    mUpdateNoteBtn->Enabled = true;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mUpdateNoteBtnClick(TObject *Sender)
{
	//Apply and post updates to database
	ImagesAndMoviesDM->notesCDS->ApplyUpdates(-1);
    mUpdateNoteBtn->Enabled = false;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::DBNavigator2Click(TObject *Sender, TNavigateBtn Button)
{
	//
}


void __fastcall TMainForm::DBNavigator2BeforeAction(TObject *Sender, TNavigateBtn Button)

{
	if(Button == nbInsert)
    {    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mNewNoteBtnClick(TObject *Sender)
{
    int imageID = ImagesAndMoviesDM->imagesCDS->Fields->FieldByName("id")->Value;
    Log(lInfo) <<"Current image note ID: "<<imageID;

	TButton* b = dynamic_cast<TButton*>(Sender);

	try
    {
        if(b == mNewNoteBtn)
        {
        	int userID = getCurrentUserID(mUsersCB);
            ImagesAndMoviesDM->notesCDS->Active = false;
            mClientDBSession.insertImageNote(imageID, userID, "New note..");
            ImagesAndMoviesDM->notesCDS->Active = true;
            ImagesAndMoviesDM->notesCDS->Refresh();
        }
        else if(b == mDeleteNoteBtn)
        {
            int noteID = ImagesAndMoviesDM->notesCDS->Fields->FieldByName("id")->Value;
            Log(lInfo) << "Removing image note with ID: "<<noteID;
            ImagesAndMoviesDM->notesCDS->Active = false;
            mClientDBSession.deleteNoteWithID(noteID);
            ImagesAndMoviesDM->notesCDS->Active = true;
            ImagesAndMoviesDM->notesCDS->Refresh();
        }
    }
    catch(...)
    {
    	handleSQLiteException();
    }

}

void __fastcall TMainForm::DBMemo1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	mUpdateNoteBtn->Enabled = true;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mImagesGridCellClick(TColumn *Column)
{
	//Retrieve file name and show the image
	String f =	ImagesAndMoviesDM->imagesCDS->FieldByName("file_name")->AsString;
    string fName = joinPath(mSnapShotFolder, stdstr(f));

    if(fileExists(fName))
    {
		Image1->Picture->LoadFromFile(fName.c_str());
    }
    else
    {
    	Log(lError) << "The file: "<<fName<<" could not be found";
    }


	mNotesGrid->Width = mNotesGrid->Width + 1;
	mNotesGrid->Width = mNotesGrid->Width - 1;
}

void TMainForm::populateUsers()
{
    try
    {
    	populateUsersCB(mUsersCB, mClientDBSession);
    }
    catch(...)
    {
    	handleSQLiteException();
    }
}

void __fastcall TMainForm::mImagesGridDblClick(TObject *Sender)
{
//    //Check if we have a valid file
//    string fName = stdstr(lb->Items->Strings[lb->ItemIndex]);
//    string fldr =  mSnapShotFolder;
//
//    fName = joinPath(fldr, fName);
//    if(fileExists(fName))
//    {
//        ShellExecuteA(NULL, NULL, stdstr(fName).c_str(), 0, 0, SW_SHOWNA);
//    }
}

void __fastcall TMainForm::Panel3Resize(TObject *Sender)
{
	mOneToOneBtn->Width = Panel3->Width / 2;
	mOneToTwoBtn->Width = Panel3->Width / 2;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Button1Click(TObject *Sender)
{
	ImagesAndMoviesDM->sensorsCDS->Active = false;
   	ImagesAndMoviesDM->sensorsCDS->Active = true;
}



//---------------------------------------------------------------------------
void __fastcall TMainForm::mMoviesLBDblClick(TObject *Sender)
{
//	TListBox* lb = dynamic_cast<TListBox*>(Sender);
//    if(lb == mMoviesLB)
//    {
//        //Check if we have a valid file
//        string fName = stdstr(lb->Items->Strings[lb->ItemIndex]);
//        string fldr =  mMoviesFolder;
//
//        fName = joinPath(fldr, fName);
//        if(fileExists(fName))
//        {
//            ShellExecuteA(NULL, NULL, stdstr(fName).c_str(), 0, 0, SW_SHOWNA);
//        }
//    }
//    else if (lb == mShotsLB)
//    {
//        //Check if we have a valid file
//        string fName = stdstr(lb->Items->Strings[lb->ItemIndex]);
//        string fldr =  mSnapShotFolder;
//
//        fName = joinPath(fldr, fName);
//        if(fileExists(fName))
//        {
//            ShellExecuteA(NULL, NULL, stdstr(fName).c_str(), 0, 0, SW_SHOWNA);
//        }
//    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Delete1Click(TObject *Sender)
{
//    TListBox* lb = dynamic_cast<TListBox*>(mMediaPopup->PopupComponent);
//
//   	if(lb && lb->ItemIndex == -1)
//    {
//		return;
//    }
//
//    if(lb == mMoviesLB)
//    {
//        //Delete current selected item
//        string fName = stdstr(lb->Items->Strings[lb->ItemIndex]);
//        string fldr  = mMoviesFolder;
//
//        fName = joinPath(fldr, fName);
//        if(fileExists(fName))
//        {
//            removeFile(fName);
//            updateVideoFileLB();
//        }
//    }
//    else if (lb == mShotsLB)
//    {
//        //Delete current selected item
//        string fName = stdstr(lb->Items->Strings[lb->ItemIndex]);
//        string fldr =  mSnapShotFolder;
//
//        fName = joinPath(fldr, fName);
//        if(fileExists(fName))
//        {
//            removeFile(fName);
//            updateShotsLB();
//        }
//    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::DeleteAll1Click(TObject *Sender)
{
//    TListBox* lb = dynamic_cast<TListBox*>(mMediaPopup->PopupComponent);
//
//   	if(lb && lb->ItemIndex == -1)
//    {
//		return;
//    }
//
//    if(lb == mMoviesLB)
//    {
//        while(lb->Count)
//        {
//            string fName = stdstr(lb->Items->Strings[0]);
//            string fldr =  mMoviesFolder;
//            fName = joinPath(fldr, fName);
//            if(fileExists(fName))
//            {
//                removeFile(fName);
//                updateVideoFileLB();
//            }
//        }
//    }
//    else if (lb == mShotsLB)
//    {
//        while(lb->Count)
//        {
//            string fName = stdstr(lb->Items->Strings[0]);
//            string fldr =  mSnapShotFolder;
//            fName = joinPath(fldr, fName);
//            if(fileExists(fName))
//            {
//                removeFile(fName);
//                updateShotsLB();
//            }
//        }
//    }
}

