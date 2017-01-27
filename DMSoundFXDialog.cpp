// DMSoundFXDialog.cpp : implementation file
//

#include "stdafx.h"
#include <mmsystem.h>

#include "DM Helper.h"
#include "DMSoundFXDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDMSoundFXDialog dialog


CDMSoundFXDialog::CDMSoundFXDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDMSoundFXDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDMSoundFXDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	Create(CDMSoundFXDialog::IDD, pParent);
}


void CDMSoundFXDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDMSoundFXDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDMSoundFXDialog, CDialog)
	//{{AFX_MSG_MAP(CDMSoundFXDialog)
	ON_BN_CLICKED(IDC_SOUND_BUTTON1, OnSoundButton1)
	ON_BN_CLICKED(IDC_SOUND_BUTTON2, OnSoundButton2)
	ON_BN_CLICKED(IDC_SOUND_BUTTON3, OnSoundButton3)
	ON_BN_CLICKED(IDC_SOUND_BUTTON4, OnSoundButton4)
	ON_BN_CLICKED(IDC_SOUND_BUTTON5, OnSoundButton5)
	ON_BN_CLICKED(IDC_SOUND_BUTTON6, OnSoundButton6)
	ON_BN_CLICKED(IDC_SOUND_BUTTON7, OnSoundButton7)
	ON_BN_CLICKED(IDC_SOUND_BUTTON8, OnSoundButton8)
	ON_BN_CLICKED(IDC_SOUND_BUTTON9, OnSoundButton9)
	ON_BN_CLICKED(IDC_SOUND_BUTTON10, OnSoundButton10)
	ON_BN_CLICKED(IDC_SOUND_BUTTON11, OnSoundButton11)
	ON_BN_CLICKED(IDC_SOUND_BUTTON12, OnSoundButton12)
	ON_BN_CLICKED(IDC_SOUND_BUTTON13, OnSoundButton13)
	ON_BN_CLICKED(IDC_SOUND_BUTTON14, OnSoundButton14)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDMSoundFXDialog message handlers


BOOL CDMSoundFXDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	ShowWindow(SW_SHOW);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDMSoundFXDialog::OnSoundButton1() 
{
	// TODO: Add your control notification handler code here
	
	PlaySound ((LPCSTR) IDW_SQUEAK_DOOR, AfxGetInstanceHandle() , SND_ASYNC | SND_RESOURCE);
}

void CDMSoundFXDialog::OnSoundButton2() 
{
	// TODO: Add your control notification handler code here
	
	PlaySound ((LPCSTR) IDW_SMASHING_DOOR, AfxGetInstanceHandle() , SND_ASYNC | SND_RESOURCE);
}

void CDMSoundFXDialog::OnSoundButton3() 
{
	// TODO: Add your control notification handler code here
	
	PlaySound ((LPCSTR) IDW_GLASS_BREAK, AfxGetInstanceHandle() , SND_ASYNC | SND_RESOURCE);
}

void CDMSoundFXDialog::OnSoundButton4() 
{
	// TODO: Add your control notification handler code here

	PlaySound ((LPCSTR) IDW_GHOST, AfxGetInstanceHandle() , SND_ASYNC | SND_RESOURCE);
	
}

void CDMSoundFXDialog::OnSoundButton5() 
{
	// TODO: Add your control notification handler code here

	PlaySound ((LPCSTR) IDW_ARROW_THUMP, AfxGetInstanceHandle() , SND_ASYNC | SND_RESOURCE);
}

void CDMSoundFXDialog::OnSoundButton6() 
{
	// TODO: Add your control notification handler code here
	
	PlaySound ((LPCSTR) IDW_SWORD_UNSHEATHE, AfxGetInstanceHandle() , SND_ASYNC | SND_RESOURCE);
}

void CDMSoundFXDialog::OnSoundButton7() 
{
	// TODO: Add your control notification handler code here
	
	PlaySound ((LPCSTR) IDW_SWORD_FIGHT, AfxGetInstanceHandle() , SND_ASYNC | SND_RESOURCE);
}

void CDMSoundFXDialog::OnSoundButton8() 
{
	// TODO: Add your control notification handler code here
	PlaySound ((LPCSTR) IDW_MALE_SCREAM, AfxGetInstanceHandle() , SND_ASYNC | SND_RESOURCE);
}

void CDMSoundFXDialog::OnSoundButton9() 
{
	// TODO: Add your control notification handler code here
	PlaySound ((LPCSTR) IDW_FEMALE_SCREAM, AfxGetInstanceHandle() , SND_ASYNC | SND_RESOURCE);
}

void CDMSoundFXDialog::OnSoundButton10() 
{
	// TODO: Add your control notification handler code here
	
	PlaySound ((LPCSTR) IDW_FIREBALL, AfxGetInstanceHandle() , SND_ASYNC | SND_RESOURCE);
}

void CDMSoundFXDialog::OnSoundButton11() 
{
	// TODO: Add your control notification handler code here

	PlaySound ((LPCSTR) IDW_LIGHTNING, AfxGetInstanceHandle() , SND_ASYNC | SND_RESOURCE);
}

void CDMSoundFXDialog::OnSoundButton12() 
{
	// TODO: Add your control notification handler code here
	
	PlaySound ((LPCSTR) IDW_MAGIC_WAND, AfxGetInstanceHandle() , SND_ASYNC | SND_RESOURCE);
}

void CDMSoundFXDialog::OnSoundButton13() 
{
	// TODO: Add your control notification handler code here
	
	PlaySound ((LPCSTR) IDW_CHANT, AfxGetInstanceHandle() , SND_ASYNC | SND_RESOURCE);
}

void CDMSoundFXDialog::OnSoundButton14() 
{
	// TODO: Add your control notification handler code here
	
	PlaySound ((LPCSTR) IDW_CHOIR, AfxGetInstanceHandle() , SND_ASYNC | SND_RESOURCE);
}
