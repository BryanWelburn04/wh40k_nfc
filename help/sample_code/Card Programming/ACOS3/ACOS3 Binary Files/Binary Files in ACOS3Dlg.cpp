//  Copyright(C):      Advanced Card Systems Ltd
//
//  File:              CBinaryFilesinACOS3Dlg.cpp
//
//  Description:       This sample program outlines the steps on how to
//                     implement the binary file support in ACOS3-24K
//
//  Author:	           Wazer Emmanuel R. Benal
//
//  Date:	           June 23, 2008
//
//  Revision Trail:	   (Date/Author/Description)
//	
//	10/17/2012	     Valerie C. Dio			 recoded to object oriented
//
//======================================================================

#include "stdafx.h"
#include "Binary Files in ACOS3.h"
#include "Binary Files in ACOS3Dlg.h"
#include "winscard.h"
#include "PcscProvider.h"
#include "PcscReader.h"
#include "Helper.h"
#include "Acos3.h"
#include "CardSelector.h"

#include <iostream>
#include <vector>

//Define constants//////////////////////////////////////////////////
#define MAX 262
////////////////////////////////////////////////////////////////////

//ACOS3 Binary Files Inlude File
#include "WINSCARD.h"

//Global Variables
CBinaryFilesinACOS3Dlg	*_pThis = NULL;
Helper _cHelper;
byte _IssuerCode[] = { 0x41, 0x43, 0x4F, 0x53, 0x54, 0x45, 0x53, 0x54 };
CString _sReaderName;

// Function prototypes
void onSendCommand(const byte* pSendData, const int iLen);
void onReceiveCommand(const byte* pReceiveData, const int iLen);

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//Error checking for inputs that needs to be in hex format
int HexCheck( char chData1, char chData2 )
{
	int iRetval = 1;
	bool bState1, bState2;

	if( chData1 == '0' ||
		chData1 == '1' ||
		chData1 == '2' ||
		chData1 == '3' ||
		chData1 == '4' ||
		chData1 == '5' ||
		chData1 == '6' ||
		chData1 == '7' ||
		chData1 == '8' ||
		chData1 == '9' ||
		chData1 == 'A' ||
		chData1 == 'B' ||
		chData1 == 'C' ||
		chData1 == 'D' ||
		chData1 == 'E' ||
		chData1 == 'F' ||
		chData1 == 'a' ||
		chData1 == 'b' ||
		chData1 == 'c' ||
		chData1 == 'd' ||
		chData1 == 'e' ||
		chData1 == 'f' )
	{	
		bState1 = true;	
	}
	else
	{	
		bState1 = false;	
	}

	if( chData2 == '0' ||
		chData2 == '1' ||
		chData2 == '2' ||
		chData2 == '3' ||
		chData2 == '4' ||
		chData2 == '5' ||
		chData2 == '6' ||
		chData2 == '7' ||
		chData2 == '8' ||
		chData2 == '9' ||
		chData2 == 'A' ||
		chData2 == 'B' ||
		chData2 == 'C' ||
		chData2 == 'D' ||
		chData2 == 'E' ||
		chData2 == 'F' ||
		chData2 == 'a' ||
		chData2 == 'b' ||
		chData2 == 'c' ||
		chData2 == 'd' ||
		chData2 == 'e' ||
		chData2 == 'f' ||
		chData2 == NULL )
	{	
		bState2 = true;	
	}
	else
	{	
		bState2 = false;	
	}

	if( bState1 == true && bState2 == true )	
		iRetval = 0;	
	else	
		iRetval = 1;	
				
	return iRetval;
}

void CBinaryFilesinACOS3Dlg::enableControls(BOOL bState)
{

	_pThis->ButtonConnect.EnableWindow( bState );
	_pThis->ButtonFormat.EnableWindow( bState );
	_pThis->ButtonRead.EnableWindow( bState );
	_pThis->ButtonWrite.EnableWindow( bState );

	_pThis->TextBoxFileID1.EnableWindow( bState );
	_pThis->TextBoxFileID2.EnableWindow( bState );
	_pThis->TextBoxLen1.EnableWindow( bState );
	_pThis->TextBoxLen2.EnableWindow( bState );
	_pThis->TextBoxReadWriteFileId1.EnableWindow( bState );
	_pThis->TextBoxReadWriteFileId2.EnableWindow( bState );
	_pThis->TextBoxOffset1.EnableWindow( bState );
	_pThis->TextBoxOffset2.EnableWindow( bState );
	_pThis->TextBoxLength.EnableWindow( bState );
	_pThis->TextBoxData.EnableWindow( bState );
	
}

