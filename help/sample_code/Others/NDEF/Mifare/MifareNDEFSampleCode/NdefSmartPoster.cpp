// NdefSmartPoster.cpp : implementation file
//
#include "stdafx.h"
#include "MifareNDEFSampleCode.h"
#include "NdefSmartPoster.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNdefSmartPoster dialog


CNdefSmartPoster::CNdefSmartPoster(CWnd* pParent /*=NULL*/)
	: CDialog(CNdefSmartPoster::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNdefSmartPoster)
	//}}AFX_DATA_INIT
}


void CNdefSmartPoster::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNdefSmartPoster)
	DDX_Control(pDX, IDC_EDITSMARTPOSTERURI, m_EditBoxSmartPosterUri);
	DDX_Control(pDX, IDC_COMBOSMARTPOSTERPREFIXURI, m_ComboBoxSmartPosterPrefixUri);
	DDX_Control(pDX, IDC_BUTTONENCODESMARTPOSTER, m_ButtonEncodeSmartPoster);
	DDX_Control(pDX, IDC_COMBOSMARTPOSTERACTION, m_ComboBoxSmartPosterAction);
	DDX_Control(pDX, IDC_COMBOSMARTPOSTERTITLELANGUAGE, m_ComboBoxSmartPosterTitleLanguage);
	DDX_Control(pDX, IDC_STATICSMARTPOSTERACTION, m_StaticSmartPosterAction);
	DDX_Control(pDX, IDC_STATICSMARTPOSTERURI, m_StaticSmartPosterUri);
	DDX_Control(pDX, IDC_STATICSMARTPOSTERTITLE, m_StaticSmartPosterTitle);
	DDX_Control(pDX, IDC_STATICSMARTPOSTERREQUIRED, m_StaticSmartPosterRequired);
	DDX_Control(pDX, IDC_EDITSMARTPOSTERTITLE, m_EditBoxSmartPosterTitle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNdefSmartPoster, CDialog)
	//{{AFX_MSG_MAP(CNdefSmartPoster)
	ON_BN_CLICKED(IDC_BUTTONENCODESMARTPOSTER, OnButtonencodesmartposter)
	ON_CBN_SELCHANGE(IDC_COMBOSMARTPOSTERPREFIXURI, OnSelchangeCombosmartposterprefixuri)
	ON_EN_KILLFOCUS(IDC_EDITSMARTPOSTERURI, OnKillfocusEditsmartposteruri)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNdefSmartPoster message handlers

BOOL CNdefSmartPoster::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	this->pFrmParent_ = (CMifareNDEFSampleCodeDlg*) this->GetParent();	

	CDialog::OnInitDialog();
	
	m_ComboBoxSmartPosterTitleLanguage.AddString("en");
	m_ComboBoxSmartPosterTitleLanguage.AddString("zn-CN");
	m_ComboBoxSmartPosterTitleLanguage.AddString("de");

	m_ComboBoxSmartPosterPrefixUri.AddString("None");
	m_ComboBoxSmartPosterPrefixUri.AddString("http://www.");
	m_ComboBoxSmartPosterPrefixUri.AddString("https://www.");
	m_ComboBoxSmartPosterPrefixUri.AddString("http://");
	m_ComboBoxSmartPosterPrefixUri.AddString("https://");
	m_ComboBoxSmartPosterPrefixUri.AddString("tel:");
	m_ComboBoxSmartPosterPrefixUri.AddString("mailto:");

	m_ComboBoxSmartPosterAction.AddString("No Action");
	m_ComboBoxSmartPosterAction.AddString("Do the action");
	m_ComboBoxSmartPosterAction.AddString("Save For Later");
	m_ComboBoxSmartPosterAction.AddString("Open for editing");

	m_EditBoxSmartPosterTitle.SetLimitText(170);
	m_EditBoxSmartPosterUri.SetLimitText(512);

	//Initial Value
	m_ComboBoxSmartPosterTitleLanguage.SetCurSel(0);
	m_ComboBoxSmartPosterPrefixUri.SetCurSel(0);
	m_ComboBoxSmartPosterAction.SetCurSel(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNdefSmartPoster::OnButtonencodesmartposter() 
{
	Ndef*			pNdef = new Ndef();
	char			aTitle[1024];
	char			aUri[1024];
	CString			sTitle = "";
	ACTION_RECORD	eActionRecord;
	vector<byte>	uBuffer;
	CString			sSmartPosterUriChecker = "";
    
    switch (m_ComboBoxSmartPosterAction.GetCurSel())
    {
        case 1:  eActionRecord = DO_THE_ACTION; break;
        case 2:  eActionRecord = SAVE_FOR_LATER; break;
        case 3:  eActionRecord = OPEN_FOR_EDITING; break;
        default: eActionRecord = NO_ACTION; break;
    }
    
    try
    {
		m_ComboBoxSmartPosterTitleLanguage.GetWindowText(sTitle);

		if(m_EditBoxSmartPosterUri.GetWindowTextLength() == 0)
		{
			cHelper_.showErrorMessage("Please key-in Smart Poster URI.");
			m_EditBoxSmartPosterUri.SetFocus();
			return;
		}

		m_EditBoxSmartPosterTitle.GetWindowText(aTitle, m_EditBoxSmartPosterTitle.GetWindowTextLength() + 1);
		m_EditBoxSmartPosterUri.GetWindowText(aUri, m_EditBoxSmartPosterUri.GetWindowTextLength() + 1);

		sSmartPosterUriChecker = aUri;

		sSmartPosterUriChecker.TrimLeft(_T(" "));

		if (sSmartPosterUriChecker == "")
		{
			cHelper_.showErrorMessage("Please key-in Smart Poster URI.");
			m_EditBoxSmartPosterUri.SetFocus();
			return;
		}

		if (m_ComboBoxSmartPosterPrefixUri.GetCurSel() == TEL_URI_INDEX)
		{
			if (!cNdef_.isTelUriValid(aUri))
			{
				cHelper_.showErrorMessage("Please enter correct tel number.");
				m_EditBoxSmartPosterUri.SetFocus();
				return;
			}
		}


        uBuffer = pNdef->encodeSmartPoster(sTitle,
										   CString(aTitle, strlen(aTitle)),
										   (URI_IDENTIFIER_CODE)m_ComboBoxSmartPosterPrefixUri.GetCurSel(),
										   CString(aUri, strlen(aUri)),
										   eActionRecord);
        
        if (uBuffer.size() < 1)
        {
            pFrmParent_->m_EditBoxNdefMessage.SetWindowText("");
            return;
        }
        
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

	delete pNdef;
	
}

void CNdefSmartPoster::OnSelchangeCombosmartposterprefixuri() 
{
	m_EditBoxSmartPosterUri.SetWindowText("");
}

void CNdefSmartPoster::OnKillfocusEditsmartposteruri() 
{
	CString sUriFromComboBoxUri;
	CString sUriFromEditBoxUri;
	
	m_ComboBoxSmartPosterPrefixUri.GetWindowText(sUriFromComboBoxUri);
	m_EditBoxSmartPosterUri.GetWindowText(sUriFromEditBoxUri);

	sUriFromComboBoxUri = _T(sUriFromComboBoxUri);
	sUriFromEditBoxUri = _T(sUriFromEditBoxUri);

	if (!strncmp(sUriFromComboBoxUri, sUriFromEditBoxUri, strlen(sUriFromComboBoxUri)))
	{
		sUriFromEditBoxUri.Replace(_T(sUriFromComboBoxUri), _T(""));
		m_EditBoxSmartPosterUri.SetWindowText(sUriFromEditBoxUri);
	}
}
