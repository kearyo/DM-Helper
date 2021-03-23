#include "afxwin.h"
#if !defined(AFX_DMPARTYDIALOG_H__F7594913_8F18_43D9_AE8F_0CF1FF1CFCEC__INCLUDED_)
#define AFX_DMPARTYDIALOG_H__F7594913_8F18_43D9_AE8F_0CF1FF1CFCEC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMPartyDialog.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// DMPartyDialog dialog

extern UINT DMPartyTimeThreadProc(LPVOID pData);
extern UINT DMPartyCalendarThreadProc(LPVOID pData);

class CDMCalendarDialog;
class CDMPartyLogDialog;
class CDMEventLoggerDialog;
class CDMInitiativeDialog;
class CDMSubPartyDialog;

class DMPartyDialog : public CDialog
{
// Construction
public:
	DMPartyDialog(CDMHelperDlg* pMainDialog, cDNDParty *pParty, CWnd* pParent = NULL);   // standard constructor
	virtual ~DMPartyDialog();

	CDMHelperApp *m_pApp;

	CWnd* m_pParent;
	CDMHelperDlg* m_pMainDialog;

	cDNDParty *m_pParty;
	cDNDPartyLog *m_pPartyLog;
	CString m_szPartySaveFilePath;

	cDNDSubParty *m_pSubParty;
	DWORD m_dwParentPartyID;
	DWORD m_dwSubPartyID;

	CDMBaseCharViewDialog *m_pBaseCharDlg;
	CDMCharViewDialog *m_pSelectedCharacterDialog;
	CDMCharViewDialog *m_pOldSelectedCharacterDialog;
	
	cDMBaseNPCViewDialog *m_pSelectedNPCDialog;
	cDMBaseNPCViewDialog *m_pOldSelectedNPCDialog;

	DMPartyDialog *m_pOpposingPartyDialog;
	cDNDSubParty *m_pOpposingSubParty;

	CDMBaseCharViewDialog *m_pBaseOpposingCharDlg;
	CDMCharViewDialog *m_pOpposingCharacterDialog;
	CDMCharViewDialog *m_pOldOpposingCharacterDialog;

	cDMBaseNPCViewDialog *m_pOpposingNPCDialog;
	cDMBaseNPCViewDialog *m_pOldOpposingNPCDialog;

	CDMCalendarDialog *m_pCalendarDialog;
	CDMPartyLogDialog *m_pPartyLogDialog;
	CDMEventLoggerDialog *m_pEventLoggerDialog;
	CDMInitiativeDialog *m_pInitiativeDialog;

	CDMSubPartyDialog *m_pSubPartyDialog;

	ULONG	m_lPartyHash;
	CString m_szSaveFileName;

	int m_nPartyScrollPos;
	int m_nOpponentPartyScrollPos;

	BOOL m_bInitDialogCalled;

	Bitmap* m_pPortraitBackDropBitmap;
	Bitmap* m_pSelectedCharacterBitmap;
	Bitmap* m_pSelectedOpponentBitmap;

	CWinThread *m_pPartyClockThread;
	CWinThread *m_pPartyCalendarThread;

	void SetPartyPortraits();
	void SetOpposingPartyPortraits();

	void PrePositionWindow();
	void Initialize();
	void UpdateSelections();
	void RefreshTime();
	void Refresh();
	void Broadcast();
	BOOL SetSelectedTarget(CDMBaseCharViewDialog *pBaseCharDlg);
	BOOL FindPartyListSelection(CDMBaseCharViewDialog *pBaseDlg, BOOL bTarget = FALSE, BOOL *pbOpponentParty = NULL);

	cDNDSubParty * ExternalSubPartyCommand(char *szSubPartyName, DWORD dwSubPartyID);

	void LoadPartyExternal();
	void StorePocketPCs();
	void SavePartyToFile(char *szFileName);
	void LoadPartyFromFile(char *szFileName);
	BOOL FindAndSelectPartyTab();
	BOOL FindAndSelectCharacterTab(DWORD dwCharID);
	BOOL FindAndSelectOpposingPartyTab();

