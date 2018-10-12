// DMRandomEncounterDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DM HelperDlg.h"
#include "DMLoadFileDialog.h"
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
#include "DMCalendarDialog.h"
#include "DMPartyLogDialog.h"
#include "DMEventLoggerDialog.h"
#include "DMInitiativeDialog.h"
#include "DMRandomEncounterDialog.h"


// CDMRandomEncounterDialog dialog

IMPLEMENT_DYNAMIC(CDMRandomEncounterDialog, CDialog)

CDMRandomEncounterDialog::CDMRandomEncounterDialog(cDNDParty *pParentParty, CWnd* pParent /*=NULL*/)
	: CDialog(CDMRandomEncounterDialog::IDD, pParent)
	, m_szRolled(_T(""))
	, m_szLevelLabel(_T("Level:"))
	, m_szForceRoll(_T(""))
	, m_bUseFiendFolio(FALSE)
	, m_bUseMonsterManualII(FALSE)
	, m_szNumLimit(_T("128"))
{
	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_pParentParty = pParentParty;

	m_pParty = NULL;

	m_pSelectedTable = NULL;
	m_pOldSelectedTable = NULL;
}

CDMRandomEncounterDialog::~CDMRandomEncounterDialog()
{
}

void CDMRandomEncounterDialog::Init()
{
	m_szRolled = _T("");

	int nPos = 0;

	m_cTableCombo.ResetContent();

	m_DungeonTable.m_szTableName = _T("DUNGEON");
	m_cTableCombo.InsertString(nPos, "DUNGEON");
	m_cTableCombo.SetItemData(nPos++, (DWORD_PTR)&m_DungeonTable);

	cDNDRandomEncounterTable *pTable = NULL;
	for(int i = 0; i < m_pApp->m_RandomEncounterTables.GetCount(); ++ i)
	{
		pTable = m_pApp->m_RandomEncounterTables.GetAt(i);

		if(pTable != NULL)
		{
			if(pTable->m_szTableName.Find(_T("SUBTABLE"), 0) >= 0)
				continue;

			//if(pTable->m_szTableName.Find(_T("MONSTER LEVEL"), 0) >= 0)
			//	continue;

			if(!m_pApp->m_Settings.m_bUseMonsterManualII && !m_pApp->m_Settings.m_bUseFiendFolio)
			{
				if(pTable->m_szTableName.Find(_T("MMII"), 0) >= 0)
					continue;
			}

			m_cTableCombo.InsertString(nPos, pTable->m_szTableName);
			m_cTableCombo.SetItemData(nPos++, (DWORD_PTR)pTable);
		}
	}

	m_pSelectedTable = &m_DungeonTable;

	Refresh();

	m_cTableCombo.SetCurSel(0);

	m_cLevelCombo.SetCurSel(0);

	UpdateData(TRUE);

}

