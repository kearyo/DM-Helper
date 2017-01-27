// DMCustomSpellDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMCustomSpellDialog.h"


// CDMCustomSpellDialog dialog

IMPLEMENT_DYNAMIC(CDMCustomSpellDialog, CDialog)

CDMCustomSpellDialog::CDMCustomSpellDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDMCustomSpellDialog::IDD, pParent)
	, m_szSpellDescription(_T(""))
	, m_szSpellName(_T(""))
	, m_szSpellType(_T(""))
	, m_bVerbalComponent(FALSE)
	, m_bSomaticComponent(FALSE)
	, m_bMaterialComponent(FALSE)
	, m_szSpellRange(_T(""))
	, m_szSpellCastingTime(_T(""))
	, m_szSpellDuration(_T(""))
	, m_szSpellSavingThrow(_T(""))
	, m_szSpellAreaOfEffect(_T(""))
	, m_bReversible(FALSE)
{
	m_pApp = (CDMHelperApp *)AfxGetApp();
	m_nSpellLevel = -1;
	m_bChanged = FALSE;
}

CDMCustomSpellDialog::~CDMCustomSpellDialog()
{
}

void CDMCustomSpellDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TYPE_COMBO, m_cTypeList);
	DDX_Text(pDX, IDC_SPELL_DESCRIPTION_EDIT, m_szSpellDescription);
	DDX_Control(pDX, IDC_LEVEL_COMBO, m_cSpellLevels);
	DDX_Text(pDX, IDC_SPELL_NAME_EDIT, m_szSpellName);
	DDV_MaxChars(pDX, m_szSpellName, 63);
	DDX_Control(pDX, IDC_SPELL_LIST, m_cSpellList);
	DDX_CBString(pDX, IDC_TYPE_COMBO, m_szSpellType);
	DDX_Check(pDX, IDC_VERBAL_CHECK, m_bVerbalComponent);
	DDX_Check(pDX, IDC_SOMATIC_CHECK, m_bSomaticComponent);
	DDX_Check(pDX, IDC_MATERIAL_CHECK, m_bMaterialComponent);
	DDX_Control(pDX, IDC_DEBUG_SPELL_BUTTON, m_cDebugButton);
	DDX_Text(pDX, IDC_SPELL_RANGE_EDIT, m_szSpellRange);
	DDV_MaxChars(pDX, m_szSpellRange, 32);
	DDX_Text(pDX, IDC_SPELL_CASTING_TIME_EDIT, m_szSpellCastingTime);
	DDX_Text(pDX, IDC_SPELL_DURATION_EDIT, m_szSpellDuration);
	DDV_MaxChars(pDX, m_szSpellDuration, 32);
	DDX_Text(pDX, IDC_SPELL_SAVING_THROW_EDIT, m_szSpellSavingThrow);
	DDX_Text(pDX, IDC_SPELL_AREA_OF_EFFECT_EDIT, m_szSpellAreaOfEffect);
	DDV_MaxChars(pDX, m_szSpellDescription, 5120);
	DDX_Check(pDX, IDC_REVERSIBLE_CHECK, m_bReversible);
	DDX_Control(pDX, IDC_SAVE_SPELL_BUTTON, m_cSaveButton);
	DDX_Control(pDX, IDC_DELETE_SPELL_BUTTON, m_cDeleteButton);
}


