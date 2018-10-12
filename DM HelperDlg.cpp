// DM HelperDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DM HelperDlg.h"

#include "DMInventoryDialog.h"
#include "DMCharSpellsDialog.h"
#include "DMCharViewDialog.h"
#include "DMNPCViewDialog.h"
#include "DMPartyDialog.h"
#include "DMSubPartyDialog.h"
#include "cDMMapViewDialog.h"
#include "DMSoundFXDialog.h"
#include "DMSoundboardDialog.h"
#include "DMDiceRollerDialog.h"
#include "DMAboutDialog.h"
#include "DMCustomItemsDialog.h"
#include "DMCustomWeaponDialog.h"
#include "DMCustomArmorDialog.h"
#include "DMCustomSpellDialog.h"
#include "DMCustomMonsterDialog.h"
#include "cDMMapNameDialog.h"
#include "cDMPDFViewDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static OPENFILENAME    g_PDF_ofn;
char g_PDF_szFilename[MAX_PATH];

static int _MainTabControls[] = 
{
	IDC_BACKGROUND_BITMAP,
	IDC_UA_CHECK,
	IDC_HIT_POINTS_REROLL_CHECK,
	IDC_HIT_POINTS_REROLL_HALF_CHECK,
	IDC_HIT_POINTS_MAX_FIRST_LEVEL_CHECK,
	IDC_LEVEL_LIMITS_CHECK,
	IDC_D10_INITIATIVE_CHECK,
	IDC_OPT_STATIC,
	IDC_OPT_STATIC2,
	IDC_CW_STATIC,
	IDC_GP_STATIC,
	IDC_ENC_FACTOR_EDIT,
	IDC_INFLATION_STATIC,
	IDC_INFLATION_FACTOR_EDIT,
	IDC_SAVE_BUTTON,
	IDC_SOUND_EFFECTS_CHECK,
	IDC_RANDOM_NAME_CHECK,
	IDC_MONKS_STRENGTH_CHECK,
	IDC_MONKS_DEXDERITY_CHECK,
	IDC_AUTO_UPDATE_CHECK,
	IDC_MONSTER_MANUAL_II_CHECK,
	IDC_FIEND_FOLIO_CHECK,
	IDC_USE_WEAPONS_VS_AC_CHECK,
	IDC_SMALL_FONT_CHECK,
	-1
};

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_szVersion;
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
, m_szVersion(_T(""))
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_VERSION_STATIC, m_szVersion);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDMHelperDlg dialog

CDMHelperDlg::CDMHelperDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDMHelperDlg::IDD, pParent)
	, m_bUsed10Initiative(FALSE)
	, m_bUseSoundEffects(TRUE)
	, m_bUseRandomNames(FALSE)
	, m_bMonkStrengthAdjustments(FALSE)
	, m_bMonkDexderityAdjustments(FALSE)
	, m_bAutoCheckUpdates(FALSE)
	, m_bUseMonsterManualII(FALSE)
	, m_bUseFiendFolio(FALSE)
	, m_bUseWeapons_vs_AC_Chart(FALSE)
	, m_bUseSmallFont(FALSE)
{

	m_pApp = (CDMHelperApp *)AfxGetApp();
	m_pApp->m_pMainWindow = this;

	//{{AFX_DATA_INIT(CDMHelperDlg)
	m_bUseUnearthedArcana = TRUE;
	m_bUseDemiHumanLevelLimits = TRUE;
	m_bReRollOnesOnHitDie = TRUE;
	m_bReRollLessHalfOnHitDie = FALSE;
	m_bMaxHitPointsAtFirstLevel = FALSE;
	m_szEncumbranceFactorEdit = _T("1");
	m_szInflationFactor = _T("100");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bCloseandUpdate = FALSE;

	m_nOldTabItemCount = 0;

	m_pSelectedWindow = NULL;
	m_pCurrentOpenPartyWindow = NULL;
	m_pCurrentOpenMapWindow = NULL;

	m_pSelectedPartyWindow = NULL;

	m_pSelectedNPCDialog = NULL;
	m_pSelectedCharacterDialog = NULL;

	m_dwCurrentOpenSubPartyWindow = 0;

	m_pDiceRollerDialog = NULL;

	m_bInitialMaximize = FALSE;

	m_nOrientation = DMDO_DEFAULT;
}

void CDMHelperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDMHelperDlg)
	DDX_Control(pDX, IDC_BACKGROUND_BITMAP, m_cCoverBitmap);
	DDX_Control(pDX, IDC_MAIN_TAB, m_cMainTab);
	DDX_Check(pDX, IDC_UA_CHECK, m_bUseUnearthedArcana);
	DDX_Check(pDX, IDC_LEVEL_LIMITS_CHECK, m_bUseDemiHumanLevelLimits);
	DDX_Check(pDX, IDC_HIT_POINTS_REROLL_CHECK, m_bReRollOnesOnHitDie);
	DDX_Check(pDX, IDC_HIT_POINTS_REROLL_HALF_CHECK, m_bReRollLessHalfOnHitDie);
	DDX_Check(pDX, IDC_HIT_POINTS_MAX_FIRST_LEVEL_CHECK, m_bMaxHitPointsAtFirstLevel);
	DDX_Text(pDX, IDC_ENC_FACTOR_EDIT, m_szEncumbranceFactorEdit);
	DDX_Text(pDX, IDC_INFLATION_FACTOR_EDIT, m_szInflationFactor);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_D10_INITIATIVE_CHECK, m_bUsed10Initiative);
	DDX_Check(pDX, IDC_SOUND_EFFECTS_CHECK, m_bUseSoundEffects);
	DDX_Check(pDX, IDC_RANDOM_NAME_CHECK, m_bUseRandomNames);
	DDX_Check(pDX, IDC_MONKS_STRENGTH_CHECK, m_bMonkStrengthAdjustments);
	DDX_Check(pDX, IDC_MONKS_DEXDERITY_CHECK, m_bMonkDexderityAdjustments);
	DDX_Check(pDX, IDC_AUTO_UPDATE_CHECK, m_bAutoCheckUpdates);
	DDX_Check(pDX, IDC_MONSTER_MANUAL_II_CHECK, m_bUseMonsterManualII);
	DDX_Check(pDX, IDC_FIEND_FOLIO_CHECK, m_bUseFiendFolio);
	DDX_Check(pDX, IDC_USE_WEAPONS_VS_AC_CHECK, m_bUseWeapons_vs_AC_Chart);
	DDX_Check(pDX, IDC_SMALL_FONT_CHECK, m_bUseSmallFont);
}

BEGIN_MESSAGE_MAP(CDMHelperDlg, CDialog)
	//{{AFX_MSG_MAP(CDMHelperDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CHAR_VIEW, OnCharView)
	ON_BN_CLICKED(IDC_PARTY_VIEW, OnPartyView)
	ON_BN_CLICKED(IDC_UA_CHECK, OnUaCheck)
	ON_BN_CLICKED(IDC_LEVEL_LIMITS_CHECK, OnLevelLimitsCheck)
	ON_BN_CLICKED(IDC_HIT_POINTS_REROLL_CHECK, OnHitPointsRerollCheck)
	ON_BN_CLICKED(IDC_HIT_POINTS_MAX_FIRST_LEVEL_CHECK, OnHitPointsMaxFirstLevelCheck)
	ON_BN_CLICKED(IDC_HIT_POINTS_REROLL_HALF_CHECK, OnHitPointsRerollHalfCheck)
	ON_BN_CLICKED(IDC_MAP_VIEW, OnMapView)
	ON_BN_CLICKED(IDC_SOUND_FX, OnSoundFx)
	ON_COMMAND(ID_NEW_CHARACTER, OnNewCharacter)
	ON_NOTIFY(TCN_SELCHANGE, IDC_MAIN_TAB, OnSelchangeMainTab)
	ON_WM_SHOWWINDOW()
	ON_WM_DRAWITEM()
	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_OPEN_CHARACTER, OnOpenCharacter)
	ON_WM_CLOSE()
	ON_COMMAND(ID_NEW_MAP, OnNewMap)
	ON_COMMAND(ID_OPEN_MAP, OnOpenMap)
	ON_COMMAND(ID_CLOSE_CHARACTER, OnCloseCharacter)
	ON_COMMAND(ID_CLOSE_MAP, OnCloseMap)
	ON_COMMAND(ID_NEW_PARTY, OnNewParty)
	ON_COMMAND(ID_OPEN_PARTY, OnOpenParty)
	ON_COMMAND(ID_CLOSE_PARTY, OnCloseParty)
	ON_WM_SIZE()
	ON_COMMAND(ID_OPEN_SOUNDBOARD, OnOpenSoundboard)
	ON_COMMAND(ID_CLOSE_SOUNDBOARD, OnCloseSoundboard)
	ON_EN_CHANGE(IDC_ENC_FACTOR_EDIT, OnChangeEncFactorEdit)
	ON_EN_CHANGE(IDC_INFLATION_FACTOR_EDIT, OnChangeInflationFactorEdit)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_HELP_ABOUTDUNGEONMAESTRO, &CDMHelperDlg::OnHelpAboutdungeonmaestro)
	ON_COMMAND(ID_DICEROLLER_OPENDICERROLLER, &CDMHelperDlg::OnDicerollerOpendicerroller)
	ON_MESSAGE(WM_HOTKEY,OnHotKey)
	ON_COMMAND(ID_HELP_DUNGEONMAESTROUSER, &CDMHelperDlg::OnHelpDungeonmaestrouser)
	ON_BN_CLICKED(IDC_D10_INITIATIVE_CHECK, &CDMHelperDlg::OnBnClickedD10InitiativeCheck)
	ON_BN_CLICKED(IDC_SAVE_BUTTON, &CDMHelperDlg::OnBnClickedSaveButton)
	ON_BN_CLICKED(IDC_SOUND_EFFECTS_CHECK, &CDMHelperDlg::OnBnClickedSoundEffectsCheck)
	ON_COMMAND(ID_SUBPARTIES_ADDSUBPARTY, &CDMHelperDlg::OnSubpartiesAddsubparty)
	ON_COMMAND(ID_SUBPARTIES_REMOVESUBPARTY, &CDMHelperDlg::OnSubpartiesRemovesubparty)
	ON_BN_CLICKED(IDC_RANDOM_NAME_CHECK, &CDMHelperDlg::OnBnClickedRandomNameCheck)
	ON_BN_CLICKED(IDC_MONKS_STRENGTH_CHECK, &CDMHelperDlg::OnBnClickedMonksStrengthCheck)
	ON_BN_CLICKED(IDC_MONKS_DEXDERITY_CHECK, &CDMHelperDlg::OnBnClickedMonksDexderityCheck)
	ON_COMMAND(ID_HELP_CHECKFORUPDATES, &CDMHelperDlg::OnHelpCheckforupdates)
	ON_BN_CLICKED(IDC_AUTO_UPDATE_CHECK, &CDMHelperDlg::OnBnClickedAutoUpdateCheck)
	ON_BN_CLICKED(IDC_MONSTER_MANUAL_II_CHECK, &CDMHelperDlg::OnBnClickedMonsterManualIiCheck)
	ON_BN_CLICKED(IDC_FIEND_FOLIO_CHECK, &CDMHelperDlg::OnBnClickedFiendFolioCheck)
	ON_BN_CLICKED(IDC_USE_WEAPONS_VS_AC_CHECK, &CDMHelperDlg::OnBnClickedUseWeaponsVsAcCheck)
	ON_BN_CLICKED(IDC_SMALL_FONT_CHECK, &CDMHelperDlg::OnBnClickedSmallFontCheck)
	ON_COMMAND(ID_CUSTOMIZATIONS_CUSTOMITEMS, &CDMHelperDlg::OnCustomizationsCustomitems)
	ON_COMMAND(ID_CUSTOMIZATIONS_CUSTOMSPELLEDITOR, &CDMHelperDlg::OnCustomizationsCustomspelleditor)
	ON_COMMAND(ID_CUSTOMIZATIONS_CUSTOMMONSTER, &CDMHelperDlg::OnCustomizationsCustommonster)
	ON_COMMAND(ID_CUSTOMIZATIONS_CUSTOMARMOREDITOR, &CDMHelperDlg::OnCustomizationsCustomarmoreditor)
	ON_COMMAND(ID_CUSTOMIZATIONS_CUSTOMWEAPONEDITOR, &CDMHelperDlg::OnCustomizationsCustomweaponeditor)
	ON_COMMAND(ID_SUBPARTIES_SORTSUBPARTIES, &CDMHelperDlg::OnSubpartiesSortsubparties)
	ON_COMMAND(ID_SUBPARTIES_SORTSUBPARTIESNUMERICALLY, &CDMHelperDlg::OnSubpartiesSortsubpartiesnumerically)
	ON_WM_MOVE()
	ON_COMMAND(ID_DMLIBRARY_DUNGEONMASTER, &CDMHelperDlg::OnDmlibraryDungeonmaster)
	ON_COMMAND(ID_DMLIBRARY_PLAYER, &CDMHelperDlg::OnDmlibraryPlayer)
	ON_COMMAND(ID_DMLIBRARY_MONSTERMANUAL, &CDMHelperDlg::OnDmlibraryMonstermanual)
	ON_COMMAND(ID_DMLIBRARY_MONSTERMANUAL2, &CDMHelperDlg::OnDmlibraryMonstermanual2)
	ON_COMMAND(ID_DMLIBRARY_FIENDFOLIO, &CDMHelperDlg::OnDmlibraryFiendfolio)
	ON_COMMAND(ID_DMLIBRARY_UNEARTHEDARCANA, &CDMHelperDlg::OnDmlibraryUnearthedarcana)
	ON_COMMAND(ID_DMLIBRARY_FROMFILE, &CDMHelperDlg::OnDmlibraryFromfile)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDMHelperDlg message handlers

