#ifndef abProcessSequencerH
#define abProcessSequencerH
#include <list>
#include "abExporter.h"
#include "abABObject.h"
#include "abProcessSequence.h"
#include "mtkTimer.h"
#include "abProcessSequenceProject.h"
using mtk::Timer;
class LinearMove;
//---------------------------------------------------------------------------
typedef void (__closure *MyOnTimerCFPtr)();

class AB_CORE ProcessSequencer : public ABObject
{
	public:
			      	                ProcessSequencer();
                  	                ~ProcessSequencer(){}
		bool				        assignUnit(ABObject* o);
		bool				        load(const string& seqFName);
		bool				        saveCurrent(const string& folder);

		void				        clear();
		void		                start(bool continous = true);
		void		                forward();
        void		                reverse();
        void		                stop();
        bool				        isRunning();
        bool				        isCurrentProcessActive();
        void		                addProcess(Process* p);
        bool		                removeProcess(Process* p);
        bool		                removeProcess(const string& name);
        string				        getCurrentProcessName();
        string				        getCurrentSequenceName();
        int							getNumberOfProcesses();

        ProcessSequence*   	        getSequence(){return mSequence;}
        void						assignSequence(ProcessSequence* seq);

	private:
    	ProcessSequence*	        mSequence;

        							//!ExecuteAutomatic executes individual processes
                                    //!in sequence. If set false, each sequence need to be
                                    //!manually executed using the forward() function
        bool				        mExecuteAutomatic;
        Timer				        mSequenceTimer;
		void			            runThreaded();
		MyOnTimerCFPtr	            inThreadCB;
};

#endif
