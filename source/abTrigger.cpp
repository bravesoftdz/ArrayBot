#pragma hdrstop
#include "abTrigger.h"
//---------------------------------------------------------------------------

using namespace mtk;
using Poco::Timespan;

Trigger::Trigger(const string& name, triggerTestFunctionFPtr ttf, TriggerConditionOperator c)
:
mName(name),
mIsTriggered(false),
mTriggerConditionOperator(c),
mTriggerTimer(Poco::Timespan(100*Poco::Timespan::MILLISECONDS)),
mTestFunction(ttf)
{
	mTriggerTimer.assignTimerFunction(triggerTest);
}

bool Trigger::enable()
{
    mIsTriggered = false;
	mTriggerTimer.start();
    return true;
}

bool Trigger::disable()
{
    mIsTriggered = false;
	mTriggerTimer.stop();
    return true;
}

void Trigger::setTestFunction(triggerTestFunctionFPtr func)
{
    mTestFunction = func;
}

void Trigger::reset()
{
	mIsTriggered = false;
}

void Trigger::addFireFunction(FireFunction f)
{
	mFireFunction = f;
}
