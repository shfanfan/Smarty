#include <SPI.h>
#include <RF24.h>
#include "smarty.h"

RF24 radio(9,10);


Smarty::Smarty() {
  mp_hardAnalogInputPins=NULL;
  m_numHardAnalogInputPins=0;
  mp_hardDigitalInputPins=NULL;
  m_numHardDigitalInputPins=0;
  
  mp_softAnalogInputPins=NULL;
  m_numSoftAnalogInputPins=0;
  mp_softDigitalInputPins=NULL;
  m_numSoftDigitalInputPins=0;
  
  mp_genericCmdCb=NULL;
  mp_setAnalogCmdCb=NULL;
  mp_setDigitalCmdCb=NULL;
  mp_dcMotorCmdCb=NULL;
}

void Smarty::setup() {
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  radio.enableAckPayload();
  radio.setCRCLength(RF24_CRC_16);
  radio.setDataRate(RF24_1MBPS);
  radio.setChannel(90);    // will be read from the eeprom
  radio.setPayloadSize(8);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  radio.openWritingPipe((byte*)"brick");    // address of combrick
  radio.openReadingPipe(1, (byte*)"smrty"); // smarty address
}

void Smarty::setHardInputs(byte* p_hardAnalogInputPins, byte numHardAnalogInputPins, byte* p_hardDigitalInputPins, byte numHardDigitalInputPins) {
  mp_hardAnalogInputPins = p_hardAnalogInputPins;
  m_numHardAnalogInputPins = numHardAnalogInputPins;
  mp_hardDigitalInputPins = p_hardDigitalInputPins;
  m_numHardDigitalInputPins = numHardDigitalInputPins;
}

void Smarty::setSoftInputs(byte* p_softAnalogInputPins, byte numSoftAnalogInputPins, byte* p_softDigitalInputPins, byte numSoftDigitalInputPins) {
  mp_softAnalogInputPins = p_softAnalogInputPins;
  m_numSoftAnalogInputPins = numSoftAnalogInputPins;
  mp_softDigitalInputPins = p_softDigitalInputPins;
  m_numSoftDigitalInputPins = numSoftDigitalInputPins;
}

void Smarty::setCallbacks(GenericCmdCb p_genericCmdCb, SetAnalogCmdCb p_setAnalogCmdCb, SetDigitalCmdCb p_setDigitalCmdCb, DcMotorCmdCb p_dcMotorCmdCb) {
  mp_genericCmdCb = p_genericCmdCb;
  mp_setAnalogCmdCb = p_setAnalogCmdCb;
  mp_setDigitalCmdCb = p_setDigitalCmdCb;
  mp_dcMotorCmdCb = p_dcMotorCmdCb;
}

