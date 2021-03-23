// DMNPCViewDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DM HelperDlg.h"
#include "DMInventoryDialog.h"
#include "DMCharSpellsDialog.h"
#include "DMCharViewDialog.h"
#include "DMNPCViewDialog.h"
#include "DMModifyValueDialog.h"
#include "DMNPCPortraitSelectDialog.h"
#include "cDMChartLookupDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static OPENFILENAME    g_ofn;
char g_NPCV_szFilename[MAX_PATH];

cDMBaseNPCViewDialog::cDMBaseNPCViewDialog(CDMHelperDlg* pMainDialog, cDNDNonPlayerCharacter *_pNPC, cDNDCacheInventory *_pCacheInventory, CWnd* pParent /*=NULL*/)
	: CDMBaseCharViewDialog(DND_CHAR_VIEW_TYPE_NPC, cDMBaseNPCViewDialog::IDD, pParent)
{
	m_pApp = (CDMHelperApp *) AfxGetApp();

	m_pMainDialog = pMainDialog;

	m_pParent = pParent;

	m_pNPC = _pNPC;
	m_pCacheInventory = _pCacheInventory;

	m_nPage = 0;
	m_nLastPage = 0;

	m_nBaseArmorClass = 10;
	m_szMonsterName = _T("");

	m_nCharacterCopy = 1;

	m_nCharacterMovementRate = 0;

	if(m_pNPC->m_dwCharacterID == 0)
	{
		m_pNPC->m_dwCharacterID = GetUniqueID();
	}

	m_dwNPCCharacterID = m_pNPC->m_dwCharacterID;

	m_dwMapID = 0;

	m_pNPCViewDialog = NULL;
	m_pInventoryDialog = NULL;
	m_pSpellsDialog = NULL;

	m_bReversedMapIcon = FALSE;

	Create(cDMBaseNPCViewDialog::IDD, pParent);
}

cDMBaseNPCViewDialog::~cDMBaseNPCViewDialog()
{
	
}

void cDMBaseNPCViewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(cDMBaseNPCViewDialog, CDialog)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// cDMBaseNPCViewDialog message handlers

BOOL cDMBaseNPCViewDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowText(m_pNPC->m_szCharacterName);

	ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void cDMBaseNPCViewDialog::OnClose()
{
	if(m_pNPCViewDialog != NULL)
	{
		m_pNPCViewDialog->m_pBaseNPCViewDialog = NULL;
		m_pNPCViewDialog->PostMessage(WM_CLOSE);
		m_pNPCViewDialog = NULL;
	}

	if(m_pNPC != NULL)
	{
		m_pApp->m_NPCViewMap.SetAt((WORD)m_dwNPCCharacterID,NULL);
		m_pApp->m_NPCViewMap.RemoveKey((WORD)m_dwNPCCharacterID);

		if(m_pNPC->m_bIsCache)
		{
			delete m_pNPC;
			m_pNPC = NULL;
		}
		else
		{
			m_pNPC = NULL;  //exists as non-pointer in party list, so don't delete
		}
	}
	
	CDMBaseCharViewDialog::OnClose();

	delete this;

}


void cDMBaseNPCViewDialog::InitView()
{
	TRACE("INITVIEW : %s\n", m_pNPC->m_szCharacterName);

	if(m_pNPCViewDialog == NULL)
	{
		m_pNPCViewDialog = new DMNPCViewDialog(m_pMainDialog, m_pNPC, m_pCacheInventory, m_pParent);
		m_pNPCViewDialog->m_pBaseNPCViewDialog = this;
	}

	m_pNPCViewDialog->m_dwMapID = m_dwMapID;

	ShowWindow(SW_HIDE);

}

void cDMBaseNPCViewDialog::OnRollStats() 
{	
	m_pNPC->m_bRolled = FALSE;

	PDNDMONSTERMANUALENTRY pMonster = NULL;
	
	m_pApp->m_MonsterManualIndexedMap.Lookup(m_pNPC->m_nMonsterIndex, pMonster);
	
	if(pMonster != NULL)
	{
		m_pNPC->m_nBaseStats[ATTRIB_INT] = 0;
		m_pNPC->m_nDisplayStats[ATTRIB_INT] = m_pNPC->m_nBaseStats[ATTRIB_INT];

		RollMonsterTreasure(m_pNPC, pMonster, FALSE, FALSE);
	}

	ProcessCharStats();

	Refresh();
}

void cDMBaseNPCViewDialog::GetRandomName(BOOL bRename)
{
	PDNDMONSTERMANUALENTRY pMonster = NULL;

	m_pApp->m_MonsterManualIndexedMap.Lookup(m_pNPC->m_nMonsterIndex, pMonster);

	if (pMonster != NULL)
	{
		if (m_pApp->m_Settings.m_bUseRandomNames)
		{
			if (m_pNPC->m_szCharacterName[0] == 0 || strcmp(m_pNPC->m_szCharacterName , "New NPC") == 0 || bRename)
			{
				DND_CHARACTER_RACES nRace = m_pApp->GetCharacterRaceFromName(pMonster->m_szMMName);

				CString szNewName = _T("");

				if (nRace != DND_CHARACTER_RACES_UNDEF)
				{
					szNewName = m_pApp->GetRandomName(nRace, m_pNPC->m_nSex);
				}
				else
				{
					szNewName = m_pApp->GetRandomName((DND_CHARACTER_RACES)m_pNPC->m_nMonsterIndex, m_pNPC->m_nSex);
				}

				if (szNewName != _T(""))
				{
					strcpy(m_pNPC->m_szCharacterName, szNewName.Left(127));
					m_pNPC->m_szCharacterName[127] = 0;
				}
			}
		}
	}
}
	

void cDMBaseNPCViewDialog::ProcessCharStats()
{
	m_dwCharacterID = m_pNPC->m_dwCharacterID;

	m_szBaseCharName = m_pNPC->m_szCharacterName;

	if(m_pNPCViewDialog != NULL)
	{
		m_pNPCViewDialog->ProcessCharStats();

		m_szDamageDesc = m_pNPCViewDialog->m_szDamageDesc;

		m_szMoveDesc.Format("%d", m_nCharacterMovementRate);

		m_szHPDesc.Format("%d", m_pNPC->m_nHitPoints - m_pNPC->m_nCurrentDamage);
	}
	else
	{
		CString szTemp;

		m_pNPC->m_nHitPoints = 0;

		int nSTRToHitAdj = 0;
		int nSTRDamAdj = 0;

		PDNDMONSTERMANUALENTRY pMonster = NULL;
		
		m_pApp->m_MonsterManualIndexedMap.Lookup(m_pNPC->m_nMonsterIndex, pMonster);
		
		if(pMonster != NULL)
		{
			if (GetMonsterPortraitPath(m_pNPC->m_szCharacterName) == FALSE)
			{
				GetMonsterPortraitPath(pMonster->m_szMMName);
			}

			m_szMonsterName = pMonster->m_szMMName;

			m_nBaseArmorClass = pMonster->m_nBaseAC;

			m_pNPC->CalculateNPCHitDice(pMonster);

			int nBaseMove = 12;
			int nMove = nBaseMove;

			
			nMove = atoi(pMonster->m_szMovement.GetBuffer(0));
			
			/*
			if(nEncumbrance > nMaxWeight)
				nMove = nMove - (nBaseMove/4);
			if(nEncumbrance > nMaxWeight+350)
				nMove = nMove - (nBaseMove/4);
			if(nEncumbrance > nMaxWeight+700)
				nMove = nMove - (nBaseMove/4);
			if(nEncumbrance > nMaxWeight+1050)
				nMove = nMove - (nBaseMove/4);
			if(nEncumbrance > nMaxWeight+1400)
				nMove = nMove - (nBaseMove/4);

			if(nMove < 0)
				nMove = 0;
				*/

			m_nCharacterMovementRate = nMove;
			m_szMoveDesc.Format("%d", m_nCharacterMovementRate);
		}
		else
		{
			m_nBaseArmorClass = 10;

			m_pNPC->m_nHitDice = 1;
			m_pNPC->m_nHitDicePlus = 0;
		}

		//armor class calcs

		//ring ring
		int nProtectionRing = 0;

		if(m_pNPC->m_RingsWorn[0].IsProtectionRing())
		{
			nProtectionRing = m_pNPC->m_RingsWorn[0].m_nMagicAdj;
		}
		if(m_pNPC->m_RingsWorn[1].IsProtectionRing())
		{
			nProtectionRing = max(nProtectionRing, m_pNPC->m_RingsWorn[1].m_nMagicAdj);
		}

		int nDefenseAdj = 0;

		if(m_pNPC->m_ArmorWorn.m_wTypeId == 0)
		{
			m_pNPC->m_ArmorWorn.m_nArmorClass = 10;
		}

		m_nBaseArmorClass = min(m_pNPC->m_ArmorWorn.m_nArmorClass - m_pNPC->m_ArmorWorn.m_nMagicAdj, m_nBaseArmorClass);

		m_pNPC->m_nRearArmorClass = m_nBaseArmorClass - nProtectionRing;
		m_pNPC->m_nUnshieldedArmorClass = m_pNPC->m_nRearArmorClass + nDefenseAdj;
		m_pNPC->m_nCurrentArmorClass = m_pNPC->m_nUnshieldedArmorClass;

		if(m_pNPC->m_ShieldWorn.m_wTypeId)
		{
			m_pNPC->m_nCurrentArmorClass -= 1;
			m_pNPC->m_nCurrentArmorClass -= m_pNPC->m_ShieldWorn.m_nMagicAdj;
		}

		if(m_pNPC->m_nCurrentArmorClass < -10)
			m_pNPC->m_nCurrentArmorClass = -10;

		if(m_pNPC->m_nCurrentArmorClass > 10)
			m_pNPC->m_nCurrentArmorClass = 10;


		if(m_pNPC->m_nUnshieldedArmorClass > 10)
			m_pNPC->m_nUnshieldedArmorClass = 10;

		if(m_pNPC->m_nRearArmorClass > 10)
			m_pNPC->m_nRearArmorClass = 10;


		//////////////////////////////////////////////

		m_pNPC->m_nLevel[0] = m_pNPC->CalculateCombatLevel();


		int nAttackMatrix[21];
		GetToHitChart(m_pNPC, nAttackMatrix);
		
		if(pMonster != NULL)
		{
			m_szDamageDesc.Format("%s", pMonster->m_szDamage);
		}
		else
		{
			m_szDamageDesc = "-";
		}


		for(int i=0; i< 21; ++i)
		{
			szTemp.Format("%d",nAttackMatrix[i] - nSTRToHitAdj);
			m_nAttackMatrix[i] = nAttackMatrix[i] - nSTRToHitAdj;
		}

		int nWeaponCount = 1;
		for(int j=0; j < 4; ++j)
		{
			if(!m_pNPC->m_SelectedWeapons[j].m_wTypeId)
				continue;

			if(m_pNPC->m_SelectedWeapons[j].m_wTypeId > m_pApp->m_WeaponsIndexedTypeArray.GetSize())
				continue;

			int nMagicAdj = m_pNPC->m_SelectedWeapons[j].m_nMagicAdj;

			cDNDWeapon *pDamageWeapon = &m_pNPC->m_SelectedWeapons[j];

			if(IsMissileWeapon(&m_pNPC->m_SelectedWeapons[j]) && IsValidAmmoForWeapon(&m_pNPC->m_SelectedWeapons[j], &m_pNPC->m_SelectedAmmo))
			{
				nMagicAdj += m_pNPC->m_SelectedAmmo.m_nMagicAdj;

				if(m_pNPC->m_SelectedAmmo.m_szDamageSmall[0] != '-')  // for sling stone weirdness
				{
					pDamageWeapon = &m_pNPC->m_SelectedAmmo;
				}
			}

			int nSpecializedToHitBonus = 0;
			int nSpecializedDamageBonus = 0;
			if(m_pNPC->IsProficientWithWeapon(&m_pNPC->m_SelectedWeapons[j], &nSpecializedToHitBonus, &nSpecializedDamageBonus) == FALSE)
			{
				int nProfPenalty = 0;
				int nNumProfs = CalculateWeaponProficiencies(m_pNPC, &nProfPenalty);
				nMagicAdj += nProfPenalty;
			}

			if(nMagicAdj)
			{
				if(nMagicAdj > 0)
					szTemp.Format("+%d",nMagicAdj);
				else
					szTemp.Format("%d",nMagicAdj);
			}
			
			int nToHitAdj = 0;
			int nDamageAdj = 0;

			nToHitAdj += nSpecializedToHitBonus;
			nDamageAdj += nSpecializedDamageBonus;

			if(m_pNPC->m_SelectedWeapons[j].m_nExceptionalStrength)
			{
				nToHitAdj += nSTRToHitAdj;
				nDamageAdj += nSTRDamAdj;
			}

			nToHitAdj += nMagicAdj;
			nDamageAdj += nMagicAdj;

			for(i=0; i< 21; ++i)
			{
				szTemp.Format("%d",nAttackMatrix[i] - nToHitAdj);

				if(j == 0)
				{
					m_nAttackMatrix[i] = nAttackMatrix[i] - nToHitAdj;
				}
			}

			if(nDamageAdj)
			{
				szTemp.Format("%s +%d / %s +%d", pDamageWeapon->m_szDamageSmall, nDamageAdj, pDamageWeapon->m_szDamageLarge, nDamageAdj);
			}
			else
			{
				szTemp.Format("%s / %s", pDamageWeapon->m_szDamageSmall, pDamageWeapon->m_szDamageLarge);
			}

			if(IsMissileWeapon(&m_pNPC->m_SelectedWeapons[j]) && !IsValidAmmoForWeapon(&m_pNPC->m_SelectedWeapons[j], &m_pNPC->m_SelectedAmmo))
			{
				if(nWeaponCount == 1)
				{
					szTemp = "NO AMMO";
				}
				else
				{
					szTemp = _T("");
				}
			}

			if(j  == 0)
			{
				m_szDamageDesc = szTemp;
			}

			++nWeaponCount;
		}

		m_bHasBreathWeapon = FALSE;
		if (pMonster != NULL)
		{
			m_szMonsterManualName.Format("%s", pMonster->m_szMMName);
			m_szNumAttacks.Format("%s/1", pMonster->m_szNumAttacks);

			//check for breath weapon
			CString szSpecialAttack = pMonster->m_szSpecialAttack;
			szSpecialAttack.MakeUpper();
			if (szSpecialAttack.Find("BREATH WEAPON") >= 0)
			{
				m_bHasBreathWeapon = TRUE;
			}
		}
		else
		{
			m_szNumAttacks.Format("%s", CalculateAttacksPerRound(m_pNPC));
		}

		m_szHPDesc.Format("%d", m_pNPC->m_nHitPoints - m_pNPC->m_nCurrentDamage);	
	
	}
}

