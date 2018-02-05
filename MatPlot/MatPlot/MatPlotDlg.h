#pragma once

#include "Plot.h"

// CMatPlotDlg dialog

#define MATPLOT_ADDONE (WM_USER + 100) // debug

#define MATPLOT_FIGURE (WM_USER + 101)
#define MATPLOT_CLOSE (WM_USER + 102)
#define MATPLOT_CLF (WM_USER + 103)
#define MATPLOT_PLOT (WM_USER + 104)
#define MATPLOT_SCATTER (WM_USER + 105)
#define MATPLOT_CLOSE_ALL (WM_USER + 106)
#define MATPLOT_MATPLOTINIT (WM_USER + 107)
#define MATPLOT_AXIS (WM_USER + 108)
#define MATPLOT_AXIS_AUTO (WM_USER + 109)
#define MATPLOT_AXIS_MANUAL (WM_USER + 110)
#define MATPLOT_HIDE_CONTROL (WM_USER + 111)
#define MATPLOT_SHOW_CONTROL (WM_USER + 112)
#define MATPLOT_FIGURE_CLOSE (WM_USER + 113)
#define MATPLOT_XTICKS (WM_USER + 114)
#define MATPLOT_YTICKS (WM_USER + 115)

struct Plot_Param {
	double *px; double *py; int N; char color;
};

struct Axis_Param {
	double xmin; double xmax; double ymin; double ymax;
};

struct Tick_Param {
	double tickmin; double tickmax; int Nticks; 
	int txtstart; int txtspace; int Ndigits;
};

class CMatPlotDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CMatPlotDlg)

public:
	CMatPlotDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMatPlotDlg();
	virtual BOOL OnInitDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	HICON m_hIcon;

protected:
	afx_msg LRESULT figure(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT close(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT close_all(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT plot(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT scatter(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT clf(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT axis(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT axis_auto(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT axis_manual(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT hide_control(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT show_control(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT figure_close(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT xticks(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT yticks(WPARAM wparam, LPARAM lparam);

	void SetDlgAxis(const double &xmin, const double &xmax,
		const double &ymin, const double &ymax);
	void SetDlgTicksX(const double &tickmin, const double &tickmax,
		const int &Nticks, const int &txtstart,
		const int &txtspace, const int &Ndigits);
	void SetDlgTicksY(const double &tickmin, const double &tickmax,
		const int &Nticks, const int &txtstart,
		const int &txtspace, const int &Ndigits);
	int SearchOpenFig(int FigNo);

	// main_dlg debug buttons callback
	afx_msg void OnPlot();
	afx_msg void OnScatter();
	afx_msg void OnFigure();
	afx_msg void OnClose();
	afx_msg void OnCloseAll();
	afx_msg void OnFigureI();
	afx_msg void OnCloseI();
	afx_msg void OnClf();
	afx_msg void OnAxis();
	afx_msg void OnAutoaxis();
	afx_msg void OnXticks();
	afx_msg void OnYticks();

	int NFig;
	int CurrentFig; // current figure, -1 means there is no figure;
	vector <CPlot *> FigList;

	// control dialog data
	int FigNo; // input argument for figure() and close()
	int DataNo; // choose which set of data to plot
	double axis_xmin, axis_xmax, axis_ymin, axis_ymax;
	CButton *axis_auto_check;
	double xtickmin, xtickmax, ytickmin, ytickmax;
	int Nxticks, Nyticks;
	int xtxtstart, ytxtstart;
	int xtxtspace, ytxtspace;
	int Nxdigits, Nydigits;
};
