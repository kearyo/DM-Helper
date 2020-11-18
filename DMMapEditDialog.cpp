// DMMapEditDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DM HelperDlg.h"
#include "DMMapEditDialog.h"
#include "cDMMapViewDialog.h"
#include "cDMMusicTrackListDialog.h"
#include "DungeonGen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static OPENFILENAME    g_ofn;
char g_LG_szFilename[MAX_PATH];

/////////////////////////////////////////////////////////////////////////////
// DMMapEditDialog dialog


DMMapEditDialog::DMMapEditDialog(cDNDMap *pDNDMap, CWnd* pParent /*=NULL*/)
	: CDialog(DMMapEditDialog::IDD, pParent)
	, m_szWeatherBaseCoord(_T(""))
	, m_szMapLatitudeMiles(_T(""))
	, m_bMapTiles(FALSE)
	, m_bGridCheck(FALSE)
	, m_szRoomOcc(_T("50"))
	, m_szTransRed(_T(""))
	, m_szTransGreen(_T(""))
	, m_szTransBlue(_T(""))
	, m_nTileHeight(0)
	, m_szTileHeight(_T("0"))
	, m_szMapMusic(_T(""))
{
	//{{AFX_DATA_INIT(DMMapEditDialog)
	m_szMapName = _T("");
	m_szPixelsX = _T("");
	m_szPixelsY = _T("");
	m_szMapRows = _T("");
	m_szMapColumns = _T("");
	m_szMapScale = _T("");
	//}}AFX_DATA_INIT

	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_pDNDMap = pDNDMap;

	m_pDMMapViewDialog = (cDMMapViewDialog *)pParent;

	m_pDMMapViewDialog->m_MapMode = DND_MAP_MODE_EDIT;

	if(m_pDNDMap == NULL)
	{
		m_pDNDMap = new cDNDMap();
	}

	m_nMaxTiles = 0;
	
	m_bMapReadyRender = FALSE;
	m_bRefreshParent = FALSE;

	m_nTileScrollBar = 0;

	Create(DMMapEditDialog::IDD, pParent);
}


void DMMapEditDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DMMapEditDialog)
	DDX_Control(pDX, IDC_SCALE_MAP_BUTTON, m_cScaleMapButton);
	DDX_Control(pDX, IDC_MAP_SCALE_EDIT, m_cMapScaleEdit);
	DDX_Control(pDX, IDC_PARENT_MAP_COMBO, m_cParentMapCombo);
	DDX_Text(pDX, IDC_MAP_NAME_EDIT, m_szMapName);
	DDX_Text(pDX, IDC_MAP_PIXELS_X_EDIT, m_szPixelsX);
	DDX_Text(pDX, IDC_MAP_PIXELS_Y_EDIT, m_szPixelsY);
	DDX_Text(pDX, IDC_MAP_ROWS_EDIT, m_szMapRows);
	DDX_Text(pDX, IDC_MAP_COLUMNS_EDIT, m_szMapColumns);
	DDX_Text(pDX, IDC_MAP_SCALE_EDIT, m_szMapScale);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_MAP_WEATHER_BASE_COORD_EDIT, m_cWeatherBaseCoord);
	DDX_Text(pDX, IDC_MAP_WEATHER_BASE_COORD_EDIT, m_szWeatherBaseCoord);
	DDV_MaxChars(pDX, m_szWeatherBaseCoord, 12);
	DDX_Control(pDX, IDC_MAP_LATITUDE_MILES_EDIT, m_cMapLatitudeMiles);
	DDX_Text(pDX, IDC_MAP_LATITUDE_MILES_EDIT, m_szMapLatitudeMiles);
	DDV_MaxChars(pDX, m_szMapLatitudeMiles, 8);
	DDX_Check(pDX, IDC_TILES_CHECK, m_bMapTiles);
	DDX_Control(pDX, IDC_TILE_SCROLLBAR, m_cTileScroll);
	DDX_Control(pDX, IDC_GRID_CHECK, m_cGridCheck);
	DDX_Check(pDX, IDC_GRID_CHECK, m_bGridCheck);
	DDX_Control(pDX, IDC_RANDOM_DUNGEON_BUTTON, m_cRandomDungeonButton);
	DDX_Control(pDX, IDC_TILES_CHECK, m_cMapTiles);
	DDX_Control(pDX, IDC_MONSTER_LEVEL_STATIC, m_cMonsterLevelStatic);
	DDX_Control(pDX, IDC_LEVEL_COMBO, m_cMonsterLevel);
	DDX_Control(pDX, IDC_ROOM_OCC_STATIC, m_cRoomOccStatic);
	DDX_Control(pDX, IDC_ROOM_OCC_EDIT, m_cRoomOccEdit);
	DDX_Text(pDX, IDC_ROOM_OCC_EDIT, m_szRoomOcc);
	DDV_MaxChars(pDX, m_szRoomOcc, 3);
	DDX_Text(pDX, IDC_TRANS_COLOR_RED, m_szTransRed);
	DDV_MaxChars(pDX, m_szTransRed, 3);
	DDX_Text(pDX, IDC_TRANS_COLOR_GREEN, m_szTransGreen);
	DDV_MaxChars(pDX, m_szTransGreen, 3);
	DDX_Text(pDX, IDC_TRANS_COLOR_BLUE, m_szTransBlue);
	DDV_MaxChars(pDX, m_szTransBlue, 3);
	DDX_Control(pDX, IDC_RELOAD_TILES_BUTTON, m_cReloadTilesButton);
	DDX_Control(pDX, IDC_TILE_HEIGHT_STATIC, m_cTileHeightStatic);
	DDX_Control(pDX, IDC_TILE_HEIGHT_SLIDER, m_cTileHeightSlider);
	DDX_Slider(pDX, IDC_TILE_HEIGHT_SLIDER, m_nTileHeight);
	DDV_MinMaxInt(pDX, m_nTileHeight, -256, 256);
	DDX_Control(pDX, IDC_TILE_HEIGHT_EDIT, m_cTileHeightEdit);
	DDX_Text(pDX, IDC_TILE_HEIGHT_EDIT, m_szTileHeight);
	DDX_Control(pDX, IDC_MAP_MUSIC_EDIT, m_cMapMusicEdit);
	DDX_Text(pDX, IDC_MAP_MUSIC_EDIT, m_szMapMusic);
	DDX_Control(pDX, IDC_MAP_MUSIC_LABEL, m_cMusicLabel);
	DDX_Control(pDX, IDC_OPEN_TRACK_LIST_BUTTON, m_cTrackListButton);
}

