#include "stdafx.h"
#include "Plot.h"
#include "MatPlotDlg.h"
#include "afxdialogex.h"
#include "Resource.h"

IMPLEMENT_DYNAMIC(CPlot, CDialogEx)

CPlot::CPlot(CWnd* pParent)
	: CDialogEx(IDD_PLOT, pParent)
	, Nxticks{ 26 }, Nyticks{ 26 }
	, xticks{}, yticks{}
	, xtickmin{ 0. }, xtickmax{ 1. }
	, ytickmin{ 0. }, ytickmax{ 1. }
	, Nxdigits{ 2 }, Nydigits{ 2 }
	, xtxtspace{ 5 }, ytxtspace{ 5 }
	, axis_xmin{ 0 }, axis_ymin{ 0 }
	, axis_xmax{ 1 }, axis_ymax{ 1 }
	, isempty{ true }, autoaxis{ true }
	, ColorNo{ -1 }, OnSizeNo{}
{
	default_colors[0] = RGB(0, 113, 189);
	default_colors[1] = RGB(222, 81, 24);
	default_colors[2] = RGB(239, 178, 33);
	default_colors[3] = RGB(123, 44, 140);
	default_colors[4] = RGB(115, 174, 49);
	default_colors[5] = RGB(74, 190, 239);
	default_colors[6] = RGB(165, 20, 41);
}

CPlot::~CPlot()
{
	int i;
	for (i = 0; i < PlotXList.size(); i++)
		delete[] PlotXList[i];
	for (i = 0; i < PlotYList.size(); i++)
		delete[] PlotYList[i];
	for (i = 0; i < ScatterXList.size(); i++)
		delete[] ScatterXList[i];
	for (i = 0; i < ScatterYList.size(); i++)
		delete[] ScatterYList[i];
	if (xticks)
	{
		delete[] xticks; delete[] yticks;
	}
}

void CPlot::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPlot, CDialogEx)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_SIZING()
	ON_WM_CLOSE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

BOOL CPlot::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_hIcon = AfxGetApp()->LoadIcon(IDI_MATPLOT);
	SetIcon(m_hIcon, TRUE);

	pDC = GetDC();
	GetClientRect(&rc);
	m_brush.CreateSolidBrush(RGB(255, 255, 255));
	axis_x0 = rc.left + 55;
	axis_y0 = rc.bottom - 30;
	axis_x1 = rc.right - 30;
	axis_y1 = rc.top + 15;
	return TRUE;
}

// plot a curve
void CPlot::plot(double *px, double *py, int N, char color)
{
	COLORREF color0;
	if (autoaxis) {
		if (isempty) {
			AutoResizeAxis(px, py, N);
			update();
			isempty = false;
		}
		else
			if (AutoInflateAxis(px, py, N))
				update();
	}

	color0 = SwitchColor(color);
	plot0(px, py, N, color0);
	PlotAxesText();

	// store data
	double *pxdata{ new double[N] {} };
	double *pydata{ new double[N] {} };
	memcpy(pxdata, px, N * 8);
	memcpy(pydata, py, N * 8);
	PlotXList.push_back(pxdata);
	PlotYList.push_back(pydata);
	PlotNList.push_back(N);
	PlotColorList.push_back(color0);
}

// plot a curve without storing data or auto axis
void CPlot::plot0(double *px, double *py, int N, COLORREF color0)
{
	int i, x0, x, y0, y;
	CPen aPen;
	aPen.CreatePen(PS_SOLID, 1, color0);
	pDC->SelectObject(&aPen);

	ConvertCoord(x0, y0, px[0], py[0]);
	pDC->MoveTo(x0, y0);

	for (i = 1; i < N; i++) {
		ConvertCoord(x, y, px[i], py[i]);
		pDC->LineTo(x, y);
	}
}

// scatter data points
void CPlot::scatter(double *px, double *py, int N, char color)
{
	COLORREF color0;
	if (autoaxis) {
		if (isempty) {
			AutoResizeAxis(px, py, N);
			update();
			isempty = false;
		}
		else
			if (AutoInflateAxis(px, py, N))
				update();
	}

	color0 = SwitchColor(color);
	scatter0(px, py, N, color0);
	PlotAxesText();

	// store data
	double *pxdata{ new double[N] {} };
	double *pydata{ new double[N] {} };
	memcpy(pxdata, px, N * 8);
	memcpy(pydata, py, N * 8);
	ScatterXList.push_back(pxdata);
	ScatterYList.push_back(pydata);
	ScatterNList.push_back(N);
	ScatterColorList.push_back(color0);
}

// scatter data points without storing data or auto axis
void CPlot::scatter0(double *px, double *py, int N, COLORREF color0)
{
	int len = 5;
	int i, x, y;

	CPen aPen;
	aPen.CreatePen(PS_SOLID, 1, color0);
	pDC->SelectObject(&aPen);

	for (i = 0; i < N; i++) {
		ConvertCoord(x, y, px[i], py[i]);
		pDC->MoveTo(x - len, y);
		pDC->LineTo(x + len, y);
		pDC->MoveTo(x, y - len);
		pDC->LineTo(x, y + len);
	}
}

