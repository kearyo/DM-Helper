#pragma once
#include "afxcmn.h"
#include "afxwin.h"

class DMPartyDialog;

// CDMInitiativeDialog dialog

class CDMInitiativeDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMInitiativeDialog)

public:
	CDMInitiativeDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMInitiativeDialog();

	CDMHelperApp *m_pApp;

	DMPartyDialog *m_pParentPartyDialog;

	PBASECHARVIEWMAP	m_PlayersViewMap;
	PBASECHARVIEWMAP	m_MonstersViewMap;

	int m_nOldCursor;

	int m_nMaxRoundTime;

	void Refresh();
	void RefreshAndSelect();

// Dialog Data
	enum { IDD = IDD_INITIATIVE_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CColoredListCtrl m_cCharacterList;
	afx_msg void OnClose();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnBnClickedRollInitButton();
	afx_msg void OnLvnKeydownCharacterList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickCharacterList(NMHDR* pNMHDR, LRESULT* pResult);
	
	afx_msg void OnLvnItemActivateCharacterList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnOdstatechangedCharacterList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedCharacterList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedNextRoundButton();
	CBitmapButton m_cUpListButton;
	CBitmapButton m_cDownListButton;
	afx_msg void OnBnClickedUpListButton();
	afx_msg void OnBnClickedDownListButton();
	BOOL m_bRollPCs;
	afx_msg void OnBnClickedRollPcsCheck();
	afx_msg void OnBnClickedRollTargetsButton();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton m_cRollInitButton;
	CButton m_cRollTargetsButton;
	CButton m_cNextRoundButton;
	CButton m_cRollPCsCheck;
	CStatic m_cDebugText;
	CString m_szDebugText;
};
