#ifndef TABProcessSequencerFrameH
#define TABProcessSequencerFrameH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "TFloatLabeledEdit.h"
#include "TSTDStringLabeledEdit.h"
#include <Vcl.ExtCtrls.hpp>
#include "abProcessSequencer.h"
#include <System.Actions.hpp>
#include <Vcl.ActnList.hpp>
#include "TMotorMoveProcessFrame.h"
//---------------------------------------------------------------------------

class XYZUnit;
class ArrayBot;

class TABProcessSequencerFrame : public TFrame
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TLabel *Label1;
	TButton *mStartBtn;
	TListBox *mProcessesLB;
	TComboBox *mSequencesCB;
	TButton *mSaveSequenceBtn;
	TButton *mAddMoveBtn;
	TButton *mDeleteSequenceBtn;
	TButton *mAddSeqBtn;
	TButton *mDeleteMoveBtn;
	TTimer *mSequenceStatusTimer;
	TLabel *mStatusLbl;
	TButton *Button1;
	TActionList *Actions;
	TAction *addMotorAction;
	TListBox *mMoveMotorLB;
	TMotorMoveProcessFrame *TMotorMoveProcessFrame1;
	TButton *Button2;
	TAction *removeMotorA;
	TAction *addProcessA;
	TAction *removeProcessA;
	void __fastcall mDeleteSequenceBtnClick(TObject *Sender);
	void __fastcall mAddSeqBtnClick(TObject *Sender);
	void __fastcall mStartBtnClick(TObject *Sender);
	void __fastcall mSaveSequenceBtnClick(TObject *Sender);
	void __fastcall mSequencesCBChange(TObject *Sender);
	void __fastcall mProcessesLBClick(TObject *Sender);
	void __fastcall moveParEdit(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall MotorsCBChange(TObject *Sender);
	void __fastcall mSequenceTimerTimer(TObject *Sender);
	void __fastcall addProcessAExecute(TObject *Sender);
	void __fastcall removeProcessAExecute(TObject *Sender);


private:	// User declarations
	    ProcessSequencer  			mProcessSequencer;
        ArrayBot&					mAB;
		XYZUnit& 					mCoverSlipUnit;
		XYZUnit& 					mWhiskerUnit;
        string 						mMovesFileExtension;
		void __fastcall 			refreshSequencesCB();
		static int 					mFrameNr;
		void						saveSequence();
		void 						retrieveMotorsFromUnit(XYZUnit& unit);
public:
						 __fastcall TABProcessSequencerFrame(ArrayBot& ab, TComponent* Owner);
        void						init();
};

extern PACKAGE TABProcessSequencerFrame *ABProcessSequencerFrame;
#endif