// NdefTextDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NtagNDEFSampleCode.h"
#include "NdefTextDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

	//global functions
void showErrorMessage(CString sMessage);

/////////////////////////////////////////////////////////////////////////////
// NdefTextDlg dialog


NdefTextDlg::NdefTextDlg(CWnd* pParent /*=NULL*/)
	: CDialog(NdefTextDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(NdefTextDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void NdefTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NdefTextDlg)
	DDX_Control(pDX, IDC_RadioUtf8, m_RadioButtonUtf8);
	DDX_Control(pDX, IDC_RadioUtf16, m_RadioButtonUtf16);
	DDX_Control(pDX, IDC_EditMessage, m_EditMessage);
	DDX_Control(pDX, IDC_ComboBoxLanguage, m_ComboBoxLanguage);
	DDX_Control(pDX, IDC_ButtonEncodeText, m_ButtonEncodeText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NdefTextDlg, CDialog)
	//{{AFX_MSG_MAP(NdefTextDlg)
	ON_CBN_SELCHANGE(IDC_ComboBoxLanguage, OnSelchangeComboBoxLanguage)
	ON_BN_CLICKED(IDC_ButtonEncodeText, OnButtonEncodeText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NdefTextDlg message handlers

BOOL NdefTextDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	this->pFormParent_ = (NtagNDEFSampleCodeDlg*) this->GetParent();
	
	// TODO: Add extra initialization here

	m_EditMessage.SetLimitText(134);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void NdefTextDlg::OnSelchangeComboBoxLanguage() 
{
	string sMessage;
	char aMessage[1024];
	int iLimit;

	if(m_ComboBoxLanguage.GetCurSel() == 1)
	{
		iLimit = 131;
	}
	else
	{
		iLimit = 134;
	}

	m_EditMessage.GetWindowText(aMessage, m_EditMessage.GetWindowTextLength() + 1);

	sMessage = string(aMessage);

	m_EditMessage.SetLimitText(iLimit);

	if(sMessage != "" && sMessage.length() > iLimit)
	{
		sMessage.resize(iLimit);
		m_EditMessage.SetWindowText(sMessage.c_str());
	}
}

void NdefTextDlg::OnButtonEncodeText() 
{
	CString tempString;

	if(m_EditMessage.GetWindowTextLength() == 0)
	{
		showErrorMessage("Please key-in Message.");
		m_EditMessage.SetFocus();
		return;
	}
	
	m_EditMessage.GetWindowText(tempString);
	tempString.TrimLeft();
	tempString.TrimRight();

	if(tempString.Compare("") == 0)
	{
		showErrorMessage("Please key-in Message.");
		m_EditMessage.SetFocus();
		return;
	}

	encodeToNdefMessage();
}

void NdefTextDlg::encodeToNdefMessage()
{
	NdefRecord*		pTempRecord;
    NdefMessage*	pNdefMessage = new NdefMessage();
	NdefRecordType* pNdefRecordType;
	vector<byte>	uBuffer;
	bool			bIsUseUTF8 = true;
	byte			uStatusByte = 0x00;
	char			aMessageShow[100];
	char			aMessage[142];
	CString			sMessage;
	CString			sLanguage;
	int iMaximumInputLength = 141;

    // Encode Message
	m_ComboBoxLanguage.GetWindowText(sLanguage);
	m_EditMessage.GetWindowText(aMessage, m_EditMessage.GetWindowTextLength() + 1);

	sMessage = CString(aMessage, strlen(aMessage));
	sMessage.TrimLeft();
	sMessage.TrimRight();
    if (sMessage == "")
		throw NdefException("Text is empty");

    // If UTF 8  set bit 7 to 0
    // If UTF 16 set bit 7 to 1
    if(!bIsUseUTF8)
        uStatusByte = (byte) 0x80;
       
    // Length of the Language (ISO/IANA language code)
    // ex. "en-US"

    uStatusByte |= (byte)sLanguage.GetLength();

	pNdefRecordType = new NdefRecordType(NFC_FORM_WELL_KNOWN_TYPE, "T");
    pTempRecord = new NdefRecord(pNdefRecordType);
    pTempRecord->setMessageBegin(true);
	pTempRecord->setMessageEnd(true);
    pTempRecord->appendPayload(uStatusByte);
    pTempRecord->appendPayload(sLanguage);
    pTempRecord->appendPayload(sMessage);

    pNdefMessage->appendRecord(*(pTempRecord));

    uBuffer = pNdefMessage->toByteArray();

    if (uBuffer.size() > iMaximumInputLength)
    {
		sprintf(aMessageShow, "Input characters exceeds maximum input length. Please remove %d characters from URI.", uBuffer.size() - iMaximumInputLength);
        MessageBox(aMessageShow, "NTAG NDEF Sample Code", MB_OK|MB_ICONSTOP);
		return;
    }

	delete pTempRecord;
	delete pNdefMessage;
	delete pNdefRecordType;

	pFormParent_->m_EditNdefMessage.SetWindowText(cHelper_.vectorToString(uBuffer));
	pFormParent_->uSnepMessage_.clear();
	pFormParent_->uSnepMessage_ = uBuffer;
}
