#include "ComBrick.h"

#if defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
#define __AVR_ATtinyXX__
#endif

#ifndef __AVR_ATtinyXX__

ComBrick::ComBrick() :SmartyBase(true) {
	m_isComBrick = true;
	m_sync = PC_HEADER;
}

void ComBrick::checkPcMessage() {
	if (Serial.available() > 0) {                                // received a message from serial port (from PC)
		Serial.readBytes((char*)(&m_txMessage), CONSTANT_PAYLOAD_SIZE);

		sendDebug("Msg for smarty: %d", m_txMessage.m_smartyID);

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
	if (checkRx() >= 0)	{
		//Serial.write((uint8_t*)&m_helperSync, 4);
		//Serial.write((uint8_t*)(&m_rxMessage), CONSTANT_PAYLOAD_SIZE);
		sendMessageToSerial((uint8_t*)(&m_rxMessage));
		//printMessage((uint8_t*)(&m_rxMessage));
		if (m_msgDebug)
			printMessage((uint8_t*)(&m_rxMessage));
		//sendDebug("Msg from smarty: %d", m_rxMessage.m_smartyID);
	}
}

void ComBrick::sendMessageToSerial(uint8_t* p_message) {
	Serial.write((uint8_t*)&m_sync, 4);
	SmartyBase::sendMessageToSerial(p_message);
	//Serial.write(PC_FOOTER, 4);
}


#endif