BOOL CDMHelperDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	#if KEARY_BUILD
	SetWindowText("Dungeon Maestro (AD&D First Edition) - Keary's Build");
	#else
	SetWindowText("Dungeon Maestro (AD&D First Edition)");
	#endif

	RegisterHotKey(m_hWnd, 100, MOD_CONTROL, 'R');	// dice roller hotkey
	RegisterHotKey(m_hWnd, 200, MOD_CONTROL, VK_F12);	// flip secondary screen hotkey

	CMenu* pMenu = GetMenu();

	m_nOldTabItemCount = 0;

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	m_brRed = new CBrush(RED);
	m_brDarkRed = new CBrush(DRED);
	m_brYellow = new CBrush(YELLOW);
	m_brMagenta = new CBrush(MAGENTA);
	m_brWhite = new CBrush(WHITE);
	m_brBlue = new CBrush(BLUE);
	m_brBlack = new CBrush(BLACK);
	m_brGrey = new CBrush(GREY);
	m_brDarkGrey = new CBrush(DARKGREY);
	m_brGold = new CBrush(GOLD);

	m_bUseUnearthedArcana = g_bUseUnearthedArcana;
	m_bUseDemiHumanLevelLimits = g_bUseDemiHumanLevelLimits;
	g_bReRollOnesOnHitDie = m_bReRollOnesOnHitDie;

	m_bMonkStrengthAdjustments = m_pApp->m_Settings.m_bMonkStrengthAdjustments;
	m_bMonkDexderityAdjustments = m_pApp->m_Settings.m_bMonkDexderityAdjustments;
	m_bUseWeapons_vs_AC_Chart = m_pApp->m_Settings.m_bUseWeapons_vs_AC_Chart;
	m_bUseSmallFont = m_pApp->m_Settings.m_bUseSmallFont;

	m_bUseMonsterManualII = m_pApp->m_Settings.m_bUseMonsterManualII;
	m_bUseFiendFolio = m_pApp->m_Settings.m_bUseFiendFolio;

	m_bUseRandomNames = m_pApp->m_Settings.m_bUseRandomNames;

	if(m_pApp->m_UpdateParams.m_nUpdateRule == 1)
	{
		m_bAutoCheckUpdates = TRUE;
	}

	m_bUsed10Initiative = m_pApp->m_Settings.m_bUsed10Initiative;
	g_bUsed10Initiative = m_bUsed10Initiative;

	m_szEncumbranceFactorEdit.Format("%d", m_pApp->m_Settings.m_nGPEncumbranceFactor);
	m_szInflationFactor.Format("%d", m_pApp->m_Settings.m_nVendorPriceInflationFactor);

	UpdateData(FALSE);

	pMenu->EnableMenuItem(ID_CLOSE_CHARACTER, MF_GRAYED);
	pMenu->EnableMenuItem(ID_CLOSE_MAP, MF_GRAYED);
	pMenu->EnableMenuItem(ID_CLOSE_SOUNDBOARD, MF_GRAYED);

	m_cMainTab.SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);

	AddTab(this, DND_TAB_TYPE_CAMPAIGN, TRUE);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDMHelperDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDMHelperDlg::OnPaint() 
{
	CRect rect;
	GetClientRect(&rect);

	//TRACE("PAINT PARENT ! %ld\n", GetUniversalTime());

	CString szTemp;
	int nCount = 0;
	for (int i = 0; i < m_pApp->m_TabArray.GetSize(); ++i)
	{
		cDNDDisplayTab *pTab = m_pApp->m_TabArray[i];

		if(pTab!= NULL)
		{
			if(pTab->m_bHide)
				continue;

			if(pTab->m_pWindow == NULL)
				continue;

			if(pTab->m_pWindow->m_hWnd == NULL)
				continue;

			if(pTab->m_nTabType == DND_TAB_TYPE_CAMPAIGN)
			{
				szTemp = "Main";
				if(m_pSelectedWindow == NULL)
				{
					m_pSelectedWindow = pTab->m_pWindow;
				}
			}
			else
			{
				//hier
				switch(pTab->m_nTabType)
				{
					case DND_TAB_TYPE_NPC:
					case DND_TAB_TYPE_CACHE:
					{
						try
						{
							cDMBaseNPCViewDialog *pDlg = (cDMBaseNPCViewDialog *)pTab->m_pWindow;
							if (pDlg->m_pNPC != NULL)
							{
								szTemp = pDlg->m_pNPC->m_szCharacterName;
							}
							else
							{
								szTemp = _T(" ");
							}
						}
						catch (...)
						{
							break;
						}

						break;
					}
					default:
					{
						pTab->m_pWindow->GetWindowText(szTemp);
						break;
					}
				}
			}

			TCITEM ltag;
			//TRACE("TAB NAME %s\n", szTemp);
			ltag.mask = TCIF_TEXT;
			ltag.pszText = szTemp.GetBuffer(0);
			ltag.iImage = i;
			m_cMainTab.SetItem(nCount, &ltag);
			++nCount;
		}
	}

#ifdef _DEBUG
#if WORK_TESTING
	(GetDlgItem( IDC_BACKGROUND_BITMAP ))->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);
