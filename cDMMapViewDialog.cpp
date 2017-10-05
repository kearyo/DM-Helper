// cDMMapViewDialog.cpp : implementation file
//


#include "stdafx.h"
#include <math.h>
#include "memdc.h"
#include "DM Helper.h"
#include "DM HelperDlg.h"
#include "DMLoadFileDialog.h"
#include "DMMapEditDialog.h"
#include "DMCharSpellsDialog.h"
#include "DMInventoryDialog.h"
#include "DMCharViewDialog.h"
#include "DMNPCViewDialog.h"
#include "DMPartyDialog.h"
#include "cDMMapViewDialog.h"
#include "DMInputDialog.h"
#include "DungeonGen.h"
#include "DMEditMapLayersDialog.h"
#include "DMNPCPortraitSelectDialog.h"
#include "cDMMapSizingDialog.h"
#include "cDMMapSFXDialog.h"

/* Notes for weather generation

pixelY 2000.0 is equiv to latitude 40 deg N
weather charts are based on latitude 40 deg N

City of Greyhawk is at 36.25 degrees N 17.33 degrees W

70 miles = 1 degree of latitude

0 Meridian is in Rauxes

1 degree temp change for every 70 miles north or south of 40 deg N

*/

/*
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
*/

//reference
//http://msdn.microsoft.com/en-us/library/ms536317(v=vs.85).aspx



#define MAX_RAND_ROOMS	200
#define MAX_RAND_OCCUPIED_ROOMS	30


static OPENFILENAME    g_ofn;
char g_MV_szFilename[MAX_PATH];
char g_MVBP_szFilename[MAX_PATH];

int nMaxMapSizeX = 841;
int nMaxMapSizeY = 1088;



int _MapControlStater[][2] =
{
	IDC_BDROP,							TRUE,
	IDC_CHAR_STATIC,					TRUE,
	IDC_PARTY_STATIC,					TRUE,
	IDC_PARTY_STATIC2,					TRUE,
	IDC_EDIT_BUTTON,					TRUE,
//	IDC_LOAD_BUTTON,					TRUE,
	IDC_SAVE_BUTTON,					TRUE,
	
	IDC_PARTY_COMBO,					TRUE,
	IDC_SUBPARTY_COMBO,					TRUE,
	IDC_CHARACTER_COMBO,				TRUE,
	IDC_PLACE_PARTY_BUTTON,				TRUE,
	IDC_PLACE_ON_PARTY_BUTTON,			TRUE,
	IDC_PLOT_PARTY_ROUTE_BUTTON,		TRUE,
	IDC_PLOT_PARTY_EXTEND_ROUTE_BUTTON,	TRUE,
	IDC_PLACE_CHARACTER_BUTTON,			TRUE,
	IDC_PARTY_CHARACTER_BUTTON,			TRUE,
	IDC_PLACE_ON_CHARACTER_BUTTON,		TRUE,
	IDC_SET_CHARACTER_ICON_BUTTON,		TRUE,
	IDC_HIDE_BUTTON,					TRUE,

	IDC_SHOW_STATIC,					TRUE,
	IDC_HINT_STATIC,					TRUE,
	IDC_PARTIES_CHECK,					TRUE,
	IDC_ROUTES_CHECK,					TRUE,
	IDC_CACHES_CHECK,					TRUE,
	IDC_CHILD_MAPS_CHECK,				TRUE,
	IDC_SCALE_STATIC,					TRUE,
	IDC_CHAR_ICONS_CHECK,				TRUE,
	IDC_SUBPARTIES_CHECK,				TRUE,
	IDC_SCALE_SLIDER,					TRUE,
	IDC_TICKS_STATIC,					TRUE,

	IDC_SCALE_STATIC2,					TRUE,
	IDC_SCALE_SLIDER2,					TRUE,
	IDC_TICKS_STATIC2,					TRUE,

	IDC_CACHE_STATIC,					TRUE,
	IDC_PLACE_CACHE_BUTTON,				TRUE,
	IDC_PLACE_ON_CACHE_BUTTON,			TRUE,
	IDC_ADD_CACHE_BUTTON,				TRUE,
	IDC_DELETE_CACHE_BUTTON,			TRUE,
	IDC_CACHE_COMBO,					TRUE,

	IDC_LAYER_CHECK_1,					TRUE,
	IDC_LAYER_CHECK_2,					TRUE,
	IDC_LAYER_CHECK_3,					TRUE,
	IDC_LAYER_CHECK_4,					TRUE,

#if ISOMETRIC_MAPS
	IDC_ISOMETRIC_CHECK,				TRUE,
#endif
	IDC_FOG_OF_WAR_CHECK,				TRUE,

	IDC_LABELS_CHECK,					TRUE,

	IDC_SHOW_BUTTON,					FALSE,

	IDC_SCALE_SLIDER3,					TRUE,
	IDC_SCALE_STATIC3,					TRUE,
	IDC_TICKS_STATIC3,					TRUE,

	-1,									TRUE
};

/////////////////////////////////////////////////////////////////////////////
// cDMMapViewDialog dialog


cDMMapViewDialog::cDMMapViewDialog(CDMHelperDlg* pMainDialog, cDNDMap *pDNDMap, CWnd* pParent /*=NULL*/)
	: CDialog(cDMMapViewDialog::IDD, pParent)
	, m_szCacheDetails(_T(""))
	, m_bShowCachesCheck(TRUE)
	, m_bTravelHoursCheck(FALSE)
	, m_bWeatherCheck(FALSE)
	, m_bLayer1(TRUE)
	, m_bLayer2(TRUE)
	, m_bLayer3(TRUE)
	, m_bLayer4(TRUE)
	, m_bCharIcons(TRUE)
	, m_bSubPartyCheck(FALSE)
	, m_nIconScale(0)
	, m_bIsometricCheck(FALSE)
	, m_bLabelsCheck(FALSE)
	, m_bFogOfWarCheck(FALSE)
	, m_nLightingSlider(0)
{
	//{{AFX_DATA_INIT(cDMMapViewDialog)
	m_szMapLegend = _T("");
	m_bShowPartiesCheck = FALSE;
	m_bShowRoutesCheck = FALSE;
	m_bShowChildMapsCheck = FALSE;
	m_szHint = _T("");
	m_nScaleSlider = 0;
	//}}AFX_DATA_INIT

	if(pDNDMap == NULL)
	{
		m_pDNDMap = new cDNDMap();
	}
	else
	{
		m_pDNDMap = pDNDMap;
	}

	m_MapMode = DND_MAP_MODE_UNDEF;

	m_bMapPaint = FALSE;
	m_bMouseDrag = FALSE;

	m_bCatchMe = FALSE;

	m_pUpdateRect = NULL;

	m_DragPoint.x = 0;
	m_DragPoint.y = 0;

	m_bWaitingOnUserClick = FALSE;

	m_pMapEditDlg = NULL;
	m_pSelectedParty = NULL;
	m_fSelectedPartyRouteDistance = 0.0f;

	m_dwSelectedSubPartyID = 0;

	m_pEditMapLayersDialog = NULL;

	m_pSelectedCharacter = NULL;
	m_dwDraggedCharacterID = 0;

	m_nCornerX = 0;
	m_nCornerY = 0;
	
	m_nMouseX = 0;
	m_nMouseY = 0;

	m_nMousePointX = 0;
	m_nMousePointY = 0;


	m_fViewScale = 1.0f;

	m_pMainDialog = pMainDialog;
	m_pParent = pParent;

	m_szPartyAlert = _T("");

	m_pTreasureCaches = NULL;
	m_pSelectedCache = NULL;
	
	m_nMapCharacters = 0;
	m_nMapParties = 0;

	m_nMapCaches = 0;
	m_pHoverCache = NULL;

	m_nSelectedMapTile = -1;
	m_bShowTileGrid = FALSE;

	m_nRandomDungeonOccupancy = 50;

	m_bDetachedWindow = FALSE;

	m_nOrientation = DMDO_DEFAULT;

	#if _PARTICLE_WEATHER
	m_pRainParticleBitmap = NULL;
	m_pSnowParticleBitmap = NULL;
	#endif

	m_pLightingAlphaBitmap = NULL;
	m_pFogOfWarBitmap = NULL;
	m_pSFXButtonBitmap = NULL;

	m_fLightingAlpha = 0.0f;

	m_bShuttingDown = FALSE;

	//m_image = NULL;

	#if _PARTICLE_WEATHER

	m_pParticleBufferBitmap = NULL;

	for (int i = 0; i < MAX_PARTICLES; i++) 
	{
		m_Particle[i].m_fX = 0;
		m_Particle[i].m_fY = 0;
		m_Particle[i].m_fZ = MAX_PARTICLE_DEPTH * 2;
	}

	#endif

	Create(cDMMapViewDialog::IDD, pParent);
}


void cDMMapViewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(cDMMapViewDialog)
	DDX_Control(pDX, IDC_TRAVEL_BUTTON, m_cTravelButton);
	DDX_Control(pDX, IDC_SCALE_SLIDER, m_cScaleSlider);
	DDX_Control(pDX, IDC_CHARACTER_COMBO, m_cCharacterCombo);
	DDX_Control(pDX, IDC_PARTY_COMBO, m_cPartyCombo);
	DDX_Control(pDX, IDC_MAP_LEGEND, m_cMapLegend);
	DDX_Text(pDX, IDC_MAP_LEGEND, m_szMapLegend);
	DDX_Check(pDX, IDC_PARTIES_CHECK, m_bShowPartiesCheck);
	DDX_Check(pDX, IDC_ROUTES_CHECK, m_bShowRoutesCheck);
	DDX_Check(pDX, IDC_CHILD_MAPS_CHECK, m_bShowChildMapsCheck);
	DDX_Text(pDX, IDC_HINT_STATIC, m_szHint);
	DDX_Slider(pDX, IDC_SCALE_SLIDER, m_nScaleSlider);
	DDX_Control(pDX, IDC_SHOW_BUTTON, m_cShowButton);
	DDX_Control(pDX, IDC_HIDE_BUTTON, m_cHideButton);
	//}}AFX_DATA_MAP

	DDX_Control(pDX, IDC_CACHE_COMBO, m_cCacheCombo);
	DDX_Control(pDX, IDC_CACHE_DETAILS_EDIT, m_cCacheDetailsEdit);
	DDX_Text(pDX, IDC_CACHE_DETAILS_EDIT, m_szCacheDetails);
	DDV_MaxChars(pDX, m_szCacheDetails, 512);
	DDX_Check(pDX, IDC_CACHES_CHECK, m_bShowCachesCheck);
	DDX_Control(pDX, IDC_TRAVEL_MOUNTED_BUTTON, m_cTravelMountedButton);
	DDX_Control(pDX, IDC_HOURS_CHECK, m_cTravelHoursCheck);
	DDX_Check(pDX, IDC_HOURS_CHECK, m_bTravelHoursCheck);
	DDX_Control(pDX, IDC_WEATHER_CHECK, m_cWeatherCheck);
	DDX_Check(pDX, IDC_WEATHER_CHECK, m_bWeatherCheck);
	DDX_Control(pDX, IDC_REST_BUTTON, m_cRestButton);
	DDX_Check(pDX, IDC_LAYER_CHECK_1, m_bLayer1);
	DDX_Check(pDX, IDC_LAYER_CHECK_2, m_bLayer2);
	DDX_Check(pDX, IDC_LAYER_CHECK_3, m_bLayer3);
	DDX_Check(pDX, IDC_LAYER_CHECK_4, m_bLayer4);
	DDX_Control(pDX, IDC_SUBPARTY_COMBO, m_cSubPartyCombo);
	DDX_Check(pDX, IDC_CHAR_ICONS_CHECK, m_bCharIcons);
	DDX_Check(pDX, IDC_SUBPARTIES_CHECK, m_bSubPartyCheck);
	DDX_Control(pDX, IDC_SCALE_SLIDER2, m_cIconScale);
	DDX_Slider(pDX, IDC_SCALE_SLIDER2, m_nIconScale);
	DDX_Control(pDX, IDC_ISOMETRIC_CHECK, m_cIsometricCheck);
	DDX_Check(pDX, IDC_ISOMETRIC_CHECK, m_bIsometricCheck);
	DDX_Check(pDX, IDC_LABELS_CHECK, m_bLabelsCheck);
	DDX_Control(pDX, IDC_DETACH_BUTTON, m_cDetachButton);
	DDX_Control(pDX, IDC_FLIP_BUTTON, m_cFlipDisplayButton);
	DDX_Control(pDX, IDC_FOG_OF_WAR_CHECK, m_cFogOfWarCheck);
	DDX_Check(pDX, IDC_FOG_OF_WAR_CHECK, m_bFogOfWarCheck);
	DDX_Slider(pDX, IDC_SCALE_SLIDER3, m_nLightingSlider);
	DDV_MinMaxInt(pDX, m_nLightingSlider, 0, 100);
	DDX_Control(pDX, IDC_SCALE_SLIDER3, m_cLightingSlider);
}


BEGIN_MESSAGE_MAP(cDMMapViewDialog, CDialog)
	//{{AFX_MSG_MAP(cDMMapViewDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_EDIT_BUTTON, OnEditButton)
	ON_BN_CLICKED(IDC_LOAD_BUTTON, OnLoadButton)
	ON_BN_CLICKED(IDC_SAVE_BUTTON, OnSaveButton)
	ON_WM_RBUTTONDBLCLK()
	ON_WM_CLOSE()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(IDC_PARTY_COMBO, OnSelchangePartyCombo)
	ON_BN_CLICKED(IDC_PLACE_PARTY_BUTTON, OnPlacePartyButton)
	ON_BN_CLICKED(IDC_PLACE_ON_PARTY_BUTTON, OnPlaceOnPartyButton)
	ON_BN_CLICKED(IDC_PLOT_PARTY_ROUTE_BUTTON, OnPlotPartyRouteButton)
	ON_BN_CLICKED(IDC_HIDE_BUTTON, OnHideButton)
	ON_BN_CLICKED(IDC_SHOW_BUTTON, OnShowButton)
	ON_CBN_SELCHANGE(IDC_CHARACTER_COMBO, OnSelchangeCharacterCombo)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SCALE_SLIDER, OnReleasedcaptureScaleSlider)
	ON_BN_CLICKED(IDC_PLOT_PARTY_EXTEND_ROUTE_BUTTON, OnPlotPartyExtendRouteButton)
	ON_BN_CLICKED(IDC_TRAVEL_BUTTON, OnTravelButton)
	ON_BN_CLICKED(IDC_TRAVEL_MOUNTED_BUTTON, OnTravelMountedButton)
	ON_BN_CLICKED(IDC_PLACE_CHARACTER_BUTTON, OnPlaceCharacterButton)
	ON_BN_CLICKED(IDC_PARTY_CHARACTER_BUTTON, OnPartyCharacterButton)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_PLACE_CACHE_BUTTON, &cDMMapViewDialog::OnBnClickedPlaceCacheButton)
	ON_BN_CLICKED(IDC_PLACE_ON_CACHE_BUTTON, &cDMMapViewDialog::OnBnClickedPlaceOnCacheButton)
	ON_BN_CLICKED(IDC_ADD_CACHE_BUTTON, &cDMMapViewDialog::OnBnClickedAddCacheButton)
	ON_BN_CLICKED(IDC_DELETE_CACHE_BUTTON, &cDMMapViewDialog::OnBnClickedDeleteCacheButton)
	ON_CBN_SELCHANGE(IDC_CACHE_COMBO, &cDMMapViewDialog::OnCbnSelchangeCacheCombo)
	ON_BN_CLICKED(IDC_PLACE_ON_CHARACTER_BUTTON, &cDMMapViewDialog::OnBnClickedPlaceOnCharacterButton)
	ON_EN_CHANGE(IDC_CACHE_DETAILS_EDIT, &cDMMapViewDialog::OnEnChangeCacheDetailsEdit)
	ON_STN_CLICKED(IDC_BDROP, &cDMMapViewDialog::OnStnClickedBdrop)
	ON_BN_CLICKED(IDC_PARTIES_CHECK, &cDMMapViewDialog::OnBnClickedPartiesCheck)
	ON_BN_CLICKED(IDC_ROUTES_CHECK, &cDMMapViewDialog::OnBnClickedRoutesCheck)
	ON_BN_CLICKED(IDC_CHILD_MAPS_CHECK, &cDMMapViewDialog::OnBnClickedChildMapsCheck)
	ON_BN_CLICKED(IDC_CACHES_CHECK, &cDMMapViewDialog::OnBnClickedCachesCheck)
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_HOURS_CHECK, &cDMMapViewDialog::OnBnClickedHoursCheck)
	ON_BN_CLICKED(IDC_WEATHER_CHECK, &cDMMapViewDialog::OnBnClickedWeatherCheck)
	ON_BN_CLICKED(IDC_REST_BUTTON, &cDMMapViewDialog::OnBnClickedRestButton)
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_KEYUP()
	ON_WM_SHOWWINDOW()
	ON_WM_CHILDACTIVATE()
	ON_BN_CLICKED(IDC_LAYER_CHECK_1, &cDMMapViewDialog::OnBnClickedLayerCheck1)
	ON_BN_CLICKED(IDC_LAYER_CHECK_2, &cDMMapViewDialog::OnBnClickedLayerCheck2)
	ON_BN_CLICKED(IDC_LAYER_CHECK_3, &cDMMapViewDialog::OnBnClickedLayerCheck3)
	ON_BN_CLICKED(IDC_LAYER_CHECK_4, &cDMMapViewDialog::OnBnClickedLayerCheck4)
	ON_CBN_SELCHANGE(IDC_SUBPARTY_COMBO, &cDMMapViewDialog::OnCbnSelchangeSubpartyCombo)
	ON_BN_CLICKED(IDC_SUBPARTIES_CHECK, &cDMMapViewDialog::OnBnClickedSubpartiesCheck)
	ON_BN_CLICKED(IDC_CHAR_ICONS_CHECK, &cDMMapViewDialog::OnBnClickedCharIconsCheck)
	ON_BN_CLICKED(IDC_SET_CHARACTER_ICON_BUTTON, &cDMMapViewDialog::OnBnClickedSetCharacterIconButton)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SCALE_SLIDER2, &cDMMapViewDialog::OnNMReleasedcaptureScaleSlider2)
	ON_BN_CLICKED(IDC_ISOMETRIC_CHECK, &cDMMapViewDialog::OnBnClickedIsometricCheck)
	ON_BN_CLICKED(IDC_LABELS_CHECK, &cDMMapViewDialog::OnBnClickedLabelsCheck)
	ON_BN_CLICKED(IDC_DETACH_BUTTON, &cDMMapViewDialog::OnBnClickedDetachButton)
	ON_BN_CLICKED(IDC_FLIP_BUTTON, &cDMMapViewDialog::OnBnClickedFlipButton)
	ON_BN_CLICKED(IDC_FOG_OF_WAR_CHECK, &cDMMapViewDialog::OnBnClickedFogOfWarCheck)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SCALE_SLIDER3, &cDMMapViewDialog::OnNMReleasedcaptureScaleSlider3)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// cDMMapViewDialog message handlers

Bitmap *cDMMapViewDialog::ResourceToBitmap(const HINSTANCE hInstance, const int id)
{
	HBITMAP hBmp = ::LoadBitmap(hInstance, MAKEINTRESOURCE(id));
	if (hBmp == NULL) return NULL;
	return Bitmap::FromHBITMAP(hBmp, NULL);
}

BOOL cDMMapViewDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// memory bitmap 
    //m_pBitmap = new Bitmap(hInstance, L"IDB_KEOLAND_BITMAP");

	//m_pBitmap_1 = new Bitmap(L"c:/DMHelper/maps/furyondy.png", FALSE);

	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_cShowButton.LoadBitmaps(IDB_LARGE_RIGHT_BITMAP, IDB_LARGE_LEFT_BITMAP);
	m_cHideButton.LoadBitmaps(IDB_LARGE_LEFT_BITMAP, IDB_LARGE_RIGHT_BITMAP);

	CFont CourierFont;

	CourierFont.CreatePointFont(90,"Courier New");
	LOGFONT lf; CourierFont.GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	m_ListFont.CreateFontIndirect(&lf);

	m_cCacheDetailsEdit.SetFont(&m_ListFont);

	SetWindowText("New Map");
	
	ShowWindow(SW_SHOW);

	m_bShowPartiesCheck = TRUE;
	m_bShowRoutesCheck = TRUE;
	m_bShowChildMapsCheck = TRUE;
	m_bLabelsCheck = TRUE;

	m_pParent->InvalidateRect(NULL);

	m_cSubPartyCombo.EnableWindow(FALSE);

	Refresh();

	m_nScaleSlider = 4;
	m_cScaleSlider.SetRange(0, 8, TRUE);

	m_nIconScale = m_pDNDMap->m_nMapIconScale;
	m_cIconScale.SetRange(-1000, 1000, TRUE);

	if(m_pDNDMap->m_dwMapID)
	{
		OnHideButton();
	}

	m_cCacheDetailsEdit.ShowWindow(SW_HIDE);

	UpdateData(FALSE);

	SetFocus();

	m_nWindowTimer = SetTimer(1, 500, NULL);

	/*
#if ISOMETRIC_MAPS
	m_cIsometricCheck.ShowWindow(SW_SHOW);
#else
	m_cIsometricCheck.ShowWindow(SW_HIDE);
#endif
	*/

	m_pLightingAlphaBitmap = ResourceToBitmap(AfxGetInstanceHandle(), IDB_LIGHTING_ALPHA_BITMAP);

	#if _PARTICLE_WEATHER
	m_pRainParticleBitmap = ResourceToBitmap(AfxGetInstanceHandle(), IDB_RAIN_BITMAP);
	m_pSnowParticleBitmap = ResourceToBitmap(AfxGetInstanceHandle(), IDB_SNOW_BITMAP);
	#endif

	m_pFogOfWarBitmap = ResourceToBitmap(AfxGetInstanceHandle(), IDB_FOG_OF_WAR_BITMAP);
	m_pSFXButtonBitmap = ResourceToBitmap(AfxGetInstanceHandle(), IDB_SFX_BUTTON_BITMAP);

	if (m_pDNDMap->m_szMapName[0] == 0 && m_pDNDMap->m_szLoadedFilename[0] == 0)
	{
		// OnEditButton();
	}

	#if _PARTICLE_WEATHER
	m_pParticleThread = AfxBeginThread(DMParticleThreadProc, this);
	#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

PDNDPARTYVIEWDLG cDMMapViewDialog::ValidateSelectedParty()
{
	if(m_pSelectedParty != NULL)
	{
		//make sure our selected party hasn't been closed
		for (POSITION pos = m_pApp->m_PartyViewMap.GetStartPosition(); pos != NULL; )
		{
			WORD wID;
			PDNDPARTYVIEWDLG pPartyDlg = NULL;
			m_pApp->m_PartyViewMap.GetNextAssoc(pos,wID,pPartyDlg);

			if(pPartyDlg != NULL && pPartyDlg->m_pParty != NULL)
			{
				if(pPartyDlg->m_pParty == m_pSelectedParty)
				{
					return pPartyDlg;
				}
			}
		}

		//didn't find it, so clear the pointer

		Reset();
	}

	return NULL;
		
}

void cDMMapViewDialog::Reset() 
{
	m_pSelectedParty = NULL;

	m_cPartyCombo.ResetContent();

	Refresh();

	m_cPartyCombo.SetCurSel(-1);
}

void cDMMapViewDialog::Refresh() 
{
	DWORD dwStartRefreshTime = 0;
	TimeSpan("START MAP REFRESH", &dwStartRefreshTime);

	int nRow = 0;
	m_cPartyCombo.ResetContent();

	int nFoundParty = -1;
	for (POSITION pos = m_pApp->m_PartyViewMap.GetStartPosition(); pos != NULL; )
	{
		WORD wID;
		PDNDPARTYVIEWDLG pPartyDlg = NULL;
		m_pApp->m_PartyViewMap.GetNextAssoc(pos,wID,pPartyDlg);

		if(pPartyDlg != NULL && pPartyDlg->m_pParty != NULL)
		{
			if (pPartyDlg->m_dwSubPartyID == 0)
			{
				m_cPartyCombo.InsertString(nRow, pPartyDlg->m_pParty->m_szPartyName);
				m_cPartyCombo.SetItemData(nRow, (ULONG)pPartyDlg->m_pParty);

				if (pPartyDlg->m_pParty == m_pSelectedParty)
				{
					nFoundParty = nRow;
				}

				++nRow;
			}
			
		}
	}

	if(nFoundParty >= 0)
	{
		m_cPartyCombo.SetCurSel(nFoundParty);
	}
	else
	{
		m_pSelectedParty = NULL;
	}

	m_cCacheCombo.ResetContent();
	if(m_pTreasureCaches != NULL)
	{
		nRow = 0;
		int nFoundCache = -1;
		for(int i = 0; i < m_pTreasureCaches->m_Header.m_nCaches; ++i)
		{
			if(m_pTreasureCaches->m_Caches[i].m_dwCacheID != 0)
			{
				m_cCacheCombo.InsertString(nRow, m_pTreasureCaches->m_Caches[i].m_szCacheDesc);
				m_cCacheCombo.SetItemData(nRow, (ULONG)&m_pTreasureCaches->m_Caches[i]);

				if(&m_pTreasureCaches->m_Caches[i] == m_pSelectedCache)
				{
					nFoundCache = i;
				}

				++nRow;
			}
		}

		if(nFoundCache >= 0)
		{
			m_cCacheCombo.SetCurSel(nFoundCache);
		}
		else
		{
			m_pSelectedCache = NULL;
		}
	}

	TimeSpan("END MAP REFRESH", &dwStartRefreshTime);
}

