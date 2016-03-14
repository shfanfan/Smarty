#include <TimerObject.h>
#include <Smarty.h>

Smarty smartyLight;
TimerObject *timer = new TimerObject(490);

void checkAnalogOut(int pin, int delay_us, int cycles) {
  int i = 0, a = 0;
  for (i = 0; i < cycles; i++) {
    for (a = 0; a <= 255; a++) {
      analogWrite(pin, a);
      delayMicroseconds(delay_us);
    }
    for (a = 255; a >= 0; a--) {
      analogWrite(pin, a);
      delayMicroseconds(delay_us);
    }
  }  
}

void checkDigitalOut(int pin, int delay_ms, int cycles) {
  for (int i = 0; i < cycles; i++) {
      digitalWrite(pin, HIGH);
      delay(delay_ms);
      digitalWrite(pin, LOW);
      delay(delay_ms);
  }
}

void setup() {
	smartyLight.m_localDebug = true;

	smartyLight.setHardPin(0, A0, ANALOG_OUT);  // B
	smartyLight.setHardPin(1, A1, ANALOG_OUT);  // G
	smartyLight.setHardPin(2, A2, ANALOG_OUT);  // R
	smartyLight.setHardPin(3, 3, ANALOG_OUT);   // this is the Green2 LED pin, with pwm
  smartyLight.setHardPin(4, A5, DIGITAL_OUT);  // this is the Green1 LED pin, with pwm
  smartyLight.setHardPin(5, 5, ANALOG_OUT);   // this is the Red1 LED pin, with pwm
  smartyLight.setHardPin(6, 6, ANALOG_OUT);   // this is the Red2 LED pin, with pwm

  checkDigitalOut(A0, 100, 1);
  checkDigitalOut(A1, 100, 1);
  checkDigitalOut(A2, 100, 1);

  checkAnalogOut(3, 300, 1);
  checkDigitalOut(A5, 100, 1);
  checkAnalogOut(5, 300, 1);
  checkAnalogOut(6, 300, 1);

  if (!smartyLight.setupRadio(HW_ARDUINO_NANO, 1)) {
    digitalWrite(A2, HIGH);
    delay(1000);
    digitalWrite(A2, LOW);
  }
  else {
    digitalWrite(A1, HIGH);
    delay(1000);
    digitalWrite(A1, LOW);
  }
    
  timer->setOnTimer(&status);
  timer->Start();
}

void status() {
  smartyLight.sendSensorsData();
}

void loop() {
	smartyLight.poll();
	//processButtonPress();
  timer->Update();
}


