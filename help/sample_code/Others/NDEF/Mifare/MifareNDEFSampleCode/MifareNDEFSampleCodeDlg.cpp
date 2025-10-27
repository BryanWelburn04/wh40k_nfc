// MifareNDEFSampleCodeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MifareNDEFSampleCode.h"
#include "MifareNDEFSampleCodeDlg.h"
#include "NdefRecord.h"
#include "WINSCARD.h"
#include "AcsReader.h"
#include "PcscProvider.h"
#include "PcscException.h"
#include "PcscReader.h"
#include "Helper.h"
#include "MifareClassic.h"
#include "CardSelector.h"
#include "CardException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
////////////////////////////////////////////////////////////////////

//Global Variables
CMifareNDEFSampleCodeDlg *_pThis = NULL;
Helper _cHelper;

const BYTE MIFARE_TRANSPORT_KEY[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
const BYTE MIFARE_TRANSPORT_ACCESS_BITS[4] = { 0xFF, 0x07, 0x80, 0x69 };

const BYTE NDEF_PUBLIC_KEY_A_MAD_SECTOR[6] = { 0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5 };
const BYTE NDEF_PUBLIC_KEY_A_NFC_FORUM_SECTOR[6] = { 0xD3, 0xF7, 0xD3, 0xF7, 0xD3, 0xF7 };

const BYTE NDEF_SECTOR_TRAILER_MAD_SECTOR[4] = { 0x78, 0x77, 0x88, 0xC1 };
const BYTE NDEF_SECTOR_TRAILER_NFC_FORUM_SECTOR[4] = { 0x7F, 0x07, 0x88, 0x40 };

// Function prototypes
void onSendCommand(const byte* pSendData, const int iLen);
void onReceiveCommand(const byte* pReceiveData, const int iLen);

// Display the send command APDU
void onSendCommand(const byte* pSendData, const int iLen)
{
	char *pStr;
	
	pStr = _cHelper.byteAsString(pSendData, iLen, true);
	if (pStr == NULL)
	{
		return;
	}
	
	_pThis->addTitleToLog((CString)"\r\n<< " + (CString) pStr);
	
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
	
	_pThis->addTitleToLog((CString)">> " + (CString) pStr);
	
	delete [] pStr;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	
	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_AboutBox };
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
// CMifareNDEFSampleCodeDlg dialog

//Global Variables
CString _sCardName;

CMifareNDEFSampleCodeDlg::CMifareNDEFSampleCodeDlg(CWnd* pParent /*=NULL*/)
: CDialog(CMifareNDEFSampleCodeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMifareNDEFSampleCodeDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_IconAcs);
}

void CMifareNDEFSampleCodeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMifareNDEFSampleCodeDlg)
	DDX_Control(pDX, IDC_MY_TAB, m_TabControlMain);
	DDX_Control(pDX, IDC_EDIT_NFCKEYB, m_EditBoxNfcKeyB);
	DDX_Control(pDX, IDC_EDIT_NFCKEYA, m_EditBoxNfcKeyA);
	DDX_Control(pDX, IDC_EDIT_NFCACCESSBITS, m_EditBoxNfcAccessBits);
	DDX_Control(pDX, IDC_EDIT_NDEFMESSAGE, m_EditBoxNdefMessage);
	DDX_Control(pDX, IDC_EDIT_MADKEYB, m_EditBoxMadKeyB);
	DDX_Control(pDX, IDC_EDIT_MADKEYA, m_EditBoxMadKeyA);
	DDX_Control(pDX, IDC_EDIT_MADACCESSBITS, m_EditBoxMadAccessBits);
	DDX_Control(pDX, IDC_EDIT_APDULOGS, m_EditBoxApduLogs);
	DDX_Control(pDX, IDC_COMBO_READERS, m_ComboBoxReader);
	DDX_Control(pDX, IDC_BUTTON_WRITETOCARD, m_ButtonWriteToCard);
	DDX_Control(pDX, IDC_BUTTON_RESETCARD, m_ButtonResetCard);
	DDX_Control(pDX, IDC_BUTTON_RESET, m_ButtonReset);
	DDX_Control(pDX, IDC_BUTTON_READFROMCARD, m_ButtonReadFromCard);
	DDX_Control(pDX, IDC_BUTTON_QUIT, m_ButtonQuit);
	DDX_Control(pDX, IDC_BUTTON_INITIALIZE, m_ButtonInitialize);
	DDX_Control(pDX, IDC_BUTTON_CONNECT, m_ButtonConnect);
	DDX_Control(pDX, IDC_BUTTON_CLEARLOGS, m_ButtonClearLogs);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMifareNDEFSampleCodeDlg, CDialog)
//{{AFX_MSG_MAP(CMifareNDEFSampleCodeDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDC_BUTTON_INITIALIZE, OnButtonInitialize)
ON_BN_CLICKED(IDC_BUTTON_CONNECT, OnButtonConnect)
ON_BN_CLICKED(IDC_BUTTON_WRITETOCARD, OnButtonWritetocard)
ON_BN_CLICKED(IDC_BUTTON_READFROMCARD, OnButtonReadfromcard)
ON_BN_CLICKED(IDC_BUTTON_RESETCARD, OnButtonResetcard)
ON_BN_CLICKED(IDC_BUTTON_CLEARLOGS, OnButtonClearlogs)
ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
ON_BN_CLICKED(IDC_BUTTON_QUIT, OnButtonQuit)
//}}AFX_MSG_MAP
ON_EN_CHANGE(IDC_EDIT_MADKEYB, &CMifareNDEFSampleCodeDlg::OnEnChangeEditMadkeyb)
ON_EN_CHANGE(IDC_EDIT_NFCKEYB, &CMifareNDEFSampleCodeDlg::OnEnChangeEditNfckeyb)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMifareNDEFSampleCodeDlg message handlers

BOOL CMifareNDEFSampleCodeDlg::OnInitDialog()
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
	pMifare_ = NULL;
	
	// TODO: Add extra initialization here
	TCITEM tabControlItemSmartPoster;
	TCITEM tabControlItemUri;
	TCITEM tabControlItemText;
	TCITEM tabControlItemVCard;
	
	tabControlItemSmartPoster.mask = TCIF_TEXT;
	tabControlItemSmartPoster.pszText = "Smart Poster";
	tabControlItemUri.mask = TCIF_TEXT;
	tabControlItemUri.pszText = "URI";
	tabControlItemText.mask = TCIF_TEXT;
	tabControlItemText.pszText = "Text";
	tabControlItemVCard.mask = TCIF_TEXT;
	tabControlItemVCard.pszText = "vCard";
	
	m_TabControlMain.initDialogs();
	m_TabControlMain.InsertItem(0, &tabControlItemSmartPoster);
	m_TabControlMain.InsertItem(1, &tabControlItemUri);
	m_TabControlMain.InsertItem(2, &tabControlItemText);
	m_TabControlMain.InsertItem(3, &tabControlItemVCard);
	
	m_EditBoxMadKeyB.SetLimitText(KEY_B_WITH_SPACE);
	m_EditBoxNfcKeyB.SetLimitText(KEY_B_WITH_SPACE);
	
	resetWindow(false);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMifareNDEFSampleCodeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMifareNDEFSampleCodeDlg::OnPaint() 
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
HCURSOR CMifareNDEFSampleCodeDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


/////////////////////////////////////////////////////////////////////////////
// Button Events
/////////////////////////////////////////////////////////////////////////////

void CMifareNDEFSampleCodeDlg::OnButtonInitialize() 
{
	string *pReaderList = new string[10];
	uint8 uReaderCount = 10;
	char aMessage[100];
	
	try
	{
		m_ComboBoxReader.ResetContent();
		
		// Establish Context
		pPcscReader_->initialize();
		
		// List the detected smart card readers
		pPcscReader_->listReaders(pReaderList, uReaderCount);
		
		
		// Load the retrieved reader name array to the reader list
		for(int i = 0; i < uReaderCount; i++)
		{
			m_ComboBoxReader.AddString(pReaderList[i].data());
		}
		
		m_ComboBoxReader.SetCurSel(0);
		
		// Pass the function pointers to serve as the event listeners of the ProprietaryReader class
		pPcscReader_->setSendCommandFunctor(onSendCommand);
		pPcscReader_->setRecvCommandFunctor(onReceiveCommand);					
		
		m_EditBoxApduLogs.Clear();
		addTitleToLog("");
        addTitleToLog("Initialize Success");
		m_ButtonConnect.EnableWindow(true);
	}
	catch (CardException cardException)
	{		
		sprintf(aMessage, "%s%s%s%s", "[", _cHelper.byteAsString(cardException.getStatusWord(),2, true), "] ", cardException.getMessage() );
		_cHelper.showErrorMessage(aMessage);
		addErrMsgToLog(cardException.getMessage());
	}
	catch (PcscException pcscException)
	{
		_cHelper.showErrorMessage(pcscException.what());
		addErrMsgToLog(pcscException.what());
	}
	
}

void CMifareNDEFSampleCodeDlg::OnButtonConnect() 
{
	CString sReaderName;
	char aMessage[100];
	char aExceptionMessage[100];
	
	uint8 aAtr[100];
	uint32 uAtrLen = sizeof(aAtr);
	CardSelector *pCardSelector;
	
	m_ComboBoxReader.GetWindowText(sReaderName);
	
	try
	{
		// Check if a reader name is selected
		if (!strcmp(sReaderName, ""))
		{
			_cHelper.showErrorMessage("Select a smartcard reader.");
			return;
		}
		
		// Try to connect to the smartcard through the specified reader name
		if (pPcscReader_->connect((std::string)sReaderName) != 0)
			return;
		
		
		sprintf(aMessage, "\r\n%s %s", "Successfully connected to ", sReaderName);
		addTitleToLog(aMessage);
		
		pMifare_ = new MifareClassic(pPcscReader_);
		if (pMifare_ == NULL)
		{
			_cHelper.showErrorMessage("Insufficient resources for new Mifare Classic Card");
			return;
		}
		
		pPcscReader_->getStatus();
		pPcscReader_->getAtr(aAtr, uAtrLen);
		
		pCardSelector = new CardSelector(pPcscReader_);
		
		_sCardName = pCardSelector->readCardType(aAtr, uAtrLen);
		
		if (_sCardName != "Mifare Standard 1K" && _sCardName != "Mifare Standard 4K")
		{
			_cHelper.showErrorMessage("Card not supported.\r\nPlease present Mifare Classic card.");
			enableCardButtons(false);
			enableTab(false);
			return;
		}
		
		sprintf(aMessage, "%s %s", "Chip Type: ", _sCardName);
		addTitleToLog(aMessage);
		enableTab(true);
		enableCardButtons(true);
	}
	catch (CardException cardException)
	{
		
		sprintf(aExceptionMessage, "%s%s%s%s", "[", _cHelper.byteAsString(cardException.getStatusWord(),2, true), "] ", cardException.getMessage() );
		_cHelper.showErrorMessage(aExceptionMessage);
		addErrMsgToLog(cardException.getMessage());
	}
	catch (PcscException pcscException)
	{
		_cHelper.showErrorMessage(pcscException.what());
		addErrMsgToLog(pcscException.what());
	}
	
}

