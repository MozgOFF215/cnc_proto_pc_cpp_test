#include "header.h"

int currentTime;

uint32_t micros()
{
  return currentTime;
}

void digitalWrite(int pin, int value)
{
  printf("digitalWrite(pin: %d, val: %d)");
}

void analogWrite(int pin, int value)
{
  printf("analogWrite(pin: %d, val: %d)");
}