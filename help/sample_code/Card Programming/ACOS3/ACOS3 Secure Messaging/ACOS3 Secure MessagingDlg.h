// ACOS3 Secure MessagingDlg.h : header file
//

#if !defined(AFX_ACOS3SECUREMESSAGINGDLG_H__1516E07D_DE04_4D32_AC19_A36112008AA3__INCLUDED_)
#define AFX_ACOS3SECUREMESSAGINGDLG_H__1516E07D_DE04_4D32_AC19_A36112008AA3__INCLUDED_

#include "Acos3.h"
#include "PcscReader.h"
#include "PcscException.h"
#include "CardException.h"
#include "NoPasteEdit.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CACOS3SecureMessagingDlg dialog

class CACOS3SecureMessagingDlg : public CDialog
{
// Construction
public:
	CACOS3SecureMessagingDlg(CWnd* pParent = NULL);	// standard constructor
	void resetFields();
	void enableControls(bool isEnable);
	long loadReadersListToControl();
	void displayMessage(CString msg);
	bool isAcos3();
	void clearBuffers();

// Dialog Data
	//{{AFX_DATA(CACOS3SecureMessagingDlg)
	enum { IDD = IDD_ACOS3SECUREMESSAGING_DIALOG };
	CEdit	m_EditBoxData;
	CButton	m_ButtonClear;
	CButton	m_RadioButtonDes;
	CButton	m_RadioButton3Des;
	CNoPasteEdit	m_EditBoxTerminalKey;
	CEdit	m_EditBoxReadWiteLength;
	CEdit	m_EditBoxReafWriteFieldId2;
	CEdit	m_EditBoxReadWriteFieldId1;
	CEdit	m_EditBoxOffset2;
	CEdit	m_EditBoxOffset1;
	CEdit	m_EditBoxLength2;
	CEdit	m_EditBoxLength1;
	CNoPasteEdit	m_EditBoxCardKey;
	CEdit	m_EditBoxCardFormatFieldId2;
	CEdit	m_EditBoxCardFormatFieldId1;
	CEdit	m_EditBoxApduLogs;
	CComboBox	m_ComboBoxReaderNames;
	CButton	m_CheckBoxSecureMessaging;
	CButton	m_ButonWriteBinary;
	CButton	m_ButtonReset;
	CButton	m_ButtonReadBinary;
	CButton	m_ButtonQuit;
	CButton	m_ButtonOk;
	CButton	m_ButtonInitialize;
	CButton	m_ButtonFormatCard;
	CButton	m_ButtonConnect;
	CButton	m_ButtonAuthenticate;
	CString	sEditBoxApduLogsValue;
	CString	sEditBoxDataValue;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CACOS3SecureMessagingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CACOS3SecureMessagingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonInitialize();
	afx_msg void OnButtonConnect();
	afx_msg void OnButtonFormatCard();
	afx_msg void OnButtonAuthenticate();
	afx_msg void OnButtonWriteBinary();
	afx_msg void OnButtonReadBinary();
	afx_msg void OnButtonClear();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonQuit();
	afx_msg void OnRadioButtonDes();
	afx_msg void OnRadioButton3Des();
	afx_msg void OnKillfocusEditBoxReadWriteLength();
	afx_msg void OnChangeEditBoxReadWriteLength();
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

#endif // !defined(AFX_ACOS3SECUREMESSAGINGDLG_H__1516E07D_DE04_4D32_AC19_A36112008AA3__INCLUDED_)
