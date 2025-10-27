// NdefSmartPosterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NtagNDEFSampleCode.h"
#include "NdefSmartPosterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NdefSmartPosterDlg dialog

void showErrorMessage(CString sMessage);

NdefSmartPosterDlg::NdefSmartPosterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(NdefSmartPosterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(NdefSmartPosterDlg)
	//}}AFX_DATA_INIT
}


void NdefSmartPosterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NdefSmartPosterDlg)
	DDX_Control(pDX, IDC_EditSmartPosterUri, m_EditSmartPosterUri);
	DDX_Control(pDX, IDC_EditSmartPosterTitle, m_EditSmartPosterTitle);
	DDX_Control(pDX, IDC_ComboBoxUri, m_ComboBoxSmartPosterUri);
	DDX_Control(pDX, IDC_ComboBoxSmartPosterTitle, m_ComboBoxSmartPosterTitle);
	DDX_Control(pDX, IDC_ComboBoxAction, m_ComboBoxAction);
	DDX_Control(pDX, IDC_ButtonEncodeSmartPoster, m_ButtonEncodeSmartPoster);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NdefSmartPosterDlg, CDialog)
	//{{AFX_MSG_MAP(NdefSmartPosterDlg)
	ON_BN_CLICKED(IDC_ButtonEncodeSmartPoster, OnButtonEncodeSmartPoster)
	ON_CBN_SELCHANGE(IDC_ComboBoxUri, OnSelchangeComboBoxUri)
	ON_EN_KILLFOCUS(IDC_EditSmartPosterUri, OnKillfocusEditSmartPosterUri)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NdefSmartPosterDlg message handlers

BOOL NdefSmartPosterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	this->pFormParent_ = (NtagNDEFSampleCodeDlg*) this->GetParent();
	
	// TODO: Add extra initialization here

	m_EditSmartPosterTitle.SetLimitText(141);
	m_EditSmartPosterUri.SetLimitText(141);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void NdefSmartPosterDlg::OnButtonEncodeSmartPoster() 
{
	Ndef*			pNdef = new Ndef();
	char			aTitle[1024];
	CString			sTitle = "";
	CString			sSpTitle = "";
	CString			tempString;
	char			aUri[1024];
	ACTION_RECORD	eActionRecord;
	vector<byte>	uBuffer;
    
	const int MAXIMUM_INPUT_LENGTH = 141;

    switch (m_ComboBoxAction.GetCurSel())
    {
        case 1:  eActionRecord = DO_THE_ACTION; break;
        case 2:  eActionRecord = SAVE_FOR_LATER; break;
        case 3:  eActionRecord = OPEN_FOR_EDITING; break;
        default: eActionRecord = NO_ACTION; break;
    }
    
    try
    {

		if(m_EditSmartPosterUri.GetWindowTextLength() == 0)
		{
			showErrorMessage("Please key-in Smart Poster URI.");
			m_EditSmartPosterUri.SetFocus();
			return;
		}

		m_EditSmartPosterUri.GetWindowText(tempString);

		tempString.TrimLeft();
		tempString.TrimRight();

		if(tempString.Compare("") == 0)
		{
			showErrorMessage("Please key-in Smart Poster URI.");
			m_EditSmartPosterUri.SetFocus();
			return;
		}

		if((URI_IDENTIFIER_CODE)m_ComboBoxSmartPosterUri.GetCurSel() == TEL && !(pNdef->isTelUriValid(tempString)))
		{
			showErrorMessage("Please key-in correct tel number.");
			m_EditSmartPosterUri.SetFocus();
			return;
		}

		m_ComboBoxSmartPosterTitle.GetWindowText(sTitle);

		m_EditSmartPosterUri.GetWindowText(aUri, m_EditSmartPosterUri.GetWindowTextLength() + 1);

		if(!(m_EditSmartPosterTitle.GetWindowTextLength() == 0))
		{
			m_EditSmartPosterTitle.GetWindowText(aTitle, m_EditSmartPosterTitle.GetWindowTextLength() + 1);
			sSpTitle = CString(aTitle, strlen(aTitle));
		}


        uBuffer = pNdef->encodeSmartPoster(sTitle,
										   sSpTitle,
										   (URI_IDENTIFIER_CODE)m_ComboBoxSmartPosterUri.GetCurSel(),
										   CString(aUri, strlen(aUri)),
										   eActionRecord);
        
        if (uBuffer.size() < 1)
        {
            pFormParent_->m_EditNdefMessage.SetWindowText("");
            return;
        }

		if (uBuffer.size() > MAXIMUM_INPUT_LENGTH)
		{
			tempString.Format("Input characters exceeds maximum input length. Please remove %d character/s from Title or URI.", uBuffer.size() - MAXIMUM_INPUT_LENGTH);
			showErrorMessage(tempString);
			return;
		}
        
        pFormParent_->m_EditNdefMessage.SetWindowText(cHelper_.vectorToString(uBuffer));
		pFormParent_->uSnepMessage_.clear();
		pFormParent_->uSnepMessage_ = uBuffer;
    }
    catch (NdefException ex)
	{
		pFormParent_->addMessageToLogs(ex.what());
		MessageBox(ex.what(), "NDEF Exception", MB_OK);
	}
	catch (...)
	{
		pFormParent_->addMessageToLogs("System Exception");
	}

	delete pNdef;
	
}

void NdefSmartPosterDlg::OnSelchangeComboBoxUri() 
{
	m_EditSmartPosterUri.SetWindowText("");
}

void NdefSmartPosterDlg::OnKillfocusEditSmartPosterUri() 
{
	CString sComboUri;
	CString sUri;
	
	m_ComboBoxSmartPosterUri.GetWindowText(sComboUri);
	m_EditSmartPosterUri.GetWindowText(sUri);

	if(sUri.Find(sComboUri) == 0)
	{
		sUri.Replace(sComboUri, "");
		m_EditSmartPosterUri.SetWindowText(sUri);
	}
}
