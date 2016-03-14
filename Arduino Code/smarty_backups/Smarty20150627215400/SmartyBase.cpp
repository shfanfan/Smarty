#include "SmartyBase.h"
#include "EEPROMs.h"
#include <stdarg.h>

SmartyBase::SmartyBase(bool isComBrick) {
	m_debugEnable = true;
	m_isComBrick = isComBrick;

	memset(&m_rxMessage, 0, CONSTANT_PAYLOAD_SIZE);
	memset(&m_txMessage, 0, CONSTANT_PAYLOAD_SIZE);
}

void SmartyBase::readConfig() {
#if USE_EEPROM
	int address = 0;
	if (EEPROM.read(address++) == EEPROM_HEADER1 && EEPROM.read(address++) == EEPROM_HEADER2)
	{
		// read the configuration from eeprom
		m_config.m_networkAddress[0] = EEPROM.read(address++);
		m_config.m_networkAddress[1] = EEPROM.read(address++);
		m_config.m_networkAddress[2] = EEPROM.read(address++);
		m_config.m_networkAddress[3] = EEPROM.read(address++);
		m_config.m_smartyID = EEPROM.read(address++);
		m_config.m_gatewayID = EEPROM.read(address++);
		m_config.m_channel = EEPROM.read(address++);
	}
	else
	{
#endif
		// set hard-coded just for test
		m_config.m_networkAddress[0] = 0xE8;
		m_config.m_networkAddress[1] = 0xE8;
		m_config.m_networkAddress[2] = 0xF0;
		m_config.m_networkAddress[3] = 0xF0;
		m_config.m_smartyID = (m_isComBrick ? GATEWAY_ADDRESS : 0xA5);
		m_config.m_gatewayID = GATEWAY_ADDRESS;
		m_config.m_channel = 90;

#if USE_EEPROM
		// and write to eeprom
		address = 0;
		EEPROM.write(address++, EEPROM_HEADER1);
		EEPROM.write(address++, EEPROM_HEADER2);
		EEPROM.write(address++, m_config.m_networkAddress[0]);
		EEPROM.write(address++, m_config.m_networkAddress[1]);
		EEPROM.write(address++, m_config.m_networkAddress[2]);
		EEPROM.write(address++, m_config.m_networkAddress[3]);
		EEPROM.write(address++, m_config.m_smartyID);
		EEPROM.write(address++, m_config.m_gatewayID);
		EEPROM.write(address++, m_config.m_channel);
	}
#endif

	for (int i = 0; i < 4; i++) {
		m_localAddress[i] = m_config.m_networkAddress[i];
		m_destAddress[i] = m_config.m_networkAddress[i];
		m_broadcastAddress[i] = m_config.m_networkAddress[i];
	}
	m_localAddress[4] = m_config.m_smartyID;
	m_destAddress[4] = (m_isComBrick ? 1 : m_config.m_gatewayID);
	m_broadcastAddress[4] = BROADCAST_ADDRESS;
}

bool SmartyBase::configRadio() {
	//Serial.println("configRadio");
	readConfig();

	m_radio.setPins(CE_PIN, CS_PIN);
	m_radio.begin();

	// Set the PA Level low to prevent power supply related issues since this is a
	// getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
	m_radio.setPALevel(RF24_PA_LOW);
	m_radio.enableAckPayload();
	m_radio.setCRCLength(RF24_CRC_16);
	m_radio.setDataRate(RF24_1MBPS);
	m_radio.setChannel(m_config.m_channel);
	m_radio.setPayloadSize(CONSTANT_PAYLOAD_SIZE);

	//Serial.println("end configRadio");
	return (m_radio.getPayloadSize() == CONSTANT_PAYLOAD_SIZE);
}

void SmartyBase::setupRadio() {
	Serial.begin(115200);
	//Serial.println("serial ok");

	if (!configRadio())
		sendDebug("Error initializing nrf24l01");
	else
		sendDebug("Success initializing nrf24l01");
	sendDebug("I am smarty # %d", m_config.m_smartyID);

	m_radio.openWritingPipe(m_destAddress);     // address of combrick

	m_radio.openReadingPipe(MAIN_RX_PIPE, m_localAddress);    // smarty address
	//m_radio.openReadingPipe(BROADCAST_RX_PIPE, m_broadcastAddress); // broadcast address

	m_radio.startListening();
}

