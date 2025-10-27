#include "stdafx.h"
#include "winscard.h"
#include "ACOS Sample Codes.h"
#include "ACOS Sample CodesDlg.h"
#include "PcscProvider.h"
#include "PcscReader.h"
#include "Helper.h"
#include "Acos3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX 262
// GLOBAL VARIABLES
CACOSSampleCodesDlg	*_pThis = NULL;
Helper _cHelper;
byte _aIssuerCode[] = { 0x41, 0x43, 0x4F, 0x53, 0x54, 0x45, 0x53, 0x54 };

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
// CACOSSampleCodesDlg dialog

CACOSSampleCodesDlg::CACOSSampleCodesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CACOSSampleCodesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CACOSSampleCodesDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);

}

void CACOSSampleCodesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CACOSSampleCodesDlg)
	DDX_Control(pDX, IDC_BUTTON1, ButtonConnect);
	DDX_Control(pDX, IDC_BUTTON2, ButtonFormat);
	DDX_Control(pDX, IDC_BUTTON3, ButtonRead);
	DDX_Control(pDX, IDC_BUTTON4, ButtonWrite);	
	DDX_Control(pDX, IDC_BUTTON5, ButtonReset);
	DDX_Control(pDX, IDC_BUTTON6, ButtonQuit);
	DDX_Control(pDX, IDC_BUTTON8, ButtonInitialize);	
	DDX_Control(pDX, IDC_RADIO1, RadioButtonAA11);	
	DDX_Control(pDX, IDC_RADIO2, RadioButtonBB22);
	DDX_Control(pDX, IDC_RADIO3, RadioButtonCC33);
	DDX_Control(pDX, IDC_COMBO3, ComboBoxReader);
	DDX_Control(pDX, IDC_EDIT1, TextBoxData);
	DDX_Control(pDX, IDC_EDIT2, EditBoxResult);
	DDX_Text(pDX, IDC_EDIT2, EditBoxResultText);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CACOSSampleCodesDlg, CDialog)
	//{{AFX_MSG_MAP(CACOSSampleCodesDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButtonConnect)
	ON_BN_CLICKED(IDC_BUTTON2, OnButtonFormat)	
	ON_BN_CLICKED(IDC_BUTTON3, OnButtonRead)
	ON_BN_CLICKED(IDC_BUTTON4, OnButtonWrite)
	ON_BN_CLICKED(IDC_BUTTON6, OnButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON5, OnButtonReset)
	ON_BN_CLICKED(IDC_BUTTON7, OnButtonClear)
	ON_BN_CLICKED(IDC_BUTTON8, OnButtonInitialize)
	ON_BN_CLICKED(IDC_RADIO1, OnRadioButtonAA11)
	ON_BN_CLICKED(IDC_RADIO2, OnRadioButtonBB22)
	ON_BN_CLICKED(IDC_RADIO3, OnRadioButtonCC33)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CACOSSampleCodesDlg message handlers

BOOL CACOSSampleCodesDlg::OnInitDialog()
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

	// Pass the function pointers to serve as the event listeners of the PcscReader-derived class
	pPcscReader_->setSendCommandFunctor(onSendCommand);
	pPcscReader_->setRecvCommandFunctor(onReceiveCommand);	

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	EditBoxResult.SetWindowText("Program Ready\r\n");
	UpdateData(true);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CACOSSampleCodesDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CACOSSampleCodesDlg::OnPaint() 
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
HCURSOR CACOSSampleCodesDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CACOSSampleCodesDlg::onPcscException(PcscException cException)
{
	sprintf(aMessage_,"\r\n%s",CString(cException.what()));
	displayMessage(aMessage_);
	MessageBox(cException.what(), "Error", MB_ICONERROR);
}

void CACOSSampleCodesDlg::onCardException(CardException cException)
{
	sprintf(aMessage_,"\r\n[%02X %02X] %s",cException.getStatusWord()[0], 
										 cException.getStatusWord()[1], 
										 CString(cException.getMessage()));
	displayMessage(aMessage_);
	MessageBox(aMessage_, "Error", MB_ICONERROR);
}

