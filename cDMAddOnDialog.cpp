// cDMAddOnDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "cDMAddOnDialog.h"
#include "afxdialogex.h"
#include "unzip.h"

// USES:
// https://www.codeproject.com/Articles/7530/Zip-Utils-Clean-Elegant-Simple-Cplusplus-Win


size_t CurlWriteFunc(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	return fwrite(ptr, size, nmemb, stream);
}

size_t CurlReadFunc(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	return fread(ptr, size, nmemb, stream);
}

int CurlProgressFunc(cDMAddOnDialog *pDlg,
	double t, /* dltotal */
	double d, /* dlnow */
	double ultotal,
	double ulnow)
{
	if (pDlg != NULL)
	{
		if (pDlg->m_AddOnMode != DND_ADD_ON_MODES_INIT)
		{
			pDlg->m_cFileProgress.SetPos((int)(d*100.0 / t));
			pDlg->m_szFileSize.Format("%u of %d bytes", (int)d, (int)t);

			double fPerc = d / t;
			pDlg->m_szFilePerc.Format("%03d %c", (int)(fPerc * 100), 37);

			pDlg->PostMessageA(DND_WM_MESSAGE, 1, 0);
		}

		if (pDlg->m_bAbortTransfer == TRUE)
		{
			return 1;
		}
	}

	return 0;
}


UINT DMDownloadFileThreadProc(LPVOID pData)
{
	//m_pApp = (CDMHelperApp *)AfxGetApp();

	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	cDMUpdateFile *pFileData = (cDMUpdateFile*)pData;

	cDMAddOnDialog *pDlg = (cDMAddOnDialog *)pFileData->m_pWindow;
	pDlg->m_bDownloading = TRUE;

	if (pDlg != NULL && pDlg->m_AddOnMode != DND_ADD_ON_MODES_INIT)
	{
		pDlg->m_szDownloadingFile.Format("Downloading Add-On: %s", pFileData->m_szDesc);
		pDlg->PostMessageA(DND_WM_MESSAGE, 0, 0);
	}

	CURL *pCurl = pDlg->m_pCurl;

	pFileData->m_bDownloaded = FALSE;

	int nCount = 0;


	if (pCurl == NULL)
	{
		pCurl = curl_easy_init();
	}

	if (pCurl != NULL)
	{
		CURLcode res;

		CString szTemp;
		szTemp.Format("%sAddOns\\%s", pApp->m_szEXEPath, pFileData->m_szFileName); // all downloaded files go in the updates folder
		
		szTemp.MakeUpper();

		//FILE *pOutFile = fopen(szTemp, "wb");
		FILE *pOutFile = pDlg->OpenDirectoryFile(szTemp, "wb");

		int nError = GetLastError();

		if (pOutFile != NULL)
		{
			CString szURL;
			szURL.Format("%s/%s/%s", pApp->m_UpdateParams.m_szUpdateURL, "AddOns", pFileData->m_szFileName);
			szURL.Replace(" ", "%20");
			szURL.Replace("\\", "/");  // so Linux doesn't take offense to it

			char url[MAX_PATH];
			memset(url, 0, MAX_PATH * sizeof(char));
			strcpy(url, szURL.GetBuffer(0));

			curl_easy_setopt(pCurl, CURLOPT_URL, szURL.GetBuffer(0));
			curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, pOutFile);
			curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, CurlWriteFunc);
			curl_easy_setopt(pCurl, CURLOPT_READFUNCTION, CurlReadFunc);
			curl_easy_setopt(pCurl, CURLOPT_NOPROGRESS, 0L);
			curl_easy_setopt(pCurl, CURLOPT_PROGRESSFUNCTION, CurlProgressFunc);
			curl_easy_setopt(pCurl, CURLOPT_PROGRESSDATA, pDlg);

			res = curl_easy_perform(pCurl);

			if (res == CURLE_OK)
			{
				long respcode = 0L; //response code of the http transaction

				curl_easy_getinfo(pCurl, CURLINFO_RESPONSE_CODE, &respcode);// grabbing it from curl

				if (respcode == 200)
				{
					pFileData->m_bDownloaded = TRUE;
				}
				else
				{
					CString szError;
					szError.Format("CRITICAL ERROR: Unable to retrieve %s ! (Error %ld)", pFileData->m_szFileName, respcode);
					AfxMessageBox(szError, MB_OK);
					pDlg->m_bAbortTransfer = TRUE;
				}
			}

			fclose(pOutFile);
		}
		else
		{
			CString szError;
			szError.Format("CRITICAL ERROR: Unable to open %s for writing !", pFileData->m_szFileName);
			AfxMessageBox(szError, MB_OK);
			pDlg->m_bAbortTransfer = TRUE;
		}

		/* always cleanup */
		curl_easy_cleanup(pCurl);
	}

	pCurl = NULL;

	pFileData->m_bDownloading = FALSE;
	//pApp->m_pFileDownloadThread = NULL;

	delete pFileData;

	pDlg->m_bDownloading = FALSE;
	pDlg->m_szFileSize = _T("");

	if (pDlg->m_bAbortTransfer == FALSE)
	{
		switch (pDlg->m_AddOnMode)
		{
			case DND_ADD_ON_MODES_INIT:				pDlg->m_AddOnMode = DND_ADD_ON_MODES_GOT_DESC; break;
			case DND_ADD_ON_MODES_DOWNLOAD_ADD_ON:  pDlg->m_AddOnMode = DND_ADD_ON_MODES_INSTALL_ADD_ON; break;
		}

		pDlg->Update();
	}

	if (pDlg->m_bAbortTransfer == TRUE)
	{
		pDlg->PostMessage(WM_CLOSE);
	}

	return 0;
}


