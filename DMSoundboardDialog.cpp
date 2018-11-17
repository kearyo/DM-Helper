// DMSoundboardDialog.cpp : implementation file
//

#include "stdafx.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include "DM Helper.h"
#include "DMSoundboardDialog.h"
#include "DMSFXEditorDialog.h"
#include "DMSFXMapperDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DMSoundboardDialog dialog


DMSoundboardDialog::DMSoundboardDialog(CWnd* pParent /*=NULL*/)
	: CDialog(DMSoundboardDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(DMSoundboardDialog)
	m_szBoardName = _T("");
	//}}AFX_DATA_INIT

	m_pParent = pParent;

	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_nPage = 0;
	m_bEditMode = FALSE;

	Create(DMSoundboardDialog::IDD, pParent);
}


void DMSoundboardDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DMSoundboardDialog)
	DDX_Control(pDX, IDC_SOUNDBOARD_TAB, m_cSoundBoardTab);
	DDX_Text(pDX, IDC_BOARDNAME_EDIT, m_szBoardName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DMSoundboardDialog, CDialog)
	//{{AFX_MSG_MAP(DMSoundboardDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_EDIT_RADIO, OnEditRadio)
	ON_BN_CLICKED(IDC_PLAY_RADIO, OnPlayRadio)
	ON_NOTIFY(TCN_SELCHANGE, IDC_SOUNDBOARD_TAB, OnSelchangeSoundboardTab)
	ON_EN_CHANGE(IDC_BOARDNAME_EDIT, OnChangeBoardnameEdit)
	ON_BN_CLICKED(IDC_MAP_WEAPON_SFX_BUTTON, OnMapWeaponSfxButton)
	ON_BN_CLICKED(IDC_MAP_SPELL_SFX_BUTTON, OnMapSpellSfxButton)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DMSoundboardDialog message handlers

BOOL DMSoundboardDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString szDirPath = m_pApp->m_szEXEPath + "data\\sounds\\random";
	CreateDirectory(szDirPath, NULL);

	SetWindowText("Soundboard");

	int nPos = 0;

	if(m_pApp->m_Settings.m_szSoundBoardTabLabels[0][0] == 0)
	{
		m_cSoundBoardTab.InsertItem(nPos++, "Combat");
		strcpy(m_pApp->m_Settings.m_szSoundBoardTabLabels[0], "Combat");
	}
	else
	{
		m_cSoundBoardTab.InsertItem(nPos++, &m_pApp->m_Settings.m_szSoundBoardTabLabels[0][0]);
	}

	if(m_pApp->m_Settings.m_szSoundBoardTabLabels[1][0] == 0)
	{
		m_cSoundBoardTab.InsertItem(nPos++, "Spells");
		strcpy(m_pApp->m_Settings.m_szSoundBoardTabLabels[1], "Spells");
	}
	else
	{
		m_cSoundBoardTab.InsertItem(nPos++, &m_pApp->m_Settings.m_szSoundBoardTabLabels[1][0]);
	}

	if(m_pApp->m_Settings.m_szSoundBoardTabLabels[2][0] == 0)
	{
		m_cSoundBoardTab.InsertItem(nPos++, "Actions");
		strcpy(m_pApp->m_Settings.m_szSoundBoardTabLabels[2], "Actions");
	}
	else
	{
		m_cSoundBoardTab.InsertItem(nPos++, &m_pApp->m_Settings.m_szSoundBoardTabLabels[2][0]);
	}

	if(m_pApp->m_Settings.m_szSoundBoardTabLabels[3][0] == 0)
	{
		m_cSoundBoardTab.InsertItem(nPos++, "Monsters");
		strcpy(m_pApp->m_Settings.m_szSoundBoardTabLabels[3], "Monsters");
	}
	else
	{
		m_cSoundBoardTab.InsertItem(nPos++, &m_pApp->m_Settings.m_szSoundBoardTabLabels[3][0]);
	}

	if(m_pApp->m_Settings.m_szSoundBoardTabLabels[4][0] == 0)
	{
		m_cSoundBoardTab.InsertItem(nPos++, "Environment");
		strcpy(m_pApp->m_Settings.m_szSoundBoardTabLabels[4], "Environment");
	}
	else
	{
		m_cSoundBoardTab.InsertItem(nPos++, &m_pApp->m_Settings.m_szSoundBoardTabLabels[4][0]);
	}

	if(m_pApp->m_Settings.m_szSoundBoardTabLabels[5][0] == 0)
	{
		m_cSoundBoardTab.InsertItem(nPos++, "Misc");
		strcpy(m_pApp->m_Settings.m_szSoundBoardTabLabels[5], "Misc");
	}
	else
	{
		m_cSoundBoardTab.InsertItem(nPos++, &m_pApp->m_Settings.m_szSoundBoardTabLabels[5][0]);
	}

	((CButton *)(GetDlgItem( IDC_PLAY_RADIO )))->SetCheck(1);

	m_szBoardName = m_pApp->m_Settings.m_szSoundBoardTabLabels[0];

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DMSoundboardDialog::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
}

void DMSoundboardDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	//CDialog::OnCancel();
}

void DMSoundboardDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// Do not call CDialog::OnPaint() for painting messages

	CString szTemp;

	if(m_pParent != NULL)
	{
		CRect rect;
		m_pParent->GetClientRect(&rect);

		rect.top+=20;

		SetWindowPos(NULL, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
		m_cSoundBoardTab.SetWindowPos(NULL, rect.left, rect.top-20, rect.right, rect.bottom, SWP_SHOWWINDOW);
	}

	for(int i = 0; i< 80; ++i)
	{	
		int nCtrl = IDC_SOUND_BUTTON_1+i;
		//szTemp.Format("Butt %02d %02d", m_nPage, i);
		((CButton *)(GetDlgItem( nCtrl )))->SetWindowText(m_pApp->m_Settings.m_SoundFX[m_nPage][i].m_szDesc);
		//((CButton *)(GetDlgItem( nCtrl )))->SetWindowText(szTemp);
	}
}

BOOL DMSoundboardDialog::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	
	if(nID >= IDC_SOUND_BUTTON_1 && nID <= IDC_SOUND_BUTTON_80)
	{
		int nIndex = nID - IDC_SOUND_BUTTON_1;
		if(m_bEditMode)
		{
			DMSFXEditorDialog *pDlg = new DMSFXEditorDialog(&m_pApp->m_Settings.m_SoundFX[m_nPage][nIndex]);
			pDlg->DoModal();
			delete pDlg;

			InvalidateRect(NULL);

			m_pApp->SaveSettings();
		}
		else
		{
			CString szPath = m_pApp->m_Settings.m_SoundFX[m_nPage][nIndex].m_szFilePath;
			szPath.Replace("<$DMAPATH>", m_pApp->m_szEXEPath);

			if (m_pApp->PlaySoundFXFromFile(szPath) == FALSE)
			{
				m_pApp->m_Settings.m_SoundFX[m_nPage][nIndex].Clear();
			}
		}

		TRACE("PRESSED %d !\n", nID - IDC_SOUND_BUTTON_1);
	}
	
	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL DMSoundboardDialog::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::OnCommand(wParam, lParam);
}


void DMSoundboardDialog::OnPlayRadio() 
{
	// TODO: Add your control notification handler code here

	m_bEditMode = FALSE;
	
}

void DMSoundboardDialog::OnEditRadio() 
{
	// TODO: Add your control notification handler code here
	
	m_bEditMode = TRUE;
}


void DMSoundboardDialog::OnSelchangeSoundboardTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);

	m_nPage = m_cSoundBoardTab.GetCurSel();

	InvalidateRect(NULL);

	m_szBoardName = m_pApp->m_Settings.m_szSoundBoardTabLabels[m_nPage];

	UpdateData(FALSE);
	
	*pResult = 0;
}

void DMSoundboardDialog::OnChangeBoardnameEdit() 
{
	UpdateData(TRUE);

	int nPage = m_cSoundBoardTab.GetCurSel();

	strcpy(m_pApp->m_Settings.m_szSoundBoardTabLabels[nPage], m_szBoardName.GetBuffer(0));

	TCITEM ltag;
	ltag.mask = TCIF_TEXT;
	ltag.pszText = m_szBoardName.GetBuffer(0);
	m_cSoundBoardTab.SetItem(nPage, &ltag);

	UpdateData(FALSE);

	InvalidateRect(NULL);
	
}

void DMSoundboardDialog::OnMapWeaponSfxButton() 
{
	DMSFXMapperDialog *pDlg = new DMSFXMapperDialog(FALSE);
	pDlg->DoModal();

	delete pDlg;
}

void DMSoundboardDialog::OnMapSpellSfxButton() 
{
	DMSFXMapperDialog *pDlg = new DMSFXMapperDialog(TRUE);
	pDlg->DoModal();

	delete pDlg;
}

void DMSoundboardDialog::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();

	delete this;
}
