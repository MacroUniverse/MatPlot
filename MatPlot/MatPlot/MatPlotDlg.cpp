#include "stdafx.h"
#include "MatPlot.h"
#include "MatPlotDlg.h"
#include "afxdialogex.h"
#include "resource.h"

IMPLEMENT_DYNAMIC(CMatPlotDlg, CDialogEx)

CMatPlotDlg::CMatPlotDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MAIN, pParent)
	, NFig{}, CurrentFig{ -1 }
	, FigNo{}, DataNo{}
	, axis_xmin{}, axis_xmax{ 1 }
	, axis_ymin{}, axis_ymax{ 1 }
	, xtickmin{}, xtickmax{ 1 }
	, ytickmin{}, ytickmax{ 1 }
	, Nxticks{ 26 }, Nyticks{ 26 }
	, xtxtstart{ 0 }, ytxtstart{ 0 }
	, xtxtspace{ 5 }, ytxtspace{ 5 }
	, Nxdigits{ 2 }, Nydigits{ 2 }
{
}

CMatPlotDlg::~CMatPlotDlg()
{
}

void CMatPlotDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DATANO, DataNo);
	DDX_Text(pDX, IDC_I, FigNo);
	DDX_Text(pDX, IDC_XMIN, axis_xmin);
	DDX_Text(pDX, IDC_XMAX, axis_xmax);
	DDX_Text(pDX, IDC_YMIN, axis_ymin);
	DDX_Text(pDX, IDC_YMAX, axis_ymax);
	DDX_Text(pDX, IDC_XTICKMIN, xtickmin);
	DDX_Text(pDX, IDC_XTICKMAX, xtickmax);
	DDX_Text(pDX, IDC_YTICKMIN, ytickmin);
	DDX_Text(pDX, IDC_YTICKMAX, ytickmax);
	DDX_Text(pDX, IDC_NXTICKS, Nxticks);
	DDX_Text(pDX, IDC_NYTICKS, Nyticks);
	DDX_Text(pDX, IDC_XTXTSTART, xtxtstart);
	DDX_Text(pDX, IDC_YTXTSTART, ytxtstart);
	DDX_Text(pDX, IDC_XTXTSPACE, xtxtspace);
	DDX_Text(pDX, IDC_YTXTSPACE, ytxtspace);
	DDX_Text(pDX, IDC_NXDIGITS, Nxdigits);
	DDX_Text(pDX, IDC_NYDIGITS, Nydigits);
}

BEGIN_MESSAGE_MAP(CMatPlotDlg, CDialogEx)
	ON_MESSAGE(MATPLOT_CLOSE, close)
	ON_MESSAGE(MATPLOT_CLF, clf)
	ON_MESSAGE(MATPLOT_PLOT, plot)
	ON_MESSAGE(MATPLOT_FIGURE, figure)
	ON_MESSAGE(MATPLOT_SCATTER, scatter)
	ON_MESSAGE(MATPLOT_HIDE_CONTROL, hide_control)
	ON_MESSAGE(MATPLOT_SHOW_CONTROL, show_control)
	ON_MESSAGE(MATPLOT_AXIS, axis)
	ON_MESSAGE(MATPLOT_CLOSE_ALL, close_all)
	ON_MESSAGE(MATPLOT_FIGURE_CLOSE, figure_close)
	ON_MESSAGE(MATPLOT_AXIS_AUTO, axis_auto)
	ON_MESSAGE(MATPLOT_AXIS_MANUAL, axis_manual)
	ON_MESSAGE(MATPLOT_XTICKS, xticks)
	ON_MESSAGE(MATPLOT_YTICKS, yticks)

	// main_dlg test buttons
	ON_BN_CLICKED(IDC_PLOT, &CMatPlotDlg::OnPlot)
	ON_BN_CLICKED(IDC_FIGURE, &CMatPlotDlg::OnFigure)
	ON_BN_CLICKED(IDC_CLOSE, &CMatPlotDlg::OnClose)
	ON_BN_CLICKED(IDC_CLOSE_ALL, &CMatPlotDlg::OnCloseAll)
	ON_BN_CLICKED(IDC_FIGURE_I, &CMatPlotDlg::OnFigureI)
	ON_BN_CLICKED(IDC_SCATTER, &CMatPlotDlg::OnScatter)
	ON_BN_CLICKED(IDC_CLOSE_I, &CMatPlotDlg::OnCloseI)
	ON_BN_CLICKED(IDC_CLF, &CMatPlotDlg::OnClf)
	ON_BN_CLICKED(IDC_AXIS, &CMatPlotDlg::OnAxis)
	ON_BN_CLICKED(IDC_AUTOAXIS, &CMatPlotDlg::OnAutoaxis)
	ON_BN_CLICKED(IDC_XTICKS, &CMatPlotDlg::OnXticks)
	ON_BN_CLICKED(IDC_YTICKS, &CMatPlotDlg::OnYticks)
