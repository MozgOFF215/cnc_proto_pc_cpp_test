#include "header.h"

int timeCounter;

uint32_t micros()
{
  timeCounter++;
  return 0;
}