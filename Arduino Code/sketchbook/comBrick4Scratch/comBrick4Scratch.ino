#include <TimerObject.h>
#include <ComBrick.h>

ComBrick brick;
TimerObject *timer = new TimerObject(1000);
unsigned long counter = 0;

void keep_alive() {
  brick.sendDebug("KA %5d", counter++);
}

void setup() {
	brick.m_localDebug = false;
  brick.m_debugEnable = true;
  brick.m_msgDebug = false;
	brick.setupRadio(HW_ARDUINO_UNO);

  timer->setOnTimer(&keep_alive);
  timer->Start();
}

void loop() {
  brick.checkSmartyMessage();  
	brick.checkPcMessage();
  timer->Update();
}

