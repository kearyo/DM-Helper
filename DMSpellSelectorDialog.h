#pragma once
#include "afxwin.h"


// CDMSpellSelectorDialog dialog

class CDMSpellSelectorDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMSpellSelectorDialog)

public:
	CDMSpellSelectorDialog(cDNDSpell **pSpell, DND_CHARACTER_CLASSES _MagicType, int *pnSpellCount, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMSpellSelectorDialog();

	void Refresh();

	CDMHelperApp *m_pApp;

	cDNDSpell **m_pSpell;
	int *m_pnSpellCount;

	DND_CHARACTER_CLASSES m_MagicType;
	DND_CHARACTER_CLASSES m_ClassBook;
	int m_nSpellLevel;


// Dialog Data
	enum { IDD = IDD_SPELL_SELECTOR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedRadio5();
	afx_msg void OnBnClickedClericCheck();
	afx_msg void OnBnClickedDruidCheck();
	afx_msg void OnBnClickedMagicUserCheck();
	afx_msg void OnBnClickedIllusionistCheck();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadio6();
	afx_msg void OnBnClickedRadio7();
	afx_msg void OnBnClickedRadio8();
	afx_msg void OnBnClickedRadio9();
	BOOL m_bClericCheck;
	BOOL m_bDruidCheck;
	BOOL m_bMagicUserCheck;
	BOOL m_bIllusionistCheck;
	CListBox m_cSpellList;
	afx_msg void OnLbnSelchangeSpellList();
	CString m_szMultiEdit;
};
