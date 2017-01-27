// DMTreasureTypeDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMTreasureTypeDialog.h"


// cDMTreasureTypeDialog dialog

IMPLEMENT_DYNAMIC(cDMTreasureTypeDialog, CDialog)

cDMTreasureTypeDialog::cDMTreasureTypeDialog(CString *pszTreasureType, CWnd* pParent /*=NULL*/)
	: CDialog(cDMTreasureTypeDialog::IDD, pParent)
	, m_szTTMul(_T("1"))
{
	m_pszTreasureType = pszTreasureType;
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
}


BEGIN_MESSAGE_MAP(cDMTreasureTypeDialog, CDialog)
	ON_BN_CLICKED(IDCANCEL, &cDMTreasureTypeDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &cDMTreasureTypeDialog::OnBnClickedOk)
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


	OnOK();
}
