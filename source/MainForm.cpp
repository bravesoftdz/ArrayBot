#include <vcl.h>
#pragma hdrstop
#include "MainForm.h"
#include "TMemoLogger.h"
#include "TSplashForm.h"
#include "mtkStringList.h"
#include "core/atUtilities.h"
#include "arraybot/apt/atAPTMotor.h"
#include "arraybot/apt/atTCubeDCServo.h"
#include "mtkVCLUtils.h"
#include "mtkLogger.h"
#include <bitset>
#include "mtkMathUtils.h"
#include "core/atExceptions.h"
#include "sound/atSounds.h"
#include "core/atCore.h"
#include "frames/TABProcessSequencerFrame.h"
#include "frames/TXYZPositionsFrame.h"
#include "frames/TXYZUnitFrame.h"
#include "frames/TSequencerButtonsFrame.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TIntegerLabeledEdit"
#pragma link "TFloatLabeledEdit"
#pragma link "TSTDStringLabeledEdit"
#pragma link "mtkFloatLabel"
#pragma link "TAboutArrayBotFrame"
#pragma link "TAboutArrayBot_2Frame"
#pragma link "TPropertyCheckBox"
#pragma link "cspin"
#pragma link "TSoundsFrame"
#pragma link "TApplicationSoundsFrame"
#pragma resource "*.dfm"
TMain *Main;

extern string           gLogFileLocation;
extern string           gLogFileName;
extern string           gAppDataFolder;
extern TSplashForm*  	gSplashForm;
extern bool             gAppIsStartingUp;
extern string 			gApplicationRegistryRoot;

using namespace mtk;

//---------------------------------------------------------------------------
__fastcall TMain::TMain(TComponent* Owner)
:
	TRegistryForm(gApplicationRegistryRoot, "MainForm", Owner),
	mLogFileReader(joinPath(gAppDataFolder, gLogFileName), &logMsg),
    mIniFile(joinPath(gAppDataFolder, "ArrayBot.ini"), true, true),
    mLogLevel(lAny),
    mInitBotThread(),
    mAB(mIniFile, gAppDataFolder),
    mArrayCamClient(),
    mProcessSequencer(mAB, mArrayCamClient, gAppDataFolder),
	mABProcessSequencerFrame(NULL),
    mRibbonLifterFrame(NULL),
    mTheWiggler(NULL, NULL),
    mEnableSlowSpeedSound(ApplicationSound("")),
    mEnableMediumSpeedSound(ApplicationSound("")),
    mEnableFastSpeedSound(ApplicationSound("")),
	mMainPageControlChangeSound(ApplicationSound(""))
{
    //Init the CoreLibDLL -> give intra messages their ID's
	initABCoreLib();

	TMemoLogger::mMemoIsEnabled = false;
   	mLogFileReader.start(true);

    setupProperties();

    mProperties.read();
    mSoundProperties.read();

    //Give all sounds a Handle
  	mEnableSlowSpeedSound.getReference().setHandle(this->Handle);
  	mEnableMediumSpeedSound.getReference().setHandle(this->Handle);
  	mEnableFastSpeedSound.getReference().setHandle(this->Handle);
	mMainPageControlChangeSound.getReference().setHandle(this->Handle);

	mArrayCamServerPortE->update();
	mWigglerAmplitudeE->update();
	mWigglerAmplitudeStepE->update();

	mPullRelaxVelocityE->update();
	mPullRelaxAccE->update();

	//Load motors in a thread
    mInitBotThread.assingBot(&mAB);
    mInitBotThread.onFinishedInit = this->onFinishedInitBot;

    //We will setup UI frames after the bot is initialized
	mInitBotThread.start();

	WaitForDeviceInitTimer->Enabled = true;

    mTheWiggler.mMaxVelocity.setReference(&mWigglerVelocityE->getReference());
    mTheWiggler.mAmplitude.setReference(&mWigglerAmplitudeE->getReference());
    mTheWiggler.mMaxAcceleration.setReference(&mWigglerAccelerationE->getReference());

    mTheWiggler.mPullRelaxVelocity.setReference(&mPullRelaxVelocityE->getReference());
    mTheWiggler.mPullRelaxAcceleration.setReference(&mPullRelaxAccE->getReference());

	mArrayCamClient.assignOnMessageReceivedCallBack(onArrayCamMessageReceived);
    mArrayCamClient.onConnected 				= onArrayCamClientConnected;
	mArrayCamClient.onDisconnected 				= onArrayCamClientDisconnected;
}

void TMain::enableDisableUI(bool e)
{
	PageControl1->Visible = e;
	enableDisablePanel(mRightPanel, e);
	enableDisablePanel(mSequencesPanel, e);
    enableDisableGroupBox(JSGB, e);
    enableDisableGroupBox(mRibbonCreationGB, e);
}

