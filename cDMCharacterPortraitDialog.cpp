// cDMCharacterPortraitDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "cDMCharacterPortraitDialog.h"
#include "afxdialogex.h"


// cDMCharacterPortraitDialog dialog

IMPLEMENT_DYNAMIC(cDMCharacterPortraitDialog, CDialogEx)

cDMCharacterPortraitDialog::cDMCharacterPortraitDialog(Bitmap* pPortraitBitmap, CWnd* pParent /*=NULL*/)
	: CDialogEx(cDMCharacterPortraitDialog::IDD, pParent)
{
	m_pPortraitBitmap = pPortraitBitmap;
}

cDMCharacterPortraitDialog::~cDMCharacterPortraitDialog()
{
}

void cDMCharacterPortraitDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(cDMCharacterPortraitDialog, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// cDMCharacterPortraitDialog message handlers


void cDMCharacterPortraitDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	Graphics graphics(dc);

	if (m_pPortraitBitmap != NULL)
	{
		try
		{
			CRect rect;
			GetClientRect(&rect);

			graphics.DrawImage(m_pPortraitBitmap, 0, 0, rect.right, rect.bottom);
		}
		catch (...)
		{
			
		}
	}

}
