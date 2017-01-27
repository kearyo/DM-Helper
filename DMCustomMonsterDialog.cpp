// DMCustomMonsterDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMCustomMonsterDialog.h"


// CDMCustomMonsterDialog dialog

IMPLEMENT_DYNAMIC(CDMCustomMonsterDialog, CDialog)

CDMCustomMonsterDialog::CDMCustomMonsterDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDMCustomMonsterDialog::IDD, pParent)
	, m_szMonsterType(_T(""))
	, m_szAttack(_T(""))
	, m_szDefense(_T(""))
	, m_szMagicResistance(_T(""))
	, m_szAC(_T(""))
	, m_szHD(_T(""))
	, m_szInt(_T(""))
	, m_szDesc(_T(""))
	, m_szSize(_T(""))
	, m_szAttacks(_T(""))
	, m_szDamage(_T(""))
	, m_szMove(_T(""))
	, m_bChanged (FALSE)
	, m_szTreasureType(_T(""))
{
	m_pApp = (CDMHelperApp *)AfxGetApp();
}

CDMCustomMonsterDialog::~CDMCustomMonsterDialog()
{
}

void CDMCustomMonsterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MONSTER_LIST, m_cMonsterList);
	DDX_Text(pDX, IDC_MONSTER_TYPE_EDIT, m_szMonsterType);
	DDV_MaxChars(pDX, m_szMonsterType, 64);
	DDX_Control(pDX, IDC_CHAR_ALIGNMENT_COMBO, m_cAlignmentCombo);
	DDX_Control(pDX, IDC_DESC_COMBO, m_cDescCombo);
	DDX_Text(pDX, IDC_MONSTER_ATTACK_EDIT, m_szAttack);
	DDV_MaxChars(pDX, m_szAttack, 128);
	DDX_Text(pDX, IDC_MONSTER_DEFENSE_EDIT, m_szDefense);
	DDX_CBString(pDX, IDC_MAGIC_RESIST_COMBO, m_szMagicResistance);
	DDX_Text(pDX, IDC_MONSTER_AC_EDIT, m_szAC);
	DDX_Text(pDX, IDC_MONSTER_HD_EDIT, m_szHD);
	DDV_MaxChars(pDX, m_szHD, 16);
	DDX_Control(pDX, IDC_DEBUG_BUTTON, m_cDebugButton); 
	DDX_CBString(pDX, IDC_INT_COMBO, m_szInt);
	DDX_Control(pDX, IDC_INT_COMBO, m_cIntCombo);
	DDX_CBString(pDX, IDC_DESC_COMBO, m_szDesc);
	DDX_Text(pDX, IDC_MONSTER_SIZE_EDIT, m_szSize);
	DDV_MaxChars(pDX, m_szSize, 32);
	DDX_Text(pDX, IDC_MONSTER_ATTACKS_EDIT, m_szAttacks);
	DDV_MaxChars(pDX, m_szAttacks, 16);
	DDX_Text(pDX, IDC_MONSTER_DAMAGE_EDIT, m_szDamage);
	DDX_Text(pDX, IDC_MONSTER_AC_EDIT2, m_szMove);
	DDV_MaxChars(pDX, m_szMove, 32);
	DDX_Control(pDX, IDC_MAGIC_RESIST_COMBO, m_cMagicResistanceCombo);
	DDX_Control(pDX, IDC_NEW_MONSTER_BUTTON, m_cNewMonsterButton);
	DDX_Control(pDX, IDC_DELETE_MONSTER_BUTTON, m_cDeleteMonsterButton);
	DDX_Control(pDX, IDC_SAVE_MONSTER_BUTTON, m_cSaveMonsterButton);
	DDX_Text(pDX, IDC_MONSTER_TREASURE_TYPE_EDIT, m_szTreasureType);
}


void CDMCustomMonsterDialog::Populate()
{
	m_szMonsterType = m_Monster.m_szMMName;

	for(int i = 0; i <= 9; ++i)
	{
		if(m_Monster.m_nAlignment == m_cAlignmentCombo.GetItemData(i))
		{
			m_cAlignmentCombo.SetCurSel(i);
			break;
		}
	}

	m_szDesc = m_Monster.m_szType;
	m_szInt = m_Monster.m_szIntelligence;
	m_szSize = m_Monster.m_szSize;
	m_szAttack = m_Monster.m_szSpecialAttack;
	m_szDefense = m_Monster.m_szSpecialDefense;
	m_szMagicResistance = m_Monster.m_szMagicResist;
	m_szAC.Format("%d", m_Monster.m_nBaseAC);

	m_szMove = m_Monster.m_szMovement;
	m_szHD = m_Monster.m_szHD;
	m_szAttacks = m_Monster.m_szNumAttacks;
	m_szDamage = m_Monster.m_szDamage;
	m_szTreasureType = m_Monster.m_szTreasureType;

	SetButtons();
	
	UpdateData(FALSE);
}

