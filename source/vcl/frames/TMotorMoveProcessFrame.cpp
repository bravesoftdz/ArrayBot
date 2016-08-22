#include <vcl.h>
#pragma hdrstop
#include "TMotorMoveProcessFrame.h"
#include "TPositionalTriggerFrame.h"
#include "abMove.h"
#include "abAPTMotor.h"
#include "mtkVCLUtils.h"
#include "abXYZUnit.h"
#include "abArrayBot.h"
#include "abAbsoluteMove.h"
#include "abTriggerFunction.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TFloatLabeledEdit"
#pragma resource "*.dfm"
TMotorMoveProcessFrame *MotorMoveProcessFrame;

//---------------------------------------------------------------------------
__fastcall TMotorMoveProcessFrame::TMotorMoveProcessFrame(TComponent* Owner)
	: TFrame(Owner),
    mAB(NULL),
    mPosTriggerFrame(NULL)
{
	mPosTriggerFrame = new TPositionalTriggerFrame(this);
    mPosTriggerFrame->Parent = mTriggersSheet;
    mPosTriggerFrame->Align = alClient;
    mPosTriggerFrame->Visible = false;

}

void TMotorMoveProcessFrame::populate(ArrayBot* ab, AbsoluteMove* m)
{
	mAB = ab;
    MotorsCB->Clear();
	populateMotorCB();
    rePopulate(m);

    //Populate subframes
    mTriggersLB->Clear();
    if(mMove && mMove->getTrigger())
    {
	    mMove->getTrigger()->assignSubject(mMove->getUnit());
		mTriggersLB->Items->AddObject(mMove->getTrigger()->getTypeName().c_str(), (TObject*) mMove->getTrigger());
        mTriggersLB->ItemIndex = 0;
		TriggersLBClick(NULL);
        mAddTriggerB->Visible = false;
		mTriggersSheet->TabVisible = true;
    }
    else
    {
	    mPosTriggerFrame->Visible = false;
        mAddTriggerB->Visible = true;
		mTriggersSheet->TabVisible = false;
    }
}

void TMotorMoveProcessFrame::rePopulate(AbsoluteMove* m)
{
	if(!m)
    {
    	return;
    }

    mMove = m;
    MainGB->Caption = vclstr(mMove->getProcessName());
	mMovePosE->setValue(mMove->getPosition());
    mMaxVelE->setValue(mMove->getMaxVelocity());
    mAccE->setValue(mMove->getAcceleration());

    APTMotor* mtr = dynamic_cast<APTMotor*>(mMove->getUnit());
    if(!mtr)
    {
    	//Look for motor
        string unitLbl = mMove->getMotorName();
		int idx = MotorsCB->Items->IndexOf(unitLbl.c_str());
        if(idx != -1)
        {
        	//Found it, assign it
        	mMove->assignUnit( (APTMotor*) MotorsCB->Items->Objects[idx] );
			MotorsCB->ItemIndex = idx;
        }
    }
    else
    {
    	int idx = MotorsCB->Items->IndexOf(mtr->getName().c_str());
		MotorsCB->ItemIndex = idx;
    }
}

void TMotorMoveProcessFrame::populateMotorCB()
{
	vector<APTMotor*> motors = mAB->getAllMotors();
    for(int i = 0; i < motors.size(); i++)
    {
    	if(motors[i])
        {
			MotorsCB->Items->InsertObject(MotorsCB->Items->Count, motors[i]->getName().c_str(), (TObject*) motors[i]);
        }
    }
}

//---------------------------------------------------------------------------
void __fastcall TMotorMoveProcessFrame::MotorsCBClick(TObject *Sender)
{
	;
}

//---------------------------------------------------------------------------
void __fastcall TMotorMoveProcessFrame::MotorsCBChange(TObject *Sender)
{
	//Check if a motor is selected
    ABObject* obj = (ABObject*) MotorsCB->Items->Objects[MotorsCB->ItemIndex];
    APTMotor* motor = dynamic_cast<APTMotor*>(obj);

    if(motor && mMove)
    {
    	mMovePosE->Enabled = true;
        mAccE->Enabled = true;
        mMove->assignUnit(motor);
    }
    else
    {
    	mMovePosE->Enabled 	= false;
        mAccE->Enabled 		= false;
    }


    if(mMove && mMove->getTrigger())
    {
		mPosTriggerFrame->rePopulate(mMove->getTrigger());
    }
}

void __fastcall TMotorMoveProcessFrame::mMovePosEKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
    if(Key != vkReturn || mMove == NULL)
    {
    	return;
    }

    mMove->setPosition(mMovePosE->getValue());
    mMove->setMaxVelocity(mMaxVelE->getValue());
    mMove->setAcceleration(mAccE->getValue());
}

//---------------------------------------------------------------------------
void __fastcall TMotorMoveProcessFrame::TriggersLBClick(TObject *Sender)
{
	//Retrive selected trigger
    int indx = mTriggersLB->ItemIndex;
    if(indx == -1)
    {
       	mAddTriggerB->Enabled = false;
        mDeleteTriggerB->Enabled = false;
    	return;
    }

   	mAddTriggerB->Enabled = true;
    mDeleteTriggerB->Enabled = true;
    Trigger* t = (Trigger*) mTriggersLB->Items->Objects[indx];
    if(t)
    {
		//Check what kind of trigger
       	mPosTriggerFrame->populate(*mAB, t);
	    mPosTriggerFrame->Visible = true;
    }
    else
    {
	    mPosTriggerFrame->Visible = false;
    }
}

void __fastcall TMotorMoveProcessFrame::mDeleteTriggerBClick(TObject *Sender)
{
	//delete currently selected trigger
    int indx = mTriggersLB->ItemIndex;
    if(indx != -1)
    {
    	mMove->deleteTrigger();
	    mTriggersLB->Clear();
    }
}

//---------------------------------------------------------------------------
void __fastcall TMotorMoveProcessFrame::AddTriggerBClick(TObject *Sender)
{
	//Create a new Trigger
    Trigger* t = new PositionalTrigger(NULL);

    mMove->addTrigger(t);

    //Also add a trigger function
    MoveAbsolute *tf = new MoveAbsolute(NULL);
    t->assignTriggerFunction(tf);

	populate(mAB, mMove);
}


