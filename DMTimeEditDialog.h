#include "afxwin.h"
#if !defined(AFX_DMTIMEEDITDIALOG_H__B558B840_7E3B_4137_945C_7AE65122534F__INCLUDED_)
#define AFX_DMTIMEEDITDIALOG_H__B558B840_7E3B_4137_945C_7AE65122534F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DMTimeEditDialog.h : header file
//

class cDNDParty;

/////////////////////////////////////////////////////////////////////////////
// DMTimeEditDialog dialog

class DMTimeEditDialog : public CDialog
{
// Construction
public:
	DMTimeEditDialog(cDNDParty *pParty, DMPartyDialog *pPartyDialog, cDMMapViewDialog *pMapDialog = NULL, CWnd* pParent = NULL);   // standard constructor

	DMPartyDialog *m_pPartyDialog;
	cDMMapViewDialog *m_pMapDialog;

	cDNDParty *m_pParty;

	void Refresh();

// Dialog Data
	//{{AFX_DATA(DMTimeEditDialog)
	enum { IDD = IDD_TIME_EDIT_DIALOG };
	CString	m_szYear;
	CString	m_szMonth;
	CString	m_szDay;
	CString	m_szHour;
	CString	m_szTurn;
	CString	m_szSegment;
	CString	m_szRound;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DMTimeEditDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(DMTimeEditDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDecYearButton();
	afx_msg void OnIncYearButton();
	afx_msg void OnDecMonthButton();
	afx_msg void OnIncMonthButton();
	afx_msg void OnDecDayButton();
	afx_msg void OnIncDayButton();
	afx_msg void OnChangeYearEdit();
	afx_msg void OnChangeMonthEdit();
	afx_msg void OnChangeDayEdit();
	afx_msg void OnChangeHourEdit();
	afx_msg void OnChangeTurnEdit();
	afx_msg void OnChangeRoundEdit();
	afx_msg void OnChangeSegmentEdit();
	afx_msg void OnDecHourButton();
	afx_msg void OnIncHourButton();
	afx_msg void OnDecTurnButton();
	afx_msg void OnIncTurnButton();
	afx_msg void OnDecRoundButton();
	afx_msg void OnIncRoundButton();
	afx_msg void OnDecSegmentButton();
	afx_msg void OnIncSegmentButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedStartClockButton();
	CButton m_cStartClockButton;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DMTIMEEDITDIALOG_H__B558B840_7E3B_4137_945C_7AE65122534F__INCLUDED_)
