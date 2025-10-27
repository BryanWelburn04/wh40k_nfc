
// Acos3EncryptDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Acos3Encrypt.h"
#include "Acos3EncryptDlg.h"

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
CAcos3EncryptDlg *_pThis = NULL;
Helper _cHelper;
byte _aIssuerCode[] = { 0x41, 0x43, 0x4F, 0x53, 0x54, 0x45, 0x53, 0x54 };

unsigned char _aSessionKey[16];
unsigned char _aCardKey[16];
unsigned char _aTerminalKey[16];

//Function Prototypes
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
// CAcos3EncryptDlg dialog

CAcos3EncryptDlg::CAcos3EncryptDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAcos3EncryptDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAcos3EncryptDlg)
	EditTextLogsValue = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

CAcos3EncryptDlg::~CAcos3EncryptDlg()
{

}

void CAcos3EncryptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAcos3EncryptDlg)
	DDX_Control(pDX, IDC_RADIO4, RadioButton3Des);
	DDX_Control(pDX, IDC_RADIO3, RadioButtonDes);
	DDX_Control(pDX, IDC_RADIO2, RadioButtonEncrypted);
	DDX_Control(pDX, IDC_RADIO1, RadioButtonNotEncrypted);
	DDX_Control(pDX, IDC_EDIT5, EditTextLogs);
	DDX_Control(pDX, IDC_EDIT4, EditTextValue);
	DDX_Control(pDX, IDC_EDIT2, EditTextTerminalKey);
	DDX_Control(pDX, IDC_EDIT1, EditTextCardKey);
	DDX_Control(pDX, IDC_COMBO2, ComboBoxCode);
	DDX_Control(pDX, IDC_COMBO1, ComboBoxReader);
	DDX_Control(pDX, IDC_BUTTON8, ButtonQuit);
	DDX_Control(pDX, IDC_BUTTON7, ButtonReset);
	DDX_Control(pDX, IDC_BUTTON6, ButtonClear);
	DDX_Control(pDX, IDC_BUTTON5, ButtonSubmit);
	DDX_Control(pDX, IDC_BUTTON4, ButtonSetValue);
	DDX_Control(pDX, IDC_BUTTON3, ButtonFormat);
	DDX_Control(pDX, IDC_BUTTON2, ButtonConnect);
	DDX_Control(pDX, IDC_BUTTON1, ButtonInitialize);
	DDX_Text(pDX, IDC_EDIT5, EditTextLogsValue);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAcos3EncryptDlg, CDialog)
	//{{AFX_MSG_MAP(CAcos3EncryptDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonInitialize)
	ON_BN_CLICKED(IDC_BUTTON2, OnButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON3, OnButtonFormat)
	ON_BN_CLICKED(IDC_BUTTON4, OnButtonSetValue)
	ON_BN_CLICKED(IDC_BUTTON5, OnButtonSubmit)
	ON_BN_CLICKED(IDC_BUTTON6, OnButtonClear)
	ON_BN_CLICKED(IDC_BUTTON7, OnButtonReset)
	ON_BN_CLICKED(IDC_BUTTON8, OnButtonQuit)
	ON_BN_CLICKED(IDC_RADIO1, OnRadioButtonNotEncrypted)
	ON_BN_CLICKED(IDC_RADIO2, OnRadioButtonEncrypted)
	ON_BN_CLICKED(IDC_RADIO3, OnRadioButtonDes)
	ON_BN_CLICKED(IDC_RADIO4, OnRadioButton3Des)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAcos3EncryptDlg message handlers

BOOL CAcos3EncryptDlg::OnInitDialog()
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
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	_pThis = this;
	pPcscReader_ = new PcscReader();
	resetFields(false);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAcos3EncryptDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAcos3EncryptDlg::OnPaint() 
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
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CAcos3EncryptDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAcos3EncryptDlg::onPcscException(PcscException cException)
{
	sprintf(aMessage_,"\r\n%s",CString(cException.what()));
	displayMessage(aMessage_);
	MessageBox(cException.what(), "Error", MB_ICONERROR);
}

