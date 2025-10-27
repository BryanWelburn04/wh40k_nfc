/////////////////////////////////////////////////////////////////////////////
// MyTabCtrl.h : header file
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYTABCTRL_H__F2BE49C1_1B83_49E0_B759_BDC1910A5DF7__INCLUDED_)
#define AFX_MYTABCTRL_H__F2BE49C1_1B83_49E0_B759_BDC1910A5DF7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MyTabCtrl : public CTabCtrl
{
public:
	MyTabCtrl();          // Constructor
	virtual ~MyTabCtrl(); // Destructor

	//Controls
	CWnd* tabSmartPoster_ComboBoxSmartPosterTitleLanguage;
	CWnd* tabSmartPoster_ComboBoxSmartPosterPrefixUri;
	CWnd* tabSmartPoster_ComboBoxSmartPosterAction;
	CWnd* tabSmartPoster_StaticSmartPosterTitle;
	CWnd* tabSmartPoster_StaticSmartPosterUri;
	CWnd* tabSmartPoster_StaticSmartPosterAction;
	CWnd* tabSmartPoster_StaticSmartPosterRequired;
	CWnd* tabSmartPoster_EditBoxSmartPosterTitle;
	CWnd* tabSmartPoster_EditBoxSmartPosterUri;
	CWnd* tabSmartPoster_ButtonEncodeSmartPoster;

	CWnd* tabText_ComboBoxTextLanguage;
	CWnd* tabText_StaticTextLanguage;
	CWnd* tabText_StaticTextMessage;
	CWnd* tabText_RadioButtonTextUtf8;
	CWnd* tabText_RadioButtonTextUtf16;
	CWnd* tabText_EditBoxTextMessage;
	CWnd* tabText_ButtonEncodeText;

	CWnd* tabUrl_StaticUrlUri;
	CWnd* tabUrl_ComboBoxUrlUri;
	CWnd* tabUrl_EditBoxUrlUri;
	CWnd* tabUrl_ButtonEncodeUrl;

	CWnd* tabVCard_StaticVCardGiven;
	CWnd* tabVCard_StaticVCardMiddle;
	CWnd* tabVCard_StaticVCardSur;
	CWnd* tabVCard_EditBoxGiven;
	CWnd* tabVCard_EditBoxMiddle;
	CWnd* tabVCard_EditBoxSur;
	CWnd* tabVCard_ButtonEncodeVCard;
	
	// Attributes
	CDialog* pDialog[4]; // CDialog Array Variable to hold the dialogs
	int aDialogId[4];    // Array to hold the list of dialog boxes/tab pages for CTabCtrl
	int iPageCount;     // Number of tab pages
	int iSelected;
	
	// Operations
	void changeTab(int iCurrentTab, int iToFocus);
	void initDialogs(); // Function to Create the dialog boxes during startup
	void activateTabDialogs(); //Function to activate the tab dialog boxes

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MyTabCtrl)
	//}}AFX_VIRTUAL

protected:
	// Generated message map functions
	//{{AFX_MSG(MyTabCtrl)
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTABCTRL_H__F2BE49C1_1B83_49E0_B759_BDC1910A5DF7__INCLUDED_)
