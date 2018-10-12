// DMPartyDialog.cpp : implementation file
//

#include "stdafx.h"	
#include "DM Helper.h"
#include "DM HelperDlg.h"
#include "DMLoadFileDialog.h"
#include "DMFileManagementDialog.h"
#include "DMCharSpellsDialog.h"
#include "DMInventoryDialog.h"
#include "DMCharViewDialog.h"
#include "DMNPCViewDialog.h"
#include "cDMMapViewDialog.h"
#include "DMCharacterSelectorDialog.h"
#include "DMCastSpellDialog.h"
#include "DMModifyValueDialog.h"
#include "DMTimeEditDialog.h"
#include "DMPartyDialog.h"
#include "DMSubPartyDialog.h"
#include "DMCalendarDialog.h"
#include "DMPartyLogDialog.h"
#include "DMEventLoggerDialog.h"
#include "DMInitiativeDialog.h"
#include "DMRandomEncounterDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT DMPartyTimeThreadProc(LPVOID pData) 
{
	DMPartyDialog *pPartyDlg = (DMPartyDialog*)pData;

	int nCount = 0;

	do
	{
		Sleep(10);

		if(pPartyDlg->m_pPartyClockThread != NULL)
		{
			++nCount;

			if(nCount > 100)
			{
				nCount = 0;

				pPartyDlg->m_pParty->ModifyCalendar(DND_CALENDAR_MOD_SECOND, DND_CALENDAR_MOD_INCREMENT);

				pPartyDlg->PostMessageA(DND_WM_MESSAGE, 0, 0);
			}
		}

	} while(pPartyDlg->m_pPartyClockThread != NULL);

	TRACE("PARTY TIME THREAD EXITING !\n");

	return 0;
}



/////////////////////////////////////////////////////////////////////////////
// DMPartyDialog dialog


DMPartyDialog::DMPartyDialog(CDMHelperDlg* pMainDialog, cDNDParty *pParty, CWnd* pParent /*=NULL*/)
	: CDialog(DMPartyDialog::IDD, pParent)
	, m_szPartyXP(_T(""))
	, m_szPartyNameStatic(_T("Party Name:"))
{
	//{{AFX_DATA_INIT(DMPartyDialog)
	m_szPartyName = _T("New Party");
	m_szPartyMapName = _T("");
	m_szSegmentEdit = _T("");
	m_szRoundEdit = _T("");
	m_szPartyLegend = _T("");
	//}}AFX_DATA_INIT

	m_bInitDialogCalled = FALSE;

	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_pMainDialog = pMainDialog;
	m_pParent = pParent;

	if(pParty == NULL)
	{
		m_pParty = new cDNDParty();
	}
	else
	{
		m_pParty = pParty;
		m_szPartyName = m_pParty->m_szPartyName;
	}

	m_pPartyLog = NULL;

	m_dwParentPartyID = 0;
	m_dwSubPartyID = 0;
	m_pSubParty = NULL;
	m_pSubPartyDialog = NULL;

	m_pOpposingPartyDialog = NULL;
	m_pOpposingSubParty = NULL;

	m_pBaseCharDlg = NULL;
	m_pSelectedCharacterDialog = NULL;
	m_pOldSelectedCharacterDialog = NULL;

	m_pSelectedNPCDialog = NULL;
	m_pOldSelectedNPCDialog = NULL;

	m_pBaseOpposingCharDlg = NULL;
	m_pOpposingCharacterDialog = NULL;
	m_pOldOpposingCharacterDialog = NULL;

	m_pOpposingNPCDialog = NULL;
	m_pOldOpposingNPCDialog = NULL;

	m_pCalendarDialog = NULL;
	m_pPartyLogDialog = NULL;
	m_pEventLoggerDialog = NULL;
	m_pInitiativeDialog = NULL;

	m_szSaveFileName = _T("");
	m_lPartyHash = 0L;

	m_nPartyScrollPos = 0;
	m_nOpponentPartyScrollPos = 0;
	
	m_pPartyClockThread = NULL;

	m_pPortraitBackDropBitmap = NULL;
	m_pSelectedCharacterBitmap = NULL;
	m_pSelectedOpponentBitmap = NULL;

	Create(DMPartyDialog::IDD, pParent);
}

DMPartyDialog::~DMPartyDialog()
{
	if (m_pPortraitBackDropBitmap != NULL)
	{
		::delete m_pPortraitBackDropBitmap;
		m_pPortraitBackDropBitmap = NULL;
	}
	if (m_pSelectedCharacterBitmap != NULL)
	{
		::delete m_pSelectedCharacterBitmap;
		m_pSelectedCharacterBitmap = NULL;
	}
	if (m_pSelectedOpponentBitmap != NULL)
	{
		::delete m_pSelectedOpponentBitmap;
		m_pSelectedOpponentBitmap = NULL;
	}
}

void DMPartyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DMPartyDialog)
	DDX_Control(pDX, IDC_CAST_SPELL_BUTTON2, m_cCastSpellButton2);
	DDX_Control(pDX, IDC_HEAL_BUTTON2, m_cHealButton2);
	DDX_Control(pDX, IDC_WOUND_BUTTON2, m_cWoundButton2);
	DDX_Control(pDX, IDC_MISSILE_MISS_BUTTON2, m_cMissileMissButton2);
	DDX_Control(pDX, IDC_MISSILE_HIT_BUTTON2, m_cMissileHitButton2);
	DDX_Control(pDX, IDC_ATTACK_MISS_BUTTON2, m_cAttackMissButton2);
	DDX_Control(pDX, IDC_ATTACK_HIT_BUTTON2, m_cAttackHitButton2);
	DDX_Control(pDX, IDC_PARTY_COMBO2, m_cPartyCombo2);
	DDX_Control(pDX, IDC_PARTY_LIST2, m_cPartyList2);
	DDX_Control(pDX, IDC_DOWN_PARTY_BUTTON2, m_cDownPartyButton2);
	DDX_Control(pDX, IDC_UP_PARTY_BUTTON2, m_cUpPartyButton2);
	DDX_Control(pDX, IDC_MISSILE_MISS_BUTTON, m_cMissileMissButton);
	DDX_Control(pDX, IDC_MISSILE_HIT_BUTTON, m_cMissileHitButton);
	DDX_Control(pDX, IDC_CAST_SPELL_BUTTON, m_cCastSpellButton);
	DDX_Control(pDX, IDC_HEAL_BUTTON, m_cHealButton);
	DDX_Control(pDX, IDC_WOUND_BUTTON, m_cWoundButton);
	DDX_Control(pDX, IDC_ATTACK_MISS_BUTTON, m_cAttackMissButton);
	DDX_Control(pDX, IDC_ATTACK_HIT_BUTTON, m_cAttackHitButton);
	DDX_Control(pDX, IDC_DOWN_PARTY_BUTTON, m_cDownPartyButton);
	DDX_Control(pDX, IDC_UP_PARTY_BUTTON, m_cUpPartyButton);
	DDX_Control(pDX, IDC_PARTY_LEGEND, m_cPartyLegend);
	DDX_Control(pDX, IDC_CALENDAR_COMBO, m_cCalendarCombo);
	DDX_Control(pDX, IDC_MAP_NAME_EDIT, m_cPartyMapNameEdit);
	DDX_Control(pDX, IDC_ANCHOR_BUTTON, m_cAnchorButton);
	DDX_Control(pDX, IDC_PARTY_LIST, m_cPartyList);
	DDX_Text(pDX, IDC_PARTY_NAME_EDIT, m_szPartyName);
	DDX_Text(pDX, IDC_MAP_NAME_EDIT, m_szPartyMapName);
	DDX_Text(pDX, IDC_SEGMENT_EDIT, m_szSegmentEdit);
	DDX_Text(pDX, IDC_ROUND_EDIT, m_szRoundEdit);
	DDX_Text(pDX, IDC_PARTY_LEGEND, m_szPartyLegend);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COPY_MEMBER_BUTTON, m_cCopyMemberButton);
	DDX_Control(pDX, IDC_PASTE_MEMBER_BUTTON, m_cPasteMemberButton);
	DDX_Control(pDX, IDC_VIEW_CALENDAR_BUTTON, m_cViewCalendarButton);
	DDX_Control(pDX, IDC_VIEW_LOG_BUTTON, m_cViewLogButton);
	DDX_Text(pDX, IDC_PARTY_XP_EDIT, m_szPartyXP);
	DDX_Control(pDX, IDC_GRANT_PARTY_XP_BUTTON, m_cGrantPartyXPButton);
	DDX_Control(pDX, IDC_LOG_EVENT_BUTTON, m_cLogButton);
	DDX_Control(pDX, IDC_KILL_FOR_XP_BUTTON, m_cKillForXPButton);
	DDX_Control(pDX, IDC_SWAP_WEAPON_BUTTON_1, m_cSwapWeaponButton);
	DDX_Control(pDX, IDC_SWAP_WEAPON_BUTTON_2, m_cSwapWeaponButton2);
	DDX_Control(pDX, IDC_PARTY_INITIATIVE_BUTTON, m_cViewInitiativeButton);
	DDX_Control(pDX, IDC_SUB_PARTY_BUTTON, m_cSubPartyButton);
	DDX_Text(pDX, IDC_PARTY_NAME_STATIC, m_szPartyNameStatic);
	DDX_Control(pDX, IDC_SEGMENT_EDIT, m_cSegmentEdit);
	DDX_Control(pDX, IDC_ROUND_EDIT, m_cRoundEdit);
}


BEGIN_MESSAGE_MAP(DMPartyDialog, CDialog)
	//{{AFX_MSG_MAP(DMPartyDialog)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_PARTY_NAME_EDIT, OnChangePartyNameEdit)
	ON_BN_CLICKED(IDC_ADD_MEMBER_BUTTON, OnAddMemberButton)
	ON_BN_CLICKED(IDC_DELETE_MEMBER_BUTTON, OnDeleteMemberButton)
	ON_BN_CLICKED(IDC_ADD_LOAD_PARTY_BUTTON, OnLoadPartyButton)
	ON_BN_CLICKED(IDC_DELETE_SAVE_PARTY_BUTTON, OnSavePartyButton)
	ON_EN_SETFOCUS(IDC_MAP_NAME_EDIT, OnSetfocusMapNameEdit)
	ON_CBN_SELCHANGE(IDC_CALENDAR_COMBO, OnSelchangeCalendarCombo)
	ON_BN_CLICKED(IDC_DEC_SEGMENT_BUTTON, OnDecSegmentButton)
	ON_BN_CLICKED(IDC_INC_SEGMENT_BUTTON, OnIncSegmentButton)
	ON_BN_CLICKED(IDC_DEC_ROUND_BUTTON, OnDecRoundButton)
	ON_BN_CLICKED(IDC_INC_ROUND_BUTTON, OnIncRoundButton)
	ON_BN_CLICKED(IDC_INC_EDIT_TIME_BUTTON, OnIncEditTimeButton)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_UP_PARTY_BUTTON, OnUpPartyButton)
	ON_BN_CLICKED(IDC_DOWN_PARTY_BUTTON, OnDownPartyButton)
	ON_BN_CLICKED(IDC_ATTACK_HIT_BUTTON, OnAttackHitButton)
	ON_BN_CLICKED(IDC_ATTACK_MISS_BUTTON, OnAttackMissButton)
	ON_BN_CLICKED(IDC_CAST_SPELL_BUTTON, OnCastSpellButton)
	ON_BN_CLICKED(IDC_WOUND_BUTTON, OnWoundButton)
	ON_BN_CLICKED(IDC_HEAL_BUTTON, OnHealButton)
	ON_NOTIFY(NM_CLICK, IDC_PARTY_LIST, OnClickPartyList)
	ON_BN_CLICKED(IDC_MISSILE_HIT_BUTTON, OnMissileHitButton)
	ON_BN_CLICKED(IDC_MISSILE_MISS_BUTTON, OnMissileMissButton)
	ON_CBN_SELCHANGE(IDC_PARTY_COMBO2, OnSelchangePartyCombo2)
	ON_NOTIFY(NM_CLICK, IDC_PARTY_LIST2, OnClickPartyList2)
	ON_BN_CLICKED(IDC_ATTACK_HIT_BUTTON2, OnAttackHitButton2)
	ON_BN_CLICKED(IDC_ATTACK_MISS_BUTTON2, OnAttackMissButton2)
	ON_BN_CLICKED(IDC_MISSILE_HIT_BUTTON2, OnMissileHitButton2)
	ON_BN_CLICKED(IDC_MISSILE_MISS_BUTTON2, OnMissileMissButton2)
	ON_BN_CLICKED(IDC_CAST_SPELL_BUTTON2, OnCastSpellButton2)
	ON_BN_CLICKED(IDC_WOUND_BUTTON2, OnWoundButton2)
	ON_BN_CLICKED(IDC_HEAL_BUTTON2, OnHealButton2)
	ON_BN_CLICKED(IDC_UP_PARTY_BUTTON2, OnUpPartyButton2)
	ON_BN_CLICKED(IDC_DOWN_PARTY_BUTTON2, OnDownPartyButton2)
	ON_BN_CLICKED(IDC_DELETE_NPC_BUTTON, OnDeleteNpcButton)
	ON_BN_CLICKED(IDC_CREATE_NPC_BUTTON, OnCreateNpcButton)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_HEAL_PARTY_BUTTON, &DMPartyDialog::OnBnClickedHealPartyButton)
	ON_BN_CLICKED(IDC_COPY_MEMBER_BUTTON, &DMPartyDialog::OnBnClickedCopyMemberButton)
	ON_BN_CLICKED(IDC_PASTE_MEMBER_BUTTON, &DMPartyDialog::OnBnClickedPasteMemberButton)
	ON_NOTIFY(NM_DBLCLK, IDC_PARTY_LIST, &DMPartyDialog::OnNMDblclkPartyList)
	ON_NOTIFY(NM_DBLCLK, IDC_PARTY_LIST2, &DMPartyDialog::OnNMDblclkPartyList2)
	ON_BN_CLICKED(IDC_VIEW_CALENDAR_BUTTON, &DMPartyDialog::OnBnClickedViewCalendarButton)
	ON_BN_CLICKED(IDC_VIEW_LOG_BUTTON, &DMPartyDialog::OnBnClickedViewLogButton)
	ON_BN_CLICKED(IDC_LOG_EVENT_BUTTON, &DMPartyDialog::OnBnClickedLogEventButton)
	ON_BN_CLICKED(IDC_ADD_XP_BUTTON, &DMPartyDialog::OnBnClickedAddXpButton)
	ON_BN_CLICKED(IDC_GRANT_PARTY_XP_BUTTON, &DMPartyDialog::OnBnClickedGrantPartyXpButton)
	ON_EN_CHANGE(IDC_PARTY_XP_EDIT, &DMPartyDialog::OnEnChangePartyXpEdit)
	ON_BN_CLICKED(IDC_KILL_FOR_XP_BUTTON, &DMPartyDialog::OnBnClickedKillForXpButton)
	ON_BN_CLICKED(IDC_SWAP_WEAPON_BUTTON_1, &DMPartyDialog::OnBnClickedSwapWeaponButton1)
	ON_BN_CLICKED(IDC_SWAP_WEAPON_BUTTON_2, &DMPartyDialog::OnBnClickedSwapWeaponButton2)
	ON_BN_CLICKED(IDC_PARTY_INITIATIVE_BUTTON, &DMPartyDialog::OnBnClickedPartyInitiativeButton)
	ON_BN_CLICKED(IDC_SUB_PARTY_BUTTON, &DMPartyDialog::OnBnClickedSubPartyButton)
	ON_BN_CLICKED(IDC_RANDOM_ENCOUNTER_BUTTON, &DMPartyDialog::OnBnClickedRandomEncounterButton)
	ON_MESSAGE(DND_WM_MESSAGE, OnDNDMessage)
	ON_BN_CLICKED(IDC_STAT_BLOCK_BUTTON, &DMPartyDialog::OnBnClickedStatBlockButton)
	ON_BN_CLICKED(IDC_STAT_BLOCK_BUTTON2, &DMPartyDialog::OnBnClickedStatBlockButton2)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DMPartyDialog message handlers

