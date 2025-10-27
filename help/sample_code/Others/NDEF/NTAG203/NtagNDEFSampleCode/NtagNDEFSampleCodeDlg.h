#if !defined(AFX_NTAGNDEFSAMPLECODEDLG_H__8E2BDE16_553B_4B67_B78B_40C557F05199__INCLUDED_)
#define AFX_NTAGNDEFSAMPLECODEDLG_H__8E2BDE16_553B_4B67_B78B_40C557F05199__INCLUDED_

#include "resource.h"
#include "TabControl.h"
#include "AcsReader.h"
#include "Ntag203.h" 
#include "CardSelector.h"
#include "Helper.h"
#include "Ndef.h"
#include "NdefException.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NtagNDEFSampleCodeDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// NtagNDEFSampleCodeDlg dialog

class NtagNDEFSampleCodeDlg : public CDialog
{
// Construction
public:
	void addMessageToLogs(CString message);
	void resetFields() ;
	void readFromCard();
	void writeToCard();
	NtagNDEFSampleCodeDlg(CWnd* pParent = NULL);   // standard constructor

	vector<byte> uSnepMessage_;

// Dialog Data
	//{{AFX_DATA(NtagNDEFSampleCodeDlg)
	enum { IDD = IDD_DialogNtagNDEFSampleCode };
	TabControl	m_TabMain;
	CRichEditCtrl	m_RichEditApduLogs;
	CEdit	m_EditNdefMessage;
	CComboBox	m_ComboBoxReaderList;
	CButton	m_ButtonWriteToCard;
	CButton	m_ButtonReset;
	CButton	m_ButtonReadFromCard;
	CButton	m_ButtonQuit;
	CButton	m_ButtonInitalize;
	CButton	m_ButtonConnect;
	CButton	m_ButtonClear;
	CString	m_RichEditApduLogsString;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NtagNDEFSampleCodeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(NtagNDEFSampleCodeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonInitialize();
	afx_msg void OnButtonConnect();
	afx_msg void OnButtonWriteToCard();
	afx_msg void OnButtonReadFromCard();
	afx_msg void OnButtonClear();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonQuit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	PcscReader* pPcscReader_;	
	Ntag203	  * pNtag203_;
	long lReturnCode_;
	byte uCurrentSector_, uCurrentSectorTrailer_;
	Ndef cNdef_;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NTAGNDEFSAMPLECODEDLG_H__8E2BDE16_553B_4B67_B78B_40C557F05199__INCLUDED_)
