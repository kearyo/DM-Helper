#pragma once


// cDMMundaneTypeSelectorDialog dialog

class cDMMundaneTypeSelectorDialog : public CDialog
{
	DECLARE_DYNAMIC(cDMMundaneTypeSelectorDialog)

public:
	cDMMundaneTypeSelectorDialog(PMAGICTABLEITEM pMagicItem, int *pnSelected, int *pnMagicAdjustment, int *pnAmount, CWnd* pParent = NULL);   // standard constructor
	virtual ~cDMMundaneTypeSelectorDialog();

	PMAGICTABLEITEM m_pMagicItem;
	int *m_pnSelected;
	int *m_pnMagicAdjustment;
	int *m_pnAmount;

// Dialog Data
	enum { IDD = IDD_MUNDANE_TYPE_SELECTOR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedTypeRadio1();
	afx_msg void OnBnClickedTypeRadio2();
	afx_msg void OnBnClickedTypeRadio3();
	afx_msg void OnBnClickedTypeRadio4();
	afx_msg void OnBnClickedTypeRadio5();
	afx_msg void OnBnClickedTypeRadio6();
	afx_msg void OnBnClickedTypeRadio7();
	afx_msg void OnBnClickedTypeRadio8();
	afx_msg void OnBnClickedTypeRadio9();
	afx_msg void OnBnClickedTypeRadio10();
	CString m_szAmountEdit;
	afx_msg void OnBnClickedRollButton();
};
