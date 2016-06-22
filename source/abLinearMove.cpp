#pragma hdrstop
#include "abLinearMove.h"
#include "abXYZUnit.h"
#include "abAPTMotor.h"
#include "abPosition.h"
#include "mtkLogger.h"


using namespace mtk;
using namespace ab;
//---------------------------------------------------------------------------
LinearMove::LinearMove(const string& lbl, ABObject* unit, MoveType type, const ab::Position& p, double maxVel, double acc, double dwellTime)
:
Process(lbl, unit),
mMoveType(type),
mPosition(p),
mMaxVelocity(0),
mAcceleration(0)
{
	mProcessType = ptLinearMove;
    APTMotor* mtr = dynamic_cast<APTMotor*>(unit);
    if(mtr)
    {
    	mMotorName = mtr->getName();
    }
}

void LinearMove::assignUnit(ABObject* o)
{
	mUnit = NULL;
	//Check out what ABObject is
    if(dynamic_cast<XYZUnit*>(o))
    {
    	//Look for motor
        XYZUnit* xyz = dynamic_cast<XYZUnit*>(o);
        if(xyz->getMotorWithName(mMotorName))
        {
        	mUnit = xyz->getMotorWithName(mMotorName);
        }
    }
    else if(dynamic_cast<APTMotor*>(o))
    {
    	APTMotor* m = dynamic_cast<APTMotor*>(o);
        mMotorName = m->getName();
    	mUnit = m;
    }
    else if(dynamic_cast<AngleController*>(o))
    {
    	AngleController* ac = dynamic_cast<AngleController*>(o);
        mMotorName = ac->getName();
    	mUnit = ac;
    }
    if(mUnit == NULL)
    {
   		Log(lError) << "Motor Unit is NULL for LinearMove: "<<mProcessName;
    }
}

bool LinearMove::write(IniSection* sec)
{
//    IniKey* key = sec->createKey("PROCESS_NAME", getProcessName());

    double x = getPosition().x();
    IniKey* key = sec->createKey("PROCESS_TYPE", 	toString(getMoveType()));
    key = sec->createKey("MOTOR_NAME",   	toString(mMotorName));
    key = sec->createKey("POSITION", 		toString(x));
    key = sec->createKey("POSITION_NAME",  	getPosition().getLabel());
    key = sec->createKey("MAX_VELOCITY", 	toString(getMaxVelocity()));
    key = sec->createKey("ACCELERATION", 	toString(getAcceleration()));
    key = sec->createKey("DWELL_TIME",   	toString(getDwellTime()));

    return true;
}

bool LinearMove::read(IniSection* sec)
{
	if(!sec)
	{
    	return false;
    }

	IniKey* key = sec->getKey("MOVE_TYPE");

    if(key)
    {
        mMoveType = toMoveType(key->mValue);;
    }

    key = sec->getKey("MOTOR_NAME");
    if(key)
    {
        mMotorName = key->mValue;
    }

    key = sec->getKey("POSITION");
    if(key)
    {
	    mPosition = Position(mPosition.getLabel(), key->asFloat(), 0.0 , 0.0);
    }

    key = sec->getKey("POSITION_NAME");
    if(key)
    {
        mPosition.setLabel(key->mValue);
    }

    key = sec->getKey("MAX_VELOCITY", true);
    if(key)
    {
	    mMaxVelocity = key->asFloat();
    }

    key = sec->getKey("ACCELERATION", true);
    if(key)
    {
        mAcceleration = key->asFloat();
    }

    key = sec->getKey("DWELL_TIME", true);
    if (key)
    {
		mDwellTime = key->asFloat();
    }

    return true;
}

bool LinearMove::execute()
{
	XYZUnit* o = dynamic_cast<XYZUnit*>(mUnit);
    if(o)
    {
		return o->moveAbsolute(mPosition);
    }

	APTMotor* m = dynamic_cast<APTMotor*>(mUnit);
    if(m)
    {
		if(mMaxVelocity == 0 || mAcceleration == 0)
        {
        	Log(lError) << "Move cannot be executed with zero velocity or acceleration";
            return false;
        }
    	m->setVelocity(mMaxVelocity);
        m->setAcceleration(mAcceleration);
        return m->moveAbsolute(mPosition.x());
    }

	AngleController* ac = dynamic_cast<AngleController*>(mUnit);
    if(ac)
    {
		if(mMaxVelocity == 0 || mAcceleration == 0)
        {
        	Log(lError) << "Move cannot be executed with zero velocity or acceleration";
            return false;
        }
       	ac->getMotor()->setVelocity(mMaxVelocity);
        ac->getMotor()->setAcceleration(mAcceleration);
        return ac->getMotor()->moveAbsolute(mPosition.x());
    }

    return false;
}

bool LinearMove::stop()
{
	XYZUnit* o = dynamic_cast<XYZUnit*>(mUnit);
    if(o)
    {
		return o->stopAll();
    }

	APTMotor* m = dynamic_cast<APTMotor*>(mUnit);
    if(m)
    {
    	m->stop(false);
        return true;
    }

    return false;
}

bool LinearMove::undo()
{
	Position p("undo pos", mPosition.x() * -1, mPosition.y() * -1, mPosition.z() * -1);

	XYZUnit* unit = dynamic_cast<XYZUnit*>(mUnit);
    if(unit)
    {
		return unit->moveAbsolute(p);
    }

	return false;
}

bool LinearMove::isDone()
{
	APTMotor* o = dynamic_cast<APTMotor*>(mUnit);
    if(o)
    {
    	double p = o->getPosition();
    	return (isEqual(p,mPosition.x(), 5.0e-6)) ? true : false;
    }

    return false;
}

bool LinearMove::isActive()
{
	APTMotor* o = dynamic_cast<APTMotor*>(mUnit);
    if(o)
    {
    	return o->isActive();
    }

    return false;
}


MoveType toMoveType(const string& mt)
{
	if(mt == "ABSOLUTE_MOVE")
    {
    	return mtAbsolute;
    }

	if(mt == "RELATIVE_MOVE")
    {
    	return mtRelative;
    }

    return mtUnknown;
}

string	toString(MoveType mt)
{
	if(mt == mtAbsolute)
    {
    	return "ABSOLUTE_MOVE";
    }

	if(mt == mtRelative)
    {
    	return "RELATIVE_MOVE";
    }

    return "UNKNOWN_MOVE";
}
