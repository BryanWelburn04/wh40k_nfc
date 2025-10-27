// Binary Files in ACOS3Dlg.h : header file
//

#if !defined(AFX_BINARYFILESINACOS3DLG_H__B2D4B333_ED89_41A3_989F_58046F3880BB__INCLUDED_)
#define AFX_BINARYFILESINACOS3DLG_H__B2D4B333_ED89_41A3_989F_58046F3880BB__INCLUDED_

#include "Acos3.h"
#include "PcscReader.h"
#include "PcscException.h"
#include "CardException.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CBinaryFilesinACOS3Dlg dialog

class CBinaryFilesinACOS3Dlg : public CDialog
{
// Construction
public:
	CBinaryFilesinACOS3Dlg(CWnd* pParent = NULL);	// standard constructor
	void displayMessage(CString message);
	void displayTitle(CString title);
	void isAcos3Combi();
	void enableControls(BOOL state);
	void enableReadWriteBlock(bool bEnable);
	void resetFields();

// Dialog Data
	//{{AFX_DATA(CBinaryFilesinACOS3Dlg)
	enum { IDD = IDD_BINARYFILESINACOS3_DIALOG };
	CEdit	EditBoxLogs;
	CEdit	TextBoxReadWriteFileId2;
	CEdit	TextBoxReadWriteFileId1;
	CButton	ButtonFormat;
	CButton	ButtonConnect;
	CButton	ButtonInitialize;
	CButton	ButtonWrite;
	CButton	ButtonRead;
	CButton	ButtonQuit;
	CButton	ButtonReset;
	CButton	ButtonClear;
	CEdit	TextBoxLength;
	CEdit	TextBoxData;
	CEdit	TextBoxOffset1;
	CEdit	TextBoxOffset2;
	CEdit	TextBoxLen1;
	CEdit	TextBoxLen2;
	CEdit	TextBoxFileID1;
	CEdit	TextBoxFileID2;
	CComboBox	ComboBoxReader;
	CString	EditBoxLogsValue;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBinaryFilesinACOS3Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON 	m_hIconBig,
			m_hIconSmall;

	// Generated message map functions
	//{{AFX_MSG(CBinaryFilesinACOS3Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonInitialize();
	afx_msg void OnButtonConnect();
	afx_msg void OnButtonClear();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonQuit();
	afx_msg void OnButtonFormat();
	afx_msg void OnButtonRead();
	afx_msg void OnButtonWrite();
	afx_msg void OnChangeEdit9();
	afx_msg void OnKillfocusEdit9();
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

#endif // !defined(AFX_BINARYFILESINACOS3DLG_H__B2D4B333_ED89_41A3_989F_58046F3880BB__INCLUDED_)
