#include "header.h"

int currentTime;

uint32_t micros()
{
  return currentTime;
}

void digitalWrite(const char *pin, int value)
{
  printf("digitalWrite(%s, %d)\n", pin, value);
}

void analogWrite(const char *pin, int value)
{
  printf("analogWrite(%s, %d)\n", pin, value);
}