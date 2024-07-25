// cDMModifyClericSkillsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "cDMModifyClericSkillsDialog.h"
#include "afxdialogex.h"


// cDMModifyClericSkillsDialog dialog

IMPLEMENT_DYNAMIC(cDMModifyClericSkillsDialog, CDialog)

cDMModifyClericSkillsDialog::cDMModifyClericSkillsDialog(char * szCharacterName, int *pnClericTurnModifiers, BOOL bModify, CWnd* pParent /*=NULL*/)
	: CDialog(cDMModifyClericSkillsDialog::IDD, pParent)
{
	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_szCharacterName = szCharacterName;

	m_pnClericTurnModifiers = pnClericTurnModifiers;

	m_bModify = bModify;

	memcpy(m_nClericTurnModifiers, m_pnClericTurnModifiers, 13 * sizeof(int));
}

cDMModifyClericSkillsDialog::~cDMModifyClericSkillsDialog()
{
}

void cDMModifyClericSkillsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TURN_UNDEAD_EDIT_1, m_szTurnUndeadEdit[0]);
	DDX_Text(pDX, IDC_TURN_UNDEAD_EDIT_2, m_szTurnUndeadEdit[1]);
	DDX_Text(pDX, IDC_TURN_UNDEAD_EDIT_3, m_szTurnUndeadEdit[2]);
	DDX_Text(pDX, IDC_TURN_UNDEAD_EDIT_4, m_szTurnUndeadEdit[3]);
	DDX_Text(pDX, IDC_TURN_UNDEAD_EDIT_5, m_szTurnUndeadEdit[4]);
	DDX_Text(pDX, IDC_TURN_UNDEAD_EDIT_6, m_szTurnUndeadEdit[5]);
	DDX_Text(pDX, IDC_TURN_UNDEAD_EDIT_7, m_szTurnUndeadEdit[6]);
	DDX_Text(pDX, IDC_TURN_UNDEAD_EDIT_8, m_szTurnUndeadEdit[7]);
	DDX_Text(pDX, IDC_TURN_UNDEAD_EDIT_9, m_szTurnUndeadEdit[8]);
	DDX_Text(pDX, IDC_TURN_UNDEAD_EDIT_10, m_szTurnUndeadEdit[9]);
	DDX_Text(pDX, IDC_TURN_UNDEAD_EDIT_11, m_szTurnUndeadEdit[10]);
	DDX_Text(pDX, IDC_TURN_UNDEAD_EDIT_12, m_szTurnUndeadEdit[11]);
	DDX_Text(pDX, IDC_TURN_UNDEAD_EDIT_13, m_szTurnUndeadEdit[12]);
	DDX_Control(pDX, IDCANCEL, m_cCancelButton);
	DDX_Control(pDX, IDOK, m_cOKButton);
}


BEGIN_MESSAGE_MAP(cDMModifyClericSkillsDialog, CDialog)
	ON_BN_CLICKED(IDOK, &cDMModifyClericSkillsDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &cDMModifyClericSkillsDialog::OnBnClickedCancel)
	ON_EN_SETFOCUS(IDC_TURN_UNDEAD_EDIT_1, &cDMModifyClericSkillsDialog::OnEnSetfocusTurnUndeadEdit)
	ON_EN_SETFOCUS(IDC_TURN_UNDEAD_EDIT_2, &cDMModifyClericSkillsDialog::OnEnSetfocusTurnUndeadEdit)
	ON_EN_SETFOCUS(IDC_TURN_UNDEAD_EDIT_3, &cDMModifyClericSkillsDialog::OnEnSetfocusTurnUndeadEdit)
	ON_EN_SETFOCUS(IDC_TURN_UNDEAD_EDIT_4, &cDMModifyClericSkillsDialog::OnEnSetfocusTurnUndeadEdit)
	ON_EN_SETFOCUS(IDC_TURN_UNDEAD_EDIT_5, &cDMModifyClericSkillsDialog::OnEnSetfocusTurnUndeadEdit)
	ON_EN_SETFOCUS(IDC_TURN_UNDEAD_EDIT_6, &cDMModifyClericSkillsDialog::OnEnSetfocusTurnUndeadEdit)
	ON_EN_SETFOCUS(IDC_TURN_UNDEAD_EDIT_7, &cDMModifyClericSkillsDialog::OnEnSetfocusTurnUndeadEdit)
	ON_EN_SETFOCUS(IDC_TURN_UNDEAD_EDIT_8, &cDMModifyClericSkillsDialog::OnEnSetfocusTurnUndeadEdit)
	ON_EN_SETFOCUS(IDC_TURN_UNDEAD_EDIT_9, &cDMModifyClericSkillsDialog::OnEnSetfocusTurnUndeadEdit)
	ON_EN_SETFOCUS(IDC_TURN_UNDEAD_EDIT_10, &cDMModifyClericSkillsDialog::OnEnSetfocusTurnUndeadEdit)
	ON_EN_SETFOCUS(IDC_TURN_UNDEAD_EDIT_11, &cDMModifyClericSkillsDialog::OnEnSetfocusTurnUndeadEdit)
	ON_EN_SETFOCUS(IDC_TURN_UNDEAD_EDIT_12, &cDMModifyClericSkillsDialog::OnEnSetfocusTurnUndeadEdit)
	ON_EN_SETFOCUS(IDC_TURN_UNDEAD_EDIT_13, &cDMModifyClericSkillsDialog::OnEnSetfocusTurnUndeadEdit)
