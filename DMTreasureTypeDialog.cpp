// DMTreasureTypeDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMTreasureTypeDialog.h"
#include "cDMChartLookupDialog.h"


// cDMTreasureTypeDialog dialog

IMPLEMENT_DYNAMIC(cDMTreasureTypeDialog, CDialog)

cDMTreasureTypeDialog::cDMTreasureTypeDialog(CString *pszTreasureType, BOOL *pbInLair, CWnd* pParent /*=NULL*/)
	: CDialog(cDMTreasureTypeDialog::IDD, pParent)
	, m_szTTMul(_T("1"))
	, m_bInLair(FALSE)
{
	m_pszTreasureType = pszTreasureType;
	m_pbInLair = pbInLair;

	m_bInLair = *m_pbInLair;
}

cDMTreasureTypeDialog::~cDMTreasureTypeDialog()
{
}

void cDMTreasureTypeDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREASURE_TYPE_LIST, m_cTreasureTypeList);
	DDX_Text(pDX, IDC_TT_MUL_EDIT, m_szTTMul);
	DDV_MaxChars(pDX, m_szTTMul, 1);
	DDX_Check(pDX, IDC_IN_LAIR_CHECK, m_bInLair);
}


BEGIN_MESSAGE_MAP(cDMTreasureTypeDialog, CDialog)
	ON_BN_CLICKED(IDCANCEL, &cDMTreasureTypeDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &cDMTreasureTypeDialog::OnBnClickedOk)
	ON_LBN_DBLCLK(IDC_TREASURE_TYPE_LIST, &cDMTreasureTypeDialog::OnLbnDblclkTreasureTypeList)
	ON_BN_CLICKED(IDC_IN_LAIR_CHECK, &cDMTreasureTypeDialog::OnBnClickedInLairCheck)
END_MESSAGE_MAP()


// cDMTreasureTypeDialog message handlers

BOOL cDMTreasureTypeDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString szTemp;

	int nCount = 0;
	for(int i = 'A'; i <= 'Z'; ++i)
	{
		szTemp.Format("%c", i);
		m_cTreasureTypeList.InsertString(nCount++, szTemp);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void cDMTreasureTypeDialog::OnBnClickedCancel()
{
	
	OnCancel();
}

void cDMTreasureTypeDialog::OnBnClickedOk()
{
	UpdateData(TRUE);


	int nCursor = m_cTreasureTypeList.GetCurSel();

	if(nCursor >= 0)
	{
		CString szTemp;
		m_cTreasureTypeList.GetText(nCursor, szTemp);

		m_pszTreasureType->Format("%sx%s", szTemp, m_szTTMul);
	}

	*m_pbInLair = m_bInLair;

	OnOK();
}


void cDMTreasureTypeDialog::OnLbnDblclkTreasureTypeList()
{
	cDMChartLookupDialog *pDlg = new cDMChartLookupDialog(DND_CHART_TREASURE_TYPES);
	pDlg->DoModal();
	delete pDlg;
}


void cDMTreasureTypeDialog::OnBnClickedInLairCheck()
{
	UpdateData(TRUE);
}
