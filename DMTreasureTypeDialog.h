#pragma once
#include "afxwin.h"


// cDMTreasureTypeDialog dialog

class cDMTreasureTypeDialog : public CDialog
{
	DECLARE_DYNAMIC(cDMTreasureTypeDialog)

public:
	cDMTreasureTypeDialog(CString *pszTreasureType, CWnd* pParent = NULL);   // standard constructor
	virtual ~cDMTreasureTypeDialog();


	CString *m_pszTreasureType;

// Dialog Data
	enum { IDD = IDD_TREASURE_TYPE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_cTreasureTypeList;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CString m_szTTMul;
};