void CACOSSampleCodesDlg::OnButtonInitialize() 
{
	string *pReaderList = new string[10];
	uint8 uReaderCount = 10;
	int i = 0;

	try
	{
		ComboBoxReader.ResetContent();
		RadioButtonAA11.EnableWindow(false);	
		RadioButtonBB22.EnableWindow(false);
		RadioButtonCC33.EnableWindow(false);
		
		TextBoxData.EnableWindow(false);	
		ButtonConnect.EnableWindow(false);
		ButtonFormat.EnableWindow(false);
		ButtonRead.EnableWindow(false);
		ButtonWrite.EnableWindow(false);
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
					
		ButtonConnect.EnableWindow(true);		

        displayMessage("\r\nInitialize success");
	}
	catch (PcscException pcscException)
	{
		onPcscException(pcscException);
	}	
}

void CACOSSampleCodesDlg::OnButtonConnect() 
{
	CString sReaderName;
	char aMessage[100];
	uint8 aAtr[100];
	uint32 uAtrLen = sizeof(aAtr);
	CString sCardName;
	CardSelector *pCardSelector;

	ComboBoxReader.GetWindowText(sReaderName);

	try
	{
		// Check if a reader name is selected
		if (!strcmp(sReaderName, ""))
		{
			MessageBox("Select a smartcard reader", "Failed", MB_ICONERROR);
			return;
		}

		// Try to connect to the smartcard through the specified reader name
		if (pPcscReader_->connect((std::string)sReaderName) != 0)
			return;
		
		sprintf(aMessage, "%s %s", "\r\nSuccessfully connected to ", sReaderName);
		displayMessage(aMessage);		
	
		pAcos3_ = new Acos3(pPcscReader_);		

		pPcscReader_->getStatus();
		pPcscReader_->getAtr(aAtr, uAtrLen);

		pCardSelector = new CardSelector(pPcscReader_);

		sCardName = pCardSelector->readCardType(aAtr, uAtrLen);

		if(sCardName == "ACOS3")
		{
			displayMessage("Chip Type: ACOS3");
			ButtonFormat.EnableWindow(true);
		}
		else
		{
			MessageBox("Card not supported. Please use ACOS3 card.", "Error", MB_ICONERROR);
			ButtonFormat.EnableWindow(false);			
			OnRadioButtonAA11();
		}

		ButtonConnect.EnableWindow(true);
		ButtonRead.EnableWindow(false);
		ButtonWrite.EnableWindow(false);
		RadioButtonAA11.EnableWindow(false);
		RadioButtonBB22.EnableWindow(false);
		RadioButtonCC33.EnableWindow(false);
		TextBoxData.EnableWindow(false);
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

void CACOSSampleCodesDlg::OnButtonFormat() 
{
	byte aFile[2];
	byte aData[10];
	try
	{
		//submit code
		displayMessage("");
		displayMessage("Submit Code - IC");
		pAcos3_->submitCode(IC,_aIssuerCode);

		//select file ff 02
		displayMessage("");
		displayMessage("Select File");
		aFile[0] = 0xFF;
		aFile[1] = 0x02;
		pAcos3_->selectFile(aFile);

		// Write to FF 02
	    //  This will create 3 User files, no Option registers and
        //  Security Option registers defined, Personalization bit is not set.
		aData[0] = 0x00;
		aData[1] = 0x00;
		aData[2] = 0x03;
		aData[3] = 0x00;
		displayMessage("");
		displayMessage("Write Record");
		pAcos3_->writeRecord(0x00,0x00,aData, 0x04);
		displayMessage("FF 02 is updated");

		//select file ff 04
		aFile[0] = 0xFF;
		aFile[1] = 0x04;
		displayMessage("");
		displayMessage("Select File");
		pAcos3_->selectFile(aFile);

		//submit code
		displayMessage("");
		displayMessage("Submit Code - IC");
		pAcos3_->submitCode(IC,_aIssuerCode);

		// Write to FF 04
		//  Write to first record of FF 04
		aData[0] = 0x0A;
		aData[1] = 0x03;
		aData[2] = 0x00;
		aData[3] = 0x00;
		aData[4] = 0xAA;
		aData[5] = 0x11;
		aData[6] = 0x00;
		displayMessage("");
		displayMessage("Write Record");
		pAcos3_->writeRecord(0x00,0x00,aData, 0x07);
		displayMessage("User File AA 11 is defined");

		//  Write to second record of FF 04
		aData[0] = 0x10;
		aData[1] = 0x02;
		aData[2] = 0x00;
		aData[3] = 0x00;
		aData[4] = 0xBB;
		aData[5] = 0x22;
		aData[6] = 0x00;
		displayMessage("");
		displayMessage("Write Record");
		pAcos3_->writeRecord(0x01,0x00,aData, 0x07);
		displayMessage("User File BB 22 is defined");

		//  Write to third record of FF 04
		aData[0] = 0x20;
		aData[1] = 0x04;
		aData[2] = 0x00;
		aData[3] = 0x00;
		aData[4] = 0xCC;
		aData[5] = 0x33;
		aData[6] = 0x00;
		displayMessage("");
		displayMessage("Write Record");
		pAcos3_->writeRecord(0x02,0x00,aData, 0x07);
		displayMessage("User File CC 33 is defined");

		RadioButtonAA11.EnableWindow(true);	
		RadioButtonBB22.EnableWindow(true);
		RadioButtonCC33.EnableWindow(true);

		RadioButtonAA11.SetCheck(1);
		RadioButtonBB22.SetCheck(0);
		RadioButtonCC33.SetCheck(0);
		
		TextBoxData.EnableWindow(true);
		TextBoxData.SetLimitText(10);

		ButtonRead.EnableWindow(true);
		ButtonWrite.EnableWindow(true);

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

void CACOSSampleCodesDlg::OnRadioButtonAA11() 
{
	RadioButtonAA11.SetCheck(1);
	RadioButtonBB22.SetCheck(0);	
	RadioButtonCC33.SetCheck(0);	
	TextBoxData.SetWindowText("");
	TextBoxData.SetLimitText(10);
}

void CACOSSampleCodesDlg::OnRadioButtonBB22() 
{
	RadioButtonAA11.SetCheck(0);
	RadioButtonBB22.SetCheck(1);	
	RadioButtonCC33.SetCheck(0);	
	TextBoxData.SetWindowText("");
	TextBoxData.SetLimitText(16);
}

void CACOSSampleCodesDlg::OnRadioButtonCC33() 
{
	RadioButtonAA11.SetCheck(0);
	RadioButtonBB22.SetCheck(0);	
	RadioButtonCC33.SetCheck(1);
	TextBoxData.SetWindowText("");
	TextBoxData.SetLimitText(32);
}

void CACOSSampleCodesDlg::OnButtonRead() 
{
	byte aFile[2];
	byte uDataLen;
	byte* aData;
	try
	{
		if (RadioButtonAA11.GetCheck() == 1)
		{
			aFile[0] = 0xAA;
			aFile[1] = 0x11;
			uDataLen = 0x0A;
		}
		if (RadioButtonBB22.GetCheck() == 1)
		{
			aFile[0] = 0xBB;
			aFile[1] = 0x22;
			uDataLen = 0x10;
		}
		if (RadioButtonCC33.GetCheck() == 1)
		{
			aFile[0] = 0xCC;
			aFile[1] = 0x33;
			uDataLen = 0x20;
		}

		//select user file
		displayMessage("");
		displayMessage("Select File");
		pAcos3_->selectFile(aFile);

		aData = new byte[uDataLen +1];
		//memset(aData,0,sizeof(aData));

		//read first record of user file selected
		displayMessage("");
		displayMessage("Read Record");
		pAcos3_->readRecord(0x00,0x00, aData, uDataLen);

		aData[uDataLen] = 0;

		/*CString strData;
		int index = 0;
		while(index <= sizeof((byte)dataLen)-1)
		{
			strData += aData[index];
			++index;
		}
*/
		TextBoxData.SetWindowText( CString(aData) );

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

void CACOSSampleCodesDlg::OnButtonWrite() 
{
	byte aFile[2];
	byte uDataLen;
	char aTempdata[262];
	BYTE aData[256];
	try
	{
		if (RadioButtonAA11.GetCheck() == 1)
		{
			aFile[0] = 0xAA;
			aFile[1] = 0x11;
			uDataLen = 0x0A;
		}
		if (RadioButtonBB22.GetCheck() == 1)
		{
			aFile[0] = 0xBB;
			aFile[1] = 0x22;
			uDataLen = 0x10;
		}
		if (RadioButtonCC33.GetCheck() == 1)
		{
			aFile[0] = 0xCC;
			aFile[1] = 0x33;
			uDataLen = 0x20;
		}

		TextBoxData.LimitText(uDataLen);

		

		//Write Data to card
		TextBoxData.GetWindowText( aTempdata, MAX );

		if(strcmp(aTempdata, "") == 0)
		{
			MessageBox("Please key-in the data to write.", "Error", MB_ICONERROR);
			TextBoxData.SetFocus();
			return;
		}
		
		//select user aFile
		displayMessage("");
		displayMessage("Select File");
		pAcos3_->selectFile(aFile);

		if(strlen(aTempdata) > uDataLen)
		{
			MessageBox("Length of data is greater than the length specified.", "Error", MB_ICONERROR);
			TextBoxData.SetFocus();
			return;	
		}
		
		//Clear temparr
		memset(aData,0,sizeof(aData));
		strcpy((char*)aData,aTempdata);

		displayMessage("");
		displayMessage("Write Record");
		pAcos3_->writeRecord(0x00,0x00,aData, uDataLen);

		displayMessage("Data read from textbox is written to card");
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

void CACOSSampleCodesDlg::OnButtonQuit() 
{
	try
	{
		// Free existing smartcard connection handle
		if (pPcscReader_->isConnectionActive())
			pPcscReader_->disconnect();
		
	}
	catch (CardException cardException)
	{
		onCardException(cardException);
	}
	catch (PcscException pcscException)
	{
		onPcscException(pcscException);
	}	
	
	OnCancel();	
}

void CACOSSampleCodesDlg::resetFields()
{
	RadioButtonAA11.EnableWindow(false);	
	RadioButtonBB22.EnableWindow(false);
	RadioButtonCC33.EnableWindow(false);

	RadioButtonAA11.SetCheck(0);
	RadioButtonBB22.SetCheck(0);
	RadioButtonCC33.SetCheck(0);
	
	TextBoxData.SetWindowText("");
	TextBoxData.EnableWindow(false);	
	ButtonConnect.EnableWindow(false);
	ButtonFormat.EnableWindow(false);
	ButtonRead.EnableWindow(false);
	ButtonWrite.EnableWindow(false);
	ButtonInitialize.EnableWindow(true);

	ComboBoxReader.ResetContent();

	EditBoxResult.SetWindowText("Program Ready\r\n");	
	UpdateData(true);
}

void CACOSSampleCodesDlg::OnButtonReset() 
{
	try
	{
		// Free existing smartcard connection handle
		if (pPcscReader_->isConnectionActive())
			if (pPcscReader_->disconnect() != 0)
				return;

		resetFields();
		
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

void CACOSSampleCodesDlg::OnButtonClear() 
{	
	EditBoxResult.SetWindowText("");	
	UpdateData(true);
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
void CACOSSampleCodesDlg::displayMessage(CString sMessage)
{
    EditBoxResultText += sMessage + "\r\n";
	UpdateData(false);
	EditBoxResult.SetSel(0xFFFF, 0xFFFF);
}
