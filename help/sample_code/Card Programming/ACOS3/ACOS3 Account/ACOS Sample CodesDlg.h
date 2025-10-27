// ACOS Sample CodesDlg.h : header file
//

#if !defined(AFX_ACOSSAMPLECODESDLG_H__2A68B151_D788_40D9_968F_39D05779568D__INCLUDED_)
#define AFX_ACOSSAMPLECODESDLG_H__2A68B151_D788_40D9_968F_39D05779568D__INCLUDED_

#include "Acos3.h"
#include "PcscReader.h"
#include "PcscException.h"
#include "CardException.h"
#include "NoPasteEdit.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CACOSSampleCodesDlg dialog

class CACOSSampleCodesDlg : public CDialog
{
// Construction
public:
	CACOSSampleCodesDlg(CWnd* pParent = NULL);	// standard constructor

	void displayMessage(CString message);
	void isAcos3Combi();
	void enableControls(BOOL state);

// Dialog Data
	//{{AFX_DATA(CACOSSampleCodesDlg)
	enum { IDD = IDD_ACOSSAMPLECODES_DIALOG };
	CButton	CheckBoxRequireDebit;
	CButton	RadioButtonDes;
	CButton	RadioButton3Des;
	CNoPasteEdit	EditBoxRevokeDebit;
	CNoPasteEdit	EditBoxLogs;
	CNoPasteEdit	EditBoxDebitKey;
	CNoPasteEdit	EditBoxCreditKey;
	CNoPasteEdit	EditBoxCertifyKey;
	CNoPasteEdit	EditBoxAmount;
	CComboBox	ComboBoxReaderNames;
	CButton	ButtonRevokeDebit;
	CButton	ButtonReset;
	CButton	ButtonQuit;
	CButton	ButtonInquireBalance;
	CButton	ButtonInitialize;
	CButton	ButtonFormat;
	CButton	ButtonDebit;
	CButton	ButtonCredit;
	CButton	ButtonConnect;
	CButton	ButtonClear;
	CButton	m_button1;
	CButton	m_button2;
	CButton	m_button3;
	CButton	m_button4;
	CButton	m_button5;
	CButton	m_button6;
	CButton	m_button8;
	CButton	m_button9;
	CButton	m_button10;
	CButton	m_check2;	
	CButton	m_radio3;		
	CButton	m_radio4;
	CEdit	m_edit1;
	CEdit	m_edit2;
	CEdit	m_edit3;	
	CEdit	m_edit4;
	CEdit	m_edit5;		
	CEdit	m_TextBoxLogsVar;
	CString     m_TextBoxLogs;
	CComboBox	m_Combo;	
	CString	EditBoxLogsValue;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACOSSampleCodesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CACOSSampleCodesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonInitialize();
	afx_msg void OnButtonConnect();
	afx_msg void OnButtonFormat();
	afx_msg void OnButtonCredit();
	afx_msg void OnButtonDebit();
	afx_msg void OnButtonInquireBalance();
	afx_msg void OnButtonRevokeDebit();
	afx_msg void OnButtonClear();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonQuit();
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

#endif // !defined(AFX_ACOSSAMPLECODESDLG_H__2A68B151_D788_40D9_968F_39D05779568D__INCLUDED_)
