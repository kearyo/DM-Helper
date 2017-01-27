#include "afxcmn.h"
#if !defined(AFX_DMCHARSHEETDIALOG_H__A4C60DB2_A2BA_4BDA_A870_FA75EFB365A9__INCLUDED_)
#define AFX_DMCHARSHEETDIALOG_H__A4C60DB2_A2BA_4BDA_A870_FA75EFB365A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMCharSheetDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DMCharSheetDialog dialog

class DMCharSheetDialog : public CDialog
{
// Construction
public:
	DMCharSheetDialog(cDNDCharacter	*_Character, CDMCharViewDialog *_pCharViewDialog, CWnd* pParent = NULL);   // standard constructor

	CDMHelperApp *m_pApp;

	BOOL m_bPrintSheet;
	BOOL m_bSaveSheet;

	int m_nPage;

	cDNDCharacter *m_pCharacter;
	Bitmap* m_pPortraitBitmap;
	CDMCharViewDialog *m_pCharViewDialog;
	CString m_szFileName;

	void PrintBitmap(CDC *memdc, CBitmap *bmp);

	void DrawTurnUndead(CDC *memdc, int nOffX, int nOffY, int nCellX);
	void DrawSpellList(int nBitMapResource, CDC *memdc);
	void DrawCSText(char * szData, int x, int y, CDC *memdc);
	void DrawCSText(int val, int x, int y, CDC *memdc, BOOL bPlus=FALSE);
	void DrawLFText(char * szData, int x, int y, CDC *memdc);

	void DMCharSheetDialog::DrawContainerCoins(POBJECTTYPE pObj, int *pnItemCount, int *pnStartX, int *pnStartY, CDC *pmemdc, int nLevel);
	void DrawSubInventory(int *pnItemCount, DWORD dwContainerID, int *pnStartX, int *nStartY, CDC *pmemdc, int nLevel);
	

// Dialog Data
	//{{AFX_DATA(DMCharSheetDialog)
	enum { IDD = IDD_CHAR_SHEET_DIALOG };
	CStatic	m_cPortraitButton;
	CStatic	m_cSaveButton;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DMCharSheetDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DMCharSheetDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg void OnSaveButton();
	afx_msg void OnClose();
	afx_msg void OnPortraitButton();
	afx_msg void OnPrintButton();
	afx_msg void OnFlipPageButton();
	afx_msg void OnPageBackButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int m_nScrollOffset;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	int m_nSliderOffset;
	CSliderCtrl m_cSliderOffset;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DMCHARSHEETDIALOG_H__A4C60DB2_A2BA_4BDA_A870_FA75EFB365A9__INCLUDED_)
