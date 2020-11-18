// cDMOpenLockDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "cDMOpenLockDialog.h"
#include "afxdialogex.h"


// cDMOpenLockDialog dialog

IMPLEMENT_DYNAMIC(cDMOpenLockDialog, CDialogEx)

cDMOpenLockDialog::cDMOpenLockDialog(cDNDParty *pParty, cDNDCharacter	*pCharacter, CWnd* pParent /*=NULL*/)
	: CDialogEx(cDMOpenLockDialog::IDD, pParent)
	, m_szTimeComment(_T(""))
{
	m_pApp = (CDMHelperApp *)AfxGetApp();
	m_pParty = pParty;
	m_pCharacter = pCharacter;
	m_nAttemptRounds = 0;
}

cDMOpenLockDialog::~cDMOpenLockDialog()
{
}

void cDMOpenLockDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COMMENT, m_szTimeComment);
}


BEGIN_MESSAGE_MAP(cDMOpenLockDialog, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &cDMOpenLockDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &cDMOpenLockDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDFAILED, &cDMOpenLockDialog::OnBnClickedFailed)
END_MESSAGE_MAP()


// cDMOpenLockDialog message handlers

BOOL cDMOpenLockDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_nAttemptRounds = RollD4();

	m_szTimeComment.Format("Attempt requires %d rounds", m_nAttemptRounds);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void cDMOpenLockDialog::OnBnClickedCancel()
{
	CDialogEx::OnCancel();
}

void cDMOpenLockDialog::OnBnClickedFailed()
{
	m_pCharacter->m_lEarnedXP += 10 * m_nAttemptRounds;

	m_pApp->PlaySoundFX("PICK LOCK FAIL");

	m_pParty->ModifyCalendar(DND_CALENDAR_MOD_ROUND, DND_CALENDAR_MOD_INCREMENT, m_nAttemptRounds);

	m_pCharacter->MarkChanged();

	CDialogEx::OnCancel();
}

void cDMOpenLockDialog::OnBnClickedOk()
{
	m_pCharacter->m_lEarnedXP += 50 * m_nAttemptRounds;

	m_pApp->PlaySoundFX("PICK LOCK SUCCESS");

	m_pParty->ModifyCalendar(DND_CALENDAR_MOD_ROUND, DND_CALENDAR_MOD_INCREMENT, m_nAttemptRounds);

	m_pCharacter->MarkChanged();

	CDialogEx::OnOK();
}




