#pragma once
#include "afxwin.h"


// CDMDiceRollerDialog dialog

class CDMDiceRollerDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMDiceRollerDialog)

public:
	CDMDiceRollerDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMDiceRollerDialog();

	CDMHelperDlg *m_pDMHelperDlg;

	void RollDice(int nDieType);

// Dialog Data
	enum { IDD = IDD_DICE_ROLLER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void PostNcDestroy();
public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CButton m_cD20Button;
	CString m_szNumDiceEdit;
	afx_msg void OnBnClickedD20Button();
	CString m_szResultStatic;
	afx_msg void OnBnClickedD12Button();
	afx_msg void OnBnClickedD10Button();
	afx_msg void OnBnClickedD6Button();
	afx_msg void OnBnClickedD4Button2();
	afx_msg void OnBnClickedD100Button();
	CButton m_cD12Button;
	CButton m_cD10Button;
	CButton m_cD6Button;
	CButton m_cD4Button;
	CButton m_cD100Button;
	CButton m_cD8Button;
	afx_msg void OnBnClickedD8Button();
	CButton m_cDXButton;
	afx_msg void OnBnClickedDxButton();
};
