void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    if (Serial.read() == 0 &&
        Serial.read() == 0xC0) {
          if (Serial.read() == 1)
            digitalWrite(13, HIGH);
          else
            digitalWrite(13, LOW);
        }
    for (int i = 0; i < 5; i++)
      Serial.read();
  }
  sendMsgHead(0, 0xDD);
  Serial.print("test f");

  sendMsgHead(1, 0xDD);
  Serial.print("anothe");
    
  sendMsgHead(0, 0xDD);
  Serial.print("or deb");
  sendMsgHead(0, 0xDD);
  Serial.print("ug  \n\r");

  sendMsgHead(1, 0xDD);
  Serial.print("r one\n");
  
  delay(1000);
}

void sendMsgHead(byte smartyID, byte opcode) {
  Serial.write(0xEF);
  Serial.write(0xBE);
  Serial.write(0xAD);
  Serial.write(0xDE);
  Serial.write(smartyID);
  Serial.write(opcode);
}

