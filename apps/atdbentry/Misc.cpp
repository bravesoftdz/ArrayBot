#pragma hdrstop
#include "MainForm.h"
#include "mtkLogger.h"
#include "mtkVCLUtils.h"
//#include "TAboutATDBEntry.h"
#include "mtkLogger.h"
using namespace mtk;

static HWND gOtherAppWindow = NULL;
extern string gApplicationName;

//void __fastcall	TMain::OnException()
//{
//	Log(lInfo) << "Exception TMain::OnException()";
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMain::ApplicationEvents1Exception(TObject *Sender, Exception *E)
//{
//	Log(lInfo) << "Application Exception...."<<stdstr(E->Message);
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMain::AppInBox(mlxStructMessage &msg)
//{
//    if(msg.lparam == NULL)
//    {
//        return;
//    }
//
//    try
//    {
//        ApplicationMessageEnum aMsg = msg.lparam->mMessageEnum;
//
//        switch(aMsg)
//        {
//            case abSplashWasClosed:
//                Log(lDebug2) << "Splash form sent message that it was closed";
////                gSplashForm = NULL;
//            break;
//
//            default:
//            break ;
//        }
//	}
//	catch(...)
//	{
//		Log(lError) << "Received an unhandled windows message!";
//	}
//}
//
//void __fastcall TMain::LogLevelCBChange(TObject *Sender)
//{
//	if(LogLevelCB->ItemIndex == -1)
//    {
//    	return;
//    }
//
//    string lvl = stdstr(LogLevelCB->Items->Strings[LogLevelCB->ItemIndex]);
//
//    mLogLevel = toLogLevel(lvl);
//    gLogger.setLogLevel(mLogLevel);
//}
//
////---------------------------------------------------------------------------
//void __fastcall TMain::FormShow(TObject *Sender)
//{
//	this->Height = this->Height - 1;
//	this->Height = this->Height + 1;
//}

//This one is called from the reader thread
void __fastcall TMain::logMsg()
{
    infoMemo->Lines->Insert(0, (vclstr(mLogFileReader.getData())));
    mLogFileReader.purge();
}

//---------------------------------------------------------------------------
void __fastcall TMain::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(Key == vkEscape)
    {
    	Close();
    }
}


//void TMain::setupWindowTitle()
//{
//	string title = createWindowTitle(gApplicationName, Application);
//	this->Caption = vclstr(title);
//}

//---------------------------------------------------------------------------
//void __fastcall TMain::mAboutBtnClick(TObject *Sender)
//{
//	//Show about frame
//    TAboutATDBEntry* af = new TAboutATDBEntry(this);
//    af->ShowModal();
//    delete af;
//}

//BOOL CALLBACK FindOtherWindow(HWND hwnd, LPARAM lParam)
//{
//	int length = ::GetWindowTextLength( hwnd );
//	if(!length )
//	    return TRUE;
//
//	TCHAR* buffer;
//    buffer = new TCHAR[length + 1];
//	GetWindowText(hwnd, buffer, length + 1);
//
//    string s(stdstr(buffer));
//	if(startsWith(gApplicationName, s))
//	{
//		// do something with hwnd here
//		gOtherAppWindow = hwnd;
//        SetFocus(hwnd);
//        delete buffer;
//
//        //Stop enumerating..
//		return FALSE;
//	}
//
//	return TRUE;
//}
//
//void __fastcall TMain::mClearLogMemoBtnClick(TObject *Sender)
//{
//	infoMemo->Clear();
//}
