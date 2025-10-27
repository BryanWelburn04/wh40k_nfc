#pragma once


// CNoPasteEdit

class CNoPasteEdit : public CEdit
{
	DECLARE_DYNAMIC(CNoPasteEdit)

public:
	CNoPasteEdit();
	virtual ~CNoPasteEdit();

protected:
	// This line will need to be added by hand because WM_PASTE is not available in
	// class wizard
	afx_msg LRESULT OnPaste(WPARAM wParam, LPARAM lParam);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};


