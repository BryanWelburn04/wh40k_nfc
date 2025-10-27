//  Copyright(C):      Advanced Card Systems Ltd
//
//  File:              PollingDlg.cpp
//
//  Description:       This sample program outlines the steps on how to
//                     automatically detect the insertion and removal
//                     of the smart card from the smartcard reader using
//                     the PC/SC platform.
//
//  Revision Trail:   (Date/Author/Description)
//
//======================================================================
// PollingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Polling.h"
#include "PollingDlg.h"
#include "PcscProvider.h"
#include "PcscException.h"
#include "CardException.h"
#include <string>

// ==========================================================================
// Polling include file
#include "PcscReader.h"
#include "AcsReaderException.h"

// Polling GlobalVariables
CPollingDlg			    *_pThis=NULL;

uint32 PollingCallback(uint32 uEventCode, byte *pAtr, DWORD lAtrLen, void * pParam);

// ==========================================================================
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CPollingDlg dialog

CPollingDlg::CPollingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPollingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPollingDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIconBig   = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIconSmall = AfxGetApp()->LoadIcon(IDR_SMALLICON);
}

void CPollingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPollingDlg)
	DDX_Control(pDX, IDC_EDIT1, EditBoxMessage);
	DDX_Control(pDX, IDC_EditAtr, EditBoxAtr);
	DDX_Control(pDX, IDC_EditCardType, EditBoxCardType);
	DDX_Control(pDX, IDC_COMBO1, ComboboxReader);
	DDX_Control(pDX, IDC_BUTTON5, ButtonQuit);
	DDX_Control(pDX, IDC_BUTTON4, ButtonReset);
	DDX_Control(pDX, IDC_BUTTON3, ButtonEnd);
	DDX_Control(pDX, IDC_BUTTON2, ButtonStart);
	DDX_Control(pDX, IDC_BUTTON1, ButtonInitialize);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPollingDlg, CDialog)
	//{{AFX_MSG_MAP(CPollingDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnInit)
	ON_BN_CLICKED(IDC_BUTTON2, OnStartPolling)
	ON_BN_CLICKED(IDC_BUTTON3, OnEndPolling)
	ON_BN_CLICKED(IDC_BUTTON4, OnReset)
	ON_BN_CLICKED(IDC_BUTTON5, OnQuit)
	ON_MESSAGE(MESSAGE_CARD_STATUS,CardStatusHandler)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// ==========================================================================
// Polling internal routines


/////////////////////////////////////////////////////////////////////////////
// CPollingDlg message handlers

BOOL CPollingDlg::OnInitDialog()
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

	// Instantiate an object of the PcscReader-derived class
	pPcscReader_ = new PcscReader();

	EditBoxMessage.SetWindowText("");
	ButtonStart.EnableWindow(FALSE);
	ButtonEnd.EnableWindow(FALSE);
    EditBoxMessage.SetWindowText("Initialize Smart Card resources.");
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CPollingDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPollingDlg::OnPaint() 
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
HCURSOR CPollingDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIconSmall;
}

void CPollingDlg::OnInit() 
{
	string *pReaderList = new string[10];
	uint8 uReaderCount = 10;
	int i = 0;

	try 
	{
		// Establish Context
		pPcscReader_->initialize();

		// List the detected smart card readers
		pPcscReader_->listReaders(pReaderList, uReaderCount);

		ComboboxReader.ResetContent();
		for (i = 0; i < uReaderCount; i++)
		{
			ComboboxReader.AddString(pReaderList[i].data());
		}

		ComboboxReader.SetCurSel(0);
		EditBoxMessage.SetWindowText("Start polling to detect card in reader.");
		ButtonInitialize.EnableWindow(FALSE);
		ButtonReset.EnableWindow(TRUE);
		ButtonStart.EnableWindow(TRUE);	

		//delete pReaderList;

	}
	catch (PcscException ex)
	{
		MessageBox(ex.what(), "Error", MB_ICONERROR);
	}
}