void CDMCustomMonsterDialog::SetButtons()
{
	if(m_Monster.m_nMonsterIndex == 0 || m_bChanged)
	{
		m_cSaveMonsterButton.EnableWindow(TRUE);
		m_cDeleteMonsterButton.EnableWindow(FALSE);
	}
	else
	{
		m_cSaveMonsterButton.EnableWindow(FALSE);
		m_cDeleteMonsterButton.EnableWindow(TRUE);
	}
}

void CDMCustomMonsterDialog::Refresh()
{
	m_cMonsterList.ResetContent();
	int nCount = 0;
	for (POSITION pos = m_pApp->m_CustomMonsterManualIndexedMap.GetStartPosition(); pos != NULL; )
	{
		PDNDMONSTERMANUALENTRY pMonster = NULL;
		WORD wID;
		m_pApp->m_CustomMonsterManualIndexedMap.GetNextAssoc(pos,wID,pMonster);

		if(pMonster != NULL)
		{
			m_cMonsterList.InsertString(nCount, pMonster->m_szMMName);
			m_cMonsterList.SetItemData(nCount++, (DWORD_PTR)pMonster);
		}
	}


	UpdateData(FALSE);
}

void CDMCustomMonsterDialog::SaveCustomMonsters()
{

	WORD wMinKey = GetMinMapKey(&m_pApp->m_CustomMonsterManualIndexedMap);
	WORD wMaxKey = GetMaxMapKey(&m_pApp->m_CustomMonsterManualIndexedMap);

	CString szTemp;
	
	szTemp.Format("%sData/tables/CustomMonsters.dat", m_pApp->m_szEXEPath.GetBuffer(0));

	FILE *pOutFile = fopen(szTemp.GetBuffer(0), "wt");

	if(pOutFile != NULL)
	{
		fprintf(pOutFile, "# It is not recommended that you modify this file manually - use the custom monster editor in the program !\n");
		fprintf(pOutFile, "# ID|Name|Type|Rarity|Int|% In Lair|Treasure|Alignment|Number|AC|Move|HD|# Attacks|Damage|Spec Att|Spec Def|Magic Res|Size|Psi|PsiAttDef|XP|Armor|Weapon");

		for(int i = wMinKey; i <= wMaxKey; ++i)
		{
			PDNDMONSTERMANUALENTRY pMonster = NULL;
			m_pApp->m_CustomMonsterManualIndexedMap.Lookup(i, pMonster);

			if(pMonster != NULL)
			{
				fprintf(pOutFile, "\n%04d | %s | %s | %s | %s | %s | %s | %d | %s | %d | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s",
					pMonster->m_nMonsterIndex,
					pMonster->m_szMMName,
					pMonster->m_szType,
					pMonster->m_szFrequency,
					pMonster->m_szIntelligence,
					pMonster->m_szPercentinLair,
					pMonster->m_szTreasureType,
					pMonster->m_nAlignment,
					pMonster->m_szNumAppearing,
					pMonster->m_nBaseAC,
					pMonster->m_szMovement,
					pMonster->m_szHD,
					pMonster->m_szNumAttacks,
					pMonster->m_szDamage,
					pMonster->m_szSpecialAttack,
					pMonster->m_szSpecialDefense,
					pMonster->m_szMagicResist,
					pMonster->m_szSize,
					pMonster->m_szPsionicAbility,
					pMonster->m_szAttDefModes,
					pMonster->m_szXP,
					"swords",
					"armor"
					);

				
				
			}

		} // end for loop

		fclose(pOutFile);

		m_pApp->InitializeMonsters();

		Refresh();
	}


}


