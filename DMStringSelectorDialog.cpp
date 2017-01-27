// DMStringSelectorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMStringSelectorDialog.h"


// CDMStringSelectorDialog dialog

IMPLEMENT_DYNAMIC(CDMStringSelectorDialog, CDialog)

CDMStringSelectorDialog::CDMStringSelectorDialog(char *pReturnString, int nMaxChars, CWnd* pParent /*=NULL*/)
	: CDialog(CDMStringSelectorDialog::IDD, pParent)
{
	m_pParent = pParent;

	m_pReturnString = pReturnString;
	m_nMaxChars = nMaxChars;

	Create(CDMStringSelectorDialog::IDD, pParent);
}

CDMStringSelectorDialog::~CDMStringSelectorDialog()
{
}

void CDMStringSelectorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STRING_LIST, m_cStringList);
	DDX_Control(pDX, IDOK, m_cOKButton);
}


BEGIN_MESSAGE_MAP(CDMStringSelectorDialog, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CDMStringSelectorDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDMStringSelectorDialog::OnBnClickedOk)
	ON_LBN_SELCHANGE(IDC_STRING_LIST, &CDMStringSelectorDialog::OnLbnSelchangeStringList)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDMStringSelectorDialog message handlers

void CDMStringSelectorDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CDMStringSelectorDialog::OnBnClickedOk()
{
	UpdateData(TRUE);

	int nCursor = m_cStringList.GetCurSel();

	if(nCursor >= 0)
	{
		CString szLine = _T("");
		m_cStringList.GetText(nCursor, szLine);

		sprintf(m_pReturnString, "%s", szLine.Left(m_nMaxChars));

		m_pParent->InvalidateRect(NULL);
	}

	OnOK();
}

BOOL CDMStringSelectorDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cOKButton.EnableWindow(FALSE);

	AfxGetMainWnd()->BeginModalState();

	ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDMStringSelectorDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
}


void CDMStringSelectorDialog::OnLbnSelchangeStringList()
{
	m_cOKButton.EnableWindow(TRUE);
}


void CDMStringSelectorDialog::InsertString(int nIndex, char *szString)
{
	m_cStringList.InsertString(nIndex, szString);
}
void CDMStringSelectorDialog::PostNcDestroy()
{
	CDialog::PostNcDestroy();

	delete this;
}

