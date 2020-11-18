#pragma once
#include "afxwin.h"


// cMiniatureSelectionDialog dialog

class cMiniatureSelectionDialog : public CDialog
{
	DECLARE_DYNAMIC(cMiniatureSelectionDialog)

public:
	cMiniatureSelectionDialog(char *szNameBuffer, CWnd* pParent = NULL);   // standard constructor
	virtual ~cMiniatureSelectionDialog();

	CDMHelperApp *m_pApp;
	char *m_szNameBuffer;

// Dialog Data
	enum { IDD = IDD_MINI_SELECTOR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeMiniSelectionList();
	afx_msg void OnLbnDblclkMiniSelectionList();
	CListBox m_cMiniSelectList;
};