END_MESSAGE_MAP()

BOOL CMatPlotDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_hIcon = AfxGetApp()->LoadIcon(IDI_MATPLOT);
	SetIcon(m_hIcon, TRUE);

	axis_auto_check = (CButton*)GetDlgItem(IDC_AUTOAXIS);
	SetDlgItemInt(IDC_CURRENTFIG, CurrentFig);
	return TRUE;
}

LRESULT CMatPlotDlg::figure(WPARAM wparam, LPARAM lparam)
{
	int FigNo = (int)lparam;
	if (FigNo < 0) //No argument, create new figure
	{
		++NFig;
		CurrentFig = NFig - 1;
		SetDlgItemInt(IDC_CURRENTFIG, CurrentFig);
		CString num;
		num.Format(_T("figure%d"), CurrentFig + 1);
		FigList.push_back(new CPlot);
		CPlot *pPlot = FigList[CurrentFig];
		pPlot->parent = m_hWnd;
		pPlot->FigNo = CurrentFig;
		pPlot->Create(IDD_PLOT);
		pPlot->ShowWindow(SW_SHOW);
		pPlot->SetWindowTextW(num);
		SetDlgAxis(pPlot->axis_xmin, pPlot->axis_xmax,
			pPlot->axis_ymin, pPlot->axis_ymax);
		SetDlgTicksX(pPlot->xtickmin, pPlot->xtickmax, pPlot->Nxticks,
			pPlot->xtxtstart, pPlot->xtxtspace, pPlot->Nxdigits);
		SetDlgTicksY(pPlot->ytickmin, pPlot->ytickmax, pPlot->Nyticks,
			pPlot->ytxtstart, pPlot->ytxtspace, pPlot->Nydigits);
		axis_auto_check->SetCheck(1);
		return 0;//CurrentFig;
	}
	else //set current figure, or create if closed
	{
		if (NFig < FigNo + 1)
		{
			NFig = FigNo + 1;
			FigList.resize(NFig, nullptr);
		}
		CurrentFig = FigNo;
		SetDlgItemInt(IDC_CURRENTFIG, CurrentFig);
		CPlot *pPlot = FigList[CurrentFig];
		if (!pPlot) // dlg closed
		{
			CString num;
			num.Format(_T("figure%d"), CurrentFig + 1);
			FigList[CurrentFig] = new CPlot;
			pPlot = FigList[CurrentFig];
			pPlot->parent = m_hWnd;
			pPlot->FigNo = CurrentFig;
			pPlot->Create(IDD_PLOT);
			pPlot->ShowWindow(SW_SHOW);
			pPlot->SetWindowTextW(num);
		}
		SetDlgAxis(pPlot->axis_xmin, pPlot->axis_xmax,
			pPlot->axis_ymin, pPlot->axis_ymax);
		SetDlgTicksX(pPlot->xtickmin, pPlot->xtickmax, pPlot->Nxticks,
			pPlot->xtxtstart, pPlot->xtxtspace, pPlot->Nxdigits);
		SetDlgTicksY(pPlot->ytickmin, pPlot->ytickmax, pPlot->Nyticks,
			pPlot->ytxtstart, pPlot->ytxtspace, pPlot->Nydigits);
		if (pPlot->autoaxis)
			axis_auto_check->SetCheck(1);
		else
			axis_auto_check->SetCheck(0);
		return 0;
	}
}

// close the current figure, or a selected figure.
LRESULT CMatPlotDlg::close(WPARAM wparam, LPARAM lparam)
{
	int FigNo = (int)lparam;
	if (FigNo < 0)
	{
		if (CurrentFig > -1)
			::SendMessage(FigList[CurrentFig]->m_hWnd, WM_CLOSE, 0, 0);
		//FigList[CurrentFig]->EndDialog(0);
	}
	else if (FigNo < NFig && FigList[FigNo])
		::SendMessage(FigList[FigNo]->m_hWnd, WM_CLOSE, 0, 0);
		//FigList[FigNo]->EndDialog(0);
	return 0;
}