void CBinaryFilesinACOS3Dlg::resetFields()
{
	_pThis->ComboBoxReader.SetWindowText( "" );
	_pThis->ComboBoxReader.ResetContent();
	_pThis->TextBoxFileID1.SetWindowText( "" );
	_pThis->TextBoxFileID2.SetWindowText( "" );
	_pThis->TextBoxLen1.SetWindowText( "" );
	_pThis->TextBoxLen2.SetWindowText( "" );
	_pThis->TextBoxReadWriteFileId1.SetWindowText( "" );
	_pThis->TextBoxReadWriteFileId2.SetWindowText( "" );
	_pThis->TextBoxOffset1.SetWindowText( "" );
	_pThis->TextBoxOffset2.SetWindowText( "" );
	_pThis->TextBoxLength.SetWindowText( "" );
	_pThis->TextBoxData.SetWindowText( "" );

	EditBoxLogs.SetWindowText("Program Ready\r\n");
	UpdateData(true);
}

void CBinaryFilesinACOS3Dlg::enableReadWriteBlock(bool bEnable)
{
	ButtonRead.EnableWindow( bEnable );
	ButtonWrite.EnableWindow( bEnable );

	TextBoxReadWriteFileId1.EnableWindow( bEnable );
	TextBoxReadWriteFileId2.EnableWindow( bEnable );
	TextBoxOffset1.EnableWindow( bEnable );
	TextBoxOffset2.EnableWindow( bEnable );
	TextBoxLength.EnableWindow( bEnable );
	TextBoxData.EnableWindow( bEnable );
}

void CBinaryFilesinACOS3Dlg::onPcscException(PcscException cException)
{
	sprintf(aMessage_,"\r\n%s",CString(cException.what()));
	displayMessage(aMessage_);
	MessageBox(cException.what(), "Error", MB_ICONERROR);
}

void CBinaryFilesinACOS3Dlg::onCardException(CardException cException)
{
	sprintf(aMessage_,"\r\n[%02X %02X] %s",cException.getStatusWord()[0], 
										 cException.getStatusWord()[1], 
										 CString(cException.getMessage()));
	displayMessage(aMessage_);
	MessageBox(aMessage_, "Error", MB_ICONERROR);
}

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
// CBinaryFilesinACOS3Dlg dialog

CBinaryFilesinACOS3Dlg::CBinaryFilesinACOS3Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBinaryFilesinACOS3Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBinaryFilesinACOS3Dlg)
	EditBoxLogsValue = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIconBig   = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIconSmall = AfxGetApp()->LoadIcon(IDR_SMALLICON);
}

void CBinaryFilesinACOS3Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBinaryFilesinACOS3Dlg)
	DDX_Control(pDX, IDC_EDIT10, EditBoxLogs);
	DDX_Control(pDX, IDC_EDIT6, TextBoxReadWriteFileId2);
	DDX_Control(pDX, IDC_EDIT5, TextBoxReadWriteFileId1);
	DDX_Control(pDX, IDC_BUTTON3, ButtonFormat);	
	DDX_Control(pDX, IDC_BUTTON2, ButtonConnect);
	DDX_Control(pDX, IDC_BUTTON1, ButtonInitialize);
	DDX_Control(pDX, IDC_BUTTON5, ButtonWrite);
	DDX_Control(pDX, IDC_BUTTON4, ButtonRead);
	DDX_Control(pDX, IDC_BUTTON8, ButtonQuit);			
	DDX_Control(pDX, IDC_BUTTON7, ButtonReset);
	DDX_Control(pDX, IDC_BUTTON6, ButtonClear);	
	DDX_Control(pDX, IDC_EDIT9, TextBoxLength);
	DDX_Control(pDX, IDC_EDIT11, TextBoxData);	
	DDX_Control(pDX, IDC_EDIT7, TextBoxOffset1);
	DDX_Control(pDX, IDC_EDIT8, TextBoxOffset2);
	DDX_Control(pDX, IDC_EDIT3, TextBoxLen1);
	DDX_Control(pDX, IDC_EDIT4, TextBoxLen2);
	DDX_Control(pDX, IDC_EDIT1, TextBoxFileID1);
	DDX_Control(pDX, IDC_EDIT2, TextBoxFileID2);
	DDX_Control(pDX, IDC_COMBO1, ComboBoxReader);
	DDX_Text(pDX, IDC_EDIT10, EditBoxLogsValue);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBinaryFilesinACOS3Dlg, CDialog)
	//{{AFX_MSG_MAP(CBinaryFilesinACOS3Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonInitialize)
	ON_BN_CLICKED(IDC_BUTTON2, OnButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON6, OnButtonClear)
	ON_BN_CLICKED(IDC_BUTTON7, OnButtonReset)
	ON_BN_CLICKED(IDC_BUTTON8, OnButtonQuit)	
	ON_BN_CLICKED(IDC_BUTTON3, OnButtonFormat)
	ON_BN_CLICKED(IDC_BUTTON4, OnButtonRead)
	ON_BN_CLICKED(IDC_BUTTON5, OnButtonWrite)
	ON_EN_CHANGE(IDC_EDIT9, OnChangeEdit9)
	ON_EN_KILLFOCUS(IDC_EDIT9, OnKillfocusEdit9)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBinaryFilesinACOS3Dlg message handlers