void CDMCustomSpellDialog::Refresh()
{
	UpdateData(TRUE);

	CString szTemp;

	int nChecked = GetCheckedRadioButton(IDC_CLERIC_RADIO, IDC_ALL_RADIO);

	int nLevels = 7;
	switch(nChecked)
	{
		case IDC_CLERIC_RADIO:		m_nSpellBook = DND_CHARACTER_CLASS_CLERIC; break;
		case IDC_DRUID_RADIO:		m_nSpellBook = DND_CHARACTER_CLASS_DRUID; break;
		case IDC_MAGIC_USER_RADIO:	m_nSpellBook = DND_CHARACTER_CLASS_MAGE; nLevels = 9; break;
		case IDC_ILLUSIONIST_RADIO: m_nSpellBook = DND_CHARACTER_CLASS_ILLUSIONIST;	break;
		case IDC_ALL_RADIO:			m_nSpellBook = DND_CHARACTER_CLASS_UNDEF; nLevels = 9;	break;
	}

	int nCursor = m_cSpellLevels.GetCurSel();

	m_cSpellLevels.ResetContent();
	m_cSpellLevels.InsertString(0, "All");
	m_cSpellLevels.SetItemData(0, -1);

	int nStartLevel = 1;

	if(g_bUseUnearthedArcana)
	{
		nStartLevel = 0;
	}

	int nCount = 1;
	for(int i = nStartLevel; i <= nLevels; ++i)
	{
		szTemp.Format("%d", i);
		m_cSpellLevels.InsertString(nCount, szTemp);

		if(i == 0)
		{
			m_cSpellLevels.SetItemData(nCount++, 99);
		}
		else
		{
			m_cSpellLevels.SetItemData(nCount++, i);
		}
	}

	m_cSpellLevels.SetCurSel(nCursor);

	if(nCursor == -1 || (m_nSpellLevel > nLevels && m_nSpellLevel != 99))
	{
		m_nSpellLevel = -1;
		m_cSpellLevels.SetCurSel(0);
	}
	else
	{
		m_nSpellLevel = m_cSpellLevels.GetItemData(nCursor);
	}

	m_cSpellList.ResetContent();
	nCount = 0;
	int nSpellCount = m_pApp->m_CustomSpellArray.GetSize();
	for (i = 0; i < nSpellCount; ++i )
	{
		PSPELL pSpell = (PSPELL) m_pApp->m_CustomSpellArray.GetAt(i);

		if(pSpell == NULL)
			continue;

		int nCalcLevel = pSpell->m_nSpellLevel;
		if(nCalcLevel == 0)
		{
			nCalcLevel = 99;
		}

		if(!g_bUseUnearthedArcana && nCalcLevel == 99)
			continue;

		if((pSpell->m_ClassBook == m_nSpellBook || DND_CHARACTER_CLASS_UNDEF == m_nSpellBook) && (nCalcLevel == m_nSpellLevel || -1 == m_nSpellLevel))
		{
			szTemp.Format("%s ", pSpell->m_szSpellName);

			if(DND_CHARACTER_CLASS_UNDEF == m_nSpellBook)
			{
				CString szClass = GetClassName(pSpell->m_ClassBook);
				szTemp += szClass.Left(1);
			}
			if(-1 == m_nSpellLevel)
			{
				CString szLevel;
				szLevel.Format("(%d)", pSpell->m_nSpellLevel);
				szTemp += szLevel;
			}

			m_cSpellList.InsertString(nCount, szTemp);
			m_cSpellList.SetItemData(nCount, (DWORD_PTR)pSpell);

			if(pSpell->m_nSpellIdentifier == m_Spell.m_nSpellIdentifier)
			{
				m_cSpellList.SetCurSel(nCount);
			}

			++nCount;
		}
	}

	if((m_Spell.m_nSpellIdentifier == 0 || m_bChanged) && m_nSpellBook != DND_CHARACTER_CLASS_UNDEF)
	{
		m_cSaveButton.EnableWindow(TRUE);
	}
	else
	{
		m_cSaveButton.EnableWindow(FALSE);
	}

	if(m_Spell.m_nSpellIdentifier == 0)
	{
		m_cSpellList.SetCurSel(-1);
		m_cDeleteButton.EnableWindow(FALSE);
	}
	else
	{
		m_cDeleteButton.EnableWindow(TRUE);
	}

	UpdateData(FALSE);
}

