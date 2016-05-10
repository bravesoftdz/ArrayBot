#ifndef abAPTDeviceH
#define abAPTDeviceH
#include "CoreExporter.h"
#include "abABObject.h"
#include "mtkProperties.h"
#include "abUtilities.h"
#include "mtkINIFileProperties.h"
#include "mtkIniFile.h"

using namespace mtk;
struct HardwareInformation;


//---------------------------------------------------------------------------
class AB_CORE APTDevice : public ABObject
{
	public:
    							                //Every APTDevice need to be created with a serial number
    					                        APTDevice(int serial);
		virtual                                 ~APTDevice();
        bool					                isConnected();
        virtual bool			                connect() = 0;
        virtual bool			                disconnect() = 0;
        virtual bool   	                        identify() = 0;
        virtual HardwareInformation         	getHWInfo() = 0;

                                                //Enable/Disable functions simply sets boolean to
                                                //true or false.
        bool	        		                enable();
        bool	        		                disable();

        string					                getSerial();
        bool									loadProperties(IniFile& iniFile);
        void									setName(const string& name);
        string 									getName();

    protected:
		IniFileProperties	  					mProperties;
    	Property<string>                        mSerial;
    	Property<string>						mName;

								                //DeviceTypeID enum
		DeviceTypeID		 	                mDeviceTypeID;

        						                //True if connection with hardware device is
                                                //established
        bool					                mIsConnected;

        						                //Allow the device to be enabled/disabled (in software)
        bool					                mIsActive;
};
#endif