BEGIN_MESSAGE_MAP(DMMapEditDialog, CDialog)
	//{{AFX_MSG_MAP(DMMapEditDialog)
	ON_CBN_SELCHANGE(IDC_PARENT_MAP_COMBO, OnSelchangeParentMapCombo)
	ON_BN_CLICKED(IDC_MILES_RADIO, OnMilesRadio)
	ON_BN_CLICKED(IDC_FEET_RADIO, OnFeetRadio)
	ON_BN_CLICKED(IDC_SCALE_MAP_BUTTON, OnScaleMapButton)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_AUTO_SCALE_MAP_BUTTON, &DMMapEditDialog::OnBnClickedAutoScaleMapButton)
	ON_EN_CHANGE(IDC_MAP_NAME_EDIT, &DMMapEditDialog::OnEnChangeMapNameEdit)
	ON_BN_CLICKED(IDOK, &DMMapEditDialog::OnBnClickedOk)
	ON_EN_CHANGE(IDC_MAP_WEATHER_BASE_COORD_EDIT, &DMMapEditDialog::OnEnChangeMapWeatherBaseCoordEdit)
	ON_EN_CHANGE(IDC_MAP_LATITUDE_MILES_EDIT, &DMMapEditDialog::OnEnChangeMapLatitudeMilesEdit)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_TILES_CHECK, &DMMapEditDialog::OnBnClickedTilesCheck)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_GRID_CHECK, &DMMapEditDialog::OnBnClickedGridCheck)
	ON_BN_CLICKED(IDC_RANDOM_DUNGEON_BUTTON, &DMMapEditDialog::OnBnClickedRandomDungeonButton)
	ON_EN_CHANGE(IDC_MAP_PIXELS_Y_EDIT, &DMMapEditDialog::OnEnChangeMapPixelsYEdit)
	ON_EN_CHANGE(IDC_MAP_ROWS_EDIT, &DMMapEditDialog::OnEnChangeMapRowsEdit)
	ON_EN_CHANGE(IDC_MAP_COLUMNS_EDIT, &DMMapEditDialog::OnEnChangeMapColumnsEdit)
	ON_EN_CHANGE(IDC_MAP_PIXELS_X_EDIT, &DMMapEditDialog::OnEnChangeMapPixelsXEdit)
	ON_BN_CLICKED(IDC_DEBUG_BUTTON, &DMMapEditDialog::OnBnClickedDebugButton)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_RELOAD_TILES_BUTTON, &DMMapEditDialog::OnBnClickedReloadTilesButton)
	ON_BN_CLICKED(IDC_DEFINE_LEGEND_BUTTON, &DMMapEditDialog::OnBnClickedDefineLegendButton)
	ON_BN_CLICKED(IDC_OPEN_TRACK_LIST_BUTTON, &DMMapEditDialog::OnBnClickedOpenTrackListButton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DMMapEditDialog message handlers

BOOL DMMapEditDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_szMapName = m_pDNDMap->m_szMapName;

	m_szPixelsX.Format("%d", m_pDNDMap->m_nPixelSizeX);
	m_szPixelsY.Format("%d", m_pDNDMap->m_nPixelSizeY);
	
	m_szMapRows.Format("%d", m_pDNDMap->m_nRows);
	m_szMapColumns.Format("%d", m_pDNDMap->m_nColumns);

	m_szMapScale.Format("%0.5f", m_pDNDMap->m_fScaleX);

	m_szTransRed.Format("%d", m_pDNDMap->m_nTransRed);
	m_szTransGreen.Format("%d", m_pDNDMap->m_nTransGreen);
	m_szTransBlue.Format("%d", m_pDNDMap->m_nTransBlue);

	int nRow = 0;

	m_cParentMapCombo.InsertString(nRow, " ");
	m_cParentMapCombo.SetItemData(nRow, NULL);

	++nRow;

	for (POSITION pos = m_pApp->m_MapViewMap.GetStartPosition(); pos != NULL; )
	{
		WORD wID;
		PDNDMAPVIEWDLG pMapDlg = NULL;
		m_pApp->m_MapViewMap.GetNextAssoc(pos,wID,pMapDlg);

		if(pMapDlg != NULL && pMapDlg->m_pDNDMap != NULL && pMapDlg->m_pDNDMap->m_dwMapID != m_pDNDMap->m_dwMapID)
		{
			m_cParentMapCombo.InsertString(nRow, pMapDlg->m_pDNDMap->m_szMapName);
			m_cParentMapCombo.SetItemData(nRow, (ULONG)pMapDlg);

			if(m_pDNDMap->m_dwParentMapID == pMapDlg->m_pDNDMap->m_dwMapID)
			{
				m_cParentMapCombo.SetCurSel(nRow);
			}

			++nRow;
		}
	}

	m_nMaxTiles = m_pApp->LoadDungeonTiles();

	m_bMapTiles = m_pDNDMap->m_bTiles;

	int nPos = 0;
	m_cMonsterLevel.InsertString(nPos++, "NONE");
	m_cMonsterLevel.InsertString(nPos++, "CACHES ONLY");
	m_cMonsterLevel.InsertString(nPos++, "RANDOM LEVEL");
	m_cMonsterLevel.InsertString(nPos++, "1");
	m_cMonsterLevel.InsertString(nPos++, "2");
	m_cMonsterLevel.InsertString(nPos++, "3");
	m_cMonsterLevel.InsertString(nPos++, "4");
	m_cMonsterLevel.InsertString(nPos++, "5");
	m_cMonsterLevel.InsertString(nPos++, "6");
	m_cMonsterLevel.InsertString(nPos++, "7");
	m_cMonsterLevel.InsertString(nPos++, "8");
	m_cMonsterLevel.InsertString(nPos++, "9");
	m_cMonsterLevel.InsertString(nPos++, "10");
	m_cMonsterLevel.InsertString(nPos++, "11");
	m_cMonsterLevel.InsertString(nPos++, "12");
	m_cMonsterLevel.InsertString(nPos++, "13");
	m_cMonsterLevel.InsertString(nPos++, "14");
	m_cMonsterLevel.InsertString(nPos++, "15");
	m_cMonsterLevel.InsertString(nPos++, "16+");

	m_cMonsterLevel.SetCurSel(0);

	m_cTileScroll.SetScrollRange(0, m_nMaxTiles-20, TRUE);

	m_szRoomOcc.Format("%d", m_pDMMapViewDialog->m_nRandomDungeonOccupancy);

	#ifdef _DEBUG
	(GetDlgItem( IDC_DEBUG_BUTTON ))-> ShowWindow(SW_SHOW);
	#endif

	#if GAMETABLE_BUILD
	m_cMapMusicEdit.ShowWindow(SW_SHOW);
	m_cMusicLabel.ShowWindow(SW_SHOW);

	m_cTrackListButton.SetBitmap((::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_FOLDER_ICON_BITMAP))));
	m_cTrackListButton.ShowWindow(SW_SHOW);

	m_szMapMusic = m_pDNDMap->m_szMapAmbienceMusicName;

	#endif

	UpdateData(FALSE);

	ShowWindow(SW_SHOW);

	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DMMapEditDialog::Refresh()
{
	m_szMapScale.Format("%0.5f", m_pDNDMap->m_fScaleX);

	if(m_pDNDMap->m_bMapScaleFeet)
	{
		m_cMapScaleEdit.EnableWindow(TRUE);
		m_cWeatherBaseCoord.EnableWindow(FALSE);
		m_cMapLatitudeMiles.EnableWindow(FALSE);

		if(m_pDNDMap->m_dwParentMapID)
		{
			m_cScaleMapButton.ShowWindow(SW_SHOW);
		}
		else
		{	
			m_cScaleMapButton.ShowWindow(SW_HIDE);
		}
	}
	else
	{
		if(m_pDNDMap->m_dwParentMapID)
		{
			m_cMapScaleEdit.EnableWindow(FALSE);
			m_cScaleMapButton.ShowWindow(SW_SHOW);

			m_cWeatherBaseCoord.EnableWindow(FALSE);
			m_cMapLatitudeMiles.EnableWindow(FALSE);
		}
		else
		{
			m_cMapScaleEdit.EnableWindow(TRUE);
			m_cScaleMapButton.ShowWindow(SW_HIDE);

			m_cWeatherBaseCoord.EnableWindow(TRUE);
			m_cMapLatitudeMiles.EnableWindow(TRUE);

			m_szWeatherBaseCoord.Format("%0.1f", m_pDNDMap->m_fLatitude40NLine);
			m_szMapLatitudeMiles.Format("%0.2f", m_pDNDMap->m_fDegreeLatitudeMiles);
		}
	}

	if(m_pDNDMap->m_bMapScaleFeet)
	{
		((CButton *)(GetDlgItem( IDC_MILES_RADIO )))->SetCheck(0);
		((CButton *)(GetDlgItem( IDC_FEET_RADIO )))->SetCheck(1);
	}
	else
	{
		((CButton *)(GetDlgItem( IDC_MILES_RADIO )))->SetCheck(1);
		((CButton *)(GetDlgItem( IDC_FEET_RADIO )))->SetCheck(0);
	}

	if(m_pDNDMap->m_nRows && m_pDNDMap->m_nColumns && m_pDNDMap->m_nPixelSizeX && m_pDNDMap->m_nPixelSizeY)
	{
		m_bMapReadyRender = TRUE;
	}
	else
	{
		m_bMapReadyRender = FALSE;
	}

	m_cMonsterLevelStatic.ShowWindow(SW_HIDE);
	m_cMonsterLevel.ShowWindow(SW_HIDE);
	m_cRoomOccStatic.ShowWindow(SW_HIDE);
	m_cRoomOccEdit.ShowWindow(SW_HIDE);
	m_cReloadTilesButton.ShowWindow(SW_HIDE);
	m_cTileHeightStatic.ShowWindow(SW_HIDE);
	m_cTileHeightSlider.ShowWindow(SW_HIDE);
	m_cTileHeightEdit.ShowWindow(SW_HIDE);

	if(m_bMapReadyRender == FALSE)
	{
		m_bMapTiles = FALSE;
		m_cMapTiles.EnableWindow(FALSE);
		m_bRefreshParent = FALSE;
	}
	else
	{
		m_cMapTiles.EnableWindow(TRUE);

		if(m_bMapTiles)
		{
			m_cMonsterLevelStatic.ShowWindow(SW_SHOW);
			m_cMonsterLevel.ShowWindow(SW_SHOW);
			m_cRoomOccStatic.ShowWindow(SW_SHOW);
			m_cRoomOccEdit.ShowWindow(SW_SHOW);
			m_cReloadTilesButton.ShowWindow(SW_SHOW);

			if (0) //m_pDMMapViewDialog->m_bIsometricCheck) // not sure we're still not going to do this, so commented out for now
			{	
				m_cTileHeightStatic.ShowWindow(SW_SHOW);
				m_cTileHeightSlider.ShowWindow(SW_SHOW);
				m_cTileHeightEdit.ShowWindow(SW_SHOW);
			}
		}
	}

	if(m_bRefreshParent)
	{
		m_pDMMapViewDialog->m_MapMode = DND_MAP_MODE_UNDEF;
		m_pDMMapViewDialog->InvalidateRect(NULL);
	}

	m_bRefreshParent = FALSE;

	UpdateData(FALSE);
}

