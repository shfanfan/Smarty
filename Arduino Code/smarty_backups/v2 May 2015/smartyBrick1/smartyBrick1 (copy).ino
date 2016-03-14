
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

#define BUFFER_LENGTH 50
char buffer[BUFFER_LENGTH];
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
  if (Serial.available() > 0) {
    clearBuffer(buffer, BUFFER_LENGTH);
    len = Serial.readBytesUntil(10, buffer, BUFFER_LENGTH-1);
    Serial.print("Received command: ");
    Serial.write((const unsigned char*)buffer, len);
    Serial.println();
  }
  
  radio.stopListening();                                    // First, stop listening so we can talk.
  
  
  //Serial.println(F("Now sending"));

  unsigned long time = micros();                             // Take the time, and send it.  This will block until complete
   if (!radio.write( &time, sizeof(unsigned long) )){
     Serial.println(F("failed"));
   }
      
  radio.startListening();                                    // Now, continue listening
  
  unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
  boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
  
  while ( ! radio.available() ){                             // While nothing is received
    if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
        timeout = true;
        break;
    }      
  }
      
  if ( timeout ){                                             // Describe the results
      Serial.println(F("Failed, response timed out."));
  }else{
      unsigned long got_time;                                 // Grab the response, compare, and send to debugging spew
      radio.read( &got_time, sizeof(unsigned long) );
      unsigned long time = micros();
      
      // Spew it
      //Serial.print(F("Sent "));
      //Serial.print(time);
      //Serial.print(F(", Got response "));
      //Serial.print(got_time);
      //Serial.print(F(", Round-trip delay "));
      //Serial.print(time-got_time);
      //Serial.println(F(" usecs"));
  }

  // Try again 1s later
  delay(1000);
} // Loop