void CMifareNDEFSampleCodeDlg::OnButtonClearlogs() 
{
	m_EditBoxApduLogs.SetWindowText("");
}

void CMifareNDEFSampleCodeDlg::OnButtonReset() 
{
	if (pPcscReader_ == NULL)
	{
		return;
	}
	
	if (pPcscReader_->isConnectionActive())
	{
		pPcscReader_->disconnect();
	}
	
	if (pMifare_!= NULL)
	{
		delete pMifare_;
		pMifare_ = NULL;
	}	
	
	m_ComboBoxReader.ResetContent();
	
	resetWindow(false);
}

void CMifareNDEFSampleCodeDlg::OnButtonQuit() 
{
	if (pPcscReader_->isConnectionActive())
	{
		pPcscReader_->disconnect();
	}
	
	if (pMifare_!= NULL)
	{
		delete pMifare_;
		pMifare_ = NULL;
	}
	
	EndDialog(0);
	
}

void CMifareNDEFSampleCodeDlg::OnButtonWritetocard() 
{
	writeToCard();
}

void CMifareNDEFSampleCodeDlg::OnButtonReadfromcard() 
{
	readFromCard();
}

void CMifareNDEFSampleCodeDlg::OnButtonResetcard() 
{
	resetCard();
}

void CMifareNDEFSampleCodeDlg::OnEnChangeEditMadkeyb()
{
	CString sTempMadKeyB = "";

	int iTextLen = 0;
	int iKeyLen = 0;

	m_EditBoxMadKeyB.GetWindowText(sTempMadKeyB);
	iTextLen = strlen(sTempMadKeyB);

	for (int iIndex = 0; iIndex < iTextLen; iIndex++)
	{
		if ((isalpha(sTempMadKeyB[iIndex]) || isdigit(sTempMadKeyB[iIndex])) && !isspace(sTempMadKeyB[iIndex]))
		{
			iKeyLen++;
		}
	}

	if (iKeyLen == KEY_LENGTH_STRING)
	{
		m_EditBoxMadKeyB.SetLimitText(KEY_B_WITHOUT_SPACE);
	}
	else
	{
		m_EditBoxMadKeyB.SetLimitText(KEY_B_WITH_SPACE);
	}
}

void CMifareNDEFSampleCodeDlg::OnEnChangeEditNfckeyb()
{
	CString sTempNfcKeyB = "";

	int iTextLen = 0;
	int iKeyLen = 0;

	m_EditBoxNfcKeyB.GetWindowText(sTempNfcKeyB);
	iTextLen = strlen(sTempNfcKeyB);

	for (int iIndex = 0; iIndex < iTextLen; iIndex++)
	{
		if ((isalpha(sTempNfcKeyB[iIndex]) || isdigit(sTempNfcKeyB[iIndex])) && !isspace(sTempNfcKeyB[iIndex]))
		{
			iKeyLen++;
		}
	}

	if (iKeyLen == KEY_LENGTH_STRING)
	{
		m_EditBoxNfcKeyB.SetLimitText(KEY_B_WITHOUT_SPACE);
	}
	else
	{
		m_EditBoxNfcKeyB.SetLimitText(KEY_B_WITH_SPACE);
	}
}


/////////////////////////////////////////////////////////////////
//Helper Methods
/////////////////////////////////////////////////////////////////

//Card Helper

void CMifareNDEFSampleCodeDlg::resetCard()
{
	
	MIFARE_KEY_TYPE eKeyType;
	
	byte aMadKeyB[6], aNfcKeyB[6];
	BYTE aTransportValue[16] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x07, 0x80, 0x69, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };
	byte *pMifareTransportKey;
	byte uNumberOfSectors = 15;
	byte uSectorTrailer = 0;
	
	char aHolder[255];
	char aMessage[100];
	
	int i, j, iTempVal;	
	
	CString cTmpSectorToReset;
	CString sMadKeyBString, sTmpMadKeyB = "";
	CString sNfcKeyBString, sTmpNfcKeyB = "";
	
	memset(aMadKeyB, 0x00, 6);
	memset(aNfcKeyB, 0x00, 6);
	
	try
	{
		m_EditBoxMadKeyB.GetWindowText(sTmpMadKeyB);
		sTmpMadKeyB.TrimLeft(_T(" "));
		
		// Check if there is an input
		if (sTmpMadKeyB == "")
		{
			_cHelper.showErrorMessage("Please key-in hex value for MAD Key B.");
			m_EditBoxMadKeyB.SetFocus();
			return;
		}
		
		for(i = 0; i < sTmpMadKeyB.GetLength(); i++)
		{
			if (sTmpMadKeyB[i] == ' ')
				continue;
			else
			{	
				if (isxdigit(sTmpMadKeyB[i]))
					sMadKeyBString += sTmpMadKeyB[i];
				else
				{
					_cHelper.showErrorMessage("Please key-in hex value for MAD Key B.");
					m_EditBoxMadKeyB.SetFocus();
					return;
				}
			}
			
		}
		
		if ((strlen(sMadKeyBString) % 2) != 0)
		{
			_cHelper.showErrorMessage("Invalid input length. Length must be 6 bytes.");
			m_EditBoxMadKeyB.SetFocus();
			return;
		}
		
		if (strlen(sMadKeyBString) != KEY_LENGTH_STRING)
		{
			_cHelper.showErrorMessage("Invalid input length. Length must be 6 bytes.");
			m_EditBoxMadKeyB.SetFocus();
			return;
		}
		
		for ( i = 0, j = 0; i < strlen(sMadKeyBString); i += 2, j++ )
		{		
			sprintf( aHolder, "%c", sMadKeyBString[i] );
			sprintf( aHolder, "%s%c", aHolder, sMadKeyBString[i + 1] );		
			sscanf( aHolder, "%X", &iTempVal );
			aMadKeyB[j] = iTempVal;
		}
		
		//get NFC Key B
		m_EditBoxNfcKeyB.GetWindowText(sTmpNfcKeyB);
		sTmpNfcKeyB.TrimLeft(_T(" "));
		
		// Check if there is an input
		if (sTmpNfcKeyB == "")
		{
			_cHelper.showErrorMessage("Please key-in hex value for NFC Forum Key B.");
			m_EditBoxNfcKeyB.SetFocus();
			return;
		}
		
		for(i = 0; i < sTmpNfcKeyB.GetLength(); i++)
		{
			if (sTmpNfcKeyB[i] == ' ')
				continue;
			else
			{	
				if (isxdigit(sTmpNfcKeyB[i]))
					sNfcKeyBString += sTmpNfcKeyB[i];
				else
				{
					_cHelper.showErrorMessage("Please key-in hex value for NFC Forum Key B.");
					m_EditBoxNfcKeyB.SetFocus();
					return;
				}
			}
			
		}
		
		if ((strlen(sNfcKeyBString) % 2) != 0)
		{
			_cHelper.showErrorMessage("Invalid input length. Length must be 6 bytes.");
			m_EditBoxNfcKeyB.SetFocus();
			return;
		}
		
		if (strlen(sNfcKeyBString) != KEY_LENGTH_STRING)
		{
			_cHelper.showErrorMessage("Invalid input length. Length must be 6 bytes.");
			m_EditBoxNfcKeyB.SetFocus();
			return;
		}
		
		memset(aHolder, 0x00, DATA_LENGTH);
		
		memset(aHolder, 0x00, DATA_LENGTH);
		
		for ( i = 0, j = 0; i < strlen(sNfcKeyBString); i += 2, j++ )
		{		
			sprintf( aHolder, "%c", sNfcKeyBString[i] );
			sprintf( aHolder, "%s%c", aHolder, sNfcKeyBString[i + 1] );		
			sscanf( aHolder, "%X", &iTempVal );
			aNfcKeyB[j] = iTempVal;
		}
		
		addTitleToLog("\r\nLoad Authentication Keys");
		addMsgToLog("Load NFC Forum Secret Key B to slot 1");
		
		
		//Load NFC Forum Sector Secret Key B to device's memory slot number 1
		if (!pMifare_->loadAuthKey(0x00, aMadKeyB))
		{
			addErrMsgToLog("Load Key Failed");
			return;
		}
		
		//Load Mifare Transport Configuration Key (Default Key) to device's memory slot number 2
		pMifareTransportKey = new byte[6];
		memcpy(pMifareTransportKey, MIFARE_TRANSPORT_KEY, KEY_LENGTH);
		
		addMsgToLog("Load Default Key to slot 2");
		
		if (!pMifare_->loadAuthKey(0x01, pMifareTransportKey))
		{
			addErrMsgToLog("Load Key Failed");
			return;
		}
		
		addTitleToLog("\r\nReset MAD Sector 1");
		
		for (i = 0; i < MAD_SECTORS_COUNT; i++)
        {
            //Authenticate using MAD Sector Secret Key B
			addMsgToLog("Authenticate using MAD Sector Key B");
            if (i == 0)
            {
				eKeyType = KEY_TYPE_B;
				if (!pMifare_->authenticate(0x03, eKeyType, 0x00))
					continue;
            }
            else
            {
                //If failed, assume that the sector is using the transport configuration keys
                //FF FF FF FF FF FF
				eKeyType = KEY_TYPE_A;
				if (!pMifare_->authenticate(0x03, eKeyType, 0x01))
					continue;
            }
			
			if (pMifare_->updateBinary(0x03, aTransportValue))
				break;
        }
		
		if (_sCardName == "Mifare Standard 4K")
		{
			addMsgToLog("Reset MAD Sector 2");
			
			for (int i = 0; i < MAD_SECTORS_COUNT; i++)
            {
                //Authenticate using MAD Sector Secret Key B
                if (i == 0)
                {
					eKeyType = KEY_TYPE_B;
					if (!pMifare_->authenticate(0x63, eKeyType, 0x00))
						continue;
                }
                else
                {
                    //If failed, assume that the sector is using the transport configuration keys
                    //FF FF FF FF FF FF
					eKeyType = KEY_TYPE_A;
					if (!pMifare_->authenticate(0x63, eKeyType, 0x01))
						continue;
                }
				
                if (pMifare_->updateBinary(0x63, aTransportValue))
					break;
            }
		}
		
		if (_sCardName == "Mifare Standard 4K")
			uNumberOfSectors = MIFARE_4K_SECTOR_COUNT;
		else if (_sCardName == "Mifare Standard 1K")
			uNumberOfSectors = MIFARE_1K_SECTOR_COUNT;
		
		for (i = 1; i <= uNumberOfSectors; i++)
        {
            if (i <= MIFARE_4K_SECTOR_WITH_4_BLOCKS)
                uSectorTrailer = (byte)((i * BLOCK_COUNT) + MIFARE_USER_DATA_COUNT_PER_SECTOR);
            else
                uSectorTrailer = (byte)(MIFARE_4K_START_WITH_15_BLOCKS + ((i - MIFARE_4K_START_WITH_15_BLOCKS) * BLOCK_DATA_LENGTH) + MIFARE_4K_EXTENDED_USER_DATA_COUNT);
			
			cTmpSectorToReset.Format("%d",i);
            addMsgToLog("Reset Sector: " + cTmpSectorToReset);
			
			eKeyType = KEY_TYPE_B;
			
			if (!pMifare_->authenticate(uSectorTrailer, eKeyType, 0x00))
				continue;
			
			pMifare_->updateBinary(uSectorTrailer, aTransportValue);
        }
	}
	catch (CardException cardException)
	{
		sprintf(aMessage, "%s%s%s%s", "[", _cHelper.byteAsString(cardException.getStatusWord(),2, true), "] ", cardException.getMessage() );
		_cHelper.showErrorMessage(aMessage);
		addErrMsgToLog(cardException.getMessage());
	}
	catch (PcscException pcscException)
	{
		_cHelper.showErrorMessage(pcscException.what());
		addErrMsgToLog(pcscException.what());
	}
}