#endif
#endif

	if(m_cMainTab.GetItemCount() > 1)
	{
		if(m_nOldTabItemCount == 1)
		{
			SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);
		}

		m_nOldTabItemCount = m_cMainTab.GetItemCount();

		m_cMainTab.SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_SHOWWINDOW);

		int nShow = SW_HIDE;

		if(m_cMainTab.GetCurSel() == 0)
		{
			nShow = SW_SHOW;
		}

		int nControls = 0;
		int nControl = -1;
		do
		{
			nControl = _MainTabControls[nControls++];

			if(nControl != -1)
			{
				if(nControl == IDC_BACKGROUND_BITMAP && nShow == SW_SHOW)
				{
					(GetDlgItem( nControl ))->SetWindowPos(NULL, 0, 22, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE); 
				}
				else
				{
					(GetDlgItem( nControl ))->ShowWindow(nShow);
				}
			}

		} while (nControl != -1);

		/*
		(GetDlgItem( IDC_BACKGROUND_BITMAP ))->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW); 
		(GetDlgItem( IDC_UA_CHECK ))->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);
		(GetDlgItem( IDC_HIT_POINTS_REROLL_CHECK ))->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);
		(GetDlgItem( IDC_HIT_POINTS_REROLL_HALF_CHECK ))->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);
		(GetDlgItem( IDC_HIT_POINTS_MAX_FIRST_LEVEL_CHECK ))->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);

		(GetDlgItem( IDC_LEVEL_LIMITS_CHECK ))->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);
		(GetDlgItem( IDC_CHAR_VIEW ))->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);
		(GetDlgItem( IDC_PARTY_VIEW ))->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);
		(GetDlgItem( IDC_MAP_VIEW ))->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);
		(GetDlgItem( IDC_SOUND_FX ))->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);
		(GetDlgItem( IDC_OPT_STATIC ))->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);
		(GetDlgItem( IDC_CW_STATIC ))->SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_HIDEWINDOW);
		*/

		m_cMainTab.SetWindowPos(NULL, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW); 
		m_cMainTab.InvalidateRect(NULL);

		CDialog::OnPaint();
	}
	else
	{
		if (IsIconic())
		{
			CPaintDC dc(this); // device context for painting

			SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

			// Center icon in client rectangle
			int cxIcon = GetSystemMetrics(SM_CXICON);
			int cyIcon = GetSystemMetrics(SM_CYICON);
			
			int x = (rect.Width() - cxIcon + 1) / 2;
			int y = (rect.Height() - cyIcon + 1) / 2;

			// Draw the icon
			dc.DrawIcon(x, y, m_hIcon);
		}
		else
		{
			CDialog::OnPaint();
		}
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDMHelperDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDMHelperDlg::OnCharView() 
{
	cDNDCharacter *pCharacter = new cDNDCharacter();
	
	CDMCharViewDialog *pDlg = new CDMCharViewDialog(this, pCharacter, &m_cMainTab);

	AddTab(pDlg, DND_TAB_TYPE_CHARACTER, FALSE);
}

void CDMHelperDlg::OnNewCharacter() 
{
	cDNDCharacter *pCharacter = new cDNDCharacter();

	CDMCharViewDialog *pDlg = new CDMCharViewDialog(this, pCharacter, &m_cMainTab);
	
	//pDlg->PrePositionWindow();

	pDlg->ShowWindow(SW_HIDE);

	AddTab(pDlg, DND_TAB_TYPE_CHARACTER, TRUE);

	if(m_bInitialMaximize == FALSE)
	{
		PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		m_bInitialMaximize = TRUE;
		pDlg->InvalidateRect(NULL);
	}
	
}

void CDMHelperDlg::OnOpenCharacter() 
{
	cDNDCharacter *pCharacter = new cDNDCharacter();

	CDMCharViewDialog *pDlg = new CDMCharViewDialog(this, pCharacter, &m_cMainTab);

	pDlg->ShowWindow(SW_HIDE);

	if(pDlg->LoadCharacter() != NULL)
	{
		pCharacter->ValidateInventoryFlags();

		AddTab(pDlg, DND_TAB_TYPE_CHARACTER, TRUE);

		if(m_bInitialMaximize == FALSE)
		{
			PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			m_bInitialMaximize = TRUE;
			pDlg->InvalidateRect(NULL);
		}
	}
	else
	{
		pDlg->PostMessage(WM_CLOSE);
	}
}

void CDMHelperDlg::RetrieveCharacter(cDNDCharacter *pCharacter, char *szFileName) 
{
	CDMCharViewDialog *pDlg = new CDMCharViewDialog(this, pCharacter, &m_cMainTab);

	pDlg->ShowWindow(SW_HIDE);
	
	pDlg->ProcessCharStats();

	pDlg->m_szSaveFileName = szFileName;
	pDlg->m_szLoadFileName = szFileName;

	AddTab(pDlg, DND_TAB_TYPE_CHARACTER, FALSE);

	m_pApp->m_CharacterViewMap.SetAt((WORD)pCharacter->m_dwCharacterID, pDlg);

	pCharacter->ValidateInventoryFlags();

	if(pCharacter != NULL && m_pApp->m_bInitialResize == FALSE)
	{
		if(m_bInitialMaximize == FALSE)
		{
			PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			m_bInitialMaximize = TRUE;
			pDlg->InvalidateRect(NULL);
		}
	}

}

void CDMHelperDlg::RetrieveMap(cDNDMap *pMap, DWORD dwPartyID, char *szFileName) 
{
	cDMMapViewDialog *pDlg = new cDMMapViewDialog(this, pMap, &m_cMainTab);

	pDlg->SetWindowText(pDlg->m_pDNDMap->m_szMapName);

	pDlg->ShowWindow(SW_HIDE);

	AddTab(pDlg, DND_TAB_TYPE_MAP, FALSE);

	m_pApp->m_MapViewMap.SetAt((WORD)pMap->m_dwMapID, pDlg);

	if(dwPartyID)
	{
		pDlg->SelectAndCenterOnParty(dwPartyID);
	}

	pDlg->LoadMapCaches(szFileName);

}

void CDMHelperDlg::OnMapView() 
{
	cDMMapViewDialog *pDlg = new cDMMapViewDialog(this, NULL, &m_cMainTab);

	AddTab(pDlg, DND_TAB_TYPE_MAP, TRUE);
}


void CDMHelperDlg::OnNewMap() 
{

	// * FIXED BUG go from char view to select new map from menu, program loses its shit - send SW_HIDE to current window before popping up map window to prevent OnPaint race condition as long as current window is not MAIN window
	CWnd *pMainWnd = AfxGetMainWnd();

	if(m_pSelectedWindow != NULL && m_pSelectedWindow != pMainWnd)
	{
		m_pSelectedWindow->ShowWindow(SW_HIDE);
	}

	cDMMapViewDialog *pDlg = new cDMMapViewDialog(this, NULL, &m_cMainTab);

	//strcpy(pDlg->m_pDNDMap->m_szMapName, "New Map");

	cDMMapNameDialog *pNameDlg = new cDMMapNameDialog(pDlg->m_pDNDMap, this);
	pNameDlg->DoModal();
	delete pNameDlg;

	pDlg->SetWindowText(pDlg->m_pDNDMap->m_szMapName);

	AddTab(pDlg, DND_TAB_TYPE_MAP, TRUE);

	if(m_bInitialMaximize == FALSE)
	{
		PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		m_bInitialMaximize = TRUE;
	}

}


void CDMHelperDlg::OnOpenMap() 
{	
	cDMMapViewDialog *pDlg = new cDMMapViewDialog(this, NULL, &m_cMainTab);

	pDlg->ShowWindow(SW_HIDE);
	
	if(pDlg->LoadMap() != NULL)
	{
		AddTab(pDlg, DND_TAB_TYPE_MAP, TRUE);

		if(m_bInitialMaximize == FALSE)
		{
			PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			m_bInitialMaximize = TRUE;
		}
	}
	else
	{
		pDlg->PostMessage(WM_CLOSE);
	}
}



void CDMHelperDlg::OnSoundFx() 
{
	// TODO: Add your control notification handler code here

	CDMSoundFXDialog *pDlg = new CDMSoundFXDialog(&m_cMainTab);

	AddTab(pDlg, DND_TAB_TYPE_MAP, TRUE);
	
}

void CDMHelperDlg::OnOK() 
{
	//CDialog::OnOK();
}

void CDMHelperDlg::OnCancel() 
{

	CDialog::OnCancel();
}

void CDMHelperDlg::OnPartyView() 
{
	// TODO: Add your control notification handler code here
	
}

void CDMHelperDlg::OnUaCheck() 
{
	UpdateData(TRUE);
	g_bUseUnearthedArcana = m_bUseUnearthedArcana;
}

void CDMHelperDlg::OnBnClickedUseWeaponsVsAcCheck()
{
	UpdateData(TRUE);
	m_pApp->m_Settings.m_bUseWeapons_vs_AC_Chart = m_bUseWeapons_vs_AC_Chart;
}


void CDMHelperDlg::OnLevelLimitsCheck() 
{
	UpdateData(TRUE);
	g_bUseDemiHumanLevelLimits = m_bUseDemiHumanLevelLimits;	
}

void CDMHelperDlg::OnHitPointsRerollCheck() 
{
	UpdateData(TRUE);
	g_bReRollOnesOnHitDie = m_bReRollOnesOnHitDie;
}

void CDMHelperDlg::OnHitPointsMaxFirstLevelCheck() 
{
	UpdateData(TRUE);
	g_bMaxHitPointsAtFirstLevel = m_bMaxHitPointsAtFirstLevel;
}

void CDMHelperDlg::OnHitPointsRerollHalfCheck() 
{
	UpdateData(TRUE);
	g_bReRollLessHalfOnHitDie = m_bReRollLessHalfOnHitDie;
}

void CDMHelperDlg::OnBnClickedD10InitiativeCheck()
{
	UpdateData(TRUE);
	g_bUsed10Initiative = m_bUsed10Initiative;
	m_pApp->m_Settings.m_bUsed10Initiative = m_bUsed10Initiative;
}

void CDMHelperDlg::OnBnClickedSoundEffectsCheck()
{
	UpdateData(TRUE);
	g_bUseSoundEffects = m_bUseSoundEffects;
}

void CDMHelperDlg::OnBnClickedMonksStrengthCheck()
{
	UpdateData(TRUE);
	m_pApp->m_Settings.m_bMonkStrengthAdjustments = m_bMonkStrengthAdjustments;
	
}

void CDMHelperDlg::OnBnClickedMonksDexderityCheck()
{
	UpdateData(TRUE);
	m_pApp->m_Settings.m_bMonkDexderityAdjustments = m_bMonkDexderityAdjustments;
}

void CDMHelperDlg::OnBnClickedMonsterManualIiCheck()
{
	UpdateData(TRUE);
	m_pApp->m_Settings.m_bUseMonsterManualII = m_bUseMonsterManualII;
}

void CDMHelperDlg::OnBnClickedFiendFolioCheck()
{
	UpdateData(TRUE);
	m_pApp->m_Settings.m_bUseFiendFolio = m_bUseFiendFolio;
}

void CDMHelperDlg::OnBnClickedRandomNameCheck()
{
	UpdateData(TRUE);
	m_pApp->m_Settings.m_bUseRandomNames = m_bUseRandomNames;
}

void CDMHelperDlg::OnBnClickedAutoUpdateCheck()
{
	UpdateData(TRUE);

	if(m_bAutoCheckUpdates)
	{
		m_pApp->m_UpdateParams.m_nUpdateRule = 1;
	}
	else
	{
		m_pApp->m_UpdateParams.m_nUpdateRule = 0;
	}

	m_pApp->SaveUpdateParams();
}

void CDMHelperDlg::OnBnClickedSmallFontCheck()
{
	UpdateData(TRUE);
	m_pApp->m_Settings.m_bUseSmallFont = m_bUseSmallFont;
}


void CDMHelperDlg::SortTabs()
{
	int nPos = 0;
	int nTabsSize = m_pApp->m_TabArray.GetSize();

	PDNDTABTYPEARRAY _LocalArray;
	_LocalArray.SetSize(nTabsSize);

	// do campaign tabs first
	for (int i = 0; i < nTabsSize; ++i)
	{
		cDNDDisplayTab *pTab = m_pApp->m_TabArray[i];

		if(pTab == NULL)
			continue;

		pTab->m_bSorted = FALSE;

		if(pTab->m_nTabType != DND_TAB_TYPE_CAMPAIGN)
			continue;

		_LocalArray[nPos] = pTab;
		++nPos;
	}

	// then maps
	for (i = 0; i < nTabsSize; ++i)
	{
		cDNDDisplayTab *pTab = m_pApp->m_TabArray[i];

		if(pTab == NULL)
			continue;

		if(pTab->m_nTabType != DND_TAB_TYPE_MAP)
			continue;

		cDMMapViewDialog *pMapViewDialog = (cDMMapViewDialog*)pTab->m_pWindow;
		pMapViewDialog->Reset();

		DWORD dwMapID = pMapViewDialog->m_pDNDMap->m_dwMapID;

		_LocalArray[nPos] = pTab;
		++nPos;

		// then CACHES (nPCS that are a part of this map)
		for (int j = 0; j < nTabsSize; ++j)
		{
			cDNDDisplayTab *pTab = m_pApp->m_TabArray[j];

			if(pTab == NULL)
				continue;

			if(pTab->m_nTabType != DND_TAB_TYPE_CACHE)
				continue;

			if(pTab->m_bSorted)
				continue;

			cDMBaseNPCViewDialog *pNPCDlg = (cDMBaseNPCViewDialog *)pTab->m_pWindow;

			if(pNPCDlg->m_dwMapID != dwMapID)
				continue;

			if(m_pCurrentOpenMapWindow == pMapViewDialog)
			{	
				pTab->m_bHide = FALSE;
			}
			else
			{
				pTab->m_bHide = TRUE;
			}

			pTab->m_bSorted = TRUE;

			_LocalArray[nPos] = pTab;
			++nPos;
		}
	}

	// do parties next
	for (i = 0; i < nTabsSize; ++i)
	{
		cDNDDisplayTab *pTab = m_pApp->m_TabArray[i];

		if(pTab == NULL)
			continue;

		if(pTab->m_nTabType != DND_TAB_TYPE_PARTY)
			continue;

		_LocalArray[nPos] = pTab;
		++nPos;

		DMPartyDialog *pPartyDlg = (DMPartyDialog*)pTab->m_pWindow;


		DND_TAB_TYPES nDebugTypes[500];
		memset(nDebugTypes, 0, 500 * sizeof(int));
		int nDebugCount = 0;

		//then do subparties that are children of this party
		for (int j = 0; j < nTabsSize; ++j)
		{
			cDNDDisplayTab *pSubTab = m_pApp->m_TabArray[j];

			if(pSubTab == NULL)
				continue;

			nDebugTypes[nDebugCount++] = pSubTab->m_nTabType;

			if(pSubTab->m_nTabType != DND_TAB_TYPE_SUBPARTY)
				continue;

			if(pSubTab->m_bSorted)
				continue;

			CDMSubPartyDialog *pSubPartyDlg = (CDMSubPartyDialog*)pSubTab->m_pWindow;

			if(pPartyDlg != pSubPartyDlg->m_pParentPartyDialog)
				continue;
			
			if(pPartyDlg == m_pCurrentOpenPartyWindow)
			{	
				pSubTab->m_bHide = FALSE;
			}
			else
			{
				pSubTab->m_bHide = TRUE;
			}

			pSubTab->m_bSorted = TRUE;

			_LocalArray[nPos] = pSubTab;
			++nPos;
			
		}

		//then do characters that are members of this party
		for (int j = 0; j < nTabsSize; ++j)
		{
			cDNDDisplayTab *pTab = m_pApp->m_TabArray[j];

			if(pTab == NULL)
				continue;

			if(pTab->m_nTabType != DND_TAB_TYPE_CHARACTER)
				continue;

			if(pTab->m_bSorted)
				continue;

			CDMCharViewDialog *pCharDlg = (CDMCharViewDialog *)pTab->m_pWindow;

			if(!pPartyDlg->m_pParty->CharacterIsPartyMember(pCharDlg->m_pCharacter))
				continue;

			if(m_pCurrentOpenPartyWindow == pPartyDlg)
			{	
				if(m_dwCurrentOpenSubPartyWindow == 0 || m_dwCurrentOpenSubPartyWindow == pCharDlg->m_pCharacter->m_dwSubPartyID)
				{
					pTab->m_bHide = FALSE;
				}
				else
				{
					pTab->m_bHide = TRUE;
				}
			}
			else
			{
				pTab->m_bHide = TRUE;
			}

			pTab->m_bSorted = TRUE;

			_LocalArray[nPos] = pTab;
			++nPos;
			
		}

		// then NPCs that are a member of this party
		for (j = 0; j < nTabsSize; ++j)
		{
			cDNDDisplayTab *pTab = m_pApp->m_TabArray[j];

			if(pTab == NULL)
				continue;

			if(pTab->m_nTabType != DND_TAB_TYPE_NPC)
				continue;

			if(pTab->m_bSorted)
				continue;

			cDMBaseNPCViewDialog *pNPCDlg = (cDMBaseNPCViewDialog *)pTab->m_pWindow;

			if(!pPartyDlg->m_pParty->CharacterIsPartyMember(pNPCDlg->m_pNPC))
				continue;

			if(m_pCurrentOpenPartyWindow == pPartyDlg)
			{	
				if(m_dwCurrentOpenSubPartyWindow == 0 || m_dwCurrentOpenSubPartyWindow == pNPCDlg->m_pNPC->m_dwSubPartyID)
				{
					pTab->m_bHide = FALSE;
				}
				else
				{
					pTab->m_bHide = TRUE;
				}
			}
			else
			{
				pTab->m_bHide = TRUE;
			}

			pTab->m_bSorted = TRUE;

			_LocalArray[nPos] = pTab;
			++nPos;
		}

		pPartyDlg->Refresh();

	}
	// then characters
	for (i = 0; i < nTabsSize; ++i)
	{
		cDNDDisplayTab *pTab = m_pApp->m_TabArray[i];

		if(pTab == NULL)
			continue;

		if(pTab->m_nTabType != DND_TAB_TYPE_CHARACTER)
			continue;

		if(pTab->m_bSorted)
			continue;

		_LocalArray[nPos] = pTab;

		pTab->m_bSorted = TRUE;

		++nPos;
	}

	// then NPCs
	for (i = 0; i < nTabsSize; ++i)
	{
		cDNDDisplayTab *pTab = m_pApp->m_TabArray[i];

		if(pTab == NULL)
			continue;

		if(pTab->m_nTabType != DND_TAB_TYPE_NPC)
			continue;

		if(pTab->m_bSorted)
			continue;

		_LocalArray[nPos] = pTab;

		pTab->m_bSorted = TRUE;

		++nPos;
	}

	// then soundboards
	for (i = 0; i < nTabsSize; ++i)
	{
		cDNDDisplayTab *pTab = m_pApp->m_TabArray[i];

		if(pTab == NULL)
			continue;

		if(pTab->m_nTabType != DND_TAB_TYPE_SOUNDBOARD)
			continue;

		_LocalArray[nPos] = pTab;
		++nPos;
	}

	
	// then stuff them back
	for (i = 0; i < nTabsSize; ++i)
	{
		cDNDDisplayTab *pTab = _LocalArray[i];
		m_pApp->m_TabArray[i] = pTab;
	}

	m_pApp->m_SubTabArray.SetSize(nTabsSize);

	nPos = 0;
	m_cMainTab.DeleteAllItems();
	for (i = 0; i < nTabsSize; ++i)
	{
		cDNDDisplayTab *pTab = m_pApp->m_TabArray[i];

		if(pTab == NULL)
			continue;

		pTab->m_wTabId = i;

		if(pTab->m_bHide)
		{
			pTab->m_wSubTabId = -1;
			continue;
		}

		//hier
		CString szTabLabel = _T("");

		switch(pTab->m_nTabType)
		{
			case DND_TAB_TYPE_NPC:
			case DND_TAB_TYPE_CACHE:
			{
				cDMBaseNPCViewDialog *pDlg = (cDMBaseNPCViewDialog *)pTab->m_pWindow;
				szTabLabel = pDlg->m_pNPC->m_szCharacterName;
				break;
			}
			default:
			{
				pTab->m_pWindow->GetWindowText(szTabLabel);
				break;
			}
		}

		TCITEM ltag;
		ltag.mask = TCIF_TEXT;
		ltag.pszText = szTabLabel.GetBuffer(0);
		ltag.iImage = i;

		m_cMainTab.InsertItem(nPos, &ltag);

		pTab->m_wSubTabId = nPos;
		m_pApp->m_SubTabArray[nPos] = pTab;

		if(pTab->m_pWindow == m_pSelectedWindow)
		{
			m_cMainTab.SetCurSel(nPos);
			m_cMainTab.SetCurFocus(nPos);

			UpdateData(TRUE);
		}

		++nPos;
	}

	//TRACE("DONE nPos = %d\n", nPos);

}

void CDMHelperDlg::AddTab(CWnd* pWindow, DND_TAB_TYPES _nTabType, BOOL bSetPick)
{
	int nPos = m_pApp->m_TabArray.GetSize();

	cDNDDisplayTab *pTab = new cDNDDisplayTab();

	pTab->m_nTabType = _nTabType;

	pTab->m_pWindow = pWindow;

	m_pApp->m_TabArray.SetSize(nPos+1);

	m_pApp->m_TabArray[nPos] = pTab;

	if(bSetPick)
	{
		m_pSelectedWindow = pTab->m_pWindow;

		if(_nTabType == DND_TAB_TYPE_PARTY)
		{
			m_pCurrentOpenPartyWindow = pTab->m_pWindow;
		}
		if(_nTabType == DND_TAB_TYPE_PARTY)// || _nTabType == DND_TAB_TYPE_SUBPARTY)
		{
			m_pSelectedPartyWindow = (DMPartyDialog *)pTab->m_pWindow;
		}

		SortTabs();
	}


	InvalidateRect(NULL);
	m_cMainTab.InvalidateRect(NULL);

	if(bSetPick)
	{
		nPos = 0;
		for (int i = 0; i < m_pApp->m_TabArray.GetSize(); ++i)
		{
			pTab = m_pApp->m_SubTabArray[i];

			if(pTab == NULL)
				continue;

			if(pTab != NULL && pTab->m_pWindow == m_pSelectedWindow)
			{ 
				break;
			}
			++nPos;
		}

		m_cMainTab.SetCurSel(nPos);
		m_cMainTab.SetCurFocus(nPos);

		UpdateData(TRUE);

		PickTab();
	}

}

void CDMHelperDlg::DeleteTab()
{
	UpdateData(TRUE);

	int nTabSelected = m_cMainTab.GetCurSel();

	if(nTabSelected <= 0)
		return;

	cDNDDisplayTab *pDeleteTab = m_pApp->m_SubTabArray[nTabSelected];

	if(ValidateDeletedTab(pDeleteTab))
	{
		int nNewSize = m_pApp->m_TabArray.GetSize()-1;
		nTabSelected = pDeleteTab->m_wTabId;

		for (int i = nTabSelected; i < nNewSize; ++i)
		{
			cDNDDisplayTab *pTab = m_pApp->m_TabArray[i];

			if(pTab != NULL)
			{
				m_pApp->m_TabArray[i] = m_pApp->m_TabArray[i+1];
			}
		}

		m_pApp->m_TabArray.SetSize(nNewSize);

		pDeleteTab->m_pWindow->PostMessage(WM_CLOSE);

		SortTabs();

		PickTab();
	}
}

BOOL CDMHelperDlg::ValidateDeletedTab(cDNDDisplayTab *pDeleteTab)
{
	if(pDeleteTab != NULL)
	{
		switch(pDeleteTab->m_nTabType)
		{
			case DND_TAB_TYPE_MAP:
			{
				cDMMapViewDialog *pDlg = (cDMMapViewDialog *)pDeleteTab->m_pWindow;

				if(pDlg->m_pDNDMap->IsChanged() || (pDlg->m_pTreasureCaches != NULL && pDlg->m_pTreasureCaches->IsChanged()) )
				{
					if(m_pApp->SaveConfirmMap(pDlg->m_pDNDMap, FALSE) == TRUE)
					{
						pDlg->SaveExternal();
					}
				}

				break;
			}
			case DND_TAB_TYPE_PARTY:
			case DND_TAB_TYPE_SUBPARTY:
			{
				break;
			}
			case DND_TAB_TYPE_CHARACTER:
			{
				CDMCharViewDialog *pDlg = (CDMCharViewDialog *)pDeleteTab->m_pWindow;

				ValidatePartySelections(pDlg);

				if(pDlg->m_pCharacter->IsChanged() && pDlg->m_pCharacter->m_bPocketPC == FALSE)
				{
					if(m_pApp->SaveConfirmCharacter(pDlg->m_pCharacter, FALSE) == TRUE)
					{
						pDlg->SaveExternal();
					}
					else
					{
						pDlg->m_pCharacter->MarkSaved();
					}
				}

				break;
			}
			case DND_TAB_TYPE_NPC:
			{
				cDMBaseNPCViewDialog *pDlg = (cDMBaseNPCViewDialog *)pDeleteTab->m_pWindow;

				ValidatePartySelections(pDlg);

				break;
			}
		}

		return TRUE;
	}

	return FALSE;
}

void CDMHelperDlg::ValidatePartySelections(CWnd *pDyingWindow)
{
	for (int i = 0; i < m_pApp->m_TabArray.GetSize(); ++i)
	{
		cDNDDisplayTab *pTab = m_pApp->m_TabArray[i];

		if(pTab == NULL)
			continue;

		if(pTab->m_nTabType != DND_TAB_TYPE_PARTY && pTab->m_nTabType != DND_TAB_TYPE_SUBPARTY)
			continue;

		DMPartyDialog *pDlg = (DMPartyDialog *)pTab->m_pWindow;

		if(pDlg->m_pOpposingPartyDialog == pDyingWindow)
		{
			pDlg->m_pOpposingPartyDialog = NULL;
			pDlg->Initialize();
		}

		if(pDlg->m_pSelectedCharacterDialog == pDyingWindow)
			pDlg->m_pSelectedCharacterDialog = NULL;

		if(pDlg->m_pSelectedNPCDialog == pDyingWindow)
			pDlg->m_pSelectedNPCDialog = NULL;

		if(pDlg->m_pOpposingCharacterDialog == pDyingWindow)
			pDlg->m_pOpposingCharacterDialog = NULL;

		if(pDlg->m_pOpposingNPCDialog == pDyingWindow)
			pDlg->m_pOpposingNPCDialog = NULL;

	}
}

BOOL CDMHelperDlg::FindAndDeleteTab(DND_TAB_TYPES nTabType, DWORD dwFindID, BOOL bSortTabs)
{
	UpdateData(TRUE);
	
	for (int i = 0; i < m_pApp->m_TabArray.GetSize(); ++i)
	{
		cDNDDisplayTab *pTab = m_pApp->m_TabArray[i];

		if(pTab == NULL)
			continue;

		if(pTab->m_nTabType != nTabType)
			continue;

		DWORD dwWindowID = 0L;

		switch(pTab->m_nTabType)
		{
			case DND_TAB_TYPE_MAP:
			{
				cDMMapViewDialog *pDlg = (cDMMapViewDialog *)pTab->m_pWindow;
				dwWindowID = pDlg->m_pDNDMap->m_dwMapID;
				break;
			}
			case DND_TAB_TYPE_PARTY:
			{
				DMPartyDialog *pDlg = (DMPartyDialog *)pTab->m_pWindow;
				dwWindowID = pDlg->m_pParty->m_dwPartyID;

				ValidatePartySelections(pDlg);

				break;
			}
			case DND_TAB_TYPE_SUBPARTY:
			{
				CDMSubPartyDialog *pDlg = (CDMSubPartyDialog *)pTab->m_pWindow;
				dwWindowID = pDlg->m_dwSubPartyID;

				ValidatePartySelections(pDlg);

				break;
			}
			case DND_TAB_TYPE_CHARACTER:
			{
				CDMCharViewDialog *pDlg = (CDMCharViewDialog *)pTab->m_pWindow;
				dwWindowID = pDlg->m_pCharacter->m_dwCharacterID;

				ValidatePartySelections(pDlg);

				if(pDlg->m_pCharacter->IsChanged() && pDlg->m_pCharacter->m_bPocketPC == FALSE)
				{
					if(m_pApp->m_bSaveAllCharacters == TRUE || m_pApp->SaveConfirmCharacter(pDlg->m_pCharacter, TRUE) == TRUE)
					{
						pDlg->SaveExternal();
					}
					else
					{
						pDlg->m_pCharacter->MarkSaved();
					}
				}

				break;
			}
			case DND_TAB_TYPE_NPC:
			{
				cDMBaseNPCViewDialog *pDlg = (cDMBaseNPCViewDialog *)pTab->m_pWindow;
				dwWindowID = pDlg->m_pNPC->m_dwCharacterID;

				ValidatePartySelections(pDlg);

				break;
			}
			case DND_TAB_TYPE_CACHE:
			{
				cDMBaseNPCViewDialog *pDlg = (cDMBaseNPCViewDialog *)pTab->m_pWindow;
				dwWindowID = pDlg->m_pNPC->m_dwCharacterID;
				break;
			}

		}

		if(dwWindowID == dwFindID)
		{ 
			int nNewSize = m_pApp->m_TabArray.GetSize()-1;

			for (int j = i; j < nNewSize; ++j)
			{
				cDNDDisplayTab *pMoveTab = m_pApp->m_TabArray[j];

				if(pMoveTab != NULL)
				{
					m_pApp->m_TabArray[j] = m_pApp->m_TabArray[j+1];
				}
			}

			m_pApp->m_TabArray.SetSize(nNewSize);

			pTab->m_pWindow->PostMessage(WM_CLOSE);

			if(bSortTabs)
			{
				SortTabs();
				PickTab();
			}

			return TRUE;
		}
	}

	return FALSE;
}

BOOL CDMHelperDlg::FindAndSelectTab(DND_TAB_TYPES nTabType, DWORD dwFindID)
{
	UpdateData(TRUE);
	
	int nTab = 0;

	for (int i = 0; i < m_pApp->m_TabArray.GetSize(); ++i)
	{
		cDNDDisplayTab *pTab = m_pApp->m_TabArray[i];

		if(pTab == NULL)
			continue;

//		if(pTab->m_nTabType != nTabType)
//			continue;

		DWORD dwWindowID = 0L;

		switch(pTab->m_nTabType)
		{
			case DND_TAB_TYPE_MAP:
			{
				cDMMapViewDialog *pDlg = (cDMMapViewDialog *)pTab->m_pWindow;
				dwWindowID = pDlg->m_pDNDMap->m_dwMapID;
				break;
			}
			case DND_TAB_TYPE_PARTY:
			{
				DMPartyDialog *pDlg = (DMPartyDialog *)pTab->m_pWindow;
				dwWindowID = pDlg->m_pParty->m_dwPartyID;
				break;
			}
			case DND_TAB_TYPE_SUBPARTY:
			{
				DMPartyDialog *pDlg = (DMPartyDialog *)pTab->m_pWindow;
				dwWindowID = pDlg->m_dwSubPartyID;
				break;
			}
			case DND_TAB_TYPE_CHARACTER:
			{
				CDMCharViewDialog *pDlg = (CDMCharViewDialog *)pTab->m_pWindow;
				dwWindowID = pDlg->m_pCharacter->m_dwCharacterID;
				break;
			}
			case DND_TAB_TYPE_NPC:
			{
				//CRASH HERE !!!
				cDMBaseNPCViewDialog *pDlg = (cDMBaseNPCViewDialog *)pTab->m_pWindow;
				dwWindowID = pDlg->m_pNPC->m_dwCharacterID;
				break;
			}
			case DND_TAB_TYPE_CACHE:
			{
				DMNPCViewDialog *pDlg = (DMNPCViewDialog *)pTab->m_pWindow;
				dwWindowID = pDlg->m_pNPC->m_dwCharacterID;
				break;
			}

		}

		if(dwWindowID == dwFindID)
		{ 
			m_cMainTab.SetCurSel(pTab->m_wSubTabId);
			PickTab();
			return TRUE;
		}

	}

	return FALSE;
}



void CDMHelperDlg::OnSelchangeMainTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;

	PickTab();
}

