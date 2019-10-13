// cDMMapSFXDialog.cpp : implementation file
//


#include "stdafx.h"
#include <math.h>
#include "DM Helper.h"
#include "cDMMapViewDialog.h"
#include "cDMMapSFXDialog.h"
#include "afxdialogex.h"


static OPENFILENAME    g_ofn;
char g_MVGFX_szFilename[MAX_PATH];

// cDMMapSFXDialog dialog

IMPLEMENT_DYNAMIC(cDMMapSFXDialog, CDialog)

cDMMapSFXDialog::cDMMapSFXDialog(CWnd* pParent, int nSFXIndex, int nX, int nY, float fScale)
	: CDialog(cDMMapSFXDialog::IDD, pParent)
	, m_szSFXName(_T(""))
	, m_szSFXGFXFileName(_T(""))
	, m_bCycleGIF(FALSE)
	, m_bColorKeyed(FALSE)
	, m_bTranslucent(FALSE)
	, m_szAlpha(_T("0.70"))
{
	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_pMapViewDialog = (cDMMapViewDialog*)pParent;
	m_pDNDMap = m_pMapViewDialog->m_pDNDMap;

	m_nSFXIndex = nSFXIndex;
	m_nOriginalSFXIndex = nSFXIndex;

	m_nMouseX = nX;
	m_nMouseY = nY;
	m_fScale = fScale;
}

cDMMapSFXDialog::~cDMMapSFXDialog()
{
}

void cDMMapSFXDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EFFECT_NAME_EDIT, m_szSFXName);
	DDV_MaxChars(pDX, m_szSFXName, 32);
	DDX_Text(pDX, IDC_EFFECT_GFX_EDIT, m_szSFXGFXFileName);
	DDV_MaxChars(pDX, m_szSFXGFXFileName, 255);
	DDX_Control(pDX, IDC_SOUND_FX_COMBO, m_cSFXList);
	DDX_Check(pDX, IDC_SFX_CYCLE_CHECK, m_bCycleGIF);
	DDX_Control(pDX, ID_UP, m_cUpButton);
	DDX_Control(pDX, ID_DOWN, m_cDownButton);
	DDX_Control(pDX, ID_LEFT, m_cLeftButton);
	DDX_Control(pDX, ID_RIGHT, m_cRightButton);
	DDX_Check(pDX, IDC_SFX_COLOR_KEYED_CHECK, m_bColorKeyed);
	DDX_Check(pDX, IDC_SFX_TRANSLUCENT_CHECK, m_bTranslucent);
	DDX_Text(pDX, IDC_ALPHA_EDIT, m_szAlpha);
}


BEGIN_MESSAGE_MAP(cDMMapSFXDialog, CDialog)
	ON_EN_SETFOCUS(IDC_EFFECT_GFX_EDIT, &cDMMapSFXDialog::OnEnSetfocusEffectGfxEdit)
	ON_BN_CLICKED(IDC_CHOOSE_FILE_BUTTON, &cDMMapSFXDialog::OnBnClickedChooseFileButton)
	ON_BN_CLICKED(IDDELETE, &cDMMapSFXDialog::OnBnClickedDelete)
	ON_BN_CLICKED(IDREPOSITION, &cDMMapSFXDialog::OnBnClickedReposition)
	ON_BN_CLICKED(IDOK, &cDMMapSFXDialog::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EFFECT_NAME_EDIT, &cDMMapSFXDialog::OnEnChangeEffectNameEdit)
	ON_EN_CHANGE(IDC_EFFECT_GFX_EDIT, &cDMMapSFXDialog::OnEnChangeEffectGfxEdit)
	ON_BN_CLICKED(IDPREVIEW, &cDMMapSFXDialog::OnBnClickedPreview)
	ON_BN_CLICKED(IDCANCEL, &cDMMapSFXDialog::OnBnClickedCancel)
	ON_BN_CLICKED(ID_UP, &cDMMapSFXDialog::OnBnClickedUp)
	ON_BN_CLICKED(ID_DOWN, &cDMMapSFXDialog::OnBnClickedDown)
	ON_BN_CLICKED(ID_LEFT, &cDMMapSFXDialog::OnBnClickedLeft)
	ON_BN_CLICKED(ID_RIGHT, &cDMMapSFXDialog::OnBnClickedRight)
	ON_BN_CLICKED(ID_SCALE_DOWN, &cDMMapSFXDialog::OnBnClickedScaleDown)
	ON_BN_CLICKED(ID_SCALE_UP, &cDMMapSFXDialog::OnBnClickedScaleUp)
	ON_BN_CLICKED(IDC_SFX_CYCLE_CHECK, &cDMMapSFXDialog::OnBnClickedSfxCycleCheck)
	ON_BN_CLICKED(IDC_SFX_COLOR_KEYED_CHECK, &cDMMapSFXDialog::OnBnClickedSfxColorKeyedCheck)
	ON_BN_CLICKED(IDC_SFX_TRANSLUCENT_CHECK, &cDMMapSFXDialog::OnBnClickedSfxTranslucentCheck)
	ON_EN_CHANGE(IDC_ALPHA_EDIT, &cDMMapSFXDialog::OnEnChangeAlphaEdit)
