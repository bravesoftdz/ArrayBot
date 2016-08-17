#ifndef abCoreH
#define abCoreH
#include "abExporter.h"

#define MOTOR_WARNING_MESSAGE "MOTOR_WARNING_MESSAGE"

AB_CORE unsigned int 	getABCoreMessageID(const string& msg);
AB_CORE void 			initABCoreLib();

AB_CORE enum ABSound {absMotorStop = 0, absMotorWarning, absCameraShot};

AB_CORE void  playABSound(ABSound, long flags = 0);

#endif