// called when figure is when manually closed
LRESULT CMatPlotDlg::figure_close(WPARAM wparam, LPARAM lparam)
{
	FigNo = (int)lparam;
	FigList[FigNo] = nullptr;
	int NextFig = SearchOpenFig(FigNo);
	if (NextFig < 0 || CurrentFig == FigNo)
	{
		CurrentFig = NextFig;
		SetDlgItemInt(IDC_CURRENTFIG, CurrentFig);
	}
	return 0;
}

// close all figures.
LRESULT CMatPlotDlg::close_all(WPARAM wparam, LPARAM lparam)
{
	for (int i = 0; i < NFig; i++)
	{
		CPlot *pPlot = FigList[i];
		if (pPlot) // not null and not destroyed
			::SendMessage(pPlot->m_hWnd, WM_CLOSE, 0, 0);
	}
	CurrentFig = -1;
	SetDlgItemInt(IDC_CURRENTFIG, CurrentFig);
	return 0;
}

LRESULT CMatPlotDlg::plot(WPARAM wparam, LPARAM lparam)
{
	if (CurrentFig < 0) // no figure, create one
		SendMessage(MATPLOT_FIGURE, 0, 0);
	Plot_Param *ptr = (Plot_Param *)lparam;
	CPlot *pPlot = FigList[CurrentFig];
	pPlot->plot(ptr->px, ptr->py, ptr->N, ptr->color);
	if (pPlot->autoaxis)
	{
		SetDlgAxis(pPlot->axis_xmin, pPlot->axis_xmax,
			pPlot->axis_ymin, pPlot->axis_ymax);
		SetDlgTicksX(pPlot->xtickmin, pPlot->xtickmax, pPlot->Nxticks,
			pPlot->xtxtstart, pPlot->xtxtspace, pPlot->Nxdigits);
		SetDlgTicksY(pPlot->ytickmin, pPlot->ytickmax, pPlot->Nyticks,
			pPlot->ytxtstart, pPlot->ytxtspace, pPlot->Nydigits);
	}
	return 0;
}

LRESULT CMatPlotDlg::scatter(WPARAM wparam, LPARAM lparam)
{
	if (CurrentFig < 0) // no figure, create one
	{
		SendMessage(MATPLOT_FIGURE, 0, 0);
	}
	CPlot *pPlot = FigList[CurrentFig];
	Plot_Param *ptr = (Plot_Param *)lparam;
	pPlot->scatter(ptr->px, ptr->py, ptr->N, ptr->color);
	if (pPlot->autoaxis)
	{
		SetDlgAxis(pPlot->axis_xmin, pPlot->axis_xmax,
			pPlot->axis_ymin, pPlot->axis_ymax);
		SetDlgTicksX(pPlot->xtickmin, pPlot->xtickmax, pPlot->Nxticks,
			pPlot->xtxtstart, pPlot->xtxtspace, pPlot->Nxdigits);
		SetDlgTicksY(pPlot->ytickmin, pPlot->ytickmax, pPlot->Nyticks,
			pPlot->ytxtstart, pPlot->ytxtspace, pPlot->Nydigits);
	}
	return 0;
}

LRESULT CMatPlotDlg::clf(WPARAM wparam, LPARAM lparam)
{
	CPlot *pPlot = FigList[CurrentFig];
	pPlot->clf();
	SetDlgAxis(pPlot->axis_xmin, pPlot->axis_xmax,
		pPlot->axis_ymin, pPlot->axis_ymax);
	SetDlgTicksX(pPlot->xtickmin, pPlot->xtickmax, pPlot->Nxticks,
		pPlot->xtxtstart, pPlot->xtxtspace, pPlot->Nxdigits);
	SetDlgTicksY(pPlot->ytickmin, pPlot->ytickmax, pPlot->Nyticks,
		pPlot->ytxtstart, pPlot->ytxtspace, pPlot->Nydigits);
	return 0;
}

LRESULT CMatPlotDlg::axis(WPARAM wparam, LPARAM lparam)
{
	Axis_Param *ptr = (Axis_Param *)lparam;
	if (CurrentFig < 0)
		SendMessage(MATPLOT_FIGURE, 0, 0);
	CPlot *pPlot = FigList[CurrentFig];
	pPlot->axis_xmin = ptr->xmin; pPlot->axis_xmax = ptr->xmax;
	pPlot->axis_ymin = ptr->ymin; pPlot->axis_ymax = ptr->ymax;
	pPlot->set_ticks();
	pPlot->update();
	SetDlgAxis(pPlot->axis_xmin, pPlot->axis_xmax,
		pPlot->axis_ymin, pPlot->axis_ymax);
	SetDlgTicksX(pPlot->xtickmin, pPlot->xtickmax, pPlot->Nxticks,
		pPlot->xtxtstart, pPlot->xtxtspace, pPlot->Nxdigits);
	SetDlgTicksY(pPlot->ytickmin, pPlot->ytickmax, pPlot->Nyticks,
		pPlot->ytxtstart, pPlot->ytxtspace, pPlot->Nydigits);
	return 0;
}