BEGIN_MESSAGE_MAP(CDMCustomMonsterDialog, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CDMCustomMonsterDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDMCustomMonsterDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_NEW_MONSTER_BUTTON, &CDMCustomMonsterDialog::OnBnClickedNewMonsterButton)
	ON_BN_CLICKED(IDC_DELETE_MONSTER_BUTTON, &CDMCustomMonsterDialog::OnBnClickedDeleteMonsterButton)
	ON_BN_CLICKED(IDC_SAVE_MONSTER_BUTTON, &CDMCustomMonsterDialog::OnBnClickedSaveMonsterButton)
	ON_EN_CHANGE(IDC_MONSTER_TYPE_EDIT, &CDMCustomMonsterDialog::OnEnChangeMonsterTypeEdit)
	ON_CBN_SELCHANGE(IDC_CHAR_ALIGNMENT_COMBO, &CDMCustomMonsterDialog::OnCbnSelchangeCharAlignmentCombo)
	ON_EN_CHANGE(IDC_MONSTER_ATTACK_EDIT, &CDMCustomMonsterDialog::OnEnChangeMonsterAttackEdit)
	ON_EN_CHANGE(IDC_MONSTER_DEFENSE_EDIT, &CDMCustomMonsterDialog::OnEnChangeMonsterDefenseEdit)
	ON_CBN_SELCHANGE(IDC_MAGIC_RESIST_COMBO, &CDMCustomMonsterDialog::OnCbnSelchangeMagicResistCombo)
	ON_EN_CHANGE(IDC_MONSTER_AC_EDIT, &CDMCustomMonsterDialog::OnEnChangeMonsterAcEdit)
	ON_EN_CHANGE(IDC_MONSTER_HD_EDIT, &CDMCustomMonsterDialog::OnEnChangeMonsterHdEdit)
	ON_BN_CLICKED(IDC_DEBUG_BUTTON, &CDMCustomMonsterDialog::OnBnClickedDebugButton)
	ON_CBN_SELCHANGE(IDC_INT_COMBO, &CDMCustomMonsterDialog::OnCbnSelchangeIntCombo)
	ON_CBN_EDITCHANGE(IDC_DESC_COMBO, &CDMCustomMonsterDialog::OnCbnEditchangeDescCombo)
	ON_CBN_SELCHANGE(IDC_DESC_COMBO, &CDMCustomMonsterDialog::OnCbnSelchangeDescCombo)
	ON_CBN_EDITCHANGE(IDC_INT_COMBO, &CDMCustomMonsterDialog::OnCbnEditchangeIntCombo)
	ON_CBN_EDITCHANGE(IDC_MAGIC_RESIST_COMBO, &CDMCustomMonsterDialog::OnCbnEditchangeMagicResistCombo)
	ON_LBN_SELCHANGE(IDC_MONSTER_LIST, &CDMCustomMonsterDialog::OnLbnSelchangeMonsterList)
	ON_EN_CHANGE(IDC_MONSTER_SIZE_EDIT, &CDMCustomMonsterDialog::OnEnChangeMonsterSizeEdit)
	ON_EN_CHANGE(IDC_MONSTER_ATTACKS_EDIT, &CDMCustomMonsterDialog::OnEnChangeMonsterAttacksEdit)
	ON_EN_CHANGE(IDC_MONSTER_DAMAGE_EDIT, &CDMCustomMonsterDialog::OnEnChangeMonsterDamageEdit)
	ON_EN_CHANGE(IDC_MONSTER_AC_EDIT2, &CDMCustomMonsterDialog::OnEnChangeMonsterAcEdit2)
	ON_EN_CHANGE(IDC_MONSTER_TREASURE_TYPE_EDIT, &CDMCustomMonsterDialog::OnEnChangeMonsterTreasureTypeEdit)
END_MESSAGE_MAP()


// CDMCustomMonsterDialog message handlers

BOOL CDMCustomMonsterDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	for(int i = 0; i <= 9; ++i)
	{
		m_cAlignmentCombo.InsertString(i, GetAlignmentName((DND_CHARACTER_ALIGNMENTS)i));
		m_cAlignmentCombo.SetItemData(i, i);
	}
	m_cAlignmentCombo.SetCurSel(0);

	int nCount = 0;
	for (POSITION pos = m_pApp->m_MonsterDescMap.GetStartPosition(); pos != NULL; )
	{
		WORD wID;
		CString *pDesc = NULL;
		m_pApp->m_MonsterDescMap.GetNextAssoc(pos,wID,pDesc);

		if(pDesc != NULL)
		{
			//m_cDescCombo.InsertString(nCount++, *pDesc);
			m_cDescCombo.AddString(*pDesc);
		}
	}

	nCount = 0;
	for (POSITION pos = m_pApp->m_MonsterIntMap.GetStartPosition(); pos != NULL; )
	{
		WORD wID;
		CString *pInt = NULL;
		m_pApp->m_MonsterIntMap.GetNextAssoc(pos,wID,pInt);

		if(pInt != NULL)
		{
			//m_cIntCombo.InsertString(nCount++, *pInt);
			m_cIntCombo.AddString(*pInt);
		}
	}

	nCount = 0;
	for (POSITION pos = m_pApp->m_MagicResistanceMap.GetStartPosition(); pos != NULL; )
	{
		WORD wID;
		CString *pMR = NULL;
		m_pApp->m_MagicResistanceMap.GetNextAssoc(pos,wID,pMR);

		if(pMR != NULL)
		{
			//m_cMagicResistanceCombo.InsertString(nCount++, *pMR);
			m_cMagicResistanceCombo.AddString(*pMR);
		}
	}

	#ifdef _DEBUG
	m_cDebugButton.EnableWindow(TRUE);
	#else
	m_cDebugButton.ShowWindow(SW_HIDE);
	#endif

	Refresh();

	SetButtons();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDMCustomMonsterDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CDMCustomMonsterDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CDMCustomMonsterDialog::OnBnClickedNewMonsterButton()
{
	m_cMonsterList.SetCurSel(-1);

	m_Monster.Init();

	m_bChanged = TRUE;

	Populate();
}

