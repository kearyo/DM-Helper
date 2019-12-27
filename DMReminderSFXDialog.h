#pragma once


// CDMReminderSFXDialog dialog

class CDMReminderSFXDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMReminderSFXDialog)

public:
	CDMReminderSFXDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMReminderSFXDialog();

	CDMHelperApp *m_pApp;
	CWnd* m_pParent;

// Dialog Data
	enum { IDD = IDD_DM_REMINDER_SFX_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	virtual void PostNcDestroy();
};
