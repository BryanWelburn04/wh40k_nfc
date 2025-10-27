// SimplePCSC.h : main header file for the SIMPLEPCSC application
//

#if !defined(AFX_SIMPLEPCSC_H__8CDAF744_A917_11D5_923A_00010283AE0D__INCLUDED_)
#define AFX_SIMPLEPCSC_H__8CDAF744_A917_11D5_923A_00010283AE0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSimplePCSCApp:
// See SimplePCSC.cpp for the implementation of this class
//

class CSimplePCSCApp : public CWinApp
{
public:
	CSimplePCSCApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimplePCSCApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSimplePCSCApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMPLEPCSC_H__8CDAF744_A917_11D5_923A_00010283AE0D__INCLUDED_)
