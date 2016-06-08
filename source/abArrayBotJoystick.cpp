#pragma hdrstop
#include "abArrayBotJoystick.h"
#include "mtkLogger.h"
//---------------------------------------------------------------------------

using namespace mtk;

ArrayBotJoyStick::ArrayBotJoyStick()
:
mEnabled(false),
mJSMessageDispatcher(14)
{
    //Associate events with buttons
    mJSMessageDispatcher.setButtonEvents(1, mButton1.down,  mButton1.up    );
    mJSMessageDispatcher.setButtonEvents(2, mButton2.down,  mButton2.up    );
    mJSMessageDispatcher.setButtonEvents(3, mButton3.down,  mButton3.up    );
    mJSMessageDispatcher.setButtonEvents(4, mButton4.down,  mButton4.up    );

    mJSMessageDispatcher.setButtonEvents(5, mButton5.down,  mButton5.up    );
    mJSMessageDispatcher.setButtonEvents(6, mButton6.down,  mButton6.up    );

    mJSMessageDispatcher.setPOVButtonEvents(1, mPOVButton1.down,  mPOVButton1.up    );
    mJSMessageDispatcher.setPOVButtonEvents(2, mPOVButton2.down,  mPOVButton2.up    );
    mJSMessageDispatcher.setPOVButtonEvents(3, mPOVButton3.down,  mPOVButton3.up    );
    mJSMessageDispatcher.setPOVButtonEvents(4, mPOVButton4.down,  mPOVButton4.up    );

	mJSMessageDispatcher.setAxisEvent(1, mX1Axis.Move);
	mJSMessageDispatcher.setAxisEvent(2, mY1Axis.Move);

	mJSMessageDispatcher.setAxisEvent(3,  mX2Axis.Move);
	mJSMessageDispatcher.setAxisEvent(4,  mY2Axis.Move);

	//Start JS poll
    mJSMessageDispatcher.enable();
}

ArrayBotJoyStick::~ArrayBotJoyStick()
{}


void ArrayBotJoyStick::setButtonEvents(int btnNr, JoyStickEvent up, JoyStickEvent down)
{
	mJSMessageDispatcher.setButtonEvents(btnNr, up, down);
}

bool ArrayBotJoyStick::enable()
{
    mEnabled = mJSMessageDispatcher.enable();
    return mEnabled;
}

bool ArrayBotJoyStick::disable()
{
    mJSMessageDispatcher.disable();
	mEnabled = false;
    return true;
}

bool ArrayBotJoyStick::isEnabled()
{
	return mEnabled;
}

JoyStickAxis& ArrayBotJoyStick::getX1Axis()
{
	return mX1Axis;
}

JoyStickAxis& ArrayBotJoyStick::getX2Axis()
{
	return mX2Axis;
}

JoyStickAxis& ArrayBotJoyStick::getY1Axis()
{
	return mY1Axis;
}

JoyStickAxis& ArrayBotJoyStick::getY2Axis()
{
	return mY2Axis;
}

JoyStickButton& ArrayBotJoyStick::getButton(int nr)
{
	switch(nr)
    {
    	case 1:  return mButton1;
    	case 2:  return mButton2;
    	case 3:  return mButton3;
    	case 4:  return mButton4;
    	case 9:  return mButton9;
    	case 10: return mButton10;

        default:
        	Log(lError) <<"We don't have that button";
            throw("Bad stuff..");
    }
}

JoyStickButton& ArrayBotJoyStick::getPOVButton(int nr)
{
	switch(nr)
    {
    	case 1:  return mPOVButton1;
    	case 2:  return mPOVButton2;
    	case 3:  return mPOVButton3;
    	case 4:  return mPOVButton4;

        default:
        	Log(lError) <<"Don't such a button";
            throw("Bad stuff..");
    }
}
