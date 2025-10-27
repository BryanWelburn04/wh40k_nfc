// Mifare Change Key.h : main header file for the MIFARE CHANGE KEY application
//

#if !defined(AFX_MIFARECHANGEKEY_H__8369E1D3_8A50_43A0_9DCB_9A0E0462624A__INCLUDED_)
#define AFX_MIFARECHANGEKEY_H__8369E1D3_8A50_43A0_9DCB_9A0E0462624A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMifareChangeKeyApp:
// See Mifare Change Key.cpp for the implementation of this class
//

class CMifareChangeKeyApp : public CWinApp
{
public:
	CMifareChangeKeyApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMifareChangeKeyApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMifareChangeKeyApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIFARECHANGEKEY_H__8369E1D3_8A50_43A0_9DCB_9A0E0462624A__INCLUDED_)
