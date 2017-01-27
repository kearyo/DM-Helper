#pragma once
#include "afxwin.h"


// CDMItemDescriptionDialog dialog

class CDMItemDescriptionDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMItemDescriptionDialog)

public:
	CDMItemDescriptionDialog(cDNDMagicItemDescription *pMagicItemDescription, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMItemDescriptionDialog();

	CDMHelperApp *m_pApp;
	CFont m_TextFont;
	cDNDMagicItemDescription *m_pMagicItemDescription;
	RECT m_rEditBoxRect;
	int m_nScrollPos;

// Dialog Data
	enum { IDD = IDD_ITEM_DESCRIPTION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CString m_szTextBox;
	CStatic m_cTextBox;
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);

	CString m_szDescriptionEdit;
	CEdit m_cDescriptionEdit;
};
