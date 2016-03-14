#ifndef SMARTY_H
#define SMARTY_H

#include "SmartyBase.h"

typedef enum PIN_TYPE { ANALOG_IN = 1, ANALOG_OUT, DIGITAL_IN, DIGITAL_OUT };

// this struct will hold the pin configurations
typedef struct PinConfig
{
	//uint8_t  m_pinId;
	uint8_t  m_pinHwId;
	PIN_TYPE m_type;
};

// this struct will hold the soft configurations
typedef struct SoftConfig
{
	uint8_t*  mp_value;
	PIN_TYPE m_type;
};

// this struct will hold pin information both for analog and for digital pin commands
typedef struct PinCmd
{
	uint8_t  m_pinId;
	uint8_t  m_value;
};

// define pointer types for the different callbacks
typedef void(*GenericCmdCb)(uint8_t cmd, uint8_t params[6]);
typedef void(*SetAnalogCmdCb)(PinCmd cmds[3]);
typedef void(*SetDigitalCmdCb)(PinCmd cmds[3]);
typedef void(*DcMotorCmdCb)(bool speedUpdate, uint8_t speedVal, bool dirUpdate, uint8_t dirVal);
typedef void(*DefaultCmdCb)(uint8_t buffer[8]);

// this is the Smarty class
class Smarty : public SmartyBase {
protected:
	PinConfig    m_pins[NUM_HW_PINS];
	SoftConfig   m_softs[NUM_SW_VALS];
	uint8_t      m_buttonPin;
	uint8_t      m_prevButtonState;

	GenericCmdCb      mp_genericCmdCb;
	SetAnalogCmdCb    mp_setAnalogCmdCb;
	SetDigitalCmdCb   mp_setDigitalCmdCb;
	DcMotorCmdCb      mp_dcMotorCmdCb;
	DefaultCmdCb      mp_defaultCmdCb;

public:
	// constructor
	Smarty();

	// setup methods
	void setHardPin(uint8_t pinId, uint8_t pinHwId, PIN_TYPE type);
	void setSoftPin(uint8_t pinId, uint8_t* pointer, PIN_TYPE type);
	void setButtonPin(uint8_t pin);
	void setCallbacks(GenericCmdCb p_genericCmdCb = NULL, SetAnalogCmdCb p_setAnalogCmdCb = NULL, SetDigitalCmdCb p_setDigitalCmdCb = NULL, DcMotorCmdCb p_dcMotorCmdCb = NULL, DefaultCmdCb p_defaultCmdCb = NULL);

	// processing methods (in loop)
	void poll();
	void processButtonPress();
	void sendSensorsData();

	//TODO: process incomming messages and invoke propper callbacks (add callback to handle commands)
	//TODO: send hard and soft sensors data
	//TODO: process button press 
	//TODO: implement message polling from the NRF
	//TODO: implement special comm to combrick for RF configuration
};

#endif
