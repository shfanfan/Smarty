#include <Arduino.h>

typedef void (*GenericCmdCb)(byte cmd, byte param1, byte param2, byte param3, byte param4, byte param5);
typedef void (*SetAnalogCmdCb)(byte analog1, byte val1, byte analog2, byte val2, byte analog3, byte val3);
typedef void (*SetDigitalCmdCb)(byte digital1, byte val1, byte digital2, byte val2, byte digital3, byte val3);
typedef void (*DcMotorCmdCb)(bool speedUpdate, byte speedVal, bool dirUpdate, byte dirVal);

class Smarty {
public:
  
  byte* mp_hardAnalogInputPins;
  byte m_numHardAnalogInputPins;
  byte* mp_hardDigitalInputPins;
  byte m_numHardDigitalInputPins;
  
  byte* mp_softAnalogInputPins;
  byte m_numSoftAnalogInputPins;
  byte* mp_softDigitalInputPins;
  byte m_numSoftDigitalInputPins;
  
  GenericCmdCb     mp_genericCmdCb;
  SetAnalogCmdCb   mp_setAnalogCmdCb;
  SetDigitalCmdCb  mp_setDigitalCmdCb;
  DcMotorCmdCb     mp_dcMotorCmdCb;
  
public:
  Smarty();
  void setup();
  void setHardInputs(byte* p_hardAnalogInputPins=NULL, byte numHardAnalogInputPins=0, byte* p_hardDigitalInputPins=NULL, byte numHardDigitalInputPins=0);
  void setSoftInputs(byte* p_softAnalogInputPins=NULL, byte numSoftAnalogInputPins=0, byte* p_softDigitalInputPins=NULL, byte numSoftDigitalInputPins=0);
  void setCallbacks(GenericCmdCb p_genericCmdCb=NULL, SetAnalogCmdCb p_setAnalogCmdCb=NULL, SetDigitalCmdCb p_setDigitalCmdCb=NULL, DcMotorCmdCb p_dcMotorCmdCb=NULL); 
 
  //TODO: process incomming messages and invoke propper callbacks (add callback to handle commands)
  //TODO: send hard and soft sensors data
  //TODO: process button press 
  //TODO: implement message polling from the NRF
};