BEGIN_MESSAGE_MAP(CDMCustomSpellDialog, CDialog)
	ON_BN_CLICKED(IDC_CLERIC_RADIO, &CDMCustomSpellDialog::OnBnClickedClericRadio)
	ON_BN_CLICKED(IDC_SAVE_SPELL_BUTTON, &CDMCustomSpellDialog::OnBnClickedSaveSpellButton)
	ON_BN_CLICKED(IDC_DRUID_RADIO, &CDMCustomSpellDialog::OnBnClickedDruidRadio)
	ON_BN_CLICKED(IDC_MAGIC_USER_RADIO, &CDMCustomSpellDialog::OnBnClickedMagicUserRadio)
	ON_BN_CLICKED(IDC_ILLUSIONIST_RADIO, &CDMCustomSpellDialog::OnBnClickedIllusionistRadio)
	ON_CBN_SELCHANGE(IDC_LEVEL_COMBO, &CDMCustomSpellDialog::OnCbnSelchangeLevelCombo)
	ON_BN_CLICKED(IDC_DEBUG_SPELL_BUTTON, &CDMCustomSpellDialog::OnBnClickedDebugSpellButton)
	ON_LBN_SELCHANGE(IDC_SPELL_LIST, &CDMCustomSpellDialog::OnLbnSelchangeSpellList)
	ON_EN_CHANGE(IDC_SPELL_NAME_EDIT, &CDMCustomSpellDialog::OnEnChangeSpellNameEdit)
	ON_CBN_SELCHANGE(IDC_TYPE_COMBO, &CDMCustomSpellDialog::OnCbnSelchangeTypeCombo)
	ON_CBN_EDITCHANGE(IDC_TYPE_COMBO, &CDMCustomSpellDialog::OnCbnEditchangeTypeCombo)
	ON_BN_CLICKED(IDC_REVERSIBLE_CHECK, &CDMCustomSpellDialog::OnBnClickedReversibleCheck)
	ON_BN_CLICKED(IDC_VERBAL_CHECK, &CDMCustomSpellDialog::OnBnClickedVerbalCheck)
	ON_BN_CLICKED(IDC_SOMATIC_CHECK, &CDMCustomSpellDialog::OnBnClickedSomaticCheck)
	ON_EN_CHANGE(IDC_SPELL_RANGE_EDIT, &CDMCustomSpellDialog::OnEnChangeSpellRangeEdit)
	ON_EN_CHANGE(IDC_SPELL_CASTING_TIME_EDIT, &CDMCustomSpellDialog::OnEnChangeSpellCastingTimeEdit)
	ON_EN_CHANGE(IDC_SPELL_DURATION_EDIT, &CDMCustomSpellDialog::OnEnChangeSpellDurationEdit)
	ON_EN_CHANGE(IDC_SPELL_SAVING_THROW_EDIT, &CDMCustomSpellDialog::OnEnChangeSpellSavingThrowEdit)
	ON_EN_CHANGE(IDC_SPELL_AREA_OF_EFFECT_EDIT, &CDMCustomSpellDialog::OnEnChangeSpellAreaOfEffectEdit)
	ON_EN_CHANGE(IDC_SPELL_DESCRIPTION_EDIT, &CDMCustomSpellDialog::OnEnChangeSpellDescriptionEdit)
	ON_BN_CLICKED(IDC_NEW_SPELL_BUTTON, &CDMCustomSpellDialog::OnBnClickedNewSpellButton)
	ON_BN_CLICKED(IDC_DELETE_SPELL_BUTTON, &CDMCustomSpellDialog::OnBnClickedDeleteSpellButton)
	ON_BN_CLICKED(IDC_ALL_RADIO, &CDMCustomSpellDialog::OnBnClickedAllRadio)
END_MESSAGE_MAP()


// CDMCustomSpellDialog message handlers

