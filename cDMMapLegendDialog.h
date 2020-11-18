#pragma once

#include "cDMMapViewDialog.h"

// cDMMapLegendDialog dialog

class cDMMapLegendDialog : public CDialog
{
	DECLARE_DYNAMIC(cDMMapLegendDialog)

public:
	cDMMapLegendDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~cDMMapLegendDialog();

	CDMHelperApp *m_pApp;
	cDMMapViewDialog *m_pParent;
	Bitmap* m_pBitmap;
	HICON m_hIcon;

	int m_nOffsetY;
	float m_fScaleY;
	float m_fViewScale;
	int m_nMaxScrollY;

// Dialog Data
	enum { IDD = IDD_MAP_LEGEND_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnPaint();
	virtual void PostNcDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
