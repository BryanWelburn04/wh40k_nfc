// ACOS Sample Codes.h : main header file for the ACOS SAMPLE CODES application
//

#if !defined(AFX_ACOSSAMPLECODES_H__C3CECF92_FFDF_45FB_9568_DDC7BF97EB4D__INCLUDED_)
#define AFX_ACOSSAMPLECODES_H__C3CECF92_FFDF_45FB_9568_DDC7BF97EB4D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CACOSSampleCodesApp:
// See ACOS Sample Codes.cpp for the implementation of this class
//

class CACOSSampleCodesApp : public CWinApp
{
public:
	CACOSSampleCodesApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACOSSampleCodesApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CACOSSampleCodesApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACOSSAMPLECODES_H__C3CECF92_FFDF_45FB_9568_DDC7BF97EB4D__INCLUDED_)
