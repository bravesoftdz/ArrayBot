#pragma hdrstop
#pragma argsused
#include "abArduinoServer.h"
#include "mtkLogger.h"
#include "mtkUtils.h"
#include "abArduinoDevice.h"
#include <signal.h>

using namespace mtk;

bool gKeepGoing(true);
void my_handler(int s)
{
	Log(lInfo) << "Caught signal: "<< s;
	gKeepGoing = false;
}

int main()
{
	mtk::gLogger.setLogLevel(lDebug4);
    mtk::LogOutput::mShowLogTime  		= true;
    mtk::LogOutput::mLogToConsole  		= true;

	signal (SIGINT,my_handler);

    ArduinoDevice a1(3,  115200);
	try
    {
    	int count(0);
        while(a1.isConnected() && gKeepGoing == true)
        {
            while(a1.hasMessage())
            {
            	string msg = a1.getMessage();
            	Log(lInfo) << "RCVD: "<<msg;
            }

            if(count%10 == 1)
            {
            	a1.postMessage("[HELLO]");
            }

            count++;
//	        sleep(10);
        }

        a1.disConnect();
    }
//        while(s.isRunning())
//        {
//            Log(lDebug) <<"Server HeartBeat";
//            IPCMessage msg(-1, "Server HeartBeat");
////            s.broadcast(msg);
//            sleep(5000);
//        }
//    }
    catch(...)
    {
    	Log(lError) << "Something bad happened";
    }

    return 0;
}

#pragma comment(lib, "mtkCommon.lib")
#pragma comment(lib, "mtkIPC.lib")
#pragma comment(lib, "poco_foundation-static.lib")
#pragma comment(lib, "abCore.lib")

