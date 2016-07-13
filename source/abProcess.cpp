#pragma hdrstop
#include "abProcess.h"
#include "mtkXMLUtils.h"
//---------------------------------------------------------------------------

using Poco::Timespan;
using namespace mtk;

string toString(ProcessType pt)
{
	switch(pt)
    {
    	case ptMove: return "linearMove";
    	case ptCombinedMove: return "combinedMove";
    	case ptTimeDelay: return "timeDelay";
        default: return "unknown";
    }
}

ProcessType toProcessType(const string& str)
{
	if(str == "linearMove")
    {
    	return ptMove;
    }

	if(str == "combinedMove")
    {
    	return ptCombinedMove;
    }

	if(str == "timeDelay")
    {
    	return ptTimeDelay;
    }

	return ptUnknown;
}

Process::Process(const string& lbl, ProcessType pt)
:
mUnit(NULL),
mProcessName(lbl),
mPreDwellTime(0),
mPostDwellTime(0),
mTimeOut(60*Poco::Timespan::SECONDS),
mIsBeingProcessed(false),
mIsProcessed(false),
mIsStarted(false),
mProcessType(pt)
{}

string Process::getProcessType()
{
	return toString(mProcessType);
}

bool Process::start()
{
	mIsStarted = true;
	mIsBeingProcessed = true;
    mIsProcessed = false;
    return true;
}

void Process::init()
{
	mIsBeingProcessed =  false;
    mIsProcessed = false;
    mIsStarted = false;
    mEndTime = mStartTime;
}

bool Process::stop()
{
	mIsBeingProcessed 	= false;
    mIsProcessed 		= false;
    return true;

}

XMLElement* Process::addToXMLDocument(tinyxml2::XMLDocument& doc, XMLNode* docRoot)
{
    //Create XML for saving to file
    XMLElement* processNode  	= doc.NewElement("process");
    XMLNode*    rootNode 		= doc.InsertFirstChild(processNode);

    //Attributes
    //processNode->SetAttribute("id", getID().toString().c_str());

    //Elements
    XMLElement* elem = doc.NewElement("process_name");
    processNode->InsertFirstChild(elem);
        elem->SetText(mProcessName.c_str());
    processNode->InsertEndChild(elem);

    //Elements
    elem = doc.NewElement("process_type");
    processNode->InsertFirstChild(elem);
        elem->SetText(getProcessType().c_str());
    processNode->InsertEndChild(elem);

    processNode->InsertEndChild(rootNode);
    docRoot->InsertEndChild(processNode);

    return processNode;
}

//Re implemented in derived processes
XMLElement* Process::addToXMLDocumentAsChild(mtk::XMLDocument& doc, mtk::XMLNode* docRoot)
{
	return NULL;
}

bool Process::isTimedOut()
{
	return false;
//	bool res;
//    if(isBeingProcessed())
//    {
//     	Poco::Timestamp now;
//        Poco::Timespan timeElapsed(now - mStartTime);
//        return ( timeElapsed > mTimeOut) ? true : false;
//    }
//    else
//    {
//		return (Timespan(mEndTime - mStartTime) > mTimeOut) ? true : false;
//    }
}
