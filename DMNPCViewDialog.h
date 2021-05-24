#include "afxwin.h"
#if !defined(AFX_DMNPCVIEWDIALOG_H__815DCBE0_675F_4385_81A0_369C818FAABD__INCLUDED_)
#define AFX_DMNPCVIEWDIALOG_H__815DCBE0_675F_4385_81A0_369C818FAABD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMNPCViewDialog.h : header file
//

class CDMBaseCharViewDialog;

class DMInventoryDialog;
class DMCharSpellsDialog;

// cDMBaseNPCViewDialog dialog

class cDMBaseNPCViewDialog : public CDMBaseCharViewDialog
{
	/* DECLARE_DYNAMIC(cDMBaseNPCViewDialog) */

public:
	cDMBaseNPCViewDialog(CDMHelperDlg* pMainDialog, cDNDNonPlayerCharacter *_pNPC, cDNDCacheInventory *_pCacheInventory, CWnd* pParent = NULL);    // standard constructor
	virtual ~cDMBaseNPCViewDialog();

	CDMHelperApp *m_pApp;

	CWnd* m_pParent;

	DWORD m_dwNPCCharacterID;
	cDNDNonPlayerCharacter *m_pNPC;
	cDNDCacheInventory *m_pCacheInventory;

	cDNDObject m_RecoverAmmoInventory[MAX_CHARACTER_INVENTORY];

	CString m_szMonsterName;
	int m_nBaseArmorClass;

	int m_nPage;
	int m_nLastPage;

	int m_nCharacterCopy;

	int m_nCharacterMovementRate;

	DWORD m_dwMapID;  // if this NPCViewDialog is a cache, this is the map it belongs to

	BOOL m_bReversedMapIcon;

	DMNPCViewDialog *m_pNPCViewDialog;

	void InitView();
	void GetRandomName(BOOL bRename);
	void OnRollStats() ;
	void ProcessCharStats();
	void Refresh();
	void CloseSubWindows();
	void FireAmmo();
	void SwapWeapon();

	CString GetMonsterSFXID();


// Dialog Data
	enum { IDD = IDD_BASE_NPC_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
};


/////////////////////////////////////////////////////////////////////////////
// DMNPCViewDialog dialog

class DMNPCViewDialog : public CDMBaseCharViewDialog
{
// Construction
public:
	DMNPCViewDialog(CDMHelperDlg* pMainDialog, cDNDNonPlayerCharacter *_pNPC, cDNDCacheInventory *_pCacheInventory, CWnd* pParent = NULL);    // standard constructor

	virtual ~DMNPCViewDialog();

	CDMHelperApp *m_pApp;

	CWnd* m_pParent;

	cDMBaseNPCViewDialog *m_pBaseNPCViewDialog;

	DWORD m_dwNPCCharacterID;
	cDNDNonPlayerCharacter *m_pNPC;
	cDNDCacheInventory *m_pCacheInventory;

	Bitmap* m_pNPCPortraitBackdropBitmap;
	Bitmap* m_pNPCPortraitBitmap;

	CString m_szMonsterName;
	int m_nBaseArmorClass;

	int m_nPage;
	int m_nLastPage;

	int m_nCharacterCopy;
	int m_nCharacterMovementRate;

	DWORD m_dwMapID;  // if this NPCViewDialog is a cache, this is the map it belongs to

	int m_nMonsterBook;		// for PDF lookup
	CString m_szMonsterBook; // for PDF lookup

	void FireAmmo();
	void SwapWeapon();
	void AddToRecoveryInventory(int nCharInventorySlot, int nAmount);
	void ValidateRecoveryInventory();
	void CollapseRecoveryInventory();

