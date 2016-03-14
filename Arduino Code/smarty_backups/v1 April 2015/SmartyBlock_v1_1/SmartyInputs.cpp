#include "ISmarty.h"
#include "SmartyInputs.h"
#include "HelperFunctions.h"
#include <Arduino.h>

SmartyInputs::SmartyInputs(byte id, byte num_analog_inputs, byte num_digital_inputs, byte* analog_pins, byte* digital_pins)
{
  m_deviceID = id;
  m_num_analog_inputs = num_analog_inputs;
  m_num_digital_inputs = num_digital_inputs;
  mp_analog_pins = analog_pins;
  mp_digital_pins = digital_pins;
}

void SmartyInputs::Init()
{
  if (mp_analog_pins != NULL)
    for (int i = 0; i < m_num_analog_inputs; i++)
      pinMode(mp_analog_pins[i], INPUT);
  if (mp_digital_pins != NULL)
    for (int i = 0; i < m_num_digital_inputs; i++)
      pinMode(mp_digital_pins[i], INPUT);
}

void SmartyInputs::SendStatus()
{
  if (mp_analog_pins != NULL) {
    for (int i = 0; i < m_num_analog_inputs; i++) {
      Serial.print('a');
      SerialPrintFormatted(i+1, 2, false);
      Serial.print(':');
      SerialPrintFormatted(analogRead(mp_analog_pins[i]), 4, false);
      Serial.print(',');
    }
  }
  if (mp_digital_pins != NULL) {
    for (int i = 0; i < m_num_digital_inputs; i++) {
      Serial.print('d');
      SerialPrintFormatted(i+1, 2, false);
      Serial.print(':');
      Serial.print(digitalRead(mp_digital_pins[i]));
      Serial.print(',');
    }
  }
}


