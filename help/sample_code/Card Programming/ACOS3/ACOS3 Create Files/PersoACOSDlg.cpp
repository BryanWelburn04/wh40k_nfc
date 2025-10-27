/////////////////////////////////////////////////////////////////////////////
//
//	COMPANY : Advanced Card Systems, Ltd.
//
//  AUTHOR  : Alcendor Lorzano Chan
//
//	CREATED	: 07 / 11 / 2001
//
//  NOTE    : PCSC components and the reader driver for PCSC must be installed 
//		      first before this program will run successfully. If program does 
//		      not run the first time, pull out the card and run the program again.
//		      A similar display is shown in the output window when debugging.
//
//	REVISION TRAIL
//
//	June 23, 2008	Wazer Emmanuel R. Benal  -	Added the file access byte in writing to FF 04	
//	Oct 17, 2012	Valerie C. Dio				recoded to object-oriented
/////////////////////////////////////////////////////////////////////////////
// PersoACOSDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PersoACOS.h"
#include "PersoACOSDlg.h"
#include "winscard.h"
#include "PcscProvider.h"
#include "PcscReader.h"
#include "Helper.h"
#include "Acos3.h"
#include "CardSelector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//Global Variables
Helper _cHelper;
byte _aIssuerCode[] = { 0x41, 0x43, 0x4F, 0x53, 0x54, 0x45, 0x53, 0x54 };
CPersoACOSDlg	*_pThis = NULL;

// Function prototypes
void onSendCommand(const byte* pSendData, const int iLen);
void onReceiveCommand(const byte* pReceiveData, const int iLen);

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
// CPersoACOSDlg dialog

CPersoACOSDlg::CPersoACOSDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPersoACOSDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPersoACOSDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIconBig   = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hIconSmall = AfxGetApp()->LoadIcon(IDR_SMALLICON);
}

void CPersoACOSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPersoACOSDlg)
	DDX_Control(pDX, IDC_BUTTON1, ButtonInitialize);
	DDX_Control(pDX, IDC_BUTTON2, ButtonConnect);
	DDX_Control(pDX, IDC_BTNPersonalize, ButtonCreateFiles);
	DDX_Control(pDX, IDC_BUTTON3, ButtonClear);	
	DDX_Control(pDX, IDC_COMBO1, ComboBoxReader);
	DDX_Control(pDX, IDC_EDIT1, EditBoxResult);
	DDX_Text(pDX, IDC_EDIT1, EditBoxResultText);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPersoACOSDlg, CDialog)
	//{{AFX_MSG_MAP(CPersoACOSDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTNPersonalize, OnButtonCreateFiles)
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonInitialize)
	ON_BN_CLICKED(IDC_BUTTON2, OnButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON3, OnButtonReset)
	ON_BN_CLICKED(IDC_BUTTON4, OnButtonClear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPersoACOSDlg message handlers

BOOL CPersoACOSDlg::OnInitDialog()
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
	pAcos3_ = NULL;


	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIconBig,	TRUE);		// Set big icon
	SetIcon(m_hIconSmall,	FALSE);		// Set small icon

	
	// TODO: Add extra initialization here
	resetFields();
	EditBoxResult.SetWindowText("Program ready\r\n");	
	UpdateData(true);

	return TRUE;
}

void CPersoACOSDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPersoACOSDlg::OnPaint() 
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
HCURSOR CPersoACOSDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIconSmall;
}

void CPersoACOSDlg::OnOK() 
{
	// TODO: Add extra validation here

	CDialog::OnOK();
}

void CPersoACOSDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	// TODO: Add your message handler code here
	ComboBoxReader.ResetContent();   // Clear contents of the port ddlistbox
	SCardReleaseContext(m_hContext);  // Release context
}

void CPersoACOSDlg::onPcscException(PcscException cException)
{
	sprintf(aMessage_,"\r\n%s",CString(cException.what()));
	displayMessage(aMessage_);
	MessageBox(cException.what(), "Error", MB_ICONERROR);
}

void CPersoACOSDlg::onCardException(CardException cException)
{
	sprintf(aMessage_,"\r\n[%02X %02X] %s",cException.getStatusWord()[0], 
										 cException.getStatusWord()[1], 
										 CString(cException.getMessage()));
	displayMessage(aMessage_);
	MessageBox(aMessage_, "Error", MB_ICONERROR);
}