	void Refresh();
	void GetRandomName(BOOL bRename);
	void ProcessCharStats();

// Dialog Data
	//{{AFX_DATA(DMNPCViewDialog)
	enum { IDD = IDD_NPC_VIEW_DIALOG };
	CButton	m_cSpellsButton;
	CComboBox	m_cAmmunitionCombo;
	CComboBox	m_cWeaponCombo_4;
	CComboBox	m_cWeaponCombo_3;
	CComboBox	m_cWeaponCombo_2;
	CComboBox	m_cWeaponCombo_1;
	CComboBox	m_cRecoverAmmoCombo;
	CComboBox	m_cRingRightCombo;
	CComboBox	m_cRingLeftCombo;
	CComboBox	m_cShieldCombo;
	CComboBox	m_cArmorCombo;
	CListCtrl	m_cWeaponChartList;
	CComboBox	m_cAlignmentCombo;
	CComboBox	m_cCharSexCombo;
	CComboBox	m_cCharTypeCombo;
	CString	m_szCharacterName;
	CString	m_szHitPoints;
	CString	m_szCurrentHitPoints;
	CString	m_szArmorClass;
	CString	m_szShieldPlus;
	CString	m_szRingLeftPlusEdit;
	CString	m_szRingRightPlusEdit;
	CString	m_szWeaponPlus_1;
	CString	m_szWeaponPlus_2;
	CString	m_szWeaponPlus_3;
	CString	m_szWeaponPlus_4;
	CString	m_szAmmoPlus;
	CString	m_szArmorPlus;
	CString	m_szSaves_1;
	CString	m_szSaves_2;
	CString	m_szSaves_3;
	CString	m_szSaves_4;
	CString	m_szSaves_5;
	CString	m_szHDComment;
	CString	m_szIntComment;
	CString	m_szAttackComment;
	CString	m_szDefenseComment;
	CString	m_szMove;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DMNPCViewDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DMNPCViewDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCharInventory();
	afx_msg void OnSelchangeCharTypeCombo();
	afx_msg void OnChangeCharNameEdit();
	afx_msg void OnSelchangeCharAlignmentCombo();
	afx_msg void OnRollStats();
	afx_msg void OnSelchangeWeaponCombo1();
	afx_msg void OnSelchangeWeaponCombo2();
	afx_msg void OnSelchangeWeaponCombo3();
	afx_msg void OnSelchangeWeaponCombo4();
	afx_msg void OnSelchangeAmmoCombo();
	afx_msg void OnSelchangeRecoverAmmoCombo();
	afx_msg void OnChangeWeaponPlusEdit1();
	afx_msg void OnChangeWeaponPlusEdit2();
	afx_msg void OnChangeWeaponPlusEdit3();
	afx_msg void OnChangeWeaponPlusEdit4();
	afx_msg void OnChangeAmmoPlusEdit();
	afx_msg void OnSelchangeCharArmorCombo();
	afx_msg void OnSelchangeCharShieldCombo();
	afx_msg void OnChangeArmorPlusEdit();
	afx_msg void OnChangeShieldPlusEdit();
	afx_msg void OnSelchangeCharRingLCombo();
	afx_msg void OnSelchangeCharRingRCombo();
	afx_msg void OnChangeRingLPlusEdit();
	afx_msg void OnChangeRingRPlusEdit();
	afx_msg void OnSwapButton1();
	afx_msg void OnSwapButton2();
	afx_msg void OnSwapButton3();
	afx_msg void OnAddHpButton();
	afx_msg void OnRecoverAmmo();
	afx_msg void OnClose();
	afx_msg void OnSelchangeCharHelmCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString m_szXPComment;
	CComboBox m_cHelmCombo;
	CString m_szNumAttacksEdit;
	CString m_szMagicResist;
	afx_msg void OnCbnSelchangeCharSexCombo();
	CString m_szDescComment;
	CString m_szMonsterSize;
	CString m_szTreasureType;
	afx_msg void OnStnClickedNpcPortraitButton();
	afx_msg void OnStnDblclickDescComment();
	CStatic m_cDebugCharID;
	CString m_szDebugCharID;
	afx_msg void OnStnDblclickTtComment();
	afx_msg LRESULT OnDirtyWindow(UINT wParam, LONG lParam);
	afx_msg void OnBnClickedNameTransferButton();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DMNPCVIEWDIALOG_H__815DCBE0_675F_4385_81A0_369C818FAABD__INCLUDED_)


