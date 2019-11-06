#include "arduino.h"

int currentTime;
bool no_prompt;

uint32_t micros()
{
  return currentTime;
}

void digitalWrite(char *pinName, int value)
{
  if (!no_prompt)
    printf("digitalWrite(%s, %d)\n", pinName, value);
}

void analogWrite(char *pinName, int value)
{
  if (!no_prompt)
    printf("analogWrite(%s, %d)\n", pinName, value);
}

bool digitalRead(char * pinName)
{
  if (!no_prompt)
    printf("digitalRead(%s); result:%d\n", pinName, 0);
  return 0; // warning!!! todo conditions or logic
}