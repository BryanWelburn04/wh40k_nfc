// PersoACOSDlg.h : header file
//

#if !defined(AFX_PERSOACOSDLG_H__20BAD9E8_D392_11D5_923A_00010283AE0D__INCLUDED_)
#define AFX_PERSOACOSDLG_H__20BAD9E8_D392_11D5_923A_00010283AE0D__INCLUDED_

#include "Acos3.h"
#include "PcscReader.h"
#include "PcscException.h"
#include "CardException.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "winscard.h"



/////////////////////////////////////////////////////////////////////////////
// CPersoACOSDlg dialog

class CPersoACOSDlg : public CDialog
{
// Construction
public:
	CPersoACOSDlg(CWnd* pParent = NULL);	// standard constructor

	long loadReadersListToControl();
	void displayMessage(CString msg);
	void displayTitle(CString title);
	bool isAcos3();
	void resetFields();

// Dialog Data
	//{{AFX_DATA(CPersoACOSDlg)
	enum { IDD = IDD_PERSOACOS_DIALOG };
	CButton	ButtonInitialize;
	CButton	ButtonConnect;
	CButton	ButtonCreateFiles;
	CButton	ButtonClear;	
	CComboBox	ComboBoxReader;
	CString EditBoxResultText;
	CEdit	EditBoxResult;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPersoACOSDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON 	m_hIconBig,
			m_hIconSmall;

	// Generated message map functions
	//{{AFX_MSG(CPersoACOSDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnDestroy();
	afx_msg void OnButtonCreateFiles();
	afx_msg void OnButtonInitialize();
	afx_msg void OnButtonConnect();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonClear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

    SCARDHANDLE    m_hCard;
	SCARDCONTEXT   m_hContext;
	char		   m_sBuffer[MAX_PATH];

private:	
	char aMessage_[100];
	Acos3* pAcos3_;
	PcscReader* pPcscReader_;	

	void onPcscException(PcscException cException);
	void onCardException(CardException cException);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PERSOACOSDLG_H__20BAD9E8_D392_11D5_923A_00010283AE0D__INCLUDED_)
