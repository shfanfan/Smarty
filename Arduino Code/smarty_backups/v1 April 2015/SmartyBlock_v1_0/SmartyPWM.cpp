#include "IDevice.h"
#include "SmartyPWM.h"
#include "HelperFunctions.h"
#include <Arduino.h>

SmartyPWM::SmartyPWM(byte id, byte pin): m_pin(pin)
{
  m_deviceID = id;
  m_pwm = 0;
}

void SmartyPWM::Init()
{
  pinMode(m_pin, OUTPUT);
}

bool SmartyPWM::CommandForMe(char cmd, byte id)
{
  return (cmd == 'p' && id == m_deviceID);
}

bool SmartyPWM::ParseCommand(char cmd, char* buffer)
{
  bool error = false;
  
  int ipwm = parseInt(buffer, 3);
  m_pwm = constrain(ipwm, 0, 255);

  Serial.print("#debug#Command: pwm ");
  Serial.print(m_deviceID);
  Serial.print(" value = ");
  Serial.println(m_pwm);
  
  // execute command
  analogWrite(m_pin, m_pwm);
  
  return error;
}

void SmartyPWM::SendStatus()
{
}

void SmartyPWM::Update(unsigned long time)
{
}