// clean figure, reset axis
void CPlot::clf()
{
	int i{};
	// clean data
	for (i = 0; i < PlotXList.size(); i++)
		delete[] PlotXList[i];
	PlotXList.resize(0);
	for (i = 0; i < PlotYList.size(); i++)
		delete[] PlotYList[i];
	PlotYList.resize(0);
	for (i = 0; i < ScatterXList.size(); i++)
		delete[] ScatterXList[i];
	ScatterXList.resize(0);
	for (i = 0; i < ScatterYList.size(); i++)
		delete[] ScatterYList[i];
	ScatterYList.resize(0);
	PlotNList.resize(0);
	PlotColorList.resize(0);
	ScatterNList.resize(0);
	ScatterColorList.resize(0);
	ColorNo = -1;

	// reset axis and plot
	axis_xmin = 0; axis_xmax = 1;
	axis_ymin = 0; axis_ymax = 1;
	xtickmin = 0; xtickmax = 1;
	ytickmin = 0; ytickmax = 1;
	Nxticks = 26; Nyticks = 26;
	xtxtstart = 0; ytxtstart = 0;
	xtxtspace = 5; ytxtspace = 5;
	Nxdigits = 2; Nydigits = 2;
	update();
	isempty = true;
}

void CPlot::OnPaint()
{
	CDialog::OnPaint();
	// comment the following code to enhance performance
	// and use update() whenever needed
	update();
}

// This function is like OnPaint(), used to change background color
HBRUSH CPlot::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = m_brush;
	return hbr;
}

// redraw everything on dialog
void CPlot::update()
{
	int i{};
	pDC->FillRect(&rc, &m_brush);
	PlotAxes();
	for (i = 0; i < PlotNList.size(); i++)
		plot0(PlotXList[i], PlotYList[i], PlotNList[i], PlotColorList[i]);
	for (i = 0; i < ScatterNList.size(); i++)
		scatter0(ScatterXList[i], ScatterYList[i],
			ScatterNList[i], ScatterColorList[i]);
	PlotAxesText();
}

// plot axis fram and ticks
void CPlot::PlotAxes()
{
	int i{}, ticklen{ 3 }, ticklen1{ 6 };
	CPen aPen;
	COLORREF color0 = SwitchColor('k');

	GetClientRect(&rc);
	axis_x0 = rc.left + 55;
	axis_y0 = rc.bottom - 30;
	axis_x1 = rc.right - 30;
	axis_y1 = rc.top + 15;

	aPen.CreatePen(PS_SOLID, 1, color0);
	pDC->SelectObject(&aPen);

	pDC->MoveTo(axis_x0, axis_y0);
	pDC->LineTo(axis_x1, axis_y0);
	pDC->LineTo(axis_x1, axis_y1);
	pDC->LineTo(axis_x0, axis_y1);
	pDC->LineTo(axis_x0, axis_y0);

	// plot ticks
	int xtick0{}, xtick1{}, ytick0{}, ytick1{};
	ConvertCoord(xtick0, ytick0, xtickmin, ytickmin);
	ConvertCoord(xtick1, ytick1, xtickmax, ytickmax);
	// plot x ticks
	if (xticks) delete[] xticks;
	xticks = linspace(xtick0, xtick1, Nxticks);
	for (i = 0; i < Nxticks; i++) {
		pDC->MoveTo(xticks[i], axis_y0);
		pDC->LineTo(xticks[i], axis_y0 - ticklen);
		pDC->MoveTo(xticks[i], axis_y1);
		pDC->LineTo(xticks[i], axis_y1 + ticklen);
	}
	for (i = xtxtstart; i < Nxticks; i += xtxtspace) {
		pDC->MoveTo(xticks[i], axis_y0);
		pDC->LineTo(xticks[i], axis_y0 - ticklen1);
		pDC->MoveTo(xticks[i], axis_y1);
		pDC->LineTo(xticks[i], axis_y1 + ticklen1);
	}
	// plot y ticks
	if (yticks) delete[] yticks;
	yticks = linspace(ytick0, ytick1, Nyticks);
	for (i = 0; i < Nyticks; i++) {
		pDC->MoveTo(axis_x0, yticks[i]);
		pDC->LineTo(axis_x0 + ticklen, yticks[i]);
		pDC->MoveTo(axis_x1, yticks[i]);
		pDC->LineTo(axis_x1 - ticklen, yticks[i]);
	}
	for (i = ytxtstart; i < Nyticks; i += ytxtspace) {
		pDC->MoveTo(axis_x0, yticks[i]);
		pDC->LineTo(axis_x0 + ticklen1, yticks[i]);
		pDC->MoveTo(axis_x1, yticks[i]);
		pDC->LineTo(axis_x1 - ticklen1, yticks[i]);
	}
}

