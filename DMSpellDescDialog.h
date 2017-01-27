#if !defined(AFX_DMSPELLDESCDIALOG_H__60783BD9_F506_4285_A758_9FCD81B095BD__INCLUDED_)
#define AFX_DMSPELLDESCDIALOG_H__60783BD9_F506_4285_A758_9FCD81B095BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMSpellDescDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DMSpellDescDialog dialog

class DMSpellDescDialog : public CDialog
{
// Construction
public:
	DMSpellDescDialog(cDNDSpell *_pSpell, CWnd* pParent = NULL);   // standard constructor

	cDNDSpell *m_pSpell;

	void FindReferencedSpell();

// Dialog Data
	//{{AFX_DATA(DMSpellDescDialog)
	enum { IDD = IDD_SPELL_DESC_DIALOG };
	CEdit	m_cFakeEdit;
	CEdit	m_cSpellEditBox;
	CString	m_szSpellDesc;
	CString	m_szDesc1;
	CString	m_szDesc2;
	CString	m_szDesc3;
	CString	m_szDesc4;
	CString	m_szDesc5;
	CString	m_szDesc6;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DMSpellDescDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DMSpellDescDialog)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnChangeSpellDescEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DMSPELLDESCDIALOG_H__60783BD9_F506_4285_A758_9FCD81B095BD__INCLUDED_)
