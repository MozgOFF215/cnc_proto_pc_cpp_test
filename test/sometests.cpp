#include "header.h"

void printPidState(pidState *ps);

int main()
{
   currentTime = 0;
   initController(&X_pidState);

   //X_state.isStoped = false;

   X_state.destinationPos = 100;
   long pos[] = {0, 20, 40, 70, 110, 120, 110, 90, 95, 100, 105, 100, 98, 100, 100, 120, 100};
   int size = (sizeof(pos) / sizeof(*pos));

   for (int i = 0; i < size + 5; i++)
   {
      long currentPos = i < size ? pos[i] : pos[size - 1];
      X_state.currentPos = currentPos;

      printf("### iteration %d, currentPos %ld, destPos %ld, time %d\n", i, X_state.currentPos, X_state.destinationPos, currentTime);

      controller(&X_config, &X_state, &X_pidState);

      printPidState(&X_pidState);

      printf("---------------------------\n");

      currentTime++;
   }

   getchar();

   return 0;
}

void printPidState(pidState *ps)
{
   // printf("--- PID state %s ----\n", ps->axis_name);
   printf("prevTime - %ld\n", ps->prevTime);
   printf("prevPos  - %ld\n", ps->prevPos);
   printf("prevIntg - %ld\n", ps->prevIntg);
   printf("prevE    - %ld\n", ps->prevE);
   printf("prevDeltaTime - %ld\n", ps->prevDeltaTime);
   printf("kP=%f, kI=%f, kD=%f \n", ps->kP, ps->kI, ps->kD);
   printf("MV.pwm=%d, MV.direction=%s\n", ps->MV.pwm, ps->MV.direction == FORWARD ? "FORWARD" : "BACKWARD");
   printf("isFirstCycle - %s\n", ps->isFirstCycle ? "true" : "false");
}