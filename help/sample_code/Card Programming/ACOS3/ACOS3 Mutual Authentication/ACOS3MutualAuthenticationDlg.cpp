// ACOS3MutualAuthenticationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ACOS3MutualAuthentication.h"
#include "ACOS3MutualAuthenticationDlg.h"

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

#define MAX 262

//Global Variables
CACOS3MutualAuthenticationDlg *_pThis = NULL;
Helper _cHelper;
byte _pIssuerCode[] = { 0x41, 0x43, 0x4F, 0x53, 0x54, 0x45, 0x53, 0x54 };

//Function Prototypes
void onSendCommand(const byte* sendData, const int len);
void onReceiveCommand(const byte* receiveData, const int len);

/////////////////////////////////////////////////////////////////////////////
// CACOS3MutualAuthenticationDlg dialog

CACOS3MutualAuthenticationDlg::CACOS3MutualAuthenticationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CACOS3MutualAuthenticationDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CACOS3MutualAuthenticationDlg)
	TextBoxLogsValue = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

CACOS3MutualAuthenticationDlg::~CACOS3MutualAuthenticationDlg()
{
	
}

void CACOS3MutualAuthenticationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CACOS3MutualAuthenticationDlg)
	DDX_Control(pDX, RadioButton2, RadioButton3Des);
	DDX_Control(pDX, RadioButton1, RadioButtonDes);
	DDX_Control(pDX, TextBox3, TextBoxLogs);
	DDX_Control(pDX, Button7, ButtonQuit);
	DDX_Control(pDX, TextBox2, TextBoxTerminalKey);
	DDX_Control(pDX, TextBox1, TextBoxCardKey);
	DDX_Control(pDX, ComboBox1, ComboBoxReader);
	DDX_Control(pDX, Button6, ButtonReset);
	DDX_Control(pDX, Button5, ButtonClear);
	DDX_Control(pDX, Button4, ButtonMutualAuthentication);
	DDX_Control(pDX, Button3, ButtonFormatCard);
	DDX_Control(pDX, Button2, ButtonConnect);
	DDX_Control(pDX, Button1, ButtonInitialize);
	DDX_Text(pDX, TextBox3, TextBoxLogsValue);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CACOS3MutualAuthenticationDlg, CDialog)
	//{{AFX_MSG_MAP(CACOS3MutualAuthenticationDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(Button1, OnButtonInitialize)
	ON_BN_CLICKED(Button2, OnButtonConnect)
	ON_BN_CLICKED(Button3, OnButtonFormat)
	ON_BN_CLICKED(Button4, OnButtonMutualAuthentication)
	ON_BN_CLICKED(Button5, OnButtonClear)
	ON_BN_CLICKED(Button6, OnButtonReset)
	ON_BN_CLICKED(Button7, OnQuit)
	ON_BN_CLICKED(RadioButton1, OnRadioButtonDes)
	ON_BN_CLICKED(RadioButton2, OnRadioButton3Des)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CACOS3MutualAuthenticationDlg message handlers

BOOL CACOS3MutualAuthenticationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CACOS3MutualAuthenticationDlg::OnPaint() 
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
HCURSOR CACOS3MutualAuthenticationDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CACOS3MutualAuthenticationDlg::onPcscException(PcscException cException)
{
	sprintf(aMessage_,"\r\n%s",CString(cException.what()));
	displayMessage(aMessage_);
	MessageBox(cException.what(), "Error", MB_ICONERROR);
}
void CACOS3MutualAuthenticationDlg::onCardException(CardException cException)
{
	sprintf(aMessage_,"\r\n[%02X %02X] %s",cException.getStatusWord()[0], 
										   cException.getStatusWord()[1], 
										   CString(cException.getMessage()));
	displayMessage(aMessage_);
	MessageBox(aMessage_, "Error", MB_ICONERROR);
}

