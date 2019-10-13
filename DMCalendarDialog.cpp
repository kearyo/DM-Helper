// DMCalendarDialog.cpp : implementation file
//

#include "stdafx.h"
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
#include "DMCalendarDialog.h"

#define MOON_SIZE 32  // was 24
#define MOON_OFFSET (MOON_SIZE+4)

// CDMCalendarDialog dialog

IMPLEMENT_DYNAMIC(CDMCalendarDialog, CDialog)

CDMCalendarDialog::CDMCalendarDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDMCalendarDialog::IDD, pParent)
{

	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_pCalendar = m_pApp->m_pSelectedCalendar;

	m_pParentWindow = (DMPartyDialog *)pParent;

	Create(CDMCalendarDialog::IDD);
}

CDMCalendarDialog::~CDMCalendarDialog()
{
}

void CDMCalendarDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PREV_BUTTON, m_cPrevMonthButton);
	DDX_Control(pDX, IDC_NEXT_BUTTON, m_cNextMonthButton);
}


BEGIN_MESSAGE_MAP(CDMCalendarDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CDMCalendarDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDMCalendarDialog::OnBnClickedCancel)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_PREV_BUTTON, &CDMCalendarDialog::OnBnClickedPrevButton)
	ON_BN_CLICKED(IDC_NEXT_BUTTON, &CDMCalendarDialog::OnBnClickedNextButton)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()


// CDMCalendarDialog message handlers

BOOL CDMCalendarDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cPrevMonthButton.LoadBitmaps(IDB_LARGE_LEFT_BLACK_BITMAP, IDB_LARGE_LEFT_BLACK_BITMAP);
	m_cNextMonthButton.LoadBitmaps(IDB_LARGE_RIGHT_BLACK_BITMAP, IDB_LARGE_RIGHT_BLACK_BITMAP);

	m_pCellBitmap = Bitmap::FromResource(AfxGetApp()->m_hInstance, MAKEINTRESOURCEW(IDB_CALENDAR_CELL_BITMAP));
	m_pMoonsBitmap = Bitmap::FromResource(AfxGetApp()->m_hInstance, MAKEINTRESOURCEW(IDB_MOON_CELLS_BITMAP));
	//m_pMoonsBitmap->MakeTransparent(0x00FFFFFF);

	m_nCurrentYear = m_pParentWindow->m_pParty->m_nYear;
	m_nCurrentMonth = m_pParentWindow->m_pParty->m_nMonth;
	m_nCurrentDay = m_pParentWindow->m_pParty->m_nDayofMonth;

	m_nStartYear = m_pParentWindow->m_pParty->m_nYear;
	m_nStartMonth = m_pParentWindow->m_pParty->m_nMonth;
	m_nStartDay = m_pParentWindow->m_pParty->m_nDayofMonth;

	m_nMonthStartDayOfWeek = m_pCalendar->FindDayOfWeek(m_nCurrentMonth, 0, m_nCurrentYear);

	memset(m_fMoonCycleStart, 0, MAX_CALENDAR_MOONS*sizeof(float));
	for(int i = 0; i < m_pCalendar->m_nMoons; ++i)
	{
		m_fMoonCycleStart[i] = m_pCalendar->GetMoonCycleDay(i, m_nCurrentMonth, 0, m_nCurrentYear);
	}

	m_nPickers = 0;

	#if GAMETABLE_BUILD
	for (int i = 0; i < 20; ++i)
	{
		m_szRealMonthNumber[i] = "-";
	}
	int nRealMonth = 1;
	for (int i = 0; i < m_pCalendar->m_nMonthsInYear; ++i)
	{
		if (m_pCalendar->m_nDaysInMonth[i] < 28)
			continue;

		m_szRealMonthNumber[i].Format("%02d", nRealMonth);
		++nRealMonth;
	}
	#endif

	ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDMCalendarDialog::Cleanup()
{
	m_pParentWindow->m_pCalendarDialog = NULL;

	delete m_pCellBitmap;
	delete m_pMoonsBitmap;

	delete this;
}

void CDMCalendarDialog::OnBnClickedOk()
{	
	OnOK();

	Cleanup();
}

