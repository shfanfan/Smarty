#include <TimerObject.h>
#include <Smarty.h>

Smarty smartyButton;
TimerObject *timer = new TimerObject(100);

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
	smartyButton.m_localDebug = true;
  //smartyButton.m_msgDebug = true;

	smartyButton.setHardPin(0, A0, DIGITAL_OUT);  // B
	smartyButton.setHardPin(1, A1, DIGITAL_OUT);  // G
	smartyButton.setHardPin(2, A2, DIGITAL_OUT);  // R
	smartyButton.setHardPin(3, 6, DIGITAL_IN);   // this is the button pin

  checkDigitalOut(A0, 100, 1);
  checkDigitalOut(A1, 100, 1);
  checkDigitalOut(A2, 100, 1);

  if (!smartyButton.setupRadio(HW_ARDUINO_NANO, 2)) {
  //if (!smartyButton.setupRadio(HW_ARDUINO_UNO, 2)) {
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
  smartyButton.sendSensorsData();
}

void loop() {
	smartyButton.poll();
	//processButtonPress();
  timer->Update();
}