//Helper
void CACOS3MutualAuthenticationDlg::resetFields(BOOL enable)
{	
	ButtonConnect.EnableWindow(enable);
	ComboBoxReader.ResetContent();
	TextBoxLogs.SetWindowText("Program Ready\r\n");
	UpdateData(true);
	enableFields(enable);

	TextBoxCardKey.SetWindowText("");
	TextBoxTerminalKey.SetWindowText("");

	RadioButtonDes.SetCheck(1);
	RadioButton3Des.SetCheck(0);	
	TextBoxCardKey.SetLimitText(8);	
	TextBoxTerminalKey.SetLimitText(8);
}

void CACOS3MutualAuthenticationDlg::enableFields(BOOL bEnable)
{
	ButtonFormatCard.EnableWindow(bEnable);
	ButtonMutualAuthentication.EnableWindow(bEnable);
	
	RadioButtonDes.EnableWindow(bEnable);
	RadioButton3Des.EnableWindow(bEnable);

	TextBoxCardKey.EnableWindow(bEnable);
	TextBoxTerminalKey.EnableWindow(bEnable);	
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

	_pThis->displayMessage((CString)"\r\n< " + (CString) pStr);

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

	_pThis->displayMessage((CString)"> " + (CString) pStr);

	delete [] pStr;
}

//Display the message to the log window
void CACOS3MutualAuthenticationDlg::displayMessage(CString sMessage)
{
    TextBoxLogsValue += sMessage + "\r\n";
	UpdateData(false);
	TextBoxLogs.SetSel(0xFFFF, 0xFFFF);
}

void CACOS3MutualAuthenticationDlg::OnButtonInitialize() 
{	
	string *pReaderList = new string[10];
	uint8 uReaderCount = 10;
	int i = 0;

	try
	{
		ComboBoxReader.ResetContent();
		enableFields(false);

		// Establish Context
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
								
		displayMessage("\r\nInitialize success");
		ButtonConnect.EnableWindow(true);
		ComboBoxReader.SetCurSel(0);
	}
	catch (PcscException pcscException)
	{
		onPcscException(pcscException);
	}	
}

void CACOS3MutualAuthenticationDlg::OnButtonConnect() 
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
			displayMessage("Chip Type ACOS3");
			enableFields(true);

			
			ButtonMutualAuthentication.EnableWindow(false);
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

void CACOS3MutualAuthenticationDlg::OnRadioButtonDes() 
{	
	RadioButtonDes.SetCheck(1);
	RadioButton3Des.SetCheck(0);	
	TextBoxCardKey.SetWindowText("");
	TextBoxCardKey.SetLimitText(8);
	TextBoxTerminalKey.SetWindowText("");	
	TextBoxTerminalKey.SetLimitText(8);
}

void CACOS3MutualAuthenticationDlg::OnRadioButton3Des() 
{	
	RadioButtonDes.SetCheck(0);
	RadioButton3Des.SetCheck(1);	
	TextBoxCardKey.SetWindowText("");
	TextBoxCardKey.SetLimitText(16);
	TextBoxTerminalKey.SetWindowText("");
	TextBoxTerminalKey.SetLimitText(16);	
}

