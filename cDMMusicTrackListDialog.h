#pragma once
#include "afxwin.h"


// cDMMusicTrackListDialog dialog

class cDMMusicTrackListDialog : public CDialog
{
	DECLARE_DYNAMIC(cDMMusicTrackListDialog)

public:
	cDMMusicTrackListDialog(CWnd* pParent, CString *pszReturnString);   // standard constructor
	virtual ~cDMMusicTrackListDialog();

	void LoadTracks();

	CDMHelperApp *m_pApp;
	CString *m_pszReturnString;

// Dialog Data
	enum { IDD = IDD_TRACK_LIST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CListBox m_cTrackList;
	afx_msg void OnLbnDblclkTrackList();
	CString m_szTrackSearch;
	afx_msg void OnEnChangeTrackSearchEdit();
};
