// DMFileManagementDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMFileManagementDialog.h"
#include "DMInputDialog.h"
#include "afxdialogex.h"
#include <afxbutton.h>	


// CDMFileManagementDialog dialog

IMPLEMENT_DYNAMIC(CDMFileManagementDialog, CDialog)

CDMFileManagementDialog::CDMFileManagementDialog(DMLoadFileDescriptor *_pFileDescriptor, CWnd* pParent /*=NULL*/)
	: CDialog(CDMFileManagementDialog::IDD, pParent)
	, m_szDebugString(_T(""))
	, m_szFileNameEdit(_T(""))
{
	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_pFileDescriptor = _pFileDescriptor;

	m_pFileDescriptor->m_szInitialPath.MakeLower();
	m_pFileDescriptor->m_szSubDirectory.MakeLower();
	m_pFileDescriptor->m_szInitialFileName.MakeLower();
	m_pFileDescriptor->m_szFileTypes.MakeLower();
	m_pFileDescriptor->m_szReturnedPath.MakeLower();

	m_szPath = m_pFileDescriptor->m_szInitialPath + m_pFileDescriptor->m_szSubDirectory;
	m_szSubDirectory = _T("");

	m_szCutFilePath = _T("");
	m_szCutFileName = _T("");

	m_szDebugString = m_szPath;

	m_pCharacter = NULL;
	m_pBackDropBitmap = NULL;
	m_pPortraitBitmap = NULL;

	m_nScrollOffset = 0;
	m_nOldScrollOffset = 0;

	memset(m_bIsDirectory, 0, 16 * sizeof(BOOL));
}

CDMFileManagementDialog::~CDMFileManagementDialog()
{
	if (m_pCharacter != NULL)
	{
		delete m_pCharacter;
	}
	if (m_pBackDropBitmap != NULL)
	{
		delete m_pBackDropBitmap;
	}
	if (m_pPortraitBitmap != NULL)
	{
		delete m_pPortraitBitmap;
	}
}

void CDMFileManagementDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILE_BUTTON_1, m_cFileButton_1);
	DDX_Control(pDX, IDC_FILE_BUTTON_2, m_cFileButton_2);
	DDX_Control(pDX, IDC_FILE_BUTTON_3, m_cFileButton_3);
	DDX_Control(pDX, IDC_FILE_BUTTON_4, m_cFileButton_4);
	DDX_Control(pDX, IDC_FILE_BUTTON_5, m_cFileButton_5);
	DDX_Control(pDX, IDC_FILE_BUTTON_6, m_cFileButton_6);
	DDX_Control(pDX, IDC_FILE_BUTTON_7, m_cFileButton_7);
	DDX_Control(pDX, IDC_FILE_BUTTON_8, m_cFileButton_8);
	DDX_Control(pDX, IDC_FILE_BUTTON_9, m_cFileButton_9);
	DDX_Control(pDX, IDC_FILE_BUTTON_10, m_cFileButton_10);
	DDX_Control(pDX, IDC_FILE_BUTTON_11, m_cFileButton_11);
	DDX_Control(pDX, IDC_FILE_BUTTON_12, m_cFileButton_12);
	DDX_Control(pDX, IDC_FILE_BUTTON_13, m_cFileButton_13);
	DDX_Control(pDX, IDC_FILE_BUTTON_14, m_cFileButton_14);
	DDX_Control(pDX, IDC_FILE_BUTTON_15, m_cFileButton_15);
	DDX_Control(pDX, IDC_FILE_BUTTON_16, m_cFileButton_16);
	DDX_Control(pDX, IDC_FILE_SCROLLBAR, m_cFileScrollBar);
	DDX_Text(pDX, IDC_DEBUG_STATIC, m_szDebugString);
	DDX_Control(pDX, IDOK, m_cOKButton);
	DDX_Text(pDX, IDC_FILENAME_EDIT, m_szFileNameEdit);
	DDX_Control(pDX, IDC_NEW_FOLDER_BUTTON, m_cNewFolderButton);
	DDX_Control(pDX, IDC_DEBUG_STATIC, m_cDegbugText);
	DDX_Control(pDX, IDC_COPY_BUTTON, m_cCopyFileButton);
	DDX_Control(pDX, IDC_PASTE_BUTTON, m_cPasteFileButton);
	DDX_Control(pDX, IDC_DELETE_BUTTON, m_cDeleteFileButton);
	DDX_Control(pDX, IDC_FILETYPES_CHECK, m_cShowAllFileTypesCheck);
	DDX_Control(pDX, IDC_DELETE_FOLDER_BUTTON, m_cDeleteFolderButton);
	DDX_Control(pDX, IDC_GROUP_STATIC, m_cGroupBox);
}