	void SearchAndLoadPartyMembers(CString szPath);
	BOOL CharacterIsLoaded(cDNDCharacter *pCharacter);

	void SearchAndLoadMaps(CString szFilePath, CString szSubDir);
	BOOL SearchAndLoadParentMap(cDNDMap *pChildMap, CString szFilePath, CString szSubDir);
	BOOL MapIsLoaded(cDNDMap *pMap);
	void CalcPartyHash();

	void AddPartyMember(DWORD dwReturnedID, DWORD dwSubPartyID);
	void FreeParty();
	void Cleanup();
	void SavePartyExternal();

	void LogPartyEvent(BOOL bCreateLog, int nPosition, DND_LOG_EVENT_TYPES nType, char *szName, DWORD dwID, LONG lAmount, char *szComment, BOOL bRefresh = TRUE);

	void GenerateStatBlock(cDNDParty *pParty);

	void GrantXPBonusToSelectedCharacter(int nXP);

	void ClickWeaponSwapButton(BOOL bOpponentParty);
	void ClickHitButton(BOOL bOpponentParty);
	void ClickMissButton(BOOL bOpponentParty);
	void ClickSpellButton(BOOL bOpponentParty);

	void InitiativeAttack(CDMCharViewDialog *pAttackingCharacterDialog, cDMBaseNPCViewDialog *pAttackingNPCDialog, CDMCharViewDialog *pTargetCharacterDialog, cDMBaseNPCViewDialog *pTargetNPCDialog, BOOL bGrantXP);
	void InitiativeMissileAttack(CDMCharViewDialog *pAttackingCharacterDialog, cDMBaseNPCViewDialog *pAttackingNPCDialog, CDMCharViewDialog *pTargetCharacterDialog, cDMBaseNPCViewDialog *pTargetNPCDialog, BOOL bGrantXP);
	BOOL InitiativeWound(int nDamage, CDMCharViewDialog *pTargetCharacterDialog, cDMBaseNPCViewDialog *pTargetNPCDialog, BOOL bGrantXP);

// Dialog Data
	//{{AFX_DATA(DMPartyDialog)
	enum { IDD = IDD_PARTY_DIALOG };
	CButton	m_cCastSpellButton2;
	CButton	m_cHealButton2;
	CButton	m_cWoundButton2;
	CButton	m_cMissileMissButton2;
	CButton	m_cMissileHitButton2;
	CButton	m_cAttackMissButton2;
	CButton	m_cAttackHitButton2;
	CComboBox	m_cPartyCombo2;
	CColoredListCtrl	m_cPartyList2;
	CBitmapButton	m_cDownPartyButton2;
	CBitmapButton	m_cUpPartyButton2;
	CButton	m_cMissileMissButton;
	CButton	m_cMissileHitButton;
	CButton	m_cCastSpellButton;
	CButton	m_cHealButton;
	CButton	m_cWoundButton;
	CButton	m_cAttackMissButton;
	CButton	m_cAttackHitButton;
	CBitmapButton	m_cDownPartyButton;
	CBitmapButton	m_cUpPartyButton;
	CStatic	m_cPartyLegend;
	CComboBox	m_cCalendarCombo;
	CEdit	m_cPartyMapNameEdit;
	CButton	m_cAnchorButton;
	CColoredListCtrl	m_cPartyList;
	CString	m_szPartyName;
	CString	m_szPartyMapName;
	CString	m_szSegmentEdit;
	CString	m_szRoundEdit;
	CString	m_szPartyLegend;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DMPartyDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DMPartyDialog)
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnPaint();
	afx_msg void OnChangePartyNameEdit();
	afx_msg void OnAddMemberButton();
	afx_msg void OnDeleteMemberButton();
	afx_msg void OnLoadPartyButton();
	afx_msg void OnSavePartyButton();
	afx_msg void OnSetfocusMapNameEdit();
	afx_msg void OnSelchangeCalendarCombo();
	afx_msg void OnDecSegmentButton();
	afx_msg void OnIncSegmentButton();
	afx_msg void OnDecRoundButton();
	afx_msg void OnIncRoundButton();
	afx_msg void OnIncEditTimeButton();
	afx_msg void OnClose();
	afx_msg void OnUpPartyButton();
	afx_msg void OnDownPartyButton();
	afx_msg void OnAttackHitButton();
	afx_msg void OnAttackMissButton();
	afx_msg void OnCastSpellButton();
	afx_msg void OnWoundButton();
	afx_msg void OnHealButton();
	afx_msg void OnClickPartyList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMissileHitButton();
	afx_msg void OnMissileMissButton();
	afx_msg void OnSelchangePartyCombo2();
	afx_msg void OnClickPartyList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnAttackHitButton2();
	afx_msg void OnAttackMissButton2();
	afx_msg void OnMissileHitButton2();
	afx_msg void OnMissileMissButton2();
	afx_msg void OnCastSpellButton2();
	afx_msg void OnWoundButton2();
	afx_msg void OnHealButton2();
	afx_msg void OnUpPartyButton2();
	afx_msg void OnDownPartyButton2();
	afx_msg void OnDeleteNpcButton();
	afx_msg void OnCreateNpcButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedHealPartyButton();
	afx_msg void OnBnClickedCopyMemberButton();
	afx_msg void OnBnClickedPasteMemberButton();
	CButton m_cCopyMemberButton;
	CButton m_cPasteMemberButton;
	afx_msg void OnNMDblclkPartyList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkPartyList2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedViewCalendarButton();
	afx_msg void OnBnClickedViewLogButton();
	CButton m_cViewCalendarButton;
	CButton m_cViewLogButton;
	afx_msg void OnBnClickedLogEventButton();
	CString m_szPartyXP;
	afx_msg void OnBnClickedAddXpButton();
	afx_msg void OnBnClickedGrantPartyXpButton();
	afx_msg void OnEnChangePartyXpEdit();
	CButton m_cGrantPartyXPButton;
	CButton m_cLogButton;
	afx_msg void OnBnClickedKillForXpButton();
	CButton m_cKillForXPButton;
	afx_msg void OnBnClickedSwapWeaponButton1();
	afx_msg void OnBnClickedSwapWeaponButton2();
	CButton m_cSwapWeaponButton;
	CButton m_cSwapWeaponButton2;
	afx_msg void OnBnClickedPartyInitiativeButton();
	CButton m_cViewInitiativeButton;
	afx_msg void OnBnClickedSubPartyButton();
	CButton m_cSubPartyButton;
	CString m_szPartyNameStatic;
	afx_msg void OnBnClickedRandomEncounterButton();
	afx_msg LRESULT OnDNDMessage(UINT wParam, LONG lParam);
	afx_msg LRESULT OnDirtyWindow(UINT wParam, LONG lParam);
	CEdit m_cSegmentEdit;
	CEdit m_cRoundEdit;
	afx_msg void OnBnClickedStatBlockButton();
	afx_msg void OnBnClickedStatBlockButton2();
	CButton m_cPickLockButton;
	afx_msg void OnBnClickedPickLockButton();
	CButton m_cRecoverAmmoButton;
	afx_msg void OnBnClickedRecoverAmmoButton();
	CBitmapButton m_cMusicButton;
	afx_msg void OnBnClickedMusicButton();
	CButton m_cTurnUndeadButton;
	afx_msg void OnBnClickedTurnUndeadButton();
	CButton m_cFreeCastSpellButton;
	afx_msg void OnBnClickedFreeCastSpellButton();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DMPARTYDIALOG_H__F7594913_8F18_43D9_AE8F_0CF1FF1CFCEC__INCLUDED_)
