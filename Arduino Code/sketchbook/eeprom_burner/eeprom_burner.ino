#include <SPI.h>
#include "printf.h"
#include <avr/eeprom.h>

#define SMARTY      1
#define COM_BRICK   2

//chanege this according to need!!!
#define NODE_TYPE SMARTY

typedef struct RFparams
{
  uint8_t  m_zeroFive_leadingUntiGarbageVarifier;
  uint8_t  m_networkAddress[4];
  uint8_t  m_smartyID;
  uint8_t  m_gatewayID;
  uint8_t  m_channel;
  uint8_t  m_zeroSeven_trailingUntiGarbageVarifier;
};

RFparams  m_config;

void writeEeprom()
{
  m_config.m_zeroFive_leadingUntiGarbageVarifier = 0x05;
  m_config.m_networkAddress[0] = 0xE8;
  m_config.m_networkAddress[1] = 0xE8;
  m_config.m_networkAddress[2] = 0xF0;
  m_config.m_networkAddress[3] = 0xF0;
  
  #if NODE_TYPE==SMARTY
    m_config.m_smartyID = 0xA5;
  #elif 
    m_config.m_smartyID = 0x00;
  #endif
  
  m_config.m_gatewayID = 0x00;
  m_config.m_channel = 90;
  m_config.m_zeroSeven_trailingUntiGarbageVarifier = 0x07;
  
  eeprom_write_block((const void*)&m_config, (void *)0, sizeof(m_config));
}


void setup()
{
  eeprom_read_block((void*)&m_config, (void*)0, sizeof(m_config));
  Serial.begin(57600);
  printf_begin();
  
  printf("current data in eeprom:");
  printf("zeroFive_leadingUntiGarbageVarifier = %#02x\r\n",m_config.m_zeroFive_leadingUntiGarbageVarifier); 
  printf("networkAddress = %#02x-%#02x-%#02x-%#02x \r\n",m_config.m_networkAddress[0],m_config.m_networkAddress[1],m_config.m_networkAddress[2],m_config.m_networkAddress[3]); 
  printf("smartyID = %#02x\r\n",m_config.m_smartyID); 
  printf("gatewayID = %#02x\r\n",m_config.m_gatewayID); 
  printf("zeroSeven_trailingUntiGarbageVarifier = %#02x\r\n",m_config.m_zeroSeven_trailingUntiGarbageVarifier); 
 
  #if NODE_TYPE==SMARTY
    Serial.write("burn SMARTY eeprom?\n");
  #elif 
    Serial.write("burn COM BRICK eeprom?\n");
  #endif

}

void loop()
{
  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    if ( c == 'Y' )
    {
      writeEeprom();
    }
  }
  delay(5000);
  
}