END_MESSAGE_MAP()


// cDMMapSFXDialog message handlers


BOOL cDMMapSFXDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cUpButton.SetBitmap((::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_EXPAND_UP_BITMAP))));
	m_cDownButton.SetBitmap((::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_EXPAND_DOWN_BITMAP))));
	m_cLeftButton.SetBitmap((::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_LEFT_ICON_BITMAP))));
	m_cRightButton.SetBitmap((::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_RIGHT_ICON_BITMAP))));

	if (m_nSFXIndex == -1)
	{
		int nIndex = 0;

		do
		{
			if (m_pDNDMap->m_MapSFX[nIndex].m_SFXState == DND_SFX_STATE_UNDEF) // find an open slot
			{
				m_nSFXIndex = nIndex;
				break;
			}
			++nIndex;

		} while (nIndex < MAX_MAP_SFX);
	}

	if (m_nSFXIndex == -1)
	{
		m_nSFXIndex = MAX_MAP_SFX - 1;
	}

	m_szSFXName = m_pDNDMap->m_MapSFX[m_nSFXIndex].m_szSFXName;
	m_szSFXGFXFileName = m_pDNDMap->m_MapSFX[m_nSFXIndex].m_szGFXFileName;


	int nCount = 0;
	m_cSFXList.ResetContent();

	for (int i = 0; i < MAX_SOUNDBOARD_PAGES; ++i)
	{
		for (int j = 0; j < SOUNDBOARD_SOUNDS_PER_PAGE; ++j)
		{
			if (m_pApp->m_Settings.m_SoundFX[i][j].m_szDesc[0])
			{
				m_cSFXList.InsertString(nCount, m_pApp->m_Settings.m_SoundFX[i][j].m_szDesc);

				if (strcmp(m_pDNDMap->m_MapSFX[m_nSFXIndex].m_szSFXFileName, m_pApp->m_Settings.m_SoundFX[i][j].m_szDesc) == 0)
				{
					m_cSFXList.SetCurSel(nCount);
				}

				++nCount;
			}
		}
	}

	m_bCycleGIF = m_pDNDMap->m_MapSFX[m_nSFXIndex].m_bCycle;
	m_bColorKeyed = m_pDNDMap->m_MapSFX[m_nSFXIndex].m_bColorKeyed;
	m_bTranslucent = m_pDNDMap->m_MapSFX[m_nSFXIndex].m_bTranslucent;
	m_szAlpha.Format("%0.2f", m_pDNDMap->m_MapSFX[m_nSFXIndex].m_fAlpha);

	if (m_szAlpha == "0.0")
	{
		m_szAlpha = "0.7";
		m_pDNDMap->m_MapSFX[m_nSFXIndex].m_fAlpha = 0.7f;
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void cDMMapSFXDialog::OnEnChangeEffectNameEdit()
{
	UpdateData(TRUE);
}


void cDMMapSFXDialog::OnEnChangeEffectGfxEdit()
{
	UpdateData(TRUE);
}


void cDMMapSFXDialog::OnEnSetfocusEffectGfxEdit()
{
	// TODO: Add your control notification handler code here
}


void cDMMapSFXDialog::OnBnClickedChooseFileButton()
{
	g_MVGFX_szFilename[0] = 0;

	ZeroMemory(&g_ofn, sizeof(OPENFILENAME));
	g_ofn.lStructSize = sizeof(OPENFILENAME);
	g_ofn.nMaxFile = MAX_PATH;
	g_ofn.nMaxFileTitle = MAX_PATH;
	g_ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	g_ofn.hwndOwner = m_hWnd;
	g_ofn.hInstance = m_pApp->m_hInstance;
	g_ofn.lpstrFile = g_MVGFX_szFilename;
	g_ofn.lpstrInitialDir = "C:/";
	g_ofn.lpstrTitle = "Load SFX graphic file";
	//g_ofn.lpstrFilter = "Graphics Files (*.gif)\0*.bmp\0(*.gif)\0*.gif\0(*.jpg)\0*.jpg\0All Files (*.*)\0*.*\0\0";
	g_ofn.lpstrFilter = "All Files (*.*)\0*.*\0\0";
	g_ofn.lpstrDefExt = "gif";

	// Get action template file name
	if (GetOpenFileName(&g_ofn))
	{
		m_szSFXGFXFileName = g_MVGFX_szFilename;

		UpdateData(FALSE);
	}
}


void cDMMapSFXDialog::OnBnClickedDelete()
{
	cDNDMapSFX _MapSFX[MAX_MAP_SFX];

	m_pMapViewDialog->CleanupMapSFX();

	m_pDNDMap->m_MapSFX[m_nSFXIndex].Init();

	int nNewIndex = 0;

	for (int i = 0; i < MAX_MAP_SFX; ++i)
	{
		if (m_pDNDMap->m_MapSFX[i].m_SFXState != DND_SFX_STATE_UNDEF) 
		{
			memcpy(&_MapSFX[nNewIndex], &m_pDNDMap->m_MapSFX[i], sizeof(cDNDMapSFX));
			++nNewIndex;
		}
	}
	
	memcpy(&m_pDNDMap->m_MapSFX, &_MapSFX,  sizeof(cDNDMapSFX) * MAX_MAP_SFX);

	m_pMapViewDialog->UpdateDetachedMaps();

	CDialog::OnCancel();
}


void cDMMapSFXDialog::OnBnClickedReposition()
{
	// TODO: Add your control notification handler code here
}


void cDMMapSFXDialog::OnBnClickedOk()
{
	UpdateData(TRUE);

	m_pMapViewDialog->CleanupMapSFX();

	int nIndex = m_nSFXIndex;

	m_pDNDMap->m_MapSFX[nIndex].m_SFXState = DND_SFX_STATE_READY;

	strcpy(m_pDNDMap->m_MapSFX[nIndex].m_szSFXName, m_szSFXName.Left(31));
	strcpy(m_pDNDMap->m_MapSFX[nIndex].m_szGFXFileName, m_szSFXGFXFileName.Left(255));

	int nCursor = m_cSFXList.GetCurSel();

	if (nCursor >= 0)
	{
		CString szLine = "";
		m_cSFXList.GetLBText(nCursor, szLine);

		strcpy(m_pDNDMap->m_MapSFX[nIndex].m_szSFXFileName, szLine.Left(31));
	}
	else
	{
		memset(m_pDNDMap->m_MapSFX[nIndex].m_szSFXFileName, 0, sizeof(char));
	}

	m_szSFXGFXFileName.MakeUpper();

	if(m_szSFXGFXFileName.Find(".GIF") >= 0)
	{
		m_pDNDMap->m_MapSFX[nIndex].m_bAnimated = TRUE;
	}
	else
	{
		m_pDNDMap->m_MapSFX[nIndex].m_bAnimated = FALSE;
	}

	m_pDNDMap->m_MapSFX[nIndex].m_bCycle = m_bCycleGIF;
			
	if (m_nOriginalSFXIndex == -1)
	{
		m_pDNDMap->m_MapSFX[nIndex].m_nMapX = m_nMouseX;
		m_pDNDMap->m_MapSFX[nIndex].m_nMapY = m_nMouseY;

		m_pDNDMap->m_MapSFX[nIndex].m_fSpriteScale = m_fScale;
	}

	m_pMapViewDialog->UpdateDetachedMaps();

	m_pMapViewDialog->m_pDNDMap->MarkChanged();
	
	CDialog::OnOK();
}


void cDMMapSFXDialog::OnBnClickedPreview()
{
	// TODO: Add your control notification handler code here
}


void cDMMapSFXDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}





