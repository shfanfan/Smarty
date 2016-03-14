#include "Smarty.h"

int pipe_num = 0;
uint8_t opcode = 0;
PinCmd pinCmds[3];

Smarty::Smarty() :SmartyBase() {
	m_isComBrick = false;
	mp_genericCmdCb = NULL;
	mp_setAnalogCmdCb = NULL;
	mp_setDigitalCmdCb = NULL;
	mp_dcMotorCmdCb = NULL;
	mp_defaultCmdCb = NULL;

	memset(m_pins, NUM_HW_PINS*sizeof(PinConfig), 0);
	memset(m_softs, NUM_SW_VALS*sizeof(SoftConfig), 0);
	m_buttonPin = 255;
	m_prevButtonState = HIGH;
}

void Smarty::setupSmarty() {
	Serial.begin(115200);
	if (!setupRadio())
		Serial.println("Error initializing nrf24l01");
	else
		sendDebug("Success initializing nrf24l01");
	sendDebug("I am smarty # %d", m_config.m_smartyID);

	m_radio.openWritingPipe(m_destAddress);     // address of combrick

	m_radio.openReadingPipe(SMARTY_RX_PIPE, m_localAddress);    // smarty address
	//m_radio.openReadingPipe(BROADCAST_RX_PIPE, m_broadcastAddress); // broadcast address

	m_radio.startListening();
}

void Smarty::setHardPin(uint8_t pinId, uint8_t pinHwId, PIN_TYPE type) {
	// check if id is within the bounds
	if (pinId >= NUM_HW_PINS) return;
	// check if the pin is free
	if (pinHwId == 10 || pinHwId == 11 || pinHwId == 12 || pinHwId == CE_PIN || pinHwId == CS_PIN)  return;

	m_pins[pinId].m_pinHwId = pinHwId;
	m_pins[pinId].m_type = type;
	pinMode(pinHwId, (type == ANALOG_OUT || type == DIGITAL_OUT) ? OUTPUT : INPUT);

	sendDebug("Hard pin set");
}

void Smarty::setSoftPin(uint8_t pinId, uint8_t* pointer, PIN_TYPE type) {
	// check if id is within the bounds
	if (pinId >= NUM_SW_VALS) return;
	m_softs[pinId].mp_value = pointer;
	m_softs[pinId].m_type = type;
}

void Smarty::setButtonPin(uint8_t pin) {
	m_buttonPin = pin;
	pinMode(pin, INPUT_PULLUP);
}

void Smarty::setCallbacks(
	GenericCmdCb p_genericCmdCb, SetAnalogCmdCb p_setAnalogCmdCb, SetDigitalCmdCb p_setDigitalCmdCb,
	DcMotorCmdCb p_dcMotorCmdCb, DefaultCmdCb p_defaultCmdCb) {

	mp_genericCmdCb = p_genericCmdCb;
	mp_setAnalogCmdCb = p_setAnalogCmdCb;
	mp_setDigitalCmdCb = p_setDigitalCmdCb;
	mp_dcMotorCmdCb = p_dcMotorCmdCb;
	mp_defaultCmdCb = p_defaultCmdCb;
}

