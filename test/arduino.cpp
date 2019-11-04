#include "header.h"

int currentTime;
bool no_prompt;

uint32_t micros()
{
  return currentTime;
}

void digitalWrite(const char *pin, int value)
{
  if (!no_prompt)
    printf("digitalWrite(%s, %d)\n", pin, value);
}

void analogWrite(const char *pin, int value)
{
  if (!no_prompt)
    printf("analogWrite(%s, %d)\n", pin, value);
}