#if !defined(AFX_DMHPEDITDIALOG_H__31B12A1E_AEA3_4825_B840_89580B869B46__INCLUDED_)
#define AFX_DMHPEDITDIALOG_H__31B12A1E_AEA3_4825_B840_89580B869B46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMHPEditDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DMHPEditDialog dialog

class DMHPEditDialog : public CDialog
{
// Construction
public:
	DMHPEditDialog(cDNDCharacter *_Character, CWnd* pParent = NULL);   // standard constructor

	CDMCharViewDialog *m_pDMCharViewDialog;

	cDNDCharacter	*m_pCharacter;

// Dialog Data
	//{{AFX_DATA(DMHPEditDialog)
	enum { IDD = IDD_HP_EDIT_DIALOG };
	CString	m_szClass1Desc;
	CString	m_szClass2Desc;
	CString	m_szClass3Desc;
	CString	m_szHP[3][20];
	CEdit	m_cHPEdit[3][20];
	CString	m_szLLabel[20];
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DMHPEditDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DMHPEditDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_DMHPEDITDIALOG_H__31B12A1E_AEA3_4825_B840_89580B869B46__INCLUDED_)


