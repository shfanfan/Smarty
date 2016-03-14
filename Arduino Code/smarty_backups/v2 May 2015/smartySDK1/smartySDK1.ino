#include "smarty.h"
#include <SPI.h>
#include <RF24.h>

const int myHardAnalogCount = 3;
byte myHardAnalog[myHardAnalogCount] = {A0,A1,A2};

Smarty smartyLight;

void setup() {
  smartyLight.setup();
  smartyLight.setHardInputs(myHardAnalog,myHardAnalogCount);
  
  //setSoftInputs(byte* p_softAnalogInputPins=NULL, byte numSoftAnalogInputPins=0, byte* p_softDigitalInputPins=NULL, byte numSoftDigitalInputPins=0);
  //setCallbacks(GenericCmdCb p_genericCmdCb=NULL, SetAnalogCmdCb p_setAnalogCmdCb=NULL, SetDigitalCmdCb p_setDigitalCmdCb=NULL, DcMotorCmdCb p_dcMotorCmdCb=NULL); 




}

void loop() {
  
  //poll();
  //processButtonPress();
  //sendSensorsData()
}

//void doCommand(int commandNum. int param1)
//{
//
//}


