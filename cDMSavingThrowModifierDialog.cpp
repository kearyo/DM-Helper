// cDMSavingThrowModifierDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "cDMSavingThrowModifierDialog.h"
#include "afxdialogex.h"


// cDMSavingThrowModifierDialog dialog

IMPLEMENT_DYNAMIC(cDMSavingThrowModifierDialog, CDialog)

cDMSavingThrowModifierDialog::cDMSavingThrowModifierDialog(int *pnSavingThrowModifiers, CWnd* pParent /*=NULL*/)
	: CDialog(cDMSavingThrowModifierDialog::IDD, pParent)
{
	m_szSaveMod[0] = _T("+0");
	m_szSaveMod[1] = _T("+0");
	m_szSaveMod[2] = _T("+0");
	m_szSaveMod[3] = _T("+0");
	m_szSaveMod[4] = _T("+0");

	m_pnSavingThrowModifiers = pnSavingThrowModifiers;
	memcpy(m_nSavingThrowModifiers, m_pnSavingThrowModifiers, 5 * sizeof(int));
}

cDMSavingThrowModifierDialog::~cDMSavingThrowModifierDialog()
{
}

void cDMSavingThrowModifierDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_SAVE_EDIT_1, m_szSaveMod[0]);
	DDX_Text(pDX, IDC_SAVE_EDIT_2, m_szSaveMod[1]);
	DDX_Text(pDX, IDC_SAVE_EDIT_3, m_szSaveMod[2]);
	DDX_Text(pDX, IDC_SAVE_EDIT_4, m_szSaveMod[3]);
	DDX_Text(pDX, IDC_SAVE_EDIT_5, m_szSaveMod[4]);
}


BEGIN_MESSAGE_MAP(cDMSavingThrowModifierDialog, CDialog)
	ON_BN_CLICKED(IDOK, &cDMSavingThrowModifierDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &cDMSavingThrowModifierDialog::OnBnClickedCancel)
	ON_EN_SETFOCUS(IDC_SAVE_EDIT_1, &cDMSavingThrowModifierDialog::OnEnSetfocusSaveEdit1)
	ON_EN_SETFOCUS(IDC_SAVE_EDIT_2, &cDMSavingThrowModifierDialog::OnEnSetfocusSaveEdit2)
	ON_EN_SETFOCUS(IDC_SAVE_EDIT_3, &cDMSavingThrowModifierDialog::OnEnSetfocusSaveEdit3)
	ON_EN_SETFOCUS(IDC_SAVE_EDIT_4, &cDMSavingThrowModifierDialog::OnEnSetfocusSaveEdit4)
	ON_EN_SETFOCUS(IDC_SAVE_EDIT_5, &cDMSavingThrowModifierDialog::OnEnSetfocusSaveEdit5)
END_MESSAGE_MAP()


// cDMSavingThrowModifierDialog message handlers


BOOL cDMSavingThrowModifierDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	for (int i = 0; i < 5; ++i)
	{
		if (m_nSavingThrowModifiers[i] >= 0)
		{
			m_szSaveMod[i].Format("+%d", m_nSavingThrowModifiers[i]);
		}
		else
		{
			m_szSaveMod[i].Format("%d", m_nSavingThrowModifiers[i]);
		}
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void cDMSavingThrowModifierDialog::Refresh()
{
	UpdateData(TRUE);

	for (int i = 0; i < 5; ++i)
	{
		m_nSavingThrowModifiers[i] = atoi(m_szSaveMod[i].GetBuffer(0));

		if (m_nSavingThrowModifiers[i] >= 0)
		{
			m_szSaveMod[i].Format("+%d", m_nSavingThrowModifiers[i]);
		}
		else
		{
			m_szSaveMod[i].Format("%d", m_nSavingThrowModifiers[i]);
		}
	}

	UpdateData(FALSE);
}

void cDMSavingThrowModifierDialog::OnBnClickedOk()
{
	Refresh();
	memcpy(m_pnSavingThrowModifiers, m_nSavingThrowModifiers, 5 * sizeof(int));
	CDialog::OnOK();
}


void cDMSavingThrowModifierDialog::OnBnClickedCancel()
{
	CDialog::OnCancel();
}


void cDMSavingThrowModifierDialog::OnEnSetfocusSaveEdit1()
{
	Refresh();
}

void cDMSavingThrowModifierDialog::OnEnSetfocusSaveEdit2()
{
	Refresh();
}


void cDMSavingThrowModifierDialog::OnEnSetfocusSaveEdit3()
{
	Refresh();
}


void cDMSavingThrowModifierDialog::OnEnSetfocusSaveEdit4()
{
	Refresh();
}


void cDMSavingThrowModifierDialog::OnEnSetfocusSaveEdit5()
{
	Refresh();
}



