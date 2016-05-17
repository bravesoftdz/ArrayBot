#ifndef abCoverSlipAngleControllerH
#define abCoverSlipAngleControllerH
#include "abExporter.h"
#include "abABObject.h"
#include "mtkIniFile.h"
#include "mtkProperties.h"
#include "mtkINIFileProperties.h"
#include "abDeviceManager.h"
#include <string>
//---------------------------------------------------------------------------

using std::string;
using mtk::IniFile;
using mtk::IniFileProperties;
using mtk::Property;
class APTMotor;

class AB_CORE CoverSlipAngleController : public ABObject
{
	public:
      							CoverSlipAngleController(const string& name, IniFile& iniFile);
     							~CoverSlipAngleController();

		bool				    initialize();
		void 					shutDown();
		string					getName() const;
        void					setAngle(double a);

        Property<int>		    mMotorSerial;
        Property<string>   	    mName;
    	DeviceManager	        mDeviceManager;

    private:
    	APTMotor*				mAngleMotor;
        IniFile				    mIniFile;
        IniFileProperties	    mProperties;
};

#endif
