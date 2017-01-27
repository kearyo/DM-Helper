// DMSpellSelectorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMSpellSelectorDialog.h"


// CDMSpellSelectorDialog dialog

IMPLEMENT_DYNAMIC(CDMSpellSelectorDialog, CDialog)

CDMSpellSelectorDialog::CDMSpellSelectorDialog(cDNDSpell **pSpell, DND_CHARACTER_CLASSES _MagicType, int *pnSpellCount, CWnd* pParent /*=NULL*/)
	: CDialog(CDMSpellSelectorDialog::IDD, pParent)
	, m_bClericCheck(FALSE)
	, m_bDruidCheck(FALSE)
	, m_bMagicUserCheck(FALSE)
	, m_bIllusionistCheck(FALSE)
	, m_szMultiEdit(_T("1"))
{

	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_pSpell = pSpell;
	m_pnSpellCount = pnSpellCount;

	m_MagicType = _MagicType;
	m_ClassBook = DND_CHARACTER_CLASS_UNDEF;
	m_nSpellLevel = -1;
}

CDMSpellSelectorDialog::~CDMSpellSelectorDialog()
{
}

void CDMSpellSelectorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CLERIC_CHECK, m_bClericCheck);
	DDX_Check(pDX, IDC_DRUID_CHECK, m_bDruidCheck);
	DDX_Check(pDX, IDC_MAGIC_USER_CHECK, m_bMagicUserCheck);
	DDX_Check(pDX, IDC_ILLUSIONIST_CHECK, m_bIllusionistCheck);
	DDX_Control(pDX, IDC_SPELL_LIST, m_cSpellList);
	DDX_Text(pDX, IDC_MULTI_EDIT, m_szMultiEdit);
	DDV_MaxChars(pDX, m_szMultiEdit, 3);
}

