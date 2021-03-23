// DMInitiativeDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMInventoryDialog.h"
#include "DMCharSpellsDialog.h"
#include "DMCharViewDialog.h"
#include "DMNPCViewDialog.h"
#include "DMPartyDialog.h"
#include "DMInitiativeDialog.h"


// CDMInitiativeDialog dialog

IMPLEMENT_DYNAMIC(CDMInitiativeDialog, CDialog)

#define START_TURN_TIMER	21

CDMInitiativeDialog::CDMInitiativeDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDMInitiativeDialog::IDD, pParent)
	, m_bRollPCs(FALSE)
	, m_szDebugText(_T(""))
	, m_szAttacksText(_T("ATTACK #X of X THIS ROUND"))
	, m_szCombatNote(_T(""))
	, m_szTurnTimer(_T("TIME : 00s"))
{
	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_pParentPartyDialog = (DMPartyDialog *)pParent;

	m_nOldCursor = -1;

	m_nMaxRoundTime = -1;
	m_nNumAttacksThisRound = 0;
	m_nCompletedAttacksThisRound = 0;
	m_nTurnSecondsRemaining = 0;

	m_pLastAttackCharViewDlg = NULL;

	m_bRollPCs = m_pApp->m_bRollPCsInitiative;

	Create(CDMInitiativeDialog::IDD);
}

CDMInitiativeDialog::~CDMInitiativeDialog()
{
}

void CDMInitiativeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHARACTER_LIST, m_cCharacterList);
	DDX_Control(pDX, IDC_UP_LIST_BUTTON, m_cUpListButton);
	DDX_Control(pDX, IDC_DOWN_LIST_BUTTON, m_cDownListButton);
	DDX_Check(pDX, IDC_ROLL_PCS_CHECK, m_bRollPCs);
	DDX_Control(pDX, IDC_ROLL_INIT_BUTTON, m_cRollInitButton);
	DDX_Control(pDX, IDC_ROLL_TARGETS_BUTTON, m_cRollTargetsButton);
	DDX_Control(pDX, IDC_NEXT_ROUND_BUTTON, m_cNextRoundButton);
	DDX_Control(pDX, IDC_ROLL_PCS_CHECK, m_cRollPCsCheck);
	DDX_Control(pDX, IDC_DEBUG_TEXT, m_cDebugText);
	DDX_Text(pDX, IDC_DEBUG_TEXT, m_szDebugText);
	DDX_Text(pDX, IDC_ATTACKS_TEXT, m_szAttacksText);
	DDX_Control(pDX, IDC_SPELL_BUTTON, m_cSpellButton);
	DDX_Control(pDX, IDC_MUSIC_BUTTON, m_cMusicButton);
	DDX_Text(pDX, IDC_COMBAT_NOTE, m_szCombatNote);
	DDX_Control(pDX, IDC_COMBAT_NOTE, m_cCombatNote);
	DDX_Control(pDX, IDC_WEAPON_SWAP_BUTTON, m_cWeaponSwapButton);
	DDX_Control(pDX, IDC_HIT_BUTTON, m_cAttackButton);
	DDX_Control(pDX, IDC_MISS_BUTTON, m_cMissButton);
	DDX_Control(pDX, IDC_MOVE_BUTTON, m_cMoveButton);
	DDX_Control(pDX, IDC_SKIP_BUTTON, m_cSkipButton);
	DDX_Text(pDX, IDC_TIMER_NOTE, m_szTurnTimer);
	DDX_Control(pDX, IDC_TIMER_NOTE, m_cTurnTimerNote);
}


BEGIN_MESSAGE_MAP(CDMInitiativeDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CDMInitiativeDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDMInitiativeDialog::OnBnClickedCancel)
	ON_WM_CLOSE()
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_ROLL_INIT_BUTTON, &CDMInitiativeDialog::OnBnClickedRollInitButton)
	ON_NOTIFY(LVN_KEYDOWN, IDC_CHARACTER_LIST, &CDMInitiativeDialog::OnLvnKeydownCharacterList)
	ON_NOTIFY(NM_CLICK, IDC_CHARACTER_LIST, OnClickCharacterList)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_CHARACTER_LIST, &CDMInitiativeDialog::OnLvnItemActivateCharacterList)
	ON_NOTIFY(LVN_ODSTATECHANGED, IDC_CHARACTER_LIST, &CDMInitiativeDialog::OnLvnOdstatechangedCharacterList)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_CHARACTER_LIST, &CDMInitiativeDialog::OnLvnItemchangedCharacterList)
	ON_BN_CLICKED(IDC_NEXT_ROUND_BUTTON, &CDMInitiativeDialog::OnBnClickedNextRoundButton)
	ON_BN_CLICKED(IDC_UP_LIST_BUTTON, &CDMInitiativeDialog::OnBnClickedUpListButton)
	ON_BN_CLICKED(IDC_DOWN_LIST_BUTTON, &CDMInitiativeDialog::OnBnClickedDownListButton)
	ON_BN_CLICKED(IDC_ROLL_PCS_CHECK, &CDMInitiativeDialog::OnBnClickedRollPcsCheck)
	ON_BN_CLICKED(IDC_ROLL_TARGETS_BUTTON, &CDMInitiativeDialog::OnBnClickedRollTargetsButton)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_HIT_BUTTON, &CDMInitiativeDialog::OnBnClickedHitButton)
	ON_BN_CLICKED(IDC_MISS_BUTTON, &CDMInitiativeDialog::OnBnClickedMissButton)
	ON_BN_CLICKED(IDC_SPELL_BUTTON, &CDMInitiativeDialog::OnBnClickedSpellButton)
	ON_BN_CLICKED(IDC_WEAPON_SWAP_BUTTON, &CDMInitiativeDialog::OnBnClickedWeaponSwapButton)
	ON_BN_CLICKED(IDC_SKIP_BUTTON, &CDMInitiativeDialog::OnBnClickedSkipButton)
	ON_BN_CLICKED(IDC_MOVE_BUTTON, &CDMInitiativeDialog::OnBnClickedMoveButton)
	ON_BN_CLICKED(IDC_MUSIC_BUTTON, &CDMInitiativeDialog::OnBnClickedMusicButton)
	ON_WM_TIMER()
	ON_STN_DBLCLK(IDC_TIMER_NOTE, &CDMInitiativeDialog::OnStnDblclickTimerNote)
	ON_MESSAGE(DND_DIRTY_WINDOW_MESSAGE, &CDMInitiativeDialog::OnDirtyWindow)
END_MESSAGE_MAP()


// CDMInitiativeDialog message handlers

