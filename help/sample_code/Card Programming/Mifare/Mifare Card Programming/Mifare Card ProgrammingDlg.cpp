//  Copyright(C):      Advanced Card Systems Ltd
//
//  File:              MifareCardProgrammingDlg.cpp
//
//  Description:       This sample program demonstrates how to read data from and write data to
//					   PICC contactless card this also shows how to write to a value block and
//					   increment and decrement its value.
//
//  Author:            Wazer Emmanuel R. Benal
//
//	Date:              June 17, 2008
//
//	Revision Trail:   January 12, 2017 / Anthony Mark G. Tayabas / Added input verifications

//====================================================================================================

#include "stdafx.h"
#include "winscard.h"
#include "Mifare Card Programming.h"
#include "Mifare Card ProgrammingDlg.h"

#include "AcsReader.h"
#include "PcscProvider.h"
#include "PcscException.h"
#include "PcscReader.h"
#include "Helper.h"
#include "MifareClassic.h"
#include "CardSelector.h"
#include "CardException.h"

//Define constants//////////////////////////////////////////////////
#define MAX 262
#define CFG_DEFAULT_READER 2
#define CFG_DEFAULT_SLOT 0
////////////////////////////////////////////////////////////////////

//Mifare Card Programming Inlude File

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//Global Variables
	CMifareCardProgrammingDlg	*pThis = NULL;
	BYTE tempArray[262];
	Helper helper;

	// Function prototypes
	void onSendCommand(const byte* sendData, const int len);
	void onReceiveCommand(const byte* receiveData, const int len);

// Display the send command APDU
void onSendCommand(const byte* sendData, const int len)
{
	char *pStr;

	pStr = helper.byteAsString(sendData, len, true);
	if (pStr == NULL)
	{
		return;
	}

	pThis->displayMessage((CString)"<< " + (CString) pStr);

	delete [] pStr;
}

// Display the receive command APDU
void onReceiveCommand(const byte* receiveData, const int len)
{
	char *pStr;

	pStr = helper.byteAsString(receiveData, len, true);
	if (pStr == NULL)
	{
		return;
	}

	pThis->displayMessage((CString)">> " + (CString) pStr);

	delete [] pStr;
}

//Initializes the buttons and edit boxes
void CMifareCardProgrammingDlg::resetFields()
{
	pThis->ButtonInitialize.EnableWindow( true );
	pThis->ButtonClear.EnableWindow( true );
	pThis->ButtonQuit.EnableWindow( true );
	pThis->ButtonReset.EnableWindow( true );
	pThis->ButtonConnect.EnableWindow( false );
	pThis->ButtonLoadKey.EnableWindow( false );
	pThis->ButtonAuthenticate.EnableWindow( false );
	pThis->ButtonRead.EnableWindow( false );
	pThis->ButtonUpdate.EnableWindow( false );
	pThis->ButtonStoreVal.EnableWindow( false );
	pThis->ButtonInc.EnableWindow( false );
	pThis->ButtonDec.EnableWindow( false );
	pThis->ButtonReadVal.EnableWindow( false );
	pThis->ButtonRestoreVal.EnableWindow( false );


	pThis->TextBoxKeyStore.EnableWindow( false );
	pThis->TextBoxKeyVal1.EnableWindow( false );
	pThis->TextBoxKeyVal2.EnableWindow( false );
	pThis->TextBoxKeyVal3.EnableWindow( false );
	pThis->TextBoxKeyVal4.EnableWindow( false );
	pThis->TextBoxKeyVal5.EnableWindow( false );
	pThis->TextBoxKeyVal6.EnableWindow( false );
	pThis->TextBoxBlock.EnableWindow( false );
	pThis->TextBoxKeyStoreNo.EnableWindow( false );
	pThis->TextBoxStartBlock.EnableWindow( false );
	pThis->TextBoxLen.EnableWindow( false );
	pThis->TextBoxValue.EnableWindow( false );
	pThis->TextBoxBlockNo.EnableWindow( false );
	pThis->TextBoxSource.EnableWindow( false );
	pThis->TextBoxTarget.EnableWindow( false );
	pThis->TextBoxData.EnableWindow( false );
	
	pThis->RadioButtonKeyA.EnableWindow( false );
	pThis->RadioButtonKeyB.EnableWindow( false );	
	
	pThis->TextBoxKeyStore.SetWindowText( "" );
	pThis->TextBoxKeyVal1.SetWindowText( "" );
	pThis->TextBoxKeyVal2.SetWindowText( "" );
	pThis->TextBoxKeyVal3.SetWindowText( "" );
	pThis->TextBoxKeyVal4.SetWindowText( "" );
	pThis->TextBoxKeyVal5.SetWindowText( "" );
	pThis->TextBoxKeyVal6.SetWindowText( "" );
	pThis->TextBoxBlock.SetWindowText( "" );
	pThis->TextBoxKeyStoreNo.SetWindowText( "" );
	pThis->TextBoxStartBlock.SetWindowText( "" );
	pThis->TextBoxLen.SetWindowText( "" );
	pThis->TextBoxValue.SetWindowText( "" );
	pThis->TextBoxBlockNo.SetWindowText( "" );
	pThis->TextBoxSource.SetWindowText( "" );
	pThis->TextBoxTarget.SetWindowText( "" );
	pThis->TextBoxData.SetWindowText( "" );

	pThis->RadioButtonKeyA.SetCheck( true );
	pThis->RadioButtonKeyB.SetCheck( false );	

	pThis->TextBoxKeyStore.SetLimitText( 2 );
	pThis->TextBoxKeyVal1.SetLimitText( 2 );
	pThis->TextBoxKeyVal2.SetLimitText( 2 );
	pThis->TextBoxKeyVal3.SetLimitText( 2 );
	pThis->TextBoxKeyVal4.SetLimitText( 2 );
	pThis->TextBoxKeyVal5.SetLimitText( 2 );
	pThis->TextBoxKeyVal6.SetLimitText( 2 );
	pThis->TextBoxBlock.SetLimitText( 3 );
	pThis->TextBoxKeyStoreNo.SetLimitText( 2 );
	pThis->TextBoxStartBlock.SetLimitText( 3 );
	pThis->TextBoxLen.SetLimitText( 2 );
	pThis->TextBoxBlockNo.SetLimitText( 3 );
	pThis->TextBoxSource.SetLimitText( 3 );
	pThis->TextBoxTarget.SetLimitText( 3 );
	pThis->TextBoxValue .SetLimitText (10);

	pThis->EditboxResultText = "";
	addMessageToLog("Program Ready");
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
// CMifareCardProgrammingDlg dialog

CMifareCardProgrammingDlg::CMifareCardProgrammingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMifareCardProgrammingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMifareCardProgrammingDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIconBig   = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIconSmall = AfxGetApp()->LoadIcon(IDR_SMALLICON);
}	

void CMifareCardProgrammingDlg::enableLoadControls(bool bEnable)
{
	TextBoxKeyStore.EnableWindow(bEnable);
	TextBoxKeyVal1.EnableWindow(bEnable);
	TextBoxKeyVal2.EnableWindow(bEnable);
	TextBoxKeyVal3.EnableWindow(bEnable);
	TextBoxKeyVal4.EnableWindow(bEnable);
	TextBoxKeyVal5.EnableWindow(bEnable);
	TextBoxKeyVal6.EnableWindow(bEnable);
	ButtonLoadKey.EnableWindow(bEnable);
}