BOOL CBinaryFilesinACOS3Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	_pThis = this;

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

	// Instantiate an object of the PcscReader-derived class
	pPcscReader_ = new PcscReader();	

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIconBig,	TRUE);		// Set big icon
	SetIcon(m_hIconSmall,	FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	UpdateData(false);

	_pThis->TextBoxLength.LimitText(2);
	enableControls(false);

	EditBoxLogs.SetWindowText("Program Ready\r\n");
	UpdateData(true);

	_pThis->TextBoxFileID1.SetLimitText( 2 );
	_pThis->TextBoxFileID2.SetLimitText( 2 );
	_pThis->TextBoxLen1.SetLimitText( 2 );
	_pThis->TextBoxLen2.SetLimitText( 2 );
	_pThis->TextBoxReadWriteFileId1.SetLimitText( 2 );
	_pThis->TextBoxReadWriteFileId2.SetLimitText( 2 );
	_pThis->TextBoxOffset1.SetLimitText( 2 );
	_pThis->TextBoxOffset2.SetLimitText( 2 );
	_pThis->TextBoxLength.SetLimitText( 2 );
	_pThis->TextBoxData.SetLimitText( 255 );
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBinaryFilesinACOS3Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBinaryFilesinACOS3Dlg::OnPaint() 
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
HCURSOR CBinaryFilesinACOS3Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIconSmall;
}

void CBinaryFilesinACOS3Dlg::OnButtonInitialize() 
{	
	string *pReaderList = new string[10];
	uint8 uReaderCount = 10;
	int i = 0;

	try
	{
		ComboBoxReader.ResetContent();
		enableControls(false);

		// Establish Context
		pPcscReader_->initialize();

		// List the detected smart card readers
		pPcscReader_->listReaders(pReaderList, uReaderCount);

		// Load the retrieved reader name array to the reader list
		for(i = 0; i < uReaderCount; i++)
		{
			ComboBoxReader.AddString(pReaderList[i].data());
		}
		
		ComboBoxReader.SetCurSel(0);

		// Pass the function pointers to serve as the event listeners of the ProprietaryReader class
		pPcscReader_->setSendCommandFunctor(onSendCommand);
		pPcscReader_->setRecvCommandFunctor(onReceiveCommand);					

        displayMessage("\r\nInitialize success");
		ButtonConnect.EnableWindow(true);
	}
	catch (PcscException ex)
	{
		onPcscException(ex);
	}
}

