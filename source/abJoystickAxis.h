#ifndef abJoystickAxisH
#define abJoystickAxisH
#include "CoreExporter.h"
#include "abABObject.h"

//---------------------------------------------------------------------------

class APTMotor;
///The joystick Axis class encapsulate needed functionality in the ArrayBot API
///In short, a joystick axis have an associated motor to control, as well as a
///function that is called from the UI controlling the motor
class AB_CORE JoyStickAxis : public ABObject
{

    public:
							JoyStickAxis();
							~JoyStickAxis();
		void				enable();
   		void				disable();
		bool				isEnabled();

        void				assignMotor(APTMotor* motor);
		void				setNumberOfGears(int g);
		int  				getNumberOfGears();
        void				setMaxVelocity(double vel);
        void				setAcceleration(double vel);
		double				getMaxVelocity();
		double				getAcceleration();
        double 				getCurrentVelocity();
                            //This function is called by the UI
                            //as joystick messages are flowing in
		void 				Move(double newPosition);

        void				setSenseOfDirection(int sign);


    protected:
        bool				mIsEnabled;
        bool				mZeroInMiddle;
        int					mSenseOfDirection;
		APTMotor*			mMotor;
		int					mMaxPosition;
        int					mNumberOfGears;
        double 				mMaxVelocity;
        double 				mAcceleration;
        double 				mLastSetVelocity;
        double 				mRunningAverage;
        double 				mAlpha;
};

#endif