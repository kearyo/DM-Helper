#pragma once


// cDMModifyAssassinSkillsDialog dialog

class cDMModifyAssassinSkillsDialog : public CDialog
{
	DECLARE_DYNAMIC(cDMModifyAssassinSkillsDialog)

public:
	cDMModifyAssassinSkillsDialog(int *pnAssassinSkillModifiers, CWnd* pParent = NULL);   // standard constructor
	virtual ~cDMModifyAssassinSkillsDialog();

	void Refresh();

	int m_nAssassinSkillModifiers[10];
	int *m_pnAssassinSkillModifiers;

// Dialog Data
	enum { IDD = IDD_ASSASSIN_SKILL_MODIFIER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CString m_szAssassinSkillModifier[10];
	afx_msg void OnEnSetfocusAssSkillEdit1();
	afx_msg void OnEnSetfocusAssSkillEdit2();
	afx_msg void OnEnSetfocusAssSkillEdit3();
	afx_msg void OnEnSetfocusAssSkillEdit4();
	afx_msg void OnEnSetfocusAssSkillEdit5();
	afx_msg void OnEnSetfocusAssSkillEdit6();
	afx_msg void OnEnSetfocusAssSkillEdit7();
	afx_msg void OnEnSetfocusAssSkillEdit8();
	afx_msg void OnEnSetfocusAssSkillEdit9();
	afx_msg void OnEnSetfocusAssSkillEdit10();
};
