#pragma once
#include "afxwin.h"


// CDMCustomSpellDialog dialog

class CDMCustomSpellDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMCustomSpellDialog)

public:
	CDMCustomSpellDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMCustomSpellDialog();

	CDMHelperApp *m_pApp;

	cDNDSpell	m_Spell;

	BOOL m_bChanged;

	int m_nSpellLevel;
	DND_CHARACTER_CLASSES m_nSpellBook;

	CButton* m_pClericButton;
	CButton* m_pDruidButton;
	CButton* m_pMagicUserButton;
	CButton* m_pIllusionistButton;
	CButton* m_pAllButton;

	void Populate();
	void Refresh();
	void SaveCustomSpells();

// Dialog Data
	enum { IDD = IDD_CUSTOM_SPELL_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_cTypeList;
	CString m_szSpellDescription;
	afx_msg void OnBnClickedClericRadio();
	afx_msg void OnBnClickedSaveSpellButton();
	afx_msg void OnBnClickedDruidRadio();
	afx_msg void OnBnClickedMagicUserRadio();
	afx_msg void OnBnClickedIllusionistRadio();
	CComboBox m_cSpellLevels;
	afx_msg void OnCbnSelchangeLevelCombo();
	CString m_szSpellName;
	CListBox m_cSpellList;
	CString m_szSpellType;
	BOOL m_bVerbalComponent;
	BOOL m_bSomaticComponent;
	BOOL m_bMaterialComponent;
	CButton m_cDebugButton;
	afx_msg void OnBnClickedDebugSpellButton();
	CString m_szSpellRange;
	CString m_szSpellCastingTime;
	CString m_szSpellDuration;
	CString m_szSpellSavingThrow;
	CString m_szSpellAreaOfEffect;
	BOOL m_bReversible;
	afx_msg void OnLbnSelchangeSpellList();
	CButton m_cSaveButton;
	afx_msg void OnEnChangeSpellNameEdit();
	afx_msg void OnCbnSelchangeTypeCombo();
	afx_msg void OnCbnEditchangeTypeCombo();
	afx_msg void OnBnClickedReversibleCheck();
	afx_msg void OnBnClickedVerbalCheck();
	afx_msg void OnBnClickedSomaticCheck();
	afx_msg void OnEnChangeSpellRangeEdit();
	afx_msg void OnEnChangeSpellCastingTimeEdit();
	afx_msg void OnEnChangeSpellDurationEdit();
	afx_msg void OnEnChangeSpellSavingThrowEdit();
	afx_msg void OnEnChangeSpellAreaOfEffectEdit();
	afx_msg void OnEnChangeSpellDescriptionEdit();
	afx_msg void OnBnClickedNewSpellButton();
	CButton m_cDeleteButton;
	afx_msg void OnBnClickedDeleteSpellButton();
	afx_msg void OnBnClickedAllRadio();
};
