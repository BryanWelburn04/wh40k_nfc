/////////////////////////////////////////////////////////////////////////////
//
// Company	: ADVANCED CARD SYSTEMS, LTD.
//
// Name		: SimplePCSCDlg.cpp : implementation file
//
// Author	: Alcendor Lorzano Chan
//
// Date	: 14 / 09 / 2001 
//
// Revision Trail: Date/Author/Description
// 06September2005/J.I.R. Mission/ Allows APDU input
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimplePCSC.h"
#include "SimplePCSCDlg.h"

#include "PcscReader.h"
#include "PcscException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DISPLAY_TYPE_NOTIFICATION	0
#define DISPLAY_TYPE_ERROR			1
#define DISPLAY_TYPE_INPUT_DATA		2
#define DISPLAY_TYPE_OUTPUT_DATA	3
#define DISPLAY_TYPE_ATR			4

// Global variables
	PcscReader	_cPcscReader;
	uint8					_aSendBuff[262];
	uint8					_aRecvBuff[262];	
	CSimplePCSCDlg			*_pThis = NULL;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimplePCSCDlg dialog

CSimplePCSCDlg::CSimplePCSCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSimplePCSCDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSimplePCSCDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIconBig   = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIconSmall = AfxGetApp()->LoadIcon(IDR_SMALLICON);
}

void CSimplePCSCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSimplePCSCDlg)
	DDX_Control(pDX, IDC_BTNTransmit, bTransmit);
	DDX_Control(pDX, IDC_BTNStatus, bStatus);
	DDX_Control(pDX, IDC_BTNReleaseContext, bReleaseContext);
	DDX_Control(pDX, IDC_BTNListReaders, bListReaders);
	DDX_Control(pDX, IDC_BTNEstablishContext, bEstContext);
	DDX_Control(pDX, IDC_BTNEndTransaction, bEndTransaction);
	DDX_Control(pDX, IDC_BTNDisconnect, bDisconnect);
	DDX_Control(pDX, IDC_BTNConnect, bConnect);
	DDX_Control(pDX, IDC_BTNBeginTransaction, bBeginTransaction);
	DDX_Control(pDX, IDC_COMBOREADER, cbReader);
	DDX_Control(pDX, IDC_EDIT1, tDataIn);	
	DDX_Control(pDX, IDC_EDIT2, m_TextBoxLogsVar);
	DDX_Text(pDX, IDC_EDIT2, m_TextBoxLogs);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSimplePCSCDlg, CDialog)
	//{{AFX_MSG_MAP(CSimplePCSCDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTNExit, OnBTNExit)
	ON_BN_CLICKED(IDC_BTNEstablishContext, OnBTNEstablishContext)
	ON_BN_CLICKED(IDC_BTNReleaseContext, OnBTNReleaseContext)
	ON_BN_CLICKED(IDC_BTNListReaders, OnBTNListReaders)
	ON_BN_CLICKED(IDC_BTNConnect, OnBTNConnect)
	ON_BN_CLICKED(IDC_BTNDisconnect, OnBTNDisconnect)
	ON_BN_CLICKED(IDC_BTNStatus, OnBTNStatus)
	ON_BN_CLICKED(IDC_BTNBeginTransaction, OnBTNBeginTransaction)
	ON_BN_CLICKED(IDC_BTNEndTransaction, OnBTNEndTransaction)
	ON_BN_CLICKED(IDC_BTNTransmit, OnBTNTransmit)
	ON_BN_CLICKED(IDC_BTNClear, OnBTNClear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ==========================================================================
// SimplePCSCDlg internal routines
void InitButtons()
{
	_pThis->bListReaders.EnableWindow(FALSE);
	_pThis->bConnect.EnableWindow(FALSE);
	_pThis->bBeginTransaction.EnableWindow(FALSE);
	_pThis->bStatus.EnableWindow(FALSE);
	_pThis->bTransmit.EnableWindow(FALSE);
	_pThis->bEndTransaction.EnableWindow(FALSE);
	_pThis->bDisconnect.EnableWindow(FALSE);
	_pThis->bReleaseContext.EnableWindow(FALSE);
}

void DisableAPDUInput()
{
	_pThis->tDataIn.SetWindowText("");
	_pThis->tDataIn.EnableWindow(FALSE);
}

void DisplayOut(int errType, int retVal, CString prtText)
{
	char buffer[300];

	switch (errType)
	{
		case DISPLAY_TYPE_NOTIFICATION:
			_pThis->m_TextBoxLogs = _pThis->m_TextBoxLogs + prtText + "\r\n";
			break;
		case DISPLAY_TYPE_ERROR:
			sprintf(buffer, "%s%d%s %s", "[", retVal, "]", prtText);
			_pThis->m_TextBoxLogs = _pThis->m_TextBoxLogs + buffer + "\r\n";
			break;
		case DISPLAY_TYPE_INPUT_DATA:
			sprintf(buffer, "<< %s", prtText);
			_pThis->m_TextBoxLogs = _pThis->m_TextBoxLogs + buffer + "\r\n";			
			break;
		case DISPLAY_TYPE_OUTPUT_DATA:
			sprintf(buffer, ">> %s", prtText);			
			_pThis->m_TextBoxLogs = _pThis->m_TextBoxLogs + buffer + "\r\n";
			break;
		case DISPLAY_TYPE_ATR:
			sprintf(buffer, "ATR Value: %s", prtText);			
			_pThis->m_TextBoxLogs = _pThis->m_TextBoxLogs + buffer + "\r\n";
			break;
	}

	_pThis->UpdateData(false);
	_pThis->m_TextBoxLogsVar.SetSel(0xffff, 0xffff);
}

//MessageBoxes
void CSimplePCSCDlg::showInformationMessage(CString sMessage)
{
	::MessageBox(0, sMessage, "Mifare NDEF Sample Code", MB_OK|MB_ICONINFORMATION);
}

void CSimplePCSCDlg::showWarningMessage(CString sMessage)
{
	::MessageBox(0, sMessage, "Mifare NDEF Sample Code", MB_OK|MB_ICONEXCLAMATION);
}

void CSimplePCSCDlg::showErrorMessage(CString sMessage)
{
	::MessageBox(0, sMessage, "Error", MB_OK|MB_ICONSTOP);
}

/////////////////////////////////////////////////////////////////////////////
// CSimplePCSCDlg message handlers

BOOL CSimplePCSCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIconBig,	TRUE);		// Set big icon
	SetIcon(m_hIconSmall,	FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	_pThis = this;
	DisableAPDUInput();
	InitButtons();

	DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "Program Ready");
	DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSimplePCSCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSimplePCSCDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIconSmall);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSimplePCSCDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIconSmall;
}

