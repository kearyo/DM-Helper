#if !defined(AFX_DMSOUNDFXDIALOG_H__6E227536_5E7D_4177_8571_9AC27B41008B__INCLUDED_)
#define AFX_DMSOUNDFXDIALOG_H__6E227536_5E7D_4177_8571_9AC27B41008B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMSoundFXDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDMSoundFXDialog dialog

class CDMSoundFXDialog : public CDialog
{
// Construction
public:
	CDMSoundFXDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDMSoundFXDialog)
	enum { IDD = IDD_SOUND_FX_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDMSoundFXDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDMSoundFXDialog)
	afx_msg void OnSoundButton1();
	virtual BOOL OnInitDialog();
	afx_msg void OnSoundButton2();
	afx_msg void OnSoundButton3();
	afx_msg void OnSoundButton4();
	afx_msg void OnSoundButton5();
	afx_msg void OnSoundButton6();
	afx_msg void OnSoundButton7();
	afx_msg void OnSoundButton8();
	afx_msg void OnSoundButton9();
	afx_msg void OnSoundButton10();
	afx_msg void OnSoundButton11();
	afx_msg void OnSoundButton12();
	afx_msg void OnSoundButton13();
	afx_msg void OnSoundButton14();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DMSOUNDFXDIALOG_H__6E227536_5E7D_4177_8571_9AC27B41008B__INCLUDED_)
