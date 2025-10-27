#if !defined(AFX_DEVICEPOLLINGCONFIGURATIONDLG_H__1BB20755_1929_40A7_A5F3_449F471010C4__INCLUDED_)
#define AFX_DEVICEPOLLINGCONFIGURATIONDLG_H__1BB20755_1929_40A7_A5F3_449F471010C4__INCLUDED_

#include "PcscReader.h"
#include "PcscException.h"
#include "ValidationException.h"
#include "Helper.h"
#include "Acr122u.h"
#include <exception>

#if _MSC_VER > 1000
#pragma once
#endif

class CDevicePollingConfigurationDlg : public CDialog
{

public:
	CDevicePollingConfigurationDlg(CWnd* pParent = NULL);
	~CDevicePollingConfigurationDlg();

	void addMessageToLog(CString sMessage);

// Dialog Data
	//{{AFX_DATA(CDevicePollingConfigurationDlg)
	enum { IDD = IDD_DEVICEPOLLINGCONFIGURATION_DIALOG };
	CButton	m_RadioButton500ms;
	CButton	m_RadioButton250ms;
	CButton	m_GroupBoxPollingInterval;
	CButton	m_CheckBoxDetectTopaz;
	CButton	m_CheckBoxDetectIso14443TypeB;
	CButton	m_CheckBoxDetectIso14443TypeA;
	CButton	m_CheckBoxDetectFelica424;
	CButton	m_CheckBoxDetectFelica212;
	CButton	m_CheckBoxAutoPiccPolling;
	CButton	m_CheckBoxAutoAtsGeneration;
	CButton	m_GroupBoxOperatingParameters;
	CEdit	m_EditBoxApduLogs;
	CComboBox	m_ComboBoxReaderList;
	CButton	m_ButtonSetParameters;
	CButton	m_ButtonReset;
	CButton	m_ButtonConnect;
	CButton	m_ButtonClear;
	CButton	m_ButtonGetParameters;
	CButton	m_ButtonQuit;
	CButton	m_ButtonInitialize;
	CString	m_EditBoxApduLogsText;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDevicePollingConfigurationDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDevicePollingConfigurationDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void onButtonClear();
	afx_msg void onButtonInitialize();
	afx_msg void onButtonQuit();
	afx_msg void onButtonConnect();
	afx_msg void onRadioButton250ms();
	afx_msg void onRadioButton500ms();
	afx_msg void onButtonReset();
	afx_msg void onButtonGetParameters();
	afx_msg void onButtonSetParameters();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	PcscReader *pPcscReader_;
	Acr122u *pAcr122u_;
	char aMessage_[100];

	void resetControl();
	void enableControls(bool bIsEnabled);

	void onPcscException(PcscException cException);
	void onStandardException(const std::exception& cException);
	void onGeneralException();
	void onInitializeException();

	void displayReaderListToComboBox(string* pList, int32 iListSize);
	void validateSelectedReaderName(CString sReaderName);

	void uncheckAllCheckBox();
	void updateControlState(PiccOperatingParameter *pPiccOperatingParameter);
	void select250msInterval();
	void select500msInterval();

	void configureParameters(PiccOperatingParameter *pPiccOperatingParameter);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEVICEPOLLINGCONFIGURATIONDLG_H__1BB20755_1929_40A7_A5F3_449F471010C4__INCLUDED_)
