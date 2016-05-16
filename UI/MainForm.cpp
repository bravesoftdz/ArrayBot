#include <vcl.h>
#pragma hdrstop
#include "MainForm.h"
#include "TMemoLogger.h"
#include "mtkStringList.h"
#include "abUtilities.h"
#include "abAPTMotor.h"
#include "abTCubeDCServo.h"
#include "mtkVCLUtils.h"
#include "mtkLogger.h"
#include <bitset>
#include "mtkMathUtils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TIntegerLabeledEdit"
#pragma link "mtkFloatLabeledEdit"
#pragma link "TSTDStringLabeledEdit"
#pragma link "abXYZUnitFrame"
#pragma link "abMotorFrame"
#pragma resource "*.dfm"
TMain *Main;

extern string       gLogFileLocation;
extern string       gLogFileName;

using namespace mtk;
//---------------------------------------------------------------------------
__fastcall TMain::TMain(TComponent* Owner)
:
	TRegistryForm("Test", "MainForm", Owner),
	logMsgMethod(&logMsg),
	mLogFileReader(joinPath(getSpecialFolder(CSIDL_LOCAL_APPDATA), "ArrayBot", gLogFileName), logMsgMethod),
    mIniFile("ArrayBot.ini"),
    mJoyStick((int) Handle),
    mCoverSlip("CoverSlip Unit", mIniFile),
    mWhisker("Whisker Unit", mIniFile)

{
	TMemoLogger::mMemoIsEnabled = false;
}

__fastcall TMain::~TMain()
{}