void CDMHelperDlg::PickTab()
{
	UpdateData(TRUE);

	int nTabSelected = m_cMainTab.GetCurSel();

	CMenu* pMenu = GetMenu();

	BOOL bNeedSort = FALSE;

	m_pSelectedPartyWindow = NULL;

	DWORD dwOldOpenSubPartyWindow = m_dwCurrentOpenSubPartyWindow;

	if(m_pSelectedNPCDialog != NULL)
	{
		m_pSelectedNPCDialog->CloseSubWindows();
		m_pSelectedNPCDialog = NULL;
	}
	if(m_pSelectedCharacterDialog != NULL)
	{
		m_pSelectedCharacterDialog->CloseSubWindows();
		m_pSelectedCharacterDialog = NULL;
	}

	for (int i = 0; i < m_pApp->m_TabArray.GetSize(); ++i)
	{
		cDNDDisplayTab *pTab = m_pApp->m_TabArray[i];

		if(pTab != NULL)
		{
			if(pTab->m_wSubTabId == nTabSelected)
			{
				switch(pTab->m_nTabType)
				{
					case DND_TAB_TYPE_CAMPAIGN:
					{
						pMenu->EnableMenuItem(ID_CLOSE_CHARACTER, MF_GRAYED);
						pMenu->EnableMenuItem(ID_CLOSE_PARTY, MF_GRAYED);
						pMenu->EnableMenuItem(ID_CLOSE_MAP, MF_GRAYED);
						pMenu->EnableMenuItem(ID_CLOSE_SOUNDBOARD, MF_GRAYED);

						pMenu->EnableMenuItem(ID_SUBPARTIES_ADDSUBPARTY, MF_GRAYED);
						pMenu->EnableMenuItem(ID_SUBPARTIES_REMOVESUBPARTY, MF_GRAYED);

						pMenu->EnableMenuItem(ID_SUBPARTIES_SORTSUBPARTIES, MF_GRAYED);
						pMenu->EnableMenuItem(ID_SUBPARTIES_SORTSUBPARTIESNUMERICALLY, MF_GRAYED);

						break;
					}
					case DND_TAB_TYPE_PARTY:
					{
						pMenu->EnableMenuItem(ID_CLOSE_CHARACTER, MF_GRAYED);
						pMenu->EnableMenuItem(ID_CLOSE_PARTY, MF_ENABLED);
						pMenu->EnableMenuItem(ID_CLOSE_MAP, MF_GRAYED);
						pMenu->EnableMenuItem(ID_CLOSE_SOUNDBOARD, MF_GRAYED);

						pMenu->EnableMenuItem(ID_SUBPARTIES_ADDSUBPARTY, MF_ENABLED);
						pMenu->EnableMenuItem(ID_SUBPARTIES_REMOVESUBPARTY, MF_GRAYED);

						pMenu->EnableMenuItem(ID_SUBPARTIES_SORTSUBPARTIES, MF_ENABLED);
						pMenu->EnableMenuItem(ID_SUBPARTIES_SORTSUBPARTIESNUMERICALLY, MF_ENABLED);

						m_dwCurrentOpenSubPartyWindow = 0;

						if(m_pCurrentOpenPartyWindow != pTab->m_pWindow)
						{
							m_pCurrentOpenPartyWindow = pTab->m_pWindow;
							bNeedSort = TRUE;
						}
						if(m_dwCurrentOpenSubPartyWindow != dwOldOpenSubPartyWindow)
						{
							bNeedSort = TRUE;
						}
						if(m_pCurrentOpenMapWindow != NULL)
						{
							m_pCurrentOpenMapWindow = NULL;
							bNeedSort = TRUE;
						}

						DMPartyDialog *pDlg = (DMPartyDialog*)pTab->m_pWindow;

						pDlg->m_dwParentPartyID;
						pDlg->m_dwSubPartyID = 0;
						pDlg->m_pSubParty = NULL;
						pDlg->m_pSubPartyDialog = NULL;

						pDlg->Initialize();
						pDlg->UpdateSelections();
						pDlg->Refresh();

						m_pSelectedPartyWindow = pDlg;

						break;
					}
					case DND_TAB_TYPE_SUBPARTY:
					{
						pMenu->EnableMenuItem(ID_CLOSE_CHARACTER, MF_GRAYED);
						pMenu->EnableMenuItem(ID_CLOSE_PARTY, MF_GRAYED);
						pMenu->EnableMenuItem(ID_CLOSE_MAP, MF_GRAYED);
						pMenu->EnableMenuItem(ID_CLOSE_SOUNDBOARD, MF_GRAYED);

						pMenu->EnableMenuItem(ID_SUBPARTIES_ADDSUBPARTY, MF_GRAYED);
						pMenu->EnableMenuItem(ID_SUBPARTIES_REMOVESUBPARTY, MF_ENABLED);

						pMenu->EnableMenuItem(ID_SUBPARTIES_SORTSUBPARTIES, MF_GRAYED);
						pMenu->EnableMenuItem(ID_SUBPARTIES_SORTSUBPARTIESNUMERICALLY, MF_GRAYED);

						CDMSubPartyDialog *pDlg = (CDMSubPartyDialog*)pTab->m_pWindow;

						m_dwCurrentOpenSubPartyWindow = pDlg->m_dwSubPartyID;

						if(m_pCurrentOpenPartyWindow != pDlg->m_pParentPartyDialog)
						{
							bNeedSort = TRUE;
						}
						if(m_dwCurrentOpenSubPartyWindow != dwOldOpenSubPartyWindow)
						{
							bNeedSort = TRUE;
						}
						if(m_pCurrentOpenMapWindow != NULL)
						{
							m_pCurrentOpenMapWindow = NULL;
							bNeedSort = TRUE;
						}

						pDlg->m_pParentPartyDialog->m_dwParentPartyID =  pDlg->m_pParentPartyDialog->m_pParty->m_dwPartyID;
						pDlg->m_pParentPartyDialog->m_dwSubPartyID = pDlg->m_dwSubPartyID;
						pDlg->m_pParentPartyDialog->m_pSubParty = pDlg->m_pSubParty;
						pDlg->m_pParentPartyDialog->m_pSubPartyDialog = pDlg;


						pDlg->Initialize();
						pDlg->UpdateSelections();
						pDlg->Refresh();

						m_pSelectedPartyWindow = pDlg->m_pParentPartyDialog;

						break;
					}
					case DND_TAB_TYPE_CHARACTER:
					{
						pMenu->EnableMenuItem(ID_CLOSE_CHARACTER, MF_ENABLED);
						pMenu->EnableMenuItem(ID_CLOSE_PARTY, MF_GRAYED);
						pMenu->EnableMenuItem(ID_CLOSE_MAP, MF_GRAYED);
						pMenu->EnableMenuItem(ID_CLOSE_SOUNDBOARD, MF_GRAYED);

						pMenu->EnableMenuItem(ID_SUBPARTIES_ADDSUBPARTY, MF_GRAYED);
						pMenu->EnableMenuItem(ID_SUBPARTIES_REMOVESUBPARTY, MF_GRAYED);

						pMenu->EnableMenuItem(ID_SUBPARTIES_SORTSUBPARTIES, MF_GRAYED);
						pMenu->EnableMenuItem(ID_SUBPARTIES_SORTSUBPARTIESNUMERICALLY, MF_GRAYED);

						CDMCharViewDialog *pDlg = (CDMCharViewDialog*)pTab->m_pWindow;
						pDlg->m_nPage = pDlg->m_nLastPage;

						pDlg->ProcessCharStats();
						pDlg->Refresh();

						m_pSelectedCharacterDialog = pDlg;

						break;
					}
					case DND_TAB_TYPE_NPC:
					{
						pMenu->EnableMenuItem(ID_CLOSE_CHARACTER, MF_GRAYED);
						pMenu->EnableMenuItem(ID_CLOSE_PARTY, MF_GRAYED);
						pMenu->EnableMenuItem(ID_CLOSE_MAP, MF_GRAYED);
						pMenu->EnableMenuItem(ID_CLOSE_SOUNDBOARD, MF_GRAYED);

						pMenu->EnableMenuItem(ID_SUBPARTIES_ADDSUBPARTY, MF_GRAYED);
						pMenu->EnableMenuItem(ID_SUBPARTIES_REMOVESUBPARTY, MF_GRAYED);

						pMenu->EnableMenuItem(ID_SUBPARTIES_SORTSUBPARTIES, MF_GRAYED);
						pMenu->EnableMenuItem(ID_SUBPARTIES_SORTSUBPARTIESNUMERICALLY, MF_GRAYED);

						cDMBaseNPCViewDialog *pDlg = (cDMBaseNPCViewDialog*)pTab->m_pWindow;
						pDlg->m_nPage = pDlg->m_nLastPage;

						pDlg->InitView();
						
						m_pSelectedNPCDialog = pDlg;

						break;
					}
					case DND_TAB_TYPE_MAP:
					{
						pMenu->EnableMenuItem(ID_CLOSE_CHARACTER, MF_GRAYED);
						pMenu->EnableMenuItem(ID_CLOSE_PARTY, MF_GRAYED);
						pMenu->EnableMenuItem(ID_CLOSE_MAP, MF_ENABLED);
						pMenu->EnableMenuItem(ID_CLOSE_SOUNDBOARD, MF_GRAYED);

						pMenu->EnableMenuItem(ID_SUBPARTIES_ADDSUBPARTY, MF_GRAYED);
						pMenu->EnableMenuItem(ID_SUBPARTIES_REMOVESUBPARTY, MF_GRAYED);

						pMenu->EnableMenuItem(ID_SUBPARTIES_SORTSUBPARTIES, MF_GRAYED);
						pMenu->EnableMenuItem(ID_SUBPARTIES_SORTSUBPARTIESNUMERICALLY, MF_GRAYED);

						if(m_pCurrentOpenPartyWindow != pTab->m_pWindow)
						{
							m_pCurrentOpenPartyWindow = pTab->m_pWindow;
							bNeedSort = TRUE;
						}
						if(m_dwCurrentOpenSubPartyWindow != dwOldOpenSubPartyWindow)
						{
							bNeedSort = TRUE;
						}
						if(m_pCurrentOpenMapWindow != pTab->m_pWindow)
						{
							bNeedSort = TRUE;
						}

						m_pCurrentOpenMapWindow = pTab->m_pWindow;

						cDMMapViewDialog *pMapView = (cDMMapViewDialog*)m_pCurrentOpenMapWindow;

						pMapView->Refresh();

						break;
					}
					case DND_TAB_TYPE_CACHE:
					{
						TRACE("GRAB CACHE %x\n", pTab->m_pWindow);

						pMenu->EnableMenuItem(ID_CLOSE_CHARACTER, MF_GRAYED);
						pMenu->EnableMenuItem(ID_CLOSE_PARTY, MF_GRAYED);
						pMenu->EnableMenuItem(ID_CLOSE_MAP, MF_GRAYED);
						pMenu->EnableMenuItem(ID_CLOSE_SOUNDBOARD, MF_GRAYED);

						pMenu->EnableMenuItem(ID_SUBPARTIES_ADDSUBPARTY, MF_GRAYED);
						pMenu->EnableMenuItem(ID_SUBPARTIES_REMOVESUBPARTY, MF_GRAYED);

						pMenu->EnableMenuItem(ID_SUBPARTIES_SORTSUBPARTIES, MF_GRAYED);
						pMenu->EnableMenuItem(ID_SUBPARTIES_SORTSUBPARTIESNUMERICALLY, MF_GRAYED);

						cDMBaseNPCViewDialog *pDlg = (cDMBaseNPCViewDialog*)pTab->m_pWindow;

						pDlg->InitView();

						pDlg->m_pNPCViewDialog->m_nPage = 1;

						m_pSelectedNPCDialog = pDlg;

						break;
					}
					case DND_TAB_TYPE_SOUNDBOARD:
					{
						pMenu->EnableMenuItem(ID_CLOSE_CHARACTER, MF_GRAYED);
						pMenu->EnableMenuItem(ID_CLOSE_PARTY, MF_GRAYED);
						pMenu->EnableMenuItem(ID_CLOSE_MAP, MF_ENABLED);
						pMenu->EnableMenuItem(ID_CLOSE_SOUNDBOARD, MF_ENABLED);

						pMenu->EnableMenuItem(ID_SUBPARTIES_ADDSUBPARTY, MF_GRAYED);
						pMenu->EnableMenuItem(ID_SUBPARTIES_REMOVESUBPARTY, MF_GRAYED);

						pMenu->EnableMenuItem(ID_SUBPARTIES_SORTSUBPARTIES, MF_GRAYED);
						pMenu->EnableMenuItem(ID_SUBPARTIES_SORTSUBPARTIESNUMERICALLY, MF_GRAYED);

						break;
					}
				}

				m_pSelectedWindow = pTab->m_pWindow;

				if(pTab->m_nTabType != DND_TAB_TYPE_CAMPAIGN)
				{
					if(pTab->m_pWindow != NULL && pTab->m_pWindow->m_hWnd != NULL)
					{
						//pTab->m_pWindow->ShowWindow(SW_SHOW);
						CRect rect;
						GetClientRect(&rect);

						rect.top+=20;

						switch(pTab->m_nTabType)
						{
							case DND_TAB_TYPE_NPC:
							case DND_TAB_TYPE_CACHE:
							{
								cDMBaseNPCViewDialog *pDlg = (cDMBaseNPCViewDialog*)pTab->m_pWindow;

								if(pDlg->m_pNPCViewDialog != NULL)
								{
									pDlg->m_pNPCViewDialog->SetWindowPos(NULL, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
								}
								break;
							}
							case DND_TAB_TYPE_SUBPARTY:
							{
								CDMSubPartyDialog *pDlg = (CDMSubPartyDialog*)pTab->m_pWindow;
								pDlg->m_pParentPartyDialog->SetWindowPos(NULL, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
								break;
							}
							default:
							{
								pTab->m_pWindow->SetWindowPos(NULL, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
								break;
							}
						}
						
					}
				}
				//TRACE("SHOW WINDOW %d\n", i);
			}
			else
			{
				switch(pTab->m_nTabType)
				{
					case DND_TAB_TYPE_CHARACTER:
					{
						CDMCharViewDialog *pDlg = (CDMCharViewDialog*)pTab->m_pWindow;

						if(pDlg == NULL || pDlg->m_hWnd == NULL)
							break;

						pDlg->m_nLastPage = pDlg->m_nPage;
						pDlg->m_nPage = 0;

						//if(pDlg->m_pSpellsDialog == NULL)
						//{
						//	AfxMessageBox("CRITICAL ERROR 1061 !", MB_OK);
						//}

						if(pDlg->m_pInventoryDialog != NULL)
						{
							pDlg->m_pInventoryDialog->ShowWindow(SW_HIDE);
						}

						if(pDlg->m_pSpellsDialog != NULL)
						{
							pDlg->m_pSpellsDialog->ShowWindow(SW_HIDE);
						}

						break;
					}
					case DND_TAB_TYPE_NPC:
					case DND_TAB_TYPE_CACHE:
					{
						cDMBaseNPCViewDialog *pDlg = (cDMBaseNPCViewDialog*)pTab->m_pWindow;

						if(pDlg == NULL || pDlg->m_hWnd == NULL)
							break;

						pDlg->m_nLastPage = pDlg->m_nPage;
						pDlg->m_nPage = 0;

						if(pDlg->m_pInventoryDialog != NULL)
						{
							pDlg->m_pInventoryDialog->ShowWindow(SW_HIDE);
						}
						if(pDlg->m_pSpellsDialog != NULL)
						{
							pDlg->m_pSpellsDialog->ShowWindow(SW_HIDE);
						}

						break;
					}
					case DND_TAB_TYPE_MAP:
					{
						break;
					}
				}

				if(pTab->m_nTabType != DND_TAB_TYPE_CAMPAIGN)
				{
					if(pTab->m_pWindow != NULL && pTab->m_pWindow->m_hWnd != NULL)
					{
						if (pTab->m_nTabType == DND_TAB_TYPE_MAP)
						{
							cDMMapViewDialog *pMapDlg = (cDMMapViewDialog *)pTab->m_pWindow;

							if (pMapDlg->m_bDetachedWindow == FALSE)
							{
								pTab->m_pWindow->ShowWindow(SW_HIDE);
							}
						}
						else
						{
							pTab->m_pWindow->ShowWindow(SW_HIDE);
						}
					}
				}
				//TRACE("HIDE WINDOW %d\n", i);
			}	
			
		}
	}

	//AfxMessageBox("UNO !", MB_OK);
	if(bNeedSort)
	{
		SortTabs();
	}
	//AfxMessageBox("DOS !", MB_OK);

	InvalidateRect(NULL);
}

void CDMHelperDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	
	// TODO: Add your message handler code here

	InvalidateRect(NULL);
	
}		

void CDMHelperDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default

	//if(nIDCtl != IDC_MAIN_TAB) // && m_cMainTab.GetItemCount())
	//	return;

	if(lpDrawItemStruct == NULL)
	{
		AfxMessageBox("CRIT ERROR 1132", MB_OK);
	}

	if(lpDrawItemStruct->hwndItem == NULL)
	{
		AfxMessageBox("CRIT ERROR 1137", MB_OK);
	}

	char        szTabText[100];
	UINT        bkColor;
	UINT        txColor = WHITE;
	CBrush      *cbr;
	TC_ITEM     tci;

	if (m_cMainTab.m_hWnd == lpDrawItemStruct->hwndItem)
	{
	  switch (m_pApp->m_SubTabArray[lpDrawItemStruct->itemID]->m_nTabType)
	  {
		  case DND_TAB_TYPE_CAMPAIGN:
			  cbr = m_brBlack;
			  bkColor = BLACK;
			  break;

		  case DND_TAB_TYPE_PARTY:
			  cbr = m_brRed;
			  bkColor = RED;
			  break;

		  case DND_TAB_TYPE_SUBPARTY:
			  cbr = m_brDarkRed;
			  bkColor = DRED;
			  break;

		  case DND_TAB_TYPE_CHARACTER:
			  cbr = m_brGrey;
			  bkColor = GREY;
			  //txColor = BLACK;
			  break;

		  case DND_TAB_TYPE_NPC:
			  cbr = m_brDarkGrey;
			  bkColor = DARKGREY;
			  //txColor = BLACK;
			  break;

		  case DND_TAB_TYPE_MAP:
			  cbr = m_brMagenta;
			  bkColor = MAGENTA;
			  break;

		  case DND_TAB_TYPE_CACHE:
			  cbr = m_brGold;
			  bkColor = GOLD;
			  break;

		  case DND_TAB_TYPE_SOUNDBOARD:
		  default:
			  cbr = m_brBlue;
			  bkColor = BLUE;
			  break;

	  }

	  memset(szTabText, '\0', sizeof(szTabText));

	  tci.mask        = TCIF_TEXT;
	  tci.pszText     = szTabText;
	  tci.cchTextMax  = sizeof(szTabText)-1;

	  m_cMainTab.GetItem(lpDrawItemStruct->itemID, &tci);

	  CDC *dc = CDC::FromHandle(lpDrawItemStruct->hDC);

	  dc->FillRect(&lpDrawItemStruct->rcItem, cbr);
	  dc->SetTextColor(txColor);
	  dc->SetBkColor(bkColor);

	  CString szTemp;
	  szTemp.Format(" %s", tci.pszText);

	  TextOut(lpDrawItemStruct->hDC,
			  lpDrawItemStruct->rcItem.left,
			  lpDrawItemStruct->rcItem.top,
			  //tci.pszText,
			  szTemp.GetBuffer(0),
			  lstrlen(tci.pszText)+1);

	  return;
	}
	
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct); //not owner draw, so bail ?
	
}

