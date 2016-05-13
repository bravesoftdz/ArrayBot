#include <vcl.h>
#pragma hdrstop
#include "abMotorFrame.h"
#include "abAPTMotor.h"
#include <bitset>
#include "mtkVCLUtils.h"
#include "mtkLogger.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "mtkFloatLabeledEdit"
#pragma link "TIntLabel"
#pragma link "mtkFloatLabel"
#pragma resource "*.dfm"
TMotorFrame *MotorFrame;

using namespace mtk;
//---------------------------------------------------------------------------
__fastcall TMotorFrame::TMotorFrame(const string& name, TComponent* Owner)
	: TFrame(Owner),
    mMotor(NULL)
{
    TFrame::Name = (vclstr("Frame_" + name));
}

//---------------------------------------------------------------------------
void TMotorFrame::assignMotor(APTMotor* m)
{
	mMotor = m;
    if(mMotor)
    {
    	MotorGB->Caption = vclstr(mMotor->getName() + " [" + mMotor->getSerial() + "]");
        mJogVelocity->SetNumber(mMotor->getManualJogVelocity());
        mJogAcc->SetNumber(mMotor->getManualJogAcceleration());
    	mMotorStatusTimer->Enabled = true;
    }
}

//---------------------------------------------------------------------------
void __fastcall TMotorFrame::mMotorStatusTimerTimer(TObject *Sender)
{
    if(!mMotor)
    {
        return;
    }

    double p = mMotor->getPosition();
    double v = mMotor->getJogVelocity();
    double a = mMotor->getJogAcceleration();

    mMotorPositionE->SetNumber(p);
    mJogVelLbl->SetValue(v);
	mJogAccLbl->SetValue(a);


    bitset<32> bits(mMotor->getStatusBits());

    mIsActiveLabel->Caption 	= (mMotor->isActive()) 	    ? "True" : "False";
    mIsHomingLabel->Caption 	= (mMotor->isHoming()) 	    ? "True" : "False";
    mIsHomedLabel->Caption  	= (mMotor->isHomed()) 	    ? "True" : "False";
    mIsForwardingLabel->Caption = (mMotor->isForwarding()) 	? "True" : "False";
    mIsReversingLabel->Caption  = (mMotor->isReversing()) 	? "True" : "False";
}


//---------------------------------------------------------------------------
void __fastcall TMotorFrame::identifyExecute(TObject *Sender)
{
    if(mMotor)
    {
    	mMotor->identify();
    }
}

//---------------------------------------------------------------------------
void __fastcall TMotorFrame::homeExecute(TObject *Sender)
{
    if(mMotor)
    {
    	mMotor->home();
    }
}

//---------------------------------------------------------------------------
void __fastcall TMotorFrame::BtnDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y)
{
	TButton* btn = (TButton*)(Sender);
    if(btn == mFwdBtn)
    {
        double vel = mJogVelocity->GetValue();
        mMotor->setJogVelocity(vel);
        double a = mJogAcc->GetValue();
        mMotor->setJogAcceleration(a);
      	mMotor->jogForward();
    }

    if(btn == mRewBtn)
    {
        double vel = mJogVelocity->GetValue();
        mMotor->setJogVelocity(vel);
        double a = mJogAcc->GetValue();
        mMotor->setJogAcceleration(a);
      	mMotor->jogReverse();
    }
}

//---------------------------------------------------------------------------
void __fastcall TMotorFrame::BtnUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	TButton* btn = (TButton*)(Sender);
    if(btn == mFwdBtn || btn == mRewBtn)
    {
  		mMotor->stop();
    }
}

//---------------------------------------------------------------------------
void __fastcall TMotorFrame::DevEdit(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if(Key != vkReturn)
    {
    	return;
    }

    if(!mMotor)
    {
    	Log(lError) << "No motor to set values for";
        return;
    }

	mtkFloatLabeledEdit* e = dynamic_cast<mtkFloatLabeledEdit*>(Sender);
    if(e == mJogVelocity)
    {
        double vel = mJogVelocity->GetValue();
        Log(lInfo) << "New JOG velocity (mm/s): " <<vel;
		mMotor->setManualJogVelocity(vel);
        mMotor->setJogVelocity(vel);
    }
    else if(e == mJogAcc)
    {
        double a = mJogAcc->GetValue();
        Log(lInfo) << "New JOG acceleration (mm/(s*s)): " <<a;
		mMotor->setManualJogAcceleration(a);
        mMotor->setJogAcceleration(a);
    }
}

//---------------------------------------------------------------------------
void __fastcall TMotorFrame::mMotorPositionEKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift)
{
	mMotorStatusTimer->Enabled = false;
	if(Key != vkReturn)
    {
    	return;
    }

	mMotor->moveToPosition(mMotorPositionE->GetValue());
}

//---------------------------------------------------------------------------
void __fastcall TMotorFrame::mMotorPositionEKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift)
{
	if(Key == vkReturn)
    {
		mMotorStatusTimer->Enabled = true;
    	return;
    }
}

