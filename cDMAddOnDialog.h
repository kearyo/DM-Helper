#pragma once
#include "afxcmn.h"
#include "afxwin.h"


typedef enum
{
	DND_ADD_ON_MODES_INIT = 0,

	DND_ADD_ON_MODES_GOT_DESC,

	DND_ADD_ON_MODES_SELECT_ADD_ON,

	DND_ADD_ON_MODES_DOWNLOAD_ADD_ON,

	DND_ADD_ON_MODES_INSTALL_ADD_ON,

	DND_ADD_ON_MODES_DO_NOTHING,
	
} DND_ADD_ON_MODES;


class cDMUpdateFile
{

public:

	CString m_szFileName;
	CString m_szDesc;
	ULONG m_lSize;
	CString m_szVersion;
	BOOL m_bDownloading;
	BOOL m_bDownloaded;
	LPVOID m_pWindow;

	cDMUpdateFile()
	{
		m_szFileName = _T("");
		m_szDesc = _T("");
		m_lSize = 0L;
		m_szVersion = _T("");
		m_bDownloading = FALSE;
		m_bDownloaded = FALSE;
		m_pWindow = NULL;
	};

};

#define PUPDATEFILE cDMUpdateFile*
typedef CTypedPtrMap <CMapWordToPtr, WORD, PUPDATEFILE> PUPDATEFILEMAP;

class cDMAddOnDesc
{
public:

	CString m_szAddOnID;
	CString m_szAddOnDesc;
	CString m_szFileName;
	CString m_szRestartApp;


	cDMAddOnDesc()
	{
		m_szAddOnID = _T("");
		m_szAddOnDesc = _T("");
		m_szFileName = _T("");
		m_szRestartApp = _T("");
	};

};

#define PADDONDESC cDMAddOnDesc*
typedef CTypedPtrMap <CMapWordToPtr, WORD, PADDONDESC> PADDONDESCMAP;

// cDMAddOnDialog dialog

class cDMAddOnDialog : public CDialogEx
{
	DECLARE_DYNAMIC(cDMAddOnDialog)

	CDMHelperApp *m_pApp;
	CURL *m_pCurl;

	DND_ADD_ON_MODES m_AddOnMode;

	CWinThread *m_pFileDownloadThread;

	BOOL m_bAbortTransfer;
	BOOL m_bDownloading;

	int m_nFileNumber;
	int m_nTotalFiles;

	PADDONDESCMAP m_AddOnMap;	

	cDMAddOnDesc *m_pSelectedAddOn;

	CString m_szInstalledAddOnsHash;

public:
	cDMAddOnDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~cDMAddOnDialog();

	FILE *OpenDirectoryFile(CString szFilePath, char *szFileCommand);
	void Update();
	void  LoadAddOnDescs();
	DND_ADD_ON_MODES InstallAddOn();

// Dialog Data
	enum { IDD = IDD_ADD_ON_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedGoButton();
	CProgressCtrl m_cFileProgress;
	CString m_szDownloadingFile;
	CListBox m_cAddOnsList;
	afx_msg LRESULT OnDNDMessage(UINT wParam, LONG lParam);
	CButton m_cGOButton;
	CString m_szFileSize;
	CString m_szFilePerc;
	afx_msg void OnBnClickedShowInstalledCheck();
	BOOL m_bShowInstalled;
};
