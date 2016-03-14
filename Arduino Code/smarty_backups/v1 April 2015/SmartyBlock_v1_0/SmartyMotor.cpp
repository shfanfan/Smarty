#include <AFMotor.h>
#include "IDevice.h"
#include "SmartyMotor.h"
#include "HelperFunctions.h"
#include <Arduino.h>

SmartyMotor::SmartyMotor(byte id, byte shieldPlug): m_motor(shieldPlug)
{
  m_deviceID = id;
  m_dir = '0';
  m_speed = 0;
  m_current = -1;
}

void SmartyMotor::Init()
{
}

bool SmartyMotor::CommandForMe(char cmd, byte id)
{
  return (cmd == 'm' && id == m_deviceID);
}

bool SmartyMotor::ParseCommand(char cmd, char* buffer)
{
  bool error = false;
  char c_dir, tmp;
  byte c_speed, c_time;
  
  c_dir = *buffer++;
  if (c_dir == '+'|| c_dir == '-') {
    byte ic_speed = parseInt(buffer, 3);
    c_speed = constrain(ic_speed, 0, 255);
    //Serial.println(c_speed);
    tmp = *buffer++;
    if (tmp == ':') {
      c_time = parseInt(buffer, 4);
    }
    else {
      c_time = 0;
    }
  }
  else if (c_dir == '0') {
    c_speed = 0;
    c_time = 0;
  }
  else {
    error = true;
    Serial.print("error - char was: ");
    Serial.println(c_dir);
  }
  if (!error) {
    Serial.print("#debug#Command: motor ");
    Serial.print(m_deviceID);
    Serial.print(" dir = ");
    Serial.print(c_dir);
    Serial.print(" speed = ");
    Serial.print(c_speed);
    Serial.print(" time = ");
    Serial.println(c_time);
    
    // execute command:
    m_speed = c_speed;
    m_dir = c_dir;
    m_time = c_time;
    
    if (m_speed == 0 || m_dir == '0') {
      m_motor.run(RELEASE);
    } else if (m_dir == '+') {
      m_motor.run(BACKWARD);
      m_motor.setSpeed(m_speed);
    } else if (m_dir == '-') {
      m_motor.run(FORWARD);
      m_motor.setSpeed(m_speed);
    }
  }
  return error;
}

void SmartyMotor::SendStatus()
{
  Serial.print("m");
  SerialPrintFormatted(m_deviceID, 2, false);
  Serial.print(':');
  Serial.print(m_dir);
  SerialPrintFormatted(m_speed, 3, false);
  if (m_current >= 0)    Serial.print('5');
  else Serial.print('x');
  Serial.print(',');
}

void SmartyMotor::Update(unsigned long time)
{
}


