// DMCharacterNotesDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMCharacterNotesDialog.h"


// CDMCharacterNotesDialog dialog

IMPLEMENT_DYNAMIC(CDMCharacterNotesDialog, CDialog)

CDMCharacterNotesDialog::CDMCharacterNotesDialog(PDNDCHARACTER pPC, CWnd* pParent /*=NULL*/)
	: CDialog(CDMCharacterNotesDialog::IDD, pParent)
	, m_szNotes(_T(""))
	, m_szCharsRemaining(_T(""))
{
	m_pPC = pPC;
}

CDMCharacterNotesDialog::~CDMCharacterNotesDialog()
{
}

void CDMCharacterNotesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_NOTES_EDIT, m_szNotes);
	DDX_Control(pDX, IDC_NOTES_EDIT, m_cNotesEdit);
	DDX_Control(pDX, IDC_CHARS_REM_STATIC, m_cCharsRemaining);
	DDX_Text(pDX, IDC_CHARS_REM_STATIC, m_szCharsRemaining);
}


BEGIN_MESSAGE_MAP(CDMCharacterNotesDialog, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CDMCharacterNotesDialog::OnBnClickedCancel)
	ON_EN_CHANGE(IDC_NOTES_EDIT, &CDMCharacterNotesDialog::OnEnChangeNotesEdit)
	ON_BN_CLICKED(IDC_DONE, &CDMCharacterNotesDialog::OnBnClickedDone)
	ON_BN_CLICKED(IDOK, &CDMCharacterNotesDialog::OnBnClickedOk)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDMCharacterNotesDialog message handlers

BOOL CDMCharacterNotesDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CFont CourierFont;

	CourierFont.CreatePointFont(90,"Courier New");
	LOGFONT lf; CourierFont.GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	m_ListFont.CreateFontIndirect(&lf);

	m_cNotesEdit.SetFont(&m_ListFont);

	CString szTemp;
	szTemp.Format("Notes: %s", m_pPC->m_szCharacterName);

	SetWindowText(szTemp);

	m_szNotes = m_pPC->m_szDetails;

	m_cNotesEdit.SetLimitText(2047);

	UpdateData(FALSE);

	Update();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDMCharacterNotesDialog::OnBnClickedCancel()
{
	
	OnCancel();
}

void CDMCharacterNotesDialog::Update()
{
	int nLength = m_cNotesEdit.GetWindowTextLength();

	m_szCharsRemaining.Format("%d of 2048 remaining", max(0, 2048-nLength));

	m_cCharsRemaining.SetWindowText(m_szCharsRemaining);

	RECT Rect;
	m_cCharsRemaining.GetClientRect(&Rect);
	InvalidateRect(&Rect);

}

void CDMCharacterNotesDialog::OnEnChangeNotesEdit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	Update();
	
	m_pPC->MarkChanged();
}

void CDMCharacterNotesDialog::OnBnClickedDone()
{
	UpdateData(TRUE);

	strcpy(m_pPC->m_szDetails, m_szNotes.Left(2047));
	m_pPC->m_szDetails[2047] = 0;

	OnOK();
}

void CDMCharacterNotesDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//OnOK();
}

void CDMCharacterNotesDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
}
