const int redPin = 9;
boolean ledState = true;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.println("ok");
  // make the pushbutton's pin an input:
  pinMode(redPin, OUTPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(redPin, ledState ? HIGH : LOW);
  ledState = !ledState;
  if (Serial.available()) {
    Serial.print("in: ");
    while (Serial.available())
      Serial.print((char)Serial.read());
    Serial.println("");
  }
  delay(1000);
}



