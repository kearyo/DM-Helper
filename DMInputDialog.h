#pragma once
#include "afxwin.h"


// CDMInputDialog dialog

class CDMInputDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMInputDialog)

public:
	CDMInputDialog(char *pszPrompt, CString *pszRetVal, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMInputDialog();

	CString *m_pszRetVal;

// Dialog Data
	enum { IDD = IDD_INPUT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CString m_szInputEdit;
	CString m_szPrompt;
	CEdit m_cInputEdit;
};