void cDMMapViewDialog::OnPaint() 
{
	if(IsIconic())
	{
		TRACE("POOT!\n");
	}

	m_bMapPaint = TRUE;

	DWORD dwStartPaintTime = 0;
	TimeSpan("START MAP PAINT", &dwStartPaintTime);

	if(m_bCatchMe)
	{
		TRACE("CAUGHT ME !\n");
		//m_bCatchMe = FALSE;
	}

	CWaitCursor myWaitCursor;

	CPaintDC _dc(this); // device context for painting

#if USE_MEMDC
	BOOL bBuffer = TRUE;

	//if(m_fViewScale < 0.75f)
	if (m_fViewScale < 0.0f)
	{
		bBuffer = FALSE;
	}

	CDC* pDC = &_dc;
	CMemDC pmDC(pDC, bBuffer, m_pUpdateRect);
#else
	CDC* pmDC = &_dc;
#endif

	if (m_pParent != NULL || m_bDetachedWindow)
	{
		CRect rect;
		
		if (m_bDetachedWindow == FALSE && m_pParent != NULL)
		{
			m_pParent->GetClientRect(&rect);
			rect.top += 20;

			SetWindowPos(NULL, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);

			GetClientRect(&rect);
		}
		else
		{
			GetClientRect(&rect);
			rect.bottom += 20;
		}
		
		m_cMapLegend.MoveWindow(rect.left, rect.bottom-40, rect.right, 20, TRUE);

		//m_cFlipDisplayButton.MoveWindow(rect.left+10, rect.bottom - 65, rect.left+80, 20, TRUE);
		//m_cDetachButton.MoveWindow(rect.left+10, rect.bottom - 90, rect.left+80, 20, TRUE);
		m_cDetachButton.MoveWindow(rect.left + 10, rect.bottom - 65, rect.left + 80, 20, TRUE);
		
		m_cTravelButton.MoveWindow(rect.right-153, rect.top+2, 150, 35, TRUE);
		m_cTravelMountedButton.MoveWindow(rect.right-153, rect.top+45, 150, 35, TRUE);
		m_cTravelHoursCheck.MoveWindow(rect.right-280, rect.top+2, 100, 20, TRUE);
		m_cWeatherCheck.MoveWindow(rect.right-390, rect.top+2, 100, 20, TRUE);

		m_cRestButton.MoveWindow(rect.right-153, rect.top+88, 150, 35, TRUE);

		if(m_bTravelHoursCheck == TRUE)
		{
			m_cTravelButton.SetWindowText("Travel 1 Hour");
			m_cTravelMountedButton.SetWindowText("Travel Mounted 1 Hour");
		}
		else
		{
			m_cTravelButton.SetWindowText("Travel 1 Day");
			m_cTravelMountedButton.SetWindowText("Travel Mounted 1 Day");
		}
	}

	ValidateSelectedParty();

	if (m_pSelectedParty == NULL)
	{
		m_cCharacterCombo.EnableWindow(FALSE);
	}
	else
	{
		m_cCharacterCombo.EnableWindow(TRUE);
	}

	if(m_pSelectedParty == NULL || m_pDNDMap->m_bMapScaleFeet)
	{
		m_cTravelButton.ShowWindow(SW_HIDE);
		m_cTravelMountedButton.ShowWindow(SW_HIDE);
		m_cTravelHoursCheck.ShowWindow(SW_HIDE);
		m_cWeatherCheck.ShowWindow(SW_HIDE);
		m_cRestButton.ShowWindow(SW_HIDE);
	}
	else
	{
		m_cTravelButton.ShowWindow(SW_SHOW);
		m_cTravelMountedButton.ShowWindow(SW_SHOW);
		m_cTravelHoursCheck.ShowWindow(SW_SHOW);
		m_cWeatherCheck.ShowWindow(SW_SHOW);
		m_cRestButton.ShowWindow(SW_SHOW);
	}

	m_bLayer1 = m_pDNDMap->m_bDisplayLayer[0];
	m_bLayer2 = m_pDNDMap->m_bDisplayLayer[1];
	m_bLayer3 = m_pDNDMap->m_bDisplayLayer[2];
	m_bLayer4 = m_pDNDMap->m_bDisplayLayer[3];

	CRect cRect;
	GetClientRect (cRect);

	#if USE_MEMDC
	Graphics graphics(pmDC);
	#else
	Graphics graphics(dc);
	#endif

	Color blackColor(255, 0, 0, 0);
	graphics.Clear(blackColor);

	if(m_pDNDMap->m_nRows == 0 || m_pDNDMap->m_nColumns == 0)
	{
		m_bMapPaint = FALSE;
		return;
	}

	if(m_pDNDMap->m_nPixelSizeX == 0 || m_pDNDMap->m_nPixelSizeY == 0)
	{
		m_bMapPaint = FALSE;
		return;
	}

	CFont newfont;
	CFont* oldfont;

	int nFSizeX = 16;   // 12 point font
	int nFSizeY = 6;
	//BOOL success = newfont.CreatePointFont(nSize * 10, "ArialBlack", &dc);
	newfont.CreateFont(nFSizeX,nFSizeY,0,FW_BLACK,FALSE,FALSE,FALSE,0,ARABIC_CHARSET,OUT_TT_PRECIS,
	CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_ROMAN,_T("Arial"));

	oldfont = pmDC->SelectObject(&newfont);

	int nCellX = m_nCornerX / m_pDNDMap->m_nPixelSizeX;
	int nCellY = m_nCornerY / m_pDNDMap->m_nPixelSizeY;

	CString szTemp;

	for(int xx = 0; xx < m_pDNDMap->m_nColumns; ++xx)
	{
		for(int yy = 0; yy < m_pDNDMap->m_nRows; ++yy)
		{
			cDNDMapCell *pCell = &m_pDNDMap->m_Cells[xx][yy];

			int nPixelSizeX = (int)(m_pDNDMap->m_nPixelSizeX * m_fViewScale);
			int nPixelSizeY = (int)(m_pDNDMap->m_nPixelSizeY * m_fViewScale);

			int x1 = xx * nPixelSizeX + m_nCornerX;
			int y1 = yy * nPixelSizeY + m_nCornerY;

			int x2 = x1 + nPixelSizeX;
			int y2 = y1 + nPixelSizeY;

			//
#if 0
			CString szFaker;
			//szFaker.Format("<$DMAPATH>\\DATA\\maps\\bitmaps\\flanaess\\flanaess_%d.jpg",  xx + yy * m_pDNDMap->m_nColumns);
			//szFaker.Format("<$DMAPATH>\\DATA\\maps\\bitmaps\\azril\\azril-%d.png",  xx + yy * m_pDNDMap->m_nColumns);
			
			// for importing ProFantasy fractal terrains data
			char sHex[16] = {'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};

			if(yy > -1) //(yy >= 4 && yy <= 6)
			{
				szFaker.Format("<$DMAPATH>\\DATA\\maps\\bitmaps\\Ara\\b00%c00%c.jpg",  sHex[15-xx], sHex[15-yy]);
			}
			else
			{
				szFaker = _T("");
			}


			szFaker.MakeUpper();
			strcpy(pCell->m_szBitmapPath, szFaker.GetBuffer(0));
#endif

			BOOL bInRegion = FALSE;
			if(x1 <= cRect.left && y1 <= cRect.top && x2 >= cRect.right && y2 >= cRect.bottom) //view is completely inside cell
			{
				bInRegion = TRUE;
			}

			//lines
			if(x1 <= cRect.left && y1 >= cRect.top && x2 >= cRect.right && y1 <= cRect.bottom) //top line is inside view
			{
				bInRegion = TRUE;
			}

			if(x1 <= cRect.left && y2 >= cRect.top && x2 >= cRect.right && y2 <= cRect.bottom) //bottom line is inside view
			{
				bInRegion = TRUE;
			}

			if(x1 >= cRect.left && y1 <= cRect.top && x1 <= cRect.right && y2 >= cRect.bottom) //left line is inside view
			{
				bInRegion = TRUE;
			}

			if(x2 >= cRect.left && y1 <= cRect.top && x2 <= cRect.right && y2 >= cRect.bottom) //right line is inside view
			{
				bInRegion = TRUE;
			}


			//corners
			if(x1 >= cRect.left && y1 >= cRect.top && x1 <= cRect.right && y1 <= cRect.bottom) //top left corner is inside view
			{
				bInRegion = TRUE;
			}

			if(x2 >= cRect.left && y1 >= cRect.top && x2 <= cRect.right && y1 <= cRect.bottom) //top right corner is inside view
			{
				bInRegion = TRUE;
			}

			if(x1 >= cRect.left && y2 >= cRect.top && x1 <= cRect.right && y2 <= cRect.bottom) //bottom left corner is inside view
			{
				bInRegion = TRUE;
			}

			if(x2 >= cRect.left && y2 >= cRect.top && x2 <= cRect.right && y2 <= cRect.bottom) //bottom right corner is inside view
			{
				bInRegion = TRUE;
			}


			if(bInRegion == FALSE)
			{
				if(pCell->m_pBitmap != NULL)
				{
					delete pCell->m_pBitmap;
					pCell->m_pBitmap = NULL;
					//TRACE(" %d %d OUT OF REGION\n", xx, yy);
				}
			}

			if(pCell->m_szBitmapPath[0] && pCell->m_pBitmap == NULL && bInRegion == TRUE)
			{
				CString szPath;
				szPath.Format("%s", pCell->m_szBitmapPath);
				szPath.MakeUpper();
				szPath.Replace("<$DMAPATH>", m_pApp->m_szEXEPath);

				LPWSTR wcsFile = szPath.AllocSysString();
				pCell->m_pBitmap = new Bitmap(wcsFile , FALSE);	

				CheckLayer(m_bLayer1, 1, pCell->m_szBitmapPath, pCell->m_pBitmap);
				CheckLayer(m_bLayer2, 2, pCell->m_szBitmapPath, pCell->m_pBitmap);
				CheckLayer(m_bLayer3, 3, pCell->m_szBitmapPath, pCell->m_pBitmap);
				CheckLayer(m_bLayer4, 4, pCell->m_szBitmapPath, pCell->m_pBitmap);
				
				#if 0
				//ADD A LAYER ?
				if (szPath.Find("B007008") >= 0 || szPath.Find("KEOLAND") >= 0)
				{
					//CString szLayer = "C:\\tmp\\B007008_L1.PNG";
					//CString szLayer = "C:\\tmp\\BOX.PNG";
					CString szLayer = "C:\\tmp\\BOX.BMP";
					wcsFile = szLayer.AllocSysString();
					Bitmap *pLayerBitmap = new Bitmap(wcsFile, FALSE);

					Status rc;
					Graphics* g = Graphics::FromImage(pCell->m_pBitmap);

					ImageAttributes imAttr;
					imAttr.SetColorKey(Color(255, 255, 255), Color(255, 255, 255), ColorAdjustTypeBitmap);

					Rect destRect(0, 0, pLayerBitmap->GetWidth() - 1, pLayerBitmap->GetHeight() - 1);

					int XX = pLayerBitmap->GetWidth();

					rc = g->DrawImage(pLayerBitmap, destRect, 0, 0, pLayerBitmap->GetWidth() - 1, pLayerBitmap->GetHeight() - 1, Gdiplus::UnitPixel, &imAttr);
					
					delete pLayerBitmap;
					
				}
				#endif
			}
			
			if(pCell->m_pBitmap == NULL)
			{
				Pen pen(Color(255, 255, 0, 0));

				graphics.DrawLine(&pen, x1, y1, x2, y1);
				graphics.DrawLine(&pen, x2, y1, x2, y2);
				graphics.DrawLine(&pen, x1, y1, x1, y2);
				graphics.DrawLine(&pen, x1, y2, x2, y2);

				szTemp.Format("CELL %d", xx + yy * m_pDNDMap->m_nColumns);

				SetTextColor(pmDC, 0x0000FF);
				DrawMapText(szTemp.GetBuffer(0), x1+6, y1+6, pmDC);
			}
			else
			{
				graphics.DrawImage(pCell->m_pBitmap, x1, y1, nPixelSizeX, nPixelSizeY);
				
				if(m_pMapEditDlg != NULL)
				{
					Pen pen(Color(255, 255, 0, 0));

					graphics.DrawLine(&pen, x1, y1, x2, y1);
					graphics.DrawLine(&pen, x2, y1, x2, y2);
					graphics.DrawLine(&pen, x1, y1, x1, y2);
					graphics.DrawLine(&pen, x1, y2, x2, y2);

					szTemp.Format("CELL %d : %s", xx + yy * m_pDNDMap->m_nColumns, pCell->m_szBitmapPath);
					szTemp.Replace("<$DMAPATH>", ".\\");

					SetTextColor(pmDC, 0x0000FF);
					DrawMapText(szTemp.GetBuffer(0), x1+6, y1+6, pmDC);
				}
			}

		}
	}

	
	if(m_pDNDMap->m_bTiles)
	{
		m_cIsometricCheck.EnableWindow(TRUE);

		TimeSpan("TIME MAP PAINT : TILESTART", &dwStartPaintTime);
		DrawMapTiles(&graphics);
		TimeSpan("TIME MAP PAINT : TILEEND", &dwStartPaintTime);
	}
	else
	{
		m_cIsometricCheck.EnableWindow(FALSE);
	}

	SetBkMode(pmDC, TRANSPARENT);
	Pen pen(Color(255, 255, 0, 0), 5);
	Pen pen2(Color(255, 255, 0, 255), 3);
	Pen pen3(Color(255, 128, 128, 0), 3);
	Pen pen4(Color(255, 255, 255, 255), 2);
	Pen pen5(Color(255, 255, 0, 0), 2);


	//////////////////////////////////
	for (POSITION pos = m_pApp->m_MapViewMap.GetStartPosition(); pos != NULL; )
	{
		WORD wID;
		PDNDMAPVIEWDLG pMapDlg = NULL;
		m_pApp->m_MapViewMap.GetNextAssoc(pos,wID,pMapDlg);

		if(pMapDlg != NULL && pMapDlg->m_pDNDMap != NULL && pMapDlg->m_pDNDMap->m_dwParentMapID == m_pDNDMap->m_dwMapID)
		{
			float fX = 0.0f;
			float fY = 0.0f;

			float eX = 0.0f;
			float eY = 0.0f;

			if(pMapDlg->m_pDNDMap->m_bMapScaleFeet)
			{
				//feet to pixels
				fX = pMapDlg->m_pDNDMap->m_fParentMapOriginX / m_pDNDMap->m_fScaleX * m_fViewScale;
				fY = pMapDlg->m_pDNDMap->m_fParentMapOriginY / m_pDNDMap->m_fScaleY * m_fViewScale;

				eX = fX;
				eY = fY;
			}
			else
			{
				//miles to pixels
				fX =  ((pMapDlg->m_pDNDMap->m_fParentMapOriginX - m_pDNDMap->m_fParentMapOriginX) / m_pDNDMap->m_fScaleX) * m_fViewScale;
				fY =  ((pMapDlg->m_pDNDMap->m_fParentMapOriginY - m_pDNDMap->m_fParentMapOriginY) / m_pDNDMap->m_fScaleY) * m_fViewScale;

				eX = fX + ((pMapDlg->m_pDNDMap->m_nColumns * pMapDlg->m_pDNDMap->m_nPixelSizeX * pMapDlg->m_pDNDMap->m_fScaleX / m_pDNDMap->m_fScaleX) * m_fViewScale); 
				eY = fY + ((pMapDlg->m_pDNDMap->m_nRows * pMapDlg->m_pDNDMap->m_nPixelSizeY * pMapDlg->m_pDNDMap->m_fScaleY / m_pDNDMap->m_fScaleX) * m_fViewScale); 
			}

			int nX =  (int)fX;
			int nY =  (int)fY;

			nX += m_nCornerX;
			nY += m_nCornerY;

			int tX = (int)eX;
			int tY = (int)eY;

			tX += m_nCornerX;
			tY += m_nCornerY;

			if(m_bShowChildMapsCheck)
			{
				DrawChildMap(&graphics, pMapDlg->m_pDNDMap, nX, nY);

				graphics.DrawLine(&pen4, nX, nY, tX, nY);
				graphics.DrawLine(&pen4, nX, nY, nX, tY);
				graphics.DrawLine(&pen4, nX, tY, tX, tY);
				graphics.DrawLine(&pen4, tX, nY, tX, tY);

				//TRACE("TXT >>> %d %d\n", nX, nY);

				SetTextColor(pmDC, 0x000000);
				DrawMapText(pMapDlg->m_pDNDMap->m_szMapName, nX+3, nY-16, pmDC);
				SetTextColor(pmDC, 0xFFFFFF);
				DrawMapText(pMapDlg->m_pDNDMap->m_szMapName, nX+4, nY-17, pmDC);
			}


		}
	}

	if(m_bCatchMe)
	{
		TRACE("CAUGHT ME !\n");
		//m_bCatchMe = FALSE;
	}

	if (m_bFogOfWarCheck)
	{
		int nCols = m_pDNDMap->m_nColumns;
		int nRows = m_pDNDMap->m_nRows;

		int nRemX = (m_pDNDMap->m_nPixelSizeX * nCols) % 32;
		int nRemY = (m_pDNDMap->m_nPixelSizeY * nRows) % 32;

		int nFOWSizeX = ((m_pDNDMap->m_nPixelSizeX * nCols + nRemX) / 32)+1;
		int nFOWSizeY = ((m_pDNDMap->m_nPixelSizeY * nRows + nRemY) / 32)+1;

		int nPixelSizeX = (int)(32.0f * m_fViewScale);
		int nPixelSizeY = (int)(32.0f * m_fViewScale);

		for (int xx = 0; xx < nFOWSizeX; ++xx)
		{
			for (int yy = 0; yy < nFOWSizeY; ++yy)
			{
				int x1 = xx * nPixelSizeX + m_nCornerX;
				int y1 = yy * nPixelSizeY + m_nCornerY;

				int x2 = x1 + nPixelSizeX;
				int y2 = y1 + nPixelSizeY;

				if (NULL != m_pFogOfWarBitmap)
				{
					if (m_pDNDMap->m_nFogOfWarCell[yy % 100][xx % 100] == 0)
					{
						//graphics.DrawImage(m_pFogOfWarBitmap, x1, y1, nPixelSizeX, nPixelSizeY);
						float fAlpha = 0.7f;
						if (m_bDetachedWindow)
						{
							fAlpha = 1.0f;
						}

						DrawTransparentBitmap(&graphics, m_pFogOfWarBitmap, x1, y1, nPixelSizeX, nPixelSizeY, 32,32, fAlpha);
					}
				}
			}
		}
	}

	//////////////////////////////////

	DrawMapSFX(&graphics);

	//////////////////////////////////

	#if _PARTICLE_WEATHER
	if (m_pParticleBufferBitmap != NULL)
	{
		delete m_pParticleBufferBitmap;
		m_pParticleBufferBitmap = NULL;
	}
	#endif

	/////////////////////////////////


	if (NULL != m_pLightingAlphaBitmap && m_fLightingAlpha > 0.0f)
	{
		//nighttime ?
		DrawTransparentBitmap(&graphics, m_pLightingAlphaBitmap, 0, 0, m_pDNDMap->m_nPixelSizeX * m_pDNDMap->m_nColumns, m_pDNDMap->m_nPixelSizeY *m_pDNDMap->m_nRows, 32, 32, m_fLightingAlpha);
	}

	//////////////////////////////////

	m_nMapParties = 0;
	memset(m_PartyHotSpots, 0, MAX_HOTSPOTS * sizeof(CDMPartyHotSpot));

	for (pos = m_pApp->m_PartyViewMap.GetStartPosition(); pos != NULL; )
	{
		WORD wID;
		PDNDPARTYVIEWDLG pPartyDlg = NULL;
		m_pApp->m_PartyViewMap.GetNextAssoc(pos,wID,pPartyDlg);

		if(pPartyDlg != NULL && pPartyDlg->m_pParty != NULL)
		{
			float fX = 0.0f;
			float fY = 0.0f;

			if(m_pDNDMap->m_bMapScaleFeet)
			{
				//feet to pixels
				fX = pPartyDlg->m_pParty->m_fPartyLocalLocationX / m_pDNDMap->m_fScaleX * m_fViewScale;
				fY = pPartyDlg->m_pParty->m_fPartyLocalLocationY / m_pDNDMap->m_fScaleY * m_fViewScale;
			}
			else
			{
				//miles to pixels
				fX =  ((pPartyDlg->m_pParty->m_fPartyLocationX - m_pDNDMap->m_fParentMapOriginX) / m_pDNDMap->m_fScaleX) * m_fViewScale;
				fY =  ((pPartyDlg->m_pParty->m_fPartyLocationY - m_pDNDMap->m_fParentMapOriginY) / m_pDNDMap->m_fScaleY) * m_fViewScale;
			}

			int nX =  (int)fX;
			int nY =  (int)fY;

			nX += m_nCornerX;
			nY += m_nCornerY;

			if(m_bShowPartiesCheck)
			{
				graphics.DrawLine(&pen, nX-10, nY, nX+10, nY);
				graphics.DrawLine(&pen, nX, nY-10, nX, nY+10);

				//TRACE("TXT >>> %d %d\n", nX, nY);

				SetTextColor(pmDC, 0x000000);
				DrawMapText(pPartyDlg->m_pParty->m_szPartyName, nX+5, nY+5, pmDC);
				SetTextColor(pmDC, 0x0000FF);
				DrawMapText(pPartyDlg->m_pParty->m_szPartyName, nX+6, nY+6, pmDC);
			
				m_PartyHotSpots[m_nMapParties].m_nX = nX;
				m_PartyHotSpots[m_nMapParties].m_nY = nY;
				m_PartyHotSpots[m_nMapParties].m_pParty = pPartyDlg->m_pParty;

				++m_nMapParties;
			}


			if(m_pDNDMap->m_bMapScaleFeet)
			{
			}
			else
			{
				if(pPartyDlg->m_pParty == m_pSelectedParty)
				{
					m_fSelectedPartyRouteDistance = 0.0f;
				}

				//party routes
				if(pPartyDlg->m_pParty->m_WayPoints[1].m_fX != 0.0f || pPartyDlg->m_pParty->m_WayPoints[1].m_fY != 0.0f && !m_pDNDMap->m_bMapScaleFeet)
				{
					pPartyDlg->m_pParty->m_WayPoints[0].m_fX = pPartyDlg->m_pParty->m_fPartyLocationX;
					pPartyDlg->m_pParty->m_WayPoints[0].m_fY = pPartyDlg->m_pParty->m_fPartyLocationY;

					for(int i = 0; i < MAX_PARTY_WAYPOINTS-1; ++i)
					{
						if(pPartyDlg->m_pParty->m_WayPoints[i+1].m_fX == 0.0f && pPartyDlg->m_pParty->m_WayPoints[i+1].m_fY == 0.0f)
						{
							break;
						}
						else
						{
							float fX1 =  ((pPartyDlg->m_pParty->m_WayPoints[i].m_fX - m_pDNDMap->m_fParentMapOriginX) / m_pDNDMap->m_fScaleX) * m_fViewScale;
							float fY1 =  ((pPartyDlg->m_pParty->m_WayPoints[i].m_fY - m_pDNDMap->m_fParentMapOriginY) / m_pDNDMap->m_fScaleY) * m_fViewScale;

							float fX2 =  ((pPartyDlg->m_pParty->m_WayPoints[i+1].m_fX - m_pDNDMap->m_fParentMapOriginX) / m_pDNDMap->m_fScaleX) * m_fViewScale;
							float fY2 =  ((pPartyDlg->m_pParty->m_WayPoints[i+1].m_fY - m_pDNDMap->m_fParentMapOriginY) / m_pDNDMap->m_fScaleY) * m_fViewScale;

							if(pPartyDlg->m_pParty == m_pSelectedParty)
							{
								m_fSelectedPartyRouteDistance += GetDistance(pPartyDlg->m_pParty->m_WayPoints[i].m_fX, pPartyDlg->m_pParty->m_WayPoints[i].m_fY, pPartyDlg->m_pParty->m_WayPoints[i+1].m_fX, pPartyDlg->m_pParty->m_WayPoints[i+1].m_fY);
							}

							int nX1 =  (int)fX1 + m_nCornerX;
							int nY1 =  (int)fY1 + m_nCornerY;

							int nX2 =  (int)fX2 + m_nCornerX;
							int nY2 =  (int)fY2 + m_nCornerY;

							if(m_bShowRoutesCheck)
							{
								graphics.DrawEllipse(&pen2, nX1-3, nY1-3, 6 , 6);
								graphics.DrawLine(&pen2, nX1, nY1, nX2, nY2);
							}

						}

					}
				}
			}

		}

	}

#if 1
	//////////////////////////////////

	m_nMapCharacters = 0;
	memset(m_CharacterHotSpots, 0, MAX_HOTSPOTS * sizeof(CDMCharacterHotSpot));

	for (pos = m_pApp->m_CharacterViewMap.GetStartPosition();  pos != NULL; )
	{
		WORD wID;
		PDNDCHARVIEWDLG pCharDlg = NULL;
		m_pApp->m_CharacterViewMap.GetNextAssoc(pos,wID,pCharDlg);

		if(pCharDlg != NULL && pCharDlg->m_pCharacter != NULL)
		{
			float fX = 0.0f;
			float fY = 0.0f;

			if(m_pDNDMap->m_bMapScaleFeet)
			{
				//feet to pixels
				fX = pCharDlg->m_pCharacter->m_fLocalLocationX / m_pDNDMap->m_fScaleX * m_fViewScale;
				fY = pCharDlg->m_pCharacter->m_fLocalLocationY / m_pDNDMap->m_fScaleY * m_fViewScale;
			}
			else
			{
				//miles to pixels
				fX =  ((pCharDlg->m_pCharacter->m_fMapLocationX - m_pDNDMap->m_fParentMapOriginX) / m_pDNDMap->m_fScaleX) * m_fViewScale;
				fY =  ((pCharDlg->m_pCharacter->m_fMapLocationY - m_pDNDMap->m_fParentMapOriginY) / m_pDNDMap->m_fScaleY) * m_fViewScale;
			}

			if(fabs(fX + fY) < 1.0f)
				continue;

			int nX =  (int)fX;
			int nY =  (int)fY;

			nX += m_nCornerX;
			nY += m_nCornerY;

			if (m_bIsometricCheck)
			{
				fX = pCharDlg->m_pCharacter->m_fLocalLocationX / m_pDNDMap->m_fScaleX + 64;
				fY = pCharDlg->m_pCharacter->m_fLocalLocationY / m_pDNDMap->m_fScaleY + 32;

				Translate2DCoordinatesToIsometric((int)fX, (int)fY, m_fViewScale, &nX, &nY);
			}

			if (IsOnScreen(nX, nY))
			{
				if (m_bCharIcons)
				{
					DrawCharacterIcon(&graphics, nX, nY, pCharDlg->m_pCharacter->m_szIconPath, pCharDlg->m_bReversedMapIcon);
				}

				if (m_bShowPartiesCheck)
				{
					graphics.DrawLine(&pen, nX, nY - 7, nX + 7, nY + 7);
					graphics.DrawLine(&pen, nX - 7, nY + 7, nX + 7, nY + 7);
					graphics.DrawLine(&pen, nX, nY - 7, nX - 7, nY + 7);

					szTemp.Format("AC: %d HP: %d", pCharDlg->m_pCharacter->m_nCurrentArmorClass, pCharDlg->m_pCharacter->m_nHitPoints - pCharDlg->m_pCharacter->m_nCurrentDamage);

					if (m_bLabelsCheck)
					{
						SetTextColor(pmDC, 0x000000);
						DrawMapText(pCharDlg->m_pCharacter->m_szCharacterName, nX + 5, nY + 5, pmDC);
						DrawMapText(szTemp.GetBuffer(0), nX + 5, nY + 5 + 15, pmDC);
						SetTextColor(pmDC, 0x0000FF);
						DrawMapText(pCharDlg->m_pCharacter->m_szCharacterName, nX + 6, nY + 6, pmDC);
						DrawMapText(szTemp.GetBuffer(0), nX + 6, nY + 6 + 15, pmDC);
					}
				}
			}

			m_CharacterHotSpots[m_nMapCharacters].m_nX = nX;
			m_CharacterHotSpots[m_nMapCharacters].m_nY = nY;
			m_CharacterHotSpots[m_nMapCharacters].m_dwCharacterID = pCharDlg->m_pCharacter->m_dwCharacterID;
			++m_nMapCharacters;
	
		}
	}


	if(m_bCatchMe)
	{
		TRACE("CAUGHT ME !\n");
		//m_bCatchMe = FALSE;
	}

	for (pos = m_pApp->m_NPCViewMap.GetStartPosition(); (m_bShowPartiesCheck == TRUE || m_bShowCachesCheck == TRUE) && pos != NULL; )
	{
		WORD wID;
		PDNDNPCVIEWDLG pNPCDlg = NULL;
		m_pApp->m_NPCViewMap.GetNextAssoc(pos,wID,pNPCDlg);

		if(pNPCDlg != NULL && pNPCDlg->m_pNPC != NULL && pNPCDlg->m_pNPC->m_bIsCache == FALSE)
		{
			float fX = 0.0f;
			float fY = 0.0f;

			if(m_pDNDMap->m_bMapScaleFeet)
			{
				//feet to pixels
				fX = pNPCDlg->m_pNPC->m_fLocalLocationX / m_pDNDMap->m_fScaleX * m_fViewScale;
				fY = pNPCDlg->m_pNPC->m_fLocalLocationY / m_pDNDMap->m_fScaleY * m_fViewScale;
			}
			else
			{
				//miles to pixels
				fX =  ((pNPCDlg->m_pNPC->m_fMapLocationX - m_pDNDMap->m_fParentMapOriginX) / m_pDNDMap->m_fScaleX) * m_fViewScale;
				fY =  ((pNPCDlg->m_pNPC->m_fMapLocationY - m_pDNDMap->m_fParentMapOriginY) / m_pDNDMap->m_fScaleY) * m_fViewScale;
			}

			if(fabs(fX + fY) < 1.0f)
				continue;

			int nX =  (int)fX;
			int nY =  (int)fY;

			nX += m_nCornerX;
			nY += m_nCornerY;

			if (m_bIsometricCheck)
			{
				// kieran
				fX = pNPCDlg->m_pNPC->m_fLocalLocationX / m_pDNDMap->m_fScaleX + 64;
				fY = pNPCDlg->m_pNPC->m_fLocalLocationY / m_pDNDMap->m_fScaleY + 32;

				Translate2DCoordinatesToIsometric((int)fX, (int)fY, m_fViewScale, &nX, &nY);
			}

			if (IsOnScreen(nX, nY))
			{
				if (m_bCharIcons)
				{
					DrawMonsterIcon(&graphics, nX, nY, pNPCDlg->m_pNPC, pNPCDlg->m_bReversedMapIcon);
				}

				if (m_bShowPartiesCheck)
				{
					graphics.DrawLine(&pen, nX - 7, nY - 7, nX + 7, nY - 7);
					graphics.DrawLine(&pen, nX, nY + 7, nX - 7, nY - 7);
					graphics.DrawLine(&pen, nX, nY + 7, nX + 7, nY - 7);

					szTemp.Format("AC: %d HP: %d", pNPCDlg->m_pNPC->m_nCurrentArmorClass, pNPCDlg->m_pNPC->m_nHitPoints - pNPCDlg->m_pNPC->m_nCurrentDamage);

					if (m_bLabelsCheck)
					{
						SetTextColor(pmDC, 0x000000);
						DrawMapText(pNPCDlg->m_pNPC->m_szCharacterName, nX + 5, nY + 5, pmDC);
						DrawMapText(szTemp.GetBuffer(0), nX + 5, nY + 5 + 15, pmDC);
						SetTextColor(pmDC, 0x0000FF);
						DrawMapText(pNPCDlg->m_pNPC->m_szCharacterName, nX + 6, nY + 6, pmDC);
						DrawMapText(szTemp.GetBuffer(0), nX + 6, nY + 6 + 15, pmDC);
					}
				}
			}

			m_CharacterHotSpots[m_nMapCharacters].m_nX = nX;
			m_CharacterHotSpots[m_nMapCharacters].m_nY = nY;
			m_CharacterHotSpots[m_nMapCharacters].m_dwCharacterID = pNPCDlg->m_pNPC->m_dwCharacterID;
			++m_nMapCharacters;
			
		}

		// draw map caches

		m_nMapCaches = 0;
		memset(m_CacheHotSpots, 0, MAX_HOTSPOTS * sizeof(CDMCacheHotSpot));

		if(m_pTreasureCaches != NULL)
		{
			for (int i=0; i < MAX_HOTSPOTS; ++i )
			{
				if(m_pTreasureCaches->m_Caches[i].m_dwCacheID == 0)
					break;

				cDNDCacheInventory *pCache = &m_pTreasureCaches->m_Caches[i];

				if (pCache->m_szCacheDesc[0])
				{
					float fX = 0.0f;
					float fY = 0.0f;

					if (m_pDNDMap->m_bMapScaleFeet)
					{
						//feet to pixels
						fX = pCache->m_fLocalLocationX / m_pDNDMap->m_fScaleX * m_fViewScale;
						fY = pCache->m_fLocalLocationY / m_pDNDMap->m_fScaleY * m_fViewScale;
					}
					else
					{
						//miles to pixels
						fX = ((pCache->m_fMapLocationX - m_pDNDMap->m_fParentMapOriginX) / m_pDNDMap->m_fScaleX) * m_fViewScale;
						fY = ((pCache->m_fMapLocationY - m_pDNDMap->m_fParentMapOriginY) / m_pDNDMap->m_fScaleY) * m_fViewScale;
					}

					if (fabs(fX + fY) < 1.0f)
						continue;

					int nX = (int)fX;
					int nY = (int)fY;

					nX += m_nCornerX;
					nY += m_nCornerY;

					if (m_bIsometricCheck)
					{
						// kieran
						fX = pCache->m_fLocalLocationX / m_pDNDMap->m_fScaleX + 64;
						fY = pCache->m_fLocalLocationY / m_pDNDMap->m_fScaleY + 32;

						Translate2DCoordinatesToIsometric((int)fX, (int)fY, m_fViewScale, &nX, &nY);
					}

					if (IsOnScreen(nX, nY))
					{
						if (m_bShowCachesCheck)
						{
							int nBoxSize = 5;
							graphics.DrawLine(&pen3, nX - nBoxSize, nY - nBoxSize, nX + nBoxSize, nY - nBoxSize);
							graphics.DrawLine(&pen3, nX - nBoxSize, nY + nBoxSize, nX + nBoxSize, nY + nBoxSize);
							graphics.DrawLine(&pen3, nX - nBoxSize, nY - nBoxSize, nX - nBoxSize, nY + nBoxSize);
							graphics.DrawLine(&pen3, nX + nBoxSize, nY - nBoxSize, nX + nBoxSize, nY + nBoxSize);
						}
					
						if (m_bShowCachesCheck && m_bLabelsCheck)
						{
							SetTextColor(pmDC, 0x000000);
							DrawMapText(pCache->m_szCacheDesc, nX + 5, nY + 5, pmDC);

							SetTextColor(pmDC, 0x008080);
							DrawMapText(pCache->m_szCacheDesc, nX + 6, nY + 6, pmDC);
						}
					}

					m_CacheHotSpots[m_nMapCaches].m_nX = nX;
					m_CacheHotSpots[m_nMapCaches].m_nY = nY;
					m_CacheHotSpots[m_nMapCaches].m_pCache = pCache;

					m_nMapCaches = (m_nMapCaches + 1) % MAX_HOTSPOTS;

					if (m_bCatchMe)
					{
						TRACE("CAUGHT ME !\n");
						//m_bCatchMe = FALSE;
					}
				}
			}

		} //if(m_pTreasureCaches != NULL)
	}


	if(m_bMouseDrag)
	{
		float fMouseDistance = GetDistance((float)m_DragPoint.x, (float)m_DragPoint.y, (float)m_MousePoint.x, (float)m_MousePoint.y);
		int nRangeDistance = (int)fMouseDistance * 2;

		graphics.DrawEllipse(&pen5, m_DragPoint.x-nRangeDistance/2, m_DragPoint.y-nRangeDistance/2, nRangeDistance, nRangeDistance);

		graphics.DrawLine(&pen4, m_DragPoint.x, m_DragPoint.y, m_MousePoint.x, m_MousePoint.y);

		graphics.DrawLine(&pen4, m_DragPoint.x-2, m_DragPoint.y, m_DragPoint.x+2, m_DragPoint.y);
		graphics.DrawLine(&pen4, m_DragPoint.x, m_DragPoint.y-2, m_DragPoint.x, m_DragPoint.y+2);

		graphics.DrawLine(&pen4, m_MousePoint.x-2, m_MousePoint.y, m_MousePoint.x+2, m_MousePoint.y);
		graphics.DrawLine(&pen4, m_MousePoint.x, m_MousePoint.y-2, m_MousePoint.x, m_MousePoint.y+2);

		

		fMouseDistance = fMouseDistance * m_pDNDMap->m_fScaleX / m_fViewScale;

		if(m_pDNDMap->m_bMapScaleFeet)
		{
			szTemp.Format("Dist = %0.2f feet", fMouseDistance);
		}
		else
		{
			szTemp.Format("Dist = %0.2f miles", fMouseDistance);
		}

		SetTextColor(pmDC, 0x000000);
		DrawMapText(szTemp.GetBuffer(0), m_MousePoint.x+5, m_MousePoint.y, pmDC);
		SetTextColor(pmDC, 0xFFFFFF);
		DrawMapText(szTemp.GetBuffer(0), m_MousePoint.x+6, m_MousePoint.y+1, pmDC);

	}

#endif

	if(m_pUpdateRect != NULL)
	{
		delete m_pUpdateRect;
		m_pUpdateRect = NULL;
	}


	ProcessMapModes();

	m_bMapPaint = FALSE;

	TimeSpan("END MAP PAINT", &dwStartPaintTime);

}


void cDMMapViewDialog::DrawChildMap(Graphics *graphics, cDNDMap *pDNDChildMap, int nX, int nY)
{
	TRACE("SHATNER!\n");

	if (pDNDChildMap->m_bMapScaleFeet == 0)
		return;

	float fScale = (pDNDChildMap->m_fScaleX / 5280.0f / m_pDNDMap->m_fScaleX) * m_fViewScale;

	float fSizeX = (float)(pDNDChildMap->m_nPixelSizeX) * fScale;
	float fSizeY = (float)(pDNDChildMap->m_nPixelSizeY) * fScale;

	if (fSizeX < 16.0f)
	{
		return;
	}

	for (int i = 0; i < pDNDChildMap->m_nColumns; ++i)
	{
		for (int j = 0; j < pDNDChildMap->m_nRows; ++j)
		{
			cDNDMapCell *pCell = &pDNDChildMap->m_Cells[i][j];

			if (pCell->m_pBitmap == NULL)
			{
				CString szPath;
				szPath.Format("%s", pCell->m_szBitmapPath);
				szPath.MakeUpper();
				szPath.Replace("<$DMAPATH>", m_pApp->m_szEXEPath);

				LPWSTR wcsFile = szPath.AllocSysString();
				pCell->m_pBitmap = new Bitmap(wcsFile, FALSE);
			}


			graphics->DrawImage(pCell->m_pBitmap, (int)(nX+fSizeX*i), (int)(nY+fSizeY*j), (int)fSizeX, (int)fSizeY);
		}
	}

}

BOOL cDMMapViewDialog::IsOnScreen(int nTX, int nTY, int nRange)
{
	CRect rect;
	GetClientRect(&rect);

	if (nTX >= rect.left-nRange && nTX < rect.right+nRange)
	{
		if (nTY >= rect.top-nRange && nTY <= rect.bottom+nRange)
		{
			return TRUE;
		}
	}

	return FALSE;
}

#define ISO_TILE_WIDTH 80
#define ISO_TILE_DEPTH 40

void cDMMapViewDialog::Translate2DCoordinatesToIsometric(int nMX, int nMY, float fViewScale, int *nIsoX, int *nIsoY)
{
	//calculate the screen coordinates
	//note: these will then be modified by the camera

	int nCellX = (nMX + 4) / 31;
	int nCellY = (nMY + 4) / 31;

	float fWidth = ISO_TILE_WIDTH * fViewScale;
	float fDepth = ISO_TILE_DEPTH * fViewScale;

	*nIsoX = m_nCornerX - (nCellY * fWidth / 2) + (nCellX * fWidth / 2) - (fWidth / 2);
	*nIsoY = m_nCornerY + (nCellY * fDepth / 2) + (nCellX * fDepth / 2);

	*nIsoY -= ISO_TILE_DEPTH * fViewScale;
}

void cDMMapViewDialog::TranslateIsoCoordinatesTo2D(int nMX, int nMY, int *nIsoX, int *nIsoY)  // mouse picks etc.
{
	//no idea how the hell this function works, but it does

	float fFudge = 2.0f;

	float x = (float)(nMX - m_nCornerX); 
	float y = (float)(nMY - m_nCornerY);

	float fCellX = ((y + x / fFudge));
	float fCellY = ((y - x / fFudge));

	fCellX /= (float)ISO_TILE_DEPTH;
	fCellY /= (float)ISO_TILE_DEPTH;

	fCellX /= m_fViewScale;
	fCellY /= m_fViewScale;

	*nIsoX = fCellX * 31;
	*nIsoY = fCellY * 31;

}

cDNDMapTile _Tiles[MAX_MAP_TILES];

void cDMMapViewDialog::ZOrderSortTiles()
{
	memset(_Tiles, 0, MAX_MAP_TILES*sizeof(cDNDMapTile));

	int nLowTile = -1;
	int nCount = 0;
	float fMinDist = 99999.99f;
	BOOL bSorted = FALSE;

	do
	{
		for (int i = 0; i < MAX_MAP_TILES; ++i)
		{
			if (m_pDNDMap->m_Tiles[i].m_nTileFrame == 0)
			{
				if (nLowTile >= 0)
				{
					memcpy(&_Tiles[nCount], &m_pDNDMap->m_Tiles[nLowTile], sizeof(cDNDMapTile));

					m_pDNDMap->m_Tiles[nLowTile].m_nTileFrame = -1;

					fMinDist = 99999.99f;
					nLowTile = -1;
					++nCount;
				}
				else
				{
					bSorted = TRUE;
				}

				break;
			}

			if (m_pDNDMap->m_Tiles[i].m_nTileFrame == -1)
			{
				continue;
			}

			float fDX = (float)m_pDNDMap->m_Tiles[i].m_nTileX;
			float fDY = (float)m_pDNDMap->m_Tiles[i].m_nTileY;

			float fDist = sqrt(fDX*fDX + fDY*fDY);

			if (fDist <= fMinDist)
			{
				nLowTile = i;
				fMinDist = fDist;
			}

		}
	} while (bSorted == FALSE);

	//copy the sorted tiles back
	memcpy(m_pDNDMap->m_Tiles, _Tiles, MAX_MAP_TILES*sizeof(cDNDMapTile));

}

void cDMMapViewDialog::AddTile(int nTX, int nTY,  int nTile, int nRoomNumber, int nFlag)
{
	//31 is the display size of a tile 
	int nBoundaryX = 31;
	int nBoundaryY = 31;

	int nDX = nTX/nBoundaryX;
	int nDY = nTY/nBoundaryY;

	nDX *= nBoundaryX;
	nDY *= nBoundaryY;

	nDX -= 4;
	nDY -= 4;


	BOOL bReplace = FALSE;
	for(int i = 0; i < MAX_MAP_TILES; ++i)
	{
		if(m_pDNDMap->m_Tiles[i].m_nTileFrame == 0)
		{
			break;
		}

		int nDistX = abs(nDX-m_pDNDMap->m_Tiles[i].m_nTileX);
		if(nDistX < 8)
		{
			int nDistY = abs(nDY-m_pDNDMap->m_Tiles[i].m_nTileY);
			if(nDistY < 8)
			{
				m_pDNDMap->m_Tiles[i].m_nTileFrame = nTile;
				m_pDNDMap->m_Tiles[i].m_nRoomNumber = nRoomNumber;
				m_pDNDMap->m_Tiles[i].m_nRoomFlag = nFlag;
				
				bReplace = TRUE;
			}
		}
	}

	if(bReplace == FALSE)
	{
		for(int i = 0; i < MAX_MAP_TILES; ++i)
		{
			if(m_pDNDMap->m_Tiles[i].m_nTileFrame == 0)
			{
				m_pDNDMap->m_Tiles[i].m_nTileFrame = nTile;	
				m_pDNDMap->m_Tiles[i].m_nTileX = nDX;
				m_pDNDMap->m_Tiles[i].m_nTileY = nDY;
				m_pDNDMap->m_Tiles[i].m_nRoomNumber = nRoomNumber;
				m_pDNDMap->m_Tiles[i].m_nRoomFlag = nFlag;

				break;
			}
		}
	}

	if (m_bIsometricCheck)
	{
		ZOrderSortTiles();
	}

	InvalidateRect(NULL);
}

void cDMMapViewDialog::RemoveTile(int nTX, int nTY)
{
	//nTX -= m_nCornerX;
	//nTY -= m_nCornerY;

	//31 is the display size of a tile
	int nBoundary = 31;

	int nDX = nTX/nBoundary;
	int nDY = nTY/nBoundary;

	nDX *= nBoundary;
	nDY *= nBoundary;

	nDX -= 4;
	nDY -= 4;

	BOOL bReplace = FALSE;
	for(int i = 0; i < MAX_MAP_TILES; ++i)
	{
		if(m_pDNDMap->m_Tiles[i].m_nTileFrame == 0)
		{
			break;
		}

		int nDistX = abs(nDX-m_pDNDMap->m_Tiles[i].m_nTileX);
		if(nDistX < 8)
		{
			int nDistY = abs(nDY-m_pDNDMap->m_Tiles[i].m_nTileY);
			if(nDistY < 8)
			{
				m_pDNDMap->m_Tiles[i].m_nTileFrame = 0;
				bReplace = TRUE;
			}
		}
	}

	if(bReplace)
	{
		ValidateTiles();
		InvalidateRect(NULL);
	}

}

void cDMMapViewDialog::ValidateTiles()
{
	//remove any gaps in the tile list
	cDNDMapTile *pTiles = new cDNDMapTile[MAX_MAP_TILES];

	int nCount = 0;
	for(int i = 0; i < MAX_MAP_TILES; ++i)
	{
		if(m_pDNDMap->m_Tiles[i].m_nTileFrame != 0)
		{
			memcpy(&pTiles[nCount], &m_pDNDMap->m_Tiles[i], sizeof(cDNDMapTile));
			++nCount;
		}
	}

	memcpy(&m_pDNDMap->m_Tiles, pTiles, MAX_MAP_TILES*sizeof(cDNDMapTile));

	delete [] pTiles;
}



void cDMMapViewDialog::DrawMapTiles(Graphics *pg)
{
	//31 pixels is the display size of a tile

	int nTilesDrawn = 0;

	int nSizeX = m_pDNDMap->m_nPixelSizeX/31; 
	int nSizeY = m_pDNDMap->m_nPixelSizeY/31; 

	int nTileSizeX = 39;
	int nTileSizeY = 39;

	if (m_bIsometricCheck)
	{
		nTileSizeX = 79;
		nTileSizeY = 79;
	}

	float fViewScale = m_fViewScale;

	if(m_bShowTileGrid)
	{
		Pen pen(Color(255, 50, 50, 50));

		for(int x = 0; x < nSizeX; ++x)
		{
			pg->DrawLine(&pen, x*31*m_fViewScale+m_nCornerX, m_nCornerY*1.0f, x*31*m_fViewScale+m_nCornerX, m_pDNDMap->m_nPixelSizeY*m_fViewScale+m_nCornerY);
		}
		for(int y = 0; y < nSizeY; ++y)
		{
			pg->DrawLine(&pen, m_nCornerX*1.0f, y*31*m_fViewScale+m_nCornerY, m_pDNDMap->m_nPixelSizeX*m_fViewScale+m_nCornerX, y*31*m_fViewScale+m_nCornerY);
		}
	}

	for(int nPass = 0; nPass < 2; ++nPass)
	{
		for(int i = 0; i < MAX_MAP_TILES; ++i)
		{
			int nDoorCheck = m_pDNDMap->m_Tiles[i].m_nTileFrame % 10;

			switch(nDoorCheck)
			{
				case 1:
				case 2:
				case 3:
				case 4:
				{
					if(nPass == 0)
					{
						continue;
					}

					break;
				}
				default:
				{
					if(nPass == 1)
					{
						continue;
					}

					break;
				}
			}

			if(m_pDNDMap->m_Tiles[i].m_nTileFrame)
			{
				int nDrawX = (int)(m_pDNDMap->m_Tiles[i].m_nTileX*m_fViewScale + m_nCornerX);
				int nDrawY = (int)(m_pDNDMap->m_Tiles[i].m_nTileY*m_fViewScale + m_nCornerY);

				if (m_bIsometricCheck)
				{
					nDrawX = (int)(m_pDNDMap->m_Tiles[i].m_nTileX);
					nDrawY = (int)(m_pDNDMap->m_Tiles[i].m_nTileY);

					Translate2DCoordinatesToIsometric(nDrawX, nDrawY, m_fViewScale, &nDrawX, &nDrawY);
				}

				if (IsOnScreen(nDrawX, nDrawY, 80 * m_fViewScale))
				{
					DrawMapTile(pg, m_pDNDMap->m_Tiles[i].m_nTileFrame, nDrawX, nDrawY, nTileSizeX, nTileSizeY, fViewScale);
					++nTilesDrawn;
				}
			}
			else
			{
				break;
			}
		}
	}

	TRACE("DREW %d TILES\n", nTilesDrawn);
}

void cDMMapViewDialog::DrawMapTile(Graphics *pg, int nCell, int nX, int nY, int nSizeX, int nSizeY, float fViewScale)
{
	RectF dst;

	ImageAttributes attr;
	attr.SetColorKey( Color(255, 0, 255), Color(255, 0, 255), ColorAdjustTypeBitmap);

	int nCellX = nCell % 10;
	int nCellY = nCell / 10;

/*
	Status DrawImage(IN Image* image,
                     IN const RectF& destRect,
                     IN REAL srcx,
                     IN REAL srcy,
                     IN REAL srcwidth,Add
                     IN REAL srcheight,
                     IN Unit srcUnit,
                     IN const ImageAttributes* imageAttributes = NULL,
                     IN DrawImageAbort callback = NULL,
                     IN VOID* callbackData = NULL)
*/

	if (m_bIsometricCheck)
	{
		dst.X = (Gdiplus::REAL)nX;
		dst.Y = (Gdiplus::REAL)nY;
		dst.Width = (Gdiplus::REAL)nSizeX * fViewScale;
		dst.Height = (Gdiplus::REAL)nSizeY * fViewScale;

		int nSourceX = nCellX * 80 + 1;
		int nSourceY = nCellY * 80 + 1;

		if (m_pApp->m_pIsometricDungeonTilesBitmap == NULL)
		{
			m_pApp->LoadIsometricDungeonTiles();
		}

		// 78 works, dunno why
		pg->DrawImage(m_pApp->m_pIsometricDungeonTilesBitmap, dst, (Gdiplus::REAL)nSourceX, (Gdiplus::REAL)nSourceY, (Gdiplus::REAL)78, (Gdiplus::REAL)78, UnitPixel, &attr); //, UnitPixel);
	}
	else
	{
		dst.X = (Gdiplus::REAL)nX;
		dst.Y = (Gdiplus::REAL)nY;
		dst.Width = (Gdiplus::REAL)nSizeX * fViewScale;
		dst.Height = (Gdiplus::REAL)nSizeY * fViewScale;

		int nSourceX = nCellX * 40 + 1;
		int nSourceY = nCellY * 40 + 1;

		if (m_pApp->m_pDungeonTilesBitmap == NULL)
		{
			m_pApp->LoadDungeonTiles();
		}

		pg->DrawImage(m_pApp->m_pDungeonTilesBitmap, dst, (Gdiplus::REAL)nSourceX, (Gdiplus::REAL)nSourceY, (Gdiplus::REAL)39, (Gdiplus::REAL)39, UnitPixel, &attr); //, UnitPixel);
	}

}

void cDMMapViewDialog::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	InvalidateRect(NULL);
}


