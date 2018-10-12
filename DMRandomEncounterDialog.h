#pragma once
#include "afxwin.h"

class cDNDParty;

// CDMRandomEncounterDialog dialog

class CDMRandomEncounterDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMRandomEncounterDialog)

public:
	CDMRandomEncounterDialog(cDNDParty *pParentParty, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMRandomEncounterDialog();
	
	CDMHelperApp *m_pApp;

	cDNDParty *m_pParentParty;

	cDNDParty *m_pParty;

	cDNDRandomEncounterTable	m_DungeonTable;
	cDNDRandomEncounterTable	*m_pSelectedTable;
	cDNDRandomEncounterTable	*m_pOldSelectedTable;

	BOOL m_bOldUseFiendFolio;
	BOOL m_bOldUseMonsterManualII;

	void Init();
	void Refresh();

// Dialog Data
	enum { IDD = IDD_RANDOM_ENCOUNTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CEdit m_cEncounterEdit;
	CButton m_cOKButton;
	afx_msg void OnBnClickedRollButton();
	afx_msg void OnBnClickedAddRollButton();
	afx_msg void OnBnClickedClearButton();
	CListBox m_cEncounterList;
	CString m_szRolled;
	CComboBox m_cLevelCombo;
	CString m_szLevelLabel;
	CComboBox m_cTableCombo;
	afx_msg void OnCbnSelchangeTableCombo();
	CEdit m_cForceRoll;
	CString m_szForceRoll;
	BOOL m_bUseFiendFolio;
	BOOL m_bUseMonsterManualII;
	afx_msg void OnBnClickedFiendFolioCheck();
	afx_msg void OnBnClickedMonsterManualIiCheck();
	CString m_szNumLimit;
	CEdit m_cNumLimit;
	afx_msg void OnBnClickedDmgButton();
};
