// ACOS3 Secure MessagingDlg.cpp : implementation file
//
#pragma once

#include "stdafx.h"
#include "ACOS3 Secure Messaging.h"
#include "ACOS3 Secure MessagingDlg.h"
#include "winscard.h"
#include "PcscProvider.h"
#include "PcscReader.h"
#include "Helper.h"
#include "Acos3.h"
#include "CardSelector.h"

#define MAX 262

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CACOS3SecureMessagingDlg	*_pThis = NULL;
Helper _cHelper;
byte _aIssuerCode[] = { 0x41, 0x43, 0x4F, 0x53, 0x54, 0x45, 0x53, 0x54 };

BYTE _aSequenceNum[8];

unsigned char*	_pSessionKey;    // this variable holds the Session Key as computed by Mut. Auth'n
BYTE _aTmpArray[31];
unsigned char _aCKey[16];
unsigned char _aTKey[16];
unsigned char _aTmpResult[32];
unsigned char _aCRnd[8];		//Card random number
unsigned char _aTRnd[8];		//Terminal random number
unsigned char _aReverseKey[32]; //Reverse of Terminal Key

// Function prototypes
void onSendCommand(const byte* pSendData, const int iLen);
void onReceiveCommand(const byte* pReceiveData, const int iLen);

/////////////////////////////////////////////////////////////////////////////
// CACOS3SecureMessagingDlg dialog

CACOS3SecureMessagingDlg::CACOS3SecureMessagingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CACOS3SecureMessagingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CACOS3SecureMessagingDlg)
	sEditBoxApduLogsValue = _T("");
	sEditBoxDataValue = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CACOS3SecureMessagingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CACOS3SecureMessagingDlg)
	DDX_Control(pDX, EditBoxData, m_EditBoxData);
	DDX_Control(pDX, ButtonClear, m_ButtonClear);
	DDX_Control(pDX, RadioButtonDes, m_RadioButtonDes);
	DDX_Control(pDX, RadioButton3Des, m_RadioButton3Des);
	DDX_Control(pDX, EditBoxTerminalKey, m_EditBoxTerminalKey);
	DDX_Control(pDX, EditBoxReadWriteLength, m_EditBoxReadWiteLength);
	DDX_Control(pDX, EditBoxReadWriteFileId2, m_EditBoxReafWriteFieldId2);
	DDX_Control(pDX, EditBoxReadWriteFileId1, m_EditBoxReadWriteFieldId1);
	DDX_Control(pDX, EditBoxOffset2, m_EditBoxOffset2);
	DDX_Control(pDX, EditBoxOffset1, m_EditBoxOffset1);
	DDX_Control(pDX, EditBoxLength2, m_EditBoxLength2);
	DDX_Control(pDX, EditBoxLength1, m_EditBoxLength1);
	DDX_Control(pDX, EditBoxCardKey, m_EditBoxCardKey);
	DDX_Control(pDX, EditBoxCardFormatFileId2, m_EditBoxCardFormatFieldId2);
	DDX_Control(pDX, EditBoxCardFormatFileId1, m_EditBoxCardFormatFieldId1);
	DDX_Control(pDX, EditBoxApduLogs, m_EditBoxApduLogs);
	DDX_Control(pDX, ComboBoxReaderNames, m_ComboBoxReaderNames);
	DDX_Control(pDX, CheckBoxSecureMessage, m_CheckBoxSecureMessaging);
	DDX_Control(pDX, ButtonWriteBinary, m_ButonWriteBinary);
	DDX_Control(pDX, ButtonReset, m_ButtonReset);
	DDX_Control(pDX, ButtonReadBinary, m_ButtonReadBinary);
	DDX_Control(pDX, ButtonQuit, m_ButtonQuit);
	DDX_Control(pDX, ButtonInitialize, m_ButtonInitialize);
	DDX_Control(pDX, ButtonFormatCard, m_ButtonFormatCard);
	DDX_Control(pDX, ButtonConnect, m_ButtonConnect);
	DDX_Control(pDX, ButtonAuthenticate, m_ButtonAuthenticate);
	DDX_Text(pDX, EditBoxApduLogs, sEditBoxApduLogsValue);
	DDX_Text(pDX, EditBoxData, sEditBoxDataValue);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CACOS3SecureMessagingDlg, CDialog)
	//{{AFX_MSG_MAP(CACOS3SecureMessagingDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ButtonInitialize, OnButtonInitialize)
	ON_BN_CLICKED(ButtonConnect, OnButtonConnect)
	ON_BN_CLICKED(ButtonFormatCard, OnButtonFormatCard)
	ON_BN_CLICKED(ButtonAuthenticate, OnButtonAuthenticate)
	ON_BN_CLICKED(ButtonWriteBinary, OnButtonWriteBinary)
	ON_BN_CLICKED(ButtonReadBinary, OnButtonReadBinary)
	ON_BN_CLICKED(ButtonClear, OnButtonClear)
	ON_BN_CLICKED(ButtonReset, OnButtonReset)
	ON_BN_CLICKED(ButtonQuit, OnButtonQuit)
	ON_BN_CLICKED(RadioButtonDes, OnRadioButtonDes)
	ON_BN_CLICKED(RadioButton3Des, OnRadioButton3Des)
	ON_EN_KILLFOCUS(EditBoxReadWriteLength, OnKillfocusEditBoxReadWriteLength)
	ON_EN_CHANGE(EditBoxReadWriteLength, OnChangeEditBoxReadWriteLength)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CACOS3SecureMessagingDlg message handlers

