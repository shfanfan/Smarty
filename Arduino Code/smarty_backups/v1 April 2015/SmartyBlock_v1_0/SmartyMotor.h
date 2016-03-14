//#include "IDevice.h"

class SmartyMotor : public IDevice
{
  AF_DCMotor m_motor;
  char m_dir;
  byte m_speed;
  int m_current;
  unsigned long m_time;

public:
  SmartyMotor(byte id, byte shieldPlug);
  virtual void Init();
  virtual bool CommandForMe(char cmd, byte id);
  virtual bool ParseCommand(char cmd, char* buffer);
  virtual void SendStatus();
  virtual void Update(unsigned long time);
};


