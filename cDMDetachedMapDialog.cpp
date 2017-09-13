// cDMDetachedMapDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "cDMDetachedMapDialog.h"
#include "afxdialogex.h"


// cDMDetachedMapDialog dialog

IMPLEMENT_DYNAMIC(cDMDetachedMapDialog, CDialog)

cDMDetachedMapDialog::cDMDetachedMapDialog(CWnd* pParent /*=NULL*/)
	: CDialog(cDMDetachedMapDialog::IDD, pParent)
{

}

cDMDetachedMapDialog::~cDMDetachedMapDialog()
{
}

void cDMDetachedMapDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(cDMDetachedMapDialog, CDialog)
END_MESSAGE_MAP()


// cDMDetachedMapDialog message handlers
