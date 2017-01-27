#pragma once
#include "afxwin.h"


// CDMNPCPortraitSelectDialog dialog

class CDMNPCPortraitSelectDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMNPCPortraitSelectDialog)

public:
	CDMNPCPortraitSelectDialog(CString szType, CString szSourcePath, CString szDestPath, CString szMMName, CString szCharacterName, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMNPCPortraitSelectDialog();
	
	CDMHelperApp *m_pApp;

	CString m_szType;
	CString m_szSourcePath;
	CString m_szDestPath;
	CString m_szMMName;
	CString m_szCharacterName;
	CString m_szFileType;

// Dialog Data
	enum { IDD = IDD_PORTRAIT_SELECT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_nRadio1;
	int m_nRadio2;
	afx_msg void OnBnClickedOk();
	CButton m_cDeleteCheck;
};
