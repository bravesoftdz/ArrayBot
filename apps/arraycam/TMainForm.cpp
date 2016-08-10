#include <vcl.h>
#pragma hdrstop
#include "TMainForm.h"
#include "mtkLogger.h"
#include "mtkVCLUtils.h"
#include "mtkWin32Utils.h"
#include "mtkUtils.h"
#include "camera/uc480_tools.h"
#include "TSettingsForm.h"

using namespace mtk;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TPropertyCheckBox"
#pragma link "mtkFloatLabel"
#pragma resource "*.dfm"
TMainForm *MainForm;

extern string gLogFileName;
extern string gApplicationRegistryRoot;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TRegistryForm(gApplicationRegistryRoot, "MainForm", Owner),
    	mLogFileReader(joinPath(getSpecialFolder(CSIDL_LOCAL_APPDATA), "ArrayBot", gLogFileName), &logMsg),
        mCaptureVideo(false),
        mAVIID(0)
{
   	mLogFileReader.start(true);

	//Camera rendering mode
    mRenderMode = IS_RENDER_FIT_TO_WINDOW;

	mArduinoClient.assignOnMessageReceivedCallBack(onArduinoMessageReceived);
}

//This one is called from the reader thread
void __fastcall TMainForm::logMsg()
{
    infoMemo->Lines->Insert(0, (vclstr(mLogFileReader.getData())));
    mLogFileReader.purge();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mCameraStartLiveBtnClick(TObject *Sender)
{
        Log(lDebug) << "Init camera..";
        //Live
        if(!mCamera.IsInit())
        {
            openCamera();
        }

        if(mCamera.IsInit())
        {
        	int x, y;
       		mCamera.GetMaxImageSize(&x,&y);
            Log(lInfo) << "Max image size (x,y): ("<<x<<", "<<y<<")";
            mCamera.CaptureVideo( IS_WAIT );
        }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	mDisplayHandle 	= this->mCameraStreamPanel->Handle;
	mCameraStartLiveBtnClick(Sender);

    mAutoGainCB->Checked = true;
    mAutoExposureCB->Checked = true;

	mCameraStreamPanel->Width = 1280;
	mCameraStreamPanel->Height = 1024;
	mCameraStreamPanel->Top = 0;
	mCameraStreamPanel->Left = 0;
	mFitToScreenButtonClick(Sender);
	updateVideoFileLB();
	updateShotsLB();

	mArduinoClient.connect(50000);
}

LRESULT TMainForm::OnUSBCameraMessage(TMessage msg)
{
    switch ( msg.WParam )
    {
        case IS_DEVICE_REMOVED:            Beep( 400, 50 );        break;
        case IS_DEVICE_RECONNECTED:        Beep( 400, 50 );        break;

        case IS_FRAME:
            if(mCamera.mImageMemory != NULL)
            {
                mCamera.RenderBitmap(mCamera.mMemoryId, mDisplayHandle, mRenderMode);
            }
        break;
    }

    return 0;
}

//---------------------------------------------------------------------------
bool TMainForm::openCamera()
{
	mCamera.openCamera(this->Handle);
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

HWND TMainForm::GetSafeHwnd()
{
	return this->Handle;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	if(mLogFileReader.isRunning() || mCamera.IsInit())
    {
        CanClose = false;
        mShutDownTimer->Enabled = true;
        return;
    }

    CanClose = true;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mShutDownTimerTimer(TObject *Sender)
{
    mShutDownTimer->Enabled = false;
	if(mLogFileReader.isRunning())
    {
		mLogFileReader.stop();
    }

    if( mCamera.IsInit())
    {
    	mCamera.exitCamera();
    }

    Close();
}

void __fastcall TMainForm::TrackBar1Change(TObject *Sender)
{
	//Set brightness
	/* set gamma value to 1.6 */
//	INT nGamma = TrackBar1->Position;
//    HCAM hCam = mCamera.GetCameraHandle();
//	INT nRet = is_Gamma(hCam, IS_GAMMA_CMD_SET, (void*) &nGamma, sizeof(nGamma));
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::AutoParaCBClick(TObject *Sender)
{
    HCAM hCam = mCamera.GetCameraHandle();

    TCheckBox* cb = dynamic_cast<TCheckBox*>(Sender);

    double dEnable;
	int ret;
    if(cb)
    {
    	dEnable = cb->Checked ? 1 : 0;
    }

    if(cb == mAutoGainCB)
    {
	    //Enable auto gain control:
	    ret = is_SetAutoParameter (hCam, IS_SET_ENABLE_AUTO_GAIN, &dEnable, 0);
    }
    else if (cb == mAutoExposureCB)
    {
	    //Enable auto gain control:
	    ret = is_SetAutoParameter (hCam, IS_SET_ENABLE_AUTO_SHUTTER, &dEnable, 0);
    }

    //Check return value;

    //Set brightness setpoint to 128:
    double nominal = 128;
    ret = is_SetAutoParameter (hCam, IS_SET_AUTO_REFERENCE, &nominal, 0);

//    //Return shutter control limit:
//    double maxShutter;
//    ret = is_SetAutoParameter (hCam, IS_GET_AUTO_SHUTTER_MAX, &maxShutter, 0);
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mVerticalMirrorCBClick(TObject *Sender)
{
    HCAM hCam = mCamera.GetCameraHandle();
	if(mVerticalMirrorCB->Checked)
    {
		is_SetRopEffect (hCam, IS_SET_ROP_MIRROR_LEFTRIGHT, 1, 0);
    }
    else
    {
		is_SetRopEffect (hCam, IS_SET_ROP_MIRROR_LEFTRIGHT, 0, 0);
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mHorizontalMirrorCBClick(TObject *Sender)
{
    HCAM hCam = mCamera.GetCameraHandle();
	if(mHorizontalMirrorCB->Checked)
    {
		is_SetRopEffect (hCam, IS_SET_ROP_MIRROR_UPDOWN, 1, 0);
    }
    else
    {
		is_SetRopEffect (hCam, IS_SET_ROP_MIRROR_UPDOWN, 0, 0);
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mOneToTwoBtnClick(TObject *Sender)
{
	mRenderMode = IS_RENDER_DOWNSCALE_1_2;

   	int x, y;
	mCamera.GetMaxImageSize(&x,&y);
	mCameraBackPanel->Width = x/2.;
	mCameraBackPanel->Height = y/2.;
}

void __fastcall TMainForm::mOneToOneBtnClick(TObject *Sender)
{
    mRenderMode = IS_RENDER_FIT_TO_WINDOW;
    mCameraStreamPanel->Invalidate();

   	int x, y;
	mCamera.GetMaxImageSize(&x,&y);
	mCameraBackPanel->Width  = x;
	mCameraBackPanel->Height = y;
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mFitToScreenButtonClick(TObject *Sender)
{
	//Check widths and heights
    double wRatio = (double) mMainPanel->Width / mCameraBackPanel->Width;
    double hRatio = (double) mMainPanel->Height / mCameraBackPanel->Height;

    if(hRatio < wRatio)
    {
	    mCameraBackPanel->Height = mMainPanel->Height;
        mCameraBackPanel->Width *= hRatio;
    }
    else
    {
	    mCameraBackPanel->Width = mMainPanel->Width;
        mCameraBackPanel->Height *= wRatio;
    }

    mCameraBackPanel->Invalidate();
    mCameraStreamPanel->Invalidate();
	Log(lInfo) << "W x H = " <<mCameraBackPanel->Width<<","<<mCameraBackPanel->Height<<" Ratio = "<<(double) mCameraBackPanel->Width / mCameraBackPanel->Height;
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

void __fastcall TMainForm::mSnapShotBtnClick(TObject *Sender)
{
	string fldr =  joinPath(getSpecialFolder(CSIDL_LOCAL_APPDATA), "ArrayBot", "snap_shots");

    if(!folderExists(fldr))
    {
    	createFolder(fldr);
    }

    //Count files in folder
    int nrShots = countFiles(fldr, "*.jpg") + 1;

    string fName = joinPath(fldr, mtk::toString(nrShots) + ".jpg");
	if(mCamera.SaveImage(fName.c_str()))
    {
    	Log(lError) << "Failed saving snapshot..";
    }
    else
    {
    	Log(lInfo) << "Saved snap shot to: "<< fName;
    }
	updateShotsLB();
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mRecordMovieBtnClick(TObject *Sender)
{
	if(mRecordMovieBtn->Caption == "Record Movie")
    {
        Timer1->Enabled = true;

        isavi_InitAVI(&mAVIID, mCamera.GetCameraHandle());

        int w = mCamera.mSizeX;
        int h = mCamera.mSizeY;
        int retVal = isavi_SetImageSize(mAVIID, mCamera.mColorMode, w, h, 0, 0, 0);

        if(retVal != IS_AVI_NO_ERR)
        {
            Log(lError) << "There was a SetImageSize AVI error: "<<retVal;
            return;
        }

        retVal = isavi_SetImageQuality(mAVIID, 100);
        if(retVal != IS_AVI_NO_ERR)
        {
            Log(lError) << "There was a SetImageQuality AVI error: "<<retVal;
            return;
        }

        string fldr =  joinPath(getSpecialFolder(CSIDL_LOCAL_APPDATA), "ArrayBot", "movies");

        //Count files in folder
        int nrOfMovies = countFiles(fldr, "*.avi") + 1;
        string fName = joinPath(fldr, mtk::toString(nrOfMovies) + ".avi");
        int i = 1;
        while(fileExists(fName))
        {
        	nrOfMovies = countFiles(fldr, "*.avi") + ++i;
        	fName = joinPath(fldr, mtk::toString(nrOfMovies) + ".avi");
        }

        retVal = isavi_OpenAVI(mAVIID, fName.c_str());
        if(retVal != IS_AVI_NO_ERR)
        {
            Log(lError) << "There was a OpenAVI error: "<<retVal;
            return;
        }

        retVal = isavi_SetFrameRate(mAVIID, 25);
        if(retVal != IS_AVI_NO_ERR)
        {
            Log(lError) << "There was a SetFrameRate AVI error: "<<retVal;
            return;
        }

        retVal = isavi_StartAVI(mAVIID);
        if(retVal != IS_AVI_NO_ERR)
        {
            Log(lError) << "There was a StartAVI error: "<<retVal;
            return;
        }
        mRecordMovieBtn->Caption = "Stop Recording";
    }
    else
    {
        Timer1->Enabled = false;
        mRecordMovieBtn->Caption = "Record Movie";
        int retVal = isavi_StopAVI(mAVIID);
        if(retVal != IS_AVI_NO_ERR)
        {
            Log(lError) << "There was a StopAVI error: "<<retVal;
            return;
        }

        retVal = isavi_CloseAVI(mAVIID);
        if(retVal != IS_AVI_NO_ERR)
        {
            Log(lError) << "There was a CloseAVI error: "<<retVal;
            return;
        }

        retVal = isavi_ExitAVI(mAVIID);
        if(retVal != IS_AVI_NO_ERR)
        {
            Log(lError) << "There was a ExitAVI error: "<<retVal;
            return;
        }
        updateVideoFileLB();
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Timer1Timer(TObject *Sender)
{
	//Todo: this should be executed in its own thread and not in a timer..
    mCaptureVideo = true;
    static int frames(0);

    int retVal = isavi_AddFrame(mAVIID, mCamera.mImageMemory);

    if(retVal != IS_AVI_NO_ERR)
    {
        //Log(lError) << "There was an AddFrame AVI error: "<<retVal;
    }
    else
    {
        frames++;
        Log(lInfo) << "Added frame: "<<frames;
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Button1Click(TObject *Sender)
{
	infoMemo->Clear();
}

void  TMainForm::updateVideoFileLB()
{
	string fldr =  joinPath(getSpecialFolder(CSIDL_LOCAL_APPDATA), "ArrayBot", "movies");
    StringList files = getFilesInDir(fldr, "avi");

    for(int i = 0; i < files.count(); i++)
    {
		files[i] = getFileNameNoPath(files[i]);
    }

    files.sort();

	mMoviesLB->Clear();
    for(int i = 0; i < files.count(); i++)
    {
    	mMoviesLB->AddItem( vclstr(files[i]), NULL );
    }
}

void  TMainForm::updateShotsLB()
{
	string fldr =  joinPath(getSpecialFolder(CSIDL_LOCAL_APPDATA), "ArrayBot", "snap_shots");
    StringList files = getFilesInDir(fldr, "jpg");

    for(int i = 0; i < files.count(); i++)
    {
		files[i] = getFileNameNoPath(files[i]);
    }

    files.sort();

	mShotsLB->Clear();
    for(int i = 0; i < files.count(); i++)
    {
    	mShotsLB->AddItem( vclstr(files[i]), NULL );
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::mMoviesLBDblClick(TObject *Sender)
{

	TListBox* lb = dynamic_cast<TListBox*>(Sender);

    if(lb == mMoviesLB)
    {
        //Check if we have a valid file
        string fName = stdstr(lb->Items->Strings[lb->ItemIndex]);
        string fldr =  joinPath(getSpecialFolder(CSIDL_LOCAL_APPDATA), "ArrayBot", "movies");

        fName = joinPath(fldr, fName);
        if(fileExists(fName))
        {
            ShellExecuteA(NULL, NULL, stdstr(fName).c_str(), 0, 0, SW_SHOWNA);
        }
    }
    else if (lb == mShotsLB)
    {
        //Check if we have a valid file
        string fName = stdstr(lb->Items->Strings[lb->ItemIndex]);
        string fldr =  joinPath(getSpecialFolder(CSIDL_LOCAL_APPDATA), "ArrayBot", "snap_shots");

        fName = joinPath(fldr, fName);
        if(fileExists(fName))
        {
            ShellExecuteA(NULL, NULL, stdstr(fName).c_str(), 0, 0, SW_SHOWNA);
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::Delete1Click(TObject *Sender)
{

	ShowMessage(Sender->ClassName());

	if(mMoviesLB->ItemIndex == -1)
    {
		return;
    }

	//Delete current selected movie
    string fName = stdstr(mMoviesLB->Items->Strings[mMoviesLB->ItemIndex]);
	string fldr =  joinPath(getSpecialFolder(CSIDL_LOCAL_APPDATA), "ArrayBot", "movies");

    fName = joinPath(fldr, fName);
    if(fileExists(fName))
    {
    	removeFile(fName);
        updateVideoFileLB();
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::DeleteAll1Click(TObject *Sender)
{
	TMenuItem* mi = dynamic_cast<TMenuItem*>(Sender);

    TListBox* lb = dynamic_cast<TListBox*>(mi->Owner->Owner);

    if(lb == mMoviesLB)
    {
        while(mMoviesLB->Count)
        {
            string fName = stdstr(lb->Items->Strings[0]);
            string fldr =  joinPath(getSpecialFolder(CSIDL_LOCAL_APPDATA), "ArrayBot", "movies");
            fName = joinPath(fldr, fName);
            if(fileExists(fName))
            {
                removeFile(fName);
                updateVideoFileLB();
            }
        }
    }
    else if (lb == mShotsLB)
    {
        while(mMoviesLB->Count)
        {
            string fName = stdstr(lb->Items->Strings[0]);
            string fldr =  joinPath(getSpecialFolder(CSIDL_LOCAL_APPDATA), "ArrayBot", "snap_shots");
            fName = joinPath(fldr, fName);
            if(fileExists(fName))
            {
                removeFile(fName);
                updateShotsLB();
            }
        }
    }
}

void __fastcall TMainForm::mCameraStreamPanelDblClick(TObject *Sender)
{
	this->BorderStyle = (this->BorderStyle == bsNone) ? bsSingle : bsNone;
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
	TSettingsForm* sf = new TSettingsForm(*this);
    sf->ShowModal();
    delete sf;
}

void TMainForm::onArduinoMessageReceived(const string& msg)
{
	struct TLocalArgs
    {
        string msg;
        void __fastcall onPufferArduinoMessage()
        {
            if(startsWith(msg, "DHT22DATA"))
            {
                //Parse the message
                StringList l(msg,',');
                if(l.size() == 3)
                {
                    MainForm->mTemperatureLbl->SetValue(toDouble(l[1]));
                    MainForm->mHumidityE->SetValue(toDouble(l[2]));
                }
            }
//            else if(startsWith(msg, "PIN_8"))
//            {
//                StringList l(msg,'=');
//                if(l.size() == 2)
//                {
////                    Main->mCoaxLEDBtn->Caption = l[1] == "HIGH" ? "Coax LEDs OFF" : "Coax LEDs On";
//                }
//            }
//            else if(startsWith(msg, "PIN_3"))
//            {
//                StringList l(msg,'=');
//                if(l.size() == 2)
//                {
//  //                  Main->mFrontBackLEDBtn->Caption = l[1] == "HIGH" ? "Front/Back LEDs OFF" : "Front/Back LEDs On";
//                }
//            }
        }
    };

    TLocalArgs args;
    args.msg = msg;

    //This causes this fucntion to be called in the UI thread
	TThread::Synchronize(NULL, &args.onPufferArduinoMessage);
}


//---------------------------------------------------------------------------
void __fastcall TMainForm::mFrontBackLEDBtnClick(TObject *Sender)
{
	TButton* b = dynamic_cast<TButton*>(Sender);
    if(b == mFrontBackLEDBtn)
    {
    	mArduinoClient.toggleLED();
    }

    if(b == mToggleCoaxBtn)
    {
    	mArduinoClient.toggleCoax();
    }
}

//---------------------------------------------------------------------------
void __fastcall TMainForm::LightTBChange(TObject *Sender)
{
	TTrackBar* tb = dynamic_cast<TTrackBar*>(Sender);
    if(tb == mFrontLEDTB)
    {
    	int nr = tb->Position;
        stringstream s;
        s<<"SET_FRONTLED_INTENSITY="<<nr;
        mArduinoClient.request(s.str());
    }
    else if(tb == mBackLEDTB)
    {
    	int nr = tb->Position;
        stringstream s;
        s<<"SET_BACKLED_INTENSITY="<<nr;
        mArduinoClient.request(s.str());
    }
    else if(tb == mCoaxTB)
    {
    	int nr = tb->Position;
        stringstream s;
        s<<"SET_COAX_INTENSITY="<<nr;
        mArduinoClient.request(s.str());
    }

}