void CDMCalendarDialog::OnBnClickedCancel()
{
	OnCancel();

	Cleanup();
}

void CDMCalendarDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CDC memdc;
	CDC* pDC;
	pDC = &dc;

	
	CString szTemp;

	int nCX = 10;
	int nCY = 120;

	int nCellSizeX = 128;
	int nCellSizeY = 128;

	CRect Rect;
	GetClientRect (Rect);

	m_cNextMonthButton.SetWindowPos(NULL, Rect.right-22, 5, 25, 25, SW_SHOW);

	int nRoomX = Rect.right - nCX;
	int nRoomY = Rect.bottom - nCY;

	int nCenterX = ((Rect.right - Rect.left) / 2) + Rect.left;

	float fSizeX = ((float)nRoomX) / m_pCalendar->m_nDaysInWeek;

	int nCells = m_nMonthStartDayOfWeek + m_pCalendar->m_nDaysInMonth[m_nCurrentMonth];

	int nRows = nCells / m_pCalendar->m_nDaysInWeek;
	if(nCells % m_pCalendar->m_nDaysInWeek)
	{
		nRows +=1;
	}

	if(nRows == 1) //Greyhawk festival week weirdness
	{
		nRows = 4;
	}

	float fSizeY = ((float)nRoomY) / nRows;

	float fScaleX = fSizeX / nCellSizeX;
	float fScaleY = fSizeY / nCellSizeY;

	if(fScaleX < 0.85f)
	{
		fScaleX = 0.85f;
	}

	if(fScaleY < 0.65f)
	{
		fScaleY = 0.65f;
	}

	//nCellSizeX *= fScaleX;
	nCellSizeX = (int)((float)nCellSizeX * fScaleX);
	//nCellSizeY *= fScaleY;
	nCellSizeY = (int)((float)nCellSizeY * fScaleY);

	Graphics graphics(dc);

	// Create a compatible memory DC
	memdc.CreateCompatibleDC( pDC );

	//Color backColor(255, 214, 206, 131);
	Color backColor(255, 0, 0, 0);
	//Color backColor(255, 255, 255, 255);
	graphics.Clear(backColor);

	CFont newfont;
	CFont newfont2;
	CFont newfont3;
	CFont* oldfont;

	int nSize = 16;
	int success = newfont.CreatePointFont(nSize * 10, "HamletOrNot", pDC);
	
	oldfont = dc.SelectObject(&newfont);

	SetBkColor(dc, 0x000000);
	SetTextColor(dc, 0x83CED6);
	SetBkMode(dc, TRANSPARENT);

	for(int i = 0; i < m_pCalendar->m_nDaysInWeek; ++i)
	{
		szTemp.Format("%s", m_pCalendar->m_szDayNames[i]);
		DrawCalendarText(szTemp.GetBuffer(0), 15+i*nCellSizeX, 85, &dc);
	}

	nSize = 36;
	//int success = newfont.CreatePointFont(nSize * 10, "HamletOrNot", pDC);
	success = newfont2.CreatePointFont(nSize * 10, "Cloister Black", pDC);
	

	oldfont = dc.SelectObject(&newfont2);

	//did not change color on purpose here

//#ifdef _DEBUG
//	szTemp.Format("%s, %d (%d)", m_pCalendar->m_szMonthNames[m_nCurrentMonth], m_nCurrentYear, nRows);
//#else
#if GAMETABLE_BUILD
	szTemp.Format("%s of %d (%s/%d)", m_pCalendar->m_szMonthNames[m_nCurrentMonth], m_nCurrentYear, m_szRealMonthNumber[m_nCurrentMonth], m_nCurrentYear);
	int nLabelSize = (20 * szTemp.GetLength()) / 2;
#else
	szTemp.Format("%s of %d", m_pCalendar->m_szMonthNames[m_nCurrentMonth], m_nCurrentYear);
	int nLabelSize = (20 * szTemp.GetLength()) / 2;