void CDMRandomEncounterDialog::Refresh()
{
	CString szTemp;

	CString szTerrains[8] = 
	{
		_T("Plains"), 
		_T("Scrub"), 
		_T("Forest"), 
		_T("Rough"), 
		_T("Desert"), 
		_T("Hills"), 
		_T("Mountains"), 
		_T("Marsh")
	};

	if(m_pSelectedTable != m_pOldSelectedTable)
	{
		int nPos = 0;
		m_cLevelCombo.ResetContent();

		m_cLevelCombo.EnableWindow(TRUE);
		m_cForceRoll.EnableWindow(TRUE);
		if(m_pSelectedTable == &m_DungeonTable) 
		{
			m_szLevelLabel = _T("Level:");
			m_cLevelCombo.InsertString(nPos, "RANDOM");		m_cLevelCombo.SetItemData(nPos++, 0);
			m_cLevelCombo.InsertString(nPos, "1");			m_cLevelCombo.SetItemData(nPos++, 1);
			m_cLevelCombo.InsertString(nPos, "2");			m_cLevelCombo.SetItemData(nPos++, 2);
			m_cLevelCombo.InsertString(nPos, "3");			m_cLevelCombo.SetItemData(nPos++, 3);
			m_cLevelCombo.InsertString(nPos, "4");			m_cLevelCombo.SetItemData(nPos++, 4);
			m_cLevelCombo.InsertString(nPos, "5");			m_cLevelCombo.SetItemData(nPos++, 5);
			m_cLevelCombo.InsertString(nPos, "6");			m_cLevelCombo.SetItemData(nPos++, 6);
			m_cLevelCombo.InsertString(nPos, "7");			m_cLevelCombo.SetItemData(nPos++, 7);
			m_cLevelCombo.InsertString(nPos, "8");			m_cLevelCombo.SetItemData(nPos++, 8);
			m_cLevelCombo.InsertString(nPos, "9");			m_cLevelCombo.SetItemData(nPos++, 9);
			m_cLevelCombo.InsertString(nPos, "10");			m_cLevelCombo.SetItemData(nPos++, 10);
			m_cLevelCombo.InsertString(nPos, "11");			m_cLevelCombo.SetItemData(nPos++, 11);
			m_cLevelCombo.InsertString(nPos, "12");			m_cLevelCombo.SetItemData(nPos++, 12);
			m_cLevelCombo.InsertString(nPos, "13");			m_cLevelCombo.SetItemData(nPos++, 13);
			m_cLevelCombo.InsertString(nPos, "14");			m_cLevelCombo.SetItemData(nPos++, 14);
			m_cLevelCombo.InsertString(nPos, "15");			m_cLevelCombo.SetItemData(nPos++, 15);
			m_cLevelCombo.InsertString(nPos, "16+");		m_cLevelCombo.SetItemData(nPos++, 16);

			m_cForceRoll.EnableWindow(FALSE);
			m_szForceRoll = _T("");
		}
		else if(m_pSelectedTable->m_szTableName.Find(_T("MONSTER LEVEL"), 0) >= 0)
		{
			szTemp.Format("%d", m_pSelectedTable->m_nLevel);
			m_cLevelCombo.InsertString(nPos, szTemp);		
			m_cLevelCombo.SetItemData(nPos++, 0);
			m_cLevelCombo.EnableWindow(FALSE);
		}
		else
		{
			m_szLevelLabel = _T("Terrain:");

			int nTerrains = 0;
			for(int i = 0; i < 8; ++i)
			{
				if(m_pSelectedTable->m_bValidColumn[i])
				{
					m_cLevelCombo.InsertString(nPos, szTerrains[i]);	
					m_cLevelCombo.SetItemData(nPos++, i);
					++nTerrains;
				}
			}
			if(nTerrains == 0)
			{
				m_cLevelCombo.ResetContent();
				m_cLevelCombo.InsertString(0, "Default");	
				m_cLevelCombo.SetItemData(0, 0);
			}
		}

		m_cLevelCombo.SetCurSel(0);

		m_pOldSelectedTable = m_pSelectedTable;
	}


	m_cEncounterList.ResetContent();

	if(m_pParty != NULL)
	{
		int nPos = 0;

		for(int i = 0; i < MAX_NPC_PARTY_MEMBERS; ++i)
		{
			if(m_pParty->m_NPCs[i].m_dwCharacterID == 0)
			{
				break;
			}

			szTemp.Format("%s", m_pParty->m_NPCs[i].m_szCharacterName);

			m_cEncounterList.InsertString(nPos++, szTemp);
		}

	}

	UpdateData(FALSE);

}


void CDMRandomEncounterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_cOKButton);
	DDX_Control(pDX, IDC_ENCOUNTER_LIST, m_cEncounterList);
	DDX_Text(pDX, IDC_ROLL_PARTY_STATIC, m_szRolled);
	DDX_Control(pDX, IDC_LEVEL_COMBO, m_cLevelCombo);
	DDX_Text(pDX, IDC_LEVEL_LABEL_STATIC, m_szLevelLabel);
	DDX_Control(pDX, IDC_TABLE_COMBO, m_cTableCombo);
	DDX_Control(pDX, IDC_DIE_ROLL_EDIT, m_cForceRoll);
	DDX_Text(pDX, IDC_DIE_ROLL_EDIT, m_szForceRoll);
	DDX_Check(pDX, IDC_FIEND_FOLIO_CHECK, m_bUseFiendFolio);
	DDX_Check(pDX, IDC_MONSTER_MANUAL_II_CHECK, m_bUseMonsterManualII);
	DDX_Text(pDX, IDC_NUM_LIMIT_EDIT, m_szNumLimit);
	DDV_MaxChars(pDX, m_szNumLimit, 3);
	DDX_Control(pDX, IDC_NUM_LIMIT_EDIT, m_cNumLimit);
}


