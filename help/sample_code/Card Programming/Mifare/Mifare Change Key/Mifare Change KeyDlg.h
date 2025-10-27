// Mifare Change KeyDlg.h : header file
//

#if !defined(AFX_MIFARECHANGEKEYDLG_H__115C10B4_DD59_4CF0_9472_18CB81524392__INCLUDED_)
#define AFX_MIFARECHANGEKEYDLG_H__115C10B4_DD59_4CF0_9472_18CB81524392__INCLUDED_

#include "AcsReader.h" // Added by ClassView
#include "MifareClassic.h" // Added by ClassView
#include "CardSelector.h"
#include "NoPasteEdit.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMifareChangeKeyDlg dialog

class CMifareChangeKeyDlg : public CDialog
{
// Construction
public:
	CMifareChangeKeyDlg(CWnd* pParent = NULL);	// standard constructor
	void displayMessage(CString message);

	void resetFields();
	
	void validateInput(CEdit *editbox, CString *text);
	

// Dialog Data
	//{{AFX_DATA(CMifareChangeKeyDlg)
	enum { IDD = IDD_MIFARECHANGEKEY_DIALOG };
	CComboBox	ComboBoxReaderNames;
	CButton ButtonInitialize;
	CButton ButtonConnect;
	CButton ButtonLoadKey;
	CButton ButtonAuthenticate;
	CButton ButtonRead;
	CButton ButtonUpdate;
	CButton ButtonClearLogs;
	CButton ButtonReset;
	CButton ButtonQuit;
	CButton RadiobuttonKeyTypeKeyA;
	CButton RadiobuttonKeyTypeKeyB;
	CNoPasteEdit EditboxLoadKeyStoreNumber;	 
	CNoPasteEdit EditboxSectorNumber;
	CNoPasteEdit EditboxAuthenticateKeyStoreNumber;
	CNoPasteEdit EditboxCurrentSector;
	CNoPasteEdit EditboxSectorTrailerBlock;
	CNoPasteEdit EditboxKeyA;
	CString EditboxKeyAString;
	CNoPasteEdit EditboxAccessBits;
	CString EditboxAccessBitsString;
	CNoPasteEdit EditboxKeyB;
	CString EditboxKeyBString;
	CNoPasteEdit LabelValidSectors;
	CNoPasteEdit EditboxApduLogs;
	CString	EditboxApduLogsText;
	CNoPasteEdit	TextBoxKeyVal6;
	CString TextBoxValString6;
	CNoPasteEdit	TextBoxKeyVal5;
	CString TextBoxValString5;
	CNoPasteEdit	TextBoxKeyVal4;
	CString TextBoxValString4;
	CNoPasteEdit	TextBoxKeyVal3;
	CString TextBoxValString3;
	CNoPasteEdit	TextBoxKeyVal1;
	CString TextBoxValString1;
	CNoPasteEdit	TextBoxKeyVal2;
	CString TextBoxValString2;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMifareChangeKeyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMifareChangeKeyDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonClear();
	afx_msg void OnButtonQuit();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonInitialize();
	afx_msg void OnButtonConnect();
	afx_msg void OnButtonLoadKey();
	afx_msg void OnButtonAuthenticate();
	afx_msg void OnButtonRead();
	afx_msg void OnButtonUpdate();
	afx_msg void OnChangeEditBoxKeys();
	afx_msg void OnChangeEditBoxKeyA();
	afx_msg void OnChangeEditBoxAccessBits();
	afx_msg void OnChangeEditBoxKeyB();
	afx_msg void OnEditchangeComboBoxPort();
	afx_msg void OnEditchangeComboBoxSlot();
	afx_msg void OnChangeEditBoxKeyStore1();
	afx_msg void OnChangeEditBoxKeyStore2();
	afx_msg void OnChangeEditBoxKeyStore3();
	afx_msg void OnChangeEditBoxKeyStore4();
	afx_msg void OnChangeEditBoxKeyStore5();
	afx_msg void OnChangeEditBoxKeyStore6();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	PcscReader		  * pcscReader;
	MifareClassic	  * pMifare_;
	
	long returnCode;
	byte currentSector, currentSectorTrailer;

private:
	void enableLoadGroup(bool bEnable);
	void enableAuthenticateGroup(bool bEnable);
	void enableChangeKeyGroup(bool bEnable);
	void addTitleToLog(CString sMessage);
	void addMessageToLog(CString sMessage);
	void showErrorMessage(CString sMessage);
	void showWarningMessage(CString sMessage);
	void showInformationMessage(CString sMessage);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIFARECHANGEKEYDLG_H__115C10B4_DD59_4CF0_9472_18CB81524392__INCLUDED_)
