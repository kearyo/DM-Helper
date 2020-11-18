// DMCustomClassDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMCharSpellsDialog.h"
#include "DMInventoryDialog.h"
#include "DMCharViewDialog.h"
#include "DMCustomClassDialog.h"
#include "afxdialogex.h"


CString FormatInt(int nVal)
{
	CString szRetval = "";

	if (nVal)
	{
		szRetval.Format("%d", nVal);
	}

	return szRetval;
}

CString FormatLong(LONG lVal)
{
	CString szRetval = "";

	if (lVal)
	{
		szRetval.Format("%ld", lVal);
	}

	return szRetval;
}

// CDMCustomClassDialog dialog

IMPLEMENT_DYNAMIC(CDMCustomClassDialog, CDialog)

CDMCustomClassDialog::CDMCustomClassDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDMCustomClassDialog::IDD, pParent)
	, m_szClassName(_T(""))
	, m_szHitDieType(_T("6"))
	, m_szMaxChartLevelXP(_T("220000"))
	, m_szMaxChartLevel(_T("9"))
	, m_szMaxChartLevelHP(_T("2"))
	, m_szMaxChartHPLevel(_T(""))
	, m_szMagicLevel(_T(""))
	, m_bThiefSkills(FALSE)
	, m_szThiefLevel(_T(""))
	, m_szAssassinLevel(_T(""))
	, m_bAssassinSkills(FALSE)
	, m_szTurnUndeadLevel(_T(""))
	, m_bTurnUndead(FALSE)
	, m_szInitialWeaponProf(_T(""))
	, m_szAddWeaponProf(_T(""))
	, m_szAddWeaponProfLevels(_T(""))
	, m_szNonWeaponProfPenalty(_T(""))
	, m_szWeaponSpecializationLevel(_T(""))
	, m_bWeaponSpecialization(FALSE)
{
	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_pSelectedClass = &m_CustomClass;

	for (int i = 0; i < 12; ++i)
	{
		m_szBaseLevelXP[i] = "-";
		m_szMaxLevelXP[i] = "-";
	}
	m_szBaseLevelXP[0] = "0";


	for (int i = 0; i < 6; ++i)
	{
		m_szMinAttrib[i] = _T("");
		m_bPreReqCheck[i] = FALSE;
	}

	for (int i = 0; i < 12; ++i)
	{
		m_szLevelHD[i] = FormatInt(i + 1);
	}

	m_bChanged = FALSE;
}

CDMCustomClassDialog::~CDMCustomClassDialog()
{
}

void CDMCustomClassDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CLASS_NAME_COMBO, m_cClassNameCombo);
	DDX_CBString(pDX, IDC_CLASS_NAME_COMBO, m_szClassName);
	DDV_MaxChars(pDX, m_szClassName, 32);
	DDX_Text(pDX, IDC_HD_TYPE_EDIT, m_szHitDieType);
	DDX_Text(pDX, IDC_BASE_LEVEL_XP_EDIT_1, m_szBaseLevelXP[0]);
	DDX_Text(pDX, IDC_BASE_LEVEL_XP_EDIT_2, m_szBaseLevelXP[1]);
	DDX_Text(pDX, IDC_BASE_LEVEL_XP_EDIT_3, m_szBaseLevelXP[2]);
	DDX_Text(pDX, IDC_BASE_LEVEL_XP_EDIT_4, m_szBaseLevelXP[3]);
	DDX_Text(pDX, IDC_BASE_LEVEL_XP_EDIT_5, m_szBaseLevelXP[4]);
	DDX_Text(pDX, IDC_BASE_LEVEL_XP_EDIT_6, m_szBaseLevelXP[5]);
	DDX_Text(pDX, IDC_BASE_LEVEL_XP_EDIT_7, m_szBaseLevelXP[6]);
	DDX_Text(pDX, IDC_BASE_LEVEL_XP_EDIT_8, m_szBaseLevelXP[7]);
	DDX_Text(pDX, IDC_BASE_LEVEL_XP_EDIT_9, m_szBaseLevelXP[8]);
	DDX_Text(pDX, IDC_BASE_LEVEL_XP_EDIT_10, m_szBaseLevelXP[9]);
	DDX_Text(pDX, IDC_BASE_LEVEL_XP_EDIT_11, m_szBaseLevelXP[10]);
	DDX_Text(pDX, IDC_BASE_LEVEL_XP_EDIT_12, m_szBaseLevelXP[11]);


	DDX_Text(pDX, IDC_MAX_LEVEL_XP_EDIT_1, m_szMaxLevelXP[0]);
	DDX_Text(pDX, IDC_MAX_LEVEL_XP_EDIT_2, m_szMaxLevelXP[1]);
	DDX_Text(pDX, IDC_MAX_LEVEL_XP_EDIT_3, m_szMaxLevelXP[2]);
	DDX_Text(pDX, IDC_MAX_LEVEL_XP_EDIT_4, m_szMaxLevelXP[3]);
	DDX_Text(pDX, IDC_MAX_LEVEL_XP_EDIT_5, m_szMaxLevelXP[4]);
	DDX_Text(pDX, IDC_MAX_LEVEL_XP_EDIT_6, m_szMaxLevelXP[5]);
	DDX_Text(pDX, IDC_MAX_LEVEL_XP_EDIT_7, m_szMaxLevelXP[6]);
	DDX_Text(pDX, IDC_MAX_LEVEL_XP_EDIT_8, m_szMaxLevelXP[7]);
	DDX_Text(pDX, IDC_MAX_LEVEL_XP_EDIT_9, m_szMaxLevelXP[8]);
	DDX_Text(pDX, IDC_MAX_LEVEL_XP_EDIT_10, m_szMaxLevelXP[9]);
	DDX_Text(pDX, IDC_MAX_LEVEL_XP_EDIT_11, m_szMaxLevelXP[10]);
	DDX_Text(pDX, IDC_MAX_LEVEL_XP_EDIT_12, m_szMaxLevelXP[11]);

	DDX_Control(pDX, IDC_MAX_LEVEL_XP_EDIT_1, m_cMaxLevelXP[0]);
	DDX_Control(pDX, IDC_MAX_LEVEL_XP_EDIT_2, m_cMaxLevelXP[1]);
	DDX_Control(pDX, IDC_MAX_LEVEL_XP_EDIT_3, m_cMaxLevelXP[2]);
	DDX_Control(pDX, IDC_MAX_LEVEL_XP_EDIT_4, m_cMaxLevelXP[3]);
	DDX_Control(pDX, IDC_MAX_LEVEL_XP_EDIT_5, m_cMaxLevelXP[4]);
	DDX_Control(pDX, IDC_MAX_LEVEL_XP_EDIT_6, m_cMaxLevelXP[5]);
	DDX_Control(pDX, IDC_MAX_LEVEL_XP_EDIT_7, m_cMaxLevelXP[6]);
	DDX_Control(pDX, IDC_MAX_LEVEL_XP_EDIT_8, m_cMaxLevelXP[7]);
	DDX_Control(pDX, IDC_MAX_LEVEL_XP_EDIT_9, m_cMaxLevelXP[8]);
	DDX_Control(pDX, IDC_MAX_LEVEL_XP_EDIT_10, m_cMaxLevelXP[9]);
	DDX_Control(pDX, IDC_MAX_LEVEL_XP_EDIT_11, m_cMaxLevelXP[10]);
	DDX_Control(pDX, IDC_MAX_LEVEL_XP_EDIT_12, m_cMaxLevelXP[11]);

	DDX_Text(pDX, IDC_MAX_LEVEL_XP_EDIT, m_szMaxChartLevelXP);
	DDX_Text(pDX, IDC_HD_MAX_LEVEL_EDIT, m_szMaxChartLevel);


	DDX_Text(pDX, IDC_HD_MIN_HP_EDIT, m_szMaxChartLevelHP);
	DDX_Text(pDX, IDC_HD_MAX_HP_EDIT, m_szMaxChartHPLevel);
	DDX_Control(pDX, IDC_SAVING_THROW_CLASS_COMBO, m_cSavingThrowChartCombo);
	DDV_MaxChars(pDX, m_szHitDieType, 2);
	DDX_Control(pDX, IDC_MAGIC_USE_CLASS_COMBO, m_cMagicUseClassCombo);
	DDX_Text(pDX, IDC_STR_EDIT, m_szMinAttrib[0]);
	DDX_Text(pDX, IDC_INT_EDIT, m_szMinAttrib[1]);
	DDX_Text(pDX, IDC_WIS_EDIT, m_szMinAttrib[2]);
	DDX_Text(pDX, IDC_DEX_EDIT, m_szMinAttrib[3]);
	DDX_Text(pDX, IDC_CON_EDIT, m_szMinAttrib[4]);
	DDX_Text(pDX, IDC_CHA_EDIT, m_szMinAttrib[5]);
	DDX_Check(pDX, IDC_PREREQ_ATTRIB_CHECK_0, m_bPreReqCheck[0]);
	DDX_Check(pDX, IDC_PREREQ_ATTRIB_CHECK_1, m_bPreReqCheck[1]);
	DDX_Check(pDX, IDC_PREREQ_ATTRIB_CHECK_2, m_bPreReqCheck[2]);
	DDX_Check(pDX, IDC_PREREQ_ATTRIB_CHECK_3, m_bPreReqCheck[3]);
	DDX_Check(pDX, IDC_PREREQ_ATTRIB_CHECK_4, m_bPreReqCheck[4]);
	DDX_Check(pDX, IDC_PREREQ_ATTRIB_CHECK_5, m_bPreReqCheck[5]);

	DDX_Text(pDX, IDC_MAGIC_LEVEL_EDIT, m_szMagicLevel);
	DDX_Control(pDX, IDC_COMBAT_CHART_COMBO, m_cCombatChartCombo);
	DDX_Check(pDX, IDC_THIEF_SKILLS_CHECK, m_bThiefSkills);
	DDX_Text(pDX, IDC_THIEF_LEVEL_EDIT, m_szThiefLevel);
	DDX_Text(pDX, IDC_ASSASSIN_LEVEL_EDIT, m_szAssassinLevel);
	DDX_Check(pDX, IDC_ASSASSIN_SKILLS_CHECK, m_bAssassinSkills);
	DDX_Text(pDX, IDC_TURN_UNDEAD_LEVEL_EDIT, m_szTurnUndeadLevel);
	DDX_Check(pDX, IDC_TURN_UNDEAD_CHECK, m_bTurnUndead);
	DDX_Text(pDX, IDC_INIT_WEAPON_PROF_EDIT, m_szInitialWeaponProf);
	DDX_Text(pDX, IDC_ADD_WEAPON_PROF_EDIT, m_szAddWeaponProf);
	DDX_Text(pDX, IDC_ADD_WEAPON_PROF_LEVELS_EDIT, m_szAddWeaponProfLevels);
	DDX_Text(pDX, IDC_NON_WEAPON_PROF_PENALTY_EDIT, m_szNonWeaponProfPenalty);
	DDX_Control(pDX, IDC_TURN_UNDEAD_LEVEL_EDIT, m_cTurnUndeadLevel);
	DDX_Control(pDX, IDC_THIEF_LEVEL_EDIT, m_cThiefLevel);
	DDX_Control(pDX, IDC_ASSASSIN_LEVEL_EDIT, m_cAssassinLevel);
	DDX_Control(pDX, IDC_DEX_COMMENT_STATIC, m_cDexComment);
	DDX_Text(pDX, IDC_HD_LEVEL_EDIT_1, m_szLevelHD[0]);
	DDX_Text(pDX, IDC_HD_LEVEL_EDIT_2, m_szLevelHD[1]);
	DDX_Text(pDX, IDC_HD_LEVEL_EDIT_3, m_szLevelHD[2]);
	DDX_Text(pDX, IDC_HD_LEVEL_EDIT_4, m_szLevelHD[3]);
	DDX_Text(pDX, IDC_HD_LEVEL_EDIT_5, m_szLevelHD[4]);
	DDX_Text(pDX, IDC_HD_LEVEL_EDIT_6, m_szLevelHD[5]);
	DDX_Text(pDX, IDC_HD_LEVEL_EDIT_7, m_szLevelHD[6]);
	DDX_Text(pDX, IDC_HD_LEVEL_EDIT_8, m_szLevelHD[7]);
	DDX_Text(pDX, IDC_HD_LEVEL_EDIT_9, m_szLevelHD[8]);
	DDX_Text(pDX, IDC_HD_LEVEL_EDIT_10, m_szLevelHD[9]);
	DDX_Text(pDX, IDC_HD_LEVEL_EDIT_11, m_szLevelHD[10]);
	DDX_Text(pDX, IDC_HD_LEVEL_EDIT_12, m_szLevelHD[11]);
	DDX_Control(pDX, IDC_DEBUG_BUTTON, m_cDebugButton);
	DDX_Control(pDX, IDC_MAGIC_LEVEL_EDIT, m_cMagicUseLevel);
	DDX_Control(pDX, IDC_WEAPON_SPECIALIZATION_LEVEL_EDIT, m_cWeaponSpecializationLevel);
	DDX_Text(pDX, IDC_WEAPON_SPECIALIZATION_LEVEL_EDIT, m_szWeaponSpecializationLevel);
	DDX_Check(pDX, IDC_WEAPON_SPECIALIZATION_CHECK, m_bWeaponSpecialization);
}


