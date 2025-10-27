// ACOS3MutualAuthenticationDlg.h : header file
//

#if !defined(AFX_ACOS3MUTUALAUTHENTICATIONDLG_H__9357A74C_FBD4_4748_BD4C_4270698BA6AB__INCLUDED_)
#define AFX_ACOS3MUTUALAUTHENTICATIONDLG_H__9357A74C_FBD4_4748_BD4C_4270698BA6AB__INCLUDED_

#include "Acos3.h"
#include "PcscReader.h"
#include "PcscException.h"
#include "CardException.h"
#include "NoPasteEdit.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CACOS3MutualAuthenticationDlg dialog

class CACOS3MutualAuthenticationDlg : public CDialog
{
// Construction
public:
	CACOS3MutualAuthenticationDlg(CWnd* pParent = NULL);	// standard constructor
	~CACOS3MutualAuthenticationDlg();
	void displayMessage(CString msg);
	void resetFields(BOOL enable);
	void enableFields(BOOL enable);

// Dialog Data
	//{{AFX_DATA(CACOS3MutualAuthenticationDlg)
	enum { IDD = IDD_ACOS3MUTUALAUTHENTICATION_DIALOG };
	CButton	RadioButton3Des;
	CButton	RadioButtonDes;
	CEdit	TextBoxLogs;
	CButton	ButtonQuit;
	CNoPasteEdit	TextBoxTerminalKey;
	CNoPasteEdit	TextBoxCardKey;
	CComboBox	ComboBoxReader;
	CButton	ButtonReset;
	CButton	ButtonClear;
	CButton	ButtonMutualAuthentication;
	CButton	ButtonFormatCard;
	CButton	ButtonConnect;
	CButton	ButtonInitialize;
	CString	TextBoxLogsValue;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACOS3MutualAuthenticationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CACOS3MutualAuthenticationDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonInitialize();
	afx_msg void OnButtonConnect();
	afx_msg void OnButtonFormat();
	afx_msg void OnButtonMutualAuthentication();
	afx_msg void OnButtonClear();
	afx_msg void OnButtonReset();
	afx_msg void OnQuit();
	afx_msg void OnRadioButtonDes();
	afx_msg void OnRadioButton3Des();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:	
	char aMessage_[100];
	Acos3* pAcos3_;
	PcscReader* pPcscReader_;

	void onPcscException(PcscException cException);
	void onCardException(CardException cException);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACOS3MUTUALAUTHENTICATIONDLG_H__9357A74C_FBD4_4748_BD4C_4270698BA6AB__INCLUDED_)