// DMTimeEditDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMInventoryDialog.h"
#include "DMCharSpellsDialog.h"
#include "DMCharViewDialog.h"
#include "DMNPCViewDialog.h"
#include "DMPartyDialog.h"
#include "cDMMapViewDialog.h"
#include "DMTimeEditDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DMTimeEditDialog dialog


DMTimeEditDialog::DMTimeEditDialog(cDNDParty *pParty, DMPartyDialog *pPartyDialog, cDMMapViewDialog *pMapDialog, CWnd* pParent /*=NULL*/)
	: CDialog(DMTimeEditDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(DMTimeEditDialog)
	m_szYear = _T("");
	m_szMonth = _T("");
	m_szDay = _T("");
	m_szHour = _T("");
	m_szTurn = _T("");
	m_szSegment = _T("");
	m_szRound = _T("");
	//}}AFX_DATA_INIT

	m_pPartyDialog = pPartyDialog;
	m_pMapDialog = pMapDialog;

	m_pParty = pParty;
}


void DMTimeEditDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DMTimeEditDialog)
	DDX_Text(pDX, IDC_YEAR_EDIT, m_szYear);
	DDX_Text(pDX, IDC_MONTH_EDIT, m_szMonth);
	DDX_Text(pDX, IDC_DAY_EDIT, m_szDay);
	DDX_Text(pDX, IDC_HOUR_EDIT, m_szHour);
	DDX_Text(pDX, IDC_TURN_EDIT, m_szTurn);
	DDX_Text(pDX, IDC_SEGMENT_EDIT, m_szSegment);
	DDX_Text(pDX, IDC_ROUND_EDIT, m_szRound);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_START_CLOCK_BUTTON, m_cStartClockButton);
}


BEGIN_MESSAGE_MAP(DMTimeEditDialog, CDialog)
	//{{AFX_MSG_MAP(DMTimeEditDialog)
	ON_BN_CLICKED(IDC_DEC_YEAR_BUTTON, OnDecYearButton)
	ON_BN_CLICKED(IDC_INC_YEAR_BUTTON, OnIncYearButton)
	ON_BN_CLICKED(IDC_DEC_MONTH_BUTTON, OnDecMonthButton)
	ON_BN_CLICKED(IDC_INC_MONTH_BUTTON, OnIncMonthButton)
	ON_BN_CLICKED(IDC_DEC_DAY_BUTTON, OnDecDayButton)
	ON_BN_CLICKED(IDC_INC_DAY_BUTTON, OnIncDayButton)
	ON_EN_CHANGE(IDC_YEAR_EDIT, OnChangeYearEdit)
	ON_EN_CHANGE(IDC_MONTH_EDIT, OnChangeMonthEdit)
	ON_EN_CHANGE(IDC_DAY_EDIT, OnChangeDayEdit)
	ON_EN_CHANGE(IDC_HOUR_EDIT, OnChangeHourEdit)
	ON_EN_CHANGE(IDC_TURN_EDIT, OnChangeTurnEdit)
	ON_EN_CHANGE(IDC_ROUND_EDIT, OnChangeRoundEdit)
	ON_EN_CHANGE(IDC_SEGMENT_EDIT, OnChangeSegmentEdit)
	ON_BN_CLICKED(IDC_DEC_HOUR_BUTTON, OnDecHourButton)
	ON_BN_CLICKED(IDC_INC_HOUR_BUTTON, OnIncHourButton)
	ON_BN_CLICKED(IDC_DEC_TURN_BUTTON, OnDecTurnButton)
	ON_BN_CLICKED(IDC_INC_TURN_BUTTON, OnIncTurnButton)
	ON_BN_CLICKED(IDC_DEC_ROUND_BUTTON, OnDecRoundButton)
	ON_BN_CLICKED(IDC_INC_ROUND_BUTTON, OnIncRoundButton)
	ON_BN_CLICKED(IDC_DEC_SEGMENT_BUTTON, OnDecSegmentButton)
	ON_BN_CLICKED(IDC_INC_SEGMENT_BUTTON, OnIncSegmentButton)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_START_CLOCK_BUTTON, &DMTimeEditDialog::OnBnClickedStartClockButton)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DMTimeEditDialog message handlers

