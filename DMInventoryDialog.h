#include "afxwin.h"
#if !defined(AFX_DMINVENTORYDIALOG_H__E7FA7AA2_808A_46C0_9CBB_E1969B9F671E__INCLUDED_)
#define AFX_DMINVENTORYDIALOG_H__E7FA7AA2_808A_46C0_9CBB_E1969B9F671E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMInventoryDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DMInventoryDialog dialog

class CDMHelperDlg;
class CDMCharViewDialog;
class DMNPCViewDialog;
class CDMPCDetailsDialog;
class CDMScrollDetailsDialog;
class CDMContainerDetailsDialog;
class cDMMagicSwordPropertiesDialog;

class DMInventoryDialog : public CDialog
{
// Construction
public:
	DMInventoryDialog(CDMHelperDlg* pMainDialog, cDNDCharacter *_Character, CDMCharViewDialog *pPCVWindow, DMNPCViewDialog *pNPCVWindow, CWnd* pParent = NULL);   // standard constructor
	virtual ~DMInventoryDialog();

	void SelectMagicItems(BOOL bSelect);
	void InitDisplay();
	void AddContainerItemToList(DWORD dwContainerID, int *pnTotalItems, int nLevel);
	void CleanUp();
	void Refresh();
	void RefreshAll();
	void MarkCacheChanged(cDNDCharacter *pCacheCharacter);
	void DisplaySubWindows();
	void CheckItemSelect();

	CWnd* m_pParent;

	CDMHelperDlg* m_pMainDialog;

	BOOL m_bParentClosing;

	CDMCharViewDialog* m_pPCVWindow;
	DMNPCViewDialog* m_pNPCVWindow;

	cDNDCharacter *m_pCharacter;
	cDNDCharacter *m_pGiftCharacter;

	CDMHelperApp *m_pApp;

	CFont m_ListFont;

	DWORD m_dwInventoryFlag;	// flag for validity of object tables

	//int m_nSelectedInventorySlot;
	cDNDObject *m_pSelectedInventorySlot;

	CDMPCDetailsDialog *m_pPCDetailsDialog;
	CDMScrollDetailsDialog *m_pScrollDetailsDialog;
	CDMContainerDetailsDialog *m_pContainerDetailsDialog;
	cDMMagicSwordPropertiesDialog *m_pMagicSwordPropertiesDialog;

	int m_nDetailsPosX;
	int m_nDetailsPosY;

	BOOL m_bMagicItemSelect;

