#pragma once


// cDMCharacterPortraitDialog dialog

class cDMCharacterPortraitDialog : public CDialogEx
{
	DECLARE_DYNAMIC(cDMCharacterPortraitDialog)

public:
	cDMCharacterPortraitDialog(Bitmap* pPortraitBitmap, CWnd* pParent = NULL);   // standard constructor
	virtual ~cDMCharacterPortraitDialog();

	Bitmap* m_pPortraitBitmap;

// Dialog Data
	enum { IDD = IDD_CHARACTER_PORTRAIT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};