void cDMBaseNPCViewDialog::Refresh()
{
	if(m_pNPC == NULL)
		return;

	if(m_pNPC->m_bIsCache)
	{
		if(m_pCacheInventory != NULL)
		{
			// sync here
			m_pCacheInventory->m_lCopperCarried = m_pNPC->m_lCopperCarried;
			m_pCacheInventory->m_lSilverCarried = m_pNPC->m_lSilverCarried;
			m_pCacheInventory->m_lElectrumCarried = m_pNPC->m_lElectrumCarried;
			m_pCacheInventory->m_lGoldCarried = m_pNPC->m_lGoldCarried;
			m_pCacheInventory->m_lPlatinumCarried = m_pNPC->m_lPlatinumCarried;

			m_pCacheInventory->m_fMapLocationX = m_pNPC->m_fMapLocationX;
			m_pCacheInventory->m_fMapLocationY = m_pNPC->m_fMapLocationY;

			m_pCacheInventory->m_fLocalLocationX = m_pNPC->m_fLocalLocationX;
			m_pCacheInventory->m_fLocalLocationY = m_pNPC->m_fLocalLocationY;

			m_pCacheInventory->m_lSaveFlag = m_pNPC->m_lSaveFlag;

			memcpy(m_pCacheInventory->m_szCacheDesc, m_pNPC->m_szCharacterName, 128 * sizeof(char));
			memcpy(m_pCacheInventory->m_szCacheDetails, m_pNPC->m_szDetails, 512 * sizeof(char));
			memcpy(m_pCacheInventory->m_Inventory, m_pNPC->m_Inventory, MAX_CHARACTER_INVENTORY * sizeof (cDNDObject));
		}
	}
	else
	{
		m_szBaseCharName = m_pNPC->m_szCharacterName;
	}

	if (m_pNPC->m_nHitPoints && m_pNPC->m_nCurrentDamage)
	{
		float fHPPerc = (float)m_pNPC->m_nCurrentDamage / (float)m_pNPC->m_nHitPoints;
		fHPPerc *= 100.0f;

		if (fHPPerc >= 75.0f)
			m_pNPC->m_HP_State = DND_HP_STATE_CRITICAL;
		else if (fHPPerc >= 50.0f)
			m_pNPC->m_HP_State = DND_HP_STATE_WOUNDED;
		else
			m_pNPC->m_HP_State = DND_HP_STATE_INJURED;
	}
	else
	{
		m_pNPC->m_HP_State = DND_HP_STATE_OK;
	}


	if(m_pNPCViewDialog != NULL)
	{
		m_pNPCViewDialog->Refresh();
	}

	
}

void cDMBaseNPCViewDialog::CloseSubWindows()
{
	DMNPCViewDialog *pNPCViewDialog = m_pNPCViewDialog;
	if(pNPCViewDialog != NULL)
	{
		pNPCViewDialog->m_pBaseNPCViewDialog = NULL;
	}
	m_pNPCViewDialog = NULL;

	CDMBaseCharViewDialog::CloseSubWindows();

	if(pNPCViewDialog != NULL)
	{
		pNPCViewDialog->PostMessage(WM_CLOSE);
		pNPCViewDialog = NULL;
	}
}

void cDMBaseNPCViewDialog::FireAmmo()
{
	if(m_pNPCViewDialog != NULL)
	{
		m_pNPCViewDialog->FireAmmo();
	}
}

CString cDMBaseNPCViewDialog::GetMonsterSFXID()
{
	CString szRetVal = "Monster 01";

	if (m_pNPCViewDialog != NULL && m_pNPC != NULL)
	{
		CString szTemp;
		szTemp.Format("%02d", (m_pNPC->m_nMonsterIndex % 11) + 1);
		szRetVal.Replace("01", szTemp);
	}

	return szRetVal;
}

void cDMBaseNPCViewDialog::SwapWeapon()
{
	if(m_pNPCViewDialog != NULL)
	{
		m_pNPCViewDialog->SwapWeapon();
	}
}



/////////////////////////////////////////////////////////////////////////////
// DMNPCViewDialog dialog


DMNPCViewDialog::DMNPCViewDialog(CDMHelperDlg* pMainDialog, cDNDNonPlayerCharacter	*_pNPC, cDNDCacheInventory *_pCacheInventory, CWnd* pParent /*=NULL*/)
	: CDMBaseCharViewDialog(DND_CHAR_VIEW_TYPE_NPC, CDMCharViewDialog::IDD, pParent)
	, m_szXPComment(_T(""))
	, m_szNumAttacksEdit(_T(""))
	, m_szMagicResist(_T(""))
	, m_szDescComment(_T(""))
	, m_szMonsterSize(_T("SIZE:"))
	, m_szDebugCharID(_T(""))
{
	//{{AFX_DATA_INIT(DMNPCViewDialog)
	m_szCharacterName = _T("");
	m_szHitPoints = _T("");
	m_szCurrentHitPoints = _T("");
	m_szArmorClass = _T("");
	m_szShieldPlus = _T("");
	m_szRingLeftPlusEdit = _T("");
	m_szRingRightPlusEdit = _T("");
	m_szWeaponPlus_1 = _T("");
	m_szWeaponPlus_2 = _T("");
	m_szWeaponPlus_3 = _T("");
	m_szWeaponPlus_4 = _T("");
	m_szAmmoPlus = _T("");
	m_szArmorPlus = _T("");
	m_szSaves_1 = _T("");
	m_szSaves_2 = _T("");
	m_szSaves_3 = _T("");
	m_szSaves_4 = _T("");
	m_szSaves_5 = _T("");
	m_szHDComment = _T("HD:");
	m_szIntComment = _T("INT:");
	m_szAttackComment = _T("");
	m_szDefenseComment = _T("");
	m_szMove = _T("");
	//}}AFX_DATA_INIT

	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_pMainDialog = pMainDialog;

	m_pParent = pParent;

	m_pNPC = _pNPC;
	m_pCacheInventory = _pCacheInventory;

	m_nPage = 0;
	m_nLastPage = 0;

	m_nBaseArmorClass = 10;
	m_szMonsterName = _T("");

	m_nCharacterCopy = 1;

	m_nCharacterMovementRate = 0;

	if(m_pNPC->m_dwCharacterID == 0)
	{
		m_pNPC->m_dwCharacterID = GetUniqueID();
	}

	m_dwNPCCharacterID = m_pNPC->m_dwCharacterID;

	m_dwMapID = 0;

	m_pInventoryDialog = NULL;
	m_pSpellsDialog = NULL;

	m_pNPCPortraitBackdropBitmap = NULL;
	m_pNPCPortraitBitmap = NULL;

	m_nMonsterBook = 0;
	m_szMonsterBook = _T("");

	Create(DMNPCViewDialog::IDD, pParent);

}

DMNPCViewDialog::~DMNPCViewDialog()
{
	
	if (m_pNPCPortraitBackdropBitmap != NULL)
	{
		::delete m_pNPCPortraitBackdropBitmap;
		m_pNPCPortraitBackdropBitmap = NULL;
	}

	if (m_pNPCPortraitBitmap != NULL)
	{
		::delete m_pNPCPortraitBitmap;
		m_pNPCPortraitBitmap = NULL;
	}
}


void DMNPCViewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DMNPCViewDialog)
	DDX_Control(pDX, IDC_SPELLS_BUTTON, m_cSpellsButton);
	DDX_Control(pDX, IDC_AMMO_COMBO, m_cAmmunitionCombo);
	DDX_Control(pDX, IDC_WEAPON_COMBO_4, m_cWeaponCombo_4);
	DDX_Control(pDX, IDC_WEAPON_COMBO_3, m_cWeaponCombo_3);
	DDX_Control(pDX, IDC_WEAPON_COMBO_2, m_cWeaponCombo_2);
	DDX_Control(pDX, IDC_WEAPON_COMBO_1, m_cWeaponCombo_1);
	DDX_Control(pDX, IDC_RECOVER_AMMO_COMBO, m_cRecoverAmmoCombo);
	DDX_Control(pDX, IDC_CHAR_RING_R_COMBO, m_cRingRightCombo);
	DDX_Control(pDX, IDC_CHAR_RING_L_COMBO, m_cRingLeftCombo);
	DDX_Control(pDX, IDC_CHAR_SHIELD_COMBO, m_cShieldCombo);
	DDX_Control(pDX, IDC_CHAR_ARMOR_COMBO, m_cArmorCombo);
	DDX_Control(pDX, IDC_WEAPON_CHART_LIST, m_cWeaponChartList);
	DDX_Control(pDX, IDC_CHAR_ALIGNMENT_COMBO, m_cAlignmentCombo);
	DDX_Control(pDX, IDC_CHAR_SEX_COMBO, m_cCharSexCombo);
	DDX_Control(pDX, IDC_CHAR_TYPE_COMBO, m_cCharTypeCombo);
	DDX_Text(pDX, IDC_CHAR_NAME_EDIT, m_szCharacterName);
	DDX_Text(pDX, IDC_HP_EDIT, m_szHitPoints);
	DDX_Text(pDX, IDC_HP_CURRENT_EDIT, m_szCurrentHitPoints);
	DDX_Text(pDX, IDC_AC_EDIT, m_szArmorClass);
	DDX_Text(pDX, IDC_SHIELD_PLUS_EDIT, m_szShieldPlus);
	DDX_Text(pDX, IDC_RING_L_PLUS_EDIT, m_szRingLeftPlusEdit);
	DDX_Text(pDX, IDC_RING_R_PLUS_EDIT, m_szRingRightPlusEdit);
	DDX_Text(pDX, IDC_WEAPON_PLUS_EDIT_1, m_szWeaponPlus_1);
	DDX_Text(pDX, IDC_WEAPON_PLUS_EDIT_2, m_szWeaponPlus_2);
	DDX_Text(pDX, IDC_WEAPON_PLUS_EDIT_3, m_szWeaponPlus_3);
	DDX_Text(pDX, IDC_WEAPON_PLUS_EDIT_4, m_szWeaponPlus_4);
	DDX_Text(pDX, IDC_AMMO_PLUS_EDIT, m_szAmmoPlus);
	DDX_Text(pDX, IDC_ARMOR_PLUS_EDIT, m_szArmorPlus);
	DDX_Text(pDX, IDC_SAVE_EDIT_1, m_szSaves_1);
	DDX_Text(pDX, IDC_SAVE_EDIT_2, m_szSaves_2);
	DDX_Text(pDX, IDC_SAVE_EDIT_3, m_szSaves_3);
	DDX_Text(pDX, IDC_SAVE_EDIT_4, m_szSaves_4);
	DDX_Text(pDX, IDC_SAVE_EDIT_5, m_szSaves_5);
	DDX_Text(pDX, IDC_HD_COMMENT, m_szHDComment);
	DDX_Text(pDX, IDC_INT_COMMENT, m_szIntComment);
	DDX_Text(pDX, IDC_ATTACK_COMMENT, m_szAttackComment);
	DDX_Text(pDX, IDC_DEFENSE_COMMENT, m_szDefenseComment);
	DDX_Text(pDX, IDC_MOVE_EDIT, m_szMove);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_XP_COMMENT, m_szXPComment);
	DDX_Control(pDX, IDC_CHAR_HELM_COMBO, m_cHelmCombo);
	DDX_Text(pDX, IDC_NUM_ATTACKS_EDIT, m_szNumAttacksEdit);
	DDX_Text(pDX, IDC_MR_COMMENT, m_szMagicResist);
	DDX_Text(pDX, IDC_DESC_COMMENT, m_szDescComment);
	DDX_Text(pDX, IDC_SIZE_COMMENT, m_szMonsterSize);
	DDX_Text(pDX, IDC_TT_COMMENT, m_szTreasureType);
	DDX_Control(pDX, IDC_DEBUG_CHAR_ID, m_cDebugCharID);
	DDX_Text(pDX, IDC_DEBUG_CHAR_ID, m_szDebugCharID);
}


BEGIN_MESSAGE_MAP(DMNPCViewDialog, CDialog)
	//{{AFX_MSG_MAP(DMNPCViewDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHAR_INVENTORY, OnCharInventory)
	ON_CBN_SELCHANGE(IDC_CHAR_TYPE_COMBO, OnSelchangeCharTypeCombo)
	ON_EN_CHANGE(IDC_CHAR_NAME_EDIT, OnChangeCharNameEdit)
	ON_CBN_SELCHANGE(IDC_CHAR_ALIGNMENT_COMBO, OnSelchangeCharAlignmentCombo)
	ON_BN_CLICKED(IDC_ROLL_STATS, OnRollStats)
	ON_CBN_SELCHANGE(IDC_WEAPON_COMBO_1, OnSelchangeWeaponCombo1)
	ON_CBN_SELCHANGE(IDC_WEAPON_COMBO_2, OnSelchangeWeaponCombo2)
	ON_CBN_SELCHANGE(IDC_WEAPON_COMBO_3, OnSelchangeWeaponCombo3)
	ON_CBN_SELCHANGE(IDC_WEAPON_COMBO_4, OnSelchangeWeaponCombo4)
	ON_CBN_SELCHANGE(IDC_AMMO_COMBO, OnSelchangeAmmoCombo)
	ON_CBN_SELCHANGE(IDC_RECOVER_AMMO_COMBO, OnSelchangeRecoverAmmoCombo)
	ON_EN_CHANGE(IDC_WEAPON_PLUS_EDIT_1, OnChangeWeaponPlusEdit1)
	ON_EN_CHANGE(IDC_WEAPON_PLUS_EDIT_2, OnChangeWeaponPlusEdit2)
	ON_EN_CHANGE(IDC_WEAPON_PLUS_EDIT_3, OnChangeWeaponPlusEdit3)
	ON_EN_CHANGE(IDC_WEAPON_PLUS_EDIT_4, OnChangeWeaponPlusEdit4)
	ON_EN_CHANGE(IDC_AMMO_PLUS_EDIT, OnChangeAmmoPlusEdit)
	ON_CBN_SELCHANGE(IDC_CHAR_ARMOR_COMBO, OnSelchangeCharArmorCombo)
	ON_CBN_SELCHANGE(IDC_CHAR_SHIELD_COMBO, OnSelchangeCharShieldCombo)
	ON_EN_CHANGE(IDC_ARMOR_PLUS_EDIT, OnChangeArmorPlusEdit)
	ON_EN_CHANGE(IDC_SHIELD_PLUS_EDIT, OnChangeShieldPlusEdit)
	ON_CBN_SELCHANGE(IDC_CHAR_RING_L_COMBO, OnSelchangeCharRingLCombo)
	ON_CBN_SELCHANGE(IDC_CHAR_RING_R_COMBO, OnSelchangeCharRingRCombo)
	ON_EN_CHANGE(IDC_RING_L_PLUS_EDIT, OnChangeRingLPlusEdit)
	ON_EN_CHANGE(IDC_RING_R_PLUS_EDIT, OnChangeRingRPlusEdit)
	ON_BN_CLICKED(IDC_SWAP_BUTTON_1, OnSwapButton1)
	ON_BN_CLICKED(IDC_SWAP_BUTTON_2, OnSwapButton2)
	ON_BN_CLICKED(IDC_SWAP_BUTTON_3, OnSwapButton3)
	ON_BN_CLICKED(IDC_ADD_HP_BUTTON, OnAddHpButton)
	ON_BN_CLICKED(IDC_RECOVER_AMMO, OnRecoverAmmo)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_CHAR_HELM_COMBO, OnSelchangeCharHelmCombo)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_CHAR_SEX_COMBO, &DMNPCViewDialog::OnCbnSelchangeCharSexCombo)
	ON_STN_CLICKED(IDC_NPC_PORTRAIT_BUTTON, &DMNPCViewDialog::OnStnClickedNpcPortraitButton)
	ON_STN_DBLCLK(IDC_DESC_COMMENT, &DMNPCViewDialog::OnStnDblclickDescComment)
	ON_STN_DBLCLK(IDC_TT_COMMENT, &DMNPCViewDialog::OnStnDblclickTtComment)
	ON_MESSAGE(DND_DIRTY_WINDOW_MESSAGE, &DMNPCViewDialog::OnDirtyWindow)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DMNPCViewDialog message handlers

