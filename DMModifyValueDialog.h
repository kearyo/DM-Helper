#include "afxwin.h"
#if !defined(AFX_DMMODIFYVALUEDIALOG_H__1E3C846D_878E_49EC_81D5_DBF9DA9C48B3__INCLUDED_)
#define AFX_DMMODIFYVALUEDIALOG_H__1E3C846D_878E_49EC_81D5_DBF9DA9C48B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMModifyValueDialog.h : header file
//

void ModifyValue(int *_pnModifyValue, char *szLabel, LONG lAllValue, BOOL bAllowNegative = TRUE, BOOL bAddValue = TRUE);

/////////////////////////////////////////////////////////////////////////////
// DMModifyValueDialog dialog

class DMModifyValueDialog : public CDialog
{
// Construction
public:
	DMModifyValueDialog(int *_pnModifyValue, char *szLabel, LONG lAllValue, BOOL bAllowNegative = TRUE, BOOL bAddValue = TRUE, CWnd* pParent = NULL);   // standard constructor

	int *m_pnModifyValue;

	CString m_szLabel;
	BOOL m_bAllowNegative;
	BOOL m_bAddValue;
	LONG m_lAllValue;

// Dialog Data
	//{{AFX_DATA(DMModifyValueDialog)
	enum { IDD = IDD_MOD_VALUE_DIALOG };
	CButton	m_cSubtractButton;
	CString	m_szAddEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DMModifyValueDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DMModifyValueDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnChange();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	afx_msg void OnButton4();
	afx_msg void OnButton5();
	afx_msg void OnButton6();
	afx_msg void OnButton7();
	afx_msg void OnButton8();
	afx_msg void OnButton9();
	afx_msg void OnButton10();
	afx_msg void OnButtonBack();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CButton m_cOKButton;
	afx_msg void OnBnClickedButtonAll();
	CButton m_cAllButton;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DMMODIFYVALUEDIALOG_H__1E3C846D_878E_49EC_81D5_DBF9DA9C48B3__INCLUDED_)