void CDMCustomMonsterDialog::OnBnClickedDeleteMonsterButton()
{
	if(m_Monster.m_nMonsterIndex)
	{
		cDNDMonsterManualEntry *pMonster = NULL;
		m_pApp->m_CustomMonsterManualIndexedMap.Lookup(m_Monster.m_nMonsterIndex, pMonster);

		delete pMonster;

		m_pApp->m_CustomMonsterManualIndexedMap.RemoveKey(m_Monster.m_nMonsterIndex);

		SaveCustomMonsters();

		m_cMonsterList.SetCurSel(-1);

		m_Monster.Init();

		Populate();
	}
}

void CDMCustomMonsterDialog::OnBnClickedSaveMonsterButton()
{
	UpdateData(TRUE);

	int nCursor = m_cMonsterList.GetCurSel();

	if(m_Monster.m_nMonsterIndex == 0)
	{
		cDNDMonsterManualEntry *pMonster = new cDNDMonsterManualEntry();
		pMonster->CopyFrom(&m_Monster);

		// find the next available 'slot'
		int nMaxIndex = max(GetMaxMapKey(&m_pApp->m_CustomMonsterManualIndexedMap), 3999);
		int nNewIndex = nMaxIndex+1;

		int nMinIndex = GetMinMapKey(&m_pApp->m_CustomMonsterManualIndexedMap);

		if(nMaxIndex > nMinIndex)
		{
			for (int i = nMinIndex; i < nMaxIndex; ++i)
			{
				PDNDMONSTERMANUALENTRY pCheckMonster = NULL;
				m_pApp->m_CustomMonsterManualIndexedMap.Lookup(i, pCheckMonster);
			
				if(pCheckMonster == NULL)
				{
					nNewIndex = i; //found an empty slot
					break;
				}
			}
		}

		pMonster->m_nMonsterIndex = nNewIndex;
		m_pApp->m_CustomMonsterManualIndexedMap.SetAt(pMonster->m_nMonsterIndex, pMonster);
	}
	else
	{
		cDNDMonsterManualEntry *pMonster = NULL;
		m_pApp->m_CustomMonsterManualIndexedMap.Lookup(m_Monster.m_nMonsterIndex, pMonster);

		if(pMonster != NULL)
		{
			pMonster->CopyFrom(&m_Monster);
		}
	}

	SaveCustomMonsters();

	m_cMonsterList.SetCurSel(nCursor);

	m_bChanged = FALSE;

	SetButtons();
}

void CDMCustomMonsterDialog::OnEnChangeMonsterTypeEdit()
{
	UpdateData(TRUE);
	m_Monster.m_szMMName = m_szMonsterType;

	m_Monster.m_nMonsterIndex = 0;

	SetButtons();
}

void CDMCustomMonsterDialog::OnCbnSelchangeCharAlignmentCombo()
{
	UpdateData(TRUE);

	int nCursor = m_cAlignmentCombo.GetCurSel();

	if(nCursor > -1)
	{
		m_Monster.m_nAlignment = (DND_CHARACTER_ALIGNMENTS)m_cAlignmentCombo.GetItemData(nCursor);
	}

	m_bChanged = TRUE;

	SetButtons();
}

void CDMCustomMonsterDialog::OnCbnSelchangeDescCombo()
{
	UpdateData(TRUE);

	m_cDescCombo.GetLBText(m_cDescCombo.GetCurSel(), m_szDesc);

	m_Monster.m_szType = m_szDesc;

	Populate();

	m_bChanged = TRUE;

	SetButtons();
}

