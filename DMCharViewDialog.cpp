// DMCharViewDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DM HelperDlg.h"
#include "DMLoadFileDialog.h"
#include "DMFileManagementDialog.h"
#include "DMInventoryDialog.h"
#include "DMCharSpellsDialog.h"
#include "DMCharViewDialog.h"
#include "DMCharSheetDialog.h"
#include "DMPCDetailsDialog.h"
#include "DMModifyValueDialog.h"
#include "DMHPEditDialog.h"
#include "DMNPCViewDialog.h"
#include "DMLanguageSelectDialog.h"
#include "cDMMapViewDialog.h"
#include "cDMStrikeOrThrowDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static OPENFILENAME    g_ofn;
char g_CV_szFilename[MAX_PATH];

#if CUSTOM_CLASSES
#define NUM_CHARVIEW_CLASSES 27
#else
#define NUM_CHARVIEW_CLASSES 14
#endif

int nClasses[NUM_CHARVIEW_CLASSES] =
{
	DND_CHARACTER_CLASS_UNDEF,

	DND_CHARACTER_CLASS_NONE,
	DND_CHARACTER_CLASS_FIGHTER, 
	DND_CHARACTER_CLASS_RANGER,
	DND_CHARACTER_CLASS_CAVALIER,
	DND_CHARACTER_CLASS_PALADIN,
	DND_CHARACTER_CLASS_BARBARIAN,

	DND_CHARACTER_CLASS_CLERIC,
	DND_CHARACTER_CLASS_DRUID,
	DND_CHARACTER_CLASS_MONK,

	DND_CHARACTER_CLASS_MAGE,
	DND_CHARACTER_CLASS_ILLUSIONIST,

	DND_CHARACTER_CLASS_THIEF,
	DND_CHARACTER_CLASS_THIEF_ACROBAT,
	DND_CHARACTER_CLASS_ASSASSIN,

	#if CUSTOM_CLASSES
	DND_CHARACTER_CLASS_CUSTOM_1,
	DND_CHARACTER_CLASS_CUSTOM_2,
	DND_CHARACTER_CLASS_CUSTOM_3,
	DND_CHARACTER_CLASS_CUSTOM_4,
	DND_CHARACTER_CLASS_CUSTOM_5,
	DND_CHARACTER_CLASS_CUSTOM_6,
	DND_CHARACTER_CLASS_CUSTOM_7,
	DND_CHARACTER_CLASS_CUSTOM_8,
	DND_CHARACTER_CLASS_CUSTOM_9,
	DND_CHARACTER_CLASS_CUSTOM_10,
	DND_CHARACTER_CLASS_CUSTOM_11,
	DND_CHARACTER_CLASS_CUSTOM_12,
	#endif

};

////Base Class Destructor
CDMBaseCharViewDialog::~CDMBaseCharViewDialog()
{
	if(m_pApp != NULL)
	{
		WORD wID;
		for (POSITION pos = m_pApp->m_NPCViewMap.GetStartPosition(); pos != NULL; )
		{
			PDNDNPCVIEWDLG pNPCDlg = NULL;
			m_pApp->m_NPCViewMap.GetNextAssoc(pos,wID,pNPCDlg);

			if(pNPCDlg != NULL && pNPCDlg->m_pTargetBaseDlg == this)
			{
				pNPCDlg->m_pTargetBaseDlg  = NULL;
			}
		}

		for (POSITION pos = m_pApp->m_CharacterViewMap.GetStartPosition(); pos != NULL; )
		{
			PDNDCHARVIEWDLG pCharDlg = NULL;
			m_pApp->m_CharacterViewMap.GetNextAssoc(pos,wID,pCharDlg);

			if(pCharDlg != NULL && pCharDlg->m_pTargetBaseDlg == this)
			{
				pCharDlg->m_pTargetBaseDlg  = NULL;
			}
		}
	}
	
}


BOOL CDMBaseCharViewDialog::GetMonsterPortraitPath(CString szMonster)
{
	m_szMonsterPortraitPath = _T("");

	if (szMonster == _T(""))
	{
		return FALSE;
	}

	CString szPath = m_pApp->m_szEXEPath + "\\data\\monster_pictures\\" + szMonster + ".*";

	CFileFind find;
	CString strFileName;

	BOOL bFound = find.FindFile(szPath);

	while (bFound)
	{
		bFound = find.FindNextFile();
		CString strFileName;

		if (find.IsDirectory())
		{
			strFileName = find.GetFileName();

			if (strFileName == _T(".") || strFileName == _T(".."))
			{
				continue;
			}
		}
		else
		{
			strFileName = find.GetFileName();

			strFileName.MakeLower();

			if (strFileName.Find(".bmp") >= 0 || strFileName.Find(".jpg") >= 0 || strFileName.Find(".png") >= 0 || strFileName.Find(".gif") >= 0)
			{
				m_szMonsterPortraitPath = m_pApp->m_szEXEPath + "\\data\\monster_pictures\\" + strFileName;

				TRACE("GOT %s\n", strFileName);

				return TRUE;
			}
		}
	}

	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
// CDMCharViewDialog dialog


CDMCharViewDialog::CDMCharViewDialog(CDMHelperDlg* pMainDialog, cDNDCharacter	*_pCharacter, CWnd* pParent /*=NULL*/)
	: CDMBaseCharViewDialog(DND_CHAR_VIEW_TYPE_PC, CDMCharViewDialog::IDD, pParent)
	, m_szEarnedXP(_T(""))
	, m_szXPValueComment(_T(""))
	, m_szXPValue(_T(""))
	, m_szTurnUndead1(_T(""))
	, m_szTurnUndead2(_T(""))
	, m_szTurnUndead3(_T(""))
	, m_szTurnUndead4(_T(""))
	, m_szTurnUndead5(_T(""))
	, m_szTurnUndead6(_T(""))
	, m_szTurnUndead7(_T(""))
	, m_szTurnUndead8(_T(""))
	, m_szTurnUndead9(_T(""))
	, m_szTurnUndead10(_T(""))
	, m_szTurnUndead11(_T(""))
	, m_szTurnUndead12(_T(""))
	, m_szTurnUndead13(_T(""))
	, m_bKeenEar(FALSE)
	, m_bVeryKeenEar(FALSE)
	, m_szSecondarySkill(_T(""))
	, m_szConcealedMove(_T(""))
	, m_szClimbingMove(_T(""))
	, m_szSpecialMove(_T(""))
{
	//{{AFX_DATA_INIT(CDMCharViewDialog)
	m_szCharacterName = _T("");
	m_szSTREdit = _T("");
	m_szINTEdit = _T("");
	m_szWISEdit = _T("");
	m_szDEXEdit = _T("");
	m_szCONEdit = _T("");
	m_szCHAEdit = _T("");
	m_szCOMEdit = _T("");
	m_szExceptionalStrength = _T("");
	m_szLevelEdit1 = _T("");
	m_szLevelEdit2 = _T("");
	m_szLevelEdit3 = _T("");
	m_szXPEdit1 = _T("");
	m_szXPEdit2 = _T("");
	m_szXPEdit3 = _T("");
	m_szHitPoints = _T("");
	m_szCurrentHitPoints = _T("");
	m_szHPByLevel = _T("");
	m_bShowHPCheck = FALSE;
	m_szCONComment = _T("");
	m_szDEXComment = _T("");
	m_szSTRComment = _T("");
	m_szArmorClass = _T("");
	m_szArmorPlus = _T("");
	m_szShieldPlus = _T("");
	m_szINTComment = _T("");
	m_szSaves_1 = _T("-");
	m_szSaves_2 = _T("-");
	m_szSaves_3 = _T("-");
	m_szSaves_4 = _T("-");
	m_szSaves_5 = _T("-");
	m_szWISComment = _T("");
	m_szCHAComment = _T("");
	m_szThiefSkill_1 = _T("-");
	m_szThiefSkill_2 = _T("-");
	m_szThiefSkill_3 = _T("-");
	m_szThiefSkill_4 = _T("-");
	m_szThiefSkill_5 = _T("-");
	m_szThiefSkill_6 = _T("-");
	m_szThiefSkill_7 = _T("-");
	m_szThiefSkill_8 = _T("-");
	m_szWeaponPlus_1 = _T("");
	m_szWeaponPlus_2 = _T("");
	m_szWeaponPlus_3 = _T("");
	m_szWeaponPlus_4 = _T("");
	m_szCharNameStatic = _T("Character Name:");
	m_szAmmoPlus = _T("");
	m_szNumAttacks = _T("");
	m_szSpellsRemaining = _T("");
	m_szRingLeftPlusEdit = _T("");
	m_szRingRightPlusEdit = _T("");
	m_szAssEdit1 = _T("0");
	m_szAssEdit2 = _T("0");
	m_szAssEdit3 = _T("0");
	m_szAssEdit4 = _T("0");
	m_szAssEdit5 = _T("0");
	m_szAssEdit6 = _T("0");
	m_szAssEdit7 = _T("0");
	m_szAssEdit8 = _T("0");
	m_szAssEdit10 = _T("0");
	m_szAssEdit9 = _T("0");
	m_bNPCCheck = FALSE;
	m_bMonsterCheck = FALSE;
	m_szRaceStatic = _T("Race:");
	m_szMove = _T("");
	m_szSpecialAttackComment = _T("");
	m_szSpecialDefenseComment = _T("");
	//}}AFX_DATA_INIT

	//m_pApp = (CDMHelperApp *)AfxGetApp();

	m_pMainDialog = pMainDialog;

	m_pCharacter = _pCharacter;

	if(m_pCharacter == NULL)
	{
		m_pCharacter = new cDNDCharacter();
	}

	//if (strcmp(m_pCharacter->m_szCharacterName, "Elric") == 0)
	//{
	//	TRACE("!");
	//	m_pCharacter->m_bPocketPC;
	//}

	m_pParent = pParent;

	m_bHasPainted = FALSE;

	m_pCharSheetDialog = NULL;
	m_pSpellsDialog = NULL;
	m_pHPEditDialog = NULL;

	m_nSelectedSpellViewClass = 0;
	m_bFirstSpellViewRefresh = TRUE;

	m_bLoadedFromMenu = FALSE;

	m_szCharacterFirstName = _T("");

	m_szSaveFileName = _T("");
	m_szLoadFileName = _T("");
	m_szDamageDesc = _T("");  //for party view

	m_nPage = 0;
	m_nLastPage = 0;

	m_fXPScratchPad = 0.0f;

	m_nCharacterMovementRate = 0;
	m_nCalculatedXPValue = 0;

	m_pInventoryDialog = NULL;
	m_pSpellsDialog = NULL;

	m_bReversedMapIcon = FALSE;

	m_szRaceSex = "";
	m_szClassList = "";
	m_szLevelList = "";
	m_szDamageStat = "";
	m_szCharStats = "";

	if (m_pApp->m_hInstance != NULL) // unit test support
	{
		Create(CDMCharViewDialog::IDD, pParent);
	}
	
}


void CDMCharViewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDMCharViewDialog)
	DDX_Control(pDX, IDC_NPC_CHECK, m_cNPCCheck);
	DDX_Control(pDX, IDC_MONSTER_CHECK, m_cMonsterCheck);
	DDX_Control(pDX, IDC_CHAR_RING_R_COMBO, m_cRingRightCombo);
	DDX_Control(pDX, IDC_CHAR_RING_L_COMBO, m_cRingLeftCombo);
	DDX_Control(pDX, IDC_RECOVER_AMMO_COMBO, m_cRecoverAmmoCombo);
	DDX_Control(pDX, IDC_SPELLS_BUTTON, m_cSpellsButton);
	DDX_Control(pDX, IDC_AMMO_COMBO, m_cAmmunitionCombo);
	DDX_Control(pDX, IDC_ROLL_STATS, m_cRollStats);
	DDX_Control(pDX, IDC_ANCHOR_BUTTON1, m_cAnchorButton);
	DDX_Control(pDX, IDC_WEAPON_COMBO_4, m_cWeaponCombo_4);
	DDX_Control(pDX, IDC_WEAPON_COMBO_3, m_cWeaponCombo_3);
	DDX_Control(pDX, IDC_WEAPON_COMBO_2, m_cWeaponCombo_2);
	DDX_Control(pDX, IDC_WEAPON_COMBO_1, m_cWeaponCombo_1);
	DDX_Control(pDX, IDC_XP_EDIT_3, m_cXPEdit3);
	DDX_Control(pDX, IDC_XP_EDIT_2, m_cXPEdit2);
	DDX_Control(pDX, IDC_LEVEL_EDIT_3, m_cLevelEdit3);
	DDX_Control(pDX, IDC_LEVEL_EDIT_2, m_cLevelEdit2);
	DDX_Control(pDX, IDC_HP_CURRENT_EDIT, m_cCurrentEdit);
	DDX_Control(pDX, IDC_CHAR_SHIELD_COMBO, m_cShieldCombo);
	DDX_Control(pDX, IDC_AC_EDIT, m_cArmorClassEdit);
	DDX_Control(pDX, IDC_CHAR_ARMOR_COMBO, m_cArmorCombo);
	DDX_Control(pDX, IDC_CHAR_ALIGNMENT_COMBO, m_cAlignmentCombo);
	DDX_Control(pDX, IDC_STR_COMMENT, m_cSTRComment);
	DDX_Control(pDX, IDC_WEAPON_CHART_LIST, m_cWeaponChartList);
	DDX_Control(pDX, IDC_EX_STR_EDIT, m_cExceptionalStrengthEdit);
	DDX_Control(pDX, IDC_CHAR_SEX_COMBO, m_cCharSexCombo);
	DDX_Control(pDX, IDC_STR_EDIT, m_cSTREdit);
	DDX_Control(pDX, IDC_INT_EDIT, m_cINTEdit);
	DDX_Control(pDX, IDC_WIS_EDIT, m_cWISEdit);
	DDX_Control(pDX, IDC_DEX_EDIT, m_cDEXEdit);
	DDX_Control(pDX, IDC_CON_EDIT, m_cCONEdit);
	DDX_Control(pDX, IDC_CHA_EDIT, m_cCHAEdit);
	DDX_Control(pDX, IDC_COM_EDIT, m_cCOMEdit);
	DDX_Control(pDX, IDC_CHAR_CLASS_COMBO_3, m_cClassCombo_3);
	DDX_Control(pDX, IDC_CHAR_CLASS_COMBO_2, m_cClassCombo_2);
	DDX_Control(pDX, IDC_CHAR_CLASS_COMBO_1, m_cClassCombo_1);
	DDX_Control(pDX, IDC_CHAR_RACE_COMBO, m_cCharRaceCombo);
	DDX_Text(pDX, IDC_CHAR_NAME_EDIT, m_szCharacterName);
	DDX_Text(pDX, IDC_STR_EDIT, m_szSTREdit);
	DDX_Text(pDX, IDC_INT_EDIT, m_szINTEdit);
	DDX_Text(pDX, IDC_WIS_EDIT, m_szWISEdit);
	DDX_Text(pDX, IDC_DEX_EDIT, m_szDEXEdit);
	DDX_Text(pDX, IDC_CON_EDIT, m_szCONEdit);
	DDX_Text(pDX, IDC_CHA_EDIT, m_szCHAEdit);
	DDX_Text(pDX, IDC_COM_EDIT, m_szCOMEdit);
	DDX_Text(pDX, IDC_EX_STR_EDIT, m_szExceptionalStrength);
	DDX_Text(pDX, IDC_LEVEL_EDIT_1, m_szLevelEdit1);
	DDX_Text(pDX, IDC_LEVEL_EDIT_2, m_szLevelEdit2);
	DDX_Text(pDX, IDC_LEVEL_EDIT_3, m_szLevelEdit3);
	DDX_Text(pDX, IDC_XP_EDIT_1, m_szXPEdit1);
	DDX_Text(pDX, IDC_XP_EDIT_2, m_szXPEdit2);
	DDX_Text(pDX, IDC_XP_EDIT_3, m_szXPEdit3);
	DDX_Text(pDX, IDC_HP_EDIT, m_szHitPoints);
	DDX_Text(pDX, IDC_HP_CURRENT_EDIT, m_szCurrentHitPoints);
	DDX_Text(pDX, IDC_HP_BY_LEVEL, m_szHPByLevel);
	DDX_Check(pDX, IDC_SHOW_HP_CHECK, m_bShowHPCheck);
	DDX_Text(pDX, IDC_CON_COMMENT, m_szCONComment);
	DDX_Text(pDX, IDC_DEX_COMMENT, m_szDEXComment);
	DDX_Text(pDX, IDC_STR_COMMENT, m_szSTRComment);
	DDX_Text(pDX, IDC_AC_EDIT, m_szArmorClass);
	DDX_Text(pDX, IDC_ARMOR_PLUS_EDIT, m_szArmorPlus);
	DDX_Text(pDX, IDC_SHIELD_PLUS_EDIT, m_szShieldPlus);
	DDX_Text(pDX, IDC_INT_COMMENT, m_szINTComment);
	DDX_Text(pDX, IDC_SAVE_EDIT_1, m_szSaves_1);
	DDX_Text(pDX, IDC_SAVE_EDIT_2, m_szSaves_2);
	DDX_Text(pDX, IDC_SAVE_EDIT_3, m_szSaves_3);
	DDX_Text(pDX, IDC_SAVE_EDIT_4, m_szSaves_4);
	DDX_Text(pDX, IDC_SAVE_EDIT_5, m_szSaves_5);
	DDX_Text(pDX, IDC_WIS_COMMENT, m_szWISComment);
	DDX_Text(pDX, IDC_CHA_COMMENT, m_szCHAComment);
	DDX_Text(pDX, IDC_THIEF_SKILL_EDIT_1, m_szThiefSkill_1);
	DDX_Text(pDX, IDC_THIEF_SKILL_EDIT_2, m_szThiefSkill_2);
	DDX_Text(pDX, IDC_THIEF_SKILL_EDIT_3, m_szThiefSkill_3);
	DDX_Text(pDX, IDC_THIEF_SKILL_EDIT_4, m_szThiefSkill_4);
	DDX_Text(pDX, IDC_THIEF_SKILL_EDIT_5, m_szThiefSkill_5);
	DDX_Text(pDX, IDC_THIEF_SKILL_EDIT_6, m_szThiefSkill_6);
	DDX_Text(pDX, IDC_THIEF_SKILL_EDIT_7, m_szThiefSkill_7);
	DDX_Text(pDX, IDC_THIEF_SKILL_EDIT_8, m_szThiefSkill_8);
	DDX_Text(pDX, IDC_WEAPON_PLUS_EDIT_1, m_szWeaponPlus_1);
	DDX_Text(pDX, IDC_WEAPON_PLUS_EDIT_2, m_szWeaponPlus_2);
	DDX_Text(pDX, IDC_WEAPON_PLUS_EDIT_3, m_szWeaponPlus_3);
	DDX_Text(pDX, IDC_WEAPON_PLUS_EDIT_4, m_szWeaponPlus_4);
	DDX_Text(pDX, IDC_CHAR_NAME_STATIC, m_szCharNameStatic);
	DDX_Text(pDX, IDC_AMMO_PLUS_EDIT, m_szAmmoPlus);
	DDX_Text(pDX, IDC_NUM_ATTACKS_EDIT, m_szNumAttacks);
	DDX_Text(pDX, IDC_RING_L_PLUS_EDIT, m_szRingLeftPlusEdit);
	DDX_Text(pDX, IDC_RING_R_PLUS_EDIT, m_szRingRightPlusEdit);
	DDX_Text(pDX, IDC_ASS_SKILL_EDIT_1, m_szAssEdit1);
	DDX_Text(pDX, IDC_ASS_SKILL_EDIT_2, m_szAssEdit2);
	DDX_Text(pDX, IDC_ASS_SKILL_EDIT_3, m_szAssEdit3);
	DDX_Text(pDX, IDC_ASS_SKILL_EDIT_4, m_szAssEdit4);
	DDX_Text(pDX, IDC_ASS_SKILL_EDIT_5, m_szAssEdit5);
	DDX_Text(pDX, IDC_ASS_SKILL_EDIT_6, m_szAssEdit6);
	DDX_Text(pDX, IDC_ASS_SKILL_EDIT_7, m_szAssEdit7);
	DDX_Text(pDX, IDC_ASS_SKILL_EDIT_8, m_szAssEdit8);
	DDX_Text(pDX, IDC_ASS_SKILL_EDIT_10, m_szAssEdit10);
	DDX_Text(pDX, IDC_ASS_SKILL_EDIT_9, m_szAssEdit9);
	DDX_Check(pDX, IDC_NPC_CHECK, m_bNPCCheck);
	DDX_Check(pDX, IDC_MONSTER_CHECK, m_bMonsterCheck);
	DDX_Text(pDX, IDC_RACE_STATIC, m_szRaceStatic);
	DDX_Text(pDX, IDC_MOVE_EDIT, m_szMove);
	DDX_Text(pDX, IDC_SPECIAL_ATTACK_COMMENT, m_szSpecialAttackComment);
	DDX_Text(pDX, IDC_SPECIAL_DEFENSE_COMMENT, m_szSpecialDefenseComment);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHAR_HELM_COMBO, m_cHelmCombo);
	DDX_Text(pDX, IDC_EXP_EDIT, m_szEarnedXP);
	DDX_Control(pDX, IDC_CURRENT_CLASS_CHECK_1, m_cCurrentClassCheck1);
	DDX_Control(pDX, IDC_CURRENT_CLASS_CHECK_2, m_cCurrentClassCheck2);
	DDX_Control(pDX, IDC_CURRENT_CLASS_CHECK_3, m_cCurrentClassCheck3);
	DDX_Control(pDX, IDC_LEVEL_EDIT_1, m_cLevelEdit1);
	DDX_Control(pDX, IDC_XP_EDIT_1, m_cXPEdit1);
	DDX_Text(pDX, IDC_XP_VALUE_COMMENT, m_szXPValueComment);
	DDV_MaxChars(pDX, m_szXPValueComment, 32);
	DDX_Text(pDX, IDC_EXP_VALUE_EDIT, m_szXPValue);
	DDX_Control(pDX, IDC_LANGUAGES_LIST_BOX, m_cLanguagesListBox);
	DDX_Text(pDX, IDC_TURN_UNDEAD_EDIT_1, m_szTurnUndead1);
	DDX_Text(pDX, IDC_TURN_UNDEAD_EDIT_2, m_szTurnUndead2);
	DDX_Text(pDX, IDC_TURN_UNDEAD_EDIT_3, m_szTurnUndead3);
	DDX_Text(pDX, IDC_TURN_UNDEAD_EDIT_4, m_szTurnUndead4);
	DDX_Text(pDX, IDC_TURN_UNDEAD_EDIT_5, m_szTurnUndead5);
	DDX_Text(pDX, IDC_TURN_UNDEAD_EDIT_6, m_szTurnUndead6);
	DDX_Text(pDX, IDC_TURN_UNDEAD_EDIT_7, m_szTurnUndead7);
	DDX_Text(pDX, IDC_TURN_UNDEAD_EDIT_8, m_szTurnUndead8);
	DDX_Text(pDX, IDC_TURN_UNDEAD_EDIT_9, m_szTurnUndead9);
	DDX_Text(pDX, IDC_TURN_UNDEAD_EDIT_10, m_szTurnUndead10);
	DDX_Text(pDX, IDC_TURN_UNDEAD_EDIT_11, m_szTurnUndead11);
	DDX_Text(pDX, IDC_TURN_UNDEAD_EDIT_12, m_szTurnUndead12);
	DDX_Text(pDX, IDC_TURN_UNDEAD_EDIT_13, m_szTurnUndead13);
	DDX_Control(pDX, IDC_CHAR_VISION_COMBO, m_cVisionCombo);
	DDX_Check(pDX, IDC_KEEN_EARED_CHECK, m_bKeenEar);
	DDX_Control(pDX, IDC_SECONDARY_SKILL_COMBO, m_cSecondarySkillCombo);
	DDX_Check(pDX, IDC_KEEN_EARED_CHECK2, m_bVeryKeenEar);
	DDX_CBString(pDX, IDC_SECONDARY_SKILL_COMBO, m_szSecondarySkill);
	DDV_MaxChars(pDX, m_szSecondarySkill, 63);
	DDX_Text(pDX, IDC_CONCEALED_MOVE_EDIT, m_szConcealedMove);
	DDX_Text(pDX, IDC_CLIMBING_MOVE_EDIT, m_szClimbingMove);
	DDX_Text(pDX, IDC_SPECIAL_MOVE_EDIT, m_szSpecialMove);
	DDX_Control(pDX, IDC_LIGHT_SOURCE_COMBO, m_cLightSourceCombo);
}