void CMifareNDEFSampleCodeDlg::writeToCard()
{
	
	MIFARE_KEY_TYPE eKeyType = KEY_TYPE_A;
	
	byte* pMadSector = NULL;
	byte* pDataToWrite = NULL;
	byte* pMadKeyB = NULL;
	byte* pNfcKeyB = NULL;
	byte* pArrLength = NULL;                
	byte* pBuffer = NULL;
	byte* pTmpMadSector = NULL;
	byte* pTmp = NULL;
	
	byte uNumberOfNfcForumSector = 0;
	byte uNumberOfBlocksNeeded = 0;
	byte uLastSectorAuthenticated = 0;
	byte uPhysicalBlockToWrite = 0x04;           
	byte uSectorTrailer = 0x03;
	
	int iDataToWriteSize = 0;
	int iBufferLength = 0;
	int i, j, iTempVal;
	int iLenToCopy = 16;
	long lDataLength;
	
	char aHolder2[255];
	byte aMifareTransportKey[6];
	byte aTmp[2];
	byte aTmpArray[16];
	char aMessage[100];
	
	CString sTmpStr;
	CString sReaderName = "";
	CString sNfcKeyB, sTmpNfcKeyB = "";
	CString sMadKeyB, sTmpMadKeyB = "";
	CString sNdefMessage, sTmpNdefMessage = "";
	
	try
	{
		m_EditBoxNdefMessage.GetWindowText(sTmpNdefMessage);
		
		for(i = 0; i < sTmpNdefMessage.GetLength(); i++)
		{
			if (sTmpNdefMessage[i] == ' ')
				continue;
			else
				sNdefMessage += sTmpNdefMessage[i];
		}
		
		// Check if there is an input
		if (sNdefMessage == "")
		{
			_cHelper.showErrorMessage("There is no NDEF message to write. Please encode NDEF message first.");
			return;
		}
		
		//allocate memory for datatowrite
		pDataToWrite = new byte[strlen(sNdefMessage)];
		memset(pDataToWrite, 0x00, strlen(sNdefMessage));
		
		for ( i = 0, j = 0; i < strlen(sNdefMessage); i += 2, j++ )
		{		
			sprintf( aHolder2, "%c", sNdefMessage[i] );
			sprintf( aHolder2, "%s%c", aHolder2, sNdefMessage[i + 1] );		
			sscanf( aHolder2, "%X", &iTempVal );
			pDataToWrite[j] = iTempVal;
		}
		
		iDataToWriteSize = j;
		
		//GET MAD KEY B
		m_EditBoxMadKeyB.GetWindowText(sTmpMadKeyB);
		sTmpMadKeyB.TrimLeft(_T(" "));
		
		// Check if there is an input
		if (sTmpMadKeyB == "")
		{
			_cHelper.showErrorMessage("Please key-in hex value for MAD Key B.");
			m_EditBoxMadKeyB.SetFocus();
			return;
		}
		
		for(i = 0; i < sTmpMadKeyB.GetLength(); i++)
		{
			if (sTmpMadKeyB[i] == ' ')
				continue;
			else
			{	
				if (isxdigit(sTmpMadKeyB[i]))
					sMadKeyB += sTmpMadKeyB[i];
				else
				{
					_cHelper.showErrorMessage("Please key-in hex value for MAD Key B.");
					m_EditBoxMadKeyB.SetFocus();
					return;
				}
			}
		}
		
		if ((strlen(sMadKeyB) % 2) != 0)
		{
			_cHelper.showErrorMessage("Invalid input length. Length must be 6 bytes.");
			m_EditBoxMadKeyB.SetFocus();
			return;
		}
		
		if (strlen(sMadKeyB) != KEY_LENGTH_STRING)
		{
			_cHelper.showErrorMessage("Invalid input length. Length must be 6 bytes.");
			m_EditBoxMadKeyB.SetFocus();
			return;
		}
		
		memset(aHolder2, 0x00, DATA_LENGTH);
		pMadKeyB = new byte[KEY_LENGTH];
		memset(pMadKeyB, 0x00, KEY_LENGTH);
		
		for ( i = 0, j = 0; i < strlen(sMadKeyB); i += 2, j++ )
		{		
			sprintf( aHolder2, "%c", sMadKeyB[i] );
			sprintf( aHolder2, "%s%c", aHolder2, sMadKeyB[i + 1] );		
			sscanf( aHolder2, "%X", &iTempVal );
			pMadKeyB[j] = iTempVal;
		}
		
		//GET NFC KEY B
		m_EditBoxNfcKeyB.GetWindowText(sTmpNfcKeyB);
		sTmpNfcKeyB.TrimLeft(_T(" "));
		
		// Check if there is an input
		if (sTmpNfcKeyB == "")
		{
			_cHelper.showErrorMessage("Please key-in hex value for NFC Forum Key B.");
			m_EditBoxNfcKeyB.SetFocus();
			return;
		}
		
		for(i = 0; i < sTmpNfcKeyB.GetLength(); i++)
		{
			if (sTmpNfcKeyB[i] == ' ')
				continue;
			else
			{	
				if (isxdigit(sTmpNfcKeyB[i]))
					sNfcKeyB += sTmpNfcKeyB[i];
				else
				{
					_cHelper.showErrorMessage("Please key-in hex value for NFC Forum Key B.");
					m_EditBoxNfcKeyB.SetFocus();
					return;
				}
			}
		}
		
		if ((strlen(sNfcKeyB) % 2) != 0)
		{
			_cHelper.showErrorMessage("Invalid input length. Length must be 6 bytes.");
			m_EditBoxNfcKeyB.SetFocus();
			return;
		}
		
		if (strlen(sNfcKeyB) != KEY_LENGTH_STRING)
		{
			_cHelper.showErrorMessage("Invalid input length. Length must be 6 bytes.");
			m_EditBoxNfcKeyB.SetFocus();
			return;
		}
		
		memset(aHolder2, 0x00, DATA_LENGTH);
		pNfcKeyB = new byte[KEY_LENGTH];
		memset(pNfcKeyB, 0x00, KEY_LENGTH);
		
		for ( i = 0, j = 0; i < strlen(sNfcKeyB); i += 2, j++ )
		{		
			sprintf( aHolder2, "%c", sNfcKeyB[i] );
			sprintf( aHolder2, "%s%c", aHolder2, sNfcKeyB[i + 1] );		
			sscanf( aHolder2, "%X", &iTempVal );
			pNfcKeyB[j] = iTempVal;
		}
		
		addTitleToLog("\r\nLoad Authentication Key");
		
		//Load NFC Forum Sector Secret Key B to device's memory slot number 1
		memcpy(aMifareTransportKey, MIFARE_TRANSPORT_KEY, KEY_LENGTH);
		
		if (!pMifare_->loadAuthKey(0x00,aMifareTransportKey))
		{
            _cHelper.showErrorMessage("Unable to load key");
            return;
        }
		
		//Delay for ACR122 Reader
		m_ComboBoxReader.GetWindowText(sReaderName);

		//Find returns string::npos if no matches were found
		if (sReaderName.Find("ACR122") != string::npos)
			Sleep(DELAY_FOR_ACR122);
		
		//Form the data to be written to the card
		if (iDataToWriteSize < DATA_LENGTH)
		{
			//If data is less than or equal to 254 (0xFE) use one byte format
            //Refere to NXP Type MF1K/4K Tag Operation document
            //http://www.nxp.com/
			
			aTmp[0] = (BYTE)NDEF_MESSAGE_TLV;
			aTmp[1] = iDataToWriteSize;
			
			pBuffer = new byte[2];
			pBuffer[0] = (BYTE)NDEF_MESSAGE_TLV;
			pBuffer[1] = iDataToWriteSize;
			
			iBufferLength = 2;
		}
		else
		{
			//If data is greater than 254 (0xFE) use 3 bytes format
            lDataLength = iDataToWriteSize;
			
            //In 3 bytes format the last 2 bytes is for 00FE - FFFF                    
            lDataLength -= DATA_LENGTH - 1;
            
            pArrLength = new byte[3];
			
            //Indicates that two more bytes folow
            pArrLength[0] = 0xFF;
            pArrLength[1] = (byte)((lDataLength & 0xFF00) >> 8);
            pArrLength[2] = (byte)(lDataLength & 0xFF);
			
            //NDEF Message TAG
			pBuffer = new byte[4];
			pBuffer[0] = (BYTE)NDEF_MESSAGE_TLV;
			pBuffer[1] = pArrLength[0];
			pBuffer[2] = pArrLength[1];
			pBuffer[3] = pArrLength[2];
			
			iBufferLength = 4;
		}
		
		pTmp = new byte[iBufferLength];
		memcpy(pTmp, pBuffer, iBufferLength);
		
		pBuffer = new byte[iDataToWriteSize + iBufferLength + 1];
		
		memcpy(pBuffer, pTmp, iBufferLength);
		
		for (i = iBufferLength, j = 0; i < iDataToWriteSize + iBufferLength; i++, j++)
		{
			pBuffer[i] = pDataToWrite[j];			
		}
		
		//Append Terminator TLV
		pBuffer[iDataToWriteSize + iBufferLength] = (BYTE)TERMINATOR_TLV;
		
		//Write the data to Mifare Card
        //Start from NFC Form Sector 1 Block 0
		
        addTitleToLog("\r\nWrite NDEF Message To Card");
		
		//Get mumber of blocks needed
        // MAD sector blocks not included
        uNumberOfBlocksNeeded = (byte)((double)ceil(( iDataToWriteSize + iBufferLength + 1 ) / BLOCK_LENGTH));
        uPhysicalBlockToWrite = 0x04; //Start to write at sector 1 block 1 (Physical Block 0x04)
		
		for (int iIndx = 0; iIndx < uNumberOfBlocksNeeded; iIndx++, uPhysicalBlockToWrite++)
        {
            //Check if remaining data to be copied is less than 16
            if ((( iDataToWriteSize + iBufferLength + 1 ) - (iIndx * BLOCK_DATA_LENGTH)) > BLOCK_DATA_LENGTH)
                iLenToCopy = BLOCK_DATA_LENGTH;
            else
                iLenToCopy = (iDataToWriteSize + iBufferLength + 1) - (iIndx * BLOCK_DATA_LENGTH);
            
			for (i = 0, j = (iIndx * BLOCK_DATA_LENGTH); i < iLenToCopy; i++, j++)
			{
				aTmpArray[i] = pBuffer[j];
			}
			
            //Check if sector trailer
            //If sector trailer skip
            if (uPhysicalBlockToWrite > MIFARE_WITH_4_BLOCKS_TOTAL_COUNT)
            {
                if (((uPhysicalBlockToWrite - MIFARE_4K_EXTENDED_USER_DATA_COUNT) % (MIFARE_4K_EXTENDED_USER_DATA_COUNT + 1)) == 0)
                    uPhysicalBlockToWrite++;
            }
            else
            {
                if (((uPhysicalBlockToWrite - MIFARE_USER_DATA_COUNT_PER_SECTOR) % BLOCK_COUNT) == 0)
                    uPhysicalBlockToWrite++;
				
                //Check if MAD Sector 2 (Sector 16)
                //if MAD Sector skip
                if (uPhysicalBlockToWrite >= MAD2_SECTOR_START && uPhysicalBlockToWrite <= MAD2_SECTOR_LAST)
                {
					sTmpStr = "";					
					sTmpStr.Format("%d", (uPhysicalBlockToWrite / BLOCK_COUNT));
                    addMsgToLog("Skip MAD Sector : " + sTmpStr);
                    uPhysicalBlockToWrite = MAD2_SECTOR_LAST + 1; //Move to physical block 68 (Sector 17))
                }
            }
			
            if (uPhysicalBlockToWrite > MIFARE_WITH_4_BLOCKS_TOTAL_COUNT)
            {
                if (uLastSectorAuthenticated != (uPhysicalBlockToWrite / BLOCK_DATA_LENGTH))
                {
					sTmpStr = "";					
					sTmpStr.Format("%d",(((uPhysicalBlockToWrite - MIFARE_WITH_4_BLOCKS_TOTAL_COUNT) / BLOCK_DATA_LENGTH) + MIFARE_WITH_4_BLOCKS_SECTOR_COUNT));
                    addMsgToLog("Authenticate Sector: " + sTmpStr);
					
					if (!pMifare_->authenticate(uPhysicalBlockToWrite, eKeyType, 0x00))
					{
						sTmpStr = "";						
						sTmpStr.Format("%d", (uPhysicalBlockToWrite / BLOCK_DATA_LENGTH));
                        _cHelper.showErrorMessage("Unable to authenticate in sector " + sTmpStr);
						return;
					}
					
                    uLastSectorAuthenticated = (byte)(uPhysicalBlockToWrite / BLOCK_DATA_LENGTH);
                }
            }
            else
            {
				
                if (uLastSectorAuthenticated != (uPhysicalBlockToWrite / BLOCK_COUNT))
                {
					sTmpStr = "";					
					sTmpStr.Format("%d",(uPhysicalBlockToWrite / BLOCK_COUNT));
					
                    addMsgToLog("Authenticate Sector: " + sTmpStr);
					
					if (!pMifare_->authenticate(uPhysicalBlockToWrite, eKeyType, 0x00))
					{
						sTmpStr = "";						
						sTmpStr.Format("%d", (uPhysicalBlockToWrite / BLOCK_COUNT));
                        _cHelper.showErrorMessage("Unable to authenticate in sector " + sTmpStr);
						return;
					}
					
                    uLastSectorAuthenticated = (byte)(uPhysicalBlockToWrite / BLOCK_COUNT);
                }
            }
			
			sTmpStr = "";			
			sTmpStr.Format("%d", uPhysicalBlockToWrite);
            addMsgToLog("Write in block " + sTmpStr);
			
			pMifare_->updateBinary(uPhysicalBlockToWrite, aTmpArray);
        }
		
		//Initialize MAD Sector
        //If number of sector blocks needed is greater than 120 (30 sectors)
        //31st sector have 15 user blocks (1 - 30 sector have 3 user blocks)
        if (uNumberOfBlocksNeeded > MIFARE_WITH_4_BLOCKS_TOTAL_COUNT)
        {
            uNumberOfNfcForumSector = (MIFARE_WITH_4_BLOCKS_SECTOR_COUNT - 1);
            uNumberOfNfcForumSector += (byte)(ceil((uNumberOfBlocksNeeded - MIFARE_WITH_4_BLOCKS_TOTAL_COUNT) / MIFARE_4K_EXTENDED_USER_DATA_COUNT_DOUBLE));
        }
        else
        {
            uNumberOfNfcForumSector = (byte)(ceil(uNumberOfBlocksNeeded / MIFARE_USER_BLOCK));
        }
		
		pMadSector = new byte[MIFARE_WITH_4_BLOCKS_SECTOR_COUNT];
		memset(pMadSector, 0x00, MIFARE_WITH_4_BLOCKS_SECTOR_COUNT);
		
		for (i = 0 ; i < uNumberOfNfcForumSector; i++)
        {
			pMadSector[((i * COPY_LENGTH) + COPY_LENGTH)] = 0x03;
			pMadSector[((i * COPY_LENGTH) + COPY_LENGTH) + 1] = 0xE1;
			
            //Mad Sector 1 contains 15 AIDs (0-14 = 15)
            //the rest of the AIDs will bw written in MAD Sector 2
            if (i == (AIDS_COUNT - 1))
                break;
        }
		
		//Initialize MAD Sector 1
        addTitleToLog("\r\nInitialize MAD Sector 1");
		addMsgToLog("Authenticate MAD Sector 1");
		
        //Write AIDs to MAD Sector 1
		if (!pMifare_->authenticate(0x01, eKeyType, 0x00))
		{
            _cHelper.showErrorMessage("Unable to authenticate in Mifare Application Directory (MAD) sector 1");
			return;
		}
		
        //Write the first 16 bytes (first 7 AIDs) to block 1 of MAD Sector 1 (Sector 0)
		addMsgToLog("Write first 7 AIDs to block 1 of MAD1");
		pTmpMadSector = new byte[BLOCK_DATA_LENGTH];
		memset(pTmpMadSector, 0x00, BLOCK_DATA_LENGTH);
		
		for (i = 0; i < BLOCK_DATA_LENGTH; i++)
			pTmpMadSector[i] = pMadSector[i];
		
		if (!pMifare_->updateBinary(0x01, pTmpMadSector))
		{
            _cHelper.showErrorMessage("Unable to update MAD sector 1");
			return;
		}
		
		addMsgToLog("Write next 8 AIDs to block 2 of MAD1");
		memset(pTmpMadSector, 0x00, BLOCK_DATA_LENGTH);
		for (i = BLOCK_DATA_LENGTH; i < (BLOCK_DATA_LENGTH * 2); i++)
			pTmpMadSector[i - BLOCK_DATA_LENGTH] = pMadSector[i];
		
        //Write next 16 bytes (next 8 AIDs) to block 2 of MAD Sector 1 (Sector 0)
		if (!pMifare_->updateBinary(0x02, pTmpMadSector))
		{
            _cHelper.showErrorMessage("Unable to update MAD sector 1");
			return;
		}
		
		//Initialize MAD Sector 2
		
        //If number of sectors needed (AIDs) is greater than 15 then write the remaining
        //AIDs in the MAD Sector 2 (Sector 16 of the Mifare Card)
        //MAD Sector 2 can store up to 23 AIDs
        if (uNumberOfNfcForumSector >= (MIFARE_1K_TOTAL_SECTOR + 1) && uNumberOfNfcForumSector <= MIFARE_4K_TOTAL_SECTOR)
        {
            addMsgToLog("Initialize MAD Sector 2");
			
            pMadSector = new byte[MAD_SECTOR2_LENGTH];
			memset(pMadSector, 0x00, MAD_SECTOR2_LENGTH);
			
            for (int i = 15; i < uNumberOfNfcForumSector; i++)
			{                
				pMadSector[((i - AIDS_COUNT) * COPY_LENGTH)] = 0x03;
				pMadSector[((i - AIDS_COUNT) * COPY_LENGTH) + 1] = 0xE1;
			}
			
            //Write AIDs to MAD Sector 2 (Sector 16 of the Mifare Card)
			addMsgToLog("Authenticate MAD Sector 2");
			if (!pMifare_->authenticate(0x40, eKeyType, 0x00))
			{
                _cHelper.showErrorMessage("Unable to authenticate in Mifare Application Directory (MAD) sector 2");
				return;
			}
			
			memset(pTmpMadSector, 0x00, BLOCK_DATA_LENGTH);
			for (int i = 0; i < BLOCK_DATA_LENGTH; i++)
				pTmpMadSector[i] = pMadSector[i];
			
            //Write the first 16 bytes (first 7 AIDs) to block 0 of MAD Sector 2 (Sector 16 of the Mifare Card)
			addMsgToLog("Write first 7 AIDs to block 0 of MAD2");
			if (!pMifare_->updateBinary(0x40, pTmpMadSector))
			{
                _cHelper.showErrorMessage("Unable to update MAD sector 2");
				return;
			}
			
			addMsgToLog("Write next 8 AIDs to block 1 of MAD2");
			memset(pTmpMadSector, 0x00, BLOCK_DATA_LENGTH);
			for (int i = BLOCK_DATA_LENGTH; i < (BLOCK_DATA_LENGTH * 2); i++)
				pTmpMadSector[i - BLOCK_DATA_LENGTH] = pMadSector[i];
			
            //Write next 16 bytes (next 8 AIDs) to block 1 of MAD Sector 2
			if (!pMifare_->updateBinary(0x41, pTmpMadSector))
			{
                _cHelper.showErrorMessage("Unable to update MAD sector 2");
				return;
			}
			
			addMsgToLog("Write next 8 AIDs to block 2 of MAD2");
			memset(pTmpMadSector, 0x00, BLOCK_DATA_LENGTH);
			for (int i = (BLOCK_DATA_LENGTH * 2); i < (BLOCK_DATA_LENGTH * 3); i++)
				pTmpMadSector[i - (BLOCK_DATA_LENGTH * 2)] = pMadSector[i];
			
            //Write next 16 bytes (next 8 AIDs) to block 1 of MAD Sector 2
			if (!pMifare_->updateBinary(0x42, pTmpMadSector))
			{
                _cHelper.showErrorMessage("Unable to update MAD sector 2");
				return;
			}
        }
		
		addTitleToLog("\r\nChange Sector Keys");
		
        //Update Sector Keys and Access Bits                
        pBuffer = new byte[BLOCK_DATA_LENGTH];
		memset(pBuffer, 0x00, BLOCK_DATA_LENGTH);
		
		//Key A        
		for (i = 0; i < KEY_LENGTH; i++)
			pBuffer[i] = NDEF_PUBLIC_KEY_A_NFC_FORUM_SECTOR[i];
		
        //Access Bits        
		for (i = KEY_LENGTH; i < (KEY_LENGTH + ACCESS_BITS_LENGTH); i++)
			pBuffer[i] = NDEF_SECTOR_TRAILER_NFC_FORUM_SECTOR[i - KEY_LENGTH];
		
        //Key B        
		for (i = (KEY_LENGTH + ACCESS_BITS_LENGTH); i < ((KEY_LENGTH * 2) + ACCESS_BITS_LENGTH); i++)
			pBuffer[i] = pNfcKeyB[i - (KEY_LENGTH + ACCESS_BITS_LENGTH)];
		
		//Update NFC Forum Sector Keys
        //Start from sector 1
        uSectorTrailer = 0x03;
        for (i = 1; i <= uNumberOfNfcForumSector; i++)
        {
            if (i <= MIFARE_WITH_4_BLOCKS_SECTOR_COUNT)
                uSectorTrailer += BLOCK_COUNT;
            else
                uSectorTrailer += (MIFARE_4K_EXTENDED_USER_DATA_COUNT + 1);
			
            //Check if MAD Sector 2 (Sector 16, Trailer Block = Phycal Block 67)
            //Skip if MAD Sector 2 trailer
            if (uSectorTrailer == MAD2_SECTOR_TRAILER_BLOCK)
            {
                //Move to next sector trailer
                uSectorTrailer += BLOCK_COUNT;
            }
			
            if (uSectorTrailer < MAD2_SECTOR_TRAILER_BLOCK)
			{
				sTmpStr = "";
				sTmpStr.Format("%d",i);
                addMsgToLog("Update Trailer Block of NFC Forum Sector: " + sTmpStr);
			}
            else
			{
				sTmpStr = "";
				sTmpStr.Format("%d", (i + 1));
                addMsgToLog("Update Trailer Block of NFC Forum Sector: " + sTmpStr);
			}
			
			
			if (!pMifare_->authenticate(uSectorTrailer, eKeyType, 0x00))
			{
				sTmpStr = "";
				sTmpStr.Format("%d", i);
                _cHelper.showErrorMessage("Unable to authenticate in sector: " + sTmpStr);
				return;
			}
			
			if (!pMifare_->updateBinary(uSectorTrailer, pBuffer))
			{
				sTmpStr = "";
				sTmpStr.Format("%d",i);
                _cHelper.showErrorMessage("Unable to update trailer block of sector: " + sTmpStr);
				return;
			}
        }
		
		//Update MAD Sector 1 Keys
        addMsgToLog("Update MAD Sector 1 Trailer");
		
		if (!pMifare_->authenticate(0x03, eKeyType, 0x00))
		{
            _cHelper.showErrorMessage("Unable to authenticate in Mifare Application Directory (MAD) sector 1");
			return;
		}
		
        pBuffer = new byte[BLOCK_DATA_LENGTH];
		memset(pBuffer, 0x00, BLOCK_DATA_LENGTH);
		
        //Key A        
		for (i = 0; i < KEY_LENGTH; i++)
			pBuffer[i] = NDEF_PUBLIC_KEY_A_MAD_SECTOR[i];
		
        //Access Bits        
		for (i = KEY_LENGTH; i < (KEY_LENGTH + ACCESS_BITS_LENGTH); i++)
			pBuffer[i] = NDEF_SECTOR_TRAILER_MAD_SECTOR[i - KEY_LENGTH];
		
        //Key B        
		for (i = (KEY_LENGTH + ACCESS_BITS_LENGTH); i < ((KEY_LENGTH * 2) + ACCESS_BITS_LENGTH); i++)
			pBuffer[i] = pMadKeyB[i - (KEY_LENGTH + ACCESS_BITS_LENGTH)];
		
		if (!pMifare_->updateBinary(0x03, pBuffer))
		{
            _cHelper.showErrorMessage("Unable to update trailer block of MAD Sector 1");
			return;
		}
		
        //If MAD Sector is greater than 15
        //Write the remaining AIDs in the MAD Sector 2
        if (uNumberOfNfcForumSector > AIDS_COUNT)
        {
            //Update MAD Sector 2 Keys
            addMsgToLog("Update MAD Sector 2 Trailer Block");
			
			if (!pMifare_->authenticate(0x43, eKeyType, 0x00))
			{
                _cHelper.showErrorMessage("Unable to authenticate in Mifare Application Directory (MAD) sector 2");
				return;
			}
			
            pBuffer = new byte[BLOCK_DATA_LENGTH];
			memset(pBuffer, 0x00, BLOCK_DATA_LENGTH);
			
            //Key A            
			for (i = 0; i < KEY_LENGTH; i++)
				pBuffer[i] = NDEF_PUBLIC_KEY_A_MAD_SECTOR[i];
			
            //Access Bits            
			for (i = KEY_LENGTH; i < (KEY_LENGTH + ACCESS_BITS_LENGTH); i++)
				pBuffer[i] = NDEF_SECTOR_TRAILER_MAD_SECTOR[i - KEY_LENGTH];
			
            //Key B            
			for (i = (KEY_LENGTH + ACCESS_BITS_LENGTH); i < ((KEY_LENGTH * 2) + ACCESS_BITS_LENGTH); i++)
				pBuffer[i] = pMadKeyB[i - (KEY_LENGTH + ACCESS_BITS_LENGTH)];
			
			if (!pMifare_->updateBinary(0x43, pBuffer))
			{
                _cHelper.showErrorMessage("Unable to update trailer block of MAD Sector 2");
				return;
			}
			
        }		
		
	}
	catch (CardException cardException)
	{
		sprintf(aMessage, "%s%s%s%s", "[", _cHelper.byteAsString(cardException.getStatusWord(),2, true), "] ", cardException.getMessage() );
		_cHelper.showErrorMessage(aMessage);
		addErrMsgToLog(cardException.getMessage());
	}
	catch (PcscException pcscException)
	{
		_cHelper.showErrorMessage(pcscException.what());
		addErrMsgToLog(pcscException.what());
	}
}

