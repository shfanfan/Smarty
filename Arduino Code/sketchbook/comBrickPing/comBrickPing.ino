#include <TimerObject.h>
#include <ComBrick.h>

ComBrick brick;
TimerObject *timer = new TimerObject(1000);
const uint8_t smartyID = 0x01;
const uint8_t smartyPinID = 6;
bool pinState = false;

void sendPing() {
	brick.setTxParam(0, smartyPinID);
  brick.setTxParam(1, pinState ? 0 : 150);
  pinState = !pinState;
	brick.sendMessage(OPCODE_SET_ANALOG_CMD, smartyID);
}

void setup() {
	brick.m_localDebug = true;
  //brick.m_msgDebug = true;
  brick.setupRadio(HW_ARDUINO_UNO);
 
  timer->setOnTimer(&sendPing);
  timer->Start();
}

void loop() {
  brick.checkSmartyMessage();
  timer->Update();
}