//---------------------------------------------------------------------------
void __fastcall TMain::checkForDevicesExecute(TObject *Sender)
{
	if(!buildDeviceList())
    {
    	Log(lError) <<"Failed building device list";
        return;
    }

    for(int i = 0; i < 90; i++)
    {
		StringList serials = getSerialsForDeviceType((DeviceTypeID) i);
        for(u_int j = 0; j < serials.count(); j++)
        {
            Log(lInfo) <<"Found device of type '"<<::toString((DeviceTypeID) i)<<"' with serial: " <<serials[j];
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TMain::FormCreate(TObject *Sender)
{
	TMemoLogger::mMemoIsEnabled = true;
	mLogFileReader.start(true);
	InitializeUnitsAExecute(NULL);

    //Select joystick control for CoverSlip Unit
	JoyControlRG->ItemIndex = 0;
    JoyControlRG->OnClick(NULL);

    //Select medium speed
	JSSpeedsRG->ItemIndex = 1;
    JSSpeedsRG->OnClick(NULL);
}

void __fastcall TMain::InitializeUnitsAExecute(TObject *Sender)
{
	mCoverSlip.initialize();
	mWhisker.initialize();

	TXYZUnitFrame1->assignUnit(&mCoverSlip);
	TXYZUnitFrame2->assignUnit(&mWhisker);

    //JoyStick stuff.....
    mMaxXYJogVelocityJoystick->SetNumber(mJoyStick.getXAxis().getMaxVelocity());
    mXYJogAccelerationJoystick->SetNumber(mJoyStick.getXAxis().getAcceleration());

    if(mCoverSlip.getZMotor())
    {
    	mMaxZJogVelocityJoystick->SetNumber(mCoverSlip.getZMotor()->getVelocity());
    	mZJogAccelerationJoystick->SetNumber(mCoverSlip.getZMotor()->getAcceleration());
    }

	mJoyStick.connect();
    InitCloseBtn->Action = ShutDownA;
}

void __fastcall TMain::ShutDownAExecute(TObject *Sender)
{
	StatusTimer->Enabled = false;

    mJoyStick.disable();

    TXYZUnitFrame1->disable();
    TXYZUnitFrame2->disable();

    //The shutdown disconnects all devices
	mCoverSlip.shutDown();
	mWhisker.shutDown();
    InitCloseBtn->Action = InitializeUnitsA;
}

//---------------------------------------------------------------------------
void __fastcall TMain::JoyControlRGClick(TObject *Sender)
{
	//Update joystick control
    if(JoyControlRG->ItemIndex == 0)
    {
    	mCoverSlip.enableJoyStick(&mJoyStick);
        mJoyStick.getXAxis().setSenseOfDirection(1);
        mWhisker.disableJoyStick();
		mJoyStick.enable();
    }
    else if(JoyControlRG->ItemIndex == 1)
    {
        mWhisker.enableJoyStick(&mJoyStick);
        mJoyStick.getXAxis().setSenseOfDirection(-1);
		mCoverSlip.disableJoyStick();
        mJoyStick.enable();
    }
    else
    {
      	mCoverSlip.disableJoyStick();
        mWhisker.disableJoyStick();
   		mJoyStick.disable();
    }
}

void __fastcall TMain::stopAllAExecute(TObject *Sender)
{
    mJoyStick.disable();
    mCoverSlip.stopAll();
    mWhisker.stopAll();

}

//---------------------------------------------------------------------------
void __fastcall TMain::JoyStickValueEdit(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if(Key != vkReturn)
    {
    	return;
    }

	mtkFloatLabeledEdit* e = dynamic_cast<mtkFloatLabeledEdit*>(Sender);
    if(e == mMaxXYJogVelocityJoystick)
    {
        double vel = mMaxXYJogVelocityJoystick->GetValue();
        Log(lInfo) << "New jog velocity (mm/s): " <<vel;
		mJoyStick.getXAxis().setMaxVelocity(vel);
		mJoyStick.getYAxis().setMaxVelocity(vel);
    }
    else if(e == mXYJogAccelerationJoystick)
    {
        double a = mXYJogAccelerationJoystick->GetValue();
        Log(lInfo) << "New jog acceleration (mm/(s*s)): " <<a;
        mCoverSlip.getXMotor()->setJogAcceleration(a);
        mCoverSlip.getYMotor()->setJogAcceleration(a);
        mWhisker.getXMotor()->setJogAcceleration(a);
        mWhisker.getYMotor()->setJogAcceleration(a);
    }
    else if(e == mMaxZJogVelocityJoystick)
    {
        double v = mMaxZJogVelocityJoystick->GetValue();
        Log(lInfo) << "New Z jog velocity (mm/s): " <<v;
        mCoverSlip.getZMotor()->setJogVelocity(v);
        mWhisker.getZMotor()->setJogVelocity(v);
    }
    else if(e == mZJogAccelerationJoystick)
    {
        double a = mZJogAccelerationJoystick->GetValue();
        Log(lInfo) << "New Z jog acceleration (mm/(s*s)): " <<a;
        mCoverSlip.getZMotor()->setJogAcceleration(a);
        mWhisker.getZMotor()->setJogAcceleration(a);
    }
    else if(Sender == NULL) //Manually update velocities
    {
        double vel = mMaxXYJogVelocityJoystick->GetValue();
        double aXY = mXYJogAccelerationJoystick->GetValue();
        Log(lInfo) << "New jog velocity (mm/s): " <<vel;
        Log(lInfo) << "New jog acceleration (mm/(s*s)): " <<aXY;
		mJoyStick.getXAxis().setMaxVelocity(vel);
		mJoyStick.getYAxis().setMaxVelocity(vel);

        double vZ  = mMaxZJogVelocityJoystick->GetValue();
   	    double aZ  = mZJogAccelerationJoystick->GetValue();

        Log(lInfo) << "New Z jog acceleration (mm/(s*s)): " <<aZ;
        Log(lInfo) << "New Z jog velocity (mm/s): " <<vZ;
        if(mCoverSlip.getXMotor() && mCoverSlip.getYMotor())
        {
            mCoverSlip.getXMotor()->setJogAcceleration(aXY);
            mCoverSlip.getYMotor()->setJogAcceleration(aXY);

            mCoverSlip.getXMotor()->setPotentiometerVelocity(vel);
            mCoverSlip.getYMotor()->setPotentiometerVelocity(vel);
        }
        if(mWhisker.getXMotor() && mWhisker.getYMotor())
        {
            mWhisker.getXMotor()->setJogAcceleration(aXY);
            mWhisker.getYMotor()->setJogAcceleration(aXY);
        }

        if(mCoverSlip.getZMotor())
        {
        	mCoverSlip.getZMotor()->setJogVelocity(vZ);
	        mCoverSlip.getZMotor()->setJogAcceleration(aZ);
        }

        if(mWhisker.getZMotor())
        {
	        mWhisker.getZMotor()->setJogVelocity(vZ);
        	mWhisker.getZMotor()->setJogAcceleration(aZ);
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TMain::moveEdit(TObject *Sender, WORD &Key, TShiftState Shift)
{
	//Depending on which edit was edited, update the other ones
	if(Key != vkReturn)
    {
    	return;
    }

	mtkFloatLabeledEdit* e = dynamic_cast<mtkFloatLabeledEdit*>(Sender);

    if(e == mMoveVelocityVerticalE || e == mMoveAngleE)
    {
    	//Update horiz value using the angle
        double tanTheta = tan(toRadians(mMoveAngleE->GetValue()));
        if(tanTheta != 0.0)
        {
        	mMoveVelHorizE->SetNumber(mMoveVelocityVerticalE->GetValue()/tanTheta);
        }
        else
        {
			mMoveVelHorizE->SetNumber(0.0);
        }
    }
}

void __fastcall TMain::MoveBtnClick(TObject *Sender)
{
    APTMotor* yCSM 	= mCoverSlip.getYMotor();
	APTMotor* zCSM 	= mCoverSlip.getZMotor();

    APTMotor* yWM 	= mWhisker.getYMotor();
	APTMotor* zWM 	= mWhisker.getZMotor();

	if(!yCSM || !zCSM || !yWM || !zWM)
    {
    	Log(lError) << "Can't carry out this move.. one motor is absent";
        return;
    }

    double vertVel 	= mMoveVelocityVerticalE->GetNumber();
    double horizVel = mMoveVelHorizE->GetNumber();
    double acc 		= mMoveAccelerationE->GetNumber();

	//Update motors with current parameters and start the move
    zCSM->setJogVelocity(vertVel);
    zCSM->setJogAcceleration(acc);

    zWM->setJogVelocity(vertVel);
    zWM->setJogAcceleration(acc);

    double tanTheta = tan(toRadians(mMoveAngleE->GetValue()));
    yCSM->setJogVelocity(horizVel);
    yCSM->setJogAcceleration(acc / tanTheta);

    yWM->setJogVelocity(horizVel);
    yWM->setJogAcceleration(acc / tanTheta);

    //get current positions and carry out some moveTo's
    double yPos = yCSM->getPosition();
    double zPos = zCSM->getPosition();

	double newCSZPos = zPos + mVerticalMoveDistanceE->GetValue();
	double newCSYPos = (tanTheta != 0) ?
    					yPos + (newCSZPos - zPos) / tanTheta : yPos;

	//Calculate for the whisker
    yPos = yWM->getPosition();
    zPos = zWM->getPosition();

	double newWZPos = zPos + mVerticalMoveDistanceE->GetValue();
	double newWYPos = (tanTheta != 0) ?
    					(yPos + (newWZPos - zPos) / tanTheta) : yPos;

    Log(lInfo) << "Moving CS Vertical to: "	<<newCSZPos;
    Log(lInfo) << "Moving CS Horiz to: "	<<newCSYPos;

    Log(lInfo) << "Moving W Vertical to: "	<<newWZPos;
    Log(lInfo) << "Moving W Horiz to: "		<<newWYPos;


    if(newCSYPos >=150 || newWYPos >=150 )
    {
    	Log(lError) << "New CoverSlip or Whisker Y position to big: "<<newCSYPos;
        return;
    }

    if(newCSZPos >=25 || newWZPos >=25)
    {
    	Log(lError) << "New CoverSlip or Whisker Z position to big: "<<newWYPos;
        return;
    }

    yCSM->moveToPosition(newCSYPos);
    zCSM->moveToPosition(newCSZPos);

    yWM->moveToPosition(newWYPos);
    zWM->moveToPosition(newWZPos);
}

//---------------------------------------------------------------------------
void __fastcall TMain::JSSpeedsRGClick(TObject *Sender)
{
    unsigned short key  = 13;
    TShiftState ss;
	if(JSSpeedsRG->ItemIndex == 0) //Fast
    {
		mMaxXYJogVelocityJoystick->SetNumber(30);
		mXYJogAccelerationJoystick->SetNumber(10);
		mMaxZJogVelocityJoystick->SetNumber(2);
		mZJogAccelerationJoystick->SetNumber(1.5);
    }
    else if (JSSpeedsRG->ItemIndex == 1) //Medium
    {
		mMaxXYJogVelocityJoystick->SetNumber(10);
		mXYJogAccelerationJoystick->SetNumber(5);
		mMaxZJogVelocityJoystick->SetNumber(1.5);
		mZJogAccelerationJoystick->SetNumber(3);
    }
    else if (JSSpeedsRG->ItemIndex == 2) //Slow
    {
		mMaxXYJogVelocityJoystick->SetNumber(1);
		mXYJogAccelerationJoystick->SetNumber(1);
		mMaxZJogVelocityJoystick->SetNumber(0.5);
		mZJogAccelerationJoystick->SetNumber(0.1);
    }
	JoyStickValueEdit(NULL, key, ss);
}

//---------------------------------------------------------------------------
void __fastcall TMain::Button2Click(TObject *Sender)
{
	BottomPanel->Visible = !BottomPanel->Visible;
}



