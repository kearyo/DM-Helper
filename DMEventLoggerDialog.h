#pragma once
#include "afxwin.h"


// CDMEventLoggerDialog dialog

class CDMEventLoggerDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMEventLoggerDialog)

public:
	CDMEventLoggerDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMEventLoggerDialog();

	CDMHelperApp *m_pApp;
	DMPartyDialog *m_pParentWindow;
	DWORD m_dwSelectedCharacterID;
	cDNDCharacter *m_pSelectedCharacter;
	LONG m_lAmount;

	DND_LOG_EVENT_TYPES m_nEventType;
	BOOL m_bNeedCharList;
	BOOL m_bNeedEventValue;

	int m_nInsertPosition;

	void Init();
	void PopulateCharacterList();
	void Refresh();


// Dialog Data
	enum { IDD = IDD_EVENT_LOGGER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CComboBox m_cEventTypeCombo;
	afx_msg void OnClose();
	CString m_szComment;
	CButton m_cLogEventButton;
	CComboBox m_cEventTargetCombo;
	CString m_szEventTarget;
	afx_msg void OnCbnSelchangeEventTypeCombo();
	CStatic m_cEventValueLabel;
	CString m_szEventValueLabel;
	CEdit m_cEventValueEdit;
	CString m_szEventValue;
	afx_msg void OnCbnSelchangeEventTargetCombo();
};
