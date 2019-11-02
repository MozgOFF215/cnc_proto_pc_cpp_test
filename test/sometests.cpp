
#define TEST_PC_CPP

#include <stdio.h>
#include "header.h"

void printPidState(pidState *ps);

int main()
{
   timeCounter = 0;
   initController(&X_pidState);

   X_state.isStoped = false;
   
   X_state.destinationPos = 100;
   controller(&X_config, &X_state, &X_pidState);
   printPidState(&X_pidState);

   X_state.currentPos = 20;
   controller(&X_config, &X_state, &X_pidState);
   printPidState(&X_pidState);

   X_state.currentPos = 40;
   controller(&X_config, &X_state, &X_pidState);
   printPidState(&X_pidState);

   return 0;
}

void printPidState(pidState *ps)
{
   printf("--- PID state ----\n");
   printf("prevTime - %d\n", ps->prevTime);
   printf("prevPos  - %d\n", ps->prevPos);
   printf("prevIntg - %d\n", ps->prevIntg);
   printf("prevE    - %d\n", ps->prevE);
   printf("prevDeltaTime - %d\n", ps->prevDeltaTime);
   printf("kP - %f\n", ps->kP);
   printf("kI - %f\n", ps->kI);
   printf("kD - %f\n", ps->kD);
   printf("isFirstCycle - %s\n", ps->isFirstCycle ? "true" : "false");
}