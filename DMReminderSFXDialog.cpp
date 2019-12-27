// DMReminderSFXDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMReminderSFXDialog.h"
#include "afxdialogex.h"


// CDMReminderSFXDialog dialog

IMPLEMENT_DYNAMIC(CDMReminderSFXDialog, CDialog)

CDMReminderSFXDialog::CDMReminderSFXDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDMReminderSFXDialog::IDD, pParent)
{
	m_pApp = (CDMHelperApp *)AfxGetApp();
	m_pParent = pParent;
	
	Create(CDMReminderSFXDialog::IDD, pParent);
}

CDMReminderSFXDialog::~CDMReminderSFXDialog()
{
}

void CDMReminderSFXDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDMReminderSFXDialog, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CDMReminderSFXDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDMReminderSFXDialog::OnBnClickedOk)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDMReminderSFXDialog message handlers
BOOL CDMReminderSFXDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect _ParentRect;
	m_pParent->GetWindowRect(&_ParentRect);

	int nParentSizeX = _ParentRect.right - _ParentRect.left;
	int nParentSizeY = _ParentRect.bottom - _ParentRect.top;

	CRect _ChildRect;
	GetWindowRect(&_ChildRect);

	int nChildSizeX = _ChildRect.right - _ChildRect.left;
	int nChildSizeY = _ChildRect.bottom - _ChildRect.top;

	int nPosX = _ParentRect.left + (nParentSizeX / 2) - nChildSizeX / 2;
	int nPosY = _ParentRect.top + (nParentSizeY / 2) - nChildSizeY / 2;

	MoveWindow(nPosX, nPosY, nChildSizeX, nChildSizeY, TRUE);

	ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDMReminderSFXDialog::OnBnClickedCancel()
{
	m_pApp->m_pDMReminderSFXDialog = NULL;

	if (m_pApp->m_pInstantMapSFXPlacer != NULL)
	{
		m_pApp->m_pInstantMapSFXPlacer->m_bMapClicked = TRUE;
	}

	CDialog::OnCancel();

	delete this;
}


void CDMReminderSFXDialog::OnBnClickedOk()
{
	m_pApp->m_pDMReminderSFXDialog = NULL;
	CDialog::OnOK();
}


void CDMReminderSFXDialog::OnClose()
{
	m_pApp->m_pDMReminderSFXDialog = NULL;
	CDialog::OnClose();
}


void CDMReminderSFXDialog::PostNcDestroy()
{
	m_pApp->m_pDMReminderSFXDialog = NULL;
	CDialog::PostNcDestroy();

	delete this;
}
