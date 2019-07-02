// DMPartyLogDialog.cpp : implementation file
//
#include "stdafx.h"
#include "memdc.h"
#include "DM Helper.h"
#include "DM HelperDlg.h"
#include "DMLoadFileDialog.h"
#include "DMCharSpellsDialog.h"
#include "DMInventoryDialog.h"
#include "DMCharViewDialog.h"
#include "DMNPCViewDialog.h"
#include "cDMMapViewDialog.h"
#include "DMCharacterSelectorDialog.h"
#include "DMCastSpellDialog.h"
#include "DMModifyValueDialog.h"
#include "DMTimeEditDialog.h"
#include "DMPartyDialog.h"
#include "DMPartyLogDialog.h"
#include "DMEventLoggerDialog.h"


#define USE_PL_MEMDC TRUE

// CDMPartyLogDialog dialog

IMPLEMENT_DYNAMIC(CDMPartyLogDialog, CDialog)

CDMPartyLogDialog::CDMPartyLogDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDMPartyLogDialog::IDD, pParent)
{
	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_pCalendar = m_pApp->m_pSelectedCalendar;

	m_pParentWindow = (DMPartyDialog *)pParent;

	m_nScrollPos = 0;

	m_nEventStart = -1;
	m_nPickedRect = -1;

	m_nWindowTimer = 0;

	Create(CDMPartyLogDialog::IDD);
}

CDMPartyLogDialog::~CDMPartyLogDialog()
{
}

void CDMPartyLogDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_BUTTON, m_cEditButton);
	DDX_Control(pDX, IDC_DELETE_BUTTON, m_cDeleteButton);
	DDX_Control(pDX, IDC_INSERT_BUTTON, m_cInsertButton);
	DDX_Control(pDX, IDC_ERASE_BUTTON, m_cEraseLogButton);
}


BEGIN_MESSAGE_MAP(CDMPartyLogDialog, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CDMPartyLogDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CDMPartyLogDialog::OnBnClickedOk)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_DELETE_BUTTON, &CDMPartyLogDialog::OnBnClickedDeleteButton)
	ON_BN_CLICKED(IDC_INSERT_BUTTON, &CDMPartyLogDialog::OnBnClickedInsertButton)
	ON_BN_CLICKED(IDC_EDIT_BUTTON, &CDMPartyLogDialog::OnBnClickedEditButton)
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_ERASE_BUTTON, &CDMPartyLogDialog::OnBnClickedEraseButton)
	ON_WM_MOUSEWHEEL()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CDMPartyLogDialog message handlers


BOOL CDMPartyLogDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	//m_cPrevMonthButton.LoadBitmaps(IDB_LARGE_LEFT_BLACK_BITMAP, IDB_LARGE_LEFT_BLACK_BITMAP);
	//m_cNextMonthButton.LoadBitmaps(IDB_LARGE_RIGHT_BLACK_BITMAP, IDB_LARGE_RIGHT_BLACK_BITMAP);

	m_pBackgroundBitmap = Bitmap::FromResource(AfxGetApp()->m_hInstance, MAKEINTRESOURCEW(IDB_PARCHMENT_BITMAP));

	SetScrollRange(SB_VERT, 0, MAX_LOG_EVENTS-1, TRUE);

	ClearPicks();

	m_bIsDirty = FALSE;
	//m_nWindowTimer = SetTimer(9467, 500, NULL);

	ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDMPartyLogDialog::OnBnClickedCancel()
{
	if(m_pBackgroundBitmap != NULL)
	{
		delete m_pBackgroundBitmap;
		m_pBackgroundBitmap = NULL;
	}

	m_pParentWindow->m_pPartyLogDialog = NULL;

	OnCancel();

	//delete this;
}

void CDMPartyLogDialog::OnBnClickedOk()
{
	OnOK();
}

void CDMPartyLogDialog::OnClose()
{
	if(m_pBackgroundBitmap != NULL)
	{
		delete m_pBackgroundBitmap;
		m_pBackgroundBitmap = NULL;
	}

	m_pParentWindow->m_pPartyLogDialog = NULL;

	CDialog::OnClose();

	//delete this;
}


void CDMPartyLogDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	SetScrollPos(SB_VERT, m_nScrollPos, FALSE);
	
	BOOL bBuffer = FALSE;

	CRect rect;
	GetClientRect(&rect);

	CDC memdc;
	CDC* pDC = &dc;
	