void CMifareCardProgrammingDlg::enableAuthenticationControls(bool bEnable)
{
	RadioButtonKeyA.EnableWindow(bEnable);
	RadioButtonKeyB.EnableWindow(bEnable);
	TextBoxBlockNo.EnableWindow(bEnable);
	TextBoxKeyStoreNo.EnableWindow(bEnable);
	ButtonAuthenticate.EnableWindow(bEnable);
}

void CMifareCardProgrammingDlg::enableBlockFunctionControls(bool bEnable)
{
	TextBoxStartBlock.EnableWindow(bEnable);
	TextBoxLen.EnableWindow(bEnable);
	TextBoxData.EnableWindow(bEnable);
	ButtonRead.EnableWindow(bEnable);
	ButtonUpdate.EnableWindow(bEnable);
	TextBoxValue.EnableWindow(bEnable);
	TextBoxBlock.EnableWindow(bEnable);
	TextBoxSource.EnableWindow(bEnable);
	TextBoxTarget.EnableWindow(bEnable);
	ButtonStoreVal.EnableWindow(bEnable);
	ButtonInc.EnableWindow(bEnable);
	ButtonDec.EnableWindow(bEnable);
	ButtonReadVal.EnableWindow(bEnable);
	ButtonRestoreVal.EnableWindow(bEnable);
}
void CMifareCardProgrammingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMifareCardProgrammingDlg)
	DDX_Control(pDX, IDC_RICHEDIT1, EditboxResult);
	DDX_Control(pDX, IDC_BUTTON2, ButtonConnect);
	DDX_Control(pDX, IDC_BUTTON1, ButtonInitialize);
	DDX_Control(pDX, IDC_BUTTON11, ButtonRestoreVal);
	DDX_Control(pDX, IDC_BUTTON10, ButtonReadVal);
	DDX_Control(pDX, IDC_BUTTON9, ButtonDec);
	DDX_Control(pDX, IDC_BUTTON8, ButtonInc);
	DDX_Control(pDX, IDC_BUTTON5, ButtonStoreVal);
	DDX_Control(pDX, IDC_BUTTON3, ButtonLoadKey);
	DDX_Control(pDX, IDC_BUTTON4, ButtonAuthenticate);
	DDX_Control(pDX, IDC_BUTTON7, ButtonUpdate);
	DDX_Control(pDX, IDC_BUTTON6, ButtonRead);
	DDX_Control(pDX, IDC_BUTTON14, ButtonQuit);
	DDX_Control(pDX, IDC_BUTTON13, ButtonReset);
	DDX_Control(pDX, IDC_BUTTON12, ButtonClear);
	DDX_Control(pDX, IDC_RADIO7, RadioButtonKeyB);
	DDX_Control(pDX, IDC_RADIO6, RadioButtonKeyA);
	DDX_Control(pDX, IDC_EDIT22, TextBoxTarget);
	DDX_Control(pDX, IDC_EDIT21, TextBoxSource);
	DDX_Control(pDX, IDC_EDIT20, TextBoxBlock);
	DDX_Control(pDX, IDC_EDIT19, TextBoxValue);	
	DDX_Control(pDX, IDC_EDIT18, TextBoxData);
	DDX_Control(pDX, IDC_EDIT17, TextBoxLen);
	DDX_Control(pDX, IDC_EDIT16, TextBoxStartBlock);	
	DDX_Control(pDX, IDC_EDIT9, TextBoxKeyStoreNo);
	DDX_Control(pDX, IDC_EDIT8, TextBoxBlockNo);	
	DDX_Control(pDX, IDC_EDIT7, TextBoxKeyVal6);
	DDX_Text(pDX, IDC_EDIT7, TextBoxValString6);
	DDX_Control(pDX, IDC_EDIT6, TextBoxKeyVal5);
	DDX_Text(pDX, IDC_EDIT6, TextBoxValString5);
	DDX_Control(pDX, IDC_EDIT5, TextBoxKeyVal4);
	DDX_Text(pDX, IDC_EDIT5, TextBoxValString4);
	DDX_Control(pDX, IDC_EDIT4, TextBoxKeyVal3);
	DDX_Text(pDX, IDC_EDIT4, TextBoxValString3);
	DDX_Control(pDX, IDC_EDIT2, TextBoxKeyVal1);
	DDX_Text(pDX, IDC_EDIT2, TextBoxValString1);
	DDX_Control(pDX, IDC_EDIT3, TextBoxKeyVal2);
	DDX_Text(pDX, IDC_EDIT3, TextBoxValString2);
	DDX_Control(pDX, IDC_EDIT1, TextBoxKeyStore);
	DDX_Control(pDX, IDC_COMBO1, ComboboxReader);	
	DDX_Text(pDX, IDC_RICHEDIT1, EditboxResultText);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMifareCardProgrammingDlg, CDialog)
	//{{AFX_MSG_MAP(CMifareCardProgrammingDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON2, OnConnect)
	ON_BN_CLICKED(IDC_BUTTON12, OnClear)
	ON_BN_CLICKED(IDC_BUTTON13, OnReset)
	ON_BN_CLICKED(IDC_BUTTON14, OnQuit)
	ON_BN_CLICKED(IDC_RADIO6, OnKeyA)
	ON_BN_CLICKED(IDC_RADIO7, OnKeyB)	
	ON_BN_CLICKED(IDC_BUTTON3, OnLoadKey)
	ON_BN_CLICKED(IDC_BUTTON4, OnAuthen)
	ON_BN_CLICKED(IDC_BUTTON6, OnReadBlock)
	ON_BN_CLICKED(IDC_BUTTON7, OnUpdateBlock)
	ON_BN_CLICKED(IDC_BUTTON5, OnStoreVal)
	ON_BN_CLICKED(IDC_BUTTON8, OnInc)
	ON_BN_CLICKED(IDC_BUTTON9, OnDec)
	ON_BN_CLICKED(IDC_BUTTON10, OnReadVal)
	ON_BN_CLICKED(IDC_BUTTON11, OnRestore)
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonInitialize)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	ON_EN_CHANGE(IDC_EDIT3, OnChangeEdit3)
	ON_EN_CHANGE(IDC_EDIT4, OnChangeEdit4)
	ON_EN_CHANGE(IDC_EDIT5, OnChangeEdit5)
	ON_EN_CHANGE(IDC_EDIT6, OnChangeEdit6)
	ON_EN_CHANGE(IDC_EDIT7, OnChangeEdit7)
	ON_EN_CHANGE(IDC_EDIT17, OnChangeEdit17)
	ON_EN_KILLFOCUS(IDC_EDIT17, OnKillfocusEdit17)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMifareCardProgrammingDlg message handlers

BOOL CMifareCardProgrammingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	pThis = this;

	//Initialize Structures in MifareCardProgrammingDlg;
	pcscReader = new PcscReader();
	pMifare_ = NULL;

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
	
	resetFields();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMifareCardProgrammingDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMifareCardProgrammingDlg::OnPaint() 
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
HCURSOR CMifareCardProgrammingDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIconSmall;
}

