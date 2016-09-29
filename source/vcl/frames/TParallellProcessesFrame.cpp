#include <vcl.h>
#pragma hdrstop
#include "TParallellProcessesFrame.h"
#include "mtkVCLUtils.h"
#include "abProcess.h"
#include "abMove.h"
#include "abParallellProcess.h"
#include "mtkLogger.h"
#include "abArrayBot.h"
#include "apt/abAPTMotor.h"
#include "abAbsoluteMove.h"
#include "abVCLUtils.h"
#include "mtkMathUtils.h"
#include "abTriggerFunction.h"
#include "TStringInputDialog.h"
#include "arduino/abArduinoServerCommand.h"
#include "TArduinoServerCommandFrame.h"

#pragma package(smart_init)
#pragma link "TMotorMoveProcessFrame"
#pragma link "TSTDStringLabeledEdit"
#pragma link "TArrayBotBtn"
#pragma resource "*.dfm"

using namespace mtk;

TParallellProcessesFrame *ParallellProcessesFrame;
//---------------------------------------------------------------------------
__fastcall TParallellProcessesFrame::TParallellProcessesFrame(TComponent* Owner)
	: TFrame(Owner)
{
	mTMotorMoveProcessFrame 	= new TMotorMoveProcessFrame(Owner);
    mTMotorMoveProcessFrame->Parent = this;
    mTMotorMoveProcessFrame->Visible = false;
    mTMotorMoveProcessFrame->Align 	 = alClient;

    mTArduinoServerCommandFrame = new TArduinoServerCommandFrame(Owner);
    mTArduinoServerCommandFrame->Parent = this;
    mTArduinoServerCommandFrame->Visible = false;
    mTArduinoServerCommandFrame->Align = alClient;
}

void TParallellProcessesFrame::populate(ArrayBot& ab, Process* p)
{
	mAB = &ab;
	rePopulate(p);
}

void TParallellProcessesFrame::rePopulate(Process* pp)
{
	mSubProcessesLB->Clear();
    if(!pp)
    {
    	EnableDisableFrame(this, false);
        return;
    }

    stringstream c;
    c << "Update Final Process Positions \n";
    c << "("<<pp->getProcessName()<<")";
	mUpdateFinalPositionsA->Caption =  vclstr(c.str());

	//Populate, update frame with data from process
    mParallell = dynamic_cast<ParallellProcess*>(pp);

    //Fill out the listbox with moves
    for(int i = 0; i < mParallell->getNumberOfProcesses(); i++)
    {
    	Process* p = mParallell->getProcess(i);
        if(p)
        {
        	mSubProcessesLB->Items->AddObject(vclstr(p->getProcessName()), (TObject*) p);
        }
    }

  	EnableDisableFrame(this, true);

    if(mSubProcessesLB->Count)
    {
	    selectAndClickListBoxItem(mSubProcessesLB, 0);
    }
    else
    {
		mTMotorMoveProcessFrame->Visible = false;
        mTArduinoServerCommandFrame->Visible = false;
    }
}

void __fastcall TParallellProcessesFrame::addMoveAExecute(TObject *Sender)
{
	//Add a move to current process
    if(!mParallell)
    {
    	Log(lError) << "This is something else...!";
        return;
    }

    Process* lm = new AbsoluteMove("");

    //Add the move to the container.. this will give the move a name
    mParallell->addProcess(lm);

    lm->assignProcessSequence(mParallell->getProcessSequence());
    mParallell->write();

    //Add move to Listbox
    int indx = mSubProcessesLB->Items->AddObject(lm->getProcessName().c_str(), (TObject*) lm);
	mSubProcessesLB->ItemIndex = indx;

	//Select the new process
    selectItem(lm);
}

//---------------------------------------------------------------------------
void __fastcall TParallellProcessesFrame::addArduinoCommandAExecute(TObject *Sender)
{
	//Add a move to current process
    if(!mParallell)
    {
    	Log(lError) << "This is something else...!";
        return;
    }

    Process* c = new ArduinoServerCommand("");

    //Add the move to the container.. this will give the move a name
    mParallell->addProcess(c);

    c->assignProcessSequence(mParallell->getProcessSequence());
    mParallell->write();

    //Add move to Listbox
    int indx = mSubProcessesLB->Items->AddObject(c->getProcessName().c_str(), (TObject*) c);
	mSubProcessesLB->ItemIndex = indx;

	//Select the new process
    selectItem(c);
}

//---------------------------------------------------------------------------
void __fastcall TParallellProcessesFrame::removeMoveAExecute(TObject *Sender)
{
	//Get selected move
    int index = mSubProcessesLB->ItemIndex;
    if(index != -1)
    {
    	string moveName = stdstr(mSubProcessesLB->Items->Strings[index]);
        mParallell->removeProcess(moveName);
        mParallell->write();
		rePopulate(mParallell);
    }
}

