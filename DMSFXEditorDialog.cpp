// DMSFXEditorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMSFXEditorDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static OPENFILENAME    g_ofn;
char g_SFX_szFilename[MAX_PATH];



/////////////////////////////////////////////////////////////////////////////
// DMSFXEditorDialog dialog


DMSFXEditorDialog::DMSFXEditorDialog(cDNDSoundEffect *_pSoundFX, CWnd* pParent /*=NULL*/)
	: CDialog(DMSFXEditorDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(DMSFXEditorDialog)
	m_szDesc = _T("");
	m_szFilename = _T("");
	//}}AFX_DATA_INIT

	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_pSoundFX = _pSoundFX;
}


void DMSFXEditorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DMSFXEditorDialog)
	DDX_Text(pDX, IDC_DESC_EDIT, m_szDesc);
	DDX_Text(pDX, IDC_FILENAME_EDIT, m_szFilename);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DMSFXEditorDialog, CDialog)
	//{{AFX_MSG_MAP(DMSFXEditorDialog)
	ON_BN_CLICKED(IDC_CHOOSE_FILE_BUTTON, OnChooseFileButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DMSFXEditorDialog message handlers

BOOL DMSFXEditorDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_szDesc = m_pSoundFX->m_szDesc;
	m_szFilename = m_pSoundFX->m_szFilePath;

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void DMSFXEditorDialog::OnOK() 
{
	// TODO: Add extra validation here

	UpdateData(TRUE);

	strcpy(m_pSoundFX->m_szDesc, m_szDesc.GetBuffer(0));
	strcpy(m_pSoundFX->m_szFilePath, m_szFilename.GetBuffer(0));
	
	CDialog::OnOK();
}


void DMSFXEditorDialog::OnChooseFileButton() 
{
	UpdateData(TRUE);

	CString szPath;

	ZeroMemory(&g_ofn, sizeof(OPENFILENAME));
	g_ofn.lStructSize = sizeof(OPENFILENAME);
	g_ofn.nMaxFile = MAX_PATH;
	g_ofn.nMaxFileTitle = MAX_PATH;
	g_ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	g_ofn.hwndOwner   = m_hWnd;
	g_ofn.hInstance   = m_pApp->m_hInstance;
    g_ofn.lpstrFile   = g_SFX_szFilename;
	g_ofn.lpstrInitialDir = "C:/";
    g_ofn.lpstrTitle  = "Load sound file";
    //g_ofn.lpstrFilter = "Graphics Files (*.bmp)\0*.bmp\0(*.gif)\0*.gif\0(*.jpg)\0*.jpg\0All Files (*.*)\0*.*\0\0";
	g_ofn.lpstrFilter = "All Files (*.*)\0*.*\0\0";
    g_ofn.lpstrDefExt = "wav";

    // Get action template file name
    if(GetOpenFileName(&g_ofn))
	{
		szPath = g_SFX_szFilename;
		szPath.MakeUpper();
		szPath.Replace(m_pApp->m_szEXEPath, "<$DMAPATH>");

		m_szFilename = szPath;

		UpdateData(FALSE);
		
	}
	
}

