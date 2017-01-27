#if !defined(AFX_DMLOADFILEDIALOG_H__E67BDF27_2CAC_4260_BC50_DB440D0F6103__INCLUDED_)
#define AFX_DMLOADFILEDIALOG_H__E67BDF27_2CAC_4260_BC50_DB440D0F6103__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMLoadFileDialog.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// DMLoadFileDialog dialog

class DMLoadFileDialog : public CDialog
{
// Construction
public:
	DMLoadFileDialog(DMLoadFileDescriptor *_pFileDescriptor, CWnd* pParent = NULL);   // standard constructor

	CString m_szPath;

	DMLoadFileDescriptor *m_pFileDescriptor;

	void LoadFileList();
	void ReadFileName();

// Dialog Data
	//{{AFX_DATA(DMLoadFileDialog)
	enum { IDD = IDD_LOAD_FILE_DIALOG };
	CButton	m_cOKButton;
	CEdit	m_cFileNameEdit;
	CListBox	m_cFileLister;
	CString	m_szFileNameEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DMLoadFileDialog)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DMLoadFileDialog)
	virtual void OnOK();
	afx_msg void OnDelete();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkFileList();
	afx_msg void OnSetfocusFileList();
	afx_msg void OnSelchangeFileList();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSelcancelFileList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DMLOADFILEDIALOG_H__E67BDF27_2CAC_4260_BC50_DB440D0F6103__INCLUDED_)