void cDMMapSFXDialog::OnBnClickedUp()
{
	m_pDNDMap->m_MapSFX[m_nSFXIndex].m_nMapY -= 1;
	m_pMapViewDialog->InvalidateRect(NULL);
}


void cDMMapSFXDialog::OnBnClickedDown()
{
	m_pDNDMap->m_MapSFX[m_nSFXIndex].m_nMapY += 1;
	m_pMapViewDialog->InvalidateRect(NULL);
}


void cDMMapSFXDialog::OnBnClickedLeft()
{
	m_pDNDMap->m_MapSFX[m_nSFXIndex].m_nMapX -= 1;
	m_pMapViewDialog->InvalidateRect(NULL);
}


void cDMMapSFXDialog::OnBnClickedRight()
{
	m_pDNDMap->m_MapSFX[m_nSFXIndex].m_nMapX += 1;
	m_pMapViewDialog->InvalidateRect(NULL);
}


void cDMMapSFXDialog::OnBnClickedScaleDown()
{
	m_pDNDMap->m_MapSFX[m_nSFXIndex].m_fSpriteScale /= 1.1f;
	m_pMapViewDialog->InvalidateRect(NULL);
}


void cDMMapSFXDialog::OnBnClickedScaleUp()
{
	m_pDNDMap->m_MapSFX[m_nSFXIndex].m_fSpriteScale *= 1.1f;
	m_pMapViewDialog->InvalidateRect(NULL);
}


void cDMMapSFXDialog::OnBnClickedSfxCycleCheck()
{
	UpdateData(TRUE);

	m_pDNDMap->m_MapSFX[m_nSFXIndex].m_bCycle = m_bCycleGIF;
}


void cDMMapSFXDialog::OnBnClickedSfxColorKeyedCheck()
{
	UpdateData(TRUE);

	m_pDNDMap->m_MapSFX[m_nSFXIndex].m_bColorKeyed = m_bColorKeyed;
}


void cDMMapSFXDialog::OnBnClickedSfxTranslucentCheck()
{
	UpdateData(TRUE);

	m_pDNDMap->m_MapSFX[m_nSFXIndex].m_bTranslucent = m_bTranslucent;
}


void cDMMapSFXDialog::OnEnChangeAlphaEdit()
{
	UpdateData(TRUE);

	m_pDNDMap->m_MapSFX[m_nSFXIndex].m_fAlpha = atof(m_szAlpha.GetBuffer(0));
}