void CMifareNDEFSampleCodeDlg::readFromCard()
{
	MIFARE_KEY_TYPE eKeyType = KEY_TYPE_A;
	
	byte* pBuffer = NULL;
	byte* pTmpArray = NULL;
	byte* pRecvBuffer = NULL;

	NdefMessage* pTmpNdefMessage;
	NdefRecord* pTemporaryNdefRecord;
	NdefMessage* pTemporaryNdefMessage;
	NdefRecordType* pNdefRecordType;

	byte aNdefPublicKeyAMadSector[6];
	byte aNdefPublicKeyANfcForumSector[6];
	byte aRecvBuffer[1200];
	byte aComparisonConstant[2] = { 0x03, 0xE1 };
	byte aToCompare[2];

	list<NdefRecord> aNdefRecords;
	CString aVCard[3];
	char aNdefMessage[1200];
	char aTmpHolder[4];
	char aMessage[100];
	
	vector<byte> uBuffer;
	vector<byte> uTmpVector;
	byte uNumberOfSectors;
	byte uCurrentSector = 1;
	byte uPhysicalBlock = 0;
	
	int iRecvBufferLength;
	int iEndIndex = 0;
	int iTemporaryLen;
	int iTemporaryPayLoadLen;
	int iBufferLength = 0;
	int iTmpArrayLength = 0;
	int iRecordIndex = 0;
	int iCurrIdx = 0;
	int iEndIdx = 0;
	int iVCardIdx = 0;
	int iVectorIndx = 0;
	
	CString sPayLoadTypeName = "";
	CString sReaderName = "";
	CString sSeparator  = ";";
	CString sDisplayTextTemp = "";
	CString sTempString;

	try
	{
		//Load NFC Forum Sector Secret Key B to device's memory slot number 1
        addMsgToLog("Load MAD Sector Public Key");                
		
		memset(aNdefPublicKeyAMadSector, 0x00, 6);
		memcpy(aNdefPublicKeyAMadSector, NDEF_PUBLIC_KEY_A_MAD_SECTOR, 6);
		
        if (!pMifare_->loadAuthKey(0x00, aNdefPublicKeyAMadSector))
        {
            _cHelper.showErrorMessage("Unable to load key");
            return;
        }
		
        //Load Mifare Transport Configuration Key (Default Key) to device's memory slot number 2
        addMsgToLog("Load NFC Forum Sector Public Key");              
		
		memset(aNdefPublicKeyANfcForumSector, 0x00, KEY_LENGTH);
		memcpy(aNdefPublicKeyANfcForumSector, NDEF_PUBLIC_KEY_A_NFC_FORUM_SECTOR, KEY_LENGTH);
		
        if (!pMifare_->loadAuthKey(0x01, aNdefPublicKeyANfcForumSector))
        {
            _cHelper.showErrorMessage("Unable to load key");
            return;
        }
		
        m_EditBoxNdefMessage.SetWindowText("");
		
		//Read MAD Sector 1
        //Authenticate using the MAD public key
        //That is previously loaded on thevice
		addMsgToLog("Authenticate MAD1 using MAD public key");
        if (!pMifare_->authenticate(0x01, eKeyType, 0x00))
		{
            _cHelper.showErrorMessage("Invalid Card");
			return;
		}
		
		pRecvBuffer = NULL;
		addMsgToLog("Read MAD1 block 1");
		
		try
		{
			pRecvBuffer = pMifare_->readBinary(0x01);
			iRecvBufferLength = BLOCK_DATA_LENGTH;
		}
		catch(...)
		{
			iRecvBufferLength = 0;
		}

		memset(aRecvBuffer, 0x00, MAX_SIZE_OF_ARRAY);
		memcpy(aRecvBuffer, pRecvBuffer, iRecvBufferLength);
		
        pBuffer = new byte[iRecvBufferLength];
		iBufferLength = iRecvBufferLength;
		memcpy(pBuffer, aRecvBuffer, iRecvBufferLength);
		
		pRecvBuffer = NULL;
		addMsgToLog("Read MAD1 block 2");
		
		try
		{
			pRecvBuffer = pMifare_->readBinary(0x02);
			iRecvBufferLength = BLOCK_DATA_LENGTH;
		}
		catch(...)
		{
			iRecvBufferLength = 0;
		}

		memcpy(aRecvBuffer + iRecvBufferLength, pRecvBuffer, iRecvBufferLength);

		iBufferLength = iBufferLength + iRecvBufferLength;

		memset(pBuffer, 0x00, iBufferLength);
		memcpy(pBuffer, aRecvBuffer, iBufferLength);
		
		if (_sCardName == "Mifare Standard 4K")
        {
            //Read MAD Sector 2
			
            //Authenticate using the MAD public key
            //That is previously loaded on thevice
			addMsgToLog("Authenticate MAD2 using MAD public key");
            pMifare_->authenticate(0x40, eKeyType, 0x00);
			
			pRecvBuffer = NULL;
			addMsgToLog("Read MAD2 block 64");
			
			try
			{
				pRecvBuffer = pMifare_->readBinary(0x40);
				iRecvBufferLength = BLOCK_DATA_LENGTH;
			}
			catch(...)
			{
				iRecvBufferLength = 0;
			}
			
			memset(aRecvBuffer, 0x00, MAX_SIZE_OF_ARRAY);
			memcpy(aRecvBuffer, pRecvBuffer, iRecvBufferLength);
			
			iBufferLength = iBufferLength + iRecvBufferLength;
			
			pRecvBuffer = NULL;
			addMsgToLog("Read MAD2 block 65");
			
			try
			{
				pRecvBuffer = pMifare_->readBinary(0x41);
				iRecvBufferLength = BLOCK_DATA_LENGTH;
			}
			catch(...)
			{
				iRecvBufferLength = 0;
			}

			memcpy(aRecvBuffer + iRecvBufferLength, pRecvBuffer, iRecvBufferLength);
			
			pRecvBuffer = NULL;
			addMsgToLog("Read MAD2 block 66");
			
			try
			{
				pRecvBuffer = pMifare_->readBinary(0x42);
				iRecvBufferLength = BLOCK_DATA_LENGTH;
			}
			catch(...)
			{
				iRecvBufferLength = 0;
			}
			
			//Delay for ACR122 Reader
			m_ComboBoxReader.GetWindowText(sReaderName);
			
			//Find returns string::npos if no matches were found
			if (sReaderName.Find("ACR122") != string::npos)
				Sleep(DELAY_FOR_ACR122);

			memcpy(aRecvBuffer + iRecvBufferLength, pRecvBuffer, iRecvBufferLength);
			
			iBufferLength = iBufferLength + iRecvBufferLength;
        }
		
		uNumberOfSectors = 0;
		
        for (int i = 0; i < iBufferLength; i += 2)
        {
			memset(aToCompare, 0x00, 2);
			aToCompare[0] = pBuffer[i];
			aToCompare[1] = pBuffer[i + 1];
			
			if (memcmp(aToCompare, aComparisonConstant, 2) == 0)
				uNumberOfSectors++;
        }
		
		//reset bufferlength
		iBufferLength = 0;
		*pBuffer = NULL;
        uCurrentSector = 1;
		
		memset(aRecvBuffer, 0x00, MAX_SIZE_OF_ARRAY);

        for (int i = 0; i < uNumberOfSectors; i++)
        {                    
            //Skip MAD Sector 2
            if (uCurrentSector == MAD_SECTOR2_ADDRESS)
            {
                uCurrentSector++;
            }
			
            if (uCurrentSector <= MIFARE_4K_SECTOR_WITH_4_BLOCKS)
            {
                uPhysicalBlock = (byte)(uCurrentSector * BLOCK_COUNT);
                pMifare_->authenticate(uPhysicalBlock, eKeyType, 0x01);
				
                for (int j = 0; j < (BLOCK_COUNT - 1); j++)
                {					
					pRecvBuffer = NULL;
					
					try
					{
						pRecvBuffer = pMifare_->readBinary((byte)(uPhysicalBlock + j));
						iRecvBufferLength = BLOCK_DATA_LENGTH;
					}
					catch(...)
					{
						iRecvBufferLength = 0;
					}
					
					memcpy(aRecvBuffer + iBufferLength, pRecvBuffer, iRecvBufferLength);
					
					//set new bufferlength
					iBufferLength = iBufferLength + iRecvBufferLength;
					
                }
				
				pBuffer = new byte[sizeof(aRecvBuffer)];
				memcpy(pBuffer, aRecvBuffer, sizeof(aRecvBuffer));
            }
            else
            {
                uPhysicalBlock = (byte)(MIFARE_4K_START_WITH_15_BLOCKS + ((uCurrentSector - MIFARE_4K_START_SECTOR_WITH_15_BLOCKS) * BLOCK_LENGTH));
                pMifare_->authenticate(uPhysicalBlock, eKeyType, 0x01);
				
                for (int j = 0; j < MIFARE_4K_EXTENDED_USER_DATA_COUNT; j++)
                {
					pRecvBuffer = NULL;
					
					try
					{
						pRecvBuffer = pMifare_->readBinary((byte)(uPhysicalBlock + j));
						iRecvBufferLength = BLOCK_DATA_LENGTH;
					}
					catch(...)
					{
						iRecvBufferLength = 0;
					}
					memcpy(aRecvBuffer + iBufferLength, pRecvBuffer, iRecvBufferLength);
					
					//set new bufferlength
					iBufferLength = iBufferLength + iRecvBufferLength;
                }

				pBuffer = new byte[sizeof(aRecvBuffer)];
				memcpy(pBuffer, aRecvBuffer, sizeof(aRecvBuffer));
            }
            
            uCurrentSector++;
        }
		
		//Check if NDEF Message TAG is present
        if (pBuffer[0] != (byte)(NDEF_MESSAGE_TLV))
		{
            _cHelper.showErrorMessage("NDEF Message TAG is not found");
			return;
		}
		
        //Find the Terminator TLV
        for (iEndIndex = 0; iEndIndex < iBufferLength; iEndIndex++)
        {
            if (pBuffer[iEndIndex] == 0xFE)
                break;
			
            if ((iEndIndex + 1) >= iBufferLength)
			{
                _cHelper.showErrorMessage("Invalid Card.\r\nTerminator Tag is not found");
				return;
			}
        }
		
		TRACE("\nendindex %d",iEndIndex);
		
		pTmpArray = new byte[iEndIndex];
		memset(pTmpArray, 0x00, iEndIndex);
		memcpy(pTmpArray, pBuffer, iEndIndex);		
		
		pBuffer = new byte[iEndIndex];
		memset(pBuffer, 0x00, iEndIndex);
		memcpy(pBuffer, pTmpArray, iEndIndex);
		
		iBufferLength = iEndIndex;		
		
		//Remove NDEF Message Tag
        //Check if it uses one byte or 3 byte format for the length
		
        if (pBuffer[1] == 0xFF)
        {
            //Uses 3 bytes format
			iTmpArrayLength = iBufferLength - THREE_BYTE_MESSAGE_TAG;
            pTmpArray = new byte[iTmpArrayLength];
			memset(pTmpArray, 0x00, iTmpArrayLength);
			
			for (int i = 0; i < iTmpArrayLength; i++)
				pTmpArray[i] = pBuffer[i + THREE_BYTE_MESSAGE_TAG];
        }
        else
        {
			iTmpArrayLength = iBufferLength - TWO_BYTE_MESSAGE_TAG;
            pTmpArray = new byte[iTmpArrayLength];
			memset(pTmpArray, 0x00, iTmpArrayLength);
			
			for (int i = 0; i < iTmpArrayLength; i++)
				pTmpArray[i] = pBuffer[i + TWO_BYTE_MESSAGE_TAG];
			
        }
		
		TRACE("\ntmparraylength %d",iTmpArrayLength);		
		
		strcpy(aNdefMessage, "");
		
		for (int i = 0; i < iTmpArrayLength; i++)
		{
			sprintf(aTmpHolder,"%02X", pTmpArray[i]);
			strcat(aNdefMessage,aTmpHolder);
		}
		
		m_EditBoxNdefMessage.SetWindowText(aNdefMessage);		
		
		iRecordCount = 0;

		while(iVectorIndx < iTmpArrayLength)
		{
			uBuffer.push_back(pTmpArray[iVectorIndx]);
			iVectorIndx++;
		}

		pTmpNdefMessage = cNdef_.getNestedNdefMessage(0, uBuffer);

		if(pTmpNdefMessage->getNumberOfRecords() < 0)
		{
			_cHelper.showInformationMessage("No record to display");
            return;
		}

		aNdefRecords = pTmpNdefMessage->getRecords();

		sPayLoadTypeName = aNdefRecords.begin()->getRecordType()->getTypeName() ;

		if(sPayLoadTypeName == "Sp")
		{
			pTemporaryNdefRecord = &(*(aNdefRecords.begin()));
			pTemporaryNdefMessage = cNdef_.getNestedNdefMessage(pTemporaryNdefRecord);

			if(pTemporaryNdefMessage->getNumberOfRecords() < 1)
			{
				_cHelper.showInformationMessage("Invalid SmartPoster NDEF Message");
				return;
			}

			//Get URI Record (Mandatory)
			pNdefRecordType = new NdefRecordType(NFC_FORM_WELL_KNOWN_TYPE, "U");
            *pTemporaryNdefRecord = pTemporaryNdefMessage->getRecord(*pNdefRecordType);

            if (pTemporaryNdefRecord == NULL)
            {
				_cHelper.showErrorMessage("Mandatory URI Record is not found");
                return;
            }

			
			((CComboBox*)(m_TabControlMain.tabSmartPoster_ComboBoxSmartPosterPrefixUri))->SetCurSel((int)(pTemporaryNdefRecord->getPayload()[0]));
			uTmpVector.clear();

			for (int i = 0; i < (pTemporaryNdefRecord->getPayload().size() - 1); i++)
			{
				uTmpVector.push_back(pTemporaryNdefRecord->getPayload()[i+1]);
			}

			m_TabControlMain.tabSmartPoster_EditBoxSmartPosterUri->SetWindowText(_cHelper.vectorToAscii(uTmpVector));

			//Get Action Record
			((CComboBox*)(m_TabControlMain.tabSmartPoster_ComboBoxSmartPosterAction))->SetCurSel(0);
			pNdefRecordType = new NdefRecordType(NFC_FORM_WELL_KNOWN_TYPE, "act");
            *pTemporaryNdefRecord = pTemporaryNdefMessage->getRecord(*pNdefRecordType);

			if (pTemporaryNdefRecord != NULL)
			{
				if(pTemporaryNdefRecord->getPayload().size() == 0)
					((CComboBox*)(m_TabControlMain.tabSmartPoster_ComboBoxSmartPosterAction))->SetCurSel(0);
				else if((ACTION_RECORD)(pTemporaryNdefRecord->getPayload()[0]) == DO_THE_ACTION)
					((CComboBox*)(m_TabControlMain.tabSmartPoster_ComboBoxSmartPosterAction))->SetCurSel(1);
				else if((ACTION_RECORD)(pTemporaryNdefRecord->getPayload()[0]) == SAVE_FOR_LATER)
					((CComboBox*)(m_TabControlMain.tabSmartPoster_ComboBoxSmartPosterAction))->SetCurSel(2);
				else if((ACTION_RECORD)(pTemporaryNdefRecord->getPayload()[0]) == OPEN_FOR_EDITING)
					((CComboBox*)(m_TabControlMain.tabSmartPoster_ComboBoxSmartPosterAction))->SetCurSel(3);
				else
					((CComboBox*)(m_TabControlMain.tabSmartPoster_ComboBoxSmartPosterAction))->SetCurSel(0);

			}
			else
			{
				((CComboBox*)(m_TabControlMain.tabSmartPoster_ComboBoxSmartPosterAction))->SetCurSel(0);
			}

			//Get Text Record
			pNdefRecordType = new NdefRecordType(NFC_FORM_WELL_KNOWN_TYPE, "T");
            *pTemporaryNdefRecord = pTemporaryNdefMessage->getRecord(*pNdefRecordType);
			if (pTemporaryNdefRecord != NULL)
			{
				iTemporaryLen = pTemporaryNdefRecord->getPayload()[0] & 0x1F;
				uTmpVector.clear();

				for (int i = 0; i < iTemporaryLen; i++)
				{
					uTmpVector.push_back(pTemporaryNdefRecord->getPayload()[i+1]);
				}

				sTempString = _cHelper.vectorToAscii(uTmpVector);

				if(sTempString == "en")
					((CComboBox*)(m_TabControlMain.tabSmartPoster_ComboBoxSmartPosterTitleLanguage))->SetCurSel(0);
				else if(sTempString == "zn-CN")
					((CComboBox*)(m_TabControlMain.tabSmartPoster_ComboBoxSmartPosterTitleLanguage))->SetCurSel(1);
				else if(sTempString == "de")
					((CComboBox*)(m_TabControlMain.tabSmartPoster_ComboBoxSmartPosterTitleLanguage))->SetCurSel(2);
				else
					((CComboBox*)(m_TabControlMain.tabSmartPoster_ComboBoxSmartPosterTitleLanguage))->SetCurSel(0);
								
				iTemporaryPayLoadLen = pTemporaryNdefRecord->getPayload().size() - (iTemporaryLen + 1);
								
				uTmpVector.clear();

				for (int i = 0; i < iTemporaryPayLoadLen; i++)
				{
					uTmpVector.push_back(pTemporaryNdefRecord->getPayload()[i+1+iTemporaryLen]);
				}

				m_TabControlMain.tabSmartPoster_EditBoxSmartPosterTitle->SetWindowText(_cHelper.vectorToAscii(uTmpVector));
			}

			m_TabControlMain.SetCurFocus(0);
			_cHelper.showInformationMessage("Smart Poster Message Found");
		}
		else if(sPayLoadTypeName == "T")
		{
			//Get Text Record
			pTemporaryNdefRecord = &(*(aNdefRecords.begin()));
			
			if((pTemporaryNdefRecord->getPayload()[0] & 0x80) != 0x00)
			{
				((CButton*)(m_TabControlMain.tabText_RadioButtonTextUtf8))->SetCheck(0);
				((CButton*)(m_TabControlMain.tabText_RadioButtonTextUtf16))->SetCheck(1);
			}
			else
			{
				((CButton*)(m_TabControlMain.tabText_RadioButtonTextUtf8))->SetCheck(1);
				((CButton*)(m_TabControlMain.tabText_RadioButtonTextUtf16))->SetCheck(0);
			}

			if (pTemporaryNdefRecord != NULL)
			{
				iTemporaryLen = pTemporaryNdefRecord->getPayload()[0] & 0x1F;
				uTmpVector.clear();

				for (int i = 0; i < iTemporaryLen; i++)
				{
					uTmpVector.push_back(pTemporaryNdefRecord->getPayload()[i+1]);
				}

				sTempString = _cHelper.vectorToAscii(uTmpVector);

				if(sTempString == "en")
					((CComboBox*)(m_TabControlMain.tabText_ComboBoxTextLanguage))->SetCurSel(0);
				else if(sTempString == "zn-CN")
					((CComboBox*)(m_TabControlMain.tabText_ComboBoxTextLanguage))->SetCurSel(1);
				else
					((CComboBox*)(m_TabControlMain.tabText_ComboBoxTextLanguage))->SetCurSel(2);
								
				iTemporaryPayLoadLen = pTemporaryNdefRecord->getPayload().size() - (iTemporaryLen + 1);
				
				uTmpVector.clear();

				for (int i = 0; i < iTemporaryPayLoadLen; i++)
				{
					uTmpVector.push_back(pTemporaryNdefRecord->getPayload()[i+1+iTemporaryLen]);
				}

				m_TabControlMain.tabText_EditBoxTextMessage->SetWindowText(_cHelper.vectorToAscii(uTmpVector));
			}

			m_TabControlMain.SetCurFocus(2);
			_cHelper.showInformationMessage("Text Message Found");
		}
		else if(sPayLoadTypeName == "U")
		{
			pTemporaryNdefRecord = &(*(aNdefRecords.begin()));

			if (pTemporaryNdefRecord == NULL)
            {
				_cHelper.showErrorMessage("Mandatory URI Record is not found");
                return;
            }
			
			((CComboBox*)(m_TabControlMain.tabUrl_ComboBoxUrlUri))->SetCurSel((int)(pTemporaryNdefRecord->getPayload()[0]));
			uTmpVector.clear();

			for (int i = 0; i < (pTemporaryNdefRecord->getPayload().size() - 1); i++)
			{
				uTmpVector.push_back(pTemporaryNdefRecord->getPayload()[i+1]);
			}

			m_TabControlMain.tabUrl_EditBoxUrlUri->SetWindowText(_cHelper.vectorToAscii(uTmpVector));
			m_TabControlMain.SetCurFocus(1);
			_cHelper.showInformationMessage("URI Message Found");

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

			m_TabControlMain.tabVCard_EditBoxSur->SetWindowText(aVCard[0]);
			m_TabControlMain.tabVCard_EditBoxGiven->SetWindowText(aVCard[1]);
			m_TabControlMain.tabVCard_EditBoxMiddle->SetWindowText(aVCard[2]);
			m_TabControlMain.SetCurFocus(3);
			_cHelper.showInformationMessage("vCard Message Found");
		}
		else
		{
			_cHelper.showInformationMessage("Unsupported payload type");
		}
		
		pTmpNdefMessage = NULL;
		pTmpArray = NULL;
		free(pBuffer);
	}
	catch (CardException cardException)
	{
		sprintf(aMessage, "%s%s%s%s", "[", _cHelper.byteAsString(cardException.getStatusWord(),2, true), "] ", cardException.getMessage() );
		_cHelper.showErrorMessage(aMessage);
		addErrMsgToLog(cardException.getMessage());
	}
	catch (PcscException pcscException)
	{
		_cHelper.showErrorMessage(pcscException.what());
		addErrMsgToLog(pcscException.what());
	}
	catch(...)
	{
		_cHelper.showErrorMessage("Failed read from card");
	}
}