BOOL CDMInitiativeDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cUpListButton.LoadBitmaps(IDB_UP_ARROW_BITMAP, IDB_UP_ARROW_PRESSED_BITMAP);
	m_cDownListButton.LoadBitmaps(IDB_DOWN_ARROW_BITMAP, IDB_DOWN_ARROW_PRESSED_BITMAP);

	#if GAMETABLE_BUILD
	m_cMusicButton.LoadBitmaps(IDB_MUSIC_NOTE_BITMAP, IDB_MUSIC_NOTE_ALERT_BITMAP);
	m_cMusicButton.ShowWindow(SW_SHOW);
	#endif

	int nCount = 0;
	m_cCharacterList.m_nRowStateColumn = 8;
	m_cCharacterList.InsertColumn( nCount++, "Character", LVCFMT_LEFT, 150, -1 );
	m_cCharacterList.InsertColumn( nCount++, "Roll", LVCFMT_LEFT, 50, -1 );
	m_cCharacterList.InsertColumn( nCount++, "Seg/Rnd", LVCFMT_LEFT, 70, -1 );
	m_cCharacterList.InsertColumn( nCount++, "Action", LVCFMT_LEFT, 170, -1 );
	m_cCharacterList.InsertColumn( nCount++, "To Hit", LVCFMT_LEFT, 60, -1);
	m_cCharacterList.InsertColumn( nCount++, "Damage", LVCFMT_LEFT, 140, -1);
	m_cCharacterList.InsertColumn(nCount++, "Weapon", LVCFMT_LEFT, 140, -1);
	m_cCharacterList.InsertColumn(nCount++, "Move", LVCFMT_LEFT, 60, -1);
	m_cCharacterList.InsertColumn(nCount++, "HP", LVCFMT_LEFT, 60, -1);

	m_pParentPartyDialog->m_pInitiativeDialog = this;

	OnBnClickedRollInitButton();

#ifdef _DEBUG
	m_cDebugText.ShowWindow(SW_SHOW);
