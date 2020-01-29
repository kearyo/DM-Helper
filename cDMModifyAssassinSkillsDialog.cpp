// cDMModifyAssassinSkillsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "cDMModifyAssassinSkillsDialog.h"
#include "afxdialogex.h"


// cDMModifyAssassinSkillsDialog dialog

IMPLEMENT_DYNAMIC(cDMModifyAssassinSkillsDialog, CDialog)

cDMModifyAssassinSkillsDialog::cDMModifyAssassinSkillsDialog(int *pnAssassinSkillModifiers, CWnd* pParent /*=NULL*/)
	: CDialog(cDMModifyAssassinSkillsDialog::IDD, pParent)
{
	m_pnAssassinSkillModifiers = pnAssassinSkillModifiers;

	memcpy(m_nAssassinSkillModifiers, m_pnAssassinSkillModifiers, 10 * sizeof(int));
}

cDMModifyAssassinSkillsDialog::~cDMModifyAssassinSkillsDialog()
{
}

void cDMModifyAssassinSkillsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_ASS_SKILL_EDIT_1, m_szAssassinSkillModifier[0]);
	DDX_Text(pDX, IDC_ASS_SKILL_EDIT_2, m_szAssassinSkillModifier[1]);
	DDX_Text(pDX, IDC_ASS_SKILL_EDIT_3, m_szAssassinSkillModifier[2]);
	DDX_Text(pDX, IDC_ASS_SKILL_EDIT_4, m_szAssassinSkillModifier[3]);
	DDX_Text(pDX, IDC_ASS_SKILL_EDIT_5, m_szAssassinSkillModifier[4]);
	DDX_Text(pDX, IDC_ASS_SKILL_EDIT_6, m_szAssassinSkillModifier[5]);
	DDX_Text(pDX, IDC_ASS_SKILL_EDIT_7, m_szAssassinSkillModifier[6]);
	DDX_Text(pDX, IDC_ASS_SKILL_EDIT_8, m_szAssassinSkillModifier[7]);
	DDX_Text(pDX, IDC_ASS_SKILL_EDIT_9, m_szAssassinSkillModifier[8]);
	DDX_Text(pDX, IDC_ASS_SKILL_EDIT_10, m_szAssassinSkillModifier[9]);
}


BEGIN_MESSAGE_MAP(cDMModifyAssassinSkillsDialog, CDialog)
	ON_BN_CLICKED(IDOK, &cDMModifyAssassinSkillsDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &cDMModifyAssassinSkillsDialog::OnBnClickedCancel)
	ON_EN_SETFOCUS(IDC_ASS_SKILL_EDIT_1, &cDMModifyAssassinSkillsDialog::OnEnSetfocusAssSkillEdit1)
	ON_EN_SETFOCUS(IDC_ASS_SKILL_EDIT_2, &cDMModifyAssassinSkillsDialog::OnEnSetfocusAssSkillEdit2)
	ON_EN_SETFOCUS(IDC_ASS_SKILL_EDIT_3, &cDMModifyAssassinSkillsDialog::OnEnSetfocusAssSkillEdit3)
	ON_EN_SETFOCUS(IDC_ASS_SKILL_EDIT_4, &cDMModifyAssassinSkillsDialog::OnEnSetfocusAssSkillEdit4)
	ON_EN_SETFOCUS(IDC_ASS_SKILL_EDIT_5, &cDMModifyAssassinSkillsDialog::OnEnSetfocusAssSkillEdit5)
	ON_EN_SETFOCUS(IDC_ASS_SKILL_EDIT_6, &cDMModifyAssassinSkillsDialog::OnEnSetfocusAssSkillEdit6)
	ON_EN_SETFOCUS(IDC_ASS_SKILL_EDIT_7, &cDMModifyAssassinSkillsDialog::OnEnSetfocusAssSkillEdit7)
	ON_EN_SETFOCUS(IDC_ASS_SKILL_EDIT_8, &cDMModifyAssassinSkillsDialog::OnEnSetfocusAssSkillEdit8)
	ON_EN_SETFOCUS(IDC_ASS_SKILL_EDIT_9, &cDMModifyAssassinSkillsDialog::OnEnSetfocusAssSkillEdit9)
	ON_EN_SETFOCUS(IDC_ASS_SKILL_EDIT_10, &cDMModifyAssassinSkillsDialog::OnEnSetfocusAssSkillEdit10)
END_MESSAGE_MAP()


// cDMModifyAssassinSkillsDialog message handlers


BOOL cDMModifyAssassinSkillsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	for (int i = 0; i < 10; ++i)
	{
		if (m_nAssassinSkillModifiers[i] >= 0)
		{
			m_szAssassinSkillModifier[i].Format("+%d", m_nAssassinSkillModifiers[i]);
		}
		else
		{
			m_szAssassinSkillModifier[i].Format("%d", m_nAssassinSkillModifiers[i]);
		}
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void cDMModifyAssassinSkillsDialog::Refresh()
{
	UpdateData(TRUE);

	for (int i = 0; i < 10; ++i)
	{
		m_nAssassinSkillModifiers[i] = atoi(m_szAssassinSkillModifier[i].GetBuffer(0));

		if (m_nAssassinSkillModifiers[i] >= 0)
		{
			m_szAssassinSkillModifier[i].Format("+%d", m_nAssassinSkillModifiers[i]);
		}
		else
		{
			m_szAssassinSkillModifier[i].Format("%d", m_nAssassinSkillModifiers[i]);
		}
	}

	UpdateData(FALSE);
}

void cDMModifyAssassinSkillsDialog::OnBnClickedOk()
{
	Refresh();
	memcpy(m_pnAssassinSkillModifiers, m_nAssassinSkillModifiers, 10 * sizeof(int));
	CDialog::OnOK();
}


void cDMModifyAssassinSkillsDialog::OnBnClickedCancel()
{
	CDialog::OnCancel();
}


void cDMModifyAssassinSkillsDialog::OnEnSetfocusAssSkillEdit1()
{
	Refresh();
}
void cDMModifyAssassinSkillsDialog::OnEnSetfocusAssSkillEdit2()
{
	Refresh();
}

void cDMModifyAssassinSkillsDialog::OnEnSetfocusAssSkillEdit3()
{
	Refresh();
}

void cDMModifyAssassinSkillsDialog::OnEnSetfocusAssSkillEdit4()
{
	Refresh();
}

void cDMModifyAssassinSkillsDialog::OnEnSetfocusAssSkillEdit5()
{
	Refresh();
}

void cDMModifyAssassinSkillsDialog::OnEnSetfocusAssSkillEdit6()
{
	Refresh();
}

void cDMModifyAssassinSkillsDialog::OnEnSetfocusAssSkillEdit7()
{
	Refresh();
}

void cDMModifyAssassinSkillsDialog::OnEnSetfocusAssSkillEdit8()
{
	Refresh();
}

void cDMModifyAssassinSkillsDialog::OnEnSetfocusAssSkillEdit9()
{
	Refresh();
}

void cDMModifyAssassinSkillsDialog::OnEnSetfocusAssSkillEdit10()
{
	Refresh();
}

