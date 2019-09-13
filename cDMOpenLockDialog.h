#pragma once


// cDMOpenLockDialog dialog

class cDMOpenLockDialog : public CDialogEx
{
	DECLARE_DYNAMIC(cDMOpenLockDialog)

public:
	cDMOpenLockDialog(cDNDCharacter	*pCharacter, CWnd* pParent = NULL);   // standard constructor
	virtual ~cDMOpenLockDialog();

	CDMHelperApp *m_pApp;
	cDNDCharacter	*m_pCharacter;

// Dialog Data
	enum { IDD = IDD_PICK_LOCK_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