// cDMAddOnDialog dialog

IMPLEMENT_DYNAMIC(cDMAddOnDialog, CDialogEx)

cDMAddOnDialog::cDMAddOnDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(cDMAddOnDialog::IDD, pParent)
	, m_szDownloadingFile(_T(""))
	, m_szFileSize(_T(""))
	, m_szFilePerc(_T(""))
	, m_bShowInstalled(FALSE)
{
	m_pApp = (CDMHelperApp *)AfxGetApp();
	m_pCurl = NULL;

	m_AddOnMode = DND_ADD_ON_MODES_INIT;

	m_pSelectedAddOn = NULL;
	m_szInstalledAddOnsHash = _T("");
	m_pFileDownloadThread = NULL;

	m_bAbortTransfer = FALSE;
	m_bDownloading = FALSE;

	m_nFileNumber = 0;
	m_nTotalFiles = 0;
}

cDMAddOnDialog::~cDMAddOnDialog()
{
}

void cDMAddOnDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADD_ON_PROGRESS, m_cFileProgress);
	DDX_Text(pDX, IDC_FILE_NAME_STATIC, m_szDownloadingFile);
	DDX_Control(pDX, IDC_ADD_ONS_LIST, m_cAddOnsList);
	DDX_Control(pDX, IDC_GO_BUTTON, m_cGOButton);
	DDX_Text(pDX, IDC_FILE_SIZE_STATIC, m_szFileSize);
	DDX_Text(pDX, IDC_FILE_PERC_STATIC, m_szFilePerc);
	DDX_Check(pDX, IDC_SHOW_INSTALLED_CHECK, m_bShowInstalled);
}


BEGIN_MESSAGE_MAP(cDMAddOnDialog, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &cDMAddOnDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_GO_BUTTON, &cDMAddOnDialog::OnBnClickedGoButton)
	ON_MESSAGE(DND_WM_MESSAGE, OnDNDMessage)
	ON_BN_CLICKED(IDC_SHOW_INSTALLED_CHECK, &cDMAddOnDialog::OnBnClickedShowInstalledCheck)
END_MESSAGE_MAP()


// cDMAddOnDialog message handlers


void cDMAddOnDialog::OnBnClickedCancel()
{
	if (m_bDownloading)
	{
		return;
	}

	WORD wID;
	for (POSITION pos = m_AddOnMap.GetStartPosition(); pos != NULL;)
	{
		cDMAddOnDesc *pAddOn = NULL;
		m_AddOnMap.GetNextAssoc(pos, wID, pAddOn);

		if (pAddOn != NULL)
		{
			delete pAddOn;
		}
	}

	m_AddOnMap.RemoveAll();

	CDialogEx::OnCancel();
}