void CMifareCardProgrammingDlg::OnButtonInitialize()
{
	string *pReaderList = new string[10];
	uint8 uReaderCount = 10;
	int readerNameLength = 0;
	int i = 0;

	try
	{
		ComboboxReader.ResetContent();
		

		// Establish Context
		pcscReader->initialize();

		// List the detected smart card readers
		pcscReader->listReaders(pReaderList, uReaderCount);


		// Load the retrieved reader name array to the reader list
		for(i = 0; i < uReaderCount; i++)
		{
			ComboboxReader.AddString(pReaderList[i].data());
		}
		
		ComboboxReader.SetCurSel(0);

		// Pass the function pointers to serve as the event listeners of the ProprietaryReader class
		pcscReader->setSendCommandFunctor(onSendCommand);
		pcscReader->setRecvCommandFunctor(onReceiveCommand);					

		EditboxResult.Clear();
        addMessageToLog("\r\nInitialize success");
		ButtonConnect.EnableWindow(true);
	}
	catch (PcscException ex)
	{
		addTitleToLog(ex.what());
		showErrorMessage(ex.what());
	}
}

void CMifareCardProgrammingDlg::OnConnect() 
{
	CString readerName;
	char message[100];

	uint8 aAtr[100];
	uint32 uAtrLen = sizeof(aAtr);
	CardSelector *pCardSelector;
	
	ComboboxReader.GetWindowText(readerName);

	try
	{
		// Check if a reader name is selected
		if (!strcmp(readerName, ""))
		{
			showErrorMessage("Select a smartcard reader.");
			return;
		}

		// Try to connect to the smartcard through the specified reader name
		if (pcscReader->connect((std::string)readerName) != 0)
			return;
				

		sprintf(message, "%s %s", "\r\Successfully connected to ", readerName);
		addMessageToLog(message);

		pMifare_ = new MifareClassic(pcscReader);
		if (pMifare_ == NULL)
		{
			showErrorMessage("Insufficient resources for new Mifare Classic Card");
			return;
		}

		pcscReader->getStatus();
		pcscReader->getAtr(aAtr, uAtrLen);

		pCardSelector = new CardSelector(pcscReader);

		sCardName = pCardSelector->readCardType(aAtr, uAtrLen);

		if (sCardName != "Mifare Standard 1K" && sCardName != "Mifare Standard 4K")
		{
			showErrorMessage("Card not supported.\r\nPlease present Mifare Classic card.");
			enableLoadControls(false);
			return;
		}

		sprintf(message, "%s %s", "Chip Type: ", sCardName);
		addMessageToLog(message);

		enableLoadControls(true);
		
	}
	catch (PcscException ex)
	{
		addTitleToLog(ex.what());
		showErrorMessage(ex.what());
	}
}

// Display the message to the log window
void CMifareCardProgrammingDlg::addTitleToLog(CString sMessage)
{
	EditboxResultText += "\r\n" + sMessage + "\r\n";
	UpdateData(false);

	EditboxResult.SetSel(0xFFFF, 0xFFFF);
}

void CMifareCardProgrammingDlg::addMessageToLog(CString sMessage)
{
	EditboxResultText += sMessage + "\r\n";
	UpdateData(false);

	EditboxResult.SetSel(0xFFFF, 0xFFFF);
}

void CMifareCardProgrammingDlg::showErrorMessage(CString sMessage)
{
	::MessageBox(0, sMessage, "Error", MB_OK|MB_ICONSTOP);
}

void CMifareCardProgrammingDlg::showWarningMessage(CString sMessage)
{
	::MessageBox(0, sMessage, "Warning", MB_OK|MB_ICONEXCLAMATION);
}

void CMifareCardProgrammingDlg::displayMessage(CString message)
{
	EditboxResultText += message + "\r\n";
	UpdateData(false);

	EditboxResult.SetSel(0xFFFF, 0xFFFF);
}

void CMifareCardProgrammingDlg::OnClear() 
{
	EditboxResultText = "";
	UpdateData(false);

	EditboxResult.SetSel(0xFFFF, 0xFFFF);
}

void CMifareCardProgrammingDlg::OnReset() 
{
	if (pcscReader == NULL)
	{
		return;
	}

	if (pcscReader->isConnectionActive())
	{
		pcscReader->disconnect();
	}

	if (pMifare_!= NULL)
	{
		delete pMifare_;
		pMifare_ = NULL;
	}	
	
	ComboboxReader.ResetContent();
	resetFields();
}

void CMifareCardProgrammingDlg::OnQuit() 
{	
	OnCancel();
}

void CMifareCardProgrammingDlg::OnKeyA() 
{
	RadioButtonKeyA.SetCheck( true );
	RadioButtonKeyB.SetCheck( false );
}

void CMifareCardProgrammingDlg::OnKeyB() 
{
	RadioButtonKeyA.SetCheck( false );
	RadioButtonKeyB.SetCheck( true );
}

// Validate input to an editbox control
// to allow hexadecinal values only
void CMifareCardProgrammingDlg::validateInput(CEdit *editbox, CString *text)
{
	char buffer[18];

	UpdateData(true);

	editbox->GetWindowText(buffer, 18);

	if (!strstr ("0123456789ABCDEFabcdef", &buffer[strlen(buffer) - 1]))
	{
		buffer[strlen(buffer) - 1] = 0x00;
		*text = buffer;
		UpdateData(false);
		editbox->SetSel(text->GetLength(), text->GetLength());
	}
}

void CMifareCardProgrammingDlg::OnChangeEdit2()
{
	validateInput(&TextBoxKeyVal1, &TextBoxValString1);	
}

void CMifareCardProgrammingDlg::OnChangeEdit3()
{
	validateInput(&TextBoxKeyVal2, &TextBoxValString2);
}

void CMifareCardProgrammingDlg::OnChangeEdit4()
{
	validateInput(&TextBoxKeyVal3, &TextBoxValString3);
}

void CMifareCardProgrammingDlg::OnChangeEdit5()
{
	validateInput(&TextBoxKeyVal4, &TextBoxValString4);
}

void CMifareCardProgrammingDlg::OnChangeEdit6()
{
	validateInput(&TextBoxKeyVal5, &TextBoxValString5);
}

void CMifareCardProgrammingDlg::OnChangeEdit7()
{
	validateInput(&TextBoxKeyVal6, &TextBoxValString6);
}

void CMifareCardProgrammingDlg::OnChangeEdit17()
{
	char buffer[4];
	TextBoxLen.GetWindowText( buffer, 4 );
	if(strcmp( buffer, "16" ) == 0)
		TextBoxData.LimitText(16);
	if(strcmp( buffer, "32" ) == 0)
		TextBoxData.LimitText(32);
	if(strcmp( buffer, "48" ) == 0)
		TextBoxData.LimitText(48);
}

void ClearBuffers()
{
	int index;
	
	for( index = 0; index <= 262; index++ )
	{	
		tempArray[index] = 0x00;
	}
}

