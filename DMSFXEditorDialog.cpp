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
	, m_szExplainText(_T(""))
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
	DDX_Control(pDX, IDC_CUT_SOUND_BUTTON, m_cCutSoundButton);
	DDX_Control(pDX, IDC_COPY_SOUND_BUTTON, m_cCopySoundButton);
	DDX_Control(pDX, IDC_PASTE_SOUND_BUTTON, m_cPasteSoundButton);
	DDX_Control(pDX, IDC_ENCRYPT_BUTTON, m_cEncryptButton);
	DDX_Text(pDX, IDC_EXPLAIN_TEXT, m_szExplainText);
	DDX_Control(pDX, IDC_DESC_EDIT, m_cDescEdit);
}


BEGIN_MESSAGE_MAP(DMSFXEditorDialog, CDialog)
	//{{AFX_MSG_MAP(DMSFXEditorDialog)
	ON_BN_CLICKED(IDC_CHOOSE_FILE_BUTTON, OnChooseFileButton)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CUT_SOUND_BUTTON, &DMSFXEditorDialog::OnBnClickedCutSoundButton)
	ON_BN_CLICKED(IDC_COPY_SOUND_BUTTON, &DMSFXEditorDialog::OnBnClickedCopySoundButton)
	ON_BN_CLICKED(IDC_PASTE_SOUND_BUTTON, &DMSFXEditorDialog::OnBnClickedPasteSoundButton)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDCANCEL, &DMSFXEditorDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CLEAR_SOUND_BUTTON, &DMSFXEditorDialog::OnBnClickedClearSoundButton)
	ON_BN_CLICKED(IDC_ENCRYPT_BUTTON, &DMSFXEditorDialog::OnBnClickedEncryptButton)
	ON_EN_CHANGE(IDC_DESC_EDIT, &DMSFXEditorDialog::OnEnChangeDescEdit)
	ON_EN_CHANGE(IDC_FILENAME_EDIT, &DMSFXEditorDialog::OnEnChangeFilenameEdit)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DMSFXEditorDialog message handlers

BOOL DMSFXEditorDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	m_szDesc = m_pSoundFX->m_szDesc;
	m_szFilename = m_pSoundFX->m_szFilePath;

	m_bPasted = FALSE;

	#ifdef _DEBUG
	if (m_szFilename.Find(".DMS") >= 0)
	{
		m_cEncryptButton.SetWindowText("DECRYPT");
	}

	m_cEncryptButton.ShowWindow(SW_SHOW);
	#endif

	Refresh();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL DMSFXEditorDialog::StringContainsEvent(CString szEvent)
{

	CString szEvents[6] = {"ATTACK", "WAR CRY", "HURT", "DIE", "BREATH WEAPON", "EQUIP"};

	szEvent.MakeUpper();

	for (int i = 0; i < 6; ++i)
	{
		if (szEvent.Find(szEvents[i]) > -1)
		{
			return TRUE;
		}
	}

	return FALSE;
}

