// NdefVCardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NtagNDEFSampleCode.h"
#include "NdefVCardDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NdefVCardDlg dialog

void showErrorMessage(CString sMessage);

NdefVCardDlg::NdefVCardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(NdefVCardDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(NdefVCardDlg)
	//}}AFX_DATA_INIT
}


void NdefVCardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NdefVCardDlg)
	DDX_Control(pDX, IDC_EditSurname, m_EditSurname);
	DDX_Control(pDX, IDC_EditMiddleName, m_EditMiddleName);
	DDX_Control(pDX, IDC_EditGivenName, m_EditGivenName);
	DDX_Control(pDX, IDC_ButtonEncodeVCard, m_ButtonEncodeVCard);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NdefVCardDlg, CDialog)
	//{{AFX_MSG_MAP(NdefVCardDlg)
	ON_BN_CLICKED(IDC_ButtonEncodeVCard, OnButtonEncodeVCard)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NdefVCardDlg message handlers
BOOL NdefVCardDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	this->pFormParent_ = (NtagNDEFSampleCodeDlg*) this->GetParent();
	
	m_EditSurname.SetLimitText(20);
	m_EditMiddleName.SetLimitText(20);
	m_EditGivenName.SetLimitText(20);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void NdefVCardDlg::OnButtonEncodeVCard() 
{	
	CString tempString;

	if(m_EditGivenName.GetWindowTextLength() == 0)
	{
		showErrorMessage("Please key-in Given Name.");
		m_EditGivenName.SetFocus();
		return;
	}
	else
	{
		m_EditGivenName.GetWindowText(tempString);

		tempString.TrimLeft();
		tempString.TrimRight();
		
		if(tempString.Compare("") == 0)
		{
			showErrorMessage("Please key-in Given Name.");
			m_EditGivenName.SetFocus();
			return;
		}

		if(!validateCharacters(tempString))
		{
			showErrorMessage("Please key-in correct Given Name.");
			m_EditGivenName.SetFocus();
			return;
		}
	}

	
	if(m_EditMiddleName.GetWindowTextLength() != 0)
	{
		m_EditMiddleName.GetWindowText(tempString); 

		tempString.TrimLeft();
		tempString.TrimRight();
		
		if(tempString.Compare("") != 0)
		{
			if(!validateCharacters(tempString))
			{
				showErrorMessage("Please key-in correct Middle Name.");
				m_EditMiddleName.SetFocus();
				return;
			}
		}		
	}

	if(m_EditSurname.GetWindowTextLength() == 0)
	{
		showErrorMessage("Please key-in Surname.");
		m_EditSurname.SetFocus();
		return;
	}
	else
	{
		m_EditSurname.GetWindowText(tempString);
		
		tempString.TrimLeft();
		tempString.TrimRight();
		
		if(tempString.Compare("") == 0)
		{
			showErrorMessage("Please key-in Surname.");
			m_EditSurname.SetFocus();
			return;
		}

		if(!validateCharacters(tempString))
		{
			showErrorMessage("Please key-in correct Surname.");
			m_EditSurname.SetFocus();
			return;
		}
	}

	encodeVCard();	
}

void NdefVCardDlg::encodeVCard()
{
    NdefRecord*		pNdefRecord;
    NdefMessage*	pNdefMessage = new NdefMessage();
	VCardIdentificationType* pIdentificationType;
	NdefRecordType* pNdefRecordType;
	vector<byte>	uBuffer;
	CString			sFormattedString = "\r\nBEGIN:VCARD\r\nVERSION:3.0";
	char			aTempString[21];
	char aMessage[100];
	int iMaximumInputLength = 141;

	//Set Identification Type
	pIdentificationType = new VCardIdentificationType();
	m_EditGivenName.GetWindowText(aTempString, m_EditGivenName.GetWindowTextLength() + 1);
	pIdentificationType->setGivenName(CString(aTempString, strlen(aTempString)));
	
	m_EditSurname.GetWindowText(aTempString, m_EditSurname.GetWindowTextLength() + 1);
	pIdentificationType->setFamilyName(CString(aTempString, strlen(aTempString)));

	m_EditMiddleName.GetWindowText(aTempString, m_EditMiddleName.GetWindowTextLength() + 1);
	pIdentificationType->setAdditionalName(CString(aTempString, strlen(aTempString)));

	sFormattedString += "\r\n" + pIdentificationType->getFormattedString();
	sFormattedString += "\r\nEND:VCARD\r\n\r\n";

	pNdefRecordType = new NdefRecordType(MEDIATYPE, "text/x-vCard");
	pNdefRecord = new NdefRecord(pNdefRecordType);
	pNdefRecord->setMessageBegin(true);
	pNdefRecord->setMessageEnd(true);
	pNdefRecord->appendPayload(sFormattedString);

	pNdefMessage->appendRecord(*(pNdefRecord));

	uBuffer = pNdefMessage->toByteArray();

    if (uBuffer.size() > iMaximumInputLength)
    {
		sprintf(aMessage, "Input characters exceeds maximum input length. Please remove %d characters from URI.", uBuffer.size() - iMaximumInputLength);
        MessageBox(aMessage, "NTAG NDEF Sample Code", MB_OK|MB_ICONSTOP);
		return;
    }

	delete pNdefRecord;
	delete pNdefMessage;
	delete pNdefRecordType;

	pFormParent_->m_EditNdefMessage.SetWindowText(cHelper_.vectorToString(uBuffer));
	pFormParent_->addMessageToLogs(sFormattedString);
	pFormParent_->uSnepMessage_.clear();
	pFormParent_->uSnepMessage_ = uBuffer;
}

bool NdefVCardDlg::validateCharacters(CString testString)
{
	char c;

	for (int i = 0; i < testString.GetLength(); i++) {

		c = testString[i]; 

		if (!((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c== ' ' || c == '-' || c == '.')) 
		{
			 return false;
		}
	}

	return true;
}