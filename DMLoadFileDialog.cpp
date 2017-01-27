// DMLoadFileDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMLoadFileDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DMLoadFileDialog dialog


DMLoadFileDialog::DMLoadFileDialog(DMLoadFileDescriptor *_pFileDescriptor, CWnd* pParent /*=NULL*/)
	: CDialog(DMLoadFileDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(DMLoadFileDialog)
	m_szFileNameEdit = _T("");
	//}}AFX_DATA_INIT

	m_pFileDescriptor = _pFileDescriptor;

	m_pFileDescriptor->m_szInitialPath.MakeLower();
	m_pFileDescriptor->m_szSubDirectory.MakeLower();
	m_pFileDescriptor->m_szInitialFileName.MakeLower();
	m_pFileDescriptor->m_szFileTypes.MakeLower();
	m_pFileDescriptor->m_szReturnedPath.MakeLower();

	m_szPath = m_pFileDescriptor->m_szInitialPath + m_pFileDescriptor->m_szSubDirectory;

}


void DMLoadFileDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DMLoadFileDialog)
	DDX_Control(pDX, IDOK, m_cOKButton);
	DDX_Control(pDX, IDC_FILENAME_EDIT, m_cFileNameEdit);
	DDX_Control(pDX, IDC_FILE_LIST, m_cFileLister);
	DDX_Text(pDX, IDC_FILENAME_EDIT, m_szFileNameEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DMLoadFileDialog, CDialog)
	//{{AFX_MSG_MAP(DMLoadFileDialog)
	ON_BN_CLICKED(IDDELETE, OnDelete)
	ON_LBN_DBLCLK(IDC_FILE_LIST, OnDblclkFileList)
	ON_LBN_SETFOCUS(IDC_FILE_LIST, OnSetfocusFileList)
	ON_LBN_SELCHANGE(IDC_FILE_LIST, OnSelchangeFileList)
	ON_WM_KEYDOWN()
	ON_LBN_SELCANCEL(IDC_FILE_LIST, OnSelcancelFileList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DMLoadFileDialog message handlers

BOOL DMLoadFileDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	SetWindowText(m_pFileDescriptor->m_szLabel);

	if(m_pFileDescriptor->m_bSave == FALSE)
	{
		m_cFileNameEdit.EnableWindow(FALSE);
		m_cOKButton.SetWindowText("Load");
	}
	else
	{
		m_cOKButton.SetWindowText("Save");
	}

	if(m_pFileDescriptor->m_bSave && m_pFileDescriptor->m_szInitialFileName !=  _T(""))
	{
		m_pFileDescriptor->m_szInitialFileName.Replace(m_szPath, "");
		m_pFileDescriptor->m_szInitialFileName.Replace("/", "");
		m_pFileDescriptor->m_szInitialFileName.Replace("\\", "");
		m_pFileDescriptor->m_szInitialFileName.Replace(m_pFileDescriptor->m_szFileTypes, "");

		m_szFileNameEdit = m_pFileDescriptor->m_szInitialFileName;

		UpdateData(FALSE);
	}
	LoadFileList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DMLoadFileDialog::OnOK() 
{
	ReadFileName();
	
	CDialog::OnOK();
}

void DMLoadFileDialog::ReadFileName()
{
	UpdateData(TRUE);

	int nCursor = m_cFileLister.GetCurSel();

	char buffer[MAX_PATH];
	CString szPath;
	
	if(m_pFileDescriptor->m_bSave)
	{
		CString szExt = m_pFileDescriptor->m_szFileTypes;

		CString szTemp;
		szTemp.Format("%s//%s", m_szPath, m_szFileNameEdit.GetBuffer(0));
		szTemp.MakeLower();
		szTemp.Replace(szExt, "");

		szPath.Format("%s%s", szTemp.GetBuffer(0), szExt.GetBuffer(0));

		FILE *pFile = fopen(szPath.GetBuffer(0), "rb");
		if(pFile != NULL)
		{
			if(AfxMessageBox("Are you sure you wish to overwrite this file ?", MB_ICONQUESTION|MB_OKCANCEL) == IDCANCEL)
			{
				m_pFileDescriptor->m_bSuccess = FALSE;
			}
			else
			{
				m_pFileDescriptor->m_bSuccess = TRUE;
			}
			fclose(pFile);
		}
		else
		{
			m_pFileDescriptor->m_bSuccess = TRUE;
		}

	}
	else
	{
		m_pFileDescriptor->m_bSuccess = TRUE;

		m_cFileLister.GetText(nCursor, buffer);
		szPath.Format("%s//%s", m_szPath, buffer);
	}

	szPath.MakeLower();

	m_pFileDescriptor->m_szReturnedPath = szPath;

}

void DMLoadFileDialog::OnDelete() 
{	
	UpdateData(TRUE);

	int nCursor = m_cFileLister.GetCurSel();

	char buffer[MAX_PATH];
	CString szPath;

	m_cFileLister.GetText(nCursor, buffer);
	szPath.Format("%s//%s", m_szPath, buffer);

	//delete da file here
	DeleteFile(szPath);

	m_cFileLister.ResetContent();

	LoadFileList();

	InvalidateRect(NULL);

	
}

void DMLoadFileDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void DMLoadFileDialog::LoadFileList()
{
	CString DirPath;

	m_cFileLister.ResetContent();

	//DirPath.Format("%s\\%s" ,m_pszInitialPath, m_pszFileTypes);
	DirPath = m_szPath;

	//m_ctlListCtrl.SetRedraw(FALSE);
	if(!DirPath.IsEmpty())
	{
		//m_ctlListCtrl.SetCurDirectory(DirPath); 
		//m_ctlListCtrl.ClearImageFileArray();
		//jpg,bmp,png,tiff,jpeg,gif

		CFileFind find;
		CString strFile;

		if(DirPath.Right(1) != _T("\\"))
			strFile = DirPath + _T("\\*.*");
		else
			strFile = DirPath + _T("*.*");

		//UINT nCountImages=0;
		BOOL bFound = find.FindFile(strFile);
		int nIndex = 0;
		while(bFound)
		{
			bFound = find.FindNextFile();
			CString strFileName;

			if (find.IsDirectory())
			{
				//strFileName = find.GetFileName();
				//CString szDirName = "+" + strFileName;
				//m_cFileLister.InsertString(nIndex, szDirName);

				continue;
			}

			strFileName = find.GetFileName();
			CString ext4=strFileName.Right(4);
			CString ext5=strFileName.Right(5);

			if(!strFileName.IsEmpty() && (strFileName.GetLength()>4))
			{ 
				//if(ext3.CompareNoCase("bmp")==0 || ext3.CompareNoCase("dib")==0 || 
				//ext3.CompareNoCase("jpg")==0 || ext3.CompareNoCase("tif")==0 || 
				//ext3.CompareNoCase("png")==0 || ext4.CompareNoCase("jpeg")==0 ||
				//ext4.CompareNoCase("tiff")==0 ) //If Condition true add filename to list control
				if(ext4.CompareNoCase(m_pFileDescriptor->m_szFileTypes)==0 || ext5.CompareNoCase(m_pFileDescriptor->m_szFileTypes)==0)
				{
					m_cFileLister.InsertString(nIndex, strFileName);
					++nIndex;
				}
			}
		}

	}

}


void DMLoadFileDialog::OnDblclkFileList() 
{
	ReadFileName();
	
	CDialog::OnOK();
}

void DMLoadFileDialog::OnSetfocusFileList() 
{
	// TODO: Add your control notification handler code here
	TRACE("BANG!\n");
}

void DMLoadFileDialog::OnSelchangeFileList() 
{

	UpdateData(TRUE);

	int nCursor = m_cFileLister.GetCurSel();

	char buffer[MAX_PATH];
	m_cFileLister.GetText(nCursor, buffer);

	m_szFileNameEdit = buffer;

	UpdateData(FALSE);
}

void DMLoadFileDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void DMLoadFileDialog::OnSelcancelFileList() 
{
	// TODO: Add your control notification handler code here

	TRACE("BEEP\n");
}

BOOL DMLoadFileDialog::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->hwnd == m_cFileLister.m_hWnd)
	{
		switch(pMsg->message)
		{
			case WM_KEYDOWN:
			{
				if(pMsg->wParam == VK_DELETE)
				{
					TRACE("TRIP ! %d %d\n", pMsg->wParam, VK_DELETE);
					OnDelete();
				}
				break;
			}
		}
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}