//UI Helper

// Displays the message with the corresponding color
void CMifareNDEFSampleCodeDlg::displayOut( int iType, CString sStr, COLORREF color )
{
	int iOldLines = 0,
		iNewLines = 0,
		iScroll = 0;
	long lInsertPoint = 0;
	CHARFORMAT cf;
	
	// Save number of lines before insertion of new text
	iOldLines = _pThis->m_EditBoxApduLogs.GetLineCount();
	
	//Initialize character format structure
	cf.cbSize		= sizeof( CHARFORMAT );
	cf.dwMask		= CFM_COLOR;
	cf.dwEffects	= 0;	// To disable CFE_AUTOCOLOR
	cf.crTextColor	= color;
	
	// Set insertion point to end of text
	lInsertPoint = m_EditBoxApduLogs.GetWindowTextLength();
	m_EditBoxApduLogs.SetSel( lInsertPoint, -1 );
	
	// Set the character format
	//EditBoxApduLogs.SetSelectionCharFormat( cf );
	
	// Insert string at the current caret poisiton
	if(iType == 0)
		m_EditBoxApduLogs.ReplaceSel( "<< " + sStr + "\r\n");
	else
		m_EditBoxApduLogs.ReplaceSel( ">> " + sStr + "\r\n\n");
	
	iNewLines = m_EditBoxApduLogs.GetLineCount();
	iScroll	= iNewLines - iOldLines;
	m_EditBoxApduLogs.LineScroll( 1 );
}

