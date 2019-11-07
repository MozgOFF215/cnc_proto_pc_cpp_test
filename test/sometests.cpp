#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include "arduino.h"
#include "controller.h"
#include "parser_my.h"

void printPidState(pidState *ps);
void printGraph(int val);
void printGraph(double val, double min, double max);

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
void addResult(std::vector<Result> *results, Result *result, pidState *ps, long iteration, long currentPos, long destPos, long currentTime);

int main()
{
   currentTime = 0;
   no_prompt = true;
   //initController(&X_pidState);
   X_pidState.kP = 1;
   X_pidState.kI = 0.5;
   X_pidState.kD = 0.5;

   char codes[3][20] = {"G g -l6", "G0034 M86 X34 ", "d8782 x0 l"};
   for (int i = 0; i < 3; i++)
   {
      const char *p = codes[i];
      parserCode c;
      do
      {
         c = getCode(p);
         printf("command [%s] letter [%c] code [%d]\n", p, c.letter, c.codenum);
         p = c.pNextSymbol;
      } while (c.pNextSymbol != nullptr);
   }

   X_config.Stop(&X_state, "init PID test");

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
         X_state.destinationPos = destPos;

         long currentPos = prevPos + gain * j;
         X_state.currentPos = currentPos;

         if (!no_prompt)
            printf("### iteration %d, currentPos %ld, destPos %ld, time %d\n", i * j, X_state.currentPos, X_state.destinationPos, currentTime);

         controller(&X_config, &X_state, &X_pidState);

         if (!no_prompt)
            printPidState(&X_pidState);

         addResult(&results, &result, &X_pidState, i * 1000 + j, currentPos, destPos, currentTime);

         if (no_prompt && (j % 100 == 0))
            printGraph(X_pidState.MV.pwm);

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

void addResult(std::vector<Result> *results, Result *result, pidState *ps, long iteration, long currentPos, long destPos, long currentTime)
{
   result->iteration = iteration;
   result->currTime = currentTime;
   result->currentPos = currentPos;
   result->destPos = destPos;
   result->pwm = ps->MV.direction == FORWARD ? ps->MV.pwm : -(ps->MV.pwm);
   result->pMV = ps->pMV;
   result->iMV = ps->iMV;
   result->dMV = ps->dMV;
   result->isFirstCycle = ps->isFirstCycle;
   result->kD = ps->kD;
   result->kI = ps->kI;
   result->kP = ps->kP;
   result->dt = ps->dt;
   result->e = ps->e;
   result->position = ps->position;
   result->ts = ps->ts;

   results->push_back(*result);
}

void printPidState(pidState *ps)
{
   // printf("--- PID state %s ----\n", ps->axis_name);
   printf("prevTime - %ld\n", ps->ts);
   printf("prevPos  - %ld\n", ps->position);
   printf("prevIntg - %f\n", ps->iMV);
   printf("prevE    - %ld\n", ps->e);
   printf("prevDeltaTime - %ld\n", ps->dt);
   printf("kP=%f, kI=%f, kD=%f \n", ps->kP, ps->kI, ps->kD);
   printf("MV.pwm=%ld, MV.direction=%s\n", ps->MV.pwm, ps->MV.direction == FORWARD ? "FORWARD" : "BACKWARD");
   printf("isFirstCycle - %s\n", ps->isFirstCycle ? "true" : "false");
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