#endif
//#endif

	//DrawCalendarText(szTemp.GetBuffer(0), 40, 15, &dc);
	DrawCalendarText(szTemp.GetBuffer(0), abs(nCenterX - nLabelSize), 15, &dc);
	//DrawCalendarText("l", abs(nCenterX), 1, &dc);


	SetBkColor(dc, 0x83CED6);
	SetTextColor(dc, 0x000000);
	SetBkMode(dc, TRANSPARENT);

	int nDayColumns = 0;
	int nMonthDay = 1;

	int nSelectedX = 0;
	int nSelectedY = 0;
	int nSelectedNum = 0;

	m_nPickers = 0;

	float fMoonDay[MAX_CALENDAR_MOONS];
	float fSelectedMoonDay[MAX_CALENDAR_MOONS];
	for(int j = 0; j < m_pCalendar->m_nMoons; ++j)
	{
		fMoonDay[j] = m_fMoonCycleStart[j];
		fSelectedMoonDay[j] = 0.0f;
	}
	 

	for(int i = 0; ; ++i)
	{
		if(i >= m_nMonthStartDayOfWeek)
		{
			if(nMonthDay == m_nStartDay+1 && m_nCurrentMonth == m_nStartMonth && m_nCurrentYear == m_nStartYear)
			{
				nSelectedX = nCX-20;
				nSelectedY = nCY-20;

				nSelectedNum = nMonthDay;

				for(int j = 0; j < m_pCalendar->m_nMoons; ++j)
				{
					fSelectedMoonDay[j] = fMoonDay[j];
				}
			}
			else
			{
				graphics.DrawImage(m_pCellBitmap, nCX, nCY, nCellSizeX, nCellSizeY);

				szTemp.Format("%d", nMonthDay);
				DrawCalendarText(szTemp.GetBuffer(0), nCX+20, nCY+10, &dc);	

				int nDrawnMoons = 0;
				for(int j = 0; j < m_pCalendar->m_nMoons; ++j)
				{
					int nMoonIcon = GetMoonPhaseIcon(j, m_pCalendar->GetMoonPhase(j, fMoonDay[j]));

					if(nMoonIcon > -1)
					{
						int nLabelLength = szTemp.GetLength();
						DrawMoon(&graphics, nMoonIcon, nCX+nCellSizeX-55-MOON_OFFSET*nDrawnMoons, nCY+20, MOON_SIZE);
						++nDrawnMoons;
					}
				}
			}

			m_Picker[m_nPickers].m_nX1 = nCX;
			m_Picker[m_nPickers].m_nY1 = nCY;

			m_Picker[m_nPickers].m_nX2 = nCX+nCellSizeX;
			m_Picker[m_nPickers].m_nY2 = nCY+nCellSizeY;

			m_Picker[m_nPickers].m_nDay = nMonthDay;
			m_Picker[m_nPickers].m_nMonth = m_nCurrentMonth;
			m_Picker[m_nPickers].m_nYear = m_nCurrentYear;


			++m_nPickers;

			m_Picker[m_nPickers].Reset();

			nMonthDay += 1;

			for(int j = 0; j < m_pCalendar->m_nMoons; ++j)
			{
				fMoonDay[j] = m_pCalendar->IncrementMoonDay(j, fMoonDay[j]);
			}

		}

		nCX+=nCellSizeX;
		nDayColumns += 1;

		if(nDayColumns >= m_pCalendar->m_nDaysInWeek)
		{
			nDayColumns = 0;
			nCX = 10;
			nCY += nCellSizeY;
		}

		if(nMonthDay > m_pCalendar->m_nDaysInMonth[m_nCurrentMonth])
		{
			break;
		}
	}

	if(nSelectedNum)
	{
		graphics.DrawImage(m_pCellBitmap, nSelectedX, nSelectedY, nCellSizeX+40, nCellSizeY+40);

		szTemp.Format("%d", nSelectedNum);
		DrawCalendarText(szTemp.GetBuffer(0), nSelectedX+30, nSelectedY+20, &dc);	

		int nDrawnMoons = 0;
		for(int j = 0; j < m_pCalendar->m_nMoons; ++j)
		{
			int nMoonIcon = GetMoonPhaseIcon(j, m_pCalendar->GetMoonPhase(j, fSelectedMoonDay[j]));

			if(nMoonIcon > -1)
			{
				//DrawMoon(&graphics, nMoonIcon, nSelectedX+nCellSizeX+40-74-66*nDrawnMoons, nSelectedY+20, MOON_SIZE);
				DrawMoon(&graphics, nMoonIcon, nSelectedX+nCellSizeX+30-46-MOON_OFFSET*nDrawnMoons, nSelectedY+26, MOON_SIZE);
				++nDrawnMoons;
			}
		}

		Pen pen(Color(255, 128,128,128), 3);

		graphics.DrawLine(&pen, nSelectedX, nSelectedY, nSelectedX+nCellSizeX+40, nSelectedY); // top line
		graphics.DrawLine(&pen, nSelectedX, nSelectedY, nSelectedX, nSelectedY+nCellSizeY+40); // left line
		graphics.DrawLine(&pen, nSelectedX, nSelectedY+nCellSizeY+40, nSelectedX+nCellSizeX+40, nSelectedY+nCellSizeY+40); // bottom line
		graphics.DrawLine(&pen, nSelectedX+nCellSizeX+40, nSelectedY, nSelectedX+nCellSizeX+40, nSelectedY+nCellSizeY+40); // right line
	
	}

	/////////////////////////////////////////
	//Do it again with a different font to write events on the calendar
	/////////////////////////////////////////

	nCX = 10;
	nCY = 120;

	nSize = 10;
	success = newfont3.CreatePointFont(nSize * 10, "HamletOrNot", pDC);
	
	oldfont = dc.SelectObject(&newfont3);

	nDayColumns = 0;
	nMonthDay = 1;

	CString szDayText = _T("");
	CString szTodayText = _T("");

	for(int j = 0; j < m_pCalendar->m_nMoons; ++j)
	{
		fMoonDay[j] = m_fMoonCycleStart[j];
		fSelectedMoonDay[j] = 0.0f;
	}
	 

	for(int i = 0; ; ++i)
	{
		if(i >= m_nMonthStartDayOfWeek)
		{
			for(int j = 0; j < m_pCalendar->m_nNumHolidays; ++j)
			{
				if(m_pCalendar->m_Holidays[j].m_nMonth == m_nCurrentMonth + 1 && m_pCalendar->m_Holidays[j].m_nDay == nMonthDay)
				{
					szTemp.Format("%s\n", m_pCalendar->m_Holidays[j].m_szDesc);
					szTemp.Replace("*", "\n");

					szDayText += szTemp;
				}
			}

			BOOL bFirstBirthday = TRUE;
			for(int j = 0; m_pParentWindow->m_pParty->m_dwPartyRoster[j] != 0; ++j)
			{
				PDNDCHARVIEWDLG pCharDlg = NULL;
				m_pApp->m_CharacterViewMap.Lookup((WORD)m_pParentWindow->m_pParty->m_dwPartyRoster[j], pCharDlg);

				if(pCharDlg != NULL && pCharDlg->m_pCharacter != NULL)
				{
					if(pCharDlg->m_pCharacter->m_nDOBMonth == m_nCurrentMonth+1 && pCharDlg->m_pCharacter->m_nDOBDay == nMonthDay)
					{
						if(bFirstBirthday)
						{
							szDayText += _T("Birthdays:\n");
							bFirstBirthday = FALSE;
						}
						szTemp.Format("%s\n", pCharDlg->m_pCharacter->m_szCharacterName);
						szDayText += szTemp;
					}
				}
			}

			int nDrawnMoons = 0;
			for(int j = 0; j < m_pCalendar->m_nMoons; ++j)
			{
				int nMoonPhase = m_pCalendar->GetMoonPhase(j, fMoonDay[j]);
				int nMoonIcon = GetMoonPhaseIcon(j, nMoonPhase);

				if(nMoonIcon > -1)
				{
					szTemp.Format("%s, %s\n", m_pCalendar->m_szMoonName[j], m_pCalendar->m_szMoonPhaseName[j][nMoonPhase]);
					szDayText += szTemp;
				}
			}

			if(nMonthDay == m_nStartDay+1 && m_nCurrentMonth == m_nStartMonth && m_nCurrentYear == m_nStartYear)
			{
				
			}
			else
			{
				//DrawCalendarText(szTemp.GetBuffer(0), nCX+20, nCY+10, &dc);	
			}

			if(szDayText != _T(""))
			{
				SetBkColor(dc, 0x000000);
				SetTextColor(dc, 0xFFFFFF);
				SetBkMode(dc, TRANSPARENT);

				DrawCalendarText(szDayText.GetBuffer(0), nCX+20, nCY+60, &dc);

				SetBkColor(dc, 0x000000);
				SetTextColor(dc, 0x000000);
				SetBkMode(dc, TRANSPARENT);

				DrawCalendarText(szDayText.GetBuffer(0), nCX+21, nCY+61, &dc);
			}

			nMonthDay += 1;

			for(int j = 0; j < m_pCalendar->m_nMoons; ++j)
			{
				fMoonDay[j] = m_pCalendar->IncrementMoonDay(j, fMoonDay[j]);
			}
			
			szDayText = _T("");

		}

		nCX+=nCellSizeX;
		nDayColumns += 1;

		if(nDayColumns >= m_pCalendar->m_nDaysInWeek)
		{
			nDayColumns = 0;
			nCX = 10;
			nCY += nCellSizeY;
		}

		if(nMonthDay > m_pCalendar->m_nDaysInMonth[m_nCurrentMonth])
		{
			break;
		}
	}

}

