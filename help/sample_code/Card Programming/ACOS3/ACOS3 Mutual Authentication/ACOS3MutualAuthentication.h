// ACOS3MutualAuthentication.h : main header file for the ACOS3MUTUALAUTHENTICATION application
//

#if !defined(AFX_ACOS3MUTUALAUTHENTICATION_H__DD28BE26_3D7B_463D_B59A_64A9CBFA13AD__INCLUDED_)
#define AFX_ACOS3MUTUALAUTHENTICATION_H__DD28BE26_3D7B_463D_B59A_64A9CBFA13AD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CACOS3MutualAuthenticationApp:
// See ACOS3MutualAuthentication.cpp for the implementation of this class
//

class CACOS3MutualAuthenticationApp : public CWinApp
{
public:
	CACOS3MutualAuthenticationApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACOS3MutualAuthenticationApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CACOS3MutualAuthenticationApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACOS3MUTUALAUTHENTICATION_H__DD28BE26_3D7B_463D_B59A_64A9CBFA13AD__INCLUDED_)
