#pragma once
#include "afxwin.h"


// CDMCustomWeaponDialog dialog

class CDMCustomWeaponDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMCustomWeaponDialog)

public:
	CDMCustomWeaponDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMCustomWeaponDialog();

	CDMHelperApp *m_pApp;
	BOOL m_bChanged;

	cDNDWeapon m_Weapon;

	char * PlusMinus(int nVal);
	void ClearWeapon();
	void CheckSaveChangedWeapon();
	void SaveCustomWeapon();
	void PopulateWeaponList();
	void PopulateAmmoList();
	void InitView();
	void Refresh();

// Dialog Data
	enum { IDD = IDD_CUSTOM_WEAPON_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CComboBox m_cCoinCombo;
	CString m_szWeaponName;
	CString m_szEncumbrance;
	CString m_szCost;
	BOOL m_bMissile;
	BOOL m_bAmmo;
	BOOL m_bSword;
	afx_msg void OnBnClickedMissileCheck();
	CStatic m_cAmmoLabel;
	CComboBox m_cAmmoCombo;
	afx_msg void OnBnClickedAmmoCheck();
	afx_msg void OnBnClickedSwordCheck();
	CButton m_cAmmo;
	CButton m_cSword;
	CListBox m_cWeaponList;
	CButton m_cDebugButton;
	afx_msg void OnBnClickedDebugButton();
	CString m_szXP;
	afx_msg void OnBnClickedNewWeaponButton();
	afx_msg void OnBnClickedDeleteWeaponButton();
	afx_msg void OnBnClickedSaveWeaponButton();
	BOOL m_bCleric;
	BOOL m_bMagicUser;
	BOOL m_bThief;
	CString m_szWeaponVsAC_0;
	CString m_szWeaponVsAC_1;
	CString m_szWeaponVsAC_2;
	CString m_szWeaponVsAC_3;
	CString m_szWeaponVsAC_4;
	CString m_szWeaponVsAC_5;
	CString m_szWeaponVsAC_6;
	CString m_szWeaponVsAC_7;
	CString m_szWeaponVsAC_8;
	CString m_szWeaponVsAC_9;
	CString m_szWeaponVsAC_10;
	CString m_szDamageSmallLow;
	CString m_szDamageSmallHigh;
	CString m_szDamageLargeLow;
	CString m_szDamageLargeHigh;
	CButton m_cExceptionalStrength;
	BOOL m_bExceptionalStrength;
	CString m_szSpaceRequired;
	CString m_szWeaponSpeed;
	afx_msg void OnLbnSelchangeWeaponList();
	CButton m_cDebug2Button;
	afx_msg void OnBnClickedDebugButton2();
	afx_msg void OnCbnSelchangeAmmoCombo();
	CButton m_cDebug3Button;
	afx_msg void OnBnClickedDebugButton3();
	CString m_szDebugString;
	CButton m_cDeleteWeaponButton;
	CButton m_cSaveWeaponButton;
	afx_msg void OnEnChangeWeaponNameEdit();
	afx_msg void OnEnChangeWeaponEncumbranceEdit();
	afx_msg void OnEnChangeWeaponCostEdit();
	afx_msg void OnCbnSelchangeCoinCombo();
	afx_msg void OnEnChangeWeaponXpEdit();
	afx_msg void OnEnChangeWeaponVsAcEdit0();
	afx_msg void OnEnChangeWeaponVsAcEdit1();
	afx_msg void OnEnChangeWeaponVsAcEdit2();
	afx_msg void OnEnChangeWeaponVsAcEdit3();
	afx_msg void OnEnChangeWeaponVsAcEdit4();
	afx_msg void OnEnChangeWeaponVsAcEdit5();
	afx_msg void OnEnChangeWeaponVsAcEdit6();
	afx_msg void OnEnChangeWeaponVsAcEdit7();
	afx_msg void OnEnChangeWeaponVsAcEdit8();
	afx_msg void OnEnChangeWeaponVsAcEdit9();
	afx_msg void OnEnChangeWeaponVsAcEdit10();
	afx_msg void OnEnChangeDamageSmallLow();
	afx_msg void OnEnChangeDamageSmallHigh();
	afx_msg void OnEnChangeDamageLargeLow();
	afx_msg void OnEnChangeDamageLargeHigh();
	afx_msg void OnEnChangeSpaceReqd();
	afx_msg void OnEnChangeWeaponSpeed();
	afx_msg void OnBnClickedClericCheck();
	afx_msg void OnBnClickedMagicuserCheck();
	afx_msg void OnBnClickedThiefCheck();
	afx_msg void OnBnClickedExceptStrCheck();
	CButton m_cDebug4Button;
	afx_msg void OnBnClickedDebugButton4();
	CButton m_cCleric;
	CButton m_cMagicUser;
	CButton m_cThief;
};