#endif

	SetTimer(8088, 1000, NULL);

	ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDMInitiativeDialog::Refresh()
{

	m_PlayersViewMap.RemoveAll();
	m_MonstersViewMap.RemoveAll();

	int nCursor = GetSelectedListCtrlItem(&m_cCharacterList);
	m_cCharacterList.m_nSelectedRow = nCursor;

	int nRow = 0;
	BOOL bRemove = m_cCharacterList.DeleteAllItems();
	m_cCharacterList.m_RowState.clear();

	CString szTemp;
	WORD wID;

	int nPlayerViewCount = 0;
	int nMonsterViewCount = 0;

	//for(int nSegment = 20; nSegment >= -10; --nSegment)
	for (int nSegment = -10; nSegment <= 20; ++nSegment)
	{
		for (POSITION pos = m_pApp->m_CharacterViewMap.GetStartPosition(); pos != NULL; )
		{
			PDNDCHARVIEWDLG pCharDlg = NULL;
			m_pApp->m_CharacterViewMap.GetNextAssoc(pos,wID,pCharDlg);

			if(pCharDlg != NULL && pCharDlg->m_pCharacter != NULL && pCharDlg->m_pCharacter->m_dwCharacterID)
			{
				BOOL bShow = FALSE;
				BOOL bMonsters = FALSE;

				if(m_pParentPartyDialog->m_dwSubPartyID == 0)
				{
					if(m_pParentPartyDialog->m_pParty->CharacterIsPartyMember(pCharDlg->m_pCharacter))
					{
						bShow = TRUE;
					}
				}
				else
				{
					if(m_pParentPartyDialog->m_dwSubPartyID == pCharDlg->m_pCharacter->m_dwSubPartyID)
					{
						bShow = TRUE;
					}
				}

				if(m_pParentPartyDialog->m_pOpposingPartyDialog != NULL && m_pParentPartyDialog->m_pOpposingPartyDialog->m_pParty != NULL)
				{
					if(m_pParentPartyDialog->m_pOpposingSubParty == NULL)
					{
						if(m_pParentPartyDialog->m_pOpposingPartyDialog->m_pParty->CharacterIsPartyMember(pCharDlg->m_pCharacter))
						{
							bShow = TRUE;
							bMonsters = TRUE;
						}
					}
					else
					{
						if(m_pParentPartyDialog->m_pOpposingSubParty->m_dwSubPartyID == pCharDlg->m_pCharacter->m_dwSubPartyID)
						{
							bShow = TRUE;
							bMonsters = TRUE;
						}
					}
				}

				if(bShow == FALSE)
				{
					continue;
				}

				pCharDlg->m_nInitiativeSegment = pCharDlg->m_nInitiativeRoll - pCharDlg->m_nDexAdjToInitiative;

				if(pCharDlg->m_nInitiativeSegment != nSegment)
					continue;

				if(pCharDlg->m_nInitiativeSegment > 9)
					continue;

				if (bMonsters == TRUE)
				{
					m_MonstersViewMap.SetAt(nMonsterViewCount++, (CDMBaseCharViewDialog*)pCharDlg);
				}
				else
				{
					m_PlayersViewMap.SetAt(nPlayerViewCount++, (CDMBaseCharViewDialog*)pCharDlg);

				}

				int nActionSegment = m_pParentPartyDialog->m_pParty->m_nSegment + nSegment;
				int nActionRound = m_pParentPartyDialog->m_pParty->m_nRound;

				if(nActionSegment < 0)
				{
					nActionSegment = 10 + nActionSegment;
					nActionRound -= 1;

					if(nActionRound < 0)
					{
						nActionRound = 10 + nActionRound;
					}
				}

				if(nActionSegment > 9)
				{
					nActionSegment = nActionSegment % 10;
					nActionRound += 1;
				}

				if(pCharDlg->m_nInitiativeSegment < 10)
				{
					int nRoundTime = nActionRound * 10 + nActionSegment;

					if(nRoundTime > m_nMaxRoundTime)
					{
						m_nMaxRoundTime = nRoundTime;
					}
				}

				int nCol = 1;

				szTemp.Format("%s", pCharDlg->m_pCharacter->m_szCharacterName);

				int nLength = szTemp.GetLength();
				if(nLength < 15) 
				{
					CString szSpaces = _T("                                        ");
					szTemp += szSpaces.Left(15-nLength);
				}

				m_cCharacterList.InsertItem(nRow, szTemp);

				if (pCharDlg->m_nDexAdjToInitiative)
				{
					szTemp.Format("%d (%d)", pCharDlg->m_nInitiativeRoll, pCharDlg->m_nInitiativeRoll - pCharDlg->m_nDexAdjToInitiative);
				}
				else
				{
					szTemp.Format("%d", pCharDlg->m_nInitiativeRoll);
				}
				m_cCharacterList.SetItemText(nRow, nCol++, szTemp);

				CString szAdds;
				szAdds.Format("%d", pCharDlg->m_nInitiativeRoll);

#if 0
				if(pCharDlg->m_nDexAdjToInitiative)
				{
					CString szDex;
					if(pCharDlg->m_nDexAdjToInitiative > 0)
					{
						szDex.Format("+%d", pCharDlg->m_nDexAdjToInitiative);
					}
					else
					{
						szDex.Format("%d", pCharDlg->m_nDexAdjToInitiative);
					}

					szAdds += szDex;
				}

				if(pCharDlg->m_nCastingSpellSegments)
				{
					CString szCast;
					if(pCharDlg->m_nCastingSpellSegments > 0)
					{
						szCast.Format("+%d", pCharDlg->m_nCastingSpellSegments);
					}
					else
					{
						szCast.Format("%d", pCharDlg->m_nCastingSpellSegments);
					}

					szAdds += szCast;
				}

				szTemp.Format("%s/%d", szAdds, nActionRound % 10);
#else
			
				szTemp.Format("%d/%d", nActionSegment, nActionRound % 10);
#endif
				m_cCharacterList.SetItemText(nRow, nCol++, szTemp);

				if (pCharDlg->m_szInitiativeAction == _T("") && pCharDlg->m_bInitiativeCasting == FALSE && pCharDlg->m_pTargetBaseDlg != NULL)
				{
					if (IsCharacterAlive(pCharDlg->m_pTargetBaseDlg))
					{
						pCharDlg->m_szInitiativeAction.Format("attacking %s", pCharDlg->m_pTargetBaseDlg->m_szBaseCharName);
					}
					else
					{
						pCharDlg->m_szInitiativeAction = "TARGET DEAD";
					}
				}

				m_cCharacterList.SetItemText(nRow, nCol++, pCharDlg->m_szInitiativeAction);

				m_cCharacterList.SetItemText(nRow, nCol++, pCharDlg->m_szToHitDesc);

				m_cCharacterList.SetItemText(nRow, nCol++, pCharDlg->m_szDamageDesc);

				m_cCharacterList.SetItemText(nRow, nCol++, pCharDlg->m_szWeaponDesc);

				m_cCharacterList.SetItemText(nRow, nCol++, pCharDlg->m_szMoveDesc);

				m_cCharacterList.SetItemText(nRow, nCol++, pCharDlg->m_szHPDesc);

				m_cCharacterList.SetItemData(nRow, (DWORD)pCharDlg);

				m_cCharacterList.m_RowState.push_back(pCharDlg->m_pCharacter->m_HP_State);

				++nRow;
			}
		}

		///////////////////////////////////////////////////

		for (POSITION pos = m_pApp->m_NPCViewMap.GetStartPosition(); pos != NULL; )
		{
			PDNDNPCVIEWDLG pNPCDlg = NULL;
			m_pApp->m_NPCViewMap.GetNextAssoc(pos,wID,pNPCDlg);

			if(pNPCDlg != NULL && pNPCDlg->m_pNPC != NULL && pNPCDlg->m_pNPC->m_dwCharacterID)
			{
				if(pNPCDlg->m_pNPC->m_bIsCache)
					continue;

				BOOL bShow = FALSE;
				BOOL bMonsters = FALSE;

				if(m_pParentPartyDialog->m_dwSubPartyID == 0)
				{
					if(m_pParentPartyDialog->m_pParty->CharacterIsPartyMember(pNPCDlg->m_pNPC))
					{
						bShow = TRUE;
					}
				}
				else
				{
					if(m_pParentPartyDialog->m_dwSubPartyID == pNPCDlg->m_pNPC->m_dwSubPartyID)
					{
						bShow = TRUE;
					}
				}

				if(m_pParentPartyDialog->m_pOpposingPartyDialog != NULL && m_pParentPartyDialog->m_pOpposingPartyDialog->m_pParty != NULL)
				{
					if(m_pParentPartyDialog->m_pOpposingSubParty == NULL)
					{
						if(m_pParentPartyDialog->m_pOpposingPartyDialog->m_pParty->CharacterIsPartyMember(pNPCDlg->m_pNPC))
						{
							bShow = TRUE;
							bMonsters = TRUE;
						}
					}
					else
					{
						if(m_pParentPartyDialog->m_pOpposingSubParty->m_dwSubPartyID == pNPCDlg->m_pNPC->m_dwSubPartyID)
						{
							bShow = TRUE;
							bMonsters = TRUE;
						}
					}
				}

				if(bShow == FALSE)
				{
					continue;
				}

				pNPCDlg->m_nInitiativeSegment = pNPCDlg->m_nInitiativeRoll - pNPCDlg->m_nDexAdjToInitiative;

				if(pNPCDlg->m_nInitiativeSegment != nSegment)
					continue;

				if(pNPCDlg->m_nInitiativeSegment > 9)
					continue;

				if (bMonsters == TRUE)
				{
					m_MonstersViewMap.SetAt(nMonsterViewCount++, (CDMBaseCharViewDialog*)pNPCDlg);
				}
				else
				{
					m_PlayersViewMap.SetAt(nPlayerViewCount++, (CDMBaseCharViewDialog*)pNPCDlg);
				}


				int nActionSegment = m_pParentPartyDialog->m_pParty->m_nSegment + nSegment;
				int nActionRound = m_pParentPartyDialog->m_pParty->m_nRound;

				if(nActionSegment < 0)
				{
					nActionSegment = 10 + nActionSegment;
					nActionRound -= 1;

					if(nActionRound < 0)
						nActionRound = 10 + nActionRound;
				}

				if(nActionSegment > 9)
				{
					nActionSegment = nActionSegment % 10;

					nActionRound += 1;
				}

				int nRoundTime = nActionRound * 10 + nActionSegment;

				if(nRoundTime > m_nMaxRoundTime)
				{
					m_nMaxRoundTime = nRoundTime;
				}

				int nCol = 1;

				szTemp.Format("%s", pNPCDlg->m_pNPC->m_szCharacterName);

				int nLength = szTemp.GetLength();
				if(nLength < 15) 
				{
					CString szSpaces = _T("                                        ");
					szTemp += szSpaces.Left(15-nLength);
				}

				m_cCharacterList.InsertItem(nRow, szTemp);

				szTemp.Format("%d", pNPCDlg->m_nInitiativeRoll);
				m_cCharacterList.SetItemText(nRow, nCol++, szTemp);

				szTemp.Format("%d/%d", nActionSegment, nActionRound % 10);
				m_cCharacterList.SetItemText(nRow, nCol++, szTemp);

				if (pNPCDlg->m_bInitiativeCasting == FALSE && pNPCDlg->m_szInitiativeAction == _T("") && pNPCDlg->m_pTargetBaseDlg != NULL)
				{
					if (IsCharacterAlive(pNPCDlg->m_pTargetBaseDlg))
					{
						pNPCDlg->m_szInitiativeAction.Format("attacking %s", pNPCDlg->m_pTargetBaseDlg->m_szBaseCharName);
					}
					else
					{
						pNPCDlg->m_szInitiativeAction = "TARGET DEAD";
					}	
				}

				m_cCharacterList.SetItemText(nRow, nCol++, pNPCDlg->m_szInitiativeAction);

				m_cCharacterList.SetItemText(nRow, nCol++, pNPCDlg->m_szToHitDesc);
				 
				m_cCharacterList.SetItemText(nRow, nCol++, pNPCDlg->m_szDamageDesc);

				m_cCharacterList.SetItemText(nRow, nCol++, pNPCDlg->m_szWeaponDesc);

				m_cCharacterList.SetItemText(nRow, nCol++, pNPCDlg->m_szMoveDesc);

				m_cCharacterList.SetItemText(nRow, nCol++, pNPCDlg->m_szHPDesc);

				m_cCharacterList.SetItemData(nRow, (DWORD)pNPCDlg);

				m_cCharacterList.m_RowState.push_back(pNPCDlg->m_pNPC->m_HP_State);

				++nRow;
			}
		}
	}

	m_pParentPartyDialog->UpdateSelections();
	
	if(m_nOldCursor > -1)
	{
		SetSelectedListCtrlItem(&m_cCharacterList, m_nOldCursor);
		m_cCharacterList.m_nSelectedRow = m_nOldCursor;
	}

}

