#pragma once
#include "afxwin.h"


// CDMScrollDetailsDialog dialog

class CDMScrollDetailsDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMScrollDetailsDialog)

public:
	CDMScrollDetailsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMScrollDetailsDialog();

	void UpdateButtons();
	void Refresh();

	CDMHelperApp *m_pApp;

	DMInventoryDialog *m_pParentWindow;

	int m_nSelectedSpellIndex;

// Dialog Data
	enum { IDD = IDD_SCROLL_DETAILS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CListBox m_cScrollSpellList;
	afx_msg void OnBnClickedAddSpellButton();
	afx_msg void OnLbnSelchangeScrollSpellList();
	afx_msg void OnBnClickedDeleteSpellButton();
	afx_msg void OnBnClickedCastButton();
	afx_msg void OnBnClickedCopySpellButton();
	afx_msg void OnPaint();
	CString m_szChargesEdit;
	afx_msg void OnEnChangeChargesEdit();
};
