#pragma once

#if defined(MFCDLL_LIBRARY)
#  define MFCDLL_EXPORT __declspec(dllexport)
#else
#  define MFCDLL_EXPORT __declspec(dllimport)
#endif

namespace MatPlot
{
	MFCDLL_EXPORT void MatPlotInit();
	MFCDLL_EXPORT void MatPlotClose();
	MFCDLL_EXPORT void show_control();
	MFCDLL_EXPORT void hide_control();
	MFCDLL_EXPORT void figure(int FigNo = -1);
	MFCDLL_EXPORT void close(int FigNo = -1);
	MFCDLL_EXPORT void close_all();
	MFCDLL_EXPORT void plot(double *px, double *py, int N, char color = 'a');
	MFCDLL_EXPORT void scatter(double *px, double *py, int N, char color = 'a');
	MFCDLL_EXPORT void clf();
	MFCDLL_EXPORT void axis(double xmin, double xmax, double ymin, double ymax);
	MFCDLL_EXPORT void axis_auto();
	MFCDLL_EXPORT void axis_manual();
	MFCDLL_EXPORT void xticks(double tickmin, double tickmax, int Nticks = 26, 
								int txtstart = 0, int txtspace = 5, int Ndigits = 2);
	MFCDLL_EXPORT void yticks(double tickmin, double tickmax, int Nticks = 26, 
								int txtstart = 0, int txtspace = 5, int Ndigits = 2);
}
