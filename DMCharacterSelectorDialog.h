#if !defined(AFX_DMCHARACTERSELECTORDIALOG_H__EFAAB19A_6189_4ABD_B080_0BAD2B313977__INCLUDED_)
#define AFX_DMCHARACTERSELECTORDIALOG_H__EFAAB19A_6189_4ABD_B080_0BAD2B313977__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMCharacterSelectorDialog.h : header file
//


typedef enum
{
	DND_SELECTOR_CHARACTER = 0,

	DND_SELECTOR_CHARACTER_CASTER,

	DND_SELECTOR_MAP,

	DND_SELECTOR_PARTY,
	
	DND_SELECTOR_LAST_TYPE,

} DND_SELECTOR_TYPES;


/////////////////////////////////////////////////////////////////////////////
// DMCharacterSelectorDialog dialog

class DMCharacterSelectorDialog : public CDialog
{
// Construction
public:
	DMCharacterSelectorDialog(DWORD *pdwReturnedID, DWORD _dwParentPartyID, DND_SELECTOR_TYPES _SelectorType, DND_CHARACTER_CLASSES _SelectorClass = DND_CHARACTER_CLASS_UNDEF, DWORD dwExcludeID = 0, CWnd* pParent = NULL);   // standard constructor
	
	CDMHelperApp * m_pApp;

	DND_SELECTOR_TYPES m_SelectorType;

	DND_CHARACTER_CLASSES m_SelectorClass;

	DWORD m_dwExcludeID;

	DWORD m_dwParentPartyID;

	DWORD *m_pdwReturnedID;

// Dialog Data
	//{{AFX_DATA(DMCharacterSelectorDialog)
	enum { IDD = IDD_CHARACTER_SELECTOR_DIALOG };
	CListBox	m_cCharacterList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DMCharacterSelectorDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DMCharacterSelectorDialog)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkCharacterList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString m_szComment;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DMCHARACTERSELECTORDIALOG_H__EFAAB19A_6189_4ABD_B080_0BAD2B313977__INCLUDED_)