void CMifareNDEFSampleCodeDlg::displayOut(CString sStr, TEXT_TYPE eTextType)
{
	int iOldLines = 0,
		iNewLines = 0,
		iScroll = 0;
	long lInsertPoint = 0;
	CHARFORMAT cf;
	
	// Save number of lines before insertion of new text
	iOldLines = _pThis->m_EditBoxApduLogs.GetLineCount();
	
	//Initialize character format structure
	cf.cbSize		= sizeof( CHARFORMAT );
	cf.dwMask		= CFM_COLOR;
	cf.dwEffects	= 0;	// To disable CFE_AUTOCOLOR
	cf.crTextColor	= BLACK;
	
	// Set insertion point to end of text
	lInsertPoint = m_EditBoxApduLogs.GetWindowTextLength();
	m_EditBoxApduLogs.SetSel( lInsertPoint, -1 );
	
	// Set the character format
	//EditBoxApduLogs.SetSelectionCharFormat( cf );
	
	// Insert string at the current caret poisiton
	if (eTextType == TITLE_TO_LOGS)
	{
		m_EditBoxApduLogs.ReplaceSel(sStr + "\r\n");
	}
	else if (eTextType == MSG_TO_LOGS)
	{
		m_EditBoxApduLogs.ReplaceSel("\r\n" + sStr);
	}
	else if (eTextType == ERR_TO_LOGS)
	{
		m_EditBoxApduLogs.ReplaceSel("\r\n" + sStr + "\r\n");
	}
	
	iNewLines = m_EditBoxApduLogs.GetLineCount();
	iScroll	= iNewLines - iOldLines;
	m_EditBoxApduLogs.LineScroll( 1 );	
}

