#pragma once
#include "afxwin.h"


// DMContainerDetailsDialog dialog

class CDMContainerDetailsDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMContainerDetailsDialog)

public:
	CDMContainerDetailsDialog(DWORD dwContainerID, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMContainerDetailsDialog();

	DMInventoryDialog *m_pParentWindow;
	DWORD m_dwContainerID;

	POBJECTTYPE m_pSelectedInventoryObject;
	POBJECTTYPE m_pSelectedContainerObject;

	void Refresh(DWORD dwContainerID);

	CFont m_ListFont;

// Dialog Data
	enum { IDD = IDD_CONTAINER_DETAILS_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedPutInButton();
	afx_msg void OnBnClickedTakeOutButton();
	CBitmapButton m_cPutInButton;
	CBitmapButton m_cTakeOutButton;
	CListBox m_cInventoryList;
	CListBox m_cContainerList;
	afx_msg void OnLbnSelchangeInventoryList();
	afx_msg void OnLbnSelchangeContainerList();
	CButton m_cPPButton;
	CButton m_cGPButton;
	CButton m_cEPButton;
	CButton m_cSPButton;
	CButton m_cCPButton;
	afx_msg void OnBnClickedPpButton();
	afx_msg void OnBnClickedGpButton();
	afx_msg void OnBnClickedEpButton();
	afx_msg void OnBnClickedSpButton();
	afx_msg void OnBnClickedCpButton();
	CStatic m_cContainDetailsStatic;
	afx_msg void OnBnClickedAllCoinsButton();
};
