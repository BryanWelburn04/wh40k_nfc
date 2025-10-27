//===========================================================================================
// 
//  Author          : Charlotte Morfe
// 
//  File            : Mifare Change KeyDlg.cpp
// 
//  Copyright (C)   : Advanced Card Systems Ltd.
// 
//  Description     : This file contains all functions, methods and properties used for 
//                    reading and updating the sector trailer of mifare
// 
//  Date            : October 27, 2011
// 
//  Revision Traile : [Author] / [Date of modification] / [Details of Modifications done]
// 
// 
//=========================================================================================
//////////////////////////////////////////////////////////////////////
// Mifare Change KeyDlg.cpp : implementation file
//

#include "stdafx.h"
#include "winscard.h"
#include "Mifare Change Key.h"
#include "Mifare Change KeyDlg.h"

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


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// Global variables
CMifareChangeKeyDlg *pThis = NULL;
CButton buttonControls[17];
CEdit editboxControls[6];
CButton checkboxControls[8];
CButton radiobuttonControls[8];
Helper helper;
BYTE tempArray[262];
CString sCardName;

// Function prototupes
void onSendCommand(const byte* sendData, const int len);
void onReceiveCommand(const byte* receiveData, const int len);

/////////////////////////////////////////////////////////////////////////////
// CMifareChangeKeyDlg dialog