void CACOS3MutualAuthenticationDlg::OnButtonFormat() 
{	
	byte aFile[2];
	byte aData[10];
	char aCardKey[100], aTerminalKey[100];
	int iIndex;
	
	try
	{

		TextBoxCardKey.GetWindowText(aCardKey, 100);
		if(strcmp(aCardKey, "") == 0)
		{
			MessageBox("Please key-in numeric value for Card Key", "Error", MB_ICONERROR);			
			TextBoxCardKey.SetFocus();
			return;
		}

		TextBoxTerminalKey.GetWindowText(aTerminalKey, 100);
		if(strcmp(aTerminalKey, "") == 0)
		{
			MessageBox("Please key-in numeric value for Terminal Key", "Error", MB_ICONERROR);			
			TextBoxTerminalKey.SetFocus();
			return;
		}

		//Validate input
		if (RadioButtonDes.GetCheck() == 1)
		{			
			if (strlen(aCardKey) < 8) 
			{
				MessageBox("Invalid input length. Length must be 8.", "Error", MB_ICONERROR);			
				TextBoxCardKey.SetFocus();
				return;
			}

			
			if (strlen(aTerminalKey) < 8) 
			{
				MessageBox("Invalid input length. Length must be 8.", "Error", MB_ICONERROR);			
				TextBoxTerminalKey.SetFocus();
				return;
			}
		}
		else
		{
			if (strlen(aCardKey) < 16) 
			{
				MessageBox("Invalid input length. Length must be 16.", "Error", MB_ICONERROR);			
				TextBoxCardKey.SetFocus();
				return;
			}

			if (strlen(aTerminalKey) < 16) 
			{
				MessageBox("Invalid input length. Length must be 16.", "Error", MB_ICONERROR);		
				TextBoxTerminalKey.SetFocus();
				return;
			}
		}
	
		// Submit code
		pAcos3_->submitCode(IC, _pIssuerCode);

		// Clear card
		pAcos3_->clearCard();

		// Submit code
		pAcos3_->submitCode(IC, _pIssuerCode);
		
		// Select file FF 02
		aFile[0] = 0xFF;
		aFile[1] = 0x02;
		pAcos3_->selectFile(aFile);		

		// Write to FF 02
		// This will create 3 User files, no option register and
		// Security option registers defined, personalization bit is not set.
		if(RadioButtonDes.GetCheck() == 1)
		{
			aData[0] = 0x00;
		}
		else
		{
			aData[0] = 0x02;
		}

		aData[1] = 0x00;
		aData[2] = 0x03;
		aData[3] = 0x00;
		pAcos3_->writeRecord(0x00, 0x00, aData, 0x04);
		displayMessage("Update FF 02 OK");
		
		// Submit code
		pAcos3_->submitCode(IC, _pIssuerCode);

		// Select file FF 03
		aFile[0] = 0xFF;
		aFile[1] = 0x03;
		pAcos3_->selectFile(aFile);
		
		//Write the keys to the card
		if (RadioButtonDes.GetCheck() == 1)
		{			
			//	Record 02 for Card key						
			for (iIndex = 0; iIndex < 8; iIndex++)
			{        
				aData[iIndex] = aCardKey[iIndex];            
			}
			pAcos3_->writeRecord(0x02, 0x00, aData, 0x08);
			displayMessage("Write card key OK");

			//  Record 03 for Terminal key		
			for (iIndex = 0; iIndex < 8; iIndex++)
			{        
				aData[iIndex] = aTerminalKey[iIndex];            
			}
			pAcos3_->writeRecord(0x03, 0x00, aData, 0x08);
			displayMessage("Write terminal key OK");
		}
		else
		{
			//	Record 02 for Card key
			for (iIndex = 0; iIndex < 8; iIndex++)
			{        
				aData[iIndex] = aCardKey[iIndex];            
			}
			pAcos3_->writeRecord(0x02, 0x00, aData, 0x08);
			displayMessage("Write left half of card key OK");

			//	Record 12 for Card key
			for (iIndex = 0; iIndex < 8; iIndex++)
			{        
				aData[iIndex] = aCardKey[iIndex + 8];            
			}
			pAcos3_->writeRecord(0x0C, 0x00, aData, 0x08);
			displayMessage("Write right half of card key OK");

			//  Record 03 for Terminal key		
			for (iIndex = 0; iIndex < 8; iIndex++)
			{        
				aData[iIndex] = aTerminalKey[iIndex];            
			}
			pAcos3_->writeRecord(0x03, 0x00, aData, 0x08);
			displayMessage("Write left half of terminal key OK");

			//  Record 13 for Terminal key					
			for (iIndex = 0; iIndex < 8; iIndex++)
			{        
				aData[iIndex] = aTerminalKey[iIndex + 8];            
			}
			pAcos3_->writeRecord(0x0D, 0x00, aData, 0x08);
			displayMessage("Write right half of terminal key OK");
		}

		displayMessage("Format card OK");
		ButtonMutualAuthentication.EnableWindow(true);
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

void CACOS3MutualAuthenticationDlg::OnButtonMutualAuthentication() 
{	
	BYTE aSequenceNumber[8];
	BYTE aTempArray[31];
	
	char aCardKey[100], aTerminalKey[100];
	unsigned char aTempBuffer[8];	
	unsigned char aSessionKey [16]; // Holds the Session Key as computed by mutual authentication	
	unsigned char aCKey[16];
	unsigned char aTKey[16];
	unsigned char aTempResult[32];
	unsigned char aCRnd[8];		//Card random number
	unsigned char aTRnd[8];		//Terminal random number
	unsigned char aReverseKey[32]; //Reverse of Terminal Key
	unsigned int uIndex;

	//Validate input

	TextBoxCardKey.GetWindowText(aCardKey, 100);
	if(strcmp(aCardKey, "") == 0)
	{
		MessageBox("Please key-in numeric value for Card Key", "Error", MB_ICONERROR);			
		TextBoxCardKey.SetFocus();
		return;
	}

	TextBoxTerminalKey.GetWindowText(aTerminalKey, 100);
	if(strcmp(aTerminalKey, "") == 0)
	{
		MessageBox("Please key-in numeric value for Terminal Key", "Error", MB_ICONERROR);			
		TextBoxTerminalKey.SetFocus();
		return;
	}

	if(RadioButtonDes.GetCheck() == 1)
	{
		if (strlen(aCardKey) < 8) 
		{
			MessageBox("Invalid input length. Length must be 8.", "Error", MB_ICONERROR);			
			TextBoxCardKey.SetFocus();
			return;
		}

		
		if (strlen(aTerminalKey) < 8) 
		{
			MessageBox("Invalid input length. Length must be 8.", "Error", MB_ICONERROR);			
			TextBoxTerminalKey.SetFocus();
			return;
		}
	}
	else
	{
		if (strlen(aCardKey) < 16) 
		{
			MessageBox("Invalid input length. Length must be 16.", "Error", MB_ICONERROR);			
			TextBoxCardKey.SetFocus();
			return;
		}

		if (strlen(aTerminalKey) < 16) 
		{
			MessageBox("Invalid input length. Length must be 16.", "Error", MB_ICONERROR);		
			TextBoxTerminalKey.SetFocus();
			return;
		}
	}

	try
	{	
		//Start session and generate card random number
		pAcos3_->startSession(aCRnd);
		displayMessage("Start session OK");

		for(uIndex = 0; uIndex < 6; uIndex++)
		{
			aSequenceNumber[uIndex] = 0x00;
		}

		aSequenceNumber[6] = aCRnd[6];
		aSequenceNumber[7] = aCRnd[7];

		//Retrieve Terminal Key from the Input Template		
		for (uIndex = 0;uIndex < strlen(aTerminalKey); uIndex++)
		{
			aTKey[uIndex] = aTerminalKey[uIndex];
		}

		//Encrypt Random No (cRnd) with Terminal Key (tKey)
		//aTempArray will hold the 8-byte Enrypted number
		for (uIndex = 0;uIndex < 8;uIndex++)
		{
			aTempArray[uIndex] = aCRnd[uIndex];
		}
		
		//Encrypt terminal key
		if(RadioButtonDes.GetCheck() == 1)
			pAcos3_->Acos3DES(aTempArray,aTKey, 1);
		else
			pAcos3_->Acos3DES3(aTempArray,aTKey, 1);

		//Issue Authenticate command using 8-byte Encrypted No (aTempArray)
		//and Random Terminal number (aTRnd)				
		for (uIndex = 0;uIndex < 8;uIndex++)
		{
			aTempArray[uIndex+8] = aTRnd[uIndex];
		}	

		pAcos3_->authenticate(aTempArray,aTRnd,aTempBuffer);
		displayMessage("Authenticate OK");

		for (uIndex = 0;uIndex < 8;uIndex++)
		{
			aTempResult[uIndex] = aTempBuffer[uIndex];
		}		
		
		//Retrieve Card Key from the Input Template	
		for (uIndex = 0;uIndex <= strlen(aCardKey) - 1;uIndex++)
		{
			aCKey[uIndex] = aCardKey[uIndex];
		}

		//Compute the Session Key
		if (RadioButtonDes.GetCheck() == 1)
		{
			//SessionKey = DES (DES(RNDc, KC) XOR RNDt, KT)
				
			//Calculate DES(cRnd,cKey)
			for (uIndex = 0;uIndex < 8;uIndex++)
			{
				aTempArray[uIndex] = aCRnd[uIndex];
			}
		
			pAcos3_->Acos3DES(aTempArray, aCKey, 1);
			
			//XOR the result with aTRnd
			for (uIndex = 0;uIndex < 8;uIndex++)
			{
				aTempArray[uIndex] = aTempArray[uIndex] ^= aTRnd[uIndex];
			}		

			//DES the result with tKey
			pAcos3_->Acos3DES(aTempArray, aTKey, 1);

			for (uIndex = 0;uIndex < 8;uIndex++)
			{
				aSessionKey[uIndex] = aTempArray[uIndex]; 
			}
		}
		else
		{
			//Left half aSessionKey =  3DES (3DES (cRnd, cKey), tKey)
			//Right half aSessionKey = 3DES (aTRnd, REV (tKey))
			//aTempArray = 3DES (cRnd, cKey)
			for (uIndex = 0;uIndex < 8;uIndex++)
			{
				aTempArray[uIndex] = aCRnd[uIndex];
			}	

			pAcos3_->Acos3DES3(aTempArray, aCKey, 1);
			pAcos3_->Acos3DES3(aTempArray, aTKey, 1);
				
			//Left Half of Session Key is done
			for (uIndex = 0;uIndex < 8;uIndex++)
			{
				aSessionKey[uIndex] = aTempArray[uIndex];
			}	

			//Compute reverseKey of tKey
			//Just swap its left side with right side
			//reverseKey = right half of tKey + left half of tKey

			for (uIndex = 0;uIndex<8;uIndex++)
			{
				aReverseKey[uIndex] = aTKey[8 + uIndex];
			}	
			
			for (uIndex = 0;uIndex < 8;uIndex++)
			{
				aReverseKey[8 + uIndex] = aTKey[uIndex];
			}
			
			//Compute aTempArray = 3DES (aTRnd, reverseKey)
			for (uIndex = 0;uIndex < 8;uIndex++)
			{
				aTempArray[uIndex] = aTRnd[uIndex];
			}	
			
			pAcos3_->Acos3DES3(aTempArray, aReverseKey, 1);
				
			//Right Half of Session Key is done
			for (uIndex = 0;uIndex < 8;uIndex++)
			{
				aSessionKey[uIndex + 8] = aTempArray[uIndex];
			}
		}

		displayMessage("Generate session key OK");
		
		//Validate computed session key
		for (uIndex = 0;uIndex<8;uIndex++)
		{
			aTempArray[uIndex] = aTRnd[uIndex];
		}	
			
		if (RadioButtonDes.GetCheck() == 1)
			pAcos3_->Acos3DES(aTempArray, aSessionKey, 1);	
		else
			pAcos3_->Acos3DES3(aTempArray, aSessionKey, 1);
		
		for (uIndex = 0;uIndex<8;uIndex++)
		{
			if (aTempResult[uIndex] != aTempArray[uIndex])
			{			
				displayMessage("Validate session key failed");
				return;
			}
		}	
			
		displayMessage("Mutual Authentication OK");
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

void CACOS3MutualAuthenticationDlg::OnButtonClear() 
{
	TextBoxLogs.SetWindowText("");
	UpdateData(true);
	
}

void CACOS3MutualAuthenticationDlg::OnButtonReset() 
{
	// TODO: Add your control notification handler code here
	resetFields(false);
}

void CACOS3MutualAuthenticationDlg::OnQuit() 
{
	try
	{
		OnCancel();
	}
	catch (PcscException ex)
	{
		MessageBox(ex.what(), "PCSC Exception");
	}
}