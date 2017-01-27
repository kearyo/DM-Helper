// DMDungeonMonsterDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMDungeonMonsterDialog.h"


// CDMDungeonMonsterDialog dialog

IMPLEMENT_DYNAMIC(CDMDungeonMonsterDialog, CDialog)

CDMDungeonMonsterDialog::CDMDungeonMonsterDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDMDungeonMonsterDialog::IDD, pParent)
{

}

CDMDungeonMonsterDialog::~CDMDungeonMonsterDialog()
{
}

void CDMDungeonMonsterDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDMDungeonMonsterDialog, CDialog)
END_MESSAGE_MAP()


// CDMDungeonMonsterDialog message handlers
