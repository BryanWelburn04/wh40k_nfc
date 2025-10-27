#if !defined(AFX_NDEFVCARDDLG_H__74CF3F44_1DD9_412C_942F_9D4A666B76FB__INCLUDED_)
#define AFX_NDEFVCARDDLG_H__74CF3F44_1DD9_412C_942F_9D4A666B76FB__INCLUDED_

#include "NtagNDEFSampleCodeDlg.h"
#include "NdefRecord.h"
#include "VCardIdentificationType.h"
#include "Ndef.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NdefVCardDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// NdefVCardDlg dialog

class NdefVCardDlg : public CDialog
{
// Construction
public:
	NdefVCardDlg(CWnd* pParent = NULL);   // standard constructor
	void encodeVCard();
	bool validateCharacters(CString testString);

// Dialog Data
	//{{AFX_DATA(NdefVCardDlg)
	enum { IDD = IDD_DialogVCard };
	CEdit	m_EditSurname;
	CEdit	m_EditMiddleName;
	CEdit	m_EditGivenName;
	CButton	m_ButtonEncodeVCard;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(NdefVCardDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(NdefVCardDlg)
	afx_msg void OnButtonEncodeVCard();
	//}}AFX_MSG
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

private:
	Helper cHelper_;
	NtagNDEFSampleCodeDlg *pFormParent_; // Pointer to Main Window
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NDEFVCARDDLG_H__74CF3F44_1DD9_412C_942F_9D4A666B76FB__INCLUDED_)
