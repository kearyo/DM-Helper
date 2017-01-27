#pragma once
#include "afxwin.h"


// CDMEditMapLayersDialog dialog

class CDMEditMapLayersDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMEditMapLayersDialog)

public:
	CDMEditMapLayersDialog(cDNDMapCell *pCell, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMEditMapLayersDialog();

	CDMHelperApp *m_pApp;

	cDMMapViewDialog *m_pMapViewDialog;
	cDNDMapCell *m_pCell;

	void Refresh();
	BOOL LayerExists(int nLayer);
	void AddLayer(int nLayer);
	void DeleteLayer(int nLayer);

// Dialog Data
	enum { IDD = IDD_MAP_LAYER_EDIT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	CString m_szCellLabel;
	CButton m_cEditLayer1;
	CButton m_cEditLayer2;
	CButton m_cEditLayer3;
	CButton m_cEditLayer4;
	CButton m_cDeleteLayer1;
	CButton m_cDeleteLayer2;
	CButton m_cDeleteLayer3;
	CButton m_cDeleteLayer4;
	afx_msg void OnBnClickedEditLayer1();
	afx_msg void OnBnClickedEditLayer2();
	afx_msg void OnBnClickedEditLayer3();
	afx_msg void OnBnClickedEditLayer4();
	afx_msg void OnBnClickedDeleteLayer1();
	afx_msg void OnBnClickedDeleteLayer2();
	afx_msg void OnBnClickedDeleteLayer3();
	afx_msg void OnBnClickedDeleteLayer4();
};