void DMSFXEditorDialog::Refresh()
{
	m_szExplainText = _T("");

	CString szFileName = m_szFilename;

	szFileName.MakeUpper();

	CString szDiskName = _T("");
	CString szText1 = _T("");
	CString szText2 = _T("");

	int nEndPath = szFileName.ReverseFind('\\');
	if (nEndPath >= 0)
	{
		szDiskName = szFileName.Mid(nEndPath+1, szFileName.GetLength() - nEndPath);
	}

	CString szDesc = m_szDesc;
	szDesc.MakeUpper();
	int nFindPC = szDesc.Find(" PC ");
	if (nFindPC > 0 && StringContainsEvent(szDesc))
	{
		CString szPCName = m_szDesc.Mid(0, nFindPC);
		CString szEvent = m_szDesc.Mid(nFindPC + 4, MAX_PATH);

		if (szDesc.Find("CHILD PC") >= 0)
		{
			// do nuthin
		}
		else if (szDesc.Find("MALE") >= 0)
		{
			szText1.Format("This sound is triggered by the event '%s' for a character with the gender '%s' assuming a more specific event is not defined\n", szEvent, szPCName);
		}
		else
		{
			szText1.Format("This sound is triggered by the event '%s' for a character with the first name '%s'\n", szEvent, szPCName);
		}
	}

	if (szText1 == _T(""))
	{
		int nFindEquip = szDesc.Find(" EQUIP");
		if (nFindEquip > 0 && szDesc.Find("DEFAULT") == -1)
		{
			CString szWeapon = m_szDesc.Mid(0, nFindEquip);
			szText1.Format("This sound is triggered by the event '%s' for weapons called '%s'\n", "EQUIP", szWeapon);
		}
		int nFindHit = szDesc.Find(" HIT");
		if (nFindHit > 0 && szDesc.Find("DEFAULT") == -1)
		{
			CString szWeapon = m_szDesc.Mid(0, nFindHit);
			szText1.Format("This sound is triggered by the event '%s' for weapons called '%s'\n", "HIT", szWeapon);
		}
		int nFindBreath = szDesc.Find(" BREATH WEAPON");
		if (nFindBreath > 0)
		{
			CString szType = m_szDesc.Mid(0, nFindBreath);
			szText1.Format("This sound is triggered by the event '%s' for monsters called '%s'\n", "BREATH WEAPON", szType);
		}
		int nFindWildCard= szDesc.Find(" * ");
		if (nFindWildCard > 0)
		{
			CString szName = m_szDesc.Mid(0, nFindWildCard);
			CString szEvent = m_szDesc.Mid(nFindWildCard+3, MAX_PATH);
			szText1.Format("This sound is triggered by the event '%s' for monsters with names that wildcard match '%s'\n", szEvent, szName);
		}
	}

	if (szText1 == _T(""))
	{
		int nFindEvent = szDesc.Find(" WAR CRY");

		if (-1 == nFindEvent)
			nFindEvent = szDesc.Find(" ATTACK");

		if (-1 == nFindEvent)
			nFindEvent = szDesc.Find(" DIE");

		if (-1 != nFindEvent)
		{
			CString szName = m_szDesc.Mid(0, nFindEvent);
			CString szEvent = m_szDesc.Mid(nFindEvent + 1, MAX_PATH);
			szText1.Format("This sound is triggered by the event '%s' for entities that match '%s'\n", szEvent, szName);
		}
	}

	int nFindRandom = szFileName.Find("RANDOM_");
	if (nFindRandom > 0 && szFileName.Find("_00.") > 0)
	{
		CString szNum = szFileName.Mid(nFindRandom + 7, 2);

		int nNumFiles = atoi(szNum.GetBuffer(0));
		CString szLastName = szDiskName;
		szNum.Format("_%02d.", nNumFiles - 1);
		szLastName.Replace("_00.", szNum);

		szText2.Format("When played this sound effect will play one of %d random sounds chosen from :\n%s thru %s", nNumFiles, szDiskName, szLastName);

	}

	int nFindMulti = szFileName.Find("MULTI_");
	if (nFindMulti > 0 && szFileName.Find("_00.") > 0)
	{
		CString szNum = szFileName.Mid(nFindRandom + 7, 2);

		int nNumFiles = atoi(szNum.GetBuffer(0));
		CString szLastName = szDiskName;
		
		szLastName.Replace("_00.", "_XX.");

		szText2.Format("When played, this sound effect will play a sound file %s where XX is the attack # of the monster for the current round",szLastName);

	}

	if (szText1 != _T("") || szText2 != _T(""))
	{
		m_szExplainText = "Notes:\n";
		m_szExplainText += szText1;

		if (szText2 != _T(""))
		{
			m_szExplainText += szDiskName;
			m_szExplainText += ":\n";

			m_szExplainText += szText2;
		}	
	}

	
	UpdateData(FALSE);
}


void DMSFXEditorDialog::OnEnChangeFilenameEdit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	UpdateData(TRUE);

	Refresh();
}


void DMSFXEditorDialog::OnEnChangeDescEdit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	UpdateData(TRUE);

	Refresh();
}


void DMSFXEditorDialog::OnOK() 
{
	
	UpdateData(TRUE);

	strcpy(m_pSoundFX->m_szDesc, m_szDesc.GetBuffer(0));
	strcpy(m_pSoundFX->m_szFilePath, m_szFilename.GetBuffer(0));

	if (m_bPasted && m_pApp->m_pSoundFXCutPasteBuffer != NULL)
	{
		if (m_pApp->m_SoundFXCutPasteType == DND_EDIT_TYPE_CUT)
		{
			memset(m_pApp->m_pSoundFXCutPasteBuffer, 0, sizeof(cDNDSoundEffect));
		}

		m_pApp->m_SoundFXCutPasteType = DND_EDIT_TYPE_NONE;
		m_pApp->m_pSoundFXCutPasteBuffer = NULL;
	}
	
	CDialog::OnOK();
}