void CBinaryFilesinACOS3Dlg::OnButtonConnect() 
{
	char aMessage[100];

	uint8 aAtr[100];
	uint32 uAtrLen = sizeof(aAtr);
	CString sCardName;
	CardSelector *pCardSelector;
	
	ComboBoxReader.GetWindowText(_sReaderName);

	try
	{
		// Check if a reader name is selected
		if (!strcmp(_sReaderName, ""))
		{
			MessageBox("Select a smartcard reader.", "Error", MB_ICONERROR);
			return;
		}

		// Try to connect to the smartcard through the specified reader name
		if (pPcscReader_->connect((std::string)_sReaderName) != 0)
			return;	
		
		
		sprintf(aMessage, "\r\n%s %s", "Successfully connected to ", _sReaderName);
		displayMessage(aMessage);
		
		
		pAcos3_ = new Acos3(pPcscReader_);	
		if (pAcos3_ == NULL)
		{
			MessageBox("Insufficient resources for new ACOS3 Card.", "Error", MB_ICONERROR);
			return;
		}

		pPcscReader_->getStatus();
		pPcscReader_->getAtr(aAtr, uAtrLen);

		pCardSelector = new CardSelector(pPcscReader_);

		sCardName = pCardSelector->readCardType(aAtr, uAtrLen);

		if(sCardName == "ACOS3")
		{
			displayMessage("Chip Type: ACOS3");
			enableControls(true);

			ButtonRead.EnableWindow(false);
			enableReadWriteBlock(false);
		}
		else
		{
			MessageBox("Card not supported. Please use ACOS3 card.", "Error", MB_ICONERROR);
			enableControls(false);
		}

		ButtonConnect.EnableWindow(true);
	}
	catch (PcscException ex)
	{
		onPcscException(ex);
	}
}

void CBinaryFilesinACOS3Dlg::OnButtonClear() 
{
	EditBoxLogs.SetWindowText( "" );
	UpdateData(true);
}

void CBinaryFilesinACOS3Dlg::OnButtonReset() 
{	
	if (pPcscReader_ == NULL)
	{
		return;
	}

	if (pPcscReader_->isConnectionActive())
	{
		// Close the connection
		pPcscReader_->disconnect();
	}

	resetFields();
	enableControls(false);
}

void CBinaryFilesinACOS3Dlg::OnButtonQuit() 
{	
	this->EndDialog(0);
}

