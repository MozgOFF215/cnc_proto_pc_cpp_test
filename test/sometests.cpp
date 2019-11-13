#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include "arduino.h"
#include "controller.h"
#include "parser_my.h"
#include "rs232.h"
#include "vector.h"

void printPidState(State *st);
void printGraph(int val);
void printGraph(double val, double min, double max);
void send(const char *str)
{
   RS232_cputs(0, str);
   printf("send:%s", str);
}

struct Result
{
   long iteration;
   long currTime;
   long currentPos;
   long destPos;
   long ts;
   double pMV;
   double iMV;
   double dMV;
   int pwm;
   double kP;
   double kI;
   double kD;
   long position;
   long dt;
   long e;
   bool isFirstCycle;
};

void saveToCsv(std::vector<Result> *results);
void addResult(std::vector<Result> *results, Result *result, State *st, long iteration, long currentPos, long destPos, long currentTime);
int test_parser();

int main()
{
   test_parser();
   return (0);

   currentTime = 0;
   no_prompt = true;
   //initController(&X_pidState);
   X_state.kP = 1;
   X_state.kI = 0.5;
   X_state.kD = 0.5;

   //int *p_darr = new parserCode[1];

   char codes[3][100] = {"N-1 G0 X200.3 Y-2.500 *255", "M150;X50", "G0 X-10.1 ; jkjdkjsdj"};
   for (int i = 0; i < 3; i++)
   {
      parse_my((const char *)codes[i], &send);
      //printf("GO\n");
   }

   getchar();
   return 0;

   X_state.Stop(&X_state, "init PID test");

   std::vector<Result> results;

   long pos[] = {0, 0, 0, 20, 40, 70, 110, 120, 110, 90, 95, 100, 105, 100, 98, 100, 100, 100, 100};
   long dest[] = {0, 0, 100};

   int sizePosArray = (sizeof(pos) / sizeof(*pos));
   int sizeDestArray = (sizeof(dest) / sizeof(*dest));

   long prevPos = pos[0];

   for (int i = 0; i < sizePosArray + 5; i++)
   {
      long currPos = i < sizePosArray ? pos[i] : pos[sizePosArray - 1];
      long delta = currPos - prevPos;
      double gain = delta / 1000.0;

      for (int j = 0; j < 1000; j++)
      {
         Result result;

         long destPos = i < sizeDestArray ? dest[i] : dest[sizeDestArray - 1];
         X_state.goTo_Strokes(destPos);

         long currentPos = prevPos + gain * j;
         X_state.currentPos = currentPos;

         if (!no_prompt)
            printf("### iteration %d, currentPos %ld, destPos %ld, time %d\n", i * j, X_state.currentPos, X_state.getDestination(), currentTime);

         controller(&X_state);

         if (!no_prompt)
            printPidState(&X_state);

         addResult(&results, &result, &X_state, i * 1000 + j, currentPos, destPos, currentTime);

         if (no_prompt && (j % 100 == 0))
            printGraph(X_state.MV.pwm);

         if (!no_prompt)
            printf("---------------------------\n");

         currentTime += 100; //µS
      }

      prevPos = currPos;
   }

   saveToCsv(&results);

   getchar();

   return 0;
}

int test_parser()
{
   //-------------------------------------------------------------------------------------------------------------------------

   int n,             //i,
       cport_nr = 0,  /* /dev/ttyS0 (COM1 on windows) */
       bdrate = 9600; /* 9600 baud */

   unsigned char buf[4096];

   char mode[] = {'8', 'N', '1', 0};

   if (RS232_OpenComport(cport_nr, bdrate, mode, 0))
   {
      printf("Can not open comport\n");

      return (0);
   }

   printf("waiting...\n");

   while (1)
   {
      n = RS232_PollComport(cport_nr, buf, 4095);

      if (n > 0)
      {
         buf[n] = 0; // always put a "null" at the end of a string!

         /* 
         for (i = 0; i < n; i++)
         {
            if (buf[i] < 32) // replace unreadable control-codes by dots 
            {
               buf[i] = '.';
            }
         }
*/
         printf("recieve:%s", (char *)buf);

         parse_my((const char *)buf, &send);
      }

      Sleep(100);
   }

   //-------------------------------------------------------------------------------------------------------------------------
}

