/////////////////////////////////////////////////////////////////////////////
//
// Company	: ADVANCED CARD SYSTEMS, LTD.
//
// Name		: SimplePCSCDlg.h : header file
//
// Author	: Alcendor Lorzano Chan
//
//	Date	: 14 / 09 / 20001 
//
///////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMPLEPCSCDLG_H__8CDAF746_A917_11D5_923A_00010283AE0D__INCLUDED_)
#define AFX_SIMPLEPCSCDLG_H__8CDAF746_A917_11D5_923A_00010283AE0D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>


//Standard Template Library.
typedef CList< CString,CString&> StringList;


typedef struct _strAPDU{
	BYTE CLA,INS,P1,P2,P3;
	BYTE Buffer[256];
	bool bSend;}APDU; 


/////////////////////////////////////////////////////////////////////////////
// CSimplePCSCDlg dialog

class CSimplePCSCDlg : public CDialog
{
// Construction
public:
	CSimplePCSCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSimplePCSCDlg)
	enum { IDD = IDD_SIMPLEPCSC_DIALOG };
	CButton	bTransmit;
	CButton	bStatus;
	CButton	bReleaseContext;
	CButton	bListReaders;
	CButton	bEstContext;
	CButton	bEndTransaction;
	CButton	bDisconnect;
	CButton	bConnect;
	CButton	bBeginTransaction;
	CComboBox	cbReader;
	CEdit	tDataIn;
	CEdit	m_TextBoxLogsVar;
	CString     m_TextBoxLogs;	
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimplePCSCDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON 	m_hIconBig,
		m_hIconSmall;

	// Generated message map functions
	//{{AFX_MSG(CSimplePCSCDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBTNExit();
	afx_msg void OnBTNEstablishContext();
	afx_msg void OnBTNReleaseContext();
	afx_msg void OnBTNListReaders();
	afx_msg void OnBTNConnect();
	afx_msg void OnBTNDisconnect();
	afx_msg void OnBTNStatus();
	afx_msg void OnBTNBeginTransaction();
	afx_msg void OnBTNEndTransaction();
	afx_msg void OnBTNTransmit();
	afx_msg void OnBTNClear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void showInformationMessage(CString sMessage);
	void showWarningMessage(CString sMessage);
	void showErrorMessage(CString sMessage);

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMPLEPCSCDLG_H__8CDAF746_A917_11D5_923A_00010283AE0D__INCLUDED_)
