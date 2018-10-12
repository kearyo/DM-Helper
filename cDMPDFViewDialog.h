#pragma once
//#include "pdfviewerctrl1.h"
#include "afxwin.h"


// cDMPDFViewDialog dialog

class cDMPDFViewDialog : public CDialogEx
{
	DECLARE_DYNAMIC(cDMPDFViewDialog)

public:
	cDMPDFViewDialog(CString szPDFPath, int nPage, CWnd* pParent = NULL);   // standard constructor
	virtual ~cDMPDFViewDialog();

	void Cleanup();
	void Refresh();

	CDMHelperApp *m_pApp;

	CString m_szPDFPath;
	int m_nPDFPage;
	int m_nPDFPages;
	DWORD m_dwPDFID; 

// Dialog Data
	enum { IDD = IDD_PDF_VIEW_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	CString m_szPages;
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedPageBackButton();
	afx_msg void OnBnClickedPageFwdButton();
	afx_msg void OnBnClickedZoomInButton();
	afx_msg void OnBnClickedZoomOutButton();
	CString m_szPage;
	afx_msg void OnEnChangePageEdit();
	CEdit m_cPage;
	afx_msg void OnEnChangeSearchEdit();
	afx_msg void OnBnClickedSearchButton();
	CString m_szSearch;
	CButton m_cPageLeftButton;
	CButton m_cPageRightButton;
	CButton m_cZoomInButton;
	CButton m_cZoomOutButton;
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnClose();
};
