// cMusicMoodDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "cMusicMoodDialog.h"
#include "afxdialogex.h"


// cMusicMoodDialog dialog

IMPLEMENT_DYNAMIC(cMusicMoodDialog, CDialog)

cMusicMoodDialog::cMusicMoodDialog(CWnd* pParent /*=NULL*/)
	: CDialog(cMusicMoodDialog::IDD, pParent)
{
	m_pApp = (CDMHelperApp *)AfxGetApp();
}

cMusicMoodDialog::~cMusicMoodDialog()
{
}

void cMusicMoodDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(cMusicMoodDialog, CDialog)
	ON_BN_CLICKED(IDCANCEL, &cMusicMoodDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &cMusicMoodDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_MUSIC_MOOD_BUTTON1, &cMusicMoodDialog::OnBnClickedMusicMoodButton1)
	ON_BN_CLICKED(IDC_MUSIC_MOOD_BUTTON2, &cMusicMoodDialog::OnBnClickedMusicMoodButton2)
	ON_BN_CLICKED(IDC_MUSIC_MOOD_BUTTON3, &cMusicMoodDialog::OnBnClickedMusicMoodButton3)
	ON_BN_CLICKED(IDC_MUSIC_MOOD_BUTTON4, &cMusicMoodDialog::OnBnClickedMusicMoodButton4)
	ON_BN_CLICKED(IDC_MUSIC_MOOD_BUTTON5, &cMusicMoodDialog::OnBnClickedMusicMoodButton5)
END_MESSAGE_MAP()


// cMusicMoodDialog message handlers


void cMusicMoodDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}


void cMusicMoodDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();
}


void cMusicMoodDialog::OnBnClickedMusicMoodButton1()
{
	m_pApp->SendRemoteMusicCommand("SETMOOD NONE");
	CDialog::OnOK();
}


void cMusicMoodDialog::OnBnClickedMusicMoodButton2()
{
	m_pApp->SendRemoteMusicCommand("SETMOOD COMBAT");
	CDialog::OnOK();
}


void cMusicMoodDialog::OnBnClickedMusicMoodButton3()
{
	m_pApp->SendRemoteMusicCommand("SETMOOD SOMBER");
	CDialog::OnOK();
}


void cMusicMoodDialog::OnBnClickedMusicMoodButton4()
{
	m_pApp->SendRemoteMusicCommand("SETMOOD TAVERN");
	CDialog::OnOK();
}


void cMusicMoodDialog::OnBnClickedMusicMoodButton5()
{
	m_pApp->SendRemoteMusicCommand("SETMOOD SPOOKY");
	CDialog::OnOK();
}