BOOL DMNPCViewDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowText("New NPC");

	int nCount = 0;

	m_cCharTypeCombo.InsertString(nCount, "Undefined");
	m_cCharTypeCombo.SetItemData(nCount, 0);
	++nCount;

	for (int i = 0; i < m_pApp->m_MonsterManualOrderedArray.GetSize(); ++i)
	{
		PDNDMONSTERMANUALENTRY pMonster = m_pApp->m_MonsterManualOrderedArray.GetAt(i);

		if(pMonster != NULL)
		{
			if(pMonster->m_nMonsterIndex < 1000)
				continue;

			if(pMonster->m_nMonsterIndex != m_pNPC->m_nMonsterIndex)
			{
				if(m_pApp->m_Settings.m_bUseMonsterManualII == FALSE && pMonster->m_nMonsterIndex / 1000 == 2)
					continue;

				if(m_pApp->m_Settings.m_bUseFiendFolio == FALSE && pMonster->m_nMonsterIndex / 1000 == 3)
					continue;
			}

			m_cCharTypeCombo.InsertString(nCount, pMonster->m_szMMName);
			m_cCharTypeCombo.SetItemData(nCount, pMonster->m_nMonsterIndex);
			++nCount;
		}
	}

	m_cCharSexCombo.InsertString(0, "Male");
	m_cCharSexCombo.SetItemData(0, 0);
	m_cCharSexCombo.InsertString(1, "Female");
	m_cCharSexCombo.SetItemData(1, 1);

	m_cCharSexCombo.SetCurSel(0);

	for(i = 0; i <= 9; ++i)
	{
		m_cAlignmentCombo.InsertString(i, GetAlignmentName((DND_CHARACTER_ALIGNMENTS)i));
		m_cAlignmentCombo.SetItemData(i, i);
	}
	m_cAlignmentCombo.SetCurSel(0);



	nCount = 0;
	m_cWeaponChartList.InsertColumn( nCount++, "Weapon", LVCFMT_LEFT, 128, -1 );
	m_cWeaponChartList.InsertColumn( nCount++, "+", LVCFMT_LEFT, 30, -1 );
	m_cWeaponChartList.InsertColumn( nCount++, "10", LVCFMT_LEFT, 32,  -1 );
	m_cWeaponChartList.InsertColumn( nCount++, "9", LVCFMT_LEFT, 32,  -1 );
	m_cWeaponChartList.InsertColumn( nCount++, "8", LVCFMT_LEFT, 32,  -1 );
	m_cWeaponChartList.InsertColumn( nCount++, "7", LVCFMT_LEFT, 32,  -1 );
	m_cWeaponChartList.InsertColumn( nCount++, "6", LVCFMT_LEFT, 32,  -1 );
	m_cWeaponChartList.InsertColumn( nCount++, "5", LVCFMT_LEFT, 32,  -1 );
	m_cWeaponChartList.InsertColumn( nCount++, "4", LVCFMT_LEFT, 32,  -1 );
	m_cWeaponChartList.InsertColumn( nCount++, "3", LVCFMT_LEFT, 32,  -1 );
	m_cWeaponChartList.InsertColumn( nCount++, "2", LVCFMT_LEFT, 32,  -1 );
	m_cWeaponChartList.InsertColumn( nCount++, "1", LVCFMT_LEFT, 32,  -1 );
	m_cWeaponChartList.InsertColumn( nCount++, "0", LVCFMT_LEFT, 32,  -1 );

	m_cWeaponChartList.InsertColumn( nCount++, "-1", LVCFMT_LEFT, 32,  -1 );
	m_cWeaponChartList.InsertColumn( nCount++, "-2", LVCFMT_LEFT, 32,  -1 );
	m_cWeaponChartList.InsertColumn( nCount++, "-3", LVCFMT_LEFT, 32,  -1 );
	m_cWeaponChartList.InsertColumn( nCount++, "-4", LVCFMT_LEFT, 32,  -1 );
	m_cWeaponChartList.InsertColumn( nCount++, "-5", LVCFMT_LEFT, 32,  -1 );
	m_cWeaponChartList.InsertColumn( nCount++, "-6", LVCFMT_LEFT, 32,  -1 );
	m_cWeaponChartList.InsertColumn( nCount++, "-7", LVCFMT_LEFT, 32,  -1 );
	m_cWeaponChartList.InsertColumn( nCount++, "-8", LVCFMT_LEFT, 32,  -1 );
	m_cWeaponChartList.InsertColumn( nCount++, "-9", LVCFMT_LEFT, 32,  -1 );
	m_cWeaponChartList.InsertColumn( nCount++, "-10", LVCFMT_LEFT, 32,  -1 );
	m_cWeaponChartList.InsertColumn( nCount++, "DAMAGE", LVCFMT_LEFT, 128,  -1 );

	//m_pParent->InvalidateRect(NULL);
	
	if(m_pNPC->m_bIsCache)
	{
		m_nPage = 1;
	}
	else
	{
		m_pNPCPortraitBackdropBitmap = ::new Bitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_NPC_PORTRAIT_BACKDROP_BITMAP)), FALSE);
	}

	m_bInitDialogCalled = TRUE;

	ProcessCharStats();
	Refresh();

	//ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DMNPCViewDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	Graphics graphics(dc);

	if(m_pNPC->m_bIsCache)
	{
		m_nPage = 1;
	}
	
	if(m_pParent != NULL)
	{
		CRect rect;
		m_pParent->GetClientRect(&rect);

		rect.top+=20;

		SetWindowPos(NULL, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
	}
	
	if (m_nPage == 0)
	{
		if (m_pNPCPortraitBitmap != NULL)
		{
			try
			{
				graphics.DrawImage(m_pNPCPortraitBitmap, 645, 35, 192, 192);
			}
			catch (...)
			{
				::delete m_pNPCPortraitBitmap;
				m_pNPCPortraitBitmap = NULL;
			}
		}
		else if (m_pNPCPortraitBackdropBitmap != NULL)
		{
			try
			{
				graphics.DrawImage(m_pNPCPortraitBackdropBitmap, 645, 35, 192, 192);
			}
			catch (...)
			{
				::delete m_pNPCPortraitBackdropBitmap;
				m_pNPCPortraitBackdropBitmap = NULL;
			}
		}
	}
	if(m_nPage == 1)
	{
		ShowWindow(SW_HIDE);

		if(m_pInventoryDialog == NULL)
		{
			m_pInventoryDialog = new DMInventoryDialog(m_pMainDialog, m_pNPC, NULL, this, m_pParent);
		}

		m_pInventoryDialog->ShowWindow(SW_SHOW);

		m_pInventoryDialog->InitDisplay();
		m_pInventoryDialog->InvalidateRect(NULL);
	}
	else if(m_nPage == 2)
	{
		ShowWindow(SW_HIDE);
		m_pSpellsDialog->ShowWindow(SW_SHOW);

		m_pSpellsDialog->InitDisplay();
		m_pSpellsDialog->InvalidateRect(NULL);
	}


	if(m_pSpellsDialog != NULL)
	{
		if(!m_pSpellsDialog->m_bRefreshed)
		{
			m_pSpellsDialog->Refresh();
		}
	}

}

void DMNPCViewDialog::OnOK() 
{
	CDialog::OnOK();
}

void DMNPCViewDialog::OnCancel() 
{
	CDialog::OnCancel();
}


