
#include "stdafx.h"
#include "TabControl.h"
#include "NdefSmartPosterDlg.h"
#include "NdefTextDlg.h"
#include "NdefUriDlg.h"
#include "NdefVCardDlg.h"
#include "NtagNDEFSampleCode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Constructor/Destructor
//////////////////////////////////////////////////////////////////////
TabControl::TabControl()
{
   aM_DialogID[0] = IDD_DialogSmartPoster;
   aM_DialogID[1] = IDD_DialogUrl;
   aM_DialogID[2] = IDD_DialogText;
   aM_DialogID[3] = IDD_DialogVCard;

   aM_Dialog[0] = new NdefSmartPosterDlg();
   aM_Dialog[1] = new NdefUriDlg();
   aM_Dialog[2] = new NdefTextDlg();
   aM_Dialog[3] = new NdefVCardDlg();

   aM_nPageCount = 4;
}

TabControl::~TabControl()
{
	try
	{
		if (aM_Dialog[0]!= NULL)
			delete aM_Dialog[0];

		if (aM_Dialog[1]!= NULL)
			delete aM_Dialog[1];

		if (aM_Dialog[2]!= NULL)	
			delete aM_Dialog[2];

		if (aM_Dialog[3]!= NULL)
			delete aM_Dialog[3];
	}
	catch(...)
	{
		//silently catch any exceptions
	}
}

//////////////////////////////////////////////////////////////////////
// Implementation of member functions
//////////////////////////////////////////////////////////////////////
void TabControl::initDialogs()
{
	aM_Dialog[0]->Create(aM_DialogID[0],GetParent());
	aM_Dialog[1]->Create(aM_DialogID[1],GetParent());
	aM_Dialog[2]->Create(aM_DialogID[2],GetParent());
	aM_Dialog[3]->Create(aM_DialogID[3],GetParent());

	tabSmartPoster_ComboBoxSmartPosterTitleLanguage = aM_Dialog[0]->GetDlgItem(IDC_ComboBoxSmartPosterTitle);
	tabSmartPoster_ComboBoxSmartPosterPrefixUri 	= aM_Dialog[0]->GetDlgItem(IDC_ComboBoxUri);
	tabSmartPoster_ComboBoxSmartPosterAction 		= aM_Dialog[0]->GetDlgItem(IDC_ComboBoxAction);
	tabSmartPoster_StaticSmartPosterTitle 			= aM_Dialog[0]->GetDlgItem(IDC_StaticSmartPosterTitle);
	tabSmartPoster_StaticSmartPosterUri 			= aM_Dialog[0]->GetDlgItem(IDC_StaticSmartPosterUri);
	tabSmartPoster_StaticSmartPosterAction			= aM_Dialog[0]->GetDlgItem(IDC_StaticSmartPosterAction);
	tabSmartPoster_StaticSmartPosterRequired		= aM_Dialog[0]->GetDlgItem(IDC_StaticSmartPosterRequiredFields);
	tabSmartPoster_EditBoxSmartPosterTitle			= aM_Dialog[0]->GetDlgItem(IDC_EditSmartPosterTitle);
	tabSmartPoster_EditBoxSmartPosterUri 			= aM_Dialog[0]->GetDlgItem(IDC_EditSmartPosterUri);
	tabSmartPoster_ButtonEncodeSmartPoster			= aM_Dialog[0]->GetDlgItem(IDC_ButtonEncodeSmartPoster);

	tabUrl_StaticUrlUri								= aM_Dialog[1]->GetDlgItem(IDC_StaticUri);
	tabUrl_ComboBoxUrlUri							= aM_Dialog[1]->GetDlgItem(IDC_ComboBoxUriUri);
	tabUrl_EditBoxUrlUri							= aM_Dialog[1]->GetDlgItem(IDC_EditUriUri);
	tabUrl_ButtonEncodeUrl							= aM_Dialog[1]->GetDlgItem(IDC_ButtonEncodeUrl);

	tabText_ComboBoxTextLanguage					= aM_Dialog[2]->GetDlgItem(IDC_ComboBoxLanguage);
	tabText_StaticTextLanguage						= aM_Dialog[2]->GetDlgItem(IDC_StaticTextLanguage);
	tabText_StaticTextMessage						= aM_Dialog[2]->GetDlgItem(IDC_StaticTextMessage);;
	tabText_RadioButtonTextUtf8						= aM_Dialog[2]->GetDlgItem(IDC_RadioUtf8);
	tabText_RadioButtonTextUtf16					= aM_Dialog[2]->GetDlgItem(IDC_RadioUtf16);
	tabText_EditBoxTextMessage						= aM_Dialog[2]->GetDlgItem(IDC_EditMessage);
	tabText_ButtonEncodeText						= aM_Dialog[2]->GetDlgItem(IDC_ButtonEncodeText);

	tabVCard_StaticVCardGiven						= aM_Dialog[3]->GetDlgItem(IDC_StaticGivenName);
	tabVCard_StaticVCardMiddle						= aM_Dialog[3]->GetDlgItem(IDC_StaticMiddleName);
	tabVCard_StaticVCardSur							= aM_Dialog[3]->GetDlgItem(IDC_StaticSurname);
	tabVCard_EditBoxGiven							= aM_Dialog[3]->GetDlgItem(IDC_EditGivenName);
	tabVCard_EditBoxMiddle							= aM_Dialog[3]->GetDlgItem(IDC_EditMiddleName);
	tabVCard_EditBoxSur								= aM_Dialog[3]->GetDlgItem(IDC_EditSurname);
	tabVCard_ButtonEncodeVCard						= aM_Dialog[3]->GetDlgItem(IDC_ButtonEncodeVCard);
}

