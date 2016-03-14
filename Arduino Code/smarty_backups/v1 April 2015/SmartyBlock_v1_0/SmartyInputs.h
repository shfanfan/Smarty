class SmartyInputs : public IDevice
{
  byte m_num_analog_inputs;
  byte m_num_digital_inputs;
  byte* mp_analog_pins;
  byte* mp_digital_pins;

public:
  SmartyInputs(byte id, byte num_analog_inputs, byte num_digital_inputs, byte* analog_pins, byte* digital_pins);
  virtual void Init();
  virtual bool CommandForMe(char cmd, byte id)  {  return false;  }
  virtual bool ParseCommand(char cmd, char* buffer)  {  return true;  }
  virtual void SendStatus();
  virtual void Update(unsigned long time)  {}
};


