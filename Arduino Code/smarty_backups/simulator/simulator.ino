
uint8_t buffer[8];
uint8_t analog1 = 0;
uint8_t analog2 = 0;
uint8_t analog3 = 0;
uint8_t digitals = 0;
uint8_t smartyId = 0X01;

void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
}

void loop() {  
  buffer[0] = smartyId;      // Smarty ID.
  buffer[1] = 0xD0;      // opcode for sensors data report
  buffer[2] = analog1;
  buffer[3] = analog2;
  buffer[4] = analog3;
  buffer[5] = map(analogRead(A0), 0, 1023, 0, 255);    // map A0 analog in to [0;255]
  buffer[6] = map(analogRead(A1), 0, 1023, 0, 255);    // map A1 analog in to [0;255]  
  buffer[7] = digitals;
  
  // simulate updates
  digitals ++;
  if(digitals > 30)  
    digitals = 0;
    
  smartyId++;
  if(smartyId > 8)
    smartyId = 1;
    
  analog1++;
  analog2 += 2;
  analog3 += 5;

#if 1  
  //send to helper
  Serial.write(buffer, 8);
  
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