BOOL CACOS3SecureMessagingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	_pThis = this;

	// Instantiate an object of the PcscReader-derived class
	pPcscReader_ = new PcscReader();


	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	sEditBoxApduLogsValue = "Program Ready\r\n";
	UpdateData(false);

	m_EditBoxCardKey.LimitText(8);
	m_EditBoxTerminalKey.LimitText(8);
	m_EditBoxCardFormatFieldId1.LimitText(2);
	m_EditBoxCardFormatFieldId2.LimitText(2);
	m_EditBoxLength1.LimitText(2);
	m_EditBoxLength2.LimitText(2);
	m_EditBoxOffset1.LimitText(2);
	m_EditBoxOffset2.LimitText(2);
	m_EditBoxReadWiteLength.LimitText(2);
	m_EditBoxReadWriteFieldId1.LimitText(2);
	m_EditBoxReafWriteFieldId2.LimitText(2);

	m_RadioButtonDes.SetCheck(1);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CACOS3SecureMessagingDlg::OnPaint() 
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
HCURSOR CACOS3SecureMessagingDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//Error checking for inputs that needs to be in hex format
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

void CACOS3SecureMessagingDlg::onPcscException(PcscException cException)
{
	sprintf(aMessage_,"\r\n%s",CString(cException.what()));
	displayMessage(aMessage_);
	MessageBox(cException.what(), "Error", MB_ICONERROR);
}

void CACOS3SecureMessagingDlg::onCardException(CardException cException)
{
	sprintf(aMessage_,"\r\n[%02X %02X] %s",cException.getStatusWord()[0], 
										 cException.getStatusWord()[1], 
										 CString(cException.getMessage()));
	displayMessage(aMessage_);
	MessageBox(aMessage_, "Error", MB_ICONERROR);
}

