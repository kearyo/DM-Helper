// cDMStrikeOrThrowDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "cDMStrikeOrThrowDialog.h"
#include "afxdialogex.h"


// cDMStrikeOrThrowDialog dialog

IMPLEMENT_DYNAMIC(cDMStrikeOrThrowDialog, CDialogEx)

cDMStrikeOrThrowDialog::cDMStrikeOrThrowDialog(BOOL *pbThowWeapon, CWnd* pParent /*=NULL*/)
	: CDialogEx(cDMStrikeOrThrowDialog::IDD, pParent)
{
	m_pbThowWeapon = pbThowWeapon;
}

cDMStrikeOrThrowDialog::~cDMStrikeOrThrowDialog()
{
}

void cDMStrikeOrThrowDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(cDMStrikeOrThrowDialog, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &cDMStrikeOrThrowDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &cDMStrikeOrThrowDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// cDMStrikeOrThrowDialog message handlers


void cDMStrikeOrThrowDialog::OnBnClickedCancel()
{
	*m_pbThowWeapon = FALSE;
	CDialogEx::OnCancel();
}


void cDMStrikeOrThrowDialog::OnBnClickedOk()
{
	*m_pbThowWeapon = TRUE;
	CDialogEx::OnOK();
}