END_MESSAGE_MAP()


// cDMModifyClericSkillsDialog message handlers


BOOL cDMModifyClericSkillsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	int nCtrls[] = {
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
		IDC_TURN_UNDEAD_EDIT_13 };


	if (m_bModify == FALSE)
	{
		SetWindowText("Turn Undead");
		m_cOKButton.SetWindowText("SUCCESS !");
		m_cCancelButton.SetWindowText("FAILED");

		(GetDlgItem(IDC_PLUS_MINUS))->ShowWindow(SW_HIDE);

		for (int i = 0; i < 13; ++i)
		{
			(GetDlgItem(nCtrls[i]))->EnableWindow(FALSE);

			switch (m_nClericTurnModifiers[i])
			{
				case -2: m_szTurnUndeadEdit[i] = "dmn"; break;
				case -1: m_szTurnUndeadEdit[i] = "turn"; break;
				case 99: m_szTurnUndeadEdit[i] = "-"; break;
				default: m_szTurnUndeadEdit[i].Format("%d", m_nClericTurnModifiers[i]); break;
			}
		}
		
	}
	else
	{
		for (int i = 0; i < 13; ++i)
		{
			if (m_nClericTurnModifiers[i] >= 0)
			{
				m_szTurnUndeadEdit[i].Format("+%d", m_nClericTurnModifiers[i]);
			}
			else
			{
				m_szTurnUndeadEdit[i].Format("%d", m_nClericTurnModifiers[i]);
			}
		}
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void cDMModifyClericSkillsDialog::Refresh()
{
	UpdateData(TRUE);

	for (int i = 0; i < 13; ++i)
	{
		m_nClericTurnModifiers[i] = atoi(m_szTurnUndeadEdit[i].GetBuffer(0));

		if (m_nClericTurnModifiers[i] >= 0)
		{
			m_szTurnUndeadEdit[i].Format("+%d", m_nClericTurnModifiers[i]);
		}
		else
		{
			m_szTurnUndeadEdit[i].Format("%d", m_nClericTurnModifiers[i]);
		}
	}

	UpdateData(FALSE);
}

void cDMModifyClericSkillsDialog::OnBnClickedOk()
{
	if (m_bModify)
	{
		Refresh();
		memcpy(m_pnClericTurnModifiers, m_nClericTurnModifiers, 13 * sizeof(int));
	}
	else
	{
		if (!m_pApp->PlayPCSoundFX("* PC Cast Spell", m_szCharacterName, "NADA", FALSE, 1)) // "1" is the spell identifier for whatever the first spell is - so the sound is consistent
		{
			m_pApp->m_szLoggedError.Format("WARN - SFX (%s PC Cast Spell) NOT FOUND", m_szCharacterName);
		}
		if (!m_pApp->PlaySoundFX("TURN UNDEAD"))
		{
			m_pApp->m_szLoggedError.Format("WARN - SFX (TURN UNDEAD) NOT FOUND");
		}
	}

	CDialog::OnOK();
}


void cDMModifyClericSkillsDialog::OnBnClickedCancel()
{
	if (!m_bModify)
	{
		if (!m_pApp->PlayPCSoundFX("* PC Cast Spell", m_szCharacterName, "NADA", FALSE, 1))
		{
			m_pApp->m_szLoggedError.Format("WARN - SFX (%s PC Cast Spell) NOT FOUND", m_szCharacterName);
		}
		if (!m_pApp->PlaySoundFX("SPELL FAILURE"))
		{
			m_pApp->m_szLoggedError.Format("WARN - SFX (SPELL FAILURE) NOT FOUND");
		}
	}

	CDialog::OnCancel();
}

void cDMModifyClericSkillsDialog::OnEnSetfocusTurnUndeadEdit()
{
	Refresh();
}
