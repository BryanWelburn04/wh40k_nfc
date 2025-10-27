// MifareNDEFSampleCodeDlg.h : header file
//

#include "MyTabCtrl.h"
#include "NDEF.h"
#include "NdefSmartPoster.h"

#include "AcsReader.h" // Added by ClassView
#include "MifareClassic.h" // Added by ClassView
#include "CardSelector.h"
#include "Helper.h"
#include <vector>

#if !defined(AFX_MIFARENDEFSAMPLECODEDLG_H__CB649930_7FB0_4C74_97E8_9D151D14D763__INCLUDED_)
#define AFX_MIFARENDEFSAMPLECODEDLG_H__CB649930_7FB0_4C74_97E8_9D151D14D763__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// Define constants
/////////////////////////////////////////////////////////////////////////////
#define BLACK RGB(0, 0, 0)
#define KEY_LENGTH_STRING 12
#define KEY_LENGTH 6
#define ACCESS_BITS_LENGTH 4
#define DATA_LENGTH 255
#define MIFARE_WITH_4_BLOCKS_SECTOR_COUNT 32
#define MIFARE_4K_START_SECTOR_WITH_15_BLOCKS 32
#define MAD_SECTOR2_LENGTH 48
#define BLOCK_DATA_LENGTH 16
#define MIFARE_WITH_4_BLOCKS_TOTAL_COUNT 127 
#define MIFARE_4K_START_WITH_15_BLOCKS 128
#define MIFARE_4K_TOTAL_SECTOR 38
#define MIFARE_1K_TOTAL_SECTOR 35
#define MAD2_SECTOR_TRAILER_BLOCK 67
#define MIFARE_4K_EXTENDED_USER_DATA_COUNT 15
#define MIFARE_USER_DATA_COUNT_PER_SECTOR 3
#define MIFARE_1K_SECTOR_COUNT 15
#define MIFARE_4K_SECTOR_COUNT 39
#define MIFARE_4K_SECTOR_WITH_4_BLOCKS 31
#define AIDS_COUNT 15  
#define MAD2_SECTOR_START 64
#define MAD2_SECTOR_LAST 68
#define BLOCK_COUNT 4
#define COPY_LENGTH 2
#define BLOCK_LENGTH 16.00
#define MIFARE_4K_EXTENDED_USER_DATA_COUNT_DOUBLE 15.00
#define MIFARE_USER_BLOCK 3.00
#define MAD_SECTOR2_ADDRESS 16
#define THREE_BYTE_MESSAGE_TAG 4
#define TWO_BYTE_MESSAGE_TAG 2
#define MAD_SECTORS_COUNT 2
#define KEY_B_WITH_SPACE 17
#define KEY_B_WITHOUT_SPACE 12
#define DELAY_FOR_ACR122 2000
#define MAX_SIZE_OF_ARRAY 1200
/////////////////////////////////////////////////////////////////////////////

enum TEXT_TYPE
{
	MSG_TO_LOGS = 0,
	TITLE_TO_LOGS = 1,
	ERR_TO_LOGS = 2
};

/////////////////////////////////////////////////////////////////////////////
// CMifareNDEFSampleCodeDlg dialog

class CMifareNDEFSampleCodeDlg : public CDialog
{
// Construction
public:
	CMifareNDEFSampleCodeDlg(CWnd* pParent = NULL);	// standard constructor
	void displayOut(int iType, CString sStr, COLORREF color);
	void displayOut(CString sStr, TEXT_TYPE eTextType);
	void displayOutReceive( CString sStr, COLORREF color);
	void addMsgToLog(CString sStr);
	void addTitleToLog(CString sStr);
	void addErrMsgToLog(CString sStr);
	void addMsgToReceive(CString sStr);

	int iRecordCount;

// Dialog Data
	//{{AFX_DATA(CMifareNDEFSampleCodeDlg)
	enum { IDD = IDD_DialogMifareNdefSampleCode };
	MyTabCtrl m_TabControlMain;
	CEdit m_EditBoxNfcKeyB;
	CEdit m_EditBoxNfcKeyA;
	CEdit m_EditBoxNfcAccessBits;
	CEdit m_EditBoxNdefMessage;
	CEdit m_EditBoxMadKeyB;
	CEdit m_EditBoxMadKeyA;
	CEdit m_EditBoxMadAccessBits;
	CEdit m_EditBoxApduLogs;
	CComboBox m_ComboBoxReader;
	CButton	m_ButtonWriteToCard;
	CButton	m_ButtonResetCard;
	CButton	m_ButtonReset;
	CButton	m_ButtonReadFromCard;
	CButton	m_ButtonQuit;
	CButton	m_ButtonInitialize;
	CButton	m_ButtonConnect;
	CButton	m_ButtonClearLogs;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMifareNDEFSampleCodeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMifareNDEFSampleCodeDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonInitialize();
	afx_msg void OnButtonConnect();
	afx_msg void OnButtonWritetocard();
	afx_msg void OnButtonReadfromcard();
	afx_msg void OnButtonResetcard();
	afx_msg void OnButtonClearlogs();
	afx_msg void OnButtonReset();
	afx_msg void OnButtonQuit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	PcscReader		  *pPcscReader_;	
	MifareClassic	  *pMifare_;
	Ndef			   cNdef_;
	
public:
	vector<byte> uSnepMessage;

private:
	void resetWindow(bool bEnable);
	void enableTab(bool bEnable);
	void enableCardButtons(bool bEnable);
	void resetCard();
	void writeToCard();
	void readFromCard();
public:
	afx_msg void OnEnChangeEditMadkeyb();
	afx_msg void OnEnChangeEditNfckeyb();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MIFARENDEFSAMPLECODEDLG_H__CB649930_7FB0_4C74_97E8_9D151D14D763__INCLUDED_)