void cDMMapViewDialog::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus();

	if(nFlags & MK_SHIFT)
	{
		m_bMouseDrag = TRUE;
		m_DragPoint = point;
	}

	for (int i = 0; i < m_nMapCharacters; ++i)
	{
		if (abs(m_CharacterHotSpots[i].m_nX - point.x) < 10 && abs(m_CharacterHotSpots[i].m_nY - point.y) < 10)
		{
			m_dwDraggedCharacterID = m_CharacterHotSpots[i].m_dwCharacterID;
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void cDMMapViewDialog::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bMouseDrag)
	{
		m_bMouseDrag = FALSE;
		InvalidateRect(NULL);
	}

	if (m_dwDraggedCharacterID != 0)
	{
		cDNDCharacter *pSelectedCharacter = NULL;
		WORD wID;
		for (POSITION pos = m_pApp->m_CharacterViewMap.GetStartPosition(); pos != NULL;)
		{
			PDNDCHARVIEWDLG pCharDlg = NULL;
			m_pApp->m_CharacterViewMap.GetNextAssoc(pos, wID, pCharDlg);

			if (pCharDlg != NULL && pCharDlg->m_pCharacter != NULL && pCharDlg->m_pCharacter->m_dwCharacterID == m_dwDraggedCharacterID)
			{
				pSelectedCharacter = pCharDlg->m_pCharacter;
				break;
			}
		}

		if (pSelectedCharacter != NULL)
		{
			if (m_pDNDMap->m_bMapScaleFeet)
			{
				//pixels to feet
				float fX = (float)(point.x - m_nCornerX) * m_pDNDMap->m_fScaleX / m_fViewScale;
				float fY = (float)(point.y - m_nCornerY) * m_pDNDMap->m_fScaleY / m_fViewScale;

				//kieran
				if (m_bIsometricCheck)
				{
					int nIsoX = 0;
					int nIsoY = 0;
					TranslateIsoCoordinatesTo2D(point.x, point.y, &nIsoX, &nIsoY);

					fX = ((float)nIsoX)* m_pDNDMap->m_fScaleX;
					fY = ((float)nIsoY)* m_pDNDMap->m_fScaleY;
				}


				pSelectedCharacter->m_fLocalLocationX = fX;
				pSelectedCharacter->m_fLocalLocationY = fY;

				pSelectedCharacter->MarkChanged();
			}
			else
			{
				//pixels to miles
				float fX = (float)(point.x - m_nCornerX) * m_pDNDMap->m_fScaleX / m_fViewScale + m_pDNDMap->m_fParentMapOriginX;
				float fY = (float)(point.y - m_nCornerY) * m_pDNDMap->m_fScaleY / m_fViewScale + m_pDNDMap->m_fParentMapOriginY;

				pSelectedCharacter->m_fMapLocationX = fX;
				pSelectedCharacter->m_fMapLocationY = fY;

				pSelectedCharacter->MarkChanged();
			}

			UpdateDetachedMaps();
		}

		cDNDNonPlayerCharacter *pSelectedNPC = NULL;
		for (pos = m_pApp->m_NPCViewMap.GetStartPosition(); pos != NULL;)
		{
			PDNDNPCVIEWDLG pNPCDlg = NULL;
			m_pApp->m_NPCViewMap.GetNextAssoc(pos, wID, pNPCDlg);

			if (pNPCDlg != NULL && pNPCDlg->m_pNPC != NULL && pNPCDlg->m_pNPC->m_dwCharacterID == m_dwDraggedCharacterID)
			{
				pSelectedNPC = pNPCDlg->m_pNPC;
				break;
			}
		}

		if (pSelectedNPC != NULL)
		{
			if (m_pDNDMap->m_bMapScaleFeet)
			{
				//pixels to feet
				float fX = (float)(point.x - m_nCornerX) * m_pDNDMap->m_fScaleX / m_fViewScale;
				float fY = (float)(point.y - m_nCornerY) * m_pDNDMap->m_fScaleY / m_fViewScale;

				//kieran
				if (m_bIsometricCheck)
				{
					int nIsoX = 0;
					int nIsoY = 0;
					TranslateIsoCoordinatesTo2D(point.x, point.y, &nIsoX, &nIsoY);

					fX = ((float)nIsoX)* m_pDNDMap->m_fScaleX;
					fY = ((float)nIsoY)* m_pDNDMap->m_fScaleY;
				}


				pSelectedNPC->m_fLocalLocationX = fX;
				pSelectedNPC->m_fLocalLocationY = fY;
			}
			else
			{
				//pixels to miles
				float fX = (float)(point.x - m_nCornerX) * m_pDNDMap->m_fScaleX / m_fViewScale + m_pDNDMap->m_fParentMapOriginX;
				float fY = (float)(point.y - m_nCornerY) * m_pDNDMap->m_fScaleY / m_fViewScale + m_pDNDMap->m_fParentMapOriginY;

				pSelectedNPC->m_fMapLocationX = fX;
				pSelectedNPC->m_fMapLocationY = fY;
			}
		}


		m_dwDraggedCharacterID = 0;

		UpdateDetachedMaps();

		InvalidateRect(NULL);
	}

	CDialog::OnLButtonUp(nFlags, point);
}


