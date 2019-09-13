#include "afxwin.h"
#if !defined(AFX_DMSOUNDBOARDDIALOG_H__B281FB7F_6CA1_4808_B680_7D5EFB98947D__INCLUDED_)
#define AFX_DMSOUNDBOARDDIALOG_H__B281FB7F_6CA1_4808_B680_7D5EFB98947D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMSoundboardDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DMSoundboardDialog dialog

class DMSoundboardDialog : public CDialog
{
// Construction
public:
	DMSoundboardDialog(CWnd* pParent = NULL);   // standard constructor

	CDMHelperApp *m_pApp;

	CWnd* m_pParent;

	int m_nPage;
	BOOL m_bEditMode;

	void Init();

// Dialog Data
	//{{AFX_DATA(DMSoundboardDialog)
	enum { IDD = IDD_SOUNDBOARD_DIALOG };
	CTabCtrl	m_cSoundBoardTab;
	CString	m_szBoardName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DMSoundboardDialog)
	public:
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DMSoundboardDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg void OnEditRadio();
	afx_msg void OnPlayRadio();
	afx_msg void OnSelchangeSoundboardTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnChangeBoardnameEdit();
	afx_msg void OnMapWeaponSfxButton();
	afx_msg void OnMapSpellSfxButton();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedAddBoardButton();
	afx_msg void OnBnClickedDeleteBoardButton();
	CButton m_cDeleteBoardButton;
	CButton m_cAddBoardButton;
	afx_msg void OnBnClickedExportBoardsButton();
	afx_msg void OnBnClickedImportBoardsButton();
	CButton m_cExportBoardsButton;
	CButton m_cImportBoardsButton;
	CButton m_cResetDXSoundsButton;
	afx_msg void OnBnClickedResetDxSoundsButton();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DMSOUNDBOARDDIALOG_H__B281FB7F_6CA1_4808_B680_7D5EFB98947D__INCLUDED_)
