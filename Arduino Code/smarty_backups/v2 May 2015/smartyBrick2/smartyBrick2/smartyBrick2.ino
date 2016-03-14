#include <ComBrick.h>

ComBrick brick;

void setup() {
  brick.setupComBrick();
}

void loop() {
  brick.checkPcMessage();  

  brick.checkSmartyMessage();
  
  delay(10);
} // Loop

