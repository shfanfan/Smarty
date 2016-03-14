#include <TimerObject.h>
#include <ComBrick.h>

ComBrick brick;
TimerObject *timer = new TimerObject(1000);
int counter = 0;

void debug() {
  brick.sendDebug("Debug Test %d\r\n", counter++);
  brick.sendDebug("01234567");
}
void setup() {
	brick.m_localDebug = false;
  brick.m_debugEnable = true;
  //brick.m_msgDebug = true;
	brick.setupRadio();
 
  timer->setOnTimer(&debug);
  timer->Start();
}

void loop() {
  brick.checkSmartyMessage();  
	brick.checkPcMessage();
  //timer->Update();
}

