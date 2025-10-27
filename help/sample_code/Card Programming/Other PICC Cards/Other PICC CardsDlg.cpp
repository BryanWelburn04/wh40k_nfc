//  Copyright(C):      Advanced Card Systems Ltd
//
//  File:              COtherPICCCardsDlg.cpp
//
//  Description:       This sample program outlines the steps on how to
//                     transact with other PICC cards using ACR1281
//
//  Author:            Wazer Emmanuel R. Benal
//
//	Date:              June 19, 2008
//
//	Revision Trail:
//  Date               Author                   Description
//  March 20, 2014     Maria Czarina Burgos     Converted to OOP
//====================================================================================================

#include "stdafx.h"
#include "Other PICC Cards.h"
#include "Other PICC CardsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Global Variable
/////////////////////////////////////////////////////////////////////////////
COtherPICCCardsDlg	*pThis = NULL;

/////////////////////////////////////////////////////////////////////////////
// Helper
/////////////////////////////////////////////////////////////////////////////
void sendCommandFunctor(const byte *pData, const int iLength)
{
	CString sDataString = "";
	CString sByteString = "";

	for (int i = 0; i < iLength; i++)
	{
		sByteString.Format("%02X", pData[i]);
		sDataString += " " + sByteString;		
	}

	sDataString.TrimLeft();
	sDataString.TrimRight();
	
	pThis->displayOut(0, sDataString, GREEN);
}

void recvCommandFunctor(const byte *pData, const int iLength)
{
	CString sDataString = "";
	CString sByteString = "";

	for (int i = 0; i < iLength; i++)
	{
		sByteString.Format("%02X", pData[i]);
		sDataString += " " + sByteString;		
	}

	sDataString.TrimLeft();
	sDataString.TrimRight();
	
	pThis->displayOut(1, sDataString, GREEN);
}

// Displays the message with the corresponding color
void COtherPICCCardsDlg::displayOut( int iType, CString str, COLORREF color )
{
	int nOldLines = 0,
		nNewLines = 0,
		nScroll = 0;
	long nInsertPoint = 0;
	CHARFORMAT cf;

	// Save number of lines before insertion of new text
	nOldLines = pThis->rbResult.GetLineCount();

	//Initialize character format structure
	cf.cbSize		= sizeof( CHARFORMAT );
	cf.dwMask		= CFM_COLOR;
	cf.dwEffects	= 0;	// To disable CFE_AUTOCOLOR
	cf.crTextColor	= color;

	// Set insertion point to end of text
	nInsertPoint = rbResult.GetWindowTextLength();
	rbResult.SetSel( nInsertPoint, -1 );
	
	// Set the character format
	rbResult.SetSelectionCharFormat( cf );

	// Insert string at the current caret poisiton
	if(iType == 0)
		rbResult.ReplaceSel( "\r\n<< " + str );
	else
		rbResult.ReplaceSel( "\r\n>> " + str + "\r\n\n");

	nNewLines = rbResult.GetLineCount();
	nScroll	= nNewLines - nOldLines;
	rbResult.LineScroll( 1 );
}

void COtherPICCCardsDlg::displayOut( bool isSpace, CString str, COLORREF color )
{
	int nOldLines = 0,
		nNewLines = 0,
		nScroll = 0;
	long nInsertPoint = 0;
	CHARFORMAT cf;

	// Save number of lines before insertion of new text
	nOldLines = pThis->rbResult.GetLineCount();

	//Initialize character format structure
	cf.cbSize		= sizeof( CHARFORMAT );
	cf.dwMask		= CFM_COLOR;
	cf.dwEffects	= 0;	// To disable CFE_AUTOCOLOR
	cf.crTextColor	= color;

	// Set insertion point to end of text
	nInsertPoint = rbResult.GetWindowTextLength();
	rbResult.SetSel( nInsertPoint, -1 );
	
	// Set the character format
	rbResult.SetSelectionCharFormat( cf );

	// Insert string at the current caret poisiton
	if(isSpace)
		rbResult.ReplaceSel("\r\n" + str + "\r\n");
	else
		rbResult.ReplaceSel( str );

	nNewLines = rbResult.GetLineCount();
	nScroll	= nNewLines - nOldLines;
	rbResult.LineScroll( 1 );	
}

