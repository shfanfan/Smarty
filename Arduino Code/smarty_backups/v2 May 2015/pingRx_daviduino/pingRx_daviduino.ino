#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 8 & 9

RF24 radio(10, 8);  // ce, cs

const short blue_pin = 5;
const short green_pin = 6;
const short red_pin = 9;

//
// Topology
//

// Single radio pipe address for the 2 nodes to communicate.
const uint64_t pipe = 0xE8E8F0F0E1LL;

//
// Role management
//
// Set up role.  This sketch uses the same software for all the nodes in this
// system.  Doing so greatly simplifies testing.  The hardware itself specifies
// which node it is.
//
// This is done through the role_pin
//

// The various roles supported by this sketch
typedef enum { role_sender = 1, role_receiver } role_e;

// The debug-friendly names of those roles
const char* role_friendly_name[] = { "invalid", "Sender", "Receiver"};

// The role of the current running sketch
role_e role;

// Interrupt handler, check the radio because we got an IRQ
void sender_check_radio(void);
void receiver_check_radio(void);

//
// Payload
//

const int min_payload_size = 4;
const int max_payload_size = 32;
int payload_size_increments_by = 2;
int next_payload_size = min_payload_size;

char receive_payload[max_payload_size+1]; // +1 to allow room for a terminating NULL char

//
// Test state
//

const int interval = 1; //*< ms to wait between sends */

uint8_t pipe_number = 1; // Which pipe to send on.

//
// Setup 
//

void setup(void)
{
  //detachInterrupt(0);

  pinMode(red_pin, OUTPUT);
  pinMode(green_pin, OUTPUT);
  pinMode(blue_pin, OUTPUT);

  role = role_receiver;

  //
  // Print preamble
  //

  Serial.begin(2400);
  //printf_begin();
  Serial.print("RF24 tests pingpair_test, role: ");
  Serial.println(role_friendly_name[role]);

  //
  // Setup and configure rf radio
  //

  if (!radio.begin())
    Serial.println("Error initializing nRF24L01");

  // We will be using the Ack Payload feature, so please enable it
  radio.enableAckPayload();

  // Optional: Increase CRC length for improved reliability
  radio.setCRCLength(RF24_CRC_16);

  // Optional: Decrease data rate for improved reliability
  radio.setDataRate(RF24_1MBPS);

  // Optional: Pick a high channel
  radio.setChannel(90);

  /*{
    next_payload_size = 16;
    payload_size_increments_by = 0;
    radio.setPayloadSize(next_payload_size);
  }*/

  // enable dynamic payloads
  radio.enableDynamicPayloads();

  if ( role == role_sender )
  {
    radio.openReadingPipe(5, (uint64_t)0);
  }

  //
  // Open pipes to other nodes for communication
  //

  // This simple sketch opens a single pipe for these two nodes to communicate
  // back and forth.  One listens on it, the other talks to it.

  if ( role == role_sender )
  {
    radio.openWritingPipe(pipe);
  }
  else
  {
    radio.openReadingPipe(pipe_number,pipe);
  }

  //
  // Dump the configuration of the rf unit for debugging
  //

  radio.printDetails();

  //
  // Attach interrupt handler to interrupt #0 (using pin 2)
  // on BOTH the sender and receiver
  //

  if ( role == role_receiver )
  {
    attachInterrupt(0, receiver_check_radio, FALLING);
    Serial.println("OK, listening.");
  }
  else
  {
    attachInterrupt(0, sender_check_radio, FALLING);
  }
  delay(50);

  //
  // Start listening
  //

  if ( role == role_receiver )
    radio.startListening();
}

//
// Loop 
//

static uint32_t message_count = 0;
static uint32_t last_message_count = 0;

void loop(void)
{
  //
  // Sender role.  Repeatedly send the current time
  //

  if (role == role_sender)
  {
    // The payload will always be the same, what will change is how much of it we send.
    static char send_payload[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ789012";

    // First, stop listening so we can talk.
    radio.stopListening();

    // Send it.  This will block until complete
    //printf("\n\rNow sending length %i...",next_payload_size);
    radio.startWrite( send_payload, next_payload_size,0 );

    // Update size for next time.
    next_payload_size += payload_size_increments_by;
    if ( next_payload_size > max_payload_size )
      next_payload_size = min_payload_size;
    
    // Try again soon
    delay(interval);
    
    // Timeout if we have not received anything back ever
    if ( ! last_message_count && millis() > interval * 100 )
    {
      //Serial.println("No responses received.  Are interrupts connected??\n\r");
    }
  }

  //
  // Receiver role: Does nothing!  All the work is in IRQ
  //
  
}

void sender_check_radio(void)
{
  // What happened?
  bool tx,fail,rx;
  radio.whatHappened(tx,fail,rx);

  // Have we successfully transmitted?
  //if ( tx )    Serial.print("Send:OK ");

  // Have we failed to transmit?
  if ( fail )  Serial.print("Send:Failed ");

  // Not powering down since radio is in standby mode
  //if ( ( tx || fail ) && ( role == role_sender ) )
    //radio.powerDown();

  // Did we receive a message?
  if ( rx )
  {
    // If we're the sender, we've received an ack payload
    radio.read(&message_count,sizeof(message_count));
    //if (message_count % 100 == 0)
    {
      Serial.print("Ack: ");
      Serial.print(message_count);
   
      // is this ack what we were expecting?  to account
      // for failures, we simply want to make sure we get a
      // DIFFERENT ack every time.
      if ( message_count != last_message_count )
      {
  	Serial.println("  OK");
      }
      else
      {
  	Serial.println("  FAILED");
      }
    }
    last_message_count = message_count;
  }
}

void receiver_check_radio(void)
{
  // What happened?
  bool tx,fail,rx;
  static bool led = false;
  radio.whatHappened(tx,fail,rx);

  Serial.print("<int> ");
  
  // Have we successfully transmitted?
  //if ( tx )    Serial.println("Ack Payload:Sent ");

  // Have we failed to transmit?
  if ( fail )  Serial.println("Ack Payload:Failed ");

  // Not powering down since radio is in standby mode
  //if ( ( tx || fail ) && ( role == role_sender ) )
    //radio.powerDown();

  // Did we receive a message?
  if ( rx )
  {
    // If we're the receiver, we've received a time message
    // Get this payload and dump it
    size_t len = max_payload_size;
    memset(receive_payload,0,max_payload_size);
    
    len = radio.getDynamicPayloadSize();
    
    radio.read( receive_payload, len );
    
    // Put a zero at the end for easy printing
    receive_payload[len] = 0;

    // Spew it
    //prbuf_in += sprintf(prbuf_in,"Recv size=%i val=%s len=%u\n\r",len,receive_payload,strlen(receive_payload));

    /*Serial.print("len = ");
    Serial.println(len);
    /*Serial.print(" msg count = ");
    Serial.println(message_count);*/
   // Add an ack packet for the next time around.
    radio.writeAckPayload( pipe_number, &message_count, sizeof(message_count) );

    ++message_count;
    
    //if (message_count % 100 == 0)
    {
      digitalWrite(red_pin, led ? HIGH : LOW);
      led = !led;
    }
  }
}