LRESULT CMatPlotDlg::axis_auto(WPARAM wparam, LPARAM lparam)
{
	if (CurrentFig < 0) // no figure, create one
		SendMessage(MATPLOT_FIGURE, 0, 0);
	FigList[CurrentFig]->autoaxis = true;
	axis_auto_check->SetCheck(1);
	return 0;
}

LRESULT CMatPlotDlg::axis_manual(WPARAM wparam, LPARAM lparam)
{
	if (CurrentFig < 0) // no figure, create one
		SendMessage(MATPLOT_FIGURE, 0, 0);
	FigList[CurrentFig]->autoaxis = false;
	axis_auto_check->SetCheck(0);
	return 0;
}

LRESULT CMatPlotDlg::xticks(WPARAM wparam, LPARAM lparam)
{
	if (CurrentFig < 0)
		SendMessage(MATPLOT_FIGURE, 0, 0);
	Tick_Param *param = (Tick_Param *)lparam;
	CPlot *pPlot = FigList[CurrentFig];
	pPlot->xtickmin = param->tickmin; pPlot->xtickmax = param->tickmax;
	pPlot->Nxticks = param->Nticks; 
	pPlot->xtxtstart = param->txtstart; pPlot->xtxtspace = param->txtspace;
	pPlot->Nxdigits = param->Ndigits;
	SetDlgTicksX(param->tickmin, param->tickmax, param->Nticks,
		param->txtstart, param->txtspace, param->Ndigits);
	pPlot->update();
	return 0;
}

LRESULT CMatPlotDlg::yticks(WPARAM wparam, LPARAM lparam)
{
	if (CurrentFig < 0)
		SendMessage(MATPLOT_FIGURE, 0, 0);
	Tick_Param *param = (Tick_Param *)lparam;
	CPlot *pPlot = FigList[CurrentFig];
	pPlot->ytickmin = param->tickmin; pPlot->ytickmax = param->tickmax;
	pPlot->Nyticks = param->Nticks;
	pPlot->ytxtstart = param->txtstart; pPlot->ytxtspace = param->txtspace;
	pPlot->Nydigits = param->Ndigits;
	SetDlgTicksY(param->tickmin, param->tickmax, param->Nticks,
		param->txtstart, param->txtspace, param->Ndigits);
	pPlot->update();
	return 0;
}

LRESULT CMatPlotDlg::hide_control(WPARAM wparam, LPARAM lparam)
{
	ShowWindow(SW_HIDE);
	UpdateWindow();
	return 0;
}

LRESULT CMatPlotDlg::show_control(WPARAM wparam, LPARAM lparam)
{
	ShowWindow(SW_SHOW);
	UpdateWindow();
	return 0;
}

// set edit boxes showing axis limits
void CMatPlotDlg::SetDlgAxis(const double &xmin, const double &xmax,
	const double &ymin, const double &ymax)
{
	CString num;
	num.Format(_T("%1.2f"), xmin);
	SetDlgItemText(IDC_XMIN, num);
	num.Format(_T("%1.2f"), xmax);
	SetDlgItemText(IDC_XMAX, num);
	num.Format(_T("%1.2f"), ymin);
	SetDlgItemText(IDC_YMIN, num);
	num.Format(_T("%1.2f"), ymax);
	SetDlgItemText(IDC_YMAX, num);
}

// set edit boxes showing x tick parameters
void CMatPlotDlg::SetDlgTicksX(const double &tickmin, const double &tickmax,
	const int &Nticks, const int &txtstart,
	const int &txtspace, const int &Ndigits)
{
	CString num;
	num.Format(_T("%1.2f"), tickmin);
	SetDlgItemText(IDC_XTICKMIN, num);
	num.Format(_T("%1.2f"), tickmax);
	SetDlgItemText(IDC_XTICKMAX, num);

	SetDlgItemInt(IDC_NXTICKS, Nticks);
	SetDlgItemInt(IDC_XTXTSTART, txtstart);
	SetDlgItemInt(IDC_XTXTSPACE, txtspace);
	SetDlgItemInt(IDC_NXDIGITS, Ndigits);
}