BOOL CDMHelperDlg::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::OnChildNotify(message, wParam, lParam, pLResult);
}

HBRUSH CDMHelperDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{

	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CDMHelperDlg::CheckSaveData()
{
	for (POSITION pos = m_pApp->m_MapViewMap.GetStartPosition(); pos != NULL; )
	{
		WORD wID;
		cDMMapViewDialog *pMapDlg = NULL;
		m_pApp->m_MapViewMap.GetNextAssoc(pos,wID,pMapDlg);

		if(pMapDlg != NULL && pMapDlg->m_pDNDMap->m_dwMapID)
		{
			if(pMapDlg->m_pDNDMap->IsChanged() || (pMapDlg->m_pTreasureCaches != NULL && pMapDlg->m_pTreasureCaches->IsChanged()) )
			{
				if(m_pApp->SaveConfirmMap(pMapDlg->m_pDNDMap, FALSE) == TRUE)
				{
					pMapDlg->SaveExternal();
				}
			}
		}
	}

	m_pApp->m_bSaveAllParties = FALSE;

	for (POSITION pos = m_pApp->m_PartyViewMap.GetStartPosition(); pos != NULL; )
	{
		WORD wID;
		PDNDPARTYVIEWDLG pPartyDlg = NULL;
		m_pApp->m_PartyViewMap.GetNextAssoc(pos,wID,pPartyDlg);

		if(pPartyDlg != NULL && pPartyDlg->m_pParty->m_dwPartyID)
		{
			if(pPartyDlg->m_pParty->IsChanged() && pPartyDlg->m_dwSubPartyID == 0)
			{
				if(m_pApp->SaveConfirmParty(pPartyDlg->m_pParty, FALSE) == TRUE)
				{
					pPartyDlg->SavePartyExternal();
				}
			}
		}

		m_pApp->m_bSaveAllCharacters = FALSE;
		for (POSITION pos = m_pApp->m_CharacterViewMap.GetStartPosition(); pos != NULL; )
		{
			PDNDCHARVIEWDLG pCharDlg = NULL;
			m_pApp->m_CharacterViewMap.GetNextAssoc(pos,wID,pCharDlg);

			if(pCharDlg != NULL && pCharDlg->m_pCharacter != NULL && pCharDlg->m_pCharacter->m_dwCharacterID && pCharDlg->m_pCharacter->IsChanged() && pCharDlg->m_pCharacter->m_bPocketPC == FALSE)
			{
				if(m_pApp->m_bSaveAllCharacters == TRUE || m_pApp->SaveConfirmCharacter(pCharDlg->m_pCharacter, TRUE) == TRUE)
				{
					pCharDlg->SaveExternal();
				}
				else
				{
					pCharDlg->m_pCharacter->MarkSaved();
				}
			}
		}
	}
}

void CDMHelperDlg::OnClose() 
{
	CString szQuery = _T("Are you sure you wish to close the program ?");

	if(m_bCloseandUpdate)
	{
		szQuery = _T("Dungeon Maestro must exit to check for updates.\n\r\n\rAre you sure you wish to close the program ?");
	}

	if(AfxMessageBox(szQuery, MB_ICONQUESTION|MB_OKCANCEL) == IDCANCEL)
	{
		m_bCloseandUpdate = FALSE;
		return;
	}

	for (POSITION pos = m_pApp->m_DetachedMapViewMap.GetStartPosition(); pos != NULL;)
	{
		WORD wID;
		PDNDMAPVIEWDLG pMapDlg = NULL;
		m_pApp->m_DetachedMapViewMap.GetNextAssoc(pos, wID, pMapDlg);

		if (pMapDlg != NULL)
		{
			pMapDlg->PostMessage(WM_CLOSE);
			Sleep(1000);
		}
	}
	m_pApp->m_DetachedMapViewMap.RemoveAll();

	CheckSaveData();

	delete m_brRed;
	delete m_brDarkRed;
	delete m_brYellow;
	delete m_brMagenta;
	delete m_brWhite;
	delete m_brBlue;
	delete m_brBlack;
	delete m_brGrey;
	delete m_brDarkGrey;
	delete m_brGold;

	if(m_bCloseandUpdate)
	{
		StartDungeonMaestroUpdate();
	}

	if (m_nOrientation != DMDO_DEFAULT)
	{
		OnFlipSecondaryScreen();
	}
	
	CDialog::OnClose();
}

void CDMHelperDlg::OnCloseCharacter() 
{
	DeleteTab();
}

void CDMHelperDlg::OnCloseMap() 
{
	DeleteTab();
}

void CDMHelperDlg::OnNewParty() 
{
	DMPartyDialog *pDlg = new DMPartyDialog(this, NULL, &m_cMainTab);

	pDlg->ShowWindow(SW_HIDE);

	AddTab(pDlg, DND_TAB_TYPE_PARTY, TRUE);

	if(m_bInitialMaximize == FALSE)
	{
		PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		m_bInitialMaximize = TRUE;
	}
	
}

void CDMHelperDlg::OnOpenParty() 
{
	DMPartyDialog *pDlg = new DMPartyDialog(this, NULL, &m_cMainTab);

	pDlg->ShowWindow(SW_HIDE);

	pDlg->LoadPartyExternal();

	if(pDlg->m_pParty->m_dwPartyID)
	{
		AddTab(pDlg, DND_TAB_TYPE_PARTY, TRUE);	

		if(m_bInitialMaximize == FALSE)
		{
			PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
			m_bInitialMaximize = TRUE;
		}
	}
	else
	{
		AfxMessageBox("This party is already loaded !", MB_OK);
		pDlg->PostMessage(WM_CLOSE);
	}
}

void CDMHelperDlg::CloseParty(DWORD dwPartyID) 
{
	DMPartyDialog *pDlg = NULL;

	if(m_pApp->m_PartyViewMap.Lookup((WORD)dwPartyID, pDlg) && pDlg != NULL)
	{
		CloseParty(pDlg, TRUE);
	}
}

void CDMHelperDlg::CloseParty(DMPartyDialog *pDlg, BOOL bQuiet)
{
	pDlg->Cleanup();

	m_pApp->m_PartyViewMap.SetAt((WORD)pDlg->m_pParty->m_dwPartyID, NULL);
	m_pApp->m_PartyViewMap.RemoveKey((WORD)pDlg->m_pParty->m_dwPartyID);

	for (POSITION pos = m_pApp->m_PartyViewMap.GetStartPosition(); pos != NULL; )
	{
		WORD wID;
		PDNDPARTYVIEWDLG pPartyDlg = NULL;
		m_pApp->m_PartyViewMap.GetNextAssoc(pos,wID,pPartyDlg);

		if(pPartyDlg != NULL && pPartyDlg->m_pOpposingPartyDialog == pDlg) // TODO check correctness here
		{
			pPartyDlg->m_pOpposingPartyDialog = NULL;
			pPartyDlg->m_pOpposingCharacterDialog = NULL;
			pPartyDlg->m_pOpposingNPCDialog = NULL;
			pPartyDlg->m_cPartyList2.DeleteAllItems();
			pPartyDlg->Refresh();
		}
	}

	if(pDlg->m_pParty->m_dwPartyRoster[0])
	{
		if(bQuiet == TRUE || AfxMessageBox("Close party character windows ?", MB_YESNO) == IDYES)
		{
			m_pApp->m_bSaveAllCharacters = FALSE;
			for(int i = 0; i < MAX_PARTY_MEMBERS; ++i)
			{
				if(pDlg->m_pParty->m_dwPartyRoster[i] != 0)
				{
					PDNDNPCVIEWDLG pNPCDlg = NULL;
					m_pApp->m_NPCViewMap.Lookup((WORD)pDlg->m_pParty->m_dwPartyRoster[i], pNPCDlg);

					if(pNPCDlg == NULL) //only want PC's right now, NPCs are cleaned up below
					{
						FindAndDeleteTab(DND_TAB_TYPE_CHARACTER, pDlg->m_pParty->m_dwPartyRoster[i], FALSE);
					}
				}
			}
		}
	}

	for(int i = 0; i < MAX_NPC_PARTY_MEMBERS; ++i)
	{
		if(pDlg->m_pParty->m_NPCs[i].m_dwCharacterID != 0)
		{
			if(FindAndDeleteTab(DND_TAB_TYPE_NPC, pDlg->m_pParty->m_NPCs[i].m_dwCharacterID, FALSE))
			{
				m_pApp->m_NPCViewMap.SetAt((WORD)pDlg->m_pParty->m_NPCs[i].m_dwCharacterID, NULL);
				m_pApp->m_NPCViewMap.RemoveKey((WORD)pDlg->m_pParty->m_NPCs[i].m_dwCharacterID);
			}
		}
	}

	SortTabs();

	FindAndDeleteTab(DND_TAB_TYPE_PARTY, pDlg->m_pParty->m_dwPartyID);
	
	SortTabs();

}

void CDMHelperDlg::OnCloseParty() 
{
	if(m_pCurrentOpenPartyWindow == NULL)
		return;

	DMPartyDialog *pDlg = (DMPartyDialog *)m_pCurrentOpenPartyWindow;

	if(pDlg->m_pParty->IsChanged() && pDlg->m_dwSubPartyID == 0)
	{
		if(m_pApp->SaveConfirmParty(pDlg->m_pParty, FALSE) == TRUE)
		{
			pDlg->SavePartyExternal();
		}
	}

	CloseParty(pDlg, FALSE);

	int nTabsOpen = m_pApp->m_TabArray.GetSize();

	if(nTabsOpen <= 2)
	{
		m_bInitialMaximize = FALSE;
		SendMessage ( WM_SYSCOMMAND, SC_RESTORE, 0 ); 
	}
}


void CDMHelperDlg::OnOpenSoundboard() 
{
	DMSoundboardDialog *pDlg =  new DMSoundboardDialog(&m_cMainTab);

	pDlg->ShowWindow(SW_HIDE);

	AddTab(pDlg, DND_TAB_TYPE_SOUNDBOARD, TRUE);

	if(m_bInitialMaximize == FALSE)
	{
		PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		m_bInitialMaximize = TRUE;
	}
}

void CDMHelperDlg::OnCloseSoundboard() 
{
	DeleteTab();
}


void CDMHelperDlg::PositionMapViews()
{
	for (POSITION pos = m_pApp->m_MapViewMap.GetStartPosition(); pos != NULL;)
	{
		WORD wID;
		PDNDMAPVIEWDLG pMapDlg = NULL;
		m_pApp->m_MapViewMap.GetNextAssoc(pos, wID, pMapDlg);

		if (pMapDlg != NULL)
		{
			pMapDlg->InvalidateRect(NULL);
		}
	}
}

void CDMHelperDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	PositionMapViews();

	InvalidateRect(NULL);
}


