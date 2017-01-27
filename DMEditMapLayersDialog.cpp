// DMEditMapLayersDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "cDMMapViewDialog.h"
#include "DMEditMapLayersDialog.h"
#include "afxdialogex.h"


static OPENFILENAME    g_lofn;
char g_LMV_szFilename[MAX_PATH];
char g_LMVBP_szFilename[MAX_PATH];

// CDMEditMapLayersDialog dialog

IMPLEMENT_DYNAMIC(CDMEditMapLayersDialog, CDialog)

CDMEditMapLayersDialog::CDMEditMapLayersDialog(cDNDMapCell *pCell, CWnd* pParent /*=NULL*/)
	: CDialog(CDMEditMapLayersDialog::IDD, pParent)
	, m_szCellLabel(_T(""))
{

	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_pMapViewDialog = (cDMMapViewDialog *)pParent;
	m_pCell = pCell;

	Create(CDMEditMapLayersDialog::IDD, pParent);
}

CDMEditMapLayersDialog::~CDMEditMapLayersDialog()
{
	m_pMapViewDialog->m_pEditMapLayersDialog = NULL;
}

void CDMEditMapLayersDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_CELL_LABEL, m_szCellLabel);
	DDX_Control(pDX, IDC_EDIT_LAYER_1, m_cEditLayer1);
	DDX_Control(pDX, IDC_EDIT_LAYER_2, m_cEditLayer2);
	DDX_Control(pDX, IDC_EDIT_LAYER_3, m_cEditLayer3);
	DDX_Control(pDX, IDC_EDIT_LAYER_4, m_cEditLayer4);
	DDX_Control(pDX, IDC_DELETE_LAYER_1, m_cDeleteLayer1);
	DDX_Control(pDX, IDC_DELETE_LAYER_2, m_cDeleteLayer2);
	DDX_Control(pDX, IDC_DELETE_LAYER_3, m_cDeleteLayer3);
	DDX_Control(pDX, IDC_DELETE_LAYER_4, m_cDeleteLayer4);
}


BEGIN_MESSAGE_MAP(CDMEditMapLayersDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CDMEditMapLayersDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDMEditMapLayersDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_EDIT_LAYER_1, &CDMEditMapLayersDialog::OnBnClickedEditLayer1)
	ON_BN_CLICKED(IDC_EDIT_LAYER_2, &CDMEditMapLayersDialog::OnBnClickedEditLayer2)
	ON_BN_CLICKED(IDC_EDIT_LAYER_3, &CDMEditMapLayersDialog::OnBnClickedEditLayer3)
	ON_BN_CLICKED(IDC_EDIT_LAYER_4, &CDMEditMapLayersDialog::OnBnClickedEditLayer4)
	ON_BN_CLICKED(IDC_DELETE_LAYER_1, &CDMEditMapLayersDialog::OnBnClickedDeleteLayer1)
	ON_BN_CLICKED(IDC_DELETE_LAYER_2, &CDMEditMapLayersDialog::OnBnClickedDeleteLayer2)
	ON_BN_CLICKED(IDC_DELETE_LAYER_3, &CDMEditMapLayersDialog::OnBnClickedDeleteLayer3)
	ON_BN_CLICKED(IDC_DELETE_LAYER_4, &CDMEditMapLayersDialog::OnBnClickedDeleteLayer4)
END_MESSAGE_MAP()


// CDMEditMapLayersDialog message handlers

