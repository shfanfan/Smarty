#include "ComBrick.h"

#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
#define __AVR_ATtinyXX__
#endif

#ifndef __AVR_ATtinyXX__

ComBrick::ComBrick():SmartyBase() {
	m_isComBrick = true;
}

void ComBrick::setupComBrick() {
	Serial.begin(115200);
	if (!setupRadio()) sendDebug("Error initializing nrf24l01");
	else sendDebug("Success initializing nrf24l01");

  m_radio.openWritingPipe(m_destAddress);     // address of combrick
  m_radio.openReadingPipe(COMBRICK_RX_PIPE, m_localAddress);    // smarty address

  m_radio.startListening();  
  sendDebug("comBrick v2");
}

void ComBrick::checkPcMessage() {
  if (Serial.available() > 0) {                                // received a message from serial port (from PC)
    Serial.readBytes((char*)(&m_txMessage), CONSTANT_PAYLOAD_SIZE);
  
    sendDebug("Received message for smarty: %d", m_txMessage.m_smartyID);

    sendMessageToID(m_txMessage.m_smartyID);

/*
    // First, stop listening so we can talk.  
    m_radio.stopListening();
    
    // TODO: check the network map if we need more hops
    m_destAddress[4] = m_txMessage.m_smartyID;
    m_radio.openWritingPipe(m_destAddress);

    // send the whole message to smarty
	if (!m_radio.write(&m_txMessage, CONSTANT_PAYLOAD_SIZE)){
      sendDebug("failed");
  	}

    // Now, continue listening
    m_radio.startListening();
*/
  }

}

void ComBrick::checkSmartyMessage() {
  // check if received a message from smarty
  if (checkRx() >= 0)
    Serial.write((uint8_t*)(&m_rxMessage), CONSTANT_PAYLOAD_SIZE);
}

#endif