void CDMInitiativeDialog::RefreshAndSelect()
{
	Refresh();
	OnBnClickedDownListButton();
	OnBnClickedUpListButton();
}

void CDMInitiativeDialog::OnBnClickedOk()
{
	OnOK();
}

void CDMInitiativeDialog::OnBnClickedCancel()
{
	m_pParentPartyDialog->m_pInitiativeDialog = NULL;

	OnCancel();

	delete this;
}

void CDMInitiativeDialog::OnClose()
{
	m_pParentPartyDialog->m_pInitiativeDialog = NULL;

	CDialog::OnClose();

	delete this;
}

void CDMInitiativeDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CDMInitiativeDialog::OnBnClickedRollInitButton()
{
	WORD wID;
	for (POSITION pos = m_pApp->m_CharacterViewMap.GetStartPosition(); pos != NULL; )
	{
		PDNDCHARVIEWDLG pCharDlg = NULL;
		m_pApp->m_CharacterViewMap.GetNextAssoc(pos,wID,pCharDlg);

		if(pCharDlg != NULL && pCharDlg->m_pCharacter != NULL && pCharDlg->m_pCharacter->m_dwCharacterID)
		{
			if(pCharDlg->m_pCharacter->IsAlive() == FALSE)
			{
				pCharDlg->m_nInitiativeRoll = 99;
			}
			else if(pCharDlg->m_bNPCCheck == FALSE && m_bRollPCs == FALSE)
			{
				pCharDlg->m_nInitiativeRoll = 9;
			}
			else
			{
				pCharDlg->m_nInitiativeRoll = RollD10();
			}
			pCharDlg->m_szInitiativeAction = _T("");
			pCharDlg->m_bInitiativeCasting = FALSE;
			pCharDlg->m_nCastSpellCursorPos = -1;
			pCharDlg->m_nCastingSpellSegments = 0;
		}
	}
	for (POSITION pos = m_pApp->m_NPCViewMap.GetStartPosition(); pos != NULL; )
	{
		PDNDNPCVIEWDLG pNPCDlg = NULL;
		m_pApp->m_NPCViewMap.GetNextAssoc(pos,wID,pNPCDlg);

		if(pNPCDlg != NULL && pNPCDlg->m_pNPC != NULL && pNPCDlg->m_pNPC->m_dwCharacterID)
		{
			if(pNPCDlg->m_pNPC->m_bIsCache)
				continue;

			if(pNPCDlg->m_pNPC->IsAlive() == FALSE)
			{
				pNPCDlg->m_nInitiativeRoll = 99;
			}
			else
			{
				pNPCDlg->m_nInitiativeRoll = RollD10();
			}
			pNPCDlg->m_szInitiativeAction = _T("");
			pNPCDlg->m_bInitiativeCasting = FALSE;
			pNPCDlg->m_nCastSpellCursorPos = -1;
			pNPCDlg->m_nCastingSpellSegments = 0;
		}
	}

	m_nOldCursor = 0;

	Refresh();

	m_pParentPartyDialog->UpdateSelections();

	SetSelectedListCtrlItem(&m_cCharacterList, m_nOldCursor);
	m_cCharacterList.m_nSelectedRow = m_nOldCursor;

	CDMBaseCharViewDialog *pDlg = (CDMBaseCharViewDialog*)m_cCharacterList.GetItemData(m_nOldCursor);
	SetAttackData(pDlg);
	UpdateData(FALSE);

	if(pDlg != NULL)
	{
		m_pParentPartyDialog->FindPartyListSelection(pDlg);
		m_pParentPartyDialog->SetSelectedTarget(pDlg);
	}
	
}

void CDMInitiativeDialog::OnBnClickedRollTargetsButton()
{
	int nPlayersCount = m_PlayersViewMap.GetCount();
	int nMonstersCount = m_MonstersViewMap.GetCount();

	if (nPlayersCount == 0 || nMonstersCount == 0)
	{
		return;
	}

	for (int i = 0; i < nPlayersCount; ++i)
	{
		CDMBaseCharViewDialog *pDlg = NULL;

		if (m_PlayersViewMap.Lookup(i, pDlg) && pDlg != NULL)
		{
			pDlg->m_szToHitDesc = _T("-");

			int nTarget = rand() % nMonstersCount;

			CDMBaseCharViewDialog *pTargetDlg = NULL;

			if (m_MonstersViewMap.Lookup(nTarget, pTargetDlg) && pTargetDlg != NULL)
			{
				pDlg->m_pTargetBaseDlg = pTargetDlg;
				pDlg->m_szInitiativeAction.Format("attacking %s", pTargetDlg->m_szBaseCharName);
			}
		}
	}
	for (int i = 0; i < nMonstersCount; ++i)
	{
		CDMBaseCharViewDialog *pDlg = NULL;

		if (m_MonstersViewMap.Lookup(i, pDlg) && pDlg != NULL)
		{
			pDlg->m_szToHitDesc = _T("-");

			int nTarget = rand() % nPlayersCount;

			CDMBaseCharViewDialog *pTargetDlg = NULL;

			if (m_PlayersViewMap.Lookup(nTarget, pTargetDlg) && pTargetDlg != NULL)
			{
				pDlg->m_pTargetBaseDlg = pTargetDlg;
				pDlg->m_szInitiativeAction.Format("attacking %s", pTargetDlg->m_szBaseCharName);
			}
		}
	}

	RefreshAndSelect();
}