// set edit boxes showing y tick parameters
void CMatPlotDlg::SetDlgTicksY(const double &tickmin, const double &tickmax,
	const int &Nticks, const int &txtstart,
	const int &txtspace, const int &Ndigits)
{
	CString num;
	num.Format(_T("%1.2f"), tickmin);
	SetDlgItemText(IDC_YTICKMIN, num);
	num.Format(_T("%1.2f"), tickmax);
	SetDlgItemText(IDC_YTICKMAX, num);
	
	SetDlgItemInt(IDC_NYTICKS, Nticks);
	SetDlgItemInt(IDC_YTXTSTART, txtstart);
	SetDlgItemInt(IDC_YTXTSPACE, txtspace);
	SetDlgItemInt(IDC_NYDIGITS, Ndigits);
}

// find the next open figure, if none, return -1
int CMatPlotDlg::SearchOpenFig(int FigNo)
{
	int i;
	// forward search
	for (i = FigNo + 1; i < NFig; i++)
	{
		if (FigList[i])
			return i;
	}
	// backward search
	for (i = FigNo - 1; i >= 0; i--)
	{
		if (FigList[i])
			return i;
	}
	return -1;
}

// main_dlg test buttons callback =============================

void CMatPlotDlg::OnPlot()
{
	int N = 100;
	const double pi = 3.141592653589793;
	double *px = linspace(0., 2 * pi, N);
	double *py{ new double[N] {} };

	UpdateData();
	for (int i{}; i < N; i++)
		py[i] = DataNo * sin(px[i]);

	Plot_Param param;
	param.px = px; param.py = py; param.N = N; param.color = 'a';
	SendMessage(MATPLOT_PLOT, 0, (LPARAM)&param);
	delete[] px;
	delete[] py;
}

void CMatPlotDlg::OnScatter()
{
	int N = 20;
	const double pi = 3.141592653589793;
	double *px = linspace(0., 2 * pi, N);
	double *py{ new double[N] {} };

	UpdateData();
	for (int i{}; i < N; i++)
		py[i] = DataNo * sin(px[i]);

	Plot_Param param;
	param.px = px; param.py = py; param.N = N; param.color = 'a';
	SendMessage(MATPLOT_SCATTER, 0, (LPARAM)&param);
	delete[] px;
	delete[] py;
}

void CMatPlotDlg::OnFigure()
{
	SendMessage(MATPLOT_FIGURE, 0, -1);
}

void CMatPlotDlg::OnClose()
{
	SendMessage(MATPLOT_CLOSE, 0, -1);
}

void CMatPlotDlg::OnCloseAll()
{
	SendMessage(MATPLOT_CLOSE_ALL, 0, 0);
}

void CMatPlotDlg::OnFigureI()
{
	UpdateData();
	SendMessage(MATPLOT_FIGURE, 0, FigNo);
}

void CMatPlotDlg::OnCloseI()
{
	UpdateData();
	SendMessage(MATPLOT_CLOSE, 0, FigNo);
}

void CMatPlotDlg::OnClf()
{
	SendMessage(MATPLOT_CLF, 0, 0);
}

void CMatPlotDlg::OnAxis()
{
	UpdateData();
	Axis_Param param;
	param.xmin = axis_xmin;
	param.xmax = axis_xmax;
	param.ymin = axis_ymin;
	param.ymax = axis_ymax;
	SendMessage(MATPLOT_AXIS, 0, (LPARAM)&param);
}

void CMatPlotDlg::OnAutoaxis()
{
	int state = axis_auto_check->GetCheck(); // 1 is checked, 0 is unchecked
	if (state == 1)
		SendMessage(MATPLOT_AXIS_AUTO, 0, 0);
	else
		SendMessage(MATPLOT_AXIS_MANUAL, 0, 0);
}

void CMatPlotDlg::OnXticks()
{
	UpdateData();
	Tick_Param param;
	param.tickmin = xtickmin; param.tickmax = xtickmax;
	param.Nticks = Nxticks; param.txtstart = xtxtstart;
	param.txtspace = xtxtspace; param.Ndigits = Nxdigits;
	SendMessage(MATPLOT_XTICKS, 0, (LPARAM)&param);
}

void CMatPlotDlg::OnYticks()
{
	UpdateData();
	Tick_Param param;
	param.tickmin = ytickmin; param.tickmax = ytickmax;
	param.Nticks = Nyticks; param.txtstart = ytxtstart;
	param.txtspace = ytxtspace; param.Ndigits = Nydigits;
	SendMessage(MATPLOT_YTICKS, 0, (LPARAM)&param);
}