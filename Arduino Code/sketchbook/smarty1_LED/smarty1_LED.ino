#include <TimerObject.h>
#include <Smarty.h>

Smarty smartyLight;
TimerObject *timer = new TimerObject(490);
uint8_t counter = 0;

void setup() {
	smartyLight.m_localDebug = true;
	smartyLight.setupRadio(1);

	smartyLight.setHardPin(0, A0, ANALOG_IN);
	smartyLight.setHardPin(1, A1, ANALOG_IN);
	smartyLight.setHardPin(2, A2, ANALOG_IN);
	smartyLight.setHardPin(3, 3, ANALOG_OUT);    // this is the LED pin, with pwm  
	smartyLight.setSoftPin(2, &counter, ANALOG_IN);
	int i = 0;
	for (i = 0; i < 3; i++) {
		analogWrite(3, 250);	// turn the LED on (HIGH is the voltage level)
		delay(300);             // wait for a second
		analogWrite(3, 0);      // turn the LED off by making the voltage LOW
		delay(200);
	}
  
  timer->setOnTimer(&status);
  timer->Start();
}

void status() {
  smartyLight.sendSensorsData();
  counter++;
}

void loop() {
	smartyLight.poll();
	//processButtonPress();
  timer->Update();
}


