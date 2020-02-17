#pragma once


// cDMChartLookupDialog dialog

class cDMChartLookupDialog : public CDialog
{
	DECLARE_DYNAMIC(cDMChartLookupDialog)

public:
	cDMChartLookupDialog(DND_CHART_TYPES _ChartType, CWnd* pParent = NULL);   // standard constructor
	virtual ~cDMChartLookupDialog();

	DND_CHART_TYPES m_ChartType;
	BOOL m_bRotateHeader;
	BOOL m_bSmallFont;

	int m_nHeaderX;
	int m_nHeaderY;
	int m_nShiftHeaderX;

	int m_nLabelX;
	int m_nLabelY;
	int m_nShiftLabelY;

	int m_nDataX;


	CString m_szMatrix[30][20];

// Dialog Data
	enum { IDD = IDD_CHART_LOOKUP_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
