
/*
* Getting Started example sketch for nRF24L01+ radios
* This is a very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/

#include <SPI.h>
#include "RF24.h"

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 0;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(9,10);
/**********************************************************/

byte addresses[][6] = {"1Node","2Node"};

#define BUFFER_LENGTH 32
char command[BUFFER_LENGTH];
char report[BUFFER_LENGTH];
const char sync[] = "#smarty#";
int len = 0;
char cmd = 0;
byte device_id = 0;

// Used to control whether this node is sending or receiving
bool role = 1;

void clearBuffer(char* buffer, int len) {
  for (int i = 0; i < len; i++) {
    buffer[i] = 0;
  }
}

void setup() {
  Serial.begin(57600);
  Serial.println(F("Smarty Brick v1"));
  
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
 // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_LOW);
  radio.enableAckPayload();
  radio.setCRCLength(RF24_CRC_16);
  radio.setDataRate(RF24_1MBPS);
  radio.setChannel(90);
  radio.setPayloadSize(32);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
  
  // Start the radio listening for data
  radio.startListening();
}

void loop() {
  if (Serial.available() > 0) {                                // received a message from serial port (from PC)
    clearBuffer(command, BUFFER_LENGTH);
    len = Serial.readBytesUntil(10, command, BUFFER_LENGTH-1);
    Serial.print("Received message from PC: ");
    if (len > 32) len = 32;
    Serial.write((const unsigned char*)command, len);
    Serial.println();
  
    radio.stopListening();                                    // First, stop listening so we can talk.
    
     if (!radio.write((const unsigned char*)command, 32)){    // send the whole message to smarty
       Serial.println(F("failed"));
     }
        
    radio.startListening();                                    // Now, continue listening
  }
  
  if ( radio.available() )  {                                 // received a message from smarty
      radio.read(report, 32);
      Serial.print("Received message from smarty: ");
      Serial.write((const unsigned char*)report, 32);
      Serial.println();
  }

  delay(10);
} // Loop