void CDMCalendarDialog::OnClose()
{
	CDialog::OnClose();

	Cleanup();
}

void CDMCalendarDialog::DrawCalendarText(char * szData, int x, int y, CDC *memdc)
{

	CRect rect;
	GetClientRect(&rect);
	
	rect.left = x;
	rect.top = y;
	rect.right = rect.left + 700;
	rect.bottom = rect.top + 500;

	rect.left = x;
	rect.top = y;

	//DT_SINGLELINE
	memdc->DrawText(szData, &rect, DT_LEFT );

}

void CDMCalendarDialog::DrawMoon(Graphics *pg, int nCell, int nX, int nY, int nSize)
{
	RectF dst;

	dst.X = (Gdiplus::REAL)nX;
	dst.Y = (Gdiplus::REAL)nY;
	dst.Width = (Gdiplus::REAL)nSize;
	dst.Height = (Gdiplus::REAL)nSize;

	ImageAttributes attr;
	attr.SetColorKey( Color(255, 0, 255), Color(255, 0, 255), ColorAdjustTypeBitmap);

	int nCellX = nCell / 4;
	int nCellY = nCell % 4;

	int nSourceX = nCellX * 65 + 1;
	int nSourceY = nCellY * 65 + 1;

/*
	Status DrawImage(IN Image* image,
                     IN const RectF& destRect,
                     IN REAL srcx,
                     IN REAL srcy,
                     IN REAL srcwidth,
                     IN REAL srcheight,
                     IN Unit srcUnit,
                     IN const ImageAttributes* imageAttributes = NULL,
                     IN DrawImageAbort callback = NULL,
                     IN VOID* callbackData = NULL)
*/

	pg->DrawImage(m_pMoonsBitmap, dst, (Gdiplus::REAL)nSourceX, (Gdiplus::REAL)nSourceY, (Gdiplus::REAL)63, (Gdiplus::REAL)63, UnitPixel, &attr); //, UnitPixel);
	//graphics.DrawImage(m_pMoonsBitmap, nSelectedX+90, nSelectedY+10, 1, 1+130, 63, 63, UnitPixel);
	//graphics.DrawImage(m_pMoonsBitmap, nSelectedX, nSelectedY, 64, 64);

}