BEGIN_MESSAGE_MAP(CDMCustomClassDialog, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CDMCustomClassDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDMCustomClassDialog::OnBnClickedOk)

	ON_EN_CHANGE(IDC_HD_TYPE_EDIT, &CDMCustomClassDialog::OnEnChangeHdTypeEdit)

	ON_EN_CHANGE(IDC_MAX_LEVEL_XP_EDIT_1, &CDMCustomClassDialog::OnEnChangeValue)
	ON_EN_CHANGE(IDC_MAX_LEVEL_XP_EDIT_2, &CDMCustomClassDialog::OnEnChangeValue)
	ON_EN_CHANGE(IDC_MAX_LEVEL_XP_EDIT_3, &CDMCustomClassDialog::OnEnChangeValue)
	ON_EN_CHANGE(IDC_MAX_LEVEL_XP_EDIT_4, &CDMCustomClassDialog::OnEnChangeValue)
	ON_EN_CHANGE(IDC_MAX_LEVEL_XP_EDIT_5, &CDMCustomClassDialog::OnEnChangeValue)
	ON_EN_CHANGE(IDC_MAX_LEVEL_XP_EDIT_6, &CDMCustomClassDialog::OnEnChangeValue)
	ON_EN_CHANGE(IDC_MAX_LEVEL_XP_EDIT_7, &CDMCustomClassDialog::OnEnChangeValue)
	ON_EN_CHANGE(IDC_MAX_LEVEL_XP_EDIT_8, &CDMCustomClassDialog::OnEnChangeValue)
	ON_EN_CHANGE(IDC_MAX_LEVEL_XP_EDIT_9, &CDMCustomClassDialog::OnEnChangeValue)
	ON_EN_CHANGE(IDC_MAX_LEVEL_XP_EDIT_10, &CDMCustomClassDialog::OnEnChangeValue)
	ON_EN_CHANGE(IDC_MAX_LEVEL_XP_EDIT_11, &CDMCustomClassDialog::OnEnChangeValue)
	ON_EN_CHANGE(IDC_MAX_LEVEL_XP_EDIT_12, &CDMCustomClassDialog::OnEnChangeValue)
	ON_EN_CHANGE(IDC_MAX_LEVEL_XP_EDIT, &CDMCustomClassDialog::OnEnChangeValue)
	ON_EN_CHANGE(IDC_HD_MAX_LEVEL_EDIT, &CDMCustomClassDialog::OnEnChangeValue)

	ON_EN_CHANGE(IDC_MAX_LEVEL_XP_EDIT, &CDMCustomClassDialog::OnEnChangeValue)
	ON_EN_CHANGE(IDC_HD_MAX_LEVEL_EDIT, &CDMCustomClassDialog::OnEnChangeValue)

	ON_EN_CHANGE(IDC_HD_MIN_HP_EDIT, &CDMCustomClassDialog::OnEnChangeValue)
	ON_EN_CHANGE(IDC_HD_MAX_HP_EDIT, &CDMCustomClassDialog::OnEnChangeValue)

	ON_EN_CHANGE(IDC_STR_EDIT, &CDMCustomClassDialog::OnEnChangeValue)
	ON_EN_CHANGE(IDC_INT_EDIT, &CDMCustomClassDialog::OnEnChangeValue)
	ON_EN_CHANGE(IDC_WIS_EDIT, &CDMCustomClassDialog::OnEnChangeValue)
	ON_EN_CHANGE(IDC_DEX_EDIT, &CDMCustomClassDialog::OnEnChangeValue)
	ON_EN_CHANGE(IDC_CON_EDIT, &CDMCustomClassDialog::OnEnChangeValue)
	ON_EN_CHANGE(IDC_CHA_EDIT, &CDMCustomClassDialog::OnEnChangeValue)

	ON_BN_CLICKED(IDC_PREREQ_ATTRIB_CHECK_0, &CDMCustomClassDialog::OnEnChangeValue)
	ON_BN_CLICKED(IDC_PREREQ_ATTRIB_CHECK_1, &CDMCustomClassDialog::OnEnChangeValue)
	ON_BN_CLICKED(IDC_PREREQ_ATTRIB_CHECK_2, &CDMCustomClassDialog::OnEnChangeValue)
	ON_BN_CLICKED(IDC_PREREQ_ATTRIB_CHECK_3, &CDMCustomClassDialog::OnEnChangeValue)
	ON_BN_CLICKED(IDC_PREREQ_ATTRIB_CHECK_4, &CDMCustomClassDialog::OnEnChangeValue)
	ON_BN_CLICKED(IDC_PREREQ_ATTRIB_CHECK_5, &CDMCustomClassDialog::OnEnChangeValue)

	ON_CBN_SELCHANGE(IDC_COMBAT_CHART_COMBO, &CDMCustomClassDialog::OnEnChangeValue)
	ON_CBN_SELCHANGE(IDC_SAVING_THROW_CLASS_COMBO, &CDMCustomClassDialog::OnEnChangeValue)
	ON_CBN_SELCHANGE(IDC_MAGIC_USE_CLASS_COMBO, &CDMCustomClassDialog::OnEnChangeValue)

	ON_EN_CHANGE(IDC_MAGIC_LEVEL_EDIT, &CDMCustomClassDialog::OnEnChangeValue)

	ON_EN_CHANGE(IDC_HD_LEVEL_EDIT_1, &CDMCustomClassDialog::OnEnChangeHdLevelEdit1)	// under the HD column
	
	
	ON_BN_CLICKED(IDC_DEBUG_BUTTON, &CDMCustomClassDialog::OnBnClickedDebugButton)
	ON_CBN_EDITCHANGE(IDC_CLASS_NAME_COMBO, &CDMCustomClassDialog::OnCbnEditchangeClassNameCombo)
	ON_CBN_SELCHANGE(IDC_CLASS_NAME_COMBO, &CDMCustomClassDialog::OnCbnSelchangeClassNameCombo)
	
	
	ON_BN_CLICKED(IDC_THIEF_SKILLS_CHECK, &CDMCustomClassDialog::OnEnChangeValue)
	ON_EN_CHANGE(IDC_THIEF_LEVEL_EDIT, &CDMCustomClassDialog::OnEnChangeValue)

	ON_BN_CLICKED(IDC_ASSASSIN_SKILLS_CHECK, &CDMCustomClassDialog::OnEnChangeValue)
	ON_EN_CHANGE(IDC_ASSASSIN_LEVEL_EDIT, &CDMCustomClassDialog::OnEnChangeValue)

	ON_BN_CLICKED(IDC_TURN_UNDEAD_CHECK, &CDMCustomClassDialog::OnEnChangeValue)
	ON_EN_CHANGE(IDC_TURN_UNDEAD_LEVEL_EDIT, &CDMCustomClassDialog::OnEnChangeValue)

	ON_EN_CHANGE(IDC_INIT_WEAPON_PROF_EDIT, &CDMCustomClassDialog::OnEnChangeValue)
	ON_EN_CHANGE(IDC_ADD_WEAPON_PROF_EDIT, &CDMCustomClassDialog::OnEnChangeValue)
	ON_EN_CHANGE(IDC_ADD_WEAPON_PROF_LEVELS_EDIT, &CDMCustomClassDialog::OnEnChangeValue)
	ON_EN_CHANGE(IDC_NON_WEAPON_PROF_PENALTY_EDIT, &CDMCustomClassDialog::OnEnChangeValue)
	
	ON_BN_CLICKED(IDC_WEAPON_SPECIALIZATION_CHECK, &CDMCustomClassDialog::OnBnClickedWeaponSpecializationCheck)
	ON_EN_CHANGE(IDC_WEAPON_SPECIALIZATION_LEVEL_EDIT, &CDMCustomClassDialog::OnEnChangeWeaponSpecializationLevelEdit)
