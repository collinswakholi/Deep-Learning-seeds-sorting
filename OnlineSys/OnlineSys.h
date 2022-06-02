
// OnlineSys.h : main header file for the PROJECT_NAME application
//

#if _MSC_VER>1000
#pragma once
#endif
#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

#define EURESYS_SURFACE_COUNT 3
// COnlineSysApp:
// See OnlineSys.cpp for the implementation of this class
//

class COnlineSysApp : public CWinApp
{
public:
	COnlineSysApp();

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern COnlineSysApp theApp;