#if USE_PL_MEMDC
	bBuffer = TRUE;

	CGDIMemDC pmDC(pDC, bBuffer, &rect);

	Graphics graphics(pmDC);

	pDC = pmDC;
#else
	Graphics graphics(dc);
#endif

	Color backColor(255, 0, 0, 0);
	graphics.Clear(backColor);


	CRect Brect;
	m_cEraseLogButton.GetClientRect(&Brect);


	if(m_nPickedRect >= 0)
	{
		//m_cEditButton.ShowWindow(SW_SHOW);
		m_cDeleteButton.ShowWindow(SW_SHOW);
		m_cInsertButton.ShowWindow(SW_SHOW);
		m_cEraseLogButton.ShowWindow(SW_SHOW);

		m_cEraseLogButton.SetWindowPos(NULL, rect.right - Brect.right - 3, rect.bottom - Brect.bottom - 3, Brect.right, Brect.bottom, SW_SHOW);
	}
	else
	{
		//m_cEditButton.ShowWindow(SW_HIDE);
		m_cDeleteButton.ShowWindow(SW_HIDE);
		m_cInsertButton.ShowWindow(SW_HIDE);
		m_cEraseLogButton.ShowWindow(SW_HIDE);
	}


	graphics.DrawImage(m_pBackgroundBitmap, rect.left, rect.top, rect.right-24, rect.bottom);

	if(m_pApp->m_pSelectedCalendar == NULL)
		return;

	if(m_pParentWindow->m_pPartyLog == NULL)
		return;

#if 1
	CFont newfont;
	CFont* oldfont;

	int nSize = 14;
	int success = newfont.CreatePointFont(nSize * 10, "HamletOrNot", pDC);

	oldfont = pDC->SelectObject(&newfont);
#endif

	SetTextColor(*pDC, 0x000000);
	SetBkMode(*pDC, TRANSPARENT);

	SetScrollRange(SB_VERT, 0, m_pParentWindow->m_pPartyLog->m_LogHeader.m_nEvents, TRUE);
	
	
	CString szDate = _T("");
	CString szEventDate = _T("");
	CString szLogLine = _T("");
	CString szAmount = _T("");
	CString szComment = _T("");

	int nLineY = 30;
	int nLineSize = 25;

	m_nEventStart = m_pParentWindow->m_pPartyLog->m_LogHeader.m_nEvents - m_nScrollPos;
	int nEnd = max(m_nEventStart - 100, 0);
	
	ClearPicks();

	int nEventCount = 0;
	for(int i = m_nEventStart; i >= nEnd; --i)
	{
		cDNDPartyLogEvent *pEvent = &m_pParentWindow->m_pPartyLog->m_Event[i];

		if(pEvent->m_EventType == DND_LOG_EVENT_TYPE_UNDEFINED)
			continue;

		m_nEvents[nEventCount] = i;  // here the fucker is

		int nMonth = (abs(pEvent->m_nMonth)) % m_pApp->m_pSelectedCalendar->m_nMonthsInYear;
		int nDay = pEvent->m_nDay + 1;

		szEventDate.Format("The %d%s day of %s, in the year %d", nDay, GetNumberSuffix(nDay), m_pApp->m_pSelectedCalendar->m_szMonthNames[nMonth], pEvent->m_nYear);

		if(szEventDate != szDate)
		{
			nLineSize = DrawLogText(szEventDate.GetBuffer(0), 30, nLineY, pDC);
			szDate = szEventDate;
			nLineY += nLineSize;
		}

		int nMinutes = pEvent->m_nTurn * 10 + pEvent->m_nRound;
		int nSeconds = pEvent->m_nSegment * 6;

		//continue;

#if 0 // #ifdef _DEBUG
		szLogLine.Format("(%d- (%d:%d)) %02d:%02d:%02d %s", i, nEventCount, m_nPickRects, pEvent->m_nHour, nMinutes, nSeconds, GetLogEventDesc(pEvent->m_EventType));
#else
		szLogLine.Format("%02d:%02d:%02d %s", pEvent->m_nHour, nMinutes, nSeconds, GetLogEventDesc(pEvent->m_EventType));
#endif

		//continue;

		szLogLine.Replace("$PNAME", m_pParentWindow->m_pParty->m_szPartyName);
		szLogLine.Replace("$CNAME", pEvent->m_szName);

		if(szLogLine.Find("$REALDATE") > -1)
		{
			struct tm * timeinfo;
			timeinfo = localtime ((const time_t *)&pEvent->m_tEventID);

			CString szRealDate;
			szRealDate.Format("%s", asctime(timeinfo));

			szLogLine.Replace("$REALDATE", szRealDate);
		}

		szAmount.Format("%ld", pEvent->m_lAmount);
		szLogLine.Replace("$AMOUNT", szAmount);

		szComment.Format(" %s", pEvent->m_szComment);

		szLogLine += szComment;

		nLineSize = DrawLogText(szLogLine.GetBuffer(0), 50, nLineY, pDC);

		//continue;
		TRACE("LOG PAINT %d ERR %d %d\n", i, m_nEventStart, m_nPickRects);

		m_PickRect[m_nPickRects].left = 30;
		m_PickRect[m_nPickRects].top = nLineY;
		m_PickRect[m_nPickRects].right = rect.right-80;

		nLineY += nLineSize;

		m_PickRect[m_nPickRects].bottom = nLineY;

		//continue;

		if(m_nPickRects == m_nPickedRect)
		{
			Pen pen(Color(255, 128,128,128), 3);
			graphics.DrawLine(&pen, m_PickRect[m_nPickRects].left, m_PickRect[m_nPickRects].top, m_PickRect[m_nPickRects].right, m_PickRect[m_nPickRects].top); // top line
			graphics.DrawLine(&pen, m_PickRect[m_nPickRects].left, m_PickRect[m_nPickRects].top, m_PickRect[m_nPickRects].left, m_PickRect[m_nPickRects].bottom); // left line
			graphics.DrawLine(&pen, m_PickRect[m_nPickRects].left, m_PickRect[m_nPickRects].bottom, m_PickRect[m_nPickRects].right, m_PickRect[m_nPickRects].bottom); // bottom line
			graphics.DrawLine(&pen, m_PickRect[m_nPickRects].right, m_PickRect[m_nPickRects].top, m_PickRect[m_nPickRects].right, m_PickRect[m_nPickRects].bottom); // right line
		}

		m_nPickRects += 1;

		if(m_nPickRects >= 99)
		{
			m_nPickRects = 99;
		}

		++nEventCount;

	} // for(int i = m_nEventStart; i >= nEnd; --i)

	pDC->SelectObject(&oldfont);

}

