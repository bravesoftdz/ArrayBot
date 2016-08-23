//---------------------------------------------------------------------------

#include <System.hpp>
#pragma hdrstop
USEFORM("..\forms\TSplashForm.cpp", SplashForm);
USEFORM("..\forms\abAddJoyStickSettingForm.cpp", AddJoyStickSettingForm);
USEFORM("..\frames\TAboutArrayBot_2Frame.cpp", AboutArrayBotFrame_2); /* TFrame: File Type */
USEFORM("..\frames\TAboutArrayBotFrame.cpp", AboutArrayBotFrame); /* TFrame: File Type */
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------

//   Package source.
//---------------------------------------------------------------------------


#pragma argsused
extern "C" int _libmain(unsigned long reason)
{
	return 1;
}


//---------------------------------------------------------------------------
#pragma comment(lib, "mtkCommon.lib")
#pragma comment(lib, "VCLCommon.bpi")
#pragma comment(lib, "DuneComponents.bpi")
#pragma comment(lib, "poco_foundation-static.lib")


#pragma comment(lib, "abCore.lib")
#pragma comment(lib, "abVCLCore.bpi")