CMifareChangeKeyDlg::CMifareChangeKeyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMifareChangeKeyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMifareChangeKeyDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMifareChangeKeyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMifareChangeKeyDlg)
	DDX_Control(pDX, IDC_COMBO1, ComboBoxReaderNames);
	DDX_Control(pDX, IDC_ButtonInitialize, ButtonInitialize);
	DDX_Control(pDX, IDC_ButtonConnect, ButtonConnect);
	DDX_Control(pDX, IDC_ButtonLoadKey, ButtonLoadKey);
	DDX_Control(pDX, IDC_ButtonAuthenticate, ButtonAuthenticate);
	DDX_Control(pDX, IDC_ButtonRead, ButtonRead);
	DDX_Control(pDX, IDC_ButtonUpdate, ButtonUpdate);
	DDX_Control(pDX, IDC_ButtonClear, ButtonClearLogs);
	DDX_Control(pDX, IDC_ButtonReset, ButtonReset);
	DDX_Control(pDX, IDC_ButtonQuit, ButtonQuit);
	DDX_Control(pDX, IDC_RadioButtonKeyA, RadiobuttonKeyTypeKeyA);
	DDX_Control(pDX, IDC_RadioButtonKeyB, RadiobuttonKeyTypeKeyB);
	DDX_Control(pDX, IDC_EditBoxKeyStoreNo, EditboxLoadKeyStoreNumber);
	DDX_Control(pDX, IDC_EditBoxSectorNo, EditboxSectorNumber);	
	DDX_Control(pDX, IDC_EditBoxKeyStoreNoAuth, EditboxAuthenticateKeyStoreNumber);
	DDX_Control(pDX, IDC_EditBoxCurrentSector, EditboxCurrentSector);
	DDX_Control(pDX, IDC_EditBoxSectorTrailerBlock, EditboxSectorTrailerBlock);
	DDX_Control(pDX, IDC_EditBoxKeyA, EditboxKeyA);
	DDX_Text(pDX, IDC_EditBoxKeyA, EditboxKeyAString);
	DDX_Control(pDX, IDC_EditBoxAccessBits, EditboxAccessBits);
	DDX_Text(pDX, IDC_EditBoxAccessBits, EditboxAccessBitsString);
	DDX_Control(pDX, IDC_EditBoxKeyB, EditboxKeyB);
	DDX_Text(pDX, IDC_EditBoxKeyB, EditboxKeyBString);
	DDX_Control(pDX, IDC_LabelValidSectors, LabelValidSectors);
	DDX_Control(pDX, IDC_EditboxApduLogs, EditboxApduLogs);
	DDX_Text(pDX, IDC_EditboxApduLogs, EditboxApduLogsText);
	DDX_Control(pDX, IDC_EditBoxKeyStore6, TextBoxKeyVal6);
	DDX_Text(pDX, IDC_EditBoxKeyStore6, TextBoxValString6);
	DDX_Control(pDX, IDC_EditBoxKeyStore5, TextBoxKeyVal5);
	DDX_Text(pDX, IDC_EditBoxKeyStore5, TextBoxValString5);
	DDX_Control(pDX, IDC_EditBoxKeyStore4, TextBoxKeyVal4);
	DDX_Text(pDX, IDC_EditBoxKeyStore4, TextBoxValString4);
	DDX_Control(pDX, IDC_EditBoxKeyStore3, TextBoxKeyVal3);
	DDX_Text(pDX, IDC_EditBoxKeyStore3, TextBoxValString3);
	DDX_Control(pDX, IDC_EditBoxKeyStore1, TextBoxKeyVal1);
	DDX_Text(pDX, IDC_EditBoxKeyStore1, TextBoxValString1);
	DDX_Control(pDX, IDC_EditBoxKeyStore2, TextBoxKeyVal2);
	DDX_Text(pDX, IDC_EditBoxKeyStore2, TextBoxValString2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMifareChangeKeyDlg, CDialog)
	//{{AFX_MSG_MAP(CMifareChangeKeyDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ButtonClear, OnButtonClear)
	ON_BN_CLICKED(IDC_ButtonQuit, OnButtonQuit)
	ON_BN_CLICKED(IDC_ButtonReset, OnButtonReset)
	ON_BN_CLICKED(IDC_ButtonInitialize, OnButtonInitialize)
	ON_BN_CLICKED(IDC_ButtonConnect, OnButtonConnect)
	ON_BN_CLICKED(IDC_ButtonLoadKey, OnButtonLoadKey)
	ON_BN_CLICKED(IDC_ButtonAuthenticate, OnButtonAuthenticate)
	ON_BN_CLICKED(IDC_ButtonRead, OnButtonRead)
	ON_BN_CLICKED(IDC_ButtonUpdate, OnButtonUpdate)
	ON_EN_CHANGE(IDC_EditBoxKeys, OnChangeEditBoxKeys)
	ON_EN_CHANGE(IDC_EditBoxKeyA, OnChangeEditBoxKeyA)
	ON_EN_CHANGE(IDC_EditBoxAccessBits, OnChangeEditBoxAccessBits)
	ON_EN_CHANGE(IDC_EditBoxKeyB, OnChangeEditBoxKeyB)
	ON_CBN_EDITCHANGE(IDC_ComboBoxReaderList, OnEditchangeComboBoxPort)
	ON_CBN_EDITCHANGE(IDC_ComboBoxReaderList2, OnEditchangeComboBoxSlot)
	ON_EN_CHANGE(IDC_EditBoxKeyStore1, OnChangeEditBoxKeyStore1)
	ON_EN_CHANGE(IDC_EditBoxKeyStore2, OnChangeEditBoxKeyStore2)
	ON_EN_CHANGE(IDC_EditBoxKeyStore3, OnChangeEditBoxKeyStore3)
	ON_EN_CHANGE(IDC_EditBoxKeyStore4, OnChangeEditBoxKeyStore4)
	ON_EN_CHANGE(IDC_EditBoxKeyStore5, OnChangeEditBoxKeyStore5)
	ON_EN_CHANGE(IDC_EditBoxKeyStore6, OnChangeEditBoxKeyStore6)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMifareChangeKeyDlg message handlers

BOOL CMifareChangeKeyDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Store the address of the pointer to the 'this'
	pThis = this;

	pcscReader = new PcscReader();
	
	pMifare_ = NULL;

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CString temp = "Program Ready";

	resetFields();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMifareChangeKeyDlg::OnPaint() 
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
HCURSOR CMifareChangeKeyDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


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

