#pragma once
#include <cmath>
#include <vector>
using namespace std;

class CPlot : public CDialogEx
{
	DECLARE_DYNAMIC(CPlot)

public:
	CPlot(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPlot();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PLOT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()
	HICON m_hIcon;
protected:
	void plot0(double *px, double *py, int N, COLORREF color0);
	void scatter0(double *px, double *py, int N, COLORREF color0);
	void PlotAxes();
	void PlotAxesText();
	void ConvertCoord(int &x, int &y, const double &dx, const double &dy);
	bool AutoInflateAxis(double *px, double *py, int N);
	void AutoResizeAxis(double *px, double *py, int N);
	COLORREF SwitchColor(char color);

public:
	void plot(double *px, double *py, int N, char color = 'a');
	void scatter(double *px, double *py, int N, char color = 'a');
	void clf();
	void set_ticks();
	void update();
	

	CDC *pDC; // dialog device context
	CRect rc; // for dialog rect
	CBrush m_brush; // for dialog background color;
	int axis_x0, axis_y0, axis_x1, axis_y1; // boundaries of the plot box
	double axis_xmin, axis_xmax, axis_ymin, axis_ymax; // boundaries of the data values
	int Nxticks, Nyticks; // number of data ticks on each axis (0 not included)
	int *xticks, *yticks; // pixel coordinates for ticks
	double xtickmin, xtickmax; // real coordinates of first and last ticks
	double ytickmin, ytickmax;
	int xtxtstart, ytxtstart; // which tick to start text, 0 means first
	int xtxtspace, ytxtspace; // number of tick spaces between two texts
	int Nxdigits, Nydigits; // number of digits to display on axis
	bool isempty; // whether there is no data ploted yet
	bool autoaxis; // whether the axis can automatically change
	COLORREF default_colors[7]; // the colors to rotate using plot() and scatter()
	int ColorNo; // the index of default_colors.
	HWND parent; // m_hWnd of the parent dialog
	int FigNo; // the figure number of this figure
	int OnSizeNo; // the # of times OnSize() is called

	// all plot data
	vector<double*> PlotXList;
	vector<double*> PlotYList;
	vector<int> PlotNList;
	vector<COLORREF> PlotColorList;
	vector<double *> ScatterXList;
	vector<double *> ScatterYList;
	vector<int> ScatterNList;
	vector<COLORREF> ScatterColorList;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

// linspace like Matlab, parray must be of size N.
inline double* linspace(double var1, double var2, int N)
{
	double *parray{ new double[N] };
	double interv = (var2 - var1) / (N - 1);
	parray[0] = var1; parray[N - 1] = var2;
	for (int i{ 1 }; i < N - 1; i++)
		parray[i] = var1 + interv*i;
	return parray;
}

// linspace for integers
inline int* linspace(int var1, int var2, int N)
{
	int *parray{ new int[N] {} };
	double *pdarray;
	pdarray = linspace((double)var1, (double)var2, N);
	for (int i{}; i < N; i++)
		parray[i] = static_cast<int>(round(pdarray[i]));
	delete[] pdarray;
	return parray;
}

// find the maximum value in an array
inline double arraymax(double *px, int N)
{
	double maxval{ -1.7e308 };
	for (int i = 0; i < N; i++) {
		if (maxval < px[i])
			maxval = px[i];
	}
	return maxval;
}

// find the minimum value in an array
inline double arraymin(double *px, int N)
{
	double minval{ 1.7e308 };
	for (int i = 0; i < N; i++) {
		if (minval > px[i])
			minval = px[i];
	}
	return minval;
}