void CDMInitiativeDialog::OnBnClickedNextRoundButton()
{
	int nNewSegment = m_nMaxRoundTime % 10;
	int nNewRound = (m_nMaxRoundTime / 10) % 10;

	if(m_pParentPartyDialog->m_pParty->m_nRound != nNewRound || m_pParentPartyDialog->m_pParty->m_nSegment != nNewSegment)
	{
		int nError = 0;
		do
		{
			m_pParentPartyDialog->m_pParty->ModifyCalendar(DND_CALENDAR_MOD_SEGMENT, DND_CALENDAR_MOD_INCREMENT);
			++nError;

		} while ((m_pParentPartyDialog->m_pParty->m_nRound != nNewRound || m_pParentPartyDialog->m_pParty->m_nSegment != nNewSegment) && nError < 20);

		m_pParentPartyDialog->m_pParty->ModifyCalendar(DND_CALENDAR_MOD_SEGMENT, DND_CALENDAR_MOD_INCREMENT);

		m_pParentPartyDialog->Refresh();
	}

	m_nMaxRoundTime = -1;

	WORD wID;
	for (POSITION pos = m_pApp->m_CharacterViewMap.GetStartPosition(); pos != NULL; )
	{
		PDNDCHARVIEWDLG pCharDlg = NULL;
		m_pApp->m_CharacterViewMap.GetNextAssoc(pos,wID,pCharDlg);

		if(pCharDlg != NULL && pCharDlg->m_pCharacter != NULL && pCharDlg->m_pCharacter->m_dwCharacterID)
		{
			if(pCharDlg->m_nInitiativeRoll < 10)
			{
				if(pCharDlg->m_pCharacter->IsAlive() == FALSE)
				{
					pCharDlg->m_nInitiativeRoll = 99;
				}
				else if(pCharDlg->m_bNPCCheck == FALSE && m_bRollPCs == FALSE)
				{
					pCharDlg->m_nInitiativeRoll = 9;
				}
				else
				{
					pCharDlg->m_nInitiativeRoll = RollD10();
				}
				//pCharDlg->m_szInitiativeAction = _T("");
				//pCharDlg->m_bInitiativeCasting = FALSE;
				//pCharDlg->m_nCastSpellCursorPos = -1;
				//pCharDlg->m_nCastingSpellSegments = 0;

				if (pCharDlg->m_bInitiativeCasting == FALSE)
				{
					pCharDlg->m_szInitiativeAction = _T("");
				}
			}
			else
			{
				pCharDlg->m_nInitiativeRoll = pCharDlg->m_nInitiativeRoll - 10;
			}
		}
	}
	for (POSITION pos = m_pApp->m_NPCViewMap.GetStartPosition(); pos != NULL; )
	{
		PDNDNPCVIEWDLG pNPCDlg = NULL;
		m_pApp->m_NPCViewMap.GetNextAssoc(pos,wID,pNPCDlg);

		if(pNPCDlg != NULL && pNPCDlg->m_pNPC != NULL && pNPCDlg->m_pNPC->m_dwCharacterID)
		{
			if(pNPCDlg->m_pNPC->m_bIsCache)
				continue;

			if(pNPCDlg->m_pNPC->IsAlive() == FALSE)
			{
				pNPCDlg->m_nInitiativeRoll = 99;
			}
			else
			{
				pNPCDlg->m_nInitiativeRoll = RollD10();
			}
			//pNPCDlg->m_szInitiativeAction = _T("");
			//pNPCDlg->m_bInitiativeCasting = FALSE;
			//pNPCDlg->m_nCastSpellCursorPos = -1;
			//pNPCDlg->m_nCastingSpellSegments = 0;

			if (pNPCDlg->m_bInitiativeCasting == FALSE)
			{
				pNPCDlg->m_szInitiativeAction = _T("");
			}
		}
	}

	m_nOldCursor = 0;

	Refresh();

	m_pParentPartyDialog->UpdateSelections();

	SetSelectedListCtrlItem(&m_cCharacterList, m_nOldCursor);
	m_cCharacterList.m_nSelectedRow = m_nOldCursor;

	CDMBaseCharViewDialog *pDlg = (CDMBaseCharViewDialog*)m_cCharacterList.GetItemData(m_nOldCursor);
	SetAttackData(pDlg);

	if(pDlg != NULL)
	{
		m_pParentPartyDialog->FindPartyListSelection(pDlg);
		m_pParentPartyDialog->SetSelectedTarget(pDlg);
	}
	
}

void CDMInitiativeDialog::OnLvnKeydownCharacterList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	
	UpdateData(FALSE);

	int nInitiative = -1;

	switch(pLVKeyDow->wVKey)
	{
		case 0x30:	nInitiative = 0; break;
		case 0x31:	nInitiative = 1; break;
		case 0x32:	nInitiative = 2; break;
		case 0x33:	nInitiative = 3; break;
		case 0x34:	nInitiative = 4; break;
		case 0x35:	nInitiative = 5; break;
		case 0x36:	nInitiative = 6; break;
		case 0x37:	nInitiative = 7; break;
		case 0x38:	nInitiative = 8; break;
		case 0x39:	nInitiative = 9; break;

		default:
		{
			TRACE("UNDEF key %x\n", pLVKeyDow->wVKey);
		}
	}

	int nCursor = GetSelectedListCtrlItem(&m_cCharacterList);

	if(nInitiative > -1 && nCursor > -1)
	{
		CDMBaseCharViewDialog *pDlg = (CDMBaseCharViewDialog *)m_cCharacterList.GetItemData(nCursor);
		pDlg->m_nInitiativeRoll = nInitiative;
		Refresh();	

		nCursor = GetSelectedListCtrlItem(&m_cCharacterList);

		if(nCursor > -1)
		{
			pDlg = (CDMBaseCharViewDialog *)m_cCharacterList.GetItemData(nCursor);
			SetAttackData(pDlg);
			m_pParentPartyDialog->FindPartyListSelection(pDlg);
			m_pParentPartyDialog->SetSelectedTarget(pDlg);
		}
	}

	*pResult = 0;
}


void CDMInitiativeDialog::OnClickCharacterList(NMHDR* pNMHDR, LRESULT* pResult) 
{
#if 0 // OnLvnItemchangedCharacterList actually handles this
	UpdateData(FALSE);

	if (pNMHDR->code == NM_RCLICK)
	{
		TRACE("BOOYA\n");
	}

	int nCursor = GetSelectedListCtrlItem(&m_cCharacterList);

	m_cCharacterList.m_nSelectedRow = nCursor;

	if(nCursor > -1)
	{
		CDMBaseCharViewDialog *pDlg = (CDMBaseCharViewDialog *)m_cCharacterList.GetItemData(nCursor);

		if(pDlg != NULL)
		{
			m_pParentPartyDialog->FindPartyListSelection(pDlg);
			m_pParentPartyDialog->SetSelectedTarget(pDlg);

			m_pParentPartyDialog->Refresh();
		}

		m_nOldCursor = nCursor;

		Refresh();
	}
#endif

	*pResult = 0;
}

void CDMInitiativeDialog::OnLvnItemActivateCharacterList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CDMInitiativeDialog::OnLvnOdstatechangedCharacterList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLVODSTATECHANGE pStateChanged = reinterpret_cast<LPNMLVODSTATECHANGE>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CDMInitiativeDialog::OnLvnItemchangedCharacterList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	/*
	BOOL bSelectedNow = (pNMLV->uNewState  & LVIS_SELECTED);
	BOOL bSelectedBefore = (pNMLV->uOldState  & LVIS_SELECTED);
	if (!bSelectedNow && bSelectedBefore)
	{
		return;
	}
	*/
	
	UpdateData(FALSE);

	int nOtherSelected = -1;

	int nCursor = GetLastSelectedListCtrlItem(&m_cCharacterList, m_nOldCursor, &nOtherSelected);
	m_cCharacterList.m_nSelectedRow = nCursor;

	int nVeryOldCursor = m_nOldCursor;

	CDMBaseCharViewDialog *pDlg = NULL;
	CDMBaseCharViewDialog *pTargetDlg = NULL;

	// HERE KEARY
	if(m_nOldCursor != nCursor)
	{
		if(nCursor > -1)
		{
			pDlg = (CDMBaseCharViewDialog *)m_cCharacterList.GetItemData(nCursor);
			SetAttackData(pDlg);

			if(pDlg != NULL)
			{
				m_pParentPartyDialog->FindPartyListSelection(pDlg);
				m_pParentPartyDialog->SetSelectedTarget(pDlg);
			}

			//#ifdef _DEBUG
			//m_szDebugText.Format("SEL = %d", nCursor);
			//UpdateData(FALSE);
			//#endif

			m_nOldCursor = nCursor;

			if (m_pParentPartyDialog->m_pParty->CharacterIsPartyMember(pDlg->m_dwCharacterID))
			{
				m_nTurnSecondsRemaining = START_TURN_TIMER;
			}
			else
			{
				m_nTurnSecondsRemaining = 0;
			}
		}
	}

	if (nCursor != -1)
	{
		pDlg = (CDMBaseCharViewDialog *)m_cCharacterList.GetItemData(nCursor);
		SetAttackData(pDlg);

		if (nOtherSelected != -1)
		{
			pTargetDlg = (CDMBaseCharViewDialog *)m_cCharacterList.GetItemData(nOtherSelected);

			if (pDlg != NULL && pTargetDlg != NULL)
			{
				pDlg->m_pTargetBaseDlg = pTargetDlg;

				m_pParentPartyDialog->FindPartyListSelection(pDlg);
				m_pParentPartyDialog->SetSelectedTarget(pDlg);

				pDlg->m_szInitiativeAction.Format("attacking %s", pTargetDlg->m_szBaseCharName);

				m_pParentPartyDialog->Refresh();

				Refresh();
			}
			//#ifdef _DEBUG
			//m_szDebugText.Format("OTHER SEL = %d (%d) %d", nFirstSelected, nCursor, m_nOldCursor);
			//UpdateData(FALSE);
			//#endif

			ClearSelectedListCtrlItems(&m_cCharacterList);
			SetSelectedListCtrlItem(&m_cCharacterList, nCursor);
		}
		else
		{
			//pDlg->m_szInitiativeAction = " ????? ";
		}
	}

	#ifdef _DEBUG
	m_szDebugText.Format("SEL = %d (nOther = %d) %d", nCursor, nOtherSelected, nVeryOldCursor);
	UpdateData(FALSE);
	#endif

	*pResult = 0;
}



