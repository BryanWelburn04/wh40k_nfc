#if !defined(AFX_NDEFSMARTPOSTERDLG_H__EA2E963F_0B6A_4855_992C_136B15260F95__INCLUDED_)
#define AFX_NDEFSMARTPOSTERDLG_H__EA2E963F_0B6A_4855_992C_136B15260F95__INCLUDED_

#include "NtagNDEFSampleCodeDlg.h"
#include "Ndef.h"
#include "NdefException.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NdefSmartPosterDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// NdefSmartPosterDlg dialog

class NdefSmartPosterDlg : public CDialog
{
// Construction
public:
	
	void validateInput(CEdit *pEditbox, CString *pText);
	NdefSmartPosterDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(NdefSmartPosterDlg)
	enum { IDD = IDD_DialogSmartPoster };
	CEdit	m_EditSmartPosterUri;
	CEdit	m_EditSmartPosterTitle;
	CComboBox	m_ComboBoxSmartPosterUri;
	CComboBox	m_ComboBoxSmartPosterTitle;
	CComboBox	m_ComboBoxAction;
	CButton	m_ButtonEncodeSmartPoster;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NdefSmartPosterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(NdefSmartPosterDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonEncodeSmartPoster();
	afx_msg void OnSelchangeComboBoxUri();
	afx_msg void OnKillfocusEditSmartPosterUri();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	Helper cHelper_;
	NtagNDEFSampleCodeDlg *pFormParent_; // Pointer to Main Window
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NDEFSMARTPOSTERDLG_H__EA2E963F_0B6A_4855_992C_136B15260F95__INCLUDED_)