void CMifareCardProgrammingDlg::OnLoadKey() 
{	
	//byte* key;
	byte* keyVal1;
	byte* keyVal2;
	byte* keyVal3;
	byte* keyVal4;
	byte* keyVal5;
	byte* keyVal6;
	byte keyNumber = 0x20,temp[12];	
	char buffer[20];
	bool inputError = false;
	int iKeyStoreSize = 2;
	//char keyValue[20];
	int i;

    try
    {
		memset(buffer, NULL, 20);
		TextBoxKeyStore.GetWindowText( buffer, 20 );
			
		if(!strcmp(buffer, ""))
			inputError = true;			
		if( atoi( buffer ) < 0 )
			inputError = true;
		else if( atoi( buffer ) > 1 )
			inputError = true;	
			
		if (inputError == true)
        {				
			showErrorMessage("Please key-in Key Store Number from 00 to 01.");
            TextBoxKeyStore.SetFocus();
			return;
		}

		for (i = 0; i < iKeyStoreSize; i++)
		{
			if (!isdigit(buffer[i]) && buffer[i] != NULL)
			{
				showErrorMessage("Please key-in Key Store Number from 00 to 01.");
				TextBoxKeyStore.SetFocus();
				return;
			}
		}

		keyNumber = (byte) atoi(buffer);
		
		TextBoxKeyVal1.GetWindowText( buffer, 20 );
		keyVal1 = helper.getBytes(buffer);
		if(!strcmp(buffer, ""))
			inputError = true;			
		if( strlen(buffer) != 2 )
			inputError = true;

		TextBoxKeyVal2.GetWindowText( buffer, 20 );
		keyVal2 = helper.getBytes(buffer);
		if(!strcmp(buffer, ""))
			inputError = true;			
		if( strlen(buffer) != 2 )
			inputError = true;

		TextBoxKeyVal3.GetWindowText( buffer, 20 );
		keyVal3 = helper.getBytes(buffer);
		if(!strcmp(buffer, ""))
			inputError = true;			
		if( strlen(buffer) != 2 )
			inputError = true;

		TextBoxKeyVal4.GetWindowText( buffer, 20 );
		keyVal4 = helper.getBytes(buffer);
		if(!strcmp(buffer, ""))
			inputError = true;			
		if( strlen(buffer) != 2 )
			inputError = true;

		TextBoxKeyVal5.GetWindowText( buffer, 20 );
		keyVal5 = helper.getBytes(buffer);
		if(!strcmp(buffer, ""))
			inputError = true;			
		if( strlen(buffer) != 2 )
			inputError = true;

		TextBoxKeyVal6.GetWindowText( buffer, 20 );
		keyVal6 = helper.getBytes(buffer);
		if(!strcmp(buffer, ""))
			inputError = true;			
		if( strlen(buffer) != 2 )
			inputError = true;
	
		if (inputError == true)
		{				
			showErrorMessage("Please key-in hex value for Key Value.");
			TextBoxKeyVal1.SetFocus();
			return;
		}
		else
		{
			for (i = 0; i < 2; i++)
			temp[i] = keyVal1[i];
			for (i = 0; i < 2; i++)
			temp[i+1] = keyVal2[i];
			for (i = 0; i < 2; i++)
			temp[i+2] = keyVal3[i];
			for (i = 0; i < 2; i++)
			temp[i+3] = keyVal4[i];
			for (i = 0; i < 2; i++)
			temp[i+4] = keyVal5[i];
			for (i = 0; i < 2; i++)
			temp[i+5] = keyVal6[i];

		}		
		
		
		addTitleToLog("Load Authentication Key");
		pMifare_->loadAuthKey(keyNumber, temp);
        addMessageToLog("Load Key success");

		enableAuthenticationControls(true);
		pThis->ButtonAuthenticate.EnableWindow( true );
		pThis->TextBoxBlockNo.EnableWindow( true );
		pThis->RadioButtonKeyA.EnableWindow( true );
		pThis->RadioButtonKeyB.EnableWindow( true );		
	}
    catch (CardException cardException)
	{
		char message[100];
		sprintf(message, "%s%s%s%s", "[", helper.byteAsString(cardException.getStatusWord(),2, true), "] ", cardException.getMessage() );
		addTitleToLog(message);
		showErrorMessage(cardException.getMessage());
		
	}
	catch (PcscException pcscException)
	{
		addTitleToLog(pcscException.what());
		showErrorMessage(pcscException.what());
	}

}

void CMifareCardProgrammingDlg::OnAuthen() 
{
	byte blockNumber = 0x00;	
    MIFARE_KEY_TYPE eKeyType = KEY_TYPE_A;

	//For Non-Volatile  : 0x00-0x1F
    //For Volatile      : 0x20
    byte keyNumber = 0x20;

	char keyStoreNum[20];
	char blockNum[20];		
	bool inputError = false;

	int i;
	int iBlockNumberSize = 3;
	int iKeyStoreSize = 2;

    try
    {
		memset(blockNum, NULL, 20);
		memset(keyStoreNum, NULL, 20);

		TextBoxBlockNo.GetWindowText( blockNum , 20 );
		TextBoxKeyStoreNo.GetWindowText( keyStoreNum, 20 );

		if(!strcmp(blockNum, ""))
		{
			showErrorMessage("Please key-in numeric value for Block Number.");
            TextBoxBlockNo.SetFocus();
            return;
		}

		if (!strcmp(keyStoreNum, ""))
		{
			showErrorMessage("Please key-in Key Store Number from 00 to 01.");
            TextBoxKeyStore.SetFocus();
            return;
		}

		for (i = 0; i < iBlockNumberSize; i++)
		{
			if (!isdigit(blockNum[i]) && blockNum[i] != NULL)
			{
				showErrorMessage("Please key-in numeric value for Block Number.");
				TextBoxBlockNo.SetFocus();
				return;
			}
		}

		for (i = 0; i < iKeyStoreSize; i++)
		{
			if (!isdigit(keyStoreNum[i]) && keyStoreNum[i] != NULL)
			{
				showErrorMessage("Please key-in Key Store Number from 00 to 01.");
				TextBoxKeyStore.SetFocus();
				return;
			}
		}
		

		if(sCardName == "Mifare Standard 1K")
		{
			if( atoi( blockNum ) < 0 || atoi( blockNum ) > 63 )
			{
				showErrorMessage("Please key-in valid Block Number. Valid value: 0 - 63.");
				TextBoxBlockNo.SetFocus();
				return;
			}
		}
		else if(sCardName == "Mifare Standard 4K")
		{
			if( atoi( blockNum ) < 0 || atoi( blockNum ) > 255 )
			{
				showErrorMessage("Please key-in valid Block Number. Valid value: 0 - 255.");
				TextBoxBlockNo.SetFocus();
				return;
			}
		}

		if( atoi( keyStoreNum ) < 0 )
			inputError = true;
		else if( atoi( keyStoreNum ) > 1 )
			inputError = true;	
			
		if (inputError == true)
        {				
			showErrorMessage("Please key-in Key Store Number from 00 to 01.");
            TextBoxKeyStoreNo.SetFocus();
            return;
		}

		keyNumber = (byte) atoi(keyStoreNum);

		if ((RadioButtonKeyB.GetCheck() == TRUE))
			eKeyType = KEY_TYPE_B;

		/*if (eChipType_ == CHIP_TYPE_MIFARE_CLASSIC_4K)
		{
			if ( atoi( blockNum ) > 255 )
			{			
				char errorMessage[100]; 
				sprintf(errorMessage, "%s%s", "Card does not have block ", blockNum); 
				MessageBox(errorMessage);				
                TextBoxBlockNo.SetFocus();
                return;
			}

		}
        else
        {
			if (atoi ( blockNum ) > 128)
            {
				char errorMessage[100]; 
				sprintf(errorMessage, "%s%s", "Card does not have block ", blockNum); 
				MessageBox(errorMessage);				
				TextBoxBlockNo.SetFocus();
                return;
			}
			
		}*/

		blockNumber = (byte) atoi(blockNum);
	
		//TODOBREY acr1281UC1->authenticate(atoi(blockNum), keyType, keyNumber);

		addTitleToLog("Authenticate Key");
		pMifare_->authenticate(blockNumber, eKeyType, keyNumber);
        addMessageToLog("Authenticate success");

		
		pThis->ButtonRead.EnableWindow( true );
		pThis->ButtonUpdate.EnableWindow( true );
		pThis->TextBoxStartBlock.EnableWindow( true );
		pThis->TextBoxLen.EnableWindow( true );
		pThis->ButtonStoreVal.EnableWindow( true );
		pThis->ButtonInc.EnableWindow( true );
		pThis->ButtonDec.EnableWindow( true );
		pThis->ButtonReadVal.EnableWindow( true );
		pThis->ButtonRestoreVal.EnableWindow( true );
		pThis->TextBoxValue.EnableWindow( true );
		pThis->TextBoxBlock.EnableWindow( true );
		pThis->TextBoxSource.EnableWindow( true );
		pThis->TextBoxTarget.EnableWindow( true );
		pThis->TextBoxData.EnableWindow( true );
		enableBlockFunctionControls(true);
	}
    catch (CardException cardException)
	{
		char message[100];
		sprintf(message, cardException.getMessage() );
		addTitleToLog(message);
		showErrorMessage(cardException.getMessage());
	}
	catch (PcscException pcscException)
	{
		addTitleToLog(pcscException.what());
		showErrorMessage(pcscException.what());
	}
	catch (...)
	{
		addTitleToLog("Authenticate failed");	
		showErrorMessage("Authenticate failed");
	}
}