LRESULT CPollingDlg::CardStatusHandler(WPARAM, LPARAM lParam)
{
	PollingReaderParam *pParam;
	CString sDisplay;
	CString sStatus;
	CString sCardName;
	CString sReaderName;
	
	char *pTempAtr;

	pParam = (PollingReaderParam *)lParam;
	
	switch (pParam->uStatus)
	{
	case POLL_STATUS_UNKNOWN:
		sStatus = "Unknown Status in";
		break;
	case POLL_STATUS_FOUND:
		sStatus = "Card found";
		break;
	case POLL_STATUS_NOT_FOUND:
		sStatus = "Card is removed";
		break;
	case POLL_STATUS_ERROR:
		sStatus = "Error in Polling";
		break;
	case POLL_STATUS_NEW_READER_FOUND:
		sStatus = "New Reader Found";
		break;
	case POLL_STATUS_READER_REMOVED:
		sStatus = "Reader Removed";
		break;
	default:
		sStatus.Format("Unidentified Status:%04x",pParam->uStatus);
		break;
	}	

	sDisplay.Format("%s", sStatus);
	EditBoxMessage.SetWindowText(sDisplay);

	EditBoxAtr.SetWindowText("");
	EditBoxCardType.SetWindowText("");

	if(pParam->pAtr != 0)
	{	
		try
		{
			ComboboxReader.GetWindowText(sReaderName);
			pCardSelector_ = new CardSelector(pPcscReader_, (std::string)sReaderName);

			pTempAtr = cHelper_.byteAsString(pParam->pAtr, pParam->lAtrLen, true);
			EditBoxAtr.SetWindowText(pTempAtr);

			// get card type
			sCardName = pCardSelector_->readCardType(pParam->pAtr, pParam->lAtrLen);
		}
		catch (CardException cardException)
		{
			MessageBox(cardException.getMessage(), "Error", MB_ICONERROR);
			//displayMessage(cardException.getMessage());
		}
		catch (PcscException pcscException)
		{
			MessageBox(pcscException.what(), "Error", MB_ICONERROR);
			//displayMessage(pcscException.what());
		}

		EditBoxCardType.SetWindowText(sCardName);
	}


	delete pParam;
	return 0;
}

void CPollingDlg::OnStartPolling() 
{
	CString sTmpStr; 
	int32 iIndex;

	try 
	{		
		iIndex = ComboboxReader.GetCurSel();

		if (iIndex == -1)
		{
			EditBoxMessage.SetWindowText("Please select Reader");
			return;
		}	
		
		ComboboxReader.GetLBText(iIndex, sTmpStr);

		pPcscReader_->setPollingCallback(PollingCallback);
		pPcscReader_->startPolling(&sTmpStr,1);
		
		ButtonStart.EnableWindow(FALSE);
		ButtonEnd.EnableWindow(TRUE);
		EditBoxMessage.SetWindowText("");
	}
	catch (AcsReaderException cException)
	{
		EditBoxMessage.SetWindowText(cException.what());
	}
}

void CPollingDlg::OnEndPolling() 
{
	try 
	{
		pPcscReader_->stopPolling();

		ButtonStart.EnableWindow(TRUE);
		ButtonEnd.EnableWindow(FALSE);
		EditBoxMessage.SetWindowText("Polling routine ended.");
		EditBoxAtr.SetWindowText("");
		EditBoxCardType.SetWindowText("");
	}
	catch (AcsReaderException cException)
	{
		//silently discard whatever data
	}
}

void CPollingDlg::OnReset() 
{
	ButtonInitialize.EnableWindow(TRUE);
	ButtonStart.EnableWindow(FALSE);
	ButtonEnd.EnableWindow(FALSE);

    // Close SC reader
	try
	{
		pPcscReader_->stopPolling();
		pPcscReader_->cleanup();

		ComboboxReader.ResetContent();
		ComboboxReader.EnableWindow(TRUE);
		
		EditBoxMessage.SetWindowText("Initialize Smart Card resources.");
		EditBoxAtr.SetWindowText("");
		EditBoxCardType.SetWindowText("");
		
	}
	catch (AcsReaderException cException)
	{
		EditBoxMessage.SetWindowText(cException.what());
	}
}

void CPollingDlg::OnQuit() 
{
    int iTmpResult = 0;

    // Close SC reader
	try
	{
		pPcscReader_->stopPolling();
		pPcscReader_->cleanup();
		
		delete pPcscReader_;
	}
	catch (AcsReaderException cException)
	{
		
	}

    EndDialog(iTmpResult);	
}

uint32 PollingCallback(uint32 uStatus, byte *pAtr, DWORD lAtrLen, void * pParam)
{	
	PollingReaderParam *pEventParam;
	CString sReaderName = (int8 *) pParam;

	pEventParam = new PollingReaderParam;

	if (pEventParam == NULL)
	{
		return ACSTAT_FAIL;
	}

	pEventParam->uStatus = uStatus;
	pEventParam->sReaderName = sReaderName;	
	pEventParam->pAtr = pAtr;
	pEventParam->lAtrLen = lAtrLen;

	PostMessage((HWND)_pThis->m_hWnd,MESSAGE_CARD_STATUS,0,(LPARAM)pEventParam);

	return ACSTAT_OK;
}
