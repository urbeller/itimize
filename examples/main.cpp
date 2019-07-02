#include "itimize.h"
#include <iomanip>
#include <cmath>

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

int main(int , char **)
{

  TIME_NAMED_BLOCK("Main function: ");

  // Each of these functions call TIME_BLOCK().
  double res1 = func1();
  double res2 = func2();

  // For recursive functions, it is better to create a block 
  // and time outside of the function body. Otherwise, each 
  // recusrive call will create a Ticker object!!
  {
    TIME_NAMED_BLOCK("line " STR1(__LINE__));
    int res3 = fibonacci(45);
  }

	return 0;
}