void CPersoACOSDlg::resetFields()
{
	ButtonConnect.EnableWindow(false);
	ButtonCreateFiles.EnableWindow(false);
	ComboBoxReader.ResetContent();
}

void CPersoACOSDlg::OnButtonCreateFiles() 
{
	byte aFile1[2], aFile2[2], aFile3[2];
	try
	{
		//submit IC
		pAcos3_->submitCode(IC, _aIssuerCode);

		//clear card
		displayTitle("\r\nClear Card");
		pAcos3_->clearCard();

		//reset card
		if (pPcscReader_->isConnectionActive())
			if (pPcscReader_->disconnect() != 0)
				return;

		//submit IC
		displayTitle("\r\nSubmit Code - IC");
		pAcos3_->submitCode(IC, _aIssuerCode);

		//select personalization file
		displayTitle("\r\nSelect File - FF 02");
		pAcos3_->selectFile(PERSONALIZATION);

		//set option registers and security option registers
		OptionRegister* pOptionRegister = new OptionRegister();

		pOptionRegister->setRequireMutualAuthenticationOnInquireAccount(false);               
        pOptionRegister->setRequireMutualAuthenticationOnAccountTransaction(false); 
        pOptionRegister->setRequireMutualAuthenticationOnAccountTransaction(false); 
        pOptionRegister->setEnableRevokeDebitCommand(false); 
        pOptionRegister->setRequirePinDuringDebit(false); 
        pOptionRegister->setEnableDebitMac(false); 
        pOptionRegister->setEnableChangePinCommand(false); 
        pOptionRegister->setEnableAccount(false); 

		SecurityOptionRegister* pSecurityOptionRegister = new SecurityOptionRegister();
	
		pSecurityOptionRegister->setIssuerCode(false);
        pSecurityOptionRegister->setPin(false);
        pSecurityOptionRegister->setAccessCondition5(false);
        pSecurityOptionRegister->setAccessCondition4(false);
        pSecurityOptionRegister->setAccessCondition3(false);
        pSecurityOptionRegister->setAccessCondition2(false);
        pSecurityOptionRegister->setAccessCondition1(false);

		//write record to personalization file
		//number of files = 3
		displayTitle("\r\nWrite Record");
		pAcos3_->configurePersonalizationFile(pOptionRegister, pSecurityOptionRegister, 0x03);

		//select user file management file
		displayTitle("\r\nSelect File - FF 04");
		pAcos3_->selectFile(USER_FILE_MGMT_FILE);
		pAcos3_->submitCode(IC,_aIssuerCode);
		
		 //Set the read and write security attributes
		SecurityAttribute* pReadSecurityAttribute = new SecurityAttribute();

		//Set read to free access
        pReadSecurityAttribute->getAccessCondition1();
        pReadSecurityAttribute->getAccessCondition2();
        pReadSecurityAttribute->getAccessCondition3();
        pReadSecurityAttribute->getAccessCondition4();
        pReadSecurityAttribute->getAccessCondition5();
        pReadSecurityAttribute->getIssuerCode();
		pReadSecurityAttribute->getPin();

        SecurityAttribute* pWriteSecurityAttribute = new SecurityAttribute();

        //Set write to free access
        pWriteSecurityAttribute->setAccessCondition1(false);
        pWriteSecurityAttribute->setAccessCondition2(false);
        pWriteSecurityAttribute->setAccessCondition3(false);
        pWriteSecurityAttribute->setAccessCondition4(false);
        pWriteSecurityAttribute->setAccessCondition5(false);
        pWriteSecurityAttribute->setIssuerCode(false);
        pWriteSecurityAttribute->setPin(false);

		//create files
		//Create Files
        //Record File ID:       AA 11
        //Record Number:        0x00
        //Record Length:        0x05
        //Number of Records:    0x03
        displayTitle("\r\nWrite Record");
		byte file[2] = {0xAA, 0x11};
        pAcos3_->createRecordFile(0x00, file,
            0x03, 0x05, pWriteSecurityAttribute, pReadSecurityAttribute, false, false);

        //Record File ID:       BB 22
        //Record Number:        0x01
        //Record Length:        0x0A (10)
        //Number of Records:    0x02
        displayTitle("\r\nWrite Record");
		byte file2[2] = {0xBB, 0x22};
        pAcos3_->createRecordFile(0x01, file2,
            0x02, 0x0A, pWriteSecurityAttribute, pReadSecurityAttribute, false, false);

        //Record File ID:       CC 33
        //Record Number:        0x02
        //Record Length:        0x06
        //Number of Records:    0x04
        displayTitle("\r\nWrite Record");
		byte file3[2] = {0xCC, 0x33};
        pAcos3_->createRecordFile(0x02,file3,
            0x04, 0x06, pWriteSecurityAttribute, pReadSecurityAttribute, false, false);

        //Select files to verify that they are created
        //The response is 90 [RecordNumber] if the file is selected successfully
        displayTitle("\r\nSelect File - AA 11");

		aFile1[0] = 0xAA;
		aFile1[1] = 0x11;
        pAcos3_->selectFile(aFile1);
        
		displayTitle("\r\nSelect File - BB 22");

		aFile2[0] = 0xBB;
		aFile2[1] = 0x22;
        pAcos3_->selectFile(aFile2);
        
		displayTitle("\r\nSelect File - CC 33");
		
		aFile3[0] = 0xCC;
		aFile3[1] = 0x33;
        pAcos3_->selectFile(aFile3);
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


void CPersoACOSDlg::OnButtonInitialize() 
{
	string *pReaderList = new string[10];
	uint8 uReaderCount = 10;
	int i = 0;

	try
	{
		ComboBoxReader.ResetContent();
		resetFields();

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
					
		ButtonConnect.EnableWindow(true);		

        displayMessage("\r\nInitialize success");
	}
	catch (PcscException pcscException)
	{
		onPcscException(pcscException);
	}
}

void CPersoACOSDlg::OnButtonConnect() 
{	
	CString sReaderName;
	char aMessage[100];

	uint8 aAtr[100];
	uint32 uAtrLen = 100;
	CString sCardName;
	CardSelector *pCardSelector;
	
	ComboBoxReader.GetWindowText(sReaderName);

	try
	{
		// Check if a reader name is selected
		if (!strcmp(sReaderName, ""))
		{
			MessageBox("Select a smartcard reader.", "Error", MB_ICONERROR);
			return;
		}

		// Try to connect to the smartcard through the specified reader name
		if (pPcscReader_->connect((std::string)sReaderName) != 0)
			return;
		
		sprintf(aMessage, "\r\n%s %s", "Successfully connected to ", sReaderName);
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
			displayMessage("Chip Type ACOS3.");
			ButtonCreateFiles.EnableWindow(true);
		}
		else
		{
			MessageBox("Card not supported. Please use ACOS3 card.", "Error", MB_ICONERROR);
			ButtonCreateFiles.EnableWindow(false);	
		}

		ButtonConnect.EnableWindow(true);
	}
	catch (PcscException pcscException)
	{
		onPcscException(pcscException);
	}
}

void CPersoACOSDlg::OnButtonReset() 
{
	try
	{
		// Free existing smartcard connection handle
		if (pPcscReader_->isConnectionActive())
			if (pPcscReader_->disconnect() != 0)
				return;

		resetFields();
		EditBoxResult.SetWindowText("Program ready\r\n");	
		UpdateData(true);
	}
	catch (PcscException pcscException)
	{
		onPcscException(pcscException);
	}	
}

void CPersoACOSDlg::OnButtonClear() 
{	
	EditBoxResult.SetWindowText("");	
	UpdateData(true);
}


//Helper

// Display the send command APDU
void onSendCommand(const byte* pSendData, const int iLen)
{
	_pThis->displayMessage((CString)"\r\n< " + (CString)_cHelper.byteAsString(pSendData, iLen, true));
}

// Display the receive command APDU
void onReceiveCommand(const byte* pReceiveData, const int iLen)
{
	_pThis->displayMessage((CString)"> " + (CString)_cHelper.byteAsString(pReceiveData, iLen, true));
}

//Display the message to the log window
void CPersoACOSDlg::displayMessage(CString sMessage)
{
    EditBoxResultText += sMessage + "\r\n";
	UpdateData(false);
	EditBoxResult.SetSel(0xFFFF, 0xFFFF);
}

void CPersoACOSDlg::displayTitle(CString sTitle)
{
    EditBoxResultText += sTitle;
	UpdateData(false);
	EditBoxResult.SetSel(0xFFFF, 0xFFFF);
}