// Selection change event for the class derived from CTabCtrl
void TabControl::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	 activateTabDialogs();
	*pResult = 0;
}

void TabControl::activateTabDialogs()
{
	 int nSel = GetCurSel();
	 if(aM_Dialog[nSel]->m_hWnd)
		aM_Dialog[nSel]->ShowWindow(SW_HIDE);

	 CRect l_rectClient;
	 CRect l_rectWnd;

	 GetClientRect(l_rectClient);
	 AdjustRect(FALSE,l_rectClient);
	 GetWindowRect(l_rectWnd);
	 GetParent()->ScreenToClient(l_rectWnd);
	 l_rectClient.OffsetRect(l_rectWnd.left,l_rectWnd.top);
	 for(int nCount=0; nCount < aM_nPageCount; nCount++)
	 {
		aM_Dialog[nCount]->SetWindowPos(&wndTop, l_rectClient.left, l_rectClient.top, l_rectClient.Width(), l_rectClient.Height(), SWP_HIDEWINDOW);
	 }
	 
	 aM_Dialog[nSel]->SetWindowPos(&wndTop, l_rectClient.left, l_rectClient.top, l_rectClient.Width(), l_rectClient.Height(), SWP_SHOWWINDOW);
	 aM_Dialog[nSel]->ShowWindow(SW_SHOW);
}

void TabControl::enableControls(bool isEnable)
{
	tabSmartPoster_ComboBoxSmartPosterTitleLanguage->EnableWindow(isEnable);
	tabSmartPoster_ComboBoxSmartPosterPrefixUri->EnableWindow(isEnable);
	tabSmartPoster_ComboBoxSmartPosterAction->EnableWindow(isEnable);
	tabSmartPoster_StaticSmartPosterTitle->EnableWindow(isEnable);
	tabSmartPoster_StaticSmartPosterUri->EnableWindow(isEnable);
	tabSmartPoster_StaticSmartPosterAction->EnableWindow(isEnable);
	tabSmartPoster_StaticSmartPosterRequired->EnableWindow(isEnable);
	tabSmartPoster_EditBoxSmartPosterTitle->EnableWindow(isEnable);
	tabSmartPoster_EditBoxSmartPosterUri->EnableWindow(isEnable);
	tabSmartPoster_ButtonEncodeSmartPoster->EnableWindow(isEnable);
	
	tabText_ComboBoxTextLanguage->EnableWindow(isEnable);
	tabText_StaticTextLanguage->EnableWindow(isEnable);
	tabText_StaticTextMessage->EnableWindow(isEnable);
	tabText_RadioButtonTextUtf8->EnableWindow(isEnable);
	tabText_RadioButtonTextUtf16->EnableWindow(isEnable);
	tabText_EditBoxTextMessage->EnableWindow(isEnable);
	tabText_ButtonEncodeText->EnableWindow(isEnable);
	
	tabUrl_StaticUrlUri->EnableWindow(isEnable);
	tabUrl_ComboBoxUrlUri->EnableWindow(isEnable);
	tabUrl_EditBoxUrlUri->EnableWindow(isEnable);
	tabUrl_ButtonEncodeUrl->EnableWindow(isEnable);
	
	tabVCard_StaticVCardGiven->EnableWindow(isEnable);
	tabVCard_StaticVCardMiddle->EnableWindow(isEnable);
	tabVCard_StaticVCardSur->EnableWindow(isEnable);
	tabVCard_EditBoxGiven->EnableWindow(isEnable);
	tabVCard_EditBoxMiddle->EnableWindow(isEnable);
	tabVCard_EditBoxSur->EnableWindow(isEnable);
	tabVCard_ButtonEncodeVCard->EnableWindow(isEnable);
}

void TabControl::resetFields() 
{
	((CComboBox*)tabSmartPoster_ComboBoxSmartPosterTitleLanguage)->SetCurSel(0);
	((CComboBox*)tabSmartPoster_ComboBoxSmartPosterPrefixUri)->SetCurSel(0);
	((CComboBox*)tabSmartPoster_ComboBoxSmartPosterAction)->SetCurSel(0);

	((CComboBox*)tabUrl_ComboBoxUrlUri)->SetCurSel(0);
	((CButton*)tabText_RadioButtonTextUtf8)->SetCheck(1);
	((CButton*)tabText_RadioButtonTextUtf16)->SetCheck(0);

	((CComboBox*)tabText_ComboBoxTextLanguage)->SetCurSel(0);
}


BEGIN_MESSAGE_MAP(TabControl, CTabCtrl)
	//{{AFX_MSG_MAP(MyTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