BEGIN_MESSAGE_MAP(CDMCharViewDialog, CDialog)
	//{{AFX_MSG_MAP(CDMCharViewDialog)
	ON_BN_CLICKED(IDC_ROLL_STATS, OnRollStats)
	ON_EN_CHANGE(IDC_CHAR_NAME_EDIT, OnChangeCharNameEdit)
	ON_CBN_SELCHANGE(IDC_CHAR_RACE_COMBO, OnSelchangeCharRaceCombo)
	ON_CBN_SELCHANGE(IDC_CHAR_CLASS_COMBO_1, OnSelchangeCharClassCombo1)
	ON_CBN_SELCHANGE(IDC_CHAR_CLASS_COMBO_2, OnSelchangeCharClassCombo2)
	ON_CBN_SELCHANGE(IDC_CHAR_CLASS_COMBO_3, OnSelchangeCharClassCombo3)
	ON_CBN_KILLFOCUS(IDC_CHAR_SEX_COMBO, OnKillfocusCharSexCombo)
	ON_EN_CHANGE(IDC_XP_EDIT_1, OnChangeXpEdit1)
	ON_EN_CHANGE(IDC_XP_EDIT_2, OnChangeXpEdit2)
	ON_EN_CHANGE(IDC_XP_EDIT_3, OnChangeXpEdit3)
	ON_EN_CHANGE(IDC_LEVEL_EDIT_1, OnChangeLevelEdit1)
	ON_EN_CHANGE(IDC_CON_EDIT, OnChangeConEdit)
	ON_EN_CHANGE(IDC_STR_EDIT, OnChangeStrEdit)
	ON_BN_CLICKED(IDC_SHOW_HP_CHECK, OnShowHpCheck)
	ON_EN_CHANGE(IDC_INT_EDIT, OnChangeIntEdit)
	ON_EN_CHANGE(IDC_WIS_EDIT, OnChangeWisEdit)
	ON_EN_CHANGE(IDC_DEX_EDIT, OnChangeDexEdit)
	ON_EN_CHANGE(IDC_CHA_EDIT, OnChangeChaEdit)
	ON_EN_CHANGE(IDC_COM_EDIT, OnChangeComEdit)
	ON_EN_SETFOCUS(IDC_HP_EDIT, OnSetfocusHpEdit)
	ON_EN_CHANGE(IDC_EX_STR_EDIT, OnChangeExStrEdit)
	ON_BN_CLICKED(IDC_CHAR_SHEET, OnCharSheet)
	ON_CBN_SELCHANGE(IDC_CHAR_ALIGNMENT_COMBO, OnSelchangeCharAlignmentCombo)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_CHAR_ARMOR_COMBO, OnSelchangeCharArmorCombo)
	ON_CBN_SELCHANGE(IDC_CHAR_SHIELD_COMBO, OnSelchangeCharShieldCombo)
	ON_EN_CHANGE(IDC_ARMOR_PLUS_EDIT, OnChangeArmorPlusEdit)
	ON_EN_CHANGE(IDC_SHIELD_PLUS_EDIT, OnChangeShieldPlusEdit)
	ON_BN_CLICKED(IDC_SAVE, OnSave)
	ON_BN_CLICKED(IDC_LOAD, OnLoad)
	ON_CBN_SELCHANGE(IDC_CHAR_SEX_COMBO, OnSelchangeCharSexCombo)
	ON_EN_KILLFOCUS(IDC_EX_STR_EDIT, OnKillfocusExStrEdit)
	ON_EN_SETFOCUS(IDC_AC_EDIT, OnSetfocusAcEdit)
	ON_EN_CHANGE(IDC_LEVEL_EDIT_3, OnChangeLevelEdit3)
	ON_EN_CHANGE(IDC_LEVEL_EDIT_2, OnChangeLevelEdit2)
	ON_BN_CLICKED(IDC_ADD_XP_BUTTON_1, OnAddXpButton1)
	ON_BN_CLICKED(IDC_ADD_XP_BUTTON_2, OnAddXpButton2)
	ON_BN_CLICKED(IDC_ADD_XP_BUTTON_3, OnAddXpButton3)
	ON_CBN_SELCHANGE(IDC_WEAPON_COMBO_1, OnSelchangeWeaponCombo1)
	ON_CBN_SELCHANGE(IDC_WEAPON_COMBO_2, OnSelchangeWeaponCombo2)
	ON_CBN_SELCHANGE(IDC_WEAPON_COMBO_3, OnSelchangeWeaponCombo3)
	ON_CBN_SELCHANGE(IDC_WEAPON_COMBO_4, OnSelchangeWeaponCombo4)
	ON_EN_CHANGE(IDC_WEAPON_PLUS_EDIT_1, OnChangeWeaponPlusEdit1)
	ON_EN_CHANGE(IDC_WEAPON_PLUS_EDIT_2, OnChangeWeaponPlusEdit2)
	ON_EN_CHANGE(IDC_WEAPON_PLUS_EDIT_3, OnChangeWeaponPlusEdit3)
	ON_EN_CHANGE(IDC_WEAPON_PLUS_EDIT_4, OnChangeWeaponPlusEdit4)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_SWAP_BUTTON_1, OnSwapButton1)
	ON_BN_CLICKED(IDC_SWAP_BUTTON_2, OnSwapButton2)
	ON_BN_CLICKED(IDC_SWAP_BUTTON_3, OnSwapButton3)
	ON_BN_CLICKED(IDC_CHAR_INVENTORY, OnCharInventory)
	ON_BN_CLICKED(IDC_ADD_HP_BUTTON, OnAddHpButton)
	ON_BN_CLICKED(IDC_RECOVER_AMMO, OnRecoverAmmo)
	ON_EN_CHANGE(IDC_AMMO_PLUS_EDIT, OnChangeAmmoPlusEdit)
	ON_CBN_SELCHANGE(IDC_AMMO_COMBO, OnSelchangeAmmoCombo)
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
	ON_BN_CLICKED(IDC_SPELLS_BUTTON, OnSpellsButton)
	ON_CBN_SELCHANGE(IDC_CHAR_RING_L_COMBO, OnSelchangeCharRingLCombo)
	ON_CBN_SELCHANGE(IDC_CHAR_RING_R_COMBO, OnSelchangeCharRingRCombo)
	ON_EN_CHANGE(IDC_RING_L_PLUS_EDIT, OnChangeRingLPlusEdit)
	ON_EN_CHANGE(IDC_RING_R_PLUS_EDIT, OnChangeRingRPlusEdit)
	ON_BN_CLICKED(IDC_NPC_CHECK, OnNpcCheck)
	ON_BN_CLICKED(IDC_MONSTER_CHECK, OnMonsterCheck)
	ON_CBN_SELCHANGE(IDC_CHAR_HELM_COMBO, OnSelchangeCharHelmCombo)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_ADD_EXP_BUTTON, &CDMCharViewDialog::OnBnClickedAddExpButton)
	ON_EN_CHANGE(IDC_EXP_EDIT, &CDMCharViewDialog::OnEnChangeExpEdit)
	ON_BN_CLICKED(IDC_CURRENT_CLASS_CHECK_1, &CDMCharViewDialog::OnBnClickedCurrentClassCheck1)
	ON_BN_CLICKED(IDC_CURRENT_CLASS_CHECK_2, &CDMCharViewDialog::OnBnClickedCurrentClassCheck2)
	ON_BN_CLICKED(IDC_CURRENT_CLASS_CHECK_3, &CDMCharViewDialog::OnBnClickedCurrentClassCheck3)
	ON_EN_CHANGE(IDC_EXP_VALUE_EDIT, &CDMCharViewDialog::OnEnChangeExpValueEdit)
	ON_LBN_DBLCLK(IDC_LANGUAGES_LIST_BOX, &CDMCharViewDialog::OnLbnDblclkLanguagesListBox)
	ON_LBN_SELCHANGE(IDC_LANGUAGES_LIST_BOX, &CDMCharViewDialog::OnLbnSelchangeLanguagesListBox)
	ON_CBN_SELCHANGE(IDC_CHAR_VISION_COMBO, &CDMCharViewDialog::OnCbnSelchangeCharVisionCombo)
	ON_BN_CLICKED(IDC_KEEN_EARED_CHECK, &CDMCharViewDialog::OnBnClickedKeenEaredCheck)
	ON_BN_CLICKED(IDC_KEEN_EARED_CHECK2, &CDMCharViewDialog::OnBnClickedKeenEaredCheck2)
	ON_CBN_EDITCHANGE(IDC_SECONDARY_SKILL_COMBO, &CDMCharViewDialog::OnCbnEditchangeSecondarySkillCombo)
	ON_CBN_SELCHANGE(IDC_SECONDARY_SKILL_COMBO, &CDMCharViewDialog::OnCbnSelchangeSecondarySkillCombo)
	ON_EN_CHANGE(IDC_CONCEALED_MOVE_EDIT, &CDMCharViewDialog::OnEnChangeConcealedMoveEdit)
	ON_EN_CHANGE(IDC_CLIMBING_MOVE_EDIT, &CDMCharViewDialog::OnEnChangeClimbingMoveEdit)
	ON_EN_CHANGE(IDC_SPECIAL_MOVE_EDIT, &CDMCharViewDialog::OnEnChangeSpecialMoveEdit)
	ON_CBN_SELCHANGE(IDC_LIGHT_SOURCE_COMBO, &CDMCharViewDialog::OnCbnSelchangeLightSourceCombo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDMCharViewDialog message handlers

BOOL CDMCharViewDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	//if (strcmp(m_pCharacter->m_szCharacterName, "Elric") == 0)
	//{
	//	TRACE("!");
	//}

	m_cLanguagesListBox.SetColor(COLOR_WINDOWTEXT, COLOR_WINDOWTEXT);

	//m_SelColor = GetSysColor(COLOR_HIGHLIGHTTEXT);
    //m_Color = GetSysColor(COLOR_WINDOWTEXT);

	SetWindowText("New Character");

	for(int i = 0; i < DND_CHARACTER_RACE_LAST_RACE; ++i)
	{
		m_cCharRaceCombo.InsertString(i, GetRaceName((DND_CHARACTER_RACES)i));
		m_cCharRaceCombo.SetItemData(i, i);
	}

	m_cCharRaceCombo.SetCurSel(0);

	InitClassListBox();

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


	POSITION pos;
	WORD wID = 0;
	/*
	int nArmorCount = 0;
	for (pos = pApp->m_ArmorOrderedTypeMap.GetStartPosition(); pos != NULL; )
	{
		cDNDArmor *pArmor = NULL;
		pApp->m_ArmorOrderedTypeMap.GetNextAssoc(pos,wID,pArmor);

		if(pArmor != NULL)
		{
			m_cArmorCombo.InsertString(nArmorCount, pArmor->m_szType);
			m_cArmorCombo.SetItemData(nArmorCount, pArmor->m_wTypeId);

			//TRACE("ARMOR %d %s at %d\n", pArmor->m_wTypeId, pArmor->m_szType, nArmorCount);
			++nArmorCount;
		}
	}
	*/
	
	int nShieldCount = 0;
	for (pos = m_pApp->m_ShieldsOrderedTypeMap.GetStartPosition(); pos != NULL; )
	{
		cDNDShield *pShield = NULL;
		m_pApp->m_ShieldsOrderedTypeMap.GetNextAssoc(pos,wID,pShield);

		if(pShield != NULL)
		{
			m_cShieldCombo.InsertString(nShieldCount, pShield->m_szType);
			m_cShieldCombo.SetItemData(nShieldCount, pShield->m_wTypeId);

			//TRACE("ARMOR %d %s at %d\n", pArmor->m_wTypeId, pArmor->m_szType, nArmorCount);
			++nShieldCount;
		}
	}
	m_cShieldCombo.SetCurSel(0);

	
	for (int nWeaponCount = 0; nWeaponCount < m_pApp->m_WeaponsOrderedTypeArray.GetSize(); ++nWeaponCount)
	{
		cDNDWeapon *pWeapon = m_pApp->m_WeaponsOrderedTypeArray[nWeaponCount];

		if(pWeapon != NULL)
		{
			m_cWeaponCombo_1.InsertString(nWeaponCount, pWeapon->m_szType);
			m_cWeaponCombo_1.SetItemData(nWeaponCount, pWeapon->m_wTypeId);

			m_cWeaponCombo_2.InsertString(nWeaponCount, pWeapon->m_szType);
			m_cWeaponCombo_2.SetItemData(nWeaponCount, pWeapon->m_wTypeId);

			m_cWeaponCombo_3.InsertString(nWeaponCount, pWeapon->m_szType);
			m_cWeaponCombo_3.SetItemData(nWeaponCount, pWeapon->m_wTypeId);

			m_cWeaponCombo_4.InsertString(nWeaponCount, pWeapon->m_szType);
			m_cWeaponCombo_4.SetItemData(nWeaponCount, pWeapon->m_wTypeId);
		}
	}
	m_cWeaponCombo_1.SetCurSel(0);
	m_cWeaponCombo_2.SetCurSel(0);
	m_cWeaponCombo_3.SetCurSel(0);
	m_cWeaponCombo_4.SetCurSel(0);

	int nCount = 0;
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

	for(i = DND_CHARACTER_VISION_STANDARD; i < DND_CHARACTER_VISION_LAST_TYPE; ++i)
	{
		m_cVisionCombo.InsertString(i-1, GetVisionTypeName((DND_CHARACTER_VISION_TYPES)i));
		m_cVisionCombo.SetItemData(i-1, (DWORD_PTR)i);
	}

	nCount = 0;
	m_cSecondarySkillCombo.InsertString(nCount++," ");
	m_cSecondarySkillCombo.InsertString(nCount++,"Armorer");
	m_cSecondarySkillCombo.InsertString(nCount++,"Bowyer/fletcher");
	m_cSecondarySkillCombo.InsertString(nCount++,"Farmer/gardener");
	m_cSecondarySkillCombo.InsertString(nCount++,"Fisher (netting)");
	m_cSecondarySkillCombo.InsertString(nCount++,"Forester");
	m_cSecondarySkillCombo.InsertString(nCount++,"Gambler");
	m_cSecondarySkillCombo.InsertString(nCount++,"Hunter/fisher (hook and line)");
	m_cSecondarySkillCombo.InsertString(nCount++,"Husbandman (animal husbandry)");
	m_cSecondarySkillCombo.InsertString(nCount++,"Jeweler/lapidary");
	m_cSecondarySkillCombo.InsertString(nCount++,"Leather worker/tanner");
	m_cSecondarySkillCombo.InsertString(nCount++,"Limner/painter");
	m_cSecondarySkillCombo.InsertString(nCount++,"Mason/carpenter");
	m_cSecondarySkillCombo.InsertString(nCount++,"Miner");
	m_cSecondarySkillCombo.InsertString(nCount++,"Navigator (fresh or salt water)");
	m_cSecondarySkillCombo.InsertString(nCount++,"Sailor (fresh or salt)");
	m_cSecondarySkillCombo.InsertString(nCount++,"Shipwright (boats or ships)");
	m_cSecondarySkillCombo.InsertString(nCount++,"Tailor/weaver");
	m_cSecondarySkillCombo.InsertString(nCount++,"Teamster/freighter");
	m_cSecondarySkillCombo.InsertString(nCount++,"Trader/barterer");
	m_cSecondarySkillCombo.InsertString(nCount++,"Trapper/furrier");
	m_cSecondarySkillCombo.InsertString(nCount++,"Woodworker/cabinetmaker");


	m_nDayofWeek = 0;
	m_nDayofMonth = 0;
	m_nMonth = 0;
	m_nYear = 0;

	m_bInitDialogCalled = TRUE;

	Refresh();

	//if (strcmp(m_pCharacter->m_szCharacterName, "Elric") == 0)
	//{
	//	TRACE("!");
	//}

	//m_pParent->InvalidateRect(NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDMCharViewDialog::InitClassListBox()
{
	m_cClassCombo_1.ResetContent();

	int nIndex = 0;
	for (int i = 0; i < NUM_CHARVIEW_CLASSES; ++i)
	{
		if (IsDefinedClass((DND_CHARACTER_CLASSES)nClasses[i]))
		{
			m_cClassCombo_1.InsertString(nIndex, GetClassName((DND_CHARACTER_CLASSES)nClasses[i]));
			m_cClassCombo_1.SetItemData(nIndex, nClasses[i]);
			++nIndex;
		}
	}
	m_cClassCombo_1.SetCurSel(0);
}

void CDMCharViewDialog::OnOK() 
{
	//CDialog::OnOK();
}

void CDMCharViewDialog::OnCancel() 
{
	//CDialog::OnCancel();
}

void CDMCharViewDialog::MateToParentWindow() 
{
}

void CDMCharViewDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// Do not call CDialog::OnPaint() for painting messages

	if(m_pCharacter == NULL)
	{
		return;
	}

	if(m_nPage == -1)
		return;

	if(m_pParent != NULL)
	{
		CRect rect;
		m_pParent->GetClientRect(&rect);

		rect.top+=20;

		SetWindowPos(NULL, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
	}
	
	if(m_pCharacter->m_dwCharacterID == 0L)
	{
		m_cRollStats.EnableWindow(TRUE);
	}
	else
	{
		m_cRollStats.SetWindowText("Grant Earned XP");

		if(m_pCharacter->m_lEarnedXP)
		{
			m_cRollStats.EnableWindow(TRUE);
		}
		else
		{
			m_cRollStats.EnableWindow(FALSE);
		}
	}

	if(m_nPage == 1)
	{
		if(m_pInventoryDialog == NULL)
		{
			m_pInventoryDialog = new DMInventoryDialog(m_pMainDialog, m_pCharacter, this, NULL, m_pParent);
		}

		ShowWindow(SW_HIDE);
		m_pInventoryDialog->ShowWindow(SW_SHOW);

		m_pInventoryDialog->InitDisplay();
		m_pInventoryDialog->InvalidateRect(NULL);
		
	}
	else if(m_nPage == 2)
	{
		if(m_pSpellsDialog == NULL)
		{
			m_pSpellsDialog = new DMCharSpellsDialog(m_pCharacter, this, m_pParent);
		}

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

void CDMCharViewDialog::OnChangeCharNameEdit() 
{	
	UpdateData(TRUE);

	strcpy(m_pCharacter->m_szCharacterName, m_szCharacterName.GetBuffer(0));

	m_szSaveFileName = _T(""); 
	m_szLoadFileName = _T("");

	m_pCharacter->MarkChanged();

	Refresh();
}


void CDMCharViewDialog::OnSelchangeCharRaceCombo() 
{
	UpdateData(TRUE);

	int nCursor = m_cCharRaceCombo.GetCurSel();

	m_pCharacter->m_nRace = (DND_CHARACTER_RACES)m_cCharRaceCombo.GetItemData(nCursor);

	if(m_pCharacter->m_nRace == DND_CHARACTER_RACE_HUMAN)
	{
		m_pCharacter->m_Class[1] = DND_CHARACTER_CLASS_UNDEF;
		m_pCharacter->m_Class[2] = DND_CHARACTER_CLASS_UNDEF;
		m_pCharacter->m_nDualClassClass = 0;
	}

	m_pCharacter->m_VisionType = DND_CHARACTER_VISION_UNDEF;

	m_cCurrentClassCheck1.SetCheck(1);

	m_cLevelEdit1.EnableWindow(TRUE);
	m_cXPEdit1.EnableWindow(TRUE);

	m_pCharacter->m_nHeight = 0; //forces a reset/re-roll
	m_pCharacter->m_nWeight = 0; //forces a reset/re-roll

	ProcessCharStats();

	m_pCharacter->MarkChanged();

	Refresh();
	
}


void CDMCharViewDialog::OnSelchangeCharClassCombo1() 
{
	UpdateData(TRUE);

	int nCursor = m_cClassCombo_1.GetCurSel();

	m_pCharacter->m_Class[0] = (DND_CHARACTER_CLASSES)m_cClassCombo_1.GetItemData(nCursor);

	if (m_pCharacter->m_Class[0] == DND_CHARACTER_CLASS_NONE)
	{
		m_pCharacter->m_nLevel[0] = 0;
	}
	else if(m_pCharacter->m_Class[0] != DND_CHARACTER_CLASS_UNDEF && m_pCharacter->m_nLevel[0] == 0)
	{
		m_pCharacter->m_nLevel[0] = 1;
	}

	m_pCharacter->MarkChanged();

	ProcessCharStats();

	Refresh();
}

void CDMCharViewDialog::OnSelchangeCharClassCombo2() 
{
	UpdateData(TRUE);

	int nCursor = m_cClassCombo_2.GetCurSel();

	m_pCharacter->m_Class[1] = (DND_CHARACTER_CLASSES)m_cClassCombo_2.GetItemData(nCursor);

	if(m_pCharacter->m_Class[1] != DND_CHARACTER_CLASS_UNDEF && m_pCharacter->m_nLevel[1] == 0)
	{
		m_pCharacter->m_nLevel[1] = 1;
	}

	m_pCharacter->MarkChanged();

	ProcessCharStats();

	Refresh();
	
}

void CDMCharViewDialog::OnSelchangeCharClassCombo3() 
{
	UpdateData(TRUE);

	int nCursor = m_cClassCombo_3.GetCurSel();

	m_pCharacter->m_Class[2] = (DND_CHARACTER_CLASSES)m_cClassCombo_3.GetItemData(nCursor);

	if(m_pCharacter->m_Class[2] != DND_CHARACTER_CLASS_UNDEF && m_pCharacter->m_nLevel[2] == 0)
	{
		m_pCharacter->m_nLevel[2] = 1;
	}

	m_pCharacter->MarkChanged();

	ProcessCharStats();

	Refresh();
	
}


void CDMCharViewDialog::Refresh()
{
	if (m_hWnd == NULL) // unit test support
		return;

	if(m_pCharacter == NULL)
		return;

	CString szTemp;

	m_szRaceSex = "";
	m_szClassList = "";
	m_szLevelList = "";

	UpdateData(TRUE);
	
	/*
	if(m_pCharacter->m_bIsNPC == FALSE)
	{
		m_bMonsterCheck = FALSE;
		//m_cMonsterCheck.ShowWindow(SW_HIDE);
	}
	else
	{
		//m_cMonsterCheck.ShowWindow(SW_SHOW);
	}
	*/

	//if (strcmp(m_pCharacter->m_szCharacterName, "Elric") == 0)
	//{
	//	TRACE("!");
	//}

	if(m_bMonsterCheck)
		m_szRaceStatic = _T("Type:");
	else
		m_szRaceStatic = _T("Race:");

	m_szCharacterName = m_pCharacter->m_szCharacterName;

	m_szBaseCharName = m_szCharacterName;

	int curPos = 0;
	m_szCharacterFirstName = m_szCharacterName.Tokenize(_T(" "), curPos);


	if(m_pCharacter->m_Class[0] == DND_CHARACTER_CLASS_UNDEF)
	{
		m_pCharacter->m_Class[0] = DND_CHARACTER_CLASS_FIGHTER;
	}

	if (m_pCharacter->m_Class[0] == DND_CHARACTER_CLASS_NONE)
	{
		m_pCharacter->m_nLevel[0] = 0;
	}
	else if(m_pCharacter->m_nLevel[0] == 0)
	{
		m_pCharacter->m_nLevel[0] = 1;
	}

	#if 0 // #ifdef _DEBUG

	#define ATTRIB_STR		0
	#define ATTRIB_INT		1
	#define ATTRIB_WIS		2
	#define ATTRIB_DEX		3
	#define ATTRIB_CON		4
	#define ATTRIB_CHA		5
	#define ATTRIB_COM		6
	#define ATTRIB_EXSTR	7

	m_szSTREdit.Format("%d-%d", m_pCharacter->m_nBaseStats[ATTRIB_STR], m_pCharacter->m_nDisplayStats[ATTRIB_STR]);
	m_szINTEdit.Format("%d-%d", m_pCharacter->m_nBaseStats[ATTRIB_INT], m_pCharacter->m_nDisplayStats[ATTRIB_INT]);
	m_szWISEdit.Format("%d-%d", m_pCharacter->m_nBaseStats[ATTRIB_WIS], m_pCharacter->m_nDisplayStats[ATTRIB_WIS]);
	m_szDEXEdit.Format("%d-%d", m_pCharacter->m_nBaseStats[ATTRIB_DEX], m_pCharacter->m_nDisplayStats[ATTRIB_DEX]);
	m_szCONEdit.Format("%d-%d", m_pCharacter->m_nBaseStats[ATTRIB_CON], m_pCharacter->m_nDisplayStats[ATTRIB_CON]);
	m_szCHAEdit.Format("%d-%d", m_pCharacter->m_nBaseStats[ATTRIB_CHA], m_pCharacter->m_nDisplayStats[ATTRIB_CHA]);
	m_szCOMEdit.Format("%d-%d", m_pCharacter->m_nBaseStats[ATTRIB_COM], m_pCharacter->m_nDisplayStats[ATTRIB_COM]);
														 
	#else

	m_szSTREdit.Format("%d", m_pCharacter->m_nDisplayStats[ATTRIB_STR]);
	m_szINTEdit.Format("%d", m_pCharacter->m_nDisplayStats[ATTRIB_INT]);
	m_szWISEdit.Format("%d", m_pCharacter->m_nDisplayStats[ATTRIB_WIS]);
	m_szDEXEdit.Format("%d", m_pCharacter->m_nDisplayStats[ATTRIB_DEX]);
	m_szCONEdit.Format("%d", m_pCharacter->m_nDisplayStats[ATTRIB_CON]);
	m_szCHAEdit.Format("%d", m_pCharacter->m_nDisplayStats[ATTRIB_CHA]);
	m_szCOMEdit.Format("%d", m_pCharacter->m_nDisplayStats[ATTRIB_COM]);

	#endif

	if(g_bUseUnearthedArcana) 
	{
		(GetDlgItem( IDC_COMELINESS_STATIC ))->ShowWindow(SW_SHOW);
		(GetDlgItem( IDC_COM_EDIT ))->ShowWindow(SW_SHOW);
	}
	else
	{
		(GetDlgItem( IDC_COMELINESS_STATIC ))->ShowWindow(SW_HIDE);
		(GetDlgItem( IDC_COM_EDIT ))->ShowWindow(SW_HIDE);
	}

	if(m_pCharacter->m_nDisplayStats[ATTRIB_STR] == 18 && CharacterIsFighter(m_pCharacter))
	{
		m_szExceptionalStrength.Format("%02d", m_pCharacter->m_nDisplayStats[ATTRIB_EXSTR]);
		m_cExceptionalStrengthEdit.ShowWindow(SW_SHOW);
		m_cSTRComment.SetWindowPos(NULL, 98+36,88+32, 20,200, SW_SHOW);

		m_szCharStats.Format("S %s/%s; ", m_szSTREdit, m_szExceptionalStrength);
	}
	else
	{
		m_cExceptionalStrengthEdit.ShowWindow(SW_HIDE);
		m_cSTRComment.SetWindowPos(NULL, 93,88+32, 20,200, SW_SHOW);

		m_szCharStats.Format("S %s; ", m_szSTREdit);
	}

	CString szAttribs;
	szAttribs.Format("I %s; W %s; D %s; C %s; Ch %s", m_szINTEdit, m_szWISEdit, m_szDEXEdit, m_szCONEdit, m_szCHAEdit);

	m_szCharStats += szAttribs;

	m_szLevelEdit1.Format("%d", m_pCharacter->m_nLevel[0]);
	m_szLevelEdit2.Format("%d", m_pCharacter->m_nLevel[1]);
	m_szLevelEdit3.Format("%d", m_pCharacter->m_nLevel[2]);

	m_szXPEdit1.Format("%ld", m_pCharacter->m_lExperience[0]);
	m_szXPEdit2.Format("%ld", m_pCharacter->m_lExperience[1]);
	m_szXPEdit3.Format("%ld", m_pCharacter->m_lExperience[2]);

	m_szEarnedXP.Format("%ld", m_pCharacter->m_lEarnedXP);

	m_szHitPoints.Format("%d", m_pCharacter->m_nHitPoints);
	m_szCurrentHitPoints.Format("%d", m_pCharacter->m_nHitPoints - m_pCharacter->m_nCurrentDamage);

	if(m_bShowHPCheck)
	{
		m_szHPByLevel = m_szHPByLevelBuffer;
	}
	else
	{
		m_szHPByLevel = _T("");
	}

	m_szSTRComment = m_szSTRCommentBuffer;
	m_szINTComment = m_szINTCommentBuffer;
	m_szWISComment = m_szWISCommentBuffer;
	m_szDEXComment = m_szDEXCommentBuffer;
	m_szCONComment = m_szCONCommentBuffer;
	m_szCHAComment = m_szCHACommentBuffer;
	

	if(m_pCharacter->m_szCharacterName[0] != 0)
	{
		SetWindowText(m_pCharacter->m_szCharacterName);
	}

	m_cCurrentClassCheck1.SetCheck(0);
	m_cCurrentClassCheck2.SetCheck(0);
	m_cCurrentClassCheck3.SetCheck(0);

	m_cCurrentClassCheck1.EnableWindow(FALSE);
	m_cCurrentClassCheck2.EnableWindow(FALSE);
	m_cCurrentClassCheck3.EnableWindow(FALSE);

	if (m_pCharacter->m_Class[0] != DND_CHARACTER_CLASS_UNDEF)
	{
		m_szClassList = GetClassName(m_pCharacter->m_Class[0]);
		m_szLevelList = GetStringFromInt(m_pCharacter->m_nLevel[0]);
	}

	if(m_pCharacter->m_nRace == DND_CHARACTER_RACES_UNDEF || m_pCharacter->m_nRace == DND_CHARACTER_RACE_HUMAN) // human dual class section
	{
		m_cClassCombo_2.ResetContent();
		m_cClassCombo_3.ResetContent();

		m_cClassCombo_2.EnableWindow(FALSE);
		m_cClassCombo_3.EnableWindow(FALSE);

		m_cCurrentClassCheck1.EnableWindow(TRUE);
		if(m_pCharacter->m_Class[0] != DND_CHARACTER_CLASS_UNDEF)
		{
			m_cCurrentClassCheck2.EnableWindow(TRUE);

			BOOL bEnable = FALSE;
			m_cClassCombo_2.ResetContent();
			int index = 0;
			for (int i = 0; i < NUM_CHARVIEW_CLASSES; ++i)
			{
				if(ClassIsValidDualClass((DND_CHARACTER_CLASSES)nClasses[i], m_pCharacter->m_Class[0], m_pCharacter->m_Class[2]))
				{
					if(index == 0)
					{
						m_cClassCombo_2.InsertString(index, "");
						m_cClassCombo_2.SetItemData(index, 0);
						++index;
					}
					#if CUSTOM_CLASSES
					if (IsDefinedClass((DND_CHARACTER_CLASSES)nClasses[i]) == TRUE)
					#else
					if(1)
					#endif
					{
						m_cClassCombo_2.InsertString(index, GetClassName((DND_CHARACTER_CLASSES)nClasses[i]));
						m_cClassCombo_2.SetItemData(index, nClasses[i]);
						++index;
						bEnable = TRUE;
					}
				}
			}
			if(bEnable)
			{
				m_cClassCombo_2.EnableWindow(TRUE);

				for(int j = 0; j < m_cClassCombo_2.GetCount(); ++j)
				{
					if(m_cClassCombo_2.GetItemData(j) == (unsigned int)m_pCharacter->m_Class[1])
					{
						m_cClassCombo_2.SetCurSel(j);

						if (m_pCharacter->m_Class[1] != DND_CHARACTER_CLASS_UNDEF)
						{
							m_szClassList += "/";
							m_szClassList += GetClassName(m_pCharacter->m_Class[1]);
							m_szLevelList += "/";
							m_szLevelList += GetStringFromInt(m_pCharacter->m_nLevel[1]);
						}

						break;
					}
				}
			}
		}

		if(m_pCharacter->m_Class[1] != DND_CHARACTER_CLASS_UNDEF)
		{
			m_cCurrentClassCheck3.EnableWindow(TRUE);

			BOOL bEnable = FALSE;
			m_cClassCombo_3.ResetContent();
			int index = 0;
			for (int i = 0; i < NUM_CHARVIEW_CLASSES; ++i)
			{
				if(ClassIsValidDualClass((DND_CHARACTER_CLASSES)nClasses[i], m_pCharacter->m_Class[0], m_pCharacter->m_Class[1]))
				{
					if(index == 0)
					{
						m_cClassCombo_3.InsertString(index, "");
						m_cClassCombo_3.SetItemData(index, 0);
						++index;
					}
					#if CUSTOM_CLASSES
					if (IsDefinedClass((DND_CHARACTER_CLASSES)nClasses[i]) == TRUE)
					#else
					if(1)
					#endif
					{
						m_cClassCombo_3.InsertString(index, GetClassName((DND_CHARACTER_CLASSES)nClasses[i]));
						m_cClassCombo_3.SetItemData(index, nClasses[i]);
						++index;
						bEnable = TRUE;
					}
				}
			}
			if(bEnable)
			{
				m_cClassCombo_3.EnableWindow(TRUE);

				for(int j = 0; j < m_cClassCombo_3.GetCount(); ++j)
				{
					if(m_cClassCombo_3.GetItemData(j) == (unsigned int)m_pCharacter->m_Class[2]) 
					{
						m_cClassCombo_3.SetCurSel(j);

						if (m_pCharacter->m_Class[2] != DND_CHARACTER_CLASS_UNDEF)
						{
							m_szClassList += "/";
							m_szClassList += GetClassName(m_pCharacter->m_Class[2]);
							m_szLevelList += "/";
							m_szLevelList += GetStringFromInt(m_pCharacter->m_nLevel[2]);
						}

						break;
					}
				}
			}
		
		}

		switch(m_pCharacter->m_nDualClassClass)
		{
			case 0:
			{
				m_cCurrentClassCheck1.SetCheck(1);

				m_cLevelEdit1.EnableWindow(TRUE);
				m_cXPEdit1.EnableWindow(TRUE);

				m_cLevelEdit2.EnableWindow(FALSE);
				m_cLevelEdit3.EnableWindow(FALSE);

				m_cXPEdit2.EnableWindow(FALSE);
				m_cXPEdit3.EnableWindow(FALSE);

				break;
			}
			case 1:
			{
				m_cCurrentClassCheck2.SetCheck(1);

				m_cLevelEdit2.EnableWindow(TRUE);
				m_cXPEdit2.EnableWindow(TRUE);

				m_cLevelEdit1.EnableWindow(FALSE);
				m_cLevelEdit3.EnableWindow(FALSE);

				m_cXPEdit1.EnableWindow(FALSE);
				m_cXPEdit3.EnableWindow(FALSE);

				break;
			}
			case 2:
			{
				m_cCurrentClassCheck3.SetCheck(1);

				m_cLevelEdit3.EnableWindow(TRUE);
				m_cXPEdit3.EnableWindow(TRUE);

				m_cLevelEdit1.EnableWindow(FALSE);
				m_cLevelEdit2.EnableWindow(FALSE);

				m_cXPEdit1.EnableWindow(FALSE);
				m_cXPEdit2.EnableWindow(FALSE);

				break;
			}
			default: //just in case it got borked somewhere
			{
				m_pCharacter->m_nDualClassClass = 0;

				break;
			}
		}

	}
	else // non-human multi-class section
	{
		m_pCharacter->m_nDualClassClass = 0; // just in case it got borked somewhere

		m_cCurrentClassCheck1.SetCheck(1);

		BOOL bEnable = FALSE;
		m_cClassCombo_2.ResetContent();
		int index = 0;
		for (int i = 0; i < NUM_CHARVIEW_CLASSES; ++i)
		{
			if(ClassIsValidMultiClass(m_pCharacter->m_Class[0], (DND_CHARACTER_CLASSES)nClasses[i]))
			{
				if(index == 0)
				{
					m_cClassCombo_2.InsertString(index, "");
					m_cClassCombo_2.SetItemData(index, 0);
					++index;
				}
				m_cClassCombo_2.InsertString(index, GetClassName((DND_CHARACTER_CLASSES)nClasses[i]));
				m_cClassCombo_2.SetItemData(index, nClasses[i]);
				++index;
				bEnable = TRUE;
			}
		}
		if(bEnable)
		{
			for(int j = 0; j < m_cClassCombo_2.GetCount(); ++j)
			{
				if(m_cClassCombo_2.GetItemData(j) == (unsigned int)m_pCharacter->m_Class[1])
				{
					m_cClassCombo_2.SetCurSel(j);
					if(m_pCharacter->m_Class[1] != DND_CHARACTER_CLASS_UNDEF)
					{
						m_cCurrentClassCheck2.SetCheck(1);

						if (m_pCharacter->m_Class[1] != DND_CHARACTER_CLASS_UNDEF)
						{
							m_szClassList += "/";
							m_szClassList += GetClassName(m_pCharacter->m_Class[1]);
							m_szLevelList += "/";
							m_szLevelList += GetStringFromInt(m_pCharacter->m_nLevel[1]);
						}
					}

					break;
				}
			}
			m_cClassCombo_2.EnableWindow(TRUE);
			if(m_pCharacter->m_Class[1] != DND_CHARACTER_CLASS_UNDEF)
			{
				m_cLevelEdit2.EnableWindow(TRUE);
				m_cXPEdit2.EnableWindow(TRUE);
			}
			else
			{
				m_cLevelEdit2.EnableWindow(FALSE);
				m_cXPEdit2.EnableWindow(FALSE);
			}
		}
		else
		{
			m_cClassCombo_2.SetCurSel(0);
			m_cClassCombo_2.EnableWindow(FALSE);
			m_cLevelEdit2.EnableWindow(FALSE);
			m_cXPEdit2.EnableWindow(FALSE);
		}

		bEnable = FALSE;
		m_cClassCombo_3.ResetContent();
		index = 0;
		for (i = 0; i < NUM_CHARVIEW_CLASSES; ++i)
		{
			if(ClassIsValidMultiClass(m_pCharacter->m_Class[0], (DND_CHARACTER_CLASSES)nClasses[i]) && ClassIsValidMultiClass(m_pCharacter->m_Class[1], (DND_CHARACTER_CLASSES)nClasses[i]))
			{
				if(index == 0)
				{
					m_cClassCombo_3.InsertString(index, "");
					m_cClassCombo_3.SetItemData(index, 0);
					++index;
				}
				m_cClassCombo_3.InsertString(index, GetClassName((DND_CHARACTER_CLASSES)nClasses[i]));
				m_cClassCombo_3.SetItemData(index, nClasses[i]);
				++index;
				bEnable = TRUE;
			}
		}
		if(bEnable)
		{
			for(int j = 0; j < m_cClassCombo_3.GetCount(); ++j)
			{
				if(m_cClassCombo_3.GetItemData(j) == (unsigned int)m_pCharacter->m_Class[2])
				{
					m_cClassCombo_3.SetCurSel(j);

					if(m_pCharacter->m_Class[2] != DND_CHARACTER_CLASS_UNDEF)
					{
						m_cCurrentClassCheck3.SetCheck(1);

						if (m_pCharacter->m_Class[2] != DND_CHARACTER_CLASS_UNDEF)
						{
							m_szClassList += "/";
							m_szClassList += GetClassName(m_pCharacter->m_Class[2]);
							m_szLevelList += "/";
							m_szLevelList += GetStringFromInt(m_pCharacter->m_nLevel[2]);
						}
					}

					break;
				}
			}
			m_cClassCombo_3.EnableWindow(TRUE);
			if(m_pCharacter->m_Class[2] != DND_CHARACTER_CLASS_UNDEF)
			{
				m_cLevelEdit3.EnableWindow(TRUE);
				m_cXPEdit3.EnableWindow(TRUE);
			}
			else
			{
				m_cLevelEdit3.EnableWindow(FALSE);
				m_cXPEdit3.EnableWindow(FALSE);
			}
		}
		else
		{
			m_cClassCombo_3.SetCurSel(0);
			m_cClassCombo_3.EnableWindow(FALSE);
			m_cLevelEdit3.EnableWindow(FALSE);
			m_cXPEdit3.EnableWindow(FALSE);
		}
	}

	for(int i = 0; i < m_cCharRaceCombo.GetCount(); ++i)
	{
		int nRace = m_cCharRaceCombo.GetItemData(i);

		if(nRace == m_pCharacter->m_nRace)
		{
			m_cCharRaceCombo.SetCurSel(i);
			m_szRaceSex = GetRaceName(m_pCharacter->m_nRace);
			break;
		}
	}

	for(i = 0; i < m_cCharSexCombo.GetCount(); ++i)
	{
		int nSex = m_cCharSexCombo.GetItemData(i);

		if(nSex == m_pCharacter->m_nSex)
		{
			m_cCharSexCombo.SetCurSel(i);
			m_szRaceSex += " ";
			
			if (m_pCharacter->m_nSex)
			{
				m_szRaceSex += "F";
			}
			else
			{
				m_szRaceSex += "M";
			}

			break;
		}
	}

	for(i = 0; i < m_cClassCombo_1.GetCount(); ++i)
	{
		int nClass = m_cClassCombo_1.GetItemData(i);

		if(nClass == m_pCharacter->m_Class[0])
		{
			m_cClassCombo_1.SetCurSel(i);
			break;
		}
	}

	for(i = 0; i < m_cClassCombo_2.GetCount(); ++i)
	{
		int nClass = m_cClassCombo_2.GetItemData(i);

		if(nClass == m_pCharacter->m_Class[1])
		{
			m_cClassCombo_2.SetCurSel(i);
			break;
		}
	}

	for(i = 0; i < m_cClassCombo_3.GetCount(); ++i)
	{
		int nClass = m_cClassCombo_3.GetItemData(i);

		if(nClass == m_pCharacter->m_Class[2])
		{
			m_cClassCombo_3.SetCurSel(i);
			break;
		}
	}


	for(i = 0; i < m_cAlignmentCombo.GetCount(); ++i)
	{
		int nAlignment = m_cAlignmentCombo.GetItemData(i);

		if(nAlignment == m_pCharacter->m_nAlignment)
		{
			m_cAlignmentCombo.SetCurSel(i);
			break;
		}
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
		if(m_pCharacter->m_Inventory[i].m_ObjectType == DND_OBJECT_TYPE_ARMOR)
		{
			if(m_pCharacter->m_Inventory[i].m_nMagicAdj > 0 && m_pCharacter->m_Inventory[i].m_bEnchanted == FALSE)
				szTemp.Format("%s +%d", m_pCharacter->m_Inventory[i].m_szType, m_pCharacter->m_Inventory[i].m_nMagicAdj);
			else if(m_pCharacter->m_Inventory[i].m_nMagicAdj < 0 && m_pCharacter->m_Inventory[i].m_bEnchanted == FALSE)
				szTemp.Format("%s %d", m_pCharacter->m_Inventory[i].m_szType, m_pCharacter->m_Inventory[i].m_nMagicAdj);
			else szTemp = m_pCharacter->m_Inventory[i].m_szType;

			m_cArmorCombo.InsertString(nArmorCount, szTemp);

			m_cArmorCombo.SetItemData(nArmorCount++, m_pCharacter->m_Inventory[i].m_dwObjectID);
		}
	}

	BOOL bFoundIt = FALSE;
	for(i = 0; i < m_cArmorCombo.GetCount(); ++i)
	{
		DWORD nArmorId = m_cArmorCombo.GetItemData(i);

		if(nArmorId == m_pCharacter->m_ArmorWorn.m_dwObjectID)
		{
			m_cArmorCombo.SetCurSel(i);
			bFoundIt = TRUE;
			break;
		}
	}

	if(bFoundIt == FALSE) //remove armor that no longer is in inventory
	{
		m_cArmorCombo.SetCurSel(0);
		_BlankArmor.CopyTo(&m_pCharacter->m_ArmorWorn);
		m_pCharacter->m_ArmorWorn.m_dwObjectID = 0L;
		m_pCharacter->m_ArmorWorn.m_nMagicAdj = 0;
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
		if(m_pCharacter->m_Inventory[i].m_ObjectType == DND_OBJECT_TYPE_HELMET)
		{
			if(m_pCharacter->m_Inventory[i].m_nMagicAdj > 0 && m_pCharacter->m_Inventory[i].m_bEnchanted == FALSE)
				szTemp.Format("%s +%d", m_pCharacter->m_Inventory[i].m_szType, m_pCharacter->m_Inventory[i].m_nMagicAdj);
			else if(m_pCharacter->m_Inventory[i].m_nMagicAdj < 0 && m_pCharacter->m_Inventory[i].m_bEnchanted == FALSE)
				szTemp.Format("%s %d", m_pCharacter->m_Inventory[i].m_szType, m_pCharacter->m_Inventory[i].m_nMagicAdj);
			else szTemp = m_pCharacter->m_Inventory[i].m_szType;

			m_cHelmCombo.InsertString(nHelmCount, szTemp);

			m_cHelmCombo.SetItemData(nHelmCount++, m_pCharacter->m_Inventory[i].m_dwObjectID);
		}
	}

	bFoundIt = FALSE;
	for(i = 0; i < m_cHelmCombo.GetCount(); ++i)
	{
		DWORD nHelmId = m_cHelmCombo.GetItemData(i);

		if(nHelmId == m_pCharacter->m_HelmWorn.m_dwObjectID)
		{
			m_cHelmCombo.SetCurSel(i);
			bFoundIt = TRUE;
			break;
		}
	}

	if(bFoundIt == FALSE) //remove helmet that no longer is in inventory
	{
		m_cHelmCombo.SetCurSel(0);
		_BlankHelmet.CopyTo(&m_pCharacter->m_HelmWorn);
		m_pCharacter->m_HelmWorn.m_dwObjectID = 0L;
		m_pCharacter->m_HelmWorn.m_nMagicAdj = 0;
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
		if(m_pCharacter->m_Inventory[i].m_ObjectType == DND_OBJECT_TYPE_SHIELD)
		{
			if(m_pCharacter->m_Inventory[i].m_nMagicAdj > 0 && m_pCharacter->m_Inventory[i].m_bEnchanted == FALSE)
				szTemp.Format("%s +%d", m_pCharacter->m_Inventory[i].m_szType, m_pCharacter->m_Inventory[i].m_nMagicAdj);
			else if(m_pCharacter->m_Inventory[i].m_nMagicAdj < 0 && m_pCharacter->m_Inventory[i].m_bEnchanted == FALSE)
				szTemp.Format("%s %d", m_pCharacter->m_Inventory[i].m_szType, m_pCharacter->m_Inventory[i].m_nMagicAdj);
			else szTemp = m_pCharacter->m_Inventory[i].m_szType;

			m_cShieldCombo.InsertString(nShieldCount, szTemp);
			m_cShieldCombo.SetItemData(nShieldCount++, m_pCharacter->m_Inventory[i].m_dwObjectID);
		}
	}

	bFoundIt = FALSE;
	for(i = 0; i < m_cShieldCombo.GetCount(); ++i)
	{
		DWORD dwShieldId = m_cShieldCombo.GetItemData(i);

		if(dwShieldId == m_pCharacter->m_ShieldWorn.m_dwObjectID)
		{
			m_cShieldCombo.SetCurSel(i);
			bFoundIt = TRUE;
			break;
		}
	}

	if(bFoundIt == FALSE) //remove shield that no longer is in inventory
	{
		m_cShieldCombo.SetCurSel(0);
		_BlankShield.CopyTo(&m_pCharacter->m_ShieldWorn);
		m_pCharacter->m_ShieldWorn.m_dwObjectID = 0L;
		m_pCharacter->m_ShieldWorn.m_nMagicAdj = 0;
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
		if(m_pCharacter->m_Inventory[i].m_ObjectType == DND_OBJECT_TYPE_RING)
		{
			if(m_pCharacter->m_Inventory[i].m_nMagicAdj > 0)
				szTemp.Format("%s +%d", m_pCharacter->m_Inventory[i].m_szType, m_pCharacter->m_Inventory[i].m_nMagicAdj);
			else if(m_pCharacter->m_Inventory[i].m_nMagicAdj < 0)
				szTemp.Format("%s %d", m_pCharacter->m_Inventory[i].m_szType, m_pCharacter->m_Inventory[i].m_nMagicAdj);
			else szTemp = m_pCharacter->m_Inventory[i].m_szType;

			m_cRingLeftCombo.InsertString(nRingCount, szTemp);
			m_cRingLeftCombo.SetItemData(nRingCount, m_pCharacter->m_Inventory[i].m_dwObjectID);

			m_cRingRightCombo.InsertString(nRingCount, szTemp);
			m_cRingRightCombo.SetItemData(nRingCount, m_pCharacter->m_Inventory[i].m_dwObjectID);

			++nRingCount;
		}
	}

	bFoundIt = FALSE;
	for(i = 0; i < m_cRingLeftCombo.GetCount(); ++i)
	{
		DWORD dwRingId = m_cRingLeftCombo.GetItemData(i);

		if(dwRingId == m_pCharacter->m_RingsWorn[0].m_dwObjectID)
		{
			m_cRingLeftCombo.SetCurSel(i);
			bFoundIt = TRUE;
			break;
		}
	}

	if(bFoundIt == FALSE) //remove ring that no longer is in inventory
	{
		m_cRingLeftCombo.SetCurSel(0);
		_BlankRing.CopyTo(&m_pCharacter->m_RingsWorn[0]);
		m_pCharacter->m_RingsWorn[0].m_dwObjectID = 0L;
		m_pCharacter->m_RingsWorn[0].m_nMagicAdj = 0;
	}

	bFoundIt = FALSE;
	for(i = 0; i < m_cRingRightCombo.GetCount(); ++i)
	{
		DWORD dwRingId = m_cRingRightCombo.GetItemData(i);

		if(dwRingId == m_pCharacter->m_RingsWorn[1].m_dwObjectID)
		{
			m_cRingRightCombo.SetCurSel(i);
			bFoundIt = TRUE;
			break;
		}
	}

	if(bFoundIt == FALSE) //remove ring that no longer is in inventory
	{
		m_cRingRightCombo.SetCurSel(0);
		_BlankRing.CopyTo(&m_pCharacter->m_RingsWorn[1]);
		m_pCharacter->m_RingsWorn[1].m_dwObjectID = 0L;
		m_pCharacter->m_RingsWorn[1].m_nMagicAdj = 0;
	}

	
	m_szArmorClass.Format("%d / %d / %d", m_pCharacter->m_nCurrentArmorClass, m_pCharacter->m_nUnshieldedArmorClass, m_pCharacter->m_nRearArmorClass);

	if(m_pCharacter->m_ArmorWorn.m_wTypeId && m_pCharacter->m_ArmorWorn.m_nMagicAdj && m_pCharacter->m_ArmorWorn.m_bEnchanted == FALSE)
	{
		if(m_pCharacter->m_ArmorWorn.m_nMagicAdj > 0)
			m_szArmorPlus.Format("+%d",m_pCharacter->m_ArmorWorn.m_nMagicAdj);
		else
			m_szArmorPlus.Format("%d",m_pCharacter->m_ArmorWorn.m_nMagicAdj);
	}
	else
	{
		m_szArmorPlus = "";
	}

	if(m_pCharacter->m_ShieldWorn.m_wTypeId && m_pCharacter->m_ShieldWorn.m_nMagicAdj && m_pCharacter->m_ShieldWorn.m_bEnchanted == FALSE)
	{
		if(m_pCharacter->m_ShieldWorn.m_nMagicAdj > 0)
			m_szShieldPlus.Format("+%d",m_pCharacter->m_ShieldWorn.m_nMagicAdj);
		else
			m_szShieldPlus.Format("%d",m_pCharacter->m_ShieldWorn.m_nMagicAdj);
	}
	else
	{
		m_szShieldPlus = "";
	}

	//ring ring
	if(m_pCharacter->m_RingsWorn[0].m_wTypeId && m_pCharacter->m_RingsWorn[0].m_nMagicAdj)
	{
		if(m_pCharacter->m_RingsWorn[0].m_nMagicAdj > 0)
			m_szRingLeftPlusEdit.Format("+%d",m_pCharacter->m_RingsWorn[0].m_nMagicAdj);
		else
			m_szRingLeftPlusEdit.Format("%d",m_pCharacter->m_RingsWorn[0].m_nMagicAdj);
	}
	else
	{
		m_szRingLeftPlusEdit = "";
	}

	if(m_pCharacter->m_RingsWorn[1].m_wTypeId && m_pCharacter->m_RingsWorn[1].m_nMagicAdj)
	{
		if(m_pCharacter->m_RingsWorn[1].m_nMagicAdj > 0)
			m_szRingRightPlusEdit.Format("+%d",m_pCharacter->m_RingsWorn[1].m_nMagicAdj);
		else
			m_szRingRightPlusEdit.Format("%d",m_pCharacter->m_RingsWorn[1].m_nMagicAdj);
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
	m_cLightSourceCombo.ResetContent();

	int nWeaponCount = 0;
	int nAmmoCount = 0;
	int nLightSourceCount = 0;

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

	m_cLightSourceCombo.InsertString(nLightSourceCount, "None");
	m_cLightSourceCombo.SetItemData(nLightSourceCount, 0);
	m_cLightSourceCombo.SetCurSel(0);
	++nLightSourceCount;

	BOOL bFoundLightSource = FALSE;

	for (i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
	{
		if(m_pCharacter->m_Inventory[i].m_ObjectType == DND_OBJECT_TYPE_WEAPON)
		{
			if(m_pCharacter->m_Inventory[i].m_nMagicAdj && m_pCharacter->m_Inventory[i].m_bEnchanted == FALSE)
			{
				if(m_pCharacter->m_Inventory[i].m_nMagicAdj)
					szTemp.Format("%s +%d", m_pCharacter->m_Inventory[i].m_szType, m_pCharacter->m_Inventory[i].m_nMagicAdj);
				else
					szTemp.Format("%s %d", m_pCharacter->m_Inventory[i].m_szType, m_pCharacter->m_Inventory[i].m_nMagicAdj);
			}
			else
			{
				szTemp = m_pCharacter->m_Inventory[i].m_szType;
			}

			m_cWeaponCombo_1.InsertString(nWeaponCount, szTemp);
			m_cWeaponCombo_1.SetItemData(nWeaponCount, m_pCharacter->m_Inventory[i].m_dwObjectID);

			m_cWeaponCombo_2.InsertString(nWeaponCount, szTemp);
			m_cWeaponCombo_2.SetItemData(nWeaponCount, m_pCharacter->m_Inventory[i].m_dwObjectID);

			m_cWeaponCombo_3.InsertString(nWeaponCount, szTemp);
			m_cWeaponCombo_3.SetItemData(nWeaponCount, m_pCharacter->m_Inventory[i].m_dwObjectID);

			m_cWeaponCombo_4.InsertString(nWeaponCount, szTemp);
			m_cWeaponCombo_4.SetItemData(nWeaponCount, m_pCharacter->m_Inventory[i].m_dwObjectID);

			++nWeaponCount;
		}

		if(m_pCharacter->m_Inventory[i].m_ObjectType == DND_OBJECT_TYPE_AMMO || IsThrowableWeapon(m_pCharacter->m_Inventory[i].m_nFlags))
		{
			BOOL bAddAmmo = FALSE;

			if(IsThrowableWeapon(m_pCharacter->m_Inventory[i].m_nFlags))
			{
				if(m_pCharacter->m_SelectedWeapons[0].m_wTypeId == m_pCharacter->m_Inventory[i].m_wTypeId)
				{
					bAddAmmo = TRUE;
				}
			}
			else if(IsValidAmmoForWeapon(&m_pCharacter->m_SelectedWeapons[0], &m_pCharacter->m_Inventory[i]) )
			{
				bAddAmmo = TRUE;
			}

			if(bAddAmmo)
			{
				if(m_pCharacter->m_Inventory[i].m_lAmount == 0)
					m_pCharacter->m_Inventory[i].m_lAmount = 1;

				if(m_pCharacter->m_Inventory[i].m_nMagicAdj && m_pCharacter->m_Inventory[i].m_bEnchanted == FALSE)
				{
					if(m_pCharacter->m_Inventory[i].m_nMagicAdj)
						szTemp.Format("%s +%d (%d)", m_pCharacter->m_Inventory[i].m_szType, m_pCharacter->m_Inventory[i].m_nMagicAdj, m_pCharacter->m_Inventory[i].m_lAmount);
					else
						szTemp.Format("%s %d (%d)", m_pCharacter->m_Inventory[i].m_szType, m_pCharacter->m_Inventory[i].m_nMagicAdj, m_pCharacter->m_Inventory[i].m_lAmount);
				}
				else
				{
					szTemp.Format("%s (%d)",  m_pCharacter->m_Inventory[i].m_szType, m_pCharacter->m_Inventory[i].m_lAmount);
				}

				m_cAmmunitionCombo.InsertString(nAmmoCount, szTemp);
				m_cAmmunitionCombo.SetItemData(nAmmoCount, m_pCharacter->m_Inventory[i].m_dwObjectID);

				++nAmmoCount;
			}
		}

		POBJECTTYPE pLightObject = m_pApp->m_LightSourceIndexedTypeArray.GetAt(m_pCharacter->m_Inventory[i].m_wTypeId);
		if (NULL != pLightObject)
		{
			if (m_pCharacter->m_Inventory[i].m_wTypeId >= 10000 || m_pCharacter->m_Inventory[i].m_nMagicAdj)
			{
				// TRACE("LIGHT!");
				szTemp.Format("%s (%d ft.)", m_pCharacter->m_Inventory[i].m_szType, pLightObject->m_nCost);
				m_cLightSourceCombo.InsertString(nLightSourceCount, szTemp);
				m_cLightSourceCombo.SetItemData(nLightSourceCount, m_pCharacter->m_Inventory[i].m_dwObjectID);

				if (m_pCharacter->m_Inventory[i].m_dwObjectID == m_pCharacter->m_nLightSourceID)
				{
					m_cLightSourceCombo.SetCurSel(nLightSourceCount);
					m_pCharacter->m_nLightSourceRange = pLightObject->m_nCost;
					bFoundLightSource = TRUE;
				}

				++nLightSourceCount;
			}
		}
	}

	if (FALSE == bFoundLightSource)
	{
		m_pCharacter->m_nLightSourceRange = 0;
		m_pCharacter->m_nLightSourceID = 0;
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

		if(dwWeaponId == m_pCharacter->m_SelectedWeapons[0].m_dwObjectID)
		{
			m_cWeaponCombo_1.SetCurSel(i);
			bFoundIt = TRUE;
			break;
		}
	}

	if(bFoundIt == FALSE) //remove weapon that no longer is in inventory
	{
		m_cWeaponCombo_1.SetCurSel(0);
		_BlankWeapon.CopyTo(&m_pCharacter->m_SelectedWeapons[0]);
		m_pCharacter->m_SelectedWeapons[0].m_dwObjectID = 0L;
		m_pCharacter->m_SelectedWeapons[0].m_nMagicAdj = 0;
	}

	//WEAPON 2
	bFoundIt = FALSE;
	for(i = 0; i < m_cWeaponCombo_2.GetCount(); ++i)
	{
		DWORD dwWeaponId = m_cWeaponCombo_2.GetItemData(i);

		if(dwWeaponId == m_pCharacter->m_SelectedWeapons[1].m_dwObjectID)
		{
			m_cWeaponCombo_2.SetCurSel(i);
			bFoundIt = TRUE;
			break;
		}
	}

	if(bFoundIt == FALSE) //remove weapon that no longer is in inventory
	{
		m_cWeaponCombo_2.SetCurSel(0);
		_BlankWeapon.CopyTo(&m_pCharacter->m_SelectedWeapons[1]);
		m_pCharacter->m_SelectedWeapons[1].m_dwObjectID = 0L;
		m_pCharacter->m_SelectedWeapons[1].m_nMagicAdj = 0;
	}

	//WEAPON 3
	bFoundIt = FALSE;
	for(i = 0; i < m_cWeaponCombo_3.GetCount(); ++i)
	{
		DWORD dwWeaponId = m_cWeaponCombo_3.GetItemData(i);

		if(dwWeaponId == m_pCharacter->m_SelectedWeapons[2].m_dwObjectID)
		{
			m_cWeaponCombo_3.SetCurSel(i);
			bFoundIt = TRUE;
			break;
		}
	}

	if(bFoundIt == FALSE) //remove weapon that no longer is in inventory
	{
		m_cWeaponCombo_3.SetCurSel(0);
		_BlankWeapon.CopyTo(&m_pCharacter->m_SelectedWeapons[2]);
		m_pCharacter->m_SelectedWeapons[2].m_dwObjectID = 0L;
		m_pCharacter->m_SelectedWeapons[2].m_nMagicAdj = 0;
	}

	//WEAPON 4
	bFoundIt = FALSE;
	for(i = 0; i < m_cWeaponCombo_4.GetCount(); ++i)
	{
		DWORD dwWeaponId = m_cWeaponCombo_4.GetItemData(i);

		if(dwWeaponId == m_pCharacter->m_SelectedWeapons[3].m_dwObjectID)
		{
			m_cWeaponCombo_4.SetCurSel(i);
			bFoundIt = TRUE;
			break;
		}
	}

	if(bFoundIt == FALSE) //remove weapon that no longer is in inventory
	{
		m_cWeaponCombo_4.SetCurSel(0);
		_BlankWeapon.CopyTo(&m_pCharacter->m_SelectedWeapons[3]);
		m_pCharacter->m_SelectedWeapons[3].m_dwObjectID = 0L;
		m_pCharacter->m_SelectedWeapons[3].m_nMagicAdj = 0;
	}

	//AMMO
	bFoundIt = FALSE;
	for(i = 0; i < m_cAmmunitionCombo.GetCount(); ++i)
	{
		DWORD dwWeaponId = m_cAmmunitionCombo.GetItemData(i);

		if(dwWeaponId == m_pCharacter->m_SelectedAmmo.m_dwObjectID)
		{
			m_cAmmunitionCombo.SetCurSel(i);
			bFoundIt = TRUE;
			break;
		}
	}

	if(bFoundIt == FALSE) //remove weapon that no longer is in inventory
	{
		m_cAmmunitionCombo.SetCurSel(0);
		_BlankWeapon.CopyTo(&m_pCharacter->m_SelectedAmmo);
		m_pCharacter->m_SelectedAmmo.m_dwObjectID = 0L;
		m_pCharacter->m_SelectedAmmo.m_nMagicAdj = 0;
	}


	/////////////////////////////////////////////////////////

	if(m_pCharacter->m_SelectedWeapons[0].m_wTypeId && m_pCharacter->m_SelectedWeapons[0].m_nMagicAdj)
	{
		if(m_pCharacter->m_SelectedWeapons[0].m_nMagicAdj > 0)
			m_szWeaponPlus_1.Format("+%d",m_pCharacter->m_SelectedWeapons[0].m_nMagicAdj);
		else
			m_szWeaponPlus_1.Format("%d",m_pCharacter->m_SelectedWeapons[0].m_nMagicAdj);
	}
	else
	{
		m_szWeaponPlus_1 = "";
	}

	if(m_pCharacter->m_SelectedWeapons[1].m_wTypeId && m_pCharacter->m_SelectedWeapons[1].m_nMagicAdj)
	{
		if(m_pCharacter->m_SelectedWeapons[1].m_nMagicAdj > 0)
			m_szWeaponPlus_2.Format("+%d",m_pCharacter->m_SelectedWeapons[1].m_nMagicAdj);
		else
			m_szWeaponPlus_2.Format("%d",m_pCharacter->m_SelectedWeapons[1].m_nMagicAdj);
	}
	else
	{
		m_szWeaponPlus_2 = "";
	}

	if(m_pCharacter->m_SelectedWeapons[2].m_wTypeId && m_pCharacter->m_SelectedWeapons[2].m_nMagicAdj)
	{
		if(m_pCharacter->m_SelectedWeapons[2].m_nMagicAdj > 0)
			m_szWeaponPlus_3.Format("+%d",m_pCharacter->m_SelectedWeapons[2].m_nMagicAdj);
		else
			m_szWeaponPlus_3.Format("%d",m_pCharacter->m_SelectedWeapons[2].m_nMagicAdj);
	}
	else
	{
		m_szWeaponPlus_3 = "";
	}

	if(m_pCharacter->m_SelectedWeapons[3].m_wTypeId && m_pCharacter->m_SelectedWeapons[3].m_nMagicAdj)
	{
		if(m_pCharacter->m_SelectedWeapons[3].m_nMagicAdj > 0)
			m_szWeaponPlus_4.Format("+%d",m_pCharacter->m_SelectedWeapons[3].m_nMagicAdj);
		else
			m_szWeaponPlus_4.Format("%d",m_pCharacter->m_SelectedWeapons[3].m_nMagicAdj);
	}
	else
	{
		m_szWeaponPlus_4 = "";
	}

	if(m_pCharacter->m_SelectedAmmo.m_wTypeId && m_pCharacter->m_SelectedAmmo.m_nMagicAdj)
	{
		if(m_pCharacter->m_SelectedAmmo.m_nMagicAdj > 0)
			m_szAmmoPlus.Format("+%d",m_pCharacter->m_SelectedAmmo.m_nMagicAdj);
		else
			m_szAmmoPlus.Format("%d",m_pCharacter->m_SelectedAmmo.m_nMagicAdj);
	}
	else
	{
		m_szAmmoPlus = "";
	}


	int nMaxWeight = 0;
	int nEncumbrance = CalculateEncumbrance(m_pCharacter, &nMaxWeight);

	int nBaseMove = CalculateBaseMovement(m_pCharacter);

	int nMove = nBaseMove;

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

	// armor worn movement check
	if(m_pCharacter->m_ArmorWorn.m_wTypeId && m_pCharacter->m_ArmorWorn.m_nMagicAdj == 0)
	{
		if(nMove > m_pCharacter->m_ArmorWorn.m_nMove)
		{
			nMove = m_pCharacter->m_ArmorWorn.m_nMove;
		}
	}

	if(nMove < 0)
	{
		nMove = 0;
	}

	m_szMove.Format("%d", nMove);

	m_nCharacterMovementRate = nMove;

	GetCharacterVision(m_pCharacter);


	for(i = 0; i < m_cVisionCombo.GetCount(); ++i)
	{
		if((DND_CHARACTER_VISION_TYPES)m_cVisionCombo.GetItemData(i) == m_pCharacter->m_VisionType)
		{
			m_cVisionCombo.SetCurSel(i);
			break;
		}
	}

	m_bKeenEar = m_pCharacter->m_bKeenEar;
	m_bVeryKeenEar = m_pCharacter->m_bVeryKeenEar;

	//if (strcmp(m_pCharacter->m_szCharacterName, "Elric") == 0)
	//{
	//	TRACE("!");
	//}

	m_szSecondarySkill.Format("%s", m_pCharacter->m_szSecondarySkill);

	if(m_pCharacter->m_nConcealedMove)
	{
		m_szConcealedMove.Format("%d", m_pCharacter->m_nConcealedMove);
	}
	else
	{
		m_szConcealedMove = _T("");
	}

	if(m_pCharacter->m_nClimbingMove)
	{
		m_szClimbingMove.Format("%d", m_pCharacter->m_nClimbingMove);
	}
	else
	{
		m_szClimbingMove = _T("");
	}

	if(m_pCharacter->m_nSpecialMove)
	{
		m_szSpecialMove.Format("%d", m_pCharacter->m_nSpecialMove);
	}
	else
	{
		m_szSpecialMove = _T("");
	}
	

	/////////////////////////////////////////////////////////
	//Special abilities from Monster Manual

	int nSpecialAbility = 0;
	int nExceptionalAbility = 0;
	
	PDNDMONSTERMANUALENTRY pMonster = NULL;
	m_pApp->m_PCMonsterManualIndexedMap.Lookup(m_pCharacter->m_nRace, pMonster);

	if(pMonster != NULL)
	{
		m_szSpecialAttackComment.Format( "SPECIAL ATTACK: %s", pMonster->m_szSpecialAttack);
		m_szSpecialDefenseComment.Format( "SPECIAL DEFENSE: %s", pMonster->m_szSpecialDefense);

		if(pMonster->m_szSpecialAttack.GetLength())
			nSpecialAbility += 1;

		if(pMonster->m_szSpecialDefense.GetLength())
			nExceptionalAbility += 1;
	}
	else
	{
		m_szSpecialAttackComment = _T("");
		m_szSpecialDefenseComment = _T("");
	}

	int nXPValue = GetXPValueForCharacter(m_pCharacter, &nSpecialAbility, &nExceptionalAbility);

	if(m_pCharacter->m_nXPValue == 0)
	{
		m_nCalculatedXPValue = nXPValue;

		m_szXPValue.Format("%d", nXPValue);


		if(nSpecialAbility || nExceptionalAbility)
		{
			m_szXPValueComment.Format("(%dSpc%dExc)", nSpecialAbility, nExceptionalAbility);
		}
		else
		{
			m_szXPValueComment = _T("");
		}
	}
	else
	{
		m_nCalculatedXPValue = m_pCharacter->m_nXPValue;

		m_szXPValue.Format("%d", m_pCharacter->m_nXPValue);
		m_szXPValueComment.Format("CALC:%d (%dSpc%dExc)", nXPValue, nSpecialAbility, nExceptionalAbility);
	}


	//Languages
	m_cLanguagesListBox.ResetContent();

	int nIndex = 0;
	for(int i = 0; i < MAX_CHARACTER_LANGUAGES && m_pCharacter->m_nLanguages[i] > 0; ++i)
	{
		cDNDLanguage *pLanguage = (cDNDLanguage *)m_pApp->m_LanguageArray.GetAt(m_pCharacter->m_nLanguages[i]);
		m_cLanguagesListBox.InsertString(nIndex++, pLanguage->m_szLanguageName);
	}

	/////////////////////////////////////////////////////////


	if(m_pCharSheetDialog != NULL)
	{
		m_pCharSheetDialog->InvalidateRect(NULL);
	}

	BOOL bIsSpellCaster = GetSpellClasses(m_pCharacter);

	if(bIsSpellCaster)
	{
		if(m_pSpellsDialog == NULL)
		{
			m_pSpellsDialog = new DMCharSpellsDialog(m_pCharacter, this, m_pParent);
			m_bFirstSpellViewRefresh = TRUE;
		}

		if(m_pSpellsDialog != NULL && m_pSpellsDialog->m_pCharacter != NULL)
		{
			int nOldClass = m_nSelectedSpellViewClass;
			m_pSpellsDialog->Refresh();
			
			if(!m_bFirstSpellViewRefresh)
			{
				m_nSelectedSpellViewClass = nOldClass;
			}
			m_bFirstSpellViewRefresh = FALSE;
		}

		m_cSpellsButton.EnableWindow(TRUE);
	}
	else
	{
		m_cSpellsButton.EnableWindow(FALSE);
	}

	//generate height/weight if they are unknown
	if(m_pCharacter->m_nHeight == 0 || m_pCharacter->m_nWeight == 0)
	{
		GenerateHeightWeight(m_pCharacter);
	}

	//generate age if they are unknown
	if(m_pCharacter->m_nAge == 0)
	{
		m_pCharacter->m_nAge = GenerateCharacterAge(m_pCharacter);
	}

	if(m_pCharacter->m_nDOBMonth == 0)
	{
		m_pCharacter->m_nDOBMonth = (rand()%12)+1;
	}
	if(m_pCharacter->m_nDOBDay == 0)
	{
		m_pCharacter->m_nDOBDay = (rand()%28)+1;
	}

	if(m_pCharacter->m_nDOBYear == 0 && m_nYear != 0)
	{
		m_pCharacter->m_nDOBYear = m_nYear - m_pCharacter->m_nAge;

		//not past birthday yet ?
		if(m_nMonth < m_pCharacter->m_nDOBMonth || (m_nMonth == m_pCharacter->m_nDOBMonth && m_nDayofMonth < m_pCharacter->m_nDOBDay))
		{
			m_pCharacter->m_nDOBYear -= 1;
		}
	}


	UpdateData(FALSE);

	m_pApp->m_pMainWindow->InvalidateRect(NULL);

	m_pParent->InvalidateRect(NULL);


} // end ::Refresh()

void CDMCharViewDialog::ProcessCharStats() 
{
	//if (strcmp(m_pCharacter->m_szCharacterName, "Elric") == 0)
	//{
	//	TRACE("!");
	//}

	CString szTemp;

	int nRaceBonus[7] = {0,0,0,0,0,0,0};

	m_bNPCCheck = m_pCharacter->m_bIsHenchman || m_pCharacter->m_bPocketPC;

	AddStatModifiersByRace(m_pCharacter, nRaceBonus);

	m_szSTRCommentBuffer = _T("");
	m_szINTCommentBuffer = _T("");
	m_szDEXCommentBuffer = _T("");
	m_szCONCommentBuffer = _T("");

	for(int i = 0; i<= ATTRIB_COM; ++i)
	{
		if(m_pCharacter->m_nEditedStats[i] != 0)
		{
			m_pCharacter->m_nDisplayStats[i] = m_pCharacter->m_nEditedStats[i];
		}
		else
		{
			m_pCharacter->m_nDisplayStats[i] = m_pCharacter->m_nBaseStats[i] + nRaceBonus[i];
		}

		m_pCharacter->m_nDisplayStats[i] = min(25, m_pCharacter->m_nDisplayStats[i]);
	}

	if(m_pCharacter->m_nEditedStats[ATTRIB_EXSTR] != 0)
	{
		m_pCharacter->m_nDisplayStats[ATTRIB_EXSTR] = m_pCharacter->m_nEditedStats[ATTRIB_EXSTR];
	}
	else
	{
		m_pCharacter->m_nDisplayStats[ATTRIB_EXSTR] = m_pCharacter->m_nBaseStats[ATTRIB_EXSTR]; 
	}

	m_pCharacter->m_nDisplayStats[ATTRIB_EXSTR] = min(100, m_pCharacter->m_nDisplayStats[ATTRIB_EXSTR]);


	int nSTRDamAdj = 0;
	int nWeightAllow = 0;
	int nOpenDoors = 0;
	int nBendBars = 0;

	int nSTRToHitAdj = CalculateSTRAdjustments(m_pCharacter, &nSTRDamAdj, &nWeightAllow, &nOpenDoors, &nBendBars);

	if(nSTRToHitAdj >= 0)
		szTemp.Format("TH:+%d ",nSTRToHitAdj);
	else
		szTemp.Format("TH:%d ",nSTRToHitAdj);

	m_szSTRCommentBuffer += szTemp;

	if(nSTRDamAdj >= 0)
		szTemp.Format("DMG:+%d ",nSTRDamAdj);
	else
		szTemp.Format("DMG:%d ",nSTRDamAdj);

	m_szSTRCommentBuffer += szTemp;

	if(nWeightAllow >= 0)
		szTemp.Format("WA:+%d ",nWeightAllow);
	else
		szTemp.Format("WA:%d ",nWeightAllow);

	m_szSTRCommentBuffer += szTemp;

	szTemp.Format("OD:1-%d BBLG:%d%c", nOpenDoors, nBendBars, 37);

	m_szSTRCommentBuffer += szTemp;

	int nKnowSpell = 0;
	int nMinSpells = 0;
	int nMaxSpells = 0;

	int nAddLang = CalculateINTAdjustments(m_pCharacter, &nKnowSpell, &nMinSpells, &nMaxSpells);

	szTemp.Format("ALang:%d ", nAddLang);
	m_szINTCommentBuffer += szTemp;

	if(nKnowSpell)
	{
		szTemp.Format("KSpell:%d%c MinS:%d MaxS:", nKnowSpell, 37, nMinSpells);
		m_szINTCommentBuffer += szTemp;

		if(nMaxSpells > 18)
			szTemp="ALL";
		else
			szTemp.Format("%d", nMaxSpells);

		m_szINTCommentBuffer += szTemp;
	}

	int nSpellFailure = 100;
	char *szSpellBonus;
	int nMagicAttackAdj = CalculateWISAdjustments(m_pCharacter,  &szSpellBonus, &nSpellFailure);

	if(nMagicAttackAdj > 0)
		m_szWISCommentBuffer.Format("MagAttAdj: +%d SpellBonus: %s SpellFail: %d%c", nMagicAttackAdj, szSpellBonus, nSpellFailure, 37);
	else
		m_szWISCommentBuffer.Format("MagAttAdj: %d SpellBonus: %s SpellFail: %d%c", nMagicAttackAdj, szSpellBonus, nSpellFailure, 37);


	int nDefenseAdj = 0;
	int nReactAttack = CalculateDEXAdjustments(m_pCharacter, &nDefenseAdj);

	if(nReactAttack < 0)
		szTemp.Format("RA Adj:%d ",nReactAttack);
	else
		szTemp.Format("RA Adj:+%d ",nReactAttack);

	m_szDEXCommentBuffer += szTemp;

	if(nDefenseAdj < 0)
		szTemp.Format("DefAdj:%d ",nDefenseAdj);
	else
		szTemp.Format("DefAdj:+%d ",nDefenseAdj);

	m_szDEXCommentBuffer += szTemp;

	m_nDexAdjToInitiative = nReactAttack;


	m_pCharacter->m_nHitPoints = 0;
	
	int nSysShock = 0;
	int nResSurvival = 0;
	int nConBonus = CalculateCONAdjustments(m_pCharacter, &nSysShock, &nResSurvival);

	if(nConBonus)
	{
		if(nConBonus > 0)
			szTemp.Format("HPAdj: +%d ",nConBonus);
		else
			szTemp.Format("HPAdj: %d ",nConBonus);

		m_szCONCommentBuffer += szTemp;
	}

	szTemp.Format("SS:%d%c RS:%d%c", nSysShock, 37, nResSurvival, 37);
	m_szCONCommentBuffer += szTemp;


	int nLoyaltyBase = 0;
	int nReactAdj = 0;

	int nMaxHenchmen = CalculateCHAAdjustments(m_pCharacter, &nLoyaltyBase, &nReactAdj);

	CString szPart1;
	CString szPart2;

	if(nLoyaltyBase >=0)
		szPart1.Format("+%d%c", nLoyaltyBase, 37);
	else
		szPart1.Format("%d%c", nLoyaltyBase, 37);

	if(nReactAdj >=0)
		szPart2.Format("+%d%c", nLoyaltyBase, 37);
	else
		szPart2.Format("%d%c", nLoyaltyBase, 37);

	m_szCHACommentBuffer.Format("MaxHench:%d LoyalBase:%s ReactAdj:%s", nMaxHenchmen, szPart1.GetBuffer(0), szPart2.GetBuffer(0));


	//////////////////////////////////////////////////////////////

	// fix up hitpoint rolls for dual classed humans
	if(m_pCharacter->m_nRace == DND_CHARACTER_RACE_HUMAN)
	{
		if(m_pCharacter->m_Class[1] != DND_CHARACTER_CLASS_UNDEF)
		{
			for(int i = m_pCharacter->m_nLevel[0]; i >= 0; --i)
			{
				m_pCharacter->m_nHitPointRolls[1][i] = 0;
			}
		}

		if(m_pCharacter->m_Class[2] != DND_CHARACTER_CLASS_UNDEF)
		{
			for(int i = m_pCharacter->m_nLevel[1]; i >= 0; --i)
			{
				m_pCharacter->m_nHitPointRolls[2][i] = 0;
			}
		}
	}

	int nTotalClasses = 0;

	for(int j = 0; j < 3; ++j)
	{
		if(!m_pCharacter->m_Class[j]) 
			continue;

		++nTotalClasses;

		int nXtraHitPoints = 0;
		int nMaxHitDice = GetMaxHitDiceByClass(m_pCharacter->m_Class[j], &nXtraHitPoints);

		int nHDLevel = m_pCharacter->m_nLevel[j];

		int nStartAtLevel = 1;

		BOOL bClassHas2HDAtFirstLevel = FALSE;
		#if CUSTOM_CLASSES
		if (IsCustomClass(m_pCharacter->m_Class[j]))
		{
			if (GetCustomClass(m_pCharacter->m_Class[j])->m_nFirstLevelHD == 2)
			{
				bClassHas2HDAtFirstLevel = TRUE;
			}
		}
		#endif

		if (m_pCharacter->m_Class[j] == DND_CHARACTER_CLASS_NONE || m_pCharacter->m_Class[j] == DND_CHARACTER_CLASS_RANGER || m_pCharacter->m_Class[j] == DND_CHARACTER_CLASS_MONK || bClassHas2HDAtFirstLevel == TRUE)// || m_pCharacter->m_Class[j] == DND_CHARACTER_CLASS_BARBARIAN)
		{
			nStartAtLevel = 0;
		}

		m_szHPByLevelBuffer = "HP by level: ";

		BOOL bFirstDie = TRUE;
		int nAddedXtraPoints = 0;
		for(i = nStartAtLevel; i <= nHDLevel; ++i)
		{
			if(i <= nMaxHitDice)
			{
				int nAddPoints = 0;
				
				if(m_pCharacter->m_nHitPointRolls[j][i] > 0)
				{
					nAddPoints = (m_pCharacter->m_nHitPointRolls[j][i] + nConBonus);
				
					if(nAddPoints < 1)
						nAddPoints = 1;
				}

				m_pCharacter->m_nHitPoints += nAddPoints;

				if(bFirstDie)
				{
					#if 0
					if(nConBonus)
					{
						szTemp.Format("%d(+%d)", m_pCharacter->m_nHitPointRolls[0][i],nConBonus);
					}
					else
					{
						szTemp.Format("%d", (m_pCharacter->m_nHitPointRolls[0][i]));
					}
					#else
					szTemp.Format("%d", nAddPoints);
					#endif
				}
				else
				{
					#if 0
					if(nConBonus)
					{
						szTemp.Format("+%d(+%d)", m_pCharacter->m_nHitPointRolls[j][i],nConBonus);
					}
					else
					{
						szTemp.Format("+%d", m_pCharacter->m_nHitPointRolls[j][i]);
					}
					#else
					szTemp.Format("+%d", nAddPoints);	
					#endif
				}

				 bFirstDie = FALSE;

				 m_szHPByLevelBuffer += szTemp;
			}
			else
			{
				m_pCharacter->m_nHitPoints += nXtraHitPoints;
				nAddedXtraPoints += nXtraHitPoints;
			}
		}

		if(nAddedXtraPoints)
		{
			szTemp.Format("+%d", nAddedXtraPoints);
			m_szHPByLevelBuffer += szTemp;
		}
	} // end j loop

	if(nTotalClasses)
	{	
		if(m_pCharacter->m_nRace != DND_CHARACTER_RACE_HUMAN)
		{
			m_pCharacter->m_nHitPoints /= nTotalClasses;
		}
	}

	//armor class calcs

	//ring ring
	int nProtectionRing = 0;

	if(m_pCharacter->m_RingsWorn[0].IsProtectionRing())
	{
		nProtectionRing = m_pCharacter->m_RingsWorn[0].m_nMagicAdj;
	}
	if(m_pCharacter->m_RingsWorn[1].IsProtectionRing())
	{
		nProtectionRing = max(nProtectionRing, m_pCharacter->m_RingsWorn[1].m_nMagicAdj);
	}

	int nArmorProtectionRing = nProtectionRing;

	if(m_pCharacter->m_ArmorWorn.m_wTypeId == 0)
	{
		m_pCharacter->m_ArmorWorn.m_nArmorClass = CalculateBaseArmorClass(m_pCharacter);
	}
	else
	{
		if(m_pCharacter->m_ArmorWorn.m_nMagicAdj)
		{
			nArmorProtectionRing = 0;
		}
	}


	m_pCharacter->m_nRearArmorClass = m_pCharacter->m_ArmorWorn.m_nArmorClass - m_pCharacter->m_ArmorWorn.m_nMagicAdj - nArmorProtectionRing;
	m_pCharacter->m_nUnshieldedArmorClass = m_pCharacter->m_nRearArmorClass + nDefenseAdj;
	m_pCharacter->m_nCurrentArmorClass = m_pCharacter->m_nUnshieldedArmorClass;

	if(m_pCharacter->m_ShieldWorn.m_wTypeId)
	{
		m_pCharacter->m_nCurrentArmorClass -= 1;
		m_pCharacter->m_nCurrentArmorClass -= m_pCharacter->m_ShieldWorn.m_nMagicAdj;
	}

	if(m_pCharacter->m_nCurrentArmorClass < -10)
		m_pCharacter->m_nCurrentArmorClass = -10;

	if(m_pCharacter->m_nCurrentArmorClass > 10)
		m_pCharacter->m_nCurrentArmorClass = 10;


	if(m_pCharacter->m_nUnshieldedArmorClass > 10)
		m_pCharacter->m_nUnshieldedArmorClass = 10;

	if(m_pCharacter->m_nUnshieldedArmorClass < -10)
		m_pCharacter->m_nUnshieldedArmorClass = -10;

	if(m_pCharacter->m_nRearArmorClass > 10)
		m_pCharacter->m_nRearArmorClass = 10;

	if(m_pCharacter->m_nRearArmorClass < -10)
		m_pCharacter->m_nRearArmorClass = -10;


	//////////////////////////////////////////////

	int nSavesMatrix[5];
	GetSavingThrows(m_pCharacter, nSavesMatrix);

	m_szSaves_1.Format("%d", nSavesMatrix[0]);
	m_szSaves_2.Format("%d", nSavesMatrix[1]);
	m_szSaves_3.Format("%d", nSavesMatrix[2]);
	m_szSaves_4.Format("%d", nSavesMatrix[3]);
	m_szSaves_5.Format("%d", nSavesMatrix[4]);

	//////////////////////////////////////////////

	int nTurnUndeadMatrix[13];
	BOOL bTurnUndead = GetTurnUndeadMatrix(m_pCharacter, nTurnUndeadMatrix);

	int nShowTurnUndead = SW_HIDE;

	if(bTurnUndead)
	{
		nShowTurnUndead = SW_SHOW;

		CString *pszUndeadStrings[13] = 
		{
			&m_szTurnUndead1,
			&m_szTurnUndead2,
			&m_szTurnUndead3,
			&m_szTurnUndead4,
			&m_szTurnUndead5,
			&m_szTurnUndead6,
			&m_szTurnUndead7,
			&m_szTurnUndead8,
			&m_szTurnUndead9,
			&m_szTurnUndead10,
			&m_szTurnUndead11,
			&m_szTurnUndead12,
			&m_szTurnUndead13
		};

		for(i = 0; i < 13; ++i)
		{
			switch(nTurnUndeadMatrix[i])
			{
				case -2:	*pszUndeadStrings[i] = _T("dmn"); break;
				case -1:	*pszUndeadStrings[i] = _T("turn"); break;
				case 99:	*pszUndeadStrings[i] = _T("-"); break;
				default:	pszUndeadStrings[i]->Format("%d", nTurnUndeadMatrix[i]); break;
			}
		}

	}

	int nTurnUndeadControls[15] = 
	{
		IDC_TURN_STATIC_1,
		IDC_TURN_STATIC_2,
		IDC_TURN_UNDEAD_EDIT_1,
		IDC_TURN_UNDEAD_EDIT_2,
		IDC_TURN_UNDEAD_EDIT_3,
		IDC_TURN_UNDEAD_EDIT_4,
		IDC_TURN_UNDEAD_EDIT_5,
		IDC_TURN_UNDEAD_EDIT_6,
		IDC_TURN_UNDEAD_EDIT_7,
		IDC_TURN_UNDEAD_EDIT_8,
		IDC_TURN_UNDEAD_EDIT_9,
		IDC_TURN_UNDEAD_EDIT_10,
		IDC_TURN_UNDEAD_EDIT_11,
		IDC_TURN_UNDEAD_EDIT_12,
		IDC_TURN_UNDEAD_EDIT_13
	};

	if (m_hWnd != NULL)
	{
		for (i = 0; i < 15; ++i)
		{
			(GetDlgItem(nTurnUndeadControls[i]))->ShowWindow(nShowTurnUndead);
		}
	}

	//////////////////////////////////////////////

	float fThiefSkillMatrix[8];
	GetThiefSkills(m_pCharacter, fThiefSkillMatrix);

	int nShowThiefStats = SW_HIDE;
	if(fThiefSkillMatrix[1] > 0.0f)
	{
		m_szThiefSkill_1.Format("%3.0f", fThiefSkillMatrix[0]);
		m_szThiefSkill_2.Format("%3.0f", fThiefSkillMatrix[1]);
		m_szThiefSkill_3.Format("%3.0f", fThiefSkillMatrix[2]);
		m_szThiefSkill_4.Format("%3.0f", fThiefSkillMatrix[3]);
		m_szThiefSkill_5.Format("%3.0f", fThiefSkillMatrix[4]);
		m_szThiefSkill_6.Format("%3.0f", fThiefSkillMatrix[5]);
		m_szThiefSkill_7.Format("%3.0f", fThiefSkillMatrix[6]);
		m_szThiefSkill_8.Format("%3.0f", fThiefSkillMatrix[7]);
		nShowThiefStats = SW_SHOW;
	}
	else
	{
		m_szThiefSkill_1 = _T("-");
		m_szThiefSkill_2 = _T("-");
		m_szThiefSkill_3 = _T("-");
		m_szThiefSkill_4 = _T("-");
		m_szThiefSkill_5 = _T("-");
		m_szThiefSkill_6 = _T("-");
		m_szThiefSkill_7 = _T("-");
		m_szThiefSkill_8 = _T("-");
	}

	int nThiefControls[17] = 
	{
		IDC_THIEF_STATIC_0,
		IDC_THIEF_STATIC_1,
		IDC_THIEF_STATIC_2,
		IDC_THIEF_STATIC_3,
		IDC_THIEF_STATIC_4,
		IDC_THIEF_STATIC_5,
		IDC_THIEF_STATIC_6,
		IDC_THIEF_STATIC_7,
		IDC_THIEF_STATIC_8,

		IDC_THIEF_SKILL_EDIT_1,
		IDC_THIEF_SKILL_EDIT_2,
		IDC_THIEF_SKILL_EDIT_3,
		IDC_THIEF_SKILL_EDIT_4,
		IDC_THIEF_SKILL_EDIT_5,
		IDC_THIEF_SKILL_EDIT_6,
		IDC_THIEF_SKILL_EDIT_7,
		IDC_THIEF_SKILL_EDIT_8
	};

	if (m_hWnd != NULL)
	{
		for (i = 0; i < 17; ++i)
		{
			(GetDlgItem(nThiefControls[i]))->ShowWindow(nShowThiefStats);
		}
	}

	//////////////////////////////////////////////

	int nAssassinMatrix[10];
	int nAssassinLevel = GetAssassinationTable(m_pCharacter, nAssassinMatrix);

	int nShowAssassinStats = SW_HIDE;
	if(nAssassinLevel > 0)
	{
		m_szAssEdit1.Format("%d", nAssassinMatrix[0]);
		m_szAssEdit2.Format("%d", nAssassinMatrix[1]);
		m_szAssEdit3.Format("%d", nAssassinMatrix[2]);
		m_szAssEdit4.Format("%d", nAssassinMatrix[3]);
		m_szAssEdit5.Format("%d", nAssassinMatrix[4]);
		m_szAssEdit6.Format("%d", nAssassinMatrix[5]);
		m_szAssEdit7.Format("%d", nAssassinMatrix[6]);
		m_szAssEdit8.Format("%d", nAssassinMatrix[7]);
		m_szAssEdit9.Format("%d", nAssassinMatrix[8]);
		m_szAssEdit10.Format("%d", nAssassinMatrix[9]);
	
		nShowAssassinStats = SW_SHOW;
	}
	else
	{
		m_szAssEdit1 = _T("-");
		m_szAssEdit2 = _T("-");
		m_szAssEdit3 = _T("-");
		m_szAssEdit4 = _T("-");
		m_szAssEdit5 = _T("-");
		m_szAssEdit6 = _T("-");
		m_szAssEdit7 = _T("-");
		m_szAssEdit8 = _T("-");
		m_szAssEdit9 = _T("-");
		m_szAssEdit10 = _T("-");
	}

	int nAssControls[21] = 
	{
		IDC_ASS_STATIC_0,
		IDC_ASS_STATIC_1,
		IDC_ASS_STATIC_2,
		IDC_ASS_STATIC_3,
		IDC_ASS_STATIC_4,
		IDC_ASS_STATIC_5,
		IDC_ASS_STATIC_6,
		IDC_ASS_STATIC_7,
		IDC_ASS_STATIC_8,
		IDC_ASS_STATIC_9,
		IDC_ASS_STATIC_10,

		IDC_ASS_SKILL_EDIT_1,
		IDC_ASS_SKILL_EDIT_2,
		IDC_ASS_SKILL_EDIT_3,
		IDC_ASS_SKILL_EDIT_4,
		IDC_ASS_SKILL_EDIT_5,
		IDC_ASS_SKILL_EDIT_6,
		IDC_ASS_SKILL_EDIT_7,
		IDC_ASS_SKILL_EDIT_8,
		IDC_ASS_SKILL_EDIT_9,
		IDC_ASS_SKILL_EDIT_10
	};

	if (m_hWnd != NULL) // unit test support
	{
		for (i = 0; i < 21; ++i)
		{
			(GetDlgItem(nAssControls[i]))->ShowWindow(nShowAssassinStats);
		}
	}

	//////////////////////////////////////////////

	if (m_hWnd != NULL) // unit test support
	{
		BOOL bRemove = m_cWeaponChartList.DeleteAllItems();

		int nAttackMatrix[21];
		GetToHitChart(m_pCharacter, nAttackMatrix);

		m_cWeaponChartList.InsertItem(0, "BASE");

		//m_szDamageDesc = "-";
		m_szDamageDesc.Format("%s", m_pCharacter->GetUnarmedDamageDesc());
		m_cWeaponChartList.SetItemText(0, 23, m_szDamageDesc);

		for (i = 0; i < 21; ++i)
		{
			szTemp.Format("%d", nAttackMatrix[i] - nSTRToHitAdj);
			m_nAttackMatrix[i] = nAttackMatrix[i] - nSTRToHitAdj;
			m_cWeaponChartList.SetItemText(0, i + 2, szTemp);
		}

		int nWeaponCount = 1;
		for (j = 0; j < 4; ++j)
		{
			if (!m_pCharacter->m_SelectedWeapons[j].m_wTypeId)
				continue;

			if (m_pCharacter->m_SelectedWeapons[j].m_wTypeId > m_pApp->m_WeaponsIndexedTypeArray.GetSize())
				continue;

			m_cWeaponChartList.InsertItem(nWeaponCount, m_pCharacter->m_SelectedWeapons[j].m_szType);

			int nMagicAdj = m_pCharacter->m_SelectedWeapons[j].m_nMagicAdj;

			cDNDWeapon *pDamageWeapon = &m_pCharacter->m_SelectedWeapons[j];

			if (IsMissileWeapon(&m_pCharacter->m_SelectedWeapons[j]) && IsValidAmmoForWeapon(&m_pCharacter->m_SelectedWeapons[j], &m_pCharacter->m_SelectedAmmo))
			{
				nMagicAdj += m_pCharacter->m_SelectedAmmo.m_nMagicAdj;

				if (m_pCharacter->m_SelectedAmmo.m_szDamageSmall[0] != '-')  // for sling stone weirdness
				{
					pDamageWeapon = &m_pCharacter->m_SelectedAmmo;
				}
			}

			int nSpecializedToHitBonus = 0;
			int nSpecializedDamageBonus = 0;
			if (m_pCharacter->IsProficientWithWeapon(&m_pCharacter->m_SelectedWeapons[j], &nSpecializedToHitBonus, &nSpecializedDamageBonus) == FALSE)
			{
				int nProfPenalty = 0;
				int nNumProfs = CalculateWeaponProficiencies(m_pCharacter, &nProfPenalty);
				nMagicAdj += nProfPenalty;
			}

			if (nMagicAdj)
			{
				if (nMagicAdj > 0)
					szTemp.Format("+%d", nMagicAdj);
				else
					szTemp.Format("%d", nMagicAdj);

				m_cWeaponChartList.SetItemText(nWeaponCount, 1, szTemp);
			}

			int nToHitAdj = 0;
			int nDamageAdj = 0;

			nToHitAdj += nSpecializedToHitBonus;
			nDamageAdj += nSpecializedDamageBonus;

			if (m_pCharacter->m_SelectedWeapons[j].m_nExceptionalStrength)
			{
				nToHitAdj += nSTRToHitAdj;
				nDamageAdj += nSTRDamAdj;
			}

			nToHitAdj += nMagicAdj;
			nDamageAdj += nMagicAdj;

			for (i = 0; i < 21; ++i)
			{
				szTemp.Format("%d", nAttackMatrix[i] - nToHitAdj);
				m_cWeaponChartList.SetItemText(nWeaponCount, i + 2, szTemp);

				if (j == 0)
				{
					m_nAttackMatrix[i] = nAttackMatrix[i] - nToHitAdj;
				}
			}

			if (nDamageAdj)
			{
				szTemp.Format("%s +%d / %s +%d %s", pDamageWeapon->m_szDamageSmall, nDamageAdj, pDamageWeapon->m_szDamageLarge, nDamageAdj, GetMonkWeaponDamageAdj(m_pCharacter, j));
			}
			else
			{
				szTemp.Format("%s / %s %s", pDamageWeapon->m_szDamageSmall, pDamageWeapon->m_szDamageLarge, GetMonkWeaponDamageAdj(m_pCharacter, j));
			}

			if (IsMissileWeapon(&m_pCharacter->m_SelectedWeapons[j]) && !IsValidAmmoForWeapon(&m_pCharacter->m_SelectedWeapons[j], &m_pCharacter->m_SelectedAmmo))
			{
				if (nWeaponCount == 1)
				{
					szTemp = "NO AMMO";
				}
				else
				{
					szTemp = _T("");
				}
			}

			if (j == 0)
			{
				szTemp.Replace("+-", "-");
				m_szDamageDesc = szTemp;
				m_szDamageStat = szTemp;
				m_szDamageStat.Replace(" ", "");
			}

			m_cWeaponChartList.SetItemText(nWeaponCount, 23, szTemp);

			++nWeaponCount;
		}

		m_szNumAttacks.Format("%s", CalculateAttacksPerRound(m_pCharacter));

		m_pCharacter->ValidateLanguages();

		//////////////////////////////////////////////

		//if (strcmp(m_pCharacter->m_szCharacterName, "Elric") == 0)
		//{
		//	TRACE("!");
		//}

		UpdateData(FALSE);

	}
	
}


void CDMCharViewDialog::RollHitpoints()
{
	int nDieType = GetHitDieTypeByClass(m_pCharacter->m_Class[0]);

	BOOL bClassHas2HDAtFirstLevel = FALSE;
	#if CUSTOM_CLASSES
	if (IsCustomClass(m_pCharacter->m_Class[0]))
	{
		if (GetCustomClass(m_pCharacter->m_Class[0])->m_nFirstLevelHD == 2)
		{
			bClassHas2HDAtFirstLevel = TRUE;
		}
	}
	#endif

	//assuming 18 is max HD for any char, as it is the max for a monk
	for(int i = 0; i <= 18; ++i)
	{
		if(g_bMaxHitPointsAtFirstLevel && i == 0)
		{
			m_pCharacter->m_nHitPointRolls[0][i] = nDieType;
		}
		else if (g_bMaxHitPointsAtFirstLevel && i == 1 && (m_pCharacter->m_Class[0] == DND_CHARACTER_CLASS_RANGER || m_pCharacter->m_Class[0] == DND_CHARACTER_CLASS_MONK || bClassHas2HDAtFirstLevel == TRUE))
		{
			m_pCharacter->m_nHitPointRolls[0][i] = nDieType;
		}
		else if(g_bReRollLessHalfOnHitDie)
		{
			int nRoll = 0;
			do
			{
				nRoll = nRollDie(nDieType);
			} while(nRoll <= nDieType/2);

			m_pCharacter->m_nHitPointRolls[0][i] = nRoll;
		}
		else if(g_bReRollOnesOnHitDie)
		{
			int nRoll = 0;
			do
			{
				nRoll = nRollDie(nDieType);
			} while(nRoll < 2);

			m_pCharacter->m_nHitPointRolls[0][i] = nRoll;
		}
		else
		{
			m_pCharacter->m_nHitPointRolls[0][i] = nRollDie(nDieType);
		}
	}

	if(m_pCharacter->m_Class[1])
	{
		nDieType = GetHitDieTypeByClass(m_pCharacter->m_Class[1]);

		for(int i = 0; i <= 18; ++i)
		{
			m_pCharacter->m_nHitPointRolls[1][i] = nRollDie(nDieType);
		}
	}
	else
	{
		for(int i = 0; i <= 18; ++i)
		{
			m_pCharacter->m_nHitPointRolls[1][i] = 0;
		}
	}

	if(m_pCharacter->m_Class[2])
	{
		nDieType = GetHitDieTypeByClass(m_pCharacter->m_Class[2]);

		for(int i = 0; i <= 18; ++i)
		{
			m_pCharacter->m_nHitPointRolls[2][i] = nRollDie(nDieType);
		}
	}
	else
	{
		for(int i = 0; i <= 18; ++i)
		{
			m_pCharacter->m_nHitPointRolls[2][i] = 0;
		}
	}

}

void CDMCharViewDialog::NPCRollStats()
{
	DWORD dwOldID = m_pCharacter->m_dwCharacterID;

	m_pCharacter->m_dwCharacterID = 0;

	OnRollStats();

	RollHitpoints();

	m_pCharacter->m_dwCharacterID = dwOldID;
}

void CDMCharViewDialog::ExternalRollPCStats() // unit test support
{
	OnRollStats();
}

void CDMCharViewDialog::OnRollStats()  //also grant earned XP
{
	if(m_pCharacter->m_dwCharacterID == 0L)
	{
		/*
		int nPasses = 0;
		int nRoll = 0;

		do
		{
			nRoll = RollStat();
			++nPasses;
		}
		while(nRoll < 18);

		TRACE(">> %d PASSES TO GET AN 18 !\n", nPasses);
		*/
		
		BOOL bCharRolled = TRUE;

		int nMinStats[7] = {0,0,0,0,0,0,0};

		GetMinimumStatsByClass(m_pCharacter, nMinStats);

		m_pCharacter->m_nEditedStats[ATTRIB_STR] = 0;
		m_pCharacter->m_nEditedStats[ATTRIB_INT] = 0;
		m_pCharacter->m_nEditedStats[ATTRIB_WIS] = 0;
		m_pCharacter->m_nEditedStats[ATTRIB_DEX] = 0;
		m_pCharacter->m_nEditedStats[ATTRIB_CON] = 0;
		m_pCharacter->m_nEditedStats[ATTRIB_CHA] = 0;
		m_pCharacter->m_nEditedStats[ATTRIB_COM] = 0;
		m_pCharacter->m_nEditedStats[ATTRIB_EXSTR] = 0;

		do
		{
			bCharRolled = TRUE;
			m_pCharacter->m_nBaseStats[ATTRIB_STR] = RollStat();
			m_pCharacter->m_nBaseStats[ATTRIB_INT] = RollStat();
			m_pCharacter->m_nBaseStats[ATTRIB_WIS] = RollStat();
			m_pCharacter->m_nBaseStats[ATTRIB_DEX] = RollStat();
			m_pCharacter->m_nBaseStats[ATTRIB_CON] = RollStat();
			m_pCharacter->m_nBaseStats[ATTRIB_CHA] = RollStat();
			m_pCharacter->m_nBaseStats[ATTRIB_COM] = RollStat();

			ProcessCharStats();

			if(m_pCharacter->m_nDisplayStats[ATTRIB_STR] == 18 && CharacterIsFighter(m_pCharacter))
			{
				m_pCharacter->m_nBaseStats[ATTRIB_EXSTR] = RollD100();
			}
			else
			{
				m_pCharacter->m_nBaseStats[ATTRIB_EXSTR] = 0;
			}

			for(int i = 0; i < 7; ++i)
			{
				if(nMinStats[i] && m_pCharacter->m_nDisplayStats[i] < nMinStats[i])
				{
					bCharRolled = FALSE;
					break;
				}

				if(nMinStats[i] < 0 && m_pCharacter->m_nDisplayStats[i] > abs(nMinStats[i]))
				{
					bCharRolled = FALSE;
					break;
				}
			}

		} while(bCharRolled == FALSE);

		if(m_szCharacterName.GetLength() == 0 && m_pApp->m_Settings.m_bUseRandomNames)
		{
			m_szCharacterName = m_pApp->GetRandomName(m_pCharacter->m_nRace, m_pCharacter->m_nSex);
			strcpy(m_pCharacter->m_szCharacterName, m_szCharacterName.Left(127));
			m_pCharacter->m_szCharacterName[127] = 0;
		}
		
		RollHitpoints();

		ProcessCharStats();

		Refresh();

		m_pCharacter->MarkChanged();
	}
	else if(m_pCharacter->m_lEarnedXP)
	{
		BOOL bLeveled = GrantCharacterEarnedExperience(m_pCharacter);

		ProcessCharStats();

		Refresh();

		m_pCharacter->MarkChanged();
	}
}




void CDMCharViewDialog::OnKillfocusCharSexCombo() 
{
	// TODO: Add your control notification handler code here
	
}

void CDMCharViewDialog::OnChangeXpEdit1() 
{

	UpdateData(TRUE);

	m_pCharacter->m_lExperience[0] = atol(m_szXPEdit1.GetBuffer(0));

	m_pCharacter->m_nLevel[0] = GetExperienceLevelByClass(m_pCharacter, 0);

	Refresh();

	m_pCharacter->MarkChanged();
	
}

void CDMCharViewDialog::OnChangeXpEdit2() 
{

	UpdateData(TRUE);

	m_pCharacter->m_lExperience[1] = atol(m_szXPEdit2.GetBuffer(0));

	m_pCharacter->m_nLevel[1] = GetExperienceLevelByClass(m_pCharacter, 1);

	Refresh();

	m_pCharacter->MarkChanged();
	
}

void CDMCharViewDialog::OnChangeXpEdit3() 
{

	UpdateData(TRUE);

	m_pCharacter->m_lExperience[2] = atol(m_szXPEdit3.GetBuffer(0));

	m_pCharacter->m_nLevel[2] = GetExperienceLevelByClass(m_pCharacter, 2);

	Refresh();

	m_pCharacter->MarkChanged();
	
}

void CDMCharViewDialog::OnChangeLevelEdit1() 
{
	UpdateData(TRUE);

	m_pCharacter->m_nLevel[0] = atoi(m_szLevelEdit1.GetBuffer(0));

	if(m_szLevelEdit1 != "")
	{
		m_pCharacter->m_lExperience[0] = GetExperiencePointsForLevelByClass(m_pCharacter->m_Class[0], m_pCharacter->m_nLevel[0]);

		ProcessCharStats();

		Refresh();
	}

	m_pCharacter->MarkChanged();
}


void CDMCharViewDialog::OnChangeLevelEdit2() 
{
	UpdateData(TRUE);

	m_pCharacter->m_nLevel[1] = atoi(m_szLevelEdit2.GetBuffer(0));

	if(m_szLevelEdit2 != "")
	{
		m_pCharacter->m_lExperience[1] = GetExperiencePointsForLevelByClass(m_pCharacter->m_Class[1], m_pCharacter->m_nLevel[1]);

		ProcessCharStats();

		Refresh();
	}

	m_pCharacter->MarkChanged();
	
}


void CDMCharViewDialog::OnChangeLevelEdit3() 
{
	UpdateData(TRUE);

	m_pCharacter->m_nLevel[2] = atoi(m_szLevelEdit3.GetBuffer(0));

	if(m_szLevelEdit3 != "")
	{
		m_pCharacter->m_lExperience[2] = GetExperiencePointsForLevelByClass(m_pCharacter->m_Class[2], m_pCharacter->m_nLevel[2]);

		ProcessCharStats();

		Refresh();
	}
	m_pCharacter->MarkChanged();

	
}

void CDMCharViewDialog::OnEnChangeExpEdit()
{
	UpdateData(TRUE);

	m_pCharacter->m_lEarnedXP = atoi(m_szEarnedXP.GetBuffer(0));

	if(m_szEarnedXP != "")
	{
		Refresh();
	}

	m_pCharacter->MarkChanged();
}


void CDMCharViewDialog::OnChangeStrEdit() 
{	
	UpdateData(TRUE);

	if(m_szSTREdit != "")
	{
		m_pCharacter->m_nEditedStats[ATTRIB_STR] = atoi(m_szSTREdit.GetBuffer(0));

		if(m_pCharacter->m_nEditedStats[ATTRIB_STR] == 18 && CharacterIsFighter(m_pCharacter) && m_pCharacter->m_nEditedStats[ATTRIB_EXSTR] == 0)
		{
			m_pCharacter->m_nEditedStats[ATTRIB_EXSTR] = 1;
		}
		else if(m_pCharacter->m_nEditedStats[ATTRIB_STR] != 18 || !CharacterIsFighter(m_pCharacter))
		{
			m_pCharacter->m_nEditedStats[ATTRIB_EXSTR] = 0;
		}

		ProcessCharStats();

		Refresh();
	}

	m_pCharacter->MarkChanged();
}

void CDMCharViewDialog::OnChangeExStrEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);

	m_pCharacter->m_nEditedStats[ATTRIB_EXSTR] = atoi(m_szExceptionalStrength.GetBuffer(0));

	m_pCharacter->MarkChanged();
}


void CDMCharViewDialog::OnKillfocusExStrEdit() 
{
	// TODO: Add your control notification handler code here

	ProcessCharStats();
	Refresh();
	
}


void CDMCharViewDialog::OnChangeIntEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	if(m_szINTEdit != "")
	{
		m_pCharacter->m_nEditedStats[ATTRIB_INT] = atoi(m_szINTEdit.GetBuffer(0));

		ProcessCharStats();

		Refresh();
	}

	m_pCharacter->MarkChanged();
	
}

void CDMCharViewDialog::OnChangeWisEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	if(m_szWISEdit != "")
	{
		m_pCharacter->m_nEditedStats[ATTRIB_WIS] = atoi(m_szWISEdit.GetBuffer(0));

		ProcessCharStats();

		Refresh();
	}

	m_pCharacter->MarkChanged();
	
}

void CDMCharViewDialog::OnChangeDexEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	if(m_szDEXEdit != "")
	{
		m_pCharacter->m_nEditedStats[ATTRIB_DEX] = atoi(m_szDEXEdit.GetBuffer(0));

		ProcessCharStats();

		Refresh();
	}

	m_pCharacter->MarkChanged();
	
}