BOOL DMTimeEditDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(m_pPartyDialog != NULL)
	{
		if(m_pPartyDialog->m_pPartyClockThread != NULL)
		{
			m_cStartClockButton.SetWindowText("Stop Clock");
		}
		else
		{
			m_cStartClockButton.SetWindowText("Start Clock");
		}
	}


	Refresh();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DMTimeEditDialog::Refresh()
{
	if(m_pParty->m_nSegment)
		m_szSegment.Format("%d", m_pParty->m_nSegment);
	else
		m_szSegment = _T("");

	if(m_pParty->m_nRound)
		m_szRound.Format("%d", m_pParty->m_nRound);
	else
		m_szRound = _T("");

	if(m_pParty->m_nTurn)
		m_szTurn.Format("%d", m_pParty->m_nTurn);
	else
		m_szTurn = _T("");

	if(m_pParty->m_nHour)
		m_szHour.Format("%d", m_pParty->m_nHour);
	else
		m_szHour = _T("");

	if(m_pParty->m_nDayofMonth > -1)
		m_szDay.Format("%d", m_pParty->m_nDayofMonth+1);
	else
		m_szDay = _T("");

	if(m_pParty->m_nMonth > -1)
		m_szMonth.Format("%d", m_pParty->m_nMonth+1);
	else
		m_szMonth = _T("");

	if(m_pParty->m_nYear)
		m_szYear.Format("%d", m_pParty->m_nYear);
	else
		m_szYear = _T("");

	if(m_pPartyDialog != NULL)
	{
		m_pPartyDialog->Refresh();
	}

	if(m_pMapDialog != NULL)
	{
		m_pMapDialog->Refresh();
	}

	UpdateData(FALSE);
}

void DMTimeEditDialog::OnOK() 
{
	CDialog::OnOK();
}

void DMTimeEditDialog::OnCancel() 
{
	CDialog::OnCancel();
}

void DMTimeEditDialog::OnDecYearButton() 
{
	m_pParty->ModifyCalendar(DND_CALENDAR_MOD_YEAR, DND_CALENDAR_MOD_DECREMENT);

	Refresh();
}

void DMTimeEditDialog::OnIncYearButton() 
{
	m_pParty->ModifyCalendar(DND_CALENDAR_MOD_YEAR, DND_CALENDAR_MOD_INCREMENT);

	Refresh();
}

void DMTimeEditDialog::OnDecMonthButton() 
{
	m_pParty->ModifyCalendar(DND_CALENDAR_MOD_MONTH, DND_CALENDAR_MOD_DECREMENT);

	Refresh();
}
	
void DMTimeEditDialog::OnIncMonthButton() 
{
	m_pParty->ModifyCalendar(DND_CALENDAR_MOD_MONTH, DND_CALENDAR_MOD_INCREMENT);

	Refresh();
	
}

void DMTimeEditDialog::OnDecDayButton() 
{
	m_pParty->ModifyCalendar(DND_CALENDAR_MOD_DAY, DND_CALENDAR_MOD_DECREMENT);

	Refresh();
	
}

void DMTimeEditDialog::OnIncDayButton() 
{
	m_pParty->ModifyCalendar(DND_CALENDAR_MOD_DAY, DND_CALENDAR_MOD_INCREMENT);

	Refresh();
	
}


void DMTimeEditDialog::OnDecHourButton() 
{
	m_pParty->ModifyCalendar(DND_CALENDAR_MOD_HOUR, DND_CALENDAR_MOD_DECREMENT);

	Refresh();
}

void DMTimeEditDialog::OnIncHourButton() 
{
	m_pParty->ModifyCalendar(DND_CALENDAR_MOD_HOUR, DND_CALENDAR_MOD_INCREMENT);

	Refresh();
	
}

void DMTimeEditDialog::OnDecTurnButton() 
{
	m_pParty->ModifyCalendar(DND_CALENDAR_MOD_TURN, DND_CALENDAR_MOD_DECREMENT);

	Refresh();
}

void DMTimeEditDialog::OnIncTurnButton() 
{
	m_pParty->ModifyCalendar(DND_CALENDAR_MOD_TURN, DND_CALENDAR_MOD_INCREMENT);

	Refresh();
}

