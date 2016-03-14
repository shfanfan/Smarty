//#include "IDevice.h"

class SmartyPWM : public IDevice
{
  byte m_pin;
  byte m_pwm;

public:
  SmartyPWM(byte id, byte pin);
  virtual void Init();
  virtual bool CommandForMe(char cmd, byte id);
  virtual bool ParseCommand(char cmd, char* buffer);
  virtual void SendStatus();
  virtual void Update(unsigned long time);
};