END_MESSAGE_MAP()


// CDMCustomClassDialog message handlers


void CDMCustomClassDialog::OnBnClickedCancel()
{

	if (m_bChanged)
	{
		if (AfxMessageBox("Exit without saving changes ?", MB_YESNO) != IDYES)
		{
			return;
		}
	}

	CDialog::OnCancel();
}


void CDMCustomClassDialog::OnBnClickedOk()
{
	CString szFileName;

	szFileName.Format("%sData\\tables\\CustomClasses.dat", m_pApp->m_szEXEPath.GetBuffer(0));

	Update();

	#if CUSTOM_CLASSES
	if (m_pSelectedClass->m_bClassDefined == FALSE)
	{
		for (int i = 0; i < MAX_CUSTOM_CLASSES; ++i)
		{
			if (_gCustomClass[i].m_bClassDefined == FALSE)
			{
				memcpy(&_gCustomClass[i], m_pSelectedClass, sizeof(cDNDCustomClass));
				_gCustomClass[i].m_bClassDefined = TRUE;
				_gCustomClass[i].m_Class = (DND_CHARACTER_CLASSES)(i + DND_CHARACTER_CLASS_CUSTOM_1);
				break;
			}
		}
	}
	#endif

	for (int i = 0; i < MAX_CUSTOM_CLASSES; ++i)
	{
		if (_gCustomClass[i].m_szClassName[0] == 0 || _gCustomClass[i].m_bClassDefined != TRUE)
		{
			_gCustomClass[i].m_bClassDefined = FALSE;
		}

		if (_gCustomClass[i].m_nFirstLevelHD <= 0 || _gCustomClass[i].m_nFirstLevelHD > 2)
		{
			_gCustomClass[i].m_nFirstLevelHD = 1;
		}
	}

	for (POSITION pos = m_pApp->m_CharacterViewMap.GetStartPosition(); pos != NULL;)
	{
		WORD wID;
		PDNDCHARVIEWDLG pCharDlg = NULL;
		m_pApp->m_CharacterViewMap.GetNextAssoc(pos, wID, pCharDlg);

		if (pCharDlg != NULL && pCharDlg->m_pCharacter != NULL)
		{
			pCharDlg->InitClassListBox();
			pCharDlg->Refresh();
		}
	}


	FILE *pOutFile = fopen(szFileName, "wb");

	if (NULL != pOutFile)
	{
		UINT nVersion = DMH_CURRENT_VERSION;

		fwrite(&nVersion, sizeof(UINT), 1, pOutFile);	
		fwrite(&_gCustomClass, MAX_CUSTOM_CLASSES*sizeof(cDNDCustomClass), 1, pOutFile);

		fclose(pOutFile);
	}

	CDialog::OnOK();
}

