// cDMMapNameDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "cDMMapNameDialog.h"
#include "afxdialogex.h"


// cDMMapNameDialog dialog

IMPLEMENT_DYNAMIC(cDMMapNameDialog, CDialog)

cDMMapNameDialog::cDMMapNameDialog(cDNDMap *pDNDMap, CWnd* pParent /*=NULL*/)
	: CDialog(cDMMapNameDialog::IDD, pParent)
	, m_szMapName(_T(""))
{
	m_pDNDMap = pDNDMap;
}

cDMMapNameDialog::~cDMMapNameDialog()
{
}

void cDMMapNameDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_cNameEdit);
	DDX_Text(pDX, IDC_EDIT1, m_szMapName);
	DDV_MaxChars(pDX, m_szMapName, 128);
}


BEGIN_MESSAGE_MAP(cDMMapNameDialog, CDialog)
	ON_BN_CLICKED(IDCANCEL, &cDMMapNameDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &cDMMapNameDialog::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT1, &cDMMapNameDialog::OnEnChangeEdit1)
END_MESSAGE_MAP()


// cDMMapNameDialog message handlers

BOOL cDMMapNameDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cNameEdit.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void cDMMapNameDialog::OnBnClickedCancel()
{
	if (m_pDNDMap->m_szMapName[0] != 0)
	{
		CDialog::OnCancel();
	}
}


void cDMMapNameDialog::OnBnClickedOk()
{
	if (m_pDNDMap->m_szMapName[0] != 0)
	{
		CDialog::OnOK();
	}
}


void cDMMapNameDialog::OnEnChangeEdit1()
{
	UpdateData(TRUE);

	strcpy(m_pDNDMap->m_szMapName, m_szMapName.Left(259));
}


