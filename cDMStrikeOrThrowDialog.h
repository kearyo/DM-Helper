#pragma once


// cDMStrikeOrThrowDialog dialog

class cDMStrikeOrThrowDialog : public CDialogEx
{
	DECLARE_DYNAMIC(cDMStrikeOrThrowDialog)

public:
	cDMStrikeOrThrowDialog(BOOL *pbThowWeapon, CWnd* pParent = NULL);   // standard constructor
	virtual ~cDMStrikeOrThrowDialog();

	BOOL *m_pbThowWeapon;

// Dialog Data
	enum { IDD = IDD_STRIKEORTHROW_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};
