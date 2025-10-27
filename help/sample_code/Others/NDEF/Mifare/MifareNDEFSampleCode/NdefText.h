#if !defined(AFX_NDEFTEXT_H__6B9EA347_6540_48CE_ABE9_052D0B0A1A7B__INCLUDED_)
#define AFX_NDEFTEXT_H__6B9EA347_6540_48CE_ABE9_052D0B0A1A7B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NdefText.h : header file
//
#include "MifareNDEFSampleCodeDlg.h"
#include "NdefRecord.h"
/////////////////////////////////////////////////////////////////////////////
// CNdefText dialog

class CNdefText : public CDialog
{
// Construction
public:
	CNdefText(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNdefText)
	enum { IDD = IDD_DialogText };
	CStatic	m_StaticTextMessage;
	CStatic	m_StaticTextLanguage;
	CEdit m_EditBoxTextMessage;
	CComboBox m_ComboBoxTextLanguage;
	CButton	m_ButtonEncodeText;
	CButton	m_RadioButtonUtf8;
	CButton	m_RadioButtonUtf16;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNdefText)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNdefText)
	afx_msg void OnButtonencodetext();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	Helper cHelper_;
	CMifareNDEFSampleCodeDlg *pFrmParent_; //Pointer to Main Window
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NDEFTEXT_H__6B9EA347_6540_48CE_ABE9_052D0B0A1A7B__INCLUDED_)
