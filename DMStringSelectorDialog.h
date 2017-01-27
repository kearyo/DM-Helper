#pragma once
#include "afxwin.h"


// CDMStringSelectorDialog dialog

class CDMStringSelectorDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMStringSelectorDialog)

public:
	CDMStringSelectorDialog(char *pReturnString, int nMaxChars, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMStringSelectorDialog();

	void InsertString(int nIndex, char *szString);

	CWnd* m_pParent;

	int m_nMaxChars;

	char *m_pReturnString;

// Dialog Data
	enum { IDD = IDD_STRING_SELECTOR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_cStringList;
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CButton m_cOKButton;
	afx_msg void OnLbnSelchangeStringList();
protected:
	virtual void PostNcDestroy();
public:
	afx_msg void OnPaint();
};
