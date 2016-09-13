#ifndef abPufferArduinoH
#define abPufferArduinoH
#include "abArduinoDevice.h"
#include "abLeicaUC7Controller.h"
//---------------------------------------------------------------------------

//!The puffer arduino class expose a simple interface to control the puffer arduino.
class AB_CORE PufferArduino : public ArduinoDevice
{
    public:
                                PufferArduino(int portNr, int baudRate = 9600);
                                ~PufferArduino(){}

		bool					enableDisableSimulator(bool flag);
        bool					enablePuffer();
        bool					disablePuffer();

		bool					manualPuff();

        bool					setSimualorSpeed(int ms);
		bool					setCutThicknessPreset(int preset);
        int						getLastCutThicknessPreset();
		bool					setDeltaY(int dy);

    protected:
    	LeicaUC7Controller		mTheLeica;


};
#endif
