// DMSaveConfirmDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMSaveConfirmDialog.h"


// CDMSaveConfirmDialog dialog

IMPLEMENT_DYNAMIC(CDMSaveConfirmDialog, CDialog)

CDMSaveConfirmDialog::CDMSaveConfirmDialog(char *pszMsg, BOOL *pbSave, BOOL *pbSaveAll, CWnd* pParent /*=NULL*/)
	: CDialog(CDMSaveConfirmDialog::IDD, pParent)
	, m_szSavePrompt(_T(""))
{

	m_szSavePrompt.Format("%s", pszMsg);

	m_pbSave = pbSave;
	*m_pbSave = FALSE;

	m_pbSaveAll = pbSaveAll;
}

CDMSaveConfirmDialog::~CDMSaveConfirmDialog()
{
}

void CDMSaveConfirmDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK2, m_cYesToAllButton);
	DDX_Text(pDX, IDC_PROMPT_TEXT, m_szSavePrompt);
}


BEGIN_MESSAGE_MAP(CDMSaveConfirmDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CDMSaveConfirmDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDOK2, &CDMSaveConfirmDialog::OnBnClickedOk2)
	ON_BN_CLICKED(IDCANCEL, &CDMSaveConfirmDialog::OnBnClickedCancel)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDMSaveConfirmDialog message handlers

BOOL CDMSaveConfirmDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	if(m_pbSaveAll == NULL)
	{
		m_cYesToAllButton.ShowWindow(SW_HIDE);
	}

	//m_pCellBitmap = Bitmap::FromResource(AfxGetApp()->m_hInstance, MAKEINTRESOURCEW(IDB_PARCHMENT_BITMAP));

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDMSaveConfirmDialog::OnBnClickedOk()
{
	*m_pbSave = TRUE;
	
	OnOK();
}

void CDMSaveConfirmDialog::OnBnClickedOk2()
{
	*m_pbSave = TRUE;

	if(m_pbSaveAll != NULL)
	{
		*m_pbSaveAll = TRUE;
	}
	
	OnOK();
}

void CDMSaveConfirmDialog::OnBnClickedCancel()
{
	*m_pbSave = FALSE;
	
	OnCancel();
}

void CDMSaveConfirmDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	/*
	CDC memdc;
	CDC* pDC;
	pDC = &dc;
	
	Graphics graphics(dc);

	//Color backColor(255, 0, 0, 0);
	//graphics.Clear(backColor);

	CRect rect;
	GetClientRect(&rect);

	CFont newfont;
	CFont newfont2;
	CFont* oldfont;

	pDC->SetTextAlign(TA_CENTER);

	int nSize = 9;
	int success = newfont.CreatePointFont(nSize * 10, "Arial Black", pDC);
	
	oldfont = dc.SelectObject(&newfont);

	SetTextColor(dc, 0x000000);
	SetBkMode(dc, TRANSPARENT);

	//graphics.DrawImage(m_pCellBitmap, rect.left, rect.top, rect.right, rect.bottom);

	DrawPromptText(m_szSavePrompt.GetBuffer(0), 0, 0, &dc);
	*/
}

void CDMSaveConfirmDialog::DrawPromptText(char * szData, int x, int y, CDC *memdc)
{
	CRect rect;
	GetClientRect(&rect);
	
	//rect.top = y;
	//rect.bottom = rect.top + 50;

	memdc->DrawText(szData, &rect, DT_SINGLELINE | DT_RIGHT | DT_VCENTER );
}
