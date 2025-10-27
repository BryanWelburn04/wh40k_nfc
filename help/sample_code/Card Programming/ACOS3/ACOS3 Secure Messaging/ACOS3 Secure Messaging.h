// ACOS3 Secure Messaging.h : main header file for the ACOS3 SECURE MESSAGING application
//

#if !defined(AFX_ACOS3SECUREMESSAGING_H__7A7A99E1_AA35_47D9_A42C_FC67709D9DD0__INCLUDED_)
#define AFX_ACOS3SECUREMESSAGING_H__7A7A99E1_AA35_47D9_A42C_FC67709D9DD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CACOS3SecureMessagingApp:
// See ACOS3 Secure Messaging.cpp for the implementation of this class
//

class CACOS3SecureMessagingApp : public CWinApp
{
public:
	CACOS3SecureMessagingApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACOS3SecureMessagingApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CACOS3SecureMessagingApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACOS3SECUREMESSAGING_H__7A7A99E1_AA35_47D9_A42C_FC67709D9DD0__INCLUDED_)