void CDMCharViewDialog::OnChangeConEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	if(m_szCONEdit != "")
	{
		m_pCharacter->m_nEditedStats[ATTRIB_CON] = atoi(m_szCONEdit.GetBuffer(0));

		ProcessCharStats();

		Refresh();
	}

	m_pCharacter->MarkChanged();
	
}


void CDMCharViewDialog::OnChangeChaEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	if(m_szCHAEdit != "")
	{
		m_pCharacter->m_nEditedStats[ATTRIB_CHA] = atoi(m_szCHAEdit.GetBuffer(0));

		ProcessCharStats();

		Refresh();
	}

	m_pCharacter->MarkChanged();
	
}

void CDMCharViewDialog::OnChangeComEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	if(m_szCOMEdit != "")
	{
		m_pCharacter->m_nEditedStats[ATTRIB_COM] = atoi(m_szCOMEdit.GetBuffer(0));

		ProcessCharStats();

		Refresh();
	}

	m_pCharacter->MarkChanged();
	
}


void CDMCharViewDialog::OnShowHpCheck() 
{
	// TODO: Add your control notification handler code here
	
	UpdateData(TRUE);

	ProcessCharStats();

	Refresh();
}


void CDMCharViewDialog::OnSetfocusHpEdit() 
{
	// TODO: Add your control notification handler code here
	if(m_pHPEditDialog == NULL)
	{
		m_pHPEditDialog = new DMHPEditDialog(m_pCharacter, this);
	}

	GotoDlgCtrl((CWnd *)&m_cCurrentEdit);
	
}