void __fastcall TMain::WndProc(TMessage& Message)
{
    if (Message.Msg == getABCoreMessageID("MOTOR_WARNING_MESSAGE") && getABCoreMessageID("MOTOR_WARNING_MESSAGE") != 0)
    {
    	MotorMessageData* msg = reinterpret_cast<MotorMessageData*>(Message.WParam);
       	APTMotor* mtr = mAB.getMotorWithSerial(msg->mSerial);

        if(!mtr)
        {
        	//real bad....
        }

		//Handle the warning..
        if(msg->mCurrentPosition >= msg->mPositionLimits.getValue().getMax())
        {
            if(mtr)
            {
            	if(mtr->getLastCommand() != mcStopHard)
                {
            		mtr->stop();
	                //playABSound(absMotorWarning);
    	            Log(lInfo) << "Stopped motor: "<<mtr->getName();
                }
            }
        }

        if(mtr->isInDangerZone())
        {
        	//playABSound(absMotorWarning);
        }


        //Message is now consumed.. delete it
        delete msg;
    }
    else
    {
        TForm::WndProc(Message);
    }
}

//---------------------------------------------------------------------------
void __fastcall TMain::WaitForDeviceInitTimerTimer(TObject *Sender)
{
	if(!mInitBotThread.isRunning()) //Not waiting for devices any more
    {
		WaitForDeviceInitTimer->Enabled = false;

        //Create MoveSequencer frame
        mABProcessSequencerFrame = new TABProcessSequencerFrame(mProcessSequencer, gAppDataFolder, mMoveSequencesPage);
        mABProcessSequencerFrame->Parent = mMoveSequencesPage;
        mABProcessSequencerFrame->Align = alClient;
        mABProcessSequencerFrame->init();

        //The sequencer buttons frame holds shortcut buttons for preprogrammed sequences
        mSequencerButtons = new TSequencerButtonsFrame(mProcessSequencer, mSequencesPanel);
        mSequencerButtons->Parent = mSequencesPanel;
        mSequencerButtons->Align = alClient;
        mSequencerButtons->update();

        setupUIFrames();
        enableDisableUI(true);
    }
}

void __fastcall TMain::reInitBotAExecute(TObject *Sender)
{
	mAB.initialize();

	mXYZUnitFrame1->assignUnit(&mAB.getCoverSlipUnit());
	mXYZUnitFrame2->assignUnit(&mAB.getWhiskerUnit());

    //ArrayBotJoyStick stuff.....
    mMaxXYJogVelocityJoystick->setValue(mAB.getJoyStick().getX1Axis().getMaxVelocity());
    mXYJogAccelerationJoystick->setValue(mAB.getJoyStick().getX1Axis().getAcceleration());

    if(mAB.getCoverSlipUnit().getZMotor())
    {
    	mMaxZJogVelocityJoystick->setValue(mAB.getCoverSlipUnit().getZMotor()->getVelocity());
    	mZJogAccelerationJoystick->setValue(mAB.getCoverSlipUnit().getZMotor()->getAcceleration());
    }

    ReInitBotBtn->Action = ShutDownA;
}

void __fastcall TMain::stopAllAExecute(TObject *Sender)
{
	mAB.stopAll();
    mTheWiggler.stopWiggle();
}

//---------------------------------------------------------------------------
void __fastcall TMain::mLiftTimerTimer(TObject *Sender)
{
	if(mAB.isActive())
    {
    	LiftBtn->Action = abortLiftA;
    }
    else
    {
    	LiftBtn->Action = liftA;
		mLiftTimer->Enabled = false;

    	//Re-enable the joystick.
		mAB.enableJoyStick();
    }
}

//---------------------------------------------------------------------------
void __fastcall TMain::abortLiftAExecute(TObject *Sender)
{
	mAB.stopAll();
    Log(lInfo) << "The lift was aborted";

    //Re-enable the joystick.
	mAB.enableJoyStick();
}

PairedMove* TMain::getCurrentPairedMove()
{
	if(mLiftCB->ItemIndex > -1)
    {
    	return (PairedMove*) mLiftCB->Items->Objects[mLiftCB->ItemIndex];
    }
    return NULL;
}