void CACOS3SecureMessagingDlg::OnButtonInitialize() 
{
	string *pReaderList = new string[10];
	uint8 uReaderCount = 10;
	int i = 0;

	try
	{
		m_ComboBoxReaderNames.ResetContent();
		enableControls(false);

		// Establish Context
		pPcscReader_->initialize();

		// List the detected smart card readers
		pPcscReader_->listReaders(pReaderList, uReaderCount);


		// Load the retrieved reader name array to the reader list
		for(i = 0; i < uReaderCount; i++)
		{
			m_ComboBoxReaderNames.AddString(pReaderList[i].data());
		}
		
		m_ComboBoxReaderNames.SetCurSel(0);

		// Pass the function pointers to serve as the event listeners of the ProprietaryReader class
		pPcscReader_->setSendCommandFunctor(onSendCommand);
		pPcscReader_->setRecvCommandFunctor(onReceiveCommand);
					
		m_ButtonConnect.EnableWindow(true);		

		m_EditBoxApduLogs.Clear();
        displayMessage("\r\nInitialize success");
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
	char *pStr;

	pStr = _cHelper.byteAsString(pSendData, iLen, true);
	if (pStr == NULL)
	{
		return;
	}

	_pThis->displayMessage((CString)"\r\n<< " + (CString) pStr);

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

	_pThis->displayMessage((CString)">> " + (CString) pStr);

	delete [] pStr;
}

//Display the message to the log window
void CACOS3SecureMessagingDlg::displayMessage(CString sMessage)
{
    sEditBoxApduLogsValue += sMessage + "\r\n";
	UpdateData(false);
	m_EditBoxApduLogs.SetSel(0xFFFF, 0xFFFF);
}

void CACOS3SecureMessagingDlg::enableControls(bool bIsEnable)
{
	m_ButonWriteBinary.EnableWindow(bIsEnable);
	m_ButtonAuthenticate.EnableWindow(bIsEnable);
	m_ButtonConnect.EnableWindow(bIsEnable);
	m_ButtonFormatCard.EnableWindow(bIsEnable);
	m_ButtonReadBinary.EnableWindow(bIsEnable);
	m_CheckBoxSecureMessaging.EnableWindow(bIsEnable);
	m_EditBoxCardFormatFieldId1.EnableWindow(bIsEnable);
	m_EditBoxCardFormatFieldId2.EnableWindow(bIsEnable);
	m_EditBoxCardKey.EnableWindow(bIsEnable);
	m_EditBoxLength1.EnableWindow(bIsEnable);
	m_EditBoxLength2.EnableWindow(bIsEnable);
	m_EditBoxOffset1.EnableWindow(bIsEnable);
	m_EditBoxOffset2.EnableWindow(bIsEnable);
	m_EditBoxReadWiteLength.EnableWindow(bIsEnable);
	m_EditBoxReadWriteFieldId1.EnableWindow(bIsEnable);
	m_EditBoxReafWriteFieldId2.EnableWindow(bIsEnable);
	m_EditBoxTerminalKey.EnableWindow(bIsEnable);
	m_EditBoxData.EnableWindow(bIsEnable);
	m_RadioButton3Des.EnableWindow(bIsEnable);
	m_RadioButtonDes.EnableWindow(bIsEnable);
}

void CACOS3SecureMessagingDlg::OnButtonConnect() 
{
	CString sReaderName;
	char aMessage[100];

	uint8 aAtr[100];
	uint32 uAtrLen = sizeof(aAtr);
	CString sCardName;
	CardSelector *pCardSelector;
	
	m_ComboBoxReaderNames.GetWindowText(sReaderName);

	try
	{
		// Check if a reader name is selected
		if (!strcmp(sReaderName, ""))
		{
			MessageBox("Select a smartcard reader", "Error", MB_ICONERROR);
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
			enableControls(true);
		}
		else
		{
			MessageBox("Card not supported. Please use ACOS3 card.", "Error", MB_ICONERROR);
			enableControls(false);			
		}

		m_ButtonConnect.EnableWindow(true);
		m_ButtonAuthenticate.EnableWindow(false);
		m_ButonWriteBinary.EnableWindow(false);
		m_ButtonReadBinary.EnableWindow(false);
		m_EditBoxData.EnableWindow(false);
		m_EditBoxOffset1.EnableWindow(false);
		m_EditBoxOffset2.EnableWindow(false);
		m_EditBoxReadWiteLength.EnableWindow(false);
		m_EditBoxReadWriteFieldId1.EnableWindow(false);
		m_EditBoxReafWriteFieldId2.EnableWindow(false);

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


void CACOS3SecureMessagingDlg::OnButtonFormatCard() 
{
	byte aFile[2];
	byte aData[10];

	char tempstr[MAX], tempstr2[MAX], holder[4];
	char aFileId1[3], aFileId2[3], aLength1[3], aLength2[3];
	char aCardKey[100], aTerminalKey[100];
	int iIndex, iTempval, iLength1, iLength2;
	BYTE aTemparr[32];

	try
	{
		m_EditBoxCardKey.GetWindowText(aCardKey, 100);
		if(strcmp(aCardKey, "") == 0)
		{
			MessageBox("Please key-in numeric value for Card Key.", "Error", MB_ICONERROR);
			m_EditBoxCardKey.SetFocus();
			return;
		}

		m_EditBoxTerminalKey.GetWindowText(aTerminalKey, 100);
		if(strcmp(aTerminalKey, "") == 0)
		{
			MessageBox("Please key-in numeric value for Terminal Key.", "Error", MB_ICONERROR);
			m_EditBoxTerminalKey.SetFocus();
			return;
		}

		if (m_RadioButtonDes.GetCheck() == 1)
		{			
			if (strlen(aCardKey) < 8) 
			{
				MessageBox("Invalid input length. Length must be 8", "Error", MB_ICONERROR);
				m_EditBoxCardKey.SetFocus();
				return;
			}
			
			if (strlen(aTerminalKey) < 8) 
			{
				MessageBox("Invalid input length. Length must be 8", "Error", MB_ICONERROR);
				m_EditBoxTerminalKey.SetFocus();
				return;
			}
		}
		else
		{
			if (strlen(aCardKey) < 16) 
			{
				MessageBox("Invalid input length. Length must be 16", "Error", MB_ICONERROR);
				m_EditBoxCardKey.SetFocus();
				return;
			}

			if (strlen(aTerminalKey) < 16) 
			{
				MessageBox("Invalid input length. Length must be 16", "Error", MB_ICONERROR);
				m_EditBoxTerminalKey.SetFocus();
				return;
			}
		}

		m_EditBoxCardFormatFieldId1.GetWindowText( aFileId1, 3);
		if( strcmp( aFileId1, "" ) == 0 || HexCheck( aFileId1[0], aFileId1[1] ) != 0 )
		{	
			MessageBox("Please key-in hex value for File ID", "Error", MB_ICONERROR);
			m_EditBoxCardFormatFieldId1.SetFocus();
			return;	
		}

		m_EditBoxCardFormatFieldId2.GetWindowText( aFileId2, 3);
		if( strcmp( aFileId2, "" ) == 0 || HexCheck( aFileId2[0], aFileId2[1] ) != 0 )
		{	
			MessageBox("Please key-in hex value for File ID", "Error", MB_ICONERROR);
			m_EditBoxCardFormatFieldId2.SetFocus();
			return;	
		}

		m_EditBoxLength1.GetWindowText( aLength1, 3);
		if( strcmp( aLength1, "" ) == 0 || HexCheck( aLength1[0], aLength1[1] ) != 0 )
		{	
			MessageBox("Please key-in hex value for Length", "Error", MB_ICONERROR);
			m_EditBoxLength1.SetFocus();
			return;	
		}

		m_EditBoxLength2.GetWindowText( aLength2, 3);
		if( strcmp( aLength2, "" ) == 0 || HexCheck( aLength2[0], aLength2[1] ) != 0 )
		{	
			MessageBox("Please key-in hex value for Length", "Error", MB_ICONERROR);
			m_EditBoxLength2.SetFocus();
			return;	
		}

		sscanf( aLength1, "%X", &iLength1);
		sscanf( aLength2, "%X", &iLength2);

		if((iLength1 == 0) && (iLength2 == 0))
		{
			m_EditBoxLength2.SetFocus();
			MessageBox("Please key-in valid Length. Valid value: 01h - FFFFh", "Error", MB_ICONERROR);
			return;	
		}

		// submit code
		displayMessage("\r\nSubmit Code - IC");
		pAcos3_->submitCode(IC, _aIssuerCode);

		displayMessage("\r\nClear Card");
		pAcos3_->clearCard();

		displayMessage("\r\nSubmit Code - IC");
		pAcos3_->submitCode(IC, _aIssuerCode);

		// select aFile ff 02
		displayMessage("\r\nSelect File - FF 02");
		aFile[0] = 0xFF;
		aFile[1] = 0x02;
		pAcos3_->selectFile(aFile);
		
		//Write to FF02
		//This will create 1 binary file, no Option registers and
		//Security Option registers defined, Personalization bit
		//is not set
		if (m_RadioButtonDes.GetCheck() == 1)
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
		displayMessage("\r\nWrite Record");
		pAcos3_->writeRecord(0x00, 0x00, aData, 0x04);
		displayMessage("FF 02 is updated");	

		// select file ff 03
		displayMessage("\r\nSelect File - FF 03");
		aFile[0] = 0xFF;
		aFile[1] = 0x03;
		pAcos3_->selectFile(aFile);

		if (m_RadioButtonDes.GetCheck() == 1)
		{
			//	Record 02 for Card key
			for (iIndex = 0; iIndex < 8; iIndex++)
			{        
				aData[iIndex] = aCardKey[iIndex];            
			}

			displayMessage("\r\nWrite Record");
			pAcos3_->writeRecord(0x02, 0x00, aData, 0x08);

			//  Record 03 for Terminal key					
			for (iIndex = 0; iIndex < 8; iIndex++)
			{        
				aData[iIndex] = aTerminalKey[iIndex];            
			}
			displayMessage("\r\nWrite Record");
			pAcos3_->writeRecord(0x03, 0x00, aData, 0x08);
		}
		else
		{
			//	Record 02 for Card key
			for (iIndex = 0; iIndex < 8; iIndex++)
			{        
				aData[iIndex] = aCardKey[iIndex];            
			}
			displayMessage("\r\nWrite Record");
			pAcos3_->writeRecord(0x02, 0x00, aData, 0x08);

			//	Record 12 for Card key
			for (iIndex = 0; iIndex < 8; iIndex++)
			{        
				aData[iIndex] = aCardKey[iIndex + 8];            
			}
			displayMessage("\r\nWrite Record");
			pAcos3_->writeRecord(0x0C, 0x00, aData, 0x08);

			//  Record 03 for Terminal key					
			for (iIndex = 0; iIndex < 8; iIndex++)
			{        
				aData[iIndex] = aTerminalKey[iIndex];            
			}
			displayMessage("\r\nWrite Record");
			pAcos3_->writeRecord(0x03, 0x00, aData, 0x08);

			//  Record 13 for Terminal key					
			for (iIndex = 0; iIndex < 8; iIndex++)
			{        
				aData[iIndex] = aTerminalKey[iIndex + 8];            
			}
			displayMessage("\r\nWrite Record");
			pAcos3_->writeRecord(0x0D, 0x00, aData, 0x08);
		}
		displayMessage("FF 03 is updated");	

		// select file ff 04
		displayMessage("\r\nSelect File - FF 04");
		aFile[0] = 0xFF;
		aFile[1] = 0x04;
		pAcos3_->selectFile(aFile);

		// submit code
		displayMessage("\r\nSubmit Code - IC");
		pAcos3_->submitCode(IC, _aIssuerCode);

		sscanf( aLength1, "%X", &iTempval );
		if( strcmp( aLength1, "" ) == 0 || HexCheck( aLength1[0], aLength1[1] ) != 0)
		{
			aTemparr[0] = 0x00;
		}			
		else
		{
			aTemparr[0] = iTempval;
		}

		sscanf( aLength2, "%X", &iTempval );
		aTemparr[1] = iTempval;
		aTemparr[2] = 0x00;
		aTemparr[3] = 0x00;

		sscanf( aFileId1, "%X", &iTempval );
		aTemparr[4] = iTempval;

		sscanf( aFileId2, "%X", &iTempval );
		aTemparr[5] = iTempval;

		if (m_CheckBoxSecureMessaging.GetCheck() == 1)
			aTemparr[6] = 0xE0;
		else
			aTemparr[6] = 0x80;

		displayMessage("\r\nWrite Record");
		pAcos3_->writeRecord(0x00, 0x00, aTemparr, 0x07);		

		displayMessage("Format Card successful");
		displayMessage("");

		m_ButtonAuthenticate.EnableWindow(true);
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

void CACOS3SecureMessagingDlg::OnButtonAuthenticate() 
{
	unsigned char aTmpBuff[8];
	char aCardKey[100], aTerminalKey[100];
	unsigned int uIndx;
	byte* pTemp = new byte[8];
	byte* pTRndBeta = new byte[8];

	_pSessionKey = new unsigned char[16];	

	m_EditBoxCardKey.GetWindowText(aCardKey, 100);

	if(strcmp(aCardKey, "") == 0)
	{
		MessageBox("Please key-in numeric value for Card Key.", "Error");
		m_EditBoxCardKey.SetFocus();
		delete [] pTemp;
		delete [] pTRndBeta;
		return;
	}

	m_EditBoxTerminalKey.GetWindowText(aTerminalKey, 100);

	if(strcmp(aTerminalKey, "") == 0)
	{
		MessageBox("Please key-in numeric value for Terminal Key.", "Error");
		m_EditBoxTerminalKey.SetFocus();
		delete [] pTemp;
		delete [] pTRndBeta;
		return;
	}

	if (m_RadioButtonDes.GetCheck() == 1)
	{		
		if (strlen(aCardKey) < 8) 
		{
			MessageBox("Invalid input length. Length must be 8", "Error");
			m_EditBoxCardKey.SetFocus();
			delete [] pTemp;
			delete [] pTRndBeta;
			return;
		}

		if (strlen(aTerminalKey) < 8) 
		{
			MessageBox("Invalid input length. Length must be 8", "Error");
			m_EditBoxTerminalKey.SetFocus();
			delete [] pTemp;
			delete [] pTRndBeta;
			return;
		}
	}
	else
	{
		if (strlen(aCardKey) < 16) 
		{
			MessageBox("Invalid input length. Length must be 16", "Error");
			m_EditBoxCardKey.SetFocus();
			delete [] pTemp;
			delete [] pTRndBeta;
			return;
		}

		if (strlen(aTerminalKey) < 16) 
		{
			MessageBox("Invalid input length. Length must be 8", "Error");
			m_EditBoxTerminalKey.SetFocus();
			delete [] pTemp;
			delete [] pTRndBeta;
			return;
		}
	}

	try
	{		
		displayMessage("\r\nStart Session");
		pAcos3_->startSession(_aCRnd);

		for(uIndx = 0; uIndx < 6; uIndx++)
		{
			_aSequenceNum[uIndx] = 0x00;
		}

		_aSequenceNum[6] = _aCRnd[6];
		_aSequenceNum[7] = _aCRnd[7];

		//Retrieve Terminal Key from Input Template
		for (uIndx = 0;uIndx < strlen(aTerminalKey); uIndx++)
		{
			_aTKey[uIndx] = aTerminalKey[uIndx];
		}

		// Encrypt Random No (_aCRnd) with Terminal Key (tKey)
		//    tmpArray1 will hold the 8-byte Enrypted number
		for (uIndx = 0;uIndx < 8;uIndx++)
		{
			_aTmpArray[uIndx] = _aCRnd[uIndx];
		}

		if (m_RadioButtonDes.GetCheck() == 1)
			pAcos3_->Acos3DES(_aTmpArray,_aTKey, 1);
		else
			pAcos3_->Acos3DES3(_aTmpArray,_aTKey, 1);

		// Issue Authenticate command using 8-byte Encrypted No (_aTmpArray)
		//    and Random Terminal number (TRnd)
		for (uIndx = 0;uIndx < 8;uIndx++)
		{
			_aTmpArray[uIndx+8] = _aTRnd[uIndx];
		}		

		displayMessage("\r\nAuthenticate");
		pAcos3_->authenticate(_aTmpArray, _aTRnd, aTmpBuff);

		for (uIndx = 0;uIndx < 8;uIndx++)
		{
			_aTmpResult[uIndx] = aTmpBuff[uIndx];
		}

		for (uIndx = 0;uIndx <= strlen(aCardKey) - 1;uIndx++)
		{
			_aCKey[uIndx] = aCardKey[uIndx];
		}

		if (m_RadioButtonDes.GetCheck() == 1)
		{
			// SessionKey = DES (DES(RNDc, KC) XOR RNDt, KT)
				
			// calculate DES(cRnd,cKey)
			for (uIndx = 0;uIndx < 8;uIndx++)
			{
				_aTmpArray[uIndx] = _aCRnd[uIndx];
			}
		
			pAcos3_->Acos3DES(_aTmpArray, _aCKey, 1);
			
			// XOR the result with tRnd
			for (uIndx = 0;uIndx < 8;uIndx++)
			{
				_aTmpArray[uIndx] = _aTmpArray[uIndx] ^= _aTRnd[uIndx];
			}		

			// DES the result with tKey
			pAcos3_->Acos3DES(_aTmpArray, _aTKey, 1);

			for (uIndx = 0;uIndx < 8;uIndx++)
			{
				_pSessionKey[uIndx] = _aTmpArray[uIndx]; 
			}
		}
		else
		{
			// Left half SessionKey =  3DES (3DES (CRnd, cKey), tKey)
			// Right half SessionKey = 3DES (TRnd, REV (tKey))
			// tmpArray1 = 3DES (CRnd, cKey)
			for (uIndx = 0;uIndx < 8;uIndx++)
			{
				_aTmpArray[uIndx] = _aCRnd[uIndx];
			}	

			pAcos3_->Acos3DES3(_aTmpArray, _aCKey, 1);

			pAcos3_->Acos3DES3(_aTmpArray, _aTKey, 1);
				
			//Left Half of Session Key is done
			for (uIndx = 0;uIndx < 8;uIndx++)
			{
				_pSessionKey[uIndx] = _aTmpArray[uIndx];
			}	

			// compute ReverseKey of tKey
			// just swap its left side with right side
			// ReverseKey = right half of tKey + left half of tKey
			for (uIndx = 0;uIndx<8;uIndx++)
			{
				_aReverseKey[uIndx] = _aTKey[8 + uIndx];
			}	
			
			for (uIndx = 0;uIndx < 8;uIndx++)
			{
				_aReverseKey[8 + uIndx] = _aTKey[uIndx];
			}
			
			// compute _aTmpArray = 3DES (TRnd, ReverseKey)

			for (uIndx = 0;uIndx < 8;uIndx++)
			{
				_aTmpArray[uIndx] = _aTRnd[uIndx];
			}	
			
			pAcos3_->Acos3DES3(_aTmpArray, _aReverseKey, 1);
				
			//Right Half of Session Key is done
			for (uIndx = 0;uIndx < 8;uIndx++)
			{
				_pSessionKey[uIndx + 8] = _aTmpArray[uIndx];
			}
		}

		for (uIndx = 0;uIndx < 8;uIndx++)
		{
			_aTmpArray[uIndx] = _aTRnd[uIndx];
		}	
			
		if (m_RadioButtonDes.GetCheck() == 1)
			pAcos3_->Acos3DES(_aTmpArray, _pSessionKey, 1);	
		else
			pAcos3_->Acos3DES3(_aTmpArray, _pSessionKey, 1);
		
		for (uIndx = 0;uIndx<8;uIndx++)
		{
			if (_aTmpResult[uIndx] != _aTmpArray[uIndx])
			{			
				displayMessage("Mutual Authentication failed");
				delete [] pTemp;
				delete [] pTRndBeta;
				return;
			}
		}	
		
		m_ButonWriteBinary.EnableWindow(true);
		m_ButtonReadBinary.EnableWindow(true);
		m_EditBoxData.EnableWindow(true);
		m_EditBoxOffset1.EnableWindow(true);
		m_EditBoxOffset2.EnableWindow(true);
		m_EditBoxReadWiteLength.EnableWindow(true);
		m_EditBoxReadWriteFieldId1.EnableWindow(true);
		m_EditBoxReafWriteFieldId2.EnableWindow(true);

		sEditBoxDataValue = "";		

		displayMessage("Mutual Authentication successful");
		displayMessage("");
	
	}
	catch (CardException cardException)
	{
		onCardException(cardException);
	}
	catch (PcscException pcscException)
	{
		onPcscException(pcscException);
	}

	delete [] pTemp;
	delete [] pTRndBeta;
	
}

void CACOS3SecureMessagingDlg::OnButtonWriteBinary() 
{
	char aFileId1[3], aFileId2[3], aOffset1[3], aOffset2[3], aLength[3], aData[255];
	int iIndex;
	int iTempval, iTemplen, iDes, iLengthToWrite;
	BYTE aFile[2], uHiByte, uLoByte;
	BYTE aTemparr[256];

	//memset(aTemparr,0x00,sizeof(aTemparr));
	memset(aTemparr,0x00, 256);

	m_EditBoxReadWriteFieldId1.GetWindowText( aFileId1, 3 );
	if( strcmp( aFileId1, "" ) == 0 || HexCheck( aFileId1[0], aFileId1[1] ) != 0 )
	{	
		MessageBox("Please key-in hex value for File ID.", "Error", MB_ICONERROR);
		m_EditBoxReadWriteFieldId1.SetFocus();
		return;	
	}
	m_EditBoxReafWriteFieldId2.GetWindowText( aFileId2, 3 );
	if( strcmp( aFileId2, "" ) == 0 || HexCheck( aFileId2[0], aFileId2[1] ) != 0 )
	{	
		MessageBox("Please key-in hex value for File ID.", "Error", MB_ICONERROR);
		m_EditBoxReafWriteFieldId2.SetFocus();
		return;	
	}
	m_EditBoxOffset1.GetWindowText( aOffset1, 3 );
	if( strcmp( aOffset1, "" ) == 0 || HexCheck( aOffset1[0], aOffset1[1] ) != 0 )
	{	
		MessageBox("Please key-in hex value for Offset.", "Error", MB_ICONERROR);
		m_EditBoxOffset1.SetFocus();
		return;	
	}
	m_EditBoxOffset2.GetWindowText( aOffset2, 3 );
	if( strcmp( aOffset2, "" ) == 0 || HexCheck( aOffset2[0], aOffset2[1] ) != 0 )
	{	
		MessageBox("Please key-in hex value for Offset.", "Error", MB_ICONERROR);
		m_EditBoxOffset2.SetFocus();
		return;	
	}
	m_EditBoxReadWiteLength.GetWindowText(aLength, 3);
	if( strcmp( aLength, "" ) == 0 || HexCheck( aLength[0], aLength[1] ) != 0 )
	{	
		MessageBox("Please key-in hex value for Length.", "Error", MB_ICONERROR);
		m_EditBoxReadWiteLength.SetFocus();
		return;	
	}

	m_EditBoxData.GetWindowText( aData, MAX );

	if( strcmp( aData, "" ) == 0 )
	{	
		MessageBox("Please key-in data to write.", "Error", MB_ICONERROR);
		m_EditBoxData.SetFocus();
		return;	
	}
	
	sscanf( aFileId1, "%X", &iTempval );
	aFile[0] = iTempval;

	sscanf( aFileId2, "%X", &iTempval );
	aFile[1] = iTempval;
	
	sscanf( aOffset1, "%X", &iTempval );
	if( strcmp( aOffset1, "" ) == 0 || HexCheck( aOffset1[0], aOffset1[1] ) != 0 )
	{
		m_EditBoxOffset1.SetWindowText( "00" );
		uHiByte = 0x00;	
	}
	else
	{	
		uHiByte = iTempval;	
	}
	
	sscanf( aOffset2, "%X", &iTempval );
	uLoByte = iTempval;

	sscanf( aLength, "%X", &iTempval );

	if (m_CheckBoxSecureMessaging.GetCheck() == 1)
	{
		if(iTempval <= 0 || iTempval > 240)
		{
			MessageBox("Please key-in valid Length. Valid value when Secure Messaging is enabled: 1h - F0h.", "Error", MB_ICONERROR);
			m_EditBoxReadWiteLength.SetFocus();
			return;	
		}
	}
	else
	{
		if(iTempval <= 0 || iTempval > 255)
		{
			MessageBox("Please key-in valid Length. Valid value: 1h - FFh.", "Error", MB_ICONERROR);
			m_EditBoxReadWiteLength.SetFocus();
			return;	
		}
	}

	iTemplen = iTempval;
	iLengthToWrite = iTemplen;	

	if(strlen(aData) > iTemplen)
	{
		MessageBox("Length of data is greater than the length specified.", "Error", MB_ICONERROR);
		m_EditBoxData.SetFocus();
		return;	
	}

	try
	{
		displayMessage("Select File");
		pAcos3_->selectFile(aFile);
	    
		//memset(aTemparr,0,sizeof(aTemparr));
		memset(aTemparr,0,256);

		//Write Data to card
		//m_EditBoxData.GetWindowText( tempdata, MAX );
		for( iIndex = 0; iIndex != strlen( aData ); iIndex++ )	
		{
			aTemparr[iIndex] = (char)aData[iIndex];
		}
	
		
		if (m_RadioButtonDes.GetCheck() == 1)
			iDes = 1;
		else
			iDes = 0;

		displayMessage("\r\nWrite Binary");
		if (m_CheckBoxSecureMessaging.GetCheck() == 1)
		{
			pAcos3_->writeBinarySM(1, uHiByte, uLoByte, iTemplen, aTemparr, _aSequenceNum, _pSessionKey, iDes);			
			displayMessage("Secure Messaging Success!");
		}
		else
		{
			try
			{
				pAcos3_->writeBinary(0, uHiByte, uLoByte, aTemparr, iTemplen);
			}
			catch(CardException cardException)
			{
				onCardException(cardException);
			}
		}

		m_EditBoxData.SetWindowText("");
		sEditBoxDataValue = "";
		UpdateData(false);
		
		displayMessage("\r\nWrite Binary success");
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

void CACOS3SecureMessagingDlg::OnButtonReadBinary() 
{
	char tempstr[MAX], aFileId1[3], aFileId2[3], aOffset1[3], aOffset2[3], aLength[3];
	int iIndex, iTempval, iTemplen, iDes;
	byte aFile[2];
	BYTE uHiByte, uLoByte;
	byte* pReadBufferString = NULL;	

	m_EditBoxReadWriteFieldId1.GetWindowText( aFileId1, 3);
	if( strcmp( aFileId1, "" ) == 0 || HexCheck( aFileId1[0], aFileId1[1] ) != 0 )
	{	
		MessageBox("Please key-in hex value for File ID.", "Error", MB_ICONERROR);
		m_EditBoxReadWriteFieldId1.SetFocus();
		return;	
	}
	m_EditBoxReafWriteFieldId2.GetWindowText( aFileId2, 3);
	if( strcmp( aFileId2, "" ) == 0 || HexCheck( aFileId2[0], aFileId2[1] ) != 0 )
	{	
		MessageBox("Please key-in hex value for File ID.", "Error", MB_ICONERROR);
		m_EditBoxReafWriteFieldId2.SetFocus();
		return;	
	}
	m_EditBoxOffset1.GetWindowText( aOffset1, 3);
	if( strcmp( aOffset1, "" ) == 0 || HexCheck( aOffset1[0], aOffset1[1] ) != 0 )
	{	
		MessageBox("Please key-in hex value for Offset.", "Error", MB_ICONERROR);
		m_EditBoxOffset1.SetFocus();
		return;	
	}
	m_EditBoxOffset2.GetWindowText( aOffset2, 3);
	if( strcmp( aOffset2, "" ) == 0 || HexCheck( aOffset2[0], aOffset2[1] ) != 0 )
	{	
		MessageBox("Please key-in hex value for Offset.", "Error", MB_ICONERROR);
		m_EditBoxOffset2.SetFocus();
		return;	
	}
	m_EditBoxReadWiteLength.GetWindowText( aLength, 3);
	if( strcmp( aLength, "" ) == 0 || HexCheck( aLength[0], aLength[1] ) != 0 )
	{	
		MessageBox("Please key-in hex value for Length.", "Error", MB_ICONERROR);
		m_EditBoxReadWiteLength.SetFocus();
		return;	
	}
	
	sscanf( aFileId1, "%X", &iTempval );
	aFile[0] = iTempval;

	sscanf( aFileId2, "%X", &iTempval );
	aFile[1] = iTempval;

	sscanf( aOffset1, "%X", &iTempval );
	if( strcmp( aOffset1, "" ) == 0 || HexCheck( aOffset1[0], aOffset1[1] ) != 0 )
	{		
		m_EditBoxOffset1.SetWindowText( "00" );
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

	if (m_CheckBoxSecureMessaging.GetCheck() == 1)
	{
		if(iTemplen <= 0 || iTemplen > 240)
		{
			MessageBox("Please key-in valid Length. Valid value when Secure Messaging is enabled: 1h - F0h.", "Error", MB_ICONERROR);
			m_EditBoxReadWiteLength.SetFocus();
			return;	
		}
	}
	else
	{
		if(iTemplen <= 0 || iTemplen > 255)
		{
			MessageBox("Please key-in valid Length. Valid value: 1h - FFh.","Error.", MB_ICONERROR);
			m_EditBoxReadWiteLength.SetFocus();
			return;	
		}
	}

	try
	{
	
		displayMessage("Select File");
		pAcos3_->selectFile(aFile);

		pReadBufferString = new byte[iTemplen];
		if(pReadBufferString == NULL)
		{
			displayMessage("Unable to Allocate Memory For Read Buffer");
			return;
		}		

		if (m_RadioButtonDes.GetCheck() == 1)
			iDes = 1;
		else
			iDes = 0;

		displayMessage("\r\nRead Binary");
		if (m_CheckBoxSecureMessaging.GetCheck() == 1)
		{
			
			pAcos3_->readBinarySM(uHiByte, uLoByte, 
				iTemplen, _aSequenceNum, _pSessionKey, pReadBufferString, iDes);

			sprintf( tempstr, "" );
			iIndex = 0;		
			iTempval = iTemplen;
		
			while( iIndex < iTempval )
			{	
				sprintf( tempstr, "%s%c", tempstr, pReadBufferString[iIndex] );
				iIndex++;	
			}
		
			m_EditBoxData.SetWindowText( tempstr );
			sEditBoxDataValue = tempstr;
			//UpdateData(true);
							
		}
		else
		{
			
			pAcos3_->readBinary(uHiByte, uLoByte, pReadBufferString, iTemplen);

			sprintf( tempstr, "" );
			iIndex = 0;		
			iTempval = iTemplen;
		
			while( iIndex < iTempval )
			{	
				sprintf( tempstr, "%s%c", tempstr, pReadBufferString[iIndex] );
				iIndex++;	
			}
		
			m_EditBoxData.SetWindowText( tempstr );
			sEditBoxDataValue = tempstr;
			//UpdateData(true);
		}
		
		displayMessage("Read Binary successful");
		
	}
	catch (CardException cardException)
	{
		m_EditBoxData.SetWindowText("");
		sEditBoxDataValue = "";

		onCardException(cardException);
	}
	catch (PcscException pcscException)
	{
		onPcscException(pcscException);
	}	
}

void CACOS3SecureMessagingDlg::OnButtonClear() 
{
	m_EditBoxApduLogs.SetWindowText("");
	sEditBoxApduLogsValue.Empty();
	
}

void CACOS3SecureMessagingDlg::OnButtonReset() 
{
	if (pPcscReader_ == NULL)
	{
		return;
	}

	if (pPcscReader_->isConnectionActive())
	{
		pPcscReader_->disconnect();
	}

	pPcscReader_->cleanup();
		

	enableControls(false);

	m_EditBoxTerminalKey.SetWindowText("");
	m_EditBoxCardKey.SetWindowText("");
	m_EditBoxCardFormatFieldId1.SetWindowText("");
	m_EditBoxCardFormatFieldId2.SetWindowText("");
	m_EditBoxOffset1.SetWindowText("");
	m_EditBoxOffset2.SetWindowText("");
	m_EditBoxLength1.SetWindowText("");
	m_EditBoxLength2.SetWindowText("");
	m_EditBoxReadWriteFieldId1.SetWindowText("");
	m_EditBoxReafWriteFieldId2.SetWindowText("");
	m_EditBoxReadWiteLength.SetWindowText("");
	m_EditBoxApduLogs.SetWindowText("");
	m_EditBoxData.SetWindowText("");

	m_ComboBoxReaderNames.ResetContent();

	m_RadioButton3Des.SetCheck(0);
	m_RadioButtonDes.SetCheck(1);
	m_CheckBoxSecureMessaging.SetCheck(0);

	sEditBoxApduLogsValue.Empty();
	sEditBoxDataValue.Empty();
	displayMessage("Program Ready");

	UpdateData();
	
}

void CACOS3SecureMessagingDlg::OnButtonQuit() 
{
	this->EndDialog(0);
	
}

void CACOS3SecureMessagingDlg::OnRadioButtonDes() 
{
	if(m_RadioButtonDes.GetCheck() == 1)
	{
		m_RadioButton3Des.SetCheck(0);

		m_EditBoxCardKey.SetWindowText("");
		m_EditBoxCardKey.LimitText(8);
		m_EditBoxTerminalKey.SetWindowText("");
		m_EditBoxTerminalKey.LimitText(8);
	}
}

void CACOS3SecureMessagingDlg::OnRadioButton3Des() 
{
	if(m_RadioButton3Des.GetCheck() == 1)
	{
		m_RadioButtonDes.SetCheck(0);

		m_EditBoxCardKey.SetWindowText("");
		m_EditBoxCardKey.LimitText(16);
		m_EditBoxTerminalKey.SetWindowText("");
		m_EditBoxTerminalKey.LimitText(16);
	}
	
}

void CACOS3SecureMessagingDlg::OnKillfocusEditBoxReadWriteLength() 
{
	int iLength;  
	unsigned int uTempval;
	char aLength[4];

	m_EditBoxReadWiteLength.GetWindowText( aLength, 4 );
	sscanf( aLength, "%X", &uTempval );
	iLength = uTempval;

	m_EditBoxData.LimitText(iLength);
	
}

void CACOS3SecureMessagingDlg::OnChangeEditBoxReadWriteLength() 
{
	m_EditBoxData.SetWindowText("");
	
}
