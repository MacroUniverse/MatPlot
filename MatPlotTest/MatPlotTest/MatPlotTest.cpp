#include <Windows.h>
#include <cmath>
#include "MatPlot.h"

using namespace MatPlot;
int main()
{
	double x[5]{ 1,2,3,4,5 };
	double y[5]{ 1,3,2,4,3 };
	int N{ 5 };
	MatPlotInit();
	figure();
	figure();
	figure();
	plot(x, y, N);
	Sleep(60000);
	MatPlotClose();
	return 0;
}
