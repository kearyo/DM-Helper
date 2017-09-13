#pragma once


// cDMDetachedMapDialog dialog

class cDMDetachedMapDialog : public CDialog
{
	DECLARE_DYNAMIC(cDMDetachedMapDialog)

public:
	cDMDetachedMapDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~cDMDetachedMapDialog();

// Dialog Data
	enum { IDD = IDD_DETACHED_MAP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