int SmartyBase::checkRx() {
	uint8_t pipe_num = -1;
	if (m_radio.available(&pipe_num))
	{
		m_radio.read(&m_rxMessage, CONSTANT_PAYLOAD_SIZE);
		return pipe_num;
	}
	else
		return -1;
}

/* This method handles lowest-level transmission of a message to a destination
   The message is assumed to be complete and contained in the m_txMessage buffer
   The method checks if it is necessary to update the m_destAddress variable */
bool SmartyBase::sendMessageToAddr(uint8_t addr) {
	// First, stop listening so we can talk
	m_radio.stopListening();

	// Check if we have to update the destination address
	//if (m_destAddress[4] != addr) {
		m_destAddress[4] = addr;
		m_radio.openWritingPipe(m_destAddress);
	//}

	// Send the message
	bool res = m_radio.write(&m_txMessage, CONSTANT_PAYLOAD_SIZE);
	if (!res) sendDebug("failed to send message");

	// Now, resume listening so we catch the next packets.
	m_radio.startListening();

	return res;
}

/* This method handles network-level transmission of a message
   It checks the relevant path including eventual relay hops
   It calls sendMessageToAddr */
bool SmartyBase::sendMessageToID(uint8_t destID) {
	// This line should be extended to check the network map
	uint8_t relayID = destID;
	return sendMessageToAddr(relayID);
}

/* This methods sets the opcode and smartyID in the transmitted message
   It calls sendMessageToID */
bool SmartyBase::sendMessage(uint8_t opcode, uint8_t destID) {
	m_txMessage.m_opcode = opcode;
	m_txMessage.m_smartyID = (m_isComBrick? destID : m_config.m_smartyID);
	if (destID == m_config.m_smartyID){
		Serial.print("msg: [");
		uint8_t* p = (uint8_t*)(&m_txMessage);
		for (int i = 0; i < CONSTANT_PAYLOAD_SIZE; i++) {
			if (*p < 0x10) Serial.print("0");
			Serial.print(*p, HEX);
			p++;
			Serial.print(" ");
		}
		Serial.println("].");
		//Serial.write((uint8_t*)(&m_txMessage), CONSTANT_PAYLOAD_SIZE);
		return true;
	}
	else{
		return sendMessageToID(destID);
	}
}

bool SmartyBase::transferMessage() {
	memcpy(&m_txMessage, &m_rxMessage, CONSTANT_PAYLOAD_SIZE);
	return sendMessageToID(m_txMessage.m_smartyID);
}

void SmartyBase::setTxParam(uint8_t id, uint8_t value) {
	m_txMessage.m_params[id] = value;
}

void SmartyBase::sendDebug(const char* format, ...) {
	if (!m_debugEnable)
		return;

	//Serial.println("formatting string");
	memset(m_debugBuffer, 0, MAX_DEBUG_BUFFER);
	// first create the formatted string
	va_list va;
	va_start(va, format);
	int numChars = vsnprintf(m_debugBuffer, MAX_DEBUG_BUFFER-1, format, va);
	va_end(va);
	//Serial.println("end formatting string");

	if (m_localDebug)
		Serial.println(m_debugBuffer);
	else {
		int i = 0;
		// then send it in chunks of 8 bytes
		for (; i + CONSTANT_CONTENT_SIZE < strlen(m_debugBuffer); i += CONSTANT_CONTENT_SIZE) {
			memset(&m_txMessage, CONSTANT_PAYLOAD_SIZE, 0);   // could be omitted, just to do it clean and safe
			memcpy(m_txMessage.m_params, m_debugBuffer + i, CONSTANT_CONTENT_SIZE);
			sendMessage(OPCODE_DEBUG_MESSAGE_REPORT);
		}

		memset(&m_txMessage, CONSTANT_PAYLOAD_SIZE, 0);   // could be omitted, just to do it clean and safe
		if (i < strlen(m_debugBuffer))
			memcpy(m_txMessage.m_params, m_debugBuffer + i, strlen(m_debugBuffer) - i);
		sendMessage(OPCODE_DEBUG_MESSAGE_REPORT);

		//Send a new line at the end of the debug message
		char newline[] = { ' ', ' ', ' ', ' ', '\r', '\n' };
		memset(&m_txMessage, CONSTANT_PAYLOAD_SIZE, 0);   // could be omitted, just to do it clean and safe
		memcpy(m_txMessage.m_params, newline, CONSTANT_CONTENT_SIZE);
		sendMessage(OPCODE_DEBUG_MESSAGE_REPORT);
	}
}

