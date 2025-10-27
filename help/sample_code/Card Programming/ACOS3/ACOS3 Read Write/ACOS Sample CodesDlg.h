// ACOS Sample CodesDlg.h : header file
//

#if !defined(AFX_ACOSSAMPLECODESDLG_H__2A68B151_D788_40D9_968F_39D05779568D__INCLUDED_)
#define AFX_ACOSSAMPLECODESDLG_H__2A68B151_D788_40D9_968F_39D05779568D__INCLUDED_

#include "Acos3.h"
#include "PcscReader.h"
#include "PcscException.h"
#include "CardException.h"
#include "CardSelector.h"

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
	void resetFields();
	long loadReadersListToControl();
	void displayMessage(CString msg);
	void clearBuffers();

// Dialog Data
	//{{AFX_DATA(CACOSSampleCodesDlg)
	enum { IDD = IDD_ACOSSAMPLECODES_DIALOG };
	CButton	ButtonConnect;
	CButton	ButtonFormat;
	CButton	ButtonRead;
	CButton	ButtonWrite;
	CButton	ButtonInitialize;
	CButton	RadioButtonAA11;
	CButton	RadioButtonBB22;	
	CButton	RadioButtonCC33;
	CButton	ButtonReset;
	CButton	ButtonQuit;	
	CComboBox	ComboBoxReader;
	CString     EditBoxResultText;	
	CEdit	EditBoxResult;
	CEdit	TextBoxData;	
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
	afx_msg void OnButtonConnect();
	afx_msg void OnButtonFormat();
	afx_msg void OnButtonRead();
	afx_msg void OnButtonWrite();
	afx_msg void OnButtonQuit();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonClear();
	afx_msg void OnButtonInitialize();
	afx_msg void OnRadioButtonAA11();
	afx_msg void OnRadioButtonBB22();
	afx_msg void OnRadioButtonCC33();
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
