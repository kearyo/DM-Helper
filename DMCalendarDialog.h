#pragma once
#include "afxwin.h"


class CDMCalendarDayPicker
{
public:

	int m_nX1;
	int m_nY1;

	int m_nX2;
	int m_nY2;

	int m_nMonth;
	int m_nDay;
	int m_nYear;

	void Reset()
	{
		m_nX1 = -1;
		m_nY1 = -1;

		m_nX2 = -1;
		m_nY2 = -1;

		m_nMonth = 0;
		m_nDay = 0;
		m_nYear = 0;
	};

	void CopyTo(CDMCalendarDayPicker *pDest)
	{
		memcpy(pDest, this, sizeof(CDMCalendarDayPicker));
	}

	CDMCalendarDayPicker()
	{
		Reset();
	};

	~CDMCalendarDayPicker()
	{
	};

};

// CDMCalendarDialog dialog

class DMPartyDialog;

class CDMCalendarDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMCalendarDialog)

public:
	CDMCalendarDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMCalendarDialog();

	void Cleanup();

	void DrawCalendarText(char * szData, int x, int y, CDC *memdc);
	void DrawMoon(Graphics *pg, int nCell, int nX, int nY, int nSize);
	int GetMoonPhaseIcon(int nMoon, int nMoonPhase);

	CDMHelperApp *m_pApp;
	DMPartyDialog *m_pParentWindow;
	PCALENDAR	m_pCalendar;

	Bitmap* m_pCellBitmap;
	Bitmap* m_pMoonsBitmap;
	//Image *m_pMoonsImage;

	int m_nCurrentYear;
	int m_nCurrentMonth;
	int m_nCurrentDay;

	int m_nStartYear;
	int m_nStartMonth;
	int m_nStartDay;

	int m_nMonthStartDayOfWeek;
	float m_fMoonCycleStart[MAX_CALENDAR_MOONS];

	int m_nPickers;
	CDMCalendarDayPicker m_Picker[200];

	#if GAMETABLE_BUILD
	CString m_szRealMonthNumber[20];
	#endif

// Dialog Data
	enum { IDD = IDD_CALENDAR_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
public:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedPrevButton();
	afx_msg void OnBnClickedNextButton();
	CBitmapButton m_cPrevMonthButton;
	CBitmapButton m_cNextMonthButton;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
};
