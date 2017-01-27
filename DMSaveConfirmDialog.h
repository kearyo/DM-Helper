#pragma once
#include "afxwin.h"


// CDMSaveConfirmDialog dialog

class CDMSaveConfirmDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMSaveConfirmDialog)

public:
	CDMSaveConfirmDialog(char *pszMsg, BOOL *pbSave, BOOL *pbSaveAll, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMSaveConfirmDialog();

	Bitmap* m_pCellBitmap;

	BOOL *m_pbSave;
	BOOL *m_pbSaveAll;

	//CString m_szSavePrompt;

	void DrawPromptText(char * szData, int x, int y, CDC *memdc);

// Dialog Data
	enum { IDD = IDD_SAVE_CONFIRM_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedOk2();
	afx_msg void OnBnClickedCancel();
	CButton m_cYesToAllButton;
	afx_msg void OnPaint();
	CString m_szSavePrompt;
};