BEGIN_MESSAGE_MAP(CDMFileManagementDialog, CDialog)
	ON_BN_CLICKED(IDC_FILE_BUTTON_1, &CDMFileManagementDialog::OnBnClickedFileButton1)
	ON_BN_CLICKED(IDC_FILE_BUTTON_2, &CDMFileManagementDialog::OnBnClickedFileButton2)
	ON_BN_CLICKED(IDC_FILE_BUTTON_3, &CDMFileManagementDialog::OnBnClickedFileButton3)
	ON_BN_CLICKED(IDC_FILE_BUTTON_4, &CDMFileManagementDialog::OnBnClickedFileButton4)
	ON_BN_CLICKED(IDC_FILE_BUTTON_5, &CDMFileManagementDialog::OnBnClickedFileButton5)
	ON_BN_CLICKED(IDC_FILE_BUTTON_6, &CDMFileManagementDialog::OnBnClickedFileButton6)
	ON_BN_CLICKED(IDC_FILE_BUTTON_7, &CDMFileManagementDialog::OnBnClickedFileButton7)
	ON_BN_CLICKED(IDC_FILE_BUTTON_8, &CDMFileManagementDialog::OnBnClickedFileButton8)
	ON_BN_CLICKED(IDC_FILE_BUTTON_9, &CDMFileManagementDialog::OnBnClickedFileButton9)
	ON_BN_CLICKED(IDC_FILE_BUTTON_10, &CDMFileManagementDialog::OnBnClickedFileButton10)
	ON_BN_CLICKED(IDC_FILE_BUTTON_11, &CDMFileManagementDialog::OnBnClickedFileButton11)
	ON_BN_CLICKED(IDC_FILE_BUTTON_12, &CDMFileManagementDialog::OnBnClickedFileButton12)
	ON_BN_CLICKED(IDC_FILE_BUTTON_13, &CDMFileManagementDialog::OnBnClickedFileButton13)
	ON_BN_CLICKED(IDC_FILE_BUTTON_14, &CDMFileManagementDialog::OnBnClickedFileButton14)
	ON_BN_CLICKED(IDC_FILE_BUTTON_15, &CDMFileManagementDialog::OnBnClickedFileButton15)
	ON_BN_CLICKED(IDC_FILE_BUTTON_16, &CDMFileManagementDialog::OnBnClickedFileButton16)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDOK, &CDMFileManagementDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_NEW_FOLDER_BUTTON, &CDMFileManagementDialog::OnBnClickedNewFolderButton)
	ON_BN_CLICKED(IDC_COPY_BUTTON, &CDMFileManagementDialog::OnBnClickedCopyButton)
	ON_BN_CLICKED(IDC_PASTE_BUTTON, &CDMFileManagementDialog::OnBnClickedPasteButton)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, &CDMFileManagementDialog::OnBnClickedDeleteButton)
	ON_BN_CLICKED(IDC_FILETYPES_CHECK, &CDMFileManagementDialog::OnBnClickedFiletypesCheck)
	ON_BN_CLICKED(IDC_DELETE_FOLDER_BUTTON, &CDMFileManagementDialog::OnBnClickedDeleteFolderButton)
END_MESSAGE_MAP()


// CDMFileManagementDialog message handlers