void CSimplePCSCDlg::OnBTNExit() 
{	
	try
	{
		_cPcscReader.endTransaction();
		_cPcscReader.disconnect();
		_cPcscReader.cleanup();
		
	}
	catch(AcsReaderException cException)
	{
	
	}
	CDialog::OnOK();
}


void CSimplePCSCDlg::OnBTNEstablishContext() 
{
	char aMessage[100];

	try
	{
		_cPcscReader.initialize();
		
		DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "SCardEstablishContext() success");
		DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "");
		bEstContext.EnableWindow(FALSE);
		bListReaders.EnableWindow(TRUE);
		bReleaseContext.EnableWindow(TRUE);
	}
	catch(AcsReaderException cException)
	{
		sprintf(aMessage, "%s%d%s %s", "[", _cPcscReader.getReturnCode(),"]", cException.what());
		showErrorMessage(aMessage);

		DisplayOut(DISPLAY_TYPE_ERROR, _cPcscReader.getReturnCode(), cException.what());
		DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "");
	}
}

void CSimplePCSCDlg::OnBTNReleaseContext() 
{
	char aMessage[100];

	try
	{
		_cPcscReader.cleanup();
		
		DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "SCardReleaseContext() success");
		DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "");

		bEstContext.EnableWindow(TRUE);
		bListReaders.EnableWindow(FALSE);
		bConnect.EnableWindow(FALSE);
		bBeginTransaction.EnableWindow(FALSE);
		bStatus.EnableWindow(FALSE);
		bTransmit.EnableWindow(FALSE);
		bEndTransaction.EnableWindow(FALSE);
		bDisconnect.EnableWindow(FALSE);
		bReleaseContext.EnableWindow(FALSE);
		DisableAPDUInput();
		cbReader.ResetContent();
		cbReader.EnableWindow(FALSE);
	}
	catch(AcsReaderException cException)
	{
		sprintf(aMessage, "%s%d%s %s", "[", _cPcscReader.getReturnCode(),"]", cException.what());
		showErrorMessage(aMessage);

		DisplayOut(DISPLAY_TYPE_ERROR, _cPcscReader.getReturnCode(), cException.what());
		DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "");
	}
}

