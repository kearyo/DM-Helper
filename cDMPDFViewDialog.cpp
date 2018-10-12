// cDMPDFViewDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "cDMPDFViewDialog.h"



// cDMPDFViewDialog dialog

IMPLEMENT_DYNAMIC(cDMPDFViewDialog, CDialogEx)

cDMPDFViewDialog::cDMPDFViewDialog(CString szPDFPath, int nPage, CWnd* pParent /*=NULL*/)
: CDialogEx(cDMPDFViewDialog::IDD, pParent)
	, m_szPages(_T(""))
	, m_szPage(_T(""))
	, m_szSearch(_T(""))
{
	m_pApp = (CDMHelperApp *)AfxGetApp();

	//m_pcPDFView = new CPdfviewerctrl1();

	//m_pcPDFView->m_hWnd = NULL;
	m_szPDFPath = szPDFPath;
	m_nPDFPage = nPage;
	m_nPDFPages = 0;

	m_dwPDFID = GetUniqueID();

	Create(cDMPDFViewDialog::IDD);
}

cDMPDFViewDialog::~cDMPDFViewDialog()
{
	TRACE("BYE!");
}

void cDMPDFViewDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_PDFVIEWERCTRL1, *m_pcPDFView);
	DDX_Text(pDX, IDC_PAGE_STATIC, m_szPages);
	DDX_Text(pDX, IDC_PAGE_EDIT, m_szPage);
	DDX_Control(pDX, IDC_PAGE_EDIT, m_cPage);
	DDX_Text(pDX, IDC_SEARCH_EDIT, m_szSearch);
	DDX_Control(pDX, IDC_PAGE_BACK_BUTTON, m_cPageLeftButton);
	DDX_Control(pDX, IDC_PAGE_FWD_BUTTON, m_cPageRightButton);
	DDX_Control(pDX, IDC_ZOOM_IN_BUTTON, m_cZoomInButton);
	DDX_Control(pDX, IDC_ZOOM_OUT_BUTTON, m_cZoomOutButton);
}


BEGIN_MESSAGE_MAP(cDMPDFViewDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &cDMPDFViewDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &cDMPDFViewDialog::OnBnClickedCancel)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_PAGE_BACK_BUTTON, &cDMPDFViewDialog::OnBnClickedPageBackButton)
	ON_BN_CLICKED(IDC_PAGE_FWD_BUTTON, &cDMPDFViewDialog::OnBnClickedPageFwdButton)
	ON_BN_CLICKED(IDC_ZOOM_IN_BUTTON, &cDMPDFViewDialog::OnBnClickedZoomInButton)
	ON_BN_CLICKED(IDC_ZOOM_OUT_BUTTON, &cDMPDFViewDialog::OnBnClickedZoomOutButton)
	ON_EN_CHANGE(IDC_PAGE_EDIT, &cDMPDFViewDialog::OnEnChangePageEdit)
	ON_EN_CHANGE(IDC_SEARCH_EDIT, &cDMPDFViewDialog::OnEnChangeSearchEdit)
	ON_BN_CLICKED(IDC_SEARCH_BUTTON, &cDMPDFViewDialog::OnBnClickedSearchButton)
	ON_WM_MOUSEWHEEL()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// cDMPDFViewDialog message handlers


void cDMPDFViewDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
}


BOOL cDMPDFViewDialog::OnInitDialog()
{

	CDialogEx::OnInitDialog();

	m_cPageLeftButton.SetBitmap((::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_WIDE_LEFT_BUTTON_BITMAP))));
	m_cPageRightButton.SetBitmap((::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_WIDE_RIGHT_BUTTON_BITMAP))));

	m_cZoomInButton.SetBitmap((::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_ZOOM_IN_BUTTON_BITMAP))));
	m_cZoomOutButton.SetBitmap((::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_ZOOM_OUT_BUTTON_BITMAP))));

	//m_pcPDFView->LoadPDFFile(m_szPDFPath);

	//m_nPDFPages = m_pcPDFView->GetTotalPage();

	ShowWindow(SW_SHOW);

	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void cDMPDFViewDialog::OnBnClickedCancel()
{
	
	CDialogEx::OnCancel();

	Cleanup();
}

void cDMPDFViewDialog::OnClose()
{

	CDialogEx::OnClose();

	Cleanup();
}


void cDMPDFViewDialog::Cleanup()
{
	m_pApp->m_PDFViewMap.SetAt((WORD)m_dwPDFID, NULL);
	m_pApp->m_PDFViewMap.RemoveKey((WORD)m_dwPDFID);

	/*
	if (NULL != m_pcPDFView)
	{
		delete m_pcPDFView;
		m_pcPDFView = NULL;
	}
	*/

//	delete this;
}


void cDMPDFViewDialog::Refresh()
{
	m_szPage.Format("%d", m_nPDFPage);
	m_cPage.SetWindowText(m_szPage);


	m_szPages.Format("of %d", m_nPDFPages);

	//m_pcPDFView->GoToPage(m_nPDFPage);

	UpdateData(FALSE);
}

void cDMPDFViewDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting

}


void cDMPDFViewDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	RECT rRect;

	GetClientRect(&rRect);

	//if (m_pcPDFView->m_hWnd != NULL)
	{
		//m_pcPDFView->SetWindowPos(this, rRect.left, rRect.top + 40, rRect.right, rRect.bottom, SWP_NOACTIVATE);
	}

	InvalidateRect(NULL);
}


void cDMPDFViewDialog::OnBnClickedPageBackButton()
{
	m_nPDFPage -= 1;

	if (m_nPDFPage <= 1)
	{
		m_nPDFPage = 1;
	}

	Refresh();
}


void cDMPDFViewDialog::OnBnClickedPageFwdButton()
{
	//short nHappy = m_pcPDFView->GetPDFPageNo();

	m_nPDFPage += 1;

	if (m_nPDFPage > m_nPDFPages)
	{
		m_nPDFPage = m_nPDFPages;
	}

	Refresh();
}


void cDMPDFViewDialog::OnBnClickedZoomInButton()
{
	//m_pcPDFView->PDFZoomIn();
}


void cDMPDFViewDialog::OnBnClickedZoomOutButton()
{
	//m_pcPDFView->PDFZoomOut();
}


void cDMPDFViewDialog::OnEnChangePageEdit()
{
	UpdateData(TRUE);

	m_nPDFPage = atoi(m_szPage.GetBuffer(0));
	
	if (m_nPDFPage <= 1)
	{
		m_nPDFPage = 1;
	}

	if (m_nPDFPage > m_nPDFPages)
	{
		m_nPDFPage = m_nPDFPages;
	}

	//m_pcPDFView->GoToPage(m_nPDFPage);
}


void cDMPDFViewDialog::OnEnChangeSearchEdit()
{
	UpdateData(TRUE);

	//m_pcPDFView->Search(m_szSearch);
	
	//m_nPDFPage = m_cPDFView.GetPDFPageNo();
	//Refresh();
}


void cDMPDFViewDialog::OnBnClickedSearchButton()
{
	//int nJoe = m_pcPDFView->SearchNextText();
	TRACE("!");

	//m_nPDFPage = m_cPDFView.GetPDFPageNo();
	//Refresh();
}


BOOL cDMPDFViewDialog::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if ((nFlags & MK_SHIFT))
	{
		//if (zDelta > 0)
			//m_pcPDFView->PDFZoomIn();
		//else
			//m_pcPDFView->PDFZoomOut();

		return TRUE;
	}

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