void cDMMapViewDialog::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	switch(m_MapMode)
	{
		//BOOKMARK
		case DND_MAP_MODE_PLACE_PARTY:
		{
			if(m_pSelectedParty != NULL)
			{
				m_pSelectedParty->m_dwPartyMapID = m_pDNDMap->m_dwMapID;

				if(m_pDNDMap->m_bMapScaleFeet)
				{
					m_pSelectedParty->m_fPartyLocationX = m_pDNDMap->m_fParentMapOriginX;
					m_pSelectedParty->m_fPartyLocationY = m_pDNDMap->m_fParentMapOriginY;

					//pixels to feet
					float fX = (float)(point.x - m_nCornerX) * m_pDNDMap->m_fScaleX / m_fViewScale;
					float fY = (float)(point.y - m_nCornerY) * m_pDNDMap->m_fScaleY / m_fViewScale;

					m_pSelectedParty->m_fPartyLocalLocationX = fX;
					m_pSelectedParty->m_fPartyLocalLocationY = fY;

					m_pSelectedParty->MarkChanged();
				}
				else
				{
					//pixels to miles
					float fX = (float)(point.x - m_nCornerX) * m_pDNDMap->m_fScaleX / m_fViewScale + m_pDNDMap->m_fParentMapOriginX;
					float fY = (float)(point.y - m_nCornerY) * m_pDNDMap->m_fScaleY / m_fViewScale + m_pDNDMap->m_fParentMapOriginY;

					m_pSelectedParty->m_fPartyLocationX = fX;
					m_pSelectedParty->m_fPartyLocationY = fY;

					m_pSelectedParty->MarkChanged();
				}
			}

			m_MapMode = DND_MAP_MODE_UNDEF;
			
			break;
		}

		case DND_MAP_MODE_PLACE_CHARACTER:
		{
			if(m_pSelectedParty != NULL && m_pSelectedCharacter != NULL)
			{
				if(m_pDNDMap->m_bMapScaleFeet)
				{
					m_pSelectedParty->m_fPartyLocationX = m_pDNDMap->m_fParentMapOriginX;
					m_pSelectedParty->m_fPartyLocationY = m_pDNDMap->m_fParentMapOriginY;

					//pixels to feet
					float fX = (float)(point.x - m_nCornerX) * m_pDNDMap->m_fScaleX / m_fViewScale;
					float fY = (float)(point.y - m_nCornerY) * m_pDNDMap->m_fScaleY / m_fViewScale;

					//kieran
					if (m_bIsometricCheck)
					{
						int nIsoX = 0;
						int nIsoY = 0;
						TranslateIsoCoordinatesTo2D(point.x, point.y, &nIsoX, &nIsoY);

						fX = ((float)nIsoX)* m_pDNDMap->m_fScaleX;
						fY = ((float)nIsoY)* m_pDNDMap->m_fScaleY;
					}

					m_pSelectedCharacter->m_fLocalLocationX = fX;
					m_pSelectedCharacter->m_fLocalLocationY = fY;

					m_pSelectedCharacter->MarkChanged();
				}
				else
				{
					//pixels to miles
					float fX = (float)(point.x - m_nCornerX) * m_pDNDMap->m_fScaleX / m_fViewScale + m_pDNDMap->m_fParentMapOriginX;
					float fY = (float)(point.y - m_nCornerY) * m_pDNDMap->m_fScaleY / m_fViewScale + m_pDNDMap->m_fParentMapOriginY;

					m_pSelectedCharacter->m_fMapLocationX = fX;
					m_pSelectedCharacter->m_fMapLocationY = fY;

					m_pSelectedCharacter->MarkChanged();
				}

				UpdateDetachedMaps();
			}

			m_MapMode = DND_MAP_MODE_UNDEF;
			
			break;
		}
		case DND_MAP_MODE_PLACE_CACHE:
		{
			if(m_pSelectedCache != NULL)
			{
				if(m_pDNDMap->m_bMapScaleFeet)
				{
					//pixels to feet
					float fX = (float)(point.x - m_nCornerX) * m_pDNDMap->m_fScaleX / m_fViewScale;
					float fY = (float)(point.y - m_nCornerY) * m_pDNDMap->m_fScaleY / m_fViewScale;

					float fOldX = m_pSelectedCache->m_fLocalLocationX;
					float fOldY = m_pSelectedCache->m_fLocalLocationY;

					//kieran
					if (m_bIsometricCheck)
					{
						int nIsoX = 0;
						int nIsoY = 0;
						TranslateIsoCoordinatesTo2D(point.x, point.y, &nIsoX, &nIsoY);

						fX = ((float)nIsoX)* m_pDNDMap->m_fScaleX;
						fY = ((float)nIsoY)* m_pDNDMap->m_fScaleY;
					}

					m_pSelectedCache->m_fLocalLocationX = fX;
					m_pSelectedCache->m_fLocalLocationY = fY;

					CacheMoveAssociatedSubParty(m_pSelectedCache, fOldX, fOldY);
				}
				else
				{
					//pixels to miles
					float fX = (float)(point.x - m_nCornerX) * m_pDNDMap->m_fScaleX / m_fViewScale + m_pDNDMap->m_fParentMapOriginX;
					float fY = (float)(point.y - m_nCornerY) * m_pDNDMap->m_fScaleY / m_fViewScale + m_pDNDMap->m_fParentMapOriginY;

					m_pSelectedCache->m_fMapLocationX = fX;
					m_pSelectedCache->m_fMapLocationY = fY;
				}

				m_pDNDMap->MarkChanged();

				UpdateDetachedMaps();
			}
	
			m_MapMode = DND_MAP_MODE_UNDEF;
			
			break;
		} 

		case DND_MAP_MODE_PLOT_PARTY_ROUTE:
		{
			if(m_pSelectedParty != NULL)
			{
				if(m_pDNDMap->m_bMapScaleFeet)
				{
				}
				else
				{
					m_pSelectedParty->m_WayPoints[0].m_fX = m_pSelectedParty->m_fPartyLocationX;
					m_pSelectedParty->m_WayPoints[0].m_fY = m_pSelectedParty->m_fPartyLocationY;

					m_pSelectedParty->MarkChanged();

					for(int i = 1; i < MAX_PARTY_WAYPOINTS; ++i)
					{
						if(m_pSelectedParty->m_WayPoints[i].m_fX == 0.0f && m_pSelectedParty->m_WayPoints[i].m_fY == 0.0f)
						{
							//pixels to miles
							float fX = (float)(point.x - m_nCornerX) * m_pDNDMap->m_fScaleX / m_fViewScale + m_pDNDMap->m_fParentMapOriginX;
							float fY = (float)(point.y - m_nCornerY) * m_pDNDMap->m_fScaleY / m_fViewScale + m_pDNDMap->m_fParentMapOriginY;

							m_pSelectedParty->m_WayPoints[i].m_fX = fX;
							m_pSelectedParty->m_WayPoints[i].m_fY = fY;

							break;
						}

					}

					
				}

				UpdateDetachedMaps();
			}

			break;
		}


		/////////////////////////////////////////////////////////////


		case DND_MAP_MODE_PICK_CHILD_POINT_1:
		{
			m_pApp->m_MapScaler.m_nChildMapX1 = (point.x - m_nCornerX);
			m_pApp->m_MapScaler.m_nChildMapY1 = (point.y - m_nCornerY);

			m_MapMode = DND_MAP_MODE_PICKED_CHILD_POINT_1;
			break;
		}
		case DND_MAP_MODE_PICK_PARENT_POINT_1:
		{
			m_pApp->m_MapScaler.m_nParentMapX1 = (point.x - m_nCornerX);
			m_pApp->m_MapScaler.m_nParentMapY1 = (point.y - m_nCornerY);

			m_MapMode = DND_MAP_MODE_PICKED_PARENT_POINT_1;
			break;
		}

		case DND_MAP_MODE_PICK_CHILD_POINT_2:
		{
			m_pApp->m_MapScaler.m_nChildMapX2 = (point.x - m_nCornerX);
			m_pApp->m_MapScaler.m_nChildMapY2 = (point.y - m_nCornerY);

			m_MapMode = DND_MAP_MODE_PICKED_CHILD_POINT_2;
			break;
		}
		case DND_MAP_MODE_PICK_PARENT_POINT_2:
		{
			m_pApp->m_MapScaler.m_nParentMapX2 = (point.x - m_nCornerX);
			m_pApp->m_MapScaler.m_nParentMapY2 = (point.y - m_nCornerY);

			m_MapMode = DND_MAP_MODE_PICKED_PARENT_POINT_2;
			break;
		}

		case DND_MAP_MODE_PICK_SCALE_POINT_1:
		{
			m_pApp->m_MapScaler.m_nParentMapX1 = (point.x - m_nCornerX);
			m_pApp->m_MapScaler.m_nParentMapY1 = (point.y - m_nCornerY);

			m_MapMode = DND_MAP_MODE_PICKED_SCALE_POINT_1;
			break;
		}

		case DND_MAP_MODE_PICK_SCALE_POINT_2:
		{
			m_pApp->m_MapScaler.m_nParentMapX2 = (point.x - m_nCornerX);
			m_pApp->m_MapScaler.m_nParentMapY2 = (point.y - m_nCornerY);

			m_MapMode = DND_MAP_MODE_PICKED_SCALE_POINT_2;
			break;
		}

		default:
		{
			BOOL bFoundHotSpot = FALSE;

			for(int i = 0; i < m_nMapParties; ++i)
			{
				if(abs(m_PartyHotSpots[i].m_nX - point.x) < 10 && abs(m_PartyHotSpots[i].m_nY - point.y) < 10)
				{
					cDNDParty *pPreviousParty = m_pSelectedParty;

					m_pSelectedParty = m_PartyHotSpots[i].m_pParty;
					bFoundHotSpot = TRUE;
		
					for(int j = 0; j < m_cPartyCombo.GetCount(); ++j)
					{
						cDNDParty *pParty = (cDNDParty *)m_cPartyCombo.GetItemData(i);

						if(pParty == m_pSelectedParty)
						{
							m_cPartyCombo.SetCurSel(i);
							OnSelchangePartyCombo();
							break;
						}
					}

					if(pPreviousParty == m_pSelectedParty)
					{
						m_pMainDialog->FindAndSelectTab(DND_TAB_TYPE_PARTY, m_pSelectedParty->m_dwPartyID);
					}

					break;
				}
			}

			if(bFoundHotSpot == FALSE)
			{
				for(int i = 0; i < m_nMapCharacters ; ++i)
				{
					if(abs(m_CharacterHotSpots[i].m_nX - point.x) < 10 && abs(m_CharacterHotSpots[i].m_nY - point.y) < 10)
					{
						DWORD dwCharacterID = m_CharacterHotSpots[i].m_dwCharacterID;
						bFoundHotSpot = TRUE;

						// find the party this character is a member of

						DWORD dwPartyID = 0;
						for (POSITION pos = m_pApp->m_PartyViewMap.GetStartPosition(); pos != NULL; )
						{
							WORD wID;
							PDNDPARTYVIEWDLG pPartyDlg = NULL;
							m_pApp->m_PartyViewMap.GetNextAssoc(pos,wID,pPartyDlg);

							if(pPartyDlg != NULL && pPartyDlg->m_pParty != NULL)
							{
								if(pPartyDlg->m_pParty->CharacterIsPartyMember(dwCharacterID))
								{
									dwPartyID = pPartyDlg->m_pParty->m_dwPartyID;
									m_pMainDialog->FindAndSelectTab(DND_TAB_TYPE_PARTY, dwPartyID);
									break;
								}
							}
						}
						
						if(dwPartyID)
						{
							m_pMainDialog->FindAndSelectTab(DND_TAB_TYPE_CHARACTER, dwCharacterID);
						}

						break;
					}
				}

				if(bFoundHotSpot == FALSE)
				{
					if(m_pHoverCache != NULL)
					{
						bFoundHotSpot = FindAndSelectCacheTab(m_pHoverCache->m_dwCacheID);
					}
				}

				if (bFoundHotSpot == FALSE) // SFX CHECK
				{
					int nTX = 0;
					int nTY = 0;

					if (m_bIsometricCheck)
					{
						TranslateIsoCoordinatesTo2D(point.x, point.y, &nTX, &nTY);
					}
					else
					{
						nTX = (int)((point.x - m_nCornerX) / m_fViewScale);
						nTY = (int)((point.y - m_nCornerY) / m_fViewScale);
					}

					int nSFXIndex = -1;
					for (int i = 0; i < MAX_MAP_SFX && m_pDNDMap->m_MapSFX[i].m_SFXState != DND_SFX_STATE_UNDEF; ++i)
					{
						int nX1 = m_pDNDMap->m_MapSFX[i].m_nMapX;
						int nY1 = m_pDNDMap->m_MapSFX[i].m_nMapY;

						int nX2 = nX1 + 32;
						int nY2 = nY1 + 16;

						if (nTX >= nX1 && nTX <= nX2 && nTY >= nY1 && nTY <= nY2)
						{
							nSFXIndex = i; // found one !
							bFoundHotSpot = TRUE;
							break;
						}
					}

					if (nFlags & MK_CONTROL)
					{
						if (m_bDetachedWindow == FALSE)
						{
							cDMMapSFXDialog *pDlg = new cDMMapSFXDialog(this, nSFXIndex, nTX, nTY, m_fViewScale);
							pDlg->DoModal();
							delete pDlg;
						}
					}
					else if (nSFXIndex >= 0)
					{
						switch (m_pDNDMap->m_MapSFX[nSFXIndex].m_SFXState)
						{
							case DND_SFX_STATE_READY:				m_pDNDMap->m_MapSFX[nSFXIndex].m_SFXState = DND_SFX_STATE_TRIGGERED_START; break;
							case DND_SFX_STATE_TRIGGERED_START:		m_pDNDMap->m_MapSFX[nSFXIndex].m_SFXState = DND_SFX_STATE_READY; break;
							case DND_SFX_STATE_TRIGGERED:			m_pDNDMap->m_MapSFX[nSFXIndex].m_SFXState = DND_SFX_STATE_READY; break;
						}

						UpdateDetachedMaps();

					}
				}

				if(bFoundHotSpot == FALSE) // center the map on the click
				{
					if(m_nSelectedMapTile >= 10)
					{
						int nTX = 0;
						int nTY = 0;

						if (m_bIsometricCheck)
						{
							TranslateIsoCoordinatesTo2D(point.x, point.y, &nTX, &nTY);
						}
						else
						{
							nTX = (int)((point.x - m_nCornerX) / m_fViewScale);
							nTY = (int)((point.y - m_nCornerY) / m_fViewScale);
						}

						AddTile(nTX, nTY, m_nSelectedMapTile, 0, 0);
					}
					else
					{
						CRect Rect;
						GetWindowRect (Rect);

						int nSizeX = Rect.right - Rect.left;
						int nSizeY = Rect.bottom - Rect.top;

						nSizeX /= 2;
						nSizeY /= 2;

						float fX = 0.0f;
						float fY = 0.0f;

						int nDX = point.x - nSizeX;
						int nDY = point.y - nSizeY;	

						m_nCornerX -= nDX;
						m_nCornerY -= nDY;
					}

					InvalidateRect(NULL);
				}
			}


			break;
		}
	}
	
	InvalidateRect(NULL);

	CDialog::OnLButtonDblClk(nFlags, point);
}


void cDMMapViewDialog::CacheMoveAssociatedSubParty(cDNDCacheInventory *pCache, float fOldX, float fOldY)
{
	//find a subparty associated with this cache in a dungeon

	cDNDParty *pParty = NULL;
	cDNDSubParty *pSubParty = NULL;

	DWORD dwPartyID = 0;
	for (POSITION pos = m_pApp->m_PartyViewMap.GetStartPosition(); pos != NULL; )
	{
		WORD wID;
		PDNDPARTYVIEWDLG pPartyDlg = NULL;
		m_pApp->m_PartyViewMap.GetNextAssoc(pos,wID,pPartyDlg);

		if(pPartyDlg != NULL && pPartyDlg->m_pParty != NULL)
		{
			for(int i = 0; i < MAX_SUB_PARTIES; ++i)
			{
				if(pPartyDlg->m_pParty->m_SubParty[i].m_dwSubPartyID && strcmp(pPartyDlg->m_pParty->m_SubParty[i].m_szSubPartyName, pCache->m_szCacheDesc) == 0)
				{
					pParty = pPartyDlg->m_pParty;
					pSubParty = &pPartyDlg->m_pParty->m_SubParty[i];
					break;
				}
			}
		}
	}

	if(pParty == NULL || pSubParty == NULL)
		return;

	WORD wID;
	for (POSITION pos = m_pApp->m_CharacterViewMap.GetStartPosition(); pos != NULL; )
	{
		PDNDCHARVIEWDLG pCharDlg = NULL;
		m_pApp->m_CharacterViewMap.GetNextAssoc(pos,wID,pCharDlg);

		if(pCharDlg != NULL && pCharDlg->m_pCharacter != NULL && pCharDlg->m_pCharacter->m_dwCharacterID && pCharDlg->m_pCharacter->m_dwSubPartyID == pSubParty->m_dwSubPartyID)
		{
			float fDistX = fOldX - pCharDlg->m_pCharacter->m_fLocalLocationX;
			float fDistY = fOldY - pCharDlg->m_pCharacter->m_fLocalLocationY;
		
			pCharDlg->m_pCharacter->m_fLocalLocationX = pCache->m_fLocalLocationX - fDistX;
			pCharDlg->m_pCharacter->m_fLocalLocationY = pCache->m_fLocalLocationY - fDistY;
		}
	}

	for (pos = m_pApp->m_NPCViewMap.GetStartPosition(); pos != NULL; )
	{
		PDNDNPCVIEWDLG pNPCDlg = NULL;
		m_pApp->m_NPCViewMap.GetNextAssoc(pos,wID,pNPCDlg);

		if(pNPCDlg != NULL && pNPCDlg->m_pNPC != NULL && pNPCDlg->m_pNPC->m_dwCharacterID && pNPCDlg->m_pNPC->m_dwSubPartyID == pSubParty->m_dwSubPartyID)
		{
			float fDistX = fOldX - pNPCDlg->m_pNPC->m_fLocalLocationX;
			float fDistY = fOldY - pNPCDlg->m_pNPC->m_fLocalLocationY;
		
			pNPCDlg->m_pNPC->m_fLocalLocationX = pCache->m_fLocalLocationX - fDistX;
			pNPCDlg->m_pNPC->m_fLocalLocationY = pCache->m_fLocalLocationY - fDistY;
		}
	}

}


void cDMMapViewDialog::OnRButtonDown(UINT nFlags, CPoint point) 
{

	for (int i = 0; i < m_nMapCharacters; ++i)
	{
		if (abs(m_CharacterHotSpots[i].m_nX - point.x) < 10 && abs(m_CharacterHotSpots[i].m_nY - point.y) < 10)
		{
			DWORD dwDraggedCharacterID = m_CharacterHotSpots[i].m_dwCharacterID;

			PDNDCHARVIEWDLG pCharDlg = NULL;
			if (m_pApp->m_CharacterViewMap.Lookup((WORD)dwDraggedCharacterID, pCharDlg) && pCharDlg != NULL)
			{
				pCharDlg->m_bReversedMapIcon = !pCharDlg->m_bReversedMapIcon;
				InvalidateRect(NULL);
				break;
			}

			PDNDNPCVIEWDLG pNPCDlg = NULL;
			if (m_pApp->m_NPCViewMap.Lookup((WORD)dwDraggedCharacterID, pNPCDlg) && pNPCDlg != NULL)
			{
				pNPCDlg->m_bReversedMapIcon = !pNPCDlg->m_bReversedMapIcon;
				InvalidateRect(NULL);
				break;
			}		
		}
	}
	
	CDialog::OnRButtonDown(nFlags, point);
}

BOOL cDMMapViewDialog::OnEraseBkgnd(CDC* pDC)
{

#if USE_MEMDC
	return FALSE;
#else
	return CDialog::OnEraseBkgnd(pDC);
#endif
}

void cDMMapViewDialog::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	CDialog::OnRButtonDblClk(nFlags, point);


	if(m_pDNDMap->m_nRows == 0 || m_pDNDMap->m_nColumns == 0)
		return;

	if(m_pDNDMap->m_nPixelSizeX == 0 || m_pDNDMap->m_nPixelSizeY == 0)
		return;

	if(m_nSelectedMapTile >= 10)
	{
		int nTX = 0;
		int nTY = 0;

		if (m_bIsometricCheck)
		{
			TranslateIsoCoordinatesTo2D(point.x, point.y, &nTX, &nTY);
		}
		else
		{
			nTX = (int)((point.x - m_nCornerX) / m_fViewScale);
			nTY = (int)((point.y - m_nCornerY) / m_fViewScale);
		}

		RemoveTile(nTX, nTY);
		return;
	}
	


	int nMapX = (int)((float)(-m_nCornerX + point.x) / m_fViewScale);
	int nMapY = (int)((float)(-m_nCornerY + point.y) / m_fViewScale);

	int nCellX = nMapX / m_pDNDMap->m_nPixelSizeX;
	int nCellY = nMapY / m_pDNDMap->m_nPixelSizeY;

	TRACE("MAP CELL %d %d\n", nCellX, nCellY);

	if (nCellX == 0 && nCellY == 0)
	{
		if (strcmp(m_pDNDMap->m_szMapName, "New Map") == 0 || m_pDNDMap->m_szMapName[0] == 0)
		{
			AfxMessageBox("Please name this map !", MB_OK);
			return;
		}
	}


	cDNDMapCell *pCell = &m_pDNDMap->m_Cells[nCellX][nCellY];

	if ((nFlags & MK_CONTROL) && (nFlags & MK_SHIFT))
	{
		if (m_pEditMapLayersDialog == NULL)
		{
			m_pEditMapLayersDialog = new CDMEditMapLayersDialog(pCell, this);
		}
		return;
	}

	if (!(nFlags & MK_CONTROL))
	{
		return;
	}

	if(pCell->m_pBitmap != NULL)
	{
		delete pCell->m_pBitmap;
		pCell->m_pBitmap = NULL;
	}


	CString szPath;

	ZeroMemory(&g_ofn, sizeof(OPENFILENAME));
	g_ofn.lStructSize = sizeof(OPENFILENAME);
	g_ofn.nMaxFile = MAX_PATH;
	g_ofn.nMaxFileTitle = MAX_PATH;
	g_ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	g_ofn.hwndOwner   = m_hWnd;
	g_ofn.hInstance   = m_pApp->m_hInstance;
    g_ofn.lpstrFile   = g_MVBP_szFilename;
	g_ofn.lpstrInitialDir = "C:/";
    g_ofn.lpstrTitle  = "Load map bitmap file";
    //g_ofn.lpstrFilter = "Graphics Files (*.bmp)\0*.bmp\0(*.gif)\0*.gif\0(*.jpg)\0*.jpg\0All Files (*.*)\0*.*\0\0";
	g_ofn.lpstrFilter = "All Files (*.*)\0*.*\0\0";
    g_ofn.lpstrDefExt = "bmp";

    // Get action template file name
    if(GetOpenFileName(&g_ofn))
	{
		szPath = g_MVBP_szFilename;

		LPWSTR wcsFile = szPath.AllocSysString();
		pCell->m_pBitmap = new Bitmap(wcsFile , FALSE);

		szPath.Replace(m_pApp->m_szEXEPath, "<$DMAPATH>");
		strcpy(pCell->m_szBitmapPath, szPath.GetBuffer(0));

		if (nCellX == 0 && nCellY == 0)
		{
			int nImageWidth = pCell->m_pBitmap->GetWidth();
			int nImageHeight = pCell->m_pBitmap->GetHeight();

			if (nImageWidth > m_pDNDMap->m_nPixelSizeX || nImageHeight > m_pDNDMap->m_nPixelSizeY)
			{
				cDMMapSizingDialog *pDlg = new cDMMapSizingDialog(m_pDNDMap, this);
				pDlg->DoModal();
				delete pDlg;
			}
			else if (m_pDNDMap->m_nRows == 1 && m_pDNDMap->m_nColumns == 1)
			{
				if (nImageWidth < m_pDNDMap->m_nPixelSizeX || nImageHeight < m_pDNDMap->m_nPixelSizeY)
				{
					m_pDNDMap->m_nPixelSizeX = nImageWidth;
					m_pDNDMap->m_nPixelSizeY = nImageHeight;
				}
			}
		}

		InvalidateRect(NULL);

		m_pDNDMap->MarkChanged();
		
	}

}

void cDMMapViewDialog::OnTimer(UINT_PTR nIDEvent)
{
	UpdateMapLegend();

	CDialog::OnTimer(nIDEvent);
}

void cDMMapViewDialog::UpdateMapLegend()
{
	CString szOldLegend = m_szMapLegend; 

	//pixels to miles
	float fX = (float)(m_nMousePointX - m_nCornerX) * m_pDNDMap->m_fScaleX / m_fViewScale + m_pDNDMap->m_fParentMapOriginX;
	float fY = (float)(m_nMousePointY - m_nCornerY) * m_pDNDMap->m_fScaleY / m_fViewScale + m_pDNDMap->m_fParentMapOriginY;

	m_szMapLegend.Format("(%0.2fx - %0.2fy)", fX, fY);

	if(m_pSelectedParty != NULL && m_pApp->m_pSelectedCalendar != NULL)
	{
		int nMinutes = m_pSelectedParty->m_nTurn * 10 + m_pSelectedParty->m_nRound;
		int nSeconds = m_pSelectedParty->m_nSegment * 6 + m_pApp->m_nCalendarSecond;

		CString szPartyLegend;
		szPartyLegend.Format("   %02d:%02d:%02d %s, the %d%s day of %s in the year %d", 
			m_pSelectedParty->m_nHour, nMinutes, nSeconds, 
			m_pApp->m_pSelectedCalendar->m_szDayNames[m_pSelectedParty->m_nDayofWeek], 
			m_pSelectedParty->m_nDayofMonth+1, 
			GetNumberSuffix(m_pSelectedParty->m_nDayofMonth+1),
			m_pApp->m_pSelectedCalendar->m_szMonthNames[m_pSelectedParty->m_nMonth], 
			m_pSelectedParty->m_nYear);

		m_szMapLegend += szPartyLegend;

		float fMoveDay = ((float)m_pSelectedParty->m_nPartyMovementRate) * 2.5f;

		CString szDist;
		szDist.Format("  Route Distance %0.1f miles at %0.1f miles per day", m_fSelectedPartyRouteDistance, fMoveDay);
		m_szMapLegend += szDist;

		if(m_pDNDMap->m_bMapScaleFeet == FALSE && m_bWeatherCheck == TRUE) 
		{
			CString szWeather = _T("");
			GenerateWeather(&szWeather);

			m_szMapLegend += " (";
			m_szMapLegend += szWeather;
			m_szMapLegend += ") ";
		}

		m_szMapLegend += " ";
		m_szMapLegend += m_szPartyAlert;
	}

	if(szOldLegend != m_szMapLegend)
	{
		UpdateData(FALSE);

		RECT Rect;
		m_cMapLegend.GetWindowRect(&Rect);
		InvalidateRect(&Rect);
	}
}

