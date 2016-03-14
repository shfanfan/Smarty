#include <Arduino.h>

void jumpToEnd() {
  while (Serial.available() > 0) {
    if (Serial.read() == 0)  return;
  }
}

void clearBuffer(char* buffer, int len) {
  for (int i = 0; i < len; i++) {
    buffer[i] = 0;
  }
}

int parseInt(char*& buffer, int len) {
  int value = 0, digit;
  while (len-- > 0) {
    digit = constrain(*buffer - '0', 0, 9);
    value = 10*value + digit;
    buffer++;
  }
  //Serial.println(value);
  return value;
}

/*int strlen(const char* s) {
  int len = 0;
  while (s[len++] != 0) {}
  return len;
}*/

int strncmp(char* str1, char* str2, int n) {
  while (n--) {
    /*Serial.print("comparing: ");
    Serial.print(*str1);
    Serial.print(" and ");
    Serial.println(*str2);*/
    if (*str1 > *str2)  return 1;
    if (*str1 < *str2)  return -1;
    str1++;
    str2++;
  }
  return 0;
}

void SerialPrintFormatted(int value, int digits, int sign) {
  if (sign) {
    if (value < 0) {
      Serial.print('-');
      value = -value;
    }
    else Serial.print('+');
  }
  int tens = 1, d = digits-1;
  while (d--)  tens*=10;
  for (d = 0; d < digits; d++) {
    int digit = value / tens;
    Serial.print(digit);
    if (digit > 0) {
      value -= digit*tens;
    }
    tens /= 10;
  }
}


