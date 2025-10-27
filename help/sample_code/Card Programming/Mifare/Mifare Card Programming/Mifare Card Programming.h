// Mifare Card Programming.h : main header file for the MIFARE CARD PROGRAMMING application
//

#if !defined(AFX_MIFARECARDPROGRAMMING_H__FBD9783B_6DF3_4746_81CF_6223EDE7AE19__INCLUDED_)
#define AFX_MIFARECARDPROGRAMMING_H__FBD9783B_6DF3_4746_81CF_6223EDE7AE19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMifareCardProgrammingApp:
// See Mifare Card Programming.cpp for the implementation of this class
//

class CMifareCardProgrammingApp : public CWinApp
{
public:
	CMifareCardProgrammingApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMifareCardProgrammingApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMifareCardProgrammingApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIFARECARDPROGRAMMING_H__FBD9783B_6DF3_4746_81CF_6223EDE7AE19__INCLUDED_)
