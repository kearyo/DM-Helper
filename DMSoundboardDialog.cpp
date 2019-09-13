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

static OPENFILENAME    g_oportfn;
char g_SFXPORT_szFilename[MAX_PATH];

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
	DDX_Control(pDX, IDC_DELETE_BOARD_BUTTON, m_cDeleteBoardButton);
	DDX_Control(pDX, IDC_ADD_BOARD_BUTTON, m_cAddBoardButton);
	DDX_Control(pDX, IDC_EXPORT_BOARDS_BUTTON, m_cExportBoardsButton);
	DDX_Control(pDX, IDC_IMPORT_BOARDS_BUTTON, m_cImportBoardsButton);
	DDX_Control(pDX, IDC_RESET_DX_SOUNDS_BUTTON, m_cResetDXSoundsButton);
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
	ON_BN_CLICKED(IDC_ADD_BOARD_BUTTON, &DMSoundboardDialog::OnBnClickedAddBoardButton)
	ON_BN_CLICKED(IDC_DELETE_BOARD_BUTTON, &DMSoundboardDialog::OnBnClickedDeleteBoardButton)
	ON_BN_CLICKED(IDC_EXPORT_BOARDS_BUTTON, &DMSoundboardDialog::OnBnClickedExportBoardsButton)
	ON_BN_CLICKED(IDC_IMPORT_BOARDS_BUTTON, &DMSoundboardDialog::OnBnClickedImportBoardsButton)
	ON_BN_CLICKED(IDC_RESET_DX_SOUNDS_BUTTON, &DMSoundboardDialog::OnBnClickedResetDxSoundsButton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DMSoundboardDialog message handlers

BOOL DMSoundboardDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString szDirPath = m_pApp->m_szEXEPath + "data\\sounds\\random";
	CreateDirectory(szDirPath, NULL);

	SetWindowText("Soundboard");

	#if KEARY_BUILD
	m_cExportBoardsButton.ShowWindow(SW_SHOW);
	m_cImportBoardsButton.ShowWindow(SW_SHOW);
	#endif

	#ifdef _DEBUG
	m_cResetDXSoundsButton.ShowWindow(SW_SHOW);
	#endif

	Init();

	((CButton *)(GetDlgItem( IDC_PLAY_RADIO )))->SetCheck(1);

	m_szBoardName = m_pApp->m_Settings.m_szSoundBoardTabLabels[0];

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DMSoundboardDialog::Init()
{
	m_cSoundBoardTab.DeleteAllItems();

	int nPos = 0;

	if (m_pApp->m_Settings.m_szSoundBoardTabLabels[0][0] == 0)
	{
		m_cSoundBoardTab.InsertItem(nPos++, "Combat");
		strcpy(m_pApp->m_Settings.m_szSoundBoardTabLabels[0], "Combat");
	}
	else
	{
		m_cSoundBoardTab.InsertItem(nPos++, &m_pApp->m_Settings.m_szSoundBoardTabLabels[0][0]);
	}

	if (m_pApp->m_Settings.m_szSoundBoardTabLabels[1][0] == 0)
	{
		m_cSoundBoardTab.InsertItem(nPos++, "Spells");
		strcpy(m_pApp->m_Settings.m_szSoundBoardTabLabels[1], "Spells");
	}
	else
	{
		m_cSoundBoardTab.InsertItem(nPos++, &m_pApp->m_Settings.m_szSoundBoardTabLabels[1][0]);
	}

	if (m_pApp->m_Settings.m_szSoundBoardTabLabels[2][0] == 0)
	{
		m_cSoundBoardTab.InsertItem(nPos++, "Actions");
		strcpy(m_pApp->m_Settings.m_szSoundBoardTabLabels[2], "Actions");
	}
	else
	{
		m_cSoundBoardTab.InsertItem(nPos++, &m_pApp->m_Settings.m_szSoundBoardTabLabels[2][0]);
	}

	if (m_pApp->m_Settings.m_szSoundBoardTabLabels[3][0] == 0)
	{
		m_cSoundBoardTab.InsertItem(nPos++, "Monsters");
		strcpy(m_pApp->m_Settings.m_szSoundBoardTabLabels[3], "Monsters");
	}
	else
	{
		m_cSoundBoardTab.InsertItem(nPos++, &m_pApp->m_Settings.m_szSoundBoardTabLabels[3][0]);
	}

	if (m_pApp->m_Settings.m_szSoundBoardTabLabels[4][0] == 0)
	{
		m_cSoundBoardTab.InsertItem(nPos++, "Environment");
		strcpy(m_pApp->m_Settings.m_szSoundBoardTabLabels[4], "Environment");
	}
	else
	{
		m_cSoundBoardTab.InsertItem(nPos++, &m_pApp->m_Settings.m_szSoundBoardTabLabels[4][0]);
	}

	if (m_pApp->m_Settings.m_szSoundBoardTabLabels[5][0] == 0)
	{
		m_cSoundBoardTab.InsertItem(nPos++, "Misc");
		strcpy(m_pApp->m_Settings.m_szSoundBoardTabLabels[5], "Misc");
	}
	else
	{
		m_cSoundBoardTab.InsertItem(nPos++, &m_pApp->m_Settings.m_szSoundBoardTabLabels[5][0]);
	}

	for (int i = nPos; nPos < MAX_SOUNDBOARD_PAGES; ++i)
	{
		if (m_pApp->m_Settings.m_szSoundBoardTabLabels[i][0] == 0)
		{
			break;
		}

		m_cSoundBoardTab.InsertItem(i, &m_pApp->m_Settings.m_szSoundBoardTabLabels[i][0]);
	}
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

	if (m_nPage < 5)
	{
		m_cAddBoardButton.EnableWindow(FALSE);
	}
	else
	{
		m_cAddBoardButton.EnableWindow(TRUE);
	}

	if (m_nPage < 6)
	{
		m_cDeleteBoardButton.EnableWindow(FALSE);
	}
	else
	{
		m_cDeleteBoardButton.EnableWindow(TRUE);
	}


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
	
	return CDialog::OnCommand(wParam, lParam);
}


void DMSoundboardDialog::OnPlayRadio() 
{
	

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


void DMSoundboardDialog::OnBnClickedAddBoardButton()
{
	/*
	int nPages = m_cSoundBoardTab.GetItemCount();

	if (nPages >= MAX_SOUNDBOARD_PAGES - 1)
		return;

	if (AfxMessageBox("Are you sure you wish to add a new soundboard  ?", MB_YESNO) != IDYES)
		return;

	strcpy(m_pApp->m_Settings.m_szSoundBoardTabLabels[nPages], "New Board");

	Init();

	m_cSoundBoardTab.SetCurSel(nPages);

	m_szBoardName = m_pApp->m_Settings.m_szSoundBoardTabLabels[nPages];

	m_nPage = nPages;

	InvalidateRect(NULL);

	m_szBoardName = m_pApp->m_Settings.m_szSoundBoardTabLabels[m_nPage];

	UpdateData(FALSE);
	*/

	if (AfxMessageBox("Are you sure you wish to insert a new soundboard  ?", MB_YESNO) != IDYES)
		return;

	for (int i = MAX_SOUNDBOARD_PAGES - 1; i >= m_nPage+2; --i)
	{
		memcpy(m_pApp->m_Settings.m_szSoundBoardTabLabels[i], m_pApp->m_Settings.m_szSoundBoardTabLabels[i - 1], 64 * sizeof(char));
		memcpy(m_pApp->m_Settings.m_SoundFX[i], m_pApp->m_Settings.m_SoundFX[i - 1], SOUNDBOARD_SOUNDS_PER_PAGE * sizeof(cDNDSoundEffect));
	}

	++m_nPage;

	strcpy(m_pApp->m_Settings.m_szSoundBoardTabLabels[m_nPage], "New Board");
	memset(m_pApp->m_Settings.m_SoundFX[m_nPage], 0, SOUNDBOARD_SOUNDS_PER_PAGE * sizeof(cDNDSoundEffect));

	Init();

	m_cSoundBoardTab.SetCurSel(m_nPage);

	m_szBoardName = m_pApp->m_Settings.m_szSoundBoardTabLabels[m_nPage];


	InvalidateRect(NULL);

	m_szBoardName = m_pApp->m_Settings.m_szSoundBoardTabLabels[m_nPage];

	UpdateData(FALSE);

}


void DMSoundboardDialog::OnBnClickedDeleteBoardButton()
{
	int nPages = m_cSoundBoardTab.GetItemCount();

	if (nPages < 6)
		return;

	if (AfxMessageBox("Soundboard will be deleted completely !\nAre you sure ?", MB_YESNO) != IDYES)
		return;


	for (int i = m_nPage; i < MAX_SOUNDBOARD_PAGES - 1; ++i)
	{
		/*
		cDNDSoundEffect m_SoundFX[MAX_SOUNDBOARD_PAGES][SOUNDBOARD_SOUNDS_PER_PAGE];
		char m_szSoundBoardTabLabels[MAX_SOUNDBOARD_PAGES][64];
		*/
		memcpy(m_pApp->m_Settings.m_szSoundBoardTabLabels[i], m_pApp->m_Settings.m_szSoundBoardTabLabels[i + 1], 64 * sizeof(char));
		memcpy(m_pApp->m_Settings.m_SoundFX[i], m_pApp->m_Settings.m_SoundFX[i + 1], SOUNDBOARD_SOUNDS_PER_PAGE * sizeof(cDNDSoundEffect));
	}

	m_pApp->m_Settings.m_szSoundBoardTabLabels[MAX_SOUNDBOARD_PAGES - 1][0] = 0;

	m_nPage -= 1;

	Init();

	m_cSoundBoardTab.SetCurSel(m_nPage);

	m_szBoardName = m_pApp->m_Settings.m_szSoundBoardTabLabels[m_nPage];

	InvalidateRect(NULL);

	m_szBoardName = m_pApp->m_Settings.m_szSoundBoardTabLabels[m_nPage];

	UpdateData(FALSE);
}


void DMSoundboardDialog::OnBnClickedExportBoardsButton()
{
	ZeroMemory(&g_oportfn, sizeof(OPENFILENAME));
	g_oportfn.lStructSize = sizeof(OPENFILENAME);
	g_oportfn.nMaxFile = MAX_PATH;
	g_oportfn.nMaxFileTitle = MAX_PATH;
	g_oportfn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	g_oportfn.hwndOwner = m_hWnd;
	g_oportfn.hInstance = m_pApp->m_hInstance;
	g_oportfn.lpstrFile = g_SFXPORT_szFilename;
	g_oportfn.lpstrInitialDir = "C:/";
	g_oportfn.lpstrTitle = "Export soundboard data file";
	g_oportfn.lpstrFilter = "(*.sbd)\0*.sbd\0All Files (*.*)\0*.*\0\0";
	g_oportfn.lpstrDefExt = "sbd";

	// Get action template file name
	if (GetSaveFileName(&g_oportfn))
	{
		//g_SFXPORT_szFilename
		FILE *pOutFile = fopen(g_SFXPORT_szFilename, "wb");

		if (pOutFile != NULL)
		{
			/*
			cDNDSoundEffect m_SoundFX[MAX_SOUNDBOARD_PAGES][SOUNDBOARD_SOUNDS_PER_PAGE];

			char m_szSoundBoardTabLabels[MAX_SOUNDBOARD_PAGES][64];

			cDNDSoundEffectMapper m_WeaponSFX[MAX_WEAPONS_DEFINED][4];
			cDNDSoundEffectMapper m_SpellSFX[5*MAX_SPELL_LEVELS*MAX_SPELLS_PER_LEVEL];
			*/

			fwrite(&m_pApp->m_Settings.m_SoundFX, sizeof(cDNDSoundEffect)*MAX_SOUNDBOARD_PAGES*SOUNDBOARD_SOUNDS_PER_PAGE, 1, pOutFile);
			fwrite(&m_pApp->m_Settings.m_szSoundBoardTabLabels, sizeof(char)*MAX_SOUNDBOARD_PAGES*64, 1, pOutFile);
			fwrite(&m_pApp->m_Settings.m_WeaponSFX, sizeof(cDNDSoundEffectMapper)*MAX_WEAPONS_DEFINED*4, 1, pOutFile);
			fwrite(&m_pApp->m_Settings.m_SpellSFX, sizeof(cDNDSoundEffectMapper) * 5 * MAX_SPELL_LEVELS*MAX_SPELLS_PER_LEVEL, 1, pOutFile);

			fclose(pOutFile);
		}
	}
}


void DMSoundboardDialog::OnBnClickedImportBoardsButton()
{
	CString szPath;

	ZeroMemory(&g_oportfn, sizeof(OPENFILENAME));
	g_oportfn.lStructSize = sizeof(OPENFILENAME);
	g_oportfn.nMaxFile = MAX_PATH;
	g_oportfn.nMaxFileTitle = MAX_PATH;
	g_oportfn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	g_oportfn.hwndOwner = m_hWnd;
	g_oportfn.hInstance = m_pApp->m_hInstance;
	g_oportfn.lpstrFile = g_SFXPORT_szFilename;
	g_oportfn.lpstrInitialDir = "C:/";
	g_oportfn.lpstrTitle = "Import soundboard data file";
	g_oportfn.lpstrFilter = "(*.sbd)\0*.sbd\0All Files (*.*)\0*.*\0\0";
	g_oportfn.lpstrDefExt = "sbd";

	// Get action template file name
	if (GetOpenFileName(&g_oportfn))
	{
		FILE *pInFile = fopen(g_SFXPORT_szFilename, "rb");
		CString szFileName = g_SFXPORT_szFilename;

		m_pApp->ImportSoundBoards(szFileName, FALSE);
	}
}

void DMSoundboardDialog::OnBnClickedResetDxSoundsButton()
{
	#if USE_DX_SOUND
	m_pApp->ShutDownDXSoundFX();
	#endif
}