void TParallellProcessesFrame::selectItem(Process* p)
{
	if(dynamic_cast<AbsoluteMove*>(p))
    {
        mTMotorMoveProcessFrame->populate(mAB, dynamic_cast<AbsoluteMove*>(p));
        mTMotorMoveProcessFrame->Visible = true;
       	mTArduinoServerCommandFrame->Visible = false;
        EnableDisableFrame(mTMotorMoveProcessFrame, true);
    }
    else if(dynamic_cast<ArduinoServerCommand*>(p))
    {
        mTMotorMoveProcessFrame->Visible = false;
        mTArduinoServerCommandFrame->populate(mAB, dynamic_cast<ArduinoServerCommand*>(p));
        mTArduinoServerCommandFrame->Visible = true;
        EnableDisableFrame(mTArduinoServerCommandFrame, true);
    }
}

void __fastcall TParallellProcessesFrame::mSubProcessesLBClick(TObject *Sender)
{
	if(mSubProcessesLB->ItemIndex == -1 || mParallell == NULL)
    {
    	return;
    }

	//Get Current itemIndex, retrieve the move and populate the move frame
	string moveName = stdstr(mSubProcessesLB->Items->Strings[mSubProcessesLB->ItemIndex]);
    Process* p = mParallell->getProcess(moveName);
    selectItem(p);
}

//---------------------------------------------------------------------------
void __fastcall TParallellProcessesFrame::mUpdateFinalPositionsAExecute(TObject *Sender)
{
	//Populate, update frame with data from process
    if(!mParallell)
    {
    	return;
    }

    if(mParallell->getNumberOfProcesses() == 0)
    {
    	MessageDlg("There are no moves to update", mtInformation, TMsgDlgButtons() << mbOK, 0);
    }

    //Fill out the listbox with moves
    for(int i = 0; i < mParallell->getNumberOfProcesses(); i++)
    {
    	Process* p = mParallell->getProcess(i);
        if(!p)
        {
        	continue;
        }

        //Check if the process is a move process, and if so check if we can
        //update its final position
        AbsoluteMove* am = dynamic_cast<AbsoluteMove*>(p);
        if(am)
        {
            APTMotor* mtr = mAB->getMotorWithName(am->getMotorName());
            if(mtr && isEqual(am->getPosition(), mtr->getPosition(), 1.e-4) == false)
            {
                stringstream msg;
                msg <<
                "Update final motor position for motor: "<<am->getMotorName() <<
                "\n("<<am->getPosition()<<" -> "<< mtr->getPosition()<<")";

                if(MessageDlg(vclstr(msg.str()), mtConfirmation, TMsgDlgButtons() << mbYes<<mbNo, 0) == mrYes)
                {
                	am->setPosition(mtr->getPosition());

                    //Save updated sequence
                    mAB->getProcessSequencer().saveCurrent();
                }
            }

            //Check if this move has a trigger
            PositionalTrigger* pt = dynamic_cast<PositionalTrigger*>(am->getTrigger());
            if(pt)
            {
            	//Get the trigger function
                MoveAbsolute *fn = dynamic_cast<MoveAbsolute*>(pt->getTriggerFunction());
            	APTMotor* mtr = dynamic_cast<APTMotor*>(pt->getSubject());
                if(mtr && fn)
                {
                    if(isEqual(fn->getPosition(), mtr->getPosition(), 1.e-4) == false)
                    {
                        stringstream msg;
                        msg <<
                        "Update final TRIGGERED motor position for motor: "<<mtr->getName() <<
                        "\n("<<fn->getPosition()<<" -> "<< mtr->getPosition()<<")";

                        if(MessageDlg(vclstr(msg.str()), mtConfirmation, TMsgDlgButtons() << mbYes<<mbNo, 0) == mrYes)
                        {
                            fn->setPosition(mtr->getPosition());

                            //Save updated sequence
                            mAB->getProcessSequencer().saveCurrent();
                        }
                    }
                }
            }
        }
    }

    //Update UI
	selectAndClickListBoxItem(mSubProcessesLB, 0);
}

//---------------------------------------------------------------------------
void __fastcall TParallellProcessesFrame::mRenameBtnClick(TObject *Sender)
{
	//Open string input form
	TStringInputDialog* t = new TStringInputDialog(this);

    t->Caption = "Rename Action";
	Process* p =  getCurrentlySelectedSubProcess();
    if(!p)
    {
    	return;
    }
    t->setText(p->getProcessName());

    if(t->ShowModal() == mrOk)
    {
		//Rename the currently selected sequence
    	string newName(t->getText());

		p->setProcessName(newName);
		p->write();

        int indx = mSubProcessesLB->ItemIndex;
        if(updateListBoxItemCaption(mSubProcessesLB, indx, newName) == false)
        {

        }

        if(selectAndClickListBoxItem(mSubProcessesLB, newName) == false)
        {
        	//bad..
        }
    }
    delete t;
}

Process* TParallellProcessesFrame::getCurrentlySelectedSubProcess()
{
	if(mSubProcessesLB->Count > 0 && mSubProcessesLB->ItemIndex != -1)
    {
	    Process* p = (Process*) mSubProcessesLB->Items->Objects[mSubProcessesLB->ItemIndex];
        return p;
    }
    return NULL;
}



