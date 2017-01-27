#pragma once
#include "afxwin.h"


// CDMCustomItemsDialog dialog

class CDMCustomItemsDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMCustomItemsDialog)

public:
	CDMCustomItemsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMCustomItemsDialog();

	CDMHelperApp *m_pApp;
	cDNDObject	m_Object;
	BOOL m_bChanged;

	void CheckSaveChangedObject();
	void ClearObject();
	void SaveCustomItems();
	void InitView();
	void Refresh();

// Dialog Data
	enum { IDD = IDD_CUSTOM_ITEM_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	CListBox m_cItemList;
	CString m_szItemName;
	CComboBox m_cCoinCombo;
	afx_msg void OnBnClickedNewItemButton();
	afx_msg void OnBnClickedSaveItemButton();
	CString m_szEncumbrance;
	CString m_szCost;
	afx_msg void OnLbnSelchangeItemList();
	afx_msg void OnBnClickedContainerCheck();
	BOOL m_bContainer;
	afx_msg void OnBnClickedDeleteItemButton();
	CButton m_cDeleteItem;
	BOOL m_bRations;
	afx_msg void OnBnClickedRationsCheck();
	CString m_szXP;
	afx_msg void OnEnChangeItemNameEdit();
	afx_msg void OnEnChangeItemEncumbranceEdit();
	afx_msg void OnEnChangeItemCostEdit();
	afx_msg void OnEnChangeItemXpEdit();
	afx_msg void OnCbnSelchangeCoinCombo();
	CButton m_cSaveButton;
};
