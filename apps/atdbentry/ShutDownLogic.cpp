#pragma hdrstop
#include "MainForm.h"
#include "mtkLogger.h"
using namespace mtk;

//---------------------------------------------------------------------------
void __fastcall TMain::ShutDownTimerTimer(TObject *Sender)
{
	ShutDownTimer->Enabled = false;
	if(mLogFileReader.isRunning())
	{
		Log(lDebug) << "Shutting down log file reader";
		mLogFileReader.stop();
        mLogFileReader.assignOnMessageCallBack(NULL);
	}

    //This will save any ini parameters in the frame
    for(int i = 0; i < mFrames.size(); i++)
    {
    	delete mFrames[i];
    }

    mFrames.clear();
	Close();
}

//---------------------------------------------------------------------------
void __fastcall TMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	Log(lInfo) << "Closing down....";

	//Check if active stuff is going on.. if so call the ShutDown in the
    //Timer fire    if(
   	CanClose = (
    			    mLogFileReader.isRunning()
    			||  mFrames.size()

                ) ? false : true;


	if(CanClose == false)
	{
		ShutDownTimer->Enabled = true;
	}
}

//---------------------------------------------------------------------------
void __fastcall TMain::FormClose(TObject *Sender, TCloseAction &Action)
{
	Log(lInfo) << "In FormClose";
}

