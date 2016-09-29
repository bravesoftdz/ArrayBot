#ifndef abATDBServerSessionH
#define abATDBServerSessionH
#include "abExporter.h"
#include "abABObject.h"
#include "abDBUtils.h"
#include "Poco/Data/RecordSet.h"
//---------------------------------------------------------------------------

using namespace Poco::Data;
using Poco::Data::RecordSet;


//!The ATDB server session class encapsulate a database session
//!with the ATDB MySQL database
class AB_CORE ATDBServerSession : public ABObject
{
    public:
					        	        ATDBServerSession(const string& host="127.0.0.1", const string& user="atdb_client", const string& password="atdb123"){}
					        	        ~ATDBServerSession(){}

										//!Statements
		RecordSet* 						getBlocks(dbSQLKeyword kw = dbDescending);
        bool							insertBlock(int userID, const string& lbl, const string& note);
        bool							deleteBlock(int bId);
		RecordSet*        				getNotesForBlockWithID(int blockID);
		bool							addNoteForBlockWithID(int blockID, int userID);
		bool							deleteNoteWithID(int noteID);
		bool							updateNoteWithID(int noteID, const string& note);

		RecordSet* 						getUsers(dbSQLKeyword kw = dbAscending);

		bool							isConnected();//{return mTheSession ? true : false;}
		bool							connect();
        bool							disConnect();

    protected:
        string					        mDataBaseName;
        string					        mHost;
        string 					        mDataBaseUser;
        string					        mDataBasePassword;
		Session*	        			mTheSession;
};
#endif