void DMMapEditDialog::OnOK() 
{
	UpdateData(TRUE);

	strcpy(m_pDNDMap->m_szMapName, m_szMapName.GetBuffer(0));
	m_pDNDMap->m_nPixelSizeX = atoi(m_szPixelsX.GetBuffer(0));
	m_pDNDMap->m_nPixelSizeY = atoi(m_szPixelsY.GetBuffer(0));
	m_pDNDMap->m_nRows = atoi(m_szMapRows.GetBuffer(0));
	m_pDNDMap->m_nColumns = atoi(m_szMapColumns.GetBuffer(0));
	
	if(m_pDNDMap->m_dwParentMapID == 0 || m_pDNDMap->m_bMapScaleFeet)
	{
		m_pDNDMap->m_fScaleX = (float)atof(m_szMapScale.GetBuffer(0));
		m_pDNDMap->m_fScaleY = m_pDNDMap->m_fScaleX;
	}

	m_pDMMapViewDialog->ReturnFromEdit();

	m_pDMMapViewDialog->m_MapMode = DND_MAP_MODE_UNDEF;

	m_pDNDMap->m_nTransRed = atoi(m_szTransRed.GetBuffer(0));
	m_pDNDMap->m_nTransGreen = atoi(m_szTransGreen.GetBuffer(0));
	m_pDNDMap->m_nTransBlue = atoi(m_szTransBlue.GetBuffer(0));

	#if GAMETABLE_BUILD
	strcpy(m_pDNDMap->m_szMapAmbienceMusicName, m_szMapMusic.Left(255));
	#endif

	m_pDMMapViewDialog->ForceRefresh();

	CDialog::OnOK();

	delete this;  
}