void COtherPICCCardsDlg::resetApplication()
{
	if(pPICCClass_ == NULL)
	{
		try
		{
			if (pPICCClass_ != NULL)
			{
				pPICCClass_->disconnect();
				pPICCClass_ = NULL;
			}
		}
		catch (PcscException cException)
		{
			displayOut(true, cException.what(), RED);
			MessageBox(cException.what(), "PCSC Exception", MB_OK);
			return;
		}
		catch (...)
		{
			displayOut(true, "System Exception", RED);
			return;
		}
	}

	cbReader.SetWindowText( "" );
	cbReader.ResetContent();

	rbResult.SetWindowText( "" );
	displayOut( true, "Program Ready", BLACK );

	btnInit.EnableWindow( true );
	btnClear.EnableWindow( true );
	btnReset.EnableWindow( true );
	btnQuit.EnableWindow( true );
	btnConnect.EnableWindow( false );	
	btnGetData.EnableWindow( false );
	btnSendCmd.EnableWindow( false );

	check1.EnableWindow( false );
	tbCLA.EnableWindow( false );
	tbINS.EnableWindow( false );
	tbP1.EnableWindow( false );
	tbP2.EnableWindow( false );
	tbLc.EnableWindow( false );
	tbLe.EnableWindow( false );
	tbData.EnableWindow( false );

	check1.SetCheck( false );
	tbCLA.SetWindowText( "" );
	tbINS.SetWindowText( "" );
	tbP1.SetWindowText( "" );
	tbP2.SetWindowText( "" );
	tbLc.SetWindowText( "" );
	tbLe.SetWindowText( "" );
	tbData.SetWindowText( "" );
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
/////////////////////////////////////////////////////////////////////////////
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
// COtherPICCCardsDlg dialog
/////////////////////////////////////////////////////////////////////////////
COtherPICCCardsDlg::COtherPICCCardsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COtherPICCCardsDlg::IDD, pParent)
	, pPICCClass_(NULL)
{
	//{{AFX_DATA_INIT(COtherPICCCardsDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIconBig   = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIconSmall = AfxGetApp()->LoadIcon(IDR_SMALLICON);
}

void COtherPICCCardsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COtherPICCCardsDlg)
	DDX_Control(pDX, IDC_RICHEDIT1, rbResult);
	DDX_Control(pDX, IDC_COMBO1, cbReader);
	DDX_Control(pDX, IDC_BUTTON7, btnQuit);
	DDX_Control(pDX, IDC_BUTTON6, btnReset);
	DDX_Control(pDX, IDC_BUTTON5, btnClear);
	DDX_Control(pDX, IDC_BUTTON4, btnSendCmd);
	DDX_Control(pDX, IDC_BUTTON3, btnGetData);
	DDX_Control(pDX, IDC_BUTTON2, btnConnect);
	DDX_Control(pDX, IDC_BUTTON1, btnInit);
	DDX_Control(pDX, IDC_EDIT7, tbData);
	DDX_Control(pDX, IDC_EDIT6, tbLe);
	DDX_Control(pDX, IDC_EDIT5, tbLc);
	DDX_Control(pDX, IDC_EDIT4, tbP2);
	DDX_Control(pDX, IDC_EDIT3, tbP1);
	DDX_Control(pDX, IDC_EDIT2, tbINS);
	DDX_Control(pDX, IDC_EDIT1, tbCLA);	
	DDX_Control(pDX, IDC_CHECK1, check1);
	DDX_Text(pDX, IDC_EDIT7, strTbData);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COtherPICCCardsDlg, CDialog)
	//{{AFX_MSG_MAP(COtherPICCCardsDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnInit)
	ON_BN_CLICKED(IDC_BUTTON2, OnConnect)
	ON_BN_CLICKED(IDC_BUTTON3, OnGetData)
	ON_BN_CLICKED(IDC_BUTTON4, OnSendCmd)
	ON_BN_CLICKED(IDC_BUTTON5, OnClear)
	ON_BN_CLICKED(IDC_BUTTON6, OnReset)
	ON_BN_CLICKED(IDC_BUTTON7, OnQuit)	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COtherPICCCardsDlg message handlers
/////////////////////////////////////////////////////////////////////////////
BOOL COtherPICCCardsDlg::OnInitDialog()
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

	// Set the icon for this dialog. The framework does this automatically
	// when the application's main window is not a dialog
	SetIcon(m_hIconBig,	  TRUE);  // Set big icon
	SetIcon(m_hIconSmall, FALSE); // Set small icon
	
	// TODO: Add extra initialization here
	pThis = this;
	pPICCClass_ = new PICCClass();
	pPICCClass_->setSendCommandFunctor(sendCommandFunctor);
	pPICCClass_->setRecvCommandFunctor(recvCommandFunctor);
	pThis->tbCLA.SetLimitText( 2 );
	pThis->tbINS.SetLimitText( 2 );
	pThis->tbP1.SetLimitText( 2 );
	pThis->tbP2.SetLimitText( 2 );
	pThis->tbLc.SetLimitText( 2 );
	pThis->tbLe.SetLimitText( 2 );

	resetApplication();
	
	return TRUE; // return TRUE  unless you set the focus to a control
}