void CDMHelperDlg::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	PositionMapViews();
}


void CDMHelperDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class

	for (int i = 0; i < m_pApp->m_TabArray.GetSize(); ++i)
	{
		cDNDDisplayTab *pTab = m_pApp->m_TabArray[i];
		delete pTab;
	}
	
	CDialog::PostNcDestroy();
}


void CDMHelperDlg::OnChangeEncFactorEdit() 
{
	UpdateData(TRUE);

	m_pApp->m_Settings.m_nGPEncumbranceFactor = atoi(m_szEncumbranceFactorEdit.GetBuffer(0));
}

void CDMHelperDlg::OnChangeInflationFactorEdit() 
{
	UpdateData(TRUE);

	m_pApp->m_Settings.m_nVendorPriceInflationFactor = abs(atoi(m_szInflationFactor.GetBuffer(0)));
}

void CDMHelperDlg::OnHelpAboutdungeonmaestro()
{
	CAboutDlg *pDlg = new CAboutDlg();
	pDlg->DoModal();
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	LONG lValue = DMH_CURRENT_VERSION;

	//10000 
	LONG lMajor = DMH_CURRENT_VERSION / 10000L;
	lValue -= lMajor * 10000L;

	LONG lMinor = lValue / 1000L;
	lValue -= lMinor * 1000L;

	m_szVersion.Format("Version: %01d.%01d.%03d", lMajor, lMinor, lValue);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}


