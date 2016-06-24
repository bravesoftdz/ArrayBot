#pragma hdrstop
#include "abProcessSequence.h"
#include "abProcess.h"
#include "mtkIniFile.h"
#include "abLinearMove.h"
#include "mtkLogger.h"
//---------------------------------------------------------------------------

using namespace mtk;

ProcessSequence::ProcessSequence(const string& name, const string& fileExt)
:
mName(name),
mFileExtension(fileExt)
{}

ProcessSequence::~ProcessSequence()
{}

bool ProcessSequence::assignUnit(ABObject* o)
{
    Process* p = getFirst();
    while(p)
    {
    	p->assignUnit(o);
        p = getNext();
    }
    return true;
}

void ProcessSequence::clear()
{
	mProcesses.clear();
}

void ProcessSequence::init()
{
    Process* process = getFirst();
    int count = 1;
    while(process)
    {
    	process->init();
        process = getNext();
    }
}

bool ProcessSequence::read(const string& fName)
{
	IniFile f(fName, true);
    int count = 0;
	IniSection* sec = f.getSection(count);
    IniKey* key;

    clear();
    while(sec)
    {
    	key = sec->getKey("PROCESS_TYPE");
        if(key && key->mValue == "ABSOLUTE_MOVE")
        {
	    	LinearMove* p = new LinearMove(sec->mName, NULL);

            if(p->read(sec))
            {
            	add(p);
            }
            else
            {
            	Log(lError) <<"Failed reading process: "<<sec->mName;
            }
        }
        else if ((key = sec->getKey("NAME") ), key != NULL)
        {
        	mName = key->mValue;
        }


        //Get next section
        sec = f.getSection(++count);
    }
    return true;
}

bool ProcessSequence::isFirst(Process* p)
{
	return p == *(mProcesses.begin());
}

bool ProcessSequence::write(const string& folder)
{
	//Save to file
    string fullFName = joinPath(folder, mName + "." + mFileExtension);
	IniFile f(fullFName);

    //Save some info about the Process Sequence
    IniSection* genSec = f.createSection("SEQUENCE_INFO");
    genSec->createKey("NAME", mName);

    Process* process = getFirst();
    int count = 1;
    while(process)
    {
        IniSection *sec = f.createSection(process->getProcessName());
        if(sec)
        {
            process->write(sec);
        }
        count++;
        process = getNext();
    }

    return f.save();
}

bool ProcessSequence::add(Process* pos)
{
	//Check label
    mProcesses.push_back(pos);
    mProcessIter = mProcesses.begin();
    return true;
}

bool ProcessSequence::remove(Process* p)
{
	//Find item
    mProcesses.remove(p);
    return false;
}

bool ProcessSequence::remove(const string& lbl)
{
	//Find item
    return false;
}

Process* ProcessSequence::getFirst() const
{
    mProcessIter = mProcesses.begin();
    if(mProcessIter != mProcesses.end())
    {
        return (*mProcessIter);
    }
    return NULL;
}

Process* ProcessSequence::getCurrent() const
{
    if(mProcessIter != mProcesses.end())
    {
        return (*mProcessIter);
    }
    return NULL;
}

Process* ProcessSequence::getNext() const
{
    if(mProcessIter != mProcesses.end())
    {
        mProcessIter++;
        if(mProcessIter != mProcesses.end())
        {
            return (*mProcessIter);
        }
    }
    return NULL;
}

Process* ProcessSequence::getPrevious() const
{
    if(mProcessIter != mProcesses.end())
    {
        mProcessIter--;
        if(mProcessIter != mProcesses.end())
        {
            return (*mProcessIter);
        }
    }
    return NULL;
}