void CMifareCardProgrammingDlg::OnReadBlock() 
{
//	char buffer[4],blockNum[4],tempStr[MAX];
	char buffer[4],tempStr[MAX];
	int index, tempval,tempLen,blockNumber;
	//byte* tempByte;
	byte* tempArray;

	int i;
	int iBufferSize =4;

	try
	{
		TextBoxData.SetWindowText( "" );

		//Validate inputs
		memset( buffer, NULL, iBufferSize );
		TextBoxStartBlock.GetWindowText( buffer, iBufferSize );
		sscanf( buffer, "%d", &tempval );
		if( strcmp( buffer, "" ) == 0 )
		{
			showErrorMessage("Please key-in numeric value for Start Block Number.");
			TextBoxStartBlock.SetFocus();
			return;
		}

		if(sCardName == "Mifare Standard 1K")
		{
			if( tempval > 63 )
			{
				showErrorMessage("Please key-in valid Start Block. Valid value: 0 - 63.");
				TextBoxStartBlock.SetFocus();
				return;
			}
		}
		else if(sCardName == "Mifare Standard 4K")
		{
			if( tempval > 255 )
			{
				showErrorMessage("Please key-in valid Start Block. Valid value: 0 - 255.");
				TextBoxStartBlock.SetFocus();
				return;
			}
		}

		for (i = 0; i < iBufferSize; i++)
		{
			if (!isdigit(buffer[i]) && buffer[i] != NULL)
			{
				showErrorMessage("Please key-in numeric value for Start Block Number.");
				TextBoxStartBlock.SetFocus();
				return;
			}
		}

		TextBoxLen.GetWindowText( buffer, iBufferSize );
		if( strcmp( buffer, "" ) == 0 )
		{
			showErrorMessage("Please key-in numeric value for Length.");
			TextBoxLen.SetFocus();
			return;
		}

		for (i = 0; i < iBufferSize; i++)
		{
			if (!isdigit(buffer[i]) && buffer[i] != NULL)
			{
				showErrorMessage("Please key-in numeric value for Length.");
				TextBoxLen.SetFocus();
				return;
			}
		}

		TextBoxStartBlock.GetWindowText( buffer, iBufferSize );
		sscanf( buffer, "%d", &blockNumber );		

		TextBoxLen.GetWindowText( buffer, iBufferSize );
		sscanf( buffer, "%d", &tempLen );

		if(tempLen <= 0 || tempLen > 16)
		{
			showErrorMessage("Invalid Length. Valid value: 1 - 16.");
			TextBoxLen.SetFocus();
			return;
		}

		addTitleToLog("Read Binary");

		//TODOBREY tempArray = mifareClassic->readBinary(blockNumber,tempLen);
		tempArray = pMifare_->readBinary(blockNumber, tempLen);


		sprintf( tempStr, "" );
		for( index = 0; index != tempLen; index++ )
		{	
			sprintf( tempStr, "%s%c", tempStr, tempArray[index] );	
		}

		TextBoxData.SetWindowText( tempStr );

		addMessageToLog("Read success");

	}
	catch (CardException cardException)
	{
		char message[100];
		//sprintf(message, "%s%s%s%s", "[", helper.byteAsString(cardException.getStatusWord(),2, true), "] ", cardException.getMessage() );
		sprintf(message, "%s%s%s%s", "[", "63 00", "] ", cardException.getMessage() );
		addTitleToLog(message);
		showErrorMessage(cardException.getMessage());
	}
	catch (PcscException pcscException)
	{
		addTitleToLog(pcscException.what());
		showErrorMessage(pcscException.what());
	}
	catch (...)
	{
		addTitleToLog("Read Failed");
		showErrorMessage("Read Failed");
	}

}

void CMifareCardProgrammingDlg::OnUpdateBlock() 
{	
	char tempStr[MAX],buffer[50];
	int index, tempval,blockNumber;
	int tempLen;

	int i;
	int iBufferSize = 50;
	
	try
	{
		//Validate inputs
		memset( buffer, NULL, iBufferSize );
		TextBoxStartBlock.GetWindowText( buffer, iBufferSize );

		sscanf( buffer, "%d", &tempval );
		if( strcmp( buffer, "" ) == 0 )
		{
			showErrorMessage("Please key-in numeric value for Start Block.");
			TextBoxStartBlock.SetFocus();
			return;
		}

		if(sCardName == "Mifare Standard 1K")
		{
			if( tempval > 63 )
			{
				showErrorMessage("Please key-in valid Start Block. Valid value: 0 - 63.");
				TextBoxStartBlock.SetFocus();
				return;
			}
		}
		else if(sCardName == "Mifare Standard 4K")
		{
			if( tempval > 255 )
			{
				showErrorMessage("Please key-in valid Start Block. Valid value: 0 - 255.");
				TextBoxStartBlock.SetFocus();
				return;
			}
		}
		
		for (i = 0; i < iBufferSize; i++)
		{
			if (!isdigit(buffer[i]) && buffer[i] != NULL)
			{
				showErrorMessage("Please key-in numeric value for Start Block Number.");
				TextBoxStartBlock.SetFocus();
				return;
			}
		}

		if (tempval <= 127)
		{
			if ((tempval + 1) % 4 == 0)
			{
				showWarningMessage("The block to be updated is a sector trailer. Please change the Start Block Number.");
				TextBoxStartBlock.SetFocus();
				return;
			}
		}
		
		if (tempval > 127)
		{
			if ((tempval + 1) % 16 == 0)
			{
				showWarningMessage("The block to be updated is a sector trailer. Please change the Start Block Number.");
				TextBoxStartBlock.SetFocus();
				return;
			}
		}

		memset( buffer, NULL, iBufferSize );
		TextBoxData.GetWindowText( buffer, 50 );

		if( strcmp( buffer, "" ) == 0 )
		{
			showErrorMessage("Please key-in data to write.");
			TextBoxData.SetFocus();
			return;
		}

		memset( buffer, NULL, iBufferSize );
		TextBoxLen.GetWindowText( buffer, iBufferSize );

		if( strcmp( buffer, "" ) == 0 )
		{
			showErrorMessage("Please key-in numeric value for Length.");
			TextBoxLen.SetFocus();
			return;
		}

		for (i = 0; i < iBufferSize; i++)
		{
			if (!isdigit(buffer[i]) && buffer[i] != NULL)
			{
				showErrorMessage("Please key-in numeric value for Length.");
				TextBoxLen.SetFocus();
				return;
			}
		}

		//First put the input in a char array then 
		//put it into a int variable using sscanf
		TextBoxStartBlock.GetWindowText( buffer, 4 );
		sscanf( buffer, "%d", &blockNumber );

		TextBoxLen.GetWindowText( buffer, 4 );
		sscanf( buffer, "%d", &tempLen );

		if (tempLen != 16)
		{
			showErrorMessage("Invalid Length. Length must be 16.");
			TextBoxLen.SetFocus();
			return;
		}

		TextBoxData.GetWindowText( tempStr, 50 );
		ClearBuffers();
		for( index = 0; index < strlen( tempStr ); index++ )
		{
			tempArray[index] = int( tempStr[index] );		
		}
		
		addTitleToLog("Update Binary");

		//write to card
		//TODOBREY mifareClassic->updateBinary(blockNumber,tempArray,tempLen);
		pMifare_->updateBinary(blockNumber,tempArray,tempLen);

		addMessageToLog("Update success");

		TextBoxData.SetWindowText( "" );
	}
	catch (CardException cardException)
	{
		char message[100];
		//sprintf(message, "%s%s%s%s", "[", helper.byteAsString(cardException.getStatusWord(),2, true), "] ", cardException.getMessage() );
		sprintf(message, "%s%s%s%s", "[", "63 00", "] ", cardException.getMessage() );
		addTitleToLog(message);
		showErrorMessage(cardException.getMessage());
		
	}
	catch (PcscException pcscException)
	{
		showErrorMessage(pcscException.what());
		addTitleToLog(pcscException.what());
	}
	catch (...)
	{
		addTitleToLog("Update failed");
		showErrorMessage("Update failed");
	}
}