void CAcos3EncryptDlg::onCardException(CardException cException)
{
	sprintf(aMessage_,"\r\n[%02X %02X] %s",cException.getStatusWord()[0], 
										 cException.getStatusWord()[1], 
										 CString(cException.getMessage()));
	displayMessage(aMessage_);
	MessageBox(aMessage_, "Error", MB_ICONERROR);
}

void CAcos3EncryptDlg::setRadioButtonsLimit()
{
	if(RadioButtonDes.GetCheck() == 1)
	{
		EditTextCardKey.SetLimitText(8);
		EditTextTerminalKey.SetLimitText(8);
		EditTextValue.SetLimitText(8);	
	}
	else
	{
		EditTextCardKey.SetLimitText(16);
		EditTextTerminalKey.SetLimitText(16);
		EditTextValue.SetLimitText(16);	
	}
}

//Helper
void CAcos3EncryptDlg::resetFields(BOOL enable)
{		
	ButtonConnect.EnableWindow(enable);
	ComboBoxReader.ResetContent();
	EditTextLogs.SetWindowText("Program Ready\r\n");
	UpdateData(true);
	enableFields(enable);

	EditTextCardKey.SetWindowText("");
	EditTextTerminalKey.SetWindowText("");
	EditTextValue.SetWindowText("");

	RadioButtonNotEncrypted.SetCheck(1);
	RadioButtonEncrypted.SetCheck(0);
	RadioButtonDes.SetCheck(1);
	RadioButton3Des.SetCheck(0);

	ComboBoxCode.SetCurSel(-1);
}

void CAcos3EncryptDlg::enableFields(BOOL bEnable)
{
	ButtonFormat.EnableWindow(bEnable);
	ButtonSetValue.EnableWindow(bEnable);
	ButtonSubmit.EnableWindow(bEnable);
	
	RadioButtonNotEncrypted.EnableWindow(bEnable);
	RadioButtonEncrypted.EnableWindow(bEnable);
	RadioButtonDes.EnableWindow(false);
	RadioButton3Des.EnableWindow(false);

	EditTextCardKey.SetLimitText(8);
	EditTextTerminalKey.SetLimitText(8);
	EditTextValue.SetLimitText(8);

	EditTextCardKey.EnableWindow(bEnable);
	EditTextTerminalKey.EnableWindow(bEnable);
	EditTextValue.EnableWindow(bEnable);

	ComboBoxCode.EnableWindow(bEnable);
}

// Display the send command APDU
void onSendCommand(const byte* pSendData, const int iLen)
{
	char *pStr;

	pStr = _cHelper.byteAsString(pSendData, iLen, true);
	if (pStr == NULL)
	{
		return;
	}

	_pThis->displayMessage((CString)"\r\n<<" + (CString) pStr);

	delete [] pStr;
}

// Display the receive command APDU
void onReceiveCommand(const byte* pReceiveData, const int iLen)
{
	char *pStr;

	pStr = _cHelper.byteAsString(pReceiveData, iLen, true);
	if (pStr == NULL)
	{
		return;
	}

	_pThis->displayMessage((CString)">>" + (CString) pStr);

	delete [] pStr;
}

//Display the message to the log window
void CAcos3EncryptDlg::displayMessage(CString sMessage)
{
    EditTextLogsValue += sMessage + "\r\n";
	UpdateData(false);
	EditTextLogs.SetSel(0xFFFF, 0xFFFF);
}

void CAcos3EncryptDlg::displayTitle(CString sTitle)
{
    EditTextLogsValue += sTitle;
	UpdateData(false);
	EditTextLogs.SetSel(0xFFFF, 0xFFFF);
}