void CBinaryFilesinACOS3Dlg::OnButtonFormat() 
{
	char aTempstr[MAX], aTempstr2[MAX];
	char aFileId1[2], aFileId2[2], aLength1[2], aLength2[2];
	int iTempval, iLength1, iLength2;
	BYTE aTempArr[32];

	TextBoxFileID1.GetWindowText( aFileId1, 4 );
	if( strcmp( aFileId1, "" ) == 0 || HexCheck( aFileId1[0], aFileId1[1] ) != 0 )
	{	
		TextBoxFileID1.SetFocus();
		MessageBox("Please key-in hex value for File ID.", "Error", MB_ICONERROR);
		return;	
	}

	TextBoxFileID2.GetWindowText( aFileId2, 4 );
	if( strcmp( aFileId2, "" ) == 0 || HexCheck( aFileId2[0], aFileId2[1] ) != 0 )
	{
		TextBoxFileID2.SetFocus();
		MessageBox("Please key-in hex value for File ID.", "Error", MB_ICONERROR);
		return;	
	}

	TextBoxLen1.GetWindowText( aLength1, 4 );
	if( strcmp( aLength1, "" ) == 0 || HexCheck( aLength1[0], aLength1[1] ) != 0 )
	{
		TextBoxLen1.SetFocus();
		MessageBox("Please key-in hex value for Length.", "Error", MB_ICONERROR);
		return;	
	}

	TextBoxLen2.GetWindowText( aLength2, 4 );
	if( strcmp( aLength2, "" ) == 0 || HexCheck( aLength2[0], aLength2[1] ) != 0 )
	{
		TextBoxLen2.SetFocus();
		MessageBox("Please key-in hex value for Length.", "Error", MB_ICONERROR);
		return;	
	}

	sscanf( aLength1, "%X", &iLength1);
	sscanf( aLength2, "%X", &iLength2);

	if((iLength1 == 0) && (iLength2 == 0))
	{
		TextBoxLen2.SetFocus();
		MessageBox("Please key-in valid Length. Valid value: 01h - FFFFh", "Error", MB_ICONERROR);
		return;	
	}

	//Send IC Code

	try{
		

		displayTitle("\r\nSubmit Code - IC");
		pAcos3_->submitCode(IC, _IssuerCode);	

		displayTitle("\r\nClear Card");
		pAcos3_->clearCard();

		displayTitle("\r\nSelect File - FF 02");
		pAcos3_->selectFile(PERSONALIZATION);

		displayTitle("\r\nSubmit Code - IC");
		pAcos3_->submitCode(IC, _IssuerCode);	


		//Write to FF 02
		//This will create 1 binary file, no Option registers and
		//Security Option registers defined, Personalization bit
		//is not set
		aTempArr[0] = 0x00;
		aTempArr[1] = 0x00;
		aTempArr[2] = 0x01;
		aTempArr[3] = 0x00;

		displayTitle("\r\nWrite Record");
		pAcos3_->writeRecord(0x00,0x00,aTempArr,4);


		displayMessage( "File FF 02 is updated" );
		displayMessage( "" );

		//Perform a reset for changes in the pAcos3_ to take effect
		pPcscReader_->disconnect();
		pPcscReader_->connect((std::string)_sReaderName);

		displayMessage( "Card reset is successful");

		displayTitle("\r\nSubmit Code - IC");
		pAcos3_->submitCode(IC,_IssuerCode);	

		//Select FF 04
		displayTitle("\r\nSelect File - FF 04");
		pAcos3_->selectFile(USER_FILE_MGMT_FILE);

		//ReSend IC Code
		displayTitle("\r\nSubmit Code - IC");
		pAcos3_->submitCode(IC, _IssuerCode);	

		//Write to FF 04
		sscanf( aLength1, "%X", &iTempval );
		if( strcmp( aLength1, "" ) == 0 || HexCheck( aLength1[0], aLength1[1] ) != 0)
		{
			aTempArr[0] = 0x00;
		}				
		else
		{
			aTempArr[0] = iTempval;	
		}

		sscanf( aLength2, "%X", &iTempval );
		aTempArr[1] = iTempval;

		aTempArr[2] = 0x00;
		aTempArr[3] = 0x00;

		sscanf( aFileId1, "%X", &iTempval );
		aTempArr[4] = iTempval;

		sscanf( aFileId2, "%X", &iTempval );
		aTempArr[5] = iTempval;

		aTempArr[6] = 0x80;

		displayTitle("\r\nCreate Binary File");
		pAcos3_->writeRecord(0x00, 0x00, aTempArr, 7);
		
		sprintf( aTempstr, "" );
		sprintf( aTempstr, aFileId1 );
		sprintf( aTempstr, "%s%s", aTempstr, aFileId2 );
		sprintf( aTempstr2, "Binary User file %s is defined. Size: 0x%02x%02x bytes", aTempstr, aTempArr[0],aTempArr[1] );		
		displayMessage( aTempstr2 );

		enableReadWriteBlock(true);
	}
	catch (CardException cardException)
	{
		onCardException(cardException);
	}
	catch (PcscException pcscException)
	{
		onPcscException(pcscException);
	}

}	