void CSimplePCSCDlg::OnBTNListReaders() 
{
	PcscReaderName *pReaderList = NULL;
	uint32 uNumReaders;
	uint32 uIdx;
	char aMessage[100];

	try
	{
		_cPcscReader.listReaders(&pReaderList, uNumReaders);
		if (uNumReaders == 0 || pReaderList == NULL)
		{
			DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "No PC/SC reader is detected in your system.");
    		return;
		}

		// Populate Reader List
		cbReader.ResetContent();
		for(uIdx = 0; uIdx < uNumReaders; uIdx++)
		{
			cbReader.AddString(pReaderList[uIdx].getNameBuffer());
		}

		cbReader.EnableWindow(TRUE);
		cbReader.SetCurSel(0);
		DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "SCardListReaders() success");
		DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "");
		bConnect.EnableWindow(TRUE);
	}
	catch(AcsReaderException cException)
	{
		sprintf(aMessage, "%s%d%s %s", "[", _cPcscReader.getReturnCode(),"]", cException.what());
		showErrorMessage(aMessage);

		DisplayOut(DISPLAY_TYPE_ERROR, _cPcscReader.getReturnCode(), cException.what());
		DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "");
	}

	if (pReaderList != NULL)
	{
		delete [] pReaderList;
	}
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CSimplePCSCDlg::OnBTNConnect() 
{
	int8  aReaderNameName[256];
	int32 iIdx = cbReader.GetCurSel();
	char aMessage[100];

	if (iIdx == -1)
	{
		DisplayOut(DISPLAY_TYPE_ERROR, 0, "No Reader Selected");
        return;
	}

	cbReader.GetLBText(iIdx, aReaderNameName);

	try 
	{
		_cPcscReader.connect(aReaderNameName);

		DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "SCardConnect() success");
		DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "");
		bListReaders.EnableWindow(FALSE);
		bConnect.EnableWindow(FALSE);
		bBeginTransaction.EnableWindow(TRUE);
		bDisconnect.EnableWindow(TRUE);
		bReleaseContext.EnableWindow(TRUE);
	}
	catch(AcsReaderException cException)
	{
		sprintf(aMessage, "%s%d%s %s", "[", _cPcscReader.getReturnCode(),"]", cException.what());
		showErrorMessage(aMessage);

		DisplayOut(DISPLAY_TYPE_ERROR, _cPcscReader.getReturnCode(), cException.what());
		DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "");
	}
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CSimplePCSCDlg::OnBTNDisconnect() 
{
	char aMessage[100];

	try 
	{
		_cPcscReader.disconnect();

		DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "SCardDisconnect() success");
		DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "");

		bListReaders.EnableWindow(TRUE);
		bConnect.EnableWindow(TRUE);
		bBeginTransaction.EnableWindow(FALSE);
		bDisconnect.EnableWindow(FALSE);
		bReleaseContext.EnableWindow(TRUE);		
	}
	catch(AcsReaderException cException)
	{
		sprintf(aMessage, "%s%d%s %s", "[", _cPcscReader.getReturnCode(),"]", cException.what());
		showErrorMessage(aMessage);

		DisplayOut(DISPLAY_TYPE_ERROR, _cPcscReader.getReturnCode(), cException.what());
		DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "");
	}
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CSimplePCSCDlg::OnBTNStatus() 
{
	uint8 aATRVal[40];
	uint32 uATRLen = 40;
	uint32 uIdx;
	char aTmpArray[150];
	uint32 uActiveProtocol;
	uint32 uCurrentState;
	char aMessage[100];
	
	try
	{
		_cPcscReader.getAtr(aATRVal,uATRLen);	
		
		DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "SCardStatus() success");

		memset(aTmpArray,0,sizeof(aTmpArray));
		for (uIdx = 0;uIdx < uATRLen; uIdx++)
		{
		   sprintf(aTmpArray, "%s%02X ", aTmpArray, aATRVal[uIdx]);		   
		}
		DisplayOut(DISPLAY_TYPE_ATR, 0, aTmpArray);

		uActiveProtocol = _cPcscReader.getActiveProtocol();
		uCurrentState	= _cPcscReader.getCurrenState();

		switch(uCurrentState)
		{
			case SCARD_UNKNOWN     : m_TextBoxLogs = m_TextBoxLogs + "State : SCARD_UNKNOWN" + "\r\n"; break;
			case SCARD_ABSENT      : m_TextBoxLogs = m_TextBoxLogs + "State : SCARD_ABSENT" + "\r\n";break;
			case SCARD_PRESENT     : m_TextBoxLogs = m_TextBoxLogs + "State : SCARD_PRESENT" + "\r\n";break;
			case SCARD_SWALLOWED   : m_TextBoxLogs = m_TextBoxLogs + "State : SCARD_SWALLOWED" + "\r\n";break;
			case SCARD_POWERED     : m_TextBoxLogs = m_TextBoxLogs + "State : SCARD_POWERED" + "\r\n";break;
			case SCARD_NEGOTIABLE  : m_TextBoxLogs = m_TextBoxLogs + "State : SCARD_NEGOTIABLE" + "\r\n";break;
			case SCARD_SPECIFIC    : m_TextBoxLogs = m_TextBoxLogs + "State : SCARD_SPECIFIC" + "\r\n";break;
		}; // switch case 	
			
		switch(uActiveProtocol)
		{
			case SCARD_PROTOCOL_UNDEFINED : m_TextBoxLogs = m_TextBoxLogs + "Protocol : SCARD_PROTOCOL_UNDEFINED" + "\r\n"; break;
			case SCARD_PROTOCOL_T0        : m_TextBoxLogs = m_TextBoxLogs + "Protocol : SCARD_PROTOCOL_T0" + "\r\n"; break;		
			case SCARD_PROTOCOL_T1        : m_TextBoxLogs = m_TextBoxLogs + "Protocol : SCARD_PROTOCOL_T1" + "\r\n"; break;			
			case SCARD_PROTOCOL_RAW       : m_TextBoxLogs = m_TextBoxLogs + "Protocol : SCARD_PROTOCOL_RAW" + "\r\n"; break;			
			case SCARD_PROTOCOL_DEFAULT   : m_TextBoxLogs = m_TextBoxLogs + "Protocol : SCARD_PROTOCOL_DEFAULT" + "\r\n"; break;			
		}; // switch case

		DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "");

		UpdateData(false);
		m_TextBoxLogsVar.SetSel(0xffff, 0xffff);
	}
	catch(AcsReaderException cException)
	{
		sprintf(aMessage, "%s%d%s %s", "[", _cPcscReader.getReturnCode(),"]", cException.what());
		showErrorMessage(aMessage);

		DisplayOut(DISPLAY_TYPE_ERROR, _cPcscReader.getReturnCode(), cException.what());
		DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "");
	}
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CSimplePCSCDlg::OnBTNBeginTransaction() 
{
	char aMessage[100];

	try
	{
		_cPcscReader.beginTransaction();

		DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "SCardBeginTransaction() success");
		DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "");

		tDataIn.EnableWindow(TRUE);
		bBeginTransaction.EnableWindow(FALSE);
		bStatus.EnableWindow(TRUE);
		bTransmit.EnableWindow(TRUE);
		bEndTransaction.EnableWindow(TRUE);
		bDisconnect.EnableWindow(FALSE);
	}
	catch(AcsReaderException cException)
	{
		sprintf(aMessage, "%s%d%s %s", "[", _cPcscReader.getReturnCode(),"]", cException.what());
		showErrorMessage(aMessage);

		DisplayOut(DISPLAY_TYPE_ERROR, _cPcscReader.getReturnCode(), cException.what());
		DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "");
	}
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CSimplePCSCDlg::OnBTNEndTransaction() 
{
	char aMessage[100];

	try
	{
		_cPcscReader.endTransaction();
		DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "SCardEndTransaction() success");
		DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "");

		bBeginTransaction.EnableWindow(TRUE);
		bStatus.EnableWindow(FALSE);
		bTransmit.EnableWindow(FALSE);
		bEndTransaction.EnableWindow(FALSE);
		bDisconnect.EnableWindow(TRUE);
		DisableAPDUInput();
	}
	catch(AcsReaderException cException)
	{
		sprintf(aMessage, "%s%d%s %s", "[", _cPcscReader.getReturnCode(),"]", cException.what());
		showErrorMessage(aMessage);

		DisplayOut(DISPLAY_TYPE_ERROR, _cPcscReader.getReturnCode(), cException.what());
		DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "");
	}
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CSimplePCSCDlg::OnBTNTransmit() 
{
	int8 tmpString[257], tmpStr[257], tmpData[257], buffer[257];
	int32   tmpLen, j, k;
	uint32  i;
	uint32	uSendLen;
    uint32	RecvLen = sizeof(_aRecvBuff);
	char aMessage[100];

	tmpLen = tDataIn.GetWindowText(tmpStr, 257);
	if  (tmpLen < 1)
	{
		_pThis->tDataIn.SetFocus();
		m_TextBoxLogs = m_TextBoxLogs + "No Input Data" + "\r\n";	
		UpdateData(false);
		m_TextBoxLogsVar.SetSel(0xffff, 0xffff);
		return;
	}
    j = 0;
	for (i=0;tmpStr[i] != 0; i++)
	{
		if (tmpStr[i] != 32) 
		{ 
			tmpData[j] = tmpStr[i];
			j++;
		}
	}
	if (j%2 != 0)
	{
		_pThis->tDataIn.SetFocus();
		m_TextBoxLogs = m_TextBoxLogs + "Invalid input data, uneven number of characters" + "\r\n";
		UpdateData(false);
		m_TextBoxLogsVar.SetSel(0xffff, 0xffff);
		return;
	}

	if (j/2 < 5)
	{
		_pThis->tDataIn.SetFocus();
		m_TextBoxLogs = m_TextBoxLogs + "Insufficient input data" + "\r\n";	
		UpdateData(false);
		m_TextBoxLogsVar.SetSel(0xffff, 0xffff);
		return;
	}

	if (j/2 < 6)
	{
	    strcpy(tmpString, "");
		for (k=i=0; i < 5; i++)
		{
			sscanf(&tmpData[k], "%02X", &_aSendBuff[i]);
			k += 2;
		}
		for(i=0; i<5; i++)
		{
			sprintf(buffer, "%02X ", _aSendBuff[i] & 0x00FF);
			strcat(tmpString, buffer);
		}
		uSendLen = 5;
	}
	else
	{
	    strcpy(tmpString, "");
		for (k=i=0; i < 5; i++)
		{
			sscanf(&tmpData[k], "%02X", &_aSendBuff[i]);
			k += 2;
		}
		uSendLen = _aSendBuff[4] + 5;
		if ( (uint32)(j/2) < uSendLen)
		{
			_pThis->tDataIn.SetFocus();
			m_TextBoxLogs = m_TextBoxLogs + "Insufficient input data" + "\r\n";	
			UpdateData(false);
			m_TextBoxLogsVar.SetSel(0xffff, 0xffff);
			return;
		}
		for (k=i=0; i < _aSendBuff[4]; i++)
		{
			sscanf(&tmpData[k+10], "%02X", &_aSendBuff[i+5]);
			k += 2;
		}		
		for(i=0; i < uSendLen; i++)
		{
			sprintf(buffer, "%02X ", _aSendBuff[i] & 0x00FF);
			strcat(tmpString, buffer);
		}
	}

	DisplayOut(DISPLAY_TYPE_INPUT_DATA, 0, tmpString);
	
	try 
	{
		_cPcscReader.sendApduCommand(_aSendBuff,uSendLen,
									 _aRecvBuff,RecvLen);

		strcpy(tmpString, "");
		for(i=0; i<RecvLen; i++)
		{
			sprintf(buffer, "%02X ", _aRecvBuff[i] & 0x00FF);
			strcat(tmpString, buffer);
		}

		DisplayOut(DISPLAY_TYPE_OUTPUT_DATA, 0, tmpString);
		DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "SCardTransmit() success");
		DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "");
	}
	catch(AcsReaderException cException)
	{
		sprintf(aMessage, "%s%d%s %s", "[", _cPcscReader.getReturnCode(),"]", cException.what());
		showErrorMessage(aMessage);

		DisplayOut(DISPLAY_TYPE_ERROR, _cPcscReader.getReturnCode(), cException.what());
		DisplayOut(DISPLAY_TYPE_NOTIFICATION, 0, "");
	}	
}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void CSimplePCSCDlg::OnBTNClear() 
{
	m_TextBoxLogsVar.SetWindowText("");	
	UpdateData(true);
}


