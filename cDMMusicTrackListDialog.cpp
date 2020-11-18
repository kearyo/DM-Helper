// cDMMusicTrackListDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "cDMMusicTrackListDialog.h"
#include "afxdialogex.h"


// cDMMusicTrackListDialog dialog

IMPLEMENT_DYNAMIC(cDMMusicTrackListDialog, CDialog)

cDMMusicTrackListDialog::cDMMusicTrackListDialog(CWnd* pParent, CString *pszReturnString)
	: CDialog(cDMMusicTrackListDialog::IDD, pParent)
	, m_szTrackSearch(_T(""))
{
	m_pApp = (CDMHelperApp *)AfxGetApp();
	m_pszReturnString = pszReturnString;
}

cDMMusicTrackListDialog::~cDMMusicTrackListDialog()
{
}

void cDMMusicTrackListDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TRACK_LIST, m_cTrackList);
	DDX_Text(pDX, IDC_TRACK_SEARCH_EDIT, m_szTrackSearch);
}


BEGIN_MESSAGE_MAP(cDMMusicTrackListDialog, CDialog)
	ON_BN_CLICKED(IDOK, &cDMMusicTrackListDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &cDMMusicTrackListDialog::OnBnClickedCancel)
	ON_LBN_DBLCLK(IDC_TRACK_LIST, &cDMMusicTrackListDialog::OnLbnDblclkTrackList)
	ON_EN_CHANGE(IDC_TRACK_SEARCH_EDIT, &cDMMusicTrackListDialog::OnEnChangeTrackSearchEdit)
END_MESSAGE_MAP()


// cDMMusicTrackListDialog message handlers


BOOL cDMMusicTrackListDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	LoadTracks();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void cDMMusicTrackListDialog::OnBnClickedOk()
{
	CDialog::OnOK();
}


void cDMMusicTrackListDialog::OnBnClickedCancel()
{
	CDialog::OnCancel();
}


void cDMMusicTrackListDialog::OnLbnDblclkTrackList()
{
	UpdateData(FALSE);

	int nCursor = m_cTrackList.GetCurSel();

	if (nCursor > -1)
	{
		CString szTrack = "";
		m_cTrackList.GetText(nCursor, szTrack);

		*m_pszReturnString = szTrack;
	}

	OnOK();
}

void cDMMusicTrackListDialog::LoadTracks()
{
	m_cTrackList.ResetContent();

	CString szFileName = m_pApp->m_szEXEPath + "track_list.dat";
	FILE *pInFile = fopen(szFileName, "rt");

	char szLine[512];
	int nIndex = 0;
	if (pInFile != NULL)
	{
		do
		{
			memset(szLine, 0, 512 * sizeof(char));
			fgets(szLine, 512, pInFile);

			CString szTemp = szLine;
			szTemp.Replace("\n", "");
			szTemp.Replace("\r", "");
			szTemp.Replace(".MP3", "");

			if (szTemp.GetLength() < 1)
				continue;

			if (m_szTrackSearch != _T(""))
			{
				if (szTemp.Find(m_szTrackSearch) == -1)
				{
					continue;
				}
			}

			m_cTrackList.InsertString(nIndex, szTemp);

			if (szTemp == *m_pszReturnString)
			{
				m_cTrackList.SetCurSel(nIndex);
			}

			++nIndex;

		} while (!feof(pInFile));

		fclose(pInFile);
	}

}


void cDMMusicTrackListDialog::OnEnChangeTrackSearchEdit()
{
	UpdateData(TRUE);

	m_szTrackSearch.MakeUpper();

	LoadTracks();
}