void CMifareCardProgrammingDlg::OnStoreVal() 
{
	char holder[11];
	int tempval2;
	long int tempval1;
	DWORD Amount;

	int i;
	int iBufferSize = 11;
	int iMaxAmountDigit = 11;

	try
	{
		//Validate Inputs
		memset( holder, NULL, iBufferSize );
		TextBoxValue.GetWindowText( holder, iMaxAmountDigit );

		tempval1 = strtol(holder,NULL,10);

		if( strcmp( holder, "" ) == 0 )
		{	
			showErrorMessage("Please key-in numeric value for Amount.");
			TextBoxValue.SetFocus();
			return;	
		}
		else if( tempval1 > 2147483647 || tempval1 <= 0)
		{	
			showErrorMessage("Please key-in valid Amount. Valid value: 1 - 2,147,483,647.");
			TextBoxValue.SetFocus();
			return;	
		}

		for (i = 0; i < iBufferSize; i++)
		{
			if (!isdigit(holder[i]) && holder[i] != NULL)
			{
				showErrorMessage("Please key-in numeric value for Amount.");
				TextBoxValue.SetFocus();
				return;
			}
		}

		memset( holder, NULL, iBufferSize );
		TextBoxBlock.GetWindowText( holder, 4 );
		sscanf( holder, "%d", &tempval2 );

		if( strcmp( holder, "" ) == 0 )
		{	
			showErrorMessage("Please key-in numeric value for Block Number.");
			TextBoxBlock.SetFocus();
			return;	
		}

		if(sCardName == "Mifare Standard 1K")
		{
			if( tempval2 > 63 )
			{
				showErrorMessage("Please key-in valid Block Number. Valid value: 0 - 63.");
				TextBoxBlock.SetFocus();
				return;	
			}
		}
		else if(sCardName == "Mifare Standard 4K")
		{
			if( tempval2 > 255 )
			{
				showErrorMessage("Please key-in valid Block Number. Valid value: 0 - 255.");
				TextBoxBlock.SetFocus();
				return;	
			}
		}
	
		if (tempval2 <= 127)
		{
			if ((tempval2 + 1) % 4 == 0)
			{
				showWarningMessage("The block to be updated is a sector trailer. Please change the Start Block Number.");
				TextBoxBlock.SetFocus();
				return;
			}
		}
		
		if (tempval2 > 127)
		{
			if ((tempval2 + 1) % 16 == 0)
			{
				showWarningMessage("The block to be updated is a sector trailer. Please change the Start Block Number.");
				TextBoxBlock.SetFocus();
				return;
			}
		}

		for (i = 0; i < 4; i++)
		{
			if (!isdigit(holder[i]) && holder[i] != NULL)
			{
				showErrorMessage("Please key-in numeric value for Block Number.");
				TextBoxBlock.SetFocus();
				return;
			}
		}

		TextBoxSource.SetWindowText( "" );
		TextBoxTarget.SetWindowText( "" );

		//First put the input in a char array then 
		//put it into a int variable using sscanf
		TextBoxValue.GetWindowText( holder, 12 );
		sscanf( holder, "%lu", &tempval1 );
		Amount = tempval1;
		
		TextBoxBlock.GetWindowText( holder, 4 );
		sscanf( holder, "%d", &tempval2 );

		addTitleToLog("Store Value");

		//store amount
		//TODOBREY mifareClassic->store(tempval2, Amount);
		pMifare_->store(tempval2, Amount);
		addMessageToLog("Store Value success");
	}
	catch (CardException cardException)
	{
		char message[100];
		sprintf(message, "%s%s%s%s", "[", helper.byteAsString(cardException.getStatusWord(),2, true), "] ", cardException.getMessage() );
		addTitleToLog(cardException.getMessage());
		showErrorMessage(message);
	}
	catch (PcscException pcscException)
	{
		showErrorMessage(pcscException.what());
		addTitleToLog(pcscException.what());
	}
	catch (...)
	{
		addTitleToLog("Stored Value Failed");
		showErrorMessage("Stored Value Failed");
	}
}

