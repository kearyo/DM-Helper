#pragma once
#include "afxwin.h"


// CDMCharacterNotesDialog dialog

class CDMCharacterNotesDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMCharacterNotesDialog)

public:
	CDMCharacterNotesDialog(PDNDCHARACTER pPC, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMCharacterNotesDialog();

	void Update();

	CFont m_ListFont;

	PDNDCHARACTER m_pPC;

// Dialog Data
	enum { IDD = IDD_CHAR_NOTES_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	CString m_szNotes;
	CEdit m_cNotesEdit;
	afx_msg void OnEnChangeNotesEdit();
	afx_msg void OnBnClickedDone();
	afx_msg void OnBnClickedOk();
	CStatic m_cCharsRemaining;
	CString m_szCharsRemaining;
	afx_msg void OnPaint();
};
