// DMItemDescriptionDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMItemDescriptionDialog.h"
#include "afxdialogex.h"


// CDMItemDescriptionDialog dialog

IMPLEMENT_DYNAMIC(CDMItemDescriptionDialog, CDialog)

CDMItemDescriptionDialog::CDMItemDescriptionDialog(cDNDMagicItemDescription *pMagicItemDescription, CWnd* pParent /*=NULL*/)
	: CDialog(CDMItemDescriptionDialog::IDD, pParent)
	, m_szTextBox(_T(""))
	, m_szDescriptionEdit(_T(""))
{
	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_pMagicItemDescription = pMagicItemDescription;

	m_nScrollPos = 0;

}

CDMItemDescriptionDialog::~CDMItemDescriptionDialog()
{
}

void CDMItemDescriptionDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TEXT_BOX, m_szTextBox);
	DDX_Control(pDX, IDC_TEXT_BOX, m_cTextBox);
	DDX_Text(pDX, IDC_DESCRIPTION_EDIT, m_szDescriptionEdit);
	DDX_Control(pDX, IDC_DESCRIPTION_EDIT, m_cDescriptionEdit);
}


BEGIN_MESSAGE_MAP(CDMItemDescriptionDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CDMItemDescriptionDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDMItemDescriptionDialog::OnBnClickedCancel)
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_WM_SIZING()
END_MESSAGE_MAP()


// CDMItemDescriptionDialog message handlers


BOOL CDMItemDescriptionDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CFont FixedFont;

	FixedFont.CreatePointFont(90, "FixedSys");
	LOGFONT lf; FixedFont.GetLogFont(&lf);
	//lf.lfWeight = FW_BOLD;
	m_TextFont.CreateFontIndirect(&lf);

	//m_cTextBox.SetFont(&m_TextFont);
	m_cDescriptionEdit.SetFont(&m_TextFont);


	SetWindowText(m_pMagicItemDescription->m_szMagicItemName);

	CString szTemp = m_pMagicItemDescription->m_szDescription;

	//szTemp.Replace("\n", "@");
	//szTemp.Replace("\r", "*");

	szTemp.Replace("\n", "\r\n");

	//m_szTextBox = szTemp;
	m_szDescriptionEdit = szTemp;

	UpdateData(FALSE);

	GetClientRect(&m_rEditBoxRect);

	m_cDescriptionEdit.SetFocus();

	//SetScrollRange(SB_VERT, 0, 680, 1);

	return FALSE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDMItemDescriptionDialog::OnBnClickedOk()
{
	CDialog::OnOK();
}


void CDMItemDescriptionDialog::OnBnClickedCancel()
{
	CDialog::OnCancel();
}


void CDMItemDescriptionDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	GetClientRect(&m_rEditBoxRect);
	m_rEditBoxRect.left += 5;
	m_rEditBoxRect.top += 5;
	m_rEditBoxRect.right -= 5;
	m_rEditBoxRect.bottom -= 5;

	//m_rEditBoxRect.top -= m_nScrollPos;

	m_cDescriptionEdit.MoveWindow(&m_rEditBoxRect, 0);

}


void CDMItemDescriptionDialog::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{

	switch (nSBCode)
	{
		case SB_LINEUP:
		{
			m_nScrollPos -= 10;

			if (m_nScrollPos < 0)
				m_nScrollPos = 0;

			TRACE("UP %d %d\n", nSBCode, m_nScrollPos);

			break;
		}
		case SB_LINEDOWN:
		{
			m_nScrollPos += 10;

			int nMin = 0;
			int nMax = 0;
			GetScrollRange(SB_VERT, &nMin, &nMax);

			if (m_nScrollPos >= nMax)
				m_nScrollPos = nMax;

			TRACE("DN %d %d\n", nSBCode, m_nScrollPos);

			break;
		}
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
		{
			m_nScrollPos = nPos;

			TRACE("TK %d %d\n", nSBCode, m_nScrollPos);

			break;
		}
		default:
		{
			TRACE(">> %d %d\n", nSBCode, m_nScrollPos);

			CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
			break;
		}
	}

	SetScrollPos(SB_VERT, m_nScrollPos, 1);

	UpdateData(TRUE);

	InvalidateRect(NULL);
}


BOOL CDMItemDescriptionDialog::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	
	if(zDelta > 0)
	{
		m_nScrollPos -= 10;

		if (m_nScrollPos < 0)
			m_nScrollPos = 0;
	}
	else if (zDelta < 0)
	{
		m_nScrollPos += 10;

		int nMin = 0;
		int nMax = 0;
		GetScrollRange(SB_VERT, &nMin, &nMax);

		if (m_nScrollPos >= nMax)
			m_nScrollPos = nMax;
	}

	SetScrollPos(SB_VERT, m_nScrollPos, 1);

	UpdateData(TRUE);

	InvalidateRect(NULL);

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}




void CDMItemDescriptionDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	InvalidateRect(NULL);
}


void CDMItemDescriptionDialog::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialog::OnSizing(fwSide, pRect);

	InvalidateRect(NULL);
}