void CMifareCardProgrammingDlg::OnInc() 
{
	char holder[12];
	int tempval2;
	unsigned long tempval1;
	DWORD Amount;

	int i;
	int iBufferSize = 12;
	int iMaxAmountDigit = 10;

	try
	{
		//Validate Inputs
		memset( holder, NULL, iBufferSize );
		TextBoxValue.GetWindowText( holder, iMaxAmountDigit );

		tempval1 = strtoul(holder, NULL, iMaxAmountDigit);

		if( strcmp( holder, "" ) == 0 )
		{	
			showErrorMessage("Please key-in numeric value for Amount.");
			TextBoxValue.SetFocus();
			return;	
		}
		else if( tempval1 > 2147483647 )
		{	
			showErrorMessage("Please key-in valid Amount. Valid value: 1 - 2,147,483,647.");
			TextBoxValue.SetFocus();
			return;	
		}

		for (i = 0; i < iBufferSize; i++)
		{
			if (!isdigit(holder[i]) && holder[i] != NULL)
			{
				showErrorMessage("Please key-in numeric value for Amount.");
				TextBoxValue.SetFocus();
				return;
			}
		}

		memset( holder, NULL, iBufferSize );
		TextBoxBlock.GetWindowText( holder, 4 );
		sscanf( holder, "%d", &tempval2 );

		if( strcmp( holder, "" ) == 0 )
		{	
			showErrorMessage("Please key-in numeric value for Block Number.");
			TextBoxBlock.SetFocus();
			return;	
		}

		if(sCardName == "Mifare Standard 1K")
		{
			if( tempval2 > 63 )
			{
				showErrorMessage("Please key-in valid Block Number. Valid value: 0 - 63.");
				TextBoxBlock.SetFocus();
				return;	
			}
		}
		else if(sCardName == "Mifare Standard 4K")
		{
			if( tempval2 > 255 )
			{
				showErrorMessage("Please key-in valid Block Number. Valid value: 0 - 255.");
				TextBoxBlock.SetFocus();
				return;	
			}
		}
	
		if (tempval2 <= 127)
		{
			if ((tempval2 + 1) % 4 == 0)
			{
				showWarningMessage("The block to be updated is a sector trailer. Please change the Start Block Number.");
				TextBoxBlock.SetFocus();
				return;
			}
		}
		
		if (tempval2 > 127)
		{
			if ((tempval2 + 1) % 16 == 0)
			{
				showWarningMessage("The block to be updated is a sector trailer. Please change the Start Block Number.");
				TextBoxBlock.SetFocus();
				return;
			}
		}


		for (i = 0; i < 4; i++)
		{
			if (!isdigit(holder[i]) && holder[i] != NULL)
			{
				showErrorMessage("Please key-in numeric value for Block Number.");
				TextBoxBlock.SetFocus();
				return;
			}
		}

		TextBoxSource.SetWindowText( "" );
		TextBoxTarget.SetWindowText( "" );

		//First put the input in a char array then 
		//put it into a int variable using sscanf
		TextBoxValue.GetWindowText( holder, 12 );
		sscanf( holder, "%lu", &tempval1 );
		Amount = tempval1;

		TextBoxBlock.GetWindowText( holder, 4 );
		sscanf( holder, "%d", &tempval2 );	

		addTitleToLog("Increment");
		//increment amount
		//TODOBREY mifareClassic->increment(tempval2, Amount);
		pMifare_->increment(tempval2, Amount);
		addMessageToLog("Increment success");
	}
	catch (CardException cardException)
	{
		char message[100];
		sprintf(message, "%s%s%s%s", "[", helper.byteAsString(cardException.getStatusWord(),2, true), "] ", cardException.getMessage() );
		showErrorMessage(cardException.getMessage());
		addTitleToLog(message);
	}
	catch (PcscException pcscException)
	{
		showErrorMessage(pcscException.what());
		addTitleToLog(pcscException.what());
	}
	catch (...)
	{
		addTitleToLog("Increment Failed");
		showErrorMessage("Increment Failed");
	}
}

void CMifareCardProgrammingDlg::OnDec() 
{
	char holder[12];
	int tempval2;
	unsigned long tempval1;
	DWORD Amount;

	int i;
	int iBufferSize = 12;
	int iMaxAmountDigit = 10;
	
	try
	{
		//Validate inputs
		memset( holder, NULL, iBufferSize );
		TextBoxValue.GetWindowText( holder, iMaxAmountDigit );

		tempval1 = strtoul(holder, NULL, iMaxAmountDigit);

		if( strcmp( holder, "" ) == 0 )
		{
			showErrorMessage("Please key-in numeric value for Amount.");
			TextBoxValue.SetFocus();
			return;
		}
		else if( tempval1 > 2147483647 )
		{
			showErrorMessage("Please key-in valid Amount. Valid value: 1 - 2147483647.");
			TextBoxValue.SetFocus();
			return;
		}

		for (i = 0; i < iBufferSize; i++)
		{
			if (!isdigit(holder[i]) && holder[i] != NULL)
			{
				showErrorMessage("Please key-in numeric value for Amount.");
				TextBoxValue.SetFocus();
				return;
			}
		}
		
		memset( holder, NULL, iBufferSize );
		TextBoxBlock.GetWindowText( holder, 4 );
		sscanf( holder, "%d", &tempval2 );

		if( strcmp( holder, "" ) == 0 )
		{
			showErrorMessage("Please key-in numeric value for Block Number.");
			TextBoxBlock.SetFocus();
			return;
		}

		if(sCardName == "Mifare Standard 1K")
		{
			if( tempval2 > 63 )
			{
				showErrorMessage("Please key-in valid Block Number. Valid value: 0 - 63.");
				TextBoxBlock.SetFocus();
				return;	
			}
		}
		else if(sCardName == "Mifare Standard 4K")
		{
			if( tempval2 > 255 )
			{
				showErrorMessage("Please key-in valid Block Number. Valid value: 0 - 255.");
				TextBoxBlock.SetFocus();
				return;	
			}
		}
	
		if (tempval2 <= 127)
		{
			if ((tempval2 + 1) % 4 == 0)
			{
				showWarningMessage("The block to be updated is a sector trailer. Please change the Start Block Number.");
				TextBoxBlock.SetFocus();
				return;
			}
		}
		
		if (tempval2 > 127)
		{
			if ((tempval2 + 1) % 16 == 0)
			{
				showWarningMessage("The block to be updated is a sector trailer. Please change the Start Block Number.");
				TextBoxBlock.SetFocus();
				return;
			}
		}

		for (i = 0; i < 4; i++)
		{
			if (!isdigit(holder[i]) && holder[i] != NULL)
			{
				showErrorMessage("Please key-in numeric value for Block Number.");
				TextBoxBlock.SetFocus();
				return;
			}
		}

		TextBoxSource.SetWindowText( "" );
		TextBoxTarget.SetWindowText( "" );
		
		//First put the input in a char array then 
		//put it into a int variable using sscanf
		TextBoxValue.GetWindowText( holder, 12 );
		sscanf( holder, "%lu", &tempval1 );
		Amount = tempval1;

		TextBoxBlock.GetWindowText( holder, 4 );
		sscanf( holder, "%d", &tempval2 );	

		addTitleToLog("Decrement");
		//decrement amount
		//TODOBREY mifareClassic->decrement(tempval2, Amount);
		pMifare_->decrement(tempval2, Amount);
		addMessageToLog("Decrement success");
	}
	catch (CardException cardException)
	{
		char message[100];
		sprintf(message, "%s%s%s%s", "[", helper.byteAsString(cardException.getStatusWord(),2, true), "] ", cardException.getMessage() );
		showErrorMessage(cardException.getMessage());
		addTitleToLog(message);
	}
	catch (PcscException pcscException)
	{
		showErrorMessage(pcscException.what());
		addTitleToLog(pcscException.what());
	}
	catch (...)
	{
		addTitleToLog("Decrement Failed");
		showErrorMessage("Decrement Failed");
	}
}

