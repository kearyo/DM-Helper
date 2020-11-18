// cDMRandomNameGeneratorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "cDMRandomNameGeneratorDialog.h"
#include "afxdialogex.h"


// cDMRandomNameGeneratorDialog dialog

IMPLEMENT_DYNAMIC(cDMRandomNameGeneratorDialog, CDialogEx)

cDMRandomNameGeneratorDialog::cDMRandomNameGeneratorDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(cDMRandomNameGeneratorDialog::IDD, pParent)
	, m_szGeneratedName(_T(""))
	, m_bMaleCheck(TRUE)
	, m_bFemaleCheck(FALSE)
	, m_szCopiedNote(_T(""))
{
	m_pApp = (CDMHelperApp *)AfxGetApp();
}

cDMRandomNameGeneratorDialog::~cDMRandomNameGeneratorDialog()
{
}

void cDMRandomNameGeneratorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_szGeneratedName);
	DDX_Control(pDX, IDC_EDIT1, m_GeneratedNameEdit);
	DDX_Check(pDX, IDC_MALE_CHECK, m_bMaleCheck);
	DDX_Check(pDX, IDC_FEMALE_CHECK, m_bFemaleCheck);
	DDX_Text(pDX, IDC_COPIED_STATIC, m_szCopiedNote);
}


BEGIN_MESSAGE_MAP(cDMRandomNameGeneratorDialog, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &cDMRandomNameGeneratorDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_HUMAN_BUTTON, &cDMRandomNameGeneratorDialog::OnBnClickedHumanButton)
	ON_BN_CLICKED(IDC_MALE_CHECK, &cDMRandomNameGeneratorDialog::OnBnClickedMaleCheck)
	ON_BN_CLICKED(IDC_FEMALE_CHECK, &cDMRandomNameGeneratorDialog::OnBnClickedFemaleCheck)
	ON_BN_CLICKED(IDC_ELVEN_BUTTON, &cDMRandomNameGeneratorDialog::OnBnClickedElvenButton)
	ON_BN_CLICKED(IDC_DWARVEN_BUTTON, &cDMRandomNameGeneratorDialog::OnBnClickedDwarvenButton)
	ON_BN_CLICKED(IDC_HALFLING_BUTTON, &cDMRandomNameGeneratorDialog::OnBnClickedHalflingButton)
	ON_BN_CLICKED(IDC_GNOME_BUTTON, &cDMRandomNameGeneratorDialog::OnBnClickedGnomeButton)
	ON_BN_CLICKED(IDC_BUGBEAR_BUTTON, &cDMRandomNameGeneratorDialog::OnBnClickedBugbearButton)
	ON_BN_CLICKED(IDC_DRAGON_BUTTON, &cDMRandomNameGeneratorDialog::OnBnClickedDragonButton)
	ON_BN_CLICKED(IDC_GOBLIN_BUTTON, &cDMRandomNameGeneratorDialog::OnBnClickedGoblinButton)
	ON_BN_CLICKED(IDC_HOBGOBLIN_BUTTON, &cDMRandomNameGeneratorDialog::OnBnClickedHobgoblinButton)
	ON_BN_CLICKED(IDC_KOBOLD_BUTTON, &cDMRandomNameGeneratorDialog::OnBnClickedKoboldButton)
	ON_BN_CLICKED(IDC_OGRE_BUTTON, &cDMRandomNameGeneratorDialog::OnBnClickedOgreButton)
	ON_BN_CLICKED(IDC_ORC_BUTTON, &cDMRandomNameGeneratorDialog::OnBnClickedOrcButton)
	ON_BN_CLICKED(IDOK, &cDMRandomNameGeneratorDialog::OnBnClickedOk)
END_MESSAGE_MAP()


DND_CHARACTER_RACES cDMRandomNameGeneratorDialog::GetMonsterRaceFromName(CString szName)
{
	/*PDNDMONSTERMANUALENTRY pMonster = NULL;

	m_pApp->m_MonsterManualIndexedMap.Lookup(m_pNPC->m_nMonsterIndex, pMonster);

	if (pMonster != NULL)
	{ */

	WORD wID;
	for (POSITION pos = m_pApp->m_MonsterManualIndexedMap.GetStartPosition(); pos != NULL;)
	{
		PDNDMONSTERMANUALENTRY pMonster = NULL;
		m_pApp->m_MonsterManualIndexedMap.GetNextAssoc(pos, wID, pMonster);

		if (pMonster != NULL)
		{
			if (pMonster->m_szMMName == szName)
			{
				return (DND_CHARACTER_RACES)pMonster->m_nMonsterIndex;
			}
		}
	}


	return DND_CHARACTER_RACES_UNDEF;
}

