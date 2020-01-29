#pragma once


// cDMModifyThiefSkillsDialog dialog

class cDMModifyThiefSkillsDialog : public CDialog
{
	DECLARE_DYNAMIC(cDMModifyThiefSkillsDialog)

public:
	cDMModifyThiefSkillsDialog(int *pnThiefSkillModifiers, CWnd* pParent = NULL);   // standard constructor
	virtual ~cDMModifyThiefSkillsDialog();

	void Refresh();

	int m_nThiefSkillModifiers[8];
	int *m_pnThiefSkillModifiers;

// Dialog Data
	enum { IDD = IDD_THIEF_SKILL_MODIFIER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	CString m_szThiefSkillModifier[8];
	afx_msg void OnEnSetfocusThiefSkillEdit1();
	afx_msg void OnEnSetfocusThiefSkillEdit2();
	afx_msg void OnEnSetfocusThiefSkillEdit3();
	afx_msg void OnEnSetfocusThiefSkillEdit4();
	afx_msg void OnEnSetfocusThiefSkillEdit5();
	afx_msg void OnEnSetfocusThiefSkillEdit6();
	afx_msg void OnEnSetfocusThiefSkillEdit7();
	afx_msg void OnEnSetfocusThiefSkillEdit8();
};