BOOL CDMEditMapLayersDialog::OnInitDialog()
{
	CDialog::OnInitDialog();


	ShowWindow(SW_SHOW);

	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDMEditMapLayersDialog::Refresh()
{
	m_cDeleteLayer1.EnableWindow(FALSE);
	m_cDeleteLayer2.EnableWindow(FALSE);
	m_cDeleteLayer3.EnableWindow(FALSE);
	m_cDeleteLayer4.EnableWindow(FALSE);

	m_szCellLabel.Format("Cell Layers For : %s", m_pCell->m_szBitmapPath);

	m_szCellLabel.Replace("<$DMAPATH>", "");

	if (LayerExists(1))
	{
		m_cEditLayer1.SetWindowText("Change Layer 1");
		m_cDeleteLayer1.EnableWindow(TRUE);
	}
	else
	{
		m_cEditLayer1.SetWindowText("Add Layer 1");
	}
	//
	if (LayerExists(2))
	{
		m_cEditLayer2.SetWindowText("Change Layer 2");
		m_cDeleteLayer2.EnableWindow(TRUE);
	}
	else
	{
		m_cEditLayer2.SetWindowText("Add Layer 2");
	}
	//
	if (LayerExists(3))
	{
		m_cEditLayer3.SetWindowText("Change Layer 3");
		m_cDeleteLayer3.EnableWindow(TRUE);
	}
	else
	{
		m_cEditLayer3.SetWindowText("Add Layer 3");
	}
	//
	if (LayerExists(4))
	{
		m_cEditLayer4.SetWindowText("Change Layer 4");
		m_cDeleteLayer4.EnableWindow(TRUE);
	}
	else
	{
		m_cEditLayer4.SetWindowText("Add Layer 4");
	}

	UpdateData(FALSE);
}

void CDMEditMapLayersDialog::OnBnClickedOk()
{
	CDialog::OnOK();

	delete this;
}


void CDMEditMapLayersDialog::OnBnClickedCancel()
{
	CDialog::OnCancel();

	delete this;
}

BOOL CDMEditMapLayersDialog::LayerExists(int nLayer)
{
	CString szBitmapPath = m_pCell->m_szBitmapPath;

	szBitmapPath.Replace("<$DMAPATH>", m_pApp->m_szEXEPath);

	CString szLayer;
	szLayer.Format("_L%d.???", nLayer);

	szBitmapPath.Replace(".JPG", "");
	szBitmapPath.Replace(".BMP", "");
	szBitmapPath.Replace(".PNG", "");
	szBitmapPath += szLayer;

	CFileFind find;

	BOOL bFound = find.FindFile(szBitmapPath);

	while (bFound)
	{
		int nPos = szBitmapPath.ReverseFind('\\');

		bFound = find.FindNextFile();

		szLayer = find.GetFileName();

		return(TRUE);
	}

	return FALSE;

}

void CDMEditMapLayersDialog::AddLayer(int nLayer)
{
	CString szBitmapPath = m_pCell->m_szBitmapPath;

	szBitmapPath.Replace("<$DMAPATH>", m_pApp->m_szEXEPath);

	CString szLayer;
	szLayer.Format("_L%d.???", nLayer);

	szBitmapPath.Replace(".JPG", "");
	szBitmapPath.Replace(".BMP", "");
	szBitmapPath.Replace(".PNG", "");
	szBitmapPath += szLayer;

	CString szPath;

	ZeroMemory(&g_lofn, sizeof(OPENFILENAME));
	g_lofn.lStructSize = sizeof(OPENFILENAME);
	g_lofn.nMaxFile = MAX_PATH;
	g_lofn.nMaxFileTitle = MAX_PATH;
	g_lofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	g_lofn.hwndOwner = m_hWnd;
	g_lofn.hInstance = m_pApp->m_hInstance;
	g_lofn.lpstrFile = g_LMVBP_szFilename;
	g_lofn.lpstrInitialDir = "C:/";
	g_lofn.lpstrTitle = "Load map layer bitmap file";
	//g_lofn.lpstrFilter = "Graphics Files (*.bmp)\0*.bmp\0(*.gif)\0*.gif\0(*.jpg)\0*.jpg\0All Files (*.*)\0*.*\0\0";
	g_lofn.lpstrFilter = "All Files (*.*)\0*.*\0\0";
	g_lofn.lpstrDefExt = "bmp";

	// Get action template file name
	if (GetOpenFileName(&g_lofn))
	{
		szPath = g_LMVBP_szFilename;

		szPath.MakeUpper();

		if (szPath.Find("BMP" >= 0))
		{
			szBitmapPath.Replace("???", "BMP");
		}
		else if (szPath.Find("JPG" >= 0))
		{
			szBitmapPath.Replace("???", "JPG");
		}
		else if (szPath.Find("PNG" >= 0))
		{
			szBitmapPath.Replace("???", "PNG");
		}

		CopyFile(szPath, szBitmapPath, FALSE);

		m_pMapViewDialog->ForceRefresh();

		Refresh();
	}


}

void CDMEditMapLayersDialog::DeleteLayer(int nLayer)
{
	CString szBitmapPath = m_pCell->m_szBitmapPath;

	szBitmapPath.Replace("<$DMAPATH>", m_pApp->m_szEXEPath);

	CString szLayer;
	szLayer.Format("_L%d.BMP", nLayer);

	szBitmapPath.Replace("\\.\\", "\\");
	szBitmapPath.Replace("\\", "/");
	szBitmapPath.Replace("//", "/");

	szBitmapPath.Replace("/", "\\");

	szBitmapPath.Replace(".JPG", "");
	szBitmapPath.Replace(".BMP", "");
	szBitmapPath.Replace(".PNG", "");
	szBitmapPath += szLayer;

	DWORD dwError = 0;
	int nRetVal = DeleteFile(szBitmapPath);

	szBitmapPath.Replace(".BMP", ".JPG");
	nRetVal = DeleteFile(szBitmapPath);

	szBitmapPath.Replace(".JPG", ".PNG");
	nRetVal = DeleteFile(szBitmapPath);

	dwError = GetLastError();

	if (dwError != NO_ERROR)
	{
	}

	m_pMapViewDialog->ForceRefresh();

	Refresh();
}


void CDMEditMapLayersDialog::OnBnClickedEditLayer1()
{
	AddLayer(1);
}


void CDMEditMapLayersDialog::OnBnClickedEditLayer2()
{
	AddLayer(2);
}


void CDMEditMapLayersDialog::OnBnClickedEditLayer3()
{
	AddLayer(3);
}


void CDMEditMapLayersDialog::OnBnClickedEditLayer4()
{
	AddLayer(4);
}


void CDMEditMapLayersDialog::OnBnClickedDeleteLayer1()
{
	DeleteLayer(1);
}


void CDMEditMapLayersDialog::OnBnClickedDeleteLayer2()
{
	DeleteLayer(2);
}


void CDMEditMapLayersDialog::OnBnClickedDeleteLayer3()
{
	DeleteLayer(3);
}


void CDMEditMapLayersDialog::OnBnClickedDeleteLayer4()
{
	DeleteLayer(4);
}
