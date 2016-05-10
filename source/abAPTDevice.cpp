#pragma hdrstop
#include "abAPTDevice.h"
#include "mtkLogger.h"
//---------------------------------------------------------------------------


APTDevice::APTDevice(int serial)
	:
	mSerial(toString(serial)),
	mIsConnected(false),
	mDeviceTypeID(didUnknown),
    mName("<none>")
{
    mProperties.setSection(toString(serial));
    mProperties.add((BaseProperty*) &mName.setup("NAME", 			"<none>", true));
}

APTDevice::~APTDevice()
{

}

void APTDevice::setName(const string& name)
{
	mName = name;
}

string APTDevice::getName()
{
	return mName.getValue();
}

string APTDevice::getSerial()
{
	return mSerial.getValue();
}

bool APTDevice::isConnected()
{
	return mIsConnected;
}

bool APTDevice::loadProperties(IniFile& iniFile)
{
	Log(lInfo) << "Loading properties for APT device with serial: "<<mSerial;
    mProperties.setIniFile(&iniFile);
    mProperties.read();
}