void CDMHelperDlg::OnDicerollerOpendicerroller()
{
	if(m_pDiceRollerDialog == NULL)
	{
		m_pDiceRollerDialog = new CDMDiceRollerDialog(this);
	}
	else
	{
		m_pDiceRollerDialog->SendMessage ( WM_SYSCOMMAND, SC_RESTORE, 0 ); 
	}
}

void CDMHelperDlg::OnFlipSecondaryScreen()
{
	DISPLAY_DEVICE dd;
	dd.cb = sizeof(DISPLAY_DEVICE);

	BOOL bFoundSecondaryScreen = FALSE;

	BOOL bDone = FALSE;
	DWORD deviceNum = 0;
	while (bDone == FALSE && EnumDisplayDevices(NULL, deviceNum, &dd, 0))
	{
		//DumpDevice(dd, 0);
		DISPLAY_DEVICE newdd = { 0 };
		newdd.cb = sizeof(DISPLAY_DEVICE);
		DWORD monitorNum = 0;
		while (bDone == FALSE && EnumDisplayDevices(dd.DeviceName, monitorNum, &newdd, 0))
		{
			monitorNum++;

			if (deviceNum == 1 && monitorNum == 1)
			{
				bDone = TRUE;
				bFoundSecondaryScreen = TRUE;
			}
		}
		deviceNum++;
	}

	if (bFoundSecondaryScreen == FALSE)
	{
		return;
	}

	DEVMODE devMode;
	long r;

	// Init DEVMODE to current settings
	ZeroMemory(&devMode, sizeof(DEVMODE));
	devMode.dmSize = sizeof(devMode);

	EnumDisplaySettingsEx(dd.DeviceName, ENUM_CURRENT_SETTINGS, &devMode, NULL);
	//ShowDevMode(devMode);

	/* Rotate Orientation - 180 */

	if (m_nOrientation == DMDO_DEFAULT)
		m_nOrientation = DMDO_180;
	else
		m_nOrientation = DMDO_DEFAULT;

	devMode.dmDisplayOrientation = m_nOrientation;

	/* Rotate Orientation - 90 */
	//devMode.dmDisplayOrientation = DMDO_90;

	//swap(devMode.dmPelsHeight, devMode.dmPelsWidth);
	//int nSwap = devMode.dmPelsHeight;
	//devMode.dmPelsHeight = devMode.dmPelsWidth;
	//devMode.dmPelsWidth = nSwap;

	devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYORIENTATION;
	r = ChangeDisplaySettingsEx(dd.DeviceName, &devMode, NULL, CDS_RESET, NULL);

	switch (r)
	{
	case DISP_CHANGE_SUCCESSFUL:
		//cout << "ChangeDisplaySettingsEx returns DISP_CHANGE_SUCCESSFUL" << endl;
		break;
	case DISP_CHANGE_BADDUALVIEW:
		//cout << "ChangeDisplaySettingsEx returns DISP_CHANGE_BADDUALVIEW" << endl;
		break;
	case DISP_CHANGE_BADFLAGS:
		//cout << "ChangeDisplaySettingsEx returns DISP_CHANGE_BADFLAGS" << endl;
		break;
	case DISP_CHANGE_BADMODE:
		//cout << "ChangeDisplaySettingsEx returns DISP_CHANGE_BADMODE" << endl;
		break;
	case DISP_CHANGE_BADPARAM:
		//cout << "ChangeDisplaySettingsEx returns DISP_CHANGE_BADPARAM" << endl;
		break;
	case DISP_CHANGE_FAILED:
		//cout << "ChangeDisplaySettingsEx returns DISP_CHANGE_FAILED" << endl;
		break;
	case DISP_CHANGE_NOTUPDATED:
		//cout << "ChangeDisplaySettingsEx returns DISP_CHANGE_NOTUPDATED" << endl;
		break;
	case DISP_CHANGE_RESTART:
		//cout << "ChangeDisplaySettingsEx returns DISP_CHANGE_RESTART" << endl;
		break;
	default:
		//cout << "ChangeDisplaySettingsEx - Unexpected return value." << endl;
		break;

	}
}