void CBinaryFilesinACOS3Dlg::OnButtonRead() 
{	
	char aFileId1[2], aFileId2[2], aOffset1[2], aOffset2[2], aLength[2];
	int iTempval, iTemplen;
	BYTE uFileId1, uFileId2, uHiByte, uLoByte;
	byte *pBinaryBuffer;
	byte aFileName[2];
	CString sResult;

	TextBoxReadWriteFileId1.GetWindowText( aFileId1, 4 );
	if( strcmp( aFileId1, "" ) == 0 || HexCheck( aFileId1[0], aFileId1[1] ) != 0 )
	{	
		MessageBox("Please key-in hex value for File ID.","Error", MB_ICONERROR);
		TextBoxReadWriteFileId1.SetFocus();
		return;	
	}
	TextBoxReadWriteFileId2.GetWindowText( aFileId2, 4 );
	if( strcmp( aFileId2, "" ) == 0 || HexCheck( aFileId2[0], aFileId2[1] ) != 0 )
	{	
		MessageBox("Please key-in hex value for File ID.","Error", MB_ICONERROR);
		TextBoxReadWriteFileId2.SetFocus();
		return;	
	}
	TextBoxOffset1.GetWindowText( aOffset1, 4 );
	if( strcmp( aOffset1, "" ) == 0 || HexCheck( aOffset1[0], aOffset1[1] ) != 0 )
	{	
		MessageBox("Please key-in hex value for Offset.","Error", MB_ICONERROR);
		TextBoxOffset1.SetFocus();
		return;	
	}
	TextBoxOffset2.GetWindowText( aOffset2, 4 );
	if( strcmp( aOffset2, "" ) == 0 || HexCheck( aOffset2[0], aOffset2[1] ) != 0 )
	{	
		MessageBox("Please key-in hex value for Offset.","Error", MB_ICONERROR);
		TextBoxOffset2.SetFocus();
		return;	
	}
	TextBoxLength.GetWindowText( aLength, 4 );
	if( strcmp( aLength, "" ) == 0 || HexCheck( aLength[0], aLength[1] ) != 0 )
	{	
		MessageBox("Please key-in hex value for Length.","Error", MB_ICONERROR);
		TextBoxLength.SetFocus();
		return;	
	}	

	sscanf( aFileId1, "%X", &iTempval );
	uFileId1 = iTempval;

	sscanf( aFileId2, "%X", &iTempval );
	uFileId2 = iTempval;

	sscanf( aOffset1, "%X", &iTempval );
	if( strcmp( aOffset1, "" ) == 0 || HexCheck( aOffset1[0], aOffset1[1] ) != 0 )
	{		
		TextBoxOffset1.SetWindowText( "00" );
		uHiByte = 0x00;	
	}
	else
	{	
		uHiByte = iTempval;	
	}
	
	sscanf( aOffset2, "%X", &iTempval );
	uLoByte = iTempval;

	sscanf( aLength, "%X", &iTempval );
	iTemplen = iTempval;
	
	if(iTemplen == 0)
	{
		MessageBox("Please key-in valid Length. Valid value: 1h - FFh","Error", MB_ICONERROR);
		TextBoxLength.SetFocus();
		return;
	}

	pBinaryBuffer = new byte[iTemplen +1]; // +1 fo rnull terminator
	if (pBinaryBuffer == NULL)
	{
		displayMessage( "Unable to allocate Binary Buffer." );
		return;
	}

	try{

	    aFileName[0] = uFileId1;
		aFileName[1] = uFileId2;
		
		displayTitle("\r\nSelect File");
		//Select User File
		pAcos3_->selectFile(aFileName);	

		displayMessage( "" );

		displayTitle("Read Binary");
		//Select User File
		pAcos3_->readBinary(uHiByte, uLoByte, pBinaryBuffer,iTemplen);

		pBinaryBuffer[iTemplen] = 0;

		TextBoxData.SetWindowText( CString(pBinaryBuffer) );	
	}
	catch (CardException cardException)
	{
		onCardException(cardException);
	}
	catch (PcscException pcscException)
	{
		onPcscException(pcscException);
	}

	delete [] pBinaryBuffer;	
}

