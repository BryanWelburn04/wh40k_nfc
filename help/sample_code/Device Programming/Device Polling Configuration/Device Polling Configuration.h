// Device Polling Configuration.h : main header file for the DEVICE POLLING CONFIGURATION application
//

#if !defined(AFX_DEVICEPOLLINGCONFIGURATION_H__2C708102_0A54_48CC_AA36_3E7B8656DD3E__INCLUDED_)
#define AFX_DEVICEPOLLINGCONFIGURATION_H__2C708102_0A54_48CC_AA36_3E7B8656DD3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CDevicePollingConfigurationApp:
// See Device Polling Configuration.cpp for the implementation of this class
//

class CDevicePollingConfigurationApp : public CWinApp
{
public:
	CDevicePollingConfigurationApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDevicePollingConfigurationApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDevicePollingConfigurationApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEVICEPOLLINGCONFIGURATION_H__2C708102_0A54_48CC_AA36_3E7B8656DD3E__INCLUDED_)
