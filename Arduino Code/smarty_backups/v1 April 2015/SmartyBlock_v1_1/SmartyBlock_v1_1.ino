#include <AFMotor.h>
#include "ISmarty.h"
#include "SmartyMotor.h"
#include "SmartyPWM.h"
#include "SmartyInputs.h"
#include "HelperFunctions.h"
#include <MemoryFree.h>

#define BUFFER_LENGTH 50
char buffer[BUFFER_LENGTH];
const char sync[] = "#smarty#";
int len = 0;
char cmd = 0;
byte device_id = 0;

unsigned long prevTime, time;

#define MAX_NUM_DEVICES 100
int num_devices = 5;
IDevice* pDevices[MAX_NUM_DEVICES];

byte analog_pins[] = { A0, A1, A2 };
byte digital_pins[] = { A3, A4, A5 };
SmartyMotor motor1(1, 3);
SmartyMotor motor2(2, 4);
SmartyPWM pwm1(1, 9);
SmartyPWM pwm2(2, 10);
SmartyInputs inputs(100, sizeof(analog_pins), sizeof(digital_pins), analog_pins, digital_pins);

void setup() {
  Serial.begin(115200);
  prevTime = millis();
  
  for (int i = 0;i < MAX_NUM_DEVICES; i++)
    pDevices[i] = NULL;
  pDevices[0] = &motor1;
  pDevices[1] = &motor2;
  pDevices[2] = &pwm1;
  pDevices[3] = &pwm2;
  pDevices[4] = &inputs;

  for (int i = 0; i < num_devices; i++)
    if (pDevices[i] != NULL)
      pDevices[i]->Init();
      
  //Serial.print("FreeMemory (out of 2k available) : ");
  //Serial.println(freeMemory());
}

void loop() {
  if (Serial.available() > 0) {
    clearBuffer(buffer, BUFFER_LENGTH);
    len = Serial.readBytesUntil(10, buffer, BUFFER_LENGTH-1);
    byte index = strlen(sync);
    char *tmp = buffer + index;
    if (strncmp(buffer, sync, index) == 0) {  // command starts with sync
      cmd = *tmp++;
      device_id = parseInt(tmp, 2);
      tmp++;  // increment for colon
      //Serial.print("command: ");
      //Serial.println(cmd);
      for (int i = 0; i < num_devices; i++)
        if (pDevices[i] != NULL && pDevices[i]->CommandForMe(cmd, device_id))
          pDevices[i]->ParseCommand(cmd, tmp);
    }
  }
  time = millis();
  if (time - prevTime > 10) {  // each 100 msec update status and send
    Serial.print(sync);
    for (int i = 0; i < num_devices; i++)
      if (pDevices[i] != NULL) {
        pDevices[i]->Update(time);
        pDevices[i]->SendStatus();
      }
    Serial.println();
    prevTime = time;
  }
}