void CMifareChangeKeyDlg::enableLoadGroup(bool bEnable)
{
	EditboxLoadKeyStoreNumber.EnableWindow(bEnable);
	TextBoxKeyVal1.EnableWindow(bEnable);
	TextBoxKeyVal2.EnableWindow(bEnable);
	TextBoxKeyVal3.EnableWindow(bEnable);
	TextBoxKeyVal4.EnableWindow(bEnable);
	TextBoxKeyVal5.EnableWindow(bEnable);
	TextBoxKeyVal6.EnableWindow(bEnable);
	ButtonLoadKey.EnableWindow(bEnable);
}

void CMifareChangeKeyDlg::enableAuthenticateGroup(bool bEnable)
{
	EditboxAuthenticateKeyStoreNumber.EnableWindow(bEnable);
	EditboxSectorNumber.EnableWindow(bEnable);
	EditboxAuthenticateKeyStoreNumber.EnableWindow(bEnable);
	RadiobuttonKeyTypeKeyA.EnableWindow(bEnable);
	RadiobuttonKeyTypeKeyB.EnableWindow(bEnable);
	ButtonAuthenticate.EnableWindow(bEnable);
	ButtonAuthenticate.EnableWindow(bEnable);
	RadiobuttonKeyTypeKeyA.EnableWindow(bEnable);
	RadiobuttonKeyTypeKeyB.EnableWindow(bEnable);
}

void CMifareChangeKeyDlg::enableChangeKeyGroup(bool bEnable)
{
	ButtonRead.EnableWindow(bEnable);
	ButtonUpdate.EnableWindow(bEnable);
	EditboxKeyA.EnableWindow(bEnable);
	EditboxKeyB.EnableWindow(bEnable);
	EditboxAccessBits.EnableWindow(bEnable);
}

void CMifareChangeKeyDlg::resetFields()
{

	ButtonInitialize.EnableWindow (true);
	ButtonConnect.EnableWindow(false);
	ButtonLoadKey.EnableWindow(false);
	ButtonAuthenticate.EnableWindow(false);
	ButtonRead.EnableWindow(false);
	ButtonUpdate.EnableWindow(false);
	
	RadiobuttonKeyTypeKeyA.EnableWindow(false);
	RadiobuttonKeyTypeKeyB.EnableWindow(false);
	RadiobuttonKeyTypeKeyA.SetCheck(true);
	RadiobuttonKeyTypeKeyB.SetCheck(false);

	EditboxLoadKeyStoreNumber.EnableWindow(false);
	EditboxSectorNumber.EnableWindow(false);
	EditboxAuthenticateKeyStoreNumber.EnableWindow(false);
	EditboxCurrentSector.EnableWindow(false);
	EditboxSectorTrailerBlock.EnableWindow(false);
	EditboxKeyA.EnableWindow(false);
	EditboxAccessBits.EnableWindow(false);
	EditboxKeyB.EnableWindow(false);
	TextBoxKeyVal1.EnableWindow(false);
	TextBoxKeyVal2.EnableWindow(false);
	TextBoxKeyVal3.EnableWindow(false);
	TextBoxKeyVal4.EnableWindow(false);
	TextBoxKeyVal5.EnableWindow(false);
	TextBoxKeyVal6.EnableWindow(false);
	TextBoxKeyVal1.SetLimitText(2);
    TextBoxKeyVal1.SetWindowText("");
	TextBoxKeyVal2.SetLimitText(2);
	TextBoxKeyVal2.SetWindowText("");
	TextBoxKeyVal3.SetLimitText(2);
	TextBoxKeyVal3.SetWindowText("");
	TextBoxKeyVal4.SetLimitText(2);
	TextBoxKeyVal4.SetWindowText("");
	TextBoxKeyVal5.SetLimitText(2);
	TextBoxKeyVal5.SetWindowText("");
	TextBoxKeyVal6.SetLimitText(2);
	TextBoxKeyVal6.SetWindowText("");
	EditboxLoadKeyStoreNumber.SetLimitText(2);
	EditboxSectorNumber.SetLimitText(2);
	EditboxAuthenticateKeyStoreNumber.SetLimitText(2);
	EditboxCurrentSector.SetLimitText(2);
	EditboxSectorTrailerBlock.SetLimitText(2);
	EditboxKeyA.SetLimitText(12);
	EditboxAccessBits.SetLimitText(8);
	EditboxKeyB.SetLimitText(12);
	EditboxLoadKeyStoreNumber.SetWindowText("");
	EditboxSectorNumber.SetWindowText("");
	EditboxAuthenticateKeyStoreNumber.SetWindowText("");
	EditboxCurrentSector.SetWindowText("");
	EditboxSectorTrailerBlock.SetWindowText("");
	EditboxKeyA.SetWindowText("");
	EditboxAccessBits.SetWindowText("");
	EditboxKeyB.SetWindowText("");

	OnButtonClear();
	addMessageToLog("Program Ready");
	

	UpdateData(false);
}


