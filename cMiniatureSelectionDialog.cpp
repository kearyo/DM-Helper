// cMiniatureSelectionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "cMiniatureSelectionDialog.h"
#include "afxdialogex.h"


// cMiniatureSelectionDialog dialog

IMPLEMENT_DYNAMIC(cMiniatureSelectionDialog, CDialog)

cMiniatureSelectionDialog::cMiniatureSelectionDialog(char *szNameBuffer, CWnd* pParent /*=NULL*/)
	: CDialog(cMiniatureSelectionDialog::IDD, pParent)
{
	m_pApp = (CDMHelperApp *)AfxGetApp();
	m_szNameBuffer = szNameBuffer;
}

cMiniatureSelectionDialog::~cMiniatureSelectionDialog()
{
}

void cMiniatureSelectionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MINI_SELECTION_LIST, m_cMiniSelectList);
}


BEGIN_MESSAGE_MAP(cMiniatureSelectionDialog, CDialog)
	ON_BN_CLICKED(IDOK, &cMiniatureSelectionDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &cMiniatureSelectionDialog::OnBnClickedCancel)
	ON_LBN_SELCHANGE(IDC_MINI_SELECTION_LIST, &cMiniatureSelectionDialog::OnLbnSelchangeMiniSelectionList)
	ON_LBN_DBLCLK(IDC_MINI_SELECTION_LIST, &cMiniatureSelectionDialog::OnLbnDblclkMiniSelectionList)
END_MESSAGE_MAP()


// cMiniatureSelectionDialog message handlers

BOOL cMiniatureSelectionDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	#if GAMETABLE_BUILD
	CString szSelected = m_szNameBuffer;
	int nIndex = 0;
	m_cMiniSelectList.InsertString(nIndex++, "None");
	for (auto it = m_pApp->m_MiniUpdateMap.begin(); it != m_pApp->m_MiniUpdateMap.end(); it++)
	{
		pMiniUpdatePtr pMini = it->second;
		m_cMiniSelectList.InsertString(nIndex++, pMini->m_szMiniName);

		if (m_szNameBuffer == pMini->m_szMiniName)
		{
			m_cMiniSelectList.SetCurSel(nIndex-1);
		}
	}
	#endif

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void cMiniatureSelectionDialog::OnBnClickedOk()
{
	UpdateData(TRUE);

	#if GAMETABLE_BUILD
	int nPos = m_cMiniSelectList.GetCurSel();

	if (nPos > -1)
	{
		CString szMiniSelected;
		m_cMiniSelectList.GetText(nPos, szMiniSelected);
		strcpy(m_szNameBuffer, szMiniSelected);
	}
	#endif
	CDialog::OnOK();
}


void cMiniatureSelectionDialog::OnBnClickedCancel()
{
	CDialog::OnCancel();
}



void cMiniatureSelectionDialog::OnLbnSelchangeMiniSelectionList()
{
	
}


void cMiniatureSelectionDialog::OnLbnDblclkMiniSelectionList()
{
	OnBnClickedOk();
}
