#include "afxwin.h"
#if !defined(AFX_DMCHARVIEWDIALOG_H__6D7192DC_E853_4EE7_BB61_9B0F6C4477A6__INCLUDED_)
#define AFX_DMCHARVIEWDIALOG_H__6D7192DC_E853_4EE7_BB61_9B0F6C4477A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMCharViewDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDMCharViewDialog dialog

typedef enum
{
	DND_CHAR_VIEW_TYPE_UNDEF = 0,
	DND_CHAR_VIEW_TYPE_PC,
	DND_CHAR_VIEW_TYPE_NPC

} DND_CHAR_VIEW_TYPES;

class DMCharSheetDialog;
class DMHPEditDialog;
class DMInventoryDialog;
class DMCharSpellsDialog;

class CDMBaseCharViewDialog : public CDialog
{

public:

	DND_CHAR_VIEW_TYPES m_CharViewType;

	CDMHelperApp *m_pApp;

	CDMHelperDlg* m_pMainDialog;

	DMInventoryDialog *m_pInventoryDialog;
	DMCharSpellsDialog *m_pSpellsDialog;

	int m_nSelectedSpellViewClass;
	BOOL m_bFirstSpellViewRefresh;

	CString m_szDamageDesc;
	CString m_szToHitDesc;

	int m_nAttackMatrix[21];
	CString m_szNumAttacks;

	int m_nInitiativeRoll;
	int m_nInitiativeSegment;
	int m_nDexAdjToInitiative;
	int m_nCastingSpellSegments;
	int m_nCastSpellCursorPos;
	BOOL m_bInitiativeCasting;

	CString m_szInitiativeAction;
	CString m_szBaseCharName;

	BOOL m_bInitDialogCalled;

	CDMBaseCharViewDialog *m_pTargetBaseDlg;

	cDNDObject m_RecoverAmmoInventory[MAX_CHARACTER_INVENTORY];

	CString m_szMonsterPortraitPath;
	BOOL GetMonsterPortraitPath(CString szMonster);

	CString m_szCharacterFirstName;
	CString m_szMonsterManualName;

	BOOL m_bHasBreathWeapon;

	CDMBaseCharViewDialog(DND_CHAR_VIEW_TYPES _ViewType, UINT nIDTemplate, CWnd* pParentWnd)
	{
		m_pApp = (CDMHelperApp *)AfxGetApp();

		m_pMainDialog = NULL;

		m_pInventoryDialog = NULL;
		m_pSpellsDialog = NULL;

		m_CharViewType = _ViewType;
		
		m_szDamageDesc = _T("");
		m_szToHitDesc = _T("-");
		m_szNumAttacks = _T("");

		m_szInitiativeAction = _T("");
		m_szBaseCharName = _T("");

		m_nInitiativeRoll = 9;
		m_nInitiativeSegment = 0;

		m_nDexAdjToInitiative = 0;

		m_nCastingSpellSegments = 0;
		m_nCastSpellCursorPos = -1;

		m_bInitiativeCasting = FALSE;

		m_pTargetBaseDlg = NULL;

		m_bInitDialogCalled = FALSE;

		m_szMonsterPortraitPath = _T("");

		m_szCharacterFirstName = _T("");
		m_szMonsterManualName = _T("");

		m_bHasBreathWeapon = FALSE;

		CDialog::CDialog(nIDTemplate, pParentWnd);
	}

	~CDMBaseCharViewDialog();

	DND_CHAR_VIEW_TYPES	GetType()
	{
		return m_CharViewType;
	}

	void CloseSubWindows()
	{
		if(m_pInventoryDialog != NULL)
		{
			m_pInventoryDialog->m_bParentClosing = TRUE;
			m_pInventoryDialog->PostMessage(WM_CLOSE);
			m_pInventoryDialog = NULL;
		}

		if(m_pSpellsDialog != NULL)
		{
			m_pSpellsDialog->m_pSiblingWindow = NULL;
			m_pSpellsDialog->PostMessage(WM_CLOSE);
			m_pSpellsDialog = NULL;
		}
	}

