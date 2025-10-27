#if !defined(AFX_NDEFURI_H__898A1232_0934_4657_9ACB_DF925467BE44__INCLUDED_)
#define AFX_NDEFURI_H__898A1232_0934_4657_9ACB_DF925467BE44__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NdefUri.h : header file
//
#include "MifareNDEFSampleCodeDlg.h"
#include "Ndef.h"

#define TEL_URI_INDEX 5

/////////////////////////////////////////////////////////////////////////////
// CNdefUri dialog

class CNdefUri : public CDialog
{
// Construction
public:
	CNdefUri(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNdefUri)
	enum { IDD = IDD_DialogUri };
	CButton	m_ButtonEncodeUrl;
	CComboBox m_ComboBoxUrlUri;
	CEdit m_EditBoxUrlUri;
	CStatic	m_StaticUrlUri;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNdefUri)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNdefUri)
	afx_msg void OnButtonencodeurl();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombourluri();
	afx_msg void OnKillfocusEditurluri();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	Helper cHelper_;
	Ndef cNdef_;
	CMifareNDEFSampleCodeDlg *pFrmParent_; //Pointer to Main Window
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NDEFURI_H__898A1232_0934_4657_9ACB_DF925467BE44__INCLUDED_)
