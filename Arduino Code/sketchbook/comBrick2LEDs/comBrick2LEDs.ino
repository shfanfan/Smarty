  #include <TimerObject.h>
#include <ComBrick.h>

ComBrick brick;
TimerObject *timer = new TimerObject(300);
const uint8_t smartyID = 0x01;
const uint8_t smartyPinIDs[] = {3,5,6};
int pinCounter = 0;
bool pinState = false;

void sendPing() {
	brick.setTxParam(0, smartyPinIDs[pinCounter]);
  brick.setTxParam(1, pinState ? 0 : 150);
  pinState = !pinState;
  //if (pinCounter == 1)
	//  brick.sendMessage(OPCODE_SET_DIGITAL_CMD, smartyID);
  //else
    brick.sendMessage(OPCODE_SET_ANALOG_CMD, smartyID);
  if (pinState == false) {
    pinCounter++;
    if (pinCounter == 3)  pinCounter = 0;
  }
}

void setup() {
	brick.m_localDebug = true;
	brick.setupRadio(HW_ARDUINO_UNO);
 
  timer->setOnTimer(&sendPing);
  timer->Start();
}

void loop() {
  brick.checkSmartyMessage();
  timer->Update();
}
