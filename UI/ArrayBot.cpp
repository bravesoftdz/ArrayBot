#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <string>
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
#include "mtkLogger.h"
#include "mtkVCLUtils.h"
#include "abExceptions.h"
#include "TSplashForm.h"
#include "mtkRestartApplicationUtils.h"
using namespace mtk;
using namespace std;

//---------------------------------------------------------------------------
USEFORM("MainForm.cpp", Main);
USEFORM("..\source\VCL\abMotorFrame.cpp", MotorFrame); /* TFrame: File Type */
USEFORM("..\source\VCL\abAddLiftForm.cpp", AddLiftSettingForm);
USEFORM("..\source\VCL\abXYZUnitFrame.cpp", XYZUnitFrame); /* TFrame: File Type */
//---------------------------------------------------------------------------
extern string       gLogFileLocation            = "";
extern string       gLogFileName                = "ArrayBot.log";
extern string 		gApplicationRegistryRoot  = "\\Software\\Allen Institute\\ArrayBot\\0.5.0";
extern string 		gAppDataFolder = joinPath(getSpecialFolder(CSIDL_LOCAL_APPDATA), "ArrayBot");
static HWND         gOtherAppWindow             = NULL;
extern string       gDefaultAppTheme            = "Iceberg Classico";
extern string       gRestartMutexName           = "AmalyticoRestartMutex";
extern string       gFullDateTimeFormat         = "%Y-%m-%dT%H:%M:%S";
extern string       gDateFormat                 = "%Y-%m-%d";
extern string       gTimeFormat                 = "%H:%M:%S";
extern string       gCommonAppDataLocation      = ""; //Filled out later
extern bool         gIsDevelopmentRelease       = false;
extern bool         gAppIsStartingUp            = true;
extern bool         gHideSplash                 = true;
extern TSplashForm* gSplashForm                 = NULL;
//extern SQLite       gDB                         ;

void setupLogging();

BOOL CALLBACK FindOtherWindow(HWND hwnd, LPARAM lParam) ;

//---------------------------------------------------------------------------
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
    //The app mutex is used to check for already running instances
    HANDLE appMutex;

	try
	{
		// Initialize restart code
		// Check if this instance is restarted and
		// wait while previos instance finish
		if (mtk::checkForCommandLineFlag("--Restart"))
		{
            //TODO: Fix this.. not working properly..
            //            MessageDlg("Wait...", mtWarning, TMsgDlgButtons() << mbOK, 0);
			mtk::WaitForPreviousProcessToFinish(gRestartMutexName);
            Sleep(1000);
		}

        //Look at this later... does not work yet
        const char appMutexName [] = "arrayBotAppMutex";
        appMutex = ::CreateMutexA(NULL, FALSE, appMutexName);
        if( ERROR_ALREADY_EXISTS == GetLastError() )
        {
             Log(lInfo) << "Arraybot is already running!";
            // Program already running somewhere
            ::EnumWindows(FindOtherWindow, NULL);

            if(gOtherAppWindow != NULL)
            {
                //Send a custom message to restore window here..
                Log(lInfo) << "Arraybot is already running!";
            }

            return(1); // Exit program
        }

        setupLogging();
		Application->Initialize();
		Application->MainFormOnTaskBar = true;


        Log(lInfo) << "The Logfile was opened..";

		Application->Title = "ArrayBot - Software for Robots";
		TStyleManager::TrySetStyle("Sapphire Kamri");
		Application->CreateForm(__classid(TMain), &Main);
		Application->CreateForm(__classid(TAddLiftSettingForm), &AddLiftSettingForm);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
    catch(const ABException& e)
    {
		Application->ShowException(&Exception(e.what()));
    }
	catch (...)
	{
		try
		{
			throw Exception("Unhandled exception");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}

void setupLogging()
{
	//Get Application folder
	string fldr =  joinPath(getSpecialFolder(CSIDL_LOCAL_APPDATA), "ArrayBot");
	if(!folderExists(fldr))
	{
		createFolder(fldr);
	}

	gLogFileLocation = fldr;

	string fullLogFileName(joinPath(gLogFileLocation, gLogFileName));
	clearFile(fullLogFileName);
	mtk::gLogger.logToFile(fullLogFileName);
	LogOutput::mShowLogLevel = true;
	LogOutput::mShowLogTime = false;
	LogOutput::mUseLogTabs = false;
//	mtk::gLogger.setLogLevel(lInfo);
	mtk::gLogger.setLogLevel(lDebug5);
	Log(lInfo) << "Logger was setup";
}

//---------------------------------------------------------------------------
#pragma comment(lib, "mtkCommon.lib")
#pragma comment(lib, "mtkMath.lib")
#pragma comment(lib, "abCore.lib")
#pragma comment(lib, "poco_foundation-static.lib")
#pragma comment(lib, "VCLCommon.bpi")
#pragma comment(lib, "DuneForms.lib")


