#pragma once


// CDMTestDialog dialog

class CDMTestDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMTestDialog)

public:
	CDMTestDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMTestDialog();

// Dialog Data
	enum { IDD = IDD_TEST_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