COtherPICCCardsDlg::~COtherPICCCardsDlg() 
{	
	try
	{
		if (pPICCClass_ != NULL)
			pPICCClass_->disconnect();

		delete pPICCClass_;
		pPICCClass_ = NULL;
	}
	catch (PcscException cException)
	{
		displayOut(true, cException.what(), RED);
		MessageBox(cException.what(), "System Exception", MB_OK);
	}
	catch (...)
	{
		displayOut(true, "System Exception", RED);
		return;
	}
}

void COtherPICCCardsDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
// to draw the icon.  For MFC applications using the document/view model,
// this is automatically done for you by the framework.

void COtherPICCCardsDlg::OnPaint() 
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
// the minimized window.
HCURSOR COtherPICCCardsDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIconSmall;
}

/////////////////////////////////////////////////////////////////////////////
// Button Events
/////////////////////////////////////////////////////////////////////////////
void COtherPICCCardsDlg::OnInit() 
{
	StringVector strVectReaderList;

	cbReader.ResetContent();

	try
	{
		pPICCClass_->initialize();
		pPICCClass_->listReaders(strVectReaderList);

		for(int iIdx = 0; iIdx != strVectReaderList.size(); iIdx++)
		{		
			string sReaderName = strVectReaderList[iIdx];
			this->cbReader.AddString(sReaderName.data());
		}

		displayOut(true, "Initialize success", BLACK);
		cbReader.SetCurSel( 0 );
		btnConnect.EnableWindow( true );
	}
	catch (PcscException cException)
	{
		displayOut(true, cException.what(), RED);
		MessageBox(cException.what(), "Card Exception", MB_OK);
	}
	catch (...)
	{
		displayOut(true, "System Exception", RED);
		return;
	}
}

void COtherPICCCardsDlg::OnConnect() 
{	
	// This procedure connects the reader to the Smart Card inserted

	char cMessage[256];
	char cReaderName [128];

	this->cbReader.GetLBText(cbReader.GetCurSel (), cReaderName);

	try
	{
		pPICCClass_->connect(cReaderName);
		pPICCClass_->selectReader(cReaderName);

		sprintf(cMessage, "%s %s", "Successfully connected to ", cReaderName);
		displayOut( true, cMessage, BLACK );

		btnReset.EnableWindow( true );
		btnGetData.EnableWindow( true );
		btnSendCmd.EnableWindow( true );

		check1.EnableWindow( true );
		tbCLA.EnableWindow( true );
		tbINS.EnableWindow( true );
		tbP1.EnableWindow( true );
		tbP2.EnableWindow( true );
		tbLc.EnableWindow( true );
		tbLe.EnableWindow( true );
		tbData.EnableWindow( true );
	}
	catch (CardException cException)
	{
		displayOut(true, cException.getMessage(), RED);
		MessageBox(cException.getMessage(), "Card Exception", MB_OK);
	}
	catch (PcscException cException)
	{
		displayOut(true, cException.what(), RED);
		MessageBox(cException.what(), "PCSC Exception", MB_OK);	
	}
	catch (...)
	{
		displayOut(true, "System Exception", RED);
		return;
	}
}

void COtherPICCCardsDlg::OnGetData() 
{
	byte uIso14333A;
    byte uCardVersion[255];
	int  iLength;

	if( check1.GetCheck() )
		uIso14333A = 0x01; // P1 : ISO 14443 A Card
	else
		uIso14333A = 0x00; // P1 : Other cards

	try
    {
		displayOut(false, "Get Data", BLACK);
        pPICCClass_->getData(uIso14333A, uCardVersion, iLength);
		char *pCardVersion = helper_.byteAsString(uCardVersion, iLength, true);

        if (check1.GetCheck() == BST_CHECKED)
			displayOut(false, "UID: ", BLACK);
        else
			displayOut(false, "ATS: ", BLACK);
		
		displayOut(true, pCardVersion, BLACK);
		delete[] pCardVersion;

    }
	catch (CardException cException)
	{
		displayOut(true, cException.getMessage(), RED);
		MessageBox(cException.getMessage(), "Card Exception", MB_OK);
	}
	catch (PcscException cException)
	{
		displayOut(true, cException.what(), RED);
		MessageBox(cException.what(), "PCSC Exception", MB_OK);
	}
	catch (...)
	{
		displayOut(true, "System Exception", RED);
		return;
	}
}

