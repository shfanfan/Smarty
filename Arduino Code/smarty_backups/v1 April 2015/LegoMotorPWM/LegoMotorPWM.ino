const int pwm1 = 3;
const int pwm2 = 5;


void setup() {
  pinMode(pwm1, OUTPUT);
  pinMode(pwm2, OUTPUT);
}

void loop() {
  digitalWrite(pwm1, HIGH);
  digitalWrite(pwm2, LOW);
  delay(1000);
  digitalWrite(pwm2, HIGH);
  digitalWrite(pwm1, LOW);
  delay(1000);
}

