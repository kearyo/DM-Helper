// DMUnitTestDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DMUnitTestDialog.h"
#include "afxdialogex.h"


// CDMUnitTestDialog dialog

IMPLEMENT_DYNAMIC(CDMUnitTestDialog, CDialogEx)

CDMUnitTestDialog::CDMUnitTestDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDMUnitTestDialog::IDD, pParent)
{
	Create(CDMUnitTestDialog::IDD);
}

CDMUnitTestDialog::~CDMUnitTestDialog()
{
}

void CDMUnitTestDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDMUnitTestDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDMUnitTestDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CDMUnitTestDialog message handlers


void CDMUnitTestDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}