BOOL CDMFileManagementDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString szWindowTitle = _T("Load ");

	if (m_pFileDescriptor->m_bSave)
	{
		szWindowTitle = _T("Save ");
		m_szFileNameEdit = m_pFileDescriptor->m_szInitialFileName;
	}

	if (m_pFileDescriptor->m_szFileTypes == _T(".dmp"))
	{
		szWindowTitle += _T("Party");
		m_cGroupBox.SetWindowText("Parties:");
	}
	else if (m_pFileDescriptor->m_szFileTypes == _T(".dmc"))
	{
		szWindowTitle += _T("Character");
		m_cGroupBox.SetWindowText("Characters:");
	}
	else if (m_pFileDescriptor->m_szFileTypes == _T(".dmm"))
	{
		szWindowTitle += _T("Map");
		m_cGroupBox.SetWindowText("Maps:");
	}

	if (m_pFileDescriptor->m_szOriginalFileName != _T(""))
	{
		CString szSubDirs = m_pFileDescriptor->m_szOriginalFileName;
		szSubDirs.MakeLower();

		CString szRemove = m_pApp->m_szEXEPath;
		szRemove.MakeLower();

		szSubDirs.Replace(szRemove, "");

		szSubDirs.Replace("\\", "/");
		szSubDirs.Replace("//", "/");

		szRemove = "/" + m_pFileDescriptor->m_szInitialFileName;
		szSubDirs.Replace(szRemove, "");

		szSubDirs.Replace(".dmc", "");
		szSubDirs.Replace(".dmp", "");
		szSubDirs.Replace(".dmm", "");

		szSubDirs.Replace("data/characters", "");
		szSubDirs.Replace("data/maps", "");

		szSubDirs.Replace("/", "^");

		m_szSubDirectory = szSubDirs;
	}
	// C:\SOURCE\DM HELPER\.\RELEASE\data\characters\Seekers\dword greybeard.dmc"
	// c:\source\dm helper\.\release\data/characters//Oldtimers\alerion glamdrif.dmc"
	// "C:\SOURCE\DM HELPER\.\RELEASE\"



	SetWindowText(szWindowTitle);

	CFont ArialFont;
	ArialFont.CreatePointFont(90, "Arial Black");
	LOGFONT lf; ArialFont.GetLogFont(&lf);
	lf.lfWeight = FW_NORMAL;
	m_ListFont.CreateFontIndirect(&lf);

	CFont ArialFontBold;
	ArialFontBold.CreatePointFont(110, "Arial Black");
	LOGFONT lfb; ArialFontBold.GetLogFont(&lfb);
	lfb.lfWeight = FW_BOLD;
	m_ListFontBold.CreateFontIndirect(&lfb);

	m_FileButtons[0] = &m_cFileButton_1;
	m_FileButtons[1] = &m_cFileButton_2;
	m_FileButtons[2] = &m_cFileButton_3;
	m_FileButtons[3] = &m_cFileButton_4;
	m_FileButtons[4] = &m_cFileButton_5;
	m_FileButtons[5] = &m_cFileButton_6;
	m_FileButtons[6] = &m_cFileButton_7;
	m_FileButtons[7] = &m_cFileButton_8;
	m_FileButtons[8] = &m_cFileButton_9;
	m_FileButtons[9] = &m_cFileButton_10;
	m_FileButtons[10] = &m_cFileButton_11;
	m_FileButtons[11] = &m_cFileButton_12;
	m_FileButtons[12] = &m_cFileButton_13;
	m_FileButtons[13] = &m_cFileButton_14;
	m_FileButtons[14] = &m_cFileButton_15;
	m_FileButtons[15] = &m_cFileButton_16;

	m_pBackDropBitmap = new Bitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PARCHMENT_BITMAP)), FALSE);
	m_pPortraitBitmap = new Bitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PH_BITMAP)), FALSE);

	m_bTopIsUpDirectory = FALSE;
	m_nLastButtonSelected = -1;

	if (m_pFileDescriptor->m_bSave)
	{
		m_cOKButton.SetWindowText("Save");
	}
	else
	{
		m_cOKButton.SetWindowText("Load");
	}

	m_cDegbugText.ShowWindow(SW_HIDE);

	UpdateData(FALSE);

	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDMFileManagementDialog::Refresh()
{
	for (int i = 0; i < 16; ++i)
	{
		m_FileButtons[i]->ShowWindow(SW_HIDE);
		m_FileButtons[i]->EnableWindow(TRUE);
	}

	memset(m_bIsDirectory, 0, 16 * sizeof(BOOL));

	if (m_nOldScrollOffset != m_nScrollOffset)
	{
		m_nOldScrollOffset = m_nScrollOffset;
		m_nLastButtonSelected = -1;
	}

	/////////////////////////

	UpdateData(TRUE);

	BOOL bShowAll = m_cShowAllFileTypesCheck.GetCheck();

	m_bTopIsUpDirectory = FALSE;

	int nItemsCount = 0;
	int nIndex = 0;

	CString DirPath;
	DirPath = m_szPath;
	DirPath += m_szSubDirectory;
	DirPath.Replace("^", "\\");

	m_szDebugString = m_szPath + m_szSubDirectory;

	int nPos = m_szSubDirectory.ReverseFind('^');
	
	if (nPos >= 0)
	{
		if (nItemsCount >= m_nScrollOffset)
		{
			int nLength = m_szSubDirectory.GetLength() - nPos;
			CString szSubDirName = m_szSubDirectory.Right(nLength);
			szSubDirName.Replace("^", "");

			m_FileButtons[nIndex]->m_nAlignStyle = CMFCButton::ALIGN_LEFT;
			m_FileButtons[nIndex]->SetFont(&m_ListFont);

			m_FileButtons[nIndex]->SetImage(IDB_UP_FOLDER_ICON_BITMAP, IDB_UP_FOLDER_ICON_BITMAP);
			m_FileButtons[nIndex]->SetWindowText(szSubDirName);

			m_FileButtons[nIndex]->ShowWindow(SW_SHOW);

			m_bTopIsUpDirectory = TRUE;

			++nIndex;
		}
		++nItemsCount;
	}

	if (!DirPath.IsEmpty())
	{
		CFileFind find;
		CString strFile;

		if (DirPath.Right(1) != _T("\\"))
			strFile = DirPath + _T("\\*.*");
		else
			strFile = DirPath + _T("*.*");

		BOOL bFound = find.FindFile(strFile);

		while (bFound)
		{
			bFound = find.FindNextFile();
			CString strFileName;

			if (find.IsDirectory())
			{
				strFileName = find.GetFileName();

				if (strFileName == _T(".") || strFileName == _T("..") || strFileName == _T("portraits") || strFileName == _T("bitmaps") || strFileName == _T("icons"))
				{
					continue;
				}

				CString szDirName = strFileName;

				if (nItemsCount >= m_nScrollOffset)
				{
					if (nIndex < 16)
					{
						m_FileButtons[nIndex]->m_nAlignStyle = CMFCButton::ALIGN_LEFT;
						m_FileButtons[nIndex]->SetFont(&m_ListFont);

						m_FileButtons[nIndex]->SetImage(IDB_FOLDER_ICON_BITMAP, IDB_FOLDER_ICON_BITMAP);
						m_FileButtons[nIndex]->SetWindowText(szDirName);

						m_bIsDirectory[nIndex] = TRUE;

						m_FileButtons[nIndex]->ShowWindow(SW_SHOW);
					}

					++nIndex;
				}
				++nItemsCount;

				continue;
			}

			strFileName = find.GetFileName();
			CString ext4 = strFileName.Right(4);
			CString ext5 = strFileName.Right(5);

			if (!strFileName.IsEmpty() && (strFileName.GetLength()>4))
			{
				if ((bShowAll || ext4.CompareNoCase(m_pFileDescriptor->m_szFileTypes) == 0) || ext5.CompareNoCase(m_pFileDescriptor->m_szFileTypes) == 0)
				{
					if (nItemsCount >= m_nScrollOffset)
					{
						if (nIndex < 16)
						{
							m_FileButtons[nIndex]->m_nAlignStyle = CMFCButton::ALIGN_LEFT;
							m_FileButtons[nIndex]->SetFont(&m_ListFont);

							m_FileButtons[nIndex]->SetImage((HBITMAP)NULL);
							m_FileButtons[nIndex]->SetWindowText(strFileName);

							m_FileButtons[nIndex]->ShowWindow(SW_SHOW);

							if (ext4.CompareNoCase(m_pFileDescriptor->m_szFileTypes) == 0)
							{
								m_FileButtons[nIndex]->EnableWindow(TRUE);
							}
							else
							{
								m_FileButtons[nIndex]->EnableWindow(FALSE);
							}
						}
					

						++nIndex;
					}

					++nItemsCount;
				}
			}
		}

	}

	m_cFileScrollBar.SetScrollRange(0, nItemsCount-1);
	m_cFileScrollBar.SetScrollPos(m_nScrollOffset);


	UpdateData(FALSE);
}

