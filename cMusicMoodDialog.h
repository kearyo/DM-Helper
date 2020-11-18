#pragma once


// cMusicMoodDialog dialog

class cMusicMoodDialog : public CDialog
{
	DECLARE_DYNAMIC(cMusicMoodDialog)

public:
	cMusicMoodDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~cMusicMoodDialog();

	CDMHelperApp *m_pApp;

// Dialog Data
	enum { IDD = IDD_MUSIC_MOOD_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedMusicMoodButton1();
	afx_msg void OnBnClickedMusicMoodButton2();
	afx_msg void OnBnClickedMusicMoodButton3();
	afx_msg void OnBnClickedMusicMoodButton4();
	afx_msg void OnBnClickedMusicMoodButton5();
};
