#include "afxwin.h"
#if !defined(AFX_DMCASTSPELLDIALOG_H__4B53703E_54B1_42D1_B158_17ECEF9AF09C__INCLUDED_)
#define AFX_DMCASTSPELLDIALOG_H__4B53703E_54B1_42D1_B158_17ECEF9AF09C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMCastSpellDialog.h : header file
//


class cDNDCharacter;

/////////////////////////////////////////////////////////////////////////////
// DMCastSpellDialog dialog

class DMCastSpellDialog : public CDialog
{
// Construction
public:
	DMCastSpellDialog(DMPartyDialog *_pPartyDialog, CDMBaseCharViewDialog *_pBaseCharViewDialog, cDNDCharacter *_pCharacter, CWnd* pParent = NULL);   // standard constructor

	CDMHelperApp *m_pApp;

	DMPartyDialog *m_pPartyDialog; 
	CDMBaseCharViewDialog *m_pBaseCharViewDialog;
	cDNDCharacter	*m_pCharacter;

	void Refresh();

// Dialog Data
	//{{AFX_DATA(DMCastSpellDialog)
	enum { IDD = IDD_SPELL_CAST_DIALOG };
	CListBox	m_cSpellList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DMCastSpellDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DMCastSpellDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSpellInfo();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CButton m_cCastButton;
	afx_msg void OnLbnDblclkSpellList();
	afx_msg void OnLbnSelchangeSpellList();
	CString m_szSpellDesc;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DMCASTSPELLDIALOG_H__4B53703E_54B1_42D1_B158_17ECEF9AF09C__INCLUDED_)