//---------------------------------------------------------------------------
void __fastcall TMain::liftAExecute(TObject *Sender)
{
	PairedMove* pm = getCurrentPairedMove();

    pm->assignMotor1(mAB.getCoverSlipUnit().getZMotor());
    pm->assignMotor2(mAB.getWhiskerUnit().getZMotor());
    if(!pm)
    {
    	Log(lError) << "Can't carry out this move.. at least one motor is absent";
        return;
    }

    if(!pm->check())
    {
		Log(lError) << pm->getCheckMessage();
        MessageDlg(pm->getCheckMessage().c_str(), mtError, TMsgDlgButtons() << mbOK, 0);
        return;
    }

    //Re-enable the joystick after finish.
	mAB.disableJoyStick();
    pm->execute();
    mLiftTimer->Enabled = true;
   	LiftBtn->Action = abortLiftA;
}

//---------------------------------------------------------------------------
void __fastcall TMain::mLiftCBChange(TObject *Sender)
{
	//Update edits
    //Assign editbox references to Lifting parameters
	PairedMove* pm = getCurrentPairedMove();
	mMoveVelocityVerticalE->setReference(pm->mVelocity);
	mMoveAccelerationE->setReference(pm->mAcceleration);
}

//---------------------------------------------------------------------------
void __fastcall TMain::AppInBox(mlxStructMessage &msg)
{
    if(msg.lparam == NULL)
    {
        return;
    }

    try
    {
        ApplicationMessageEnum aMsg = msg.lparam->mMessageEnum;

        switch(aMsg)
        {
            case abSplashWasClosed:
                Log(lDebug2) << "Splash form sent message that it was closed";
                gSplashForm = NULL;
            break;

            case abSequencerUpdate:
                Log(lDebug2) << "Update sequencer shortcuts";
                mSequencerButtons->update();
            break;
            default:
            break ;
        }
	}
	catch(...)
	{
		Log(lError) << "Received an unhandled windows message!";
	}
}

//---------------------------------------------------------------------------
void __fastcall TMain::UIUpdateTimerTimer(TObject *Sender)
{
  	//TODO: Clean up joystick mess..

	//Check if Joystick is working and update radiogroup if not
	if(mJoyStickRG->ItemIndex != mAB.getJoyStick().getID())
    {
    	int indx = mAB.getJoyStick().getID();
        if(indx != -1)
        {
			mJoyStickRG->ItemIndex = indx;
        }
    }

	int indx = mJoyStickRG->ItemIndex;

    //Check validity
    if(!mAB.getJoyStick().isValid())
    {
        mJSStatusL->Caption = "Current joystick could not be found!";
        mJSCSBtn->Enabled = false;
    }
    else
    {
        mJSStatusL->Caption = "";
        mJSCSBtn->Enabled = true;
       	mJSCSBtn->Caption = (mAB.getJoyStick().isEnabled()) ? "Disable JS" : "Enable JS";
    }
}

//---------------------------------------------------------------------------
void __fastcall TMain::mUnitControlRGClick(TObject *Sender)
{
	if(mUnitControlRG->ItemIndex == 0)//Both W and CS
    {
        mAB.enableCoverSlipUnit();
        mAB.enableWhiskerUnit();
    }
    else if(mUnitControlRG->ItemIndex == 1)//Only CS
    {
        mAB.enableCoverSlipUnit();
        mAB.disableWhiskerUnit();
    }
    else if(mUnitControlRG->ItemIndex == 2)//Only Z
    {
        mAB.disableCoverSlipUnit();
        mAB.enableWhiskerUnit();
    }
    else
    {
        mAB.disableCoverSlipUnit();
        mAB.disableWhiskerUnit();
    }
}

//---------------------------------------------------------------------------
void __fastcall TMain::PageControl1Change(TObject *Sender)
{
	//Check what tab got selected
	if(PageControl1->TabIndex == pcMoveSequences)
    {
    	//Reload the currently selected sequence
		mABProcessSequencerFrame->mSequencesCBChange(Sender);
    }

    else if(PageControl1->TabIndex == pcMain)
    {
        mSequencesPanel->Parent = mFrontPage;
        mSequencerButtons->update();

    }
    else if(PageControl1->TabIndex == pcMotors)
    {
    	//
    }

	else if(PageControl1->TabIndex == pcAbout)
    {
		TAboutArrayBotFrame_21->populate();
    }
    mMainPageControlChangeSound.getReference().play();
}

//---------------------------------------------------------------------------
void __fastcall TMain::mASStartBtnClick(TObject *Sender)
{
	if(mASStartBtn->Caption == "Connect")
    {
    	mArrayCamClient.connect(mArrayCamServerPortE->getValue());
        mASStartBtn->Caption == "Connecting";
    }
    else
    {
    	mArrayCamClient.disConnect();
    }
}

//---------------------------------------------------------------------------
void __fastcall TMain::mSequencesPanelResize(TObject *Sender)
{
    mSequencerButtons->update();
}

//---------------------------------------------------------------------------
void __fastcall TMain::Button1Click(TObject *Sender)
{
	mArrayCamClient.startVideo();
}