void CDMSpellSelectorDialog::Refresh()
{
	switch(m_MagicType)
	{
		case DND_CHARACTER_CLASS_CLERIC:
		{
			m_ClassBook = m_MagicType;

			m_bClericCheck = TRUE;
			m_bDruidCheck = FALSE;
			m_bMagicUserCheck = FALSE;
			m_bIllusionistCheck = FALSE;

			(GetDlgItem( IDC_CLERIC_CHECK ))-> EnableWindow(TRUE);
			(GetDlgItem( IDC_DRUID_CHECK ))-> EnableWindow(FALSE);
			(GetDlgItem( IDC_MAGIC_USER_CHECK ))-> EnableWindow(FALSE);
			(GetDlgItem( IDC_ILLUSIONIST_CHECK ))-> EnableWindow(FALSE);

			break;
		}
		case DND_CHARACTER_CLASS_DRUID:
		{
			m_ClassBook = m_MagicType;

			m_bClericCheck = FALSE;
			m_bDruidCheck = TRUE;
			m_bMagicUserCheck = FALSE;
			m_bIllusionistCheck = FALSE;

			(GetDlgItem( IDC_CLERIC_CHECK ))-> EnableWindow(FALSE);
			(GetDlgItem( IDC_DRUID_CHECK ))-> EnableWindow(TRUE);
			(GetDlgItem( IDC_MAGIC_USER_CHECK ))-> EnableWindow(FALSE);
			(GetDlgItem( IDC_ILLUSIONIST_CHECK ))-> EnableWindow(FALSE);

			break;
		}
		case DND_CHARACTER_CLASS_MAGE:
		{
			m_ClassBook = m_MagicType;

			m_bClericCheck = FALSE;
			m_bDruidCheck = FALSE;
			m_bMagicUserCheck = TRUE;
			m_bIllusionistCheck = FALSE;

			(GetDlgItem( IDC_CLERIC_CHECK ))-> EnableWindow(FALSE);
			(GetDlgItem( IDC_DRUID_CHECK ))-> EnableWindow(FALSE);
			(GetDlgItem( IDC_MAGIC_USER_CHECK ))-> EnableWindow(TRUE);
			(GetDlgItem( IDC_ILLUSIONIST_CHECK ))-> EnableWindow(FALSE);

			break;
		}
		case DND_CHARACTER_CLASS_ILLUSIONIST:
		{
			m_ClassBook = m_MagicType;

			m_bClericCheck = FALSE;
			m_bDruidCheck = FALSE;
			m_bMagicUserCheck = FALSE;
			m_bIllusionistCheck = TRUE;

			(GetDlgItem( IDC_CLERIC_CHECK ))-> EnableWindow(FALSE);
			(GetDlgItem( IDC_DRUID_CHECK ))-> EnableWindow(FALSE);
			(GetDlgItem( IDC_MAGIC_USER_CHECK ))-> EnableWindow(FALSE);
			(GetDlgItem( IDC_ILLUSIONIST_CHECK ))-> EnableWindow(TRUE);

			break;
		}
	}

	if(m_bClericCheck)
	{
		(GetDlgItem( IDC_RADIO8 ))-> ShowWindow(SW_HIDE);
		(GetDlgItem( IDC_RADIO9 ))-> ShowWindow(SW_HIDE);
	}
	else if(m_bDruidCheck)
	{
		(GetDlgItem( IDC_RADIO8 ))-> ShowWindow(SW_HIDE);
		(GetDlgItem( IDC_RADIO9 ))-> ShowWindow(SW_HIDE);

	}
	else if(m_bMagicUserCheck)
	{
		(GetDlgItem( IDC_RADIO8 ))-> ShowWindow(SW_SHOW);
		(GetDlgItem( IDC_RADIO9 ))-> ShowWindow(SW_SHOW);
	}
	else if(m_bIllusionistCheck)
	{
		(GetDlgItem( IDC_RADIO8 ))-> ShowWindow(SW_HIDE);
		(GetDlgItem( IDC_RADIO9 ))-> ShowWindow(SW_HIDE);
	}

	*m_pSpell = NULL;

	m_cSpellList.ResetContent();

	if(m_nSpellLevel >= 0 && m_nSpellLevel <= 9)
	{
		cDNDSpellBook *pBook = m_pApp->m_SpellBooks.GetAt(m_ClassBook);

		if(pBook != NULL)
		{
			int nIndex = 0;

			for(int i = 0; i < MAX_SPELLS_PER_LEVEL; ++i)
			{
				if(pBook->m_Spells[m_nSpellLevel][i].m_bSpellValid)
				{
					CString szTemp = pBook->m_Spells[m_nSpellLevel][i].m_szSpellName;
					m_cSpellList.InsertString(nIndex, szTemp);
					m_cSpellList.SetItemData(nIndex, (DWORD_PTR)&pBook->m_Spells[m_nSpellLevel][i]);
					++nIndex;
				}
			}
		}
	}

	UpdateData(FALSE);

}


BEGIN_MESSAGE_MAP(CDMSpellSelectorDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CDMSpellSelectorDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDMSpellSelectorDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_RADIO5, &CDMSpellSelectorDialog::OnBnClickedRadio5)
	ON_BN_CLICKED(IDC_CLERIC_CHECK, &CDMSpellSelectorDialog::OnBnClickedClericCheck)
	ON_BN_CLICKED(IDC_DRUID_CHECK, &CDMSpellSelectorDialog::OnBnClickedDruidCheck)
	ON_BN_CLICKED(IDC_MAGIC_USER_CHECK, &CDMSpellSelectorDialog::OnBnClickedMagicUserCheck)
	ON_BN_CLICKED(IDC_ILLUSIONIST_CHECK, &CDMSpellSelectorDialog::OnBnClickedIllusionistCheck)
	ON_BN_CLICKED(IDC_RADIO1, &CDMSpellSelectorDialog::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CDMSpellSelectorDialog::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CDMSpellSelectorDialog::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CDMSpellSelectorDialog::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_RADIO6, &CDMSpellSelectorDialog::OnBnClickedRadio6)
	ON_BN_CLICKED(IDC_RADIO7, &CDMSpellSelectorDialog::OnBnClickedRadio7)
	ON_BN_CLICKED(IDC_RADIO8, &CDMSpellSelectorDialog::OnBnClickedRadio8)
	ON_BN_CLICKED(IDC_RADIO9, &CDMSpellSelectorDialog::OnBnClickedRadio9)
	ON_LBN_SELCHANGE(IDC_SPELL_LIST, &CDMSpellSelectorDialog::OnLbnSelchangeSpellList)
