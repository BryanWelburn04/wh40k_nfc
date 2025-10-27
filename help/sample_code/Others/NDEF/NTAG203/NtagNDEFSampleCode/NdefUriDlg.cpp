// NdefUriDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NtagNDEFSampleCode.h"
#include "NdefUriDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NdefUriDlg dialog
void showErrorMessage(CString sMessage);

NdefUriDlg::NdefUriDlg(CWnd* pParent /*=NULL*/)
	: CDialog(NdefUriDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(NdefUriDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void NdefUriDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NdefUriDlg)
	DDX_Control(pDX, IDC_EditUriUri, m_EditUriUri);
	DDX_Control(pDX, IDC_ComboBoxUriUri, m_ComboBoxUriUri);
	DDX_Control(pDX, IDC_ButtonEncodeUrl, m_ButtonEncodeUri);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NdefUriDlg, CDialog)
	//{{AFX_MSG_MAP(NdefUriDlg)
	ON_CBN_SELCHANGE(IDC_ComboBoxUriUri, OnSelchangeComboBoxUriUri)
	ON_BN_CLICKED(IDC_ButtonEncodeUrl, OnButtonEncodeUrl)
	ON_EN_KILLFOCUS(IDC_EditUriUri, OnKillfocusEditUriUri)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NdefUriDlg message handlers
BOOL NdefUriDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	this->pFormParent_ = (NtagNDEFSampleCodeDlg*) this->GetParent();
	
	m_EditUriUri.SetLimitText(136);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void NdefUriDlg::OnSelchangeComboBoxUriUri() 
{
	m_EditUriUri.SetWindowText("");	
}

void NdefUriDlg::OnButtonEncodeUrl() 
{
	CString tempString;
	Ndef* pNdef = new Ndef();

	if(m_EditUriUri.GetWindowTextLength() == 0)
	{
		showErrorMessage("Please key-in URI.");
		m_EditUriUri.SetFocus();
		return;
	}
	
	m_EditUriUri.GetWindowText(tempString);

	tempString.TrimLeft();
	tempString.TrimRight();
	
	if(tempString.Compare("") == 0)
	{
		showErrorMessage("Please key-in URI.");
		m_EditUriUri.SetFocus();
		return;
	}

	if((URI_IDENTIFIER_CODE)m_ComboBoxUriUri.GetCurSel() == TEL && !(pNdef->isTelUriValid(tempString)))
	{
		showErrorMessage("Please key-in correct tel number.");
		m_EditUriUri.SetFocus();
		return;
	}

	encodeToNdefUri();
	
	delete pNdef;
}

void NdefUriDlg::encodeToNdefUri()
{
    NdefRecord*		pTempRecord;
    NdefMessage*	pNdefMessage = new NdefMessage();
	NdefRecordType* pNdefRecordType;
	vector<byte>	uBuffer;
	char			aUri[1024];
	char aMessage[100];
	int iMaximumInputLength = 141;

    //Encode URI
	pNdefRecordType = new NdefRecordType(NFC_FORM_WELL_KNOWN_TYPE, "U");
	pTempRecord = new NdefRecord(pNdefRecordType);

	m_EditUriUri.GetWindowText(aUri, m_EditUriUri.GetWindowTextLength() + 1);

    //URI Prefix
    //See URI Record Type Definition 
    //http://www.nfc-forum.org
	pTempRecord->setMessageBegin(true);
	pTempRecord->setMessageEnd(true);
    pTempRecord->appendPayload((byte)(URI_IDENTIFIER_CODE)m_ComboBoxUriUri.GetCurSel());
    pTempRecord->appendPayload(CString(aUri, strlen(aUri)));

    pNdefMessage->appendRecord(*(pTempRecord));

    uBuffer = pNdefMessage->toByteArray();

    if (uBuffer.size() > iMaximumInputLength)
    {
		sprintf(aMessage, "Input characters exceeds maximum input length. Please remove %d characters from URI.", uBuffer.size() - iMaximumInputLength);
        MessageBox(aMessage, "NTAG NDEF Sample Code", MB_OK|MB_ICONSTOP);
		return;
    }

	delete pTempRecord;
	delete pNdefMessage;
	delete pNdefRecordType;

	pFormParent_->m_EditNdefMessage.SetWindowText(cHelper_.vectorToString(uBuffer));
	pFormParent_->uSnepMessage_.clear();
	pFormParent_->uSnepMessage_ = uBuffer;

}

void NdefUriDlg::OnKillfocusEditUriUri() 
{
	CString sComboUri;
	CString sUri;
	
	m_ComboBoxUriUri.GetWindowText(sComboUri);
	m_EditUriUri.GetWindowText(sUri);

	if(sUri.Find(sComboUri) == 0)
	{
		sUri.Replace(sComboUri, "");
		m_EditUriUri.SetWindowText(sUri);
	}	
}