	int GetAttacksPerRound(int nCurrentRound)
	{
		int nNum = 0;
		int nDem = 0;

		int nNumAttacks = 0;

		sscanf(m_szNumAttacks.GetBuffer(0), "%d/%d", &nNum, &nDem);

		if (nDem == 0)
		{
			nDem = 1;
		}

		if (nDem > 1)
		{
			if (nCurrentRound % 2 == 0)
			{
				nNumAttacks = (nNum / nDem) + 1;
			}
			else
			{
				nNumAttacks = (nNum % nDem);
			}
		}
		else
		{
			nNumAttacks = nNum;
		}

		return nNumAttacks;
	}

};

class CDMCharViewDialog : public CDMBaseCharViewDialog
{
// Construction
public:
	CDMCharViewDialog(CDMHelperDlg* pMainDialog, cDNDCharacter	*_pCharacter, CWnd* pParent = NULL);   // standard constructor

	CWnd* m_pParent;

	//CDMHelperApp *m_pApp;

	BOOL m_bLoadedFromMenu;

	CString m_szSaveFileName;
	CString m_szLoadFileName;

	cDNDCharacter	*m_pCharacter;

	DMCharSheetDialog *m_pCharSheetDialog;
	DMHPEditDialog *m_pHPEditDialog;

	CString m_szHPByLevelBuffer;

	CString m_szSTRCommentBuffer;
	CString m_szINTCommentBuffer;
	CString m_szWISCommentBuffer;
	CString m_szDEXCommentBuffer;
	CString m_szCONCommentBuffer;
	CString m_szCHACommentBuffer;

	void MateToParentWindow();

	void ProcessCharStats();

	void RollHitpoints();

	void Refresh();

	cDNDCharacter *LoadCharacter();
	void InitClassListBox();
	void PrePositionWindow();
	BOOL LoadCharacterFromFile(char *szFileName);
	void SaveExternal();
	void SaveCharacterToFile(char *szFileName);
	void SwapWeapon();
	void FireAmmo();
	void AddToRecoveryInventory(int nCharInventorySlot, int nAmount);
	void ValidateRecoveryInventory();
	void CollapseRecoveryInventory();
	void NPCRollStats();

	BOOL m_bHasPainted;
	int m_nPage;
	int m_nLastPage;

	int m_nDayofWeek;
	int m_nDayofMonth;
	int m_nMonth;
	int m_nYear;

	float m_fXPScratchPad;

	int m_nCharacterMovementRate;
	int m_nCalculatedXPValue;

	BOOL m_bReversedMapIcon;

