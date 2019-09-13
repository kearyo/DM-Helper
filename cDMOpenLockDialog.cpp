// cDMOpenLockDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "cDMOpenLockDialog.h"
#include "afxdialogex.h"


// cDMOpenLockDialog dialog

IMPLEMENT_DYNAMIC(cDMOpenLockDialog, CDialogEx)

cDMOpenLockDialog::cDMOpenLockDialog(cDNDCharacter	*pCharacter, CWnd* pParent /*=NULL*/)
	: CDialogEx(cDMOpenLockDialog::IDD, pParent)
{
	m_pApp = (CDMHelperApp *)AfxGetApp();
	m_pCharacter = pCharacter;
}

cDMOpenLockDialog::~cDMOpenLockDialog()
{
}

void cDMOpenLockDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(cDMOpenLockDialog, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &cDMOpenLockDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &cDMOpenLockDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// cDMOpenLockDialog message handlers


void cDMOpenLockDialog::OnBnClickedCancel()
{
	
	m_pCharacter->m_lEarnedXP += 100;

	m_pApp->PlaySoundFX("PICK LOCK FAIL");

	CDialogEx::OnCancel();
}


void cDMOpenLockDialog::OnBnClickedOk()
{
	m_pCharacter->m_lEarnedXP += 50;

	m_pApp->PlaySoundFX("PICK LOCK SUCCESS");

	CDialogEx::OnOK();
}
