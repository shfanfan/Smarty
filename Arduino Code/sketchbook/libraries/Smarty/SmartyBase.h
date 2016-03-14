#ifndef SMARTYBASE_H
#define SMARTYBASE_H

#include <Arduino.h>
#include "Smarty_icd.h"
#include "Smarty_config.h"
#include "RF24s.h"

// this struct will hold the RF configuration parameters saved in the eeprom
typedef struct RFparams
{
	uint8_t  m_networkAddress[4];
	uint8_t  m_smartyID;
	uint8_t  m_gatewayID;
	uint8_t  m_channel;
};

class SmartyBase {
protected:
	RF24 	  m_radio;
	RFparams  m_config;
	char      m_debugBuffer[MAX_DEBUG_BUFFER];

	uint8_t   m_localAddress[5];
	uint8_t   m_destAddress[5];
	uint8_t   m_broadcastAddress[5];

	Message   m_rxMessage;
	Message   m_txMessage;
	Message   m_txMessage_tmp;

	bool      m_isComBrick;

	SmartyBase(bool isComBrick);

	void readConfig();
	bool configRadio(uint8_t hwType);

	int  checkRx();

	bool sendMessageToAddr(uint8_t addr);
	bool sendMessageToID(uint8_t destID);
	bool transferMessage();
	
	void printMessage(uint8_t*);
	
	virtual void sendMessageToSerial(uint8_t*);

public:
	bool      m_debugEnable;	// to enable or disable sendDebug output
	bool      m_localDebug;		// to redirect sendDebug to local serial or to sending smarty 0xDD messages
	bool	  m_msgDebug;		// to redirect all sendMessage to printMessage on local serial

	bool setupRadio(uint8_t hwType, uint8_t smartyID = 0);
	bool sendMessage(uint8_t opcode, uint8_t destID = GATEWAY_ADDRESS);
	void setTxParam(uint8_t id, uint8_t value);
	void sendDebug(const char* format, ...);
};

#endif
