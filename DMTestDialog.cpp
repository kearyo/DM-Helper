// DMTestDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMTestDialog.h"
#include "afxdialogex.h"


// CDMTestDialog dialog

IMPLEMENT_DYNAMIC(CDMTestDialog, CDialog)

CDMTestDialog::CDMTestDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDMTestDialog::IDD, pParent)
{

}

CDMTestDialog::~CDMTestDialog()
{
}

void CDMTestDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDMTestDialog, CDialog)
END_MESSAGE_MAP()


// CDMTestDialog message handlers
