#if !defined(AFX_NDEFVCARD_H__0E47A5C2_7F09_4E13_8637_2AA35239E35C__INCLUDED_)
#define AFX_NDEFVCARD_H__0E47A5C2_7F09_4E13_8637_2AA35239E35C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NdefVCard.h : header file
//
#include "MifareNDEFSampleCodeDlg.h"
#include "NdefRecord.h"
#include "VCardIdentificationType.h"

/////////////////////////////////////////////////////////////////////////////
// CNdefVCard dialog

class CNdefVCard : public CDialog
{
// Construction
public:
	CNdefVCard(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNdefVCard)
	enum { IDD = IDD_DialogVCard };
	CStatic	m_StaticVCardSur;
	CStatic	m_StaticVCardMiddle;
	CStatic	m_StaticVCardGiven;
	CEdit m_EditBoxVCardSur;
	CEdit m_EditBoxVCardMiddle;
	CEdit m_EditBoxVCardGiven;
	CButton	m_ButtonEncodeVCard;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNdefVCard)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNdefVCard)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonencodevcard();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	Helper cHelper_;
	Ndef cNdef_;
	CMifareNDEFSampleCodeDlg *pFrmParent_; // Pointer to Main Window
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NDEFVCARD_H__0E47A5C2_7F09_4E13_8637_2AA35239E35C__INCLUDED_)
