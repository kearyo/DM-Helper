#pragma once
#include "afxwin.h"


// CDMAboutDialog dialog

class CDMAboutDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMAboutDialog)

public:
	CDMAboutDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMAboutDialog();

// Dialog Data
	enum { IDD = IDD_ABOUT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_szVersion;
};
