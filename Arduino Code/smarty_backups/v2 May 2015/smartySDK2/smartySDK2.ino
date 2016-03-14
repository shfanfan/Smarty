#include <Smarty.h>

Smarty smartyLight;

uint8_t counter = 0;

void setup() {
  smartyLight.setupSmarty();
  
  smartyLight.setHardPin(0, A0, ANALOG_IN);
  smartyLight.setHardPin(1, A1, ANALOG_IN);
  smartyLight.setHardPin(2, A2, ANALOG_IN);
  smartyLight.setHardPin(3, 3, ANALOG_OUT);
  
  smartyLight.setSoftPin(4, &counter, ANALOG_IN);
  
  //setSoftInputs(byte* p_softAnalogInputPins=NULL, byte numSoftAnalogInputPins=0, byte* p_softDigitalInputPins=NULL, byte numSoftDigitalInputPins=0);
  //setCallbacks(GenericCmdCb p_genericCmdCb=NULL, SetAnalogCmdCb p_setAnalogCmdCb=NULL, SetDigitalCmdCb p_setDigitalCmdCb=NULL, DcMotorCmdCb p_dcMotorCmdCb=NULL); 
  
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
}

void loop() {
  smartyLight.poll();
  //processButtonPress();
  smartyLight.sendSensorsData();
  counter++;
  delay(1000);
}


