// NdefVCard.cpp : implementation file
//

#include "stdafx.h"
#include "MifareNDEFSampleCode.h"
#include "NdefVCard.h"
#include <cctype>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNdefVCard dialog


CNdefVCard::CNdefVCard(CWnd* pParent /*=NULL*/)
	: CDialog(CNdefVCard::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNdefVCard)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CNdefVCard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNdefVCard)
	DDX_Control(pDX, IDC_STATICVCARDSUR, m_StaticVCardSur);
	DDX_Control(pDX, IDC_STATICVCARDMIDDLE, m_StaticVCardMiddle);
	DDX_Control(pDX, IDC_STATICVCARDGIVEN, m_StaticVCardGiven);
	DDX_Control(pDX, IDC_EDITVCARDSUR, m_EditBoxVCardSur);
	DDX_Control(pDX, IDC_EDITVCARDMIDDLE, m_EditBoxVCardMiddle);
	DDX_Control(pDX, IDC_EDITVCARDGIVEN, m_EditBoxVCardGiven);
	DDX_Control(pDX, IDC_BUTTONENCODEVCARD, m_ButtonEncodeVCard);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNdefVCard, CDialog)
	//{{AFX_MSG_MAP(CNdefVCard)
	ON_BN_CLICKED(IDC_BUTTONENCODEVCARD, OnButtonencodevcard)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNdefVCard message handlers

BOOL CNdefVCard::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	this->pFrmParent_ =  (CMifareNDEFSampleCodeDlg*) this->GetParent();
	
	m_EditBoxVCardGiven.SetLimitText(20);
	m_EditBoxVCardMiddle.SetLimitText(20);
	m_EditBoxVCardSur.SetLimitText(20);

	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNdefVCard::OnButtonencodevcard() 
{
	NdefRecordType* pNdefRecordType = new NdefRecordType(MEDIATYPE, "text/x-vCard");
	NdefRecord*		pNdefRecord = new NdefRecord(pNdefRecordType);
	VCardIdentificationType* pVCardIdentificationType = new VCardIdentificationType();

    vector<byte>	uBuffer;
	char			aGivenName[100];
	char			aMiddleName[100];
	char			aLastName[100];

	CString	sFormattedString = "BEGIN:VCARD\r\nVERSION:3.0";
	CString sGivenName;
	CString sMiddleName;
	CString sSurname;

	try
	{
		m_EditBoxVCardGiven.GetWindowText(sGivenName);
		m_EditBoxVCardMiddle.GetWindowText(sMiddleName);
		m_EditBoxVCardSur.GetWindowText(sSurname);

		if(sGivenName.Trim() == "")
		{
			cHelper_.showErrorMessage("Please key-in Given Name.");
			m_EditBoxVCardGiven.SetFocus();
			return;
		}

		if(sSurname.Trim() == "")
		{
			cHelper_.showErrorMessage("Please key-in Surname.");
			m_EditBoxVCardSur.SetFocus();
			return;
		}

		if (!cNdef_.isNameValid(sGivenName))
		{
			cHelper_.showErrorMessage("Please key-in correct Given Name.");
			m_EditBoxVCardGiven.SetFocus();
			return;
		}

		if (!cNdef_.isNameValid(sMiddleName))
		{
			cHelper_.showErrorMessage("Please key-in correct Middle Name.");
			m_EditBoxVCardMiddle.SetFocus();
			return;
		}

		if (!cNdef_.isNameValid(sSurname))
		{
			cHelper_.showErrorMessage("Please key-in correct Surname.");
			m_EditBoxVCardSur.SetFocus();
			return;
		}

		m_EditBoxVCardGiven.GetWindowText(aGivenName, m_EditBoxVCardGiven.GetWindowTextLength() + 1);
		m_EditBoxVCardMiddle.GetWindowText(aMiddleName, m_EditBoxVCardMiddle.GetWindowTextLength() + 1);
		m_EditBoxVCardSur.GetWindowText(aLastName, m_EditBoxVCardSur.GetWindowTextLength() + 1);

		pVCardIdentificationType->setGivenName(CString(aGivenName, strlen(aGivenName)));
		pVCardIdentificationType->setAdditionalName(CString(aMiddleName, strlen(aMiddleName)));
		pVCardIdentificationType->setFamilyName(CString(aLastName, strlen(aLastName)));

		sFormattedString += "\r\n" + pVCardIdentificationType->getFormattedString();
		sFormattedString += "\r\nEND:VCARD\r\n\r\n";

		uBuffer = cHelper_.asciiToVector(sFormattedString);

		pNdefRecord->setMessageBegin(true);
		pNdefRecord->setMessageEnd(true);
		pNdefRecord->appendPayload(uBuffer);
		
		uBuffer = pNdefRecord->encodeToNdef();

		pFrmParent_->m_EditBoxNdefMessage.SetWindowText(cHelper_.vectorToString(uBuffer));
		pFrmParent_->addTitleToLog(sFormattedString);
		pFrmParent_->uSnepMessage.clear();
		pFrmParent_->uSnepMessage = uBuffer;
	}
	catch (NdefException ex)
	{
		pFrmParent_->addErrMsgToLog(ex.what());
		cHelper_.showErrorMessage(ex.what());
	}
	catch (...)
	{
		pFrmParent_->addErrMsgToLog("System Exception");
	}

	delete pNdefRecordType;
	delete pNdefRecord;
	delete pVCardIdentificationType;
	
}
