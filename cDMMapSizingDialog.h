#pragma once
#include "afxcmn.h"


// cDMMapSizingDialog dialog

class cDMMapSizingDialog : public CDialog
{
	DECLARE_DYNAMIC(cDMMapSizingDialog)

public:
	cDMMapSizingDialog(cDNDMap *pDNDMap, CWnd* pParent = NULL);   // standard constructor
	virtual ~cDMMapSizingDialog();

	CDMHelperApp *m_pApp;

	cDNDMap *m_pDNDMap;
	Bitmap* m_pOriginalBitmap;

// Dialog Data
	enum { IDD = IDD_MAP_SIZING_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedResizeCellButton();
	afx_msg void OnBnClickedResizeMapButton();
	CProgressCtrl m_cProgressBar;
	virtual BOOL OnInitDialog();
};
