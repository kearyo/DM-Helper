#include "afxwin.h"
#include "afxcmn.h"
#if !defined(AFX_DMMAPEDITDIALOG_H__94FAB518_2A38_4894_9691_08FDDC55E629__INCLUDED_)
#define AFX_DMMAPEDITDIALOG_H__94FAB518_2A38_4894_9691_08FDDC55E629__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMMapEditDialog.h : header file
//

class cDMMapViewDialog;

/////////////////////////////////////////////////////////////////////////////
// DMMapEditDialog dialog

class DMMapEditDialog : public CDialog
{
// Construction
public:
	DMMapEditDialog(cDNDMap *pDNDMap, CWnd* pParent = NULL);   // standard constructor

	CDMHelperApp *m_pApp;

	cDNDMap *m_pDNDMap;

	cDMMapViewDialog *m_pDMMapViewDialog;

	BOOL m_bMapReadyRender;
	BOOL m_bRefreshParent;
		
	int m_nMaxTiles;
	int m_nTileScrollBar;


	void Refresh();
	void StoreData();

	void DrawMapTiles(Graphics *pg, int nCell, int nX, int nY, int nSize);

// Dialog Data
	//{{AFX_DATA(DMMapEditDialog)
	enum { IDD = IDD_MAP_EDIT_DIALOG };
	CButton	m_cScaleMapButton;
	CEdit	m_cMapScaleEdit;
	CComboBox	m_cParentMapCombo;
	CString	m_szMapName;
	CString	m_szPixelsX;
	CString	m_szPixelsY;
	CString	m_szMapRows;
	CString	m_szMapColumns;
	CString	m_szMapScale;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DMMapEditDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DMMapEditDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeParentMapCombo();
	afx_msg void OnMilesRadio();
	afx_msg void OnFeetRadio();
	afx_msg void OnScaleMapButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAutoScaleMapButton();
	afx_msg void OnEnChangeMapNameEdit();
	CEdit m_cWeatherBaseCoord;
	afx_msg void OnBnClickedOk();
	CString m_szWeatherBaseCoord;
	CEdit m_cMapLatitudeMiles;
	CString m_szMapLatitudeMiles;
	afx_msg void OnEnChangeMapWeatherBaseCoordEdit();
	afx_msg void OnEnChangeMapLatitudeMilesEdit();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedTilesCheck();
	BOOL m_bMapTiles;
	CScrollBar m_cTileScroll;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CButton m_cGridCheck;
	BOOL m_bGridCheck;
	afx_msg void OnBnClickedGridCheck();
	CButton m_cRandomDungeonButton;
	afx_msg void OnBnClickedRandomDungeonButton();
	CButton m_cMapTiles;
	afx_msg void OnEnChangeMapPixelsYEdit();
	afx_msg void OnEnChangeMapRowsEdit();
	afx_msg void OnEnChangeMapColumnsEdit();
	afx_msg void OnEnChangeMapPixelsXEdit();
	CStatic m_cMonsterLevelStatic;
	CComboBox m_cMonsterLevel;
	afx_msg void OnBnClickedDebugButton();
	CStatic m_cRoomOccStatic;
	CEdit m_cRoomOccEdit;
	CString m_szRoomOcc;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CString m_szTransRed;
	CString m_szTransGreen;
	CString m_szTransBlue;
	CButton m_cReloadTilesButton;
	afx_msg void OnBnClickedReloadTilesButton();
	CStatic m_cTileHeightStatic;
	CSliderCtrl m_cTileHeightSlider;
	int m_nTileHeight;
	CEdit m_cTileHeightEdit;
	CString m_szTileHeight;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DMMAPEDITDIALOG_H__94FAB518_2A38_4894_9691_08FDDC55E629__INCLUDED_)
