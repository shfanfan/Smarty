const int pinSpeed = 6;
const int pinPlus = 7;
const int pinMinus = 8;
/*const int pinSpeed = 3;
const int pinPlus = 2;
const int pinMinus = 4;*/

void setup() {
  pinMode(pinSpeed, OUTPUT);
  pinMode(pinPlus, OUTPUT);
  pinMode(pinMinus, OUTPUT);
  
  digitalWrite(pinPlus, HIGH);
  digitalWrite(pinMinus, LOW);
  analogWrite(pinSpeed, 255);
}

void loop() {
}
