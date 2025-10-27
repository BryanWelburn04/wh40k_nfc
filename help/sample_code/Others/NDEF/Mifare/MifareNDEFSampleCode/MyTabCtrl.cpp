/////////////////////////////////////////////////////////////////////
//MyTabCtrl.cpp

#include "stdafx.h"
#include "MifareNDEFSampleCode.h"
#include "MyTabCtrl.h"
#include "NdefSmartPoster.h"
#include "NdefText.h"
#include "NdefUri.h"
#include "NdefVCard.h"
#include "MifareNDEFSampleCodeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//////////////////////////////////////////////////////////////////////
// Constructor/Destructor
//////////////////////////////////////////////////////////////////////
MyTabCtrl::MyTabCtrl()
{
   aDialogId[0] = IDD_DialogSmartPoster;
   aDialogId[1] = IDD_DialogUri;
   aDialogId[2] = IDD_DialogText;
   aDialogId[3] = IDD_DialogVCard;

   pDialog[0] = new CNdefSmartPoster();
   pDialog[1] = new CNdefUri();
   pDialog[2] = new CNdefText();
   pDialog[3] = new CNdefVCard();

   iPageCount = 4;
}

MyTabCtrl::~MyTabCtrl()
{
	try
	{
		if (pDialog[0]!= NULL)
			delete pDialog[0];

		if (pDialog[1]!= NULL)
			delete pDialog[1];

		if (pDialog[2]!= NULL)	
			delete pDialog[2];

		if (pDialog[3]!= NULL)
			delete pDialog[3];
	}
	catch(...)
	{
		//silently catch any exceptions
	}
}

void MyTabCtrl::changeTab(int iCurrentTab, int iToFocus)
{
	pDialog[iCurrentTab]->ShowWindow(SW_HIDE);
	pDialog[iToFocus]->ShowWindow(SW_SHOW);
	pDialog[iToFocus]->SetFocus();
}