void CDMCustomMonsterDialog::OnCbnEditchangeDescCombo()
{
	UpdateData(TRUE);

	m_Monster.m_szType = m_szDesc;

	m_bChanged = TRUE;

	SetButtons();
}


void CDMCustomMonsterDialog::OnCbnSelchangeIntCombo()
{
	UpdateData(TRUE);

	m_cIntCombo.GetLBText(m_cIntCombo.GetCurSel(), m_szInt);

	m_Monster.m_szIntelligence = m_szInt;

	Populate();

	m_bChanged = TRUE;

	SetButtons();
}

void CDMCustomMonsterDialog::OnCbnEditchangeIntCombo()
{
	UpdateData(TRUE);

	m_Monster.m_szIntelligence = m_szInt;

	m_bChanged = TRUE;

	SetButtons();
}

void CDMCustomMonsterDialog::OnEnChangeMonsterSizeEdit()
{
	UpdateData(TRUE);

	m_Monster.m_szSize = m_szSize;

	m_bChanged = TRUE;

	SetButtons();
}


void CDMCustomMonsterDialog::OnEnChangeMonsterAttackEdit()
{
	UpdateData(TRUE);

	m_Monster.m_szSpecialAttack = m_szAttack;

	m_bChanged = TRUE;

	SetButtons();
}

void CDMCustomMonsterDialog::OnEnChangeMonsterDefenseEdit()
{
	UpdateData(TRUE);

	m_Monster.m_szSpecialDefense = m_szDefense;

	m_bChanged = TRUE;

	SetButtons();
}

void CDMCustomMonsterDialog::OnCbnSelchangeMagicResistCombo()
{
	UpdateData(TRUE);

	m_Monster.m_szMagicResist = m_szMagicResistance;

	m_bChanged = TRUE;

	SetButtons();
}

void CDMCustomMonsterDialog::OnCbnEditchangeMagicResistCombo()
{
	UpdateData(TRUE);

	m_Monster.m_szMagicResist = m_szMagicResistance;

	m_bChanged = TRUE;

	SetButtons();
}

void CDMCustomMonsterDialog::OnEnChangeMonsterAcEdit()
{
	UpdateData(TRUE);

	m_Monster.m_nBaseAC = atoi(m_szAC.GetBuffer(0));

	m_bChanged = TRUE;

	SetButtons();
}


void CDMCustomMonsterDialog::OnEnChangeMonsterAcEdit2()
{
	UpdateData(TRUE);

	m_Monster.m_szMovement = m_szMove;

	m_bChanged = TRUE;

	SetButtons();
}


void CDMCustomMonsterDialog::OnEnChangeMonsterHdEdit()
{
	UpdateData(TRUE);

	m_Monster.m_szHD = m_szHD;

	m_bChanged = TRUE;

	SetButtons();
}

void CDMCustomMonsterDialog::OnEnChangeMonsterAttacksEdit()
{
	UpdateData(TRUE);

	m_Monster.m_szNumAttacks = m_szAttacks;

	m_bChanged = TRUE;

	SetButtons();
}

void CDMCustomMonsterDialog::OnEnChangeMonsterDamageEdit()
{
	UpdateData(TRUE);

	m_Monster.m_szDamage = m_szDamage;

	m_bChanged = TRUE;

	SetButtons();
}

void CDMCustomMonsterDialog::OnEnChangeMonsterTreasureTypeEdit()
{
	UpdateData(TRUE);

	m_Monster.m_szTreasureType = m_szTreasureType;

	m_bChanged = TRUE;

	SetButtons();
}



void CDMCustomMonsterDialog::OnBnClickedDebugButton()
{
	cDNDMonsterManualEntry *pMonster = NULL;
	m_pApp->m_MonsterManualIndexedMap.Lookup(1256, pMonster);

	if(pMonster != NULL)
	{
		m_Monster.CopyFrom(pMonster);
		m_Monster.m_nMonsterIndex = 0;
		Populate();
	}
}




void CDMCustomMonsterDialog::OnLbnSelchangeMonsterList()
{
	UpdateData(TRUE);

	int nCursor = m_cMonsterList.GetCurSel();

	if(nCursor > -1)
	{
		cDNDMonsterManualEntry *pMonster = (cDNDMonsterManualEntry *)m_cMonsterList.GetItemData(nCursor);

		if(pMonster != NULL)
		{
			m_Monster.CopyFrom(pMonster);
			Populate();
		}

		m_bChanged = FALSE;

		SetButtons();
	}
}