BOOL CDMCustomSpellDialog::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_pClericButton = (CButton*)GetDlgItem(IDC_CLERIC_RADIO);
	m_pDruidButton = (CButton*)GetDlgItem(IDC_DRUID_RADIO);
	m_pMagicUserButton = (CButton*)GetDlgItem(IDC_MAGIC_USER_RADIO);
	m_pIllusionistButton = (CButton*)GetDlgItem(IDC_ILLUSIONIST_RADIO);
	m_pAllButton = (CButton*)GetDlgItem(IDC_ALL_RADIO);
	
	m_nSpellBook = DND_CHARACTER_CLASS_CLERIC;
	m_pAllButton->SetCheck(TRUE);

	/*
	m_cTypeList.InsertString(0, "Abjuration");
	m_cTypeList.InsertString(1, "Alteration");
	m_cTypeList.InsertString(2, "Conjuration/Summoning");
	m_cTypeList.InsertString(3, "Divination");
	m_cTypeList.InsertString(4, "Enchantment/Charm");
	m_cTypeList.InsertString(5, "Invocation");
	*/

	int nCount = 0;
	for (POSITION pos = m_pApp->m_SpellTypeMap.GetStartPosition(); pos != NULL; )
	{
		WORD wID;
		CString *pType = NULL;
		m_pApp->m_SpellTypeMap.GetNextAssoc(pos,wID,pType);

		if(pType != NULL)
		{
			m_cTypeList.InsertString(nCount, *pType);
		}
	}


#ifdef _DEBUG
	m_cDebugButton.ShowWindow(SW_SHOW);
#endif

	m_cSpellList.SetHorizontalExtent(1000);

	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDMCustomSpellDialog::OnLbnSelchangeSpellList()
{
	UpdateData(TRUE);

	int nCursor = m_cSpellList.GetCurSel();

	if(nCursor < 0)
		return;

	PSPELL pSpell = (PSPELL)m_cSpellList.GetItemData(nCursor);

	memcpy(&m_Spell, pSpell, sizeof(cDNDSpell));

	if(m_Spell.m_nSpellLevel == 0)
	{
		m_Spell.m_nSpellLevel = 99;
	}

	m_pClericButton->SetCheck(FALSE);
	m_pDruidButton->SetCheck(FALSE);
	m_pMagicUserButton->SetCheck(FALSE);
	m_pIllusionistButton->SetCheck(FALSE);
	m_pAllButton->SetCheck(FALSE);

	switch(m_Spell.m_ClassBook)
	{
		case DND_CHARACTER_CLASS_CLERIC:		m_pClericButton->SetCheck(TRUE);		break;
		case DND_CHARACTER_CLASS_DRUID:			m_pDruidButton->SetCheck(TRUE);			break;
		case DND_CHARACTER_CLASS_MAGE:			m_pMagicUserButton->SetCheck(TRUE);		break;
		case DND_CHARACTER_CLASS_ILLUSIONIST:	m_pIllusionistButton->SetCheck(TRUE);	break;
	}

	for(int i = 0; i < m_cSpellLevels.GetCount(); ++i)
	{
		if(m_cSpellLevels.GetItemData(i) == m_Spell.m_nSpellLevel)
		{
			m_cSpellLevels.SetCurSel(i);
			break;
		}
	}

	Populate();

	if(m_Spell.m_nSpellIdentifier == 0)
	{
		m_cSpellList.SetCurSel(-1);
		m_cDeleteButton.EnableWindow(FALSE);
	}
	else
	{
		m_cDeleteButton.EnableWindow(TRUE);
	}

	Refresh();

}

void CDMCustomSpellDialog::Populate()
{
	
	m_szSpellName = m_Spell.m_szSpellName;
	m_szSpellType = m_Spell.m_szSpellType;

	if(m_szSpellType.Find(" Reversible") > -1)
	{
		m_szSpellType.Replace(" Reversible", "");
		m_bReversible = TRUE;
	}
	else
	{
		m_bReversible = FALSE;
	}
	m_szSpellType.Replace("(", "");
	m_szSpellType.Replace(")", "");

	m_szSpellRange = m_Spell.m_szSpellRange;
	m_szSpellCastingTime = m_Spell.m_szSpellCastingTime;
	m_szSpellDuration = m_Spell.m_szSpellDuration;
	m_szSpellSavingThrow = m_Spell.m_szSpellSavingThrow;
	m_szSpellAreaOfEffect = m_Spell.m_szSpellAreaOfEffect;
	m_szSpellDescription = m_Spell.m_szSpellDesc;

	m_szSpellDescription.Replace("Explanation/Description:", "");

	m_bVerbalComponent = FALSE;
	m_bSomaticComponent = FALSE;
	m_bMaterialComponent = FALSE;

	CString szComponents = m_Spell.m_szSpellComponents;

	if(szComponents.Find("V") >= 0)
		m_bVerbalComponent = TRUE;

	if(szComponents.Find("S") >= 0)
		m_bSomaticComponent = TRUE;

	if(szComponents.Find("M") >= 0)
		m_bMaterialComponent = TRUE;

	m_cSaveButton.EnableWindow(FALSE);
	m_bChanged = FALSE;

	UpdateData(FALSE);
}


