// Acos3Encrypt.h : main header file for the ACOS3ENCRYPT application
//

#if !defined(AFX_ACOS3ENCRYPT_H__6D94A0DF_C93C_4C66_8F55_72F9B4B6EF4A__INCLUDED_)
#define AFX_ACOS3ENCRYPT_H__6D94A0DF_C93C_4C66_8F55_72F9B4B6EF4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAcos3EncryptApp:
// See Acos3Encrypt.cpp for the implementation of this class
//

class CAcos3EncryptApp : public CWinApp
{
public:
	CAcos3EncryptApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAcos3EncryptApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CAcos3EncryptApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACOS3ENCRYPT_H__6D94A0DF_C93C_4C66_8F55_72F9B4B6EF4A__INCLUDED_)