void CMifareNDEFSampleCodeDlg::displayOutReceive( CString sStr, COLORREF color )
{
	int iOldLines = 0,
		iNewLines = 0,
		iScroll = 0;
	long lInsertPoint = 0;
	CHARFORMAT cf;
	
	// Save number of lines before insertion of new text
	iOldLines = _pThis->m_EditBoxApduLogs.GetLineCount();
	
	//Initialize character format structure
	cf.cbSize		= sizeof( CHARFORMAT );
	cf.dwMask		= CFM_COLOR;
	cf.dwEffects	= 0;	// To disable CFE_AUTOCOLOR
	cf.crTextColor	= color;
	
	// Set insertion point to end of text
	lInsertPoint = m_EditBoxApduLogs.GetWindowTextLength();
	m_EditBoxApduLogs.SetSel( lInsertPoint, -1 );
	
	// Set the character format
	//EditBoxApduLogs.SetSelectionCharFormat( cf );
	
	// Insert string at the current caret poisiton
	m_EditBoxApduLogs.ReplaceSel( sStr + "\r\n");
	
	iNewLines = m_EditBoxApduLogs.GetLineCount();
	iScroll	= iNewLines - iOldLines;
	m_EditBoxApduLogs.LineScroll( 1 );	
}

void CMifareNDEFSampleCodeDlg::addMsgToLog(CString sStr)
{
	displayOut( sStr, MSG_TO_LOGS );
}

