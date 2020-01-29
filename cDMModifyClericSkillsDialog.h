#pragma once


// cDMModifyClericSkillsDialog dialog

class cDMModifyClericSkillsDialog : public CDialog
{
	DECLARE_DYNAMIC(cDMModifyClericSkillsDialog)

public:
	cDMModifyClericSkillsDialog(int *pnClericTurnModifiers, CWnd* pParent = NULL);   // standard constructor
	virtual ~cDMModifyClericSkillsDialog();

	void Refresh();

	int m_nClericTurnModifiers[13];
	int *m_pnClericTurnModifiers;

// Dialog Data
	enum { IDD = IDD_CLERIC_SKILL_MODIFIER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CString m_szTurnUndeadEdit[13];
	afx_msg void OnEnSetfocusTurnUndeadEdit();
};
