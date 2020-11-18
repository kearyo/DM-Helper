// cDMMapLegendDialog.cpp : implementation file
//

#include "stdafx.h"
#include "memdc.h"
#include "DM Helper.h"
#include "cDMMapLegendDialog.h"
#include "afxdialogex.h"


// cDMMapLegendDialog dialog

IMPLEMENT_DYNAMIC(cDMMapLegendDialog, CDialog)

cDMMapLegendDialog::cDMMapLegendDialog(CWnd* pParent /*=NULL*/)
	: CDialog(cDMMapLegendDialog::IDD, pParent)
{
	m_pApp = (CDMHelperApp *)AfxGetApp();
	m_pParent = (cDMMapViewDialog *)pParent;
	m_pBitmap = NULL;

	Create(cDMMapLegendDialog::IDD, pParent);
}

cDMMapLegendDialog::~cDMMapLegendDialog()
{
	
}

void cDMMapLegendDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(cDMMapLegendDialog, CDialog)
	ON_BN_CLICKED(IDCANCEL, &cDMMapLegendDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &cDMMapLegendDialog::OnBnClickedOk)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// cDMMapLegendDialog message handlers


void cDMMapLegendDialog::OnBnClickedCancel()
{
	CDialog::OnCancel();

	if (NULL != m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	m_pParent->m_pMapLegendDialog = nullptr;

}

void cDMMapLegendDialog::OnBnClickedOk()
{
	CDialog::OnOK();
}



BOOL cDMMapLegendDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rParentRect;
	m_pParent->GetClientRect(&rParentRect);

	if (m_pApp->m_hInstance != NULL) // unit test support
	{
		m_hIcon = m_pApp->LoadIcon(IDR_MAINFRAME);
	}

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_nOffsetY = 0;
	m_fScaleY = 1.0f;
	m_fViewScale = 1.0f;
	m_nMaxScrollY = 0;

	CString m_szBitmapPath = m_pParent->m_pDNDMap->m_szMapLegendPath;
	m_szBitmapPath.MakeUpper();
	m_szBitmapPath.Replace("<$DMAPATH>", m_pApp->m_szEXEPath);
	//CString m_szBitmapPath = "D:\\source\\DM Helper\\Release\\Data\\maps\\bitmaps\\Flanaess_576\\Flanaess_576_legend.png";

	CString szPath;
	szPath.Format("%s", m_szBitmapPath);
	szPath.MakeUpper();
	szPath.Replace("<$DMAPATH>", m_pApp->m_szEXEPath);

	LPWSTR wcsFile = szPath.AllocSysString();
	m_pBitmap = new Bitmap(wcsFile, FALSE);

	if (m_pBitmap != NULL)
	{
		m_fScaleY = (float)m_pBitmap->GetHeight() / (float)m_pBitmap->GetWidth();
		m_fViewScale = 400.0f / (float)m_pBitmap->GetWidth();
		m_nMaxScrollY = 400 * m_fScaleY - (float)m_pBitmap->GetHeight() * m_fViewScale;
	}

	SetWindowPos(NULL, rParentRect.right - 400, 70, 400, 400 * m_fScaleY, SWP_SHOWWINDOW);

	

	CRect rClientRect;
	GetClientRect(&rClientRect);

	if (m_pBitmap != NULL)
	{
		m_fViewScale = (float)rClientRect.right / (float)m_pBitmap->GetWidth();
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void cDMMapLegendDialog::OnPaint()
{
	CPaintDC _dc(this); // device context for painting

	CRect rUpdateRect;
	GetClientRect(&rUpdateRect);

	if (NULL != m_pBitmap)
	{
		#if USE_MEMDC
		BOOL bBuffer = TRUE;

		CDC* pDC = &_dc;
		CGDIMemDC pmDC(pDC, bBuffer, &rUpdateRect);
		#else
		CDC* pmDC = &_dc;
		#endif

		#if USE_MEMDC
		Graphics graphics(pmDC);
		#else
		Graphics graphics(dc);
		#endif

		Color blackColor(255, 0, 0, 0);
		graphics.Clear(blackColor);

		int nSourceX = 0;
		int nSourceY = 0;

		int nSizeX = m_pBitmap->GetWidth();
		int nSizeY = m_pBitmap->GetHeight();

		int nX = 0;
		int nY = 0;

		RectF dst;
		dst.X = (Gdiplus::REAL)0;
		dst.Y = (Gdiplus::REAL)0;
		dst.Width = (Gdiplus::REAL)rUpdateRect.right;
		dst.Height = (Gdiplus::REAL)dst.Width*m_fScaleY;

		graphics.DrawImage(m_pBitmap, dst, (Gdiplus::REAL)nSourceX, (Gdiplus::REAL)nSourceY + m_nOffsetY, (Gdiplus::REAL)nSizeX, (Gdiplus::REAL)nSizeY, UnitPixel); //, UnitPixel);

		//graphics.DrawImage(m_pBitmap, rUpdateRect.left, rUpdateRect.top, rUpdateRect.right, rUpdateRect.bottom);
	}
}


void cDMMapLegendDialog::PostNcDestroy()
{
	if (NULL != m_pBitmap)
	{
		delete m_pBitmap;
		m_pBitmap = NULL;
	}

	CDialog::PostNcDestroy();
}


void cDMMapLegendDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	InvalidateRect(NULL);
}


BOOL cDMMapLegendDialog::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_nOffsetY -= zDelta;

	if (m_nOffsetY < 0)
	{
		m_nOffsetY = 0;
	}

	int nSizeY = 0;
	float fViewScale = 1.0f;

	CRect rUpdateRect;
	GetClientRect(&rUpdateRect);

	if (m_pBitmap != NULL)
	{
		//fViewScale = 400.0f / (float)rUpdateRect.right *m_fViewScale;
		//nSizeY = (float)m_pBitmap->GetHeight() * fViewScale;
		nSizeY = (float)m_pBitmap->GetHeight();
	}

	if (m_nOffsetY > nSizeY)
	{
		m_nOffsetY = nSizeY;
	}

	InvalidateRect(NULL);

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


BOOL cDMMapLegendDialog::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}