void cDMMapViewDialog::OnMouseMove(UINT nFlags, CPoint point) 
{
	BOOL bUpdateDetachedMaps = FALSE;

	ValidateSelectedParty();

	if (m_dwDraggedCharacterID)
	{
		CDialog::OnMouseMove(nFlags, point);
		return;
	}

	m_MousePoint = point;

	static int nLastX;
	static int nLastY;

	CRect rect;
	GetClientRect(&rect);

	int nSizeX = rect.right - rect.left;
	int nSizeY = rect.bottom - rect.top;

	if (m_bIsometricCheck)
	{
		nSizeX *= 4 * m_fViewScale;
		nSizeY *= 8 * m_fViewScale;
	}

	int nBorderX1 = nSizeX / 2;
	int nBorderY1 = nSizeY / 2;

	int nBorderX2 = (int)(m_pDNDMap->m_nPixelSizeX * m_fViewScale) * m_pDNDMap->m_nColumns - nSizeX * 2;
	int nBorderY2 = (int)(m_pDNDMap->m_nPixelSizeY * m_fViewScale) * m_pDNDMap->m_nRows - nSizeY * 2;

	m_nMousePointX = point.x;
	m_nMousePointY = point.y;

	m_nMouseX = point.x - m_nCornerX;
	m_nMouseY = point.y - m_nCornerY;

	UpdateMapLegend();

	if(!(nFlags & MK_SHIFT))
	{
		m_bMouseDrag = FALSE;

		m_nUnshiftedMouseX = m_nMouseX;
		m_nUnshiftedMouseY = m_nMouseY;
	}

	if(nFlags & MK_LBUTTON)
	{
		if(m_bMouseDrag == FALSE)
		{
			int nDiffX = point.x - nLastX;
			int nDiffY = point.y - nLastY;

			m_nCornerX += nDiffX;
			m_nCornerY += nDiffY;

			/*
			if(m_nCornerX > nBorderX1)
				m_nCornerX = nBorderX1;

			if(m_nCornerY > nBorderY1)
				m_nCornerY = nBorderY1;

			if(m_nCornerX < nBorderX2)
				m_nCornerX = nBorderX2;

			if(m_nCornerY < nBorderY2)
				m_nCornerY = nBorderY2;
				*/
		}

		//TRACE("TOUCH ME ! (%d %d (%d))\n", m_nCornerX, nBorderX2, m_pDNDMap->m_nPixelSizeX);

		InvalidateRect(NULL);
	}

	nLastX = point.x;
	nLastY = point.y;

	int nShowCacheDetails = SW_HIDE;
	m_pHoverCache = NULL;
	for (int i=0; i < MAX_HOTSPOTS; ++i )
	{
		if(m_CacheHotSpots[i].m_pCache == NULL)
			break;

		if(abs(m_CacheHotSpots[i].m_nX - point.x) < 10 && abs(m_CacheHotSpots[i].m_nY - point.y) < 10)
		{
			m_pHoverCache = m_CacheHotSpots[i].m_pCache;
			m_szCacheDetails = m_CacheHotSpots[i].m_pCache->m_szCacheDetails;
			m_cCacheDetailsEdit.SetWindowPos(NULL, point.x+5, point.y+5, 0, 0, SW_SHOW);
			nShowCacheDetails = SW_SHOW;
		}			
	}

	BOOL bWasVisible = m_cCacheDetailsEdit.IsWindowVisible();

	m_cCacheDetailsEdit.ShowWindow(nShowCacheDetails);

	if(bWasVisible && nShowCacheDetails == SW_HIDE)
	{
		if(m_pUpdateRect == NULL)
		{
			m_pUpdateRect = new CRect();
		}

		m_cCacheDetailsEdit.GetWindowRect(m_pUpdateRect);

		//m_bCatchMe = TRUE;

		m_pUpdateRect->top -= 72; // ?? dunno why
		m_pUpdateRect->bottom += 64;
		m_pUpdateRect->left -= 72; // ?? dunno why
		m_pUpdateRect->right += 64;

		InvalidateRect(m_pUpdateRect);
	}
	else if ((nFlags & MK_SHIFT) && m_bFogOfWarCheck)
	{
		int nStartX = m_nUnshiftedMouseX;
		int nEndX = m_nMouseX;
		
		if (m_nMouseX < m_nUnshiftedMouseX)
		{
			nStartX = m_nMouseX; 
			nEndX = m_nUnshiftedMouseX;
		}

		int nStartY = m_nUnshiftedMouseY;
		int nEndY = m_nMouseY;

		if (m_nMouseY < m_nUnshiftedMouseY)
		{
			nStartY = m_nMouseY;
			nEndY = m_nUnshiftedMouseY;
		}

		for (int nXX = nStartX; nXX < nEndX; ++nXX)
		{
			for (int nYY = nStartY; nYY < nEndY; ++nYY)
			{
				int nFogX = max(0, (int)(nXX / m_fViewScale / 32.0f)) % 100;
				int nFogY = max(0, (int)(nYY / m_fViewScale / 32.0f)) % 100;

				int nOldFog = m_pDNDMap->m_nFogOfWarCell[nFogY][nFogX];

				if (nFlags & MK_CONTROL)
				{
					m_pDNDMap->m_nFogOfWarCell[nFogY][nFogX] = 0;
				}
				else
				{
					m_pDNDMap->m_nFogOfWarCell[nFogY][nFogX] = 1;
				}

				if (m_pDNDMap->m_nFogOfWarCell[nFogY][nFogX] != nOldFog)
				{
					bUpdateDetachedMaps = TRUE;
				}
			}
		}

		if (bUpdateDetachedMaps)
		{
			UpdateDetachedMaps();

			m_pDNDMap->MarkChanged();
		}
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void cDMMapViewDialog::UpdateDetachedMaps()
{
	InvalidateRect(NULL);

	for (POSITION pos = m_pApp->m_DetachedMapViewMap.GetStartPosition(); pos != NULL;)
	{
		WORD wID;
		PDNDMAPVIEWDLG pMapDlg = NULL;
		m_pApp->m_DetachedMapViewMap.GetNextAssoc(pos, wID, pMapDlg);

		if (pMapDlg != NULL && pMapDlg->m_pDNDMap != NULL && pMapDlg->m_pDNDMap->m_dwMapID == m_pDNDMap->m_dwMapID)
		{
			SyncDetachedMaps(this, pMapDlg, TRUE);
		}
	}

	for (POSITION pos = m_pApp->m_MapViewMap.GetStartPosition(); pos != NULL;)
	{
		WORD wID;
		PDNDMAPVIEWDLG pMapDlg = NULL;
		m_pApp->m_MapViewMap.GetNextAssoc(pos, wID, pMapDlg);

		if (pMapDlg != this && pMapDlg != NULL && pMapDlg->m_pDNDMap != NULL && pMapDlg->m_pDNDMap->m_dwMapID == m_pDNDMap->m_dwMapID)
		{
			SyncDetachedMaps(this, pMapDlg, FALSE);
		}
	}
}

void cDMMapViewDialog::SyncDetachedMaps(PDNDMAPVIEWDLG pMapDlg1, PDNDMAPVIEWDLG pMapDlg2, BOOL bSyncSFX)
{
	pMapDlg1->m_pDNDMap->m_nFogOfWarFlag = (pMapDlg1->m_pDNDMap->m_nFogOfWarFlag + 1) % 32000;

	pMapDlg2->m_pDNDMap->m_nFogOfWarFlag = pMapDlg1->m_pDNDMap->m_nFogOfWarFlag;

	memcpy(pMapDlg2->m_pDNDMap->m_nFogOfWarCell, pMapDlg1->m_pDNDMap->m_nFogOfWarCell, 100 * 100 * sizeof(int));

	pMapDlg2->m_nIconScale = pMapDlg1->m_nIconScale;

	pMapDlg2->m_nLightingSlider = pMapDlg1->m_nLightingSlider;

	pMapDlg2->m_fLightingAlpha = float(pMapDlg2->m_nLightingSlider) / 100.0f;

	if (bSyncSFX)
	{
		pMapDlg2->CleanupMapSFX();

		for (int i = 0; i < MAX_MAP_SFX; ++i)
		{
			memcpy(&pMapDlg2->m_pDNDMap->m_MapSFX[i], &pMapDlg1->m_pDNDMap->m_MapSFX[i], sizeof(cDNDMapSFX));
			pMapDlg2->m_pDNDMap->m_MapSFX[i].m_pDataPtr = NULL;
		}
	}

	pMapDlg2->InvalidateRect(NULL);
}

void cDMMapViewDialog::OnEditButton() 
{

	OnHideButton();

	(GetDlgItem(  IDC_SHOW_BUTTON ))->ShowWindow(SW_HIDE);

	if(m_pMapEditDlg == NULL)
	{
		m_pMapEditDlg = new DMMapEditDialog(m_pDNDMap, this);
	}

	m_pDNDMap->MarkChanged();
}

void cDMMapViewDialog::ReturnFromEdit()
{
	OnShowButton();

	if(m_pMapEditDlg != NULL)
	{
		m_pMapEditDlg->PostMessage(WM_CLOSE);
		m_pMapEditDlg = NULL;
	}
}

cDNDMap* cDMMapViewDialog::LoadMap()
{
	OnLoadButton();

	return m_pDNDMap;
}

void cDMMapViewDialog::LoadMapFromFile(char *szFileName)
{
	FILE *pInFile = fopen(szFileName, "rb");

	if(pInFile != NULL)
	{
		for(int i = 0; i < MAX_MAP_CELLS; ++i)
		{
			for(int j = 0; j < MAX_MAP_CELLS; ++j)
			{
				cDNDMapCell *pCell = &m_pDNDMap->m_Cells[i][j];

				if(pCell->m_pBitmap != NULL)
				{
					delete pCell->m_pBitmap;
					pCell->m_pBitmap = NULL;
				}
			}
		}

		if(m_pDNDMap != NULL)
		{
			delete m_pDNDMap;
			m_pDNDMap = new cDNDMap();
		}

		fread(m_pDNDMap, sizeof(cDNDMap), 1, pInFile);

		fclose(pInFile);

		if(m_pDNDMap->m_Version < 10009)
		{
			m_pDNDMap->m_bTiles = FALSE;
			memset(m_pDNDMap->m_Tiles, 0, MAX_MAP_TILES * sizeof(cDNDMapTile));
		}

		if (m_pDNDMap->m_Version < 10026)
		{
			m_pDNDMap->m_bDisplayLayer[0] = FALSE;
			m_pDNDMap->m_bDisplayLayer[1] = FALSE;
			m_pDNDMap->m_bDisplayLayer[2] = FALSE;
			m_pDNDMap->m_bDisplayLayer[3] = FALSE;

			m_pDNDMap->m_nTransRed = 255;
			m_pDNDMap->m_nTransGreen = 255;
			m_pDNDMap->m_nTransBlue = 255;
		}

		m_bLayer1 = m_pDNDMap->m_bDisplayLayer[0];
		m_bLayer2 = m_pDNDMap->m_bDisplayLayer[1];
		m_bLayer3 = m_pDNDMap->m_bDisplayLayer[2];
		m_bLayer4 = m_pDNDMap->m_bDisplayLayer[3];

		strcpy(m_pDNDMap->m_szLoadedFilename, szFileName);

		for (int i = 0; i < MAX_MAP_SFX; ++i)
		{
			m_pDNDMap->m_MapSFX[i].m_pDataPtr = NULL;
		}
		
		m_pDNDMap->MarkSaved();

		SetWindowText(m_pDNDMap->m_szMapName);

		m_pApp->m_pMainWindow->InvalidateRect(NULL);

		m_pParent->InvalidateRect(NULL);

		InvalidateRect(NULL);

		if(m_pDNDMap->m_dwMapID != 0)
		{
			m_pApp->m_MapViewMap.SetAt((WORD)m_pDNDMap->m_dwMapID, this);
		}	

		LoadMapCaches(szFileName);
	}
}

void cDMMapViewDialog::LoadMapCaches(char *szFileName)
{
	CString szCacheFileName = szFileName;

	szCacheFileName.MakeLower();

	szCacheFileName.Replace(".dmm", ".dmi");

	FILE *pInFile = fopen(szCacheFileName.GetBuffer(0), "rb");

	if(pInFile != NULL)
	{
		if(m_pTreasureCaches == NULL)
		{
			m_pTreasureCaches = new cDNDTreasureCache();
		}

		//fread(m_pTreasureCaches, sizeof(cDNDTreasureCache), 1, pInFile);
		fread(&m_pTreasureCaches->m_Header, sizeof(cDNDTreasureCacheHeader), 1, pInFile);
		fread(&m_pTreasureCaches->m_Caches, sizeof(cDNDCacheInventory), m_pTreasureCaches->m_Header.m_nCaches, pInFile);

		fclose(pInFile);

		m_pTreasureCaches->MarkSaved();

		for(int i = 0; i < MAX_MAP_CACHES; ++i)
		{
			//m_pTreasureCaches->m_Caches[i].MarkSaved();

			if(m_pTreasureCaches->m_Caches[i].m_dwCacheID != 0)
			{
				cDNDNonPlayerCharacter *pNPC = new cDNDNonPlayerCharacter();

				memset(pNPC, 0, sizeof(cDNDNonPlayerCharacter));

				pNPC->m_bIsNPC = TRUE;
				pNPC->m_bIsCache = TRUE;
				pNPC->m_dwCharacterID = m_pTreasureCaches->m_Caches[i].m_dwCacheID;

				strcpy(pNPC->m_szCharacterName, m_pTreasureCaches->m_Caches[i].m_szCacheDesc);
				strcpy(pNPC->m_szDetails, m_pTreasureCaches->m_Caches[i].m_szCacheDetails);

				pNPC->m_fMapLocationX = m_pTreasureCaches->m_Caches[i].m_fMapLocationX;
				pNPC->m_fMapLocationY = m_pTreasureCaches->m_Caches[i].m_fMapLocationY;

				pNPC->m_fLocalLocationX = m_pTreasureCaches->m_Caches[i].m_fLocalLocationX;
				pNPC->m_fLocalLocationY = m_pTreasureCaches->m_Caches[i].m_fLocalLocationY;

				pNPC->m_lCopperCarried = m_pTreasureCaches->m_Caches[i].m_lCopperCarried;
				pNPC->m_lSilverCarried = m_pTreasureCaches->m_Caches[i].m_lSilverCarried;
				pNPC->m_lElectrumCarried = m_pTreasureCaches->m_Caches[i].m_lElectrumCarried;
				pNPC->m_lGoldCarried = m_pTreasureCaches->m_Caches[i].m_lGoldCarried;
				pNPC->m_lPlatinumCarried = m_pTreasureCaches->m_Caches[i].m_lPlatinumCarried;
				
				memcpy(pNPC->m_Inventory, m_pTreasureCaches->m_Caches[i].m_Inventory ,MAX_CHARACTER_INVENTORY * sizeof (cDNDObject));

				pNPC->ValidateInventoryFlags();

				cDMBaseNPCViewDialog *pDlg = new cDMBaseNPCViewDialog(m_pMainDialog, pNPC, &m_pTreasureCaches->m_Caches[i], &m_pMainDialog->m_cMainTab);
				pDlg->m_dwMapID = m_pDNDMap->m_dwMapID;

				m_pApp->m_NPCViewMap.SetAt((WORD)m_pTreasureCaches->m_Caches[i].m_dwCacheID, pDlg);

				m_pMainDialog->AddTab(pDlg, DND_TAB_TYPE_CACHE, FALSE);
			}
			else
			{
				break;
			}
		}
		
	}
}

void cDMMapViewDialog::OnLoadButton() 
{
	DMLoadFileDescriptor FileDesc;
	FileDesc.m_szLabel = "Load DM Map File:";
	FileDesc.m_szInitialPath = m_pApp->m_szEXEPath;
	FileDesc.m_szSubDirectory = "data/maps";
	FileDesc.m_szFileTypes = ".dmm";
	FileDesc.m_bSave = FALSE;

#if NEW_FILE_MANAGEMENT_WINDOW
	m_pApp->CallFileManagementDialog(&FileDesc);
#else
	DMLoadFileDialog *pDlg = new DMLoadFileDialog(&FileDesc);
	pDlg->DoModal();
	delete pDlg;
#endif
	
	

	if(FileDesc.m_bSuccess)
	{
		LoadMapFromFile(FileDesc.m_szReturnedPath.GetBuffer(0));
	}

}

void cDMMapViewDialog::SaveExternal() 
{
	OnSaveButton();
}

void cDMMapViewDialog::OnSaveButton() 
{
	DMLoadFileDescriptor FileDesc;
	FileDesc.m_szLabel = "Save DM Map File:";
	FileDesc.m_szInitialPath = m_pApp->m_szEXEPath;
	FileDesc.m_szSubDirectory = "data/maps";
	FileDesc.m_szFileTypes = ".dmm";
	FileDesc.m_bSave = TRUE;

	FileDesc.m_szInitialFileName = m_pDNDMap->m_szMapName;
	FileDesc.m_szOriginalFileName = m_pDNDMap->m_szLoadedFilename;

#if NEW_FILE_MANAGEMENT_WINDOW
	m_pApp->CallFileManagementDialog(&FileDesc);
#else
	DMLoadFileDialog *pDlg = new DMLoadFileDialog(&FileDesc);
	pDlg->DoModal();
	delete pDlg;
#endif

	if(FileDesc.m_bSuccess)
	{
		SaveMapToFile(FileDesc.m_szReturnedPath.GetBuffer(0));

		if (m_pDNDMap->m_szLoadedFilename[0] == 0)
		{
			strcpy(m_pDNDMap->m_szLoadedFilename, FileDesc.m_szReturnedPath.Left(511));
		}
	}	
}

void cDMMapViewDialog::SaveMapToFile(char *szFileName)
{
	FILE *pOutFile = fopen(szFileName, "wb");

	if(pOutFile != NULL)
	{
		//path fixup
		for(int i = 0; i < MAX_MAP_CELLS; ++i)
		{
			for(int j = 0; j < MAX_MAP_CELLS; ++j)
			{
				cDNDMapCell *pCell = &m_pDNDMap->m_Cells[i][j];

				if(pCell->m_pBitmap)
				{
					delete pCell->m_pBitmap;
					pCell->m_pBitmap = NULL;
				}

				if(pCell->m_szBitmapPath[0])
				{
					CString szPath;
					szPath.Format("%s", pCell->m_szBitmapPath);
					szPath.MakeUpper();

					szPath.Replace(m_pApp->m_szEXEPath, "<$DMAPATH>");
					strcpy(pCell->m_szBitmapPath, szPath.GetBuffer(0));
				}
			}
		}

		if(m_pDNDMap->m_dwMapID == 0L)
		{
			m_pDNDMap->m_dwMapID = GetUniqueID();

			m_pApp->m_MapViewMap.SetAt((WORD)m_pDNDMap->m_dwMapID, this);
		}

		m_pDNDMap->m_Version = DMH_CURRENT_VERSION;

		fwrite(m_pDNDMap, sizeof(cDNDMap), 1, pOutFile);

		fclose(pOutFile);

		m_pDNDMap->MarkSaved();

		CString szCacheFileName = szFileName;
		szCacheFileName.MakeLower();
		szCacheFileName.Replace(".dmm", ".dmi");

		DeleteFile(szCacheFileName); //if it's empty, it needs to disappear from the harddrive

		if(m_pTreasureCaches != NULL)
		{
			m_pTreasureCaches->CountCaches();

			if(m_pTreasureCaches->m_Header.m_nCaches)
			{
				m_pTreasureCaches->MarkSaved();

				pOutFile = fopen(szCacheFileName.GetBuffer(0), "wb");

				if(pOutFile != NULL)
				{
					fwrite(&m_pTreasureCaches->m_Header, sizeof(cDNDTreasureCacheHeader), 1, pOutFile);
					fwrite(&m_pTreasureCaches->m_Caches, sizeof(cDNDCacheInventory), m_pTreasureCaches->m_Header.m_nCaches, pOutFile);

					fclose(pOutFile);
				}
			}

		}


	}
	else
	{
		AfxMessageBox("Unable to save file !", MB_OK);
	}
}



void cDMMapViewDialog::OnClose() 
{
	m_bShuttingDown = TRUE;

	KillTimer(m_nWindowTimer);

	if(m_pDNDMap != NULL)
	{
		CleanupMapSFX();

		m_pApp->m_MapViewMap.RemoveKey((WORD)m_pDNDMap->m_dwMapID);
		m_pApp->m_DetachedMapViewMap.RemoveKey((WORD)m_pDNDMap->m_dwMapID);

		delete m_pDNDMap;
		m_pDNDMap = NULL;
	}

	if(m_pMapEditDlg != NULL)
	{
		m_pMapEditDlg->PostMessageA(WM_CLOSE);
		m_pMapEditDlg = NULL;
	}

	if(m_pTreasureCaches != NULL)
	{
		for(int i = 0; i < MAX_MAP_CACHES; ++i)
		{
			if(m_pTreasureCaches->m_Caches[i].m_dwCacheID != 0)
			{
				m_pApp->m_NPCViewMap.SetAt((WORD)m_pTreasureCaches->m_Caches[i].m_dwCacheID, NULL);
				m_pApp->m_NPCViewMap.RemoveKey((WORD)m_pTreasureCaches->m_Caches[i].m_dwCacheID);
			}
		}

		delete m_pTreasureCaches;
		m_pTreasureCaches = NULL;
	}

	m_pApp->ResetDataPicker(&m_MonsterNameIndexer);

	#if _PARTICLE_WEATHER
	if (m_pRainParticleBitmap != NULL)
	{
		delete m_pRainParticleBitmap;
	}

	if (m_pSnowParticleBitmap != NULL)
	{
		delete m_pSnowParticleBitmap;
	}
	#endif

	if (m_pLightingAlphaBitmap != NULL)
	{
		delete m_pLightingAlphaBitmap;
	}

	if (m_pFogOfWarBitmap != NULL)
	{
		delete m_pFogOfWarBitmap;
	}

	if (m_pSFXButtonBitmap != NULL)
	{
		delete m_pSFXButtonBitmap;
	}
	
	CDialog::OnClose();

	delete this;
}

void cDMMapViewDialog::PostNcDestroy() 
{
	m_bShuttingDown = TRUE;

	if(m_pDNDMap != NULL)
	{
		CleanupMapSFX();

		m_pApp->m_MapViewMap.RemoveKey((WORD)m_pDNDMap->m_dwMapID);

		delete m_pDNDMap;
		m_pDNDMap = NULL;
	}
	
	CDialog::PostNcDestroy();

	delete this;
}


void cDMMapViewDialog::DrawMapText(char * szData, int x, int y, CDC *memdc)
{

	CRect rect;
	GetClientRect(&rect);

	int nOldX = x;

	if(m_pUpdateRect != NULL)
	{
		CString szTemp = szData;

		if(szTemp.Find("Celia", 0) != -1)
		{
			TRACE("?\n");
		}

		x -= m_pUpdateRect->left;
		y -= m_pUpdateRect->top;

		rect.left = x;
		rect.top = y;
	}

	if(x >= rect.left && x <= rect.right && y >= rect.top && y <= rect.bottom)
	{
		rect.left = x;
		rect.top = y;
		rect.right = rect.left + 500;
		rect.bottom = rect.top + 50;

		memdc->DrawText(szData, &rect, DT_SINGLELINE | DT_LEFT );
	}

}

void cDMMapViewDialog::OnSelchangePartyCombo() 
{
	cDNDParty *pOldParty = m_pSelectedParty;

	int nCursor = m_cPartyCombo.GetCurSel();

	m_cSubPartyCombo.EnableWindow(FALSE);

	if(nCursor >= 0)
	{
		m_pSelectedParty = (cDNDParty *)m_cPartyCombo.GetItemData(nCursor);
		m_szHint.Format("Selected Party: %s", m_pSelectedParty->m_szPartyName);

		m_cSubPartyCombo.ResetContent();

		if (m_pSelectedParty != NULL)
		{
			int nFoundSubParty = 0;
			m_cSubPartyCombo.InsertString(0, _T(""));
			m_cSubPartyCombo.SetItemData(0, 0);
			int nSubRow = 1;
			for (int i = 0; i < MAX_SUB_PARTIES; ++i)
			{
				if (m_pSelectedParty->m_SubParty[i].m_dwSubPartyID)
				{
					m_cSubPartyCombo.EnableWindow(TRUE);
					m_cSubPartyCombo.InsertString(nSubRow, m_pSelectedParty->m_SubParty[i].m_szSubPartyName);
					m_cSubPartyCombo.SetItemData(nSubRow, m_pSelectedParty->m_SubParty[i].m_dwSubPartyID);

					if (m_pSelectedParty->m_SubParty[i].m_dwSubPartyID == m_dwSelectedSubPartyID)
					{
						nFoundSubParty = i+1;
					}
					++nSubRow;
				}
			}

			if (nFoundSubParty)
			{
				m_cSubPartyCombo.SetCurSel(nFoundSubParty);
			}
			else
			{
				m_dwSelectedSubPartyID = 0;
			}
		}


		m_cCharacterCombo.ResetContent();

		int nRow = 0;
		WORD wID;
		for (POSITION pos = m_pApp->m_CharacterViewMap.GetStartPosition(); pos != NULL; )
		{
			PDNDCHARVIEWDLG pCharDlg = NULL;
			m_pApp->m_CharacterViewMap.GetNextAssoc(pos,wID,pCharDlg);

			if(pCharDlg != NULL && pCharDlg->m_pCharacter != NULL && pCharDlg->m_pCharacter->m_dwCharacterID)
			{
				if(!m_pSelectedParty->CharacterIsPartyMember(pCharDlg->m_pCharacter))
					continue;

				if (m_dwSelectedSubPartyID != 0 && pCharDlg->m_pCharacter->m_dwSubPartyID != m_dwSelectedSubPartyID)
					continue;

				m_cCharacterCombo.InsertString(nRow, pCharDlg->m_pCharacter->m_szCharacterName);
				m_cCharacterCombo.SetItemData(nRow, (ULONG)pCharDlg->m_pCharacter);

				++nRow;
			}
		}
		for (pos = m_pApp->m_NPCViewMap.GetStartPosition(); pos != NULL; )
		{
			PDNDNPCVIEWDLG pNPCDlg = NULL;
			m_pApp->m_NPCViewMap.GetNextAssoc(pos,wID,pNPCDlg);

			if(pNPCDlg != NULL && pNPCDlg->m_pNPC != NULL && pNPCDlg->m_pNPC->m_dwCharacterID)
			{
				if(!m_pSelectedParty->CharacterIsPartyMember(pNPCDlg->m_pNPC))
					continue;

				if (m_dwSelectedSubPartyID != 0 && pNPCDlg->m_pNPC->m_dwSubPartyID != m_dwSelectedSubPartyID)
					continue;

				m_cCharacterCombo.InsertString(nRow, pNPCDlg->m_pNPC->m_szCharacterName);
				m_cCharacterCombo.SetItemData(nRow, (ULONG)pNPCDlg->m_pNPC);

				++nRow;
			}
		}
	  
	}
	else
	{
		m_pSelectedParty = NULL;
		m_szHint = _T("");
	}

	if (m_pSelectedParty != NULL)
	{
		m_cCharacterCombo.EnableWindow(TRUE);
	}

	UpdateData(FALSE);
	
}

void cDMMapViewDialog::OnCbnSelchangeSubpartyCombo()
{
	int nCursor = m_cSubPartyCombo.GetCurSel();

	if (nCursor > -1)
	{
		m_dwSelectedSubPartyID = m_cSubPartyCombo.GetItemData(nCursor);
		OnSelchangePartyCombo();
	}
}


void cDMMapViewDialog::OnPlacePartyButton() 
{
	if(m_pSelectedParty != NULL)
	{
		m_MapMode = DND_MAP_MODE_PLACE_PARTY;
		m_szHint = _T("Double click on the party's new location");
	}
	else
	{
		m_szHint = _T("No party selected !");
	}

	UpdateData(FALSE);
	
}

void cDMMapViewDialog::OnPlaceCharacterButton() 
{
	if(m_pSelectedCharacter != NULL)
	{
		m_MapMode = DND_MAP_MODE_PLACE_CHARACTER;
		m_szHint = _T("Double click on the character's new location");
	}
	else
	{
		m_szHint = _T("No character selected !");
	}

	UpdateData(FALSE);
	
}


void cDMMapViewDialog::OnPlaceOnPartyButton() 
{
	if(m_pSelectedParty != NULL)
	{
		CRect Rect;
		GetWindowRect (Rect);

		int nSizeX = Rect.right - Rect.left;
		int nSizeY = Rect.bottom - Rect.top;

		nSizeX /= 2;
		nSizeY /= 2;

		float fX = 0.0f;
		float fY = 0.0f;

		if(m_pDNDMap->m_bMapScaleFeet)
		{
			//feet to pixels
			fX =  (m_pSelectedParty->m_fPartyLocalLocationX) / m_pDNDMap->m_fScaleX * m_fViewScale;
			fY =  (m_pSelectedParty->m_fPartyLocalLocationY) / m_pDNDMap->m_fScaleY * m_fViewScale;
		}
		else
		{
			//miles to pixels
			fX =  (m_pSelectedParty->m_fPartyLocationX - m_pDNDMap->m_fParentMapOriginX) / m_pDNDMap->m_fScaleX * m_fViewScale;
			fY =  (m_pSelectedParty->m_fPartyLocationY - m_pDNDMap->m_fParentMapOriginY) / m_pDNDMap->m_fScaleY * m_fViewScale;
		}

		m_nCornerX = (int)(nSizeX - fX);
		m_nCornerY = (int)(nSizeY - fY);

		m_szHint.Format("Centered on party: %s", m_pSelectedParty->m_szPartyName);

		InvalidateRect(NULL);
	}
	else
	{
		m_szHint = _T("No party selected !");
	}

	UpdateData(FALSE);
}


void cDMMapViewDialog::OnPlotPartyRouteButton() 
{

	if(m_pSelectedParty != NULL)
	{
		m_MapMode = DND_MAP_MODE_PLOT_PARTY_ROUTE;
		m_szHint = _T("Double each waypoint along the route, press ESC when done");

		for(int i = 0; i < MAX_PARTY_WAYPOINTS; ++i)
		{
			m_pSelectedParty->m_WayPoints[i].Clear();
		}
	}
	else
	{
		m_szHint = _T("No party selected !");
	}

	UpdateData(FALSE);
	
}

void cDMMapViewDialog::OnPlotPartyExtendRouteButton() 
{
	// TODO: Add your control notification handler code here
	if(m_pSelectedParty != NULL)
	{
		m_MapMode = DND_MAP_MODE_PLOT_PARTY_ROUTE;
		m_szHint = _T("Double each waypoint along the route, press ESC when done");
	}
	else
	{
		m_szHint = _T("No party selected !");
	}

	UpdateData(FALSE);
}

void cDMMapViewDialog::OnHideButton() 
{
	for(int i = 0; _MapControlStater[i][0] != -1; ++i)
	{
		if(_MapControlStater[i][1])
			(GetDlgItem(  _MapControlStater[i][0] ))-> ShowWindow(SW_HIDE);
		else
			(GetDlgItem(  _MapControlStater[i][0] ))-> ShowWindow(SW_SHOW);
	}

	SetFocus();
	
}

void cDMMapViewDialog::OnShowButton() 
{
	for(int i = 0; _MapControlStater[i][0] != -1; ++i)
	{
		if(!_MapControlStater[i][1])
			(GetDlgItem(  _MapControlStater[i][0] ))-> ShowWindow(SW_HIDE);
		else
			(GetDlgItem(  _MapControlStater[i][0] ))-> ShowWindow(SW_SHOW);
	}

	Refresh();
}

void cDMMapViewDialog::OnSelchangeCharacterCombo() 
{
	int nCursor = m_cCharacterCombo.GetCurSel();

	if(nCursor >= 0)
	{
		m_pSelectedCharacter = (cDNDCharacter *)m_cCharacterCombo.GetItemData(nCursor);
		m_szHint.Format("Selected Character: %s", m_pSelectedCharacter->m_szCharacterName);

	}

	UpdateData(FALSE);
	
}



BOOL cDMMapViewDialog::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


void cDMMapViewDialog::SelectAndCenterOnParty(DWORD dwPartyID)
{
	for(int i = 0; i < m_cPartyCombo.GetCount(); ++i)
	{
		cDNDParty *pParty = (cDNDParty *)m_cPartyCombo.GetItemData(i);

		if(pParty->m_dwPartyID == dwPartyID)
		{
			m_cPartyCombo.SetCurSel(i);
			m_pSelectedParty = pParty;

			UpdateData(FALSE);

			OnPlaceOnPartyButton();

			break;
		}
	}


}

BOOL cDMMapViewDialog::FindAndSelectMapTab(DWORD dwMapID)
{
	for (int i = 0; i < m_pApp->m_TabArray.GetSize(); ++i)
	{
		cDNDDisplayTab *pTab = m_pApp->m_TabArray[i];

		if(pTab != NULL)
		{
			if(pTab->m_nTabType == DND_TAB_TYPE_MAP)
			{

				cDMMapViewDialog *pDlg = (cDMMapViewDialog *)pTab->m_pWindow;

				if(pDlg->m_pDNDMap->m_dwMapID == dwMapID)
				{
					CDMHelperDlg *pMainDlg = (CDMHelperDlg *)m_pApp->m_pMainWindow;

					pMainDlg->m_cMainTab.SetCurSel(i);
					pMainDlg->PickTab();

					return TRUE;
				}

			}
		}
	}

	return FALSE;
}

void cDMMapViewDialog::ProcessMapModes()
{ 
	#ifdef _DEBUG
	if(m_pDNDMap->m_bMapScaleFeet)
	{
		TRACE("KERBOOM!\n");
	}
	if(m_MapMode == DND_MAP_MODE_PICKED_CHILD_POINT_1)
	{
		TRACE("SHAT!\n");
	}
	#endif

	switch(m_MapMode)
	{
		case DND_MAP_MODE_PICK_MSG_CHILD_POINT_1:
		{
			if(m_bWaitingOnUserClick == FALSE)
			{
				m_bWaitingOnUserClick = TRUE;

				if(AfxMessageBox("Double left click on a location on this map that corresponds to a location on the parent map", MB_ICONQUESTION|MB_OKCANCEL) == IDOK)
				{
					m_MapMode = DND_MAP_MODE_PICK_CHILD_POINT_1;
				}
				else
				{
					m_MapMode = DND_MAP_MODE_UNDEF;
				}

				m_bWaitingOnUserClick = FALSE;
			}

			break;
		}
		case DND_MAP_MODE_PICKED_CHILD_POINT_1:
		{
			m_MapMode = DND_MAP_MODE_UNDEF;

			m_pApp->m_MapScaler.m_pParentMapView->m_MapMode = DND_MAP_MODE_PICK_MSG_PARENT_POINT_1;
			FindAndSelectMapTab(m_pApp->m_MapScaler.m_dwParentMapID);

			break;
		}
		case DND_MAP_MODE_PICK_MSG_PARENT_POINT_1:
		{
			if(m_bWaitingOnUserClick == FALSE)
			{
				m_bWaitingOnUserClick = TRUE;
				if(AfxMessageBox("Double left click on a location on this map that corresponds to the location you just selected on the child map", MB_ICONQUESTION|MB_OKCANCEL) == IDOK)
				{
					m_MapMode = DND_MAP_MODE_PICK_PARENT_POINT_1;
				}
				else
				{
					m_MapMode = DND_MAP_MODE_UNDEF;
				}
				m_bWaitingOnUserClick = FALSE;
			}

			break;
		}
		case DND_MAP_MODE_PICKED_PARENT_POINT_1:
		{
			m_MapMode = DND_MAP_MODE_UNDEF;

			if(m_pApp->m_MapScaler.m_pChildMapView->m_pDNDMap->m_bMapScaleFeet)
			{
				m_pApp->m_MapScaler.m_pChildMapView->m_MapMode = DND_MAP_MODE_PICK_MSG_COMPLETE;
			}
			else
			{
				m_pApp->m_MapScaler.m_pChildMapView->m_MapMode = DND_MAP_MODE_PICK_MSG_CHILD_POINT_2;
			}

			FindAndSelectMapTab(m_pApp->m_MapScaler.m_dwChildMapID);

			break;
		}
		case DND_MAP_MODE_PICK_MSG_CHILD_POINT_2:
		{
			if(m_bWaitingOnUserClick == FALSE)
			{
				m_bWaitingOnUserClick = TRUE;
				if(AfxMessageBox("Double left click on another location on this map that corresponds to a location on the parent map", MB_ICONQUESTION|MB_OKCANCEL) == IDOK)
				{
					m_MapMode = DND_MAP_MODE_PICK_CHILD_POINT_2;
				}
				else
				{
					m_MapMode = DND_MAP_MODE_UNDEF;
				}
				m_bWaitingOnUserClick = FALSE;
			}

			break;
		}
		case DND_MAP_MODE_PICKED_CHILD_POINT_2:
		{
			m_MapMode = DND_MAP_MODE_UNDEF;

			m_pApp->m_MapScaler.m_pParentMapView->m_MapMode = DND_MAP_MODE_PICK_MSG_PARENT_POINT_2;
			FindAndSelectMapTab(m_pApp->m_MapScaler.m_dwParentMapID);

			break;
		}
		case DND_MAP_MODE_PICK_MSG_PARENT_POINT_2:
		{
			if(m_bWaitingOnUserClick == FALSE)
			{
				m_bWaitingOnUserClick = TRUE;
				if(AfxMessageBox("Double left click on a location on this map that corresponds to the location you just selected on the child map", MB_ICONQUESTION|MB_OKCANCEL) == IDOK)
				{
					m_MapMode = DND_MAP_MODE_PICK_PARENT_POINT_2;
				}
				else
				{
					m_MapMode = DND_MAP_MODE_UNDEF;
				}
				m_bWaitingOnUserClick = FALSE;
			}

			break;
		}
		case DND_MAP_MODE_PICKED_PARENT_POINT_2:
		{
			m_MapMode = DND_MAP_MODE_UNDEF;

			m_pApp->m_MapScaler.m_pChildMapView->m_MapMode = DND_MAP_MODE_PICK_MSG_COMPLETE;
			FindAndSelectMapTab(m_pApp->m_MapScaler.m_dwChildMapID);

			break;
		}
		case DND_MAP_MODE_PICK_MSG_COMPLETE:
		{
			m_MapMode = DND_MAP_MODE_UNDEF;

			if(m_pDNDMap->m_bMapScaleFeet)
			{
				float fX = (float)(m_pApp->m_MapScaler.m_nParentMapX1) * m_pApp->m_MapScaler.m_pParentMapView->m_pDNDMap->m_fScaleX / m_pApp->m_MapScaler.m_pParentMapView->m_fViewScale + m_pApp->m_MapScaler.m_pParentMapView->m_pDNDMap->m_fParentMapOriginX;
				float fY = (float)(m_pApp->m_MapScaler.m_nParentMapY1) * m_pApp->m_MapScaler.m_pParentMapView->m_pDNDMap->m_fScaleY / m_pApp->m_MapScaler.m_pParentMapView->m_fViewScale + m_pApp->m_MapScaler.m_pParentMapView->m_pDNDMap->m_fParentMapOriginY;

				m_pDNDMap->m_fParentMapOriginX = fX;
				m_pDNDMap->m_fParentMapOriginY = fY;
			}
			else
			{
				float fPX1 = (float)m_pApp->m_MapScaler.m_nParentMapX1;
				float fPY1 = (float)m_pApp->m_MapScaler.m_nParentMapY1;

				float fPX2 = (float)m_pApp->m_MapScaler.m_nParentMapX2;
				float fPY2 = (float)m_pApp->m_MapScaler.m_nParentMapY2;

				float fCX1 = (float)m_pApp->m_MapScaler.m_nChildMapX1;
				float fCY1 = (float)m_pApp->m_MapScaler.m_nChildMapY1;

				float fCX2 = (float)m_pApp->m_MapScaler.m_nChildMapX2;
				float fCY2 = (float)m_pApp->m_MapScaler.m_nChildMapY2;

				float fParentDist = (float) sqrt (((fPX1-fPX2) * (fPX1-fPX2)) + ((fPY1-fPY2) * (fPY1-fPY2)) );
				float fChildDist = (float) sqrt (((fCX1-fCX2) * (fCX1-fCX2)) + ((fCY1-fCY2) * (fCY1-fCY2)) );


				float dX = (fPX1-fPX2) / (fCX1-fCX2);
				float dY = (fPY1-fPY2) / (fCY1-fCY2);

				m_pDNDMap->m_fScaleX = dX * m_pApp->m_MapScaler.m_pParentMapView->m_pDNDMap->m_fScaleX;
				m_pDNDMap->m_fScaleY = dY * m_pApp->m_MapScaler.m_pParentMapView->m_pDNDMap->m_fScaleY;

				m_pDNDMap->m_fParentMapOriginX = (fPX1 * m_pApp->m_MapScaler.m_pParentMapView->m_pDNDMap->m_fScaleX) - (fCX1 * m_pDNDMap->m_fScaleX);
				m_pDNDMap->m_fParentMapOriginY = (fPY1 * m_pApp->m_MapScaler.m_pParentMapView->m_pDNDMap->m_fScaleY) - (fCY1 * m_pDNDMap->m_fScaleY);
			}

			AfxMessageBox("Map scaling complete !", MB_OK);

			break;
		}

		//////////////////////////////////////

		case DND_MAP_MODE_PICK_MSG_SCALE_POINT_1:
		{
			if(m_bWaitingOnUserClick == FALSE)
			{
				m_bWaitingOnUserClick = TRUE;
				if(AfxMessageBox("Double left click on a location on this map that is the first point (of two points) you know the exact distance between", MB_ICONQUESTION|MB_OKCANCEL) == IDOK)
				{
					m_MapMode = DND_MAP_MODE_PICK_SCALE_POINT_1;
				}
				else
				{
					m_MapMode = DND_MAP_MODE_UNDEF;
				}
				m_bWaitingOnUserClick = FALSE;
			}

			break;
		}

		case DND_MAP_MODE_PICKED_SCALE_POINT_1:
		{
			m_MapMode = DND_MAP_MODE_PICK_MSG_SCALE_POINT_2;

			InvalidateRect(NULL);

			break;
		}

		case DND_MAP_MODE_PICK_MSG_SCALE_POINT_2:
		{
			if(m_bWaitingOnUserClick == FALSE)
			{
				m_bWaitingOnUserClick = TRUE;
				if(AfxMessageBox("Double left click on a location on this map that is the second point (of two points) you know the exact distance between", MB_ICONQUESTION|MB_OKCANCEL) == IDOK)
				{
					m_MapMode = DND_MAP_MODE_PICK_SCALE_POINT_2;
				}
				else
				{
					m_MapMode = DND_MAP_MODE_UNDEF;
				}
				m_bWaitingOnUserClick = FALSE;
			}

			break;
		}

		case DND_MAP_MODE_PICKED_SCALE_POINT_2:
		{
			m_MapMode = DND_MAP_MODE_UNDEF;

			CString szPrompt;

			if(m_pDNDMap->m_bMapScaleFeet)
			{
				szPrompt = "Enter the distance (in feet) between the two points, and hit OK";
			}
			else
			{
				szPrompt = "Enter the distance (in miles) between the two points, and hit OK";
			}

			CString szRetVal = _T("");

			CDMInputDialog *pDlg = new CDMInputDialog(szPrompt.GetBuffer(0), &szRetVal);
			pDlg->DoModal();

			double fEnteredDistance = atof(szRetVal.GetBuffer(0));

			double fX1 = m_pApp->m_MapScaler.m_nParentMapX1;
			double fY1 = m_pApp->m_MapScaler.m_nParentMapY1;

			double fX2 = m_pApp->m_MapScaler.m_nParentMapX2;
			double fY2 = m_pApp->m_MapScaler.m_nParentMapY2;

			double dX = fX1-fX2;
			double dY = fY1-fY2;

			double fDist = sqrt( dX*dX + dY*dY) / m_fViewScale;

			if(fDist != 0.0f)
			{
				double fScale = fEnteredDistance / fDist;

				if(fScale != 0.0f)
				{
					m_pDNDMap->m_fScaleX = (float)fScale;
					m_pDNDMap->m_fScaleY = (float)fScale;

					if(m_pMapEditDlg != NULL)
					{
						m_pMapEditDlg->Refresh();
					}

					CString szScaled;

					if(m_pDNDMap->m_bMapScaleFeet)
					{
						szScaled.Format("Map scaling complete !\nNew scale = 1 foot = %0.5f pixels", fScale);
					}
					else
					{
						szScaled.Format("Map scaling complete !\nNew scale = 1 mile = %0.5f pixels ", fScale);
					}

					AfxMessageBox(szScaled, MB_OK);
				}
			}

			break;
		}
	}

}

void cDMMapViewDialog::OnReleasedcaptureScaleSlider(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	float fScales[9] = {0.125f, 0.25f, 0.50f, 0.75f, 1.0f, 2.0f, 4.0, 8.0f, 16.0f };
	
	UpdateData(TRUE);

	float fOldScale = m_fViewScale;

	m_fViewScale = fScales[m_nScaleSlider];

	CRect Rect;
	GetWindowRect (Rect);

	int nSizeX = Rect.right - Rect.left;
	int nSizeY = Rect.bottom - Rect.top;

	nSizeX /= 2;
	nSizeY /= 2;

	float fX = 0.0f;
	float fY = 0.0f;

	m_pSelectedParty->m_fPartyLocalLocationX;

	//BOOKMARK
	if(m_pDNDMap->m_bMapScaleFeet)
	{
		//spot we are looking at before the scale changed
		float fCX = (-m_nCornerX + nSizeX) * m_pDNDMap->m_fScaleX / fOldScale;
		float fCY = (-m_nCornerY + nSizeY) * m_pDNDMap->m_fScaleY / fOldScale;

		//feet to pixels
		fX =  fCX / m_pDNDMap->m_fScaleX * m_fViewScale;
		fY =  fCY / m_pDNDMap->m_fScaleY * m_fViewScale;
	}
	else
	{
		//spot we are looking at before the scale changed
		//float fCX = (-m_nCornerX - m_pDNDMap->m_fParentMapOriginX + nSizeX) * m_pDNDMap->m_fScaleX / fOldScale;
		//float fCY = (-m_nCornerY - m_pDNDMap->m_fParentMapOriginY + nSizeY) * m_pDNDMap->m_fScaleY / fOldScale;

		//pixels to miles
		float fCX = (-m_nCornerX + nSizeX) * m_pDNDMap->m_fScaleX / fOldScale;
		fCX +=  m_pDNDMap->m_fParentMapOriginX;

		//pixels to miles
		float fCY = (-m_nCornerY + nSizeY) * m_pDNDMap->m_fScaleY / fOldScale;
		fCY +=  m_pDNDMap->m_fParentMapOriginY;

		//miles to pixels
		fX =  (fCX - m_pDNDMap->m_fParentMapOriginX) / m_pDNDMap->m_fScaleX * m_fViewScale;
		fY =  (fCY - m_pDNDMap->m_fParentMapOriginY) / m_pDNDMap->m_fScaleY * m_fViewScale;
	}

	m_nCornerX = (int)(nSizeX - fX);
	m_nCornerY = (int)(nSizeY - fY);


	InvalidateRect(NULL);

	*pResult = 0;
}


void cDMMapViewDialog::OnCancel() 
{
	m_MapMode = DND_MAP_MODE_UNDEF;
	m_szHint = _T("");

	UpdateData(FALSE);
	
	//CDialog::OnCancel();
}

void cDMMapViewDialog::OnBnClickedFogOfWarCheck()
{
	UpdateData(TRUE);

	InvalidateRect(NULL);
}


void cDMMapViewDialog::OnTravelButton() 
{
	PDNDPARTYVIEWDLG pPartyDlg = ValidateSelectedParty();

	if(pPartyDlg != NULL && m_pSelectedParty != NULL)
	{
		m_szPartyAlert = _T("");

		float fMoveDay = ((float)m_pSelectedParty->m_nPartyMovementRate) * 2.5f;

		float fTime = 144.0; //144 turns in a day

		if(m_bTravelHoursCheck)
		{
			fTime = 6.0f; // 6 turns in an hour
			fMoveDay /= 12.0f;
		}

		float fTravelled = 0.0f;

		if(fMoveDay > 0.0f)
		{
			fTravelled = m_pSelectedParty->TravelRoute(fMoveDay);
		}

		if(fTravelled > 0.0f)
		{
			int nTurns = (int)(fTravelled / fMoveDay * fTime + 0.5f); 

			int nOldDay = m_pSelectedParty->m_nDayofMonth;
			for(int i = 0; i < nTurns; ++i)
			{
				m_pSelectedParty->ModifyCalendar(DND_CALENDAR_MOD_TURN, DND_CALENDAR_MOD_INCREMENT);
			}

			if(nOldDay != m_pSelectedParty->m_nDayofMonth)
			{
				if(m_pSelectedParty->EatRations() == FALSE)
				{
					m_szPartyAlert = "Party member is out of rations !";
				}
			}

			if(nTurns > 72)
			{
				LONG lDistance = (LONG)(fTravelled + 0.5f);

				if(fTravelled < fMoveDay)
				{
					pPartyDlg->LogPartyEvent(FALSE, APPEND_TO_LOG, DND_LOG_EVENT_TYPE_PARTY_FINISHED_JOURNEY, m_pSelectedParty->m_szPartyName, m_pSelectedParty->m_dwPartyID, lDistance, "");
				}
				else
				{
					pPartyDlg->LogPartyEvent(FALSE, APPEND_TO_LOG, DND_LOG_EVENT_TYPE_PARTY_TRAVELED, m_pSelectedParty->m_szPartyName, m_pSelectedParty->m_dwPartyID, lDistance, "");
				}
			}
		}
	}

	CPoint point;
	GetCursorPos(&point);
	OnMouseMove(0, point);

	InvalidateRect(NULL);

	Refresh();
	
}

void cDMMapViewDialog::OnBnClickedRestButton()
{
	PDNDPARTYVIEWDLG pPartyDlg = ValidateSelectedParty();

	if(pPartyDlg != NULL && m_pSelectedParty != NULL)
	{
		m_szPartyAlert = _T("");

		int nTurns = 6; 

		int nOldDay = m_pSelectedParty->m_nDayofMonth;
		for(int i = 0; i < nTurns; ++i)
		{
			m_pSelectedParty->ModifyCalendar(DND_CALENDAR_MOD_TURN, DND_CALENDAR_MOD_INCREMENT);
		}

		if(nOldDay != m_pSelectedParty->m_nDayofMonth)
		{
			if(m_pSelectedParty->EatRations() == FALSE)
			{
				m_szPartyAlert = "Party member is out of rations !";
			}

			pPartyDlg->LogPartyEvent(FALSE, APPEND_TO_LOG, DND_LOG_EVENT_TYPE_PARTY_RESTED, m_pSelectedParty->m_szPartyName, m_pSelectedParty->m_dwPartyID, 0, "");
		}		
	}

	CPoint point;
	GetCursorPos(&point);
	OnMouseMove(0, point);

	InvalidateRect(NULL);

	Refresh();
}


void cDMMapViewDialog::OnTravelMountedButton() 
{
	PDNDPARTYVIEWDLG pPartyDlg = ValidateSelectedParty();

	if(pPartyDlg != NULL && m_pSelectedParty != NULL)
	{
		m_szPartyAlert = _T("");

		float fMoveDay = ((float)m_pSelectedParty->m_nPartyMovementRate) * 2.5f;

		fMoveDay *= 3.0f; //for mounted movement

		float fTime = 144.0; //144 turns in a day

		if(m_bTravelHoursCheck)
		{
			fTime = 6.0f; // 6 turns in an hour
			fMoveDay /= 12.0f;
		}

		float fTravelled = 0.0f;

		if(fMoveDay > 0.0f)
		{
			fTravelled = m_pSelectedParty->TravelRoute(fMoveDay);
		}

		if(fTravelled > 0.0f)
		{
			int nTurns = (int)(fTravelled / fMoveDay * fTime + 0.5f); 

			int nOldDay = m_pSelectedParty->m_nDayofMonth;
			for(int i = 0; i < nTurns; ++i)
			{
				m_pSelectedParty->ModifyCalendar(DND_CALENDAR_MOD_TURN, DND_CALENDAR_MOD_INCREMENT);
			}

			if(nOldDay != m_pSelectedParty->m_nDayofMonth)
			{
				if(m_pSelectedParty->EatRations() == FALSE)
				{
					m_szPartyAlert = "Party member is out of rations !";
				}
			}

			if(nTurns > 72)
			{
				LONG lDistance = (LONG)(fTravelled + 0.5f);

				if(fTravelled < fMoveDay)
				{
					pPartyDlg->LogPartyEvent(FALSE, APPEND_TO_LOG, DND_LOG_EVENT_TYPE_PARTY_FINISHED_JOURNEY, m_pSelectedParty->m_szPartyName, m_pSelectedParty->m_dwPartyID, lDistance, "");
				}
				else
				{
					pPartyDlg->LogPartyEvent(FALSE, APPEND_TO_LOG, DND_LOG_EVENT_TYPE_PARTY_TRAVELED, m_pSelectedParty->m_szPartyName, m_pSelectedParty->m_dwPartyID, lDistance, "");
				}
			}
		}
	}

	CPoint point;
	GetCursorPos(&point);
	OnMouseMove(0, point);

	InvalidateRect(NULL);

	Refresh();
}

void cDMMapViewDialog::OnBnClickedPlaceOnCharacterButton()
{
	if(m_pSelectedCharacter != NULL)
	{
		if(m_pSelectedCharacter->m_fMapLocationX == 0.0f &&
		m_pSelectedCharacter->m_fMapLocationY == 0.0f &&
		m_pSelectedCharacter->m_fLocalLocationX == 0.0f &&
		m_pSelectedCharacter->m_fLocalLocationY == 0.0f)
		{
			//not seperate from party
			OnPlaceOnPartyButton();
			return;
		}


		CRect Rect;
		GetWindowRect (Rect);

		int nSizeX = Rect.right - Rect.left;
		int nSizeY = Rect.bottom - Rect.top;

		nSizeX /= 2;
		nSizeY /= 2;

		float fX = 0.0f;
		float fY = 0.0f;

		if(m_pDNDMap->m_bMapScaleFeet)
		{
			//feet to pixels
			fX =  (m_pSelectedCharacter->m_fLocalLocationX) / m_pDNDMap->m_fScaleX * m_fViewScale;
			fY =  (m_pSelectedCharacter->m_fLocalLocationY) / m_pDNDMap->m_fScaleY * m_fViewScale;
		}
		else
		{
			//miles to pixels
			fX =  (m_pSelectedCharacter->m_fMapLocationX - m_pDNDMap->m_fParentMapOriginX) / m_pDNDMap->m_fScaleX * m_fViewScale;
			fY =  (m_pSelectedCharacter->m_fMapLocationY - m_pDNDMap->m_fParentMapOriginY) / m_pDNDMap->m_fScaleY * m_fViewScale;
		}

		m_nCornerX = (int)(nSizeX - fX);
		m_nCornerY = (int)(nSizeY - fY);

		m_szHint.Format("Centered on character: %s", m_pSelectedCharacter->m_szCharacterName);

		InvalidateRect(NULL);
	}
	else
	{
		m_szHint = _T("No character selected !");
	}

	InvalidateRect(NULL);
}


void cDMMapViewDialog::OnPartyCharacterButton() 
{
	if(m_pSelectedCharacter != NULL)
	{
		m_pSelectedCharacter->m_fMapLocationX = 0.0f;
		m_pSelectedCharacter->m_fMapLocationY = 0.0f;

		m_pSelectedCharacter->m_fLocalLocationX = 0.0f;
		m_pSelectedCharacter->m_fLocalLocationY = 0.0f;
	}
	else
	{
		m_szHint = _T("No character selected !");
	}

	InvalidateRect(NULL);
}


void cDMMapViewDialog::OnBnClickedSetCharacterIconButton()
{
	if (m_pSelectedCharacter == NULL)
	{
		return;
	}

	PDNDNPCVIEWDLG pNPCDlg = NULL;
	m_pApp->m_NPCViewMap.Lookup((WORD)m_pSelectedCharacter->m_dwCharacterID, pNPCDlg);

	g_MVBP_szFilename[0] = 0;

	ZeroMemory(&g_ofn, sizeof(OPENFILENAME));
	g_ofn.lStructSize = sizeof(OPENFILENAME);
	g_ofn.nMaxFile = MAX_PATH;
	g_ofn.nMaxFileTitle = MAX_PATH;
	g_ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	g_ofn.hwndOwner = m_hWnd;
	g_ofn.hInstance = m_pApp->m_hInstance;
	g_ofn.lpstrFile = g_MVBP_szFilename;
	g_ofn.lpstrInitialDir = "C:/";

	if (pNPCDlg == NULL)
	{
		g_ofn.lpstrTitle = "Load character icon bitmap file";
	}
	else
	{
		g_ofn.lpstrTitle = "Load NPC icon bitmap file";
	}

	//g_ofn.lpstrFilter = "Graphics Files (*.bmp)\0*.bmp\0(*.gif)\0*.gif\0(*.jpg)\0*.jpg\0All Files (*.*)\0*.*\0\0";
	g_ofn.lpstrFilter = "All Files (*.*)\0*.*\0\0";
	g_ofn.lpstrDefExt = "bmp";

	// Get action template file name
	if (GetOpenFileName(&g_ofn))
	{
		if (pNPCDlg == NULL)
		{
			strcpy(m_pSelectedCharacter->m_szIconPath, g_MVBP_szFilename);
			m_pSelectedCharacter->MarkChanged();
		}
		else if (pNPCDlg->m_pNPC != NULL)
		{
			PDNDMONSTERMANUALENTRY pMonster = NULL;
			m_pApp->m_MonsterManualIndexedMap.Lookup(pNPCDlg->m_pNPC->m_nMonsterIndex, pMonster);

			if (pMonster != NULL)
			{
				CString szSourcePath = g_MVBP_szFilename;
				szSourcePath.MakeLower();

				CDMNPCPortraitSelectDialog *pDlg = new CDMNPCPortraitSelectDialog("Icon", szSourcePath, "data\\monster_pictures\\icons\\", pMonster->m_szMMName, pNPCDlg->m_pNPC->m_szCharacterName);
				pDlg->DoModal();
				delete pDlg;
			}

		}

		InvalidateRect(NULL);
	}


}


void cDMMapViewDialog::OnBnClickedPlaceCacheButton()
{
	if(m_pSelectedCache != NULL)
	{
		m_MapMode = DND_MAP_MODE_PLACE_CACHE;
		m_szHint = _T("Double click on the cache's new location");
	}
	else
	{
		m_szHint = _T("No cache selected !");
	}

	UpdateData(FALSE);
}

void cDMMapViewDialog::OnBnClickedPlaceOnCacheButton()
{
	if(m_pSelectedCache != NULL)
	{
		CRect Rect;
		GetWindowRect (Rect);

		int nSizeX = Rect.right - Rect.left;
		int nSizeY = Rect.bottom - Rect.top;

		nSizeX /= 2;
		nSizeY /= 2;

		float fX = 0.0f;
		float fY = 0.0f;

		if(m_pDNDMap->m_bMapScaleFeet)
		{
			//feet to pixels
			fX =  (m_pSelectedCache->m_fLocalLocationX) / m_pDNDMap->m_fScaleX * m_fViewScale;
			fY =  (m_pSelectedCache->m_fLocalLocationY) / m_pDNDMap->m_fScaleY * m_fViewScale;
		}
		else
		{
			//miles to pixels
			fX =  (m_pSelectedCache->m_fMapLocationX - m_pDNDMap->m_fParentMapOriginX) / m_pDNDMap->m_fScaleX * m_fViewScale;
			fY =  (m_pSelectedCache->m_fMapLocationY - m_pDNDMap->m_fParentMapOriginY) / m_pDNDMap->m_fScaleY * m_fViewScale;
		}

		m_nCornerX = (int)(nSizeX - fX);
		m_nCornerY = (int)(nSizeY - fY);

		m_szHint.Format("Centered on cache: %s", m_pSelectedCache->m_szCacheDesc);

		InvalidateRect(NULL);
	}
	else
	{
		m_szHint = _T("No cache selected !");
	}

}

cDNDNonPlayerCharacter *cDMMapViewDialog::AddCache(char *szCacheName, int nCachePosX, int nCachePosY, BOOL bPickTab)
{
	cDNDNonPlayerCharacter *pNPC = NULL;

	if(m_pTreasureCaches == NULL)
	{
		m_pTreasureCaches = new cDNDTreasureCache();
	}

	int nCacheSlot = -1;
	for(int i = 0; i < MAX_MAP_CACHES; ++i)
	{
		if(m_pTreasureCaches->m_Caches[i].m_dwCacheID == 0L)
		{
			nCacheSlot = i;
			break;
		}
	}

	if(nCacheSlot >= 0)
	{
		if(m_pDNDMap->m_dwMapID == 0)
		{
			m_pDNDMap->m_dwMapID = GetUniqueID();
		}

		m_pTreasureCaches->m_Caches[nCacheSlot].m_dwParentMapID = m_pDNDMap->m_dwMapID;
		m_pTreasureCaches->m_Caches[nCacheSlot].m_dwCacheID = GetUniqueID();
	
		pNPC = new cDNDNonPlayerCharacter();

		memset(pNPC, 0, sizeof(cDNDNonPlayerCharacter));

		pNPC->m_dwCharacterID = m_pTreasureCaches->m_Caches[nCacheSlot].m_dwCacheID;
		pNPC->m_bIsNPC = TRUE;
		pNPC->m_bIsCache = TRUE;

		pNPC->m_fLocalLocationX = (float)nCachePosX;
		pNPC->m_fLocalLocationY = (float)nCachePosY;

		pNPC->m_fMapLocationX = (float)nCachePosX;
		pNPC->m_fMapLocationY = (float)nCachePosY;

		strcpy(pNPC->m_szCharacterName, szCacheName);

		cDMBaseNPCViewDialog *pDlg = new cDMBaseNPCViewDialog(m_pMainDialog, pNPC, &m_pTreasureCaches->m_Caches[nCacheSlot], &m_pMainDialog->m_cMainTab);

		pDlg->m_dwMapID = m_pDNDMap->m_dwMapID;

		pDlg->Refresh();

		m_pMainDialog->AddTab(pDlg, DND_TAB_TYPE_CACHE, bPickTab);

		m_pApp->m_NPCViewMap.SetAt((WORD)m_pTreasureCaches->m_Caches[nCacheSlot].m_dwCacheID, pDlg);

		m_pTreasureCaches->CountCaches();

		Reset();

		m_pDNDMap->MarkChanged();

	}

	return pNPC;
}

void cDMMapViewDialog::OnBnClickedAddCacheButton()
{
	AddCache("New Cache", 0, 0, TRUE);
}


BOOL cDMMapViewDialog::FindAndSelectCacheTab(DWORD dwCacheID)
{
	int nSkippedTabs = 0;

	for (int i = 0; i < m_pApp->m_TabArray.GetSize(); ++i)
	{
		cDNDDisplayTab *pTab = m_pApp->m_TabArray[i];

		if(pTab != NULL)
		{
			if(pTab->m_bHide)
			{
				++nSkippedTabs;
			}
			else if(pTab->m_nTabType == DND_TAB_TYPE_CACHE)
			{
				cDMBaseNPCViewDialog *pNPCDlg = (cDMBaseNPCViewDialog *)pTab->m_pWindow;
				
				if(pNPCDlg->m_pNPC->m_dwCharacterID == dwCacheID)
				{
					CDMHelperDlg *pMainDlg = (CDMHelperDlg *)m_pApp->m_pMainWindow;

					pMainDlg->m_cMainTab.SetCurSel(i-nSkippedTabs);
					pMainDlg->PickTab();

					return TRUE;
				}

			}
			else
			{
				TRACE("TAB TYPE %d is %d\n", i, pTab->m_nTabType);
			}
		}
	}

	return FALSE;
}

void cDMMapViewDialog::OnBnClickedDeleteCacheButton()
{
	if(m_pSelectedCache == NULL)
	{
		m_szHint.Format("No cache selected !");
		UpdateData(FALSE);
		return;
	}

	if(AfxMessageBox("Permanently delete selected cache ?", MB_YESNO) != IDYES)
	{
		return;
	}

	for(int i = 0; i < MAX_MAP_CACHES; ++i)
	{
		if(m_pTreasureCaches->m_Caches[i].m_dwCacheID != 0)
		{
			if(&m_pTreasureCaches->m_Caches[i] == m_pSelectedCache)
			{
				CDMHelperDlg *pMainDlg = (CDMHelperDlg *)m_pApp->m_pMainWindow;

				if(pMainDlg->FindAndDeleteTab(DND_TAB_TYPE_CACHE, m_pTreasureCaches->m_Caches[i].m_dwCacheID))
				{
					memset(&m_pTreasureCaches->m_Caches[i], 0, sizeof(cDNDCacheInventory));

					ValidateCaches();

					FindAndSelectMapTab(m_pDNDMap->m_dwMapID);

					m_pApp->m_NPCViewMap.SetAt((WORD)m_pTreasureCaches->m_Caches[i].m_dwCacheID, NULL);
					m_pApp->m_NPCViewMap.RemoveKey((WORD)m_pTreasureCaches->m_Caches[i].m_dwCacheID);

					Reset();

					m_pDNDMap->MarkChanged();
				}

				return;
			}
		}
	}
}

void cDMMapViewDialog::ValidateCaches()
{
	memset(m_CacheBuffer, 0, sizeof(cDNDCacheInventory) * MAX_MAP_CACHES);

	int nCount = 0;
	for(int i = 0; i < MAX_MAP_CACHES; ++i)
	{
		if(m_pTreasureCaches->m_Caches[i].m_dwCacheID != 0)
		{
			memcpy(&m_CacheBuffer[nCount], &m_pTreasureCaches->m_Caches[i], sizeof(cDNDCacheInventory)); 
			++nCount;
		}
	}

	memcpy(&m_pTreasureCaches->m_Caches, &m_CacheBuffer, sizeof(cDNDCacheInventory) * MAX_MAP_CACHES); 

	m_pTreasureCaches->CountCaches();

}

void cDMMapViewDialog::DeleteCaches()
{
	if(m_pTreasureCaches == NULL)
		return;

	CDMHelperDlg *pMainDlg = (CDMHelperDlg *)m_pApp->m_pMainWindow;

	for(int i = 0; i < MAX_MAP_CACHES; ++i)
	{
		if(m_pTreasureCaches->m_Caches[i].m_dwCacheID != 0)
		{
			if(pMainDlg->FindAndDeleteTab(DND_TAB_TYPE_CACHE, m_pTreasureCaches->m_Caches[i].m_dwCacheID, FALSE))
			{
				memset(&m_pTreasureCaches->m_Caches[i], 0, sizeof(cDNDCacheInventory));

				m_pApp->m_NPCViewMap.SetAt((WORD)m_pTreasureCaches->m_Caches[i].m_dwCacheID, NULL);
				m_pApp->m_NPCViewMap.RemoveKey((WORD)m_pTreasureCaches->m_Caches[i].m_dwCacheID);
			}
			
		}
	}

	pMainDlg->SortTabs();

	Reset();

	m_pDNDMap->MarkChanged();

	ValidateCaches();
}

void cDMMapViewDialog::OnCbnSelchangeCacheCombo()
{
	int nCursor = m_cCacheCombo.GetCurSel();

	if(nCursor >= 0)
	{
		m_pSelectedCache = (cDNDCacheInventory *)m_cCacheCombo.GetItemData(nCursor);
		m_szHint.Format("Selected Cache: %s", m_pSelectedCache->m_szCacheDesc);
	}

	UpdateData(FALSE);
}


void cDMMapViewDialog::OnEnChangeCacheDetailsEdit()
{
	UpdateData(TRUE);

	if(m_pHoverCache != NULL)
	{
		strcpy(m_pHoverCache->m_szCacheDetails, m_szCacheDetails.Left(511));

		for (POSITION pos = m_pApp->m_NPCViewMap.GetStartPosition(); pos != NULL; )
		{
			WORD wID;
			PDNDNPCVIEWDLG pNPCDlg = NULL;
			m_pApp->m_NPCViewMap.GetNextAssoc(pos,wID,pNPCDlg);

			if(pNPCDlg != NULL && pNPCDlg->m_pNPC != NULL && pNPCDlg->m_pNPC->m_dwCharacterID == m_pHoverCache->m_dwCacheID)
			{
				strcpy(pNPCDlg->m_pNPC->m_szDetails, m_szCacheDetails.Left(511));
				
				pNPCDlg->m_pNPC->MarkChanged();

				m_pDNDMap->MarkChanged();
			}
		}
	}
}

void cDMMapViewDialog::OnStnClickedBdrop()
{

}

void cDMMapViewDialog::OnBnClickedPartiesCheck()
{
	UpdateData(TRUE);
	InvalidateRect(NULL);
}

void cDMMapViewDialog::OnBnClickedRoutesCheck()
{
	UpdateData(TRUE);
	InvalidateRect(NULL);
}

void cDMMapViewDialog::OnBnClickedChildMapsCheck()
{
	UpdateData(TRUE);
	InvalidateRect(NULL);
}

void cDMMapViewDialog::OnBnClickedCachesCheck()
{
	UpdateData(TRUE);
	InvalidateRect(NULL);
}

void cDMMapViewDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(m_MapMode != DND_MAP_MODE_UNDEF)
	{
		m_MapMode = DND_MAP_MODE_UNDEF;
		InvalidateRect(NULL);
	}

	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void cDMMapViewDialog::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	if(m_bMouseDrag)
	{
		m_bMouseDrag = FALSE;
		InvalidateRect(NULL);
	}

	CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}


/*
BOOL cDMMapViewDialog::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	//{0.125f, 0.25f, 0.50f, 0.75f, 1.0f, 2.0f, 4.0, 8.0f, 16.0f }; m_fViewScale

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}
*/

#ifdef _DEBUG
#define MAX_MSCALE 128.0f
#else
#define MAX_MSCALE 16.0f
#endif

BOOL cDMMapViewDialog::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	float fOldScale = m_fViewScale;

	if(zDelta < 0)
	{
		m_fViewScale -= 0.1f;
		if(m_fViewScale < 0.125f)
		{
			m_fViewScale = 0.125f;

			if (fOldScale == m_fViewScale)
			{
				return CDialog::OnMouseWheel(nFlags, zDelta, pt);
			}
		}
	}
	else
	{
		m_fViewScale += 0.2f;
		if (m_fViewScale > MAX_MSCALE)
		{
			m_fViewScale = MAX_MSCALE;
		}
	}

	CRect Rect;
	GetWindowRect (Rect);

	int nSizeX = Rect.right - Rect.left;
	int nSizeY = Rect.bottom - Rect.top;

	nSizeX /= 2;
	nSizeY /= 2;

	float fX = 0.0f;
	float fY = 0.0f;

	//BOOKMARK
	if(m_pDNDMap->m_bMapScaleFeet)
	{
		//spot we are looking at before the scale changed
		float fCX = (-m_nCornerX + nSizeX) * m_pDNDMap->m_fScaleX / fOldScale;
		float fCY = (-m_nCornerY + nSizeY) * m_pDNDMap->m_fScaleY / fOldScale;

		//feet to pixels
		fX =  fCX / m_pDNDMap->m_fScaleX * m_fViewScale;
		fY =  fCY / m_pDNDMap->m_fScaleY * m_fViewScale;
	}
	else
	{
		//spot we are looking at before the scale changed
		//float fCX = (-m_nCornerX - m_pDNDMap->m_fParentMapOriginX + nSizeX) * m_pDNDMap->m_fScaleX / fOldScale;
		//float fCY = (-m_nCornerY - m_pDNDMap->m_fParentMapOriginY + nSizeY) * m_pDNDMap->m_fScaleY / fOldScale;

		//pixels to miles
		float fCX = (-m_nCornerX + nSizeX) * m_pDNDMap->m_fScaleX / fOldScale;
		fCX +=  m_pDNDMap->m_fParentMapOriginX;

		//pixels to miles
		float fCY = (-m_nCornerY + nSizeY) * m_pDNDMap->m_fScaleY / fOldScale;
		fCY +=  m_pDNDMap->m_fParentMapOriginY;

		//miles to pixels
		fX =  (fCX - m_pDNDMap->m_fParentMapOriginX) / m_pDNDMap->m_fScaleX * m_fViewScale;
		fY =  (fCY - m_pDNDMap->m_fParentMapOriginY) / m_pDNDMap->m_fScaleY * m_fViewScale;
	}

	m_nCornerX = (int)(nSizeX - fX);
	m_nCornerY = (int)(nSizeY - fY);


	InvalidateRect(NULL);
	
	//return TRUE;
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void cDMMapViewDialog::OnBnClickedHoursCheck()
{
	UpdateData(TRUE);
	InvalidateRect(NULL);
}

void cDMMapViewDialog::OnBnClickedWeatherCheck()
{
	UpdateData(TRUE);
	InvalidateRect(NULL);
}


void cDMMapViewDialog::GenerateWeather(CString *pszWeatherData)
{
	if(m_pSelectedParty == NULL)
	{
		return;
	}

	PDNDPARTYVIEWDLG pPartyDlg = ValidateSelectedParty();

	if(pPartyDlg == NULL)
	{
		return;
	}

	if(m_pApp->m_pSelectedCalendar == NULL)
	{
		return;
	}

	if(m_pDNDMap->m_fDegreeLatitudeMiles <= 0.0f)
	{
		*pszWeatherData = _T("INVALID WEATHER COORDINATE SETUP");
		return;
	}

	float fLatitude40NLine = m_pDNDMap->m_fLatitude40NLine;
	float fLatitudeMiles = m_pDNDMap->m_fDegreeLatitudeMiles;

	float fEquatorLine =  fLatitude40NLine + 40.0f * fLatitudeMiles;
	float fLatitude40SLine = fEquatorLine + 40.0f * fLatitudeMiles;

	char szCloudDesc[4][16] = 
	{
		"clear\0",
		"partly cloudy\0",
		"cloudy\0",
	};

	char szPrecipitationDesc[19][32] =
	{
		"no precipitation\0",
		"heavy blizzard\0",
		"blizzard\0",
		"heavy snowstorm\0",
		"snowstorm\0",
		"sleetstorm\0",
		"hailstorm\0",
		"heavy fog\0",
		"light fog\0",
		"mist\0",
		"drizzle\0",
		"light rainstrom\0",
		"heavy rainstorm\0",
		"thunderstorm\0",
		"tropical storm\0",
		"monsoon\0",
		"gale\0",
		"hurricane\0",
		"special\0",
	};

	CString szRetVal = _T("");

	//already calculated, so return it
	for(int i=0; i < MAX_WEATHER_FORECASTS; ++i)
	{
		if(m_pSelectedParty->m_WeatherForecast[i].m_nMonth == m_pSelectedParty->m_nMonth && m_pSelectedParty->m_WeatherForecast[i].m_nDay == m_pSelectedParty->m_nDayofMonth)
		{
			float fWeatherDistance = GetDistance(m_pSelectedParty->m_WeatherForecast[i].m_fMapX, m_pSelectedParty->m_WeatherForecast[i].m_fMapY, m_pSelectedParty->m_fPartyLocationX, m_pSelectedParty->m_fPartyLocationY);
			// we don't need to scale here determined experimentally
				
			if(fWeatherDistance < 50.0f) // weather should change if we've traveled more than 50 miles
			{
				float fTempDiff = (float)m_pSelectedParty->m_WeatherForecast[i].m_nHighTemperature - (float)m_pSelectedParty->m_WeatherForecast[i].m_nLowTemperature;
				float fDegreeHour = fTempDiff / 12.0f;

				float fCurrentHour = 0.0f;
				float fCurrentTemp = 0.0f;

				if(m_pSelectedParty->m_nHour >= 0 && m_pSelectedParty->m_nHour <= 4) // falling temp from 0000 to 0400
				{
					fCurrentHour = (float)(m_pSelectedParty->m_nHour + 24 - 16);
					fCurrentTemp =  (float)m_pSelectedParty->m_WeatherForecast[i].m_nHighTemperature - (fCurrentHour * fDegreeHour);
				}
				else if(m_pSelectedParty->m_nHour >= 16 && m_pSelectedParty->m_nHour <= 24) // falling temp from 1600 to 2400
				{
					fCurrentHour = m_pSelectedParty->m_nHour - 16.0f;
					fCurrentTemp =  (float)m_pSelectedParty->m_WeatherForecast[i].m_nHighTemperature - (fCurrentHour * fDegreeHour);
				}
				else if(m_pSelectedParty->m_nHour >= 4 && m_pSelectedParty->m_nHour <= 16) // rising temp from 0400 to 1600
				{
					fCurrentHour = m_pSelectedParty->m_nHour - 4.0f;
					fCurrentTemp = (fCurrentHour * fDegreeHour) + (float)m_pSelectedParty->m_WeatherForecast[i].m_nLowTemperature;
				}

				int nCurrentTemp = (int)(fCurrentTemp + 0.5f);

				szRetVal.Format("%d deg (%dhi %dlo), %s, %s", nCurrentTemp, m_pSelectedParty->m_WeatherForecast[i].m_nHighTemperature, m_pSelectedParty->m_WeatherForecast[i].m_nLowTemperature, szCloudDesc[m_pSelectedParty->m_WeatherForecast[i].m_nCloudsType], szPrecipitationDesc[m_pSelectedParty->m_WeatherForecast[i].m_nPrecipitationType]);
				*pszWeatherData = szRetVal;

				return;
			}
			else
			{
				break;
			}
		}
	}

	int nWeatherBaseline[7][12] = 
	{
		//	0			1			2			3			4			5			6			7			8			9			10			11
		//	0			1			2			4			5			6			8			9			10			12			13			14
		//	Fireseek	Readying	Coldeven ^	Planting	Flocktime	Wealsun	^	Reaping		Goodmonth	Harvester ^	Patchwall	Ready'reat	Sunsebb  ^
			32,			34,			42,			52,			63,			71,			77,			75,			68,			57,			46,			33,		// Base temp
			100,		64,			84,			106,		106,		88,			64,			46,			86,			105,		106,		85,		// Daily high adj
			200,		104,		104,		84,			106,		66,			66,			66,			86,			105,		104,		200,	// Daily low adj																					// Daily low adj
																																						// Sky Conditions
			23,			25,			27,			20,			20,			20,			22,			25,			33,			35,			20,			25,		// Clear
			50,			50,			54,			55,			53,			60,			62,			60,			54,			60,			50,			50,		// Partly cloudy
			100,		100,		100,		100,		100,		100,		100,		100,		100,		100,		100,		100,	// Cloudy
			46,			40,			44,			42,			42,			36,			33,			33,			33,			36,			40,			43		// Precip chance %
	};

	int nPrecipitationTable[18][5] =
	{
	//  Roll		Min Temp	Max Temp		Cont %		Rainbow		
		  1002,		-99,		10,				5,			0,			// heavy blizzard
		  3005,		-99,		20,				10,			0,			// blizzard
		  6010,		-99,		25,				20,			0,			// heavy snowstorm
		 11020,		-99,		35,				25,			1,			// snowstorm
		 21025,		-99,		35,				20,			0,			// sleetstorm
		 26027,		-99,		65,				10,			0,			// hailstorm
		 28030,		20,			60,				25,			1,			// heavy fog
		 31038,		30,			70,				30,			3,			// light fog
		 39040,		30,			999,			15,			10,			// mist
		 41045,		25,			999,			20,			5,			// drizzle
		 46060,		25,			999,			45,			15,			// light rainstrom
		 61070,		25,			999,			30,			20,			// heavy rainstorm
		 71084,		30,			999,			15,			20,			// thunderstorm
		 85089,		40,			999,			20,			10,			// tropical storm
		 90094,		40,			999,			30,			5,			// monsoon
		 95097,		55,			999,			15,			10,			// gale
		 98099,		40,			999,			20,			5,			// hurricane
		100100,		55,			999,			1,			0,			// special
	};

	

	int nDayIndexer[MAX_CALENDAR_MONTHS_IN_YEAR];
	memset(nDayIndexer, 0, MAX_CALENDAR_MONTHS_IN_YEAR * sizeof(int));

	int nCounter = 0;
	for(int i = 0; i < min(m_pApp->m_pSelectedCalendar->m_nMonthsInYear, MAX_CALENDAR_MONTHS_IN_YEAR); ++i)
	{
		if(m_pApp->m_pSelectedCalendar->m_nDaysInMonth[i] < 4*m_pApp->m_pSelectedCalendar->m_nDaysInWeek)
		{
			nDayIndexer[i] = nCounter;
		}
		else
		{
			nDayIndexer[i] = nCounter;
			++nCounter;
		}
	}

	int nDay = m_pSelectedParty->m_nDayofMonth;
	int nMonth = m_pSelectedParty->m_nMonth;

	//////////////////////////////////////////
	// latitude adjustment

	float fLatitude40Line = fLatitude40NLine;

	if(m_pSelectedParty->m_fPartyLocationY > fEquatorLine)
	{
		fLatitude40Line = fLatitude40SLine;
	}

	float fLatitide = (m_pSelectedParty->m_fPartyLocationY - fLatitude40Line) * m_pDNDMap->m_fScaleY;
	float fLatitudeTemp = fLatitide / fLatitudeMiles * 2.0f;


	for(int i = 0; i < MAX_WEATHER_FORECASTS-1; ++i)
	{
		int nWeatherMonth = abs(nMonth % m_pApp->m_pSelectedCalendar->m_nMonthsInYear);

		m_pSelectedParty->m_WeatherForecast[i].m_nMonth = nWeatherMonth;

		nWeatherMonth = abs(nDayIndexer[nWeatherMonth] % 12);

		m_pSelectedParty->m_WeatherForecast[i].m_nDay = nDay;


		m_pSelectedParty->m_WeatherForecast[i].m_fMapX = m_pSelectedParty->m_fPartyLocationX;
		m_pSelectedParty->m_WeatherForecast[i].m_fMapY = m_pSelectedParty->m_fPartyLocationY;

		//////////////////////////////////////////
		// high temp

		m_pSelectedParty->m_WeatherForecast[i].m_nHighTemperature = nWeatherBaseline[0][nWeatherMonth];

		m_pSelectedParty->m_WeatherForecast[i].m_nHighTemperature += (int)(fLatitudeTemp + 0.5f);

		int nDieRoll = nWeatherBaseline[1][nWeatherMonth] / 10;
		int nDiePlus = nWeatherBaseline[1][nWeatherMonth] % 10;

		int nRoll =  (rand() % nDieRoll) + nDiePlus;

		m_pSelectedParty->m_WeatherForecast[i].m_nHighTemperature += nRoll;

		//////////////////////////////////////////
		// low temp

		m_pSelectedParty->m_WeatherForecast[i].m_nLowTemperature = nWeatherBaseline[0][nWeatherMonth];

		m_pSelectedParty->m_WeatherForecast[i].m_nLowTemperature += (int)(fLatitudeTemp + 0.5f);

		nDieRoll = nWeatherBaseline[2][nWeatherMonth] / 10;
		nDiePlus = nWeatherBaseline[2][nWeatherMonth] % 10;

		nRoll =  (rand() % nDieRoll) + nDiePlus;

		m_pSelectedParty->m_WeatherForecast[i].m_nLowTemperature -= nRoll;

		//////////////////////////////////////////
		// clouds

		nRoll = RollD100();

		if(nRoll <= nWeatherBaseline[3][nWeatherMonth])
			m_pSelectedParty->m_WeatherForecast[i].m_nCloudsType = 0;
		else if(nRoll <= nWeatherBaseline[4][nWeatherMonth])
			m_pSelectedParty->m_WeatherForecast[i].m_nCloudsType = 1;
		else if(nRoll <= nWeatherBaseline[5][nWeatherMonth])
			m_pSelectedParty->m_WeatherForecast[i].m_nCloudsType = 2;
		else
			m_pSelectedParty->m_WeatherForecast[i].m_nCloudsType = 0;

		//////////////////////////////////////////
		// precipitation

		m_pSelectedParty->m_WeatherForecast[i].m_nPrecipitationType = 0;

		nRoll = RollD100();

		if(nRoll < nWeatherBaseline[6][nWeatherMonth]) // yes we have precip
		{
			BOOL bFoundPrecip = FALSE;

			for(int k = 0; k < 3 && bFoundPrecip == FALSE; ++k)
			{
				nRoll = RollD100();

				for(int j = 0; j < 18; ++j)
				{
					int nLow =  nPrecipitationTable[j][0] / 1000;
					int nHigh = nPrecipitationTable[j][0] % 100;

					if(nRoll >= nLow && nRoll <= nHigh)
					{
						if(m_pSelectedParty->m_WeatherForecast[i].m_nLowTemperature >= nPrecipitationTable[j][1]) //min temp
						{
							if(m_pSelectedParty->m_WeatherForecast[i].m_nHighTemperature <= nPrecipitationTable[j][2]) // max temp
							{
								TRACE("GOT %s at %d-%d degrees\n", szPrecipitationDesc[j], m_pSelectedParty->m_WeatherForecast[i].m_nLowTemperature, m_pSelectedParty->m_WeatherForecast[i].m_nHighTemperature);

								m_pSelectedParty->m_WeatherForecast[i].m_nPrecipitationType = j;

								bFoundPrecip = TRUE;
								break;
							}
						}
					}
				}
			}
		}

		//////////////////////////////////////////

		++nDay;

		if(nDay > m_pApp->m_pSelectedCalendar->m_nDaysInMonth[nMonth])
		{
			nDay = 0;

			++nMonth;

			if(nMonth > m_pApp->m_pSelectedCalendar->m_nMonthsInYear)
			{
				nMonth = 0;
			}
		}
	}

	m_pSelectedParty->m_WeatherForecast[MAX_WEATHER_FORECASTS-1].m_fMapX = 0.0f;
	m_pSelectedParty->m_WeatherForecast[MAX_WEATHER_FORECASTS-1].m_fMapY = 0.0f;

	m_pSelectedParty->MarkChanged();

	GenerateWeather(pszWeatherData); // call again with valid data

}


void cDMMapViewDialog::GenerateRandomDungeon(int nSizeX, int nSizeY, BOOL bCreateCaches, int nMonsterLevel)
{
	CWaitCursor wait;

	CString szTemp;

	m_pApp->ResetDataPicker(&m_MonsterNameIndexer);

	memset(m_pDNDMap->m_Tiles, 0, MAX_MAP_TILES*sizeof(cDNDMapTile));

	nSizeX = min(nSizeX, 2048/32);
	nSizeY = min(nSizeY, 2048/32);

	if(m_pDNDMap->m_dwMapID == 0L)
	{
		m_pDNDMap->m_dwMapID = GetUniqueID();
		m_pApp->m_MapViewMap.SetAt((WORD)m_pDNDMap->m_dwMapID, this);
	}

	CDungeonGen *pDungeon = new CDungeonGen(nSizeX, nSizeY);

	int nNumRooms = 0;
	int nOccupiedRooms = 0;

	for(int x = 0; x < nSizeX; ++x)
	{
		for(int y = 0; y < nSizeY; ++y)
		{
			int nTileIndex = pDungeon->m_nTiles[x][y] % 1000;

			int nData = pDungeon->m_nTiles[x][y] / 1000;

			int nRoomNumber = nData % 1000;
			int nTileFlag = nData / 1000;

			switch(nTileIndex)
			{
				case DG_FLOOR:
				{
					AddTile(x*31, y*31, 10, nRoomNumber, nTileFlag);

					if(nTileFlag == 1)
					{
						++nNumRooms;
					}

					break;
				}
				case DG_CORRIDOR:
				{
					AddTile(x*31, y*31, 10, nRoomNumber, nTileFlag);
					break;
				}
				case DG_FOUNTAIN:
				{
					AddTile(x*31, y*31, 15, nRoomNumber, nTileFlag);
					break;
				}
				case DG_TRAPDOOR:
				{
					AddTile(x*31, y*31, 16, nRoomNumber, nTileFlag);
					break;
				}
				case DG_DOOR:
				{
					if( pDungeon->IsOpenTile(x-1, y) || pDungeon->IsOpenTile(x+1, y) ) // east/west door
					{
						AddTile(x*31, y*31, 14, nRoomNumber, nTileFlag);
					}
					else //north/south door
					{
						AddTile(x*31, y*31, 11, nRoomNumber, nTileFlag); 
					}

					break;
				}
			}
		}
	}

	nNumRooms = min(nNumRooms, MAX_RAND_ROOMS);

	//add room caches in order
	if(nNumRooms && bCreateCaches)
	{
		RECT rRoomWalls[MAX_RAND_ROOMS];

		for(int i = 0;  i < MAX_RAND_ROOMS; ++i)
		{
			rRoomWalls[i].left = 30000;
			rRoomWalls[i].top = 30000;

			rRoomWalls[i].right = -30000;
			rRoomWalls[i].bottom = -30000;
		}

		//get room coordinates
		for(int nRoom = 0; nRoom < nNumRooms; ++nRoom)
		{
			for(int i = 0; i < MAX_MAP_TILES; ++i)
			{
				if(m_pDNDMap->m_Tiles[i].m_nTileFrame == 0)
				{
					break;
				}

				if(m_pDNDMap->m_Tiles[i].m_nRoomNumber == nRoom+1)
				{
					int nLeft = m_pDNDMap->m_Tiles[i].m_nTileX + 10;
					int nTop = m_pDNDMap->m_Tiles[i].m_nTileY + 10;

					if(nLeft < rRoomWalls[nRoom].left)
						rRoomWalls[nRoom].left = nLeft;

					if(m_pDNDMap->m_Tiles[i].m_nTileX > rRoomWalls[nRoom].right)
						rRoomWalls[nRoom].right = m_pDNDMap->m_Tiles[i].m_nTileX;

					if(nTop < rRoomWalls[nRoom].top)
						rRoomWalls[nRoom].top = nTop;

					if(m_pDNDMap->m_Tiles[i].m_nTileY > rRoomWalls[nRoom].bottom)
						rRoomWalls[nRoom].bottom = m_pDNDMap->m_Tiles[i].m_nTileY;
				}
			}
		}

		cDNDParty *pParty = new cDNDParty();
		pParty->m_dwPartyID = GetUniqueID();

		pParty->m_dwPartyMapID = m_pDNDMap->m_dwMapID;

		m_pDNDMap->m_dwRandomDungeonPartyID = pParty->m_dwPartyID;

		DWORD dwSubPartyID = 0;
		cDNDSubParty *pSubParty = NULL;
		DMPartyDialog *pMainPartyDialog = NULL;
		PDNDMONSTERMANUALENTRY pMonsterManualEntry = NULL;
		cDNDNonPlayerCharacter *pCacheNPC = NULL;
		int nCacheCount = 0;

		if(nMonsterLevel >= 0)
		{
			if(nMonsterLevel == 0)
			{
				nMonsterLevel = (rand()%16) + 1;
			}

			dwSubPartyID = GetUniqueID();

			RECT rRoom = rRoomWalls[0];
			rRoom.left = (int)(rRoom.left * m_pDNDMap->m_fScaleX);
			rRoom.top = (int)(rRoom.top * m_pDNDMap->m_fScaleX);
			rRoom.right = (int)(rRoom.right * m_pDNDMap->m_fScaleX);
			rRoom.bottom = (int)(rRoom.bottom * m_pDNDMap->m_fScaleX);

			m_pApp->GenerateRandomParty(_T("DUNGEON"), NULL, pParty, dwSubPartyID, 0, nMonsterLevel, 0, &rRoom, &m_MonsterNameIndexer, &pMonsterManualEntry);

			sprintf(pParty->m_szPartyName, "%s Monsters", m_pDNDMap->m_szMapName);

			pMainPartyDialog = m_pApp->m_pMainWindow->GenerateRandomEncounterTabs(pParty, NULL, NULL, FALSE);
			pMainPartyDialog->m_szPartyMapName = m_pDNDMap->m_szMapName;

			pSubParty = pMainPartyDialog->ExternalSubPartyCommand("Room 1", dwSubPartyID);

			if(pSubParty != NULL)
			{
				pSubParty->m_nSubPartyType = DND_SUBPARTY_TYPE_DUNGEON;
			}

			++nOccupiedRooms;
		}

		
		for(int nRoom = 0; nRoom < nNumRooms; ++nRoom)
		{
			for(int i = 0; i < MAX_MAP_TILES; ++i)
			{
				if(m_pDNDMap->m_Tiles[i].m_nTileFrame == 0)
				{
					break;
				}

				if(m_pDNDMap->m_Tiles[i].m_nRoomFlag == 1 && m_pDNDMap->m_Tiles[i].m_nRoomNumber == nRoom+1)
				{
					if(m_pDNDMap->m_Tiles[i].m_nRoomNumber == 1 || (nOccupiedRooms < MAX_RAND_OCCUPIED_ROOMS && RollD100() <= m_nRandomDungeonOccupancy))
					{
						szTemp.Format("Room %d", nOccupiedRooms);

						int nCX = (int)((m_pDNDMap->m_Tiles[i].m_nTileX + 15) * m_pDNDMap->m_fScaleX);
						int nCY = (int)((m_pDNDMap->m_Tiles[i].m_nTileY + 15) * m_pDNDMap->m_fScaleX);

						pCacheNPC = AddCache(szTemp.GetBuffer(0), nCX, nCY, FALSE);

						if(nRoom > 0)
						{
							if(nMonsterLevel >= 0)
							{
								dwSubPartyID = GetUniqueID();

								RECT rRoom = rRoomWalls[nRoom];
								rRoom.left = (int)(rRoom.left * m_pDNDMap->m_fScaleX);
								rRoom.top = (int)(rRoom.top * m_pDNDMap->m_fScaleX);
								rRoom.right = (int)(rRoom.right * m_pDNDMap->m_fScaleX);
								rRoom.bottom = (int)(rRoom.bottom * m_pDNDMap->m_fScaleX);

								m_pApp->GenerateRandomParty(_T("DUNGEON"), NULL, pParty, dwSubPartyID, 0, nMonsterLevel, 0, &rRoom, &m_MonsterNameIndexer, &pMonsterManualEntry);
								pSubParty = pMainPartyDialog->ExternalSubPartyCommand(szTemp.GetBuffer(0), dwSubPartyID);

								if(pSubParty != NULL)
								{
									pSubParty->m_nSubPartyType = DND_SUBPARTY_TYPE_DUNGEON;
								}

								m_pApp->m_pMainWindow->GenerateRandomEncounterTabs(pParty, pSubParty, pMainPartyDialog, FALSE);
							}
						}

						
						if(pMonsterManualEntry)
						{
							RollMonsterTreasure(pCacheNPC, pMonsterManualEntry, TRUE, FALSE);
						
							for(int i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
							{
								pCacheNPC->m_Inventory[i].CopyFull(&m_pTreasureCaches->m_Caches[nCacheCount].m_Inventory[i]);
							}

							sprintf(m_pTreasureCaches->m_Caches[nCacheCount].m_szCacheDetails, "%s (Treasure Type: %s)\n\0", pMonsterManualEntry->m_szMMName, pMonsterManualEntry->m_szTreasureType);
							strcpy(pCacheNPC->m_szDetails, m_pTreasureCaches->m_Caches[nCacheCount].m_szCacheDetails);

							++nCacheCount;
						}

						++nOccupiedRooms;
					}
					

					break;
				}
			}
		}

		
	}

	m_pDNDMap->MarkChanged();

	delete pDungeon;

	InvalidateRect(NULL);

	m_pApp->m_pMainWindow->SortTabs();
}



void cDMMapViewDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
}

void cDMMapViewDialog::OnChildActivate()
{
	CDialog::OnChildActivate();
}

void cDMMapViewDialog::OnBnClickedSubpartiesCheck()
{
	// TODO: Add your control notification handler code here
}


void cDMMapViewDialog::OnBnClickedCharIconsCheck()
{
	UpdateData(TRUE);

	InvalidateRect(NULL);
}


void cDMMapViewDialog::OnBnClickedLayerCheck1()
{
	UpdateData(TRUE);

	m_pDNDMap->MarkChanged();

	m_pDNDMap->m_bDisplayLayer[0] = m_bLayer1;

	ForceRefresh();
}


void cDMMapViewDialog::OnBnClickedLayerCheck2()
{
	UpdateData(TRUE);

	m_pDNDMap->MarkChanged();

	m_pDNDMap->m_bDisplayLayer[1] = m_bLayer2;

	ForceRefresh();
}


void cDMMapViewDialog::OnBnClickedLayerCheck3()
{
	UpdateData(TRUE);

	m_pDNDMap->MarkChanged();

	m_pDNDMap->m_bDisplayLayer[2] = m_bLayer3;

	ForceRefresh();
}


void cDMMapViewDialog::OnBnClickedLayerCheck4()
{
	UpdateData(TRUE);

	m_pDNDMap->MarkChanged();

	m_pDNDMap->m_bDisplayLayer[3] = m_bLayer4;

	ForceRefresh();
}

void cDMMapViewDialog::OnBnClickedIsometricCheck()
{
	UpdateData(TRUE);

	ForceRefresh();
}


void cDMMapViewDialog::ForceRefresh()
{
	for (int xx = 0; xx < m_pDNDMap->m_nColumns; ++xx)
	{
		for (int yy = 0; yy < m_pDNDMap->m_nRows; ++yy)
		{
			cDNDMapCell *pCell = &m_pDNDMap->m_Cells[xx][yy];

			if (pCell->m_pBitmap != NULL)
			{
				delete pCell->m_pBitmap;
				pCell->m_pBitmap = NULL;
			}
		}
	}

	InvalidateRect(NULL);
}


void cDMMapViewDialog::CheckLayer(BOOL bCheck, int nLayer, CString szBitmapPath, Bitmap *pBaseBitmap)
{
	if (bCheck == FALSE)
		return;

	// "<$DMAPATH>\DATA\MAPS\BITMAPS\FLANAESS\FLANAESS_321.JPG"

	CFileFind find;

	CString szLayer;
	szLayer.Format("_L%d.???", nLayer);

	szBitmapPath.Replace(".JPG", "");
	szBitmapPath.Replace(".BMP", "");
	szBitmapPath.Replace(".PNG", "");
	szBitmapPath += szLayer;

	szBitmapPath.Replace("<$DMAPATH>", m_pApp->m_szEXEPath);

	BOOL bFound = find.FindFile(szBitmapPath);

	while (bFound)
	{
		int nPos = szBitmapPath.ReverseFind('\\');

		bFound = find.FindNextFile();

		szLayer = find.GetFileName();

		CString szNewPath = szBitmapPath.Left(nPos) + "\\" + szLayer;

		LPWSTR wcsFile = szNewPath.AllocSysString();

		Bitmap *pLayerBitmap = new Bitmap(wcsFile, FALSE);

		Status rc;
		Graphics* g = Graphics::FromImage(pBaseBitmap);

		ImageAttributes imAttr;
		imAttr.SetColorKey(Color(m_pDNDMap->m_nTransRed, m_pDNDMap->m_nTransGreen, m_pDNDMap->m_nTransBlue), Color(m_pDNDMap->m_nTransRed, m_pDNDMap->m_nTransGreen, m_pDNDMap->m_nTransBlue), ColorAdjustTypeBitmap);

		Rect destRect(0, 0, pLayerBitmap->GetWidth() - 1, pLayerBitmap->GetHeight() - 1);

		rc = g->DrawImage(pLayerBitmap, destRect, 0, 0, pLayerBitmap->GetWidth() - 1, pLayerBitmap->GetHeight() - 1, Gdiplus::UnitPixel, &imAttr);

		delete pLayerBitmap;

		break;
	}
}

void cDMMapViewDialog::DrawCharacterIcon(Graphics *pGraphics, int nX, int nY, char *szFileName, BOOL bReversed)
{
	m_pDNDMap->m_nMapIconScale = m_nIconScale;

	if (szFileName[0] == 0)
	{
		return;
	}

	CString szNewPath = szFileName;

	LPWSTR wcsFile = szNewPath.AllocSysString();

	Bitmap *pIconBitmap = new Bitmap(wcsFile, FALSE);

	Status rc;

	Color TransColor;

	pIconBitmap->GetPixel(0, 0, &TransColor);

	ImageAttributes imAttr;
	imAttr.SetColorKey(TransColor, TransColor, ColorAdjustTypeBitmap);

	nY -= 8; // red triangle height

	float fIconScale = 0.25f;

	float fWidth = (float)pIconBitmap->GetWidth();
	float fHeight = (float)pIconBitmap->GetHeight();

	int nScale = m_nIconScale;
	float fScale = 1.0f;

	if (nScale > 0)
	{
		fScale = 1.0f + (((float)nScale) / 1000.0f);
	}

	if (nScale < 0)
	{
		fScale = 1.0f -(((float)-nScale) / 1100.0f);
	}


	fWidth *= m_fViewScale;
	fHeight *= m_fViewScale;

	fWidth *= fScale;
	fHeight *= fScale;

	fWidth *= fIconScale;
	fHeight *= fIconScale;

	
	nY = nY - (int)fHeight;

	int nOffsetX = 0;
	if (bReversed)
	{
		fWidth = -fWidth;
		nOffsetX = (int)(-fWidth/2);
	}
	else
	{
		nX = nX - (int)(fWidth / 2);
	}

	Rect destRect(nX + nOffsetX, nY, (int)fWidth, (int)fHeight);

	rc = pGraphics->DrawImage(pIconBitmap, destRect, 0, 0, pIconBitmap->GetWidth() - 1, pIconBitmap->GetHeight() - 1, Gdiplus::UnitPixel, &imAttr);

	delete pIconBitmap;
}



void cDMMapViewDialog::DrawMonsterIcon(Graphics *pGraphics, int nX, int nY, cDNDNonPlayerCharacter *pNPC, BOOL bReversed)
{
	if (pNPC == NULL)
	{
		return;
	}

	CString szPath = m_pApp->m_szEXEPath + _T("data//monster_pictures//icons//");
	CString szSearchName = _T("");
	
	//find by monster name
	szSearchName = pNPC->m_szCharacterName;
	szSearchName += ".*";

	szSearchName.MakeUpper();
	szPath += szSearchName;
	szPath.MakeUpper();

	CFileFind find;
	BOOL bFound = find.FindFile(szPath);

	while (bFound)
	{
		bFound = find.FindNextFile();

		CString strFileName = find.GetFileName();

		strFileName.MakeUpper();

		szPath.Replace(szSearchName, strFileName);

		DrawCharacterIcon(pGraphics, nX, nY, szPath.GetBuffer(0), bReversed);

		return;
	}


	//find by monster type
	szPath = m_pApp->m_szEXEPath + _T("data//monster_pictures//icons//");
	szSearchName = _T("");

	PDNDMONSTERMANUALENTRY pMonster = NULL;
	m_pApp->m_MonsterManualIndexedMap.Lookup(pNPC->m_nMonsterIndex, pMonster);

	if (pMonster != NULL)
	{
		szSearchName = pMonster->m_szMMName;
		szSearchName += ".*";
	}
	else
	{
		return;
	}

	szSearchName.MakeUpper();
	szPath += szSearchName;
	szPath.MakeUpper();


	bFound = find.FindFile(szPath);

	while (bFound)
	{
		bFound = find.FindNextFile();

		CString strFileName = find.GetFileName();

		strFileName.MakeUpper();

		szPath.Replace(szSearchName, strFileName);

		DrawCharacterIcon(pGraphics, nX, nY, szPath.GetBuffer(0), bReversed);

		return;
	}
	
}

void cDMMapViewDialog::DrawTransparentBitmap(Graphics* g, Bitmap *pBitmap, int nX, int nY, int nSizeX, int nSizeY, int nBitmapSizeX, int nBitmapSizeY, float fAlpha)
{
	Status rc;

	ColorMatrix ClrMatrix = {
		1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f, fAlpha, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	ImageAttributes ImgAttr;

	ImgAttr.SetColorMatrix(&ClrMatrix, ColorMatrixFlagsDefault,
		ColorAdjustTypeBitmap);

	Rect destRect(nX, nY, nSizeX, nSizeY);

	g->DrawImage(pBitmap, destRect, 0, 0, nBitmapSizeX, nBitmapSizeY, Gdiplus::UnitPixel, &ImgAttr);
	//g->DrawImage(m_pFogOfWarBitmap, nX, nY, nSizeX, nSizeY);
}



void cDMMapViewDialog::OnNMReleasedcaptureScaleSlider2(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	UpdateData(TRUE);

	InvalidateRect(NULL);

	UpdateDetachedMaps();
}



void cDMMapViewDialog::OnNMReleasedcaptureScaleSlider3(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	UpdateData(TRUE);

	m_fLightingAlpha = float(m_nLightingSlider) / 100.0f;

	InvalidateRect(NULL);

	UpdateDetachedMaps();
}



void cDMMapViewDialog::OnBnClickedLabelsCheck()
{
	UpdateData(TRUE);

	InvalidateRect(NULL);
}


void cDMMapViewDialog::OnBnClickedDetachButton()
{
	//TRACE("PING");
	//DWORD style = GetWindowLong(m_hWnd, GWL_STYLE);
	//style &= ~WS_CAPTION;

	if (m_bDetachedWindow == TRUE)
	{
		return;
	}

	CDMHelperDlg *pMainDlg = (CDMHelperDlg *)m_pApp->m_pMainWindow;

	int nTabSelected = pMainDlg->m_cMainTab.GetCurSel();

	if (nTabSelected <= 0)
		return;
	
	SetWindowLong(m_hWnd, GWL_STYLE, WS_OVERLAPPEDWINDOW);

	::SetWindowPos(m_hWnd, 0, 0, 0, 0, 0, 39);

	m_bDetachedWindow = TRUE;

	m_bLabelsCheck = FALSE;

	m_pApp->m_DetachedMapViewMap.SetAt(m_pDNDMap->m_dwMapID, this);

	m_pParent = NULL;
	SetParent(NULL);

	cDNDDisplayTab *pDeleteTab = m_pApp->m_SubTabArray[nTabSelected];

	if (pMainDlg->ValidateDeletedTab(pDeleteTab))
	{
		int nNewSize = m_pApp->m_TabArray.GetSize() - 1;
		nTabSelected = pDeleteTab->m_wTabId;

		for (int i = nTabSelected; i < nNewSize; ++i)
		{
			cDNDDisplayTab *pTab = m_pApp->m_TabArray[i];

			if (pTab != NULL)
			{
				m_pApp->m_TabArray[i] = m_pApp->m_TabArray[i + 1];
			}
		}

		m_pApp->m_TabArray.SetSize(nNewSize);

		//pDeleteTab->m_pWindow->PostMessage(WM_CLOSE);

		pMainDlg->SortTabs();

		pMainDlg->PickTab();
	}

	m_cDetachButton.ShowWindow(SW_HIDE);

	PostMessage(WM_SYSCOMMAND, SC_RESTORE, 0);

}


BOOL cDMMapViewDialog::GetMonitorInfo(int nDeviceIndex, LPSTR lpszMonitorInfo)
{

	BOOL bResult = TRUE;

	DISPLAY_DEVICE DispDev;
	char szDeviceName[32];

	ZeroMemory(&DispDev, sizeof(DISPLAY_DEVICE));
	DispDev.cb = sizeof(DISPLAY_DEVICE);

	// After first call to EnumDisplayDevices DispDev.DeviceString 
	//contains graphic card name
	if (EnumDisplayDevices(NULL, nDeviceIndex, &DispDev, 0)) {
		lstrcpy(szDeviceName, DispDev.DeviceName);

		// after second call DispDev.DeviceString contains monitor's name 
		EnumDisplayDevices(szDeviceName, 0, &DispDev, 0);

		lstrcpy(lpszMonitorInfo, DispDev.DeviceString);
	}
	else 
	{
		bResult = FALSE;
	}

	//FreeLibrary(hInstUserLib);

	return bResult;
}


void cDMMapViewDialog::OnBnClickedFlipButton()
{
	// this function replaced by CONTROL+F hotkey

#if 0
	DISPLAY_DEVICE dd;
	dd.cb = sizeof(DISPLAY_DEVICE);

	BOOL bDone = FALSE;
	DWORD deviceNum = 0;
	while (bDone == FALSE && EnumDisplayDevices(NULL, deviceNum, &dd, 0))
	{
		//DumpDevice(dd, 0);
		DISPLAY_DEVICE newdd = { 0 };
		newdd.cb = sizeof(DISPLAY_DEVICE);
		DWORD monitorNum = 0;
		while (bDone == FALSE && EnumDisplayDevices(dd.DeviceName, monitorNum, &newdd, 0))
		{
			monitorNum++;

			if (deviceNum == 1 && monitorNum == 1)
			{
				bDone = TRUE;
			}
		}
		deviceNum++;
	}


	DEVMODE devMode;
	long r;

	// Init DEVMODE to current settings
	ZeroMemory(&devMode, sizeof(DEVMODE));
	devMode.dmSize = sizeof(devMode);

	EnumDisplaySettingsEx(dd.DeviceName, ENUM_CURRENT_SETTINGS, &devMode, NULL);
	//ShowDevMode(devMode);

	/* Rotate Orientation - 180 */

	if (m_nOrientation == DMDO_DEFAULT)
		m_nOrientation = DMDO_180;
	else
		m_nOrientation = DMDO_DEFAULT;

	devMode.dmDisplayOrientation = m_nOrientation;

	/* Rotate Orientation - 90 */
	//devMode.dmDisplayOrientation = DMDO_90;

	//swap(devMode.dmPelsHeight, devMode.dmPelsWidth);
	//int nSwap = devMode.dmPelsHeight;
	//devMode.dmPelsHeight = devMode.dmPelsWidth;
	//devMode.dmPelsWidth = nSwap;

	devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYORIENTATION;
	r = ChangeDisplaySettingsEx(dd.DeviceName, &devMode, NULL, CDS_RESET, NULL);

	switch (r)
	{
	case DISP_CHANGE_SUCCESSFUL:
		//cout << "ChangeDisplaySettingsEx returns DISP_CHANGE_SUCCESSFUL" << endl;
		break;
	case DISP_CHANGE_BADDUALVIEW:
		//cout << "ChangeDisplaySettingsEx returns DISP_CHANGE_BADDUALVIEW" << endl;
		break;
	case DISP_CHANGE_BADFLAGS:
		//cout << "ChangeDisplaySettingsEx returns DISP_CHANGE_BADFLAGS" << endl;
		break;
	case DISP_CHANGE_BADMODE:
		//cout << "ChangeDisplaySettingsEx returns DISP_CHANGE_BADMODE" << endl;
		break;
	case DISP_CHANGE_BADPARAM:
		//cout << "ChangeDisplaySettingsEx returns DISP_CHANGE_BADPARAM" << endl;
		break;
	case DISP_CHANGE_FAILED:
		//cout << "ChangeDisplaySettingsEx returns DISP_CHANGE_FAILED" << endl;
		break;
	case DISP_CHANGE_NOTUPDATED:
		//cout << "ChangeDisplaySettingsEx returns DISP_CHANGE_NOTUPDATED" << endl;
		break;
	case DISP_CHANGE_RESTART:
		//cout << "ChangeDisplaySettingsEx returns DISP_CHANGE_RESTART" << endl;
		break;
	default:
		//cout << "ChangeDisplaySettingsEx - Unexpected return value." << endl;
		break;

	}

#endif
}

void cDMMapViewDialog::CleanupMapSFX()
{
	BOOL bKilledGIF = FALSE;
	int nIndex = 0;
	do
	{
		if (m_pDNDMap->m_MapSFX[nIndex].m_SFXState != DND_SFX_STATE_UNDEF) // find an open slot
		{
			if (m_pDNDMap->m_MapSFX[nIndex].m_bAnimated)
			{
				if (m_pDNDMap->m_MapSFX[nIndex].m_pDataPtr != NULL)
				{
					ImageEx* _GIFImage = (ImageEx*)m_pDNDMap->m_MapSFX[nIndex].m_pDataPtr;
					_GIFImage->SetPause(TRUE);
					_GIFImage->Destroy();
					m_pDNDMap->m_MapSFX[nIndex].m_pDataPtr = NULL;

					bKilledGIF = TRUE;
				}
			}
			else
			{
				Bitmap* pSFXBitmap = (Bitmap*)m_pDNDMap->m_MapSFX[nIndex].m_pDataPtr;
				if (pSFXBitmap != NULL)
				{
					delete pSFXBitmap;
				}
				m_pDNDMap->m_MapSFX[nIndex].m_pDataPtr = NULL;
			}
		}

		++nIndex;

	} while (nIndex < MAX_MAP_SFX);

	if (m_bShuttingDown == TRUE && bKilledGIF == TRUE) // sleep to let the GIF threads exit cleanly
	{
		Sleep(1000);
	}
}

void cDMMapViewDialog::DrawMapSFX(Graphics* g)
{
	
	int nIndex = 0;

	do
	{
		if (m_pDNDMap->m_MapSFX[nIndex].m_SFXState != DND_SFX_STATE_UNDEF) // find an open slot
		{
			//strcpy(m_pDNDMap->m_MapSFX[nIndex].m_szSFXName, m_szSFXName.Left(31));
			//strcpy(m_pDNDMap->m_MapSFX[nIndex].m_szGFXFileName, m_szSFXGFXFileName.Left(255));
			//m_pDNDMap->m_MapSFX[nIndex].m_szSFXFileName[32];


			//m_pDNDMap->m_MapSFX[nIndex].m_bCycle = FALSE;
			//m_pDNDMap->m_MapSFX[nIndex].m_bAnimated = FALSE;

			//m_pDNDMap->m_MapSFX[nIndex].m_nMapX = m_nMouseX;
			//m_pDNDMap->m_MapSFX[nIndex].m_nMapY = m_nMouseY;

			//m_pDNDMap->m_MapSFX[nIndex].m_fScale;

			//m_pDNDMap->m_MapSFX[nIndex].m_pDataPtr = NULL;

			//m_pSFXButtonBitmap

			float fX = 0.0f;
			float fY = 0.0f;

			if (m_pDNDMap->m_bMapScaleFeet)
			{
				//feet to pixels
				fX = m_pDNDMap->m_MapSFX[nIndex].m_nMapX / m_pDNDMap->m_fScaleX * m_fViewScale;
				fY = m_pDNDMap->m_MapSFX[nIndex].m_nMapY / m_pDNDMap->m_fScaleY * m_fViewScale;
			}
			else
			{
				//miles to pixels
				fX = ((m_pDNDMap->m_MapSFX[nIndex].m_nMapX - m_pDNDMap->m_fParentMapOriginX) / m_pDNDMap->m_fScaleX) * m_fViewScale;
				fY = ((m_pDNDMap->m_MapSFX[nIndex].m_nMapY - m_pDNDMap->m_fParentMapOriginY) / m_pDNDMap->m_fScaleY) * m_fViewScale;
			}

			int nX = (int)fX;
			int nY = (int)fY;

			nX += m_nCornerX;
			nY += m_nCornerY;

			switch (m_pDNDMap->m_MapSFX[nIndex].m_SFXState)
			{
				case DND_SFX_STATE_READY:
				{
					if (m_pDNDMap->m_MapSFX[nIndex].m_bAnimated)
					{
						if (m_pDNDMap->m_MapSFX[nIndex].m_pDataPtr != NULL)
						{
							ImageEx* _GIFImage = (ImageEx*)m_pDNDMap->m_MapSFX[nIndex].m_pDataPtr;
							_GIFImage->Destroy();
							m_pDNDMap->m_MapSFX[nIndex].m_pDataPtr = NULL;
						}
					}
					else
					{
					}

					break;
				}
				case DND_SFX_STATE_TRIGGERED_START:
				{
					m_pDNDMap->m_MapSFX[nIndex].m_SFXState = DND_SFX_STATE_TRIGGERED;

					if (m_pDNDMap->m_MapSFX[nIndex].m_szSFXFileName[0] != 0 && m_bDetachedWindow == FALSE)
					{
						m_pApp->PlaySoundFX(m_pDNDMap->m_MapSFX[nIndex].m_szSFXFileName);
					}

					// fall thru
				}
				case DND_SFX_STATE_TRIGGERED:
				{
					if (m_pDNDMap->m_MapSFX[nIndex].m_bAnimated)
					{
						if (m_pDNDMap->m_MapSFX[nIndex].m_pDataPtr == NULL)
						{
							CString szPath = m_pDNDMap->m_MapSFX[nIndex].m_szGFXFileName;
							LPWSTR wcsFile = szPath.AllocSysString();
							ImageEx* _GIFImage = new ImageEx(wcsFile, FALSE);
							_GIFImage->InitAnimation(m_hWnd, CPoint(nX, nY), m_pDNDMap->m_MapSFX[nIndex].m_fScale, m_pDNDMap->m_MapSFX[nIndex].m_bCycle, &m_bMapPaint);
							m_pDNDMap->m_MapSFX[nIndex].m_pDataPtr = (LPVOID)_GIFImage;
						}
						else
						{
							ImageEx* _GIFImage = (ImageEx*)m_pDNDMap->m_MapSFX[nIndex].m_pDataPtr;
							_GIFImage->Position(nX, nY, m_fViewScale, m_pDNDMap->m_MapSFX[nIndex].m_fScale);
							_GIFImage->ResetBackground();

							if (_GIFImage->IsCycleComplete() && m_pDNDMap->m_MapSFX[nIndex].m_bCycle == FALSE)
							{
								m_pDNDMap->m_MapSFX[nIndex].m_SFXState = DND_SFX_STATE_READY;
								InvalidateRect(NULL);
							}
						}
					}
					else
					{
						if (m_pDNDMap->m_MapSFX[nIndex].m_pDataPtr == NULL)
						{
							CString szPath = m_pDNDMap->m_MapSFX[nIndex].m_szGFXFileName;
							LPWSTR wcsFile = szPath.AllocSysString();

							Bitmap* pSFXBitmap = new Bitmap(wcsFile, FALSE);
							m_pDNDMap->m_MapSFX[nIndex].m_pDataPtr = (LPVOID)pSFXBitmap;
						}

						if (m_pDNDMap->m_MapSFX[nIndex].m_pDataPtr != NULL)
						{
							Bitmap* pSFXBitmap = (Bitmap*)m_pDNDMap->m_MapSFX[nIndex].m_pDataPtr;

							ImageAttributes imAttr;
							imAttr.SetColorKey(Color(m_pDNDMap->m_nTransRed, m_pDNDMap->m_nTransGreen, m_pDNDMap->m_nTransBlue), Color(m_pDNDMap->m_nTransRed, m_pDNDMap->m_nTransGreen, m_pDNDMap->m_nTransBlue), ColorAdjustTypeBitmap);

							int nWidth = pSFXBitmap->GetWidth();
							int nHeight = pSFXBitmap->GetHeight();

							float fScale = m_fViewScale / m_pDNDMap->m_MapSFX[nIndex].m_fScale;

							nWidth *= fScale;
							nHeight *= fScale;

							Rect destRect(nX - nWidth / 2, nY - nHeight / 2, nWidth - 1, nHeight - 1);

							g->DrawImage(pSFXBitmap, destRect, 0, 0, pSFXBitmap->GetWidth() - 1, pSFXBitmap->GetHeight() - 1, Gdiplus::UnitPixel, &imAttr);
						}

					}



					break;
				}
			}

			if (m_bLabelsCheck)
			{
				g->DrawImage(m_pSFXButtonBitmap, nX, nY, 32, 16);
			}

		}
		else
		{
			break;
		}

		++nIndex;

	} while (nIndex < MAX_MAP_SFX);
}

#if _PARTICLE_WEATHER
UINT DMParticleThreadProc(LPVOID pData)
{
	cDMMapViewDialog *pMapViewDlg = (cDMMapViewDialog*)pData;

	Sleep(5000);

	do
	{
		Sleep(10);

		pMapViewDlg->UpdateParticles();

	} while (pMapViewDlg->m_bShuttingDown == FALSE);

	return 0;
}

void cDMMapViewDialog::UpdateParticles()
{
	HDC hDC = ::GetDC(m_hWnd);
	if (hDC)
	{
		Graphics graphics(hDC);

		int nSizeX = (m_pDNDMap->m_nColumns * m_pDNDMap->m_nPixelSizeX) * m_fViewScale;
		int nSizeY = (m_pDNDMap->m_nRows * m_pDNDMap->m_nPixelSizeY)* m_fViewScale;
		
		if (m_bMapPaint == FALSE) // rain and snow
		{
			POINT pointScreen;
			pointScreen.x = 0;
			pointScreen.y = 0;

			ClientToScreen(&pointScreen);

			int nCols = m_pDNDMap->m_nColumns;
			int nRows = m_pDNDMap->m_nRows;

			if (m_pParticleBufferBitmap == NULL)
			{
				HDC scrdc, memdc;
				HBITMAP membit;
				scrdc = ::GetDC(0);

				memdc = CreateCompatibleDC(scrdc);
				membit = CreateCompatibleBitmap(scrdc, nSizeX, nSizeY);
				HBITMAP hOldBitmap = (HBITMAP)SelectObject(memdc, membit);
				BitBlt(memdc, 0, 0, nSizeX, nSizeY, scrdc, pointScreen.x, pointScreen.y, SRCCOPY);

				m_pParticleBufferBitmap = new Bitmap(membit, NULL);

				SelectObject(memdc, hOldBitmap);

				DeleteObject(memdc);
				DeleteObject(membit);
				::ReleaseDC(0, scrdc);
			}
		}

		

		POINT pointScreen;
		pointScreen.x = 0;
		pointScreen.y = 0;

		//ClientToScreen(&pointScreen);

		if (m_bMapPaint == FALSE && m_pParticleBufferBitmap != NULL)
		{
			graphics.DrawImage(m_pParticleBufferBitmap, 0, 0, nSizeX, nSizeY);
		}

		int halfWidth = nSizeX / 2;
		int halfHeight = nSizeY / 2;

		ImageAttributes attr;
		attr.SetColorKey(Color(255, 0, 255), Color(255, 0, 255), ColorAdjustTypeBitmap);

		for (int i = 0; i < MAX_PARTICLES; i++)
			//for (int i = 0; i < 2; i++)
		{
			m_Particle[i].m_fZ += 5.0;

			if (m_Particle[i].m_fZ > MAX_PARTICLE_DEPTH)
			{
				m_Particle[i].m_fX = (rand() % nSizeX) - halfWidth;
				m_Particle[i].m_fY = (rand() % nSizeY) - halfHeight;
				m_Particle[i].m_fZ = (rand() % MAX_PARTICLE_DEPTH);
				m_Particle[i].m_fSize = 8.0f;
			}

			m_Particle[i].m_fSize -= 0.2f;

			float k = 128.0 / m_Particle[i].m_fZ;

			float px = m_Particle[i].m_fX * k + halfWidth;
			float py = m_Particle[i].m_fY  * k + halfHeight;

			RectF dst;
			dst.X = (int)(px + m_nCornerX)* m_fViewScale;
			dst.Y = (int)(py + m_nCornerY)* m_fViewScale;
			dst.Width = max((int)m_Particle[i].m_fSize,1);
			dst.Height = max((int)m_Particle[i].m_fSize, 1);

			if (m_bMapPaint == FALSE)
			{
				if (i % 2)
					graphics.DrawImage(m_pRainParticleBitmap, dst, 0, 0, 8, 8, UnitPixel, &attr); //, UnitPixel);
				else
					graphics.DrawImage(m_pSnowParticleBitmap, dst, 0, 0, 8, 8, UnitPixel, &attr); //, UnitPixel);
			}
		}

		
	}
}
#endif

