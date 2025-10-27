
#include "stdafx.h"
#include "winscard.h"
#include "ACOS Sample Codes.h"
#include "ACOS Sample CodesDlg.h"
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

// GLOBAL VARIABLES
CACOSSampleCodesDlg *_pThis = NULL;

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
	EditBoxLogsValue = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON2);

}


void CACOSSampleCodesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CACOSSampleCodesDlg)		
	DDX_Control(pDX, IDC_CheckBoxRequireDebit, CheckBoxRequireDebit);
	DDX_Control(pDX, IDC_RadioButtonDes, RadioButtonDes);
	DDX_Control(pDX, IDC_RadioButton3Des, RadioButton3Des);
	DDX_Control(pDX, IDC_EditBoxRevokeDebit, EditBoxRevokeDebit);
	DDX_Control(pDX, IDC_EditBoxLogs, EditBoxLogs);
	DDX_Control(pDX, IDC_EditBoxDebitKey, EditBoxDebitKey);
	DDX_Control(pDX, IDC_EditBoxCreditKey, EditBoxCreditKey);
	DDX_Control(pDX, IDC_EditBoxCertifyKey, EditBoxCertifyKey);
	DDX_Control(pDX, IDC_EditBoxAmount, EditBoxAmount);
	DDX_Control(pDX, IDC_ComboBoxReaderNames, ComboBoxReaderNames);
	DDX_Control(pDX, IDC_ButtonRevokeDebit, ButtonRevokeDebit);
	DDX_Control(pDX, IDC_ButtonReset, ButtonReset);
	DDX_Control(pDX, IDC_ButtonQuit, ButtonQuit);
	DDX_Control(pDX, IDC_ButtonInquireBalance, ButtonInquireBalance);
	DDX_Control(pDX, IDC_ButtonInitialize, ButtonInitialize);
	DDX_Control(pDX, IDC_ButtonFormat, ButtonFormat);
	DDX_Control(pDX, IDC_ButtonDebit, ButtonDebit);
	DDX_Control(pDX, IDC_ButtonCredit, ButtonCredit);
	DDX_Control(pDX, IDC_ButtonConnect, ButtonConnect);
	DDX_Control(pDX, IDC_ButtonClear, ButtonClear);		
	DDX_Text(pDX, IDC_EditBoxLogs, EditBoxLogsValue);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CACOSSampleCodesDlg, CDialog)
	//{{AFX_MSG_MAP(CACOSSampleCodesDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ButtonInitialize, OnButtonInitialize)
	ON_BN_CLICKED(IDC_ButtonConnect, OnButtonConnect)
	ON_BN_CLICKED(IDC_ButtonFormat, OnButtonFormat)
	ON_BN_CLICKED(IDC_ButtonCredit, OnButtonCredit)
	ON_BN_CLICKED(IDC_ButtonDebit, OnButtonDebit)
	ON_BN_CLICKED(IDC_ButtonInquireBalance, OnButtonInquireBalance)
	ON_BN_CLICKED(IDC_ButtonRevokeDebit, OnButtonRevokeDebit)
	ON_BN_CLICKED(IDC_ButtonClear, OnButtonClear)
	ON_BN_CLICKED(IDC_ButtonReset, OnButtonReset)
	ON_BN_CLICKED(IDC_ButtonQuit, OnButtonQuit)
	ON_BN_CLICKED(IDC_RadioButtonDes, OnRadioButtonDes)
	ON_BN_CLICKED(IDC_RadioButton3Des, OnRadioButton3Des)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CACOSSampleCodesDlg message handlers

BOOL CACOSSampleCodesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	_pThis = this;

	// Instantiate an object of the PcscReader-derived class
	pPcscReader_ = new PcscReader();
	pAcos3_ = NULL;

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	EditBoxLogsValue = "Program Ready\r\n";
	UpdateData(false);

	EditBoxCreditKey.SetLimitText(8);
	EditBoxDebitKey.SetLimitText(8);
	EditBoxCertifyKey.SetLimitText(8);
	EditBoxRevokeDebit.SetLimitText(8);
	EditBoxAmount.SetLimitText(8);
	RadioButtonDes.SetCheck(true);

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

HCURSOR CACOSSampleCodesDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

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

//Display the message to the log window
void CACOSSampleCodesDlg::displayMessage(CString msg)
{
    EditBoxLogsValue += msg + "\r\n";
	UpdateData(false);
	EditBoxLogs.SetSel(0xFFFF, 0xFFFF);
}

