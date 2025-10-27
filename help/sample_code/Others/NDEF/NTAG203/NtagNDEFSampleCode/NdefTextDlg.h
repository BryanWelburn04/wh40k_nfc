#if !defined(AFX_NDEFTEXTDLG_H__C8E2D3F8_A936_43FC_BDA5_56510C63D027__INCLUDED_)
#define AFX_NDEFTEXTDLG_H__C8E2D3F8_A936_43FC_BDA5_56510C63D027__INCLUDED_

#include "NtagNDEFSampleCodeDlg.h"
#include "NdefRecord.h"
#include "Ndef.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NdefTextDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// NdefTextDlg dialog

class NdefTextDlg : public CDialog
{
// Construction
public:
	NdefTextDlg(CWnd* pParent = NULL);   // standard constructor
	void encodeToNdefMessage();
// Dialog Data
	//{{AFX_DATA(NdefTextDlg)
	enum { IDD = IDD_DialogText };
	CButton	m_RadioButtonUtf8;
	CButton	m_RadioButtonUtf16;
	CEdit	m_EditMessage;
	CComboBox	m_ComboBoxLanguage;
	CButton	m_ButtonEncodeText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NdefTextDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(NdefTextDlg)
	afx_msg void OnSelchangeComboBoxLanguage();
	afx_msg void OnButtonEncodeText();
	//}}AFX_MSG
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	Helper cHelper_;
	NtagNDEFSampleCodeDlg *pFormParent_; // Pointer to Main Window
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NDEFTEXTDLG_H__C8E2D3F8_A936_43FC_BDA5_56510C63D027__INCLUDED_)
