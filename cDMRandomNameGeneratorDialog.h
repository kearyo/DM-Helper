#pragma once
#include "afxwin.h"


// cDMRandomNameGeneratorDialog dialog

class cDMRandomNameGeneratorDialog : public CDialogEx
{
	DECLARE_DYNAMIC(cDMRandomNameGeneratorDialog)

public:
	cDMRandomNameGeneratorDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~cDMRandomNameGeneratorDialog();

	CDMHelperApp *m_pApp;
	DND_CHARACTER_RACES GetMonsterRaceFromName(CString szName);
	void GenerateName(DND_CHARACTER_RACES nRace);

// Dialog Data
	enum { IDD = IDD_RANDOM_NAME_GENERATOR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedHumanButton();
	CString m_szGeneratedName;
	CEdit m_GeneratedNameEdit;
	virtual BOOL OnInitDialog();
	BOOL m_bMaleCheck;
	BOOL m_bFemaleCheck;
	afx_msg void OnBnClickedMaleCheck();
	afx_msg void OnBnClickedFemaleCheck();
	afx_msg void OnBnClickedElvenButton();
	afx_msg void OnBnClickedDwarvenButton();
	afx_msg void OnBnClickedHalflingButton();
	afx_msg void OnBnClickedGnomeButton();
	afx_msg void OnBnClickedBugbearButton();
	afx_msg void OnBnClickedDragonButton();
	afx_msg void OnBnClickedGoblinButton();
	afx_msg void OnBnClickedHobgoblinButton();
	afx_msg void OnBnClickedKoboldButton();
	afx_msg void OnBnClickedOgreButton();
	afx_msg void OnBnClickedOrcButton();
	afx_msg void OnBnClickedOk();
	CString m_szCopiedNote;
};
