#include <SPI.h>
#include "RF24.h"

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 1;

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(10,8);
//RF24 radio(9,10);
/**********************************************************/

byte addresses[][6] = {"1Node","2Node"};

// Used to control whether this node is sending or receiving
bool role = 0;

#define BUFFER_LENGTH 32
char command[BUFFER_LENGTH];
//char report[BUFFER_LENGTH];
char report[BUFFER_LENGTH] = "Smarty periodic report";
const char sync[] = "#smarty#";
int len = 0;
char cmd = 0;
byte device_id = 0;

const short blue_pin = 5;
const short green_pin = 6;
const short red_pin = 9;

void setup() {
  Serial.begin(57600);
  Serial.println(F("Smarty Daviduino v1"));
  
  pinMode(red_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);

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
  
  digitalWrite(green_pin, HIGH);
  delay(100);
  digitalWrite(green_pin, LOW);
}

void loop() {
  if( radio.available()){
    radio.read(command, 32);             // Get the payload
    
    // and now process the command
    //...
    digitalWrite(red_pin, HIGH);
    delay(100);
    digitalWrite(red_pin, LOW);
  }
   
  {
    radio.stopListening();                                        // First, stop listening so we can talk
    radio.write(report, 32);              // Send the final one back.      
    radio.startListening();                                       // Now, resume listening so we catch the next packets.     
    //Serial.print(F("Sent response "));
    //Serial.write((const unsigned char*)buffer, 32);
    delay(100);
  }
} // Loop