void CAcos3EncryptDlg::OnButtonInitialize() 
{
	string *pReaderList = new string[10];
	uint8 uReaderCount = 10;
	int i = 0;

	try
	{
		ComboBoxReader.ResetContent();
		enableFields(false);

		// Establish Context
		if (pPcscReader_ == NULL)
			pPcscReader_ = new PcscReader();

		pPcscReader_->initialize();

		// List the detected smart card readers
		pPcscReader_->listReaders(pReaderList, uReaderCount);

		// Load the retrieved reader name array to the reader list		
		for(i = 0; i < uReaderCount; i++)
		{
			ComboBoxReader.AddString(pReaderList[i].data());
		}

		// Pass the function pointers to serve as the event listeners of the ProprietaryReader class
		pPcscReader_->setSendCommandFunctor(onSendCommand);
		pPcscReader_->setRecvCommandFunctor(onReceiveCommand);
						
		ButtonConnect.EnableWindow(true);
		displayMessage("\r\nInitialize success");

		ComboBoxReader.SetCurSel(0);
	}
	catch (PcscException pcscException)
	{
		onPcscException(pcscException);
	}
	
}

void CAcos3EncryptDlg::getSessionKey()
{
	unsigned char aPlainTRnd[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	unsigned char aEncryptedTRnd[8];
	unsigned char aPlainCRnd[8];
	unsigned char aEncryptedCRnd[8];	
	uint8 aTRndToVerify[8];

	char aBuffer[100];
	int i = 0;

	//Get card key and terminal key from input
	EditTextCardKey.GetWindowText(aBuffer, 100);
	for (i = 0; i < strlen((char*)aBuffer); i++)
		_aCardKey[i] = aBuffer[i];

	EditTextTerminalKey.GetWindowText(aBuffer, 100);
	for (i = 0; i < strlen((char*)aBuffer); i++)
		_aTerminalKey[i] = aBuffer[i];

	//Store the random number generated by the card (aPlainCRnd)
	displayTitle("\r\nStart Session");
	pAcos3_->startSession(aPlainCRnd);
	memcpy(aEncryptedCRnd, aPlainCRnd, 8);

	//Encrypt plainCRnd with Terminal Key (tKey)
	//encryptedCRnd will hold the 8-byte encrypted number
	if (RadioButtonDes.GetCheck() == 1)
		pAcos3_->Acos3DES(aEncryptedCRnd, _aTerminalKey, 1);
	else
		pAcos3_->Acos3DES3(aEncryptedCRnd, _aTerminalKey, 1);

	//Issue Authenticate command using encryptedCRnd and random terminal number (aPlainTRnd)
	displayTitle("\r\nAuthenticate");
	pAcos3_->authenticate(aEncryptedCRnd, aPlainTRnd, aEncryptedTRnd);

	if (RadioButtonDes.GetCheck() == 1)
	{
		unsigned char tempSessionKey[8];
		
		//Single DES
		//Prepare session key
		//SessionKey = DES(DES (CRnd, cKey) XOR TRnd, tKey)

		//Calculate DES(CRnd, cKey)
		memcpy(tempSessionKey, aPlainCRnd, 8);
		pAcos3_->Acos3DES(tempSessionKey, _aCardKey, 1);

		//XOR the result with aPlainTRnd
		for (i = 0; i < 8; i++)
			tempSessionKey[i] ^= aPlainTRnd[i];

		//DES the result with tKey
		pAcos3_->Acos3DES(tempSessionKey, _aTerminalKey, 1);
		memcpy(_aSessionKey, tempSessionKey, 8);

		//Verify Session Key
		memcpy(aTRndToVerify, aPlainTRnd, 8);
		pAcos3_->Acos3DES(aTRndToVerify, _aSessionKey, 1);
		if (memcmp(aTRndToVerify, aEncryptedTRnd, 8))
			throw CardException("Verify session key failed");
	}
	else
	{
		//Triple DES
		unsigned char leftHalfSessionKey[8];
		unsigned char rightHalfSessionKey[8];
		unsigned char reverseKey[16];
		
		//Left half of _aSessionKey = 3DES(3DES(CRnd, cKey), tKey)
		memcpy(leftHalfSessionKey, aPlainCRnd, 8);
		pAcos3_->Acos3DES3(leftHalfSessionKey, _aCardKey, 1); //3DES(CRnd, cKey)
		pAcos3_->Acos3DES3(leftHalfSessionKey, _aTerminalKey, 1); //3DES(3DES(CRnd, cKey), tKey)
		
		//Copy the left half to SessionKey
		memcpy(_aSessionKey, leftHalfSessionKey, 8);

		//Right half of SessionKey = 3DES(TRnd, Reverse(_aTerminalKey))
		//Compute the reverse key of tKey (Reverse(tKey))
		memcpy(reverseKey, _aTerminalKey + 8, 8);
		memcpy(reverseKey + 8, _aTerminalKey, 8);
	
		//3DES(TRnd, ReverseKey)
		memcpy(rightHalfSessionKey, aPlainTRnd, 8);
		pAcos3_->Acos3DES3(rightHalfSessionKey, reverseKey, 1);
		
		//Copy the right half to _aSessionKey
		memcpy(_aSessionKey + 8, rightHalfSessionKey, 8);


		//Verify Session Key
		memcpy(aTRndToVerify, aPlainTRnd, 8);
		pAcos3_->Acos3DES3(aTRndToVerify, _aSessionKey, 1);
		if (memcmp(aTRndToVerify, aEncryptedTRnd, 8))
			throw CardException("Verify session key failed");
	}
	
}

void CAcos3EncryptDlg::OnButtonConnect() 
{
	CString sReaderName;

	uint8 aAtr[100];
	uint32 uAtrLen = sizeof(aAtr);
	CString sCardName;
	CardSelector *pCardSelector;

	try
	{
		ComboBoxReader.GetWindowText(sReaderName);

		// Check if a reader name is selected
		if (!strcmp(sReaderName, ""))
		{
			MessageBox("Select a smartcard reader", "Error", MB_ICONERROR);
			return;
		}

		// Try to connect to the smartcard through the specified reader name		
		if (pPcscReader_->connect((std::string)sReaderName) != 0)
			return;
		
		displayMessage("\r\nSuccessfully connected to " + sReaderName);		
		
		pAcos3_ = new Acos3(pPcscReader_);	
		if (pAcos3_ == NULL)
		{
			MessageBox("Insufficient resources for new ACOS3 Card", "Error", MB_ICONERROR);
			return;
		}
		
		pPcscReader_->getStatus();
		pPcscReader_->getAtr(aAtr, uAtrLen);
		
		pCardSelector = new CardSelector(pPcscReader_);

		sCardName = pCardSelector->readCardType(aAtr, uAtrLen);

		if(sCardName == "ACOS3")
		{
			displayMessage("Chip Type: ACOS3");
			enableFields(true);
			
			if(RadioButtonEncrypted.GetCheck() == 1)
			{
				RadioButtonDes.EnableWindow(true);
				RadioButton3Des.EnableWindow(true);
				setRadioButtonsLimit();
			}

			ButtonSubmit.EnableWindow(false);
			ButtonSetValue.EnableWindow(false);
			EditTextValue.EnableWindow(false);
			ComboBoxCode.EnableWindow(false);
		}	
		else
		{
			MessageBox("Card not supported. Please use ACOS3 card", "Error", MB_ICONERROR);
			enableFields(false);	
		}

		ButtonConnect.EnableWindow(true);
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

void CAcos3EncryptDlg::OnButtonFormat() 
{
	byte aFileId[2];
	byte aData[10];
	char aCardKey[17], aTerminalKey[17];
	int i = 0;	

	try
	{		
		//Validate input
		EditTextCardKey.GetWindowText(aCardKey, 100);
		if (strcmp(aCardKey, "") == 0) 
		{
			MessageBox("Please key-in numeric value for Card Key", "Error", MB_ICONERROR);			
			EditTextCardKey.SetFocus();
			return;
		}

		EditTextTerminalKey.GetWindowText(aTerminalKey, 100);
		if (strcmp(aTerminalKey, "") == 0) 
		{
			MessageBox("Please key-in numeric value for Terminal Key", "Error", MB_ICONERROR);			
			EditTextTerminalKey.SetFocus();
			return;
		}

		if (RadioButtonDes.GetCheck() == 1)
		{			
			if (strlen(aCardKey) < 8) 
			{
				MessageBox("Invalid input length. Length must be 8.", "Error", MB_ICONERROR);			
				EditTextCardKey.SetFocus();
				return;
			}
			
			if (strlen(aTerminalKey) < 8) 
			{
				MessageBox("Invalid input length. Length must be 8.", "Error", MB_ICONERROR);			
				EditTextTerminalKey.SetFocus();
				return;
			}
		}
		else
		{
			if (strlen(aCardKey) < 16) 
			{
				MessageBox("Invalid input length. Length must be 16", "Error", MB_ICONERROR);			
				EditTextCardKey.SetFocus();
				return;
			}

			if (strlen(aTerminalKey) < 16) 
			{
				MessageBox("Invalid input length. Length must be 16", "Error", MB_ICONERROR);			
				EditTextTerminalKey.SetFocus();
				return;
			}
		}
		
		//Submit code
		displayTitle("\r\nSubmit Code - IC");
		pAcos3_->submitCode(IC, _aIssuerCode);

		//Clear card
		//pAcos3_->clearCard();

		// Submit code
		//pAcos3_->submitCode(IC, _aIssuerCode);
		
		//Select file FF 02
		displayTitle("\r\nSelect File");
		aFileId[0] = 0xFF;
		aFileId[1] = 0x02;
		pAcos3_->selectFile(aFileId);		

		//Write to FF 02
		//This step will define the Option Registers and Security Option Registers.
		//Personalization bit is not set. Although secret codes may be set individually, 
		//this program adopts uniform encryption option for all codes to simplify the coding.
		//Issuer Code (IC) is not encrypted to remove the risk of locking the ACOS card for wrong IC submission.
		if(RadioButtonDes.GetCheck() == 1)
			aData[0] = 0x00; //3DES is disabled, DES option only
		else
			aData[0] = 0x02; //3DES is enabled
		
		if (RadioButtonNotEncrypted.GetCheck() == 1)
			aData[1] = 0x00; //Encryption is not set
		else
			aData[1] = 0x7E; //Encryption on all codes except IC is enabled
		
		displayTitle("\r\nWrite Record");
		aData[2] = 0x03;
		aData[3] = 0x00;
		pAcos3_->writeRecord(0x00, 0x00, aData, 0x04);
		displayMessage("FF 02 is updated");
		
		//Submit code
		displayTitle("\r\nSubmit Code - IC");
		pAcos3_->submitCode(IC, _aIssuerCode);

		//Select file FF 03
		displayTitle("\r\nSelect File");
		aFileId[0] = 0xFF;
		aFileId[1] = 0x03;
		pAcos3_->selectFile(aFileId);
		
		//Write the keys to the card
		if (RadioButtonDes.GetCheck() == 1)
		{			
			//Record 02 for Card key					
			for (i = 0; i < 8; i++)
			{        
				aData[i] = aCardKey[i];            
			}
			displayTitle("\r\nWrite Record");
			pAcos3_->writeRecord(0x02, 0x00, aData, 0x08);

			//Record 03 for Terminal key		
			for (i = 0; i < 8; i++)
			{        
				aData[i] = aTerminalKey[i];            
			}
			displayTitle("\r\nWrite Record");
			pAcos3_->writeRecord(0x03, 0x00, aData, 0x08);
			displayMessage("FF 03 is updated");
		}
		else
		{
			//Record 02 for left half of card key
			for (i = 0; i < 8; i++)
			{        
				aData[i] = aCardKey[i];            
			}
			displayTitle("\r\nWrite Record");
			pAcos3_->writeRecord(0x02, 0x00, aData, 0x08);

			//Record 12 for right half of card key
			for (i = 0; i < 8; i++)
			{        
				aData[i] = aCardKey[i + 8];            
			}
			displayTitle("\r\nWrite Record");
			pAcos3_->writeRecord(0x0C, 0x00, aData, 0x08);

			//Record 03 for left half of terminal key	
			for (i = 0; i < 8; i++)
			{        
				aData[i] = aTerminalKey[i];            
			}
			displayTitle("\r\nWrite Record");
			pAcos3_->writeRecord(0x03, 0x00, aData, 0x08);

			//Record 13 for right half of terminal key					
			for (i = 0; i < 8; i++)
			{        
				aData[i] = aTerminalKey[i + 8];            
			}
			displayTitle("\r\nWrite Record");
			pAcos3_->writeRecord(0x0D, 0x00, aData, 0x08);
		}

		displayMessage("\r\nFormat card successful");
		
		ButtonSetValue.EnableWindow(true);
		EditTextValue.EnableWindow(true);
		ComboBoxCode.EnableWindow(true);
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

void CAcos3EncryptDlg::OnButtonSetValue() 
{
	byte aFileId[2];
	byte aData[10];
	byte uTempByte;
	char aCode[9];
	char aCodeType[32];
	char aErrorMessage[100];
	int i = 0;
	CString sTempString;

	try
	{	
		//Validate Input
		if (ComboBoxCode.GetCurSel() < 0)
		{
			MessageBox("Please select code", "Error", MB_ICONERROR);
			return;
		}
		
		EditTextValue.GetWindowText(aCode, 9);

		ComboBoxCode.GetWindowText(aCodeType,sizeof(aCodeType));
		if(strcmp(aCode, "") == 0)
		{
			sprintf(aErrorMessage,"Please key-in numeric value for %s.",aCodeType);
			MessageBox(aErrorMessage, "Error", MB_ICONERROR);					
			EditTextValue.SetFocus();
			return;
		}

		if (strlen(aCode) < 8) 
		{
			MessageBox("Invalid input length. Length must be 8.", "Error", MB_ICONERROR);			
			EditTextValue.SetFocus();
			return;
		}
		
		for(i = 0; i < 8; i++)
		{
			if(!isdigit(aCode[i]))
			{
				sprintf(aErrorMessage,"Please key-in numeric value for %s.",aCodeType);
				MessageBox(aErrorMessage, "Error", MB_ICONERROR);			
				EditTextValue.SetFocus();
				return;
			}
		}

		for (i = 0; i < 8; i++)
			aData[i] = aCode[i];
		
		//Submit code
		displayTitle("\r\nSubmit Code - IC");
		pAcos3_->submitCode(IC, _aIssuerCode);

		aFileId[0] = 0xFF;
		aFileId[1] = 0x03;
		displayTitle("\r\nSelect File");
		pAcos3_->selectFile(aFileId);

		switch(ComboBoxCode.GetCurSel())
		{			
			case 0:
				uTempByte = 0x05;
				sTempString = "Application Code 1";
				break;
			case 1:
				uTempByte = 0x06;
				sTempString = "Application Code 2";
				break;
			case 2:
				uTempByte = 0x07;
				sTempString = "Application Code 3";
				break;
			case 3:
				uTempByte = 0x08;
				sTempString = "Application Code 4";
				break;
			case 4:
				uTempByte = 0x09;
				sTempString = "Application Code 5";
				break;
			case 5:
				uTempByte = 0x01;
				sTempString = "PIN";
				break;
			default:
				MessageBox("Invalid Code Type");
				break;
		}

		displayTitle("\r\nWrite Record");
		pAcos3_->writeRecord(uTempByte, 0x00, aData, 8);
		displayMessage(sTempString + CString(" set successfully"));
		ButtonSubmit.EnableWindow(true);
	
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

void CAcos3EncryptDlg::OnButtonSubmit() 
{	
	CODE_TYPE eCodeType;
	CString sTempString;
	char aCodeType[32];
	char aErrorMessage[100];
	byte aResponse[2];
	unsigned char aTempArray[9];
	char aCardKey[17], aTerminalKey[17], aCode[9], aResult[100];
	int i = 0;

	try
	{
		//Validate input
		EditTextCardKey.GetWindowText(aCardKey, 100);
		if (strcmp(aCardKey, "") == 0) 
		{
			MessageBox("Please key-in numeric value for Card Key", "Error", MB_ICONERROR);			
			EditTextCardKey.SetFocus();
			return;
		}

		EditTextTerminalKey.GetWindowText(aTerminalKey, 100);
		if (strcmp(aTerminalKey, "") == 0) 
		{
			MessageBox("Please key-in numeric value for Terminal Key", "Error", MB_ICONERROR);			
			EditTextTerminalKey.SetFocus();
			return;
		}

		if (RadioButtonDes.GetCheck() == 1)
		{			
			if (strlen(aCardKey) < 8) 
			{
				MessageBox("Invalid input length. Length must be 8.", "Error", MB_ICONERROR);			
				EditTextCardKey.SetFocus();
				return;
			}
			
			if (strlen(aTerminalKey) < 8) 
			{
				MessageBox("Invalid input length. Length must be 8.", "Error", MB_ICONERROR);			
				EditTextTerminalKey.SetFocus();
				return;
			}
		}
		else
		{
			if (strlen(aCardKey) < 16) 
			{
				MessageBox("Invalid input length. Length must be 16", "Error", MB_ICONERROR);			
				EditTextCardKey.SetFocus();
				return;
			}

			if (strlen(aTerminalKey) < 16) 
			{
				MessageBox("Invalid input length. Length must be 16", "Error", MB_ICONERROR);			
				EditTextTerminalKey.SetFocus();
				return;
			}
		}

		if (ComboBoxCode.GetCurSel() < 0)
		{
			MessageBox("Please select code type", "Error", MB_ICONERROR);
			return;
		}
		
		EditTextValue.GetWindowText(aCode, 9);

		ComboBoxCode.GetWindowText(aCodeType,sizeof(aCodeType));
		if(strcmp(aCode, "") == 0)
		{
			sprintf(aErrorMessage,"Please key-in numeric value for %s.",aCodeType);
			MessageBox(aErrorMessage, "Error", MB_ICONERROR);					
			EditTextValue.SetFocus();
			return;
		}
		
		if (strlen(aCode) < 8) 
		{
			MessageBox("Invalid input length. Length must be 8.", "Error", MB_ICONERROR);			
			EditTextValue.SetFocus();
			return;
		}	

		for(i = 0; i < 8; i++)
		{
			if(!isdigit(aCode[i]))
			{
				sprintf(aErrorMessage,"Please key-in numeric value for %s.",aCodeType);
				MessageBox(aErrorMessage, "Error", MB_ICONERROR);			
				EditTextValue.SetFocus();
				return;
			}
		}

		//Get code from input
		for (i = 0; i < 8; i++)
			aTempArray[i] = aCode[i];

		//Encrypt the code with the sessionKey
		if (RadioButtonEncrypted.GetCheck() == 1)
		{
			getSessionKey();

			
			if (RadioButtonDes.GetCheck() == 1)
				pAcos3_->Acos3DES(aTempArray, _aSessionKey, 1);
			else
				pAcos3_->Acos3DES3(aTempArray, _aSessionKey, 1);
		}
		
		switch(ComboBoxCode.GetCurSel())
		{
			case 0:	
				eCodeType = AC1;
				sTempString = "Application Code 1";
				break;
			case 1:	
				eCodeType = AC2;
				sTempString = "Application Code 2";
				break;
			case 2:	
				eCodeType = AC3;
				sTempString = "Application Code 3";
				break;
			case 3:	
				eCodeType = AC4;
				sTempString = "Application Code 4";
				break;
			case 4:	
				eCodeType = AC5;
				sTempString = "Application Code 5";
				break;
			case 5:	
				eCodeType = PIN;
				sTempString = "PIN";
				break;
			default:
				MessageBox("Invalid code type", "Warning");
				return;
		}

		//Submit the encrypted code
		displayTitle("\r\nSubmit Code");
		pAcos3_->submitAppCodes(eCodeType, aTempArray, aResponse);

		if (aResponse[0] == 0x90)
			displayMessage(sTempString + " submitted successfully");
		else
		{
			if (aResponse[0] == 0x63)
			{
				i = aResponse[1] & 0x0F;
				sprintf(aResult, "%d", i);
				MessageBox("[63 C" + (CString)aResult + "] Invalid Pin/key/code; [" + (CString)aResult + "] retries left; MAC cryptographic checksum is wrong.", "Error", MB_ICONERROR);
				displayMessage("\r\n[63 C" + (CString)aResult + "] Invalid Pin/key/code; [" + (CString)aResult + "] retries left; MAC cryptographic checksum is wrong.");
			}
			else
			{
				MessageBox("Submit " + sTempString + " failed", "Error", MB_ICONERROR);
			}
		}
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

void CAcos3EncryptDlg::OnButtonClear() 
{
	EditTextLogs.SetWindowText("");
	UpdateData(true);
}

void CAcos3EncryptDlg::OnButtonReset() 
{
	try
	{
		if (pPcscReader_ == NULL)
			return;

		if (pPcscReader_->isConnectionActive())
			pPcscReader_->disconnect();

		pPcscReader_->cleanup();	
		resetFields(false);
	}
	catch(PcscException ex)
	{
		MessageBox(ex.what(),"Pcsc Exception");
	}	
}

void CAcos3EncryptDlg::OnButtonQuit() 
{
	try
	{
		OnCancel();		
	}
	catch(PcscException ex)
	{
		MessageBox(ex.what(),"Pcsc Exception");
	}
}

void CAcos3EncryptDlg::OnRadioButtonNotEncrypted() 
{
	RadioButtonNotEncrypted.SetCheck(1);
	RadioButtonEncrypted.SetCheck(0);
	EditTextCardKey.SetLimitText(8);
	EditTextTerminalKey.SetLimitText(8);
	EditTextCardKey.SetWindowText("");
	EditTextTerminalKey.SetWindowText("");
	RadioButtonDes.SetCheck(1);
	RadioButton3Des.SetCheck(0);
	RadioButtonDes.EnableWindow(false);
	RadioButton3Des.EnableWindow(false);
}

void CAcos3EncryptDlg::OnRadioButtonEncrypted() 
{
	RadioButtonNotEncrypted.SetCheck(0);
	RadioButtonEncrypted.SetCheck(1);
	RadioButtonDes.EnableWindow(true);
	RadioButton3Des.EnableWindow(true);
}

void CAcos3EncryptDlg::OnRadioButtonDes() 
{
	RadioButtonNotEncrypted.SetCheck(0);
	RadioButtonEncrypted.SetCheck(1);
	RadioButtonDes.SetCheck(1);
	RadioButton3Des.SetCheck(0);
	EditTextCardKey.SetLimitText(8);
	EditTextTerminalKey.SetLimitText(8);
	EditTextValue.SetLimitText(8);
	EditTextCardKey.SetWindowText("");
	EditTextTerminalKey.SetWindowText("");	
}

void CAcos3EncryptDlg::OnRadioButton3Des() 
{
	RadioButtonNotEncrypted.SetCheck(0);
	RadioButtonEncrypted.SetCheck(1);
	RadioButtonDes.SetCheck(0);
	RadioButton3Des.SetCheck(1);
	EditTextCardKey.SetLimitText(16);
	EditTextTerminalKey.SetLimitText(16);
	EditTextValue.SetLimitText(8);
	EditTextCardKey.SetWindowText("");
	EditTextTerminalKey.SetWindowText("");
}
