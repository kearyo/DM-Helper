#pragma once
#include "afxwin.h"


// CDMPartyLogDialog dialog

class CDMPartyLogDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMPartyLogDialog)

public:
	CDMPartyLogDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMPartyLogDialog();

	CDMHelperApp *m_pApp;
	DMPartyDialog *m_pParentWindow;
	PCALENDAR	m_pCalendar;

	Bitmap* m_pBackgroundBitmap;

	int m_nScrollPos;

	int m_nPickedEvent;
	int m_nEventStart;
	int m_nPickedRect;

	int m_nPickRects;
	CRect m_PickRect[100];
	int m_nEvents[100];

	int DrawLogText(char * szData, int x, int y, CDC *memdc);
	void ClearPicks();

// Dialog Data
	enum { IDD = IDD_PARTY_LOG_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CButton m_cEditButton;
	CButton m_cDeleteButton;
	CButton m_cInsertButton;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedDeleteButton();
	afx_msg void OnBnClickedInsertButton();
	afx_msg void OnBnClickedEditButton();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	CButton m_cEraseLogButton;
	afx_msg void OnBnClickedEraseButton();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
