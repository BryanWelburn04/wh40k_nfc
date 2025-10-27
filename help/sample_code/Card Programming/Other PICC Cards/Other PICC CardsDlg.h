// Other PICC CardsDlg.h : header file
//

#if !defined(AFX_OTHERPICCCARDSDLG_H__8089C9E2_CE69_4243_A77E_BE94269FD9D2__INCLUDED_)
#define AFX_OTHERPICCCARDSDLG_H__8089C9E2_CE69_4243_A77E_BE94269FD9D2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PICCClass.h"
#include "PcscException.h"
#include "CardException.h"
#include "Helper.h"

/////////////////////////////////////////////////////////////////////////////
// Define constants
/////////////////////////////////////////////////////////////////////////////
#define BLACK RGB(0, 0, 0)
#define RED RGB(255, 0, 0)
#define GREEN RGB(0, 0x99, 0)
#define MAX 262
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// COtherPICCCardsDlg dialog
/////////////////////////////////////////////////////////////////////////////
class COtherPICCCardsDlg : public CDialog
{
public:
	// Constructor
	COtherPICCCardsDlg(CWnd* pParent = NULL); // Standard constructor
	~COtherPICCCardsDlg(); // Destructor

	// Instance
	PICCClass		*pPICCClass_;
	Helper			helper_;

	// Dialog Data
	//{{AFX_DATA(COtherPICCCardsDlg)
	enum { IDD = IDD_OTHERPICCCARDS_DIALOG };
	CRichEditCtrl	rbResult;
	CComboBox		cbReader;
	CButton			btnQuit;
	CButton			btnReset;
	CButton			btnClear;
	CButton			btnSendCmd;
	CButton			check1;
	CButton			btnGetData;
	CButton			btnConnect;
	CButton			btnInit;
	CString			strTbData;
	CEdit			tbData;
	CEdit			tbLe;
	CEdit			tbLc;
	CEdit			tbP2;
	CEdit			tbP1;
	CEdit			tbINS;
	CEdit			tbCLA;	
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COtherPICCCardsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
	//}}AFX_VIRTUAL

protected:
	// Implementation
	HICON		 	m_hIconBig,
					m_hIconSmall;

	// Generated message map functions
	//{{AFX_MSG(COtherPICCCardsDlg)
	virtual BOOL	OnInitDialog();
	afx_msg void	OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void	OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void	OnInit();
	afx_msg void	OnConnect();
	afx_msg void	OnClear();
	afx_msg void	OnReset();
	afx_msg void	OnQuit();
	afx_msg void	OnGetData();
	afx_msg void	OnSendCmd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	// Other Functions
	void			resetApplication();
public:
	void			displayOut( int iType, CString str, COLORREF color );
	void			displayOut( bool isSpace, CString str, COLORREF color );
};

/////////////////////////////////////////////////////////////////////////////
// Delegates
/////////////////////////////////////////////////////////////////////////////
void sendCommandFunctor(const byte *pData, const int iLength);
void recvCommandFunctor(const byte *pData, const int iLength);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OTHERPICCCARDSDLG_H__8089C9E2_CE69_4243_A77E_BE94269FD9D2__INCLUDED_)
