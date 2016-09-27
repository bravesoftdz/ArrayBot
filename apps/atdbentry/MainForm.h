#ifndef MainFormH
#define MainFormH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "mtkLogFileReader.h"
#include "TRegistryForm.h"
#include "mtkIniFile.h"
#include "mtkIniFileProperties.h"
#include "mtkLogLevel.h"
#include <Vcl.ExtCtrls.hpp>

#include "database/abATDBServerSession.h"
#include "TArrayBotBtn.h"
#include <Vcl.Buttons.hpp>


//---------------------------------------------------------------------------

using namespace mtk;
class TMain : public TRegistryForm
{
__published:	// IDE-managed Components
	TMemo *infoMemo;
	TTimer *ShutDownTimer;
	TArrayBotButton *ArrayBotButton1;
	TArrayBotButton *ArrayBotButton2;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall ShutDownTimerTimer(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ArrayBotButton1Click(TObject *Sender);
	void __fastcall ArrayBotButton2Click(TObject *Sender);

private:	// User declarations
        LogFileReader                       mLogFileReader;
        void __fastcall                     logMsg();
        IniFile						        mIniFile;
        IniFileProperties  			        mProperties;
		mtk::Property<mtk::LogLevel>        mLogLevel;
        ATDBServerSession					mServerSession;
public:		// User declarations
	__fastcall TMain(TComponent* Owner);
		__fastcall 					        ~TMain();
};
//---------------------------------------------------------------------------
extern PACKAGE TMain *Main;
//---------------------------------------------------------------------------
#endif
