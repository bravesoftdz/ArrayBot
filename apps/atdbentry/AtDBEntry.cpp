#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
#include <string>
#include <Vcl.Styles.hpp>
#include <Vcl.Themes.hpp>
#include "mtkLogger.h"
#include "mtkVCLUtils.h"
#include "abExceptions.h"
#include "TATDBEntrySplashForm.h"
#include "mtkRestartApplicationUtils.h"

//---------------------------------------------------------------------------
USEFORM("forms\TAboutATDBEntry.cpp", AboutATDBEntry);
USEFORM("MainForm.cpp", Main);
USEFORM("frames\TAboutATDBEntryFrame.cpp", AboutATDBDataEntryFrame); /* TFrame: File Type */

//---------------------------------------------------------------------------
using namespace mtk;
using namespace std;

extern string		gApplicationName			= "ATDBData Entry";
extern string       gLogFileLocation            = "";
extern string       gLogFileName                = "ATDBDataEntry.log";
extern string 		gApplicationRegistryRoot  	= "\\Software\\Allen Institute\\ATDBDataEntry\\0.5.0";
extern string 		gAppDataFolder 				= joinPath(getSpecialFolder(CSIDL_LOCAL_APPDATA), "ArrayBot");
static HWND         gOtherAppWindow             = NULL;
extern string       gDefaultAppTheme            = "Iceberg Classico";
extern string       gRestartMutexName           = "ATDBDataEntryRestartMutex";
extern string       gFullDateTimeFormat         = "%Y-%m-%dT%H:%M:%S";
extern string       gDateFormat                 = "%Y-%m-%d";
extern string       gTimeFormat                 = "%H:%M:%S";

extern string       gCommonAppDataLocation      = "";
extern bool         gIsDevelopmentRelease       = false;
extern bool         gAppIsStartingUp            = true;

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
        const char appMutexName [] = "atdbDataEntryAppMutex";
        appMutex = ::CreateMutexA(NULL, FALSE, appMutexName);
        if( ERROR_ALREADY_EXISTS == GetLastError() )
        {
             Log(lInfo) << "ArduinoController is already running!";
            // Program already running somewhere
            ::EnumWindows(FindOtherWindow, NULL);

            if(gOtherAppWindow != NULL)
            {
                //Send a custom message to restore window here..
                Log(lInfo) << "ATDBDataEntry is already running!";
            }

            return(1); // Exit program
        }

        setupLogging();

		Application->Initialize();
		Application->MainFormOnTaskBar = true;

		TStyleManager::TrySetStyle("Iceberg Classico");
		Application->Title = gApplicationName.c_str();
		Application->CreateForm(__classid(TMain), &Main);
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

//---------------------------------------------------------------------------
#if defined DSL_STATIC
	#pragma comment(lib, "mtkCommon-static.lib")
	#pragma comment(lib, "mtkMath-static.lib")
	#pragma comment(lib, "mtkIPC-static.lib")
#else
	#pragma comment(lib, "mtkCommon.lib")
	#pragma comment(lib, "mtkMath.lib")
	#pragma comment(lib, "mtkIPC.lib")
#endif

#pragma comment(lib, "abCore.lib")

#pragma comment(lib, "libmysqlB.lib")

#pragma comment(lib, "poco_mysql_connector-static.lib")
#pragma comment(lib, "poco_data-static.lib")
#pragma comment(lib, "poco_foundation-static.lib")
#pragma comment(lib, "tinyxml2-static.lib")
#pragma comment(lib, "VCLCommon.bpi")
#pragma comment(lib, "DuneForms.lib")