void CBinaryFilesinACOS3Dlg::OnButtonWrite() 
{	
	char aFileId1[2], aFileId2[2], aOffset1[2], aOffset2[2], aLength[2], aData[255];
	char aTempdata[MAX];
	unsigned int iTempval, iTemplen;
	BYTE uFileId1, uFileId2, uHiByte, uLoByte;
	BYTE aTempArr[256];
	byte aFileName[2]; 
	CString sResult;


	TextBoxReadWriteFileId1.GetWindowText( aFileId1, 4 );
	if( strcmp( aFileId1, "" ) == 0 || HexCheck( aFileId1[0], aFileId1[1] ) != 0 )
	{	
		MessageBox("Please key-in hex value for File ID.","Error", MB_ICONERROR);
		TextBoxReadWriteFileId1.SetFocus();
		return;	
	}

	TextBoxReadWriteFileId2.GetWindowText( aFileId2, 4 );
	if( strcmp( aFileId2, "" ) == 0 || HexCheck( aFileId2[0], aFileId2[1] ) != 0 )
	{	
		MessageBox("Please key-in hex value for File ID.","Error", MB_ICONERROR);
		TextBoxReadWriteFileId2.SetFocus();
		return;	
	}

	TextBoxOffset1.GetWindowText( aOffset1, 4 );
	if( strcmp( aOffset1, "" ) == 0 || HexCheck( aOffset1[0], aOffset1[1] ) != 0 )
	{	
		MessageBox("Please key-in hex value for File Offset.","Error", MB_ICONERROR);
		TextBoxOffset1.SetFocus();
		return;	
	}

	TextBoxOffset2.GetWindowText( aOffset2, 4 );
	if( strcmp( aOffset2, "" ) == 0 || HexCheck( aOffset2[0], aOffset2[1] ) != 0 )
	{	
		MessageBox("Please key-in hex value for File Offset.","Error", MB_ICONERROR);
		TextBoxOffset2.SetFocus();
		return;	
	}

	TextBoxLength.GetWindowText( aLength, 4 );
	if( strcmp( aLength, "" ) == 0 || HexCheck( aLength[0], aLength[1] ) != 0 )
	{	
		MessageBox("Please key-in hex value for Length.","Error", MB_ICONERROR);
		TextBoxLength.SetFocus();
		return;	
	}
	
	TextBoxData.GetWindowText( aData, 255);
	if(strcmp(aData, "") == 00)
	{
		MessageBox("Please key-in the data to write.","Error", MB_ICONERROR);
		TextBoxData.SetFocus();
		return;	
	}

	sscanf( aFileId1, "%X", &iTempval );
	uFileId1 = iTempval;

	sscanf( aFileId2, "%X", &iTempval );
	uFileId2 = iTempval;

	sscanf( aOffset1, "%X", &iTempval );
	if( strcmp( aOffset1, "" ) == 0 || HexCheck( aOffset1[0], aOffset1[1] ) != 0 )
	{		
		TextBoxOffset1.SetWindowText( "00" );
		uHiByte = 0x00;	
	}
	else
	{	
		uHiByte = iTempval;	
	}
	
	sscanf( aOffset2, "%X", &iTempval );
	uLoByte = iTempval;

	sscanf( aLength, "%X", &iTempval );

	if(iTempval <= 0 || iTempval > 255)
	{
		MessageBox("Please key-in valid Length. Valid value: 1h - FFh","Error", MB_ICONERROR);
		TextBoxLength.SetFocus();
		return;	
	}

	iTemplen = iTempval;

	//Get Text From GUI copy to aTempArr
	TextBoxData.GetWindowText( aTempdata, MAX );
	
	if(strlen(aTempdata) > iTemplen)
	{
		MessageBox("Length of data is greater than the length specified.","Error", MB_ICONERROR);
		TextBoxData.SetFocus();
		return;	
	}

	//Clear aTempArr
	memset(aTempArr,0,sizeof(aTempArr));
	strcpy((char*)aTempArr,aTempdata);

	try
	{		
		aFileName[0] = uFileId1;
		aFileName[1] = uFileId2;	
		
		displayTitle("\r\nSelect File");

		//Select User File
		pAcos3_->selectFile(aFileName);
		
		displayMessage( "" );

		displayTitle("Write Binary");
		//Write Data to card
		pAcos3_->writeBinary(0,uHiByte,uLoByte,aTempArr,iTemplen);

		TextBoxData.SetWindowText("");
	}
	catch (CardException cardException)
	{
		onCardException(cardException);
	}
	catch (PcscException pcscException)
	{
		onPcscException(pcscException);
	}
}

//Helper

// Display the send command APDU
void onSendCommand(const byte* pSendData, const int iLen)
{
	_pThis->displayMessage((CString)"\r\n<< " + (CString)_cHelper.byteAsString(pSendData, iLen, true));
}

// Display the receive command APDU
void onReceiveCommand(const byte* pReceiveData, const int iLen)
{
	_pThis->displayMessage((CString)">> " + (CString)_cHelper.byteAsString(pReceiveData, iLen, true));
}

//Display the message to the log window
void CBinaryFilesinACOS3Dlg::displayMessage(CString sMessage)
{
    EditBoxLogsValue += sMessage + "\r\n";
	UpdateData(false);
	EditBoxLogs.SetSel(0xFFFF, 0xFFFF);
}

//Display the title to the log window
void CBinaryFilesinACOS3Dlg::displayTitle(CString sTitle)
{
    EditBoxLogsValue += sTitle;
	UpdateData(false);
	EditBoxLogs.SetSel(0xFFFF, 0xFFFF);
}

void CBinaryFilesinACOS3Dlg::OnKillfocusEdit9() 
{
	int iLength;  
	unsigned int uTempval;
	char aLength[4];

	TextBoxLength.GetWindowText( aLength, 4 );
	sscanf( aLength, "%X", &uTempval );
	iLength = uTempval;

	_pThis->TextBoxData.LimitText(iLength);
}

void CBinaryFilesinACOS3Dlg::OnChangeEdit9() 
{
	
	TextBoxData.SetWindowText( "" );
	
}