void CDMCharViewDialog::OnSelchangeCharSexCombo() 
{
	UpdateData(TRUE);

	int nCursor = m_cCharSexCombo.GetCurSel();

	m_pCharacter->m_nSex = nCursor;

	m_pCharacter->m_nHeight = 0; //forces a reset/re-roll
	m_pCharacter->m_nWeight = 0; //forces a reset/re-roll

	ProcessCharStats();

	Refresh();

	m_pCharacter->MarkChanged();
}



void CDMCharViewDialog::OnSelchangeCharAlignmentCombo() 
{
	UpdateData(TRUE);

	int nCursor = m_cAlignmentCombo.GetCurSel();

	m_pCharacter->m_nAlignment = (DND_CHARACTER_ALIGNMENTS)m_cAlignmentCombo.GetItemData(nCursor);

	ProcessCharStats();

	Refresh();

	m_pCharacter->MarkChanged();
}

void CDMCharViewDialog::OnSelchangeCharArmorCombo() 
{
	UpdateData(TRUE);

	int nCursor = m_cArmorCombo.GetCurSel();

	DWORD dwArmorID = m_cArmorCombo.GetItemData(nCursor);

	cDNDObject *pInvItem = m_pCharacter->FindObjectInInventory(dwArmorID);

	if(pInvItem != NULL)
	{
		cDNDArmor *pArmor = NULL;

		m_pApp->m_ArmorIndexedTypeMap.Lookup(pInvItem->m_wTypeId, pArmor);

		if(pArmor != NULL)
		{
			pArmor->CopyTo(&m_pCharacter->m_ArmorWorn);
			m_pCharacter->m_ArmorWorn.m_dwObjectID = pInvItem->m_dwObjectID;
			m_pCharacter->m_ArmorWorn.m_nMagicAdj = pInvItem->m_nMagicAdj;

			pInvItem->m_dwContainerID = 0;
		}

		ProcessCharStats();

		Refresh();

		if(m_pInventoryDialog != NULL)
		{
			m_pInventoryDialog->RefreshAll();
		}

		m_pCharacter->MarkChanged();
	}
	
}

