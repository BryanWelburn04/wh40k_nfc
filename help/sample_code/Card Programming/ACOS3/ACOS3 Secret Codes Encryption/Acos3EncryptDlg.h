// Acos3EncryptDlg.h : header file
//

#if !defined(AFX_ACOS3ENCRYPTDLG_H__4614D323_3924_4576_A2C7_EEBCCC03EEC2__INCLUDED_)
#define AFX_ACOS3ENCRYPTDLG_H__4614D323_3924_4576_A2C7_EEBCCC03EEC2__INCLUDED_

#include "Acos3.h"
#include "PcscReader.h"
#include "PcscException.h"
#include "CardException.h"
#include "NoPasteEdit.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAcos3EncryptDlg dialog

class CAcos3EncryptDlg : public CDialog
{
// Construction
public:
	CAcos3EncryptDlg(CWnd* pParent = NULL);
	~CAcos3EncryptDlg();
	void displayMessage(CString msg);
	void displayTitle(CString sTitle);
	void resetFields(BOOL enable);
	void enableFields(BOOL enable);
	void CAcos3EncryptDlg::getSessionKey();

// Dialog Data
	//{{AFX_DATA(CAcos3EncryptDlg)
	enum { IDD = IDD_ACOS3ENCRYPT_DIALOG };
	CButton	RadioButton3Des;
	CButton	RadioButtonDes;
	CButton	RadioButtonEncrypted;
	CButton	RadioButtonNotEncrypted;
	CEdit	EditTextLogs;
	CNoPasteEdit	EditTextValue;
	CNoPasteEdit	EditTextTerminalKey;
	CNoPasteEdit	EditTextCardKey;
	CComboBox	ComboBoxCode;
	CComboBox	ComboBoxReader;
	CButton	ButtonQuit;
	CButton	ButtonReset;
	CButton	ButtonClear;
	CButton	ButtonSubmit;
	CButton	ButtonSetValue;
	CButton	ButtonFormat;
	CButton	ButtonConnect;
	CButton	ButtonInitialize;
	CString	EditTextLogsValue;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAcos3EncryptDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAcos3EncryptDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRadioNotEncrypted();
	afx_msg void OnButtonInitialize();
	afx_msg void OnButtonConnect();
	afx_msg void OnButtonFormat();
	afx_msg void OnButtonSetValue();
	afx_msg void OnButtonSubmit();
	afx_msg void OnButtonClear();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonQuit();
	afx_msg void OnRadioButtonNotEncrypted();
	afx_msg void OnRadioButtonEncrypted();
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

	void setRadioButtonsLimit();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACOS3ENCRYPTDLG_H__4614D323_3924_4576_A2C7_EEBCCC03EEC2__INCLUDED_)
