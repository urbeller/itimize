#include "itimize.h"
#include <iomanip>
#include <cmath>

using namespace std;
using namespace itimize;

int main(int , char **)
{

	TimeCentral *tc = TimeCentral::getInstance();

	{
		double f = 0;
		TIME_BLOCK();

		for(int i = 0; i < 1E7; ++i)
		{
			f += (drand48());
		}
		cout << f << endl;
	}

	return 0;
}