BEGIN_MESSAGE_MAP(CDMRandomEncounterDialog, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CDMRandomEncounterDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDMRandomEncounterDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_ROLL_BUTTON, &CDMRandomEncounterDialog::OnBnClickedRollButton)
	ON_BN_CLICKED(IDC_ADD_ROLL_BUTTON, &CDMRandomEncounterDialog::OnBnClickedAddRollButton)
	ON_BN_CLICKED(IDC_CLEAR_BUTTON, &CDMRandomEncounterDialog::OnBnClickedClearButton)
	ON_CBN_SELCHANGE(IDC_TABLE_COMBO, &CDMRandomEncounterDialog::OnCbnSelchangeTableCombo)
	ON_BN_CLICKED(IDC_FIEND_FOLIO_CHECK, &CDMRandomEncounterDialog::OnBnClickedFiendFolioCheck)
	ON_BN_CLICKED(IDC_MONSTER_MANUAL_II_CHECK, &CDMRandomEncounterDialog::OnBnClickedMonsterManualIiCheck)
	ON_BN_CLICKED(IDC_DMG_BUTTON, &CDMRandomEncounterDialog::OnBnClickedDmgButton)
END_MESSAGE_MAP()


// CDMRandomEncounterDialog message handlers
BOOL CDMRandomEncounterDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bUseFiendFolio = m_pApp->m_Settings.m_bUseFiendFolio;
	m_bUseMonsterManualII = m_pApp->m_Settings.m_bUseMonsterManualII;

	m_bOldUseFiendFolio = m_bUseFiendFolio;
	m_bOldUseMonsterManualII = m_bUseMonsterManualII;

	m_cNumLimit.EnableWindow(FALSE);

	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDMRandomEncounterDialog::OnBnClickedCancel()
{
	if(m_pParty != NULL)
	{
		delete m_pParty;
	}

	m_pApp->m_Settings.m_bUseFiendFolio = m_bOldUseFiendFolio;
	m_pApp->m_Settings.m_bUseMonsterManualII = m_bOldUseMonsterManualII;

	OnCancel();
}

void CDMRandomEncounterDialog::OnBnClickedOk()
{
	if(m_pParty != NULL)
	{
		UpdateData(TRUE);

		int nLimit = atoi(m_szNumLimit.GetBuffer(0));

		for(int i = 0; i < 128; ++i)
		{
			if(m_pParty->m_NPCs[i].m_dwCharacterID)
			{
				if( i >= nLimit)
				{
					memset(&m_pParty->m_NPCs[i], 0, sizeof(cDNDNonPlayerCharacter));
				}
			}
		}

		if(m_pParentParty != NULL)
		{
			sprintf(m_pParty->m_szPartyName, "Encounter %d/%d/%d %02d:%02d", m_pParentParty->m_nMonth, m_pParentParty->m_nDayofMonth, m_pParentParty->m_nYear, m_pParentParty->m_nTurn, m_pParentParty->m_nRound);

			m_pParentParty->ModifyCalendar(DND_CALENDAR_MOD_ROUND, DND_CALENDAR_MOD_INCREMENT);
		}
		else
		{
			strcpy(m_pParty->m_szPartyName, "Random Encounter");
		}

		m_pParty->m_dwPartyID = GetUniqueID();

		DMPartyDialog *pDlg = m_pApp->m_pMainWindow->GenerateRandomEncounterTabs(m_pParty, NULL, NULL, TRUE);

		if(m_pParentParty != NULL)
		{
			DMPartyDialog *pParentDlg = NULL;
			m_pApp->m_PartyViewMap.Lookup((WORD)m_pParentParty->m_dwPartyID, pParentDlg);

			if(pParentDlg != NULL)
			{
				pParentDlg->m_pOpposingPartyDialog = pDlg;
				pParentDlg->Refresh();

				m_pApp->m_pMainWindow->FindAndSelectTab(DND_TAB_TYPE_PARTY, m_pParentParty->m_dwPartyID);
			}
		}
	}

	m_pApp->m_Settings.m_bUseFiendFolio = m_bOldUseFiendFolio;
	m_pApp->m_Settings.m_bUseMonsterManualII = m_bOldUseMonsterManualII;

	OnOK();
}

