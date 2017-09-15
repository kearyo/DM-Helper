#pragma once
#include "afxwin.h"


// cDMMapNameDialog dialog

class cDMMapNameDialog : public CDialog
{
	DECLARE_DYNAMIC(cDMMapNameDialog)

public:
	cDMMapNameDialog(cDNDMap *pDNDMap, CWnd* pParent = NULL);   // standard constructor
	virtual ~cDMMapNameDialog();

	cDNDMap *m_pDNDMap;

// Dialog Data
	enum { IDD = IDD_NAME_MAP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CEdit m_cNameEdit;
	CString m_szMapName;
	afx_msg void OnEnChangeEdit1();
};
