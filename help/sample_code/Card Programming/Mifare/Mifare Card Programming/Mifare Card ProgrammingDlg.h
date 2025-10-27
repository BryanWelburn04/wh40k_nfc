// Mifare Card ProgrammingDlg.h : header file
//

#if !defined(AFX_MIFARECARDPROGRAMMINGDLG_H__325CCA34_7013_41E3_B561_872715E8499A__INCLUDED_)
#define AFX_MIFARECARDPROGRAMMINGDLG_H__325CCA34_7013_41E3_B561_872715E8499A__INCLUDED_

#include "AcsReader.h" // Added by ClassView
#include "MifareClassic.h" // Added by ClassView
#include "CardSelector.h"
#include "NoPasteEdit.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMifareCardProgrammingDlg dialog

class CMifareCardProgrammingDlg : public CDialog
{
// Construction
public:
	void resetFields();
	void displayMessage(CString message);
	void validateInput(CEdit *editbox, CString *text);
	void enableLoadControls(bool enable);
	void enableAuthenticationControls(bool enable);
	void enableBlockFunctionControls(bool enable);
	afx_msg void OnButtonInitialize();
	CMifareCardProgrammingDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMifareCardProgrammingDlg)
	enum { IDD = IDD_MIFARECARDPROGRAMMING_DIALOG };
	CNoPasteEdit	EditboxResult;
	CButton	ButtonConnect;
	CButton	ButtonInitialize;
	CButton	ButtonRestoreVal;
	CButton	ButtonReadVal;
	CButton	ButtonDec;
	CButton	ButtonInc;
	CButton	ButtonStoreVal;	
	CButton	ButtonLoadKey;
	CButton	ButtonAuthenticate;
	CButton	ButtonUpdate;
	CButton	ButtonRead;
	CButton	ButtonQuit;
	CButton	ButtonReset;
	CButton	ButtonClear;
	CButton	RadioButtonKeyB;
	CButton	RadioButtonKeyA;
	CNoPasteEdit	TextBoxTarget;
	CNoPasteEdit	TextBoxSource;
	CNoPasteEdit	TextBoxBlock;
	CNoPasteEdit	TextBoxValue;	
	CNoPasteEdit	TextBoxData;
	CNoPasteEdit	TextBoxLen;
	CNoPasteEdit	TextBoxStartBlock;	
	CNoPasteEdit	TextBoxKeyStoreNo;
	CNoPasteEdit	TextBoxBlockNo;	
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
	CNoPasteEdit	TextBoxKeyStore;	
	CComboBox	ComboboxReader;
	CString		EditboxResultText;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMifareCardProgrammingDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON 	m_hIconBig,
			m_hIconSmall;

	// Generated message map functions
	//{{AFX_MSG(CMifareCardProgrammingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnInit();
	afx_msg void OnConnect();
	afx_msg void OnClear();
	afx_msg void OnReset();
	afx_msg void OnQuit();
	afx_msg void OnKeyA();
	afx_msg void OnKeyB();
	afx_msg void OnLoadKey();
	afx_msg void OnAuthen();
	afx_msg void OnReadBlock();
	afx_msg void OnUpdateBlock();
	afx_msg void OnStoreVal();
	afx_msg void OnInc();
	afx_msg void OnDec();
	afx_msg void OnReadVal();
	afx_msg void OnRestore();
	afx_msg void OnKillfocusEdit17();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	PcscReader		  * pcscReader;	
	MifareClassic	  * pMifare_;
	CString			  sCardName;

//TODOBREY	CHIP_TYPE currentChipType; 
	long returnCode;
	byte currentSector, currentSectorTrailer;
	
public:
	afx_msg void OnChangeEdit2();
	afx_msg void OnChangeEdit3();
	afx_msg void OnChangeEdit4();
	afx_msg void OnChangeEdit5();
	afx_msg void OnChangeEdit6();
	afx_msg void OnChangeEdit7();
	afx_msg void OnChangeEdit17();
//	afx_msg void OnEnChangeEdit17();

private:
	void addTitleToLog(CString sMessage);
	void addMessageToLog(CString sMessage);
	void showErrorMessage(CString sMessage);
	void showWarningMessage(CString sMessage);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIFARECHANGEKEYDLG_H__115C10B4_DD59_4CF0_9472_18CB81524392__INCLUDED_)
