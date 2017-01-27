#pragma once


class DMInventoryDialog;

// CDMPCDetailsDialog dialog

class CDMPCDetailsDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMPCDetailsDialog)

public:
	CDMPCDetailsDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMPCDetailsDialog();

	DMInventoryDialog *m_pParentWindow;
	PDNDCHARACTER m_pPC;

	void Init(BOOL bAgeEdit = FALSE);
	void Refresh(BOOL bCharacterChanged = TRUE ,BOOL bClosing = FALSE);

// Dialog Data
	enum { IDD = IDD_PC_DETAILS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnPaint();
	CString m_szPlayerWillTitle;
	CString m_szPlayerWill;
	CString m_szPlayerName;
	CString m_szDateCreated;
	CString m_szPatronDeity;
	CString m_szPlaceofOrigin;
	CString m_szReligion;
	CString m_szHeight;
	CString m_szWeight;
	CString m_szAge;
	CString m_szDOBMonth;
	CString m_szDOBDay;
	CString m_szDOBYear;
	CString m_szAgeAdjust;
	CString m_szHairColor;
	CString m_szEyeColor;
	afx_msg void OnEnChangeCharAgeEdit();
	afx_msg void OnStnClickedWillStatic();
	afx_msg void OnEnChangePlayerNameEdit();
	afx_msg void OnEnChangeCharCreateDateEdit();
	afx_msg void OnEnChangePlayerDeityEdit();
	afx_msg void OnEnChangeCharOriginEdit();
	afx_msg void OnEnChangePlayerReligionEdit();
	afx_msg void OnEnChangeCharHeightEdit();
	afx_msg void OnEnChangeCharWeightEdit();
	afx_msg void OnEnChangeCharDobMonthEdit();
	afx_msg void OnEnChangeCharDobDayEdit();
	afx_msg void OnEnChangeCharAgeAdjustEdit();
	afx_msg void OnEnChangeCharHairColorEdit();
	afx_msg void OnEnChangeCharEyeColorEdit();
	afx_msg void OnEnChangePlayerWillEdit();
	afx_msg void OnEnChangeCharDobYearEdit();
	afx_msg void OnBnClickedNotesButton();
	afx_msg void OnClose();
};