void DMMapEditDialog::OnCancel() 
{
	m_pDMMapViewDialog->ReturnFromEdit();

	// m_pDMMapViewDialog->m_MapMode = DND_MAP_MODE_UNDEF;  // NO !

	m_pDMMapViewDialog->InvalidateRect(NULL);
	
	CDialog::OnCancel();

	delete this;
}

void DMMapEditDialog::OnSelchangeParentMapCombo() 
{
	int nCursor = m_cParentMapCombo.GetCurSel();
	if(nCursor >= 0)
	{
		PDNDMAPVIEWDLG pMapDlg = (PDNDMAPVIEWDLG)m_cParentMapCombo.GetItemData(nCursor);

		if(pMapDlg != NULL)
		{
			m_pDNDMap->m_dwParentMapID = pMapDlg->m_pDNDMap->m_dwMapID;

			Refresh();
		}
	}
	
}

void DMMapEditDialog::OnMilesRadio() 
{
	m_pDNDMap->m_bMapScaleFeet = FALSE;
}

void DMMapEditDialog::OnFeetRadio() 
{
	m_pDNDMap->m_bMapScaleFeet = TRUE;
}

void DMMapEditDialog::StoreData()
{
	UpdateData(TRUE);

	strcpy(m_pDNDMap->m_szMapName, m_szMapName.GetBuffer(0));
	m_pDNDMap->m_nPixelSizeX = atoi(m_szPixelsX.GetBuffer(0));
	m_pDNDMap->m_nPixelSizeY = atoi(m_szPixelsY.GetBuffer(0));
	m_pDNDMap->m_nRows = atoi(m_szMapRows.GetBuffer(0));
	m_pDNDMap->m_nColumns = atoi(m_szMapColumns.GetBuffer(0));
	
	if(m_pDNDMap->m_dwParentMapID == 0 || m_pDNDMap->m_bMapScaleFeet)
	{
		m_pDNDMap->m_fScaleX = (float)atof(m_szMapScale.GetBuffer(0));
		m_pDNDMap->m_fScaleY = m_pDNDMap->m_fScaleX;
	}

	m_pDMMapViewDialog->ReturnFromEdit();

	m_pDMMapViewDialog->m_MapMode = DND_MAP_MODE_UNDEF;

	//m_pDMMapViewDialog->InvalidateRect(NULL);

}