// Display the message to the log window
void CMifareChangeKeyDlg::addTitleToLog(CString sMessage)
{
	EditboxApduLogsText += "\r\n" + sMessage + "\r\n";
	UpdateData(false);

	EditboxApduLogs.SetSel(0xFFFF, 0xFFFF);
}

void CMifareChangeKeyDlg::addMessageToLog(CString sMessage)
{
	EditboxApduLogsText += sMessage + "\r\n";
	UpdateData(false);

	EditboxApduLogs.SetSel(0xFFFF, 0xFFFF);
}

void CMifareChangeKeyDlg::showErrorMessage(CString sMessage)
{
	::MessageBox(0, sMessage, "Error", MB_OK|MB_ICONSTOP);
}

void CMifareChangeKeyDlg::showWarningMessage(CString sMessage)
{
	::MessageBox(0, sMessage, "Warning", MB_OK|MB_ICONEXCLAMATION);
}

void CMifareChangeKeyDlg::showInformationMessage(CString sMessage)
{
	::MessageBox(0, sMessage, "Information", MB_OK|MB_ICONINFORMATION);
}

void CMifareChangeKeyDlg::displayMessage(CString message)
{
	EditboxApduLogsText += message + "\r\n";
	UpdateData(false);

	EditboxApduLogs.SetSel(0xFFFF, 0xFFFF);
}


void CMifareChangeKeyDlg::OnButtonInitialize() 
{	
	string *pReaderList = new string[10];
	uint8 uReaderCount = 10;
	int readerNameLength = 0;
	int i = 0;

	try
	{
		ComboBoxReaderNames.ResetContent();

		// Establish Context
		pcscReader->initialize();

		// List the detected smart card readers
		pcscReader->listReaders(pReaderList, uReaderCount);


		// Load the retrieved reader name array to the reader list
		for(i = 0; i < uReaderCount; i++)
		{
			ComboBoxReaderNames.AddString(pReaderList[i].data());
		}
		
		ComboBoxReaderNames.SetCurSel(0);

		// Pass the function pointers to serve as the event listeners of the ProprietaryReader class
		pcscReader->setSendCommandFunctor(onSendCommand);
		pcscReader->setRecvCommandFunctor(onReceiveCommand);					

		EditboxApduLogs.Clear();
        addMessageToLog("\r\nInitialize success");
		ButtonConnect.EnableWindow(true);
	}
	catch (PcscException ex)
	{
		addTitleToLog(ex.what());
		showErrorMessage(ex.what());
	}
}
void CMifareChangeKeyDlg::OnButtonConnect() 
{
	CString readerName;
	char message[100];

	uint8 aAtr[100];
	uint32 uAtrLen = sizeof(aAtr);
	
	CardSelector *pCardSelector;
	
	ComboBoxReaderNames.GetWindowText(readerName);

	try
	{
		// Check if a reader name is selected
		if (!strcmp(readerName, ""))
		{
			showErrorMessage("Select a smartcard reader");
			return;
		}

		// Try to connect to the smartcard through the specified reader name
		if (pcscReader->connect((std::string)readerName) != 0)
			return;
				

		sprintf(message, "%s %s", "\r\nSuccessfully connected to ", readerName);
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
			enableLoadGroup(false);
			return;
		}

		if (sCardName == "Mifare Standard 1K")
			pThis->LabelValidSectors.SetWindowText("00 - 15");
		else if (sCardName == "Mifare Standard 4K")
			pThis->LabelValidSectors.SetWindowText("00 - 39");

		sprintf(message, "%s %s", "Chip Type: ", sCardName);
		addMessageToLog(message);
		enableLoadGroup(true);
		
	}
	catch (PcscException ex)
	{
		addTitleToLog(ex.what());
		showErrorMessage(ex.what());
	}
	
}