void DMSFXEditorDialog::OnChooseFileButton() 
{
	UpdateData(TRUE);

	CString szPath;

	g_SFX_szFilename[0] = 0;

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

		Refresh();
		
	}
	
}

void DMSFXEditorDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages

	if (m_pApp->m_pSoundFXCutPasteBuffer == NULL || m_pApp->m_pSoundFXCutPasteBuffer == m_pSoundFX)
	{
		m_cPasteSoundButton.EnableWindow(FALSE);
	}
	else
	{
		m_cPasteSoundButton.EnableWindow(TRUE);
	}
}


void DMSFXEditorDialog::OnBnClickedCutSoundButton()
{
	m_pApp->m_SoundFXCutPasteType = DND_EDIT_TYPE_CUT;
	m_pApp->m_pSoundFXCutPasteBuffer = m_pSoundFX;
	InvalidateRect(NULL);

	PostMessage(WM_CLOSE);
}


void DMSFXEditorDialog::OnBnClickedCopySoundButton()
{
	m_pApp->m_SoundFXCutPasteType = DND_EDIT_TYPE_COPY;
	m_pApp->m_pSoundFXCutPasteBuffer = m_pSoundFX;
	InvalidateRect(NULL);

	PostMessage(WM_CLOSE);
}


void DMSFXEditorDialog::OnBnClickedPasteSoundButton()
{
	if (m_pApp->m_pSoundFXCutPasteBuffer != NULL)
	{
		m_szDesc = m_pApp->m_pSoundFXCutPasteBuffer->m_szDesc;
		m_szFilename = m_pApp->m_pSoundFXCutPasteBuffer->m_szFilePath;
		//m_bPasted = TRUE;
	}

	UpdateData(FALSE);
}


void DMSFXEditorDialog::OnBnClickedClearSoundButton()
{
	m_szDesc = "";
	m_szFilename = "";

	UpdateData(FALSE);

}


void DMSFXEditorDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialog::OnCancel();
}


BOOL DMSFXEditorDialog::FileExists(CString szPath)
{

	FILE *pInFile = fopen(szPath, "rb");

	if (NULL == pInFile)
	{
		return FALSE;
	}

	fclose(pInFile);

	return TRUE;
}

void DMSFXEditorDialog::OnBnClickedEncryptButton()
{

#ifdef DEBUG

	m_szFilename.MakeUpper();
	if (m_szFilename.Find(".DMS") >= 0)  // decrypt
	{
		CString szFileName = m_szFilename;

		szFileName.Replace("<$DMAPATH>", m_pApp->m_szEXEPath);

		FILE *pInfile = fopen(szFileName, "rb");

		if (pInfile != NULL)
		{
			fseek(pInfile, 0, SEEK_END);
			LONG lSize = ftell(pInfile);
			lSize -= DM_ENCRYPT_OFFSET;

			char *pBuffer = (char *)malloc(lSize * sizeof(char));
			memset(pBuffer, 0, lSize * sizeof(char));

			fseek(pInfile, DM_ENCRYPT_OFFSET, SEEK_SET);

			fread(pBuffer, 1, lSize*sizeof(char), pInfile);
			fclose(pInfile);

			DeleteFile(szFileName);
			m_szFilename.Replace(".DMS", ".WAV");
			szFileName.Replace(".DMS", ".WAV");

			FILE *pOutfile = fopen(szFileName, "wb");

			if (pOutfile != NULL)
			{
				fwrite(pBuffer, 1, lSize*sizeof(char), pOutfile);

				fclose(pOutfile);
			}

			free(pBuffer);

			UpdateData(FALSE);

			OnOK();
		}

	}
	else
	{
		CString szFileName = m_szFilename;

		if (m_pApp->EncryptWAVFile(szFileName))
		{
			m_szFilename.Replace(".WAV", ".DMS");

			UpdateData(FALSE);

			OnOK();
		}	
		else
		{
			szFileName.Replace(".WAV", ".DMS");

			if (TRUE == FileExists(szFileName))
			{
				m_szFilename.Replace(".WAV", ".DMS");

				UpdateData(FALSE);

				OnOK();
			}
		}
	}

#endif
}




