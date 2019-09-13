#pragma once

// CDMUnitTestDialog dialog

class CDMUnitTestDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CDMUnitTestDialog)

public:
	CDMUnitTestDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMUnitTestDialog();

// Dialog Data
	enum { IDD = IDD_DM_UNIT_TEST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