void CDMRandomEncounterDialog::OnBnClickedRollButton()
{
	CWaitCursor myWaitCursor;

	UpdateData(TRUE);

	m_cNumLimit.EnableWindow(TRUE);

	if(m_pParty != NULL)
	{
		delete m_pParty;
	}

	int nOverrideDieRoll = atoi(m_szForceRoll.GetBuffer(0));

	m_pParty = new cDNDParty();

	int nTerrain = 0;
	int nLevel = 0;

	if(m_pSelectedTable->m_szTableName == _T("DUNGEON"))
	{
		nLevel = m_cLevelCombo.GetItemData(m_cLevelCombo.GetCurSel());

		if(nLevel == 0)
		{
			nLevel = (rand()%16) + 1;
		}
		
		nTerrain = 0;
	}
	else
	{
		nLevel = 0;
		nTerrain = m_cLevelCombo.GetItemData(m_cLevelCombo.GetCurSel());
	}

	m_szRolled = m_pApp->GenerateRandomParty(m_pSelectedTable->m_szTableName, m_pParentParty, m_pParty, 0, nTerrain, nLevel, nOverrideDieRoll, NULL, NULL, NULL);

	int nCount = 0;
	for(int i = 0; i < 128; ++i)
	{
		if(m_pParty->m_NPCs[i].m_dwCharacterID)
		{
			++nCount;
		}
	}

	m_szNumLimit.Format("%d", nCount);

	Refresh();
}

void CDMRandomEncounterDialog::OnBnClickedAddRollButton()
{
	CWaitCursor myWaitCursor;

	UpdateData(TRUE);

	m_cNumLimit.EnableWindow(TRUE);

	if(m_pParty == NULL)
	{
		m_pParty = new cDNDParty();
	}
	else
	{
		int nLimit = atoi(m_szNumLimit.GetBuffer(0));

		for(int i = 0; i < 128; ++i)
		{
			if(m_pParty->m_NPCs[i].m_dwCharacterID)
			{
				if( i >= nLimit)
				{
					memset(&m_pParty->m_NPCs[i], 0, sizeof(cDNDNonPlayerCharacter));
				}
			}
		}
	}

	int nOverrideDieRoll = atoi(m_szForceRoll.GetBuffer(0));

	int nTerrain = 0;
	int nLevel = 0;

	if(m_pSelectedTable->m_szTableName == _T("DUNGEON"))
	{
		nLevel = m_cLevelCombo.GetItemData(m_cLevelCombo.GetCurSel());

		if(nLevel == 0)
		{
			nLevel = (rand()%16) + 1;
		}
		
		nTerrain = 0;
	}
	else
	{
		nLevel = 0;
		nTerrain = m_cLevelCombo.GetItemData(m_cLevelCombo.GetCurSel());
	}

	m_szRolled = m_pApp->GenerateRandomParty(m_pSelectedTable->m_szTableName, m_pParentParty, m_pParty, 0, nTerrain, nLevel, nOverrideDieRoll, NULL, NULL, NULL);

	int nCount = 0;
	for(int i = 0; i < 128; ++i)
	{
		if(m_pParty->m_NPCs[i].m_dwCharacterID)
		{
			++nCount;
		}
	}

	m_szNumLimit.Format("%d", nCount);

	Refresh();

}

void CDMRandomEncounterDialog::OnBnClickedClearButton()
{
	if(m_pParty != NULL)
	{
		delete m_pParty;
		m_pParty = NULL;
	}

	m_szRolled = _T("");

	Refresh();
}


void CDMRandomEncounterDialog::OnCbnSelchangeTableCombo()
{
	UpdateData(TRUE);

	int nCursor = m_cTableCombo.GetCurSel();

	m_pSelectedTable = (cDNDRandomEncounterTable*)m_cTableCombo.GetItemData(nCursor);

	Refresh();
}

void CDMRandomEncounterDialog::OnBnClickedFiendFolioCheck()
{
	UpdateData(TRUE);
	m_pApp->m_Settings.m_bUseFiendFolio = m_bUseFiendFolio;

	if(m_pParty != NULL)
	{
		delete m_pParty;
		m_pParty = NULL;
	}

	Init();
}

void CDMRandomEncounterDialog::OnBnClickedMonsterManualIiCheck()
{
	UpdateData(TRUE);
	m_pApp->m_Settings.m_bUseMonsterManualII = m_bUseMonsterManualII;

	if(m_pParty != NULL)
	{
		delete m_pParty;
		m_pParty = NULL;
	}

	Init();
}



void CDMRandomEncounterDialog::OnBnClickedDmgButton()
{
	CDMHelperDlg *pMainDlg = (CDMHelperDlg *)m_pApp->m_pMainWindow;
	pMainDlg->OpenPDFDocument("PDF\\DMGuide.pdf", 175);
}
