// NtagNDEFSampleCodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include <list>
#include <tchar.h>
#include "Math.h"
#include "winscard.h"
#include "NtagNDEFSampleCode.h"
#include "NtagNDEFSampleCodeDlg.h"

#include "AcsReader.h"
#include "PcscProvider.h"
#include "PcscException.h"
#include "PcscReader.h"
#include "Helper.h"
#include "Ntag203.h"
#include "CardSelector.h"
#include "CardException.h"
#include "ReferenceData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

	// Global variables
	NtagNDEFSampleCodeDlg *_pThis = NULL;
	
	Helper _cHelper;

	// Function prototypes
	void onSendCommand(const byte* pSendData, const int iLen);
	void onReceiveCommand(const byte* pReceiveData, const int iLen);

	//global functions
	void showInformationMessage(CString sMessage);
	void showWarningMessage(CString sMessage);
	void showErrorMessage(CString sMessage);

// Display the send command APDU
void onSendCommand(const byte* pSendData, const int iLen)
{
	char *pStr;

	pStr = _cHelper.byteAsString(pSendData, iLen, true);
	if (pStr == NULL)
	{
		return;
	}

	_pThis->addMessageToLogs((CString)"<< " + (CString) pStr);

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

	_pThis->addMessageToLogs((CString)">> " + (CString) pStr + "\r\n");

	delete [] pStr;
}

/////////////////////////////////////////////////////////////////////////////
// NtagNDEFSampleCodeDlg dialog


NtagNDEFSampleCodeDlg::NtagNDEFSampleCodeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(NtagNDEFSampleCodeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(NtagNDEFSampleCodeDlg)
	m_RichEditApduLogsString = _T("");
	//}}AFX_DATA_INIT

	m_hIcon = AfxGetApp()->LoadIcon(IDI_IconAcs);
}


void NtagNDEFSampleCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NtagNDEFSampleCodeDlg)
	DDX_Control(pDX, IDC_TabMain, m_TabMain);
	DDX_Control(pDX, IDC_RichEditApduLogs, m_RichEditApduLogs);
	DDX_Control(pDX, IDC_EditNdefMessage, m_EditNdefMessage);
	DDX_Control(pDX, IDC_ComboBoxReaderList, m_ComboBoxReaderList);
	DDX_Control(pDX, IDC_ButtonWriteToCard, m_ButtonWriteToCard);
	DDX_Control(pDX, IDC_ButtonReset, m_ButtonReset);
	DDX_Control(pDX, IDC_ButtonReadFromCard, m_ButtonReadFromCard);
	DDX_Control(pDX, IDC_ButtonQuit, m_ButtonQuit);
	DDX_Control(pDX, IDC_ButtonInitialize, m_ButtonInitalize);
	DDX_Control(pDX, IDC_ButtonConnect, m_ButtonConnect);
	DDX_Control(pDX, IDC_ButtonClear, m_ButtonClear);
	DDX_Text(pDX, IDC_RichEditApduLogs, m_RichEditApduLogsString);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NtagNDEFSampleCodeDlg, CDialog)
	//{{AFX_MSG_MAP(NtagNDEFSampleCodeDlg)
	ON_BN_CLICKED(IDC_ButtonInitialize, OnButtonInitialize)
	ON_BN_CLICKED(IDC_ButtonConnect, OnButtonConnect)
	ON_BN_CLICKED(IDC_ButtonWriteToCard, OnButtonWriteToCard)
	ON_BN_CLICKED(IDC_ButtonReadFromCard, OnButtonReadFromCard)
	ON_BN_CLICKED(IDC_ButtonClear, OnButtonClear)
	ON_BN_CLICKED(IDC_ButtonReset, OnButtonReset)
	ON_BN_CLICKED(IDC_ButtonQuit, OnButtonQuit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NtagNDEFSampleCodeDlg message handlers

BOOL NtagNDEFSampleCodeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	_pThis = this;

	// Instantiate an object of the PcscReader-derived class
	pPcscReader_ = new PcscReader();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	TCITEM tcItemSmartPoster;
	TCITEM tcItemUri;
	TCITEM tcItemText;
	TCITEM tcItemVCard;

	tcItemSmartPoster.mask = TCIF_TEXT;
	tcItemSmartPoster.pszText = "Smart Poster";
	tcItemUri.mask = TCIF_TEXT;
	tcItemUri.pszText = "URI";
	tcItemText.mask = TCIF_TEXT;
	tcItemText.pszText = "Text";
	tcItemVCard.mask = TCIF_TEXT;
	tcItemVCard.pszText = "vCard";

	m_TabMain.initDialogs();
	m_TabMain.InsertItem(0, &tcItemSmartPoster);
	m_TabMain.InsertItem(1, &tcItemUri);
	m_TabMain.InsertItem(2, &tcItemText);
	m_TabMain.InsertItem(3, &tcItemVCard);
	
	m_TabMain.SetCurFocus(0);
	m_TabMain.activateTabDialogs();
	resetFields();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// Display the message to the log window
void NtagNDEFSampleCodeDlg::addMessageToLogs(CString sMessage)
{
	m_RichEditApduLogsString += sMessage + "\r\n";
	UpdateData(false);

	m_RichEditApduLogs.SetSel(0xFFFF, 0xFFFF);
	m_RichEditApduLogs.PostMessage(WM_VSCROLL, SB_BOTTOM, 0);
}

void NtagNDEFSampleCodeDlg::OnButtonInitialize() 
{
	string *pReaderList = new string[10];
	uint8 uReaderCount = 10;
	int iReaderNameLength = 0;
	int i = 0;

	try
	{
		m_ComboBoxReaderList.ResetContent();

		// Establish Context
		pPcscReader_->initialize();

		// List the detected smart card readers
		pPcscReader_->listReaders(pReaderList, uReaderCount);


		// Load the retrieved reader name array to the reader list
		for(i = 0; i < uReaderCount; i++)
		{
			m_ComboBoxReaderList.AddString(pReaderList[i].data());
		}
		
		m_ComboBoxReaderList.SetCurSel(0);

		// Pass the function pointers to serve as the event listeners of the ProprietaryReader class
		pPcscReader_->setSendCommandFunctor(onSendCommand);
		pPcscReader_->setRecvCommandFunctor(onReceiveCommand);					

        addMessageToLogs("Initialize Success");
        addMessageToLogs("");
		m_ButtonConnect.EnableWindow(true);
	}
	catch (PcscException ex)
	{
		MessageBox(ex.what(), "Error");
		addMessageToLogs(ex.what());
	}
	catch (...)
	{
		showErrorMessage("Initialize Failed");
		addMessageToLogs("Initialize Failed");
	}
	
}

void NtagNDEFSampleCodeDlg::OnButtonConnect() 
{
	CString sReaderName;
	char aMessage[100];

	uint8 aAtr[100];
	uint32 uAtrLen = sizeof(aAtr);
	CString sCardName;
	CardSelector *pCardSelector;
	
	m_ComboBoxReaderList.GetWindowText(sReaderName);

	try
	{
		// Check if a reader name is selected
		if (!strcmp(sReaderName, ""))
		{
			showErrorMessage("Select a smartcard reader.");
			return;
		}

		// Try to connect to the smartcard through the specified reader name
		if (pPcscReader_->connect((std::string)sReaderName) != 0)
			return;
				

		sprintf(aMessage, "%s %s", "Successfully connected to ", sReaderName);
		addMessageToLogs(aMessage);

		pNtag203_ = new Ntag203(pPcscReader_);
		if (pNtag203_ == NULL)
		{
			showErrorMessage("Insufficient resources for new NTAG203");
			return;
		}

		pPcscReader_->getStatus();
		pPcscReader_->getAtr(aAtr, uAtrLen);

		pCardSelector = new CardSelector(pPcscReader_);

		sCardName = pCardSelector->readCardType(aAtr, uAtrLen);

		if (sCardName != "NTAG203")
		{
			showErrorMessage("Card not supported.\r\nPlease present NTAG203 card.");
			m_TabMain.enableControls(false);
			m_TabMain.EnableWindow(false);
			m_ButtonReadFromCard.EnableWindow(false);
			m_ButtonWriteToCard.EnableWindow(false);
			return;
		}

		sprintf(aMessage, "%s %s", "Chip Type: ", sCardName);
		addMessageToLogs(aMessage);

		m_TabMain.enableControls(true);
		m_TabMain.EnableWindow(true);
		m_ButtonReadFromCard.EnableWindow(true);
		m_ButtonWriteToCard.EnableWindow(true);
	}
	catch (PcscException ex)
	{
		showErrorMessage(ex.what());
		addMessageToLogs(ex.what());
	}
	catch (...)
	{
		showErrorMessage("Connect Failed");
		addMessageToLogs("Connect Failed");
	}
	
}

void NtagNDEFSampleCodeDlg::OnButtonWriteToCard() 
{
	writeToCard();	
}

void NtagNDEFSampleCodeDlg::OnButtonReadFromCard() 
{
	readFromCard();
}

void NtagNDEFSampleCodeDlg::writeToCard()
{
	byte* pDtaToWrite;
	byte *pBuffer, *pArrLength, *pTempByte;
	byte uBlockNumber = 0x04;
	bool bInputError = false;
	CString sTempStr;
	CString sBuffer;
	int iLen, iDataLength, iIndex = 0, iLenToCopy;
	int iNumberOfWrites;

	const int MAXIMUM_INPUT_LENGTH = 141;
	const int DATA_LENGTH = 255;
	const int BLOCK_DATA_LENGTH = 4;
	const double BLOCK_LENGTH = 4.00;


	try
	{
		
		if(m_EditNdefMessage.GetWindowTextLength() == 0)
		{
			showErrorMessage("There is no NDEF message to write. Please encode NDEF message first.");
			return;
		}

		m_EditNdefMessage.GetWindowText(sBuffer);

		pDtaToWrite = _cHelper.getBytes(sBuffer);

		iLen = (strlen(sBuffer))/2;

		if (iLen > MAXIMUM_INPUT_LENGTH)
		{
			showErrorMessage("NDEF Message too long. NDEF length should be at most 141 bytes.");
			return;
		}

		if (iLen < 1)
		{
			showErrorMessage("There is no NDEF message to write. Please encode NDEF message first.");
			return;
		}


		//Form the data to be written to the card
		if(iLen < DATA_LENGTH)
		{
			//If data is less than or equal to 254 (0xFE) use one byte format
            //Refer to Type 2 Tag Operation document
            //http://www.nxp.com/
			pBuffer = new byte[iLen + 3];
			pBuffer[0] = (byte)NDEF_MESSAGE_TLV;
			pBuffer[1] = (byte)iLen;

			iIndex = 2;
		}
		else
		{
			//If data is greater than 254 (0xFE) use 3 bytes format
			iDataLength = iLen;

			//In 3 bytes format the last 2 bytes is for 00FE - FFFF                    
            iDataLength -= 254;

			//Indicates that two more bytes folow
			pArrLength = new byte[3];
			pArrLength[0] = 0xFF;
            pArrLength[1] = (byte)((iDataLength & 0xFF00) >> 8);
            pArrLength[2] = (byte)(iDataLength & 0xFF);

            //NDEF Message TAG
			pBuffer = new byte[iLen + 5];
			pBuffer[0] = (byte)NDEF_MESSAGE_TLV;

			//length
			pBuffer[1] = pArrLength[0];
			pBuffer[2] = pArrLength[1];
			pBuffer[3] = pArrLength[2];

			iIndex = 4;
		}

		for (int i = 0; i < iLen; i++)
		{
			pBuffer[i+iIndex] = pDtaToWrite[i];
		}

		//Append Terminator TLV
		pBuffer[iLen + iIndex] = (byte)TERMINATOR_TLV;

		//newpBuffer length
		iLen = iLen + iIndex + 1;

		//Write the data to NTAG
        //Start from Block 4
		addMessageToLogs("\nWrite NDEF Message To Card");

		iNumberOfWrites = ceil((double)iLen/ BLOCK_LENGTH);

		 for (int iIndx = 0; iIndx < iNumberOfWrites; iIndx++, uBlockNumber += (BLOCK_DATA_LENGTH/4))
		 {

			 pTempByte = new byte[BLOCK_DATA_LENGTH];
			 iLenToCopy = BLOCK_DATA_LENGTH;

			 for (int i = 0; i < iLenToCopy; i++)
			 {

			 	pTempByte[i] = pBuffer[i+(iIndx * BLOCK_DATA_LENGTH)];

				if((i+(iIndx * BLOCK_DATA_LENGTH)) >= iLen)
					pTempByte[i] = 0x00;
			 }

			 sTempStr = "";					
			 sTempStr.Format("%d", uBlockNumber);
             addMessageToLogs("Write in block " + sTempStr);

			 pNtag203_->updateBinary(uBlockNumber, pTempByte, iLenToCopy);

		 }
	}
	catch (PcscException ex)
	{
		showErrorMessage(ex.what());
		addMessageToLogs("");
		addMessageToLogs(ex.what());
	}
	catch (...)
	{
		showErrorMessage("Write Failed");
		addMessageToLogs("");
		addMessageToLogs("Write Failed");
	}
}

void NtagNDEFSampleCodeDlg::readFromCard()
{
	byte *pBuffer;
	byte* pTempArray;
	vector<byte> uTempArray, uTempVector;
	byte uBlockNumber = 0x04;
	bool bInputError = false;

	CString sBuffer;
	CString sTempStr;

	int iDataLength, iDataCount, iIndex = 0;
	int iNumberOfReads, iEndIndex = 0;
	int iCurrIdx = 0;
	int iEndIdx = 0;
	int iVCardIdx = 0;
	int iTemporaryLen, iTemporaryPayLoadLen;

	NdefMessage* pNdefMessage;
	NdefMessage* pTemporaryNdefMessage;
	NdefRecord* pTemporaryNdefRecord;
	NdefRecordType* pNdefRecordType;

	list<NdefRecord> aNdefRecords;
	CString sPayLoadTypeName, sTempString;
	CString aVCard[3];
	CString sSeparator  = ";";
	CString sDisplayTextTemp = "";

	const int BLOCK_DATA_LENGTH = 16;
	const double BLOCK_LENGTH = 16.00;
	const int THREE_BYTE_FORMAT_ADDTL = 5;
    const int NON_THREE_BYTE_FORMAT_ADDTL = 3;

	try
	{
		m_EditNdefMessage.SetWindowText("");

		iDataCount = 0;

		sTempStr = "";					
		sTempStr.Format("%d", uBlockNumber);
        addMessageToLogs("\r\nRead block " + sTempStr);

		pTempArray = pNtag203_->readBinary(uBlockNumber, BLOCK_DATA_LENGTH);
		iDataLength = pTempArray[1];

		if (iDataLength == 0xFF)
		{
			iNumberOfReads = (ceil)(((double)iDataLength + THREE_BYTE_FORMAT_ADDTL)/BLOCK_LENGTH);
		}
		else
		{
			iNumberOfReads = (ceil)(((double)iDataLength + NON_THREE_BYTE_FORMAT_ADDTL)/BLOCK_LENGTH);
		}

		pBuffer = new byte[iNumberOfReads * BLOCK_DATA_LENGTH];

		for (int i = 0; i < BLOCK_DATA_LENGTH; i++)
		{
			pBuffer[i] = pTempArray[i];
			iDataCount++;
		}

		uBlockNumber += 4;

		//Read tag
		for (int indx = 1; indx <= iNumberOfReads - 1; indx++, uBlockNumber += 4)
		{
			pTempArray = new byte[BLOCK_DATA_LENGTH];

			sTempStr = "";					
			sTempStr.Format("%d", uBlockNumber);
            addMessageToLogs("Read block " + sTempStr);

			pTempArray = pNtag203_->readBinary(uBlockNumber, BLOCK_DATA_LENGTH);

			for (int i = 0; i < BLOCK_DATA_LENGTH; i++)
			{
				pBuffer[i+(indx*BLOCK_DATA_LENGTH)] = pTempArray[i];
				iDataCount++;
			}

		}

		//Check if NDEF Message TAG is present
		if(pBuffer[0] != NDEF_MESSAGE_TLV)
		{
			showErrorMessage("NDEF Message TAG is not found");
			return;
		}

		//Find the Terminator TLV
        for (iEndIndex = 0; iEndIndex < iDataCount; iEndIndex++)
        {
			if(pBuffer[iEndIndex] == TERMINATOR_TLV)
				break;

			if ((iEndIndex + 1) >= iDataCount)
			{
				showErrorMessage("Invalid Card.\r\nTerminator Tag is not found");
				return;
			}
		}

		//Remove NDEF Message Tag
        //Check if it uses one byte or 3 byte format for the length
        if (pBuffer[1] == 0xFF)
		{
			//Uses 3 bytes format
			uTempArray.clear();

			for (int i = 0; i < (iEndIndex-(THREE_BYTE_FORMAT_ADDTL - 1)); i++)
			{
				uTempArray.push_back(pBuffer[i+(THREE_BYTE_FORMAT_ADDTL)]);
			}
		}
		else
		{
			uTempArray.clear();

			for (int i = 0; i < (iEndIndex-(NON_THREE_BYTE_FORMAT_ADDTL - 1)); i++)
			{
				uTempArray.push_back(pBuffer[i+(NON_THREE_BYTE_FORMAT_ADDTL - 1)]);
			}
		}

		m_EditNdefMessage.SetWindowText(_cHelper.vectorToString(uTempArray));

		pNdefMessage = new NdefMessage();
		pNdefMessage = cNdef_.getNestedNdefMessage(0, uTempArray);

		if(pNdefMessage->getNumberOfRecords() < 0)
		{
			showInformationMessage("No record to display");
            return;
		}

		aNdefRecords = pNdefMessage->getRecords();

		sPayLoadTypeName = aNdefRecords.begin()->getRecordType()->getTypeName() ;

		if(sPayLoadTypeName == "Sp")
		{
			pTemporaryNdefRecord = &(*(aNdefRecords.begin()));
			pTemporaryNdefMessage = cNdef_.getNestedNdefMessage(pTemporaryNdefRecord);

			if(pTemporaryNdefMessage->getNumberOfRecords() < 1)
			{
				showInformationMessage("Invalid SmartPoster NDEF Message");
				return;
			}

			//Get URI Record (Mandatory)
			pNdefRecordType = new NdefRecordType(NFC_FORM_WELL_KNOWN_TYPE, "U");
            *pTemporaryNdefRecord = pTemporaryNdefMessage->getRecord(*pNdefRecordType);

            if (pTemporaryNdefRecord == NULL)
            {
				showErrorMessage("Mandatory URI Record is not found");
                return;
            }

			
			((CComboBox*)(m_TabMain.tabSmartPoster_ComboBoxSmartPosterPrefixUri))->SetCurSel((int)(pTemporaryNdefRecord->getPayload()[0]));
			uTempVector.clear();

			for (int i = 0; i < (pTemporaryNdefRecord->getPayload().size() - 1); i++)
			{
				uTempVector.push_back(pTemporaryNdefRecord->getPayload()[i+1]);
			}

			m_TabMain.tabSmartPoster_EditBoxSmartPosterUri->SetWindowText(_cHelper.vectorToAscii(uTempVector));

			//Get Action Record
			((CComboBox*)(m_TabMain.tabSmartPoster_ComboBoxSmartPosterAction))->SetCurSel(0);
			pNdefRecordType = new NdefRecordType(NFC_FORM_WELL_KNOWN_TYPE, "act");
            *pTemporaryNdefRecord = pTemporaryNdefMessage->getRecord(*pNdefRecordType);

			if (pTemporaryNdefRecord != NULL)
			{
				if(pTemporaryNdefRecord->getPayload().size() == 0)
					((CComboBox*)(m_TabMain.tabSmartPoster_ComboBoxSmartPosterAction))->SetCurSel(0);
				else if((ACTION_RECORD)(pTemporaryNdefRecord->getPayload()[0]) == DO_THE_ACTION)
					((CComboBox*)(m_TabMain.tabSmartPoster_ComboBoxSmartPosterAction))->SetCurSel(1);
				else if((ACTION_RECORD)(pTemporaryNdefRecord->getPayload()[0]) == SAVE_FOR_LATER)
					((CComboBox*)(m_TabMain.tabSmartPoster_ComboBoxSmartPosterAction))->SetCurSel(2);
				else if((ACTION_RECORD)(pTemporaryNdefRecord->getPayload()[0]) == OPEN_FOR_EDITING)
					((CComboBox*)(m_TabMain.tabSmartPoster_ComboBoxSmartPosterAction))->SetCurSel(3);
				else
					((CComboBox*)(m_TabMain.tabSmartPoster_ComboBoxSmartPosterAction))->SetCurSel(0);

			}
			else
			{
				((CComboBox*)(m_TabMain.tabSmartPoster_ComboBoxSmartPosterAction))->SetCurSel(0);
			}

			//Get Text Record
			pNdefRecordType = new NdefRecordType(NFC_FORM_WELL_KNOWN_TYPE, "T");
            *pTemporaryNdefRecord = pTemporaryNdefMessage->getRecord(*pNdefRecordType);
			if (pTemporaryNdefRecord != NULL)
			{
				iTemporaryLen = pTemporaryNdefRecord->getPayload()[0] & 0x1F;
				uTempVector.clear();

				for (int i = 0; i < iTemporaryLen; i++)
				{
					uTempVector.push_back(pTemporaryNdefRecord->getPayload()[i+1]);
				}

				sTempString = _cHelper.vectorToAscii(uTempVector);

				if(sTempString == "en")
					((CComboBox*)(m_TabMain.tabSmartPoster_ComboBoxSmartPosterTitleLanguage))->SetCurSel(0);
				else if(sTempString == "zn-CN")
					((CComboBox*)(m_TabMain.tabSmartPoster_ComboBoxSmartPosterTitleLanguage))->SetCurSel(1);
				else if(sTempString == "de")
					((CComboBox*)(m_TabMain.tabSmartPoster_ComboBoxSmartPosterTitleLanguage))->SetCurSel(2);
				else
					((CComboBox*)(m_TabMain.tabSmartPoster_ComboBoxSmartPosterTitleLanguage))->SetCurSel(0);
								
				iTemporaryPayLoadLen = pTemporaryNdefRecord->getPayload().size() - (iTemporaryLen + 1);
								
				uTempVector.clear();

				for (int i = 0; i < iTemporaryPayLoadLen; i++)
				{
					uTempVector.push_back(pTemporaryNdefRecord->getPayload()[i+1+iTemporaryLen]);
				}

				m_TabMain.tabSmartPoster_EditBoxSmartPosterTitle->SetWindowText(_cHelper.vectorToAscii(uTempVector));
			}

			m_TabMain.SetCurFocus(0);
			showInformationMessage("Smart Poster Message Found");
		}
		else if(sPayLoadTypeName == "T")
		{
			//Get Text Record
			pTemporaryNdefRecord = &(*(aNdefRecords.begin()));
			
			if((pTemporaryNdefRecord->getPayload()[0] & 0x80) != 0x00)
			{
				((CButton*)(m_TabMain.tabText_RadioButtonTextUtf8))->SetCheck(0);
				((CButton*)(m_TabMain.tabText_RadioButtonTextUtf16))->SetCheck(1);
			}
			else
			{
				((CButton*)(m_TabMain.tabText_RadioButtonTextUtf8))->SetCheck(1);
				((CButton*)(m_TabMain.tabText_RadioButtonTextUtf16))->SetCheck(0);
			}

			if (pTemporaryNdefRecord != NULL)
			{
				iTemporaryLen = pTemporaryNdefRecord->getPayload()[0] & 0x1F;
				uTempVector.clear();

				for (int i = 0; i < iTemporaryLen; i++)
				{
					uTempVector.push_back(pTemporaryNdefRecord->getPayload()[i+1]);
				}

				sTempString = _cHelper.vectorToAscii(uTempVector);

				if(sTempString == "en")
					((CComboBox*)(m_TabMain.tabText_ComboBoxTextLanguage))->SetCurSel(0);
				else if(sTempString == "zn-CN")
					((CComboBox*)(m_TabMain.tabText_ComboBoxTextLanguage))->SetCurSel(1);
				else
					((CComboBox*)(m_TabMain.tabText_ComboBoxTextLanguage))->SetCurSel(2);
								
				iTemporaryPayLoadLen = pTemporaryNdefRecord->getPayload().size() - (iTemporaryLen + 1);
				
				uTempVector.clear();

				for (int i = 0; i < iTemporaryPayLoadLen; i++)
				{
					uTempVector.push_back(pTemporaryNdefRecord->getPayload()[i+1+iTemporaryLen]);
				}

				m_TabMain.tabText_EditBoxTextMessage->SetWindowText(_cHelper.vectorToAscii(uTempVector));
			}

			m_TabMain.SetCurFocus(2);
			showInformationMessage("Text Message Found");
		}
		else if(sPayLoadTypeName == "U")
		{
			pTemporaryNdefRecord = &(*(aNdefRecords.begin()));

			if (pTemporaryNdefRecord == NULL)
            {
				showErrorMessage("Mandatory URI Record is not found");
                return;
            }
			
			((CComboBox*)(m_TabMain.tabUrl_ComboBoxUrlUri))->SetCurSel((int)(pTemporaryNdefRecord->getPayload()[0]));
			uTempVector.clear();

			for (int i = 0; i < (pTemporaryNdefRecord->getPayload().size() - 1); i++)
			{
				uTempVector.push_back(pTemporaryNdefRecord->getPayload()[i+1]);
			}

			m_TabMain.tabUrl_EditBoxUrlUri->SetWindowText(_cHelper.vectorToAscii(uTempVector));
			m_TabMain.SetCurFocus(1);
			showInformationMessage("URI Message Found");

		}
		else if(sPayLoadTypeName == "text/x-vCard")
		{
			iCurrIdx		 = 0;
			iEndIdx			 = 0;
			iVCardIdx = 0;
			sSeparator       = ";";
			sDisplayTextTemp = "";

			pTemporaryNdefRecord = &(*(aNdefRecords.begin()));
			sDisplayTextTemp = _cHelper.vectorToAscii(pTemporaryNdefRecord->getPayload());
			
			iCurrIdx  = sDisplayTextTemp.Find("\r\nN:", 0);
			iEndIdx   = sDisplayTextTemp.Find("END:VCARD", 0);
			for(iCurrIdx += 4; iCurrIdx < iEndIdx-2; iCurrIdx++)
			{
				if(sDisplayTextTemp[iCurrIdx] == sSeparator)
				{
					aVCard[iVCardIdx] = sTempString;
					sTempString = "";
					iVCardIdx++;
				}
				else
				{
					sTempString += sDisplayTextTemp[iCurrIdx];
				}
			}

			aVCard[iVCardIdx] = sTempString;

			m_TabMain.tabVCard_EditBoxSur->SetWindowText(aVCard[0]);
			m_TabMain.tabVCard_EditBoxGiven->SetWindowText(aVCard[1]);
			m_TabMain.tabVCard_EditBoxMiddle->SetWindowText(aVCard[2]);
			m_TabMain.SetCurFocus(3);
			showInformationMessage("vCard Message Found");
		}
		else
		{
			showInformationMessage("Unsupported payload type");
		}

	}
	catch (PcscException ex)
	{
		showErrorMessage(ex.what());
		addMessageToLogs(ex.what());
	}
	catch (...)
	{
		showErrorMessage("Read Failed");
		addMessageToLogs("Read Failed");
	}	
}



void NtagNDEFSampleCodeDlg::OnButtonClear() 
{
	m_RichEditApduLogsString = "";
	m_RichEditApduLogs.SetWindowText( "" );	
}

void NtagNDEFSampleCodeDlg::OnButtonReset() 
{
	m_TabMain.enableControls(false);
	m_ButtonWriteToCard.EnableWindow(false);
    m_ButtonReadFromCard.EnableWindow(false);
	m_ButtonConnect.EnableWindow(false);
	m_ComboBoxReaderList.ResetContent();
	resetFields();
}

void NtagNDEFSampleCodeDlg::OnButtonQuit() 
{
	this->EndDialog(0);	
}

void NtagNDEFSampleCodeDlg::resetFields() 
{
	m_TabMain.enableControls(false);
	m_TabMain.SetCurFocus(0);

	m_EditNdefMessage.SetWindowText("");
	m_RichEditApduLogsString = "";
	addMessageToLogs("Program Ready");
	addMessageToLogs("");

	m_TabMain.tabSmartPoster_EditBoxSmartPosterTitle->SetWindowText("Advanced Card Systems Ltd.");
	m_TabMain.tabSmartPoster_EditBoxSmartPosterUri->SetWindowText("http://www.acs.com.hk");
	m_TabMain.tabUrl_EditBoxUrlUri->SetWindowText("http://www.acs.com.hk");
	m_TabMain.tabText_EditBoxTextMessage->SetWindowText("Hello World!");
	m_TabMain.tabVCard_EditBoxGiven->SetWindowText("Given Name");
	m_TabMain.tabVCard_EditBoxMiddle->SetWindowText("Middle Name");
	m_TabMain.tabVCard_EditBoxSur->SetWindowText("Surname");

	m_TabMain.EnableWindow(false);

	m_TabMain.resetFields();

}

void showInformationMessage(CString sMessage)
{
	MessageBox(0, sMessage, "Information", MB_OK|MB_ICONINFORMATION);
}

void showWarningMessage(CString sMessage)
{
	MessageBox(0, sMessage, "Warning", MB_OK|MB_ICONEXCLAMATION);
}

void showErrorMessage(CString sMessage)
{
	MessageBox(0, sMessage, "Error", MB_OK|MB_ICONSTOP);
}