void cDMRandomNameGeneratorDialog::GenerateName(DND_CHARACTER_RACES nRace)
{
	int nGender = 0;
	if (m_bFemaleCheck)
	{
		nGender = 1;
	}

	m_szGeneratedName = m_pApp->GetRandomName(nRace, nGender);
	UpdateData(FALSE);

	const char* output = m_szGeneratedName.GetBuffer(0);
	const size_t len = strlen(output) + 1;
	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);
	memcpy(GlobalLock(hMem), output, len);
	GlobalUnlock(hMem);
	OpenClipboard();
	EmptyClipboard();
	SetClipboardData(CF_TEXT, hMem);
	CloseClipboard();

	m_szCopiedNote.Format("\"%s\" copied to clipboard !", m_szGeneratedName);
	UpdateData(FALSE);
}

// cDMRandomNameGeneratorDialog message handlers

BOOL cDMRandomNameGeneratorDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void cDMRandomNameGeneratorDialog::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

void cDMRandomNameGeneratorDialog::OnBnClickedMaleCheck()
{
	UpdateData(TRUE);
	m_bFemaleCheck = !m_bMaleCheck;
	UpdateData(FALSE);
}

void cDMRandomNameGeneratorDialog::OnBnClickedFemaleCheck()
{
	UpdateData(TRUE);
	m_bMaleCheck = !m_bFemaleCheck;
	UpdateData(FALSE);
}

void cDMRandomNameGeneratorDialog::OnBnClickedHumanButton()
{
	GenerateName(DND_CHARACTER_RACE_HUMAN);
}

void cDMRandomNameGeneratorDialog::OnBnClickedElvenButton()
{
	GenerateName(DND_CHARACTER_RACE_ELF);
}


void cDMRandomNameGeneratorDialog::OnBnClickedDwarvenButton()
{
	GenerateName(DND_CHARACTER_RACE_DWARF);
}


void cDMRandomNameGeneratorDialog::OnBnClickedHalflingButton()
{
	GenerateName(DND_CHARACTER_RACE_HALFLING);
}


void cDMRandomNameGeneratorDialog::OnBnClickedGnomeButton()
{
	GenerateName(DND_CHARACTER_RACE_GNOME);
}


void cDMRandomNameGeneratorDialog::OnBnClickedBugbearButton()
{
	DND_CHARACTER_RACES nRace = GetMonsterRaceFromName("Bugbear");
	GenerateName(nRace);
}


void cDMRandomNameGeneratorDialog::OnBnClickedDragonButton()
{
	DND_CHARACTER_RACES nRace = GetMonsterRaceFromName("Dragon - Red");
	GenerateName(nRace);
}


void cDMRandomNameGeneratorDialog::OnBnClickedGoblinButton()
{
	DND_CHARACTER_RACES nRace = GetMonsterRaceFromName("Goblin");
	GenerateName(nRace);
}


void cDMRandomNameGeneratorDialog::OnBnClickedHobgoblinButton()
{
	DND_CHARACTER_RACES nRace = GetMonsterRaceFromName("Hobgoblin");
	GenerateName(nRace);
}


void cDMRandomNameGeneratorDialog::OnBnClickedKoboldButton()
{
	DND_CHARACTER_RACES nRace = GetMonsterRaceFromName("Kobold");
	GenerateName(nRace);
}


void cDMRandomNameGeneratorDialog::OnBnClickedOgreButton()
{
	DND_CHARACTER_RACES nRace = GetMonsterRaceFromName("Ogre");
	GenerateName(nRace);
}


void cDMRandomNameGeneratorDialog::OnBnClickedOrcButton()
{
	DND_CHARACTER_RACES nRace = GetMonsterRaceFromName("Orc");
	GenerateName(nRace);
}


void cDMRandomNameGeneratorDialog::OnBnClickedOk()
{
	CDialogEx::OnOK();
}
