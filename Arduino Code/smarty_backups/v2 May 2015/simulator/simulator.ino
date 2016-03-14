
uint8_t buffer[10];
uint8_t analog1 = 0;
uint8_t analog2 = 0;
uint8_t analog3 = 0;
uint8_t digitals = 0;

void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
}

void loop() {
  buffer[0] = 0x01;      // source id
  buffer[1] = 0x00;      // dest id. the combrick is 0
  buffer[2] = 0xD0;      // opcode for sensors data report
  buffer[3] = analog1;
  buffer[4] = analog2;
  buffer[5] = analog3;
  buffer[6] = map(analogRead(A0), 0, 1023, 0, 255);    // map A0 analog in to [0;255]
  buffer[7] = map(analogRead(A1), 0, 1023, 0, 255);    // map A1 analog in to [0;255]
  buffer[8] = map(analogRead(A2), 0, 1023, 0, 255);    // map A2 analog in to [0;255]
  buffer[9] = digitals;
  
  // simulate updates
  digitals = 0x10 - digitals;    // toggles bit 4
  analog1++;
  analog2 += 2;
  analog3 += 5;

#if 0  
  // send to helper
  Serial.write(buffer, 10);
  
#else

  for (int i = 0; i < 10; i++)
  {
    if (buffer[i] < 16)
      Serial.print("0");
    Serial.print(buffer[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  
#endif

  delay(1000);
}