void CDMFileManagementDialog::SelectedFile(int nSelected)
{
	CString szName;
	m_FileButtons[nSelected]->GetWindowText(szName);

	// m_szFileNameEdit = _T(""); // NONO

	if (m_pPortraitBitmap != NULL)
	{
		delete m_pPortraitBitmap;
		m_pPortraitBitmap = NULL;
	}

	if (1)
	{
		if (m_bIsDirectory[nSelected] == FALSE)
		{
			CString szPath = m_szPath;

			m_szFileNameEdit = szName;

			CString szFileName = szPath + m_szSubDirectory + "\\" + szName;
			szFileName.Replace("^", "//");

			if (LoadCharacterFromFile(szFileName.GetBuffer(0)) && m_pCharacter != NULL)
			{
				CString szTemp = m_pCharacter->m_szPortraitPath;

				szTemp.MakeUpper();
				szTemp.Replace("<$DMAPATH>", m_pApp->m_szEXEPath);

				LPWSTR wcsFile = szTemp.AllocSysString();
				m_pPortraitBitmap = new Bitmap(wcsFile, FALSE);

				int nHeight = m_pPortraitBitmap->GetHeight();

				if (nHeight == 0)
				{
					delete m_pPortraitBitmap;
					m_pPortraitBitmap = NULL;
				}
			}

			if (nSelected == m_nLastButtonSelected)
			{
				TRACE("DBL CLICK ! (#d %d)\n", nSelected, m_nLastButtonSelected);
				SendMessage(WM_COMMAND, IDOK, BN_CLICKED);
			}

			m_nLastButtonSelected = nSelected;
		}
		else
		{
			m_nLastButtonSelected = -1;
		}
		
		if (m_pPortraitBitmap == NULL)
		{
			m_pPortraitBitmap = new Bitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PH_BITMAP)), FALSE);
		}
	}


	UpdateData(FALSE);

	if (m_bIsDirectory[nSelected])
	{
		m_szSubDirectory += "^";
		m_szSubDirectory += szName;

		m_nScrollOffset = 0;

		Refresh();
	}
	
	
	InvalidateRect(NULL);
	
}

