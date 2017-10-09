#include "afxwin.h"
#include "afxcmn.h"
#if !defined(AFX_CDMMAPVIEWDIALOG_H__BB278983_68BC_4CA1_B382_BFCB8A52BA59__INCLUDED_)
#define AFX_CDMMAPVIEWDIALOG_H__BB278983_68BC_4CA1_B382_BFCB8A52BA59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// cDMMapViewDialog.h : header file
//


#define USE_MEMDC	TRUE 

/////////////////////////////////////////////////////////////////////////////
// cDMMapViewDialog dialog

typedef enum
{
	DND_MAP_MODE_UNDEF = 0,

	DND_MAP_MODE_EDIT,

	DND_MAP_MODE_PLACE_PARTY,
	DND_MAP_MODE_PLACE_CHARACTER,
	DND_MAP_MODE_PLACE_CACHE,

	DND_MAP_MODE_PLOT_PARTY_ROUTE,


	DND_MAP_MODE_PICK_MSG_CHILD_POINT_1,
	DND_MAP_MODE_PICK_CHILD_POINT_1,
	DND_MAP_MODE_PICKED_CHILD_POINT_1,

	DND_MAP_MODE_PICK_MSG_PARENT_POINT_1,
	DND_MAP_MODE_PICK_PARENT_POINT_1,
	DND_MAP_MODE_PICKED_PARENT_POINT_1,

	DND_MAP_MODE_PICK_MSG_CHILD_POINT_2,
	DND_MAP_MODE_PICK_CHILD_POINT_2,
	DND_MAP_MODE_PICKED_CHILD_POINT_2,

	DND_MAP_MODE_PICK_MSG_PARENT_POINT_2,
	DND_MAP_MODE_PICK_PARENT_POINT_2,
	DND_MAP_MODE_PICKED_PARENT_POINT_2,

	DND_MAP_MODE_PICK_MSG_SCALE_POINT_1,
	DND_MAP_MODE_PICK_SCALE_POINT_1,
	DND_MAP_MODE_PICKED_SCALE_POINT_1,

	DND_MAP_MODE_PICK_MSG_SCALE_POINT_2,
	DND_MAP_MODE_PICK_SCALE_POINT_2,
	DND_MAP_MODE_PICKED_SCALE_POINT_2,


	DND_MAP_MODE_PICK_MSG_COMPLETE,


	DND_MAP_LAST_MODE,

} DND_MAP_MODES;


class CDMPartyHotSpot
{
public:

	int m_nX;
	int m_nY;

	cDNDParty *m_pParty;

	CDMPartyHotSpot()
	{
		m_nX = 0;
		m_nY = 0;

		m_pParty = NULL;
	};

	void Clear()
	{
		m_nX = 0;
		m_nY = 0;

		m_pParty = NULL;
	};
};

class CDMCharacterHotSpot
{
public:

	int m_nX;
	int m_nY;

	DWORD m_dwCharacterID;

	CDMCharacterHotSpot()
	{
		m_nX = 0;
		m_nY = 0;

		m_dwCharacterID = 0L;
	};

	void Clear()
	{
		m_nX = 0;
		m_nY = 0;

		m_dwCharacterID = 0L;
	};
};

class CDMCacheHotSpot
{
public:

	int m_nX;
	int m_nY;

	cDNDCacheInventory *m_pCache;

	CDMCacheHotSpot()
	{
		m_nX = 0;
		m_nY = 0;

		m_pCache = NULL;
	};

	void Clear()
	{
		m_nX = 0;
		m_nY = 0;

		m_pCache = NULL;
	};
};

#define MAX_HOTSPOTS	MAX_MAP_CACHES

class DMMapEditDialog;
class CDMEditMapLayersDialog;

class cDMMapViewDialog : public CDialog
{
// Construction
public:
	cDMMapViewDialog(CDMHelperDlg* pMainDialog, cDNDMap *pDNDMap, CWnd* pParent = NULL);   // standard constructor

	CDMHelperApp *m_pApp;

	CWnd* m_pParent;
	CDMHelperDlg* m_pMainDialog;

	CDMPartyHotSpot m_PartyHotSpots[MAX_HOTSPOTS];
	CDMCharacterHotSpot m_CharacterHotSpots[MAX_HOTSPOTS];
	cDNDCacheInventory m_CacheBuffer[MAX_HOTSPOTS];
	CDMCacheHotSpot m_CacheHotSpots[MAX_HOTSPOTS];

	PDATAPICKERMAP	m_MonsterNameIndexer;

	CFont m_ListFont;

	BOOL m_bMapPaint;

	BOOL m_bMouseDrag;
	CPoint m_DragPoint;
	CPoint m_MousePoint;

	CRect* m_pUpdateRect;

	BOOL m_bDetachedWindow;

	int m_nOrientation;

	BOOL m_bCatchMe;	//this is a debugging variable

	//DWORD m_dwRandomDungeonPartyID;
	

	cDNDMap *LoadMap();
	void LoadMapFromFile(char *szFileName);
	void SaveExternal();
	void SaveMapToFile(char *szFileName);
	void LoadMapCaches(char *szFileName);
	Bitmap *ResourceToBitmap(const HINSTANCE hInstance, const int id);