// clean outside of axis box and plot axis text
void CPlot::PlotAxesText()
{
	// clean outside of axis box
	CRect rc1;
	rc1 = rc; rc1.right = axis_x0;
	pDC->FillRect(&rc1, &m_brush);
	rc1 = rc; rc1.left = axis_x1 + 1;
	pDC->FillRect(&rc1, &m_brush);
	rc1 = rc; rc1.bottom = axis_y1;
	pDC->FillRect(&rc1, &m_brush);
	rc1 = rc; rc1.top = axis_y0 + 1;
	pDC->FillRect(&rc1, &m_brush);

	// print x text
	int i;
	CString str, format;
	CRect str_rc;
	double interv = (xtickmax - xtickmin) / (Nxticks - 1);
	format.Format(_T("%%1.%df"), Nxdigits);
	str_rc.top = axis_y0 + 10;
	str_rc.bottom = str_rc.top + 20;
	for (i = xtxtstart; i < Nxticks; i += xtxtspace) {
		str.Format(format, xtickmin + interv*i);
		str_rc.left = xticks[i] - 25;
		str_rc.right = str_rc.left + 50;
		pDC->DrawText(str, str_rc, DT_CENTER);
	}
	// print y text
	interv = (ytickmax - ytickmin) / (Nyticks - 1);
	format.Format(_T("%%1.%df"), Nydigits);
	str_rc.right = axis_x0 - 5;
	str_rc.left = str_rc.right - 50;
	
	for (i = ytxtstart; i < Nyticks; i += ytxtspace) {
		str.Format(format, ytickmin + interv*i);
		str_rc.top = yticks[i] - 8;
		str_rc.bottom = str_rc.top + 20;
		pDC->DrawText(str, str_rc, DT_RIGHT);
	}
	UpdateWindow();
}

// input plot coord (dx, dy), dc coord (x, y)
inline void CPlot::ConvertCoord(int &x, int &y, const double &dx, const double &dy)
{
	x = static_cast<int>(round(axis_x0 +
		(dx - axis_xmin) / (axis_xmax - axis_xmin) * (axis_x1 - axis_x0)));
	y = static_cast<int>(round(axis_y0 -
		(dy - axis_ymin) / (axis_ymax - axis_ymin) * (axis_y0 - axis_y1)));
}

// set ticks parameters from axis_min and axis_max
void CPlot::set_ticks()
{
	// this is a very simple implementation
	xtickmin = axis_xmin; xtickmax = axis_xmax;
	ytickmin = axis_ymin; ytickmax = axis_ymax;
	//TODO: NticksX, NticksY; xtxtspace, ytxtspace;
}


// if a list of coordinates falls out of axis box, inflate axis
// return true if changed, return false if not.
bool CPlot::AutoInflateAxis(double *px, double *py, int N)
{
	bool change = false;
	double dtemp = arraymin(px, N);
	if (axis_xmin > dtemp) {
		axis_xmin = dtemp; change = true;
	}
	dtemp = arraymax(px, N);
	if (axis_xmax < dtemp) {
		axis_xmax = dtemp; change = true;
	}
	dtemp = arraymin(py, N);
	if (axis_ymin > dtemp) {
		axis_ymin = dtemp; change = true;
	}
	dtemp = arraymax(py, N);
	if (axis_ymax < dtemp) {
		axis_ymax = dtemp; change = true;
	}
	if (change)
		set_ticks();
	return change;
}

// resize axis to fit just one set of data
void CPlot::AutoResizeAxis(double *px, double *py, int N)
{
	double L = 0.5;
	if (N == 1) {

		axis_xmin = px[0] - L; axis_xmax = px[0] + L;
		axis_ymin = py[0] - L; axis_ymax = py[0] + L;
	}
	else {
		axis_xmin = arraymin(px, N);
		axis_xmax = arraymax(px, N);
		axis_ymin = arraymin(py, N);
		axis_ymax = arraymax(py, N);
		if (axis_xmin == axis_xmax) {
			axis_xmin -= L; axis_xmax += L;
		}
		if (axis_ymin == axis_ymax) {
			axis_ymin -= L; axis_ymax += L;
		}
	}
	set_ticks();
}

// define color symbol, same as Matlab
COLORREF CPlot::SwitchColor(char color)
{
	switch (color) {
	case 'a': // auto color rotation
		++ColorNo; ColorNo %= 7;
		return default_colors[ColorNo];
	case 'b': return RGB(0, 0, 255);
	case 'r': return RGB(255, 0, 0);
	case 'y': return RGB(255, 255, 0);
	case 'm': return RGB(255, 0, 255);
	case 'g': return RGB(0, 255, 0);
	case 'c': return RGB(0, 255, 255);
	case 'k': return RGB(0, 0, 0);
	case 'w': return RGB(255, 255, 255);
	default: return RGB(0, 0, 255);
	}
}

// when user change size of figure
void CPlot::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	update();
}

// for restoring window to normal size.
void CPlot::OnSize(UINT nType, int cx, int cy)
{
	// need to skip the first time because the dialog has not been created yet.
	CDialogEx::OnSize(nType, cx, cy);
	if (OnSizeNo < 1)
		++OnSizeNo;
	else if (nType == SIZE_RESTORED)
		update();
}

void CPlot::OnClose()
{
	::SendMessage(parent, MATPLOT_FIGURE_CLOSE, 0, FigNo);
	CDialogEx::OnClose();
}