void CDMCustomSpellDialog::OnBnClickedClericRadio()
{
	m_Spell.m_nSpellIdentifier = 0;
	Refresh();
}

void CDMCustomSpellDialog::OnBnClickedDruidRadio()
{
	m_Spell.m_nSpellIdentifier = 0;
	Refresh();
}

void CDMCustomSpellDialog::OnBnClickedMagicUserRadio()
{
	m_Spell.m_nSpellIdentifier = 0;
	Refresh();
}

void CDMCustomSpellDialog::OnBnClickedIllusionistRadio()
{
	m_Spell.m_nSpellIdentifier = 0;
	Refresh();
}


void CDMCustomSpellDialog::OnBnClickedAllRadio()
{
	m_Spell.Init();

	Populate();

	Refresh();
}


void CDMCustomSpellDialog::OnCbnSelchangeLevelCombo()
{
	m_Spell.m_nSpellIdentifier = 0;
	Refresh();
}


void CDMCustomSpellDialog::OnBnClickedNewSpellButton()
{
	m_Spell.Init();

	Populate();

	Refresh();
}


void CDMCustomSpellDialog::OnBnClickedSaveSpellButton()
{
	if(m_nSpellLevel < 0)
		return;

	UpdateData(TRUE);

	if(m_szSpellName.GetLength() < 1)
		return;

	cDNDSpellBook *pSpellBook = m_pApp->m_SpellBooks.GetAt(m_nSpellBook);

	if(pSpellBook == NULL)
		return;

	PSPELL pMasterListSpell = NULL;
	if(m_Spell.m_nSpellIdentifier == 0)
	{
		// find a slot in the master spell array
		int nMaxSpellIndex = 0;
		int nSpellCount = m_pApp->m_MasterSpellArray.GetSize();
		for (int i = 0; i < nSpellCount; ++i )
		{
			PSPELL pSpell = (PSPELL) m_pApp->m_MasterSpellArray.GetAt(i);
			if(pSpell->m_nSpellIdentifier > nMaxSpellIndex)
			{
				nMaxSpellIndex = pSpell->m_nSpellIdentifier;
			}
		}

		m_Spell.m_nSpellIdentifier = nMaxSpellIndex;
		pMasterListSpell = new cDNDSpell();
		m_pApp->m_CustomSpellArray.InsertAt(m_Spell.m_nSpellIdentifier, pMasterListSpell);
	}
	else
	{
		pMasterListSpell = (PSPELL) m_pApp->m_MasterSpellArray.GetAt(m_Spell.m_nSpellIdentifier);
	}
	

	strcpy(m_Spell.m_szSpellName, m_szSpellName.Left(63));
	strcpy(m_Spell.m_szSpellType, m_szSpellType.Left(53)); // room for 'Reversible'
	strcpy(m_Spell.m_szSpellRange, m_szSpellRange.Left(31));
	strcpy(m_Spell.m_szSpellCastingTime, m_szSpellCastingTime.Left(31));
	strcpy(m_Spell.m_szSpellDuration, m_szSpellDuration.Left(31));
	strcpy(m_Spell.m_szSpellSavingThrow, m_szSpellSavingThrow.Left(31));
	strcpy(m_Spell.m_szSpellAreaOfEffect, m_szSpellAreaOfEffect.Left(63));
	strcpy(m_Spell.m_szSpellDesc, m_szSpellDescription.Left(5119));

	if(m_bReversible)
	{
		strcat(m_Spell.m_szSpellType, " Reversible");
	}

	CString szTemp = _T("");
	BOOL bFirst = TRUE;

	if(m_bVerbalComponent)
	{
		szTemp += _T("V");
		bFirst = FALSE;
	}
	if(m_bSomaticComponent)
	{
		if(bFirst)
			szTemp += _T("S");
		else
			szTemp += _T(", S");

		bFirst = FALSE;
	}
	if(m_bMaterialComponent)
	{
		if(bFirst)
			szTemp += _T("M");
		else
			szTemp += _T(", M");

		bFirst = FALSE;
	}

	if(szTemp == _T(""))
	{
		szTemp = _T("N/A");
	}

	strcpy(m_Spell.m_szSpellComponents, szTemp.Left(31));

	m_Spell.m_ClassBook = m_nSpellBook;
	m_Spell.m_nSpellLevel = m_nSpellLevel;
	m_Spell.m_nSpellNumber = 127; // we really don't care here
	m_Spell.m_bSpellValid = TRUE;

	memcpy(pMasterListSpell, &m_Spell, sizeof(cDNDSpell));

	m_pApp->m_MasterSpellArray.SetAt(pMasterListSpell->m_nSpellIdentifier, pMasterListSpell);

	SaveCustomSpells();

	Refresh();

	m_bChanged = FALSE;

}