void DMMapEditDialog::OnScaleMapButton() 
{
	cDMMapViewDialog *pParentMapView = NULL;

	for (int i = 0; i < m_pApp->m_TabArray.GetSize(); ++i)
	{
		cDNDDisplayTab *pTab = m_pApp->m_TabArray[i];

		if(pTab != NULL)
		{
			if(pTab->m_nTabType == DND_TAB_TYPE_MAP)
			{
				cDMMapViewDialog *pDlg = (cDMMapViewDialog *)pTab->m_pWindow;

				if(pDlg->m_pDNDMap->m_dwMapID == m_pDMMapViewDialog->m_pDNDMap->m_dwParentMapID)
				{
					pParentMapView = pDlg;
					break;
				}
			}
		}
	}

	if(pParentMapView != NULL)
	{
		m_pApp->m_MapScaler.Reset();

		m_pApp->m_MapScaler.m_pParentMapView = pParentMapView;
		m_pApp->m_MapScaler.m_pChildMapView = m_pDMMapViewDialog;

		m_pApp->m_MapScaler.m_dwParentMapID = m_pDMMapViewDialog->m_pDNDMap->m_dwParentMapID;
		m_pApp->m_MapScaler.m_dwChildMapID = m_pDMMapViewDialog->m_pDNDMap->m_dwMapID;

		StoreData();

		if(m_pDNDMap->m_bMapScaleFeet)
		{
			m_pDMMapViewDialog->m_MapMode = DND_MAP_MODE_PICKED_CHILD_POINT_1;
		}
		else
		{
			m_pDMMapViewDialog->m_MapMode = DND_MAP_MODE_PICK_MSG_CHILD_POINT_1;
		}

		m_pDMMapViewDialog->InvalidateRect(NULL);

	}
	else
	{
		AfxMessageBox("Parent map is not loaded !", MB_OK);
	}
	
}

void DMMapEditDialog::OnBnClickedAutoScaleMapButton()
{
	m_pDMMapViewDialog->m_MapMode = DND_MAP_MODE_PICK_MSG_SCALE_POINT_1;

	m_pDMMapViewDialog->InvalidateRect(NULL);
}