void addResult(std::vector<Result> *results, Result *result, State *st, long iteration, long currentPos, long destPos, long currentTime)
{
   result->iteration = iteration;
   result->currTime = currentTime;
   result->currentPos = currentPos;
   result->destPos = destPos;
   result->pwm = st->MV.direction == FORWARD ? st->MV.pwm : -(st->MV.pwm);
   result->pMV = st->pMV;
   result->iMV = st->iMV;
   result->dMV = st->dMV;
   result->isFirstCycle = st->isFirstCycle;
   result->kD = st->kD;
   result->kI = st->kI;
   result->kP = st->kP;
   result->dt = st->dt;
   result->e = st->e;
   result->position = st->position;
   result->ts = st->ts;

   results->push_back(*result);
}

void printPidState(State *st)
{
   // printf("--- PID state %s ----\n", ps->axis_name);
   printf("prevTime - %ld\n", st->ts);
   printf("prevPos  - %ld\n", st->position);
   printf("prevIntg - %f\n", st->iMV);
   printf("prevE    - %ld\n", st->e);
   printf("prevDeltaTime - %ld\n", st->dt);
   printf("kP=%f, kI=%f, kD=%f \n", st->kP, st->kI, st->kD);
   printf("MV.pwm=%ld, MV.direction=%s\n", st->MV.pwm, st->MV.direction == FORWARD ? "FORWARD" : "BACKWARD");
   printf("isFirstCycle - %s\n", st->isFirstCycle ? "true" : "false");
}

void printGraph(int val)
{
   char graph[257];
   for (int i = 0; i < 257; i++)
   {
      graph[i] = (i == val) ? '#' : ' ';

      if (i == 256)
         graph[i] = 0;
   }

   printf("%5d%s\n", val, graph);
}

void printGraphf(double val, double min, double max)
{
   char graph[257];
   for (int i = 0; i < 257; i++)
   {
      graph[i] = (i == val) ? '#' : ' ';

      if (i == 256)
         graph[i] = 0;
   }

   printf("%5.2f%s\n", val, graph);
}

void saveToCsv(std::vector<Result> *results)
{
   // a;b;c semicolum is separator
   // 3,1415 coma is decimal point

   std::ofstream myfile;
   myfile.open("test_result.csv");
   //myfile << "This is the first cell in the first column.\n";

   myfile << "iteration;currTime;currentPos;position;destPos;pMV;iMV;dMV;pwm;kP;kI;kD;e;ts;dt;isFirstCycle\n";

   std::string str;

   for (std::vector<Result>::iterator result = results->begin(); result != results->end(); ++result)
   {
      myfile << result->iteration << ";";
      myfile << result->currTime << ";";
      myfile << result->currentPos << ";";
      myfile << result->position << ";";
      myfile << result->destPos << ";";

      str = std::to_string(result->pMV);
      std::replace(str.begin(), str.end(), '.', ',');
      myfile << str << ";";

      str = std::to_string(result->iMV);
      std::replace(str.begin(), str.end(), '.', ',');
      myfile << str << ";";

      str = std::to_string(result->dMV);
      std::replace(str.begin(), str.end(), '.', ',');
      myfile << str << ";";

      myfile << result->pwm << ";";

      str = std::to_string(result->kP);
      std::replace(str.begin(), str.end(), '.', ',');
      myfile << str << ";";

      str = std::to_string(result->kI);
      std::replace(str.begin(), str.end(), '.', ',');
      myfile << str << ";";

      str = std::to_string(result->kD);
      std::replace(str.begin(), str.end(), '.', ',');
      myfile << str << ";";

      myfile << result->e << ";";
      myfile << result->ts << ";";
      myfile << result->dt << ";";
      myfile << result->isFirstCycle << "\n";
   }

   myfile.close();
}