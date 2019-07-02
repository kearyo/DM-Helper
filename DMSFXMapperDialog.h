#if !defined(AFX_DMSFXMAPPERDIALOG_H__2078E084_DB6B_4FA5_99F7_8B627B522BCF__INCLUDED_)
#define AFX_DMSFXMAPPERDIALOG_H__2078E084_DB6B_4FA5_99F7_8B627B522BCF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMSFXMapperDialog.h : header file
//

class CDMListBox : public CListBox {
public:
   CDMListBox()
   {
   };

// Message map
protected:
   afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

   DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// DMSFXMapperDialog dialog

class DMSFXMapperDialog : public CDialog
{
// Construction
public:
	DMSFXMapperDialog(BOOL bSpells, CWnd* pParent = NULL);   // standard constructor

	void PopulateSFXList();
	void Refresh();

	CDMHelperApp *m_pApp;

	BOOL m_bSpells;

	int m_nBaseEvent;

// Dialog Data
	//{{AFX_DATA(DMSFXMapperDialog)
	enum { IDD = IDD_SFX_MAPPER_DIALOG };
	CButton	m_cMagicUserSpellCheck;
	CButton	m_cIllusionistSpellCheck;
	CButton	m_cDruidSpellCheck;
	CButton	m_cClericSpellCheck;
	CStatic	m_cInfoText;
	CButton	m_cRangedMiss;
	CButton	m_cRangedHit;
	CButton	m_cSwingMiss;
	CButton	m_cSwingHit;
	CListBox	m_cSFXMap4;
	CListBox	m_cSFXMap3;
	CListBox	m_cSFXMap2;
	CListBox	m_cSFXMap1;
	CListBox	m_cSFXList;
	CDMListBox	m_cEventList;
	CString	m_szTypeLabel;
	BOOL	m_bSwingHit;
	BOOL	m_bSwingMiss;
	BOOL	m_bRangedHit;
	BOOL	m_bRangedMiss;
	BOOL	m_bShowAllCheck;
	BOOL	m_bDruidSpellCheck;
	BOOL	m_bClericSpellCheck;
	BOOL	m_bIllusionistSpellCheck;
	BOOL	m_bMagicUserSpellCheck;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DMSFXMapperDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DMSFXMapperDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSelchangeEventList();
	afx_msg void OnMapSfx();
	afx_msg void OnPaint();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnClearSfx();
	afx_msg void OnSwingHitCheck();
	afx_msg void OnSwingMissCheck();
	afx_msg void OnRangedHitCheck();
	afx_msg void OnRangedMissCheck();
	afx_msg void OnShowAllCheck();
	afx_msg void OnClericSpellCheck();
	afx_msg void OnMagicuserSpellCheck();
	afx_msg void OnDruidSpellCheck();
	afx_msg void OnIllusionistSpellCheck();
	afx_msg void OnSelchangeSfxMap1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedPlaySfx();
	afx_msg void OnLbnDblclkSfxList();
	CString m_szFilter;
	afx_msg void OnEnChangeFilterEdit();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DMSFXMAPPERDIALOG_H__2078E084_DB6B_4FA5_99F7_8B627B522BCF__INCLUDED_)