void CDMFileManagementDialog::OnBnClickedFileButton1()
{
	// go up one directory
	if (m_bTopIsUpDirectory)
	{
		m_nLastButtonSelected = -1;

		int nPos = m_szSubDirectory.ReverseFind('^');

		if (nPos >= 0)
		{
			CString szChop = m_szSubDirectory.Left(nPos);
			m_szSubDirectory = szChop;

			m_nScrollOffset = 0;

			if (m_pPortraitBitmap != NULL)
			{
				delete m_pPortraitBitmap;
				m_pPortraitBitmap = NULL;

				m_pPortraitBitmap = new Bitmap(::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_PH_BITMAP)), FALSE);
			}

			Refresh();

			InvalidateRect(NULL);
		}
	}
	else
	{
		SelectedFile(0);
	}
}

//more hidden below
#if 1
	void CDMFileManagementDialog::OnBnClickedFileButton2()
	{
		SelectedFile(1);
	}
	void CDMFileManagementDialog::OnBnClickedFileButton3()
	{
		SelectedFile(2);
	}
	void CDMFileManagementDialog::OnBnClickedFileButton4()
	{
		SelectedFile(3);
	}
	void CDMFileManagementDialog::OnBnClickedFileButton5()
	{
		SelectedFile(4);
	}
	void CDMFileManagementDialog::OnBnClickedFileButton6()
	{
		SelectedFile(5);
	}
	void CDMFileManagementDialog::OnBnClickedFileButton7()
	{
		SelectedFile(6);
	}
	void CDMFileManagementDialog::OnBnClickedFileButton8()
	{
		SelectedFile(7);
	}
	void CDMFileManagementDialog::OnBnClickedFileButton9()
	{
		SelectedFile(8);
	}
	void CDMFileManagementDialog::OnBnClickedFileButton10()
	{
		SelectedFile(9);
	}
	void CDMFileManagementDialog::OnBnClickedFileButton11()
	{
		SelectedFile(10);
	}
	void CDMFileManagementDialog::OnBnClickedFileButton12()
	{
		SelectedFile(11);
	}
	void CDMFileManagementDialog::OnBnClickedFileButton13()
	{
		SelectedFile(12);
	}
	void CDMFileManagementDialog::OnBnClickedFileButton14()
	{
		SelectedFile(13);
	}
	void CDMFileManagementDialog::OnBnClickedFileButton15()
	{
		SelectedFile(14);
	}
	void CDMFileManagementDialog::OnBnClickedFileButton16()
	{
		SelectedFile(15);
	}