	BOOL m_bDisablePrompts;

// Dialog Data
	//{{AFX_DATA(DMInventoryDialog)
	enum { IDD = IDD_INVENTORY_DIALOG };
	CEdit	m_cRenameEdit;
	CListBox	m_cProffList;
	CComboBox	m_cGiveToCombo;
	CListBox	m_cInventoryList;
	CListBox	m_cStoreList;
	CString	m_szEncumbrance;
	CString	m_szMaxEncumbrance;
	CString	m_szGoldEdit;
	CString	m_szCopperEdit;
	CString	m_szPlatinumEdit;
	CString	m_szSilverEdit;
	CString	m_szElectrumEdit;
	BOOL	m_bPricesCheck;
	BOOL	m_bWeaponsCheck;
	BOOL	m_bArmorCheck;
	BOOL	m_bShieldsCheck;
	BOOL	m_bMiscWeaponsCheck;
	BOOL	m_bEquipCheck;
	CString	m_szTotalInvItems;
	CString	m_szProffStatic;
	CString	m_szNonProffStatic;
	CString	m_szRenameItem;
	BOOL	m_bRingsCheck;
	BOOL	m_bWeightsCheck;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DMInventoryDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DMInventoryDialog)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnGiveButton();
	afx_msg void OnBuyButton();
	afx_msg void OnGiveToButton();
	afx_msg void OnDropButton();
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeGpEdit();
	afx_msg void OnChangePpEdit();
	afx_msg void OnChangeEpEdit();
	afx_msg void OnChangeSpEdit();
	afx_msg void OnChangeCpEdit();
	afx_msg void OnStackCoinButton();
	afx_msg void OnOk();
	afx_msg void OnSelchangeInventoryList();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg void OnRadio5();
	afx_msg void OnDropAllButton();
	afx_msg void OnPricesCheck();
	afx_msg void OnWeaponsCheck();
	afx_msg void OnArmorCheck();
	afx_msg void OnShieldsCheck();
	afx_msg void OnMiscWeaponsCheck();
	afx_msg void OnEquipCheck();
	afx_msg void OnStackInvButton();
	afx_msg void OnAddSubButton();
	afx_msg void OnPaint();
	afx_msg void OnAddCustomButton();
	afx_msg void OnAddProff();
	afx_msg void OnRemoveProff();
	afx_msg void OnClose();
	afx_msg void OnSelchangeStoreList();
	afx_msg void OnRingsCheck();
	afx_msg void OnChangeRenameItemEdit();
	afx_msg void OnWeightsCheck();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_cInventoryStatic;
	CStatic m_cWPFrame;
	afx_msg void OnEnChangeCacheNameEdit();
	CString m_szCacheName;
	afx_msg void OnBnClickedMagicAdjButton();
	CButton m_cDetailsLocator;
	CEdit m_cItemXPEdit;
	CString m_szItemXP;
	CString m_szCacheDetails;
	afx_msg void OnEnChangeCacheDetailsEdit();
	CStatic m_cCachePropertiesFrame;
	afx_msg void OnEnChangeItemXpEdit();
	CButton m_cGiveWithXPCharacterCheck;
	BOOL m_bGiveWithXPCharacterCheck;
	CButton m_cGiveWithXPPartyCheck;
	BOOL m_bGiveWithXPPartyCheck;
	CButton m_cDropWithXPCharacterCheck;
	BOOL m_bDropWithXPCharacterCheck;
	CButton m_cDropWithXPPartyCheck;
	BOOL m_bDropWithXPPartyCheck;
	afx_msg void OnBnClickedGiveWithXpCharacterCheck();
	afx_msg void OnBnClickedGiveWithXpPartyCheck();
	afx_msg void OnBnClickedDropWithXpCharacterCheck();
	afx_msg void OnBnClickedDropWithXpPartyCheck();
	CString m_szCacheNameDesc;
	BOOL m_bPotionsCheck;
	afx_msg void OnBnClickedPotionsCheck();
	BOOL m_bMagicalArmorCheck;
	afx_msg void OnBnClickedMagicalArmorCheck();
	BOOL m_bMagicalSwordsCheck;
	afx_msg void OnBnClickedMagicalSwordsCheck();
	afx_msg void OnBnClickedRodsStavesWandsCheck();
	BOOL m_bRodsStavesWandsCheck;
	BOOL m_bMiscMagic1Check;
	afx_msg void OnBnClickedMiscMagic1Check();
	BOOL m_bMiscMagic2Check;
	afx_msg void OnBnClickedMiscMagic2Check();
	BOOL m_bMiscMagic3Check;
	BOOL m_bMiscMagic4Check;
	BOOL m_bMiscMagic5Check;
	afx_msg void OnBnClickedMiscMagic3Check();
	afx_msg void OnBnClickedMiscMagic4Check();
	afx_msg void OnBnClickedMiscMagic5Check();
	BOOL m_bMiscMagicSpecialCheck;
	afx_msg void OnBnClickedMiscMagicSpecialCheck();
	BOOL m_bXPCheck;
	afx_msg void OnBnClickedXpCheck();
	BOOL m_bScrollsCheck;
	afx_msg void OnBnClickedScrollsCheck();
	BOOL m_bShowContainerContents;
	afx_msg void OnBnClickedContainerContentsCheck();
	CEdit m_cCacheDetailsEdit;
	BOOL m_bItemIdentified;
	CButton m_cItemIdentified;
	afx_msg void OnBnClickedItemIdentifiedCheck();
	CString m_szInventoryLabel;
	CButton m_cTreasureTypeButton;
	afx_msg void OnBnClickedTreasureTypeButton();
	CButton m_cSpecializedCheck;
	CButton m_cDoubleSpecializedCheck;
	BOOL m_bDoubleSpecialized;
	BOOL m_bSpecialized;
	afx_msg void OnBnClickedSpecializedCheck();
	afx_msg void OnBnClickedDoubleSpecializedCheck();
	afx_msg void OnLbnSelchangeProffList();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CButton m_cItemInfoButton;
	afx_msg void OnBnClickedItemInfoButton();
	afx_msg void OnLbnDblclkStoreList();
	afx_msg void OnLbnDblclkInventoryList();
	CButton m_cGenerateTreasureButton;
	afx_msg void OnBnClickedGenTreasureButton();
	CButton m_cInLairCheck;
	BOOL m_bInLairCheck;
	afx_msg void OnBnClickedInLairCheck();
	BOOL m_bMaterialComponentCheck;
	afx_msg void OnBnClickedMagicComponentsCheck();
	CString m_szItemDesc;
	afx_msg void OnStnDblclickCacheNameStatic();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DMINVENTORYDIALOG_H__E7FA7AA2_808A_46C0_9CBB_E1969B9F671E__INCLUDED_)