void CDMCustomClassDialog::OnBnClickedDebugButton()
{
	m_pSelectedClass->Clear();

	for (int i = 0; i <= 9; ++i)
	{
		m_pSelectedClass->m_lXPLevel[i] = (i+1) * 1250;
	}

	Refresh();

	Update();

	Refresh();
}


BOOL CDMCustomClassDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	#ifdef _DEBUG
	m_cDebugButton.ShowWindow(SW_SHOW);
	#endif

	#if CUSTOM_CLASSES
	int nIndex = 0;
	if (GetNumDefinedCustomClasses() < MAX_CUSTOM_CLASSES)
	{
		m_cClassNameCombo.InsertString(nIndex++, "New Custom Class");
	}

	for (int i = 0; i < MAX_CUSTOM_CLASSES; ++i)
	{
		DND_CHARACTER_CLASSES nClass = (DND_CHARACTER_CLASSES)(i + DND_CHARACTER_CLASS_CUSTOM_1);

		if (IsDefinedClass(nClass))
		{	
			m_cClassNameCombo.InsertString(nIndex, GetCustomClass(nClass)->m_szClassName);
			++nIndex;
		}
	}
	#endif

	m_cClassNameCombo.SetCurSel(0);

	nIndex = 0;
	m_cCombatChartCombo.InsertString(nIndex, "Cleric"); m_cCombatChartCombo.SetItemData(nIndex++, DND_CHARACTER_CLASS_CLERIC);
	m_cCombatChartCombo.InsertString(nIndex, "Fighter"); m_cCombatChartCombo.SetItemData(nIndex++, DND_CHARACTER_CLASS_FIGHTER);
	m_cCombatChartCombo.InsertString(nIndex, "Magic User"); m_cCombatChartCombo.SetItemData(nIndex++, DND_CHARACTER_CLASS_MAGE);
	m_cCombatChartCombo.InsertString(nIndex, "Thief"); m_cCombatChartCombo.SetItemData(nIndex++, DND_CHARACTER_CLASS_THIEF);

	m_cCombatChartCombo.SetCurSel(0);

	nIndex = 0;
	m_cSavingThrowChartCombo.InsertString(nIndex, "Cleric"); m_cSavingThrowChartCombo.SetItemData(nIndex++, DND_CHARACTER_CLASS_CLERIC);
	m_cSavingThrowChartCombo.InsertString(nIndex, "Fighter"); m_cSavingThrowChartCombo.SetItemData(nIndex++, DND_CHARACTER_CLASS_FIGHTER);
	m_cSavingThrowChartCombo.InsertString(nIndex, "Magic User"); m_cSavingThrowChartCombo.SetItemData(nIndex++, DND_CHARACTER_CLASS_MAGE);
	m_cSavingThrowChartCombo.InsertString(nIndex, "Thief"); m_cSavingThrowChartCombo.SetItemData(nIndex++, DND_CHARACTER_CLASS_THIEF);

	m_cSavingThrowChartCombo.SetCurSel(0);


	nIndex = 0;
	m_cMagicUseClassCombo.InsertString(nIndex, "None"); m_cMagicUseClassCombo.SetItemData(nIndex++, DND_CHARACTER_CLASS_UNDEF);
	m_cMagicUseClassCombo.InsertString(nIndex, "Cleric"); m_cMagicUseClassCombo.SetItemData(nIndex++, DND_CHARACTER_CLASS_CLERIC);
	m_cMagicUseClassCombo.InsertString(nIndex, "Druid"); m_cMagicUseClassCombo.SetItemData(nIndex++, DND_CHARACTER_CLASS_DRUID);
	m_cMagicUseClassCombo.InsertString(nIndex, "Magic User"); m_cMagicUseClassCombo.SetItemData(nIndex++, DND_CHARACTER_CLASS_MAGE);
	m_cMagicUseClassCombo.InsertString(nIndex, "Illusionist"); m_cMagicUseClassCombo.SetItemData(nIndex++, DND_CHARACTER_CLASS_ILLUSIONIST);

	m_cMagicUseClassCombo.SetCurSel(0);

	Refresh();

	UpdateData(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDMCustomClassDialog::Update()
{
	UpdateData(TRUE);

	m_pSelectedClass->m_nHitDieType = atoi(m_szHitDieType.GetBuffer(0));

	m_pSelectedClass->m_nFirstLevelHD = atoi(m_szLevelHD[0].GetBuffer(0));

	for (int i = 0; i < 6; ++i)
	{
		//m_pSelectedClass->m_nMinAttributes[i] = max(atoi(m_szMinAttrib[i].GetBuffer(0)), 3);
		m_pSelectedClass->m_nMinAttributes[i] = atoi(m_szMinAttrib[i].GetBuffer(0));
		m_pSelectedClass->m_bPrerequisiteAttrib[i] = m_bPreReqCheck[i];
	}

	int nCursor = m_cCombatChartCombo.GetCurSel();
	if (nCursor > -1)
	{
		m_pSelectedClass->m_CombatClass = (DND_CHARACTER_CLASSES)m_cCombatChartCombo.GetItemData(nCursor);
	}

	nCursor = m_cSavingThrowChartCombo.GetCurSel();
	if (nCursor > -1)
	{
		m_pSelectedClass->m_SavingThrowClass = (DND_CHARACTER_CLASSES)m_cSavingThrowChartCombo.GetItemData(nCursor);
	}

	nCursor = m_cMagicUseClassCombo.GetCurSel();
	if (nCursor > -1)
	{
		m_pSelectedClass->m_MagicUseClass = (DND_CHARACTER_CLASSES)m_cMagicUseClassCombo.GetItemData(nCursor);
	}

	m_pSelectedClass->m_nMagicUseLevel = atoi(m_szMagicLevel.GetBuffer(0));

	m_pSelectedClass->m_bTurnUndead = m_bTurnUndead;
	m_pSelectedClass->m_nTurnUndeadLevel = atoi(m_szTurnUndeadLevel.GetBuffer(0));

	m_pSelectedClass->m_bThiefSkills = m_bThiefSkills;
	m_pSelectedClass->m_nThiefLevel = atoi(m_szThiefLevel.GetBuffer(0));

	m_pSelectedClass->m_bAssassinSkills = m_bAssassinSkills;
	m_pSelectedClass->m_nAssassinLevel = atoi(m_szAssassinLevel.GetBuffer(0));

	m_pSelectedClass->m_nInitialWeaponProf = atoi(m_szInitialWeaponProf.GetBuffer(0));
	m_pSelectedClass->m_nAddWeaponProfLevels = atoi(m_szAddWeaponProfLevels.GetBuffer(0));
	m_pSelectedClass->m_nNonWeaponProfPenalty = atoi(m_szNonWeaponProfPenalty.GetBuffer(0));

	m_pSelectedClass->m_nMaxChartLevel = atoi(m_szMaxChartLevel.GetBuffer(0));
	m_pSelectedClass->m_lMaxChartLevelXP = atol(m_szMaxChartLevelXP.GetBuffer(0));
	
	m_pSelectedClass->m_nMaxChartLevelHP = atoi(m_szMaxChartLevelHP.GetBuffer(0));	
	m_pSelectedClass->m_nMaxChartHPLevel = atoi(m_szMaxChartHPLevel.GetBuffer(0));

	m_pSelectedClass->m_bWeaponSpecialization = m_bWeaponSpecialization;
	m_pSelectedClass->m_nWeaponSpecializationLevel = atoi(m_szWeaponSpecializationLevel.GetBuffer(0));

	for (int i = 0; i < 12; ++i)
	{
		if (i < m_pSelectedClass->m_nMaxChartLevel)
		{
			m_pSelectedClass->m_lXPLevel[i] = atoi(m_szMaxLevelXP[i].GetBuffer(0));
		}
		else
		{
			m_pSelectedClass->m_lXPLevel[i] = m_pSelectedClass->m_lXPLevel[i - 1] + m_pSelectedClass->m_lMaxChartLevelXP;
		}
	}

	
}

void CDMCustomClassDialog::Refresh()
{
	m_szClassName = m_pSelectedClass->m_szClassName;

	switch (m_pSelectedClass->m_nHitDieType)
	{
		case 4:
		case 6:
		case 8:
		case 10:
		case 12:
		{
			break;
		}
		default:
		{
			m_pSelectedClass->m_nHitDieType = 6;
		}
	}

	m_szHitDieType.Format("%d", m_pSelectedClass->m_nHitDieType);

	if (m_pSelectedClass->m_bThiefSkills && m_pSelectedClass->m_nMinAttributes[ATTRIB_DEX] < 9)
	{
		m_pSelectedClass->m_nMinAttributes[ATTRIB_DEX] = 9;
	}

	for (int i = 0; i < 6; ++i)
	{
		m_szMinAttrib[i].Format("%d", m_pSelectedClass->m_nMinAttributes[i]);
		m_bPreReqCheck[i] = m_pSelectedClass->m_bPrerequisiteAttrib[i];
	}

	for (int i = 0; i < m_cClassNameCombo.GetCount(); ++i)
	{
		if (m_cClassNameCombo.GetItemData(i) == m_pSelectedClass->m_CombatClass)
		{
			m_cClassNameCombo.SetCurSel(i);
			break;
		}
	}

	for (int i = 0; i < m_cCombatChartCombo.GetCount(); ++i)
	{
		if (m_cCombatChartCombo.GetItemData(i) == m_pSelectedClass->m_CombatClass)
		{
			m_cCombatChartCombo.SetCurSel(i);
			break;
		}
	}

	for (int i = 0; i < m_cSavingThrowChartCombo.GetCount(); ++i)
	{
		if (m_cSavingThrowChartCombo.GetItemData(i) == m_pSelectedClass->m_SavingThrowClass)
		{
			m_cSavingThrowChartCombo.SetCurSel(i);
			break;
		}
	}

	m_cMagicUseLevel.ShowWindow(SW_HIDE);
	for (int i = 0; i < m_cMagicUseClassCombo.GetCount(); ++i)
	{
		if (m_cMagicUseClassCombo.GetItemData(i) == m_pSelectedClass->m_MagicUseClass)
		{
			m_cMagicUseClassCombo.SetCurSel(i);

			if (i)
			{
				m_cMagicUseLevel.ShowWindow(SW_SHOW);
			}
			break;
		}
	}

	m_szMagicLevel = FormatInt(m_pSelectedClass->m_nMagicUseLevel);

	m_bTurnUndead = m_pSelectedClass->m_bTurnUndead;
	m_szTurnUndeadLevel = FormatInt(m_pSelectedClass->m_nTurnUndeadLevel);

	m_bThiefSkills = m_pSelectedClass->m_bThiefSkills;
	m_szThiefLevel = FormatInt(m_pSelectedClass->m_nThiefLevel);

	m_bAssassinSkills = m_pSelectedClass->m_bAssassinSkills;
	m_szAssassinLevel = FormatInt(m_pSelectedClass->m_nAssassinLevel);

	m_bWeaponSpecialization = m_pSelectedClass->m_bWeaponSpecialization;
	m_szWeaponSpecializationLevel = FormatInt(m_pSelectedClass->m_nWeaponSpecializationLevel);

	if (m_bTurnUndead)
	{
		m_cTurnUndeadLevel.ShowWindow(SW_SHOW);
	}
	else
	{
		m_cTurnUndeadLevel.ShowWindow(SW_HIDE);
	}

	if (m_bThiefSkills)
	{
		m_cThiefLevel.ShowWindow(SW_SHOW);
		m_cDexComment.ShowWindow(SW_SHOW);
	}
	else
	{
		m_cThiefLevel.ShowWindow(SW_HIDE);
		m_cDexComment.ShowWindow(SW_HIDE);
	}

	if (m_bAssassinSkills)
	{
		m_cAssassinLevel.ShowWindow(SW_SHOW);
	}
	else
	{
		m_cAssassinLevel.ShowWindow(SW_HIDE);
	}

	if (m_bWeaponSpecialization)
	{
		m_cWeaponSpecializationLevel.ShowWindow(SW_SHOW);
	}
	else
	{
		m_cWeaponSpecializationLevel.ShowWindow(SW_HIDE);
	}

	m_szInitialWeaponProf = FormatInt(m_pSelectedClass->m_nInitialWeaponProf);
	m_szAddWeaponProfLevels = FormatInt(m_pSelectedClass->m_nAddWeaponProfLevels);
	m_szNonWeaponProfPenalty = FormatInt(m_pSelectedClass->m_nNonWeaponProfPenalty);

	m_szMaxChartLevel = FormatInt(m_pSelectedClass->m_nMaxChartLevel);
	m_szMaxChartLevelXP = FormatLong(m_pSelectedClass->m_lMaxChartLevelXP);

	m_szMaxChartLevelHP = FormatInt(m_pSelectedClass->m_nMaxChartLevelHP);
	m_szMaxChartHPLevel = FormatInt(m_pSelectedClass->m_nMaxChartHPLevel);


	for (int i = 0; i < 12; ++i)
	{
		m_szMaxLevelXP[i] = FormatLong(m_pSelectedClass->m_lXPLevel[i]);
	}

	m_szBaseLevelXP[0] = "0";

	m_szLevelHD[0] = FormatInt(m_pSelectedClass->m_nFirstLevelHD);
	int nCount = m_pSelectedClass->m_nFirstLevelHD;

	for (int i = 1; i < 12; ++i)
	{
		m_szBaseLevelXP[i] = FormatLong(m_pSelectedClass->m_lXPLevel[i-1] + 1);

		if (i < m_pSelectedClass->m_nMaxChartLevel)
		{
			m_cMaxLevelXP[i].EnableWindow(TRUE);
		}
		else
		{
			m_cMaxLevelXP[i].EnableWindow(FALSE);
		}

		if (i < m_pSelectedClass->m_nMaxChartHPLevel)
		{
			m_szLevelHD[i] = FormatInt(++nCount);
		}
		else
		{
			m_szLevelHD[i] = _T("");
		}
	}

	
	

	UpdateData(FALSE);
}

void CDMCustomClassDialog::OnEnChangeHdTypeEdit()
{
	m_bChanged = TRUE;

	UpdateData(TRUE);

	m_pSelectedClass->m_nHitDieType = atoi(m_szHitDieType.GetBuffer(0));
}


void CDMCustomClassDialog::OnEnChangeValue()
{
	m_bChanged = TRUE;

	Update();

	Refresh();
}


void CDMCustomClassDialog::OnEnChangeHdLevelEdit1()
{
	m_bChanged = TRUE;

	Update();
}

void CDMCustomClassDialog::OnBnClickedWeaponSpecializationCheck()
{
	m_bChanged = TRUE;

	Update();

	Refresh();
}

void CDMCustomClassDialog::OnEnChangeWeaponSpecializationLevelEdit()
{
	m_bChanged = TRUE;

	Update();

	Refresh();
}


void CDMCustomClassDialog::OnCbnEditchangeClassNameCombo()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	strcpy(m_pSelectedClass->m_szClassName, m_szClassName.Left(31));
}


void CDMCustomClassDialog::OnCbnSelchangeClassNameCombo()
{
	UpdateData(TRUE);

	int nPos = m_cClassNameCombo.GetCurSel();

	if (nPos > -1)
	{
		if (nPos == 0)
		{
			m_pSelectedClass = &m_CustomClass;
		}
		else
		{
			m_pSelectedClass = &_gCustomClass[nPos - 1];
			m_cClassNameCombo.SetCurSel(nPos);
		}

		Refresh();
	}
}






