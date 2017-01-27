// DMNPCPortraitSelectDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMNPCPortraitSelectDialog.h"
#include "afxdialogex.h"


// CDMNPCPortraitSelectDialog dialog

IMPLEMENT_DYNAMIC(CDMNPCPortraitSelectDialog, CDialog)

CDMNPCPortraitSelectDialog::CDMNPCPortraitSelectDialog(CString szType, CString szSourcePath, CString szDestPath, CString szMMName, CString szCharacterName, CWnd* pParent /*=NULL*/)
	: CDialog(CDMNPCPortraitSelectDialog::IDD, pParent)
	, m_nRadio1(0)
	, m_nRadio2(0)
{
	m_pApp =(CDMHelperApp*)AfxGetApp();

	m_szType = szType;
	m_szSourcePath = szSourcePath;
	m_szDestPath = szDestPath;
	m_szMMName = szMMName;
	m_szCharacterName = szCharacterName;
	m_szFileType = _T(".bmp");
}

CDMNPCPortraitSelectDialog::~CDMNPCPortraitSelectDialog()
{
}

void CDMNPCPortraitSelectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DELETE_FILE_CHECK, m_cDeleteCheck);
}


BEGIN_MESSAGE_MAP(CDMNPCPortraitSelectDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CDMNPCPortraitSelectDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CDMNPCPortraitSelectDialog message handlers


BOOL CDMNPCPortraitSelectDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString szTemp;

	szTemp.Format("Choose %s Picture File Type", m_szType);
	SetWindowText(szTemp);

	if (m_szSourcePath.Find(".jpg") >= 0)
		m_szFileType = _T(".jpg");
	else if (m_szSourcePath.Find(".png") >= 0)
		m_szFileType = _T(".png");
	else if (m_szSourcePath.Find(".gif") >= 0)
		m_szFileType = _T(".gif");

	szTemp.Format("Save as '%s%s' for NPCs of type '%s'", m_szMMName, m_szFileType, m_szMMName);
	(GetDlgItem(IDC_RADIO1))->SetWindowText(szTemp);

	szTemp.Format("Save as '%s%s' for NPC named '%s'", m_szCharacterName, m_szFileType, m_szCharacterName);
	(GetDlgItem(IDC_RADIO2))->SetWindowText(szTemp);

	CheckRadioButton(IDC_RADIO1, IDC_RADIO2, IDC_RADIO1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDMNPCPortraitSelectDialog::OnBnClickedOk()
{
	UpdateData(TRUE);

	//CString szDestFile = m_pApp->m_szEXEPath + _T("data\\monster_pictures\\");
	CString szDestFile = m_pApp->m_szEXEPath + m_szDestPath;
	CString szDeleteFile = _T("");

	if (IsDlgButtonChecked(IDC_RADIO1))
	{
		szDestFile += m_szMMName;
		szDeleteFile = m_pApp->m_szEXEPath + m_szDestPath + m_szCharacterName + ".*";
	}
	else
	{
		szDestFile += m_szCharacterName;
	}

	szDestFile += m_szFileType;

	if (m_cDeleteCheck.GetCheck())
	{
		DeleteFile(m_szSourcePath);
	}

	if (szDeleteFile != __T(""))
	{
		szDeleteFile.Replace(".*", ".png");
		DeleteFile(szDeleteFile);

		szDeleteFile.Replace(".png", ".jpg");
		DeleteFile(szDeleteFile);

		szDeleteFile.Replace(".jpg", ".bmp");
		DeleteFile(szDeleteFile);
	}

	CopyFile(m_szSourcePath, szDestFile, FALSE);

	CDialog::OnOK();
}