void CACOSSampleCodesDlg::enableControls(BOOL state)
{
	ButtonConnect.EnableWindow(state);
	ButtonFormat.EnableWindow(state);
	ButtonCredit.EnableWindow(state);
	ButtonInquireBalance.EnableWindow(state);
	ButtonDebit.EnableWindow(state);
	ButtonRevokeDebit.EnableWindow(state);
	EditBoxCreditKey.EnableWindow(state);
	EditBoxDebitKey.EnableWindow(state);
	EditBoxCertifyKey.EnableWindow(state);
	EditBoxRevokeDebit.EnableWindow(state);
	EditBoxAmount.EnableWindow(state);
	CheckBoxRequireDebit.EnableWindow(state);
	RadioButton3Des.EnableWindow(state);
	RadioButtonDes.EnableWindow(state);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CACOSSampleCodesDlg::OnButtonInitialize() 
{
	string *pReaderList = new string[10];
	uint8 uReaderCount = 10;
	int readerNameLength = 0;
	int i = 0;

	try
	{
		ComboBoxReaderNames.ResetContent();
		enableControls(false);

		// Establish Context
		pPcscReader_->initialize();

		// List the detected smart card readers
		pPcscReader_->listReaders(pReaderList, uReaderCount);


		// Load the retrieved reader name array to the reader list
		for(i = 0; i < uReaderCount; i++)
		{
			ComboBoxReaderNames.AddString(pReaderList[i].data());
		}
		
		ComboBoxReaderNames.SetCurSel(0);

		// Pass the function pointers to serve as the event listeners of the ProprietaryReader class
		pPcscReader_->setSendCommandFunctor(onSendCommand);
		pPcscReader_->setRecvCommandFunctor(onReceiveCommand);					

        displayMessage("\r\nInitialize success");
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

void CACOSSampleCodesDlg::OnButtonConnect() 
{
	CString readerName;
	char message[100];

	uint8 aAtr[100];
	uint32 uAtrLen = 100;
	CString sCardName;
	CardSelector *pCardSelector;
	
	ComboBoxReaderNames.GetWindowText(readerName);

	try
	{
		// Check if a reader name is selected
		if (!strcmp(readerName, ""))
		{
			MessageBox("Select a smartcard reader.", "Error", MB_ICONERROR);
			return;
		}

		// Try to connect to the smartcard through the specified reader name
		if (pPcscReader_->connect((std::string)readerName) != 0)
			return;
		
		sprintf(message, "\r\n%s %s", "Successfully connected to ", readerName);
		displayMessage(message);
		
		
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

			ButtonCredit.EnableWindow(false);
			ButtonDebit.EnableWindow(false);
			ButtonRevokeDebit.EnableWindow(false);
			ButtonInquireBalance.EnableWindow(false);
			EditBoxAmount.EnableWindow(false);	
			CheckBoxRequireDebit.EnableWindow(false);
		}
		else
		{
			MessageBox("Card not supported. Please use ACOS3 card.", "Error", MB_ICONERROR);
			enableControls(false);	
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

void CACOSSampleCodesDlg::OnButtonFormat() 
{
	byte aFile[2];
	byte aData[10];
	char aCreditKey[100], aDebitKey[100], aCertifyKey[100], aRevokeDebitKey[100];
	int iCtr;

	try
	{
		EditBoxCreditKey.GetWindowText(aCreditKey, 100);
		EditBoxDebitKey.GetWindowText(aDebitKey, 100);
		EditBoxCertifyKey.GetWindowText(aCertifyKey, 100);
		EditBoxRevokeDebit.GetWindowText(aRevokeDebitKey, 100);

		if(strcmp(aCreditKey, "") == 0)
		{
			MessageBox("Please key-in numeric value for Credit Key.", "Error", MB_ICONERROR);
			EditBoxCreditKey.SetFocus();
			return;
		}

		if(strcmp(aDebitKey, "") == 0)
		{
			MessageBox("Please key-in numeric value for Debit Key.", "Error", MB_ICONERROR);
			EditBoxDebitKey.SetFocus();
			return;
		}

		if (strcmp(aCertifyKey, "") == 0)
		{
			MessageBox("Please key-in numeric value for Certify Key.", "Error", MB_ICONERROR);
			EditBoxCertifyKey.SetFocus();
			return;
		}

		if (strcmp(aRevokeDebitKey, "") == 0)
		{
			MessageBox("Please key-in numeric value for Revoke Debit Key.", "Error", MB_ICONERROR);
			EditBoxRevokeDebit.SetFocus();
			return;
		}

		if (RadioButtonDes.GetCheck() == 1)
		{
			
			if (strlen(aCreditKey) < 8) 
			{
				MessageBox("Invalid input length. Length must be 8.", "Error", MB_ICONERROR);
				EditBoxCreditKey.SetFocus();
				return;
			}		
			
			if (strlen(aDebitKey) < 8) 
			{
				MessageBox("Invalid input length. Length must be 8.", "Error", MB_ICONERROR);
				EditBoxDebitKey.SetFocus();
				return;
			}

			if (strlen(aCertifyKey) < 8) 
			{
				MessageBox("Invalid input length. Length must be 8.", "Error", MB_ICONERROR);
				EditBoxCertifyKey.SetFocus();
				return;
			}

			if (strlen(aRevokeDebitKey) < 8) 
			{
				MessageBox("Invalid input length. Length must be 8.", "Error", MB_ICONERROR);
				EditBoxRevokeDebit.SetFocus();
				return;
			}
		}
		else
		{

			if (strlen(aCreditKey) < 16) 
			{
				MessageBox("Invalid input length. Length must be 16.", "Error", MB_ICONERROR);
				EditBoxCreditKey.SetFocus();
				return;
			}

			if (strlen(aDebitKey) < 16) 
			{
				MessageBox("Invalid input length. Length must be 16.", "Error", MB_ICONERROR);
				EditBoxDebitKey.SetFocus();
				return;
			}

			if (strlen(aCertifyKey) < 16) 
			{
				MessageBox("Invalid input length. Length must be 16.", "Error", MB_ICONERROR);
				EditBoxCertifyKey.SetFocus();
				return;
			}

			if (strlen(aRevokeDebitKey) < 16) 
			{
				MessageBox("Invalid input length. Length must be 16.", "Error", MB_ICONERROR);
				EditBoxRevokeDebit.SetFocus();
				return;
			}
		}

		// submit code
		pAcos3_->submitCode(IC, _aIssuerCode);

		// select file ff 02
		aFile[0] = 0xFF;
		aFile[1] = 0x02;
		pAcos3_->selectFile(aFile);

		// Write to FF 02
		// This will create 3 USer files, no option register and
		// Security option registers defined, personalization bit is not set.
		if (RadioButtonDes.GetCheck() == 1)
		{
			aData[0] = 0x29;
		}
		else
		{
			aData[0] = 0x2B;
		}
		aData[1] = 0x00;
		aData[2] = 0x30;
		aData[3] = 0x00;
		pAcos3_->writeRecord(0x00, 0x00, aData, 0x04);
		displayMessage("FF 02 is updated");

		// submit code
		pAcos3_->submitCode(IC, _aIssuerCode);

		// select file ff 05
		aFile[0] = 0xFF;
		aFile[1] = 0x05;
		pAcos3_->selectFile(aFile);

		// Record 00
		aData[0] = 0x00;
		aData[1] = 0x00;
		aData[2] = 0x00;
		aData[3] = 0x00;
		pAcos3_->writeRecord(0x00, 0x00, aData, 0x04);
		displayMessage("Record 00 is updated");

		// Record 01
		aData[0] = 0x00;
		aData[1] = 0x00;
		aData[2] = 0x01;
		aData[3] = 0x00;
		pAcos3_->writeRecord(0x01, 0x00, aData, 0x04);
		displayMessage("Record 01 is updated");

		// Record 02
		aData[0] = 0x00;
		aData[1] = 0x00;
		aData[2] = 0x00;
		aData[3] = 0x00;
		pAcos3_->writeRecord(0x02, 0x00, aData, 0x04);
		displayMessage("Record 02 is updated");

		// Record 03
		aData[0] = 0x00;
		aData[1] = 0x00;
		aData[2] = 0x01;
		aData[3] = 0x00;
		pAcos3_->writeRecord(0x03, 0x00, aData, 0x04);
		displayMessage("Record 03 is updated");

		// Record 04
		aData[0] = 0xFF;
		aData[1] = 0xFF;
		aData[2] = 0xFF;
		aData[3] = 0x00;
		pAcos3_->writeRecord(0x04, 0x00, aData, 0x04);
		displayMessage("Record 04 is updated");

		// Record 05
		aData[0] = 0x00;
		aData[1] = 0x00;
		aData[2] = 0x00;
		aData[3] = 0x00;
		pAcos3_->writeRecord(0x05, 0x00, aData, 0x04);
		displayMessage("Record 05 is updated");

		// Record 06
		aData[0] = 0x00;
		aData[1] = 0x00;
		aData[2] = 0x00;
		aData[3] = 0x00;
		pAcos3_->writeRecord(0x06, 0x00, aData, 0x04);
		displayMessage("Record 06 is updated");

		// Record 07
		aData[0] = 0x00;
		aData[1] = 0x00;
		aData[2] = 0x00;
		aData[3] = 0x00;
		pAcos3_->writeRecord(0x07, 0x00, aData, 0x04);
		displayMessage("Record 07 is updated");

		displayMessage("FF 05 is updated");

		// select file ff 06
		aFile[0] = 0xFF;
		aFile[1] = 0x06;
		pAcos3_->selectFile(aFile);

		if (RadioButtonDes.GetCheck() == 1)
		{	
			// Record 00 for Debit key
			for (iCtr = 0; iCtr < (int)strlen((char*)aDebitKey); iCtr++)
			{        
					aData[iCtr] = aDebitKey[iCtr];
			}

			pAcos3_->writeRecord(0x00, 0x00, aData, 0x08);
			displayMessage("Debit key is updated");

			// Record 01 for Credit key
			for (iCtr = 0; iCtr < (int)strlen((char*)aCreditKey); iCtr++)
			{        
					aData[iCtr] = aCreditKey[iCtr];
			}

			pAcos3_->writeRecord(0x01, 0x00, aData, 0x08);
			displayMessage("Credit key is updated");

			// Record 02 for Certify key
			for (iCtr = 0; iCtr < (int)strlen((char*)aCertifyKey); iCtr++)
			{        
					aData[iCtr] = aCertifyKey[iCtr];
			}

			pAcos3_->writeRecord(0x02, 0x00, aData, 0x08);
			displayMessage("Certify key is updated");

			// Record 03 for Revoke key
			EditBoxRevokeDebit.GetWindowText(aRevokeDebitKey, 100);
			for (iCtr = 0; iCtr < (int)strlen((char*)aRevokeDebitKey); iCtr++)
			{        
					aData[iCtr] = aRevokeDebitKey[iCtr];
			}

			pAcos3_->writeRecord(0x03, 0x00, aData, 0x08);
			displayMessage("Revoke key is updated");
		}
		else
		{
			// Record 04 for left half of Debit key
			for (iCtr = 0; iCtr < (int)((strlen((char*)aDebitKey)/2)); iCtr++)
			{        
					aData[iCtr] = aDebitKey[iCtr];
			}

			pAcos3_->writeRecord(0x04, 0x00, aData, 0x08);
			displayMessage("Left Half of Debit key is updated");

			// Record 00 for right half of Debit key
			for (iCtr = 0; iCtr < (int)((strlen((char*)aDebitKey)/2)); iCtr++)
			{        
					aData[iCtr] = aDebitKey[iCtr + 8];
			}

			pAcos3_->writeRecord(0x00, 0x00, aData, 0x08);
			displayMessage("Right Half of Debit key is updated");

			// Record 05 for left half of Credit key
			for (iCtr = 0; iCtr < (int)((strlen((char*)aCreditKey)/2)); iCtr++)
			{        
					aData[iCtr] = aCreditKey[iCtr];
			}

			pAcos3_->writeRecord(0x05, 0x00, aData, 0x08);
			displayMessage("Left Half of Credit key is updated");

			// Record 01 for right half of Credit key
			for (iCtr = 0; iCtr < (int)((strlen((char*)aCreditKey)/2)); iCtr++)
			{        
					aData[iCtr] = aCreditKey[iCtr + 8];
			}

			pAcos3_->writeRecord(0x01, 0x00, aData, 0x08);
			displayMessage("Right Half of Credit key is updated");

			// Record 06 for left half of Certify key
			for (iCtr = 0; iCtr < (int)((strlen((char*)aCertifyKey)/2)); iCtr++)
			{        
					aData[iCtr] = aCertifyKey[iCtr];
			}
			pAcos3_->writeRecord(0x06, 0x00, aData, 0x08);
			displayMessage("Left Half of Certify key is updated");

			// Record 02 for right half of Certify key
			for (iCtr = 0; iCtr < (int)((strlen((char*)aCertifyKey)/2)); iCtr++)
			{        
					aData[iCtr] = aCertifyKey[iCtr + 8];
			}

			pAcos3_->writeRecord(0x02, 0x00, aData, 0x08);
			displayMessage("Right Half of Certify key is updated");

			// Record 07 for left half of Revoke key
			for (iCtr = 0; iCtr < (int)((strlen((char*)aRevokeDebitKey)/2)); iCtr++)
			{        
					aData[iCtr] = aRevokeDebitKey[iCtr];
			}
			pAcos3_->writeRecord(0x07, 0x00, aData, 0x08);
			displayMessage("Left Half of Revoke key is updated");

			// Record 03 for right half of Revoke key
			for (iCtr = 0; iCtr < (int)((strlen((char*)aRevokeDebitKey)/2)); iCtr++)
			{        
					aData[iCtr] = aRevokeDebitKey[iCtr + 8];
			}

			pAcos3_->writeRecord(0x03, 0x00, aData, 0x08);
			displayMessage("Right Half of Revoke key is updated");
		}

		displayMessage("FF 06 is updated");
		ButtonCredit.EnableWindow(true);
		ButtonDebit.EnableWindow(true);
		ButtonRevokeDebit.EnableWindow(true);
		ButtonInquireBalance.EnableWindow(true);
		EditBoxAmount.EnableWindow(true);	
		CheckBoxRequireDebit.EnableWindow(true);

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

void CACOSSampleCodesDlg::OnButtonCredit() 
{
	unsigned char uCreditKey[16];
	unsigned char uCreditKey3DES [16];
	unsigned char uCreditKeyDES [8];
	unsigned char uTmpArray[32];
	unsigned long uCreditAmount;
	unsigned long uTempAmount;

	char aCreditKey[100];
	char aAmount[100];
	int iIndx, i, iTempval;

	byte aRandomNumber[4]		= {0x01, 0x01, 0x01, 0x01};
	byte* pInquireResponse = NULL;	
	byte aMac[4];
    byte aAtref[6];
    byte aTtrefc[4];
	byte aByteAmount[3];

	EditBoxCreditKey.GetWindowText(aCreditKey,100);

	if(strcmp(aCreditKey, "") == 0)
	{
		MessageBox("Please key-in numeric value for Credit Key.", "Error", MB_ICONERROR);
		EditBoxCreditKey.SetFocus();
		return;
	}

	if (RadioButtonDes.GetCheck() == 1)
	{				
		if (strlen(aCreditKey) < 8) 
		{
			MessageBox("Invalid input length. Length must be 8.", "Error", MB_ICONERROR);
			EditBoxCreditKey.SetFocus();
			return;
		}

		for (iIndx = 0; iIndx < 8; iIndx++)
		{
			uCreditKeyDES[iIndx] = aCreditKey[iIndx];
		}
	}
	else
	{
		if (strlen(aCreditKey) < 16) 
		{
			MessageBox("Invalid input length. Length must be 16.", "Error", MB_ICONERROR);
			EditBoxCreditKey.SetFocus();
			return;
		}

		for (iIndx = 0; iIndx < 16; iIndx++)
		{
			uCreditKey3DES[iIndx] = aCreditKey[iIndx];
		}
	}

	EditBoxAmount.GetWindowText(aAmount, 100);

	if(strcmp(aAmount, "") == 0)
	{
		MessageBox("Please key-in numeric value for Amount.", "Error", MB_ICONERROR);
		EditBoxAmount.SetFocus();
		return;
	}

	iTempval = atoi(aAmount);

	if(iTempval < 1 || iTempval > 16777215)
	{
		MessageBox("Please key-in valid Amount. Valid value: 1 - 16,777,215.", "Error", MB_ICONERROR);
		EditBoxAmount.SetFocus();
		return;
	}

	uCreditAmount = atol(aAmount);

	try
		{
		// Issue INQUIRE ACCOUNT command using any arbitrary data and Certify key
		// Arbitrary data is 01 01 01 01
		
		pInquireResponse = pAcos3_->inquireAccount(0x02, aRandomNumber);


		for (i = 0; i < 3; i++)
			aMac[i] = pInquireResponse[i];
		for (i = 0; i < 6; i++)
			aAtref[i] = pInquireResponse[i+8];
		for (i = 0; i < 4; i++)
			aTtrefc[i] = pInquireResponse[i+17];

		//Prepare MAC data block: E2 + AMT + TTREFc + ATREF + 00 + 00
		//    use uTmpArray as the data block
		
		// initialize tempArray
		for(int i = 0; i < 32; i++)
			uTmpArray[i] = 0x00;

		uTempAmount = uCreditAmount / 256;
		uTempAmount = uTempAmount / 256;
		uTempAmount = uTempAmount % 256;
		aByteAmount[0] = (unsigned char)uTempAmount;
		
		uTempAmount = uCreditAmount / 256;
		aByteAmount[1] = (unsigned char)(uTempAmount % 256);
		aByteAmount[2] = (unsigned char)uCreditAmount % 256;

		uTmpArray[0] = 0xE2;	
		for(i = 0; i < 3; i++)
			uTmpArray[i + 1] = aByteAmount[i];
		for (i = 0; i < 4; i++)
			uTmpArray[i + 4] = aTtrefc[i];
		
		iTempval = ((aAtref[4] * 256) + aAtref[5]) + 1;
		aAtref[4] = iTempval / 256;
		aAtref[5] = iTempval % 256;

		for (iIndx = 0;iIndx < 6;iIndx++)
		{
			uTmpArray[iIndx + 8] = aAtref[iIndx];
		}
			
		//uTmpArray[13] = uTmpArray[13] + 1;
		uTmpArray[14] = 0x00;
		uTmpArray[15] = 0x00;

		//Generate applicable MAC values, MAC result will be stored in uTmpArray
		if ((RadioButtonDes.GetCheck() == 1) )
		{
			for (iIndx = 0;iIndx < 8;iIndx++)
			{
				uCreditKey[iIndx] = uCreditKeyDES[iIndx];			
			}
			//MAC PROCESS

			pAcos3_->Acos3DES (uTmpArray, uCreditKey, 1); //Encryption Process
			for (iIndx = 0; iIndx < 8; iIndx++)
			{
				uTmpArray[iIndx] = uTmpArray[iIndx] ^= uTmpArray[iIndx + 8];
			}
			pAcos3_->Acos3DES (uTmpArray, uCreditKey, 1); //Encryption Process		
				
		}
		else
		{
			for (iIndx = 0;iIndx < 16;iIndx++)
			{
				uCreditKey[iIndx] = uCreditKey3DES[iIndx];			
			}
			//3MAC PROCESS

			pAcos3_->Acos3DES3 (uTmpArray, uCreditKey, 1); //Encryption Process
			for (iIndx = 0;iIndx < 8;iIndx++)
			{
				uTmpArray[iIndx] = uTmpArray[iIndx] ^= uTmpArray[iIndx + 8];
			}
			pAcos3_->Acos3DES3 (uTmpArray, uCreditKey, 1); //Encryption Process
		}

		for (iIndx = 0;iIndx < 4;iIndx++)
		{
			aMac[iIndx] = uTmpArray[iIndx];
		}

		
		pAcos3_->credit(aMac, aByteAmount, aTtrefc);

		displayMessage("Credit transaction completed");
		delete [] pInquireResponse;

	}
	catch (CardException cardException)
	{
		if (pInquireResponse!= NULL)
		{
			delete [] pInquireResponse;
		}

		onCardException(cardException);
	}
	catch (PcscException pcscException)
	{
		if (pInquireResponse!= NULL)
		{
			delete [] pInquireResponse;
		}

		onPcscException(pcscException);
	}
}

void CACOSSampleCodesDlg::OnButtonDebit() 
{
	unsigned char uDebitKey[16];
	unsigned char uDebitKey3DES [16];
	unsigned char uDebitKeyDES [8];
	unsigned char uTmpArray[32];
	unsigned long uDebitAmount;
	unsigned long uTempAmount;

	char aDebitKey[100], aAmount[100], aStrDebitResponse[100], aDebitCert[100];
	int iIndx, i, iTempval;

	byte uP1 = 0x00;
	byte aRandomNumber[4]		= {0x01, 0x01, 0x01, 0x01};
	byte* pDebitResponse;
	byte* pInquireResponse;	
	byte aMac[4];
    byte aAtref[6];
	byte aTtrefd[4];
	byte aByteAmount[3];

	EditBoxDebitKey.GetWindowText(aDebitKey,100);

	if(strcmp(aDebitKey, "") == 0)
	{
		MessageBox("Please key-in numeric value for Debit Key.", "Error", MB_ICONERROR);
		EditBoxDebitKey.SetFocus();
		return;
	}

	if (RadioButtonDes.GetCheck() == 1)
	{		
		if (strlen(aDebitKey) < 8) 
		{
			MessageBox("Invalid input length. Length must be 8.", "Error", MB_ICONERROR);
			EditBoxDebitKey.SetFocus();
			return;
		}

		for (iIndx = 0; iIndx < 8; iIndx++)
		{
			uDebitKeyDES[iIndx] = aDebitKey[iIndx];
		}
	}
	else
	{
		if (strlen(aDebitKey) < 16) 
		{
			MessageBox("Invalid input length. Length must be 16.", "Error", MB_ICONERROR);
			EditBoxDebitKey.SetFocus();
			return;
		}

		for (iIndx = 0; iIndx < 16; iIndx++)
		{
			uDebitKey3DES[iIndx] = aDebitKey[iIndx];
		}
	}

	EditBoxAmount.GetWindowText(aAmount, 100);
	
	if(strcmp(aAmount, "") == 0)
	{
		MessageBox("Please key-in numeric value for Amount.", "Error", MB_ICONERROR);
		EditBoxAmount.SetFocus();
		return;
	}

	iTempval = atoi(aAmount);

	if(iTempval > 16777215 || iTempval < 1)
	{
		MessageBox("Please key-in valid Amount. Valid value: 1 - 16,777,215.", "Error", MB_ICONERROR);
		EditBoxAmount.SetFocus();
		return;
	}

	uDebitAmount = atol(aAmount);

	try
	{
		// Issue INQUIRE ACCOUNT command using any arbitrary data and Certify key
		// Arbitrary data is 01 01 01 01		
		
		pInquireResponse = pAcos3_->inquireAccount(0x02, aRandomNumber);

		if (pInquireResponse == NULL)
		{
			MessageBox("No Inquire Response Received.", "Error", MB_ICONERROR);
			return;
		}

		for (i = 0; i < 3; i++)
			aMac[i] = pInquireResponse[i];		
		for (i = 0; i < 6; i++)
			aAtref[i] = pInquireResponse[i + 8];
		for (i = 0; i < 4; i++)
			aTtrefd[i] = pInquireResponse[i + 21];

		delete [] pInquireResponse;		

		uTempAmount = uDebitAmount / 256;
		uTempAmount = uTempAmount / 256;
		uTempAmount = uTempAmount % 256;
		aByteAmount[0] = (unsigned char)uTempAmount;
		
		uTempAmount = uDebitAmount / 256;
		aByteAmount[1] = (unsigned char)(uTempAmount % 256);
		aByteAmount[2] = (unsigned char)uDebitAmount % 256;

		// initialize tempArray
		for(int i = 0; i < 32; i++)
			uTmpArray[i] = 0x00;

		uTmpArray[0] = 0xE6;	
		for(i = 0; i < 3; i++)
			uTmpArray[i + 1] = aByteAmount[i];
		for (i = 0; i < 4; i++)
			uTmpArray[i + 4] = aTtrefd[i];
		
		iTempval = ((aAtref[4] * 256) + aAtref[5]) + 1;
		aAtref[4] = iTempval / 256;
		aAtref[5] = iTempval % 256;

		for (iIndx = 0;iIndx < 6;iIndx++)
		{
			uTmpArray[iIndx + 8] = aAtref[iIndx];
		}
			
		uTmpArray[14] = 0x00;
		uTmpArray[15] = 0x00;

			
		//Generate applicable aMac values, MAC result will be stored in uTmpArray
		if ((RadioButtonDes.GetCheck() == 1) )
		{
			for (iIndx = 0;iIndx < 8;iIndx++)
			{
				uDebitKey[iIndx] = uDebitKeyDES[iIndx];				
			}
			//MAC PROCESS

			pAcos3_->Acos3DES (uTmpArray, uDebitKey, 1); //Encryption Process
			for (iIndx = 0;iIndx < 8;iIndx++)
			{
				uTmpArray[iIndx] = uTmpArray[iIndx] ^= uTmpArray[iIndx + 8];
			}
			pAcos3_->Acos3DES (uTmpArray, uDebitKey, 1); //Encryption Process			
		}
		else
		{
			for (iIndx = 0;iIndx < 16;iIndx++)
			{
				uDebitKey[iIndx] = uDebitKey3DES[iIndx];			
			}
			//3MAC PROCESS

			pAcos3_->Acos3DES3 (uTmpArray, uDebitKey, 1); //Encryption Process
			for (iIndx = 0;iIndx < 8;iIndx++)
			{
				uTmpArray[iIndx] = uTmpArray[iIndx] ^= uTmpArray[iIndx + 8];
			}
			pAcos3_->Acos3DES3 (uTmpArray, uDebitKey, 1); //Encryption Process
		}

		for (iIndx = 0;iIndx < 4;iIndx++)
			aMac[iIndx] = uTmpArray[iIndx];

		// Format Debit command data and execute debit command
        // Using uTmpArray, the first four bytes are carried over
		uTempAmount = uDebitAmount / 256;
		uTempAmount = uTempAmount / 256;
		uTempAmount = uTempAmount % 256;
		uTmpArray[4] = (unsigned char) uTempAmount;
		uTempAmount = uDebitAmount / 256;
		uTmpArray[5] = (unsigned char) uTempAmount % 256;
		uTmpArray[6] = (unsigned char) uDebitAmount % 256;

		for (iIndx = 0;iIndx < 6;iIndx++)
			uTmpArray[iIndx + 7] = aAtref[iIndx];

		if(CheckBoxRequireDebit.GetCheck() == 0) //Without Debit Certificate		
			uP1 = 0x00;
		else
			uP1 = 0x01;

		pDebitResponse = pAcos3_->debit(aMac, aByteAmount, aTtrefd, uP1);

		if(CheckBoxRequireDebit.GetCheck() == 1)
		{
			if(pDebitResponse != NULL)
			{
				sprintf(aStrDebitResponse, "%02X %02X %02X %02X", pDebitResponse[0], pDebitResponse[1], pDebitResponse[2], pDebitResponse[3]);				
				sprintf(aDebitCert, "%s %s", "DEBIT CERTIFICATE: ", aStrDebitResponse);

				displayMessage(aDebitCert);
				delete [] pDebitResponse;			
			}
		}

		displayMessage("Debit transaction completed");

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

void CACOSSampleCodesDlg::OnButtonInquireBalance() 
{
	unsigned char uCertifyKey[16];
	unsigned char uCertifyKey3DES[16];
	unsigned char uCertifyKeyDES[8];
	unsigned char uLastTransactionType;
	unsigned char uTmpArray[32];

	char aCertifyKey[100];
	char aStringTransactionType[100];
	char aTransTypeToDisplay[100];
	int iTmpBalance;  
	int iIndx;

	byte aRandomNumber[4] = {0x01, 0x01, 0x01, 0x01};
	byte* pInquireResponse;
	byte aAtref[6];	//		 = new byte[5];
    byte aTtrefc[4];//		 = new byte[4];
	byte aTtrefd[4];//		 = new byte[4];

	EditBoxCertifyKey.GetWindowText(aCertifyKey,100);

	if (strcmp(aCertifyKey, "") == 0)
	{
		MessageBox("Please key-in numeric value for Certify Key.", "Error", MB_ICONERROR);
		EditBoxCertifyKey.SetFocus();
		return;
	}

	if (RadioButtonDes.GetCheck() == 1)
	{		
		if (strlen(aCertifyKey) < 8) 
		{
			MessageBox("Invalid input length. Length must be 8.", "Error", MB_ICONERROR);
			EditBoxCertifyKey.SetFocus();
			return;
		}

		for (iIndx = 0; iIndx < 8; iIndx++)
		{
			uCertifyKeyDES[iIndx] = aCertifyKey[iIndx];
		}
	}
	else
	{
		if (strlen(aCertifyKey) < 16) 
		{
			MessageBox("Invalid input length. Length must be 16.", "Error", MB_ICONERROR);
			EditBoxCertifyKey.SetFocus();
			return;
		}

		for (iIndx = 0; iIndx < 16; iIndx++)
		{
			uCertifyKey3DES[iIndx] = aCertifyKey[iIndx];
		}
	}	

	try
	{
		pInquireResponse = pAcos3_->inquireAccount(0x02,aRandomNumber);		

		if (pInquireResponse == NULL)
		{			
			MessageBox("Inquire account no inquire response returned.", "Error", MB_ICONERROR);
			return;
		}		

		//Check integrity of data returned by card.
		//Build MAC input data. Extract the info from ACOS card in Dataout.
		uLastTransactionType = pInquireResponse[4];
		iTmpBalance = (pInquireResponse[5] * 256 * 256) + (pInquireResponse[6] * 256) + (pInquireResponse[7]);	

		for (iIndx = 0; iIndx < 4; iIndx++)
			aTtrefc[iIndx] = pInquireResponse[iIndx + 17];

		for (iIndx = 0; iIndx < 4; iIndx++)
			aTtrefd[iIndx] = pInquireResponse[iIndx + 21];

		for (iIndx = 0; iIndx < 6; iIndx++)
			aAtref[iIndx] = pInquireResponse[iIndx + 8];

		//Move data from ACOS card as input to MAC calculations

		//Arbitrary Data
		for (iIndx = 0;iIndx < 4;iIndx++)
			uTmpArray[iIndx] = aRandomNumber[iIndx];

		uTmpArray[4] = pInquireResponse[4];          // 4 BYTE MAC + LAST TRANS TYPE
		
		for (iIndx = 0;iIndx < 3;iIndx++)
		{
			uTmpArray[iIndx + 5] = pInquireResponse[iIndx + 5];
		}
			
		for (iIndx = 0;iIndx < 6;iIndx++)
		{
			uTmpArray[iIndx + 8] = pInquireResponse[iIndx + 8];
		}

		//Pad 2 bytes of zero value according to formula.
		uTmpArray[14] = 0x00;
		uTmpArray[15] = 0x00;

		//Copy aTtrefc
		for (iIndx = 0;iIndx < 4;iIndx++)
			uTmpArray[iIndx + 16] = aTtrefc[iIndx];

		//Copy aTtrefd
		for (iIndx = 0;iIndx < 4;iIndx++)
			uTmpArray[iIndx + 20] = aTtrefd[iIndx];

		//Generate applicable MAC values		
		if ((RadioButtonDes.GetCheck() == 1) )
		{
			for (iIndx = 0;iIndx < 8;iIndx++)
				uCertifyKey[iIndx] = uCertifyKeyDES[iIndx];			

			//MAC PROCESS
			pAcos3_->Acos3DES (uTmpArray, uCertifyKey, 1); //Encryption Process
			for (iIndx = 0;iIndx < 8;iIndx++)
				uTmpArray[iIndx] = uTmpArray[iIndx] ^= uTmpArray[iIndx + 8];

			pAcos3_->Acos3DES (uTmpArray, uCertifyKey, 1); //Encryption Process				
		}
		else
		{
			for (iIndx = 0;iIndx < 16;iIndx++)
			{
				uCertifyKey[iIndx] = uCertifyKey3DES[iIndx];			
			}
			//MAC PROCESS

			pAcos3_->Acos3DES3 (uTmpArray, uCertifyKey, 1); //Encryption Process
			for (iIndx = 0;iIndx < 8;iIndx++)
			{
				uTmpArray[iIndx] = uTmpArray[iIndx] ^= uTmpArray[iIndx + 8];
			}
			pAcos3_->Acos3DES3 (uTmpArray, uCertifyKey, 1); //Encryption Process
		}

		//Compare MAC values
		for (iIndx = 0;iIndx < 4;iIndx++)
		{
			if (uTmpArray[iIndx] != pInquireResponse[iIndx])
			{	
				MessageBox("MAC is incorrect, data integrity is jeopardized.", "Error", MB_ICONERROR);
				displayMessage("MAC is incorrect, data integrity is jeopardized.");
				delete [] pInquireResponse;
				return;
			}            
		}

		switch (uLastTransactionType)
		{
			case 0x01:
				strcpy(aStringTransactionType,"DEBIT");
				break;
			case 0x02:
				strcpy(aStringTransactionType,"REVOKE DEBIT");
				break;
			case 0x03:
				strcpy(aStringTransactionType,"CREDIT");
				break;
			default:
				strcpy(aStringTransactionType,"NOT DEFINED");
				break;
		}	

		sprintf(aTransTypeToDisplay, "%s %s", "Last transaction is ", aStringTransactionType);
		displayMessage(aTransTypeToDisplay);
			
		sprintf(aTransTypeToDisplay,"%s %ld ","Amount is ",iTmpBalance);
		displayMessage(aTransTypeToDisplay);

		sprintf(aTransTypeToDisplay, "%ld ", iTmpBalance);		
		EditBoxAmount.SetWindowText(aTransTypeToDisplay);
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

void CACOSSampleCodesDlg::OnButtonRevokeDebit() 
{
	unsigned char uRevokeDebitKey[16];
	unsigned char uRevokeDebitKey3DES [16];
	unsigned char uRevokeDebitKeyDES [8];
	unsigned char uTmpArray[32];
	unsigned long uDebitAmount;
	unsigned long uTempAmount;

	char aRevokeDebitKey[100], aAmount[100];
	int iIndx, i, iTempval;

	byte aRandomNumber[4]		= {0x01, 0x01, 0x01, 0x01};
	byte* pInquireResponse;	
	byte aMac[4];		//			= new byte[4];    
    byte aAtref[6];		//			= new byte[6];
	byte aTtrefd[4];		//		= new byte[4];
	byte aByteAmount[3];//			= new byte[3];

	EditBoxRevokeDebit.GetWindowText(aRevokeDebitKey,100);

	if (strcmp(aRevokeDebitKey, "") == 0)
	{
		MessageBox("Please key-in numeric value for Revoke Debit Key.", "Error", MB_ICONERROR);
		EditBoxRevokeDebit.SetFocus();
		return;
	}

	if (RadioButtonDes.GetCheck() == 1)
	{
		if (strlen(aRevokeDebitKey) < 8) 
		{
			MessageBox("Invalid input length. Length must be 8.", "Error", MB_ICONERROR);
			EditBoxRevokeDebit.SetFocus();
			return;
		}

		for (iIndx = 0; iIndx < 8; iIndx++)
		{
			uRevokeDebitKeyDES[iIndx] = aRevokeDebitKey[iIndx];
		}
	}
	else
	{
		if (strlen(aRevokeDebitKey) < 16) 
		{
			MessageBox("Invalid input length. Length must be 16.", "Error", MB_ICONERROR);
			EditBoxRevokeDebit.SetFocus();
			return;
		}

		for (iIndx = 0; iIndx < 16; iIndx++)
		{
			uRevokeDebitKey3DES[iIndx] = aRevokeDebitKey[iIndx];
		}
	}

	EditBoxAmount.GetWindowText(aAmount, 100);
	
	if(strcmp(aAmount, "") == 0)
	{
		MessageBox("Please key-in numeric value for Amount.", "Error", MB_ICONERROR);
		EditBoxAmount.SetFocus();
		return;
	}

	iTempval = atoi(aAmount);

	if(iTempval > 16777215 || iTempval < 1)
	{
		MessageBox("Please key-in valid Amount. Valid value: 1 - 16,777,215.","Error", MB_ICONERROR);
		EditBoxAmount.SetFocus();
		return;
	}

	uDebitAmount = atol(aAmount);

	try
	{
		// Issue INQUIRE ACCOUNT command using any arbitrary data and Certify key
		// Arbitrary data is 01 01 01 01
		
		pInquireResponse = pAcos3_->inquireAccount(0x02, aRandomNumber);
		
		for (i = 0; i < 6; i++)
			aAtref[i] = pInquireResponse[i + 8];
		for (i = 0; i < 4; i++)
			aTtrefd[i] = pInquireResponse[i + 21];

		delete [] pInquireResponse;

		uTempAmount = uDebitAmount / 256;
		uTempAmount = uTempAmount / 256;
		uTempAmount = uTempAmount % 256;
		aByteAmount[0] = (unsigned char)uTempAmount;
		
		uTempAmount = uDebitAmount / 256;
		aByteAmount[1] = (unsigned char)(uTempAmount % 256);
		aByteAmount[2] = (unsigned char)uDebitAmount % 256;

		// initialize tempArray
		for(int i = 0; i < 32; i++)
			uTmpArray[i] = 0x00;

		uTmpArray[0] = 0xE8;	
		for(i = 0; i < 3; i++)
			uTmpArray[i + 1] = aByteAmount[i];
		for (i = 0; i < 4; i++)
			uTmpArray[i + 4] = aTtrefd[i];
		
		iTempval = ((aAtref[4] * 256) + aAtref[5]) + 1;
		aAtref[4] = iTempval / 256;
		aAtref[5] = iTempval % 256;

		for (iIndx = 0;iIndx < 6;iIndx++)
		{
			uTmpArray[iIndx + 8] = aAtref[iIndx];
		}
			
		//uTmpArray[13] = uTmpArray[13] + 1;
		uTmpArray[14] = 0x00;
		uTmpArray[15] = 0x00;

		//Generate applicable MAC values, MAC result will be stored in uTmpArray
		if ((RadioButtonDes.GetCheck() == 1) )
		{
			for (iIndx = 0;iIndx < 8;iIndx++)
			{
				uRevokeDebitKey[iIndx] = uRevokeDebitKeyDES[iIndx];				
			}
			//MAC PROCESS

			pAcos3_->Acos3DES (uTmpArray, uRevokeDebitKey, 1); //Encryption Process
			for (iIndx = 0;iIndx < 8;iIndx++)
			{
				uTmpArray[iIndx] = uTmpArray[iIndx] ^= uTmpArray[iIndx + 8];
			}
			pAcos3_->Acos3DES (uTmpArray, uRevokeDebitKey, 1); //Encryption Process			
		}
		else
		{
			for (iIndx = 0;iIndx < 16;iIndx++)
			{
				uRevokeDebitKey[iIndx] = uRevokeDebitKey3DES[iIndx];			
			}
			//3MAC PROCESS

			pAcos3_->Acos3DES3 (uTmpArray, uRevokeDebitKey, 1); //Encryption Process
			for (iIndx = 0;iIndx < 8;iIndx++)
			{
				uTmpArray[iIndx] = uTmpArray[iIndx] ^= uTmpArray[iIndx + 8];
			}
			pAcos3_->Acos3DES3 (uTmpArray, uRevokeDebitKey, 1); //Encryption Process
		}

		for (iIndx = 0;iIndx < 4;iIndx++)
			aMac[iIndx] = uTmpArray[iIndx];

		// Execute Revoke Debit command data and execute credit command
        // Using uTmpArray, the first four bytes storing the MAC value are carried over
		pAcos3_->revokeDebit(aMac);

		displayMessage("Revoke Debit transaction completed");
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
	EditBoxLogs.SetWindowText("");
	EditBoxLogsValue.Empty();
	
}

void CACOSSampleCodesDlg::OnButtonReset() 
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

	enableControls(false);
	RadioButtonDes.SetCheck(1);
	RadioButton3Des.SetCheck(0);
	EditBoxLogs.SetWindowText("");
	EditBoxCreditKey.SetWindowText("");
	EditBoxDebitKey.SetWindowText("");
	EditBoxCertifyKey.SetWindowText("");
	EditBoxRevokeDebit.SetWindowText("");
	EditBoxAmount.SetWindowText("");
	ComboBoxReaderNames.ResetContent();
	CheckBoxRequireDebit.SetCheck(false);
	
	EditBoxLogsValue = "Program Ready\r\n";
	UpdateData(false);
}

void CACOSSampleCodesDlg::OnButtonQuit() 
{
	this->EndDialog(0);
	
}

void CACOSSampleCodesDlg::OnRadioButtonDes() 
{
	EditBoxCreditKey.SetWindowText("");
	EditBoxDebitKey.SetWindowText("");
	EditBoxCertifyKey.SetWindowText("");
	EditBoxRevokeDebit.SetWindowText("");
	RadioButtonDes.SetCheck(1);
	RadioButton3Des.SetCheck(0);

	if (RadioButtonDes.GetCheck() == 1)
	{
		EditBoxCreditKey.SetLimitText(8);
		EditBoxDebitKey.SetLimitText(8);
		EditBoxCertifyKey.SetLimitText(8);
		EditBoxRevokeDebit.SetLimitText(8);
	}
	else
	{
		EditBoxCreditKey.SetLimitText(16);
		EditBoxDebitKey.SetLimitText(16);
		EditBoxCertifyKey.SetLimitText(16);
		EditBoxRevokeDebit.SetLimitText(16);
	}
}

void CACOSSampleCodesDlg::OnRadioButton3Des() 
{
	EditBoxCreditKey.SetWindowText("");
	EditBoxDebitKey.SetWindowText("");
	EditBoxCertifyKey.SetWindowText("");
	EditBoxRevokeDebit.SetWindowText("");
	RadioButtonDes.SetCheck(0);
	RadioButton3Des.SetCheck(1);

	if (RadioButtonDes.GetCheck() == 1)
	{

		EditBoxCreditKey.SetLimitText(8);
		EditBoxDebitKey.SetLimitText(8);
		EditBoxCertifyKey.SetLimitText(8);
		EditBoxRevokeDebit.SetLimitText(8);
	}
	else
	{
		EditBoxCreditKey.SetLimitText(16);
		EditBoxDebitKey.SetLimitText(16);
		EditBoxCertifyKey.SetLimitText(16);
		EditBoxRevokeDebit.SetLimitText(16);
	}
	
}
