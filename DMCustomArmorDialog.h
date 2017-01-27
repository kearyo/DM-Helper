#pragma once
#include "afxwin.h"


// CDMCustomArmorDialog dialog

class CDMCustomArmorDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMCustomArmorDialog)

public:
	CDMCustomArmorDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMCustomArmorDialog();

	CDMHelperApp *m_pApp;
	BOOL m_bChanged;

	cDNDArmor m_Armor;

	void CheckSaveChangedArmor();
	void ClearArmor();
	void SaveCustomArmor();
	void InitView();
	void Refresh();

// Dialog Data
	enum { IDD = IDD_CUSTOM_ARMOR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_cArmorList;
	CString m_szArmorName;
	CComboBox m_cArmorClassCombo;
	afx_msg void OnEnChangeArmorNameEdit();
	afx_msg void OnCbnSelchangeArmorClassCombo();
	afx_msg void OnLbnSelchangeArmorList();
	CComboBox m_cCoinCombo;
	afx_msg void OnCbnSelchangeCoinCombo();
	afx_msg void OnEnChangeArmorEncumbranceEdit();
	CString m_szEncumbrance;
	CString m_szCost;
	afx_msg void OnEnChangeArmorCostEdit();
	CString m_szXP;
	afx_msg void OnBnClickedThiefCheck();
	afx_msg void OnBnClickedMetalCheck();
	afx_msg void OnBnClickedNewArmorButton();
	afx_msg void OnBnClickedDeleteArmorButton();
	afx_msg void OnBnClickedSaveArmorButton();
	BOOL m_bThiefUsable;
	BOOL m_bMetalArmor;
	CButton m_cDeleteArmor;
	CComboBox m_cMoveCombo;
	CString m_szMovement;
	CButton m_cSaveArmor;
	afx_msg void OnEnChangeArmorXpEdit();
	afx_msg void OnBnClickedCancel();
};