void CMifareCardProgrammingDlg::OnReadVal() 
{
	char tempstr[MAX], holder[12];
	int tempval2;
	//DWORD Amount;
	int Amount;
	
	int i;
	int iBufferSize = 12;

	try
	{
		//Validate inputs
		memset (holder, NULL, iBufferSize );
		TextBoxBlock.GetWindowText( holder, 4 );
		sscanf( holder, "%d", &tempval2 );

		if( strcmp( holder, "" ) == 0 )
		{	
			showErrorMessage("Please key-in numeric value for Block Number.");
			TextBoxBlock.SetFocus();
			return;	
		}

		if(sCardName == "Mifare Standard 1K")
		{
			if( tempval2 > 63 )
			{
				showErrorMessage("Please key-in valid Block Number. Valid value: 0 - 63.");
				TextBoxBlock.SetFocus();
				return;	
			}
		}
		else if(sCardName == "Mifare Standard 4K")
		{
			if( tempval2 > 255 )
			{
				showErrorMessage("Please key-in valid Block Number. Valid value: 0 - 255.");
				TextBoxBlock.SetFocus();
				return;	
			}
		}
	
		if (tempval2 <= 127)
		{
			if ((tempval2 + 1) % 4 == 0)
			{
				showWarningMessage("The block to be read is a sector trailer. Please change the Start Block Number.");
				TextBoxBlock.SetFocus();
				return;
			}
		}
		
		if (tempval2 > 127)
		{
			if ((tempval2 + 1) % 16 == 0)
			{
				showWarningMessage("The block to be read is a sector trailer. Please change the Start Block Number.");
				TextBoxBlock.SetFocus();
				return;
			}
		}

		for (i = 0; i < 4; i++)
		{
			if (!isdigit(holder[i]) && holder[i] != NULL)
			{
				showErrorMessage("Please key-in numeric value for Block Number.");
				TextBoxBlock.SetFocus();
				return;
			}
		}
		
		TextBoxValue.SetWindowText( "" );
		TextBoxSource.SetWindowText( "" );
		TextBoxTarget.SetWindowText( "" );

		addTitleToLog("Read Value");

		//First put the input in a char array then 
		//put it into a int variable using sscanf
		TextBoxBlock.GetWindowText( holder, 4 );
		sscanf( holder, "%d", &tempval2 );

		//inquire amount
		//TODOBREY Amount = mifareClassic->inquireAmount(tempval2);
		Amount = pMifare_->inquireAmount(tempval2);
		
		//sprintf( tempstr, "%lu", Amount );
		sprintf( tempstr, "%d", Amount );
		TextBoxValue.SetWindowText( tempstr );
	

		addMessageToLog("Read Value success");
	}
	catch (CardException cardException)
	{
		char message[100];
		sprintf(message, "%s%s%s%s", "[", helper.byteAsString(cardException.getStatusWord(),2, true), "] ", cardException.getMessage() );
		showErrorMessage(cardException.getMessage());
		addTitleToLog(message);
	}
	catch (PcscException pcscException)
	{
		showErrorMessage(pcscException.what());
		addTitleToLog(pcscException.what());
	}
	catch (...)
	{
		addTitleToLog("Read Value Failed");
		showErrorMessage("Read Value Failed");
	}
}

void CMifareCardProgrammingDlg::OnRestore() 
{
	char holder[4];
	int tempval1,tempval2;

	int i;
	int iBufferSize = 4;
	
	try
	{
		//Validate inputs
		memset ( holder, NULL, iBufferSize);
		TextBoxSource.GetWindowText( holder, iBufferSize );
		sscanf( holder, "%d", &tempval1 );

		if( strcmp( holder, "" ) == 0 )
		{	
			showErrorMessage("Please key-in numeric value for Source Block.");
			TextBoxSource.SetFocus();
			return;	
		}

		if(sCardName == "Mifare Standard 1K")
		{
			if( tempval1 > 63 )
			{
				showErrorMessage("Please key-in valid Source Block. Valid value: 0 - 63.");
				TextBoxSource.SetFocus();
				return;	
			}
		}
		else if(sCardName == "Mifare Standard 4K")
		{
			if( tempval1 > 255 )
			{
				showErrorMessage("Please key-in valid Source Block. Valid value: 0 - 255.");
				TextBoxSource.SetFocus();
				return;	
			}
		}
	
		if (tempval1 <= 127)
		{
			if ((tempval1 + 1) % 4 == 0)
			{
				showWarningMessage("The block to be read is a sector trailer. Please change the Start Block Number.");
				TextBoxSource.SetFocus();
				return;	
			}
		}
		
		if (tempval1 > 127)
		{
			if ((tempval1 + 1) % 16 == 0)
			{
				showWarningMessage("The block to be read is a sector trailer. Please change the Start Block Number.");
				TextBoxSource.SetFocus();
				return;	
			}
		}

		for (i = 0; i < iBufferSize; i++)
		{
			if (!isdigit(holder[i]) && holder[i] != NULL)
			{
				showErrorMessage("Please key-in numeric value for Source Block.");
				TextBoxSource.SetFocus();
				return;
			}
		}
		
		memset ( holder, NULL, iBufferSize);
		TextBoxTarget.GetWindowText( holder, 4 );
		sscanf( holder, "%d", &tempval2 );

		if( strcmp( holder, "" ) == 0 )
		{	
			showErrorMessage("Please key-in numeric value for Target Block.");
			TextBoxTarget.SetFocus();
			return;	
		}

		if(sCardName == "Mifare Standard 1K")
		{
			if( tempval2 > 63 )
			{
				showErrorMessage("Please key-in valid Target Block. Valid value: 0 - 63.");
				TextBoxTarget.SetFocus();
				return;	
			}
		}
		else if(sCardName == "Mifare Standard 4K")
		{
			if( tempval2 > 255 )
			{
				showErrorMessage("Please key-in valid Target Block. Valid value: 0 - 255.");
				TextBoxTarget.SetFocus();
				return;	
			}
		}
	
		if (tempval2 <= 127)
		{
			if ((tempval2 + 1) % 4 == 0)
			{
				showWarningMessage("The block to be updated is a sector trailer. Please change the Start Block Number.");
				TextBoxTarget.SetFocus();
				return;	
			}
		}
		
		if (tempval2 > 127)
		{
			if ((tempval2 + 1) % 16 == 0)
			{
				showWarningMessage("The block to be updated is a sector trailer. Please change the Start Block Number.");
				TextBoxTarget.SetFocus();
				return;		
			}
		}

		for (i = 0; i < iBufferSize; i++)
		{
			if (!isdigit(holder[i]) && holder[i] != NULL)
			{
				showErrorMessage("Please key-in numeric value for Target Block.");
				TextBoxTarget.SetFocus();
				return;
			}
		}

		TextBoxValue.SetWindowText( "" );
		TextBoxBlock.SetWindowText( "" );

		//First put the input in a char array then 
		//put it into a int variable using sscanf
		TextBoxSource.GetWindowText( holder, 4 );
		sscanf( holder, "%d", &tempval1 );

		TextBoxTarget.GetWindowText( holder, 4 );
		sscanf( holder, "%d", &tempval2 );

		addTitleToLog("Restore Value");

		//restore amount
		//TODOBREY mifareClassic->restoreAmount(tempval1,tempval2);
		pMifare_->restoreAmount(tempval1,tempval2);
		addMessageToLog("Restore Value success");
	}
	catch (CardException cardException)
	{
		char message[100];
		sprintf(message, "%s%s%s%s", "[", helper.byteAsString(cardException.getStatusWord(),2, true), "] ", cardException.getMessage() );
		showErrorMessage(cardException.getMessage());
		displayMessage(message);
	}
	catch (PcscException pcscException)
	{
		showErrorMessage(pcscException.what());
		displayMessage(pcscException.what());
	}
	catch (...)
	{
		displayMessage("Restore Value Failed");
		showErrorMessage("Restore Failed");
	}
}

void CMifareCardProgrammingDlg::OnKillfocusEdit17() 
{
	char buffer[4];
	int tempLen;
	
	TextBoxLen.GetWindowText( buffer, 4 );
	sscanf( buffer, "%d", &tempLen );

	if (tempLen <= 0)
		tempLen = 1;
	
	pThis->TextBoxData.SetLimitText( tempLen );	
}