#endif


void CDMFileManagementDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	Graphics graphics(dc);

	if (m_pBackDropBitmap != NULL)
	{
		graphics.DrawImage(m_pBackDropBitmap, 335 - 4, 35 - 2, 192 + 6, 193 + 6);
	}
	if (m_pPortraitBitmap != NULL)
	{
		try
		{
			graphics.DrawImage(m_pPortraitBitmap, 335 + 2, 35 + 2, 192 - 4, 193 - 4);
			TRACE("PAINTED !\n");
		}
		catch (...)
		{
			if (m_pCharacter != NULL)
			{
				memset(m_pCharacter->m_szPortraitPath, 0, MAX_PATH * sizeof(char));
			}
			delete m_pPortraitBitmap;
			m_pPortraitBitmap = NULL;
		}
	}

	if (m_szFileNameEdit.GetLength() > 0)
	{
		m_cOKButton.EnableWindow(TRUE);
	}
	else
	{
		m_cOKButton.EnableWindow(FALSE);
	}

	if (m_szCutFileName == _T(""))
	{
		m_cPasteFileButton.EnableWindow(FALSE);
	}
	else
	{
		m_cPasteFileButton.EnableWindow(TRUE);
	}

	if (m_nLastButtonSelected == -1)
	{
		m_cCopyFileButton.EnableWindow(FALSE);
		m_cDeleteFileButton.EnableWindow(FALSE);
	}
	else
	{
		m_cCopyFileButton.EnableWindow(TRUE);
		m_cDeleteFileButton.EnableWindow(TRUE);
	}

	BOOL bShowAll = m_cShowAllFileTypesCheck.GetCheck();
	BOOL bButtonEnabled = m_FileButtons[1]->IsWindowVisible();

	if (bShowAll && m_bTopIsUpDirectory && bButtonEnabled == FALSE)
	{
		m_cDeleteFolderButton.EnableWindow(TRUE);
	}
	else
	{
		m_cDeleteFolderButton.EnableWindow(FALSE);
	}
	
}


void CDMFileManagementDialog::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	switch (nSBCode)
	{
		case SB_LINEUP:
		{
			m_nScrollOffset -= 1;

			if (m_nScrollOffset < 0)
				m_nScrollOffset = 0;

			TRACE("UP %d %d\n", nSBCode, m_nScrollOffset);
			Refresh();

			break;
		}
		case SB_LINEDOWN:
		{
			m_nScrollOffset += 1;

			int nMin = 0;
			int nMax = 0;
			m_cFileScrollBar.GetScrollRange(&nMin, &nMax);

			if (m_nScrollOffset >= nMax)
				m_nScrollOffset = nMax;

			TRACE("DN %d %d\n", nSBCode, m_nScrollOffset);

			Refresh();

			break;
		}
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
		{
			m_nScrollOffset = nPos;

			TRACE("TK %d %d\n", nSBCode, m_nScrollOffset);
			Refresh();

			break;
		}
		default:
		{
			TRACE(">> %d %d\n", nSBCode, m_nScrollOffset);

			CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
			break;
		}
	}

}

