#pragma hdrstop
#include "mtkStringUtils.h"
#include "abArduinoClient.h"
#include "mtkLogger.h"
//---------------------------------------------------------------------------

using namespace mtk;

ArduinoClient::ArduinoClient()
:
mMessageProcessor(*this)
{

}

ArduinoClient::~ArduinoClient()
{
    Log(lDebug3)<<"Arduino client is shutting down..";
}

bool ArduinoClient::init(int serverPort, const string& hostname, bool connectOnInit)
{
    Log(lDebug3)<<"Setting up client";
	this->assignParent(this);

    if(connectOnInit)
    {
    	bool res = SocketClient::connect(serverPort, hostname);
    }

    return true;
}

void ArduinoClient::getStatus()
{
	request("GET_STATUS");
}

void ArduinoClient::assignOnMessageReceivedCallBack(OnMessageReceivedCB cb)
{
	mMessageProcessor.assignOnMessageReceivedCallBack(cb);
}

bool ArduinoClient::isConnected()
{
	return SocketClient::isConnected();
}

bool ArduinoClient::shutDown()
{
	mReceiver.stop();

    Log(lDebug3)<<"Stop processing messages";
	mMessageProcessor.stop();

    Log(lDebug3)<<"Closing socket in ArduinoClient";
	SocketClient::close();

    Log(lDebug3)<<"ArduinoClient was shut down";
    return true;
}

bool ArduinoClient::connect(int serverPort, const string& host)
{
	//This should be done in a thread..
    bool res = init(serverPort, host, true);

    if(!mMessageProcessor.isRunning())
    {
        mMessageProcessor.start(true);
    }

    if(isConnected())
    {
    	return mReceiver.start(true);
    }
    return false;
}

bool ArduinoClient::disConnect()
{
	mMessageProcessor.stop();
	return SocketClient::close();
}

void ArduinoClient::postMessage(const string& msg)
{
    string socketMessage = msg;
    SocketClient::send(msg);
}

