// DM HelperDlg.h : header file
//

#if !defined(AFX_DMHELPERDLG_H__1CEDE95C_6818_45AD_939C_DD476C273D5B__INCLUDED_)
#define AFX_DMHELPERDLG_H__1CEDE95C_6818_45AD_939C_DD476C273D5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#define RED			RGB(128,0,0)
#define DRED		RGB(96,0,0)
#define YELLOW		RGB(128,128,0)
#define MAGENTA		RGB(128,0,128)
#define WHITE		RGB(255,255,255)
#define BLUE		RGB(0,0,128)
#define BLACK		RGB(0,0,0)
#define GREY		RGB(96,96,96)
#define DARKGREY    RGB(64,64,64)
#define GOLD		RGB(121,94,0)


class CDMDiceRollerDialog;
class CDMBaseCharViewDialog;

/////////////////////////////////////////////////////////////////////////////
// CDMHelperDlg dialog

class CDMHelperDlg : public CDialog
{
// Construction
public:
	LRESULT OnHotKey(WPARAM wParam, LPARAM lParam);
	CDMHelperDlg(CWnd* pParent = NULL);	// standard constructor

	CDMHelperApp *m_pApp;

	BOOL m_bCloseandUpdate;

	CBrush *m_brRed;
	CBrush *m_brDarkRed;
	CBrush *m_brYellow;
	CBrush *m_brMagenta;
	CBrush *m_brWhite;
	CBrush *m_brBlue;
	CBrush *m_brBlack;
	CBrush *m_brGrey;
	CBrush *m_brDarkGrey;
	CBrush *m_brGold;

	CWnd* m_pSelectedWindow;
	CWnd* m_pCurrentOpenPartyWindow;	// opened with all child tabs opened
	CWnd* m_pCurrentOpenMapWindow;

	DWORD m_dwCurrentOpenSubPartyWindow;

	DMPartyDialog *m_pSelectedPartyWindow; // currently selected party dialog

	cDMBaseNPCViewDialog  *m_pSelectedNPCDialog;
	CDMBaseCharViewDialog *m_pSelectedCharacterDialog;

	int m_nOldTabItemCount;

	BOOL m_bInitialMaximize;

	CDMDiceRollerDialog *m_pDiceRollerDialog;

	int m_nOrientation;

	void CloseParty(DWORD dwPartyID) ;
	void CloseParty(DMPartyDialog *pDlg, BOOL bQuiet);

	void SortTabs();
	void AddTab(CWnd* pWindow, DND_TAB_TYPES nTabType, BOOL bSetPick);
	void DeleteTab();
	void PickTab();
	BOOL ValidateDeletedTab(cDNDDisplayTab *pDeleteTab);
	void ValidatePartySelections(CWnd *pDyingWindow);
	BOOL FindAndDeleteTab(DND_TAB_TYPES nTabType, DWORD dwFindID, BOOL bSortTabs = TRUE);
	BOOL FindAndSelectTab(DND_TAB_TYPES nTabType, DWORD dwFindID);
	void ResetSubPartyTabs();

	void RetrieveCharacter(cDNDCharacter *pCharacter, char *szFileName);
	void RetrieveMap(cDNDMap *pMap, DWORD dwPartyID, char *szFileName);
	void CheckSaveData();

	DMPartyDialog *GenerateRandomEncounterTabs(cDNDParty *pParty, cDNDSubParty *pSubParty, DMPartyDialog *pMainPartyDialog, BOOL bPickTab);
	BOOL StartDungeonMaestroUpdate();
	BOOL DoCreateProcess(const char *pcszProcName, const char *pcszArgs);

	void PositionMapViews();

// Dialog Data
	//{{AFX_DATA(CDMHelperDlg)
	enum { IDD = IDD_DMHELPER_DIALOG };
	CStatic	m_cCoverBitmap;
	CTabCtrl	m_cMainTab;
	BOOL	m_bUseUnearthedArcana;
	BOOL	m_bUseDemiHumanLevelLimits;
	BOOL	m_bReRollOnesOnHitDie;
	BOOL	m_bReRollLessHalfOnHitDie;
	BOOL	m_bMaxHitPointsAtFirstLevel;
	CString	m_szEncumbranceFactorEdit;
	CString	m_szInflationFactor;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDMHelperDlg)
	public:
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void PostNcDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDMHelperDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCharView();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPartyView();
	afx_msg void OnUaCheck();
	afx_msg void OnLevelLimitsCheck();
	afx_msg void OnHitPointsRerollCheck();
	afx_msg void OnHitPointsMaxFirstLevelCheck();
	afx_msg void OnHitPointsRerollHalfCheck();
	afx_msg void OnMapView();
	afx_msg void OnSoundFx();
	afx_msg void OnNewCharacter();
	afx_msg void OnSelchangeMainTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnOpenCharacter();
	afx_msg void OnClose();
	afx_msg void OnNewMap();
	afx_msg void OnOpenMap();
	afx_msg void OnCloseCharacter();
	afx_msg void OnCloseMap();
	afx_msg void OnNewParty();
	afx_msg void OnOpenParty();
	afx_msg void OnCloseParty();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnOpenSoundboard();
	afx_msg void OnCloseSoundboard();
	afx_msg void OnChangeEncFactorEdit();
	afx_msg void OnChangeInflationFactorEdit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHelpAboutdungeonmaestro();
	afx_msg void OnDicerollerOpendicerroller();
	afx_msg void OnFlipSecondaryScreen();
	afx_msg void OnHelpDungeonmaestrouser();
	BOOL m_bUsed10Initiative;
	afx_msg void OnBnClickedD10InitiativeCheck();
	afx_msg void OnBnClickedSaveButton();
	BOOL m_bUseSoundEffects;
	afx_msg void OnBnClickedSoundEffectsCheck();
	afx_msg void OnSubpartiesAddsubparty();
	afx_msg void OnSubpartiesRemovesubparty();
	BOOL m_bUseRandomNames;
	afx_msg void OnBnClickedRandomNameCheck();
	BOOL m_bMonkStrengthAdjustments;
	BOOL m_bMonkDexderityAdjustments;
	afx_msg void OnBnClickedMonksStrengthCheck();
	afx_msg void OnBnClickedMonksDexderityCheck();
	afx_msg void OnHelpCheckforupdates();
	BOOL m_bAutoCheckUpdates;
	afx_msg void OnBnClickedAutoUpdateCheck();
	BOOL m_bUseMonsterManualII;
	BOOL m_bUseFiendFolio;
	afx_msg void OnBnClickedMonsterManualIiCheck();
	afx_msg void OnBnClickedFiendFolioCheck();
	BOOL m_bUseWeapons_vs_AC_Chart;
	afx_msg void OnBnClickedUseWeaponsVsAcCheck();
	BOOL m_bUseSmallFont;
	afx_msg void OnBnClickedSmallFontCheck();
	afx_msg void OnCustomizationsCustomitems();
	afx_msg void OnCustomizationsCustomspelleditor();
	afx_msg void OnCustomizationsCustommonster();
	afx_msg void OnCustomizationsCustomarmoreditor();
	afx_msg void OnCustomizationsCustomweaponeditor();
	afx_msg void OnSubpartiesSortsubparties();
	afx_msg void OnSubpartiesSortsubpartiesnumerically();
	afx_msg void OnMove(int x, int y);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DMHELPERDLG_H__1CEDE95C_6818_45AD_939C_DD476C273D5B__INCLUDED_)
