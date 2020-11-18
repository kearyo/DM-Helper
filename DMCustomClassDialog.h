#pragma once
#include "afxwin.h"


// CDMCustomClassDialog dialog

class CDMCustomClassDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMCustomClassDialog)

public:
	CDMCustomClassDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMCustomClassDialog();

	CDMHelperApp *m_pApp;

	cDNDCustomClass m_CustomClass;
	cDNDCustomClass *m_pSelectedClass;

	BOOL m_bChanged;

	void Update();
	void Refresh();

// Dialog Data
	enum { IDD = IDD_CUSTOM_CLASS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	CComboBox m_cClassNameCombo;
	CString m_szClassName;
	CString m_szHitDieType;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CString m_szBaseLevelXP[12];
	CString m_szMaxLevelXP[12];
	
	afx_msg void OnEnChangeValue();

	afx_msg void OnEnChangeHdLevelEdit1();
	CString m_szMaxChartLevelXP;
	CString m_szMaxChartLevel;

	CEdit m_cMaxLevelXP[12];
	afx_msg void OnBnClickedDebugButton();
	afx_msg void OnCbnEditchangeClassNameCombo();
	afx_msg void OnCbnSelchangeClassNameCombo();
	
	CString m_szMaxChartLevelHP;
	CString m_szMaxChartHPLevel;
	CComboBox m_cSavingThrowChartCombo;
	
	CComboBox m_cMagicUseClassCombo;
	CString m_szMinAttrib[6];
	BOOL m_bPreReqCheck[6];

	CString m_szMagicLevel;
	CComboBox m_cCombatChartCombo;
	
	BOOL m_bThiefSkills;
	CString m_szThiefLevel;
	CString m_szAssassinLevel;
	BOOL m_bAssassinSkills;
	CString m_szTurnUndeadLevel;
	BOOL m_bTurnUndead;
	CString m_szInitialWeaponProf;
	CString m_szAddWeaponProf;
	CString m_szAddWeaponProfLevels;
	CString m_szNonWeaponProfPenalty;
	CEdit m_cTurnUndeadLevel;
	CEdit m_cThiefLevel;
	CEdit m_cAssassinLevel;
	CStatic m_cDexComment;
	CString m_szLevelHD[12];
	CButton m_cDebugButton;
	afx_msg void OnEnChangeHdTypeEdit();
	CEdit m_cMagicUseLevel;
	CEdit m_cWeaponSpecializationLevel;
	CString m_szWeaponSpecializationLevel;
	afx_msg void OnBnClickedWeaponSpecializationCheck();
	BOOL m_bWeaponSpecialization;
	afx_msg void OnEnChangeWeaponSpecializationLevelEdit();
};