	void DrawMapText(char * szData, int x, int y, CDC *memdc);

	PDNDPARTYVIEWDLG ValidateSelectedParty();
	void Reset();
	void Refresh();
	void ReturnFromEdit();

	void SelectAndCenterOnParty(DWORD dwPartyID);
	BOOL FindAndSelectMapTab(DWORD dwMapID);
	void ProcessMapModes();

	cDNDNonPlayerCharacter *AddCache(char *szCacheName, int nCachePosX, int nCachePosY, BOOL bPickTab);
	BOOL FindAndSelectCacheTab(DWORD dwCacheID);
	void ValidateCaches();
	void DeleteCaches();

	void UpdateMapLegend();


	void GenerateWeather(CString *pszWeatherData);

	void ZOrderSortTiles();
	void AddTile(int nTX, int nTY,  int nTile, int nRoomNumber, int nFlag);
	void RemoveTile(int nTX, int nTY);
	void ValidateTiles();
	void DrawChildMap(Graphics *graphics, cDNDMap *pDNDChildMap, int nX, int nY);
	BOOL IsOnScreen(int nTX, int nTY, int nRange = 0);
	void Translate2DCoordinatesToIsometric(int nMX, int nMY, float fViewScale, int *nIsoX, int *nIsoY);
	void TranslateIsoCoordinatesTo2D(int nMX, int nMY, int *nIsoX, int *nIsoY);
	void DrawMapTiles(Graphics *pg);
	void DrawMapTile(Graphics *pg, int nCell, int nX, int nY, int nSizeX, int nSizeY, float fViewScale);
	void GenerateRandomDungeon(int nSizeX, int nSizeY, BOOL bCreateCaches, int nMonsterLevel);

	void CacheMoveAssociatedSubParty(cDNDCacheInventory *pCache, float fOldX, float fOldY);

	void ForceRefresh();
	void CheckLayer(BOOL bCheck, int nLayer, CString szBitmapPath, Bitmap *pBaseBitmap);

	void DrawCharacterIcon(Graphics *pGraphics, int nX, int nY, char *szFileName, BOOL bReversed);
	void DrawMonsterIcon(Graphics *pGraphics, int nX, int nY, cDNDNonPlayerCharacter *pNPC, BOOL bReversed);
	void DrawTransparentBitmap(Graphics* g, Bitmap *pBitmap, int nX, int nY, int nSizeX, int nSizeY, int nBitmapSizeX, int nBitmapSizeY, float fAlpha);
	void UpdateDetachedMaps();
	void SyncDetachedMaps(PDNDMAPVIEWDLG pMapDlg1, PDNDMAPVIEWDLG pMapDlg2, BOOL bSyncSFX);

	void CleanUp();
	void CleanupMapSFX();
	void DrawMapSFX(Graphics* g);

	//void CreateWeatherWindow();
	void PositionWeatherWindow();
	void WaitWeatherThreadExit();
	BOOL MapHasDetachedChildren();

	BOOL GetMonitorInfo(int nDeviceIndex, LPSTR lpszMonitorInfo);

	DMMapEditDialog *m_pMapEditDlg; 
	CDMEditMapLayersDialog *m_pEditMapLayersDialog;

	cDNDMap *m_pDNDMap;

	DND_MAP_MODES m_MapMode;

	BOOL m_bWaitingOnUserClick;

	cDNDParty *m_pSelectedParty;
	float m_fSelectedPartyRouteDistance;

	DWORD m_dwSelectedSubPartyID;

	DWORD m_dwDraggedPartyID;

	cDNDCharacter *m_pSelectedCharacter;
	DWORD m_dwDraggedCharacterID;
	int m_nDraggedSFXIndex;
	int m_nLastSFXPosX;
	int m_nLastSFXPosY;

	int m_nLastLeftMouseClickX;
	int m_nLastLeftMouseClickY;

	//Bitmap* m_pBitmap_1;
	//Bitmap* m_pBitmap_2;

	Bitmap* m_pLightingAlphaBitmap;
	Bitmap* m_pFogOfWarBitmap;
	Bitmap* m_pSFXButtonBitmap;

	float m_fLightingAlpha;

	CBitmap m_bmp;

	//ImageEx*			m_image;

	float m_fViewScale;

	int m_nOldCornerX;
	int m_nOldCornerY;

	int m_nCornerX;
	int m_nCornerY;

	int m_nMouseX;
	int m_nMouseY;

	int m_nUnshiftedMouseX;
	int m_nUnshiftedMouseY;

	int m_nMousePointX;
	int m_nMousePointY;

	int m_nWindowTimer;

	int m_nMapCharacters;
	int m_nMapParties;

	int m_nMapCaches;
	cDNDCacheInventory *m_pHoverCache;

	CString m_szPartyAlert;

	cDNDTreasureCache *m_pTreasureCaches;
	cDNDCacheInventory *m_pSelectedCache;

	int m_nSelectedMapTile;
	BOOL m_bShowTileGrid;

