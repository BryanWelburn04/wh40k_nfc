#if !defined(AFX_NDEFSMARTPOSTER_H__86D179D7_409B_4DD4_A330_DAF655138E52__INCLUDED_)
#define AFX_NDEFSMARTPOSTER_H__86D179D7_409B_4DD4_A330_DAF655138E52__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NdefSmartPoster.h : header file
//
#include "MifareNDEFSampleCodeDlg.h"
#include "Ndef.h"
#include "NdefException.h"

#define TEL_URI_INDEX 5

/////////////////////////////////////////////////////////////////////////////
// CNdefSmartPoster dialog

class CNdefSmartPoster : public CDialog
{
// Construction
public:
	CNdefSmartPoster(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNdefSmartPoster)
	enum { IDD = IDD_DialogSmartPoster };
	CEdit m_EditBoxSmartPosterUri;
	CComboBox m_ComboBoxSmartPosterPrefixUri;
	CButton	m_ButtonEncodeSmartPoster;
	CComboBox m_ComboBoxSmartPosterAction;
	CComboBox m_ComboBoxSmartPosterTitleLanguage;
	CStatic	m_StaticSmartPosterAction;
	CStatic	m_StaticSmartPosterUri;
	CStatic	m_StaticSmartPosterTitle;
	CStatic	m_StaticSmartPosterRequired;
	CEdit m_EditBoxSmartPosterTitle;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNdefSmartPoster)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNdefSmartPoster)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonencodesmartposter();
	afx_msg void OnSelchangeCombosmartposterprefixuri();
	afx_msg void OnKillfocusEditsmartposteruri();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	Helper cHelper_;
	Ndef cNdef_;
	CMifareNDEFSampleCodeDlg *pFrmParent_; //Pointer to Main Window
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NDEFSMARTPOSTER_H__86D179D7_409B_4DD4_A330_DAF655138E52__INCLUDED_)