END_MESSAGE_MAP()


// CDMSpellSelectorDialog message handlers

BOOL CDMSpellSelectorDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(m_pnSpellCount == NULL)
	{
		(GetDlgItem( IDC_MULTI_EDIT ))-> ShowWindow(SW_HIDE);
		(GetDlgItem( IDC_MULTI_STATIC ))-> ShowWindow(SW_HIDE);
	}

	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDMSpellSelectorDialog::OnBnClickedOk()
{
	UpdateData(TRUE);

	if(m_pnSpellCount != NULL)
	{
		*m_pnSpellCount = atoi(m_szMultiEdit.GetBuffer(0));

		if(*m_pnSpellCount < 1)
		{
			*m_pnSpellCount = 1;
		}
	}

	OnOK();
}

void CDMSpellSelectorDialog::OnBnClickedCancel()
{
	*m_pSpell = NULL;

	OnCancel();
}

void CDMSpellSelectorDialog::OnBnClickedClericCheck()
{
	UpdateData(TRUE);

	m_ClassBook = DND_CHARACTER_CLASS_CLERIC;

	m_bDruidCheck = FALSE;
	m_bMagicUserCheck = FALSE;
	m_bIllusionistCheck = FALSE;

	Refresh();
}

void CDMSpellSelectorDialog::OnBnClickedDruidCheck()
{
	UpdateData(TRUE);

	m_ClassBook = DND_CHARACTER_CLASS_DRUID;

	m_bClericCheck = FALSE;
	m_bMagicUserCheck = FALSE;
	m_bIllusionistCheck = FALSE;

	Refresh();
}

void CDMSpellSelectorDialog::OnBnClickedMagicUserCheck()
{
	UpdateData(TRUE);

	m_ClassBook = DND_CHARACTER_CLASS_MAGE;

	m_bClericCheck = FALSE;
	m_bDruidCheck = FALSE;
	m_bIllusionistCheck = FALSE;

	Refresh();
}

void CDMSpellSelectorDialog::OnBnClickedIllusionistCheck()
{
	UpdateData(TRUE);

	m_ClassBook = DND_CHARACTER_CLASS_ILLUSIONIST;

	m_bClericCheck = FALSE;
	m_bDruidCheck = FALSE;
	m_bMagicUserCheck = FALSE;

	Refresh();
}

void CDMSpellSelectorDialog::OnBnClickedRadio1()
{
	m_nSpellLevel = 1; 

	Refresh();
}

void CDMSpellSelectorDialog::OnBnClickedRadio2()
{
	m_nSpellLevel = 2; 

	Refresh();
}

void CDMSpellSelectorDialog::OnBnClickedRadio3()
{
	m_nSpellLevel = 3; 

	Refresh();
}

void CDMSpellSelectorDialog::OnBnClickedRadio4()
{
	m_nSpellLevel = 4; 

	Refresh();
}

void CDMSpellSelectorDialog::OnBnClickedRadio5()
{
	m_nSpellLevel = 5; 

	Refresh();
}

void CDMSpellSelectorDialog::OnBnClickedRadio6()
{
	m_nSpellLevel = 6; 

	Refresh();
}

void CDMSpellSelectorDialog::OnBnClickedRadio7()
{
	m_nSpellLevel = 7; 

	Refresh();
}

void CDMSpellSelectorDialog::OnBnClickedRadio8()
{
	m_nSpellLevel = 8; 

	Refresh();
}

void CDMSpellSelectorDialog::OnBnClickedRadio9()
{
	m_nSpellLevel = 9; 

	Refresh();
}

void CDMSpellSelectorDialog::OnLbnSelchangeSpellList()
{
	UpdateData(TRUE);

	int nCursor = m_cSpellList.GetCurSel();

	if(nCursor > -1)
	{
		*m_pSpell = (cDNDSpell *)m_cSpellList.GetItemData(nCursor);
	}
}
