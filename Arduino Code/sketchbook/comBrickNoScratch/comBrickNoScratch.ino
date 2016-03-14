#include <TimerObject.h>
#include <ComBrick.h>

ComBrick brick;

void setup() {
	brick.m_localDebug = true;
  brick.m_debugEnable = true;
  brick.m_msgDebug = true;
	brick.setupRadio(HW_ARDUINO_UNO);
}

void loop() {
  brick.checkSmartyMessage();  
}