void CDMCustomSpellDialog::OnBnClickedDeleteSpellButton()
{
	if(m_Spell.m_nSpellIdentifier == 0)
	{
		return;
	}

	if(AfxMessageBox("Spell will be deleted completely !\nAre you sure ?", MB_YESNO) != IDYES)
		return;

	PSPELL pMasterListSpell = m_pApp->m_CustomSpellArray.GetAt(m_Spell.m_nSpellIdentifier);

	if(pMasterListSpell != NULL)
	{
		delete pMasterListSpell;
		
		
		m_pApp->m_CustomSpellArray.SetAt(m_Spell.m_nSpellIdentifier, NULL);
		m_pApp->m_MasterSpellArray.SetAt(m_Spell.m_nSpellIdentifier, NULL);

		SaveCustomSpells();

		m_Spell.Init();
		Populate();

		Refresh();

		m_bChanged = FALSE;
	}
}


void CDMCustomSpellDialog::SaveCustomSpells()
{
	CString szTemp;
	
	szTemp.Format("%sData/tables/CustomSpells.dat", m_pApp->m_szEXEPath.GetBuffer(0));

	FILE *pOutFile = fopen(szTemp.GetBuffer(0), "wt");

	if(pOutFile != NULL)
	{
		fprintf(pOutFile, "# It is not recommended that you modify this file manually - use the custom spell editor in the program !\n");
		fprintf(pOutFile, "# File Version: (DO NOT CHANGE THE NEXT LINE !)\n");
		fprintf(pOutFile, "%ld", DMH_CURRENT_VERSION);

		int nSpellCount = m_pApp->m_CustomSpellArray.GetSize();
		for (int i = 0; i < nSpellCount; ++i )
		{
			PSPELL pSpell = (PSPELL) m_pApp->m_CustomSpellArray.GetAt(i);

			if(pSpell == NULL)
				continue;

			CString szClass = GetClassName(pSpell->m_ClassBook);
			szClass.Replace(" ", "_");

			fprintf(pOutFile, "\n#\n%s\n", pSpell->m_szSpellName);
			CString szType = pSpell->m_szSpellType;

			int nBooger = szType.GetLength();

			szType.Replace("(","");
			szType.Replace(")","");

			fprintf(pOutFile, "(%s)\n", szType);

			if(pSpell->m_nSpellLevel == 0)
				pSpell->m_nSpellLevel = 99;

			// Level: 1    Components: V, S, M    Range: 1"    Casting Time: 1 round    Duration: Permanent    Saving Throw: None    Area of Effect: Up to 27 cubic feet 
			fprintf(pOutFile, "Class:%s|Level:%d|Components:%s|Range:%s|Casting Time:%s|Duration:%s|Saving Throw:%s|Area of Effect:%s\n",
				szClass,
				pSpell->m_nSpellLevel,
				pSpell->m_szSpellComponents,
				pSpell->m_szSpellRange,
				pSpell->m_szSpellCastingTime,
				pSpell->m_szSpellDuration,
				pSpell->m_szSpellSavingThrow,
				pSpell->m_szSpellAreaOfEffect);

				szTemp = pSpell->m_szSpellDesc;
				szTemp.Replace("#", "*");
				szTemp.Replace("\n", " ");
				szTemp.Replace("\r", " ");
				szTemp.Replace("Explanation/Description:", "");
				fprintf(pOutFile, "Explanation/Description:%s", szTemp);

		}

		fclose(pOutFile);
	}

	m_pApp->InitializeSpells();


}


