// DMAboutDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMAboutDialog.h"
#include "DM Helper Common.h"


// CDMAboutDialog dialog

IMPLEMENT_DYNAMIC(CDMAboutDialog, CDialog)

CDMAboutDialog::CDMAboutDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDMAboutDialog::IDD, pParent)
	, m_szVersion(_T(""))
{

}

CDMAboutDialog::~CDMAboutDialog()
{
}

void CDMAboutDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_VERSION_STATIC, m_szVersion);
}


BEGIN_MESSAGE_MAP(CDMAboutDialog, CDialog)
END_MESSAGE_MAP()


// CDMAboutDialog message handlers

BOOL CDMAboutDialog::OnInitDialog()
{
	CDialog::OnInitDialog();


	LONG lValue = DMH_CURRENT_VERSION;

	//10000 
	LONG lMajor = DMH_CURRENT_VERSION / 10000L;
	lValue -= lMajor * 10000L;

	LONG lMinor = lValue / 1000L;
	lValue -= lMinor * 1000L;

	m_szVersion.Format("Version: %01d.%01d.%03d", lMajor, lMinor, lValue);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
