#pragma once

#ifndef __AFXWIN_H__
#error "include stdafx.h before to generate PCH file"
#endif

#include "resource.h"

class CMatPlotApp : public CWinApp
{
public:
	CMatPlotApp();

public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
