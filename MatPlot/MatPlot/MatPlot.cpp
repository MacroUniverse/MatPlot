#include "stdafx.h"
#include "MatPlot.h"
#include "MatPlotApp.h"
#include "MatPlotDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

BEGIN_MESSAGE_MAP(CMatPlotApp, CWinApp)
END_MESSAGE_MAP()

CMatPlotApp::CMatPlotApp() {}

CMatPlotApp *g_app = nullptr;
HANDLE g_handle;
//CMatPlotDlg main_dlg;
CMatPlotDlg *pmain_dlg;

BOOL CMatPlotApp::InitInstance()
{
	CWinApp::InitInstance();
	AfxGetModuleState()->m_pCurrentWinApp = this;
	m_hInstance = AfxGetInstanceHandle();

	//In order to hide the dialog, must use a modeless dialog
	//m_pMainWnd = &main_dlg;
	//main_dlg.DoModal();
	
	pmain_dlg = new CMatPlotDlg;
	pmain_dlg->Create(IDD_MAIN);
	pmain_dlg->RunModalLoop(); // message loop
	delete pmain_dlg;
	return FALSE;
}

DWORD WINAPI ThreadFun(LPVOID pM)
{
	g_app = new CMatPlotApp();
	g_app->InitApplication();
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	g_app->InitInstance();
	return 0;
}

// MatPlot.h implementation =====================================

namespace MatPlot
{
	// must call before use
	void MatPlotInit() {
		static bool first{ true };
		if (first) {
			CreateThread(NULL, 0, ThreadFun, 0, 0, NULL);
			first = false;
		}
		// wait for thread to create, increase the time if necessary
		Sleep(30);
	}

	// must call after use
	void MatPlotClose() {
		if (g_app) {
			SendMessage(pmain_dlg->m_hWnd, WM_CLOSE, 0, 0);
			WaitForSingleObject(g_handle, INFINITE);
		}
	}

	// show control panel (main dialog)
	void show_control() {
		SendMessage(pmain_dlg->m_hWnd, MATPLOT_SHOW_CONTROL, 0, 0);
	}

	// hide control panel
	void hide_control() {
		SendMessage(pmain_dlg->m_hWnd, MATPLOT_HIDE_CONTROL, 0, 0);
	}

	// if no argument, generate a new figure, if input CurrentFig, 
	// set as current figure. Note CurrentFig starts from 0.
	// if CurrentFig is closed, generate a new one.
	void figure(int FigNo) {
		SendMessage(pmain_dlg->m_hWnd, MATPLOT_FIGURE, 0, FigNo);
	}

	void close(int FigNo) {
		SendMessage(pmain_dlg->m_hWnd, MATPLOT_CLOSE, 0, FigNo);
	}

	void close_all() {
		SendMessage(pmain_dlg->m_hWnd, MATPLOT_CLOSE_ALL, 0, 0);
	}

	void clf() {
		SendMessage(pmain_dlg->m_hWnd, MATPLOT_CLF, 0, 0);
	}

	void axis(double xmin, double xmax, double ymin, double ymax) {
		Axis_Param param;
		param.xmin = xmin; param.xmax = xmax;
		param.ymin = ymin; param.ymax = ymax;
		SendMessage(pmain_dlg->m_hWnd, MATPLOT_AXIS, 0, (LPARAM)&param);
	}

	void axis_auto() {
		SendMessage(pmain_dlg->m_hWnd, MATPLOT_AXIS_AUTO, 0, 0);
	}

	void axis_manual() {
		SendMessage(pmain_dlg->m_hWnd, MATPLOT_AXIS_MANUAL, 0, 0);
	}

	// set the minimum and maximum tick coordinates, number of ticks
	// and txt intervals in unit of tick spacing
	void xticks(double tickmin, double tickmax, int Nticks,
				int txtstart, int txtspace, int Ndigits) {
		Tick_Param param;
		param.tickmin = tickmin; param.tickmax = tickmax;
		param.Nticks = Nticks; param.txtstart = txtstart;
		param.txtspace = txtspace; param.Ndigits = Ndigits;
		SendMessage(pmain_dlg->m_hWnd, MATPLOT_XTICKS, 0, (LPARAM)&param);
	}

	void yticks(double tickmin, double tickmax, int Nticks,
		int txtstart, int txtspace, int Ndigits) {
		Tick_Param param;
		param.tickmin = tickmin; param.tickmax = tickmax;
		param.Nticks = Nticks; param.txtstart = txtstart;
		param.txtspace = txtspace; param.Ndigits = Ndigits;
		SendMessage(pmain_dlg->m_hWnd, MATPLOT_YTICKS, 0, (LPARAM)&param);
	}

	void plot(double *px, double *py, int N, char color) {
		Plot_Param param;
		param.px = px; param.py = py;
		param.N = N; param.color = color;
		SendMessage(pmain_dlg->m_hWnd, MATPLOT_PLOT, 0, (LPARAM)&param);
	}

	void scatter(double *px, double *py, int N, char color) {
		Plot_Param param;
		param.px = px; param.py = py;
		param.N = N; param.color = color;
		SendMessage(pmain_dlg->m_hWnd, MATPLOT_SCATTER, 0, (LPARAM)&param);
	}
}