void CMifareChangeKeyDlg::OnButtonClear() 
{	
	EditboxApduLogsText = "";
	UpdateData(false);

	EditboxApduLogs.SetSel(0xFFFF, 0xFFFF);
}


void CMifareChangeKeyDlg::OnButtonReset() 
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

	ComboBoxReaderNames.ResetContent();
	
	resetFields();
}


void CMifareChangeKeyDlg::OnButtonQuit() 
{	
	
	OnCancel();
}


void CMifareChangeKeyDlg::OnButtonLoadKey() 
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
	//char keyValue[20];
	int i;

    try
    {
		EditboxLoadKeyStoreNumber.GetWindowText( buffer, 20 );
			
		if(!strcmp(buffer, ""))
			inputError = true;			
		if( atoi( buffer ) < 0 )
			inputError = true;
		else if( atoi( buffer ) > 1 )
			inputError = true;	
			
		if (inputError == true)
        {				
			showErrorMessage("Please key-in Key Store Number from 00 to 01.");
            EditboxLoadKeyStoreNumber.SetFocus();
			return;
		}

		for(i = 0; i < strlen(buffer);i++)
		{
			if(!isdigit(buffer[i]) && buffer[i] != NULL)
			{
				showErrorMessage("Please key-in Key Store Number from 00 to 01.");
				EditboxLoadKeyStoreNumber.SetFocus();
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
		enableAuthenticateGroup(true);
				
	}
    catch (CardException cardException)
	{
		char message[100];
		sprintf(message, "%s%s%s%s", "[", helper.byteAsString(cardException.getStatusWord(),2, true), "] ", cardException.getMessage() );
		showErrorMessage(message);
		displayMessage(cardException.getMessage());
	}
	catch (PcscException pcscException)
	{
		showErrorMessage(pcscException.what());
		displayMessage(pcscException.what());
	}
}


void CMifareChangeKeyDlg::OnButtonAuthenticate() 
{
	byte blockNumber = 0x00;	
    MIFARE_KEY_TYPE eKeyType = KEY_TYPE_A;

	//For Non-Volatile  : 0x00-0x1F
    //For Volatile      : 0x20
    byte keyNumber = 0x20;

	char keyStoreNum[20];	
	char sectorNum[20];	
	bool inputError = false;
	int iCharIterator;

    try
    {
		EditboxSectorNumber.GetWindowText( sectorNum , 20 );
		if(!strcmp(sectorNum, ""))
		{
			showErrorMessage("Please key-in numeric value for Sector Number.");
            EditboxSectorNumber.SetFocus();
            return;
		}
		
	//	if ((RadiobuttonKeySourceNonVolatileMemory.GetCheck() == TRUE))
      //  {
			EditboxAuthenticateKeyStoreNumber.GetWindowText( keyStoreNum, 20 );
			
			if(!strcmp(keyStoreNum, ""))
				inputError = true;			
			if( atoi( keyStoreNum ) < 0 )
				inputError = true;
			else if( atoi( keyStoreNum ) > 1 )
				inputError = true;	
			
			if (inputError == true)
            {				
				showErrorMessage("Please key-in Key Store Number from 00 to 01.");
                EditboxAuthenticateKeyStoreNumber.SetFocus();
                return;
			}

			for(iCharIterator = 0; iCharIterator < strlen(keyStoreNum);iCharIterator++)
			{
				if(!isdigit(keyStoreNum[iCharIterator]) && keyStoreNum[iCharIterator] != NULL)
				{
					showErrorMessage("Please key-in Key Store Number from 00 to 01.");
					EditboxAuthenticateKeyStoreNumber.SetFocus();
					return;
				}
			}


			keyNumber = (byte) atoi(keyStoreNum);
		//}

		if ((RadiobuttonKeyTypeKeyB.GetCheck() == TRUE))
			eKeyType = KEY_TYPE_B;
			
		 if (sCardName == "Mifare Standard 4K")
		{
			if ( atoi( sectorNum ) > 39 )
			{			
				showErrorMessage("Invalid Sector Number. Valid Sector Number for Mifare 4k : 0 - 39");				
                EditboxSectorNumber.SetFocus();
                return;
			}

			currentSector = atoi( sectorNum );

			//Mifare 4K is oragnized in 32 sectors with 4 blocks
			//and in 8 sectors with 16 blocks
			if (currentSector <= 31)
			{
				currentSectorTrailer = (byte)((currentSector * 4) + 3);
			}
			else
			{
				//127 is the physical address of the last block(sector trailer) of the 32nd sector                   
				currentSectorTrailer = 127;

				//Succeeding sector has contains 16 blocks
				currentSectorTrailer += (byte)(((currentSector - 32) * 16) + 16);
			}
		}
        else
        {
			if (atoi ( sectorNum ) > 15)
            {
				showErrorMessage("Invalid Sector Number. Valid Sector Number for Mifare 1k : 0 - 15");				
				EditboxSectorNumber.SetFocus();
                return;
			}
			
			currentSector = atoi( sectorNum );
            currentSectorTrailer = (byte)((currentSector * 4) + 3);
		}

		addTitleToLog("Authenticate Key");

		//acr1281UC1->authenticate(currentSectorTrailer, keyType, keyNumber);
		pMifare_->authenticate(currentSectorTrailer, eKeyType, keyNumber);

		char buffer[20];		
		itoa( currentSectorTrailer, buffer, 10 );
		EditboxSectorTrailerBlock.SetWindowText( buffer );
		
		sprintf(buffer, "");				
		itoa( currentSector, buffer, 10 );
		EditboxCurrentSector.SetWindowText( buffer );
		
        addMessageToLog("Authenticate success");
		enableChangeKeyGroup(true);
		
	}
    catch (CardException cardException)
	{
		char message[100];
		//sprintf(message, "[%s] %s", helper.byteAsString(cardException.getStatusWord(),2, true), cardException.getMessage() );
		showErrorMessage(cardException.getMessage());
		addTitleToLog(cardException.getMessage());
	}
	catch (PcscException pcscException)
	{
		addTitleToLog(pcscException.what());
		displayMessage(pcscException.what());
	}
	catch (...)
	{
		addTitleToLog("Authenticate failed");
		showErrorMessage("Authenticate failed");
	}
}

void CMifareChangeKeyDlg::OnButtonRead() 
{	
	byte* sectorTrailer;
	CString tempString;

	try
	{
		addTitleToLog("Read Binary");

		//sectorTrailer = mifareClassic->readBinary(currentSectorTrailer);	
		sectorTrailer = pMifare_->readBinary(currentSectorTrailer);
		
		// Mifare does not allow you to read the actual Key A
		// We will leave this field as blank
		EditboxKeyA.SetWindowText("");		
		
		tempString = helper.byteAsString(sectorTrailer, 6, 4, false);						
		EditboxAccessBits.SetWindowText( tempString );		
		
		tempString = helper.byteAsString(sectorTrailer, 10, 6, false);		
		EditboxKeyB.SetWindowText( tempString );
		
		addMessageToLog("Read success");

		showInformationMessage("NOTE: Mifare does not allow user to read the actual key A. 'Key A' field will be set to empty.");
	}
	catch (CardException cardException)
	{
		char message[100];
		sprintf(message, "%s%s%s%s", "[", helper.byteAsString(cardException.getStatusWord(),2, true), "] ", cardException.getMessage() );
		showErrorMessage(message);
		addTitleToLog(cardException.getMessage());
	}
	catch (PcscException pcscException)
	{
		showErrorMessage(pcscException.what());
		addTitleToLog(pcscException.what());
	}
	catch (...)
	{
		addTitleToLog("Read failed");
		showErrorMessage("Read failed");
	}
}

void CMifareChangeKeyDlg::OnButtonUpdate() 
{	
	byte* keyA;
	byte* accessBits;
	byte* keyB;
	byte newSectorTrailer[16];
	char tempKeyA[20], tempKeyB[20], tempAccessBits[20];
	int i;

	try
	{
		EditboxKeyA.GetWindowText( tempKeyA, 20 );

		if ( (!strcmp(tempKeyA, "")) || (strlen(tempKeyA) != 12) )
		{
			showErrorMessage("Please key-in hex value for Key A.");
            EditboxKeyA.SetFocus();
            return;
		}

		keyA = helper.getBytes(tempKeyA);

		EditboxAccessBits.GetWindowText( tempAccessBits, 20 );

		if ( (!strcmp(tempAccessBits, "")) || (strlen(tempAccessBits) != 8) )
		{
			showErrorMessage("Please key-in hex value for Access Bits.");
            EditboxAccessBits.SetFocus();
            return;
		}		

		accessBits = helper.getBytes(tempAccessBits);

		EditboxKeyB.GetWindowText( tempKeyB, 20 );

		if ( (!strcmp(tempKeyB, "")) || (strlen(tempKeyB) != 12) )
		{
			showErrorMessage("Please key-in hex value for Key B.");
            EditboxKeyA.SetFocus();
            return;
		}

		keyB = helper.getBytes(tempKeyB);

		byte defaultAccessBits[4] = { 0xFF, 0x07, 0x80, 0x69 };
		CString notification = "IMPORTANT: Check Access Bits!\r\n\r\n";
		notification += "Please make sure that the access bits are valid. You may refer to Mifare Classic manual for more  information.\r\n";
		notification += "If the access bits are invalid the sector will PERMANENTLY lock itself.\r\n\r\nContinue?";

		for (i = 0; i < 4; i++)
		{			
			if (accessBits[i] != defaultAccessBits[i])
			{
				if (MessageBox(notification, "Mifare Change Key", MB_YESNO | MB_ICONEXCLAMATION) != IDYES)
					return;
			}
		}

		char buffer[200], buffer2[200];
		itoa(currentSector, buffer, 10);
		
		sprintf(buffer2, "%s%s", "Please take note of the following:\r\n\r\nSector : " , buffer);  		
		sprintf(buffer, "");
		sprintf(buffer, "%s%s%s", buffer2, "\r\nKey A : ", (CString)helper.byteAsString(keyA, 6, true));  		
		sprintf(buffer2, "");
		sprintf(buffer2, "%s%s%s", buffer, "\r\nKey B : ", (CString)helper.byteAsString(keyB, 6, true));  		
		sprintf(buffer, "");
		sprintf(buffer, "%s%s%s", buffer2, "\r\nAccess Bits : ", (CString)helper.byteAsString(accessBits, 4, true));  
		showInformationMessage(buffer);

		for (i = 0; i < 6; i++)
			newSectorTrailer[i] = keyA[i];

		for (i = 0; i < 6; i++)
			newSectorTrailer[i + 6] = accessBits[i];

		for (i = 0; i < 6; i++)
			newSectorTrailer[i + 10] = keyB[i];

		addTitleToLog("Update Binary");

		//mifareClassic->updateBinary(currentSectorTrailer, newSectorTrailer);					
		pMifare_->updateBinary(currentSectorTrailer, newSectorTrailer);

		addMessageToLog("Update success");

		itoa(currentSector, buffer, 10);
		sprintf(buffer2, "%s%s", "\r\nSector      : ", buffer);
		addMessageToLog( buffer2 );

		sprintf(buffer2, "");
		sprintf(buffer2, "%s%s", "New Key A : ", (CString)helper.byteAsString(keyA, 6, true));
		addMessageToLog( buffer2 );

		sprintf(buffer2, "");
		sprintf(buffer2, "%s%s", "New Key B : ", (CString)helper.byteAsString(keyB, 6, true));
		addMessageToLog( buffer2 );

		sprintf(buffer2, "");
		sprintf(buffer2, "%s%s", "Access Bits: ", (CString)helper.byteAsString(accessBits, 4, true));
		addMessageToLog( buffer2 );
	}
	catch (CardException cardException)
	{
		char message[100];
		sprintf(message, "%s%s%s%s", "[", helper.byteAsString(cardException.getStatusWord(),2, true), "] ", cardException.getMessage() );
		showErrorMessage(message);
		addTitleToLog(cardException.getMessage());
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


// Validate input to an editbox control
// to allow hexadecinal values only
void CMifareChangeKeyDlg::validateInput(CEdit *editbox, CString *text)
{
	char buffer[18];

	UpdateData(true);

	editbox->GetWindowText(buffer, sizeof(buffer));

	if (!strstr ("0123456789ABCDEFabcdef", &buffer[strlen(buffer) - 1]))
	{
		buffer[strlen(buffer) - 1] = 0x00;
		*text = buffer;
		UpdateData(false);
		editbox->SetSel(text->GetLength(), text->GetLength());
	}
}


void CMifareChangeKeyDlg::OnChangeEditBoxKeyA() 
{
	validateInput(&EditboxKeyA, &EditboxKeyAString);
	
}


void CMifareChangeKeyDlg::OnChangeEditBoxAccessBits() 
{
	validateInput(&EditboxAccessBits, &EditboxAccessBitsString);	
}


void CMifareChangeKeyDlg::OnChangeEditBoxKeyB() 
{
	validateInput(&EditboxKeyB, &EditboxKeyBString);	
}

void CMifareChangeKeyDlg::OnEditchangeComboBoxPort() 
{
	// TODO: Add your control notification handler code here
	
}

void CMifareChangeKeyDlg::OnChangeEditBoxKeys() 
{
	//	validateInput(&EditboxKeyValueInput, &EditboxKeyValueInputString);	
}

void CMifareChangeKeyDlg::OnEditchangeComboBoxSlot() 
{
	// TODO: Add your control notification handler code here
	
}

void CMifareChangeKeyDlg::OnChangeEditBoxKeyStore1() 
{
	validateInput(&TextBoxKeyVal1, &TextBoxValString1);		
}


void CMifareChangeKeyDlg::OnChangeEditBoxKeyStore2() 
{
	validateInput(&TextBoxKeyVal2, &TextBoxValString2);
}

void CMifareChangeKeyDlg::OnChangeEditBoxKeyStore3() 
{
	validateInput(&TextBoxKeyVal3, &TextBoxValString3);
	
}

void CMifareChangeKeyDlg::OnChangeEditBoxKeyStore4() 
{
	validateInput(&TextBoxKeyVal4, &TextBoxValString4);
	
}

void CMifareChangeKeyDlg::OnChangeEditBoxKeyStore5() 
{
	validateInput(&TextBoxKeyVal5, &TextBoxValString5);
	
}

void CMifareChangeKeyDlg::OnChangeEditBoxKeyStore6() 
{
	validateInput(&TextBoxKeyVal6, &TextBoxValString6);
	
}


