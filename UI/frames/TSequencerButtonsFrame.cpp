#include <vcl.h>
#pragma hdrstop
#include <Buttons.hpp>
#include "TSequencerButtonsFrame.h"
#include "mtkVCLUtils.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TSequencerButtonsFrame *SequencerButtonsFrame;
//---------------------------------------------------------------------------
__fastcall TSequencerButtonsFrame::TSequencerButtonsFrame(ArrayBot& bot, TComponent* Owner)
	: TFrame(Owner),
    mAB(bot)
{
}
//--------------------------------------------------------------------------
void TSequencerButtonsFrame::update()
{

    for(int i = 0; i < mButtons.size(); i++)
    {
    	delete mButtons[i];
    }

	mButtons.clear();

    ProcessSequencer& psr = mAB.getProcessSequencer();
    ProcessSequences& pss = psr.getSequences();
    ProcessSequence*  ps = pss.getFirst();
    while(ps)
    {
        TSpeedButton* btn = new TSpeedButton(this);
        mButtons.push_back(btn);

        btn->Parent = this;
        btn->Caption = vclstr(ps->getName());
        btn->Align = alLeft;
        btn->OnClick = runSequenceBtnClick;
        btn->Font->Size = 14;
        ps = pss.getNext();
        btn->Width = this->Height;
    }

    //Restore back to first sequence
    ps = pss.getFirst();
}

void __fastcall TSequencerButtonsFrame::runSequenceBtnClick(TObject *Sender)
{
	;
}

void __fastcall TSequencerButtonsFrame::FrameEnter(TObject *Sender)
{
	update();
}