BOOL DMPartyDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_cUpPartyButton.LoadBitmaps(IDB_UP_ARROW_BITMAP, IDB_UP_ARROW_PRESSED_BITMAP);
	m_cDownPartyButton.LoadBitmaps(IDB_DOWN_ARROW_BITMAP, IDB_DOWN_ARROW_PRESSED_BITMAP);

	m_cUpPartyButton2.LoadBitmaps(IDB_UP_ARROW_BITMAP, IDB_UP_ARROW_PRESSED_BITMAP);
	m_cDownPartyButton2.LoadBitmaps(IDB_DOWN_ARROW_BITMAP, IDB_DOWN_ARROW_PRESSED_BITMAP);

	int nCount = 0;
	m_cPartyList.InsertColumn( nCount++, "Member", LVCFMT_LEFT, 100, -1 );
	m_cPartyList.InsertColumn( nCount++, "AC", LVCFMT_LEFT, 32, -1 );
	m_cPartyList.InsertColumn( nCount++, "HP", LVCFMT_LEFT, 50, -1 );
	m_cPartyList.InsertColumn( nCount++, "Weapon", LVCFMT_LEFT, 100, -1 );
	m_cPartyList.InsertColumn( nCount++, "#Att", LVCFMT_LEFT, 40, -1 );
	m_cPartyList.InsertColumn( nCount++, "10", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList.InsertColumn( nCount++, "9", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList.InsertColumn( nCount++, "8", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList.InsertColumn( nCount++, "7", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList.InsertColumn( nCount++, "6", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList.InsertColumn( nCount++, "5", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList.InsertColumn( nCount++, "4", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList.InsertColumn( nCount++, "3", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList.InsertColumn( nCount++, "2", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList.InsertColumn( nCount++, "1", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList.InsertColumn( nCount++, "0", LVCFMT_LEFT, 32,  -1 );

	m_cPartyList.InsertColumn( nCount++, "-1", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList.InsertColumn( nCount++, "-2", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList.InsertColumn( nCount++, "-3", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList.InsertColumn( nCount++, "-4", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList.InsertColumn( nCount++, "-5", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList.InsertColumn( nCount++, "-6", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList.InsertColumn( nCount++, "-7", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList.InsertColumn( nCount++, "-8", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList.InsertColumn( nCount++, "-9", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList.InsertColumn( nCount++, "-10", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList.InsertColumn( nCount++, "DAMAGE", LVCFMT_LEFT, 120,  -1 );
	m_cPartyList.InsertColumn( nCount++, "Ammo", LVCFMT_LEFT, 55, -1 );
	m_cPartyList.InsertColumn( nCount++, "Move", LVCFMT_LEFT, 50, -1 );
	//m_cPartyList.InsertColumn( nCount++, "WvAC", LVCFMT_LEFT, 50, -1 );


	nCount = 0;
	m_cPartyList2.InsertColumn( nCount++, "Member", LVCFMT_LEFT, 100, -1 );
	m_cPartyList2.InsertColumn( nCount++, "AC", LVCFMT_LEFT, 32, -1 );
	m_cPartyList2.InsertColumn( nCount++, "HP", LVCFMT_LEFT, 50, -1 );
	m_cPartyList2.InsertColumn( nCount++, "Weapon", LVCFMT_LEFT, 100, -1 );
	m_cPartyList2.InsertColumn( nCount++, "#Att", LVCFMT_LEFT, 40, -1 );
	m_cPartyList2.InsertColumn( nCount++, "10", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList2.InsertColumn( nCount++, "9", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList2.InsertColumn( nCount++, "8", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList2.InsertColumn( nCount++, "7", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList2.InsertColumn( nCount++, "6", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList2.InsertColumn( nCount++, "5", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList2.InsertColumn( nCount++, "4", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList2.InsertColumn( nCount++, "3", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList2.InsertColumn( nCount++, "2", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList2.InsertColumn( nCount++, "1", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList2.InsertColumn( nCount++, "0", LVCFMT_LEFT, 32,  -1 );

	m_cPartyList2.InsertColumn( nCount++, "-1", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList2.InsertColumn( nCount++, "-2", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList2.InsertColumn( nCount++, "-3", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList2.InsertColumn( nCount++, "-4", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList2.InsertColumn( nCount++, "-5", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList2.InsertColumn( nCount++, "-6", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList2.InsertColumn( nCount++, "-7", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList2.InsertColumn( nCount++, "-8", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList2.InsertColumn( nCount++, "-9", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList2.InsertColumn( nCount++, "-10", LVCFMT_LEFT, 32,  -1 );
	m_cPartyList2.InsertColumn( nCount++, "DAMAGE", LVCFMT_LEFT, 120,  -1 );
	m_cPartyList2.InsertColumn( nCount++, "Ammo", LVCFMT_LEFT, 55, -1 );
	m_cPartyList2.InsertColumn( nCount++, "Move", LVCFMT_LEFT, 50, -1 );
	//m_cPartyList2.InsertColumn( nCount++, "WvAC", LVCFMT_LEFT, 50, -1 );


	for(int i = 0; i < m_pApp->m_CalendarArray.GetSize(); ++i)
	{
		PCALENDAR pCalendar = m_pApp->m_CalendarArray.GetAt(i);

		if(pCalendar != NULL)
		{
			m_cCalendarCombo.InsertString(i, pCalendar->m_szCalendarName);
			m_cCalendarCombo.SetItemData(i, (ULONG)pCalendar);
		}
	}

	m_cKillForXPButton.EnableWindow(FALSE);

	m_pPortraitBackDropBitmap = ::new Bitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_MINI_PORTRAIT_BACKDROP_BITMAP)), FALSE);
	
	Refresh();

	ShowWindow(SW_HIDE);

	m_bInitDialogCalled = TRUE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void DMPartyDialog::PrePositionWindow()
{
	CRect top_rect;
	m_pApp->m_pMainWindow->GetWindowRect(&top_rect);

	CRect rect;
	m_cAnchorButton.GetWindowRect(&rect);

	int wsizex = top_rect.right - top_rect.left;
	int wsizey = top_rect.bottom - top_rect.top;

	int bsizex = rect.right - top_rect.right;
	int bsizey = rect.bottom - top_rect.bottom;

	if(bsizex > 0 || bsizey > 0)
	{
		if(bsizex < 0)
			bsizex = 0;
		
		if(bsizey < 0)
			bsizey = 0;

		m_pApp->m_pMainWindow->SetWindowPos(NULL, top_rect.left,top_rect.top, wsizex + bsizex, wsizey+bsizey+20, SWP_SHOWWINDOW);
	}
}

void DMPartyDialog::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
}

void DMPartyDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	//CDialog::OnCancel();
}

void DMPartyDialog::OnPaint() 
{
	RefreshTime();

	CPaintDC dc(this); // device context for painting

	Graphics graphics(dc);

	int nFMYP = 0;
	int nFMYO = 0;
	int nPSize = 64;

	if (m_pApp->m_Settings.m_bUseSmallFont)
	{
		nFMYP = -36;
		nFMYO = -70;
		nPSize = 56;
	}

	graphics.DrawImage(m_pPortraitBackDropBitmap, 64, 317 + nFMYP, nPSize + 4, nPSize + 4);
	try
	{
		if (m_pSelectedCharacterBitmap != NULL)
		{
			graphics.DrawImage(m_pSelectedCharacterBitmap, 66, 319 + nFMYP, nPSize, nPSize);
		}
	}
	catch (...)
	{
		TRACE("DrawImage(m_pSelectedCharacterBitmap ... FUBAR\n");
	}

	graphics.DrawImage(m_pPortraitBackDropBitmap, 64, 617 + nFMYO, nPSize+4, nPSize+4);
	try
	{
		if (m_pSelectedOpponentBitmap != NULL)
		{
			graphics.DrawImage(m_pSelectedOpponentBitmap, 66, 619 + nFMYO, nPSize, nPSize);
		}
	}
	catch (...)
	{
		TRACE("DrawImage(m_pSelectedOpponentBitmap ... FUBAR\n");
	}
	
	if(m_pParent != NULL)
	{
		CRect rect;
		m_pParent->GetClientRect(&rect);

		rect.top+=20;

		SetWindowPos(NULL, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);

		GetClientRect(&rect);
		m_cPartyLegend.MoveWindow(rect.left, rect.bottom-40, rect.right, rect.bottom, TRUE);
	}

	// Do not call CDialog::OnPaint() for painting messages
}

void DMPartyDialog::UpdateSelections() 
{
	if(m_pParty == NULL)
	{
		return;
	}

	if(m_pParty->m_lXP == 0L)
	{
		m_cGrantPartyXPButton.EnableWindow(FALSE);
	}
	else
	{
		m_cGrantPartyXPButton.EnableWindow(TRUE);
	}

	int nCursor = GetSelectedListCtrlItem(&m_cPartyList);
	m_cPartyList.m_nSelectedRow = nCursor;
	

	CString szTemp;

	int nRow = 0;
	BOOL bRemove = m_cPartyList.DeleteAllItems();

	m_pParty->m_nPartyMovementRate = 124;

	int nStartSegment = 0;
	int nEndSegment = 0;

	if(m_pInitiativeDialog != NULL)
	{
		nStartSegment = -10;
		nEndSegment = 20;
	}

	m_cPartyList.m_nSubSelectedColumn = -1;
	int nWvACType = -1;
	int nWvACAdj = 0;

	for(int nSegment = nStartSegment; nSegment <= nEndSegment; ++nSegment)
	{
		for(int i = 0; i < MAX_PARTY_MEMBERS; ++i)
		{
			if(m_pParty->m_dwPartyRoster[i] == 0)
				break;

			//TRACE("ADDING %d\n", i);

			PDNDCHARACTER pChar = NULL;

			PDNDCHARVIEWDLG pCharDlg = NULL;
			PDNDNPCVIEWDLG pNPCDlg = NULL;

			m_pApp->m_CharacterViewMap.Lookup((WORD)m_pParty->m_dwPartyRoster[i], pCharDlg);

			if(pCharDlg != NULL && pCharDlg->m_pCharacter != NULL)
			{
				if(m_dwSubPartyID && pCharDlg->m_pCharacter->m_dwSubPartyID != m_dwSubPartyID)
					continue;

				if(m_pInitiativeDialog != NULL)
				{
					//initiative sorting
					pCharDlg->m_nInitiativeSegment = pCharDlg->m_nInitiativeRoll - pCharDlg->m_nDexAdjToInitiative;

					if(pCharDlg->m_nInitiativeSegment != nSegment)
						continue;
				}

				pChar = pCharDlg->m_pCharacter;

				pCharDlg->m_nDayofWeek = m_pParty->m_nDayofWeek;
				pCharDlg->m_nDayofMonth = m_pParty->m_nDayofMonth;
				pCharDlg->m_nMonth = m_pParty->m_nMonth;
				pCharDlg->m_nYear = m_pParty->m_nYear;
			}

			if(pChar == NULL)
			{
				m_pApp->m_NPCViewMap.Lookup((WORD)m_pParty->m_dwPartyRoster[i], pNPCDlg);

				if(pNPCDlg != NULL && pNPCDlg->m_pNPC != NULL)
				{
					if(m_dwSubPartyID && pNPCDlg->m_pNPC->m_dwSubPartyID != m_dwSubPartyID)
						continue;

					if(m_pInitiativeDialog != NULL)
					{
						//initiative sorting
						pNPCDlg->m_nInitiativeSegment = pNPCDlg->m_nInitiativeRoll - pNPCDlg->m_nDexAdjToInitiative;

						if(pNPCDlg->m_nInitiativeSegment != nSegment)
							continue;
					}

					pChar = pNPCDlg->m_pNPC;
				}
			}

			if(pChar != NULL)
			{
				int nCol = 1;
				
				int nAttackMatrix[21];
				GetToHitChart(pChar, nAttackMatrix);

				szTemp.Format("%s", pChar->m_szCharacterName);

				int nLength = szTemp.GetLength();
				if(nLength < 15) 
				{
					CString szSpaces = _T("                                        ");
					szTemp += szSpaces.Left(15-nLength);
				}

				m_cPartyList.InsertItem(nRow, szTemp);

				if(IsMissileWeapon(&pChar->m_SelectedWeapons[0]))
					szTemp.Format("%d", pChar->m_nUnshieldedArmorClass);
				else
					szTemp.Format("%d", pChar->m_nCurrentArmorClass);

				m_cPartyList.SetItemText(nRow, nCol++, szTemp);

				szTemp.Format("%d", pChar->m_nHitPoints - pChar->m_nCurrentDamage);
				m_cPartyList.SetItemText(nRow, nCol++,szTemp);

				CString szPlus = _T("");
				if(pChar->m_SelectedWeapons[0].m_nMagicAdj)
				{
					if(pChar->m_SelectedWeapons[0].m_nMagicAdj >  0)
					{
						szPlus.Format("+%d", pChar->m_SelectedWeapons[0].m_nMagicAdj);
					}
					else
					{
						szPlus.Format("%d", pChar->m_SelectedWeapons[0].m_nMagicAdj);
					}
				}

				szTemp.Format("%s %s", pChar->m_SelectedWeapons[0].m_szType, szPlus);
				m_cPartyList.SetItemText(nRow, nCol++, szTemp);

				//weapons vs AC
				int nWvACColumn = -1;
				if(m_pApp->m_Settings.m_bUseWeapons_vs_AC_Chart)
				{
					if(m_pOpposingCharacterDialog != NULL)
					{
						nWvACType = m_pOpposingCharacterDialog->m_pCharacter->m_ArmorWorn.m_nArmorClass;
						if(m_pOpposingCharacterDialog->m_pCharacter->m_ShieldWorn.m_wTypeId)
							nWvACType -=1;

						nWvACColumn = 10 - m_pOpposingCharacterDialog->m_pCharacter->m_nCurrentArmorClass;
					}

					else if(m_pOpposingNPCDialog != NULL && m_pOpposingNPCDialog->m_pNPC->m_ArmorWorn.m_wTypeId)
					{
						nWvACType = m_pOpposingNPCDialog->m_pNPC->m_ArmorWorn.m_nArmorClass;
						if(m_pOpposingNPCDialog->m_pNPC->m_ShieldWorn.m_wTypeId)
							nWvACType -=1;

						nWvACColumn = 10 - m_pOpposingNPCDialog->m_pNPC->m_nCurrentArmorClass;
					}

					if(nWvACType > -1)
					{
						if(m_pSelectedCharacterDialog != NULL && m_pSelectedCharacterDialog == pCharDlg)
						{
							nWvACAdj = m_pSelectedCharacterDialog->m_pCharacter->m_SelectedWeapons[0].m_nWeaponVSAC[nWvACType];
						}
						else if(m_pSelectedNPCDialog != NULL && m_pSelectedNPCDialog == pNPCDlg)
						{
							nWvACAdj = m_pSelectedNPCDialog->m_pNPC->m_SelectedWeapons[0].m_nWeaponVSAC[nWvACType];
						}
					}
				}

				if(pCharDlg != NULL)
				{
					m_cPartyList.SetItemText(nRow, nCol++, pCharDlg->m_szNumAttacks);
					
					///////////////////
					if (pCharDlg == m_pSelectedCharacterDialog && m_pOpposingCharacterDialog != NULL && m_pOpposingCharacterDialog == pCharDlg->m_pTargetBaseDlg)
					{
						int nToHitMatrix = 10 - m_pOpposingCharacterDialog->m_pCharacter->m_nCurrentArmorClass;
						if (nToHitMatrix >= 0 && nToHitMatrix < 21)
						{
							pCharDlg->m_szToHitDesc.Format("%d", pCharDlg->m_nAttackMatrix[nToHitMatrix] - nWvACAdj);
						}
						else
						{
							pCharDlg->m_szToHitDesc = _T("*");
						}
					}
					if (pCharDlg == m_pSelectedCharacterDialog && m_pOpposingNPCDialog != NULL && m_pOpposingNPCDialog == pCharDlg->m_pTargetBaseDlg)
					{
						int nToHitMatrix = 10 - m_pOpposingNPCDialog->m_pNPC->m_nCurrentArmorClass;
						if (nToHitMatrix >= 0 && nToHitMatrix < 21)
						{
							pCharDlg->m_szToHitDesc.Format("%d", pCharDlg->m_nAttackMatrix[nToHitMatrix] - nWvACAdj);
						}
						else
						{
							pCharDlg->m_szToHitDesc = _T("*");
						}
					}
					///////////////////////////

					for(int i = 0; i < 21; ++i)
					{
						if(i == nWvACColumn)
						{
							#if 0
							if(nWvACAdj < 0)
							{
								szTemp.Format("%d+%d", pCharDlg->m_nAttackMatrix[i], -nWvACAdj);
							}
							else
							{
								szTemp.Format("%d-%d", pCharDlg->m_nAttackMatrix[i], nWvACAdj);
							}
							#else
							szTemp.Format("%d", pCharDlg->m_nAttackMatrix[i]-nWvACAdj);
							#endif
						}
						else
						{
							szTemp.Format("%d", pCharDlg->m_nAttackMatrix[i]);
						}
						m_cPartyList.SetItemText(nRow, nCol++,szTemp);
					}
					
					m_cPartyList.SetItemText(nRow, nCol++,pCharDlg->m_szDamageDesc);
				}

				if(pNPCDlg != NULL)
				{
					pNPCDlg->ProcessCharStats();

					m_cPartyList.SetItemText(nRow, nCol++, pNPCDlg->m_szNumAttacks);

					///////////////////
					if (pNPCDlg == m_pSelectedNPCDialog && m_pOpposingCharacterDialog != NULL && m_pOpposingCharacterDialog == pNPCDlg->m_pTargetBaseDlg)
					{
						int nToHitMatrix = 10 - m_pOpposingCharacterDialog->m_pCharacter->m_nCurrentArmorClass;
						if (nToHitMatrix >= 0 && nToHitMatrix < 21)
						{
							pNPCDlg->m_szToHitDesc.Format("%d", pNPCDlg->m_nAttackMatrix[nToHitMatrix] - nWvACAdj);
						}
						else
						{
							pNPCDlg->m_szToHitDesc = _T("*");
						}
					}
					if (pNPCDlg == m_pSelectedNPCDialog && m_pOpposingNPCDialog != NULL && m_pOpposingNPCDialog == pNPCDlg->m_pTargetBaseDlg)
					{
						int nToHitMatrix = 10 - m_pOpposingNPCDialog->m_pNPC->m_nCurrentArmorClass;
						if (nToHitMatrix >= 0 && nToHitMatrix < 21)
						{
							pNPCDlg->m_szToHitDesc.Format("%d", pNPCDlg->m_nAttackMatrix[nToHitMatrix] - nWvACAdj);
						}
						else
						{
							pNPCDlg->m_szToHitDesc = _T("*");
						}
					}
					///////////////////////////

					for(int i = 0; i < 21; ++i)
					{
						if(i == nWvACColumn)
						{
							szTemp.Format("%d", pNPCDlg->m_nAttackMatrix[i]-nWvACAdj);
						}
						else
						{
							szTemp.Format("%d", pNPCDlg->m_nAttackMatrix[i]);
						}
						m_cPartyList.SetItemText(nRow, nCol++,szTemp);
					}
					
					m_cPartyList.SetItemText(nRow, nCol++,pNPCDlg->m_szDamageDesc);
				}

				szTemp.Format("%d", pChar->m_SelectedAmmo.m_lAmount);
				m_cPartyList.SetItemText(nRow, nCol++, szTemp);

				if(pCharDlg != NULL)
				{
					if(pCharDlg->m_nCharacterMovementRate < m_pParty->m_nPartyMovementRate)
					{
						m_pParty->m_nPartyMovementRate = pCharDlg->m_nCharacterMovementRate;
					}
					szTemp.Format("%d", pCharDlg->m_nCharacterMovementRate);
					m_cPartyList.SetItemText(nRow, nCol++, szTemp);

					if(nWvACColumn >= 0 && pCharDlg == m_pSelectedCharacterDialog)
					{
						szTemp.Format("%s", szPlusOrMinus(nWvACAdj));
						m_cPartyList.m_nSubSubSelectedColumn = nCol;
						m_cPartyList.SetItemText(nRow, nCol++, szTemp);
					}

					m_cPartyList.SetItemData(nRow, (ULONG)pCharDlg); // LAST THING !
				}
				if(pNPCDlg != NULL)
				{
					if(pNPCDlg->m_nCharacterMovementRate < m_pParty->m_nPartyMovementRate)
					{
						m_pParty->m_nPartyMovementRate = pNPCDlg->m_nCharacterMovementRate;
					}
					szTemp.Format("%d", pNPCDlg->m_nCharacterMovementRate);
					m_cPartyList.SetItemText(nRow, nCol++, szTemp);

					if(nWvACColumn >= 0 && pNPCDlg == m_pSelectedNPCDialog)
					{
						szTemp.Format("%s", szPlusOrMinus(nWvACAdj));
						m_cPartyList.m_nSubSubSelectedColumn = nCol;
						m_cPartyList.SetItemText(nRow, nCol++, szTemp);
					}

					m_cPartyList.SetItemData(nRow, (ULONG)pNPCDlg);  // LAST THING !
				}

				++nRow;
			}

		} // for(int i = 0; i < MAX_PARTY_MEMBERS; ++i)	
	}

	SetSelectedListCtrlItem(&m_cPartyList, nCursor);

	BOOL bEnableCharCmds = FALSE;
	BOOL bEnableMissileWeapon = FALSE;
	BOOL bEnableMeleeWeapon = FALSE;
	BOOL bIsSpellCaster = FALSE;
	//crash here next line
	if(m_pSelectedCharacterDialog != NULL && m_pSelectedCharacterDialog->m_pCharacter != NULL)
	{
		bEnableCharCmds = TRUE;

		if(m_pSelectedCharacterDialog->m_pCharacter->m_nCurrentDamage < m_pSelectedCharacterDialog->m_pCharacter->m_nHitPoints)
		{
			bIsSpellCaster = GetSpellClasses(m_pSelectedCharacterDialog->m_pCharacter);

			if(IsThrowableWeapon(m_pSelectedCharacterDialog->m_pCharacter->m_SelectedWeapons[0].m_nFlags))
			{
				bEnableMissileWeapon = TRUE;
				bEnableMeleeWeapon = TRUE;
			}
			else if(IsMissileWeapon(&m_pSelectedCharacterDialog->m_pCharacter->m_SelectedWeapons[0]) && m_pSelectedCharacterDialog->m_pCharacter->m_SelectedAmmo.m_lAmount > 0)
			{
				bEnableMissileWeapon = TRUE;
				bEnableMeleeWeapon = FALSE;
			}
			else
			{
				bEnableMeleeWeapon = TRUE;
			}
			
		}
	}

	if(m_pSelectedNPCDialog != NULL && m_pSelectedNPCDialog->m_pNPC != NULL)
	{
		bEnableCharCmds = TRUE;

		if(m_pSelectedNPCDialog->m_pNPC->m_nCurrentDamage < m_pSelectedNPCDialog->m_pNPC->m_nHitPoints)
		{
			bIsSpellCaster = GetSpellClasses(m_pSelectedNPCDialog->m_pNPC);

			if(IsThrowableWeapon(m_pSelectedNPCDialog->m_pNPC->m_SelectedWeapons[0].m_nFlags))
			{
				bEnableMissileWeapon = TRUE;
				bEnableMeleeWeapon = TRUE;
			}
			else if(IsMissileWeapon(&m_pSelectedNPCDialog->m_pNPC->m_SelectedWeapons[0]) && m_pSelectedNPCDialog->m_pNPC->m_SelectedAmmo.m_lAmount > 0)
			{
				bEnableMissileWeapon = TRUE;
				bEnableMeleeWeapon = FALSE;
			}
			else
			{
				bEnableMeleeWeapon = TRUE;
			}
			
		}
	}

	m_cHealButton.EnableWindow(bEnableCharCmds);
	m_cWoundButton.EnableWindow(bEnableCharCmds);
	m_cSwapWeaponButton.EnableWindow(bEnableCharCmds);
	m_cAttackMissButton.EnableWindow(bEnableMeleeWeapon);
	m_cAttackHitButton.EnableWindow(bEnableMeleeWeapon);
	m_cMissileMissButton.EnableWindow(bEnableMissileWeapon);
	m_cMissileHitButton.EnableWindow(bEnableMissileWeapon);
	m_cCastSpellButton.EnableWindow(bIsSpellCaster);


	int nWvACType2 = -1;
	if(m_pOpposingPartyDialog != NULL)
	{
		nCursor = GetSelectedListCtrlItem(&m_cPartyList2);
		m_cPartyList2.m_nSelectedRow = nCursor;

		int nRow = 0;
		BOOL bRemove = m_cPartyList2.DeleteAllItems();

		m_cPartyList2.m_nSubSelectedColumn = -1;
		nWvACAdj = 0;

		for(int nSegment = nStartSegment; nSegment <= nEndSegment; ++nSegment)
		{
			for(int i = 0; i < MAX_PARTY_MEMBERS; ++i)
			{
				if(m_pOpposingPartyDialog->m_pParty->m_dwPartyRoster[i] == 0)
					break;

				PDNDCHARACTER pChar = NULL;

				PDNDCHARVIEWDLG pCharDlg = NULL;
				PDNDNPCVIEWDLG pNPCDlg = NULL;

				m_pApp->m_CharacterViewMap.Lookup((WORD)m_pOpposingPartyDialog->m_pParty->m_dwPartyRoster[i], pCharDlg);

				if(pCharDlg != NULL && pCharDlg->m_pCharacter != NULL)
				{
					if(m_pOpposingSubParty != NULL && m_pOpposingSubParty->m_dwSubPartyID != pCharDlg->m_pCharacter->m_dwSubPartyID)
					{
						continue;
					}

					if(m_pInitiativeDialog != NULL)
					{
						//initiative sorting
						pCharDlg->m_nInitiativeSegment = pCharDlg->m_nInitiativeRoll - pCharDlg->m_nDexAdjToInitiative;

						if(pCharDlg->m_nInitiativeSegment != nSegment)
							continue;
					}

					pChar = pCharDlg->m_pCharacter;
				}

				if(pChar == NULL)
				{
					m_pApp->m_NPCViewMap.Lookup((WORD)m_pOpposingPartyDialog->m_pParty->m_dwPartyRoster[i], pNPCDlg);

					if(pNPCDlg != NULL && pNPCDlg->m_pNPC != NULL)
					{
						if(m_pOpposingSubParty != NULL && m_pOpposingSubParty->m_dwSubPartyID != pNPCDlg->m_pNPC->m_dwSubPartyID)
						{
							continue;
						}

						if(m_pInitiativeDialog != NULL)
						{
							//initiative sorting
							pNPCDlg->m_nInitiativeSegment = pNPCDlg->m_nInitiativeRoll - pNPCDlg->m_nDexAdjToInitiative;

							if(pNPCDlg->m_nInitiativeSegment != nSegment)
								continue;
						}

						pChar = pNPCDlg->m_pNPC;
					}
				}

				if(pChar != NULL)
				{
					int nCol = 1;

					int nAttackMatrix[21];
					GetToHitChart(pChar, nAttackMatrix);

					szTemp.Format("%s", pChar->m_szCharacterName);

					int nLength = szTemp.GetLength();
					if(nLength < 15) 
					{
						CString szSpaces = _T("                                        ");
						szTemp += szSpaces.Left(15-nLength);
					}

					m_cPartyList2.InsertItem(nRow, szTemp);

					if(IsMissileWeapon(&pChar->m_SelectedWeapons[0]))
						szTemp.Format("%d", pChar->m_nUnshieldedArmorClass);
					else
						szTemp.Format("%d", pChar->m_nCurrentArmorClass);

					m_cPartyList2.SetItemText(nRow, nCol++, szTemp);

					szTemp.Format("%d", pChar->m_nHitPoints - pChar->m_nCurrentDamage);
					m_cPartyList2.SetItemText(nRow, nCol++,szTemp);

					CString szPlus = _T("");
					if(pChar->m_SelectedWeapons[0].m_nMagicAdj)
					{
						if(pChar->m_SelectedWeapons[0].m_nMagicAdj >  0)
						{
							szPlus.Format("+%d", pChar->m_SelectedWeapons[0].m_nMagicAdj);
						}
						else
						{
							szPlus.Format("%d", pChar->m_SelectedWeapons[0].m_nMagicAdj);
						}
					}

					szTemp.Format("%s %s", pChar->m_SelectedWeapons[0].m_szType, szPlus);
					m_cPartyList2.SetItemText(nRow, nCol++, szTemp);

					//weapons vs AC
					int nWvACColumn = -1;
					if(m_pApp->m_Settings.m_bUseWeapons_vs_AC_Chart)
					{
						if(m_pSelectedCharacterDialog != NULL)
						{
							nWvACType2 = m_pSelectedCharacterDialog->m_pCharacter->m_ArmorWorn.m_nArmorClass;
							if(m_pSelectedCharacterDialog->m_pCharacter->m_ShieldWorn.m_wTypeId)
								nWvACType2 -=1;

							nWvACColumn = 10 - m_pSelectedCharacterDialog->m_pCharacter->m_nCurrentArmorClass;
						}
						else if(m_pSelectedNPCDialog != NULL && m_pSelectedNPCDialog->m_pNPC->m_ArmorWorn.m_wTypeId)
						{
							nWvACType2 = m_pSelectedNPCDialog->m_pNPC->m_ArmorWorn.m_nArmorClass;
							if(m_pSelectedNPCDialog->m_pNPC->m_ShieldWorn.m_wTypeId)
								nWvACType2 -=1;

							nWvACColumn = 10 - m_pSelectedNPCDialog->m_pNPC->m_nCurrentArmorClass;
						}

						if(nWvACType2 > -1)
						{
							if(m_pOpposingCharacterDialog != NULL && m_pOpposingCharacterDialog == pCharDlg)
							{
								nWvACAdj = m_pOpposingCharacterDialog->m_pCharacter->m_SelectedWeapons[0].m_nWeaponVSAC[nWvACType2];
							}
							else if(m_pOpposingNPCDialog != NULL && m_pOpposingNPCDialog == pNPCDlg)
							{
								nWvACAdj = m_pOpposingNPCDialog->m_pNPC->m_SelectedWeapons[0].m_nWeaponVSAC[nWvACType2];
							}
						}
					}


					if(pCharDlg != NULL)
					{
						m_cPartyList2.SetItemText(nRow, nCol++, pCharDlg->m_szNumAttacks);
						
						///////////////////
						if (pCharDlg == m_pOpposingCharacterDialog && m_pSelectedCharacterDialog != NULL && m_pSelectedCharacterDialog == pCharDlg->m_pTargetBaseDlg)
						{
							int nToHitMatrix = 10 - m_pSelectedCharacterDialog->m_pCharacter->m_nCurrentArmorClass;
							if (nToHitMatrix >= 0 && nToHitMatrix < 21)
							{
								pCharDlg->m_szToHitDesc.Format("%d", pCharDlg->m_nAttackMatrix[nToHitMatrix] - nWvACAdj);
							}
							else
							{
								pCharDlg->m_szToHitDesc = _T("*");
							}
						}
						if (pCharDlg == m_pOpposingCharacterDialog && m_pSelectedNPCDialog != NULL && m_pSelectedNPCDialog == pCharDlg->m_pTargetBaseDlg)
						{
							int nToHitMatrix = 10 - m_pSelectedNPCDialog->m_pNPC->m_nCurrentArmorClass;
							if (nToHitMatrix >= 0 && nToHitMatrix < 21)
							{
								pCharDlg->m_szToHitDesc.Format("%d", pCharDlg->m_nAttackMatrix[nToHitMatrix] - nWvACAdj);
							}
							else
							{
								pCharDlg->m_szToHitDesc = _T("*");
							}
						}
						///////////////////////////

						for(int i = 0; i < 21; ++i)
						{
							if(i == nWvACColumn)
							{
								szTemp.Format("%d", pCharDlg->m_nAttackMatrix[i]-nWvACAdj);
							}
							else
							{
								szTemp.Format("%d", pCharDlg->m_nAttackMatrix[i]);
							}
							m_cPartyList2.SetItemText(nRow, nCol++,szTemp);
						}
						
						m_cPartyList2.SetItemText(nRow, nCol++,pCharDlg->m_szDamageDesc);

						m_cPartyList2.SetItemData(nRow, (ULONG)pCharDlg);
					}

					if(pNPCDlg != NULL)
					{
						m_cPartyList2.SetItemText(nRow, nCol++, pNPCDlg->m_szNumAttacks);

						///////////////////
						if (pNPCDlg == m_pOpposingNPCDialog && m_pSelectedCharacterDialog != NULL && m_pSelectedCharacterDialog == pNPCDlg->m_pTargetBaseDlg)
						{
							int nToHitMatrix = 10 - m_pSelectedCharacterDialog->m_pCharacter->m_nCurrentArmorClass;
							if (nToHitMatrix >= 0 && nToHitMatrix < 21)
							{
								pNPCDlg->m_szToHitDesc.Format("%d", pNPCDlg->m_nAttackMatrix[nToHitMatrix] - nWvACAdj);
							}
							else
							{
								pNPCDlg->m_szToHitDesc = _T("*");
							}
						}
						if (pNPCDlg == m_pOpposingNPCDialog && m_pSelectedNPCDialog != NULL && m_pSelectedNPCDialog == pNPCDlg->m_pTargetBaseDlg)
						{
							int nToHitMatrix = 10 - m_pSelectedNPCDialog->m_pNPC->m_nCurrentArmorClass;
							if (nToHitMatrix >= 0 && nToHitMatrix < 21)
							{
								pNPCDlg->m_szToHitDesc.Format("%d", pNPCDlg->m_nAttackMatrix[nToHitMatrix] - nWvACAdj);
							}
							else
							{
								pNPCDlg->m_szToHitDesc = _T("*");
							}
						}
						///////////////////////////

						for(int i = 0; i < 21; ++i)
						{
							if(i == nWvACColumn)
							{
								szTemp.Format("%d", pNPCDlg->m_nAttackMatrix[i]-nWvACAdj);
							}
							else
							{
								szTemp.Format("%d", pNPCDlg->m_nAttackMatrix[i]);
							}
							m_cPartyList2.SetItemText(nRow, nCol++,szTemp);
						}
						
						m_cPartyList2.SetItemText(nRow, nCol++,pNPCDlg->m_szDamageDesc);

						m_cPartyList2.SetItemData(nRow, (ULONG)pNPCDlg);
					}

					szTemp.Format("%d", pChar->m_SelectedAmmo.m_lAmount);
					m_cPartyList2.SetItemText(nRow, nCol++, szTemp);

					if(pCharDlg != NULL)
					{
						szTemp.Format("%d", pCharDlg->m_nCharacterMovementRate);
						m_cPartyList2.SetItemText(nRow, nCol++, szTemp);

						if(nWvACColumn >= 0 && pCharDlg == m_pOpposingCharacterDialog)
						{
							szTemp.Format("%s", szPlusOrMinus(nWvACAdj));
							m_cPartyList2.m_nSubSubSelectedColumn = nCol;
							m_cPartyList2.SetItemText(nRow, nCol++, szTemp);
						}
					}
					if(pNPCDlg != NULL)
					{ 
						szTemp.Format("%d", pNPCDlg->m_nCharacterMovementRate);
						m_cPartyList2.SetItemText(nRow, nCol++, szTemp);

						if(nWvACColumn >= 0 && pNPCDlg == m_pOpposingNPCDialog)
						{
							szTemp.Format("%s", szPlusOrMinus(nWvACAdj));
							m_cPartyList2.m_nSubSubSelectedColumn = nCol;
							m_cPartyList2.SetItemText(nRow, nCol++, szTemp);
						}
					}

					++nRow;
				}

			} // for(int i = 0; i < MAX_PARTY_MEMBERS; ++i)
		}

		SetSelectedListCtrlItem(&m_cPartyList2, nCursor);

		BOOL bEnableCharCmds = FALSE;
		BOOL bEnableMissileWeapon = FALSE;
		BOOL bEnableMeleeWeapon = FALSE;
		BOOL bIsSpellCaster = FALSE;
		if(m_pOpposingCharacterDialog != NULL && m_pOpposingCharacterDialog->m_pCharacter != NULL)
		{
			bEnableCharCmds = TRUE;

			if(m_pOpposingCharacterDialog->m_pCharacter->m_nCurrentDamage < m_pOpposingCharacterDialog->m_pCharacter->m_nHitPoints)
			{
				bIsSpellCaster = GetSpellClasses(m_pOpposingCharacterDialog->m_pCharacter);

				if(IsThrowableWeapon(m_pOpposingCharacterDialog->m_pCharacter->m_SelectedWeapons[0].m_nFlags))
				{
					bEnableMissileWeapon = TRUE;
					bEnableMeleeWeapon = TRUE;
				}
				else if(IsMissileWeapon(&m_pOpposingCharacterDialog->m_pCharacter->m_SelectedWeapons[0]) && m_pOpposingCharacterDialog->m_pCharacter->m_SelectedAmmo.m_lAmount > 0)
				{
					bEnableMissileWeapon = TRUE;
					bEnableMeleeWeapon = FALSE;
				}
				else
				{
					bEnableMeleeWeapon = TRUE;
				}
			}
		}

		if(m_pOpposingNPCDialog != NULL && m_pOpposingNPCDialog->m_pNPC != NULL)
		{
			bEnableCharCmds = TRUE;

			if(m_pOpposingNPCDialog->m_pNPC->m_nCurrentDamage < m_pOpposingNPCDialog->m_pNPC->m_nHitPoints)
			{
				bIsSpellCaster = GetSpellClasses(m_pOpposingNPCDialog->m_pNPC);

				if(IsThrowableWeapon(m_pOpposingNPCDialog->m_pNPC->m_SelectedWeapons[0].m_nFlags))
				{
					bEnableMissileWeapon = TRUE;
					bEnableMeleeWeapon = TRUE;
				}
				else if(IsMissileWeapon(&m_pOpposingNPCDialog->m_pNPC->m_SelectedWeapons[0]) && m_pOpposingNPCDialog->m_pNPC->m_SelectedAmmo.m_lAmount > 0)
				{
					bEnableMissileWeapon = TRUE;
					bEnableMeleeWeapon = FALSE;
				}
				else
				{
					bEnableMeleeWeapon = TRUE;
				}
			}
		}


		m_cHealButton2.EnableWindow(bEnableCharCmds);
		m_cWoundButton2.EnableWindow(bEnableCharCmds);
		m_cSwapWeaponButton2.EnableWindow(bEnableCharCmds);
		m_cAttackMissButton2.EnableWindow(bEnableMeleeWeapon);
		m_cAttackHitButton2.EnableWindow(bEnableMeleeWeapon);
		m_cMissileMissButton2.EnableWindow(bEnableMissileWeapon);
		m_cMissileHitButton2.EnableWindow(bEnableMissileWeapon);
		m_cCastSpellButton2.EnableWindow(bIsSpellCaster);

	}
	else
	{
		m_cHealButton2.EnableWindow(FALSE);
		m_cWoundButton2.EnableWindow(FALSE);
		m_cSwapWeaponButton2.EnableWindow(FALSE);
		m_cAttackMissButton2.EnableWindow(FALSE);
		m_cAttackHitButton2.EnableWindow(FALSE);
		m_cMissileMissButton2.EnableWindow(FALSE);
		m_cMissileHitButton2.EnableWindow(FALSE);
		m_cCastSpellButton2.EnableWindow(FALSE);
	}

	if(m_pSelectedCharacterDialog != NULL && m_pSelectedCharacterDialog->m_pCharacter != NULL && m_pOpposingCharacterDialog != NULL && m_pOpposingCharacterDialog->m_pCharacter != NULL)
	{
		int nAC = 10 - m_pOpposingCharacterDialog->m_pCharacter->m_nCurrentArmorClass + 5;
		m_cPartyList.m_nSelectedColumn = nAC;

		nAC = 10 - m_pSelectedCharacterDialog->m_pCharacter->m_nCurrentArmorClass + 5;
		m_cPartyList2.m_nSelectedColumn = nAC;
	}

	if(m_pSelectedCharacterDialog != NULL && m_pSelectedCharacterDialog->m_pCharacter != NULL && m_pOpposingNPCDialog != NULL && m_pOpposingNPCDialog->m_pNPC != NULL)
	{
		int nAC = 10 - m_pOpposingNPCDialog->m_pNPC->m_nCurrentArmorClass + 5;
		m_cPartyList.m_nSelectedColumn = nAC;

		nAC = 10 - m_pSelectedCharacterDialog->m_pCharacter->m_nCurrentArmorClass + 5;
		m_cPartyList2.m_nSelectedColumn = nAC;
	}

	if(m_pSelectedNPCDialog != NULL && m_pSelectedNPCDialog->m_pNPC != NULL && m_pOpposingCharacterDialog != NULL && m_pOpposingCharacterDialog->m_pCharacter != NULL)
	{
		int nAC = 10 - m_pOpposingCharacterDialog->m_pCharacter->m_nCurrentArmorClass + 5;
		m_cPartyList.m_nSelectedColumn = nAC;

		nAC = 10 - m_pSelectedNPCDialog->m_pNPC->m_nCurrentArmorClass + 5;
		m_cPartyList2.m_nSelectedColumn = nAC;
	}

	if(m_pSelectedNPCDialog != NULL && m_pSelectedNPCDialog->m_pNPC != NULL && m_pOpposingNPCDialog != NULL && m_pOpposingNPCDialog->m_pNPC != NULL)
	{
		int nAC = 10 - m_pOpposingNPCDialog->m_pNPC->m_nCurrentArmorClass + 5;
		m_cPartyList.m_nSelectedColumn = nAC;

		nAC = 10 - m_pSelectedNPCDialog->m_pNPC->m_nCurrentArmorClass + 5;
		m_cPartyList2.m_nSelectedColumn = nAC;
	}

	if(nWvACType != -1)
	{
		m_cPartyList.m_nSubSelectedColumn = 10-nWvACType+5; 
	}
	if(nWvACType2 != -1)
	{
		m_cPartyList2.m_nSubSelectedColumn = 10-nWvACType2+5; 
	}

}


void DMPartyDialog::Initialize()
{
	SetSelectedListCtrlItem(&m_cPartyList, -1);
	SetSelectedListCtrlItem(&m_cPartyList2, -1);

	m_pSelectedCharacterDialog = NULL;
	m_pSelectedNPCDialog = NULL;
	m_pBaseOpposingCharDlg = NULL;
	m_pOpposingCharacterDialog = NULL;
	m_pOpposingNPCDialog = NULL;

	if (m_pSelectedCharacterBitmap != NULL)
	{
		::delete m_pSelectedCharacterBitmap;
		m_pSelectedCharacterBitmap = NULL;
	}

	if (m_pSelectedOpponentBitmap != NULL)
	{
		::delete m_pSelectedOpponentBitmap;
		m_pSelectedOpponentBitmap = NULL;
	}
		
	if(m_pSubParty == NULL)
	{
		m_szPartyNameStatic = _T("Party Name:");
		m_szPartyName = m_pParty->m_szPartyName;
		m_cSubPartyButton.SetWindowText("Create SubParty");
	}
	else
	{
		m_szPartyNameStatic = _T("SubParty Name:");
		m_szPartyName = m_pSubParty->m_szSubPartyName;
		m_cSubPartyButton.SetWindowText("Delete SubParty");
	}

	for(int i = 0; i < m_cCalendarCombo.GetCount(); ++i)
	{
		PCALENDAR pCalendar = (PCALENDAR)m_cCalendarCombo.GetItemData(i);

		if(pCalendar != NULL)
		{
			if(m_pParty->m_dwCalendarID == pCalendar->m_dwCalendarID)
			{
				m_cCalendarCombo.SetCurSel(i);

				m_pApp->m_pSelectedCalendar = pCalendar;

				m_pParty->ModifyCalendar(DND_CALENDAR_MOD_DAY, DND_CALENDAR_MOD_INIT);

				break;
			}
		}

	}


	m_cPartyList2.DeleteAllItems();

	Refresh();
}

void DMPartyDialog::RefreshTime()
{
	if(m_pApp->m_pSelectedCalendar == NULL)
	{
		return;
	}

	if(m_pParty->m_nYear == 0)
	{
		m_pParty->m_nYear = m_pApp->m_pSelectedCalendar->m_nBaseYear;
	}

	/*
	m_pParty->m_nDayofWeek = 0;
	m_pParty->m_nDayofMonth = 0;
	m_pParty->m_nMonth = 0;
	m_pParty->m_nYear = 0;
	*/

	int nMinutes = m_pParty->m_nTurn * 10 + m_pParty->m_nRound;
	int nSeconds = m_pParty->m_nSegment * 6 + m_pApp->m_nCalendarSecond;

	m_szPartyLegend.Format("%02d:%02d:%02d %s, the %d%s day of %s in the year %d", m_pParty->m_nHour, nMinutes, nSeconds, 
		m_pApp->m_pSelectedCalendar->m_szDayNames[m_pParty->m_nDayofWeek], 
		m_pParty->m_nDayofMonth+1, 
		GetNumberSuffix(m_pParty->m_nDayofMonth+1),
		m_pApp->m_pSelectedCalendar->m_szMonthNames[m_pParty->m_nMonth], 
		m_pParty->m_nYear);


	m_szRoundEdit.Format("%d", m_pParty->m_nRound);
	m_szSegmentEdit.Format("%d", m_pParty->m_nSegment);

}

void DMPartyDialog::SetPartyPortraits()
{
	if (m_pSelectedCharacterBitmap != NULL)
	{
		::delete m_pSelectedCharacterBitmap;
		m_pSelectedCharacterBitmap = NULL;
	}

	if (m_pSelectedCharacterDialog != NULL)
	{
		CString szTemp = m_pSelectedCharacterDialog->m_pCharacter->m_szPortraitPath;

		if (szTemp != _T(""))
		{
			szTemp.MakeUpper();
			szTemp.Replace("<$DMAPATH>", m_pApp->m_szEXEPath);

			LPWSTR wcsFile = szTemp.AllocSysString();
			m_pSelectedCharacterBitmap = ::new Bitmap(wcsFile, FALSE);

			int nHeight = m_pSelectedCharacterBitmap->GetHeight();

			if (nHeight == 0)
			{
				::delete m_pSelectedCharacterBitmap;
				m_pSelectedCharacterBitmap = NULL;
			}
		}
	}

	if (m_pSelectedNPCDialog != NULL && m_pSelectedNPCDialog->m_szMonsterPortraitPath != _T(""))
	{
		LPWSTR wcsFile = m_pSelectedNPCDialog->m_szMonsterPortraitPath.AllocSysString();
		m_pSelectedCharacterBitmap = ::new Bitmap(wcsFile, FALSE);

		int nHeight = m_pSelectedCharacterBitmap->GetHeight();

		if (nHeight == 0)
		{
			::delete m_pSelectedCharacterBitmap;
			m_pSelectedCharacterBitmap = NULL;
		}
	}
}

void DMPartyDialog::SetOpposingPartyPortraits()
{
	if (m_pSelectedOpponentBitmap != NULL)
	{
		::delete m_pSelectedOpponentBitmap;
		m_pSelectedOpponentBitmap = NULL;
	}

	if (m_pOpposingCharacterDialog != NULL)
	{
		CString szTemp = m_pOpposingCharacterDialog->m_pCharacter->m_szPortraitPath;

		if (szTemp != _T(""))
		{
			szTemp.MakeUpper();
			szTemp.Replace("<$DMAPATH>", m_pApp->m_szEXEPath);

			LPWSTR wcsFile = szTemp.AllocSysString();
			m_pSelectedOpponentBitmap = ::new Bitmap(wcsFile, FALSE);

			int nHeight = m_pSelectedOpponentBitmap->GetHeight();

			if (nHeight == 0)
			{
				::delete m_pSelectedOpponentBitmap;
				m_pSelectedOpponentBitmap = NULL;
			}
		}
	}

	if (m_pOpposingNPCDialog != NULL && m_pOpposingNPCDialog->m_szMonsterPortraitPath != _T(""))
	{
		LPWSTR wcsFile = m_pOpposingNPCDialog->m_szMonsterPortraitPath.AllocSysString();
		m_pSelectedOpponentBitmap = ::new Bitmap(wcsFile, FALSE);

		int nHeight = m_pSelectedOpponentBitmap->GetHeight();

		if (nHeight == 0)
		{
			::delete m_pSelectedOpponentBitmap;
			m_pSelectedOpponentBitmap = NULL;
		}
	}
}

void DMPartyDialog::Refresh()
{
	int nColumnCount = m_cPartyList.GetHeaderCtrl()->GetItemCount();

	if(nColumnCount == 29 && m_pApp->m_Settings.m_bUseWeapons_vs_AC_Chart)
	{
		m_cPartyList.InsertColumn( 29, "WvAC", LVCFMT_LEFT, 50, -1 );
	}
	else if(nColumnCount == 30 && !m_pApp->m_Settings.m_bUseWeapons_vs_AC_Chart)
	{
		m_cPartyList.DeleteColumn(29);
	}

	nColumnCount = m_cPartyList2.GetHeaderCtrl()->GetItemCount();

	if(nColumnCount == 29 && m_pApp->m_Settings.m_bUseWeapons_vs_AC_Chart)
	{
		m_cPartyList2.InsertColumn( 29, "WvAC", LVCFMT_LEFT, 50, -1 );
	}
	else if(nColumnCount == 30 && !m_pApp->m_Settings.m_bUseWeapons_vs_AC_Chart)
	{
		m_cPartyList2.DeleteColumn(29);
	}


	if (m_pOldSelectedCharacterDialog != m_pSelectedCharacterDialog || m_pOldSelectedNPCDialog != m_pSelectedNPCDialog)
	{
		SetPartyPortraits();
	}

	m_pOldSelectedCharacterDialog = m_pSelectedCharacterDialog;

	if(m_pOldSelectedNPCDialog != NULL && m_pOldSelectedNPCDialog != m_pSelectedNPCDialog)
	{
		m_pOldSelectedNPCDialog->CloseSubWindows();
		m_pOldSelectedNPCDialog = NULL;
	}

	if (m_pOldOpposingCharacterDialog != m_pOpposingCharacterDialog || m_pOldOpposingNPCDialog != m_pOpposingNPCDialog)
	{
		SetOpposingPartyPortraits();
	}

	m_pOldOpposingCharacterDialog = m_pOpposingCharacterDialog;

	if(m_pSelectedNPCDialog != NULL)
	{
		m_pSelectedNPCDialog->InitView();
		m_pOldSelectedNPCDialog = m_pSelectedNPCDialog;
	}

	if(m_pOldOpposingNPCDialog != NULL && m_pOldOpposingNPCDialog != m_pOpposingNPCDialog)
	{
		m_pOldOpposingNPCDialog->CloseSubWindows();
		m_pOldOpposingNPCDialog = NULL;
	}

	if(m_pOpposingNPCDialog != NULL)
	{
		m_pOpposingNPCDialog->InitView();
		m_pOpposingNPCDialog = m_pOpposingNPCDialog;
	}

	UpdateSelections();

	if(m_pSubParty == NULL)
	{
		SetWindowText(m_szPartyName);
		strcpy(m_pParty->m_szPartyName, m_szPartyName.Left(127));
	}
	else
	{
		strcpy(m_pSubParty->m_szSubPartyName, m_szPartyName.Left(127));
		if(m_pSubPartyDialog != NULL)
		{
			m_pSubPartyDialog->SetWindowText(m_pSubParty->m_szSubPartyName);
		}
	}

	m_szPartyXP.Format("%ld", m_pParty->m_lXP);

	cDNDPartyLog *pPartyLog = m_pPartyLog;

	DMPartyDialog *pParentPartyDlg = NULL;
	if(m_dwParentPartyID)
	{
		//this is a subparty
		if(m_pApp->m_PartyViewMap.Lookup((WORD)m_dwParentPartyID, pParentPartyDlg))
		{
			if(pParentPartyDlg)
			{
				pPartyLog = pParentPartyDlg->m_pPartyLog;
			}
		}
	}

	if(m_pApp->m_pSelectedCalendar != NULL)
	{
		m_cViewCalendarButton.EnableWindow(TRUE);
		m_cLogButton.EnableWindow(TRUE);

		if(pPartyLog != NULL)
		{
			m_cViewLogButton.EnableWindow(TRUE);
			m_cLogButton.SetWindowText("Log Event");
		}
		else
		{
			m_cLogButton.SetWindowText("Start Log");
		}

		RefreshTime();
	}
	else
	{
		m_cViewCalendarButton.EnableWindow(FALSE);
		m_cViewLogButton.EnableWindow(FALSE);
		m_cLogButton.EnableWindow(FALSE);
	}


	cDNDDataPicker *pPicker = NULL;

	for(int k = 0; k < m_cPartyCombo2.GetCount(); ++k)
	{
		pPicker = (cDNDDataPicker *)m_cPartyCombo2.GetItemData(k);

		if(NULL != pPicker)
		{
			delete pPicker;
		}
	}
	m_cPartyCombo2.ResetContent();

	int nRow = 0;
	int nPartyCount = m_pApp->m_PartyViewMap.GetCount();
	for (POSITION pos = m_pApp->m_PartyViewMap.GetStartPosition(); pos != NULL; )
	{
		WORD wID;
		PDNDPARTYVIEWDLG pPartyDlg = NULL;
		m_pApp->m_PartyViewMap.GetNextAssoc(pos,wID,pPartyDlg);

		if(pPartyDlg != NULL && pPartyDlg->m_pParty != NULL) // && pPartyDlg != this)
		{
			pPicker = new cDNDDataPicker(pPartyDlg->m_pParty->m_szPartyName, pPartyDlg, NULL);
			
			m_cPartyCombo2.InsertString(nRow, pPicker->m_szLabel);
			m_cPartyCombo2.SetItemData(nRow, (ULONG)pPicker);

			if(pPartyDlg == m_pOpposingPartyDialog && m_pOpposingSubParty == NULL)
			{
				m_cPartyCombo2.SetCurSel(nRow);
			}

			++nRow;

			for(int i = 0; i < MAX_SUB_PARTIES; ++i)
			{
				if(pPartyDlg->m_pParty->m_SubParty[i].m_dwSubPartyID)
				{
					cDNDSubParty *pSubParty = &pPartyDlg->m_pParty->m_SubParty[i];

					pPicker = new cDNDDataPicker(pSubParty->m_szSubPartyName, pPartyDlg, pSubParty);
			
					m_cPartyCombo2.InsertString(nRow, pPicker->m_szLabel);
					m_cPartyCombo2.SetItemData(nRow, (ULONG)pPicker);

					if(pPartyDlg == m_pOpposingPartyDialog && m_pOpposingSubParty == pSubParty)
					{
						m_cPartyCombo2.SetCurSel(nRow);
					}

					++nRow;
				}
			}
			
		}
	}

	if(g_bUsed10Initiative)
	{
		m_cViewInitiativeButton.ShowWindow(SW_SHOW);
	}
	else
	{
		m_cViewInitiativeButton.ShowWindow(SW_HIDE);
	}

	if(/* m_pSelectedCharacterDialog == NULL && */ m_pSelectedNPCDialog == NULL)
	{
		m_cCopyMemberButton.EnableWindow(FALSE);
	}
	else
	{
		m_cCopyMemberButton.EnableWindow(TRUE);
	}

	if(m_pApp->m_pBaseCopyCharacterDialog == NULL)
	{
		m_cPasteMemberButton.EnableWindow(FALSE);
	}
	else
	{
		m_cPasteMemberButton.EnableWindow(TRUE);
	}
	
	m_pApp->m_pMainWindow->InvalidateRect(NULL);
	m_pParent->InvalidateRect(NULL);

	if((m_pSelectedCharacterDialog != NULL || m_pSelectedNPCDialog != NULL) && (m_pOpposingCharacterDialog != NULL || m_pOpposingNPCDialog != NULL)) 
	{
		if(m_pOpposingCharacterDialog != NULL && m_pOpposingCharacterDialog->m_pCharacter->IsAlive())
		{
			m_cKillForXPButton.EnableWindow(TRUE);
		}
		else if(m_pOpposingNPCDialog != NULL && m_pOpposingNPCDialog->m_pNPC->IsAlive())
		{
			m_cKillForXPButton.EnableWindow(TRUE);
		}
		else
		{
			m_cKillForXPButton.EnableWindow(FALSE);
		}
	}
	else
	{
		m_cKillForXPButton.EnableWindow(FALSE);
	}


	UpdateData(FALSE);

}

void DMPartyDialog::Broadcast()
{
	if(m_pParty->m_dwPartyID == 0L)
	{
		m_pParty->m_dwPartyID = GetUniqueID();

		m_pApp->m_PartyViewMap.SetAt((WORD)m_pParty->m_dwPartyID, this);
	}

	m_pParty->MarkChanged();

	for (POSITION pos = m_pApp->m_PartyViewMap.GetStartPosition(); pos != NULL; )
	{
		WORD wID;
		PDNDPARTYVIEWDLG pPartyDlg = NULL;
		m_pApp->m_PartyViewMap.GetNextAssoc(pos,wID,pPartyDlg);

		if(pPartyDlg != NULL && pPartyDlg->m_pParty != NULL && pPartyDlg != this)
		{
			pPartyDlg->Refresh();
		}
	}

}

void DMPartyDialog::OnChangePartyNameEdit() 
{
	UpdateData(TRUE);

	Refresh();	

	m_pParty->MarkChanged();
}

void DMPartyDialog::OnEnChangePartyXpEdit()
{
	UpdateData(TRUE);

	m_pParty->m_lXP = atol(m_szPartyXP.GetBuffer(0));

	Refresh();	

	m_pParty->MarkChanged();
}


void DMPartyDialog::OnAddMemberButton() 
{
	DWORD dwReturnedID = 0;
	DMCharacterSelectorDialog *pDlg = new DMCharacterSelectorDialog(&dwReturnedID, m_dwParentPartyID, DND_SELECTOR_CHARACTER);
	pDlg->DoModal();

	if(dwReturnedID)
	{
		AddPartyMember(dwReturnedID, m_dwSubPartyID);

		Broadcast();
	}
}

void DMPartyDialog::AddPartyMember(DWORD dwReturnedID, DWORD dwSubPartyID) 
{
	if(dwReturnedID)
	{
		if(m_pParty->CharacterIsPartyMember(dwReturnedID) && dwSubPartyID == 0)
		{
			return;	// character is already in party, so bail
		}

		BOOL bOKToAdd = TRUE;

		//Is the new party member an existing NPC ?
		WORD wID;
		for (POSITION pos = m_pApp->m_NPCViewMap.GetStartPosition(); pos != NULL; )
		{
			PDNDNPCVIEWDLG pNPCDlg = NULL;

			m_pApp->m_NPCViewMap.GetNextAssoc(pos,wID,pNPCDlg);

			if(pNPCDlg != NULL && pNPCDlg->m_pNPC != NULL && pNPCDlg->m_dwNPCCharacterID == dwReturnedID)
			{
				//Find a slot in the party NPC array for this NPC
				int nFoundSlot = -1;
				for(int i = 0; i < MAX_NPC_PARTY_MEMBERS; ++i)
				{
					if(m_pParty->m_NPCs[i].m_dwCharacterID == 0 || m_pParty->m_NPCs[i].m_dwCharacterID == pNPCDlg->m_pNPC->m_dwCharacterID)
					{
						nFoundSlot = i;
						break;
					}
				}
				if(nFoundSlot >= 0)
				{
					if(dwSubPartyID)
					{
						pNPCDlg->m_pNPC->m_dwSubPartyID = dwSubPartyID;

						memcpy(&m_pParty->m_NPCs[nFoundSlot], pNPCDlg->m_pNPC, sizeof(cDNDNonPlayerCharacter));
						memset(pNPCDlg->m_pNPC, 0, sizeof(cDNDNonPlayerCharacter));

						pNPCDlg->m_pNPC = &m_pParty->m_NPCs[nFoundSlot];
					}
					else
					{
						memcpy(&m_pParty->m_NPCs[nFoundSlot],  pNPCDlg->m_pNPC, sizeof(cDNDNonPlayerCharacter));
						memset(pNPCDlg->m_pNPC, 0, sizeof(cDNDNonPlayerCharacter));

						//delete pNPC here ? NOT IF IT CAME FROM A 

						pNPCDlg->m_pNPC = &m_pParty->m_NPCs[nFoundSlot];
						
						/* this doesn't work anymore after window anchor changes for USER object conservation 
						pNPCDlg->m_pInventoryDialog->m_pCharacter = pNPCDlg->m_pNPC;
						if(pNPCDlg->m_pSpellsDialog != NULL)
						{
							pNPCDlg->m_pSpellsDialog->m_pCharacter = pNPCDlg->m_pNPC;
						}
						*/

						if(pNPCDlg->m_pNPC->m_szCharacterName[0] == 0)
						{
							strcpy(pNPCDlg->m_pNPC->m_szCharacterName, "New NPC");
						}

						m_pApp->m_NPCViewMap.SetAt((WORD)pNPCDlg->m_dwNPCCharacterID, NULL);
						m_pApp->m_NPCViewMap.RemoveKey((WORD)pNPCDlg->m_dwNPCCharacterID);

						m_pApp->ValidateNPCViewMap();

						if (pNPCDlg->m_pNPC->m_dwCharacterID == 0)
						{
							pNPCDlg->m_pNPC->m_dwCharacterID = GetUniqueID();  //this NPC is actually a duplicate, so we don't collide on IDs or delete the NPC from a "module"
							pNPCDlg->m_dwNPCCharacterID = pNPCDlg->m_pNPC->m_dwCharacterID;
						}
						m_pApp->m_NPCViewMap.SetAt((WORD)pNPCDlg->m_dwNPCCharacterID, pNPCDlg);

						pNPCDlg->m_pNPC->m_dwSubPartyID = dwSubPartyID;
					}

					dwReturnedID = pNPCDlg->m_pNPC->m_dwCharacterID;	
				}
				else
				{
					bOKToAdd = FALSE;
				}
			}
		}

		if(bOKToAdd)
		{
			m_pParty->AddPartyMemberID(dwReturnedID);

			PDNDCHARVIEWDLG pCharDlg = NULL;
			m_pApp->m_CharacterViewMap.Lookup((WORD)dwReturnedID, pCharDlg);
			if(pCharDlg != NULL && pCharDlg->m_pCharacter != NULL)
			{
				if(dwSubPartyID)
				{
					pCharDlg->m_pCharacter->m_dwSubPartyID = dwSubPartyID;
					pCharDlg->m_pCharacter->MarkChanged();

				}
				else
				{
					LogPartyEvent(FALSE, APPEND_TO_LOG, DND_LOG_EVENT_TYPE_CHARACTER_JOINED_PARTY, pCharDlg->m_pCharacter->m_szCharacterName, dwReturnedID, 0L, "");
				}
			}

			PDNDNPCVIEWDLG pNPCDlg = NULL;
			if (m_pApp->m_NPCViewMap.Lookup((WORD)dwReturnedID, pNPCDlg))
			{
				if (pNPCDlg != NULL && pNPCDlg->m_pNPC != NULL)
				{
					if (dwSubPartyID)
					{
						pNPCDlg->m_pNPC->m_dwSubPartyID = dwSubPartyID;
					}
					else
					{
						LogPartyEvent(FALSE, APPEND_TO_LOG, DND_LOG_EVENT_TYPE_CHARACTER_JOINED_PARTY, pNPCDlg->m_pNPC->m_szCharacterName, dwReturnedID, 0L, "");
					}
				}
			}
		

			m_pSelectedCharacterDialog = NULL;
			m_pSelectedNPCDialog = NULL;
			m_pBaseOpposingCharDlg = NULL;
			m_pOpposingCharacterDialog = NULL;
			m_pOpposingNPCDialog = NULL;

			CDMHelperDlg *pMainDlg = (CDMHelperDlg *)m_pApp->m_pMainWindow;
			pMainDlg->SortTabs();

			UpdateSelections();
			Refresh();

			SetSelectedListCtrlItem(&m_cPartyList, -1);
			SetSelectedListCtrlItem(&m_cPartyList2, -1);
			InvalidateRect(NULL);

			Broadcast();
		}
	}
	
}

void DMPartyDialog::OnDeleteMemberButton() 
{
	DWORD dwReturnedID = 0;
	BOOL bAlreadyAsked = FALSE;

	m_pApp->m_pBaseCopyCharacterDialog = NULL;  // just in case we deleted the buffer NPC
	m_pApp->m_bCharacterCopyBufferIsNPC = FALSE;

	if(m_pSelectedCharacterDialog != NULL)
	{
		if(m_dwSubPartyID || AfxMessageBox("Remove this character from the party ?", MB_OKCANCEL) == IDOK)
		{
			dwReturnedID = m_pSelectedCharacterDialog->m_pCharacter->m_dwCharacterID;
		}
	}
	else if(m_pSelectedNPCDialog != NULL)
	{
		if(m_dwSubPartyID || AfxMessageBox("NPC will be deleted completely !\nAre you sure ?", MB_YESNO) == IDYES)
		{
			dwReturnedID = m_pSelectedNPCDialog->m_pNPC->m_dwCharacterID;
			bAlreadyAsked = TRUE;
		}
	}
	else
	{
		DMCharacterSelectorDialog *pDlg = new DMCharacterSelectorDialog(&dwReturnedID, m_dwSubPartyID, DND_SELECTOR_CHARACTER);
		pDlg->DoModal();
	}

	if(dwReturnedID)
	{
		for (POSITION pos = m_pApp->m_PartyViewMap.GetStartPosition(); pos != NULL; )
		{
			WORD wID;
			PDNDPARTYVIEWDLG pPartyDlg = NULL;
			m_pApp->m_PartyViewMap.GetNextAssoc(pos,wID,pPartyDlg);

			if(pPartyDlg != NULL && pPartyDlg->m_pParty != NULL)
			{
				if(pPartyDlg->m_pInitiativeDialog != NULL)
				{
					pPartyDlg->m_pInitiativeDialog->PostMessage(WM_CLOSE);
				}
			}
		}

		for(int i = 0; i < MAX_NPC_PARTY_MEMBERS; ++i)
		{
			if(m_pParty->m_NPCs[i].m_dwCharacterID == dwReturnedID)
			{
				if(m_dwSubPartyID)
				{
					m_pParty->m_NPCs[i].m_dwSubPartyID = 0;
					m_pParty->MarkChanged();
					Refresh();
					UpdateSelections();
					return;
				}
				else
				{
					if(bAlreadyAsked == TRUE || AfxMessageBox("NPC will be deleted completely !\nAre you sure ?", MB_YESNO) == IDYES)
					{
						if(FindAndSelectCharacterTab(dwReturnedID))
						{
							CDMHelperDlg *pMainDlg = (CDMHelperDlg *)m_pApp->m_pMainWindow;
							pMainDlg->DeleteTab();
							pMainDlg->SortTabs();
							FindAndSelectPartyTab();
							UpdateSelections();
						}

						memset(&m_pParty->m_NPCs[i], 0, sizeof(cDNDNonPlayerCharacter));
					}
					else
					{
						Refresh();
						return;
					}
				}
			}
		}


		PDNDCHARVIEWDLG pCharDlg = NULL;
		m_pApp->m_CharacterViewMap.Lookup((WORD)dwReturnedID, pCharDlg);

		if(pCharDlg != NULL && pCharDlg->m_pCharacter != NULL)
		{
			if(m_dwSubPartyID)
			{
				pCharDlg->m_pCharacter->m_dwSubPartyID = 0;
				pCharDlg->m_pCharacter->MarkChanged();
			}
			else
			{
				LogPartyEvent(FALSE, APPEND_TO_LOG, DND_LOG_EVENT_TYPE_CHARACTER_LEFT_PARTY, pCharDlg->m_pCharacter->m_szCharacterName, dwReturnedID, 0L, "");

				m_pParty->RemovePartyMemberID(dwReturnedID);

				pCharDlg->m_pCharacter->m_dwSubPartyID = 0;
			}
		}

		PDNDNPCVIEWDLG pNPCDlg = NULL;
		m_pApp->m_NPCViewMap.Lookup((WORD)dwReturnedID, pNPCDlg);

		if(pNPCDlg != NULL)
		{
			m_pApp->m_NPCViewMap.SetAt((WORD)dwReturnedID,NULL);
			m_pApp->m_NPCViewMap.RemoveKey((WORD)dwReturnedID);
		}

		SetSelectedListCtrlItem(&m_cPartyList, -1);
		InvalidateRect(NULL);

		m_pSelectedCharacterDialog = NULL;
		m_pSelectedNPCDialog = NULL;

		UpdateSelections();

		Refresh();

		Broadcast();

		m_pMainDialog->SortTabs();

		FindAndSelectPartyTab();
	}
}

BOOL DMPartyDialog::FindAndSelectPartyTab()
{
	int nSkippedTabs = 0;

	for (int i = 0; i < m_pApp->m_TabArray.GetSize(); ++i)
	{
		cDNDDisplayTab *pTab = m_pApp->m_TabArray[i];

		if(pTab != NULL)
		{
			if(pTab->m_bHide)
			{
				++nSkippedTabs;
			}
			else if(pTab->m_nTabType == DND_TAB_TYPE_PARTY)
			{
				DMPartyDialog *pPartyDlg = (DMPartyDialog *)pTab->m_pWindow;
				
				if(pPartyDlg->m_pParty == m_pParty)
				{
					CDMHelperDlg *pMainDlg = (CDMHelperDlg *)m_pApp->m_pMainWindow;

					pMainDlg->m_cMainTab.SetCurSel(i-nSkippedTabs);
					pMainDlg->PickTab();

					return TRUE;
				}

			}
		}
	}

	return FALSE;
}

BOOL DMPartyDialog::FindAndSelectCharacterTab(DWORD dwCharID)
{
	int nSkippedTabs = 0;

	for (int i = 0; i < m_pApp->m_TabArray.GetSize(); ++i)
	{
		cDNDDisplayTab *pTab = m_pApp->m_TabArray[i];

		if(pTab != NULL)
		{
			if(pTab->m_bHide)
			{
				++nSkippedTabs;
			}
			else if(pTab->m_nTabType == DND_TAB_TYPE_NPC)
			{
				cDMBaseNPCViewDialog *pNPCDlg = (cDMBaseNPCViewDialog *)pTab->m_pWindow;	//fooker
				
				if(pNPCDlg->m_pNPC != NULL && pNPCDlg->m_pNPC->m_dwCharacterID == dwCharID)
				{
					CDMHelperDlg *pMainDlg = (CDMHelperDlg *)m_pApp->m_pMainWindow;

					pMainDlg->m_cMainTab.SetCurSel(i-nSkippedTabs);
					pMainDlg->PickTab();

					return TRUE;
				}

			}
			else if(pTab->m_nTabType == DND_TAB_TYPE_CHARACTER)
			{
				CDMCharViewDialog *pPCDlg = (CDMCharViewDialog *)pTab->m_pWindow;
				
				if(pPCDlg->m_pCharacter->m_dwCharacterID == dwCharID)
				{
					CDMHelperDlg *pMainDlg = (CDMHelperDlg *)m_pApp->m_pMainWindow;

					pMainDlg->m_cMainTab.SetCurSel(i-nSkippedTabs);
					pMainDlg->PickTab();

					return TRUE;
				}

			}
			else
			{
				TRACE("TAB TYPE %d is %d\n", i, pTab->m_nTabType);
			}
		}
	}

	return FALSE;
}

BOOL DMPartyDialog::FindAndSelectOpposingPartyTab()
{
	int nSkippedTabs = 0;

	for (int i = 0; i < m_pApp->m_TabArray.GetSize(); ++i)
	{
		cDNDDisplayTab *pTab = m_pApp->m_TabArray[i];

		if(pTab != NULL)
		{
			if(pTab->m_bHide)
			{
				++nSkippedTabs;
			}
			else if(pTab->m_nTabType == DND_TAB_TYPE_PARTY || pTab->m_nTabType == DND_TAB_TYPE_SUBPARTY)
			{
				DMPartyDialog *pPartyDlg = (DMPartyDialog *)pTab->m_pWindow;
				
				if(pPartyDlg == m_pOpposingPartyDialog)
				{
					CDMHelperDlg *pMainDlg = (CDMHelperDlg *)m_pApp->m_pMainWindow;

					pMainDlg->m_cMainTab.SetCurSel(i-nSkippedTabs);
					pMainDlg->PickTab();

					return TRUE;
				}

			}
		}
	}

	return FALSE;
}

void DMPartyDialog::LoadPartyExternal()
{
	OnLoadPartyButton();

	UpdateSelections();

	Refresh();
}

void DMPartyDialog::OnLoadPartyButton() 
{
	DMLoadFileDescriptor FileDesc;
	FileDesc.m_szLabel = "Load DM Party File:";
	FileDesc.m_szInitialPath = m_pApp->m_szEXEPath;
	FileDesc.m_szSubDirectory = "data/characters";
	
	FileDesc.m_szInitialFileName = m_szPartyName;
	

	FileDesc.m_szFileTypes = ".dmp";
	FileDesc.m_bSave = FALSE;


#if NEW_FILE_MANAGEMENT_WINDOW
	CDMFileManagementDialog *pDlg = new CDMFileManagementDialog(&FileDesc);
#else
	DMLoadFileDialog *pDlg = new DMLoadFileDialog(&FileDesc);
#endif

	pDlg->DoModal();
	delete pDlg;

	if(FileDesc.m_bSuccess)
	{
		LoadPartyFromFile(FileDesc.m_szReturnedPath.GetBuffer(0));

		//see if this party exists already
		DMPartyDialog *pTestPartyDlg = NULL;
		m_pApp->m_PartyViewMap.Lookup((WORD)m_pParty->m_dwPartyID, pTestPartyDlg);

		if(pTestPartyDlg != NULL)
		{
			m_pParty->m_dwPartyID = 0L;
		}

		m_pApp->m_PartyViewMap.SetAt((WORD)m_pParty->m_dwPartyID, this);

		if(pTestPartyDlg == NULL)
		{
			CString szPath = m_pApp->m_szEXEPath + "data\\characters";
			SearchAndLoadPartyMembers(szPath);

			m_pParty->ValidateNPCList();

			cDNDNonPlayerCharacter *pNPC = NULL;
			for(int i = 0; i < MAX_NPC_PARTY_MEMBERS; ++i)
			{
				if(m_pParty->m_NPCs[i].m_dwCharacterID != 0)
				{
					if(m_pParty->m_NPCs[i].m_bPocketPC == FALSE)
					{
						pNPC = &m_pParty->m_NPCs[i];
						pNPC->m_bRolled = TRUE;
						pNPC->m_bIsNPC = TRUE;
						cDMBaseNPCViewDialog *pDlg = new cDMBaseNPCViewDialog(m_pMainDialog, pNPC, NULL, &m_pMainDialog->m_cMainTab); //TODO memory leak here
						pDlg->ProcessCharStats();

						pNPC->ValidateInventoryFlags();

						m_pMainDialog->AddTab(pDlg, DND_TAB_TYPE_NPC, FALSE);

						m_pApp->m_NPCViewMap.SetAt((WORD)pNPC->m_dwCharacterID, pDlg);
					}
					else
					{
						cDNDCharacter *pCharacter = new cDNDCharacter();

						memcpy(pCharacter, (cDNDCharacter *)&m_pParty->m_NPCs[i], sizeof(cDNDCharacter));

						CDMCharViewDialog *pDlg = new CDMCharViewDialog(m_pMainDialog, pCharacter, &m_pMainDialog->m_cMainTab);

						pDlg->ShowWindow(SW_HIDE);
						
						pDlg->ProcessCharStats();

						//pDlg->m_szSaveFileName = szFileName;

						m_pMainDialog->AddTab(pDlg, DND_TAB_TYPE_CHARACTER, FALSE);

						m_pApp->m_CharacterViewMap.SetAt((WORD)pCharacter->m_dwCharacterID, pDlg);

						pCharacter->ValidateInventoryFlags();

						m_pParty->m_NPCs[i].m_dwCharacterID = 0;
					}
				}
			}

			if(m_pParty->m_dwPartyMapID)
			{
				CString szFilePath;
				szFilePath.Format("%sdata\\maps\\", m_pApp->m_szEXEPath);
				SearchAndLoadMaps(szFilePath, _T(""));
			}

			m_pParty->MarkSaved();
		}
	}

	Refresh();

	if(m_pParty->m_Version < 10009)
	{
		memset(m_pParty->m_SubParty, 0, MAX_SUB_PARTIES * sizeof(cDNDSubParty));
	}
	else
	{
		//load subparties
		cDNDSubParty *pSubParty = NULL;

		for(int i = 0; i < MAX_SUB_PARTIES; ++i)
		{
			if(m_pParty->m_SubParty[i].m_dwSubPartyID != 0)
			{
				pSubParty = &m_pParty->m_SubParty[i];
				
				CDMSubPartyDialog *pSubPartyDialog = new CDMSubPartyDialog(pSubParty, this);

				m_pMainDialog->AddTab(pSubPartyDialog, DND_TAB_TYPE_SUBPARTY, FALSE);
			}
		}
	}
		
}

void DMPartyDialog::SavePartyExternal()
{
	OnSavePartyButton();
}

void DMPartyDialog::OnSavePartyButton() 
{
	
	DMLoadFileDescriptor FileDesc;
	FileDesc.m_szLabel = "Save DM Party File:";
	FileDesc.m_szInitialPath = m_pApp->m_szEXEPath;
	FileDesc.m_szSubDirectory = "data/characters";
	
	FileDesc.m_szInitialFileName = m_pParty->m_szPartyName;
	FileDesc.m_szInitialFileName += _T(".dmp");

	FileDesc.m_szOriginalFileName = m_szSaveFileName;
	

	FileDesc.m_szFileTypes = ".dmp";
	FileDesc.m_bSave = TRUE;

#if NEW_FILE_MANAGEMENT_WINDOW
	CDMFileManagementDialog *pDlg = new CDMFileManagementDialog(&FileDesc);
#else
	DMLoadFileDialog *pDlg = new DMLoadFileDialog(&FileDesc);
#endif

	pDlg->DoModal();
	delete pDlg;

	if(FileDesc.m_bSuccess)
	{
		SavePartyToFile(FileDesc.m_szReturnedPath.GetBuffer(0));
	}
}

  void DMPartyDialog::StorePocketPCs()
{
	CDMCharViewDialog *pCharDlg = NULL;
	WORD wID;

	for (POSITION pos = m_pApp->m_CharacterViewMap.GetStartPosition(); pos != NULL; )
	{
		pCharDlg = NULL;
	
		m_pApp->m_CharacterViewMap.GetNextAssoc(pos,wID,pCharDlg);

		if(pCharDlg != NULL && pCharDlg->m_pCharacter != NULL && pCharDlg->m_pCharacter->m_dwCharacterID && pCharDlg->m_pCharacter->m_bPocketPC)
		{
			if(m_pParty->CharacterIsPartyMember(pCharDlg->m_pCharacter))
			{
				pCharDlg->m_pCharacter->MarkSaved();

				cDNDNonPlayerCharacter _Source;
				memcpy(&_Source, pCharDlg->m_pCharacter, sizeof(cDNDCharacter));

				cDNDNonPlayerCharacter *pSlot = m_pParty->GetOpenNPCSlot();

				if(pSlot != NULL)
				{
					memcpy((LPVOID)pSlot, (LPVOID)&_Source, sizeof(cDNDNonPlayerCharacter));
				}
			}
		}
	}

}


void DMPartyDialog::SavePartyToFile(char *szFileName)
{
	CWaitCursor myWaitCursor;

	FILE *pOutFile = fopen(szFileName, "wb");

	if(pOutFile != NULL)
	{
		if(m_pParty->m_dwPartyID == 0L)
		{
			m_pParty->m_dwPartyID = GetUniqueID();
		}

		m_pParty->m_Version = DMH_CURRENT_VERSION;

		StorePocketPCs();

		m_pParty->MarkSaved();
		
		for(int i = 0; i < MAX_NPC_PARTY_MEMBERS; ++i)
		{
			if(m_pParty->m_NPCs[i].m_dwCharacterID)
			{
				m_pParty->m_NPCs[i].m_bRolled = TRUE;
			}
		}

		fwrite(m_pParty, sizeof(cDNDParty), 1, pOutFile);

		fclose(pOutFile);

		m_pParty->RemovePocketPCs();

		m_szSaveFileName = szFileName;

		if(m_pPartyLog != NULL)
		{
			CString szLogFileName = m_szSaveFileName;

			szLogFileName.MakeLower();

			szLogFileName.Replace(".dmp", ".dml");

			pOutFile = fopen(szLogFileName.GetBuffer(0), "wb");

			if(pOutFile != NULL)
			{
				fwrite(m_pPartyLog, sizeof(cDNDPartyLog), 1, pOutFile);

				fclose(pOutFile);
			}
		}
	}
}

void DMPartyDialog::LoadPartyFromFile(char *szFileName)
{
	CWaitCursor myWaitCursor;

	FILE *pInFile = fopen(szFileName, "rb");

	if(pInFile != NULL)
	{
		if(m_pParty != NULL)
		{
			delete m_pParty;
			m_pParty = new cDNDParty();
		}

		fread(m_pParty, sizeof(cDNDParty), 1, pInFile);

		fclose(pInFile);

		m_szSaveFileName = szFileName;

		CString szLogFileName = m_szSaveFileName;

		szLogFileName.MakeLower();

		szLogFileName.Replace(".dmp", ".dml");

		pInFile = fopen(szLogFileName.GetBuffer(0), "rb");

		if(pInFile != NULL)
		{
			if(m_pPartyLog == NULL)
			{
				m_pPartyLog = new cDNDPartyLog();
			}

			fread(m_pPartyLog, sizeof(cDNDPartyLog), 1, pInFile);

			fclose(pInFile);
		}
		
		m_pParty->MarkSaved();

		Initialize();
	}
}

void DMPartyDialog::SearchAndLoadPartyMembers(CString szPath)
{
	CFileFind find;
	CString strFile;

	//search directories first
	strFile.Format("%s\\*", szPath);

	BOOL bFound = find.FindFile(strFile);
	int nIndex = 0;

	bFound = find.FindFile(strFile);
	nIndex = 0;
	while (bFound)
	{
		bFound = find.FindNextFile();
		CString strFileName;

		if (find.IsDirectory())
		{
			CString szDirPath = find.GetFileName();

			if (szDirPath == "." || szDirPath == "..")
				continue;

			CString szSubDirPath = szPath + "\\" + szDirPath;

			SearchAndLoadPartyMembers(szSubDirPath);

			continue;
		}
	}

	////////////////////////

	strFile.Format("%s\\*.dmc", szPath);

	bFound = find.FindFile(strFile);
	nIndex = 0;
	while(bFound)
	{
		bFound = find.FindNextFile();
		CString strFileName;

		if (find.IsDirectory())
		{
			continue;
		}

		strFileName = find.GetFileName();
		CString ext4=strFileName.Right(4);
		CString ext5=strFileName.Right(5);

		if(!strFileName.IsEmpty() && (strFileName.GetLength()>4))
		{ 
			if(ext4.CompareNoCase(".dmc")==0) 
			{
				CString szFileName;
				szFileName.Format("%s\\%s", szPath, strFileName);

				FILE *pInFile = fopen(szFileName.GetBuffer(0), "rb");

				if(pInFile != NULL)
				{
					cDNDCharacter *pCharacter = new cDNDCharacter();
				
					fread(pCharacter, sizeof(cDNDCharacter), 1, pInFile);

					fclose(pInFile);

					if(pCharacter->m_Version < 10009)
					{
						memset(pCharacter->m_nLanguages, 0, MAX_CHARACTER_LANGUAGES * sizeof(int));
					}

					if(m_pParty->CharacterIsPartyMember(pCharacter) && !CharacterIsLoaded(pCharacter))
					{
						if (strcmp(pCharacter->m_szCharacterName, "Elric") == 0)
						{
							TRACE("!");
						}

						m_pMainDialog->RetrieveCharacter(pCharacter, szFileName.GetBuffer(0));
					}
					else
					{
						delete pCharacter;
					}
				}

				++nIndex;
			}
		}
	}
}

BOOL DMPartyDialog::CharacterIsLoaded(cDNDCharacter *pCharacter)
{
	PDNDCHARVIEWDLG pCharDlg = NULL;
	m_pApp->m_CharacterViewMap.Lookup((WORD)pCharacter->m_dwCharacterID, pCharDlg);

	if(pCharDlg != NULL)
		return TRUE;

	return FALSE;
}


void DMPartyDialog::SearchAndLoadMaps(CString szFilePath, CString szSubDir)
{
	CFileFind find;
	CString strFile;

	BOOL bSuccess = FALSE;

	strFile.Format("%s%s*.*", szFilePath, szSubDir);

	BOOL bFound = find.FindFile(strFile);
	int nIndex = 0;
	while(bFound && !bSuccess)
	{
		bFound = find.FindNextFile();
		CString strFileName;

		if (find.IsDirectory())
		{
			CString szFoundSubDir = find.GetFileName();

			if (szFoundSubDir == "." || szFoundSubDir == ".." || szFoundSubDir == "bitmaps")
				continue;

			szFoundSubDir += "\\";

			CString szNewSubDir = szSubDir + szFoundSubDir;

			SearchAndLoadMaps(szFilePath, szNewSubDir);

			continue;
		}

		strFileName = find.GetFileName();
		CString ext4=strFileName.Right(4);
		CString ext5=strFileName.Right(5);

		if(!strFileName.IsEmpty() && (strFileName.GetLength()>4))
		{ 
			if(ext4.CompareNoCase(".dmm")==0) 
			{
				CString szFileName;
				//szFileName.Format("%sdata\\maps\\%s", m_pApp->m_szEXEPath, strFileName);
				szFileName.Format("%s%s%s", szFilePath, szSubDir, strFileName);

				FILE *pInFile = fopen(szFileName.GetBuffer(0), "rb");

				if(pInFile != NULL)
				{
					cDNDMap *pMap = new cDNDMap();
				
					fread(pMap, sizeof(cDNDMap), 1, pInFile);

					fclose(pInFile);

					
					if(pMap->m_Version < 10009)
					{
						pMap->m_bTiles = FALSE;
						memset(pMap->m_Tiles, 0, MAX_MAP_TILES * sizeof(cDNDMapTile));
					}

					if (pMap->m_Version < 10026)
					{
						pMap->m_bDisplayLayer[0] = FALSE;
						pMap->m_bDisplayLayer[1] = FALSE;
						pMap->m_bDisplayLayer[2] = FALSE;
						pMap->m_bDisplayLayer[3] = FALSE;

						pMap->m_nTransRed = 255;
						pMap->m_nTransGreen = 255;
						pMap->m_nTransBlue = 255;
					}

					if(m_pParty->m_dwPartyMapID == pMap->m_dwMapID && !MapIsLoaded(pMap))
					{
						if(pMap->m_dwParentMapID)
						{
							if (SearchAndLoadParentMap(pMap, szFilePath, _T("")) == FALSE)
							{
								pMap->m_dwParentMapID = 0L;
								pMap->MarkChanged();
							}
						}

						m_pMainDialog->RetrieveMap(pMap, m_pParty->m_dwPartyID, szFileName.GetBuffer(0));
						m_szPartyMapName = pMap->m_szMapName;
						pMap->MarkSaved();

						strcpy(pMap->m_szLoadedFilename, szFileName.Left(511));

						bSuccess = TRUE;
					}
					else
					{
						delete pMap;
					}
				}

				++nIndex;
			}
		}
	}
}

BOOL DMPartyDialog::SearchAndLoadParentMap(cDNDMap *pChildMap, CString szFilePath, CString szSubDir)
{
	CFileFind find;
	CString strFile;

	BOOL bSuccess = FALSE;

	//strFile.Format("%sdata\\maps\\*.*", m_pApp->m_szEXEPath);
	strFile.Format("%s%s*.*", szFilePath, szSubDir);

	BOOL bFound = find.FindFile(strFile);
	int nIndex = 0;
	while(bFound && !bSuccess)
	{
		bFound = find.FindNextFile();
		CString strFileName;

		if (find.IsDirectory())
		{
			CString szFoundSubDir = find.GetFileName();

			if (szFoundSubDir == "." || szFoundSubDir == ".." || szFoundSubDir == "bitmaps")
				continue;

			szFoundSubDir += "\\";

			CString szNewSubDir = szSubDir + szFoundSubDir;

			if (SearchAndLoadParentMap(pChildMap, szFilePath, szNewSubDir))
			{
				return TRUE;
			}

			continue;
		}

		strFileName = find.GetFileName();
		CString ext4=strFileName.Right(4);
		CString ext5=strFileName.Right(5);

		if (ext4.CompareNoCase(".dmm") != 0)
			continue;

		TRACE("SEARCH PARENT %s (%s %ld)\n", strFileName, pChildMap->m_szMapName, pChildMap->m_dwParentMapID);

		if(!strFileName.IsEmpty() && (strFileName.GetLength()>4))
		{ 
			if(ext4.CompareNoCase(".dmm")==0) 
			{
				CString szFileName;
				//szFileName.Format("%sdata\\maps\\%s", m_pApp->m_szEXEPath, strFileName);
				szFileName.Format("%s%s%s", szFilePath, szSubDir, strFileName);

				FILE *pInFile = fopen(szFileName.GetBuffer(0), "rb");

				if(pInFile != NULL)
				{
					cDNDMap *pMap = new cDNDMap();
				
					fread(pMap, sizeof(cDNDMap), 1, pInFile);

					fclose(pInFile);

					if(pMap->m_Version < 10009)
					{
						pMap->m_bTiles = FALSE;
						memset(pMap->m_Tiles, 0, MAX_MAP_TILES * sizeof(cDNDMapTile));
					}

					if (pMap->m_Version < 10026)
					{
						pMap->m_bDisplayLayer[0] = FALSE;
						pMap->m_bDisplayLayer[1] = FALSE;
						pMap->m_bDisplayLayer[2] = FALSE;
						pMap->m_bDisplayLayer[3] = FALSE;

						pMap->m_nTransRed = 255;
						pMap->m_nTransGreen = 255;
						pMap->m_nTransBlue = 255;
					}

					if(pChildMap->m_dwParentMapID == pMap->m_dwMapID)
					{
						if(!MapIsLoaded(pMap))
						{
							if(pMap->m_dwParentMapID)
							{
								SearchAndLoadParentMap(pMap, szFilePath, szSubDir);
							}

							m_pMainDialog->RetrieveMap(pMap, m_pParty->m_dwPartyID, szFileName.GetBuffer(0));
							m_szPartyMapName = pMap->m_szMapName;
							pMap->MarkSaved();
						}

						strcpy(pChildMap->m_szLoadedFilename, szFileName.Left(511));

						bSuccess = TRUE;
					}
					else
					{
						delete pMap;
					}
				}

				++nIndex;
			}
		}
	}

	return bSuccess;

}


BOOL DMPartyDialog::MapIsLoaded(cDNDMap *pMap)
{
	PDNDMAPVIEWDLG pMapDlg = NULL;
	m_pApp->m_MapViewMap.Lookup((WORD)pMap->m_dwMapID, pMapDlg);

	if(pMapDlg != NULL)
		return TRUE;

	return FALSE;
}

void DMPartyDialog::CalcPartyHash()
{
	if(m_pParty == NULL)
		return;

	m_lPartyHash = 0L;
	for(int i = 0; i < sizeof(cDNDParty); ++i)
	{
		m_lPartyHash += ((ULONG)(m_pParty+i));
	}
}

void DMPartyDialog::OnSetfocusMapNameEdit() 
{
	m_cAnchorButton.SetFocus();

	DWORD dwReturnedID = 0;
	DMCharacterSelectorDialog *pDlg = new DMCharacterSelectorDialog(&dwReturnedID, 0, DND_SELECTOR_MAP);
	pDlg->DoModal();

	if(dwReturnedID)
	{
		m_pParty->m_dwPartyMapID = dwReturnedID;

		PDNDMAPVIEWDLG pMapDlg = NULL;
		m_pApp->m_MapViewMap.Lookup((WORD)dwReturnedID, pMapDlg);

		if(pMapDlg != NULL)
		{
			m_szPartyMapName = pMapDlg->m_pDNDMap->m_szMapName;
		}

		UpdateData(FALSE);
		InvalidateRect(NULL);

		m_pParty->MarkChanged();
	}
	
}

void DMPartyDialog::OnSelchangeCalendarCombo() 
{
	UpdateData(TRUE);

	int nCursor = m_cCalendarCombo.GetCurSel();

	if(nCursor > -1)
	{
		PCALENDAR pCalendar = (PCALENDAR)m_cCalendarCombo.GetItemData(nCursor);

		if(pCalendar != NULL)
		{
			m_pParty->m_dwCalendarID = pCalendar->m_dwCalendarID;

			m_pApp->m_pSelectedCalendar = pCalendar;

			Refresh();

			m_pParty->MarkChanged();
		}
	}
	
}

void DMPartyDialog::OnDecSegmentButton() 
{
	m_pParty->ModifyCalendar(DND_CALENDAR_MOD_SEGMENT, DND_CALENDAR_MOD_DECREMENT);

	Refresh();

	m_pParty->MarkChanged();
}

void DMPartyDialog::OnIncSegmentButton() 
{
	m_pParty->ModifyCalendar(DND_CALENDAR_MOD_SEGMENT, DND_CALENDAR_MOD_INCREMENT);

	Refresh();

	m_pParty->MarkChanged();
}

void DMPartyDialog::OnDecRoundButton() 
{
	m_pParty->ModifyCalendar(DND_CALENDAR_MOD_ROUND, DND_CALENDAR_MOD_DECREMENT);

	Refresh();
	
	m_pParty->MarkChanged();
}

void DMPartyDialog::OnIncRoundButton() 
{
	m_pParty->ModifyCalendar(DND_CALENDAR_MOD_ROUND, DND_CALENDAR_MOD_INCREMENT);

	Refresh();

	m_pParty->MarkChanged();
}

void DMPartyDialog::OnIncEditTimeButton() 
{
	DMTimeEditDialog *pDlg = new DMTimeEditDialog(m_pParty, this);

	pDlg->DoModal();

	delete pDlg;

	m_pParty->MarkChanged();
	
}

void DMPartyDialog::OnClose() 
{
	FreeParty();	

	CDialog::OnClose();

	delete this;
}

void DMPartyDialog::PostNcDestroy() 
{
	FreeParty();	

	CDialog::PostNcDestroy();

	delete this;
}

void DMPartyDialog::FreeParty()
{
	if(m_dwSubPartyID == 0)
	{
		m_pPartyClockThread = NULL;

		if(m_pPartyLog != NULL)	
		{
			delete m_pPartyLog;
			m_pPartyLog = NULL;
		}

		if(m_pParty != NULL)
		{
			m_pApp->m_PartyViewMap.RemoveKey((WORD)m_pParty->m_dwPartyID);

			delete m_pParty;
			m_pParty = NULL;
		}
	}
	else
	{
		m_pApp->m_PartyViewMap.RemoveKey((WORD)m_dwSubPartyID);
	}

}

void DMPartyDialog::Cleanup()
{
	//clean up subparties
	for(int i = 0; i < MAX_SUB_PARTIES; ++i)
	{
		if(m_pParty->m_SubParty[i].m_dwSubPartyID)
		{
			m_pMainDialog->FindAndDeleteTab(DND_TAB_TYPE_SUBPARTY, m_pParty->m_SubParty[i].m_dwSubPartyID, FALSE);
			//m_pApp->m_PartyViewMap.RemoveKey((WORD)m_pParty->m_SubParty[i].m_dwSubPartyID);
		}
	}

	m_pMainDialog->SortTabs();

	if(m_pCalendarDialog != NULL)
	{
		m_pCalendarDialog->PostMessage(WM_CLOSE, 0, 0);
		m_pCalendarDialog = NULL;
	}

	if(m_pPartyLogDialog != NULL)
	{
		m_pPartyLogDialog->PostMessage(WM_CLOSE, 0, 0);
		m_pPartyLogDialog = NULL;
	}

	if(m_pEventLoggerDialog != NULL)
	{
		m_pEventLoggerDialog->PostMessage(WM_CLOSE, 0, 0);
		m_pEventLoggerDialog = NULL;
	}

	if(m_pPartyLog != NULL)
	{
		delete m_pPartyLog;
		m_pPartyLog = NULL;
	}

	if (m_pInitiativeDialog != NULL)
	{
		m_pInitiativeDialog->PostMessage(WM_CLOSE, 0, 0);
		m_pInitiativeDialog = NULL;
	}

	for (int i = 0; i < m_pApp->m_TabArray.GetSize(); ++i)
	{
		cDNDDisplayTab *pTab = m_pApp->m_TabArray[i];

		if (pTab != NULL)
		{
			if (pTab->m_nTabType == DND_TAB_TYPE_PARTY)
			{
				DMPartyDialog *pPartyDlg = (DMPartyDialog *)pTab->m_pWindow;
				if (pPartyDlg->m_pInitiativeDialog != NULL)
				{
					pPartyDlg->m_pInitiativeDialog->PostMessage(WM_CLOSE, 0, 0);
					pPartyDlg->m_pInitiativeDialog = NULL;
				}
			}
		}
	}

}

void DMPartyDialog::OnClickPartyList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(FALSE);

	int nCursor = GetSelectedListCtrlItem(&m_cPartyList);

	m_pBaseCharDlg = NULL;
	m_pSelectedCharacterDialog = NULL;
	m_pSelectedNPCDialog = NULL;

	if(nCursor > -1)
	{
		m_pBaseCharDlg = (CDMBaseCharViewDialog *)m_cPartyList.GetItemData(nCursor);

		if(m_pBaseCharDlg != NULL)
		{
			switch(m_pBaseCharDlg->GetType())
			{
				case DND_CHAR_VIEW_TYPE_PC:		m_pSelectedCharacterDialog =  (CDMCharViewDialog *)m_pBaseCharDlg; break;
				case DND_CHAR_VIEW_TYPE_NPC:	m_pSelectedNPCDialog =  (cDMBaseNPCViewDialog *)m_pBaseCharDlg; break;
			}
		}

		if (m_pOpposingCharacterDialog != NULL)
		{
			m_pOpposingCharacterDialog->m_pTargetBaseDlg = m_pBaseOpposingCharDlg;
		}
		if (m_pOpposingNPCDialog != NULL)
		{
			m_pOpposingNPCDialog->m_pTargetBaseDlg = m_pBaseOpposingCharDlg;
		}

		Refresh();

		if (m_pInitiativeDialog != NULL)
		{
			m_pInitiativeDialog->Refresh();
		}
	}
	
	*pResult = 0;
}

void DMPartyDialog::OnClickPartyList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(FALSE);

	int nCursor = GetSelectedListCtrlItem(&m_cPartyList2);

	m_pBaseOpposingCharDlg = NULL;
	m_pOpposingCharacterDialog = NULL;
	m_pOpposingNPCDialog = NULL;

	if(nCursor > -1)
	{
		m_pBaseOpposingCharDlg = (CDMBaseCharViewDialog *)m_cPartyList2.GetItemData(nCursor);

		if(m_pBaseOpposingCharDlg != NULL)
		{
			switch(m_pBaseOpposingCharDlg->GetType())
			{
				case DND_CHAR_VIEW_TYPE_PC:		m_pOpposingCharacterDialog =  (CDMCharViewDialog *)m_pBaseOpposingCharDlg; break;
				case DND_CHAR_VIEW_TYPE_NPC:	m_pOpposingNPCDialog =  (cDMBaseNPCViewDialog *)m_pBaseOpposingCharDlg; break;
			}

			if (m_pSelectedCharacterDialog != NULL)
			{
				m_pSelectedCharacterDialog->m_pTargetBaseDlg = m_pBaseOpposingCharDlg;
			}
			if (m_pSelectedNPCDialog != NULL)
			{
				m_pSelectedNPCDialog->m_pTargetBaseDlg = m_pBaseOpposingCharDlg;
			}
		}

		Refresh();

		if (m_pInitiativeDialog != NULL)
		{
			m_pInitiativeDialog->Refresh();
		}
	}
	
	*pResult = 0;
}



BOOL DMPartyDialog::SetSelectedTarget(CDMBaseCharViewDialog *pBaseCharDlg)
{
	if(pBaseCharDlg->m_pTargetBaseDlg != NULL)
	{
		if(FindPartyListSelection(pBaseCharDlg->m_pTargetBaseDlg, TRUE) == FALSE)
		{
			pBaseCharDlg->m_pTargetBaseDlg = NULL;
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}	

	return FALSE;
}


BOOL DMPartyDialog::FindPartyListSelection(CDMBaseCharViewDialog *pBaseDlg, BOOL bTarget)
{
	if(pBaseDlg == NULL)
	{
		return FALSE;
	}

	for(int i =0; i < m_cPartyList.GetItemCount(); ++i)
	{
		if( (CDMBaseCharViewDialog *)m_cPartyList.GetItemData(i) == pBaseDlg)
		{
			m_pSelectedCharacterDialog = NULL;
			m_pSelectedNPCDialog = NULL;

			m_pBaseCharDlg = (CDMBaseCharViewDialog *)m_cPartyList.GetItemData(i);

			if(m_pBaseCharDlg != NULL)
			{
				switch(m_pBaseCharDlg->GetType())
				{
					case DND_CHAR_VIEW_TYPE_PC:		m_pSelectedCharacterDialog =  (CDMCharViewDialog *)m_pBaseCharDlg; break;
					case DND_CHAR_VIEW_TYPE_NPC:	m_pSelectedNPCDialog =  (cDMBaseNPCViewDialog *)m_pBaseCharDlg; break;
				}
			}

			SetSelectedListCtrlItem(&m_cPartyList, i);

			Refresh();	

			if (bTarget)
			{
				m_cAttackHitButton.EnableWindow(FALSE);
				m_cAttackMissButton.EnableWindow(FALSE);
				m_cMissileHitButton.EnableWindow(FALSE);
				m_cMissileMissButton.EnableWindow(FALSE);
				m_cCastSpellButton.EnableWindow(FALSE);
			}

			return TRUE;
		}
	}

	for(int i =0; i < m_cPartyList2.GetItemCount(); ++i)
	{
		if( (CDMBaseCharViewDialog *)m_cPartyList2.GetItemData(i) == pBaseDlg)
		{
			m_pOpposingCharacterDialog = NULL;
			m_pOpposingNPCDialog = NULL;

			m_pBaseOpposingCharDlg = (CDMBaseCharViewDialog *)m_cPartyList2.GetItemData(i);

			if(m_pBaseOpposingCharDlg != NULL)
			{
				switch(m_pBaseOpposingCharDlg->GetType())
				{
					case DND_CHAR_VIEW_TYPE_PC:		m_pOpposingCharacterDialog =  (CDMCharViewDialog *)m_pBaseOpposingCharDlg; break;
					case DND_CHAR_VIEW_TYPE_NPC:	m_pOpposingNPCDialog =  (cDMBaseNPCViewDialog *)m_pBaseOpposingCharDlg; break;
				}
			}

			SetSelectedListCtrlItem(&m_cPartyList2, i);

			Refresh();	

			if (bTarget)
			{
				m_cAttackHitButton2.EnableWindow(FALSE);
				m_cAttackMissButton2.EnableWindow(FALSE);
				m_cMissileHitButton2.EnableWindow(FALSE);
				m_cMissileMissButton2.EnableWindow(FALSE);
				m_cCastSpellButton2.EnableWindow(FALSE);
			}


			return TRUE;
		}
	}

	return FALSE;
}

void DMPartyDialog::OnUpPartyButton() 
{
	if(m_cPartyList.GetItemCount())
	{
		UpdateData(FALSE);

		int nCursor = GetSelectedListCtrlItem(&m_cPartyList);

		nCursor--;

		if(nCursor < 0)
			nCursor = 0;

		m_pBaseCharDlg = NULL;
		m_pSelectedCharacterDialog = NULL;
		m_pSelectedNPCDialog = NULL;

		if(nCursor >= 0)
		{
			m_pBaseCharDlg = (CDMBaseCharViewDialog *)m_cPartyList.GetItemData(nCursor);

			if(m_pBaseCharDlg != NULL)
			{
				switch(m_pBaseCharDlg->GetType())
				{
					case DND_CHAR_VIEW_TYPE_PC:		m_pSelectedCharacterDialog =  (CDMCharViewDialog *)m_pBaseCharDlg; break;
					case DND_CHAR_VIEW_TYPE_NPC:	m_pSelectedNPCDialog =  (cDMBaseNPCViewDialog *)m_pBaseCharDlg; break;
				}
			}

			SetSelectedListCtrlItem(&m_cPartyList, nCursor);

			Refresh();	
		}
	}
}

void DMPartyDialog::OnDownPartyButton() 
{
	if(m_cPartyList.GetItemCount())
	{
		UpdateData(FALSE);

		int nCursor = GetSelectedListCtrlItem(&m_cPartyList);

		nCursor++;

		if(nCursor >= m_cPartyList.GetItemCount())
			nCursor = m_cPartyList.GetItemCount()-1;

		m_pBaseCharDlg = NULL;
		m_pSelectedCharacterDialog = NULL;
		m_pSelectedNPCDialog = NULL;

		if(nCursor >= 0)
		{
			m_pBaseCharDlg = (CDMBaseCharViewDialog *)m_cPartyList.GetItemData(nCursor);

			if(m_pBaseCharDlg != NULL)
			{
				switch(m_pBaseCharDlg->GetType())
				{
					case DND_CHAR_VIEW_TYPE_PC:		m_pSelectedCharacterDialog =  (CDMCharViewDialog *)m_pBaseCharDlg; break;
					case DND_CHAR_VIEW_TYPE_NPC:	m_pSelectedNPCDialog =  (cDMBaseNPCViewDialog *)m_pBaseCharDlg; break;
				}
			}

			SetSelectedListCtrlItem(&m_cPartyList, nCursor);

			Refresh();
		}
	}
}

void DMPartyDialog::OnAttackHitButton() 
{

	if(m_pBaseCharDlg != NULL)
	{
		m_pBaseCharDlg->m_pTargetBaseDlg = m_pBaseOpposingCharDlg;
	}


	if(m_pSelectedCharacterDialog != NULL)
	{
		m_pSelectedCharacterDialog->FireAmmo();

		int nWeapon = m_pSelectedCharacterDialog->m_pCharacter->m_SelectedWeapons[0].m_wTypeId;

		if(nWeapon == 0)
		{
			nWeapon = m_pApp->m_nPummelWeaponIndex;

			if(m_pSelectedCharacterDialog->m_pCharacter->m_Class[0] == DND_CHARACTER_CLASS_MONK)
			{
				nWeapon = m_pApp->m_nMonkWeaponIndex;
			}
		}

		if(m_pInitiativeDialog != NULL)
		{
			m_pSelectedCharacterDialog->m_szInitiativeAction = _T("attack hit !");
			m_pInitiativeDialog->Refresh();
		}

		m_pApp->PlayWeaponSFX(nWeapon, 0);

		OnWoundButton2();
	}
	

	if(m_pSelectedNPCDialog != NULL)
	{
		m_pSelectedNPCDialog->FireAmmo();

		int nWeapon = m_pSelectedNPCDialog->m_pNPC->m_SelectedWeapons[0].m_wTypeId;

		if(nWeapon == 0)
		{
			nWeapon = m_pApp->m_nPummelWeaponIndex;

			if(m_pSelectedNPCDialog->m_pNPC->m_Class[0] == DND_CHARACTER_CLASS_MONK)
				nWeapon = m_pApp->m_nMonkWeaponIndex;
		}

		if(m_pInitiativeDialog != NULL)
		{
			m_pSelectedNPCDialog->m_szInitiativeAction = _T("attack hit !");
			m_pInitiativeDialog->Refresh();
		}

		m_pApp->PlayWeaponSFX(nWeapon, 0);

		OnWoundButton2();
	}
}

void DMPartyDialog::OnAttackMissButton() 
{

	if(m_pBaseCharDlg != NULL)
	{
		m_pBaseCharDlg->m_pTargetBaseDlg = m_pBaseOpposingCharDlg;
	}

	if(m_pSelectedCharacterDialog != NULL)
	{
		m_pSelectedCharacterDialog->FireAmmo();

		int nWeapon = m_pSelectedCharacterDialog->m_pCharacter->m_SelectedWeapons[0].m_wTypeId;

		if(nWeapon == 0)
		{
			nWeapon = m_pApp->m_nPummelWeaponIndex;

			if(m_pSelectedCharacterDialog->m_pCharacter->m_Class[0] == DND_CHARACTER_CLASS_MONK)
				nWeapon = m_pApp->m_nMonkWeaponIndex;
		}

		if(m_pInitiativeDialog != NULL)
		{
			m_pSelectedCharacterDialog->m_szInitiativeAction = _T("attack missed");
			m_pInitiativeDialog->Refresh();
		}

		m_pApp->PlayWeaponSFX(nWeapon, 1);
	}

	if(m_pSelectedNPCDialog != NULL)
	{
		m_pSelectedNPCDialog->FireAmmo();

		int nWeapon = m_pSelectedNPCDialog->m_pNPC->m_SelectedWeapons[0].m_wTypeId;

		if(nWeapon == 0)
		{
			nWeapon = m_pApp->m_nPummelWeaponIndex;

			if(m_pSelectedNPCDialog->m_pNPC->m_Class[0] == DND_CHARACTER_CLASS_MONK)
				nWeapon = m_pApp->m_nMonkWeaponIndex;
		}

		if(m_pInitiativeDialog != NULL)
		{
			m_pSelectedNPCDialog->m_szInitiativeAction = _T("attack missed");
			m_pInitiativeDialog->Refresh();
		}

		m_pApp->PlayWeaponSFX(nWeapon, 1);
	}
	
}


void DMPartyDialog::OnMissileHitButton() 
{
	if(m_pBaseCharDlg != NULL)
	{
		m_pBaseCharDlg->m_pTargetBaseDlg = m_pBaseOpposingCharDlg;
	}

	if(m_pSelectedCharacterDialog != NULL)
	{
		m_pSelectedCharacterDialog->FireAmmo();

		int nWeapon = m_pSelectedCharacterDialog->m_pCharacter->m_SelectedWeapons[0].m_wTypeId;

		if(m_pInitiativeDialog != NULL)
		{
			m_pSelectedCharacterDialog->m_szInitiativeAction = _T("attack hit");
			m_pInitiativeDialog->Refresh();
		}

		m_pApp->PlayWeaponSFX(nWeapon, 2);

		m_pSelectedCharacterDialog->ProcessCharStats();
		m_pSelectedCharacterDialog->Refresh();

		OnWoundButton2();
	}

	if(m_pSelectedNPCDialog != NULL)
	{
		m_pSelectedNPCDialog->FireAmmo();

		int nWeapon = m_pSelectedNPCDialog->m_pNPC->m_SelectedWeapons[0].m_wTypeId;

		if(m_pInitiativeDialog != NULL)
		{
			m_pSelectedNPCDialog->m_szInitiativeAction = _T("attack hit");
			m_pInitiativeDialog->Refresh();
		}

		m_pApp->PlayWeaponSFX(nWeapon, 2);

		m_pSelectedNPCDialog->ProcessCharStats();
		m_pSelectedNPCDialog->Refresh();

		OnWoundButton2();
	}
	
}

void DMPartyDialog::OnMissileMissButton() 
{
	if(m_pBaseCharDlg != NULL)
	{
		m_pBaseCharDlg->m_pTargetBaseDlg = m_pBaseOpposingCharDlg;
	}

	if(m_pSelectedCharacterDialog != NULL)
	{
		m_pSelectedCharacterDialog->FireAmmo();

		int nWeapon = m_pSelectedCharacterDialog->m_pCharacter->m_SelectedWeapons[0].m_wTypeId;

		if(m_pInitiativeDialog != NULL)
		{
			m_pSelectedCharacterDialog->m_szInitiativeAction = _T("attack missed");
			m_pInitiativeDialog->Refresh();
		}

		m_pApp->PlayWeaponSFX(nWeapon, 3);
			
		m_pSelectedCharacterDialog->ProcessCharStats();
		m_pSelectedCharacterDialog->Refresh();

		Refresh();
	}

	if(m_pSelectedNPCDialog != NULL)
	{
		m_pSelectedNPCDialog->FireAmmo();

		int nWeapon = m_pSelectedNPCDialog->m_pNPC->m_SelectedWeapons[0].m_wTypeId;

		if(m_pInitiativeDialog != NULL)
		{
			m_pSelectedNPCDialog->m_szInitiativeAction = _T("attack missed");
			m_pInitiativeDialog->Refresh();
		}

		m_pApp->PlayWeaponSFX(nWeapon, 3);

		m_pSelectedNPCDialog->ProcessCharStats();
		m_pSelectedNPCDialog->Refresh();

		Refresh();
	}
}


void DMPartyDialog::OnCastSpellButton() 
{
	if(m_pSelectedCharacterDialog != NULL && m_pSelectedCharacterDialog->m_pCharacter != NULL)
	{
		DMCastSpellDialog *pDlg = new DMCastSpellDialog(this, m_pSelectedCharacterDialog, m_pSelectedCharacterDialog->m_pCharacter);
		pDlg->DoModal();
		delete pDlg;

		m_pSelectedCharacterDialog->Refresh();
	}	
}

void DMPartyDialog::OnWoundButton() 
{
	if(m_pSelectedCharacterDialog != NULL && m_pSelectedCharacterDialog->m_pCharacter != NULL)
	{
		int nValue = 0;

		ModifyValue((int *)&nValue, "Add Damage:", FALSE);

		m_pSelectedCharacterDialog->m_pCharacter->m_nCurrentDamage += nValue;

		if(m_pSelectedCharacterDialog->m_pCharacter->m_nCurrentDamage < 0)
			m_pSelectedCharacterDialog->m_pCharacter->m_nCurrentDamage = 0;

		m_pSelectedCharacterDialog->ProcessCharStats();
		m_pSelectedCharacterDialog->Refresh();
	}

	if(m_pSelectedNPCDialog != NULL && m_pSelectedNPCDialog->m_pNPC != NULL)
	{
		int nValue = 0;

		ModifyValue((int *)&nValue, "Add Damage:", FALSE);

		m_pSelectedNPCDialog->m_pNPC->m_nCurrentDamage += nValue;

		if(m_pSelectedNPCDialog->m_pNPC->m_nCurrentDamage < 0)
			m_pSelectedNPCDialog->m_pNPC->m_nCurrentDamage = 0;

		m_pSelectedNPCDialog->ProcessCharStats();
		m_pSelectedNPCDialog->Refresh();
	}

	Refresh();
	
}

void DMPartyDialog::OnHealButton() 
{
	if(m_pSelectedCharacterDialog != NULL && m_pSelectedCharacterDialog->m_pCharacter != NULL)
	{
		int nValue = 0;

		ModifyValue((int *)&nValue, "Heal Damage:", FALSE);

		m_pSelectedCharacterDialog->m_pCharacter->m_nCurrentDamage -= nValue;

		if(m_pSelectedCharacterDialog->m_pCharacter->m_nCurrentDamage < 0)
			m_pSelectedCharacterDialog->m_pCharacter->m_nCurrentDamage = 0;

		m_pSelectedCharacterDialog->ProcessCharStats();

		m_pSelectedCharacterDialog->Refresh();
	}

	if(m_pSelectedNPCDialog != NULL && m_pSelectedNPCDialog->m_pNPC != NULL)
	{
		int nValue = 0;

		ModifyValue((int *)&nValue, "Heal Damage:", FALSE);

		m_pSelectedNPCDialog->m_pNPC->m_nCurrentDamage -= nValue;

		if(m_pSelectedNPCDialog->m_pNPC->m_nCurrentDamage < 0)
			m_pSelectedNPCDialog->m_pNPC->m_nCurrentDamage = 0;

		m_pSelectedNPCDialog->ProcessCharStats();

		m_pSelectedNPCDialog->Refresh();
	}

	Refresh();
}



void DMPartyDialog::OnSelchangePartyCombo2() 
{

	UpdateData(TRUE);

	int nCursor = m_cPartyCombo2.GetCurSel();

	if(nCursor > -1)
	{
		cDNDDataPicker *pPicker = (cDNDDataPicker *)m_cPartyCombo2.GetItemData(nCursor);

		m_pOpposingPartyDialog = (DMPartyDialog *)pPicker->m_pData1;
		m_pOpposingSubParty = (cDNDSubParty *)pPicker->m_pData2;

		SetSelectedListCtrlItem(&m_cPartyList2, -1);

		m_pBaseOpposingCharDlg = NULL;
		m_pOpposingCharacterDialog = NULL;
		m_pOpposingNPCDialog = NULL;

		Refresh();

		if(m_pInitiativeDialog != NULL)
		{
			m_pInitiativeDialog->Refresh();
		}
	}

}

void DMPartyDialog::OnAttackHitButton2() 
{
	if(m_pBaseOpposingCharDlg != NULL)
	{
		m_pBaseOpposingCharDlg->m_pTargetBaseDlg = m_pBaseCharDlg;
	}

	if(m_pOpposingCharacterDialog != NULL)
	{
		m_pOpposingCharacterDialog->FireAmmo();

		int nWeapon = m_pOpposingCharacterDialog->m_pCharacter->m_SelectedWeapons[0].m_wTypeId;

		if(nWeapon == 0)
		{
			nWeapon = m_pApp->m_nPummelWeaponIndex;

			if(m_pOpposingCharacterDialog->m_pCharacter->m_Class[0] == DND_CHARACTER_CLASS_MONK)
			{
				nWeapon = m_pApp->m_nMonkWeaponIndex;
			}
		}

		if(m_pInitiativeDialog != NULL)
		{
			m_pOpposingCharacterDialog->m_szInitiativeAction = _T("attack hit");
			m_pInitiativeDialog->Refresh();
		}

		m_pApp->PlayWeaponSFX(nWeapon, 0);

		OnWoundButton();
	}

	if(m_pOpposingNPCDialog != NULL)
	{
		m_pOpposingNPCDialog->FireAmmo();

		int nWeapon = m_pOpposingNPCDialog->m_pNPC->m_SelectedWeapons[0].m_wTypeId;

		if(nWeapon == 0)
		{
			nWeapon = m_pApp->m_nPummelWeaponIndex;

			if(m_pOpposingNPCDialog->m_pNPC->m_Class[0] == DND_CHARACTER_CLASS_MONK)
				nWeapon = m_pApp->m_nMonkWeaponIndex;
		}

		if(m_pInitiativeDialog != NULL)
		{
			m_pOpposingNPCDialog->m_szInitiativeAction = _T("attack hit");
			m_pInitiativeDialog->Refresh();
		}

		m_pApp->PlayWeaponSFX(nWeapon, 0);

		OnWoundButton();
	}
	
}

void DMPartyDialog::OnAttackMissButton2() 
{
	if(m_pBaseOpposingCharDlg != NULL)
	{
		m_pBaseOpposingCharDlg->m_pTargetBaseDlg = m_pBaseCharDlg;
	}

	if(m_pOpposingCharacterDialog != NULL)
	{
		m_pOpposingCharacterDialog->FireAmmo();

		int nWeapon = m_pOpposingCharacterDialog->m_pCharacter->m_SelectedWeapons[0].m_wTypeId;

		if(nWeapon == 0)
		{
			nWeapon = m_pApp->m_nPummelWeaponIndex;

			if(m_pOpposingCharacterDialog->m_pCharacter->m_Class[0] == DND_CHARACTER_CLASS_MONK)
				nWeapon = m_pApp->m_nMonkWeaponIndex;
		}

		if(m_pInitiativeDialog != NULL)
		{
			m_pOpposingCharacterDialog->m_szInitiativeAction = _T("attack missed");
			m_pInitiativeDialog->Refresh();
		}


		m_pApp->PlayWeaponSFX(nWeapon, 1);
	}

	if(m_pOpposingNPCDialog != NULL)
	{
		m_pOpposingNPCDialog->FireAmmo();

		int nWeapon = m_pOpposingNPCDialog->m_pNPC->m_SelectedWeapons[0].m_wTypeId;

		if(nWeapon == 0)
		{
			nWeapon = m_pApp->m_nPummelWeaponIndex;

			if(m_pOpposingNPCDialog->m_pNPC->m_Class[0] == DND_CHARACTER_CLASS_MONK)
				nWeapon = m_pApp->m_nMonkWeaponIndex;
		}

		if(m_pInitiativeDialog != NULL)
		{
			m_pOpposingNPCDialog->m_szInitiativeAction = _T("attack missed");
			m_pInitiativeDialog->Refresh();
		}

		m_pApp->PlayWeaponSFX(nWeapon, 1);
	}
	
}


void DMPartyDialog::OnMissileHitButton2() 
{
	if(m_pBaseOpposingCharDlg != NULL)
	{
		m_pBaseOpposingCharDlg->m_pTargetBaseDlg = m_pBaseCharDlg;
	}

	if(m_pOpposingCharacterDialog != NULL)
	{
		m_pOpposingCharacterDialog->FireAmmo();

		int nWeapon = m_pOpposingCharacterDialog->m_pCharacter->m_SelectedWeapons[0].m_wTypeId;

		if(m_pInitiativeDialog != NULL)
		{
			m_pOpposingCharacterDialog->m_szInitiativeAction = _T("attack hit");
			m_pInitiativeDialog->Refresh();
		}

		m_pApp->PlayWeaponSFX(nWeapon, 2);

		m_pOpposingCharacterDialog->ProcessCharStats();
		m_pOpposingCharacterDialog->Refresh();

		OnWoundButton();
	}
	
	if(m_pOpposingNPCDialog != NULL)
	{
		m_pOpposingNPCDialog->FireAmmo();

		int nWeapon = m_pOpposingNPCDialog->m_pNPC->m_SelectedWeapons[0].m_wTypeId;

		if(m_pInitiativeDialog != NULL)
		{
			m_pOpposingNPCDialog->m_szInitiativeAction = _T("attack hit");
			m_pInitiativeDialog->Refresh();
		}

		m_pApp->PlayWeaponSFX(nWeapon, 2);

		m_pOpposingNPCDialog->ProcessCharStats();
		m_pOpposingNPCDialog->Refresh();

		OnWoundButton();
	}
	
}

void DMPartyDialog::OnMissileMissButton2() 
{
	if(m_pBaseOpposingCharDlg != NULL)
	{
		m_pBaseOpposingCharDlg->m_pTargetBaseDlg = m_pBaseCharDlg;
	}

	if(m_pOpposingCharacterDialog != NULL)
	{
		m_pOpposingCharacterDialog->FireAmmo();

		int nWeapon = m_pOpposingCharacterDialog->m_pCharacter->m_SelectedWeapons[0].m_wTypeId;

		if(m_pInitiativeDialog != NULL)
		{
			m_pOpposingCharacterDialog->m_szInitiativeAction = _T("attack missed");
			m_pInitiativeDialog->Refresh();
		}

		m_pApp->PlayWeaponSFX(nWeapon, 3);
			
		m_pOpposingCharacterDialog->ProcessCharStats();
		m_pOpposingCharacterDialog->Refresh();

		Refresh();
	}
	
	if(m_pOpposingNPCDialog != NULL)
	{
		m_pOpposingNPCDialog->FireAmmo();

		int nWeapon = m_pOpposingNPCDialog->m_pNPC->m_SelectedWeapons[0].m_wTypeId;

		if(m_pInitiativeDialog != NULL)
		{
			m_pOpposingNPCDialog->m_szInitiativeAction = _T("attack missed");
			m_pInitiativeDialog->Refresh();
		}

		m_pApp->PlayWeaponSFX(nWeapon, 3);
			
		m_pOpposingNPCDialog->ProcessCharStats();
		m_pOpposingNPCDialog->Refresh();

		Refresh();
	}
}


void DMPartyDialog::OnCastSpellButton2() 
{

	if(m_pOpposingCharacterDialog != NULL && m_pOpposingCharacterDialog->m_pCharacter != NULL)
	{
		DMCastSpellDialog *pDlg = new DMCastSpellDialog(this, m_pOpposingCharacterDialog, m_pOpposingCharacterDialog->m_pCharacter);
		pDlg->DoModal();
		delete pDlg;

		m_pOpposingCharacterDialog->Refresh();
	}
	
}

void DMPartyDialog::OnWoundButton2() 
{
	BOOL bWasAlive = FALSE;

	if(m_pOpposingCharacterDialog != NULL && m_pOpposingCharacterDialog->m_pCharacter != NULL)
	{
		bWasAlive = m_pOpposingCharacterDialog->m_pCharacter->IsAlive();
		
		int nValue = 0;

		ModifyValue((int *)&nValue, "Add Damage:", FALSE);

		m_pOpposingCharacterDialog->m_pCharacter->m_nCurrentDamage += nValue;

		if(m_pOpposingCharacterDialog->m_pCharacter->m_nCurrentDamage < 0)
			m_pOpposingCharacterDialog->m_pCharacter->m_nCurrentDamage = 0;

		m_pOpposingCharacterDialog->ProcessCharStats();

		m_pOpposingCharacterDialog->Refresh();

		if(bWasAlive && !m_pOpposingCharacterDialog->m_pCharacter->IsAlive())
		{
			OnBnClickedKillForXpButton();
			m_pOpposingCharacterDialog = NULL;
		}
	}
	
	if(m_pOpposingNPCDialog != NULL && m_pOpposingNPCDialog->m_pNPC != NULL)
	{

		bWasAlive = m_pOpposingNPCDialog->m_pNPC->IsAlive();
		
		int nValue = 0;

		ModifyValue((int *)&nValue, "Add Damage:", FALSE);

		m_pOpposingNPCDialog->m_pNPC->m_nCurrentDamage += nValue;

		if(m_pOpposingNPCDialog->m_pNPC->m_nCurrentDamage < 0)
			m_pOpposingNPCDialog->m_pNPC->m_nCurrentDamage = 0;

		m_pOpposingNPCDialog->ProcessCharStats();

		m_pOpposingNPCDialog->Refresh();

		if(bWasAlive && !m_pOpposingNPCDialog->m_pNPC->IsAlive())
		{
			OnBnClickedKillForXpButton();
			m_pOpposingNPCDialog = NULL;
		}
	}

	Refresh();
	
}

void DMPartyDialog::OnHealButton2() 
{
	if(m_pOpposingCharacterDialog != NULL && m_pOpposingCharacterDialog->m_pCharacter != NULL)
	{
		int nValue = 0;

		ModifyValue((int *)&nValue, "Heal Damage:", FALSE);

		m_pOpposingCharacterDialog->m_pCharacter->m_nCurrentDamage -= nValue;

		if(m_pOpposingCharacterDialog->m_pCharacter->m_nCurrentDamage < 0)
			m_pOpposingCharacterDialog->m_pCharacter->m_nCurrentDamage = 0;

		m_pOpposingCharacterDialog->ProcessCharStats();

		m_pOpposingCharacterDialog->Refresh();
	}
	
	if(m_pOpposingNPCDialog != NULL && m_pOpposingNPCDialog->m_pNPC != NULL)
	{
		int nValue = 0;

		ModifyValue((int *)&nValue, "Heal Damage:", FALSE);

		m_pOpposingNPCDialog->m_pNPC->m_nCurrentDamage -= nValue;

		if(m_pOpposingNPCDialog->m_pNPC->m_nCurrentDamage < 0)
			m_pOpposingNPCDialog->m_pNPC->m_nCurrentDamage = 0;

		m_pOpposingNPCDialog->ProcessCharStats();

		m_pOpposingNPCDialog->Refresh();
	}

	Refresh();
}


void DMPartyDialog::OnUpPartyButton2() 
{
	if(m_cPartyList2.GetItemCount())
	{
		UpdateData(FALSE);

		int nCursor = GetSelectedListCtrlItem(&m_cPartyList2);

		nCursor--;

		if(nCursor < 0)
			nCursor = 0;

		m_pBaseOpposingCharDlg = NULL;
		m_pOpposingCharacterDialog = NULL;
		m_pOpposingNPCDialog = NULL;

		m_pBaseOpposingCharDlg = (CDMBaseCharViewDialog *)m_cPartyList2.GetItemData(nCursor);

		if(m_pBaseOpposingCharDlg != NULL)
		{
			switch(m_pBaseOpposingCharDlg->GetType())
			{
				case DND_CHAR_VIEW_TYPE_PC:		m_pOpposingCharacterDialog =  (CDMCharViewDialog *)m_pBaseOpposingCharDlg; break;
				case DND_CHAR_VIEW_TYPE_NPC:	m_pOpposingNPCDialog =  (cDMBaseNPCViewDialog *)m_pBaseOpposingCharDlg; break;
			}
		}

		SetSelectedListCtrlItem(&m_cPartyList2, nCursor);

		Refresh();	
	}
	
}

void DMPartyDialog::OnDownPartyButton2() 
{
	if(m_cPartyList2.GetItemCount())
	{
		UpdateData(FALSE);

		int nCursor = GetSelectedListCtrlItem(&m_cPartyList2);

		nCursor++;

		if(nCursor >= m_cPartyList2.GetItemCount())
			nCursor = m_cPartyList2.GetItemCount()-1;

		m_pBaseOpposingCharDlg = NULL;
		m_pOpposingCharacterDialog = NULL;
		m_pOpposingNPCDialog = NULL;

		m_pBaseOpposingCharDlg = (CDMBaseCharViewDialog *)m_cPartyList2.GetItemData(nCursor);

		if(m_pBaseOpposingCharDlg != NULL)
		{
			switch(m_pBaseOpposingCharDlg->GetType())
			{
				case DND_CHAR_VIEW_TYPE_PC:		m_pOpposingCharacterDialog =  (CDMCharViewDialog *)m_pBaseOpposingCharDlg; break;
				case DND_CHAR_VIEW_TYPE_NPC:	m_pOpposingNPCDialog =  (cDMBaseNPCViewDialog *)m_pBaseOpposingCharDlg; break;
			}
		}

		SetSelectedListCtrlItem(&m_cPartyList2, nCursor);

		Refresh();
	}
	
}


void DMPartyDialog::OnCreateNpcButton() 
{
	// FIXME
	//if(m_pSubParty != NULL)
	//	return;

	m_pSubParty;

	cDNDNonPlayerCharacter *pNPC = m_pParty->GetOpenNPCSlot();
	
	if(pNPC != NULL)
	{
		memset(pNPC, 0, sizeof(cDNDNonPlayerCharacter));

		strcpy(pNPC->m_szCharacterName, "New NPC");

		pNPC->m_bIsNPC = TRUE;

		pNPC->m_dwSubPartyID = m_dwSubPartyID;

		cDMBaseNPCViewDialog *pDlg = new cDMBaseNPCViewDialog(m_pMainDialog, pNPC, NULL, &m_pMainDialog->m_cMainTab);
		
		m_pMainDialog->AddTab(pDlg, DND_TAB_TYPE_NPC, TRUE);

		AddPartyMember(pNPC->m_dwCharacterID, m_dwSubPartyID);

		m_pApp->m_NPCViewMap.SetAt((WORD)pNPC->m_dwCharacterID, pDlg); // this must be done AFTER adding the party for complicated reasons

		UpdateSelections();
		Refresh();

		Broadcast();
	}
	
}


void DMPartyDialog::OnDeleteNpcButton() 
{
	//we are not using this button
}

void DMPartyDialog::OnBnClickedHealPartyButton()
{
	for (int i = 0; i < m_pApp->m_TabArray.GetSize(); ++i)
	{
		cDNDDisplayTab *pTab = m_pApp->m_TabArray[i];

		if(pTab != NULL)
		{
			if(pTab->m_nTabType == DND_TAB_TYPE_NPC)
			{
				cDMBaseNPCViewDialog *pNPCDlg = (cDMBaseNPCViewDialog *)pTab->m_pWindow;
				
				if(m_pParty->CharacterIsPartyMember(pNPCDlg->m_pNPC))
				{
					// heal this NPC
					pNPCDlg->m_pNPC->m_nCurrentDamage = 0;
					pNPCDlg->Refresh();
					pNPCDlg->m_pNPC->MarkChanged();
				}

			}
			else if(pTab->m_nTabType == DND_TAB_TYPE_CHARACTER)
			{
				CDMCharViewDialog *pPCDlg = (CDMCharViewDialog *)pTab->m_pWindow;
				
				if(m_pParty->CharacterIsPartyMember(pPCDlg->m_pCharacter))
				{
					// heal this character
					pPCDlg->m_pCharacter->m_nCurrentDamage = 0;
					pPCDlg->Refresh();
					pPCDlg->m_pCharacter->MarkChanged();
				}

			}
		}
	}

	Refresh();
}

void DMPartyDialog::OnBnClickedCopyMemberButton()
{
	//FIXME !
	//if(m_pSubParty != NULL)
	//	return;

	if(m_pSelectedNPCDialog != NULL)
	{
		m_pApp->m_pBaseCopyCharacterDialog = m_pSelectedNPCDialog;
		m_pApp->m_bCharacterCopyBufferIsNPC = TRUE;
	}

	Refresh();
}

void DMPartyDialog::OnBnClickedPasteMemberButton()
{
	//FIXME !
	//if(m_pSubParty != NULL)
	//	return;

	static DWORD dwLastID = 0L;
	
	if(m_pApp->m_pBaseCopyCharacterDialog != NULL && m_pApp->m_bCharacterCopyBufferIsNPC == FALSE)
	{
		// paste a character into this party
	}
	else if(m_pApp->m_pBaseCopyCharacterDialog != NULL && m_pApp->m_bCharacterCopyBufferIsNPC == TRUE)
	{
		// paste an NPC into this party
		cDNDNonPlayerCharacter *pNPC = new cDNDNonPlayerCharacter();
		
		if(pNPC != NULL)
		{
			cDMBaseNPCViewDialog *pOriginalNPCDialog = (cDMBaseNPCViewDialog *)m_pApp->m_pBaseCopyCharacterDialog;

			memcpy(pNPC, pOriginalNPCDialog->m_pNPC, sizeof(cDNDNonPlayerCharacter));

			sprintf(pNPC->m_szCharacterName, "%s %d", pOriginalNPCDialog->m_pNPC->m_szCharacterName, ++pOriginalNPCDialog->m_nCharacterCopy);
			pNPC->m_dwCharacterID = GetUniqueID();

			cDMBaseNPCViewDialog *pNPCDlg = new cDMBaseNPCViewDialog(m_pMainDialog, pNPC, NULL, &m_pMainDialog->m_cMainTab);
			m_pMainDialog->AddTab(pNPCDlg, DND_TAB_TYPE_NPC, FALSE);
	
			pNPCDlg->GetRandomName(TRUE);
			pNPCDlg->OnRollStats();
			pNPCDlg->ProcessCharStats();
			pNPCDlg->Refresh();

			m_pApp->m_NPCViewMap.SetAt((WORD)pNPCDlg->m_pNPC->m_dwCharacterID, pNPCDlg);
			AddPartyMember(pNPC->m_dwCharacterID, m_dwSubPartyID);		

			delete pNPC;			
		}
	}
	else
	{
		m_pApp->m_pBaseCopyCharacterDialog = NULL;
		m_pApp->m_bCharacterCopyBufferIsNPC = FALSE;
	}

	Refresh();

	Broadcast();
}


void DMPartyDialog::OnBnClickedStatBlockButton()
{
	GenerateStatBlock(m_pParty);
}


void DMPartyDialog::OnBnClickedStatBlockButton2()
{
	if (m_pOpposingPartyDialog != NULL)
	{
		GenerateStatBlock(m_pOpposingPartyDialog->m_pParty);
	}
}


void DMPartyDialog::OnNMDblclkPartyList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	if(m_pSelectedCharacterDialog != NULL)
	{
		FindAndSelectCharacterTab(m_pSelectedCharacterDialog->m_pCharacter->m_dwCharacterID);
	}
	else if(m_pSelectedNPCDialog != NULL)
	{
		FindAndSelectCharacterTab(m_pSelectedNPCDialog->m_pNPC->m_dwCharacterID);
	}
		
	*pResult = 0;
}

void DMPartyDialog::OnNMDblclkPartyList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	if(m_pOpposingPartyDialog != NULL)
	{
		if(m_pOpposingCharacterDialog != NULL)
		{
			CDMCharViewDialog *pDlg = m_pOpposingCharacterDialog;  // it can go away
			FindAndSelectOpposingPartyTab();
			FindAndSelectCharacterTab(pDlg->m_pCharacter->m_dwCharacterID);
		}
		else if(m_pOpposingNPCDialog != NULL)
		{
			cDMBaseNPCViewDialog *pDlg = m_pOpposingNPCDialog; // it can go away
			FindAndSelectOpposingPartyTab();
			FindAndSelectCharacterTab(pDlg->m_pNPC->m_dwCharacterID);
		}
	}

	*pResult = 0;
}

void DMPartyDialog::OnBnClickedViewCalendarButton()
{
	if(m_pCalendarDialog == NULL)
	{
		m_pCalendarDialog = new CDMCalendarDialog(this);
	}
}

void DMPartyDialog::OnBnClickedViewLogButton()
{
	DMPartyDialog *pParentPartyDlg = this;

	if(m_dwParentPartyID)
	{
		//this is a subparty
		if(FALSE == m_pApp->m_PartyViewMap.Lookup((WORD)m_dwParentPartyID, pParentPartyDlg))
		{
			return;
		}
	}

	if(pParentPartyDlg->m_pPartyLogDialog == NULL)
	{
		pParentPartyDlg->m_pPartyLogDialog = new CDMPartyLogDialog(pParentPartyDlg);
	}
}

void DMPartyDialog::OnBnClickedLogEventButton()
{
	if(m_pEventLoggerDialog == NULL)
	{
		m_pEventLoggerDialog = new CDMEventLoggerDialog(this);
	}
	else
	{
		m_pEventLoggerDialog->m_nInsertPosition = APPEND_TO_LOG;
		m_pEventLoggerDialog->Init();
	}
}

void DMPartyDialog::LogPartyEvent(BOOL bCreateLog, int nPosition, DND_LOG_EVENT_TYPES nType, char *szName, DWORD dwID, LONG lAmount, char *szComment)
{

	/*
	if(m_dwParentPartyID)
	{
		//this is a subparty
		DMPartyDialog *pParentPartyDlg = NULL;
		if(m_pApp->m_PartyViewMap.Lookup((WORD)m_dwParentPartyID, pParentPartyDlg))
		{
			if(pParentPartyDlg != NULL)
			{
				pParentPartyDlg->LogPartyEvent(bCreateLog, nPosition, nType, szName, dwID, lAmount, szComment);
			}
		}

		return;
	}
	*/

	cDNDPartyLog *pPartyLog = NULL;

	if(m_pPartyLog == NULL)
	{
		if(bCreateLog)
		{
			m_pPartyLog = new cDNDPartyLog();
			pPartyLog = m_pPartyLog;
		}
		else
		{
			pPartyLog = new cDNDPartyLog();
		}
	}
	else
	{
		pPartyLog = m_pPartyLog;
	}

	int nMonth = m_pParty->m_nMonth;
	int nDay = m_pParty->m_nDayofMonth;
	int nYear = m_pParty->m_nYear;

	int nHour = m_pParty->m_nHour;
	int nTurn = m_pParty->m_nTurn;
	int nRound = m_pParty->m_nRound;
	int nSegment = m_pParty->m_nSegment;

	int nLogEvent = pPartyLog->m_LogHeader.m_nEvents;
	if(nPosition != APPEND_TO_LOG)
	{
		if(pPartyLog->m_Event[nPosition].m_nYear)
		{
			nMonth = pPartyLog->m_Event[nPosition].m_nMonth;
			nDay = pPartyLog->m_Event[nPosition].m_nDay;
			nYear = pPartyLog->m_Event[nPosition].m_nYear;

			nHour = pPartyLog->m_Event[nPosition].m_nHour;
			nTurn = pPartyLog->m_Event[nPosition].m_nTurn;
			nRound = pPartyLog->m_Event[nPosition].m_nRound;
			nSegment = pPartyLog->m_Event[nPosition].m_nSegment;
		}

		int nMoveEvents = MAX_LOG_EVENTS - nPosition -1; 
		memcpy(&pPartyLog->m_Event[nPosition+1], &pPartyLog->m_Event[nPosition], nMoveEvents*sizeof(cDNDPartyLogEvent));
		nLogEvent = nPosition;
	}

	cDNDPartyLogEvent *pEvent = &pPartyLog->m_Event[nLogEvent];

	pEvent->m_EventType = nType;

	time_t rawtime;
	time (&rawtime);
	pEvent->m_tEventID = rawtime; //GetUniversalTime();

	pEvent->m_nMonth = nMonth;
	pEvent->m_nDay = nDay;
	pEvent->m_nYear = nYear;

	pEvent->m_nHour = nHour;
	pEvent->m_nTurn = nTurn;
	pEvent->m_nRound = nRound;
	pEvent->m_nSegment = nSegment;

	int nLen = 0;

	nLen = min(strlen(szName), 63);
	if(nLen)
	{
		memcpy(pEvent->m_szName, szName, nLen*sizeof(char)); pEvent->m_szName[nLen] = 0;
	}
	else
	{
		pEvent->m_szName[0] = 0;
	}

	nLen = min(strlen(szComment), 127);
	if(nLen)
	{
		memcpy(pEvent->m_szComment, szComment, nLen*sizeof(char)); pEvent->m_szComment[nLen] = 0;
	}
	else
	{
		pEvent->m_szComment[0] = 0;
	}

	pEvent->m_lAmount = lAmount;
	pEvent->m_lAmountRemaining = lAmount;

	switch(nType)
	{
		case DND_LOG_EVENT_TYPE_CHARACTER_GAINED_XP: 
		case DND_LOG_EVENT_TYPE_CHARACTER_GAINED_XP_COMBAT: 
		case DND_LOG_EVENT_TYPE_CHARACTER_GAINED_XP_CLASS_SKILL: 
		case DND_LOG_EVENT_TYPE_CHARACTER_GAINED_XP_ROLEPLAY: 
		case DND_LOG_EVENT_TYPE_CHARACTER_GAINED_XP_TREASURE: 
		case DND_LOG_EVENT_TYPE_CHARACTER_LOST_XP: 
		{
			PDNDCHARVIEWDLG pCharDlg = NULL;
			m_pApp->m_CharacterViewMap.Lookup((WORD)dwID, pCharDlg);
			if(pCharDlg != NULL && pCharDlg->m_pCharacter != NULL)
			{
				if(nType == DND_LOG_EVENT_TYPE_CHARACTER_LOST_XP)
				{
					pCharDlg->m_pCharacter->m_lEarnedXP -= lAmount;
					if(pCharDlg->m_pCharacter->m_lEarnedXP < 0)
					{
						pCharDlg->m_pCharacter->m_lEarnedXP = 0L;
					}
				}
				else
				{
					pCharDlg->m_pCharacter->m_lEarnedXP += lAmount;
				}

				pCharDlg->m_pCharacter->MarkChanged();
				pCharDlg->ProcessCharStats();
				pCharDlg->Refresh();
			}

			break;
		}
		case DND_LOG_EVENT_TYPE_PARTY_GAINED_XP:
		case DND_LOG_EVENT_TYPE_PARTY_GAINED_XP_COMBAT:
		case DND_LOG_EVENT_TYPE_PARTY_GAINED_XP_TREASURE:
		case DND_LOG_EVENT_TYPE_PARTY_LOST_XP:
		{
			if(nType == DND_LOG_EVENT_TYPE_PARTY_LOST_XP)
			{
				m_pParty->m_lXP -= lAmount;
				
				if(m_pParty->m_lXP < 0L)
				{
					m_pParty->m_lXP = 0L;
				}
			}
			else
			{
				m_pParty->m_lXP += lAmount;
			}

			Refresh();

			m_pParty->MarkChanged();

			break;
		}
	}

	pPartyLog->m_LogHeader.m_nEvents += 1;
	
	m_pParty->MarkChanged();

	if(m_pPartyLogDialog != NULL)
	{
		m_pPartyLogDialog->InvalidateRect(NULL);
	}

	if(m_pPartyLog == NULL && pPartyLog != NULL)
	{
		delete pPartyLog;
		pPartyLog = NULL;
	}

	Refresh();
}

void DMPartyDialog::OnBnClickedAddXpButton()
{
	ModifyValue((int *)&m_pParty->m_lXP, "Modify Party XP", 0);

	if(m_pParty->m_lXP < 0L)
	{
		m_pParty->m_lXP = 0L;
	}

	Refresh();

	m_pParty->MarkChanged();
}

void DMPartyDialog::OnBnClickedGrantPartyXpButton()
{
	if(m_pParty->m_lXP == 0L)
	{
		return;
	}

	if(AfxMessageBox("Are you sure you wish to grant party experience now ?", MB_ICONQUESTION|MB_OKCANCEL) == IDCANCEL)
	{
		return;
	}

	//m_fXPScratchPad
	int nRow = 0;
	WORD wID;
	PDNDCHARVIEWDLG pCharDlg = NULL;

	for (POSITION pos = m_pApp->m_CharacterViewMap.GetStartPosition(); pos != NULL; )
	{
		pCharDlg = NULL;
	
		m_pApp->m_CharacterViewMap.GetNextAssoc(pos,wID,pCharDlg);

		if(pCharDlg != NULL && pCharDlg->m_pCharacter != NULL && pCharDlg->m_pCharacter->m_dwCharacterID)
		{
			pCharDlg->m_fXPScratchPad = 0.0f;
		}
	}


	float fAvailableXP = (float)m_pParty->m_lXP;
	do
	{
		for (pos = m_pApp->m_CharacterViewMap.GetStartPosition(); pos != NULL; )
		{
			pCharDlg = NULL;
		
			m_pApp->m_CharacterViewMap.GetNextAssoc(pos,wID,pCharDlg);

			if(pCharDlg != NULL && pCharDlg->m_pCharacter != NULL && pCharDlg->m_pCharacter->m_dwCharacterID)
			{
				if(m_pParty->CharacterIsPartyMember(pCharDlg->m_pCharacter))
				{
					float fAddXP = 1.0f;

					if(pCharDlg->m_pCharacter->m_bIsHenchman)
					{
						fAddXP = 0.5f;
					}

					pCharDlg->m_fXPScratchPad += fAddXP;
					
					fAvailableXP -= 1.0f;
				}
			}
		}

	} while(fAvailableXP > 0.0f);

	for (POSITION pos = m_pApp->m_CharacterViewMap.GetStartPosition(); pos != NULL; )
	{
		pCharDlg = NULL;
	
		m_pApp->m_CharacterViewMap.GetNextAssoc(pos,wID,pCharDlg);

		if(pCharDlg != NULL && pCharDlg->m_pCharacter != NULL && pCharDlg->m_pCharacter->m_dwCharacterID)
		{
			if(m_pParty->CharacterIsPartyMember(pCharDlg->m_pCharacter))
			{
				pCharDlg->m_pCharacter->m_lEarnedXP += (LONG)pCharDlg->m_fXPScratchPad;
				pCharDlg->m_fXPScratchPad = 0.0f;
				pCharDlg->m_pCharacter->MarkChanged();

				pCharDlg->ProcessCharStats();
				pCharDlg->Refresh();
			}
		}
	}

	m_pParty->m_lXP = 0L;
	m_pParty->MarkChanged();

	Refresh();
}


void DMPartyDialog::OnBnClickedKillForXpButton()
{
	LONG lXP = 0L;

	CString szKiller = _T("");

	//if((m_pSelectedCharacterDialog != NULL || m_pSelectedNPCDialog != NULL) && (m_pOpposingCharacterDialog != NULL || m_pOpposingNPCDialog != NULL))
	CDMBaseCharViewDialog *pKillerDlg = NULL;

	if(m_pSelectedCharacterDialog != NULL)
	{
		szKiller = m_pSelectedCharacterDialog->m_pCharacter->m_szCharacterName;
		pKillerDlg = m_pSelectedCharacterDialog;
	}
	else if(m_pSelectedNPCDialog != NULL)
	{
		szKiller.Format("%s (%s)", m_pSelectedNPCDialog->m_pNPC->m_szCharacterName, m_pSelectedNPCDialog->m_szMonsterName);
		pKillerDlg = m_pSelectedNPCDialog;
	}
	else
	{
		return;
	}

	pKillerDlg->m_szInitiativeAction.Format("killed %s", pKillerDlg->m_pTargetBaseDlg->m_szBaseCharName);
	pKillerDlg->m_pTargetBaseDlg = NULL;

	CString szVictim = _T("");

	if(m_pOpposingCharacterDialog != NULL)
	{
		szVictim.Format("%s killed %s", szKiller, m_pOpposingCharacterDialog->m_pCharacter->m_szCharacterName);
		lXP = (LONG)m_pOpposingCharacterDialog->m_nCalculatedXPValue;

		if(m_pOpposingCharacterDialog->m_pCharacter->m_nCurrentDamage < m_pOpposingCharacterDialog->m_pCharacter->m_nHitPoints)
		{
			m_pOpposingCharacterDialog->m_pCharacter->m_nCurrentDamage = m_pOpposingCharacterDialog->m_pCharacter->m_nHitPoints;
		}
	}
	else if(m_pOpposingNPCDialog != NULL)
	{
		szVictim.Format("%s killed %s (%s)", szKiller, m_pOpposingNPCDialog->m_pNPC->m_szCharacterName, m_pOpposingNPCDialog->m_szMonsterName);
		lXP = (LONG)m_pOpposingNPCDialog->m_pNPC->m_nNPCXPValue;

		if(m_pOpposingNPCDialog->m_pNPC->m_nCurrentDamage < m_pOpposingNPCDialog->m_pNPC->m_nHitPoints)
		{
			m_pOpposingNPCDialog->m_pNPC->m_nCurrentDamage = m_pOpposingNPCDialog->m_pNPC->m_nHitPoints;
		}
	}
	else
	{
		return;
	}

	if(lXP == 0L)
	{
		ModifyValue((int *)&lXP, "XP for Killing This Opponent ?", 0);

		if(m_pParty->m_lXP < 0L)
		{
			m_pParty->m_lXP = 0L;
		}
	}
	
	LogPartyEvent(FALSE, APPEND_TO_LOG, DND_LOG_EVENT_TYPE_PARTY_GAINED_XP_COMBAT, m_pParty->m_szPartyName, m_pParty->m_dwPartyID, lXP, szVictim.GetBuffer(0));

}

void DMPartyDialog::OnBnClickedSwapWeaponButton1()
{
	if(m_pSelectedCharacterDialog != NULL)
	{
		m_pSelectedCharacterDialog->SwapWeapon();
		Refresh();
	}
	else if(m_pSelectedNPCDialog != NULL)
	{
		m_pSelectedNPCDialog->SwapWeapon();
		Refresh();
	}
}

void DMPartyDialog::OnBnClickedSwapWeaponButton2()
{
	if(m_pOpposingCharacterDialog != NULL)
	{
		m_pOpposingCharacterDialog->SwapWeapon();
		Refresh();
	}
	else if(m_pOpposingNPCDialog != NULL)
	{
		m_pOpposingNPCDialog->SwapWeapon();
		Refresh();
	}
}

void DMPartyDialog::OnBnClickedPartyInitiativeButton()
{
	if(m_pInitiativeDialog == NULL)
	{
		m_pInitiativeDialog = new CDMInitiativeDialog(this);
	}
}

cDNDSubParty * DMPartyDialog::ExternalSubPartyCommand(char *szSubPartyName, DWORD dwSubPartyID)
{
	if(m_dwSubPartyID == 0)
	{
		//Find a subparty slot
		cDNDSubParty *pSubParty = NULL;

		for(int i = 0; i < MAX_SUB_PARTIES; ++i)
		{
			if(m_pParty->m_SubParty[i].m_dwSubPartyID == 0)
			{
				pSubParty = &m_pParty->m_SubParty[i];
				break;
			}
		}
		
		if(pSubParty != NULL)
		{
			/*
			DMPartyDialog *pDlg = new DMPartyDialog(m_pMainDialog, m_pParty, &m_pMainDialog->m_cMainTab);

			ASSERT(pDlg->m_hWnd != NULL);

			ASSERT(pDlg->m_hWnd != (HWND)0xfeeefeee);

			pDlg->m_dwParentPartyID = m_pParty->m_dwPartyID;

			if(dwSubPartyID == 0)
			{
				pDlg->m_dwSubPartyID = GetUniqueID();
			}
			else
			{
				pDlg->m_dwSubPartyID = dwSubPartyID;
			}
			
			if(szSubPartyName != NULL)
			{
				strcpy(pSubParty->m_szSubPartyName, szSubPartyName);
			}
			else
			{
				strcpy(pSubParty->m_szSubPartyName, "New SubParty");
			}

			pSubParty->m_dwSubPartyID = pDlg->m_dwSubPartyID;


			pDlg->m_pSubParty = pSubParty;
			pDlg->m_szPartyMapName = m_szPartyMapName;
			*/

			if(szSubPartyName != NULL)
			{
				strcpy(pSubParty->m_szSubPartyName, szSubPartyName);
			}
			else
			{
				strcpy(pSubParty->m_szSubPartyName, "New SubParty");
			}

			if(dwSubPartyID)
			{
				pSubParty->m_dwSubPartyID = dwSubPartyID;
			}
			else
			{
				pSubParty->m_dwSubPartyID = GetUniqueID();
			}

			CDMSubPartyDialog *pSubPartyDialog = new CDMSubPartyDialog(pSubParty, this);

			m_pMainDialog->AddTab(pSubPartyDialog, DND_TAB_TYPE_SUBPARTY, FALSE);


			return pSubParty;
		}
	}
	else //delete the subparty
	{
		for(int i = 0; i < MAX_SUB_PARTIES; ++i)
		{
			if(m_pParty->m_SubParty[i].m_dwSubPartyID == m_dwSubPartyID)
			{
				if(m_pParty->m_SubParty[i].m_nSubPartyType == DND_SUBPARTY_TYPE_DUNGEON)
				{
					if(AfxMessageBox("Delete NPCs/Monsters that are members of this subparty ?", MB_YESNO) == IDYES)
					{
						for(int j = 0; j < MAX_NPC_PARTY_MEMBERS; ++j)
						{
							if(m_pParty->m_NPCs[j].m_dwSubPartyID == m_dwSubPartyID)
							{
								if(m_pMainDialog->FindAndDeleteTab(DND_TAB_TYPE_NPC, m_pParty->m_NPCs[j].m_dwCharacterID, FALSE))
								{
									m_pParty->RemovePartyMemberID(m_pParty->m_NPCs[j].m_dwCharacterID);
									memset(&m_pParty->m_NPCs[j], 0, sizeof(cDNDNonPlayerCharacter));

									m_pApp->m_NPCViewMap.SetAt((WORD) m_pParty->m_NPCs[j].m_dwCharacterID, NULL);
									m_pApp->m_NPCViewMap.RemoveKey((WORD) m_pParty->m_NPCs[j].m_dwCharacterID);
								}
							}
						}

						m_pApp->ValidateNPCViewMap();


						// remove NPC characters
						for(int j = 0; j < MAX_PARTY_MEMBERS; ++j)
						{
							if(m_pParty->m_dwPartyRoster[j] != 0)
							{
								PDNDCHARVIEWDLG pCharDlg = NULL;
								m_pApp->m_CharacterViewMap.Lookup((WORD)m_pParty->m_dwPartyRoster[j], pCharDlg);
								if(pCharDlg != NULL && pCharDlg->m_pCharacter != NULL && pCharDlg->m_pCharacter->m_dwSubPartyID == m_dwSubPartyID)
								{
									if(m_pMainDialog->FindAndDeleteTab(DND_TAB_TYPE_CHARACTER, m_pParty->m_dwPartyRoster[j], FALSE))
									{
										m_pApp->m_CharacterViewMap.SetAt((WORD) m_pParty->m_dwPartyRoster[j], NULL);
										m_pApp->m_CharacterViewMap.RemoveKey((WORD) m_pParty->m_dwPartyRoster[j]);

										m_pParty->m_dwPartyRoster[j] = 0;
									}
								}
							}
						}
	
						m_pParty->ValidatePartyRoster();
						m_pParty->ValidateNPCList();
					}
					
				}

				m_pParty->m_SubParty[i].m_dwSubPartyID = 0;
				m_pParty->ValidateSubParties();

				m_pMainDialog->FindAndDeleteTab(DND_TAB_TYPE_SUBPARTY, m_dwSubPartyID);

				//m_pApp->m_PartyViewMap.SetAt((WORD)m_dwSubPartyID, NULL);
				//m_pApp->m_PartyViewMap.RemoveKey((WORD)m_dwSubPartyID);

				m_pMainDialog->FindAndSelectTab(DND_TAB_TYPE_PARTY, m_pParty->m_dwPartyID);

				break;
			}
		}
	}

	return NULL;

}

void DMPartyDialog::OnBnClickedSubPartyButton()
{
	ExternalSubPartyCommand(NULL, 0);	
}

void DMPartyDialog::OnBnClickedRandomEncounterButton()
{
	CDMRandomEncounterDialog *pDlg = new CDMRandomEncounterDialog(m_pParty);

	pDlg->DoModal();
}

LRESULT DMPartyDialog::OnDNDMessage(UINT wParam, LONG lParam)
{
    
	RefreshTime();
	
	UpdateData(FALSE);

	RECT Rect;
	m_cPartyLegend.GetWindowRect(&Rect);
	InvalidateRect(&Rect, TRUE);

	m_cSegmentEdit.GetWindowRect(&Rect);
	InvalidateRect(&Rect, TRUE);

	m_cRoundEdit.GetWindowRect(&Rect);
	InvalidateRect(&Rect, TRUE);

    return 0; // I handled this message
}

void DMPartyDialog::GenerateStatBlock(cDNDParty *pParty)
{
	if (pParty == NULL)
	{
		return;
	}

	CString szFileName = "";
	szFileName.Format("%sData/statblock.txt", m_pApp->m_szEXEPath.GetBuffer(0));
	FILE *pOutFile = fopen(szFileName, "wt");

	if (pOutFile == NULL)
	{
		return;
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//DO PCS

	/*
	Gerallt Lechen: AC 4; MV 9"; Lvl Druid 5; hp 32; #AT 1; Dmg scimitar (1-8); S 15; I 13; W 15; D 12; C 11; Ch 16; AL N; SIZE M; XP 353; PH p.20.
	*/

	for (int i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		if (pParty->m_dwPartyRoster[i] == 0)
			break;

		PDNDCHARACTER pChar = NULL;

		PDNDCHARVIEWDLG pCharDlg = NULL;
		PDNDNPCVIEWDLG pNPCDlg = NULL;

		m_pApp->m_CharacterViewMap.Lookup((WORD)pParty->m_dwPartyRoster[i], pCharDlg);

		if (pCharDlg != NULL && pCharDlg->m_pCharacter != NULL)
		{
			if (m_dwSubPartyID && pCharDlg->m_pCharacter->m_dwSubPartyID != m_dwSubPartyID)
				continue;

			pChar = pCharDlg->m_pCharacter;
		}

		if (pChar == NULL)
			continue;

		CString szLine = "<NAME>, <RACE>: AC <AC>; MV <MOVE>\"; Lvl <CLASS> <LEVEL>; hp <HP>; #AT <NUMATTACKS>; Dmg <DAMAGE>; <STATS>; AL <ALIGN>; SIZE <SIZE>; XP <XP>; <PAGE>";
		CString szTemp;

		szLine.Replace("<NAME>", pChar->m_szCharacterName);
		szLine.Replace("<RACE>", pCharDlg->m_szRaceSex);

		szTemp = pCharDlg->m_szArmorClass;
		szTemp.Replace(" ", "");
		szLine.Replace("<AC>", szTemp);

		szLine.Replace("<MOVE>", pCharDlg->m_szMove);

		szLine.Replace("<CLASS>", pCharDlg->m_szClassList);
		szLine.Replace("<LEVEL>", pCharDlg->m_szLevelList);
		
		szLine.Replace("<HP>", GetStringFromInt(pChar->m_nHitPoints));

		szLine.Replace("<NUMATTACKS>", pCharDlg->m_szNumAttacks);

		szLine.Replace("<DAMAGE>", pCharDlg->m_szDamageStat);

		szLine.Replace("<STATS>", pCharDlg->m_szCharStats);

		szLine.Replace("<ALIGN>", GetShortAlignmentName(pChar->m_nAlignment));

		szLine.Replace("<SIZE>", GetCharacterSize(pChar->m_nHeight));

		szLine.Replace("<XP>", pCharDlg->m_szXPValue);

		szLine.Replace("<PAGE>", GetCharacterBook(pChar->m_Class[0]));

		fprintf(pOutFile, "%s\n", szLine);

		// write out memorized spells
		BOOL bComplete = FALSE;
		for (int nSpellClass = 0; nSpellClass < 4; ++nSpellClass)
		{
			if (pChar->m_SpellClasses[nSpellClass] == 0)
				continue;

			cDNDSpellBook *pSpellBook = NULL;
			switch (pChar->m_SpellClasses[nSpellClass])
			{
			case DND_CHARACTER_CLASS_CLERIC:
			case DND_CHARACTER_SPELL_CLASS_PALADIN_CLERIC:
			{
				pSpellBook = m_pApp->m_SpellBooks.GetAt(DND_CHARACTER_CLASS_CLERIC);
				break;
			}
			case DND_CHARACTER_CLASS_DRUID:
			case DND_CHARACTER_SPELL_CLASS_RANGER_DRUID:
			{
				pSpellBook = m_pApp->m_SpellBooks.GetAt(DND_CHARACTER_CLASS_DRUID);
				break;
			}
			case DND_CHARACTER_CLASS_MAGE:
			case DND_CHARACTER_SPELL_CLASS_RANGER_MAGE:
			{
				pSpellBook = m_pApp->m_SpellBooks.GetAt(DND_CHARACTER_CLASS_MAGE);
				break;
			}
			case DND_CHARACTER_CLASS_ILLUSIONIST:
			{
				pSpellBook = m_pApp->m_SpellBooks.GetAt(DND_CHARACTER_CLASS_ILLUSIONIST);
				break;
			}
			} //end switch

			if (pSpellBook == NULL)
				continue;

			for (int nSpellLevel = 0; nSpellLevel < 10 && !bComplete; ++nSpellLevel)
			{
				BOOL bDone = FALSE;
				int nSpellCount = 0;
				BOOL bFirstSpellThisLevel = TRUE;

				while (!bDone)
				{
					cDNDSpell *pSpell = NULL;
					int nMultipleSpells = 0;

					do
					{
						nMultipleSpells = pChar->m_nSpellsMemorized[nSpellClass][nSpellLevel][nSpellCount];
						if (nMultipleSpells > 0)
						{
							pSpell = &pSpellBook->m_Spells[nSpellLevel][nSpellCount];
							if (pSpell && !pSpell->m_bSpellValid)
							{
								pChar->m_nSpellsMemorized[nSpellClass][nSpellLevel][nSpellCount] = 0;
								pSpell = NULL;
							}
						}

						++nSpellCount;

					} while (pSpell == NULL && nSpellCount < MAX_SPELLS_PER_LEVEL && bDone == FALSE && bComplete == FALSE);

					if (bDone == FALSE && pSpell != NULL && pSpell->m_bSpellValid && bComplete == FALSE)
					{
						if (nMultipleSpells > 1)
						{
							szTemp.Format("%s (x%d)", pSpell->m_szSpellName, nMultipleSpells);
						}
						else
						{
							szTemp.Format("%s", pSpell->m_szSpellName);
						}

						if (bFirstSpellThisLevel)
						{
							fprintf(pOutFile, "\n%s Lvl %s spells:\n", GetNumberth(nSpellLevel), GetClassName(pSpellBook->m_ClassBook));
							bFirstSpellThisLevel = FALSE;
						}

						szTemp.MakeLower();
						fprintf(pOutFile, " %s\n", szTemp);
					}
					else
					{
						bDone = TRUE;
					}
				};

			} //for(int nSpellLevel = 0; nSpellLevel < 10; ++nSpellLevel)

		} // for(int nSpellClass = 0; nSpellClass < 10; ++nSpellClass)
		

		fprintf(pOutFile, "\n-------------------------------------------------------------------------------\n");

	}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//DO NPCS 

	/*
	Kobold (12): AC 7; MV 6"; HD 1-4 hit points; hp 4(x8), 2(x2), 1(x2); #AT 1; Dmg 1-4 or by weapon; AL LE; SIZE S; XP 98; MM p.57.
	Weasel, Giant: AC 6; MV 15"; HD 3+3; hp 12; #AT 1; Dmg 2-12; SA successful strike drains blood for 2-12; AL N; SIZE M; XP 173; MM p.100.
	*/

	PDNDSTATBLOCKMAP _StatBlockMap;

	int nNPC = 0;

	do
	{
		if (pParty->m_NPCs[nNPC].m_dwCharacterID)
		{
			CString szNPCType = "";

			PDNDMONSTERMANUALENTRY pMonster = NULL;
			m_pApp->m_MonsterManualIndexedMap.Lookup(pParty->m_NPCs[nNPC].m_nMonsterIndex, pMonster);

			if (pMonster != NULL)
			{
				szNPCType = pMonster->m_szMMName;
			}

			cDNDStatBlock *pBlock = NULL;

			if (!_StatBlockMap.Lookup(szNPCType, pBlock) || pBlock == NULL)
			{
				pBlock = new cDNDStatBlock();
				pBlock->m_szName = szNPCType;

				_StatBlockMap.SetAt(szNPCType, pBlock);
			}

			TRACE("%s\n", pParty->m_NPCs[nNPC].m_szCharacterName);

			if (pMonster != NULL)
			{
				pBlock->m_szAC.Format("%d", pParty->m_NPCs[nNPC].m_nCurrentArmorClass);
				pBlock->m_szMove = pMonster->m_szMovement;
				pBlock->m_szHD = pMonster->m_szHD;

				pBlock->AddHitPointsCount(pParty->m_NPCs[nNPC].m_nHitPoints);

				pBlock->m_szAttacks = pMonster->m_szNumAttacks;
				pBlock->m_szDamage = pMonster->m_szDamage;
				pBlock->m_szAlignment = GetShortAlignmentName(pMonster->m_nAlignment);
				pBlock->m_szSize = pMonster->m_szSize;
				pBlock->m_nXP += GetXPFromMonsterManual(pMonster, pParty->m_NPCs[nNPC].m_nHitDice, pParty->m_NPCs[nNPC].m_nHitDicePlus, pParty->m_NPCs[nNPC].m_nHitPoints);

				pBlock->m_szSpecialAttack = pMonster->m_szSpecialAttack;
				pBlock->m_szSpecialDefense = pMonster->m_szSpecialDefense;

				pBlock->m_szPage = pMonster->m_szBook;

				++pBlock->m_nNumber;
			}
		}
		++nNPC;

	} while (nNPC < MAX_NPC_PARTY_MEMBERS);

		
	for (POSITION pos = _StatBlockMap.GetStartPosition(); pos != NULL;)
	{
		CString szJunk = "";
		cDNDStatBlock *pBlock = NULL;
		_StatBlockMap.GetNextAssoc(pos, szJunk, pBlock);
		//Weasel, Giant: AC 6; MV 15"; HD 3+3; hp 12; #AT 1; Dmg 2-12; SA successful strike drains blood for 2-12; AL N; SIZE M; XP 173; MM p.100.
		CString szLine = "<NAME><NUMBER>: AC <AC>; MV <MOVE>\"; HD <HD>; hp <HP>; #AT <NUMATTACKS>; Dmg <DAMAGE>; <SPECIAL_ATTACK><SPECIAL_DEFENSE>AL <ALIGN>; SIZE <SIZE>; XP <XP>; <PAGE>";

		szLine.Replace("<NAME>", pBlock->m_szName);

		CString szTemp = "";
		if (pBlock->m_nNumber > 1)
		{
			szTemp.Format(" (%d)", pBlock->m_nNumber);
		}

		szLine.Replace("<NUMBER>", szTemp);

		szLine.Replace("<AC>", pBlock->m_szAC);
		szLine.Replace("<MOVE>", pBlock->m_szMove);
		szLine.Replace("<HD>", pBlock->m_szHD);

		if (pBlock->m_nNumber == 1)
		{
			szTemp.Format("%d", pBlock->m_HitPointsValue[0]);
		}
		else
		{
			pBlock->SortHPCount();

			szTemp = "";

			CString szHP = "";
			for (int i = 0; i < pBlock->m_nNumber; ++i)
			{
				if (pBlock->m_HitPointsValue[i])
				{
					if (i == 0)
					{
						if (pBlock->m_HitPointsCount[i] == 1)
						{
							szHP.Format("%d", pBlock->m_HitPointsValue[i]);
						}
						else
						{
							szHP.Format("%d(x%d)", pBlock->m_HitPointsValue[i], pBlock->m_HitPointsCount[i]);	
						}
					}
					else
					{
						if (pBlock->m_HitPointsCount[i] == 1)
						{
							szHP.Format(", %d", pBlock->m_HitPointsValue[i]);
						}
						else
						{
							szHP.Format(", %d(x%d)", pBlock->m_HitPointsValue[i], pBlock->m_HitPointsCount[i]);
						}
					}

					szTemp += szHP;
				}
			}
					
		}

		szLine.Replace("<HP>", szTemp);

		szLine.Replace("<NUMATTACKS>", pBlock->m_szAttacks);
		szLine.Replace("<DAMAGE>", pBlock->m_szDamage);

		szLine.Replace("<ALIGN>", pBlock->m_szAlignment);
		szLine.Replace("<SIZE>", pBlock->m_szSize);

		szTemp.Format("%d", pBlock->m_nXP);
		szLine.Replace("<XP>", szTemp);

		if (pBlock->m_szSpecialAttack != "" && pBlock->m_szSpecialAttack != "Nil")
		{
			szTemp.Format("SA %s; ", pBlock->m_szSpecialAttack);
		}
		else
		{
			szTemp = "";
		}
		szLine.Replace("<SPECIAL_ATTACK>", szTemp);

		if (pBlock->m_szSpecialDefense != "" && pBlock->m_szSpecialDefense != "Nil")
		{
			szTemp.Format("SD %s; ", pBlock->m_szSpecialDefense);
		}
		else
		{
			szTemp = "";
		}
		szLine.Replace("<SPECIAL_DEFENSE>", szTemp);

		szLine.Replace("<PAGE>", pBlock->m_szPage);

		fprintf(pOutFile, "%s\n", szLine);

		fprintf(pOutFile, "\n-------------------------------------------------------------------------------\n");

		delete pBlock;
	}

		

	_StatBlockMap.RemoveAll();

	

	fclose(pOutFile);

	CString szExecute = "notepad.exe ";
	szExecute += szFileName;

	WinExec(szExecute, SW_SHOW);

}