int CDMCalendarDialog::GetMoonPhaseIcon(int nMoon, int nMoonPhase)
{
	if(nMoonPhase < 0 || nMoonPhase >= MAX_CALENDAR_MOON_PHASES)
	{
		return -1;
	}

	if(m_pCalendar->m_nMoonPhaseSymbol[nMoon][nMoonPhase])
	{
		int nIcon = m_pCalendar->m_nMoonPhaseSymbol[nMoon][nMoonPhase] - 1;
		nIcon += nMoon * 4;
		return nIcon;
	}

	return -1;
}

void CDMCalendarDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	InvalidateRect(NULL);
}

void CDMCalendarDialog::OnBnClickedPrevButton()
{
	m_nCurrentMonth -= 1;

	if(m_nCurrentMonth < 0)
	{
		m_nCurrentMonth = m_pCalendar->m_nMonthsInYear-1;
		--m_nCurrentYear;

		if(m_nCurrentYear < m_pCalendar->m_nBaseYear)
		{
			m_nCurrentYear = m_pCalendar->m_nBaseYear;
			m_nCurrentMonth = 0;
		}
	}

	m_nMonthStartDayOfWeek = m_pCalendar->FindDayOfWeek(m_nCurrentMonth, 0, m_nCurrentYear);

	memset(m_fMoonCycleStart, 0, MAX_CALENDAR_MOONS*sizeof(float));
	for(int i = 0; i < m_pCalendar->m_nMoons; ++i)
	{
		m_fMoonCycleStart[i] = m_pCalendar->GetMoonCycleDay(i, m_nCurrentMonth, 0, m_nCurrentYear);
	}

	InvalidateRect(NULL);	
}