void CDMCustomSpellDialog::OnBnClickedDebugSpellButton()
{
	#ifdef _DEBUG

	m_nSpellBook = DND_CHARACTER_CLASS_MAGE;
	m_pMagicUserButton->SetCheck(TRUE);
	m_pClericButton->SetCheck(FALSE);
	m_pDruidButton->SetCheck(FALSE);
	m_pIllusionistButton->SetCheck(FALSE);
	m_pAllButton->SetCheck(FALSE);

	m_cSpellLevels.SetCurSel(5);

	m_szSpellName = _T("Cause Fiery Herpes");

	m_szSpellType = _T("Alteration");

	m_szSpellRange = _T("Touch");
	m_szSpellCastingTime = _T("2 segments");
	m_szSpellDuration = _T("Permanent");
	m_szSpellSavingThrow = _T("Negates");
	m_szSpellAreaOfEffect = _T("Creature Touched");

	m_szSpellDescription = _T("This spell causes the target creature to be immediately afflicted with burning sores in their genital area");

	m_bVerbalComponent = TRUE;
	m_bSomaticComponent = TRUE;
	m_bMaterialComponent = TRUE;

	m_bReversible = TRUE;

	UpdateData(FALSE);

	Refresh();

	#endif
}


void CDMCustomSpellDialog::OnEnChangeSpellNameEdit()
{
	m_bChanged = TRUE;
	Refresh();
}

void CDMCustomSpellDialog::OnCbnSelchangeTypeCombo()
{
	m_bChanged = TRUE;
	m_cSaveButton.EnableWindow(TRUE);
}

void CDMCustomSpellDialog::OnCbnEditchangeTypeCombo()
{
	m_bChanged = TRUE;
	m_cSaveButton.EnableWindow(TRUE);
}

void CDMCustomSpellDialog::OnBnClickedReversibleCheck()
{
	m_bChanged = TRUE;
	Refresh();
}

void CDMCustomSpellDialog::OnBnClickedVerbalCheck()
{
	m_bChanged = TRUE;
	Refresh();
}

void CDMCustomSpellDialog::OnBnClickedSomaticCheck()
{
	m_bChanged = TRUE;
	Refresh();
}

void CDMCustomSpellDialog::OnEnChangeSpellRangeEdit()
{
	m_bChanged = TRUE;
	Refresh();
}

void CDMCustomSpellDialog::OnEnChangeSpellCastingTimeEdit()
{
	m_bChanged = TRUE;
	Refresh();
}

void CDMCustomSpellDialog::OnEnChangeSpellDurationEdit()
{
	m_bChanged = TRUE;
	Refresh();
}

void CDMCustomSpellDialog::OnEnChangeSpellSavingThrowEdit()
{
	m_bChanged = TRUE;
	Refresh();
}

void CDMCustomSpellDialog::OnEnChangeSpellAreaOfEffectEdit()
{
	m_bChanged = TRUE;
	Refresh();
}

void CDMCustomSpellDialog::OnEnChangeSpellDescriptionEdit()
{
	m_bChanged = TRUE;
	m_cSaveButton.EnableWindow(TRUE);
}
