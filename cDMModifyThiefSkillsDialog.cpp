// cDMModifyThiefSkillsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "cDMModifyThiefSkillsDialog.h"
#include "afxdialogex.h"


// cDMModifyThiefSkillsDialog dialog

IMPLEMENT_DYNAMIC(cDMModifyThiefSkillsDialog, CDialog)

cDMModifyThiefSkillsDialog::cDMModifyThiefSkillsDialog(int *pnThiefSkillModifiers, CWnd* pParent /*=NULL*/)
	: CDialog(cDMModifyThiefSkillsDialog::IDD, pParent)
{
	m_pnThiefSkillModifiers = pnThiefSkillModifiers;

	memcpy(m_nThiefSkillModifiers, m_pnThiefSkillModifiers, 8 * sizeof(int));
}

cDMModifyThiefSkillsDialog::~cDMModifyThiefSkillsDialog()
{
}

void cDMModifyThiefSkillsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_THIEF_SKILL_EDIT_1, m_szThiefSkillModifier[0]);
	DDX_Text(pDX, IDC_THIEF_SKILL_EDIT_2, m_szThiefSkillModifier[1]);
	DDX_Text(pDX, IDC_THIEF_SKILL_EDIT_3, m_szThiefSkillModifier[2]);
	DDX_Text(pDX, IDC_THIEF_SKILL_EDIT_4, m_szThiefSkillModifier[3]);
	DDX_Text(pDX, IDC_THIEF_SKILL_EDIT_5, m_szThiefSkillModifier[4]);
	DDX_Text(pDX, IDC_THIEF_SKILL_EDIT_6, m_szThiefSkillModifier[5]);
	DDX_Text(pDX, IDC_THIEF_SKILL_EDIT_7, m_szThiefSkillModifier[6]);
	DDX_Text(pDX, IDC_THIEF_SKILL_EDIT_8, m_szThiefSkillModifier[7]);
}


BEGIN_MESSAGE_MAP(cDMModifyThiefSkillsDialog, CDialog)
	ON_BN_CLICKED(IDOK, &cDMModifyThiefSkillsDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &cDMModifyThiefSkillsDialog::OnBnClickedCancel)
	ON_EN_SETFOCUS(IDC_THIEF_SKILL_EDIT_1, &cDMModifyThiefSkillsDialog::OnEnSetfocusThiefSkillEdit1)
	ON_EN_SETFOCUS(IDC_THIEF_SKILL_EDIT_2, &cDMModifyThiefSkillsDialog::OnEnSetfocusThiefSkillEdit2)
	ON_EN_SETFOCUS(IDC_THIEF_SKILL_EDIT_3, &cDMModifyThiefSkillsDialog::OnEnSetfocusThiefSkillEdit3)
	ON_EN_SETFOCUS(IDC_THIEF_SKILL_EDIT_4, &cDMModifyThiefSkillsDialog::OnEnSetfocusThiefSkillEdit4)
	ON_EN_SETFOCUS(IDC_THIEF_SKILL_EDIT_5, &cDMModifyThiefSkillsDialog::OnEnSetfocusThiefSkillEdit5)
	ON_EN_SETFOCUS(IDC_THIEF_SKILL_EDIT_6, &cDMModifyThiefSkillsDialog::OnEnSetfocusThiefSkillEdit6)
	ON_EN_SETFOCUS(IDC_THIEF_SKILL_EDIT_7, &cDMModifyThiefSkillsDialog::OnEnSetfocusThiefSkillEdit7)
	ON_EN_SETFOCUS(IDC_THIEF_SKILL_EDIT_8, &cDMModifyThiefSkillsDialog::OnEnSetfocusThiefSkillEdit8)
END_MESSAGE_MAP()


// cDMModifyThiefSkillsDialog message handlers


BOOL cDMModifyThiefSkillsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	for (int i = 0; i < 8; ++i)
	{
		if (m_nThiefSkillModifiers[i] >= 0)
		{
			m_szThiefSkillModifier[i].Format("+%d", m_nThiefSkillModifiers[i]);
		}
		else
		{
			m_szThiefSkillModifier[i].Format("%d", m_nThiefSkillModifiers[i]);
		}
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void cDMModifyThiefSkillsDialog::Refresh()
{
	UpdateData(TRUE);

	for (int i = 0; i < 8; ++i)
	{
		m_nThiefSkillModifiers[i] = atoi(m_szThiefSkillModifier[i].GetBuffer(0));

		if (m_nThiefSkillModifiers[i] >= 0)
		{
			m_szThiefSkillModifier[i].Format("+%d", m_nThiefSkillModifiers[i]);
		}
		else
		{
			m_szThiefSkillModifier[i].Format("%d", m_nThiefSkillModifiers[i]);
		}
	}

	UpdateData(FALSE);
}

void cDMModifyThiefSkillsDialog::OnBnClickedOk()
{
	Refresh();
	memcpy(m_pnThiefSkillModifiers, m_nThiefSkillModifiers, 8 * sizeof(int));
	CDialog::OnOK();
}



void cDMModifyThiefSkillsDialog::OnBnClickedCancel()
{
	CDialog::OnCancel();
}


void cDMModifyThiefSkillsDialog::OnEnSetfocusThiefSkillEdit1()
{
	Refresh();
}
void cDMModifyThiefSkillsDialog::OnEnSetfocusThiefSkillEdit2()
{
	Refresh();
}
void cDMModifyThiefSkillsDialog::OnEnSetfocusThiefSkillEdit3()
{
	Refresh();
}
void cDMModifyThiefSkillsDialog::OnEnSetfocusThiefSkillEdit4()
{
	Refresh();
}
void cDMModifyThiefSkillsDialog::OnEnSetfocusThiefSkillEdit5()
{
	Refresh();
}
void cDMModifyThiefSkillsDialog::OnEnSetfocusThiefSkillEdit6()
{
	Refresh();
}
void cDMModifyThiefSkillsDialog::OnEnSetfocusThiefSkillEdit7()
{
	Refresh();
}
void cDMModifyThiefSkillsDialog::OnEnSetfocusThiefSkillEdit8()
{
	Refresh();
}


