// NdefUri.cpp : implementation file
//

#include "stdafx.h"
#include "MifareNDEFSampleCode.h"
#include "NdefUri.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNdefUri dialog


CNdefUri::CNdefUri(CWnd* pParent /*=NULL*/)
	: CDialog(CNdefUri::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNdefUri)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CNdefUri::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNdefUri)
	DDX_Control(pDX, IDC_BUTTONENCODEURL, m_ButtonEncodeUrl);
	DDX_Control(pDX, IDC_COMBOURLURI, m_ComboBoxUrlUri);
	DDX_Control(pDX, IDC_EDITURLURI, m_EditBoxUrlUri);
	DDX_Control(pDX, IDC_STATICURLURI, m_StaticUrlUri);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNdefUri, CDialog)
	//{{AFX_MSG_MAP(CNdefUri)
	ON_BN_CLICKED(IDC_BUTTONENCODEURL, OnButtonencodeurl)
	ON_CBN_SELCHANGE(IDC_COMBOURLURI, OnSelchangeCombourluri)
	ON_EN_KILLFOCUS(IDC_EDITURLURI, OnKillfocusEditurluri)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNdefUri message handlers

BOOL CNdefUri::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	this->pFrmParent_ = (CMifareNDEFSampleCodeDlg*) this->GetParent();	

	CDialog::OnInitDialog();
	
	m_ComboBoxUrlUri.AddString("None");
	m_ComboBoxUrlUri.AddString("http://www.");
	m_ComboBoxUrlUri.AddString("https://www.");
	m_ComboBoxUrlUri.AddString("http://");
	m_ComboBoxUrlUri.AddString("https://");
	m_ComboBoxUrlUri.AddString("tel:");
	m_ComboBoxUrlUri.AddString("mailto:");
	m_EditBoxUrlUri.SetLimitText(512);
	
	//Initial Value
	m_ComboBoxUrlUri.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNdefUri::OnButtonencodeurl() 
{
	NdefRecordType* pNdefRecordType = new NdefRecordType(NFC_FORM_WELL_KNOWN_TYPE, "U");
	NdefRecord*		pNdefRecord  = new NdefRecord(pNdefRecordType);
    byte			uUrlPrefix = 0x00;
    vector<byte>	uBuffer;
	char			aUri[513];
	CString			sUriChecker = "";

	try
	{
		if(m_EditBoxUrlUri.GetWindowTextLength() == 0)
		{
			cHelper_.showErrorMessage("Please key-in URI.");
			m_EditBoxUrlUri.SetFocus();
			return;
		}

		m_EditBoxUrlUri.GetWindowText(aUri, m_EditBoxUrlUri.GetWindowTextLength() + 1);

		sUriChecker = aUri;
		sUriChecker.TrimLeft(_T(" "));

		if (sUriChecker == "")
		{
			cHelper_.showErrorMessage("Please key-in URI.");
			m_EditBoxUrlUri.SetFocus();
			return;
		}

		if (m_ComboBoxUrlUri.GetCurSel() == TEL_URI_INDEX)
		{

			if (!cNdef_.isTelUriValid(aUri))
			{
				cHelper_.showErrorMessage("Please enter correct tel number.");
				m_EditBoxUrlUri.SetFocus();
				return;
			}
		}

		// Encode URI
		pNdefRecord->setMessageBegin(true);
		pNdefRecord->setMessageEnd(true);

		// URI Prefix
		// See URI Record Type Definition 
		// http://www.nfc-forum.org
		pNdefRecord->appendPayload((byte)(URI_IDENTIFIER_CODE)m_ComboBoxUrlUri.GetCurSel());
		pNdefRecord->appendPayload(CString(aUri, strlen(aUri)));

		uBuffer = pNdefRecord->encodeToNdef();

		pFrmParent_->m_EditBoxNdefMessage.SetWindowText(cHelper_.vectorToString(uBuffer));
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
	
}

void CNdefUri::OnSelchangeCombourluri() 
{
	m_EditBoxUrlUri.SetWindowText("");
}

void CNdefUri::OnKillfocusEditurluri() 
{
	CString sUriFromComboBoxUri;
	CString sUriFromEditBoxUri;
	
	m_ComboBoxUrlUri.GetWindowText(sUriFromComboBoxUri);
	m_EditBoxUrlUri.GetWindowText(sUriFromEditBoxUri);

	sUriFromComboBoxUri = _T(sUriFromComboBoxUri);
	sUriFromEditBoxUri = _T(sUriFromEditBoxUri);
	
	if (!strncmp(sUriFromComboBoxUri, sUriFromEditBoxUri, strlen(sUriFromComboBoxUri)))
	{
		sUriFromEditBoxUri.Replace(_T(sUriFromComboBoxUri), _T(""));
		m_EditBoxUrlUri.SetWindowText(sUriFromEditBoxUri);
	}
}
