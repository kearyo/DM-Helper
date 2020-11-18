#pragma once


// cDMOpenLockDialog dialog

class cDMOpenLockDialog : public CDialogEx
{
	DECLARE_DYNAMIC(cDMOpenLockDialog)

public:
	cDMOpenLockDialog(cDNDParty *pParty, cDNDCharacter	*pCharacter, CWnd* pParent = NULL);   // standard constructor
	virtual ~cDMOpenLockDialog();

	CDMHelperApp *m_pApp;
	cDNDParty *m_pParty;
	cDNDCharacter	*m_pCharacter;
	int m_nAttemptRounds;

// Dialog Data
	enum { IDD = IDD_PICK_LOCK_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CString m_szTimeComment;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedFailed();
};
