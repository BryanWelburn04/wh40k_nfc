// NdefText.cpp : implementation file
//

#include "stdafx.h"
#include "MifareNDEFSampleCode.h"
#include "NdefText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNdefText dialog


CNdefText::CNdefText(CWnd* pParent /*=NULL*/)
	: CDialog(CNdefText::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNdefText)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CNdefText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNdefText)
	DDX_Control(pDX, IDC_STATICTEXTMESSAGE, m_StaticTextMessage);
	DDX_Control(pDX, IDC_STATICTEXTLANGUAGE, m_StaticTextLanguage);
	DDX_Control(pDX, IDC_EDITTEXTMESSAGE, m_EditBoxTextMessage);
	DDX_Control(pDX, IDC_COMBOTEXTLANGUAGE, m_ComboBoxTextLanguage);
	DDX_Control(pDX, IDC_BUTTONENCODETEXT, m_ButtonEncodeText);
	DDX_Control(pDX, IDC_RADIOTEXTUTF8, m_RadioButtonUtf8);
	DDX_Control(pDX, IDC_RADIOTEXTUTF16, m_RadioButtonUtf16);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNdefText, CDialog)
	//{{AFX_MSG_MAP(CNdefText)
	ON_BN_CLICKED(IDC_BUTTONENCODETEXT, OnButtonencodetext)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNdefText message handlers

BOOL CNdefText::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	this->pFrmParent_ =  (CMifareNDEFSampleCodeDlg*) this->GetParent();	

	CDialog::OnInitDialog();
	
	m_ComboBoxTextLanguage.AddString("en");
	m_ComboBoxTextLanguage.AddString("zn-CN");
	m_ComboBoxTextLanguage.AddString("de");
	m_EditBoxTextMessage.SetLimitText(512);
	
	//Initial Value
	m_ComboBoxTextLanguage.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNdefText::OnButtonencodetext() 
{
	NdefRecordType* pNdefRecordType = new NdefRecordType(NFC_FORM_WELL_KNOWN_TYPE, "T");
	NdefRecord*		pNdefRecord = new NdefRecord(pNdefRecordType);
    byte			uStatusByte = 0x00;
    vector<byte>	uBuffer;
	char			aTextMessage[513]; //512 (Max characters) + 1 (Terminator)
	CString sTextLanguage;
    CString			sTextMessageChecker = "";
    
	try
    {
		if(m_EditBoxTextMessage.GetWindowTextLength() == 0)
		{
			cHelper_.showErrorMessage("Please key-in Message.");
			m_EditBoxTextMessage.SetFocus();
			return;
		}

		m_EditBoxTextMessage.GetWindowText(aTextMessage, m_EditBoxTextMessage.GetWindowTextLength() + 1);
		
		sTextMessageChecker = aTextMessage;
		sTextMessageChecker.TrimLeft(_T(" "));

		if (sTextMessageChecker == "")
		{
			cHelper_.showErrorMessage("Please key-in Message.");
			m_EditBoxTextMessage.SetFocus();
			return;
		}

		if (m_RadioButtonUtf16.GetCheck())
			uStatusByte = (byte)0x80;
		
		m_ComboBoxTextLanguage.GetWindowText(sTextLanguage);
		uStatusByte |= ((byte)sTextLanguage.GetLength() & 0x3F);

		pNdefRecord->setMessageBegin(true);
		pNdefRecord->setMessageEnd(true);	

		pNdefRecord->appendPayload(uStatusByte);
		pNdefRecord->appendPayload(sTextLanguage);
		pNdefRecord->appendPayload(CString(aTextMessage, strlen(aTextMessage)));

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