void CDMCharViewDialog::OnSelchangeCharHelmCombo() 
{
	UpdateData(TRUE);

	int nCursor = m_cHelmCombo.GetCurSel();

	DWORD dwHelmID = m_cHelmCombo.GetItemData(nCursor);

	cDNDObject *pInvItem = m_pCharacter->FindObjectInInventory(dwHelmID);

	if(pInvItem != NULL)
	{
		cDNDObject *pHelmet = m_pApp->m_HelmetsIndexedTypeArray.GetAt(pInvItem->m_wTypeId);

		if(pHelmet != NULL)
		{
			pHelmet->CopyTo(&m_pCharacter->m_HelmWorn);
			m_pCharacter->m_HelmWorn.m_dwObjectID = pInvItem->m_dwObjectID;
			m_pCharacter->m_HelmWorn.m_nMagicAdj = pInvItem->m_nMagicAdj;
			
			pInvItem->m_dwContainerID = 0;

		}
		else
		{
			cDNDObject _BlankHelmet;
			strcpy(_BlankHelmet.m_szType, "none");
			_BlankHelmet.CopyFull(&m_pCharacter->m_HelmWorn);
		}

		ProcessCharStats();

		Refresh();

		if(m_pInventoryDialog != NULL)
		{
			m_pInventoryDialog->RefreshAll();
		}

		m_pCharacter->MarkChanged();
	}
	
}