int CDMPartyLogDialog::DrawLogText(char * szData, int x, int y, CDC *memdc)
{
	CRect rect;
	GetClientRect(&rect);
	
	rect.left = x;
	rect.top = y;

	rect.right = rect.right - rect.left + 30;
	rect.bottom = rect.top + 500;

	return memdc->DrawText(szData, &rect, DT_LEFT | DT_WORDBREAK );
}

void CDMPartyLogDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	InvalidateRect(NULL);
}


void CDMPartyLogDialog::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateData(FALSE);

	switch(nSBCode)
	{
		case SB_LINEUP:
		{
			m_nScrollPos -= 1;

			if(m_nScrollPos < 0)
				m_nScrollPos = 0;

			m_nPickedRect = -1;
			m_nPickedEvent = -1;

			TRACE("UP %d %d\n", nSBCode, m_nScrollPos);
			InvalidateRect(NULL);
			//m_bIsDirty = TRUE;

			break;
		}
		case SB_LINEDOWN:
		{
			m_nScrollPos += 1;

			if(m_nScrollPos >= MAX_LOG_EVENTS)
				m_nScrollPos = MAX_LOG_EVENTS-1;

			m_nPickedRect = -1;
			m_nPickedEvent = -1;

			TRACE("DN %d %d\n", nSBCode, m_nScrollPos);
			InvalidateRect(NULL);
			//m_bIsDirty = TRUE;

			break;
		}
		case SB_THUMBTRACK:
		case SB_THUMBPOSITION:
		{
			m_nScrollPos = nPos;

			m_nPickedRect = -1;
			m_nPickedEvent = -1;

			TRACE("TK %d %d\n", nSBCode, m_nScrollPos);
			InvalidateRect(NULL);
			//m_bIsDirty = TRUE;

			break;
		}
		default:
		{
			TRACE(">> %d %d\n", nSBCode, m_nScrollPos);

			CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
			break;
		}
	}	
}

void CDMPartyLogDialog::ClearPicks()
{
	for(int i = 0; i< 100; ++i)
	{
		m_PickRect[i].top = 0;
		m_PickRect[i].bottom = 0;
		m_PickRect[i].left = 0;
		m_PickRect[i].right = 0;
		
		m_nEvents[i] = -1;
	}

	m_nPickRects = 0;
}

void CDMPartyLogDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_nPickedRect >= 0)
	{
		m_nPickedRect = -1;
		m_nPickedEvent = -1;
		InvalidateRect(NULL);
		//m_bIsDirty = TRUE;
	}

	if(m_pParentWindow->m_pEventLoggerDialog != NULL && m_pParentWindow->m_pEventLoggerDialog->m_nInsertPosition != APPEND_TO_LOG)
	{
		m_pParentWindow->m_pEventLoggerDialog->m_nInsertPosition = APPEND_TO_LOG;
		m_pParentWindow->m_pEventLoggerDialog->Init();
	}
	
	CDialog::OnLButtonDown(nFlags, point);
}


void CDMPartyLogDialog::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	for(int i = 0; i < 100; ++i)
	{
		if(point.y >= m_PickRect[i].top && point.y <= m_PickRect[i].bottom)
		{
			if(point.x >= m_PickRect[i].left && point.x <= m_PickRect[i].right)
			{
				m_nPickedRect = i;
				m_nPickedEvent = m_nEvents[i];

				TRACE("PICKED EVENT %d\n", m_nPickedEvent);

				InvalidateRect(NULL);
				//m_bIsDirty = TRUE;

				break;
			}
		}
	}

	CDialog::OnLButtonDblClk(nFlags, point);
}


void CDMPartyLogDialog::OnBnClickedDeleteButton()
{
	if(m_nPickedEvent >= 0)
	{
		m_pParentWindow->m_pPartyLog->DeleteEvent(m_nPickedEvent);

		m_nPickedRect = -1;
		m_nPickedEvent = -1;
		InvalidateRect(NULL);
	}
}

void CDMPartyLogDialog::OnBnClickedInsertButton()
{
	if(m_nPickedEvent >= 0)
	{
		if(m_pParentWindow->m_pEventLoggerDialog == NULL)
		{
			m_pParentWindow->m_pEventLoggerDialog = new CDMEventLoggerDialog(m_pParentWindow);
		}

		m_pParentWindow->m_pEventLoggerDialog->m_nInsertPosition = m_nPickedEvent;
		m_pParentWindow->m_pEventLoggerDialog->Init();

		//m_nPickedRect = -1;
		//m_nPickedEvent = -1;
		InvalidateRect(NULL);
	}
}

void CDMPartyLogDialog::OnBnClickedEditButton()
{
	// TODO: Add your control notification handler code here
}

void CDMPartyLogDialog::OnBnClickedEraseButton()
{
	if(AfxMessageBox("Are you sure you wish to erase the party log ?", MB_ICONQUESTION|MB_OKCANCEL) == IDCANCEL)
	{
		return;
	}

	
	m_pParentWindow->m_pPartyLog->m_LogHeader.m_nEvents = 0;

	memset(m_pParentWindow->m_pPartyLog->m_Event, 0, sizeof(cDNDPartyLogEvent) * MAX_LOG_EVENTS);

	m_pParentWindow->m_pParty->MarkChanged();

	InvalidateRect(NULL);

}


BOOL CDMPartyLogDialog::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(zDelta > 0)
	{
		m_nScrollPos -= 1;

		if (m_nScrollPos < 0)
			m_nScrollPos = 0;

		m_nPickedRect = -1;
		m_nPickedEvent = -1;

		InvalidateRect(NULL);
		//m_bIsDirty = TRUE;
	}
	else if (zDelta < 0)
	{
		m_nScrollPos += 1;

		if (m_nScrollPos >= MAX_LOG_EVENTS)
			m_nScrollPos = MAX_LOG_EVENTS - 1;

		m_nPickedRect = -1;
		m_nPickedEvent = -1;

		InvalidateRect(NULL);
		//m_bIsDirty = TRUE;
	}

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


BOOL CDMPartyLogDialog::OnEraseBkgnd(CDC* pDC)
{
#if USE_PL_MEMDC
	return FALSE;
#else
	return CDialog::OnEraseBkgnd(pDC);
#endif
}


void CDMPartyLogDialog::OnTimer(UINT_PTR nIDEvent)
{
	#if 0
	if (nIDEvent == 9467)
	{
		if (m_bIsDirty)
		{
			InvalidateRect(NULL);
			m_bIsDirty = FALSE;
		}
	}
	#endif

	CDialog::OnTimer(nIDEvent);
}
