// DMSubPartyDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DM HelperDlg.h"
#include "DMCharSpellsDialog.h"
#include "DMInventoryDialog.h"
#include "DMCharViewDialog.h"
#include "DMNPCViewDialog.h"
#include "DMPartyDialog.h"
#include "DMSubPartyDialog.h"


// CDMSubPartyDialog dialog

IMPLEMENT_DYNAMIC(CDMSubPartyDialog, CDialog)

CDMSubPartyDialog::CDMSubPartyDialog(cDNDSubParty *pSubParty, CWnd* pParent /*=NULL*/)
	: CDialog(CDMSubPartyDialog::IDD, pParent)
{
	m_pSubParty = pSubParty;
	m_pParentPartyDialog = (DMPartyDialog *)pParent;

	m_dwSubPartyID = m_pSubParty->m_dwSubPartyID;

	Create(CDMSubPartyDialog::IDD, pParent);
}

CDMSubPartyDialog::~CDMSubPartyDialog()
{
}

void CDMSubPartyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDMSubPartyDialog, CDialog)
END_MESSAGE_MAP()


// CDMSubPartyDialog message handlers

BOOL CDMSubPartyDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowText(m_pSubParty->m_szSubPartyName);

	ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDMSubPartyDialog::SyncParent()
{
	ShowWindow(SW_HIDE);
	SetWindowText(m_pSubParty->m_szSubPartyName);
	m_pParentPartyDialog->ShowWindow(SW_SHOW);
}

void CDMSubPartyDialog::Initialize() 
{
	SyncParent();
	m_pParentPartyDialog->Initialize();
}

void CDMSubPartyDialog::UpdateSelections() 
{
	SyncParent();
	m_pParentPartyDialog->UpdateSelections();
}

void CDMSubPartyDialog::Refresh() 
{
	SyncParent();
	m_pParentPartyDialog->Refresh();
}