void CDMCharViewDialog::OnSelchangeCharShieldCombo() 
{
	UpdateData(TRUE);

	int nCursor = m_cShieldCombo.GetCurSel();

	DWORD dwShieldID = m_cShieldCombo.GetItemData(nCursor);

	cDNDObject *pInvItem = m_pCharacter->FindObjectInInventory(dwShieldID);

	if(pInvItem != NULL)
	{
		cDNDShield *pShield = NULL;

		m_pApp->m_ShieldsIndexedTypeMap.Lookup(pInvItem->m_wTypeId, pShield);

		if(pShield != NULL)
		{
			pShield->CopyTo(&m_pCharacter->m_ShieldWorn);
			m_pCharacter->m_ShieldWorn.m_dwObjectID = pInvItem->m_dwObjectID;
			m_pCharacter->m_ShieldWorn.m_nMagicAdj = pInvItem->m_nMagicAdj;

			pInvItem->m_dwContainerID = 0;
		}

		ProcessCharStats();

		Refresh();

		if(m_pInventoryDialog != NULL)
		{
			m_pInventoryDialog->RefreshAll();
		}

		m_pCharacter->MarkChanged();
	}
	
}


void CDMCharViewDialog::OnChangeArmorPlusEdit() 
{
	UpdateData(TRUE);

	if(m_pCharacter->m_ArmorWorn.m_wTypeId)
	{
		m_pCharacter->m_ArmorWorn.m_nMagicAdj = atoi(m_szArmorPlus.GetBuffer(0));
	}
	else
	{
		m_pCharacter->m_ArmorWorn.m_nMagicAdj = 0;
	}


	cDNDObject *pInvItem = m_pCharacter->FindObjectInInventory(m_pCharacter->m_ArmorWorn.m_dwObjectID);
	if(pInvItem != NULL)
	{
		pInvItem->m_nMagicAdj = m_pCharacter->m_ArmorWorn.m_nMagicAdj;

		m_pCharacter->MarkChanged();
	}

	ProcessCharStats();

	Refresh();

}

