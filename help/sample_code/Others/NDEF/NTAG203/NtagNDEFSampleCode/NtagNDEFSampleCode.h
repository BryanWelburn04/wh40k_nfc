// NtagNDEFSampleCode.h : main header file for the NTAGNDEFSAMPLECODE application
//

#if !defined(AFX_NTAGNDEFSAMPLECODE_H__12A67408_A28F_455F_8541_0100A3E6D616__INCLUDED_)
#define AFX_NTAGNDEFSAMPLECODE_H__12A67408_A28F_455F_8541_0100A3E6D616__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CNtagNDEFSampleCodeApp:
// See NtagNDEFSampleCode.cpp for the implementation of this class
//

class CNtagNDEFSampleCodeApp : public CWinApp
{
public:
	CNtagNDEFSampleCodeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNtagNDEFSampleCodeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CNtagNDEFSampleCodeApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NTAGNDEFSAMPLECODE_H__12A67408_A28F_455F_8541_0100A3E6D616__INCLUDED_)