void CMifareNDEFSampleCodeDlg::addTitleToLog(CString sStr)
{
	displayOut( sStr, TITLE_TO_LOGS );
}

void CMifareNDEFSampleCodeDlg::addErrMsgToLog(CString sStr)
{
	displayOut( sStr, ERR_TO_LOGS );
}

void CMifareNDEFSampleCodeDlg::addMsgToReceive(CString sStr)
{
	displayOutReceive( sStr, BLACK );
}

void CMifareNDEFSampleCodeDlg::resetWindow(bool bEnable)
{
	//Reset Tab Fields
	m_TabControlMain.SetCurFocus(0);
	m_TabControlMain.activateTabDialogs();

	// Smartposter
	((CComboBox *)m_TabControlMain.tabSmartPoster_ComboBoxSmartPosterTitleLanguage)->SetCurSel( 0 );
	((CComboBox *)m_TabControlMain.tabSmartPoster_ComboBoxSmartPosterPrefixUri)->SetCurSel( 0 );
	((CComboBox *)m_TabControlMain.tabSmartPoster_ComboBoxSmartPosterAction)->SetCurSel( 0 );
	m_TabControlMain.tabSmartPoster_EditBoxSmartPosterTitle->SetWindowText("Advanced Card Systems Ltd.");
	m_TabControlMain.tabSmartPoster_EditBoxSmartPosterUri->SetWindowText("http://www.acs.com.hk");
	
	// Uri
	((CComboBox *)m_TabControlMain.tabUrl_ComboBoxUrlUri)->SetCurSel( 0 );
	m_TabControlMain.tabUrl_EditBoxUrlUri->SetWindowText("http://www.acs.com.hk");

	// Text
	((CComboBox *)m_TabControlMain.tabText_ComboBoxTextLanguage)->SetCurSel( 0 );
	((CButton *)m_TabControlMain.tabText_RadioButtonTextUtf8)->SetCheck(true);
	((CButton *)m_TabControlMain.tabText_RadioButtonTextUtf16)->SetCheck(false);
	m_TabControlMain.tabText_EditBoxTextMessage->SetWindowText("Hello World!");

	// VCard
	m_TabControlMain.tabVCard_EditBoxGiven->SetWindowText("Given Name");
	m_TabControlMain.tabVCard_EditBoxMiddle->SetWindowText("Middle Name");
	m_TabControlMain.tabVCard_EditBoxSur->SetWindowText("Surname");

	//Reset Fields
	m_EditBoxMadKeyA.SetWindowText("A0 A1 A2 A3 A4 A5");
	m_EditBoxMadAccessBits.SetWindowText("78 77 88 C1");
	m_EditBoxMadKeyB.SetWindowText("FF FF FF FF FF FF");
	
	m_EditBoxNfcKeyA.SetWindowText("D3 F7 D3 F7 D3 F7");
	m_EditBoxNfcAccessBits.SetWindowText("7F 07 88 40");
	m_EditBoxNfcKeyB.SetWindowText("FF FF FF FF FF FF");
	
	m_EditBoxNdefMessage.SetWindowText("");
	m_EditBoxApduLogs.SetWindowText("Program Ready\r\n");
	
	m_ButtonConnect.EnableWindow(bEnable);

	enableCardButtons(bEnable);
	enableTab(bEnable);
}

void CMifareNDEFSampleCodeDlg::enableTab(bool bEnable)
{
	m_TabControlMain.EnableWindow(bEnable);
	
	// Smartposter
	m_TabControlMain.tabSmartPoster_ComboBoxSmartPosterTitleLanguage->EnableWindow(bEnable);
	m_TabControlMain.tabSmartPoster_ComboBoxSmartPosterPrefixUri->EnableWindow(bEnable);
	m_TabControlMain.tabSmartPoster_ComboBoxSmartPosterAction->EnableWindow(bEnable);
	m_TabControlMain.tabSmartPoster_StaticSmartPosterTitle->EnableWindow(bEnable);
	m_TabControlMain.tabSmartPoster_StaticSmartPosterUri->EnableWindow(bEnable);
	m_TabControlMain.tabSmartPoster_StaticSmartPosterAction->EnableWindow(bEnable);
	m_TabControlMain.tabSmartPoster_StaticSmartPosterRequired->EnableWindow(bEnable);
	m_TabControlMain.tabSmartPoster_EditBoxSmartPosterTitle->EnableWindow(bEnable);
	m_TabControlMain.tabSmartPoster_EditBoxSmartPosterUri->EnableWindow(bEnable);
	m_TabControlMain.tabSmartPoster_ButtonEncodeSmartPoster->EnableWindow(bEnable);
	
	// Uri
	m_TabControlMain.tabUrl_StaticUrlUri->EnableWindow(bEnable);
	m_TabControlMain.tabUrl_ComboBoxUrlUri->EnableWindow(bEnable);
	m_TabControlMain.tabUrl_EditBoxUrlUri->EnableWindow(bEnable);
	m_TabControlMain.tabUrl_ButtonEncodeUrl->EnableWindow(bEnable);
	
	// Text
	m_TabControlMain.tabText_ComboBoxTextLanguage->EnableWindow(bEnable);
	m_TabControlMain.tabText_StaticTextLanguage->EnableWindow(bEnable);
	m_TabControlMain.tabText_StaticTextMessage->EnableWindow(bEnable);
	m_TabControlMain.tabText_RadioButtonTextUtf8->EnableWindow(bEnable);
	m_TabControlMain.tabText_RadioButtonTextUtf16->EnableWindow(bEnable);
	m_TabControlMain.tabText_EditBoxTextMessage->EnableWindow(bEnable);
	m_TabControlMain.tabText_ButtonEncodeText->EnableWindow(bEnable);
	
	// VCard
	m_TabControlMain.tabVCard_StaticVCardGiven->EnableWindow(bEnable);
	m_TabControlMain.tabVCard_StaticVCardMiddle->EnableWindow(bEnable);
	m_TabControlMain.tabVCard_StaticVCardSur->EnableWindow(bEnable);
	m_TabControlMain.tabVCard_EditBoxGiven->EnableWindow(bEnable);
	m_TabControlMain.tabVCard_EditBoxMiddle->EnableWindow(bEnable);
	m_TabControlMain.tabVCard_EditBoxSur->EnableWindow(bEnable);
	m_TabControlMain.tabVCard_ButtonEncodeVCard->EnableWindow(bEnable);
}

void CMifareNDEFSampleCodeDlg::enableCardButtons(bool bEnable)
{
	m_ButtonWriteToCard.EnableWindow(bEnable);
	m_ButtonResetCard.EnableWindow(bEnable);
	m_ButtonReadFromCard.EnableWindow(bEnable);

	m_EditBoxMadKeyB.EnableWindow(bEnable);
	m_EditBoxNfcKeyB.EnableWindow(bEnable);
}