void DMTimeEditDialog::OnDecRoundButton() 
{
	m_pParty->ModifyCalendar(DND_CALENDAR_MOD_ROUND, DND_CALENDAR_MOD_DECREMENT);

	Refresh();
	
}

void DMTimeEditDialog::OnIncRoundButton() 
{
	m_pParty->ModifyCalendar(DND_CALENDAR_MOD_ROUND, DND_CALENDAR_MOD_INCREMENT);

	Refresh();
	
}

void DMTimeEditDialog::OnDecSegmentButton() 
{
	m_pParty->ModifyCalendar(DND_CALENDAR_MOD_SEGMENT, DND_CALENDAR_MOD_DECREMENT);

	Refresh();
	
}

void DMTimeEditDialog::OnIncSegmentButton() 
{
	m_pParty->ModifyCalendar(DND_CALENDAR_MOD_SEGMENT, DND_CALENDAR_MOD_INCREMENT);

	Refresh();
}


void DMTimeEditDialog::OnChangeYearEdit() 
{
	UpdateData(TRUE);

	int nVal = atoi(m_szYear.GetBuffer(0));

	m_pParty->ModifyCalendar(DND_CALENDAR_MOD_YEAR, DND_CALENDAR_MOD_SET, nVal);

	Refresh();
	
}

void DMTimeEditDialog::OnChangeMonthEdit() 
{
	UpdateData(TRUE);

	int nVal = atoi(m_szMonth.GetBuffer(0))-1;

	if(nVal >= 0)
	{
		m_pParty->ModifyCalendar(DND_CALENDAR_MOD_MONTH, DND_CALENDAR_MOD_SET, nVal);
		Refresh();
	}
	
}

void DMTimeEditDialog::OnChangeDayEdit() 
{
	UpdateData(TRUE);

	int nVal = atoi(m_szDay.GetBuffer(0))-1;

	if(nVal >= 0)
	{
		m_pParty->ModifyCalendar(DND_CALENDAR_MOD_DAY, DND_CALENDAR_MOD_SET, nVal);
		Refresh();
	}

}

void DMTimeEditDialog::OnChangeHourEdit() 
{
	UpdateData(TRUE);

	int nVal = atoi(m_szHour.GetBuffer(0));

	if(nVal >= 0)
	{
		m_pParty->ModifyCalendar(DND_CALENDAR_MOD_HOUR, DND_CALENDAR_MOD_SET, nVal);
		Refresh();
	}
	
}

void DMTimeEditDialog::OnChangeTurnEdit() 
{
	UpdateData(TRUE);

	int nVal = atoi(m_szTurn.GetBuffer(0));

	if(nVal >= 0)
	{
		m_pParty->ModifyCalendar(DND_CALENDAR_MOD_TURN, DND_CALENDAR_MOD_SET, nVal);
		Refresh();
	}
}

void DMTimeEditDialog::OnChangeRoundEdit() 
{
	UpdateData(TRUE);

	int nVal = atoi(m_szRound.GetBuffer(0));

	if(nVal >= 0)
	{
		m_pParty->ModifyCalendar(DND_CALENDAR_MOD_ROUND, DND_CALENDAR_MOD_SET, nVal);
		Refresh();
	}
	
}

void DMTimeEditDialog::OnChangeSegmentEdit() 
{
	UpdateData(TRUE);

	int nVal = atoi(m_szSegment.GetBuffer(0));

	if(nVal >= 0)
	{
		m_pParty->ModifyCalendar(DND_CALENDAR_MOD_SEGMENT, DND_CALENDAR_MOD_SET, nVal);
		Refresh();
	}
	
}

void DMTimeEditDialog::OnBnClickedStartClockButton()
{
	if(m_pPartyDialog != NULL)
	{
		if(m_pPartyDialog->m_pPartyClockThread == NULL)
		{
			m_pPartyDialog->m_pPartyClockThread = AfxBeginThread(DMPartyTimeThreadProc, m_pPartyDialog);
			m_cStartClockButton.SetWindowText("Stop Clock");
		}
		else
		{
			m_pPartyDialog->m_pPartyClockThread = NULL;
			m_cStartClockButton.SetWindowText("Start Clock");
		}
	}

	InvalidateRect(NULL);
}