BOOL CDMFileManagementDialog::LoadCharacterFromFile(char *szFileName)
{
	FILE *pInFile = fopen(szFileName, "rb");

	if (pInFile != NULL)
	{
		if (m_pCharacter == NULL)
		{
			m_pCharacter = new cDNDCharacter();
		}

		fread(m_pCharacter, sizeof(cDNDCharacter), 1, pInFile);

		fclose(pInFile);

		return TRUE;
	}

	return FALSE;
}



void CDMFileManagementDialog::OnBnClickedOk()
{
	UpdateData(TRUE);
	
	m_pFileDescriptor->m_bSuccess = TRUE;
	m_pFileDescriptor->m_szReturnedPath = m_szPath + m_szSubDirectory + "\\" + m_szFileNameEdit;
	m_pFileDescriptor->m_szReturnedPath.Replace("^", "//");

	if (m_pFileDescriptor->m_szReturnedPath.Find(m_pFileDescriptor->m_szFileTypes) < 0)
	{
		m_pFileDescriptor->m_szReturnedPath += m_pFileDescriptor->m_szFileTypes;
	}

	CDialog::OnOK();
}


void CDMFileManagementDialog::OnBnClickedNewFolderButton()
{
	CString szPrompt = _T("Enter new folder name:");
	CString szRetVal = _T("");

	CDMInputDialog *pDlg = new CDMInputDialog(szPrompt.GetBuffer(0), &szRetVal);
	pDlg->DoModal();

	if (szRetVal != _T(""))
	{
		CString szNewDir = m_szPath + m_szSubDirectory + "\\" + szRetVal;
		szNewDir.Replace("^", "\\");

		CreateDirectory(szNewDir, NULL);

		m_szSubDirectory += "^";
		m_szSubDirectory += szRetVal;

		Refresh();
	}

}


void CDMFileManagementDialog::OnBnClickedCopyButton()
{
	UpdateData(TRUE);

	m_szCutFilePath = m_szPath + m_szSubDirectory + "\\";
	m_szCutFilePath.Replace("^", "//");

	m_szCutFileName = m_szFileNameEdit;

	InvalidateRect(NULL);
}


void CDMFileManagementDialog::OnBnClickedPasteButton()
{
	CString szSourceName = m_szCutFilePath + "\\" + m_szCutFileName;

	CString szDestFileName = m_szPath + m_szSubDirectory + "\\" + m_szCutFileName;
	szDestFileName.Replace("^", "//");

	szSourceName.MakeLower();
	szDestFileName.MakeLower();

	MoveFile(szSourceName, szDestFileName);

	//move party log files too
	if (szSourceName.Find(_T(".dmp") >= 0))
	{
		szSourceName.Replace(".dmp", ".dml");
		szDestFileName.Replace(".dmp", ".dml");
		MoveFile(szSourceName, szDestFileName);
	}

	m_szCutFilePath = _T("");
	m_szCutFileName = _T("");

	Refresh();
}


void CDMFileManagementDialog::OnBnClickedDeleteButton()
{
	UpdateData(TRUE);

	CString szDelFileName = m_szPath + m_szSubDirectory + "\\";
	szDelFileName.Replace("^", "//");

	szDelFileName += m_szFileNameEdit;

	if (AfxMessageBox("Are you sure you wish to delete this file ?", MB_YESNO) == IDYES)
	{
		szDelFileName.MakeLower();
		DeleteFile(szDelFileName);

		//delete party log files too
		if (szDelFileName.Find(_T(".dmp") >= 0))
		{
			szDelFileName.Replace(".dmp", ".dml");
			DeleteFile(szDelFileName);
		}

		Refresh();
	}

	

}


void CDMFileManagementDialog::OnBnClickedFiletypesCheck()
{
	Refresh();
}


void CDMFileManagementDialog::OnBnClickedDeleteFolderButton()
{
	if (AfxMessageBox("Are you sure you wish to delete this folder ?", MB_YESNO) == IDYES)
	{
		CString szDirectory = m_szPath + m_szSubDirectory;
		szDirectory.Replace("^", "//");

		RemoveDirectory(szDirectory);

		OnBnClickedFileButton1();
	}
}