	CString m_szRaceSex;
	CString m_szClassList;
	CString m_szLevelList;
	CString m_szDamageStat;
	CString m_szCharStats;


// Dialog Data
	//{{AFX_DATA(CDMCharViewDialog)
	enum { IDD = IDD_CHAR_VIEW_DIALOG };
	CButton	m_cNPCCheck;
	CButton	m_cMonsterCheck;
	CComboBox	m_cRingRightCombo;
	CComboBox	m_cRingLeftCombo;
	CComboBox	m_cRecoverAmmoCombo;
	CButton	m_cSpellsButton;
	CComboBox	m_cAmmunitionCombo;
	CButton	m_cRollStats;
	CButton	m_cAnchorButton;
	CComboBox	m_cWeaponCombo_4;
	CComboBox	m_cWeaponCombo_3;
	CComboBox	m_cWeaponCombo_2;
	CComboBox	m_cWeaponCombo_1;
	CEdit	m_cXPEdit3;
	CEdit	m_cXPEdit2;
	CEdit	m_cLevelEdit3;
	CEdit	m_cLevelEdit2;
	CEdit	m_cCurrentEdit;
	CComboBox	m_cShieldCombo;
	CEdit	m_cArmorClassEdit;
	CComboBox	m_cArmorCombo;
	CComboBox	m_cAlignmentCombo;
	CStatic	m_cSTRComment;
	CColoredListCtrl	m_cWeaponChartList;
	CEdit	m_cExceptionalStrengthEdit;
	CComboBox	m_cCharSexCombo;
	CEdit	m_cSTREdit;
	CEdit	m_cINTEdit;
	CEdit	m_cWISEdit;
	CEdit	m_cDEXEdit;
	CEdit	m_cCONEdit;
	CEdit	m_cCHAEdit;
	CEdit	m_cCOMEdit;
	CComboBox	m_cClassCombo_3;
	CComboBox	m_cClassCombo_2;
	CComboBox	m_cClassCombo_1;
	CComboBox	m_cCharRaceCombo;
	CString	m_szCharacterName;
	CString	m_szSTREdit;
	CString	m_szINTEdit;
	CString	m_szWISEdit;
	CString	m_szDEXEdit;
	CString	m_szCONEdit;
	CString	m_szCHAEdit;
	CString	m_szCOMEdit;
	CString	m_szExceptionalStrength;
	CString	m_szLevelEdit1;
	CString	m_szLevelEdit2;
	CString	m_szLevelEdit3;
	CString	m_szXPEdit1;
	CString	m_szXPEdit2;
	CString	m_szXPEdit3;
	CString	m_szHitPoints;
	CString	m_szCurrentHitPoints;
	CString	m_szHPByLevel;
	BOOL	m_bShowHPCheck;
	CString	m_szCONComment;
	CString	m_szDEXComment;
	CString	m_szSTRComment;
	CString	m_szArmorClass;
	CString	m_szArmorPlus;
	CString	m_szShieldPlus;
	CString	m_szINTComment;
	CString	m_szSaves_1;
	CString	m_szSaves_2;
	CString	m_szSaves_3;
	CString	m_szSaves_4;
	CString	m_szSaves_5;
	CString	m_szWISComment;
	CString	m_szCHAComment;
	CString	m_szThiefSkill_1;
	CString	m_szThiefSkill_2;
	CString	m_szThiefSkill_3;
	CString	m_szThiefSkill_4;
	CString	m_szThiefSkill_5;
	CString	m_szThiefSkill_6;
	CString	m_szThiefSkill_7;
	CString	m_szThiefSkill_8;
	CString	m_szWeaponPlus_1;
	CString	m_szWeaponPlus_2;
	CString	m_szWeaponPlus_3;
	CString	m_szWeaponPlus_4;
	CString	m_szCharNameStatic;
	CString	m_szAmmoPlus;
	CString	m_szSpellsRemaining;
	CString	m_szRingLeftPlusEdit;
	CString	m_szRingRightPlusEdit;
	CString	m_szAssEdit1;
	CString	m_szAssEdit2;
	CString	m_szAssEdit3;
	CString	m_szAssEdit4;
	CString	m_szAssEdit5;
	CString	m_szAssEdit6;
	CString	m_szAssEdit7;
	CString	m_szAssEdit8;
	CString	m_szAssEdit10;
	CString	m_szAssEdit9;
	BOOL	m_bNPCCheck;
	BOOL	m_bMonsterCheck;
	CString	m_szRaceStatic;
	CString	m_szMove;
	CString	m_szSpecialAttackComment;
	CString	m_szSpecialDefenseComment;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDMCharViewDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDMCharViewDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnRollStats();
	afx_msg void OnChangeCharNameEdit();
	afx_msg void OnSelchangeCharRaceCombo();
	afx_msg void OnSelchangeCharClassCombo1();
	afx_msg void OnSelchangeCharClassCombo2();
	afx_msg void OnSelchangeCharClassCombo3();
	afx_msg void OnKillfocusCharSexCombo();
	afx_msg void OnChangeXpEdit1();
	afx_msg void OnChangeXpEdit2();
	afx_msg void OnChangeXpEdit3();
	afx_msg void OnChangeLevelEdit1();
	afx_msg void OnChangeConEdit();
	afx_msg void OnChangeStrEdit();
	afx_msg void OnShowHpCheck();
	afx_msg void OnChangeIntEdit();
	afx_msg void OnChangeWisEdit();
	afx_msg void OnChangeDexEdit();
	afx_msg void OnChangeChaEdit();
	afx_msg void OnChangeComEdit();
	afx_msg void OnSetfocusHpEdit();
	afx_msg void OnChangeExStrEdit();
	afx_msg void OnCharSheet();
	afx_msg void OnSelchangeCharAlignmentCombo();
	afx_msg void OnClose();
	afx_msg void OnSelchangeCharArmorCombo();
	afx_msg void OnSelchangeCharShieldCombo();
	afx_msg void OnChangeArmorPlusEdit();
	afx_msg void OnChangeShieldPlusEdit();
	afx_msg void OnSave();
	afx_msg void OnLoad();
	afx_msg void OnSelchangeCharSexCombo();
	afx_msg void OnKillfocusExStrEdit();
	afx_msg void OnSetfocusAcEdit();
	afx_msg void OnChangeLevelEdit3();
	afx_msg void OnChangeLevelEdit2();
	afx_msg void OnAddXpButton1();
	afx_msg void OnAddXpButton2();
	afx_msg void OnAddXpButton3();
	afx_msg void OnSelchangeWeaponCombo1();
	afx_msg void OnSelchangeWeaponCombo2();
	afx_msg void OnSelchangeWeaponCombo3();
	afx_msg void OnSelchangeWeaponCombo4();
	afx_msg void OnChangeWeaponPlusEdit1();
	afx_msg void OnChangeWeaponPlusEdit2();
	afx_msg void OnChangeWeaponPlusEdit3();
	afx_msg void OnChangeWeaponPlusEdit4();
	afx_msg void OnPaint();
	afx_msg void OnSwapButton1();
	afx_msg void OnSwapButton2();
	afx_msg void OnSwapButton3();
	afx_msg void OnCharInventory();
	afx_msg void OnAddHpButton();
	afx_msg void OnRecoverAmmo();
	afx_msg void OnChangeAmmoPlusEdit();
	afx_msg void OnSelchangeAmmoCombo();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnSpellsButton();
	afx_msg void OnSelchangeCharRingLCombo();
	afx_msg void OnSelchangeCharRingRCombo();
	afx_msg void OnChangeRingLPlusEdit();
	afx_msg void OnChangeRingRPlusEdit();
	afx_msg void OnNpcCheck();
	afx_msg void OnMonsterCheck();
	afx_msg void OnSelchangeCharHelmCombo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cHelmCombo;
	CString m_szEarnedXP;
	afx_msg void OnBnClickedAddExpButton();
	afx_msg void OnEnChangeExpEdit();
	CButton m_cCurrentClassCheck1;
	CButton m_cCurrentClassCheck2;
	CButton m_cCurrentClassCheck3;
	afx_msg void OnBnClickedCurrentClassCheck1();
	afx_msg void OnBnClickedCurrentClassCheck2();
	afx_msg void OnBnClickedCurrentClassCheck3();
	CEdit m_cLevelEdit1;
	CEdit m_cXPEdit1;
	CString m_szXPValueComment;
	CString m_szXPValue;
	afx_msg void OnEnChangeExpValueEdit();
	afx_msg void OnStnClickedSwapButton1();
	CString m_szLanguages;
	CTransparentListBox m_cLanguagesListBox;
	afx_msg void OnLbnDblclkLanguagesListBox();
	afx_msg void OnLbnSelchangeLanguagesListBox();
	CString m_szTurnUndead1;
	CString m_szTurnUndead2;
	CString m_szTurnUndead3;
	CString m_szTurnUndead4;
	CString m_szTurnUndead5;
	CString m_szTurnUndead6;
	CString m_szTurnUndead7;
	CString m_szTurnUndead8;
	CString m_szTurnUndead9;
	CString m_szTurnUndead10;
	CString m_szTurnUndead11;
	CString m_szTurnUndead12;
	CString m_szTurnUndead13;
	CComboBox m_cVisionCombo;
	afx_msg void OnCbnSelchangeCharVisionCombo();
	BOOL m_bKeenEar;
	afx_msg void OnBnClickedKeenEaredCheck();
	CComboBox m_cSecondarySkillCombo;
	BOOL m_bVeryKeenEar;
	afx_msg void OnBnClickedKeenEaredCheck2();
	CString m_szSecondarySkill;
	afx_msg void OnCbnEditchangeSecondarySkillCombo();
	afx_msg void OnCbnSelchangeSecondarySkillCombo();
	CString m_szConcealedMove;
	CString m_szClimbingMove;
	CString m_szSpecialMove;
	afx_msg void OnEnChangeConcealedMoveEdit();
	afx_msg void OnEnChangeClimbingMoveEdit();
	afx_msg void OnEnChangeSpecialMoveEdit();
	CComboBox m_cLightSourceCombo;
	afx_msg void OnCbnSelchangeLightSourceCombo();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_DMCHARVIEWDIALOG_H__6D7192DC_E853_4EE7_BB61_9B0F6C4477A6__INCLUDED_)