void CDMCharViewDialog::OnChangeShieldPlusEdit() 
{

	UpdateData(TRUE);

	if(m_pCharacter->m_ShieldWorn.m_wTypeId)
	{
		m_pCharacter->m_ShieldWorn.m_nMagicAdj = atoi(m_szShieldPlus.GetBuffer(0));
	}
	else
	{
		m_pCharacter->m_ShieldWorn.m_nMagicAdj = 0;
	}


	cDNDObject *pInvItem = m_pCharacter->FindObjectInInventory(m_pCharacter->m_ShieldWorn.m_dwObjectID);
	if(pInvItem != NULL)
	{
		pInvItem->m_nMagicAdj = m_pCharacter->m_ShieldWorn.m_nMagicAdj;

		m_pCharacter->MarkChanged();
	}

	ProcessCharStats();

	Refresh();

	
}

//ring ring
void CDMCharViewDialog::OnSelchangeCharRingLCombo() 
{
	UpdateData(TRUE);

	int nCursor = m_cRingLeftCombo.GetCurSel();

	DWORD dwRingID = m_cRingLeftCombo.GetItemData(nCursor);

	cDNDObject *pInvItem = m_pCharacter->FindObjectInInventory(dwRingID);

	if(pInvItem != NULL)
	{
		cDNDRing *pRing = NULL;

		m_pApp->m_RingsIndexedTypeMap.Lookup(pInvItem->m_wTypeId, pRing);

		if(pRing != NULL)
		{
			pRing->CopyTo(&m_pCharacter->m_RingsWorn[0]);
			m_pCharacter->m_RingsWorn[0].m_dwObjectID = pInvItem->m_dwObjectID;
			m_pCharacter->m_RingsWorn[0].m_nMagicAdj = pInvItem->m_nMagicAdj;

			if(m_pCharacter->m_RingsWorn[0].m_dwObjectID == m_pCharacter->m_RingsWorn[1].m_dwObjectID) //can't wear the same ring on two hands dork
			{
				cDNDRing _BlankRing;
				 _BlankRing.CopyTo(&m_pCharacter->m_RingsWorn[1]);
				 m_pCharacter->m_RingsWorn[1].m_dwObjectID = 0;
			}
			else
			{
				pInvItem->m_dwContainerID = 0;
			}

			m_pCharacter->MarkChanged();
		}

		ProcessCharStats();

		Refresh();

		if(m_pInventoryDialog != NULL)
		{
			m_pInventoryDialog->RefreshAll();
		}
	}
	
}

void CDMCharViewDialog::OnSelchangeCharRingRCombo() 
{
	UpdateData(TRUE);

	int nCursor = m_cRingRightCombo.GetCurSel();

	DWORD dwRingID = m_cRingRightCombo.GetItemData(nCursor);

	cDNDObject *pInvItem = m_pCharacter->FindObjectInInventory(dwRingID);

	if(pInvItem != NULL)
	{
		cDNDRing *pRing = NULL;

		m_pApp->m_RingsIndexedTypeMap.Lookup(pInvItem->m_wTypeId, pRing);

		if(pRing != NULL)
		{
			pRing->CopyTo(&m_pCharacter->m_RingsWorn[1]);
			m_pCharacter->m_RingsWorn[1].m_dwObjectID = pInvItem->m_dwObjectID;
			m_pCharacter->m_RingsWorn[1].m_nMagicAdj = pInvItem->m_nMagicAdj;

			if(m_pCharacter->m_RingsWorn[0].m_dwObjectID == m_pCharacter->m_RingsWorn[1].m_dwObjectID) //can't wear the same ring on two hands dork
			{
				cDNDRing _BlankRing;
				 _BlankRing.CopyTo(&m_pCharacter->m_RingsWorn[0]);
				 m_pCharacter->m_RingsWorn[0].m_dwObjectID = 0;
			}
			else
			{
				pInvItem->m_dwContainerID = 0;
			}

			m_pCharacter->MarkChanged();
		}

		ProcessCharStats();

		Refresh();

		if(m_pInventoryDialog != NULL)
		{
			m_pInventoryDialog->RefreshAll();
		}
	}
	
}


void CDMCharViewDialog::OnChangeRingLPlusEdit() 
{
	UpdateData(TRUE);

	if(m_pCharacter->m_RingsWorn[0].m_wTypeId)
	{
		m_pCharacter->m_RingsWorn[0].m_nMagicAdj = atoi(m_szRingLeftPlusEdit.GetBuffer(0));
	}
	else
	{
		m_pCharacter->m_RingsWorn[0].m_nMagicAdj = 0;
	}


	cDNDObject *pInvItem = m_pCharacter->FindObjectInInventory(m_pCharacter->m_RingsWorn[0].m_dwObjectID);
	if(pInvItem != NULL)
	{
		pInvItem->m_nMagicAdj = m_pCharacter->m_RingsWorn[0].m_nMagicAdj;

		m_pCharacter->MarkChanged();
	}

	ProcessCharStats();

	Refresh();
	
}

void CDMCharViewDialog::OnChangeRingRPlusEdit() 
{
	UpdateData(TRUE);

	if(m_pCharacter->m_RingsWorn[1].m_wTypeId)
	{
		m_pCharacter->m_RingsWorn[1].m_nMagicAdj = atoi(m_szRingRightPlusEdit.GetBuffer(0));
	}
	else
	{
		m_pCharacter->m_RingsWorn[1].m_nMagicAdj = 0;
	}


	cDNDObject *pInvItem = m_pCharacter->FindObjectInInventory(m_pCharacter->m_RingsWorn[1].m_dwObjectID);
	if(pInvItem != NULL)
	{
		pInvItem->m_nMagicAdj = m_pCharacter->m_RingsWorn[1].m_nMagicAdj;

		m_pCharacter->MarkChanged();
	}

	ProcessCharStats();

	Refresh();
	
}


void CDMCharViewDialog::OnCharSheet() 
{
	if(m_pCharSheetDialog == NULL)
	{
		if(m_pSpellsDialog == NULL)
		{
			m_pSpellsDialog = new DMCharSpellsDialog(m_pCharacter, this, m_pParent);
			m_bFirstSpellViewRefresh = TRUE;
		}
		m_pSpellsDialog->Refresh();

		m_pCharSheetDialog = new DMCharSheetDialog(m_pCharacter, this);
	}
}

void CDMCharViewDialog::OnClose() 
{
	
	if(m_pApp->m_pBaseCopyCharacterDialog == this)
	{
		m_pApp->m_pBaseCopyCharacterDialog = NULL;
		m_pApp->m_bCharacterCopyBufferIsNPC = FALSE;
	}

	if(m_pCharSheetDialog != NULL)
	{
		m_pCharSheetDialog->m_pCharViewDialog = NULL;
		m_pCharSheetDialog->PostMessage(WM_CLOSE);
		m_pCharSheetDialog = NULL;
	}

	if(m_pInventoryDialog != NULL)
	{
		m_pInventoryDialog->PostMessage(WM_CLOSE);
		m_pInventoryDialog = NULL;
	}

	if(m_pSpellsDialog != NULL)
	{
		m_pSpellsDialog->m_pSiblingWindow = NULL;
		m_pSpellsDialog->PostMessage(WM_CLOSE);
		m_pSpellsDialog = NULL;
	}

	if(m_pHPEditDialog != NULL)
	{
		m_pHPEditDialog->PostMessage(WM_CLOSE);
		m_pHPEditDialog = NULL;
	}

	if(m_pCharacter != NULL)
	{
		m_pApp->m_CharacterViewMap.RemoveKey((WORD)m_pCharacter->m_dwCharacterID);

		delete m_pCharacter;
		m_pCharacter = NULL;
	}
	
	//Warning: calling DestroyWindow in CDialog::~CDialog --
	//OnDestroy or PostNcDestroy in derived class will not be called.

	CDialog::OnClose();
	delete this;

	//CDialog::DestroyWindow();
}

void CDMCharViewDialog::PostNcDestroy() 
{
	/*

	if(m_pCharSheetDialog != NULL)
	{
		m_pCharSheetDialog->PostMessage(WM_CLOSE);
	}

	if(m_pInventoryDialog != NULL)
	{
		m_pInventoryDialog->PostMessage(WM_CLOSE);
	}

	if(m_pSpellsDialog != NULL)
	{
		m_pSpellsDialog->PostMessage(WM_CLOSE);
	}

	if(m_pHPEditDialog != NULL)
	{
		m_pHPEditDialog->PostMessage(WM_CLOSE);
	}
	*/

	if(m_pCharacter != NULL)
	{
		m_pApp->m_CharacterViewMap.RemoveKey((WORD)m_pCharacter->m_dwCharacterID);

		delete m_pCharacter;
		m_pCharacter = NULL;
	}
	
	CDialog::PostNcDestroy();

	//delete this;
}


cDNDCharacter *CDMCharViewDialog::LoadCharacter()
{
	m_bLoadedFromMenu = TRUE;

	OnLoad();

	/*
	if(m_pCharacter != NULL && m_pApp->m_bInitialResize == FALSE)
	{
		m_pApp->m_bInitialResize = TRUE;
		PrePositionWindow();
	}
	*/
	if(m_pCharacter != NULL && m_pApp->m_pMainWindow->m_bInitialMaximize == FALSE)
	{
		m_pApp->m_pMainWindow->PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);
		m_pApp->m_pMainWindow->m_bInitialMaximize = TRUE;
		InvalidateRect(NULL);
	}

	return m_pCharacter;
}

void CDMCharViewDialog::PrePositionWindow()
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

void CDMCharViewDialog::OnLoad() 
{
	DMLoadFileDescriptor FileDesc;
	FileDesc.m_szLabel = "Load DM Character File:";
	FileDesc.m_szInitialPath = m_pApp->m_szEXEPath;
	FileDesc.m_szSubDirectory = "data/characters";
	FileDesc.m_szFileTypes = ".dmc";
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
		if(LoadCharacterFromFile(FileDesc.m_szReturnedPath.GetBuffer(0)) == FALSE)
		{
			delete m_pCharacter;
			m_pCharacter = NULL;
		}
	}
	else if(m_bLoadedFromMenu)
	{
		delete m_pCharacter;
		m_pCharacter = NULL;
	}

}

BOOL CDMCharViewDialog::LoadCharacterFromFile(char *szFileName)
{
	FILE *pInFile = fopen(szFileName, "rb");

	if(pInFile != NULL)
	{
		fread(m_pCharacter, sizeof(cDNDCharacter), 1, pInFile);

		fclose(pInFile);

		if(m_pCharacter->m_Version == 1000)
		{
			for(int i = 0; i < 4; ++ i)
			{
				for(int j = 0; j < MAX_SPELL_LEVELS; ++ j)
				{
					for(int k = 0; k < MAX_SPELLS_PER_LEVEL; ++ k)
					{
						m_pCharacter->m_nSpellBooks[i][j][k] = 0;
						m_pCharacter->m_nSpellsMemorized[i][j][k] = 0;
						m_pCharacter->m_nLastSpellsMemorized[i][j][k] = 0;
					}
				}
			}
		}

		if(m_pCharacter->m_Version < 10009)
		{
			memset(m_pCharacter->m_nLanguages, 0, MAX_CHARACTER_LANGUAGES * sizeof(int));
		}

		if (strcmp(m_pCharacter->m_szCharacterName, "Elric") == 0)
		{
			TRACE("!");
		}

		m_pCharacter->m_szSecondarySkill[63] = 0;

		m_szSaveFileName = szFileName;
		m_szLoadFileName = szFileName;
	}

	if(m_pCharacter != NULL)
	{
		if(m_pCharacter->m_dwCharacterID == 0L)
		{
			m_pCharacter->m_dwCharacterID = GetUniqueID();
		}

		CDMCharViewDialog *pCheckCharView = NULL;
		m_pApp->m_CharacterViewMap.Lookup((WORD)m_pCharacter->m_dwCharacterID, pCheckCharView);
		if(pCheckCharView != NULL)
		{
			AfxMessageBox("This character is already loaded !", MB_OK);

			return FALSE;
		}

		m_pCharacter->MarkSaved();

		m_pApp->m_CharacterViewMap.SetAt((WORD)m_pCharacter->m_dwCharacterID, this);

		UpdateData(FALSE);

		ProcessCharStats();

		Refresh();

		return TRUE;
	}

	return FALSE;
}


void CDMCharViewDialog::OnSave() 
{
	m_szSaveFileName = _T("");

	SaveExternal();
}

void CDMCharViewDialog::SaveExternal() 
{
	if(m_szSaveFileName == _T(""))
	{
		DMLoadFileDescriptor FileDesc;
		FileDesc.m_szLabel = "Save DM Character File:";
		FileDesc.m_szInitialPath = m_pApp->m_szEXEPath;
		FileDesc.m_szSubDirectory = "data/characters";

		/*
		if(m_szSaveFileName != _T(""))
		{
			FileDesc.m_szInitialFileName = m_szSaveFileName;
		}
		else
		*/
		{
			FileDesc.m_szInitialFileName = m_pCharacter->m_szCharacterName;
		}

		FileDesc.m_szFileTypes = ".dmc";
		FileDesc.m_bSave = TRUE;

#if NEW_FILE_MANAGEMENT_WINDOW
		FileDesc.m_szOriginalFileName = m_szLoadFileName;
		CDMFileManagementDialog *pDlg = new CDMFileManagementDialog(&FileDesc);
#else
		DMLoadFileDialog *pDlg = new DMLoadFileDialog(&FileDesc);
#endif

		pDlg->DoModal();
		delete pDlg;

		if(FileDesc.m_bSuccess)
		{
			SaveCharacterToFile(FileDesc.m_szReturnedPath.GetBuffer(0));
		}

	}
	else
	{
		SaveCharacterToFile(m_szSaveFileName.GetBuffer(0));
	}
}


void CDMCharViewDialog::SaveCharacterToFile(char *szFileName)
{

	FILE *pOutFile = fopen(szFileName, "wb");

	if(pOutFile != NULL)
	{
		if(m_pCharacter->m_dwCharacterID == 0L)
		{
			m_pCharacter->m_dwCharacterID = GetUniqueID();
		}

		m_pCharacter->MarkSaved();

		m_pCharacter->m_bPocketPC = FALSE;

		m_pCharacter->m_Version = DMH_CURRENT_VERSION;

		if(m_pCharacter->m_szPortraitPath[0])
		{
			CString szTemp;
			szTemp.Format("%s",m_pCharacter->m_szPortraitPath);
			szTemp.MakeUpper();
			strcpy(m_pCharacter->m_szPortraitPath, szTemp.GetBuffer(0));
		}

		if(m_pCharacter->m_dwCharacterID == 0L)
		{
			m_pCharacter->m_dwCharacterID = GetUniqueID();
		}

		m_pCharacter->MarkSaved();

		//fwrite(BadData, 264, 1, pOutFile); // for file fix up
		fwrite(m_pCharacter, sizeof(cDNDCharacter), 1, pOutFile);

		fclose(pOutFile);

		m_szSaveFileName = szFileName;

		m_pApp->m_CharacterViewMap.SetAt((WORD)m_pCharacter->m_dwCharacterID, this);
	}
}

void CDMCharViewDialog::OnSetfocusAcEdit() 
{

}


void CDMCharViewDialog::OnAddXpButton1() 
{
	ModifyValue((int *)&m_pCharacter->m_lExperience[0], "Modify XP", 0);

	m_pCharacter->m_nLevel[0] = GetExperienceLevelByClass(m_pCharacter, 0);

	ProcessCharStats();

	Refresh();

	m_pCharacter->MarkChanged();
	
}

void CDMCharViewDialog::OnAddXpButton2() 
{

	ModifyValue((int *)&m_pCharacter->m_lExperience[1], "Modify XP", 0);

	m_pCharacter->m_nLevel[1] = GetExperienceLevelByClass(m_pCharacter, 1);

	ProcessCharStats();

	Refresh();

	m_pCharacter->MarkChanged();
}

void CDMCharViewDialog::OnAddXpButton3() 
{

	ModifyValue((int *)&m_pCharacter->m_lExperience[2], "Modify XP", 0);

	m_pCharacter->m_nLevel[2] = GetExperienceLevelByClass(m_pCharacter, 2);

	ProcessCharStats();

	Refresh();

	m_pCharacter->MarkChanged();
}

void CDMCharViewDialog::OnBnClickedAddExpButton() // this is for earned but not granted XP
{
	ModifyValue((int *)&m_pCharacter->m_lEarnedXP, "Modify Earned XP", 0);

	if(m_pCharacter->m_lEarnedXP < 0L)
	{
		m_pCharacter->m_lEarnedXP = 0L;
	}

	ProcessCharStats();

	Refresh();

	m_pCharacter->MarkChanged();
}


void CDMCharViewDialog::OnSelchangeWeaponCombo1() 
{
	UpdateData(TRUE);

	int nCursor = m_cWeaponCombo_1.GetCurSel();

	DWORD dwWeaponID = m_cWeaponCombo_1.GetItemData(nCursor);

	cDNDObject *pInvItem = m_pCharacter->FindObjectInInventory(dwWeaponID);

	if(pInvItem != NULL)
	{
		cDNDWeapon *pWeapon = NULL;

		pWeapon = m_pApp->m_WeaponsIndexedTypeArray[pInvItem->m_wTypeId];

		if(pWeapon != NULL)
		{
			pWeapon->CopyTo(&m_pCharacter->m_SelectedWeapons[0]);
			m_pCharacter->m_SelectedWeapons[0].m_dwObjectID = pInvItem->m_dwObjectID;
			m_pCharacter->m_SelectedWeapons[0].m_nMagicAdj = pInvItem->m_nMagicAdj;
			m_pCharacter->m_SelectedWeapons[0].m_bMysteryItem = pInvItem->m_bMysteryItem;
		}

		//PASS AMMO !
		if(IsThrowableWeapon(pWeapon->m_nFlags))
		{
			pWeapon->CopyTo(&m_pCharacter->m_SelectedAmmo);
			m_pCharacter->m_SelectedAmmo.m_dwObjectID = pInvItem->m_dwObjectID;
			m_pCharacter->m_SelectedAmmo.m_nMagicAdj = pInvItem->m_nMagicAdj;
			m_pCharacter->m_SelectedAmmo.m_lAmount = pInvItem->m_lAmount;
			m_cAmmunitionCombo.SetCurSel(1); //dont't know why this works, but it does
		}
		else if(IsMissileWeapon(pWeapon))
		{
			for(int i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
			{
				if(m_pCharacter->m_Inventory[i].m_ObjectType == DND_OBJECT_TYPE_AMMO && IsValidAmmoForWeapon(&m_pCharacter->m_SelectedWeapons[0] ,&m_pCharacter->m_Inventory[i]))
				{
					pWeapon = m_pApp->m_AmmunitionIndexedTypeArray.GetAt(m_pCharacter->m_Inventory[i].m_wTypeId);

					if(pWeapon != NULL)
					{
						pWeapon->CopyTo(&m_pCharacter->m_SelectedAmmo);
						m_pCharacter->m_SelectedAmmo.m_dwObjectID = m_pCharacter->m_Inventory[i].m_dwObjectID;
						m_pCharacter->m_SelectedAmmo.m_nMagicAdj = m_pCharacter->m_Inventory[i].m_nMagicAdj;
						m_pCharacter->m_SelectedAmmo.m_lAmount = m_pCharacter->m_Inventory[i].m_lAmount;
						m_cAmmunitionCombo.SetCurSel(1); //dont't know why this works, but it does
					}
				}
			}

			//m_cAmmunitionCombo.SetCurSel(1); //dont't know why this works, but it does

		}

		m_pCharacter->MarkChanged();

		ProcessCharStats();

		Refresh();
	}
	
}

void CDMCharViewDialog::OnSelchangeWeaponCombo2() 
{

	int nCursor = m_cWeaponCombo_2.GetCurSel();

	DWORD dwWeaponID = m_cWeaponCombo_2.GetItemData(nCursor);

	cDNDObject *pInvItem = m_pCharacter->FindObjectInInventory(dwWeaponID);

	if(pInvItem != NULL)
	{
		cDNDWeapon *pWeapon = NULL;

		pWeapon = m_pApp->m_WeaponsIndexedTypeArray[pInvItem->m_wTypeId];

		if(pWeapon != NULL)
		{
			pWeapon->CopyTo(&m_pCharacter->m_SelectedWeapons[1]);
			m_pCharacter->m_SelectedWeapons[1].m_dwObjectID = pInvItem->m_dwObjectID;
			m_pCharacter->m_SelectedWeapons[1].m_nMagicAdj = pInvItem->m_nMagicAdj;
			m_pCharacter->m_SelectedWeapons[1].m_bMysteryItem = pInvItem->m_bMysteryItem;
		}

		m_pCharacter->MarkChanged();

		ProcessCharStats();

		Refresh();
	}
	
}

void CDMCharViewDialog::OnSelchangeWeaponCombo3() 
{

	int nCursor = m_cWeaponCombo_3.GetCurSel();

	DWORD dwWeaponID = m_cWeaponCombo_3.GetItemData(nCursor);

	cDNDObject *pInvItem = m_pCharacter->FindObjectInInventory(dwWeaponID);

	if(pInvItem != NULL)
	{
		cDNDWeapon *pWeapon = NULL;

		pWeapon = m_pApp->m_WeaponsIndexedTypeArray[pInvItem->m_wTypeId];

		if(pWeapon != NULL)
		{
			pWeapon->CopyTo(&m_pCharacter->m_SelectedWeapons[2]);
			m_pCharacter->m_SelectedWeapons[2].m_dwObjectID = pInvItem->m_dwObjectID;
			m_pCharacter->m_SelectedWeapons[2].m_nMagicAdj = pInvItem->m_nMagicAdj;
			m_pCharacter->m_SelectedWeapons[2].m_bMysteryItem = pInvItem->m_bMysteryItem;
		}

		m_pCharacter->MarkChanged();

		ProcessCharStats();

		Refresh();
	}
	
}

void CDMCharViewDialog::OnSelchangeWeaponCombo4() 
{

	int nCursor = m_cWeaponCombo_4.GetCurSel();

	DWORD dwWeaponID = m_cWeaponCombo_4.GetItemData(nCursor);

	cDNDObject *pInvItem = m_pCharacter->FindObjectInInventory(dwWeaponID);

	if(pInvItem != NULL)
	{
		cDNDWeapon *pWeapon = NULL;

		pWeapon = m_pApp->m_WeaponsIndexedTypeArray[pInvItem->m_wTypeId];

		if(pWeapon != NULL)
		{
			pWeapon->CopyTo(&m_pCharacter->m_SelectedWeapons[3]);
			m_pCharacter->m_SelectedWeapons[3].m_dwObjectID = pInvItem->m_dwObjectID;
			m_pCharacter->m_SelectedWeapons[3].m_nMagicAdj = pInvItem->m_nMagicAdj;
			m_pCharacter->m_SelectedWeapons[3].m_bMysteryItem = pInvItem->m_bMysteryItem;
		}

		m_pCharacter->MarkChanged();

		ProcessCharStats();

		Refresh();
	}
		
}

void CDMCharViewDialog::OnSelchangeAmmoCombo() 
{
	int nCursor = m_cAmmunitionCombo.GetCurSel();

	DWORD dwWeaponID = m_cAmmunitionCombo.GetItemData(nCursor);

	cDNDObject *pInvItem = m_pCharacter->FindObjectInInventory(dwWeaponID);

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
			pWeapon->CopyTo(&m_pCharacter->m_SelectedAmmo);
			m_pCharacter->m_SelectedAmmo.m_dwObjectID = pInvItem->m_dwObjectID;
			m_pCharacter->m_SelectedAmmo.m_nMagicAdj = pInvItem->m_nMagicAdj;
			m_pCharacter->m_SelectedAmmo.m_bMysteryItem = pInvItem->m_bMysteryItem;
			
			if(pInvItem->m_lAmount == 0)
				pInvItem->m_lAmount = 1;

			m_pCharacter->m_SelectedAmmo.m_lAmount = pInvItem->m_lAmount;
		}

		m_pCharacter->MarkChanged();

		ProcessCharStats();

		Refresh();
	}
}