void Smarty::poll() {
	if (m_buttonPin < 255)
		if (digitalRead(m_buttonPin) == LOW && m_prevButtonState == HIGH) {
			m_prevButtonState = LOW;
			// fire a message to the combrick
		}
		else if (digitalRead(m_buttonPin) == HIGH && m_prevButtonState == LOW) {
			m_prevButtonState = HIGH;
		}

		//  if ( m_radio.available(&pipe_num) ) {
		//    m_radio.read(&m_rxMessage, CONSTANT_PAYLOAD_SIZE);
		if ((pipe_num = checkRx()) >= 0) {

			sendDebug("Received message from pipe  %d", pipe_num);
			sendDebug("Opcode is %d", m_rxMessage.m_opcode);

			if (pipe_num == SMARTY_RX_PIPE && m_rxMessage.m_smartyID == m_config.m_smartyID) {
				//Serial.println("This command is for me");
				switch (m_rxMessage.m_opcode) {
				case OPCODE_GENERIC_CMD:
					if (mp_genericCmdCb != NULL)
						mp_genericCmdCb(m_rxMessage.m_params[0], m_rxMessage.m_params + 1);
					break;
				case OPCODE_SET_ANALOG_CMD:
					if (mp_setAnalogCmdCb != NULL) {
						memcpy(pinCmds, m_rxMessage.m_params, 6);
						mp_setAnalogCmdCb(pinCmds);
					}
					else {
						sendDebug("set analog command for me");
						memcpy(pinCmds, m_rxMessage.m_params, 6);
						for (int i = 0; i < NUM_HW_PINS; i++) {
							if (pinCmds[i].m_pinId < NUM_HW_PINS && m_pins[pinCmds[i].m_pinId].m_type == ANALOG_OUT)
							{
								analogWrite(m_pins[pinCmds[i].m_pinId].m_pinHwId, pinCmds[i].m_value);
								sendDebug("Found pin");
							}
						}
					}
					break;
				case OPCODE_SET_DIGITAL_CMD:
					if (mp_setDigitalCmdCb != NULL) {
						memcpy(pinCmds, m_rxMessage.m_params, 6);
						mp_setDigitalCmdCb(pinCmds);
					}
					else {
						memcpy(pinCmds, m_rxMessage.m_params, 6);
						for (int i = 0; i < NUM_HW_PINS; i++) {
							if (pinCmds[i].m_pinId < NUM_HW_PINS && m_pins[pinCmds[i].m_pinId].m_type == ANALOG_OUT)
								digitalWrite(m_pins[pinCmds[i].m_pinId].m_pinHwId, pinCmds[i].m_value == 0 ? LOW : HIGH);
						}
					}
					break;
				case OPCODE_DC_MOTOR_CMD:
					if (mp_dcMotorCmdCb != NULL)
						mp_dcMotorCmdCb(m_rxMessage.m_params[1], m_rxMessage.m_params[2], m_rxMessage.m_params[3], m_rxMessage.m_params[4]);
					break;
				default:
					if (mp_defaultCmdCb != NULL)
						mp_defaultCmdCb(&m_rxMessage.m_opcode);
					break;
				}
			}
			else if (pipe_num == SMARTY_RX_PIPE) {
				// the message is intended for another smarty - transfer it (relay mode)
				// this should depend on a network map saved in the eeprom
				// design should allow unlimited hops
				transferMessage();
			}
			else if (pipe_num == BROADCAST_RX_PIPE) {

			}
		}
}

void Smarty::sendSensorsData() {
	// build the report message
	int analogCounter = 0, digitalCounter = 0;
	memset(&m_txMessage, CONSTANT_PAYLOAD_SIZE, 0);

	for (int i = 0; i < NUM_HW_PINS; i++) {
		if (m_pins[i].m_type == ANALOG_IN && analogCounter < NUM_ANALOGS_IN_DATA_REPORT)
			//m_txBuffer[1+analogCounter++] = 0xBB;
			m_txMessage.m_params[2 + analogCounter++] = map(analogRead(m_pins[i].m_pinHwId), 0, 1023, 0, 255);
		else if (m_pins[i].m_type == DIGITAL_IN && digitalCounter < 8)
			bitWrite(m_txMessage.m_params[6], digitalCounter++, digitalRead(m_pins[i].m_pinHwId) == 0 ? 0 : 1);
	}

	for (int i = 0; i < NUM_SW_VALS; i++) {
		if (m_softs[i].m_type == ANALOG_IN && analogCounter < NUM_ANALOGS_IN_DATA_REPORT && m_softs[i].mp_value != NULL)
			m_txMessage.m_params[2 + analogCounter++] = *m_softs[i].mp_value;
		else if (m_softs[i].m_type == DIGITAL_IN && digitalCounter < 8 && m_softs[i].mp_value != NULL)
			bitWrite(m_txMessage.m_params[6], digitalCounter++, *m_softs[i].mp_value == 0 ? 0 : 1);
	}

	sendMessage(OPCODE_SENSORS_DATA_REPORT);
}