void CDMInitiativeDialog::OnBnClickedUpListButton()
{
	UpdateData(FALSE);

	int nCursor = GetSelectedListCtrlItem(&m_cCharacterList);

	nCursor -= 1;
	if(nCursor < 0)
		nCursor = 0;

	int nOldCursor = m_nOldCursor;

	SetSelectedListCtrlItem(&m_cCharacterList, nCursor);
	m_cCharacterList.m_nSelectedRow = nCursor;

	if(nOldCursor != nCursor)
	{
		CDMBaseCharViewDialog *pDlg = (CDMBaseCharViewDialog *)m_cCharacterList.GetItemData(nCursor);
		SetAttackData(pDlg);

		if(pDlg != NULL)
		{
			m_pParentPartyDialog->FindPartyListSelection(pDlg);
			m_pParentPartyDialog->SetSelectedTarget(pDlg);
		}

		m_nOldCursor = nCursor;
	}

	Refresh();
	
}

void CDMInitiativeDialog::NextSegment()
{
	OnBnClickedDownListButton();
}

void CDMInitiativeDialog::OnBnClickedDownListButton()
{
	UpdateData(FALSE);

	int nCursor = GetSelectedListCtrlItem(&m_cCharacterList);

	nCursor += 1;
	if(nCursor >= m_cCharacterList.GetItemCount())
	{
		nCursor -= 1;
	}

	int nOldCursor = m_nOldCursor;

	SetSelectedListCtrlItem(&m_cCharacterList, nCursor);
	m_cCharacterList.m_nSelectedRow = nCursor;

	if(nOldCursor != nCursor)
	{
		CDMBaseCharViewDialog *pDlg = (CDMBaseCharViewDialog *)m_cCharacterList.GetItemData(nCursor);
		SetAttackData(pDlg);

		if(pDlg != NULL)
		{
			m_pParentPartyDialog->FindPartyListSelection(pDlg);
			if(m_pParentPartyDialog->SetSelectedTarget(pDlg))
			{
				TRACE("OOPS");
			}
		}

		m_nOldCursor = nCursor;
	}

	Refresh();
}
void CDMInitiativeDialog::OnBnClickedRollPcsCheck()
{
	UpdateData(TRUE);

	m_pApp->m_bRollPCsInitiative = m_bRollPCs;
}




void CDMInitiativeDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CRect cRect;
	GetClientRect(&cRect);

	CRect listRect = cRect;

	listRect.top += 34;
	listRect.left += 50;
	listRect.right -= 10;
	listRect.bottom -= 70;

	m_cCharacterList.MoveWindow(&listRect, FALSE);

	CRect buttonSizeRect;
	m_cUpListButton.GetClientRect(&buttonSizeRect);

	CRect buttonRect;
	buttonRect.left = 5;
	buttonRect.top = listRect.top;
	buttonRect.right = buttonRect.left + (buttonSizeRect.right - buttonSizeRect.left);
	buttonRect.bottom = buttonRect.top + (buttonSizeRect.bottom - buttonSizeRect.top);

	m_cUpListButton.MoveWindow(&buttonRect, FALSE);

	buttonRect.bottom = listRect.bottom+7;
	buttonRect.top = buttonRect.bottom - (buttonSizeRect.bottom - buttonSizeRect.top);

	m_cDownListButton.MoveWindow(&buttonRect, FALSE);

	m_cRollInitButton.GetClientRect(&buttonSizeRect);

	buttonRect.left = listRect.left;
	buttonRect.top = listRect.bottom+10;
	buttonRect.right = buttonRect.left + (buttonSizeRect.right - buttonSizeRect.left);
	buttonRect.bottom = buttonRect.top + (buttonSizeRect.bottom - buttonSizeRect.top);

	m_cRollInitButton.MoveWindow(&buttonRect, FALSE);

	buttonRect.left += (buttonSizeRect.right - buttonSizeRect.left + 8);
	buttonRect.right = buttonRect.left + (buttonSizeRect.right - buttonSizeRect.left);

	m_cRollTargetsButton.MoveWindow(&buttonRect, FALSE);

	buttonRect.left += (buttonSizeRect.right - buttonSizeRect.left + 8);
	buttonRect.right = buttonRect.left + (buttonSizeRect.right - buttonSizeRect.left);

	m_cNextRoundButton.MoveWindow(&buttonRect, FALSE);

	buttonRect.left += (buttonSizeRect.right - buttonSizeRect.left + 8);
	//buttonRect.top += ((buttonSizeRect.bottom - buttonSizeRect.top) / 2);

	m_cRollPCsCheck.GetClientRect(&buttonSizeRect);

	buttonRect.right = buttonRect.left + (buttonSizeRect.right - buttonSizeRect.left);
	buttonRect.bottom = buttonRect.top + (buttonSizeRect.bottom - buttonSizeRect.top);

	m_cRollPCsCheck.MoveWindow(&buttonRect, FALSE);

	////////////////////

	buttonRect.left += (buttonSizeRect.right - buttonSizeRect.left + 12);

	m_cWeaponSwapButton.GetClientRect(&buttonSizeRect);

	buttonRect.right = buttonRect.left + (buttonSizeRect.right - buttonSizeRect.left);
	buttonRect.bottom = buttonRect.top + (buttonSizeRect.bottom - buttonSizeRect.top);

	m_cWeaponSwapButton.MoveWindow(&buttonRect, FALSE);

	////////////////////

	buttonRect.left += (buttonSizeRect.right - buttonSizeRect.left + 5);

	m_cAttackButton.GetClientRect(&buttonSizeRect);

	buttonRect.right = buttonRect.left + (buttonSizeRect.right - buttonSizeRect.left);
	buttonRect.bottom = buttonRect.top + (buttonSizeRect.bottom - buttonSizeRect.top);

	m_cAttackButton.MoveWindow(&buttonRect, FALSE);

	////////////////////

	buttonRect.left += (buttonSizeRect.right - buttonSizeRect.left + 5);

	m_cMissButton.GetClientRect(&buttonSizeRect);

	buttonRect.right = buttonRect.left + (buttonSizeRect.right - buttonSizeRect.left);
	buttonRect.bottom = buttonRect.top + (buttonSizeRect.bottom - buttonSizeRect.top);

	m_cMissButton.MoveWindow(&buttonRect, FALSE);

	////////////////////

	buttonRect.left += (buttonSizeRect.right - buttonSizeRect.left + 5);

	m_cSpellButton.GetClientRect(&buttonSizeRect);

	buttonRect.right = buttonRect.left + (buttonSizeRect.right - buttonSizeRect.left);
	buttonRect.bottom = buttonRect.top + (buttonSizeRect.bottom - buttonSizeRect.top);

	m_cSpellButton.MoveWindow(&buttonRect, FALSE);

	////////////////////

	buttonRect.left += (buttonSizeRect.right - buttonSizeRect.left + 5);

	m_cMoveButton.GetClientRect(&buttonSizeRect);

	buttonRect.right = buttonRect.left + (buttonSizeRect.right - buttonSizeRect.left);
	buttonRect.bottom = buttonRect.top + (buttonSizeRect.bottom - buttonSizeRect.top);

	m_cMoveButton.MoveWindow(&buttonRect, FALSE);

	////////////////////

	buttonRect.left += (buttonSizeRect.right - buttonSizeRect.left + 5);

	m_cSkipButton.GetClientRect(&buttonSizeRect);

	buttonRect.right = buttonRect.left + (buttonSizeRect.right - buttonSizeRect.left);
	buttonRect.bottom = buttonRect.top + (buttonSizeRect.bottom - buttonSizeRect.top);

	m_cSkipButton.MoveWindow(&buttonRect, FALSE);

	////////////////////////////

	buttonRect.left = 0;
	buttonRect.top = buttonRect.top + (buttonSizeRect.bottom - buttonSizeRect.top) + 8;

	m_cCombatNote.GetClientRect(&buttonSizeRect);

	buttonRect.right = buttonRect.left + (buttonSizeRect.right - buttonSizeRect.left);
	buttonRect.bottom = buttonRect.top + (buttonSizeRect.bottom - buttonSizeRect.top);

	m_cCombatNote.MoveWindow(&buttonRect, FALSE);


	////////////////////////////



	//m_cTurnTimerNote.MoveWindow(&buttonRect, FALSE);
	

}


void CDMInitiativeDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	InvalidateRect(NULL);
}


void CDMInitiativeDialog::ClearSelectedCharacterTarget()
{
	int nCursor = GetSelectedListCtrlItem(&m_cCharacterList);

	if (nCursor > -1)
	{
		CDMBaseCharViewDialog *pDlg = (CDMBaseCharViewDialog *)m_cCharacterList.GetItemData(nCursor);

		switch (pDlg->m_CharViewType)
		{
			case DND_CHAR_VIEW_TYPE_PC:
			{
				((CDMCharViewDialog*)pDlg)->m_pTargetBaseDlg = NULL; 
				return;
			}
			case DND_CHAR_VIEW_TYPE_NPC:
			{
				((cDMBaseNPCViewDialog*)pDlg)->m_pTargetBaseDlg = NULL; 
				return;
			}
		}

	}

}

BOOL CDMInitiativeDialog::IsSelectedCharacterAlive()
{
	int nCursor = GetSelectedListCtrlItem(&m_cCharacterList);

	if (nCursor > -1)
	{
		CDMBaseCharViewDialog *pDlg = (CDMBaseCharViewDialog *)m_cCharacterList.GetItemData(nCursor);

		switch (pDlg->m_CharViewType)
		{
			case DND_CHAR_VIEW_TYPE_PC:		return ((CDMCharViewDialog*)pDlg)->m_pCharacter->IsAlive();
			case DND_CHAR_VIEW_TYPE_NPC:	return ((cDMBaseNPCViewDialog*)pDlg)->m_pNPC->IsAlive();
		}
		
	}

	return FALSE;
}

BOOL CDMInitiativeDialog::IsCharacterAlive(CDMBaseCharViewDialog *pDlg)
{
	switch (pDlg->m_CharViewType)
	{
		case DND_CHAR_VIEW_TYPE_PC:		return ((CDMCharViewDialog*)pDlg)->m_pCharacter->IsAlive();
		case DND_CHAR_VIEW_TYPE_NPC:	return ((cDMBaseNPCViewDialog*)pDlg)->m_pNPC->IsAlive();
	}

	return FALSE;
}

BOOL CDMInitiativeDialog::IsSelectedCharacterInOpponentParty()
{
	BOOL bRetVal = FALSE;

	int nCursor = GetSelectedListCtrlItem(&m_cCharacterList);

	if (nCursor > -1)
	{
		CDMBaseCharViewDialog *pDlg = (CDMBaseCharViewDialog *)m_cCharacterList.GetItemData(nCursor);

		nCursor = GetSelectedListCtrlItem(&m_cCharacterList);

		if (nCursor > -1)
		{
			pDlg = (CDMBaseCharViewDialog *)m_cCharacterList.GetItemData(nCursor);
			m_pParentPartyDialog->FindPartyListSelection(pDlg, FALSE, &bRetVal);
		}
	}

	return bRetVal;
}

CDMBaseCharViewDialog *CDMInitiativeDialog::GetSelectedCharacterDialog()
{
	int nCursor = GetSelectedListCtrlItem(&m_cCharacterList);

	if (nCursor > -1)
	{
		CDMBaseCharViewDialog *pDlg = (CDMBaseCharViewDialog *)m_cCharacterList.GetItemData(nCursor);
		return pDlg;
	}

	return NULL;
}

void CDMInitiativeDialog::OnBnClickedHitButton()
{
	m_nTurnSecondsRemaining = 0;
	
	if (FALSE == IsSelectedCharacterAlive())
	{
		NextSegment();
		return;
	}

	m_pApp->m_nInitiativeCurrentAttackNumber = m_nCompletedAttacksThisRound;

	m_pParentPartyDialog->ClickHitButton(IsSelectedCharacterInOpponentParty());

	++m_nCompletedAttacksThisRound;

	Refresh();

	if (m_nCompletedAttacksThisRound >= m_nNumAttacksThisRound)
	{
		NextSegment();
	}
	else
	{
		m_szAttacksText.Format("ATTACK #%d of %d THIS ROUND", m_nCompletedAttacksThisRound + 1, m_nNumAttacksThisRound);
		UpdateData(FALSE);
	}
}