void COtherPICCCardsDlg::OnSendCmd() 
{
	char	cHolder[3];
	CString sHolderData;
	byte	uCla, uIns, uP1, uP2;
    byte	uLc = 0x00;
    byte	uLe = 0x00;
	byte	*pData;
	char	cHolderData[32767];
    int		iDataLen = 0;
    int		iCaseType = 0;

	// Validate inputs
	tbCLA.GetWindowText( cHolder, 3 );
	if( strcmp( cHolder, "" ) == 0 || helper_.hexCheck(cHolder) == false )
	{			
		tbCLA.SetFocus();
		return;	
	}

	uCla = helper_.getByte(cHolder);
	
	tbINS.GetWindowText( cHolder, 3 );
	if( strcmp( cHolder, "" ) == 0 || helper_.hexCheck(cHolder) == false )
	{		
		tbINS.SetFocus();
		return;	
	}
	
	uIns = helper_.getByte(cHolder);

	tbP1.GetWindowText( cHolder, 3 );
	if( strcmp( cHolder, "" ) == 0 || helper_.hexCheck(cHolder) == false )
	{		
		tbP1.SetFocus();
		return;	
	}
	
	uP1 = helper_.getByte(cHolder);
	
	tbP2.GetWindowText( cHolder, 3 );
	if( strcmp( cHolder, "" ) == 0 || helper_.hexCheck(cHolder) == false )
	{		
		tbP2.SetFocus();
		return;	
	}

	uP2 = helper_.getByte(cHolder);

	tbLc.GetWindowText( cHolder, 3 );
	if(strcmp( cHolder, "" ) != 0 && strcmp( cHolder, "00" ) != 0)
	{	
		tbData.GetWindowText( sHolderData );
		if( sHolderData == "" )
		{		
			tbData.SetFocus();
			return;
		}

		sHolderData.Remove(' ');
		if(!(helper_.hexCheck(sHolderData)))
		{
			displayOut(true, "Data In field has invalid format.", RED);
			tbData.SetFocus();
			return;
		}
	}

	if(strcmp( cHolder, "" ) != 0)
	{
		uLc = helper_.getByte(cHolder);

		if(uLc > 0x00)
		{
			strcpy(cHolderData, (LPCTSTR)sHolderData);
			pData = helper_.getBytes(cHolderData);
			if(pData == NULL)
			{
				displayOut(true, "Data In field has invalid format.", RED);
				tbData.SetFocus();
				return;
			}

			iDataLen = sHolderData.GetLength() / 2;

			if(uLc != iDataLen)
			{
				displayOut(true, "Lc does not match actual Data in Command.", RED);
				tbLc.SetFocus();
				return;
			}
		}		
	}

	tbLe.GetWindowText( cHolder, 3 );
	if( strcmp( cHolder, "" ) != 0 )
	{		
		if(helper_.hexCheck(cHolder) == false)
		{
			tbLe.SetFocus();
			return;
		}
		else
		{
			uLe = helper_.getByte(cHolder);
		}
	}

	tbLc.GetWindowText( cHolder, 3 );
	if(strcmp( cHolder, "" ) == 0)
	{
		if(uLe == 0x00)
			iCaseType = 0;
		else
			iCaseType = 1;
	}
	else
	{
		if (uLe == 0x00)
			iCaseType = 2;
	    else
		    iCaseType = 3;
	}

	try
	{
		displayOut(false, "Send Command", BLACK);
		pPICCClass_->sendCommand(iCaseType, uCla, uIns, uP1, uP2, uLc, uLe, pData);

		tbData.SetWindowText("");
	}
	catch (CardException cException)
	{
		displayOut(true, cException.getMessage(), RED);
		MessageBox(cException.getMessage(), "Card Exception", MB_OK);
	}
	catch (PcscException cException)
	{
		displayOut(true, cException.what(), RED);
		MessageBox(cException.what(), "PCSC Exception", MB_OK);
	}
	catch (...)
	{
		displayOut(true, "System Exception", RED);
		return;
	}

	if(uLc > 0x00)
		delete [] pData;
}

void COtherPICCCardsDlg::OnClear() 
{
	rbResult.SetWindowText( "" );
}

void COtherPICCCardsDlg::OnReset() 
{	
	resetApplication();
}

void COtherPICCCardsDlg::OnQuit() 
{	
	try
	{
		if (pPICCClass_ != NULL)
		{
			pPICCClass_->disconnect();
			delete pPICCClass_;
			pPICCClass_ = NULL;
		}
	}
	catch (PcscException cException)
	{
		displayOut(true, cException.what(), RED);
		MessageBox(cException.what(), "PCSC Exception", MB_OK);
	}
	catch (...)
	{
		displayOut(true, "System Exception", RED);
		return;
	}

	EndDialog(0);
}