//////////////////////////////////////////////////////////////////////
// Implementation of member functions
//////////////////////////////////////////////////////////////////////
void MyTabCtrl::initDialogs()
{
	pDialog[0]->Create(aDialogId[0],GetParent());
	pDialog[1]->Create(aDialogId[1],GetParent());
	pDialog[2]->Create(aDialogId[2],GetParent());
	pDialog[3]->Create(aDialogId[3],GetParent());

	tabSmartPoster_ComboBoxSmartPosterTitleLanguage = pDialog[0]->GetDlgItem(IDC_COMBOSMARTPOSTERTITLELANGUAGE);
	tabSmartPoster_ComboBoxSmartPosterPrefixUri 	= pDialog[0]->GetDlgItem(IDC_COMBOSMARTPOSTERPREFIXURI);
	tabSmartPoster_ComboBoxSmartPosterAction 		= pDialog[0]->GetDlgItem(IDC_COMBOSMARTPOSTERACTION);
	tabSmartPoster_StaticSmartPosterTitle 			= pDialog[0]->GetDlgItem(IDC_STATICSMARTPOSTERTITLE);
	tabSmartPoster_StaticSmartPosterUri 			= pDialog[0]->GetDlgItem(IDC_STATICSMARTPOSTERURI);
	tabSmartPoster_StaticSmartPosterAction			= pDialog[0]->GetDlgItem(IDC_STATICSMARTPOSTERACTION);
	tabSmartPoster_StaticSmartPosterRequired		= pDialog[0]->GetDlgItem(IDC_STATICSMARTPOSTERREQUIRED);
	tabSmartPoster_EditBoxSmartPosterTitle			= pDialog[0]->GetDlgItem(IDC_EDITSMARTPOSTERTITLE);
	tabSmartPoster_EditBoxSmartPosterUri 			= pDialog[0]->GetDlgItem(IDC_EDITSMARTPOSTERURI);
	tabSmartPoster_ButtonEncodeSmartPoster			= pDialog[0]->GetDlgItem(IDC_BUTTONENCODESMARTPOSTER);

	tabUrl_StaticUrlUri								= pDialog[1]->GetDlgItem(IDC_STATICURLURI);
	tabUrl_ComboBoxUrlUri							= pDialog[1]->GetDlgItem(IDC_COMBOURLURI);
	tabUrl_EditBoxUrlUri							= pDialog[1]->GetDlgItem(IDC_EDITURLURI);
	tabUrl_ButtonEncodeUrl							= pDialog[1]->GetDlgItem(IDC_BUTTONENCODEURL);

	tabText_ComboBoxTextLanguage					= pDialog[2]->GetDlgItem(IDC_COMBOTEXTLANGUAGE);
	tabText_StaticTextLanguage						= pDialog[2]->GetDlgItem(IDC_STATICTEXTLANGUAGE);
	tabText_StaticTextMessage						= pDialog[2]->GetDlgItem(IDC_STATICTEXTMESSAGE);;
	tabText_RadioButtonTextUtf8						= pDialog[2]->GetDlgItem(IDC_RADIOTEXTUTF8);
	tabText_RadioButtonTextUtf16					= pDialog[2]->GetDlgItem(IDC_RADIOTEXTUTF16);
	tabText_EditBoxTextMessage						= pDialog[2]->GetDlgItem(IDC_EDITTEXTMESSAGE);
	tabText_ButtonEncodeText						= pDialog[2]->GetDlgItem(IDC_BUTTONENCODETEXT);

	tabVCard_StaticVCardGiven						= pDialog[3]->GetDlgItem(IDC_STATICVCARDGIVEN);
	tabVCard_StaticVCardMiddle						= pDialog[3]->GetDlgItem(IDC_STATICVCARDMIDDLE);
	tabVCard_StaticVCardSur							= pDialog[3]->GetDlgItem(IDC_STATICVCARDSUR);
	tabVCard_EditBoxGiven							= pDialog[3]->GetDlgItem(IDC_EDITVCARDGIVEN);
	tabVCard_EditBoxMiddle							= pDialog[3]->GetDlgItem(IDC_EDITVCARDMIDDLE);
	tabVCard_EditBoxSur								= pDialog[3]->GetDlgItem(IDC_EDITVCARDSUR);
	tabVCard_ButtonEncodeVCard						= pDialog[3]->GetDlgItem(IDC_BUTTONENCODEVCARD);
}

// Selection change event for the class derived from CTabCtrl
void MyTabCtrl::OnSelchange(NMHDR* pNMHDR, LRESULT* pResult) 
{
	 activateTabDialogs();
	*pResult = 0;
}

void MyTabCtrl::activateTabDialogs()
{
	CRect cRectClient;
	CRect cRectWnd;

	iSelected = GetCurSel();

	if(pDialog[iSelected]->m_hWnd)
		pDialog[iSelected]->ShowWindow(SW_HIDE);

	GetClientRect(cRectClient);
	AdjustRect(FALSE,cRectClient);
	GetWindowRect(cRectWnd);
	GetParent()->ScreenToClient(cRectWnd);
	cRectClient.OffsetRect(cRectWnd.left,cRectWnd.top);

	for(int iCount=0; iCount < iPageCount; iCount++)
	{
		pDialog[iCount]->SetWindowPos(&wndTop, cRectClient.left, cRectClient.top, cRectClient.Width(), cRectClient.Height(), SWP_HIDEWINDOW);
	}
	 
	pDialog[iSelected]->SetWindowPos(&wndTop, cRectClient.left, cRectClient.top, cRectClient.Width(), cRectClient.Height(), SWP_SHOWWINDOW);
	pDialog[iSelected]->ShowWindow(SW_SHOW);
}

BEGIN_MESSAGE_MAP(MyTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(MyTabCtrl)
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnSelchange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
