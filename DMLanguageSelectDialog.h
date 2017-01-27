#pragma once
#include "afxwin.h"


class CDMCharViewDialog;

// CDMLanguageSelectDialog dialog

class CDMLanguageSelectDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMLanguageSelectDialog)

public:
	CDMLanguageSelectDialog(cDNDObject* pObject, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMLanguageSelectDialog();

	CDMHelperApp *m_pApp;

	CDMCharViewDialog *m_pParentCharView;
	cDNDObject *m_pObject;

	void Refresh();

// Dialog Data
	enum { IDD = IDD_LANGUAGE_SELECT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_cLanguageList;
	afx_msg void OnBnClickedOk();
};
