#include "afxwin.h"
#if !defined(AFX_DMSFXEDITORDIALOG_H__9ECB8F3E_08DF_4C8D_8A31_FAF7E99DD821__INCLUDED_)
#define AFX_DMSFXEDITORDIALOG_H__9ECB8F3E_08DF_4C8D_8A31_FAF7E99DD821__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMSFXEditorDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DMSFXEditorDialog dialog

class DMSFXEditorDialog : public CDialog
{
// Construction
public:
	DMSFXEditorDialog(cDNDSoundEffect *_pSoundFX, CWnd* pParent = NULL);   // standard constructor

	CDMHelperApp *m_pApp;

	cDNDSoundEffect *m_pSoundFX;

	BOOL m_bPasted;

	BOOL StringContainsEvent(CString szEvent);
	void Refresh();
	BOOL FileExists(CString szPath);


// Dialog Data
	//{{AFX_DATA(DMSFXEditorDialog)
	enum { IDD = IDD_SFX_EDITOR_DIALOG };
	CString	m_szDesc;
	CString	m_szFilename;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DMSFXEditorDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DMSFXEditorDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnChooseFileButton();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CButton m_cCutSoundButton;
	CButton m_cCopySoundButton;
	CButton m_cPasteSoundButton;
	afx_msg void OnBnClickedCutSoundButton();
	afx_msg void OnBnClickedCopySoundButton();
	afx_msg void OnBnClickedPasteSoundButton();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedClearSoundButton();
	CButton m_cEncryptButton;
	afx_msg void OnBnClickedEncryptButton();
	CString m_szExplainText;
	CEdit m_cDescEdit;
	afx_msg void OnEnChangeDescEdit();
	afx_msg void OnEnChangeFilenameEdit();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DMSFXEDITORDIALOG_H__9ECB8F3E_08DF_4C8D_8A31_FAF7E99DD821__INCLUDED_)