void CDMCharViewDialog::OnChangeWeaponPlusEdit1() 
{
	
	UpdateData(TRUE);

	if(m_pCharacter->m_SelectedWeapons[0].m_wTypeId)
	{
		m_pCharacter->m_SelectedWeapons[0].m_nMagicAdj = atoi(m_szWeaponPlus_1.GetBuffer(0));
	}
	else
	{
		m_pCharacter->m_SelectedWeapons[0].m_nMagicAdj = 0;
	}


	cDNDObject *pInvItem = m_pCharacter->FindObjectInInventory(m_pCharacter->m_SelectedWeapons[0].m_dwObjectID);
	if(pInvItem != NULL)
	{
		pInvItem->m_nMagicAdj = m_pCharacter->m_SelectedWeapons[0].m_nMagicAdj;

		m_pCharacter->MarkChanged();
	}

	ProcessCharStats();

	Refresh();
	
}

void CDMCharViewDialog::OnChangeWeaponPlusEdit2() 
{
	UpdateData(TRUE);

	if(m_pCharacter->m_SelectedWeapons[1].m_wTypeId)
	{
		m_pCharacter->m_SelectedWeapons[1].m_nMagicAdj = atoi(m_szWeaponPlus_2.GetBuffer(0));
	}
	else
	{
		m_pCharacter->m_SelectedWeapons[1].m_nMagicAdj = 0;
	}


	cDNDObject *pInvItem = m_pCharacter->FindObjectInInventory(m_pCharacter->m_SelectedWeapons[1].m_dwObjectID);
	if(pInvItem != NULL)
	{
		pInvItem->m_nMagicAdj = m_pCharacter->m_SelectedWeapons[1].m_nMagicAdj;

		m_pCharacter->MarkChanged();
	}

	ProcessCharStats();

	Refresh();
	
	
}

void CDMCharViewDialog::OnChangeWeaponPlusEdit3() 
{

	UpdateData(TRUE);

	if(m_pCharacter->m_SelectedWeapons[2].m_wTypeId)
	{
		m_pCharacter->m_SelectedWeapons[2].m_nMagicAdj = atoi(m_szWeaponPlus_3.GetBuffer(0));
	}
	else
	{
		m_pCharacter->m_SelectedWeapons[2].m_nMagicAdj = 0;
	}


	cDNDObject *pInvItem = m_pCharacter->FindObjectInInventory(m_pCharacter->m_SelectedWeapons[2].m_dwObjectID);
	if(pInvItem != NULL)
	{
		pInvItem->m_nMagicAdj = m_pCharacter->m_SelectedWeapons[2].m_nMagicAdj;

		m_pCharacter->MarkChanged();
	}

	ProcessCharStats();

	Refresh();
	
	
}

void CDMCharViewDialog::OnChangeWeaponPlusEdit4() 
{

	UpdateData(TRUE);

	if(m_pCharacter->m_SelectedWeapons[3].m_wTypeId)
	{
		m_pCharacter->m_SelectedWeapons[3].m_nMagicAdj = atoi(m_szWeaponPlus_4.GetBuffer(0));
	}
	else
	{
		m_pCharacter->m_SelectedWeapons[3].m_nMagicAdj = 0;
	}


	cDNDObject *pInvItem = m_pCharacter->FindObjectInInventory(m_pCharacter->m_SelectedWeapons[3].m_dwObjectID);
	if(pInvItem != NULL)
	{
		pInvItem->m_nMagicAdj = m_pCharacter->m_SelectedWeapons[3].m_nMagicAdj;

		m_pCharacter->MarkChanged();
	}

	ProcessCharStats();

	Refresh();
	
	
}


void CDMCharViewDialog::OnChangeAmmoPlusEdit() 
{
	
	UpdateData(TRUE);

	if(m_pCharacter->m_SelectedAmmo.m_wTypeId)
	{
		m_pCharacter->m_SelectedAmmo.m_nMagicAdj = atoi(m_szAmmoPlus.GetBuffer(0));
	}
	else
	{
		m_pCharacter->m_SelectedAmmo.m_nMagicAdj = 0;
	}


	cDNDObject *pInvItem = m_pCharacter->FindObjectInInventory(m_pCharacter->m_SelectedAmmo.m_dwObjectID);
	if(pInvItem != NULL)
	{
		pInvItem->m_nMagicAdj = m_pCharacter->m_SelectedAmmo.m_nMagicAdj;

		m_pCharacter->MarkChanged();
	}

	ProcessCharStats();

	Refresh();
}

void CDMCharViewDialog::SwapWeapon()
{
	OnSwapButton1();
}

void CDMCharViewDialog::OnSwapButton1() 
{
	cDNDWeapon swapWeapon;

	m_pCharacter->m_SelectedWeapons[0].CopyFull(&swapWeapon);

	m_pCharacter->m_SelectedWeapons[1].CopyFull(&m_pCharacter->m_SelectedWeapons[0]);

	swapWeapon.CopyFull(&m_pCharacter->m_SelectedWeapons[1]);

	ProcessCharStats();

	Refresh();	

	OnSelchangeWeaponCombo1();

	m_pCharacter->MarkChanged();
	
	for (int i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
	{
		if (m_pCharacter->m_Inventory[i].m_dwObjectID == m_pCharacter->m_SelectedWeapons[0].m_dwObjectID &&  m_pCharacter->m_SelectedWeapons[0].m_dwObjectID != 0)
		{
			m_pApp->PlayEquipItemSFX(m_pCharacter->m_Inventory[i].m_szType, m_pCharacter->m_SelectedWeapons[0].m_szType);
			break;
		}
	}

}

void CDMCharViewDialog::OnSwapButton2() 
{
	
	cDNDWeapon swapWeapon;

	m_pCharacter->m_SelectedWeapons[1].CopyFull(&swapWeapon);

	m_pCharacter->m_SelectedWeapons[2].CopyFull(&m_pCharacter->m_SelectedWeapons[1]);

	swapWeapon.CopyFull(&m_pCharacter->m_SelectedWeapons[2]);

	ProcessCharStats();

	Refresh();	

	m_pCharacter->MarkChanged();
}

void CDMCharViewDialog::OnSwapButton3() 
{
	cDNDWeapon swapWeapon;

	m_pCharacter->m_SelectedWeapons[2].CopyFull(&swapWeapon);

	m_pCharacter->m_SelectedWeapons[3].CopyFull(&m_pCharacter->m_SelectedWeapons[2]);

	swapWeapon.CopyFull(&m_pCharacter->m_SelectedWeapons[3]);

	ProcessCharStats();

	Refresh();	

	m_pCharacter->MarkChanged();
}

void CDMCharViewDialog::OnCharInventory() 
{
	if(m_pInventoryDialog == NULL)
	{
		m_pInventoryDialog = new DMInventoryDialog(m_pMainDialog, m_pCharacter, this, NULL, m_pParent);
	}

	if(m_pInventoryDialog != NULL && m_pInventoryDialog->m_pPCDetailsDialog != NULL)
	{
		m_pInventoryDialog->m_pPCDetailsDialog->Init();
	}

	m_nPage = 1;

	InvalidateRect(NULL);

}


void CDMCharViewDialog::OnSpellsButton() 
{
	if(m_pSpellsDialog == NULL)
	{
		m_pSpellsDialog = new DMCharSpellsDialog(m_pCharacter, this, m_pParent);
	}
	
	m_nPage = 2;

	InvalidateRect(NULL);
}


void CDMCharViewDialog::OnAddHpButton() 
{
	int nValue = 0;

	ModifyValue((int *)&nValue, "Modify Hit Points:", 0);

	m_pCharacter->m_nCurrentDamage += -nValue;

	if(m_pCharacter->m_nCurrentDamage < 0)
		m_pCharacter->m_nCurrentDamage = 0;

	ProcessCharStats();

	Refresh();

	m_pCharacter->MarkChanged();
	
}

void CDMCharViewDialog::FireAmmo(BOOL bCheckThrow)
{
	for(int i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
	{
		BOOL bThrow = FALSE;
		if(m_pCharacter->m_Inventory[i].m_wTypeId == m_pCharacter->m_SelectedAmmo.m_wTypeId && m_pCharacter->m_Inventory[i].m_lAmount == m_pCharacter->m_SelectedAmmo.m_lAmount && m_pCharacter->m_Inventory[i].m_nMagicAdj == m_pCharacter->m_SelectedAmmo.m_nMagicAdj)
		{
			// check flags
			if (bCheckThrow && IsThrowableWeapon(m_pCharacter->m_Inventory[i].m_nFlags))
			{
				cDMStrikeOrThrowDialog *pDlg = new cDMStrikeOrThrowDialog(&bThrow);
				pDlg->DoModal();

				if (bThrow == FALSE)
				{
					return;
				}
			}

			m_pCharacter->m_Inventory[i].m_lAmount -= 1;

			AddToRecoveryInventory(i, 1);

			if(m_pCharacter->m_Inventory[i].m_lAmount <= 0)
			{
				//gob ammo
				if (IsThrowableWeapon(m_pCharacter->m_Inventory[i].m_nFlags))
				{
					m_dwLastThrownObjectID = m_pCharacter->m_Inventory[i].m_dwObjectID;
				}

				cDNDObject _BlankObj;
				_BlankObj.CopyFull(&m_pCharacter->m_Inventory[i]);
				
				m_pCharacter->ValidateInventory();
			}

			OnSelchangeWeaponCombo1();

			Refresh();

			m_pCharacter->MarkChanged();

			break;
		}
	}

	if(m_cRecoverAmmoCombo.GetCurSel() == -1)
	{
		m_cRecoverAmmoCombo.SetCurSel(0);
	}

}

void CDMCharViewDialog::AddToRecoveryInventory(int nCharInventorySlot, int nAmount)
{
	for(int i = 0; i< MAX_CHARACTER_INVENTORY; ++i)
	{
		if(m_RecoverAmmoInventory[i].m_wTypeId == 0)
		{
			m_pCharacter->m_Inventory[nCharInventorySlot].CopyTo(&m_RecoverAmmoInventory[i]);

			m_RecoverAmmoInventory[i].m_nMagicAdj = m_pCharacter->m_Inventory[nCharInventorySlot].m_nMagicAdj;
			m_RecoverAmmoInventory[i].m_lAmount = nAmount;

			if (m_pCharacter->m_Inventory[nCharInventorySlot].m_lAmount == 0)
			{
				m_RecoverAmmoInventory[i].m_dwObjectID = m_pCharacter->m_Inventory[nCharInventorySlot].m_dwObjectID;
			}
			
			ValidateRecoveryInventory();

			m_pCharacter->MarkChanged();

			return;
		}
	}
}

void CDMCharViewDialog::ValidateRecoveryInventory()
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

void CDMCharViewDialog::CollapseRecoveryInventory()
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

				DWORD dwObjectID = 0;
				if (m_RecoverAmmoInventory[i].m_dwObjectID)
					dwObjectID = m_RecoverAmmoInventory[i].m_dwObjectID;
				else if (m_RecoverAmmoInventory[j].m_dwObjectID)
					dwObjectID = m_RecoverAmmoInventory[j].m_dwObjectID;

				if (dwObjectID)
				{
					m_RecoverAmmoInventory[i].m_dwObjectID = dwObjectID;
				}

				_BlankObj.CopyFull(&m_RecoverAmmoInventory[j]);
			}
		}
	}

}

void CDMCharViewDialog::ExternalRecoverAmmo()
{
	m_cRecoverAmmoCombo.SetCurSel(0);
	UpdateData(TRUE);
	OnRecoverAmmo();
}

void CDMCharViewDialog::OnRecoverAmmo() 
{
	cDNDObject _BlankObj;
	CString szTemp;

	int nCursor = m_cRecoverAmmoCombo.GetCurSel();

	if(nCursor > -1)
	{
		cDNDObject *pObject = (cDNDObject *)m_cRecoverAmmoCombo.GetItemData(nCursor);

		int nRecovered = 0;
		szTemp.Format("Recover how many ? (%d max)", pObject->m_lAmount);
		ModifyValue((int *)&nRecovered, szTemp.GetBuffer(0), pObject->m_lAmount, FALSE);

		if (nRecovered > pObject->m_lAmount)
		{
			nRecovered = pObject->m_lAmount;
		}

		BOOL bRecovered = FALSE;
		BOOL bFoundIt = FALSE;
		for(int i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
		{
			if(m_pCharacter->m_Inventory[i].m_wTypeId == pObject->m_wTypeId && m_pCharacter->m_Inventory[i].m_nMagicAdj == pObject->m_nMagicAdj)
			{
				bFoundIt = TRUE;
				m_pCharacter->m_Inventory[i].m_lAmount += nRecovered;
				bRecovered = TRUE;
				break;
			}
		}

		if(bFoundIt == FALSE)
		{
			int nSlot = MAX_CHARACTER_INVENTORY-1;

			if(m_pCharacter->m_Inventory[nSlot].m_wTypeId == 0)
			{
				pObject->CopyFull(&m_pCharacter->m_Inventory[nSlot]);

				m_pCharacter->m_Inventory[nSlot].m_lAmount = nRecovered;

				if (m_pCharacter->m_Inventory[nSlot].m_dwObjectID == 0)
				{
					m_pCharacter->m_Inventory[nSlot].m_dwObjectID = GetUniqueID();
				}

				bRecovered = TRUE;
			}
		}

		if(bRecovered)
		{
			_BlankObj.CopyFull(pObject);

			ValidateRecoveryInventory();

			m_pCharacter->ValidateInventory();

			//gob ammo
			if (m_dwLastThrownObjectID)
			{
				Refresh();

				for (i = 0; i < m_cWeaponCombo_1.GetCount(); ++i)
				{
					DWORD dwWeaponId = m_cWeaponCombo_1.GetItemData(i);

					if (dwWeaponId == m_dwLastThrownObjectID)
					{
						m_cWeaponCombo_1.SetCurSel(i);
						UpdateData(TRUE);
						break;
					}
				}
			}

			OnSelchangeWeaponCombo1();

			Refresh();

			m_pCharacter->MarkChanged();
		}

		m_dwLastThrownObjectID = 0;

	}
	
}

HBRUSH CDMCharViewDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here

	/*
	switch(nCtlColor)
	{
		case IDC_SPELL_CHART_LIST:
		{
			TRACE("WOOT !\n");
			break;
		}
		default:
		{
			TRACE("WHOIS %d %d %x %x?\n", nCtlColor, IDC_SPELL_CHART_LIST, pWnd->m_hWnd, m_cSpellChartList.m_hWnd);
		}
	}
	*/
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CDMCharViewDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	
	switch(nIDCtl)
	{
		case IDC_SPELL_CHART_LIST:
		{
			//TRACE("WOOT !\n");
			break;
		}
		default:
		{
			//TRACE("WHOIS %d %d ?\n", nIDCtl, IDC_SPELL_CHART_LIST);
		}
	}
	
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}




void CDMCharViewDialog::OnNpcCheck() 
{
	UpdateData(TRUE);

	m_pCharacter->m_bIsHenchman = m_bNPCCheck;

	Refresh();

	m_pCharacter->MarkChanged();
	
}

void CDMCharViewDialog::OnMonsterCheck() 
{
	/* NOT USED NOT NOW AND PROBABLY NOT EVER
	UpdateData(TRUE);

	m_pCharacter->m_bIsNPC = TRUE;

	Refresh();

	m_pCharacter->MarkChanged();
	*/
}



void CDMCharViewDialog::OnBnClickedCurrentClassCheck1()
{
	UpdateData(TRUE);

	m_pCharacter->m_nDualClassClass = 0;

	ProcessCharStats();

	Refresh();

	m_pCharacter->MarkChanged();
}

void CDMCharViewDialog::OnBnClickedCurrentClassCheck2()
{
	UpdateData(TRUE);

	m_pCharacter->m_nDualClassClass = 1;

	ProcessCharStats();

	Refresh();

	m_pCharacter->MarkChanged();
}

void CDMCharViewDialog::OnBnClickedCurrentClassCheck3()
{
	UpdateData(TRUE);

	m_pCharacter->m_nDualClassClass = 2;

	ProcessCharStats();

	Refresh();

	m_pCharacter->MarkChanged();
}

void CDMCharViewDialog::OnEnChangeExpValueEdit()
{
	UpdateData(TRUE);

	if(m_szXPValue != _T(""))
	{
		int nXPVal = atoi(m_szXPValue.GetBuffer(0));

		m_pCharacter->m_nXPValue = nXPVal;
	}
	else
	{
		m_pCharacter->m_nXPValue = 0;
	}


	ProcessCharStats();

	Refresh();

}


/*
void CDMCharViewDialog::OnLbnSelchangeLanguagesList()
{
	CDMLanguageSelectDialog *pDlg = new CDMLanguageSelectDialog(this);

	pDlg->DoModal();

	delete pDlg;

	Refresh();
}
*/

void CDMCharViewDialog::OnLbnDblclkLanguagesListBox()
{
	CDMLanguageSelectDialog *pDlg = new CDMLanguageSelectDialog(NULL, this);

	pDlg->DoModal();

	delete pDlg;

	Refresh();
}

void CDMCharViewDialog::OnLbnSelchangeLanguagesListBox()
{
	OnLbnDblclkLanguagesListBox();
}

void CDMCharViewDialog::OnCbnSelchangeCharVisionCombo()
{
	UpdateData(TRUE);

	int nCursor = m_cVisionCombo.GetCurSel();

	if(nCursor > -1)
	{
		m_pCharacter->m_VisionType = (DND_CHARACTER_VISION_TYPES)m_cVisionCombo.GetItemData(nCursor);

		m_pCharacter->MarkChanged();

		Refresh();
	}

}

void CDMCharViewDialog::OnBnClickedKeenEaredCheck()
{
	UpdateData(TRUE);

	m_pCharacter->m_bKeenEar = m_bKeenEar;

	if(!m_pCharacter->m_bKeenEar)
	{
		m_pCharacter->m_bVeryKeenEar = m_pCharacter->m_bKeenEar;
		m_bVeryKeenEar = m_pCharacter->m_bVeryKeenEar;
	}

	m_pCharacter->MarkChanged();

	Refresh();
}

void CDMCharViewDialog::OnBnClickedKeenEaredCheck2()
{
	UpdateData(TRUE);

	m_pCharacter->m_bVeryKeenEar = m_bVeryKeenEar;

	if(m_pCharacter->m_bVeryKeenEar)
	{
		m_pCharacter->m_bKeenEar = m_pCharacter->m_bVeryKeenEar;
		m_bKeenEar = m_pCharacter->m_bKeenEar;
	}

	m_pCharacter->MarkChanged();

	Refresh();
}

void CDMCharViewDialog::OnCbnEditchangeSecondarySkillCombo()
{
	UpdateData(TRUE);

	if (strcmp(m_pCharacter->m_szCharacterName, "Elric") == 0)
	{
		TRACE("!");
	}

	strcpy(m_pCharacter->m_szSecondarySkill, m_szSecondarySkill.Left(63));

	m_pCharacter->MarkChanged();
}

void CDMCharViewDialog::OnCbnSelchangeSecondarySkillCombo()
{
	UpdateData(TRUE);

	int nCursor = m_cSecondarySkillCombo.GetCurSel();

	if(nCursor > -1)
	{
		if (strcmp(m_pCharacter->m_szCharacterName, "Elric") == 0)
		{
			TRACE("!");
		}

		m_cSecondarySkillCombo.GetLBText(nCursor, m_pCharacter->m_szSecondarySkill);

		m_pCharacter->MarkChanged();
	}

	Refresh();
}

void CDMCharViewDialog::OnEnChangeConcealedMoveEdit()
{
	UpdateData(TRUE);

	m_pCharacter->m_nConcealedMove = atoi(m_szConcealedMove.GetBuffer(0));

	m_pCharacter->MarkChanged();
}

void CDMCharViewDialog::OnEnChangeClimbingMoveEdit()
{
	UpdateData(TRUE);

	m_pCharacter->m_nClimbingMove = atoi(m_szClimbingMove.GetBuffer(0));

	m_pCharacter->MarkChanged();
}

void CDMCharViewDialog::OnEnChangeSpecialMoveEdit()
{
	UpdateData(TRUE);

	m_pCharacter->m_nSpecialMove = atoi(m_szSpecialMove.GetBuffer(0));

	m_pCharacter->MarkChanged();
}


void CDMCharViewDialog::OnCbnSelchangeLightSourceCombo()
{
	UpdateData(TRUE);

	int nCursor = m_cLightSourceCombo.GetCurSel();

	if (nCursor > -1)
	{
		m_pCharacter->m_nLightSourceID = m_cLightSourceCombo.GetItemData(nCursor);
	}
	else
	{
		m_pCharacter->m_nLightSourceID = 0;
	}

	Refresh();

	//m_pCharacter->m_nLightSourceRange;

	for (POSITION pos = m_pApp->m_MapViewMap.GetStartPosition(); pos != NULL;)
	{
		WORD wID;
		PDNDMAPVIEWDLG pMapDlg = NULL;
		m_pApp->m_MapViewMap.GetNextAssoc(pos, wID, pMapDlg);

		if (pMapDlg != NULL && pMapDlg->m_pDNDMap != NULL && pMapDlg->m_pDNDMap->m_bMapScaleFeet)
		{
			::PostMessage(pMapDlg->m_hWnd, DND_DIRTY_WINDOW_MESSAGE, 0, 0);
		}
	}

	for (POSITION pos = m_pApp->m_DetachedMapViewMap.GetStartPosition(); pos != NULL;)
	{
		WORD wID;
		PDNDMAPVIEWDLG pMapDlg = NULL;
		m_pApp->m_DetachedMapViewMap.GetNextAssoc(pos, wID, pMapDlg);

		if (pMapDlg != NULL && pMapDlg->m_pDNDMap != NULL && pMapDlg->m_pDNDMap->m_bMapScaleFeet) 
		{
			::PostMessage(pMapDlg->m_hWnd, DND_DIRTY_WINDOW_MESSAGE, 0, 0);
		}
	}

}