BOOL cDMAddOnDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString szTemp = m_pApp->m_szEXEPath + "AddOns";
	CreateDirectory(szTemp, NULL);

	m_cFileProgress.SetRange(0, 100);

	m_cGOButton.EnableWindow(FALSE);

	Update();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void cDMAddOnDialog::Update()
{
	switch (m_AddOnMode)
	{
		case DND_ADD_ON_MODES_INIT:
		{
			m_cGOButton.EnableWindow(FALSE);
			m_cAddOnsList.EnableWindow(FALSE);

			m_cFileProgress.SetPos(0);
			m_szDownloadingFile = _T("");
			m_szFileSize = _T("");
			m_szFilePerc = _T("");

			cDMUpdateFile *pFile = new cDMUpdateFile();

			pFile->m_bDownloading = TRUE;
			pFile->m_szFileName = "DMAddOns.dat";
			pFile->m_lSize = 0;
			pFile->m_pWindow = this;

			m_pFileDownloadThread = AfxBeginThread(DMDownloadFileThreadProc, (LPVOID)pFile);

			m_cGOButton.SetWindowText("Install Add On");

			PostMessage(DND_WM_MESSAGE, 0, 0);

			break;
		}
		case DND_ADD_ON_MODES_GOT_DESC:
		{
			LoadAddOnDescs();

			m_AddOnMode = DND_ADD_ON_MODES_SELECT_ADD_ON;

			break;
		}
		case DND_ADD_ON_MODES_DOWNLOAD_ADD_ON:
		{
			cDMUpdateFile *pFile = new cDMUpdateFile();

			m_cGOButton.SetWindowText("Cancel Download");

			pFile->m_bDownloading = TRUE;
			pFile->m_szFileName = m_pSelectedAddOn->m_szFileName;
			pFile->m_szDesc = m_pSelectedAddOn->m_szAddOnDesc;
			pFile->m_lSize = 0;
			pFile->m_pWindow = this;

			m_pFileDownloadThread = AfxBeginThread(DMDownloadFileThreadProc, (LPVOID)pFile);

			break;
		}

		case DND_ADD_ON_MODES_INSTALL_ADD_ON:
		{
			m_AddOnMode = InstallAddOn();

			Update();

			break;
		}
	}
}

void cDMAddOnDialog::LoadAddOnDescs()
{
	m_pSelectedAddOn = NULL;

	WORD wID;
	for (POSITION pos = m_AddOnMap.GetStartPosition(); pos != NULL;)
	{
		cDMAddOnDesc *pAddOn = NULL;
		m_AddOnMap.GetNextAssoc(pos, wID, pAddOn);

		if (pAddOn != NULL)
		{
			delete pAddOn;
		}
	}

	m_AddOnMap.RemoveAll();

	m_cAddOnsList.ResetContent();

	CString szFileName = m_pApp->m_szEXEPath + "AddOns//DMAddOnsInstalled.dat";
	char szInLine[256];

	FILE *pInFile = fopen(szFileName, "rt");

	if (pInFile != NULL)
	{
		while (!feof(pInFile))
		{
			fscanf(pInFile, "%s", szInLine);
			CString szTemp = szInLine;
			szTemp += ",";

			m_szInstalledAddOnsHash += szTemp;
		}

		fclose(pInFile);
	}

	/////////////////////////////////

	szFileName = m_pApp->m_szEXEPath + "AddOns//DMAddOns.dat";

	
	pInFile = fopen(szFileName, "rt");

	int nCount = 0;
	if (pInFile != NULL)
	{
		while (!feof(pInFile))
		{
			fgets(szInLine, 255, pInFile);
			CString szTemp = szInLine;

			memset(szInLine, 0, 256 * sizeof(char));

			if (szTemp.Find("#") == 0)
				continue;

			szTemp.Replace(", ", ",");
			szTemp.Replace(" ,", ",");

			szTemp.Replace("\n", "");
			szTemp.Replace("\r", "");
			szTemp.TrimRight();

			cDMAddOnDesc *pAddOn = new cDMAddOnDesc();
			BOOL bValid = FALSE;

			CString sToken = _T("");
			int i = 0; // substring index to extract
			while (AfxExtractSubString(sToken, szTemp, i, ','))
			{
				switch (i)
				{
					case 0: pAddOn->m_szAddOnID = sToken; break;
					case 1: pAddOn->m_szAddOnDesc = sToken; break;
					case 2: pAddOn->m_szFileName = sToken;  break;
					case 3: pAddOn->m_szRestartApp = sToken;  bValid = TRUE; break;
				}

				++i;
			}

			if (bValid && (m_szInstalledAddOnsHash.Find(pAddOn->m_szAddOnID) == -1 || m_bShowInstalled))
			{
				m_AddOnMap.SetAt(nCount++, pAddOn);
			}
			else
			{
				delete pAddOn;
			}

		}

		fclose(pInFile);
	}

	nCount = 0;
	for (POSITION pos = m_AddOnMap.GetStartPosition(); pos != NULL;)
	{
		cDMAddOnDesc *pAddOn = NULL;
		m_AddOnMap.GetNextAssoc(pos, wID, pAddOn);

		if (pAddOn != NULL)
		{
			m_cAddOnsList.InsertString(nCount, pAddOn->m_szAddOnDesc);
			m_cAddOnsList.SetItemData(nCount, (DWORD_PTR)pAddOn);
			++nCount;

			m_cGOButton.EnableWindow(TRUE);
			m_cAddOnsList.EnableWindow(TRUE);
		}
	}

	if (nCount == 0)
	{
		m_cAddOnsList.InsertString(nCount, "No Uninstalled Add Ons Are Available");
	}

	InvalidateRect(NULL);
}

