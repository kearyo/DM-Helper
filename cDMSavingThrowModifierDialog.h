#pragma once


// cDMSavingThrowModifierDialog dialog

class cDMSavingThrowModifierDialog : public CDialog
{
	DECLARE_DYNAMIC(cDMSavingThrowModifierDialog)

public:
	cDMSavingThrowModifierDialog(int *pnSavingThrowModifiers, CWnd* pParent = NULL);   // standard constructor
	virtual ~cDMSavingThrowModifierDialog();

	int *m_pnSavingThrowModifiers;
	int m_nSavingThrowModifiers[5]; // local copy

	void Refresh();

// Dialog Data
	enum { IDD = IDD_SAVING_THROW_MOD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CString m_szSaveMod[5];
	afx_msg void OnEnSetfocusSaveEdit1();
	afx_msg void OnEnSetfocusSaveEdit2();
	afx_msg void OnEnSetfocusSaveEdit3();
	afx_msg void OnEnSetfocusSaveEdit4();
	afx_msg void OnEnSetfocusSaveEdit5();
};
