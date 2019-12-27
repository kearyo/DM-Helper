#pragma once
#include "afxwin.h"


// cDMMaterialComponentsDialog dialog

class cDMMaterialComponentsDialog : public CDialog
{
	DECLARE_DYNAMIC(cDMMaterialComponentsDialog)

public:
	cDMMaterialComponentsDialog(PDNDCHARACTER pCharacter, PSPELL pSpell, int nMultiples, std::vector<POBJECTINDEXER> *pSpellMaterialComponentsRequiredVector, BOOL bCheckComponents, BOOL bGetInfo, DND_SPELL_MATERIAL_RETURN_CODES *pnReturnCode, CWnd* pParent = NULL);   // standard constructor
	virtual ~cDMMaterialComponentsDialog();

	CDMHelperApp *m_pApp;
	PDNDCHARACTER m_pCharacter;
	PSPELL m_pSpell;
	int m_nMultiples;
	std::vector<POBJECTINDEXER> *m_pSpellMaterialComponentsRequiredVector;
	BOOL m_bCheckComponents;
	BOOL m_bGetInfo;
	DND_SPELL_MATERIAL_RETURN_CODES *m_pnReturnCode;

// Dialog Data
	enum { IDD = IDD_SPELL_COMPONENTS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	CString m_szComponents;
	CString m_szComponentsLine;
	CButton m_cCastButton;
	CButton m_cCancelButton;
	afx_msg void OnStnClickedComponentsList();
	afx_msg void OnBnClickedBuyComponentsButton();
	CButton m_cBuyComponentsButton;
	CButton m_cGiveComponentsButton;
	afx_msg void OnBnClickedGiveComponentsButton();
	CButton m_cGiveAllComponentsButton;
	CButton m_cBuyAllComponentsButton;
	afx_msg void OnBnClickedBuyAllComponentsButton();
	afx_msg void OnBnClickedGiveAllComponentsButton();
};