void cDMAddOnDialog::OnBnClickedGoButton()
{

	/*
	TCHAR szDestName[260];

	HZIP hz = OpenZip("D:\\DungeonMaestroAddOns\\Flanaess_498\\Flanaess_498.zip", 0);
	ZIPENTRY ze; GetZipItem(hz, -1, &ze); int numitems = ze.index;
	// -1 gives overall information about the zipfile
	for (int zi = 0; zi<numitems; zi++)
	{
		ZIPENTRY ze; GetZipItem(hz, zi, &ze); // fetch individual details

		sprintf(szDestName, "D:\\ziptest\\unzipped\\ANNA\\%s", ze.name);

		UnzipItem(hz, zi, szDestName);         // e.g. the item's name.
	}

	CloseZip(hz);

	*/

	if (m_bDownloading == FALSE)
	{
		UpdateData(TRUE);

		int nCursor = m_cAddOnsList.GetCurSel();

		if (nCursor > -1)
		{
			m_pSelectedAddOn = (cDMAddOnDesc *)m_cAddOnsList.GetItemData(nCursor);

			if (m_pSelectedAddOn != NULL)
			{
				m_AddOnMode = DND_ADD_ON_MODES_DOWNLOAD_ADD_ON;
				Update();
			}
		}
	}
	else
	{
		m_bAbortTransfer = TRUE;
	}

}

DND_ADD_ON_MODES cDMAddOnDialog::InstallAddOn()
{

	CString szTemp;
	szTemp.Format("%sAddOns\\%s", m_pApp->m_szEXEPath, m_pSelectedAddOn->m_szFileName);

	TCHAR szDestName[260];

	HZIP hz = OpenZip(szTemp.GetBuffer(0), 0);
	ZIPENTRY ze; GetZipItem(hz, -1, &ze); int numitems = ze.index;
	// -1 gives overall information about the zipfile
	for (int zi = 0; zi<numitems; zi++)
	{
		ZIPENTRY ze; GetZipItem(hz, zi, &ze); // fetch individual details

		sprintf(szDestName, "%s\\%s", m_pApp->m_szEXEPath, ze.name);

		UnzipItem(hz, zi, szDestName);         // e.g. the item's name.

		float fPerc = ((float)zi / (float)numitems) * 100.0f;
		m_cFileProgress.SetPos((int)fPerc);

		m_szDownloadingFile.Format("Installing : %s", ze.name);
		PostMessageA(DND_WM_MESSAGE, 0, 0);
	}

	CloseZip(hz);

	szTemp = m_pApp->m_szEXEPath + "AddOns//DMAddOnsInstalled.dat";
	FILE *pOutFile = fopen(szTemp, "at");

	if (pOutFile != NULL)
	{
		fprintf(pOutFile, "%s\n", m_pSelectedAddOn->m_szAddOnID);
		fclose(pOutFile);
	}

	m_szDownloadingFile = "Add On Installed !";
	PostMessageA(DND_WM_MESSAGE, 0, 0);

	if (m_pSelectedAddOn->m_szRestartApp.Find("YES") != -1)
	{
		if (AfxMessageBox("This add on requires that the application be exited to finish installation\nExit the application now ?", MB_YESNO) == IDYES)
		{
			PostMessage(WM_CLOSE);
			m_pApp->m_pMainWnd->PostMessage(WM_CLOSE);

			return DND_ADD_ON_MODES_DO_NOTHING;
		}
	}

	return DND_ADD_ON_MODES_INIT;

}

LRESULT cDMAddOnDialog::OnDNDMessage(UINT wParam, LONG lParam)
{
	UpdateData(FALSE);

	if (wParam == 0)
	{
		InvalidateRect(NULL);
	}

	return 0;
}

FILE * cDMAddOnDialog::OpenDirectoryFile(CString szFilePath, char *szFileCommand)
{
	FILE *pFile = fopen(szFilePath.GetBuffer(0), szFileCommand);

	if (pFile != NULL)
	{
		return pFile;
	}

	//we need to create a directory tree in here
	CString szPath = szFilePath;
	szPath.Replace(m_pApp->m_szEXEPath, "");

	char pC = 92;  //this is a backslash
	int nSpot = szPath.ReverseFind(pC);


	if (nSpot >= 0)
	{
		CString szSubPath = szPath.Left(nSpot);
		CString szDirPath;

		szDirPath.Format("%s%s", m_pApp->m_szEXEPath, szSubPath);

		SHCreateDirectoryEx(NULL, (LPCTSTR)szDirPath.GetBuffer(0), NULL);

		pFile = fopen(szFilePath.GetBuffer(0), szFileCommand);
	}

	return pFile;
}


void cDMAddOnDialog::OnBnClickedShowInstalledCheck()
{
	UpdateData(TRUE);

	m_AddOnMode = DND_ADD_ON_MODES_INIT;

	Update();
}