void DMMapEditDialog::OnBnClickedDefineLegendButton()
{
	CString szPath;

	ZeroMemory(&g_ofn, sizeof(OPENFILENAME));
	g_ofn.lStructSize = sizeof(OPENFILENAME);
	g_ofn.nMaxFile = MAX_PATH;
	g_ofn.nMaxFileTitle = MAX_PATH;
	g_ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	g_ofn.hwndOwner = m_hWnd;
	g_ofn.hInstance = m_pApp->m_hInstance;
	g_ofn.lpstrFile = g_LG_szFilename;
	g_ofn.lpstrInitialDir = "C:/";
	g_ofn.lpstrTitle = "Load legend bitmap file";
	//g_ofn.lpstrFilter = "Graphics Files (*.bmp)\0*.bmp\0(*.gif)\0*.gif\0(*.jpg)\0*.jpg\0All Files (*.*)\0*.*\0\0";
	g_ofn.lpstrFilter = "All Files (*.*)\0*.*\0\0";
	g_ofn.lpstrDefExt = "bmp";

	// Get action template file name
	if (GetOpenFileName(&g_ofn))
	{
		szPath = g_LG_szFilename;
		szPath.MakeUpper();

		szPath.Replace(m_pApp->m_szEXEPath, "<$DMAPATH>");
		
		strcpy(m_pDNDMap->m_szMapLegendPath, szPath.GetBuffer(0));

		InvalidateRect(NULL);

		m_pDNDMap->MarkChanged();

	}

	m_pDMMapViewDialog->InvalidateRect(NULL);
}


void DMMapEditDialog::OnBnClickedOpenTrackListButton()
{
	cDMMusicTrackListDialog *pDlg = new cDMMusicTrackListDialog(this, &m_szMapMusic);
	pDlg->DoModal();
	delete pDlg;

	UpdateData(FALSE);
}



void DMMapEditDialog::OnEnChangeMapNameEdit()
{
	UpdateData(TRUE);

	m_pDMMapViewDialog->SetWindowText(m_szMapName);

	m_pDMMapViewDialog->m_pMainDialog->InvalidateRect(NULL);
}

void DMMapEditDialog::OnBnClickedOk()
{
	UpdateData(TRUE);

	m_pDMMapViewDialog->m_nRandomDungeonOccupancy = atoi(m_szRoomOcc.GetBuffer(0));

	m_pDMMapViewDialog->m_nSelectedMapTile = -1;

	OnOK();
}

void DMMapEditDialog::OnEnChangeMapWeatherBaseCoordEdit()
{
	UpdateData(TRUE);
	m_pDNDMap->m_fLatitude40NLine = (float)atof(m_szWeatherBaseCoord.GetBuffer(0));		
}

void DMMapEditDialog::OnEnChangeMapLatitudeMilesEdit()
{
	UpdateData(TRUE);
	m_pDNDMap->m_fDegreeLatitudeMiles = (float)atof(m_szMapLatitudeMiles.GetBuffer(0));
}

void DMMapEditDialog::DrawMapTiles(Graphics *pg, int nCell, int nX, int nY, int nSize)
{
	if(m_pDNDMap->m_bTiles == FALSE)
	{
		m_pDMMapViewDialog->m_nSelectedMapTile = -1;
		m_cTileScroll.ShowWindow(SW_HIDE);
		m_cGridCheck.ShowWindow(SW_HIDE);
		m_cRandomDungeonButton.ShowWindow(SW_HIDE);
		return;
	}
	else
	{
		m_cTileScroll.ShowWindow(SW_SHOW);
		m_cGridCheck.ShowWindow(SW_SHOW);
		m_cRandomDungeonButton.ShowWindow(SW_SHOW);

		if(m_pDMMapViewDialog->m_nSelectedMapTile < 0)
		{
			m_pDMMapViewDialog->m_nSelectedMapTile = 0;
		}
	}

	RectF dst;

	dst.X = (Gdiplus::REAL)nX;
	dst.Y = (Gdiplus::REAL)nY;
	dst.Width = (Gdiplus::REAL)nSize;
	dst.Height = (Gdiplus::REAL)nSize;

	ImageAttributes attr;
	attr.SetColorKey( Color(255, 0, 255), Color(255, 0, 255), ColorAdjustTypeBitmap);

	int nCellX = nCell % 10;
	int nCellY = nCell / 10;

	int nSourceX = nCellX * 40 + 1;
	int nSourceY = nCellY * 40 + 1;

/*
	Status DrawImage(IN Image* image,
                     IN const RectF& destRect,
                     IN REAL srcx,
                     IN REAL srcy,
                     IN REAL srcwidth,
                     IN REAL srcheight,
                     IN Unit srcUnit,
                     IN const ImageAttributes* imageAttributes = NULL,
                     IN DrawImageAbort callback = NULL,
                     IN VOID* callbackData = NULL)
*/

	pg->DrawImage(m_pApp->m_pDungeonTilesBitmap, dst, (Gdiplus::REAL)nSourceX, (Gdiplus::REAL)nSourceY, (Gdiplus::REAL)39, (Gdiplus::REAL)39, UnitPixel, &attr); //, UnitPixel);

}

void DMMapEditDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	Graphics graphics(dc);

	for(int i = 0; i < 10; ++i)
	{
		if(i+10+m_nTileScrollBar == m_pDMMapViewDialog->m_nSelectedMapTile)
		{
			DrawMapTiles(&graphics, 1, 24+45*i, 464, 39);
		}
		else
		{
			DrawMapTiles(&graphics, 0, 24+45*i, 464, 39);
		}

		DrawMapTiles(&graphics, i+10+m_nTileScrollBar, 24+45*i, 464, 39);
	}
}

void DMMapEditDialog::OnBnClickedTilesCheck()
{
	UpdateData(TRUE);

	m_pDNDMap->m_bTiles = m_bMapTiles;

	if(m_pDNDMap->m_bTiles)
	{
		m_pDNDMap->m_fScaleX = 0.32175f;
		m_pDNDMap->m_fScaleY = m_pDNDMap->m_fScaleX;

		m_pDNDMap->m_bMapScaleFeet = TRUE;
	}

	m_bRefreshParent = TRUE;

	Refresh();

	InvalidateRect(NULL);
}

void DMMapEditDialog::OnBnClickedGridCheck()
{
	UpdateData(TRUE);

	m_pDMMapViewDialog->m_bShowTileGrid = m_bGridCheck;

	m_pDMMapViewDialog->InvalidateRect(NULL);

	InvalidateRect(NULL);
}


void DMMapEditDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	// m_pDMMapViewDialog->m_nSelectedMapTile

	TRACE(">> %d %d\n", point.x, point.y);

	if(point.x >= 25 && point.x <= 467)
	{
		if(point.y >= 463 && point.y <= 502)
		{
			int x = (point.x - 25) / 45;

			m_pDMMapViewDialog->m_nSelectedMapTile = 10 + x + m_nTileScrollBar;

			InvalidateRect(NULL);
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}


void DMMapEditDialog::OnEnChangeMapRowsEdit()
{
	UpdateData(TRUE);

	m_pDNDMap->m_nPixelSizeX = atoi(m_szPixelsX.GetBuffer(0));
	m_pDNDMap->m_nPixelSizeY = atoi(m_szPixelsY.GetBuffer(0));
	m_pDNDMap->m_nRows = atoi(m_szMapRows.GetBuffer(0));
	m_pDNDMap->m_nColumns = atoi(m_szMapColumns.GetBuffer(0));

	m_bRefreshParent = TRUE;

	Refresh();
}

void DMMapEditDialog::OnEnChangeMapColumnsEdit()
{
	UpdateData(TRUE);

	m_pDNDMap->m_nPixelSizeX = atoi(m_szPixelsX.GetBuffer(0));
	m_pDNDMap->m_nPixelSizeY = atoi(m_szPixelsY.GetBuffer(0));
	m_pDNDMap->m_nRows = atoi(m_szMapRows.GetBuffer(0));
	m_pDNDMap->m_nColumns = atoi(m_szMapColumns.GetBuffer(0));

	m_bRefreshParent = TRUE;

	Refresh();
}

void DMMapEditDialog::OnEnChangeMapPixelsXEdit()
{
	UpdateData(TRUE);

	m_pDNDMap->m_nPixelSizeX = atoi(m_szPixelsX.GetBuffer(0));
	m_pDNDMap->m_nPixelSizeY = atoi(m_szPixelsY.GetBuffer(0));
	m_pDNDMap->m_nRows = atoi(m_szMapRows.GetBuffer(0));
	m_pDNDMap->m_nColumns = atoi(m_szMapColumns.GetBuffer(0));

	m_bRefreshParent = TRUE;

	Refresh();
}


void DMMapEditDialog::OnEnChangeMapPixelsYEdit()
{
	UpdateData(TRUE);

	m_pDNDMap->m_nPixelSizeX = atoi(m_szPixelsX.GetBuffer(0));
	m_pDNDMap->m_nPixelSizeY = atoi(m_szPixelsY.GetBuffer(0));
	m_pDNDMap->m_nRows = atoi(m_szMapRows.GetBuffer(0));
	m_pDNDMap->m_nColumns = atoi(m_szMapColumns.GetBuffer(0));

	m_bRefreshParent = TRUE;

	Refresh();
}

void DMMapEditDialog::OnBnClickedRandomDungeonButton()
{
	UpdateData(TRUE);

	if(m_pDNDMap->m_dwRandomDungeonPartyID != 0L)
	{
		if(AfxMessageBox("Overwrite Existing Dungeon ?", MB_YESNO) != IDYES)
		{
			return;
		}

		CWaitCursor wait;

		m_pDMMapViewDialog->DeleteCaches();
		//AfxMessageBox("DELETED CACHES !", MB_OK);
		m_pDMMapViewDialog->m_pMainDialog->CloseParty(m_pDNDMap->m_dwRandomDungeonPartyID);
		//AfxMessageBox("DELETED NPCs !", MB_OK);
	}
	else
	{
		m_pDMMapViewDialog->DeleteCaches();
	}

	strcpy(m_pDNDMap->m_szMapName, m_szMapName.GetBuffer(0));

	int nSizeX = m_pDMMapViewDialog->m_pDNDMap->m_nPixelSizeX / 31;
	int nSizeY = m_pDMMapViewDialog->m_pDNDMap->m_nPixelSizeY / 31;

	nSizeX *= m_pDNDMap->m_nColumns;
	nSizeY *= m_pDNDMap->m_nRows;

	BOOL bCaches = FALSE;
	int nComboSelection = m_cMonsterLevel.GetCurSel();

	if(nComboSelection > 0)
	{
		bCaches = TRUE;
	}

	int nMonsterLevel = nComboSelection - 2;

	m_pDMMapViewDialog->m_nRandomDungeonOccupancy = atoi(m_szRoomOcc.GetBuffer(0));
	
	m_pDMMapViewDialog->GenerateRandomDungeon(nSizeX, nSizeY, bCaches, nMonsterLevel);

	Refresh();
}


void DMMapEditDialog::OnBnClickedDebugButton()
{
	m_szMapName = _T("Fargo");
	m_szPixelsX = _T("1024");
	m_szPixelsY = _T("1024");
	m_szMapRows= _T("1");
	m_szMapColumns = _T("1");

	UpdateData(FALSE);

	OnEnChangeMapNameEdit();

	OnEnChangeMapPixelsYEdit();

	#if 0
	//write a map html file 

	CString szFileName = _T("");
	CString szLine = _T("");
	szFileName.Format("%s/welt.html", m_pApp->m_szEXEPath);

	FILE *pOutFile = fopen(szFileName.GetBuffer(0), "wt");

	if(pOutFile != NULL)
	{
		szLine = "<table border='0' cellpadding='0' cellspacing='0' width='100%'>";
	
		fprintf(pOutFile,"%s\n", szLine.GetBuffer(0));

		for(int nRow = 0; nRow < 9; ++nRow)
		{
			fprintf(pOutFile, "   <tr border-collapse: collapse;border-spacing: 0;margin: 0; padding: 0;>\n");

			for(int nCol = 0; nCol < 60; ++nCol)
			{
				szLine.Format("     <td border-collapse: collapse;border-spacing: 0;margin: 0; padding: 0;><img src='flanaess_%d.jpg'></td>\n", nRow*60+nCol);
				fprintf(pOutFile, szLine.GetBuffer(0));
			}

			fprintf(pOutFile, "   </tr>\n");
		}

		fprintf(pOutFile, "</table>\n");

		fclose(pOutFile);
	}
	#endif

}

void DMMapEditDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nOldTileScrollBar = m_nTileScrollBar;

	if(pScrollBar == &m_cTileScroll) 
	{
		switch (nSBCode)
		{
			case SB_LINELEFT:      // Scroll left. 
			{
				--m_nTileScrollBar;

				if(m_nTileScrollBar < 0)
				{
					m_nTileScrollBar = 0;
				}

				break;
			}

			case SB_LINERIGHT:   // Scroll right. 
			{
				++m_nTileScrollBar;
				break;
			}


			case SB_THUMBPOSITION:
			case SB_THUMBTRACK: 
			{
				m_nTileScrollBar = nPos;
				break;
			}
		}	

		if(m_nTileScrollBar > m_nMaxTiles-20)
		{
			m_nTileScrollBar = m_nMaxTiles-20;
		}

		if(nOldTileScrollBar != m_nTileScrollBar)
		{
			RECT Rect;

			Rect.left = 24;
			Rect.top = 464;
			Rect.right = 24+45*10;
			Rect.bottom = 464+39;

			//DrawMapTiles(&graphics, 1, 24+45*i, 464, 39);

			m_cTileScroll.SetScrollPos(m_nTileScrollBar);

			InvalidateRect(&Rect, FALSE);
		}
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);

}


void DMMapEditDialog::OnBnClickedReloadTilesButton()
{
	if (m_pApp->m_pDungeonTilesBitmap != NULL)
	{
		delete m_pApp->m_pDungeonTilesBitmap;
		m_pApp->m_pDungeonTilesBitmap = NULL;

		m_pApp->LoadDungeonTiles();
	}

	if (m_pApp->m_pIsometricDungeonTilesBitmap != NULL)
	{
		delete m_pApp->m_pIsometricDungeonTilesBitmap;
		m_pApp->m_pIsometricDungeonTilesBitmap = NULL;
	}

	m_pDMMapViewDialog->InvalidateRect(NULL);

	InvalidateRect(NULL);
}