void DMNPCViewDialog::Refresh()
{
	if(m_hWnd == NULL)
		return;

	CString szTemp;

	UpdateData(TRUE);

	PDNDMONSTERMANUALENTRY pMonster = NULL;
	
	m_pApp->m_MonsterManualIndexedMap.Lookup(m_pNPC->m_nMonsterIndex, pMonster);

	if(pMonster != NULL)
	{
		m_szDescComment.Format("DESC: %s ", pMonster->m_szType);

		int nBook = pMonster->m_nMonsterIndex / 1000;
		switch(nBook)
		{
			case 1: m_szDescComment += _T("[ Monster Manual <PAGE> ]"); break;
			case 2: m_szDescComment += _T("[ Monster Manual II <PAGE> ]"); break;
			case 3: m_szDescComment += _T("[ Fiend Folio <PAGE> ]"); break;
		}

		m_nMonsterBook = nBook;
		m_szMonsterBook = pMonster->m_szBook;

		CString szReplace = pMonster->m_szBook;
		szReplace.Replace("MM ", "");
		szReplace.Replace("FF ", "");
		szReplace.Replace("MM2 ", "");
		m_szDescComment.Replace("<PAGE>", szReplace);

		m_szIntComment.Format("INT: %s (%d)", pMonster->m_szIntelligence, m_pNPC->m_nDisplayStats[ATTRIB_INT]);
		m_szMonsterSize.Format("SIZE: %s", pMonster->m_szSize);
		m_szAttackComment.Format( "SPECIAL ATTACK: %s", pMonster->m_szSpecialAttack);
		m_szDefenseComment.Format( "SPECIAL DEFENSE: %s", pMonster->m_szSpecialDefense);
		m_szMagicResist.Format( "MAGIC RESIST: %s", pMonster->m_szMagicResist);
		m_szTreasureType.Format( "TREASURE TYPE: %s", pMonster->m_szTreasureType);
	}
	else
	{
		m_szDescComment = "DESC: ";
		m_szIntComment = "INT: ";
		m_szAttackComment = "SPECIAL ATTACK:";
		m_szDefenseComment = "SPECIAL DEFENSE:";
		m_szMagicResist = "MAGIC RESIST:" ;
		m_szTreasureType = "TREASURE TYPE";
	}

	m_szCharacterName = m_pNPC->m_szCharacterName;
	m_szBaseCharName = m_szCharacterName;

	m_szHitPoints.Format("%d", m_pNPC->m_nHitPoints);
	m_szCurrentHitPoints.Format("%d", m_pNPC->m_nHitPoints - m_pNPC->m_nCurrentDamage);
	m_szHPDesc = m_szCurrentHitPoints;

	if(m_pNPC->m_szCharacterName[0] != 0)
	{
		SetWindowText(m_pNPC->m_szCharacterName);
	}

	for(int i = 0; i < m_cCharTypeCombo.GetCount(); ++i)
	{
		int nType = m_cCharTypeCombo.GetItemData(i);

		if(nType == m_pNPC->m_nMonsterIndex)
		{
			m_cCharTypeCombo.SetCurSel(i);
			break;
		}
	}

	for(i = 0; i < m_cCharSexCombo.GetCount(); ++i)
	{
		int nSex = m_cCharSexCombo.GetItemData(i);

		if(nSex == m_pNPC->m_nSex)
		{
			m_cCharSexCombo.SetCurSel(i);
			break;
		}
	}


	for(i = 0; i < m_cAlignmentCombo.GetCount(); ++i)
	{
		int nAlignment = m_cAlignmentCombo.GetItemData(i);

		if(nAlignment == m_pNPC->m_nAlignment)
		{
			m_cAlignmentCombo.SetCurSel(i);
			break;
		}
	}

	if(m_pNPC->m_nHitDicePlus)
	{
		if(m_pNPC->m_nHitDicePlus > 0)
			m_szHDComment.Format("HD: %d+%d", m_pNPC->m_nHitDice, m_pNPC->m_nHitDicePlus);
		else
			m_szHDComment.Format("HD: %d%d", m_pNPC->m_nHitDice, m_pNPC->m_nHitDicePlus);
	}
	else
	{
		if(m_pNPC->m_nHitDice == 0 && pMonster != NULL)
		{
			m_szHDComment.Format("HD: %s", pMonster->m_szHD);
		}
		else
		{
			m_szHDComment.Format("HD: %d", m_pNPC->m_nHitDice);
		}
	}

	if(pMonster != NULL)
	{
		szTemp = m_szHDComment;
		m_szHDComment.Format("%s ( %s )", szTemp, pMonster->m_szHD);

		m_pNPC->m_nNPCXPValue = GetXPFromMonsterManual(pMonster, m_pNPC->m_nHitDice, m_pNPC->m_nHitDicePlus, m_pNPC->m_nHitPoints);

		if(m_pNPC->m_nNPCXPValue)
		{
			m_szXPComment.Format("XP Value: %s = %d", pMonster->m_szXP, m_pNPC->m_nNPCXPValue);
		}
		else
		{
			m_szXPComment.Format("XP Value: %s", pMonster->m_szXP);
		}

		m_szMonsterName = pMonster->m_szMMName;
	}
	else
	{
		m_szMonsterName = _T("");
	}
	

	cDNDArmor _BlankArmor;
	strcpy(_BlankArmor.m_szType, "none");

	//load armors from inventory
	m_cArmorCombo.ResetContent();
	int nArmorCount = 0;

	m_cArmorCombo.InsertString(nArmorCount, _BlankArmor.m_szType);
	m_cArmorCombo.SetItemData(nArmorCount++, _BlankArmor.m_wTypeId);

	for (i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
	{
		if(m_pNPC->m_Inventory[i].m_ObjectType == DND_OBJECT_TYPE_ARMOR)
		{
			if(m_pNPC->m_Inventory[i].m_nMagicAdj > 0 && m_pNPC->m_Inventory[i].m_bEnchanted == FALSE)
				szTemp.Format("%s +%d", m_pNPC->m_Inventory[i].m_szType, m_pNPC->m_Inventory[i].m_nMagicAdj);
			else if(m_pNPC->m_Inventory[i].m_nMagicAdj < 0 && m_pNPC->m_Inventory[i].m_bEnchanted == FALSE)
				szTemp.Format("%s %d", m_pNPC->m_Inventory[i].m_szType, m_pNPC->m_Inventory[i].m_nMagicAdj);
			else szTemp = m_pNPC->m_Inventory[i].m_szType;

			m_cArmorCombo.InsertString(nArmorCount, szTemp);

			m_cArmorCombo.SetItemData(nArmorCount++, m_pNPC->m_Inventory[i].m_dwObjectID);
		}
	}

	BOOL bFoundIt = FALSE;
	for(i = 0; i < m_cArmorCombo.GetCount(); ++i)
	{
		DWORD nArmorId = m_cArmorCombo.GetItemData(i);

		if(nArmorId == m_pNPC->m_ArmorWorn.m_dwObjectID)
		{
			m_cArmorCombo.SetCurSel(i);
			bFoundIt = TRUE;
			break;
		}
	}

	if(bFoundIt == FALSE) //remove armor that no longer is in inventory
	{
		m_cArmorCombo.SetCurSel(0);
		_BlankArmor.CopyTo(&m_pNPC->m_ArmorWorn);
		m_pNPC->m_ArmorWorn.m_dwObjectID = 0L;
		m_pNPC->m_ArmorWorn.m_nMagicAdj = 0;
	}

	//////////////////////////////////////////////

	cDNDObject _BlankHelmet;
	strcpy(_BlankHelmet.m_szType, "none");

	//load helmets from inventory
	m_cHelmCombo.ResetContent();
	int nHelmCount = 0;

	m_cHelmCombo.InsertString(nHelmCount, _BlankHelmet.m_szType);
	m_cHelmCombo.SetItemData(nHelmCount++, _BlankHelmet.m_wTypeId);

	for (i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
	{
		if(m_pNPC->m_Inventory[i].m_ObjectType == DND_OBJECT_TYPE_HELMET)
		{
			if(m_pNPC->m_Inventory[i].m_nMagicAdj > 0 && m_pNPC->m_Inventory[i].m_bEnchanted == FALSE)
				szTemp.Format("%s +%d", m_pNPC->m_Inventory[i].m_szType, m_pNPC->m_Inventory[i].m_nMagicAdj);
			else if(m_pNPC->m_Inventory[i].m_nMagicAdj < 0 && m_pNPC->m_Inventory[i].m_bEnchanted == FALSE)
				szTemp.Format("%s %d", m_pNPC->m_Inventory[i].m_szType, m_pNPC->m_Inventory[i].m_nMagicAdj);
			else szTemp = m_pNPC->m_Inventory[i].m_szType;

			m_cHelmCombo.InsertString(nHelmCount, szTemp);

			m_cHelmCombo.SetItemData(nHelmCount++, m_pNPC->m_Inventory[i].m_dwObjectID);
		}
	}

	bFoundIt = FALSE;
	for(i = 0; i < m_cHelmCombo.GetCount(); ++i)
	{
		DWORD nHelmId = m_cHelmCombo.GetItemData(i);

		if(nHelmId == m_pNPC->m_HelmWorn.m_dwObjectID)
		{
			m_cHelmCombo.SetCurSel(i);
			bFoundIt = TRUE;
			break;
		}
	}

	if(bFoundIt == FALSE) //remove helmet that no longer is in inventory
	{
		m_cHelmCombo.SetCurSel(0);
		_BlankHelmet.CopyTo(&m_pNPC->m_HelmWorn);
		m_pNPC->m_HelmWorn.m_dwObjectID = 0L;
		m_pNPC->m_HelmWorn.m_nMagicAdj = 0;
	}

	//////////////////////////////////////////////


	cDNDShield _BlankShield;
	strcpy(_BlankShield.m_szType, "none");

	//load shields from inventory
	m_cShieldCombo.ResetContent();
	int nShieldCount = 0;

	m_cShieldCombo.InsertString(nShieldCount, _BlankShield.m_szType);
	m_cShieldCombo.SetItemData(nShieldCount++, _BlankShield.m_wTypeId);

	for (i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
	{
		if(m_pNPC->m_Inventory[i].m_ObjectType == DND_OBJECT_TYPE_SHIELD)
		{
			if(m_pNPC->m_Inventory[i].m_nMagicAdj > 0 && m_pNPC->m_Inventory[i].m_bEnchanted == FALSE)
				szTemp.Format("%s +%d", m_pNPC->m_Inventory[i].m_szType, m_pNPC->m_Inventory[i].m_nMagicAdj);
			else if(m_pNPC->m_Inventory[i].m_nMagicAdj < 0 && m_pNPC->m_Inventory[i].m_bEnchanted == FALSE)
				szTemp.Format("%s %d", m_pNPC->m_Inventory[i].m_szType, m_pNPC->m_Inventory[i].m_nMagicAdj);
			else szTemp = m_pNPC->m_Inventory[i].m_szType;

			m_cShieldCombo.InsertString(nShieldCount, szTemp);
			m_cShieldCombo.SetItemData(nShieldCount++, m_pNPC->m_Inventory[i].m_dwObjectID);
		}
	}

	bFoundIt = FALSE;
	for(i = 0; i < m_cShieldCombo.GetCount(); ++i)
	{
		DWORD dwShieldId = m_cShieldCombo.GetItemData(i);

		if(dwShieldId == m_pNPC->m_ShieldWorn.m_dwObjectID)
		{
			m_cShieldCombo.SetCurSel(i);
			bFoundIt = TRUE;
			break;
		}
	}

	if(bFoundIt == FALSE) //remove shield that no longer is in inventory
	{
		m_cShieldCombo.SetCurSel(0);
		_BlankShield.CopyTo(&m_pNPC->m_ShieldWorn);
		m_pNPC->m_ShieldWorn.m_dwObjectID = 0L;
		m_pNPC->m_ShieldWorn.m_nMagicAdj = 0;
	}


	//ring ring
	//load rings from inventory
	cDNDRing _BlankRing;
	strcpy(_BlankRing.m_szType, "none");

	m_cRingLeftCombo.ResetContent();
	m_cRingRightCombo.ResetContent();
	int nRingCount = 0;

	m_cRingLeftCombo.InsertString(nRingCount, _BlankRing.m_szType);
	m_cRingLeftCombo.SetItemData(nRingCount, _BlankRing.m_wTypeId);

	m_cRingRightCombo.InsertString(nRingCount, _BlankRing.m_szType);
	m_cRingRightCombo.SetItemData(nRingCount, _BlankRing.m_wTypeId);

	++nRingCount;

	for (i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
	{
		if(m_pNPC->m_Inventory[i].m_ObjectType == DND_OBJECT_TYPE_RING)
		{
			if(m_pNPC->m_Inventory[i].m_nMagicAdj > 0 && m_pNPC->m_Inventory[i].m_bEnchanted == FALSE)
				szTemp.Format("%s +%d", m_pNPC->m_Inventory[i].m_szType, m_pNPC->m_Inventory[i].m_nMagicAdj);
			else if(m_pNPC->m_Inventory[i].m_nMagicAdj < 0 && m_pNPC->m_Inventory[i].m_bEnchanted == FALSE)
				szTemp.Format("%s %d", m_pNPC->m_Inventory[i].m_szType, m_pNPC->m_Inventory[i].m_nMagicAdj);
			else szTemp = m_pNPC->m_Inventory[i].m_szType;

			m_cRingLeftCombo.InsertString(nRingCount, szTemp);
			m_cRingLeftCombo.SetItemData(nRingCount, m_pNPC->m_Inventory[i].m_dwObjectID);

			m_cRingRightCombo.InsertString(nRingCount, szTemp);
			m_cRingRightCombo.SetItemData(nRingCount, m_pNPC->m_Inventory[i].m_dwObjectID);

			++nRingCount;
		}
	}

	bFoundIt = FALSE;
	for(i = 0; i < m_cRingLeftCombo.GetCount(); ++i)
	{
		DWORD dwRingId = m_cRingLeftCombo.GetItemData(i);

		if(dwRingId == m_pNPC->m_RingsWorn[0].m_dwObjectID)
		{
			m_cRingLeftCombo.SetCurSel(i);
			bFoundIt = TRUE;
			break;
		}
	}

	if(bFoundIt == FALSE) //remove ring that no longer is in inventory
	{
		m_cRingLeftCombo.SetCurSel(0);
		_BlankRing.CopyTo(&m_pNPC->m_RingsWorn[0]);
		m_pNPC->m_RingsWorn[0].m_dwObjectID = 0L;
		m_pNPC->m_RingsWorn[0].m_nMagicAdj = 0;
	}

	bFoundIt = FALSE;
	for(i = 0; i < m_cRingRightCombo.GetCount(); ++i)
	{
		DWORD dwRingId = m_cRingRightCombo.GetItemData(i);

		if(dwRingId == m_pNPC->m_RingsWorn[1].m_dwObjectID)
		{
			m_cRingRightCombo.SetCurSel(i);
			bFoundIt = TRUE;
			break;
		}
	}

	if(bFoundIt == FALSE) //remove ring that no longer is in inventory
	{
		m_cRingRightCombo.SetCurSel(0);
		_BlankRing.CopyTo(&m_pNPC->m_RingsWorn[1]);
		m_pNPC->m_RingsWorn[1].m_dwObjectID = 0L;
		m_pNPC->m_RingsWorn[1].m_nMagicAdj = 0;
	}

	
	m_szArmorClass.Format("%d / %d / %d", m_pNPC->m_nCurrentArmorClass, m_pNPC->m_nUnshieldedArmorClass, m_pNPC->m_nRearArmorClass);

	if(m_pNPC->m_ArmorWorn.m_wTypeId && m_pNPC->m_ArmorWorn.m_nMagicAdj)
	{
		if(m_pNPC->m_ArmorWorn.m_nMagicAdj > 0 && m_pNPC->m_ArmorWorn.m_bEnchanted == FALSE)
			m_szArmorPlus.Format("+%d",m_pNPC->m_ArmorWorn.m_nMagicAdj);
		else
			m_szArmorPlus.Format("%d",m_pNPC->m_ArmorWorn.m_nMagicAdj);
	}
	else
	{
		m_szArmorPlus = "";
	}

	if(m_pNPC->m_ShieldWorn.m_wTypeId && m_pNPC->m_ShieldWorn.m_nMagicAdj)
	{
		if(m_pNPC->m_ShieldWorn.m_nMagicAdj > 0 && m_pNPC->m_ShieldWorn.m_bEnchanted == FALSE)
			m_szShieldPlus.Format("+%d",m_pNPC->m_ShieldWorn.m_nMagicAdj);
		else
			m_szShieldPlus.Format("%d",m_pNPC->m_ShieldWorn.m_nMagicAdj);
	}
	else
	{
		m_szShieldPlus = "";
	}

	//ring ring
	if(m_pNPC->m_RingsWorn[0].m_wTypeId && m_pNPC->m_RingsWorn[0].m_nMagicAdj)
	{
		if(m_pNPC->m_RingsWorn[0].m_nMagicAdj > 0 && m_pNPC->m_RingsWorn[0].m_bEnchanted == FALSE)
			m_szRingLeftPlusEdit.Format("+%d",m_pNPC->m_RingsWorn[0].m_nMagicAdj);
		else
			m_szRingLeftPlusEdit.Format("%d",m_pNPC->m_RingsWorn[0].m_nMagicAdj);
	}
	else
	{
		m_szRingLeftPlusEdit = "";
	}

	if(m_pNPC->m_RingsWorn[1].m_wTypeId && m_pNPC->m_RingsWorn[1].m_nMagicAdj)
	{
		if(m_pNPC->m_RingsWorn[1].m_nMagicAdj > 0 && m_pNPC->m_RingsWorn[1].m_bEnchanted == FALSE)
			m_szRingRightPlusEdit.Format("+%d",m_pNPC->m_RingsWorn[1].m_nMagicAdj);
		else
			m_szRingRightPlusEdit.Format("%d",m_pNPC->m_RingsWorn[1].m_nMagicAdj);
	}
	else
	{
		m_szRingRightPlusEdit = "";
	}

	//////////////////////////////////////////////

	cDNDWeapon _BlankWeapon;
	strcpy(_BlankWeapon.m_szType, "none");

	int nRecoverAmmoCursor = m_cRecoverAmmoCombo.GetCurSel();

	//load weapons from inventory
	m_cWeaponCombo_1.ResetContent();
	m_cWeaponCombo_2.ResetContent();
	m_cWeaponCombo_3.ResetContent();
	m_cWeaponCombo_4.ResetContent();
	m_cAmmunitionCombo.ResetContent();
	m_cRecoverAmmoCombo.ResetContent();

	int nWeaponCount = 0;
	int nAmmoCount = 0;

	m_cWeaponCombo_1.InsertString(nWeaponCount, _BlankWeapon.m_szType);
	m_cWeaponCombo_1.SetItemData(nWeaponCount, _BlankWeapon.m_wTypeId);

	m_cWeaponCombo_2.InsertString(nWeaponCount, _BlankWeapon.m_szType);
	m_cWeaponCombo_2.SetItemData(nWeaponCount, _BlankWeapon.m_wTypeId);

	m_cWeaponCombo_3.InsertString(nWeaponCount, _BlankWeapon.m_szType);
	m_cWeaponCombo_3.SetItemData(nWeaponCount, _BlankWeapon.m_wTypeId);

	m_cWeaponCombo_4.InsertString(nWeaponCount, _BlankWeapon.m_szType);
	m_cWeaponCombo_4.SetItemData(nWeaponCount, _BlankWeapon.m_wTypeId);

	m_cAmmunitionCombo.InsertString(nWeaponCount, _BlankWeapon.m_szType);
	m_cAmmunitionCombo.SetItemData(nWeaponCount, _BlankWeapon.m_wTypeId);

	++nWeaponCount;
	++nAmmoCount;

	for (i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
	{
		if(m_pNPC->m_Inventory[i].m_ObjectType == DND_OBJECT_TYPE_WEAPON)
		{
			if(m_pNPC->m_Inventory[i].m_nMagicAdj && m_pNPC->m_Inventory[i].m_bEnchanted == FALSE)
			{
				if(m_pNPC->m_Inventory[i].m_nMagicAdj)
					szTemp.Format("%s +%d", m_pNPC->m_Inventory[i].m_szType, m_pNPC->m_Inventory[i].m_nMagicAdj);
				else
					szTemp.Format("%s %d", m_pNPC->m_Inventory[i].m_szType, m_pNPC->m_Inventory[i].m_nMagicAdj);
			}
			else
			{
				szTemp = m_pNPC->m_Inventory[i].m_szType;
			}

			m_cWeaponCombo_1.InsertString(nWeaponCount, szTemp);
			m_cWeaponCombo_1.SetItemData(nWeaponCount, m_pNPC->m_Inventory[i].m_dwObjectID);

			m_cWeaponCombo_2.InsertString(nWeaponCount, szTemp);
			m_cWeaponCombo_2.SetItemData(nWeaponCount, m_pNPC->m_Inventory[i].m_dwObjectID);

			m_cWeaponCombo_3.InsertString(nWeaponCount, szTemp);
			m_cWeaponCombo_3.SetItemData(nWeaponCount, m_pNPC->m_Inventory[i].m_dwObjectID);

			m_cWeaponCombo_4.InsertString(nWeaponCount, szTemp);
			m_cWeaponCombo_4.SetItemData(nWeaponCount, m_pNPC->m_Inventory[i].m_dwObjectID);

			++nWeaponCount;
		}

		if(m_pNPC->m_Inventory[i].m_ObjectType == DND_OBJECT_TYPE_AMMO || IsThrowableWeapon(m_pNPC->m_Inventory[i].m_nFlags))
		{
			BOOL bAddAmmo = FALSE;

			if(IsThrowableWeapon(m_pNPC->m_Inventory[i].m_nFlags))
			{
				if(m_pNPC->m_SelectedWeapons[0].m_wTypeId == m_pNPC->m_Inventory[i].m_wTypeId)
				{
					bAddAmmo = TRUE;
				}
			}
			else if(IsValidAmmoForWeapon(&m_pNPC->m_SelectedWeapons[0], &m_pNPC->m_Inventory[i]) )
			{
				bAddAmmo = TRUE;
			}

			if(bAddAmmo)
			{
				if(m_pNPC->m_Inventory[i].m_lAmount == 0)
					m_pNPC->m_Inventory[i].m_lAmount = 1;

				if(m_pNPC->m_Inventory[i].m_nMagicAdj && m_pNPC->m_Inventory[i].m_bEnchanted == FALSE)
				{
					if(m_pNPC->m_Inventory[i].m_nMagicAdj)
						szTemp.Format("%s +%d (%d)", m_pNPC->m_Inventory[i].m_szType, m_pNPC->m_Inventory[i].m_nMagicAdj, m_pNPC->m_Inventory[i].m_lAmount);
					else
						szTemp.Format("%s %d (%d)", m_pNPC->m_Inventory[i].m_szType, m_pNPC->m_Inventory[i].m_nMagicAdj, m_pNPC->m_Inventory[i].m_lAmount);
				}
				else
				{
					szTemp.Format("%s (%d)",  m_pNPC->m_Inventory[i].m_szType, m_pNPC->m_Inventory[i].m_lAmount);
				}

				m_cAmmunitionCombo.InsertString(nAmmoCount, szTemp);
				m_cAmmunitionCombo.SetItemData(nAmmoCount, m_pNPC->m_Inventory[i].m_dwObjectID);

				++nAmmoCount;
			}
		}
	}

	//AMMO RECOVERY
	int nRecAmmoCount = 0;
	for (i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
	{
		if(m_RecoverAmmoInventory[i].m_ObjectType)
		{
			if(m_RecoverAmmoInventory[i].m_nMagicAdj && m_RecoverAmmoInventory[i].m_bEnchanted == FALSE)
			{
				if(m_RecoverAmmoInventory[i].m_nMagicAdj)
					szTemp.Format("%s +%d (%ld)", m_RecoverAmmoInventory[i].m_szType, m_RecoverAmmoInventory[i].m_nMagicAdj, m_RecoverAmmoInventory[i].m_lAmount);
				else
					szTemp.Format("%s %d (%ld)", m_RecoverAmmoInventory[i].m_szType, m_RecoverAmmoInventory[i].m_nMagicAdj, m_RecoverAmmoInventory[i].m_lAmount);
			}
			else
			{
				szTemp.Format("%s (%ld)", m_RecoverAmmoInventory[i].m_szType, m_RecoverAmmoInventory[i].m_lAmount);
			}

			m_cRecoverAmmoCombo.InsertString(nRecAmmoCount, szTemp);
			m_cRecoverAmmoCombo.SetItemData(nRecAmmoCount, (ULONG)&m_RecoverAmmoInventory[i]);

			++nRecAmmoCount;
		}
	}

	if(nRecoverAmmoCursor == -1)
		m_cRecoverAmmoCombo.SetCurSel(nRecAmmoCount-1);
	else
		m_cRecoverAmmoCombo.SetCurSel(nRecAmmoCount);

	//WEAPON 1
	bFoundIt = FALSE;
	for(i = 0; i < m_cWeaponCombo_1.GetCount(); ++i)
	{
		DWORD dwWeaponId = m_cWeaponCombo_1.GetItemData(i);

		if(dwWeaponId == m_pNPC->m_SelectedWeapons[0].m_dwObjectID)
		{
			m_cWeaponCombo_1.SetCurSel(i);
			bFoundIt = TRUE;
			break;
		}
	}

	if(bFoundIt == FALSE) //remove weapon that no longer is in inventory
	{
		m_cWeaponCombo_1.SetCurSel(0);
		_BlankWeapon.CopyTo(&m_pNPC->m_SelectedWeapons[0]);
		m_pNPC->m_SelectedWeapons[0].m_dwObjectID = 0L;
		m_pNPC->m_SelectedWeapons[0].m_nMagicAdj = 0;
	}

	//WEAPON 2
	bFoundIt = FALSE;
	for(i = 0; i < m_cWeaponCombo_2.GetCount(); ++i)
	{
		DWORD dwWeaponId = m_cWeaponCombo_2.GetItemData(i);

		if(dwWeaponId == m_pNPC->m_SelectedWeapons[1].m_dwObjectID)
		{
			m_cWeaponCombo_2.SetCurSel(i);
			bFoundIt = TRUE;
			break;
		}
	}

	if(bFoundIt == FALSE) //remove weapon that no longer is in inventory
	{
		m_cWeaponCombo_2.SetCurSel(0);
		_BlankWeapon.CopyTo(&m_pNPC->m_SelectedWeapons[1]);
		m_pNPC->m_SelectedWeapons[1].m_dwObjectID = 0L;
		m_pNPC->m_SelectedWeapons[1].m_nMagicAdj = 0;
	}

	//WEAPON 3
	bFoundIt = FALSE;
	for(i = 0; i < m_cWeaponCombo_3.GetCount(); ++i)
	{
		DWORD dwWeaponId = m_cWeaponCombo_3.GetItemData(i);

		if(dwWeaponId == m_pNPC->m_SelectedWeapons[2].m_dwObjectID)
		{
			m_cWeaponCombo_3.SetCurSel(i);
			bFoundIt = TRUE;
			break;
		}
	}

	if(bFoundIt == FALSE) //remove weapon that no longer is in inventory
	{
		m_cWeaponCombo_3.SetCurSel(0);
		_BlankWeapon.CopyTo(&m_pNPC->m_SelectedWeapons[2]);
		m_pNPC->m_SelectedWeapons[2].m_dwObjectID = 0L;
		m_pNPC->m_SelectedWeapons[2].m_nMagicAdj = 0;
	}

	//WEAPON 4
	bFoundIt = FALSE;
	for(i = 0; i < m_cWeaponCombo_4.GetCount(); ++i)
	{
		DWORD dwWeaponId = m_cWeaponCombo_4.GetItemData(i);

		if(dwWeaponId == m_pNPC->m_SelectedWeapons[3].m_dwObjectID)
		{
			m_cWeaponCombo_4.SetCurSel(i);
			bFoundIt = TRUE;
			break;
		}
	}

	if(bFoundIt == FALSE) //remove weapon that no longer is in inventory
	{
		m_cWeaponCombo_4.SetCurSel(0);
		_BlankWeapon.CopyTo(&m_pNPC->m_SelectedWeapons[3]);
		m_pNPC->m_SelectedWeapons[3].m_dwObjectID = 0L;
		m_pNPC->m_SelectedWeapons[3].m_nMagicAdj = 0;
	}

	//AMMO
	bFoundIt = FALSE;
	for(i = 0; i < m_cAmmunitionCombo.GetCount(); ++i)
	{
		DWORD dwWeaponId = m_cAmmunitionCombo.GetItemData(i);

		if(dwWeaponId == m_pNPC->m_SelectedAmmo.m_dwObjectID)
		{
			m_cAmmunitionCombo.SetCurSel(i);
			bFoundIt = TRUE;
			break;
		}
	}

	if(bFoundIt == FALSE) //remove weapon that no longer is in inventory
	{
		m_cAmmunitionCombo.SetCurSel(0);
		_BlankWeapon.CopyTo(&m_pNPC->m_SelectedAmmo);
		m_pNPC->m_SelectedAmmo.m_dwObjectID = 0L;
		m_pNPC->m_SelectedAmmo.m_nMagicAdj = 0;
	}


	/////////////////////////////////////////////////////////

	if(m_pNPC->m_SelectedWeapons[0].m_wTypeId && m_pNPC->m_SelectedWeapons[0].m_nMagicAdj)
	{
		if(m_pNPC->m_SelectedWeapons[0].m_nMagicAdj > 0)
			m_szWeaponPlus_1.Format("+%d",m_pNPC->m_SelectedWeapons[0].m_nMagicAdj);
		else
			m_szWeaponPlus_1.Format("%d",m_pNPC->m_SelectedWeapons[0].m_nMagicAdj);
	}
	else
	{
		m_szWeaponPlus_1 = "";
	}

	if(m_pNPC->m_SelectedWeapons[1].m_wTypeId && m_pNPC->m_SelectedWeapons[1].m_nMagicAdj)
	{
		if(m_pNPC->m_SelectedWeapons[1].m_nMagicAdj > 0)
			m_szWeaponPlus_2.Format("+%d",m_pNPC->m_SelectedWeapons[1].m_nMagicAdj);
		else
			m_szWeaponPlus_2.Format("%d",m_pNPC->m_SelectedWeapons[1].m_nMagicAdj);
	}
	else
	{
		m_szWeaponPlus_2 = "";
	}

	if(m_pNPC->m_SelectedWeapons[2].m_wTypeId && m_pNPC->m_SelectedWeapons[2].m_nMagicAdj)
	{
		if(m_pNPC->m_SelectedWeapons[2].m_nMagicAdj > 0)
			m_szWeaponPlus_3.Format("+%d",m_pNPC->m_SelectedWeapons[2].m_nMagicAdj);
		else
			m_szWeaponPlus_3.Format("%d",m_pNPC->m_SelectedWeapons[2].m_nMagicAdj);
	}
	else
	{
		m_szWeaponPlus_3 = "";
	}

	if(m_pNPC->m_SelectedWeapons[3].m_wTypeId && m_pNPC->m_SelectedWeapons[3].m_nMagicAdj)
	{
		if(m_pNPC->m_SelectedWeapons[3].m_nMagicAdj > 0)
			m_szWeaponPlus_4.Format("+%d",m_pNPC->m_SelectedWeapons[3].m_nMagicAdj);
		else
			m_szWeaponPlus_4.Format("%d",m_pNPC->m_SelectedWeapons[3].m_nMagicAdj);
	}
	else
	{
		m_szWeaponPlus_4 = "";
	}

	if(m_pNPC->m_SelectedAmmo.m_wTypeId && m_pNPC->m_SelectedAmmo.m_nMagicAdj)
	{
		if(m_pNPC->m_SelectedAmmo.m_nMagicAdj > 0)
			m_szAmmoPlus.Format("+%d",m_pNPC->m_SelectedAmmo.m_nMagicAdj);
		else
			m_szAmmoPlus.Format("%d",m_pNPC->m_SelectedAmmo.m_nMagicAdj);
	}
	else
	{
		m_szAmmoPlus = "";
	}

	/////////////////////////////////////////////////////////

	//int nMaxWeight = 0;
	//int nEncumbrance = CalculateEncumbrance(m_pNPC, &nMaxWeight);

	int nBaseMove = 12;
	int nMove = nBaseMove;

	//TODO some MM entries have non-numeric move fields
	if(pMonster != NULL)
	{
		nMove = atoi(pMonster->m_szMovement.GetBuffer(0));
	}

	/*
	if(nEncumbrance > nMaxWeight)
		nMove = nMove - (nBaseMove/4);
	if(nEncumbrance > nMaxWeight+350)
		nMove = nMove - (nBaseMove/4);
	if(nEncumbrance > nMaxWeight+700)
		nMove = nMove - (nBaseMove/4);
	if(nEncumbrance > nMaxWeight+1050)
		nMove = nMove - (nBaseMove/4);
	if(nEncumbrance > nMaxWeight+1400)
		nMove = nMove - (nBaseMove/4);

	if(nMove < 0)
		nMove = 0;
		*/

	m_szMove.Format("%d", nMove);

	m_nCharacterMovementRate = nMove;

	#ifdef _DEBUG
	m_szDebugCharID.Format("CHAR ID: %ld", m_pNPC->m_dwCharacterID);
	#else
	m_szDebugCharID = _T("");
	#endif

	UpdateData(FALSE);

	BOOL bIsSpellCaster = GetSpellClasses(m_pNPC);

	if(bIsSpellCaster)
	{
		if(m_pSpellsDialog != NULL && m_pSpellsDialog->m_pCharacter != NULL)
		{
			m_pSpellsDialog->Refresh();
		}

		m_cSpellsButton.EnableWindow(TRUE);
	}
	else
	{
		m_cSpellsButton.EnableWindow(FALSE);
	}


	m_pApp->m_pMainWindow->InvalidateRect(NULL);

	m_pParent->InvalidateRect(NULL);

}

void DMNPCViewDialog::ProcessCharStats()
{

	if(m_bInitDialogCalled == FALSE)
	{
		return;
	}

	CString szTemp;

	m_pNPC->m_nHitPoints = 0;

	int nSTRToHitAdj = 0;
	int nSTRDamAdj = 0;

	PDNDMONSTERMANUALENTRY pMonster = NULL;
	
	m_pApp->m_MonsterManualIndexedMap.Lookup(m_pNPC->m_nMonsterIndex, pMonster);
	
	if(pMonster != NULL)
	{
		m_nBaseArmorClass = pMonster->m_nBaseAC;

		m_pNPC->m_Class[0] = DND_CHARACTER_CLASS_MONSTER;

		m_pNPC->CalculateNPCHitDice(pMonster);

		if (m_pNPC->m_nBaseStats[ATTRIB_INT] == 0)
		{
			m_pNPC->m_nBaseStats[ATTRIB_INT] = pMonster->GetIntelligenceStat();
		}
	}
	else
	{
		m_nBaseArmorClass = 10;
		m_pNPC->m_nBaseStats[ATTRIB_INT] = 0;
		m_pNPC->m_nHitDice = 1;
		m_pNPC->m_nHitDicePlus = 0;
	}

	m_pNPC->m_nDisplayStats[ATTRIB_INT] = m_pNPC->m_nBaseStats[ATTRIB_INT];

	//armor class calcs

	//ring ring
	int nProtectionRing = 0;

	if(m_pNPC->m_RingsWorn[0].IsProtectionRing())
	{
		nProtectionRing = m_pNPC->m_RingsWorn[0].m_nMagicAdj;
	}
	if(m_pNPC->m_RingsWorn[1].IsProtectionRing())
	{
		nProtectionRing = max(nProtectionRing, m_pNPC->m_RingsWorn[1].m_nMagicAdj);
	}

	int nDefenseAdj = 0;

	if(m_pNPC->m_ArmorWorn.m_wTypeId == 0)
	{
		m_pNPC->m_ArmorWorn.m_nArmorClass = 10;
	}

	m_nBaseArmorClass = min(m_pNPC->m_ArmorWorn.m_nArmorClass - m_pNPC->m_ArmorWorn.m_nMagicAdj, m_nBaseArmorClass);

	m_pNPC->m_nRearArmorClass = m_nBaseArmorClass - nProtectionRing;
	m_pNPC->m_nUnshieldedArmorClass = m_pNPC->m_nRearArmorClass + nDefenseAdj;
	m_pNPC->m_nCurrentArmorClass = m_pNPC->m_nUnshieldedArmorClass;

	if(m_pNPC->m_ShieldWorn.m_wTypeId)
	{
		m_pNPC->m_nCurrentArmorClass -= 1;
		m_pNPC->m_nCurrentArmorClass -= m_pNPC->m_ShieldWorn.m_nMagicAdj;
	}

	if(m_pNPC->m_nCurrentArmorClass < -10)
		m_pNPC->m_nCurrentArmorClass = -10;

	if(m_pNPC->m_nCurrentArmorClass > 10)
		m_pNPC->m_nCurrentArmorClass = 10;


	if(m_pNPC->m_nUnshieldedArmorClass > 10)
		m_pNPC->m_nUnshieldedArmorClass = 10;

	if(m_pNPC->m_nRearArmorClass > 10)
		m_pNPC->m_nRearArmorClass = 10;


	//////////////////////////////////////////////

	m_pNPC->m_nLevel[0] = m_pNPC->CalculateCombatLevel();
	m_pNPC->m_nLevel[1] = m_pNPC->CalculateSaveLevel();

	int nSavesMatrix[5];
	GetSavingThrows(m_pNPC, nSavesMatrix);

	m_szSaves_1.Format("%d", nSavesMatrix[0]);
	m_szSaves_2.Format("%d", nSavesMatrix[1]);
	m_szSaves_3.Format("%d", nSavesMatrix[2]);
	m_szSaves_4.Format("%d", nSavesMatrix[3]);
	m_szSaves_5.Format("%d", nSavesMatrix[4]);

	BOOL bRemove = m_cWeaponChartList.DeleteAllItems();

	int nAttackMatrix[21];
	GetToHitChart(m_pNPC, nAttackMatrix);

	m_cWeaponChartList.InsertItem(0, "BASE");
	
	if(pMonster != NULL)
	{
		m_szDamageDesc.Format("%s", pMonster->m_szDamage);
	}
	else
	{
		m_szDamageDesc = "-";
	}


	m_cWeaponChartList.SetItemText(0,23,m_szDamageDesc);

	for(int i=0; i< 21; ++i)
	{
		szTemp.Format("%d",nAttackMatrix[i] - nSTRToHitAdj);
		m_nAttackMatrix[i] = nAttackMatrix[i] - nSTRToHitAdj;
		m_cWeaponChartList.SetItemText(0,i+2,szTemp);
	}

	int nWeaponCount = 1;
	for(int j=0; j < 4; ++j)
	{
		if(!m_pNPC->m_SelectedWeapons[j].m_wTypeId)
			continue;

		if(m_pNPC->m_SelectedWeapons[j].m_wTypeId > m_pApp->m_WeaponsIndexedTypeArray.GetSize())
			continue;

		m_cWeaponChartList.InsertItem(nWeaponCount, m_pNPC->m_SelectedWeapons[j].m_szType);

		int nMagicAdj = m_pNPC->m_SelectedWeapons[j].m_nMagicAdj;

		cDNDWeapon *pDamageWeapon = &m_pNPC->m_SelectedWeapons[j];

		if(IsMissileWeapon(&m_pNPC->m_SelectedWeapons[j]) && IsValidAmmoForWeapon(&m_pNPC->m_SelectedWeapons[j], &m_pNPC->m_SelectedAmmo))
		{
			nMagicAdj += m_pNPC->m_SelectedAmmo.m_nMagicAdj;

			if(m_pNPC->m_SelectedAmmo.m_szDamageSmall[0] != '-')  // for sling stone weirdness
			{
				pDamageWeapon = &m_pNPC->m_SelectedAmmo;
			}
		}

		int nSpecializedToHitBonus = 0;
		int nSpecializedDamageBonus = 0;
		if(m_pNPC->IsProficientWithWeapon(&m_pNPC->m_SelectedWeapons[j], &nSpecializedToHitBonus, &nSpecializedDamageBonus) == FALSE)
		{
			int nProfPenalty = 0;
			int nNumProfs = CalculateWeaponProficiencies(m_pNPC, &nProfPenalty);
			nMagicAdj += nProfPenalty;
		}

		if(nMagicAdj)
		{
			if(nMagicAdj > 0)
				szTemp.Format("+%d",nMagicAdj);
			else
				szTemp.Format("%d",nMagicAdj);

			m_cWeaponChartList.SetItemText(nWeaponCount,1,szTemp);
		}
		
		int nToHitAdj = 0;
		int nDamageAdj = 0;

		nToHitAdj += nSpecializedToHitBonus;
		nDamageAdj += nSpecializedDamageBonus;

		if(m_pNPC->m_SelectedWeapons[j].m_nExceptionalStrength)
		{
			nToHitAdj += nSTRToHitAdj;
			nDamageAdj += nSTRDamAdj;
		}

		nToHitAdj += nMagicAdj;
		nDamageAdj += nMagicAdj;

		for(i=0; i< 21; ++i)
		{
			szTemp.Format("%d",nAttackMatrix[i] - nToHitAdj);
			m_cWeaponChartList.SetItemText(nWeaponCount,i+2,szTemp);

			if(j == 0)
			{
				m_nAttackMatrix[i] = nAttackMatrix[i] - nToHitAdj;
			}
		}

		if(nDamageAdj)
		{
			szTemp.Format("%s +%d / %s +%d", pDamageWeapon->m_szDamageSmall, nDamageAdj, pDamageWeapon->m_szDamageLarge, nDamageAdj);
		}
		else
		{
			szTemp.Format("%s / %s", pDamageWeapon->m_szDamageSmall, pDamageWeapon->m_szDamageLarge);
		}

		if(IsMissileWeapon(&m_pNPC->m_SelectedWeapons[j]) && !IsValidAmmoForWeapon(&m_pNPC->m_SelectedWeapons[j], &m_pNPC->m_SelectedAmmo))
		{
			if(nWeaponCount == 1)
			{
				szTemp = "NO AMMO";
			}
			else
			{
				szTemp = _T("");
			}
		}

		if(j  == 0)
		{
			m_szDamageDesc = szTemp;
		}

		m_cWeaponChartList.SetItemText(nWeaponCount,23,szTemp);

		++nWeaponCount;
	}

	if (pMonster != NULL)
	{
		m_szNumAttacks.Format("%s/1", pMonster->m_szNumAttacks);
	}
	else
	{
		m_szNumAttacks.Format("%s", CalculateAttacksPerRound(m_pNPC));
	}

	m_szNumAttacksEdit = m_szNumAttacks;

	//////////////////////////////////////////////

	//SET PORTRAIT
	if (m_pNPCPortraitBitmap != NULL)
	{
		::delete m_pNPCPortraitBitmap;
		m_pNPCPortraitBitmap = NULL;

		m_szMonsterPortraitPath = _T("");
	}

	if (pMonster != NULL)
	{
		if (GetMonsterPortraitPath(m_pNPC->m_szCharacterName) == FALSE)
		{
			GetMonsterPortraitPath(pMonster->m_szMMName);
		}

		if (m_szMonsterPortraitPath != _T(""))
		{
			LPWSTR wcsFile = m_szMonsterPortraitPath.AllocSysString();
			m_pNPCPortraitBitmap = ::new Bitmap(wcsFile, FALSE);

			int nHeight = m_pNPCPortraitBitmap->GetHeight();

			if (nHeight == 0)
			{
				::delete m_pNPCPortraitBitmap;
				m_pNPCPortraitBitmap = NULL;

				m_szMonsterPortraitPath = _T("");
			}
		}	
	}

	UpdateData(FALSE);
}

void DMNPCViewDialog::OnCharInventory() 
{
	m_nPage = 1;

	InvalidateRect(NULL);	
}

void DMNPCViewDialog::OnChangeCharNameEdit() 
{
	UpdateData(TRUE);

	strcpy(m_pNPC->m_szCharacterName, m_szCharacterName.GetBuffer(0));

	m_pNPC->MarkChanged();

	Refresh();
	
}

void DMNPCViewDialog::OnSelchangeCharTypeCombo() 
{
	UpdateData(TRUE);

	int nCursor = m_cCharTypeCombo.GetCurSel();

	m_pNPC->m_nMonsterIndex = m_cCharTypeCombo.GetItemData(nCursor);

	PDNDMONSTERMANUALENTRY pMonster = NULL;
	
	m_pApp->m_MonsterManualIndexedMap.Lookup(m_pNPC->m_nMonsterIndex, pMonster);
	
	if(pMonster != NULL)
	{
		m_pNPC->m_nAlignment = pMonster->m_nAlignment;
	}

	OnRollStats();

	if(m_pInventoryDialog != NULL)
	{
		m_pInventoryDialog->Refresh();
	}
	
}


void DMNPCViewDialog::OnSelchangeCharAlignmentCombo() 
{
	UpdateData(TRUE);

	int nCursor = m_cAlignmentCombo.GetCurSel();

	m_pNPC->m_nAlignment = (DND_CHARACTER_ALIGNMENTS)m_cAlignmentCombo.GetItemData(nCursor);

	ProcessCharStats();

	Refresh();
}
	
void DMNPCViewDialog::GetRandomName(BOOL bRename)
{
	PDNDMONSTERMANUALENTRY pMonster = NULL;

	m_pApp->m_MonsterManualIndexedMap.Lookup(m_pNPC->m_nMonsterIndex, pMonster);

	if (pMonster != NULL)
	{
		if (m_pApp->m_Settings.m_bUseRandomNames)
		{
			if (m_szCharacterName.GetLength() == 0 || m_szCharacterName == _T("New NPC") || bRename)
			{
				DND_CHARACTER_RACES nRace = m_pApp->GetCharacterRaceFromName(pMonster->m_szMMName);

				CString szNewName = _T("");

				if (nRace != DND_CHARACTER_RACES_UNDEF)
				{
					szNewName = m_pApp->GetRandomName(nRace, m_pNPC->m_nSex);
				}
				else
				{
					szNewName = m_pApp->GetRandomName((DND_CHARACTER_RACES)m_pNPC->m_nMonsterIndex, m_pNPC->m_nSex);
				}

				if (szNewName != _T(""))
				{
					m_szCharacterName = szNewName;
					strcpy(m_pNPC->m_szCharacterName, m_szCharacterName.Left(127));
					m_pNPC->m_szCharacterName[127] = 0;
				}
			}
		}
	}
}

void DMNPCViewDialog::OnRollStats() 
{	
	m_pNPC->m_bRolled = FALSE;

	m_pNPC->m_nBaseStats[ATTRIB_INT] = 0;
	m_pNPC->m_nDisplayStats[ATTRIB_INT] = m_pNPC->m_nBaseStats[ATTRIB_INT];

	PDNDMONSTERMANUALENTRY pMonster = NULL;
	
	m_pApp->m_MonsterManualIndexedMap.Lookup(m_pNPC->m_nMonsterIndex, pMonster);
	
	if(pMonster != NULL)
	{
		RollMonsterTreasure(m_pNPC, pMonster, FALSE, FALSE);
	}

	GetRandomName(FALSE);

	ProcessCharStats();

	Refresh();

	InvalidateRect(NULL);
}


void DMNPCViewDialog::OnSelchangeWeaponCombo1() 
{
	UpdateData(TRUE);

	int nCursor = m_cWeaponCombo_1.GetCurSel();

	DWORD dwWeaponID = m_cWeaponCombo_1.GetItemData(nCursor);

	cDNDObject *pInvItem = m_pNPC->FindObjectInInventory(dwWeaponID);

	if(pInvItem != NULL)
	{
		cDNDWeapon *pWeapon = NULL;

		pWeapon = m_pApp->m_WeaponsIndexedTypeArray[pInvItem->m_wTypeId];

		if(pWeapon != NULL)
		{
			pWeapon->CopyTo(&m_pNPC->m_SelectedWeapons[0]);
			m_pNPC->m_SelectedWeapons[0].m_dwObjectID = pInvItem->m_dwObjectID;
			m_pNPC->m_SelectedWeapons[0].m_nMagicAdj = pInvItem->m_nMagicAdj;
		}

		//PASS AMMO !
		if(IsThrowableWeapon(pWeapon->m_nFlags))
		{
			pWeapon->CopyTo(&m_pNPC->m_SelectedAmmo);
			m_pNPC->m_SelectedAmmo.m_dwObjectID = pInvItem->m_dwObjectID;
			m_pNPC->m_SelectedAmmo.m_nMagicAdj = pInvItem->m_nMagicAdj;
			m_pNPC->m_SelectedAmmo.m_lAmount = pInvItem->m_lAmount;
			m_cAmmunitionCombo.SetCurSel(1); //dont't know why this works, but it does
		}
		else if(IsMissileWeapon(pWeapon))
		{
			for(int i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
			{
				if(m_pNPC->m_Inventory[i].m_ObjectType == DND_OBJECT_TYPE_AMMO && IsValidAmmoForWeapon(&m_pNPC->m_SelectedWeapons[0] ,&m_pNPC->m_Inventory[i]))
				{
					pWeapon = m_pApp->m_AmmunitionIndexedTypeArray.GetAt(m_pNPC->m_Inventory[i].m_wTypeId);

					if(pWeapon != NULL)
					{
						pWeapon->CopyTo(&m_pNPC->m_SelectedAmmo);
						m_pNPC->m_SelectedAmmo.m_dwObjectID = m_pNPC->m_Inventory[i].m_dwObjectID;
						m_pNPC->m_SelectedAmmo.m_nMagicAdj = m_pNPC->m_Inventory[i].m_nMagicAdj;
						m_pNPC->m_SelectedAmmo.m_lAmount = m_pNPC->m_Inventory[i].m_lAmount;
						m_cAmmunitionCombo.SetCurSel(1); //dont't know why this works, but it does
					}
				}
			}

		}

		ProcessCharStats();

		Refresh();
	}
	
}

void DMNPCViewDialog::OnSelchangeWeaponCombo2() 
{

	int nCursor = m_cWeaponCombo_2.GetCurSel();

	DWORD dwWeaponID = m_cWeaponCombo_2.GetItemData(nCursor);

	cDNDObject *pInvItem = m_pNPC->FindObjectInInventory(dwWeaponID);

	if(pInvItem != NULL)
	{
		cDNDWeapon *pWeapon = NULL;

		pWeapon = m_pApp->m_WeaponsIndexedTypeArray[pInvItem->m_wTypeId];

		if(pWeapon != NULL)
		{
			pWeapon->CopyTo(&m_pNPC->m_SelectedWeapons[1]);
			m_pNPC->m_SelectedWeapons[1].m_dwObjectID = pInvItem->m_dwObjectID;
			m_pNPC->m_SelectedWeapons[1].m_nMagicAdj = pInvItem->m_nMagicAdj;
		}

		ProcessCharStats();

		Refresh();
	}
	
}

void DMNPCViewDialog::OnSelchangeWeaponCombo3() 
{

	int nCursor = m_cWeaponCombo_3.GetCurSel();

	DWORD dwWeaponID = m_cWeaponCombo_3.GetItemData(nCursor);

	cDNDObject *pInvItem = m_pNPC->FindObjectInInventory(dwWeaponID);

	if(pInvItem != NULL)
	{
		cDNDWeapon *pWeapon = NULL;

		pWeapon = m_pApp->m_WeaponsIndexedTypeArray[pInvItem->m_wTypeId];

		if(pWeapon != NULL)
		{
			pWeapon->CopyTo(&m_pNPC->m_SelectedWeapons[2]);
			m_pNPC->m_SelectedWeapons[2].m_dwObjectID = pInvItem->m_dwObjectID;
			m_pNPC->m_SelectedWeapons[2].m_nMagicAdj = pInvItem->m_nMagicAdj;
		}

		ProcessCharStats();

		Refresh();
	}
	
}

void DMNPCViewDialog::OnSelchangeWeaponCombo4() 
{

	int nCursor = m_cWeaponCombo_4.GetCurSel();

	DWORD dwWeaponID = m_cWeaponCombo_4.GetItemData(nCursor);

	cDNDObject *pInvItem = m_pNPC->FindObjectInInventory(dwWeaponID);

	if(pInvItem != NULL)
	{
		cDNDWeapon *pWeapon = NULL;

		pWeapon = m_pApp->m_WeaponsIndexedTypeArray[pInvItem->m_wTypeId];

		if(pWeapon != NULL)
		{
			pWeapon->CopyTo(&m_pNPC->m_SelectedWeapons[3]);
			m_pNPC->m_SelectedWeapons[3].m_dwObjectID = pInvItem->m_dwObjectID;
			m_pNPC->m_SelectedWeapons[3].m_nMagicAdj = pInvItem->m_nMagicAdj;
		}

		ProcessCharStats();

		Refresh();
	}
		
}

void DMNPCViewDialog::OnSelchangeAmmoCombo() 
{
	int nCursor = m_cAmmunitionCombo.GetCurSel();

	DWORD dwWeaponID = m_cAmmunitionCombo.GetItemData(nCursor);

	cDNDObject *pInvItem = m_pNPC->FindObjectInInventory(dwWeaponID);

	if(pInvItem != NULL)
	{
		cDNDWeapon *pWeapon = NULL;

		pWeapon = m_pApp->m_AmmunitionIndexedTypeArray[pInvItem->m_wTypeId];

		if(pWeapon == NULL && pInvItem->m_wTypeId == 0 || IsThrowableWeapon(pInvItem->m_nFlags)) // so we can clear it
		{
			pWeapon = m_pApp->m_WeaponsIndexedTypeArray[pInvItem->m_wTypeId];
		}

		if(pWeapon != NULL)
		{
			pWeapon->CopyTo(&m_pNPC->m_SelectedAmmo);
			m_pNPC->m_SelectedAmmo.m_dwObjectID = pInvItem->m_dwObjectID;
			m_pNPC->m_SelectedAmmo.m_nMagicAdj = pInvItem->m_nMagicAdj;
			
			if(pInvItem->m_lAmount == 0)
				pInvItem->m_lAmount = 1;

			m_pNPC->m_SelectedAmmo.m_lAmount = pInvItem->m_lAmount;
		}

		ProcessCharStats();

		Refresh();
	}
}

void DMNPCViewDialog::OnSelchangeRecoverAmmoCombo() 
{
	// OOPS didn't need this function
	
}

void DMNPCViewDialog::OnChangeWeaponPlusEdit1() 
{
	
	UpdateData(TRUE);

	if(m_pNPC->m_SelectedWeapons[0].m_wTypeId)
	{
		m_pNPC->m_SelectedWeapons[0].m_nMagicAdj = atoi(m_szWeaponPlus_1.GetBuffer(0));
	}
	else
	{
		m_pNPC->m_SelectedWeapons[0].m_nMagicAdj = 0;
	}


	cDNDObject *pInvItem = m_pNPC->FindObjectInInventory(m_pNPC->m_SelectedWeapons[0].m_dwObjectID);
	if(pInvItem != NULL)
	{
		pInvItem->m_nMagicAdj = m_pNPC->m_SelectedWeapons[0].m_nMagicAdj;
	}

	ProcessCharStats();

	Refresh();
	
}

void DMNPCViewDialog::OnChangeWeaponPlusEdit2() 
{
	UpdateData(TRUE);

	if(m_pNPC->m_SelectedWeapons[1].m_wTypeId)
	{
		m_pNPC->m_SelectedWeapons[1].m_nMagicAdj = atoi(m_szWeaponPlus_2.GetBuffer(0));
	}
	else
	{
		m_pNPC->m_SelectedWeapons[1].m_nMagicAdj = 0;
	}


	cDNDObject *pInvItem = m_pNPC->FindObjectInInventory(m_pNPC->m_SelectedWeapons[1].m_dwObjectID);
	if(pInvItem != NULL)
	{
		pInvItem->m_nMagicAdj = m_pNPC->m_SelectedWeapons[1].m_nMagicAdj;
	}

	ProcessCharStats();

	Refresh();
	
	
}

void DMNPCViewDialog::OnChangeWeaponPlusEdit3() 
{

	UpdateData(TRUE);

	if(m_pNPC->m_SelectedWeapons[2].m_wTypeId)
	{
		m_pNPC->m_SelectedWeapons[2].m_nMagicAdj = atoi(m_szWeaponPlus_3.GetBuffer(0));
	}
	else
	{
		m_pNPC->m_SelectedWeapons[2].m_nMagicAdj = 0;
	}


	cDNDObject *pInvItem = m_pNPC->FindObjectInInventory(m_pNPC->m_SelectedWeapons[2].m_dwObjectID);
	if(pInvItem != NULL)
	{
		pInvItem->m_nMagicAdj = m_pNPC->m_SelectedWeapons[2].m_nMagicAdj;
	}

	ProcessCharStats();

	Refresh();
	
	
}

void DMNPCViewDialog::OnChangeWeaponPlusEdit4() 
{

	UpdateData(TRUE);

	if(m_pNPC->m_SelectedWeapons[3].m_wTypeId)
	{
		m_pNPC->m_SelectedWeapons[3].m_nMagicAdj = atoi(m_szWeaponPlus_4.GetBuffer(0));
	}
	else
	{
		m_pNPC->m_SelectedWeapons[3].m_nMagicAdj = 0;
	}


	cDNDObject *pInvItem = m_pNPC->FindObjectInInventory(m_pNPC->m_SelectedWeapons[3].m_dwObjectID);
	if(pInvItem != NULL)
	{
		pInvItem->m_nMagicAdj = m_pNPC->m_SelectedWeapons[3].m_nMagicAdj;
	}

	ProcessCharStats();

	Refresh();
	
	
}

void DMNPCViewDialog::OnChangeAmmoPlusEdit() 
{
	
	UpdateData(TRUE);

	if(m_pNPC->m_SelectedAmmo.m_wTypeId)
	{
		m_pNPC->m_SelectedAmmo.m_nMagicAdj = atoi(m_szAmmoPlus.GetBuffer(0));
	}
	else
	{
		m_pNPC->m_SelectedAmmo.m_nMagicAdj = 0;
	}


	cDNDObject *pInvItem = m_pNPC->FindObjectInInventory(m_pNPC->m_SelectedAmmo.m_dwObjectID);
	if(pInvItem != NULL)
	{
		pInvItem->m_nMagicAdj = m_pNPC->m_SelectedAmmo.m_nMagicAdj;
	}

	ProcessCharStats();

	Refresh();
}


void DMNPCViewDialog::OnSelchangeCharArmorCombo() 
{
	UpdateData(TRUE);

	int nCursor = m_cArmorCombo.GetCurSel();

	DWORD dwArmorID = m_cArmorCombo.GetItemData(nCursor);

	cDNDObject *pInvItem = m_pNPC->FindObjectInInventory(dwArmorID);

	if(pInvItem != NULL)
	{
		cDNDArmor *pArmor = NULL;

		m_pApp->m_ArmorIndexedTypeMap.Lookup(pInvItem->m_wTypeId, pArmor);

		if(pArmor != NULL)
		{
			pArmor->CopyTo(&m_pNPC->m_ArmorWorn);
			m_pNPC->m_ArmorWorn.m_dwObjectID = pInvItem->m_dwObjectID;
			m_pNPC->m_ArmorWorn.m_nMagicAdj = pInvItem->m_nMagicAdj;

			pInvItem->m_dwContainerID = 0;
		}

		ProcessCharStats();

		Refresh();

		if(m_pInventoryDialog != NULL)
		{
			m_pInventoryDialog->RefreshAll();
		}
	}
	
}

void DMNPCViewDialog::OnSelchangeCharShieldCombo() 
{
	UpdateData(TRUE);

	int nCursor = m_cShieldCombo.GetCurSel();

	DWORD dwShieldID = m_cShieldCombo.GetItemData(nCursor);

	cDNDObject *pInvItem = m_pNPC->FindObjectInInventory(dwShieldID);

	if(pInvItem != NULL)
	{
		cDNDShield *pShield = NULL;

		m_pApp->m_ShieldsIndexedTypeMap.Lookup(pInvItem->m_wTypeId, pShield);

		if(pShield != NULL)
		{
			pShield->CopyTo(&m_pNPC->m_ShieldWorn);
			m_pNPC->m_ShieldWorn.m_dwObjectID = pInvItem->m_dwObjectID;
			m_pNPC->m_ShieldWorn.m_nMagicAdj = pInvItem->m_nMagicAdj;

			pInvItem->m_dwContainerID = 0;
		}

		ProcessCharStats();

		Refresh();

		if(m_pInventoryDialog != NULL)
		{
			m_pInventoryDialog->RefreshAll();
		}
	}
	
}

void DMNPCViewDialog::OnChangeArmorPlusEdit() 
{
	UpdateData(TRUE);

	if(m_pNPC->m_ArmorWorn.m_wTypeId)
	{
		m_pNPC->m_ArmorWorn.m_nMagicAdj = atoi(m_szArmorPlus.GetBuffer(0));
	}
	else
	{
		m_pNPC->m_ArmorWorn.m_nMagicAdj = 0;
	}


	cDNDObject *pInvItem = m_pNPC->FindObjectInInventory(m_pNPC->m_ArmorWorn.m_dwObjectID);
	if(pInvItem != NULL)
	{
		pInvItem->m_nMagicAdj = m_pNPC->m_ArmorWorn.m_nMagicAdj;
	}

	ProcessCharStats();

	Refresh();

}

void DMNPCViewDialog::OnChangeShieldPlusEdit() 
{

	UpdateData(TRUE);

	if(m_pNPC->m_ShieldWorn.m_wTypeId)
	{
		m_pNPC->m_ShieldWorn.m_nMagicAdj = atoi(m_szShieldPlus.GetBuffer(0));
	}
	else
	{
		m_pNPC->m_ShieldWorn.m_nMagicAdj = 0;
	}


	cDNDObject *pInvItem = m_pNPC->FindObjectInInventory(m_pNPC->m_ShieldWorn.m_dwObjectID);
	if(pInvItem != NULL)
	{
		pInvItem->m_nMagicAdj = m_pNPC->m_ShieldWorn.m_nMagicAdj;
	}

	ProcessCharStats();

	Refresh();

	
}

//ring ring
void DMNPCViewDialog::OnSelchangeCharRingLCombo() 
{
	UpdateData(TRUE);

	int nCursor = m_cRingLeftCombo.GetCurSel();

	DWORD dwRingID = m_cRingLeftCombo.GetItemData(nCursor);

	cDNDObject *pInvItem = m_pNPC->FindObjectInInventory(dwRingID);

	if(pInvItem != NULL)
	{
		cDNDRing *pRing = NULL;

		m_pApp->m_RingsIndexedTypeMap.Lookup(pInvItem->m_wTypeId, pRing);

		if(pRing != NULL)
		{
			pRing->CopyTo(&m_pNPC->m_RingsWorn[0]);
			m_pNPC->m_RingsWorn[0].m_dwObjectID = pInvItem->m_dwObjectID;
			m_pNPC->m_RingsWorn[0].m_nMagicAdj = pInvItem->m_nMagicAdj;

			if(m_pNPC->m_RingsWorn[0].m_dwObjectID == m_pNPC->m_RingsWorn[1].m_dwObjectID) //can't wear the same ring on two hands dork
			{
				cDNDRing _BlankRing;
				 _BlankRing.CopyTo(&m_pNPC->m_RingsWorn[1]);
				 m_pNPC->m_RingsWorn[1].m_dwObjectID = 0;
			}
			else
			{
				pInvItem->m_dwContainerID = 0;
			}

		}

		ProcessCharStats();

		Refresh();

		if(m_pInventoryDialog != NULL)
		{
			m_pInventoryDialog->RefreshAll();
		}
	}
	
}

void DMNPCViewDialog::OnSelchangeCharRingRCombo() 
{
	UpdateData(TRUE);

	int nCursor = m_cRingRightCombo.GetCurSel();

	DWORD dwRingID = m_cRingRightCombo.GetItemData(nCursor);

	cDNDObject *pInvItem = m_pNPC->FindObjectInInventory(dwRingID);

	if(pInvItem != NULL)
	{
		cDNDRing *pRing = NULL;

		m_pApp->m_RingsIndexedTypeMap.Lookup(pInvItem->m_wTypeId, pRing);

		if(pRing != NULL)
		{
			pRing->CopyTo(&m_pNPC->m_RingsWorn[1]);
			m_pNPC->m_RingsWorn[1].m_dwObjectID = pInvItem->m_dwObjectID;
			m_pNPC->m_RingsWorn[1].m_nMagicAdj = pInvItem->m_nMagicAdj;

			if(m_pNPC->m_RingsWorn[0].m_dwObjectID == m_pNPC->m_RingsWorn[1].m_dwObjectID) //can't wear the same ring on two hands dork
			{
				cDNDRing _BlankRing;
				 _BlankRing.CopyTo(&m_pNPC->m_RingsWorn[0]);
				 m_pNPC->m_RingsWorn[0].m_dwObjectID = 0;
			}
			else
			{
				pInvItem->m_dwContainerID = 0;
			}
		}

		ProcessCharStats();

		Refresh();

		if(m_pInventoryDialog != NULL)
		{
			m_pInventoryDialog->RefreshAll();
		}
	}
	
}


void DMNPCViewDialog::OnChangeRingLPlusEdit() 
{
	UpdateData(TRUE);

	if(m_pNPC->m_RingsWorn[0].m_wTypeId)
	{
		m_pNPC->m_RingsWorn[0].m_nMagicAdj = atoi(m_szRingLeftPlusEdit.GetBuffer(0));
	}
	else
	{
		m_pNPC->m_RingsWorn[0].m_nMagicAdj = 0;
	}


	cDNDObject *pInvItem = m_pNPC->FindObjectInInventory(m_pNPC->m_RingsWorn[0].m_dwObjectID);
	if(pInvItem != NULL)
	{
		pInvItem->m_nMagicAdj = m_pNPC->m_RingsWorn[0].m_nMagicAdj;
	}

	ProcessCharStats();

	Refresh();
	
}

void DMNPCViewDialog::OnChangeRingRPlusEdit() 
{
	UpdateData(TRUE);

	if(m_pNPC->m_RingsWorn[1].m_wTypeId)
	{
		m_pNPC->m_RingsWorn[1].m_nMagicAdj = atoi(m_szRingRightPlusEdit.GetBuffer(0));
	}
	else
	{
		m_pNPC->m_RingsWorn[1].m_nMagicAdj = 0;
	}


	cDNDObject *pInvItem = m_pNPC->FindObjectInInventory(m_pNPC->m_RingsWorn[1].m_dwObjectID);
	if(pInvItem != NULL)
	{
		pInvItem->m_nMagicAdj = m_pNPC->m_RingsWorn[1].m_nMagicAdj;
	}

	ProcessCharStats();

	Refresh();
	
}

void DMNPCViewDialog::SwapWeapon()
{
	OnSwapButton1();
}

void DMNPCViewDialog::OnSwapButton1() 
{
	cDNDWeapon swapWeapon;

	m_pNPC->m_SelectedWeapons[0].CopyFull(&swapWeapon);

	m_pNPC->m_SelectedWeapons[1].CopyFull(&m_pNPC->m_SelectedWeapons[0]);

	swapWeapon.CopyFull(&m_pNPC->m_SelectedWeapons[1]);

	ProcessCharStats();

	Refresh();	

	OnSelchangeWeaponCombo1();

	for (int i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
	{
		if (m_pNPC->m_Inventory[i].m_dwObjectID == m_pNPC->m_SelectedWeapons[0].m_dwObjectID &&  m_pNPC->m_SelectedWeapons[0].m_dwObjectID != 0)
		{
			m_pApp->PlayEquipItemSFX(m_pNPC->m_Inventory[i].m_szType, m_pNPC->m_SelectedWeapons[0].m_szType);
			break;
		}
	}
}

void DMNPCViewDialog::OnSwapButton2() 
{
	
	cDNDWeapon swapWeapon;

	m_pNPC->m_SelectedWeapons[1].CopyFull(&swapWeapon);

	m_pNPC->m_SelectedWeapons[2].CopyFull(&m_pNPC->m_SelectedWeapons[1]);

	swapWeapon.CopyFull(&m_pNPC->m_SelectedWeapons[2]);

	ProcessCharStats();

	Refresh();	
}

void DMNPCViewDialog::OnSwapButton3() 
{
	cDNDWeapon swapWeapon;

	m_pNPC->m_SelectedWeapons[2].CopyFull(&swapWeapon);

	m_pNPC->m_SelectedWeapons[3].CopyFull(&m_pNPC->m_SelectedWeapons[2]);

	swapWeapon.CopyFull(&m_pNPC->m_SelectedWeapons[3]);

	ProcessCharStats();

	Refresh();	
}

void DMNPCViewDialog::OnAddHpButton() 
{
	int nValue = 0;

	ModifyValue((int *)&nValue, "Modify Hit Points:", 0);

	m_pNPC->m_nCurrentDamage += -nValue;

	if(m_pNPC->m_nCurrentDamage < 0)
		m_pNPC->m_nCurrentDamage = 0;

	ProcessCharStats();

	Refresh();
	
}

void DMNPCViewDialog::FireAmmo() 
{

	for(int i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
	{
		if(m_pNPC->m_Inventory[i].m_wTypeId == m_pNPC->m_SelectedAmmo.m_wTypeId && m_pNPC->m_Inventory[i].m_lAmount == m_pNPC->m_SelectedAmmo.m_lAmount && m_pNPC->m_Inventory[i].m_nMagicAdj == m_pNPC->m_SelectedAmmo.m_nMagicAdj)
		{
			m_pNPC->m_Inventory[i].m_lAmount -= 1;

			AddToRecoveryInventory(i, 1);

			if(m_pNPC->m_Inventory[i].m_lAmount <= 0)
			{
				cDNDObject _BlankObj;
				_BlankObj.CopyFull(&m_pNPC->m_Inventory[i]);
				
				m_pNPC->ValidateInventory();
			}

			OnSelchangeWeaponCombo1();

			Refresh();

			break;
		}
	}

	if(m_cRecoverAmmoCombo.GetCurSel() == -1)
	{
		m_cRecoverAmmoCombo.SetCurSel(0);
	}

}

void DMNPCViewDialog::AddToRecoveryInventory(int nCharInventorySlot, int nAmount)
{
	for(int i = 0; i< MAX_CHARACTER_INVENTORY; ++i)
	{
		if(m_RecoverAmmoInventory[i].m_wTypeId == 0)
		{
			m_pNPC->m_Inventory[nCharInventorySlot].CopyTo(&m_RecoverAmmoInventory[i]);

			m_RecoverAmmoInventory[i].m_nMagicAdj = m_pNPC->m_Inventory[nCharInventorySlot].m_nMagicAdj;
			m_RecoverAmmoInventory[i].m_lAmount = nAmount;
			
			ValidateRecoveryInventory();

			return;
		}
	}
}

void DMNPCViewDialog::ValidateRecoveryInventory()
{
	cDNDObject _Inventory[MAX_CHARACTER_INVENTORY];

	CollapseRecoveryInventory();

	int nCount = 0;

	for(int i = 0; i< MAX_CHARACTER_INVENTORY; ++i)
	{
		if(m_RecoverAmmoInventory[i].m_wTypeId != 0)
		{
			m_RecoverAmmoInventory[i].CopyFull(&_Inventory[nCount]);

			++nCount;
		}
	}

	memcpy(m_RecoverAmmoInventory, _Inventory, MAX_CHARACTER_INVENTORY * sizeof(cDNDObject));
}

void DMNPCViewDialog::CollapseRecoveryInventory()
{
	cDNDObject _BlankObj;
		
	for(int i = 0; i< MAX_CHARACTER_INVENTORY-1; ++i)
	{
		if(m_RecoverAmmoInventory[i].m_wTypeId == 0)
			continue;

		for(int j = i+1; j< MAX_CHARACTER_INVENTORY; ++j)
		{
			if(m_RecoverAmmoInventory[j].m_wTypeId == 0)
				continue;

			if(m_RecoverAmmoInventory[i].m_wTypeId == m_RecoverAmmoInventory[j].m_wTypeId && m_RecoverAmmoInventory[i].m_nMagicAdj == m_RecoverAmmoInventory[j].m_nMagicAdj)
			{
				m_RecoverAmmoInventory[i].m_lAmount += m_RecoverAmmoInventory[j].m_lAmount;
				_BlankObj.CopyFull(&m_RecoverAmmoInventory[j]);
			}
		}
	}

}

void DMNPCViewDialog::OnRecoverAmmo() 
{
	cDNDObject _BlankObj;

	int nCursor = m_cRecoverAmmoCombo.GetCurSel();

	if(nCursor > -1)
	{
		int nRecovered = 0;

		ModifyValue((int *)&nRecovered, "Recover how many ?", FALSE);

		cDNDObject *pObject = (cDNDObject *)m_cRecoverAmmoCombo.GetItemData(nCursor);

		if(nRecovered > pObject->m_lAmount)
			nRecovered = pObject->m_lAmount;

		BOOL bRecovered = FALSE;
		BOOL bFoundIt = FALSE;
		for(int i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
		{
			if(m_pNPC->m_Inventory[i].m_wTypeId == pObject->m_wTypeId && m_pNPC->m_Inventory[i].m_nMagicAdj == pObject->m_nMagicAdj)
			{
				bFoundIt = TRUE;
				m_pNPC->m_Inventory[i].m_lAmount += nRecovered;
				bRecovered = TRUE;
				break;
			}
		}

		if(bFoundIt == FALSE)
		{
			int nSlot = MAX_CHARACTER_INVENTORY-1;

			if(m_pNPC->m_Inventory[nSlot].m_wTypeId == 0)
			{
				pObject->CopyFull(&m_pNPC->m_Inventory[nSlot]);

				m_pNPC->m_Inventory[nSlot].m_lAmount = nRecovered;

				m_pNPC->m_Inventory[nSlot].m_dwObjectID = GetUniqueID();

				bRecovered = TRUE;
			}
		}

		if(bRecovered)
		{
			_BlankObj.CopyFull(pObject);

			ValidateRecoveryInventory();

			m_pNPC->ValidateInventory();

			OnSelchangeWeaponCombo1();

			Refresh();
		}

	}
	
}

void DMNPCViewDialog::OnClose() 
{
	if(m_pApp->m_pBaseCopyCharacterDialog == this)
	{
		m_pApp->m_pBaseCopyCharacterDialog = NULL;
		m_pApp->m_bCharacterCopyBufferIsNPC = FALSE;
	}

	if(m_pBaseNPCViewDialog != NULL)
	{
		m_pBaseNPCViewDialog->ProcessCharStats(); // WTF ?
	}

	if(m_pInventoryDialog != NULL)
	{
		m_pInventoryDialog->PostMessage(WM_CLOSE);
	}

	if(m_pSpellsDialog != NULL)
	{
		m_pSpellsDialog->m_pSiblingWindow = NULL;
		m_pSpellsDialog->PostMessage(WM_CLOSE);
	}
	
	CDialog::OnClose();

	delete this;
}

void DMNPCViewDialog::OnSelchangeCharHelmCombo() 
{
	UpdateData(TRUE);

	int nCursor = m_cHelmCombo.GetCurSel();

	DWORD dwHelmID = m_cHelmCombo.GetItemData(nCursor);

	cDNDObject *pInvItem = m_pNPC->FindObjectInInventory(dwHelmID);

	if(pInvItem != NULL)
	{
		cDNDObject *pHelmet = m_pApp->m_HelmetsIndexedTypeArray.GetAt(pInvItem->m_wTypeId);

		if(pHelmet != NULL)
		{
			pHelmet->CopyTo(&m_pNPC->m_HelmWorn);
			m_pNPC->m_HelmWorn.m_dwObjectID = pInvItem->m_dwObjectID;
			m_pNPC->m_HelmWorn.m_nMagicAdj = pInvItem->m_nMagicAdj;

			pInvItem->m_dwContainerID = 0;
		}
		else
		{
			cDNDObject _BlankHelmet;
			strcpy(_BlankHelmet.m_szType, "none");
			_BlankHelmet.CopyFull(&m_pNPC->m_HelmWorn);
		}

		ProcessCharStats();

		Refresh();

		if(m_pInventoryDialog != NULL)
		{
			m_pInventoryDialog->RefreshAll();
		}
	}
	
}



void DMNPCViewDialog::OnCbnSelchangeCharSexCombo()
{
	UpdateData(TRUE);

	m_pNPC->m_nSex = m_cCharSexCombo.GetCurSel();

	Refresh();
}


void DMNPCViewDialog::OnStnClickedNpcPortraitButton()
{
	PDNDMONSTERMANUALENTRY pMonster = NULL;

	m_pApp->m_MonsterManualIndexedMap.Lookup(m_pNPC->m_nMonsterIndex, pMonster);

	if (pMonster == NULL)
	{
		return;
	}

	CString szSourcePath;

	CString szDirectory;
	szDirectory.Format("%s//Data//monster_pictures", m_pApp->m_szEXEPath.GetBuffer(0));

	ZeroMemory(&g_ofn, sizeof(OPENFILENAME));
	g_ofn.lStructSize = sizeof(OPENFILENAME);
	g_ofn.nMaxFile = MAX_PATH;
	g_ofn.nMaxFileTitle = MAX_PATH;
	g_ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	g_ofn.hwndOwner = m_hWnd;
	g_ofn.hInstance = m_pApp->m_hInstance;
	g_ofn.lpstrFile = g_NPCV_szFilename;
	g_ofn.lpstrInitialDir = szDirectory.GetBuffer(0);
	g_ofn.lpstrTitle = "Load Portrait Bitmap File";
	//g_ofn.lpstrFilter = "Graphics Files (*.bmp)\0*.bmp\0(*.gif)\0*.gif\0(*.jpg)\0*.jpg\0All Files (*.*)\0*.*\0\0";
	g_ofn.lpstrFilter = "Graphics Files (*.bmp)\0*.bmp\0(*.gif)\0*.gif\0(*.jpg)\0*.jpg\0(*.png)\0*.png\0All Files (*.*)\0*.*\0\0";
	g_ofn.lpstrDefExt = "bmp";

	// Get action template file name
	if (GetOpenFileName(&g_ofn))
	{
		szSourcePath = g_NPCV_szFilename;
		szSourcePath.MakeLower();

		if (m_pNPCPortraitBitmap != NULL)
		{
			::delete m_pNPCPortraitBitmap;
			m_pNPCPortraitBitmap = NULL;

			Refresh();
			InvalidateRect(NULL);
		}
		
		CDMNPCPortraitSelectDialog *pDlg = new CDMNPCPortraitSelectDialog("Portrait", szSourcePath, "data\\monster_pictures\\", pMonster->m_szMMName, m_pNPC->m_szCharacterName);
		pDlg->DoModal();
		delete pDlg;


		ProcessCharStats();

		InvalidateRect(NULL);
	}
}


void DMNPCViewDialog::OnStnDblclickDescComment()
{
	TRACE("POON %d %s!", m_nMonsterBook, m_szMonsterBook);

	CString szPage = m_szMonsterBook;

	szPage.Replace("MM pg.", "");
	szPage.Replace("FF pg.", "");
	szPage.Replace("MM2 pg.", "");

	int nPage = atoi(szPage.GetBuffer(0));

	switch (m_nMonsterBook)
	{
		case 1:	m_pMainDialog->OpenPDFDocument("PDF\\MonsterManual.pdf", nPage+1); break;
		case 2:	m_pMainDialog->OpenPDFDocument("PDF\\MonsterManual2.pdf", nPage+1); break;
		case 3:	m_pMainDialog->OpenPDFDocument("PDF\\FiendFolio.pdf", nPage+1); break;
	}

}


void DMNPCViewDialog::OnStnDblclickTtComment()
{
	cDMChartLookupDialog *pDlg = new cDMChartLookupDialog(DND_CHART_TREASURE_TYPES);
	pDlg->DoModal();
	delete pDlg;
}

LRESULT DMNPCViewDialog::OnDirtyWindow(UINT wParam, LONG lParam)
{
	ProcessCharStats();
	Refresh();
	return 0; // I handled this message
}