void CDMCalendarDialog::OnBnClickedNextButton()
{
	m_nCurrentMonth += 1;

	if(m_nCurrentMonth >= m_pCalendar->m_nMonthsInYear)
	{
		m_nCurrentMonth = 0;
		m_nCurrentYear += 1;

		if(m_nCurrentYear > m_pCalendar->m_nBaseYear + 50)
		{
			m_nCurrentYear = m_pCalendar->m_nBaseYear + 50;
		}
	}

	m_nMonthStartDayOfWeek = m_pCalendar->FindDayOfWeek(m_nCurrentMonth, 0, m_nCurrentYear);

	memset(m_fMoonCycleStart, 0, MAX_CALENDAR_MOONS*sizeof(float));
	for(int i = 0; i < m_pCalendar->m_nMoons; ++i)
	{
		m_fMoonCycleStart[i] = m_pCalendar->GetMoonCycleDay(i, m_nCurrentMonth, 0, m_nCurrentYear);
	}

	InvalidateRect(NULL);
}

void CDMCalendarDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	for(int i = 0; i < m_nPickers; ++i)
	{
		if(point.x >= m_Picker[i].m_nX1 && point.x <= m_Picker[i].m_nX2)
		{
			if(point.y >= m_Picker[i].m_nY1 && point.y <= m_Picker[i].m_nY2)
			{
				m_nStartDay = m_Picker[i].m_nDay - 1;
				m_nStartMonth = m_Picker[i].m_nMonth;
				m_nStartYear = m_Picker[i].m_nYear;
				InvalidateRect(NULL);
				break;
			}
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CDMCalendarDialog::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	for(int i = 0; i < m_nPickers; ++i)
	{
		if(point.x >= m_Picker[i].m_nX1 && point.x <= m_Picker[i].m_nX2)
		{
			if(point.y >= m_Picker[i].m_nY1 && point.y <= m_Picker[i].m_nY2)
			{
				m_nStartDay = m_Picker[i].m_nDay - 1;
				m_nStartMonth = m_Picker[i].m_nMonth;
				m_nStartYear = m_Picker[i].m_nYear;

				if(AfxMessageBox("Change the party date to this day ?", MB_YESNO) == IDYES)
				{
					m_pParentWindow->m_pParty->m_nDayofMonth = m_nStartDay;
					m_pParentWindow->m_pParty->m_nMonth = m_nStartMonth;
					m_pParentWindow->m_pParty->m_nYear = m_nStartYear;
					m_pParentWindow->m_pParty->m_nDayofWeek = m_pApp->m_pSelectedCalendar->FindDayOfWeek(m_nStartMonth, m_nStartDay, m_nStartYear);

					m_pParentWindow->m_pParty->MarkChanged();
					m_pParentWindow->Refresh();
				}

				InvalidateRect(NULL);
				break;
			}
		}
	}

	CDialog::OnLButtonDblClk(nFlags, point);
}