	int m_nRandomDungeonOccupancy;

	BOOL m_bShuttingDown;

	HWND	m_WeatherWindowHWnd;

	BOOL m_bWeatherThreadRunning;
	
	
// Dialog Data
	//{{AFX_DATA(cDMMapViewDialog)
	enum { IDD = IDD_MAP_VIEW_DIALOG };
	CButton	m_cTravelButton;
	CSliderCtrl	m_cScaleSlider;
	CComboBox	m_cCharacterCombo;
	CComboBox	m_cPartyCombo;
	CStatic	m_cMapLegend;
	CString	m_szMapLegend;
	BOOL	m_bShowPartiesCheck;
	BOOL	m_bShowRoutesCheck;
	BOOL	m_bShowChildMapsCheck;
	CString	m_szHint;
	int		m_nScaleSlider;
	CBitmapButton m_cShowButton;
	CBitmapButton m_cHideButton;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(cDMMapViewDialog)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(cDMMapViewDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEditButton();
	afx_msg void OnLoadButton();
	afx_msg void OnSaveButton();
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangePartyCombo();
	afx_msg void OnPlacePartyButton();
	afx_msg void OnPlaceOnPartyButton();
	afx_msg void OnPlotPartyRouteButton();
	afx_msg void OnHideButton();
	afx_msg void OnShowButton();
	afx_msg void OnSelchangeCharacterCombo();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnReleasedcaptureScaleSlider(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnCancel();
	afx_msg void OnPlotPartyExtendRouteButton();
	afx_msg void OnTravelButton();
	afx_msg void OnTravelMountedButton();
	afx_msg void OnPlaceCharacterButton();
	afx_msg void OnPartyCharacterButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedPlaceCacheButton();
	afx_msg void OnBnClickedPlaceOnCacheButton();
	afx_msg void OnBnClickedAddCacheButton();
	afx_msg void OnBnClickedDeleteCacheButton();
	CComboBox m_cCacheCombo;
	afx_msg void OnCbnSelchangeCacheCombo();
	afx_msg void OnBnClickedPlaceOnCharacterButton();
	CEdit m_cCacheDetailsEdit;
	CString m_szCacheDetails;
	afx_msg void OnEnChangeCacheDetailsEdit();
	afx_msg void OnStnClickedBdrop();
	afx_msg void OnBnClickedPartiesCheck();
	afx_msg void OnBnClickedRoutesCheck();
	afx_msg void OnBnClickedChildMapsCheck();
	BOOL m_bShowCachesCheck;
	afx_msg void OnBnClickedCachesCheck();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	
	CButton m_cTravelMountedButton;
	CButton m_cTravelHoursCheck;
	BOOL m_bTravelHoursCheck;
	afx_msg void OnBnClickedHoursCheck();
	CButton m_cWeatherCheck;
	BOOL m_bWeatherCheck;
	afx_msg void OnBnClickedWeatherCheck();
	CButton m_cRestButton;
	afx_msg void OnBnClickedRestButton();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnChildActivate();
	BOOL m_bLayer1;
	BOOL m_bLayer2;
	BOOL m_bLayer3;
	BOOL m_bLayer4;
	afx_msg void OnBnClickedLayerCheck1();
	afx_msg void OnBnClickedLayerCheck2();
	afx_msg void OnBnClickedLayerCheck3();
	afx_msg void OnBnClickedLayerCheck4();
	CComboBox m_cSubPartyCombo;
	afx_msg void OnCbnSelchangeSubpartyCombo();
	BOOL m_bCharIcons;
	BOOL m_bSubPartyCheck;
	afx_msg void OnBnClickedSubpartiesCheck();
	afx_msg void OnBnClickedCharIconsCheck();
	afx_msg void OnBnClickedSetCharacterIconButton();
	CSliderCtrl m_cIconScale;
	int m_nIconScale;
	afx_msg void OnNMReleasedcaptureScaleSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	CButton m_cIsometricCheck;
	BOOL m_bIsometricCheck;
	afx_msg void OnBnClickedIsometricCheck();
	BOOL m_bLabelsCheck;
	afx_msg void OnBnClickedLabelsCheck();
	CButton m_cDetachButton;
	afx_msg void OnBnClickedDetachButton();
	afx_msg void OnBnClickedFlipButton();
	CButton m_cFlipDisplayButton;
	CButton m_cFogOfWarCheck;
	BOOL m_bFogOfWarCheck;
	afx_msg void OnBnClickedFogOfWarCheck();
	int m_nLightingSlider;
	CSliderCtrl m_cLightingSlider;
	afx_msg void OnNMReleasedcaptureScaleSlider3(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRainCheck();
	afx_msg void OnBnClickedSnowCheck();
	BOOL m_bRainCheck;
	CButton m_cRainCheck;
	CButton m_cSnowCheck;
	BOOL m_bSnowCheck;
	afx_msg void OnMove(int x, int y);
};




//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDMMAPVIEWDIALOG_H__BB278983_68BC_4CA1_B382_BFCB8A52BA59__INCLUDED_)
