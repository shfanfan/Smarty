#ifndef COMBRICK_H
#define COMBRICK_H

#include "SmartyBase.h"

class ComBrick : public SmartyBase {
protected:
	unsigned long m_sync;
	
	virtual void sendMessageToSerial(uint8_t*);
	
public:
	ComBrick();
	void checkPcMessage();
	void checkSmartyMessage();
};

#endif

