#ifndef abArduinoMessageProcessorH
#define abArduinoMessageProcessorH
#include <string>
#include "mtkThread.h"
#include "abExporter.h"
//----------------------------------------------------------------

using mtk::Thread;


namespace mtk
{
	class MessageContainer;
}
using mtk::MessageContainer;


class AB_CORE ArduinoMessageProcessor : public Thread
{
    public:
                                        ArduinoMessageProcessor(MessageContainer& mMessageContainer);
                                        ~ArduinoMessageProcessor();

                                        // overridden from Thread
        void                            worker();
        void                            run();

        bool                            start(bool inThread);
        void                            stop();
        virtual void                    processMessage(const string& msg);
        void                            suspendProcessing() {mAllowProcessing = false;}
        void                            resumeProcessing()  {mAllowProcessing = true;}

	private:
        string                          mServerName;
        bool                            mAllowProcessing;
        MessageContainer&            	mMessageContainer;

//        LogClient*                      mParent;
};

#endif
