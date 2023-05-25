#include "itimize.h"
#include <iomanip>
#include <cmath>
#include <thread>

using namespace std;
using namespace itimize;

int fibonacci(int n)
{
  if( n <= 0 )
    return 0;

  if( n == 1 )
    return 1;

  return fibonacci(n - 1)  + fibonacci(n - 2);
}

double func1()
{
  TIME_BLOCK();

  double r = 0;

  for(int i = 0; i < 1E7; ++i)
    r += cos(drand48()) * sin(drand48());

  return r;
}

double func2()
{
  TIME_BLOCK();

  double r = 0;

  for(int i = 0; i < 1E6; ++i)
    r += atan(i) * atan(i);

  return r;
}

void func3(int sec)
{

  // This will add an entry for "func3".
  // NOTE: START_TIMER must be matched with an
  // END_TIMER.
  START_TIMER("func3");
    std::this_thread::sleep_for(std::chrono::seconds(sec));
  END_TIMER("func3");
}

int main(int , char **)
{

  TIME_NAMED_BLOCK("Main function");

  // Each of these functions call TIME_BLOCK().
  double res1 = func1();
  double res2 = func2();

  // For recursive functions, it is better to create a block 
  // and time outside of the function body. Otherwise, each 
  // recusrive call will create a Ticker object!!
  {
    TIME_NAMED_BLOCK("line " STR1(__LINE__));
    int res3 = fibonacci(15);
  }


  // Show how to use the TimeCenter to log
  // multi-point timers.
  for(int i = 0; i < 3; ++i)
  {
    func3(i);
  }

  // Print timer stats (default unit is milli-secs). The format is:
  // [label_of_timer(# of instances)]: total_time avg-min-max=average/min/max times.
  PRINT_TIMER("func3");
  
  // Remove entries of labeled timer from TimeCenter.
  RESET_TIMER("func3");

  // Timer printing support th following units:
  // "s" for seconds, "ms" for millisecs (default), "us" for micro-secs.
  func3(1);
  func3(2);
  PRINT_TIMER("func3","s");
  PRINT_TIMER("func3","ms");
  PRINT_TIMER("func3","us");

	return 0;
}
