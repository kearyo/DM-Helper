#pragma once
#include "afxwin.h"



// cDMMagicSwordPropertiesDialog dialog

class cDMMagicSwordPropertiesDialog : public CDialog
{
	DECLARE_DYNAMIC(cDMMagicSwordPropertiesDialog)

public:
	cDMMagicSwordPropertiesDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~cDMMagicSwordPropertiesDialog();

	CDMHelperApp *m_pApp;

	DMInventoryDialog *m_pParentWindow;
	POBJECTTYPE m_pSelectedInventoryObject;

	void Refresh();
	void MarkChanged();

// Dialog Data
	enum { IDD = IDD_MAGIC_SWORD_PROPERTIES_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CComboBox m_cAlignmentCombo;
	CString m_szIntelligenceEdit;
	CString m_szEgoEdit;
	afx_msg void OnEnChangeIntEdit();
	afx_msg void OnCbnSelchangeAlignmentCombo();
	CString m_szPrimaryAbility1;
	CString m_szPrimaryAbility2;
	CString m_szPrimaryAbility3;
	CString m_szExceptionalPower;
	CString m_szSpecialPurpose;
	CString m_szSpecialPurposePower;
	BOOL m_bSemiEmpath;
	BOOL m_bEmpath;
	BOOL m_bSpeech;
	BOOL m_bTelepath;
	afx_msg void OnEnSetfocusLanguagesEdit();
	CButton m_cOK;
	CEdit m_cIntelligenceEdit;
	CString m_szLanguages;
	afx_msg void OnEnChangeLanguagesEdit();
	CEdit m_cLanguagesEdit;
	afx_msg void OnBnClickedSelectLanguagesButton();
	afx_msg void OnBnClickedPrimaryAbilityButton1();
	afx_msg void OnBnClickedPrimaryAbilityButton2();
	afx_msg void OnBnClickedPrimaryAbilityButton3();
	afx_msg void OnBnClickedExceptionalPowerButton();
	afx_msg void OnBnClickedSpecialPurposeButton();
	afx_msg void OnBnClickedSpecialPurposePowerButton();
	afx_msg void OnPaint();
	afx_msg void OnEnChangePrimaryAbilityEdit1();
	afx_msg void OnEnChangePrimaryAbilityEdit2();
	afx_msg void OnEnChangePrimaryAbilityEdit3();
	afx_msg void OnEnChangeExceptionalPowerEdit();
	afx_msg void OnEnChangeSpecialPurposeEdit();
	afx_msg void OnEnChangeSpecialPurposePowerEdit();
	afx_msg void OnBnClickedSemiEmpathCheck();
	afx_msg void OnBnClickedEmpathCheck();
	afx_msg void OnBnClickedSpeechCheck();
	afx_msg void OnBnClickedTelepathCheck();
};
