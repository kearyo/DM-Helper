// DMInputDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMInputDialog.h"


// CDMInputDialog dialog

IMPLEMENT_DYNAMIC(CDMInputDialog, CDialog)

CDMInputDialog::CDMInputDialog(char *pszPrompt, CString *pszRetVal, CWnd* pParent /*=NULL*/)
	: CDialog(CDMInputDialog::IDD, pParent)
	, m_szInputEdit(_T(""))
	, m_szPrompt(_T(""))
{

	m_szPrompt = pszPrompt;

	m_pszRetVal = pszRetVal;
}

CDMInputDialog::~CDMInputDialog()
{
}

void CDMInputDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_INPUT_EDIT, m_szInputEdit);
	DDV_MaxChars(pDX, m_szInputEdit, 64);
	DDX_Text(pDX, IDC_PROMPT_STATIC, m_szPrompt);
	DDX_Control(pDX, IDC_INPUT_EDIT, m_cInputEdit);
}


BEGIN_MESSAGE_MAP(CDMInputDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CDMInputDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDMInputDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDMInputDialog message handlers

BOOL CDMInputDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cInputEdit.SetFocus();

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDMInputDialog::OnBnClickedOk()
{
	UpdateData(TRUE);

	m_pszRetVal->Format("%s", m_szInputEdit.GetBuffer(0));

	OnOK();
}

void CDMInputDialog::OnBnClickedCancel()
{
	
	OnCancel();
}
