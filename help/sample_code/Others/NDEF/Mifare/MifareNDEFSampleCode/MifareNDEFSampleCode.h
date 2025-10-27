// MifareNDEFSampleCode.h : main header file for the MIFARENDEFSAMPLECODE application
//

#if !defined(AFX_MIFARENDEFSAMPLECODE_H__D01FF8CB_8286_4034_B849_2350CB0BC2A3__INCLUDED_)
#define AFX_MIFARENDEFSAMPLECODE_H__D01FF8CB_8286_4034_B849_2350CB0BC2A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMifareNDEFSampleCodeApp:
// See MifareNDEFSampleCode.cpp for the implementation of this class
//

class CMifareNDEFSampleCodeApp : public CWinApp
{
public:
	CMifareNDEFSampleCodeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMifareNDEFSampleCodeApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMifareNDEFSampleCodeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIFARENDEFSAMPLECODE_H__D01FF8CB_8286_4034_B849_2350CB0BC2A3__INCLUDED_)
