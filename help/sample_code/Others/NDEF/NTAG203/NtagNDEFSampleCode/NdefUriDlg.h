#if !defined(AFX_NDEFURIDLG_H__1F63859A_FC57_4DD7_931A_F23470E0B584__INCLUDED_)
#define AFX_NDEFURIDLG_H__1F63859A_FC57_4DD7_931A_F23470E0B584__INCLUDED_

#include "NtagNDEFSampleCodeDlg.h"
#include "Ndef.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NdefUriDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// NdefUriDlg dialog

class NdefUriDlg : public CDialog
{
// Construction
public:
	NdefUriDlg(CWnd* pParent = NULL);   // standard constructor
	void encodeToNdefUri();
// Dialog Data
	//{{AFX_DATA(NdefUriDlg)
	enum { IDD = IDD_DialogUrl };
	CEdit	m_EditUriUri;
	CComboBox	m_ComboBoxUriUri;
	CButton	m_ButtonEncodeUri;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NdefUriDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(NdefUriDlg)
	afx_msg void OnSelchangeComboBoxUriUri();
	afx_msg void OnButtonEncodeUrl();
	afx_msg void OnKillfocusEditUriUri();
	//}}AFX_MSG
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	Helper cHelper_;
	NtagNDEFSampleCodeDlg *pFormParent_; // Pointer to Main Window
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NDEFURIDLG_H__1F63859A_FC57_4DD7_931A_F23470E0B584__INCLUDED_)