BOOL CDMHelperDlg::PreTranslateMessage(MSG* pMsg) 
{
	/*
	//if(pMsg->hwnd == m_cFileLister.m_hWnd)
	{
		switch(pMsg->message)
		{
			case WM_KEYDOWN:
			{
				if(pMsg->wParam == VK_DELETE)
				{
					TRACE("TRIP ! %d %d\n", pMsg->wParam, VK_DELETE);
				}
				break;
			}
		}
	}
	*/
	
	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CDMHelperDlg::OnHotKey(WPARAM wParam, LPARAM lParam)
{

	switch(wParam)
	{
		case 100:  // CTRL+R
		{
			OnDicerollerOpendicerroller();
			break;
		}
		case 200:
		{
			OnFlipSecondaryScreen();
			break;
		}
	}

	return 0;
}



void CDMHelperDlg::OnBnClickedSaveButton()
{
	CheckSaveData();
}


void CDMHelperDlg::OnSubpartiesAddsubparty()
{
	if(m_pSelectedPartyWindow != NULL)
	{
		m_pSelectedPartyWindow->ExternalSubPartyCommand(NULL, 0);
		SortTabs();
	}
}

void CDMHelperDlg::OnSubpartiesRemovesubparty()
{
	if(m_pSelectedPartyWindow != NULL)
	{
		m_pSelectedPartyWindow->ExternalSubPartyCommand(NULL, 0);
		SortTabs();
	}
}

void CDMHelperDlg::OnSubpartiesSortsubparties()
{
	if (m_pSelectedPartyWindow == NULL)
	{
		return;
	}

	cDNDSubParty _SubParty;

	for (int i = 0; i < MAX_SUB_PARTIES - 1; ++i)
	{
		for (int j = i + 1; j < MAX_SUB_PARTIES; ++j)
		{
			if (m_pSelectedPartyWindow->m_pParty->m_SubParty[i].m_dwSubPartyID && m_pSelectedPartyWindow->m_pParty->m_SubParty[j].m_dwSubPartyID)
			{
				TRACE("COMPARE : %s to %s\n", m_pSelectedPartyWindow->m_pParty->m_SubParty[j].m_szSubPartyName, m_pSelectedPartyWindow->m_pParty->m_SubParty[i].m_szSubPartyName);

				CString szI = m_pSelectedPartyWindow->m_pParty->m_SubParty[i].m_szSubPartyName;
				CString szJ = m_pSelectedPartyWindow->m_pParty->m_SubParty[j].m_szSubPartyName;

				if ( szJ < szI )
				{
					// buff = i
					memcpy(&_SubParty, &m_pSelectedPartyWindow->m_pParty->m_SubParty[i], sizeof(cDNDSubParty));
					// i = j
					memcpy(&m_pSelectedPartyWindow->m_pParty->m_SubParty[i], &m_pSelectedPartyWindow->m_pParty->m_SubParty[j], sizeof(cDNDSubParty));
					// j = buff
					memcpy(&m_pSelectedPartyWindow->m_pParty->m_SubParty[j], &_SubParty, sizeof(cDNDSubParty));
				}
			}
		}
	}

	ResetSubPartyTabs();
}

void CDMHelperDlg::OnSubpartiesSortsubpartiesnumerically()
{
	if (m_pSelectedPartyWindow == NULL)
	{
		return;
	}

	cDNDSubParty _SubParty;
	char sChar[2];

	for (int i = 0; i < MAX_SUB_PARTIES - 1; ++i)
	{
		for (int j = i + 1; j < MAX_SUB_PARTIES; ++j)
		{
			if (m_pSelectedPartyWindow->m_pParty->m_SubParty[i].m_dwSubPartyID && m_pSelectedPartyWindow->m_pParty->m_SubParty[j].m_dwSubPartyID)
			{
				TRACE("COMPARE : %s to %s\n", m_pSelectedPartyWindow->m_pParty->m_SubParty[j].m_szSubPartyName, m_pSelectedPartyWindow->m_pParty->m_SubParty[i].m_szSubPartyName);

				CString szI = m_pSelectedPartyWindow->m_pParty->m_SubParty[i].m_szSubPartyName;
				CString szJ = m_pSelectedPartyWindow->m_pParty->m_SubParty[j].m_szSubPartyName;

				szI.MakeUpper();
				szJ.MakeUpper();

				for (int c = 65; c <= 91; ++c)
				{
					sChar[0] = c;
					sChar[1] = 0;

					CString szReplace = sChar;

					szI.Replace(szReplace, "");
					szJ.Replace(szReplace, "");
				}

				int nI = atoi(szI.GetBuffer(0));
				int nJ = atoi(szJ.GetBuffer(0));

				if (nJ < nI)
				{
					// buff = i
					memcpy(&_SubParty, &m_pSelectedPartyWindow->m_pParty->m_SubParty[i], sizeof(cDNDSubParty));
					// i = j
					memcpy(&m_pSelectedPartyWindow->m_pParty->m_SubParty[i], &m_pSelectedPartyWindow->m_pParty->m_SubParty[j], sizeof(cDNDSubParty));
					// j = buff
					memcpy(&m_pSelectedPartyWindow->m_pParty->m_SubParty[j], &_SubParty, sizeof(cDNDSubParty));
				}
			}
		}
	}

	ResetSubPartyTabs();
}

void CDMHelperDlg::ResetSubPartyTabs()
{
	if (m_pSelectedPartyWindow == NULL)
	{
		return;
	}

	for (int i = 0; i < MAX_SUB_PARTIES; ++i)
	{
		if (m_pSelectedPartyWindow->m_pParty->m_SubParty[i].m_dwSubPartyID)
		{
			FindAndDeleteTab(DND_TAB_TYPE_SUBPARTY, m_pSelectedPartyWindow->m_pParty->m_SubParty[i].m_dwSubPartyID, FALSE);
		}
	}

	for (int i = 0; i < MAX_SUB_PARTIES; ++i)
	{
		if (m_pSelectedPartyWindow->m_pParty->m_SubParty[i].m_dwSubPartyID)
		{
			CDMSubPartyDialog *pSubPartyDialog = new CDMSubPartyDialog(&m_pSelectedPartyWindow->m_pParty->m_SubParty[i], m_pSelectedPartyWindow);
			AddTab(pSubPartyDialog, DND_TAB_TYPE_SUBPARTY, FALSE);
		}
	}

	SortTabs();
}

DMPartyDialog *CDMHelperDlg::GenerateRandomEncounterTabs(cDNDParty *pParty, cDNDSubParty *pSubParty, DMPartyDialog *pMainPartyDialog, BOOL bPickTab)
{
	if(pParty == NULL && pSubParty == NULL)
		return NULL;

	DMPartyDialog *pDlg = pMainPartyDialog;

	if(pSubParty == NULL)
	{
		pDlg = new DMPartyDialog(m_pApp->m_pMainWindow, pParty, &m_pApp->m_pMainWindow->m_cMainTab);

		pDlg->m_pParty = pParty;

		m_pApp->m_PartyViewMap.SetAt((WORD)pParty->m_dwPartyID, pDlg);

		pDlg->Initialize();

		m_pApp->m_pMainWindow->AddTab(pDlg, DND_TAB_TYPE_PARTY, bPickTab);	
	}

	for(int i = 0;  i < MAX_NPC_PARTY_MEMBERS; ++i)
	{
		if(pParty->m_NPCs[i].m_dwCharacterID == 0)
			break;

		if(pSubParty != NULL && pParty->m_NPCs[i].m_dwSubPartyID != pSubParty->m_dwSubPartyID)
			continue;

		pParty->AddPartyMemberID(pParty->m_NPCs[i].m_dwCharacterID);
		//pParty->m_dwPartyRoster[i] = pParty->m_NPCs[i].m_dwCharacterID;

		if(pParty->m_NPCs[i].m_nMonsterIndex >= 1000)
		{
			pParty->m_NPCs[i].m_dwPartyID = pParty->m_dwPartyID;

			cDNDNonPlayerCharacter *pNPC = &pParty->m_NPCs[i];

			cDMBaseNPCViewDialog *pNPCDlg = new cDMBaseNPCViewDialog(m_pApp->m_pMainWindow, pNPC, NULL, &m_pApp->m_pMainWindow->m_cMainTab);

			pNPCDlg->OnRollStats();

			m_pApp->m_pMainWindow->AddTab(pNPCDlg, DND_TAB_TYPE_NPC, FALSE);

			m_pApp->m_NPCViewMap.SetAt((WORD)pNPCDlg->m_pNPC->m_dwCharacterID, pNPCDlg);
		}
		else
		{
			//this is a character
			cDNDCharacter *pCharacter = new cDNDCharacter();
			cDNDCharacter *pEnPeeCee = (cDNDCharacter *)&pParty->m_NPCs[i];

			memcpy(pCharacter, pEnPeeCee, sizeof(cDNDCharacter));

			pParty->m_NPCs[i].m_dwCharacterID = 0; //remove it from NPC space on party ValidateNPCList
			memset(&pParty->m_NPCs[i], 0, sizeof(cDNDNonPlayerCharacter));
			
			pCharacter->m_lExperience[0] = GetExperiencePointsForLevelByClass(pCharacter->m_Class[0], pCharacter->m_nLevel[0]);
			pCharacter->m_lExperience[1] = GetExperiencePointsForLevelByClass(pCharacter->m_Class[0], pCharacter->m_nLevel[1]);
			pCharacter->m_lExperience[2] = GetExperiencePointsForLevelByClass(pCharacter->m_Class[0], pCharacter->m_nLevel[2]);
			
			CDMCharViewDialog *pCharDlg = new CDMCharViewDialog(m_pApp->m_pMainWindow, pCharacter, &m_pApp->m_pMainWindow->m_cMainTab);

			m_pApp->m_pMainWindow->AddTab(pCharDlg, DND_TAB_TYPE_CHARACTER, FALSE);

			pCharDlg->NPCRollStats();

			pCharDlg->ProcessCharStats();
			pCharDlg->Refresh();

			if(GetSpellClasses(pCharacter))
			{
				NPCharacterGenerateSpells(pCharacter);
			}

			m_pApp->m_CharacterViewMap.SetAt((WORD)pCharDlg->m_pCharacter->m_dwCharacterID, pCharDlg);
		}

	}

	pDlg->Initialize();

	pDlg->Refresh();

	return pDlg;

}

BOOL CDMHelperDlg::StartDungeonMaestroUpdate()
{
	return DoCreateProcess("DM Update.exe", "DMInvoked");
}

BOOL CDMHelperDlg::DoCreateProcess(const char *pcszProcName, const char *pcszArgs)
{
	STARTUPINFO sui;
	PROCESS_INFORMATION pi;
	int nRet;

	//set up the STARTUPINFO structure,
	// then call CreateProcess to try and start the new exe.
	sui.cb = sizeof (STARTUPINFO);
	sui.lpReserved = 0;
	sui.lpDesktop = NULL;
	sui.lpTitle = NULL;
	sui.dwX = 0;
	sui.dwY = 0;
	sui.dwXSize = 0;
	sui.dwYSize = 0;
	sui.dwXCountChars = 0;
	sui.dwYCountChars = 0;
	sui.dwFillAttribute = 0;
	sui.dwFlags = 0;
	sui.wShowWindow = 0;
	sui.cbReserved2 = 0;
	sui.lpReserved2 = 0;

	char szCommandLine[512];
	char *pCommandLine = szCommandLine;

	if(pcszProcName)
	{
		if(pcszArgs)
			sprintf(szCommandLine,"%s %s", pcszProcName, pcszArgs);
		else
			sprintf(szCommandLine,"%s", pcszProcName);
	}
	else
	{
		if(pcszArgs)
			sprintf(szCommandLine,"%s", pcszArgs);
		else
			pCommandLine = NULL;
	}

	nRet = ::CreateProcess(NULL,// pointer to name of executable module 
		pCommandLine,// pointer to command line string
		NULL,// pointer to process security attributes 
		NULL,// pointer to thread security attributes 
		FALSE,// handle inheritance flag 
		DETACHED_PROCESS,// creation flags 
		NULL,// pointer to new environment block 
		NULL,// pointer to current directory name 
		&sui,// pointer to STARTUPINFO 
		&pi );// pointer to PROCESS_INFORMATION 

	return nRet;
}


void CDMHelperDlg::OnHelpCheckforupdates()
{
	m_bCloseandUpdate = TRUE;

	PostMessage(WM_CLOSE);
}


void CDMHelperDlg::OnCustomizationsCustomitems()
{
	CDMCustomItemsDialog *pDlg = new CDMCustomItemsDialog(this);

	pDlg->DoModal();

	delete pDlg;
}

void CDMHelperDlg::OnCustomizationsCustomspelleditor()
{
	CDMCustomSpellDialog *pDlg = new CDMCustomSpellDialog(this);

	pDlg->DoModal();

	delete pDlg;
}

void CDMHelperDlg::OnCustomizationsCustommonster()
{
	CDMCustomMonsterDialog *pDlg = new CDMCustomMonsterDialog(this);

	pDlg->DoModal();

	delete pDlg;
}

void CDMHelperDlg::OnCustomizationsCustomarmoreditor()
{
	CDMCustomArmorDialog *pDlg = new CDMCustomArmorDialog(this);

	pDlg->DoModal();

	delete pDlg;
}

void CDMHelperDlg::OnCustomizationsCustomweaponeditor()
{
	CDMCustomWeaponDialog *pDlg = new CDMCustomWeaponDialog(this);

	pDlg->DoModal();

	delete pDlg;
}

BOOL CDMHelperDlg::FileExists(CString szPath)
{

	FILE *pInFile = fopen(szPath, "rb");

	if (NULL == pInFile)
	{
		return FALSE;
	}

	fclose(pInFile);

	return TRUE;
}

void CDMHelperDlg::OpenPDFDocument(CString szFileName, int nPage)
{
	CString szPDFPath;

	szPDFPath.Format("%s\\data\\PDF", m_pApp->m_szEXEPath);
	CreateDirectory(szPDFPath, NULL);

	#if KEARY_BUILD

	WORD wID;
	for (POSITION pos = m_pApp->m_PDFViewMap.GetStartPosition(); pos != NULL;)
	{
		PDNDPDFVIEWDLG pDlg = NULL;
		m_pApp->m_PDFViewMap.GetNextAssoc(pos, wID, pDlg);

		if (pDlg != NULL && pDlg->m_hWnd != NULL)
		{
			pDlg->Cleanup();
			pDlg->PostMessage(WM_CLOSE);
		}
	}

#if 0
	// this doesn't work with a shit

	CString szOCXPath = "\"" + m_pApp->m_szEXEPath + "\\PDFViewer.ocx" + "\"";

	HINSTANCE hInst;
	hInst = ShellExecute(NULL, "open", "regsvr32", szOCXPath, NULL, SW_HIDE);
	// ** if registration failed: show a message box
	
	if ((int)hInst <= 32)
	{
		AfxMessageBox("Register PDFViewer.ocx failed");
	}
#endif

	// gotta buy the ocx from:
	// http://www.viscomsoft.com/purchase.php?id=1
	
	szPDFPath.Format("%s\\data\\%s", m_pApp->m_szEXEPath, szFileName);

	cDMPDFViewDialog *pDlg = new cDMPDFViewDialog(szPDFPath, nPage);

	m_pApp->m_PDFViewMap.SetAt((WORD)pDlg->m_dwPDFID, pDlg);

	//pDlg->DoModal();
	//delete pDlg;

	#else

	//DMGuide.pdf%20#page=9


	szPDFPath.Format("%s\\data\\%s", m_pApp->m_szEXEPath, szFileName);

	if (FALSE == FileExists(szPDFPath))
	{
		szFileName = "PDF\\PDFNotFound.pdf";
	}

	//start chrome "FILE:\\D:\SOURCE\DM HELPER\.\RELEASE\\data\PDF\DMGuide.pdf #page=9"
	//szPDFPath.Format("chrome %s\\data\\%s%c20#page=%d", m_pApp->m_szEXEPath, szFileName,'%', nPage);
	szPDFPath.Format("\"FILE:\\%s\\data\\%s #page=%d\"", m_pApp->m_szEXEPath, szFileName, nPage);

	//UINT nOK = WinExec(szPDFPath, SW_SHOW);

	CString szParms = "";
	szParms.Format("?#page=%d", nPage);

	//LONG lTry = ERROR_FILE_NOT_FOUND;
	HINSTANCE hRet = ShellExecute(AfxGetMainWnd()->GetSafeHwnd(), "open", "chrome", szPDFPath, NULL, SW_SHOWNORMAL);

	/*
	SHELLEXECUTEINFO ShExecInfo;

	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = NULL;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = szPDFPath.GetBuffer(0);
	ShExecInfo.lpParameters = NULL;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_MAXIMIZE;
	ShExecInfo.hInstApp = NULL;

	ShellExecuteEx(&ShExecInfo);
	*/
	
	#endif
}

void CDMHelperDlg::OnHelpDungeonmaestrouser()
{
	OpenPDFDocument("Manual\\DungeonMaestroManual.pdf", 1);

	/*
	CString szManualPath;
	szManualPath.Format("%s\\data\\Manual\\DungeonMaestroManual.pdf", m_pApp->m_szEXEPath);


	SHELLEXECUTEINFO ShExecInfo;

	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = NULL;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = szManualPath.GetBuffer(0);
	ShExecInfo.lpParameters = NULL;
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_MAXIMIZE;
	ShExecInfo.hInstApp = NULL;

	ShellExecuteEx(&ShExecInfo);
	*/

}

void CDMHelperDlg::OnDmlibraryDungeonmaster()
{
	OpenPDFDocument("PDF\\DMGuide.pdf", 1);
}

void CDMHelperDlg::OnDmlibraryPlayer()
{
	OpenPDFDocument("PDF\\PlayersHandbook.pdf", 1);
}


void CDMHelperDlg::OnDmlibraryMonstermanual()
{
	OpenPDFDocument("PDF\\MonsterManual.pdf", 1);
}


void CDMHelperDlg::OnDmlibraryMonstermanual2()
{
	OpenPDFDocument("PDF\\MonsterManual2.pdf", 1);
}


void CDMHelperDlg::OnDmlibraryFiendfolio()
{
	OpenPDFDocument("PDF\\FiendFolio.pdf", 1);
}


void CDMHelperDlg::OnDmlibraryUnearthedarcana()
{
	OpenPDFDocument("PDF\\UnearthedArcana.pdf", 1);
}


void CDMHelperDlg::OnDmlibraryFromfile()
{
	CString szPath;

	CString szDirectory;
	szDirectory.Format("%s//Data//PDF", m_pApp->m_szEXEPath.GetBuffer(0));

	ZeroMemory(&g_PDF_ofn, sizeof(OPENFILENAME));
	g_PDF_ofn.lStructSize = sizeof(OPENFILENAME);
	g_PDF_ofn.nMaxFile = MAX_PATH;
	g_PDF_ofn.nMaxFileTitle = MAX_PATH;
	g_PDF_ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	g_PDF_ofn.hwndOwner = m_hWnd;
	g_PDF_ofn.hInstance = m_pApp->m_hInstance;
	g_PDF_ofn.lpstrFile = g_PDF_szFilename;
	g_PDF_ofn.lpstrInitialDir = szDirectory.GetBuffer(0);
	g_PDF_ofn.lpstrTitle = "Load PDF file";
	//g_ofn.lpstrFilter = "Graphics Files (*.bmp)\0*.bmp\0(*.gif)\0*.gif\0(*.jpg)\0*.jpg\0All Files (*.*)\0*.*\0\0";
	g_PDF_ofn.lpstrFilter = "PDF Files (*.pdf)\0*.pdf\0All Files (*.*)\0*.*\0\0";
	g_PDF_ofn.lpstrDefExt = "pdf";

	// Get action template file name
	if (GetOpenFileName(&g_PDF_ofn))
	{
		szPath = g_PDF_szFilename;
		szPath.MakeUpper();

		int nPos = szPath.Find("PDF");

		CString szPDF = szPath.Mid(nPos, szPath.GetLength());

		OpenPDFDocument(szPDF, 1);

	}
}
