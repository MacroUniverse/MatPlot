#pragma once

#if defined(MATPLOT_EXPORTS)
#  define MATPLOT_EXPORT __declspec(dllexport)
#else
#  define MATPLOT_EXPORT __declspec(dllimport)
#endif

namespace MatPlot
{
	MATPLOT_EXPORT void MatPlotInit();
	MATPLOT_EXPORT void MatPlotClose();
	MATPLOT_EXPORT void show_control();
	MATPLOT_EXPORT void hide_control();
	MATPLOT_EXPORT void figure(int FigNo = -1);
	MATPLOT_EXPORT void close(int FigNo = -1);
	MATPLOT_EXPORT void close_all();
	MATPLOT_EXPORT void plot(double *px, double *py, int N, char color = 'a');
	MATPLOT_EXPORT void scatter(double *px, double *py, int N, char color = 'a');
	MATPLOT_EXPORT void clf();
	MATPLOT_EXPORT void axis(double xmin, double xmax, double ymin, double ymax);
	MATPLOT_EXPORT void axis_auto();
	MATPLOT_EXPORT void axis_manual();
	MATPLOT_EXPORT void xticks(double tickmin, double tickmax, int Nticks = 26, 
								int txtstart = 0, int txtspace = 5, int Ndigits = 2);
	MATPLOT_EXPORT void yticks(double tickmin, double tickmax, int Nticks = 26, 
								int txtstart = 0, int txtspace = 5, int Ndigits = 2);
}
