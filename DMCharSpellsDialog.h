#if !defined(AFX_DMCHARSPELLSDIALOG_H__CE9AE719_BBB0_400F_8E1A_5280BC7235C2__INCLUDED_)
#define AFX_DMCHARSPELLSDIALOG_H__CE9AE719_BBB0_400F_8E1A_5280BC7235C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMCharSpellsDialog.h : header file
//


typedef enum
{
	DND_SPELL_LIST = 0,
	DND_SPELLBOOK_LIST,
	DND_SPELLCAST_LIST

} DND_SPELL_LIST_TYPES;

/////////////////////////////////////////////////////////////////////////////
// DMCharSpellsDialog dialog

class CDMCharViewDialog;

class DMCharSpellsDialog : public CDialog
{
// Construction
public:
	DMCharSpellsDialog(cDNDCharacter *_Character, CDMCharViewDialog *_SiblingWindow, CWnd* pParent = NULL);   // standard constructor


	void InitDisplay();
	void Refresh();
	void RefreshSpellControls();
	void ClearSpellList(int * pnSpellList);


	CWnd* m_pParent;

	CDMCharViewDialog* m_pSiblingWindow;

	cDNDCharacter *m_pCharacter;

	CDMHelperApp *m_pApp;

	int m_nTabSpellIndexes[4];
	int m_nSpellTabIndexes[4];

	int m_nBaseSpellLevel;
	//int m_nFirstSpellClass;
	int m_nSelectedSpellClass;
	int m_nSelectedSpellMode;
	int m_nSelectedSpellLevel;

	DND_VIEW_SPELL_MODE m_ViewSpellMode;
	cDNDSpell *m_pSelectedSpell;
	cDNDSpellBook *m_pSpellBook;
	BOOL m_bMageDisplay;
	BOOL m_bRefreshed;

	int m_nSelectedSpellLists[3][MAX_SPELLS_PER_LEVEL];

	int m_nSpellChartListTopIndex;
	DWORD m_dwMasterSpellListHash;

// Dialog Data
	//{{AFX_DATA(DMCharSpellsDialog)
	enum { IDD = IDD_CHAR_SPELLS_DIALOG };
	CStatic	m_cSpellbookStatic;
	CStatic	m_cSpellsRemainSpellbook;
	CListBox	m_cSpellsMemorizedList;
	CListBox	m_cSpellBookList;
	CButton	m_cSaveSpellsButton;
	CButton	m_cRecoverSpellsButton;
	CTabCtrl	m_cSpellsModeTab;
	CTabCtrl	m_cSpellsTopTab;
	CTabCtrl	m_cSpellsLevelTab;
	CStatic	m_cSpellsStatic;
	CButton	m_cForgetSpellButton;
	CButton	m_cMemorizeSpellButton;
	CListCtrl	m_cSpellChartList;
	CString	m_szSpellsRemaining;
	CString	m_szSpellsRemainingSpellbook;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DMCharSpellsDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DMCharSpellsDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg void OnSelchangeSpellsTopTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeSpellsModeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeSpellsLevelTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnClickSpellChartList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkSpellChartList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMemorizeSpell();
	afx_msg void OnForgetSpell();
	afx_msg void OnStoreSpells();
	afx_msg void OnRestoreSpells();
	afx_msg void OnSelchangeSpellBookList();
	afx_msg void OnSelchangeSpellsMemorizedList();
	afx_msg void OnClose();
	afx_msg void OnPingButton();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DMCHARSPELLSDIALOG_H__CE9AE719_BBB0_400F_8E1A_5280BC7235C2__INCLUDED_)
