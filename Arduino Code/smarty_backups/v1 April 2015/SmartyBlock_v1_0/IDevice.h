#ifndef IDEVICE_H
//#define IDEVICE_H

typedef unsigned char byte;

class IDevice
{
protected:
  byte m_deviceID;
  unsigned long m_prev_time;
  
public:
  virtual void Init() = 0;
  virtual bool CommandForMe(char cmd, byte id) = 0;
  virtual bool ParseCommand(char cmd, char* buffer) = 0;
  virtual void Update(unsigned long time) = 0;
  virtual void SendStatus() = 0;
};

#endif

