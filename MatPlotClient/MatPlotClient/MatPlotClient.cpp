#include <Windows.h>
#include "MatPlot.h"
#include <iostream>


using namespace std;
using namespace MatPlot;

void main()
{
	const double pi = 3.141592653589793;
	int N = 100;
	double *px{ new double [N] {} };
	double *px1{ new double [N] {} };
	double *py1{ new double [N] {} };
	double *py2{ new double [N] {} };
	double *py3{ new double [N] {} };
	double *py4{ new double [N] {} };

	for (int i{}; i < N; i++)
	{
		px[i] = 2*pi*i / (N - 1);
		px1[i] = px[i] + 500;
		py1[i] = sin(px[i]) + 500;
		py2[i] = 2 * py1[i];
		py3[i] = 3 * py1[i];
		py4[i] = 5 * py1[i];
	}
		
	cout << "MatPlotInit()" << endl;
	MatPlotInit();
	Sleep(1000);
	figure();
	figure();
	Sleep(60000);

	cout << "show_control()" << endl;
	show_control();
	Sleep(2000);

	cout << "figure()" << endl;
	figure();
	Sleep(2000);

	cout << "plot(...)" << endl;
	plot(px1, py1, N);
	Sleep(2000000);

	cout << "figure(2)" << endl;
	figure(2);
	Sleep(2000);

	cout << "close(2)" << endl;
	close(2);
	Sleep(2000);

	cout << "figure(2)" << endl;
	figure(2);
	Sleep(2000);

	cout << "close(2)" << endl;
	close(2);
	Sleep(2000);

	cout << "figure(1)" << endl;
	figure(1);
	Sleep(2000);

	cout << "figure(0)" << endl;
	figure(0);
	Sleep(2000);

	cout << "plot(1)" << endl;
	plot(px, py1, N);
	Sleep(2000);

	cout << "scatter(2)" << endl;
	scatter(px, py2, N);
	Sleep(2000);

	cout << "MatPlotClose()" << endl;
	MatPlotClose();
	Sleep(1000);

	delete[] px;
	delete[] py1;
	delete[] py2;
	delete[] py3;
	delete[] py4;
}