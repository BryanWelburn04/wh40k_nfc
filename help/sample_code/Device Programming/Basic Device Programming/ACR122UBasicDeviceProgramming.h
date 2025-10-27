// ACR122UBasicDeviceProgramming.h : main header file for the ACR122UBASICDEVICEPROGRAMMING application
//

#if !defined(AFX_ACR122UBASICDEVICEPROGRAMMING_H__683E8622_0A8E_4EF4_92C0_ABA8DBECBF93__INCLUDED_)
#define AFX_ACR122UBASICDEVICEPROGRAMMING_H__683E8622_0A8E_4EF4_92C0_ABA8DBECBF93__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CACR122UBasicDeviceProgrammingApp:
// See ACR122UBasicDeviceProgramming.cpp for the implementation of this class
//

class CACR122UBasicDeviceProgrammingApp : public CWinApp
{
public:
	CACR122UBasicDeviceProgrammingApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACR122UBasicDeviceProgrammingApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CACR122UBasicDeviceProgrammingApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACR122UBASICDEVICEPROGRAMMING_H__683E8622_0A8E_4EF4_92C0_ABA8DBECBF93__INCLUDED_)