void CDMInitiativeDialog::OnBnClickedMissButton()
{
	m_nTurnSecondsRemaining = 0;

	if (FALSE == IsSelectedCharacterAlive())
	{
		NextSegment();
		return;
	}

	m_pParentPartyDialog->ClickMissButton(IsSelectedCharacterInOpponentParty());

	++m_nCompletedAttacksThisRound;

	if (m_nCompletedAttacksThisRound >= m_nNumAttacksThisRound)
	{
		NextSegment();
	}
	else
	{
		m_szAttacksText.Format("ATTACK #%d of %d THIS ROUND", m_nCompletedAttacksThisRound + 1, m_nNumAttacksThisRound);
		UpdateData(FALSE);
	}
}

void CDMInitiativeDialog::OnBnClickedMoveButton()
{
	m_nTurnSecondsRemaining = 0;

	if (FALSE == IsSelectedCharacterAlive())
	{
		NextSegment();
		return;
	}

	int nCursor = GetSelectedListCtrlItem(&m_cCharacterList);

	if (nCursor > -1)
	{
		CDMBaseCharViewDialog *pDlg = (CDMBaseCharViewDialog *)m_cCharacterList.GetItemData(nCursor);

		nCursor = GetSelectedListCtrlItem(&m_cCharacterList);

		if (nCursor > -1)
		{
			pDlg = (CDMBaseCharViewDialog *)m_cCharacterList.GetItemData(nCursor);

			int nMovement = 0;
			switch (pDlg->m_CharViewType)
			{
				case DND_CHAR_VIEW_TYPE_PC:		nMovement = ((CDMCharViewDialog*)pDlg)->m_nCharacterMovementRate; break;
				case DND_CHAR_VIEW_TYPE_NPC:	nMovement = ((cDMBaseNPCViewDialog*)pDlg)->m_nCharacterMovementRate; break;
			}

			pDlg->m_nInitiativeRoll = min(pDlg->m_nInitiativeRoll + 1, 9);
			pDlg->m_szInitiativeAction.Format("moved %d feet", nMovement);

			ClearSelectedListCtrlItems(&m_cCharacterList);
			NextSegment();
			Refresh();
			//OnBnClickedUpListButton();
			SetSelectedListCtrlItem(&m_cCharacterList, nCursor);
		}
	}

	return;
}

void CDMInitiativeDialog::OnBnClickedSkipButton()
{
	m_nTurnSecondsRemaining = 0;

	NextSegment();
}


void CDMInitiativeDialog::OnBnClickedWeaponSwapButton()
{
	m_nTurnSecondsRemaining = 0;

	m_pParentPartyDialog->ClickWeaponSwapButton(IsSelectedCharacterInOpponentParty());

	CDMBaseCharViewDialog *pDlg = (CDMBaseCharViewDialog*)m_cCharacterList.GetItemData(m_nOldCursor);
	SetAttackData(pDlg);

	pDlg->m_nInitiativeRoll = min(pDlg->m_nInitiativeRoll + 1, 9);

	Refresh();
}


void CDMInitiativeDialog::OnBnClickedSpellButton()
{
	m_nTurnSecondsRemaining = 0;

	if (FALSE == IsSelectedCharacterAlive())
	{
		NextSegment();
		return;
	}

	CDMBaseCharViewDialog *pDlg = GetSelectedCharacterDialog();

	//if (pDlg->m_bHasBreathWeapon == FALSE)
	//{
	//	ClearSelectedCharacterTarget();  ??? why did we do this ?
	//}

	m_pParentPartyDialog->ClickSpellButton(IsSelectedCharacterInOpponentParty());

	
	if (pDlg != NULL && pDlg->m_bInitiativeCasting == FALSE)
	{
		NextSegment();
	}
}

void CDMInitiativeDialog::SetAttackData(CDMBaseCharViewDialog *pDlg)
{
	m_nNumAttacksThisRound = 0;
	
	if (m_pLastAttackCharViewDlg != pDlg)
	{
		m_nCompletedAttacksThisRound = 0;
		m_pLastAttackCharViewDlg = pDlg;
	}

	if (NULL == pDlg)
	{
		return;
	}

	if (pDlg->m_bHasBreathWeapon)
	{
		m_cSpellButton.SetWindowText("BREATH");
	}
	else
	{
		m_cSpellButton.SetWindowText("SPELL");
	}

	m_nNumAttacksThisRound = pDlg->GetAttacksPerRound(m_pParentPartyDialog->m_pParty->m_nRound);

	m_szCombatNote = _T("");
	CString szTemp = _T("");

	switch (pDlg->m_CharViewType)
	{
		case DND_CHAR_VIEW_TYPE_PC:
		{
			CDMCharViewDialog* pCharViewDlg = (CDMCharViewDialog*)pDlg;
			
			if (pCharViewDlg->m_pCharacter != NULL)
			{
				if (IsMissileWeapon(&pCharViewDlg->m_pCharacter->m_SelectedWeapons[0]))
				{
					m_szCombatNote = _T("Missiles: -5 at long range, -2 at medium range");
				}

				int nSpecializedToHitBonus = 0;
				int nSpecializedDamageBonus = 0;
				if (pCharViewDlg->m_pCharacter->IsProficientWithWeapon(&pCharViewDlg->m_pCharacter->m_SelectedWeapons[0], &nSpecializedToHitBonus, &nSpecializedDamageBonus) == FALSE)
				{
					if (m_szCombatNote != _T(""))
					{
						m_szCombatNote += _T(" : ");
					}

					int nProfPenalty = 0;
					int nNumProfs = CalculateWeaponProficiencies(pCharViewDlg->m_pCharacter, &nProfPenalty);

					szTemp.Format("NOT PROFICIENT WITH THIS WEAPON ! (%d)", nProfPenalty);

					m_szCombatNote += szTemp;
				}
			}

			break;
		}
		case DND_CHAR_VIEW_TYPE_NPC:
		{
			break;
		}
	}

	m_szAttacksText.Format("ATTACK #%d of %d THIS ROUND", m_nCompletedAttacksThisRound + 1, m_nNumAttacksThisRound);

	UpdateData(FALSE);

}


void CDMInitiativeDialog::OnBnClickedMusicButton()
{
	m_pApp->SendRemoteMusicCommand("SETMOOD COMBAT");
}


void CDMInitiativeDialog::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 8088)
	{
		if (m_nTurnSecondsRemaining == 6)
		{
			m_pApp->PlaySoundFX("Time's Ticking");
		}
		else if (m_nTurnSecondsRemaining == 1)
		{
			m_pApp->PlaySoundFX("Time's Up");
		}

		--m_nTurnSecondsRemaining;

		if (m_nTurnSecondsRemaining < 0)
		{
			m_nTurnSecondsRemaining = 0;
		}

		m_szTurnTimer.Format("TIME : %02ds", m_nTurnSecondsRemaining);
		UpdateData(FALSE);
	}

	CDialog::OnTimer(nIDEvent);
}


void CDMInitiativeDialog::OnStnDblclickTimerNote()
{
	if (m_nTurnSecondsRemaining == 0)
	{
		m_nTurnSecondsRemaining = START_TURN_TIMER;
	}
	else
	{
		m_nTurnSecondsRemaining = 0;
	}
}

LRESULT CDMInitiativeDialog::OnDirtyWindow(UINT wParam, LONG lParam)
{
	Refresh();

	return 0; // I handled this message
}
