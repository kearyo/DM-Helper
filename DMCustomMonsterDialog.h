#pragma once
#include "afxwin.h"


// CDMCustomMonsterDialog dialog

class CDMCustomMonsterDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMCustomMonsterDialog)

public:
	CDMCustomMonsterDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMCustomMonsterDialog();

	CDMHelperApp *m_pApp;
	cDNDMonsterManualEntry m_Monster;
	BOOL m_bChanged;

	void Populate();
	void SetButtons();
	void Refresh();
	void SaveCustomMonsters();


// Dialog Data
	enum { IDD = IDD_CUSTOM_MONSTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CListBox m_cMonsterList;
	CString m_szMonsterType;
	CComboBox m_cAlignmentCombo;
	afx_msg void OnBnClickedNewMonsterButton();
	CComboBox m_cDescCombo;
	afx_msg void OnBnClickedDeleteMonsterButton();
	afx_msg void OnBnClickedSaveMonsterButton();
	afx_msg void OnEnChangeMonsterTypeEdit();
	afx_msg void OnCbnSelchangeCharAlignmentCombo();
	CString m_szAttack;
	CString m_szDefense;
	afx_msg void OnEnChangeMonsterAttackEdit();
	afx_msg void OnEnChangeMonsterDefenseEdit();
	CString m_szMagicResistance;
	afx_msg void OnCbnSelchangeMagicResistCombo();
	CString m_szAC;
	CString m_szHD;
	afx_msg void OnEnChangeMonsterAcEdit();
	afx_msg void OnEnChangeMonsterHdEdit();
	CButton m_cDebugButton;
	afx_msg void OnBnClickedDebugButton();
	CString m_szInt;
	CComboBox m_cIntCombo;
	afx_msg void OnCbnSelchangeIntCombo();
	CString m_szDesc;
	afx_msg void OnCbnEditchangeDescCombo();
	afx_msg void OnCbnSelchangeDescCombo();
	afx_msg void OnCbnEditchangeIntCombo();
	afx_msg void OnCbnEditchangeMagicResistCombo();
	afx_msg void OnLbnSelchangeMonsterList();
	CString m_szSize;
	afx_msg void OnEnChangeMonsterSizeEdit();
	CString m_szAttacks;
	CString m_szDamage;
	afx_msg void OnEnChangeMonsterAttacksEdit();
	afx_msg void OnEnChangeMonsterDamageEdit();
	CString m_szMove;
	afx_msg void OnEnChangeMonsterAcEdit2();
	CComboBox m_cMagicResistanceCombo;
	CButton m_cNewMonsterButton;
	CButton m_cDeleteMonsterButton;
	CButton m_cSaveMonsterButton;
	CString m_szTreasureType;
	afx_msg void OnEnChangeMonsterTreasureTypeEdit();
};
