#pragma once

#include "DMLoadFileDialog.h"
#include <afxbutton.h>	
#include "afxwin.h"

// CDMFileManagementDialog dialog

class CDMFileManagementDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMFileManagementDialog)

public:
	CDMFileManagementDialog(DMLoadFileDescriptor *_pFileDescriptor, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMFileManagementDialog();

	CDMHelperApp *m_pApp;

	DMLoadFileDescriptor *m_pFileDescriptor;
	cDNDCharacter	*m_pCharacter;
	CString m_szPath;
	CString m_szSubDirectory;

	CString m_szCutFilePath;
	CString m_szCutFileName;

	BOOL m_bTopIsUpDirectory;
	int m_nLastButtonSelected;

	Bitmap* m_pBackDropBitmap;
	Bitmap* m_pPortraitBitmap;

	CFont m_ListFont;
	CFont m_ListFontBold;

	CMFCButton *m_FileButtons[16];
	BOOL m_bIsDirectory[16];
	CString m_szDirectories[16];

	int m_nScrollOffset;
	int m_nOldScrollOffset;


	void Refresh();
	void SearchSubDirectory(CString szDirPath, CString szSearchString, BOOL bShowAll, int *pnItemsCount, int *pnIndex);

	void SelectedFile(int nSelected);
	BOOL LoadCharacterFromFile(char *szFileName);
	

// Dialog Data
	enum { IDD = IDD_DM_FILE_MANAGER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CMFCButton m_cFileButton_1;
	CMFCButton m_cFileButton_2;
	CMFCButton m_cFileButton_3;
	CMFCButton m_cFileButton_4;
	CMFCButton m_cFileButton_5;
	CMFCButton m_cFileButton_6;
	CMFCButton m_cFileButton_7;
	CMFCButton m_cFileButton_8;
	CMFCButton m_cFileButton_9;
	CMFCButton m_cFileButton_10;
	CMFCButton m_cFileButton_11;
	CMFCButton m_cFileButton_12;
	CMFCButton m_cFileButton_13;
	CMFCButton m_cFileButton_14;
	CMFCButton m_cFileButton_15;
	CMFCButton m_cFileButton_16;
	afx_msg void OnBnClickedFileButton1();
	afx_msg void OnBnClickedFileButton2();
	afx_msg void OnBnClickedFileButton3();
	afx_msg void OnBnClickedFileButton4();
	afx_msg void OnBnClickedFileButton5();
	afx_msg void OnBnClickedFileButton6();
	afx_msg void OnBnClickedFileButton7();
	afx_msg void OnBnClickedFileButton8();
	afx_msg void OnBnClickedFileButton9();
	afx_msg void OnBnClickedFileButton10();
	afx_msg void OnBnClickedFileButton11();
	afx_msg void OnBnClickedFileButton12();
	afx_msg void OnBnClickedFileButton13();
	afx_msg void OnBnClickedFileButton14();
	afx_msg void OnBnClickedFileButton15();
	afx_msg void OnBnClickedFileButton16();
	CScrollBar m_cFileScrollBar;
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CString m_szDebugString;
	CButton m_cOKButton;
	CString m_szFileNameEdit;
	afx_msg void OnBnClickedOk();
	CButton m_cNewFolderButton;
	afx_msg void OnBnClickedNewFolderButton();
	CStatic m_cDegbugText;
	CButton m_cCopyFileButton;
	CButton m_cPasteFileButton;
	CButton m_cDeleteFileButton;
	afx_msg void OnBnClickedCopyButton();
	afx_msg void OnBnClickedPasteButton();
	afx_msg void OnBnClickedDeleteButton();
	CButton m_cShowAllFileTypesCheck;
	afx_msg void OnBnClickedFiletypesCheck();
	CButton m_cDeleteFolderButton;
	afx_msg void OnBnClickedDeleteFolderButton();
	CStatic m_cGroupBox;
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	CString m_szSearchString;
	afx_msg void OnEnChangeSearchEdit();
	CEdit m_cFileNameEdit;
	CEdit m_cSearchEdit;
};
