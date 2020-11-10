// trantest.h : main header file for the TRANTEST application
//

#if !defined(AFX_TRANTEST_H__0B49C9E6_2C62_11D5_BA8A_5254AB32728A__INCLUDED_)
#define AFX_TRANTEST_H__0B49C9E6_2C62_11D5_BA8A_5254AB32728A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTrantestApp:
// See trantest.cpp for the implementation of this class
//

class CTrantestApp : public CWinApp
{
public:
	CTrantestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrantestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTrantestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANTEST_H__0B49C9E6_2C62_11D5_BA8A_5254AB32728A__INCLUDED_)
