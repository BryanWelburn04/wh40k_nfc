// NoPasteEdit.cpp : implementation file
//

#include "stdafx.h"
#include "NoPasteEdit.h"


// CNoPasteEdit

IMPLEMENT_DYNAMIC(CNoPasteEdit, CEdit)

CNoPasteEdit::CNoPasteEdit()
{
// Put any construction code here
}

CNoPasteEdit::~CNoPasteEdit()
{
// Put any destruction code here
}

BEGIN_MESSAGE_MAP(CNoPasteEdit, CEdit)
// This line is needed because there is no default macro for WM_PASTE messages
// This line will also need to be added by hand
ON_MESSAGE(WM_PASTE, OnPaste)
ON_WM_CONTEXTMENU()
END_MESSAGE_MAP()

LRESULT CNoPasteEdit::OnPaste(WPARAM wParam, LPARAM lParam)
{
// Put any code here you want to execute when the user right clicks on the edit
// control. Just leave it blank to disable the menu
	return 0L;
}

void CNoPasteEdit::OnContextMenu(CWnd* pWnd, CPoint point)
{
// Put any code here you want to execute when the user tries to paste into the edit
// conrtol. Just leave it blank to prevent pasting.
}