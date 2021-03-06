#pragma once
#include "afxwin.h"

// cDMMapSFXDialog dialog

class cDMMapSFXDialog : public CDialog
{
	DECLARE_DYNAMIC(cDMMapSFXDialog)

public:
	cDMMapSFXDialog(CWnd* pParent, int nSFXIndex, int nX, int nY, float fScale);   // standard constructor
	virtual ~cDMMapSFXDialog();

	CDMHelperApp *m_pApp;
	cDMMapViewDialog *m_pMapViewDialog;
	cDNDMap *m_pDNDMap;

	int m_nSFXIndex;
	int m_nOriginalSFXIndex;
	int m_nMouseX;
	int m_nMouseY;
	float m_fScale;

// Dialog Data
	enum { IDD = IDD_MAP_SFX_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CString m_szSFXName;
	CString m_szSFXGFXFileName;
	afx_msg void OnEnSetfocusEffectGfxEdit();
	afx_msg void OnBnClickedChooseFileButton();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedReposition();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEffectNameEdit();
	afx_msg void OnEnChangeEffectGfxEdit();
	afx_msg void OnBnClickedPreview();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedUp();
	afx_msg void OnBnClickedDown();
	afx_msg void OnBnClickedLeft();
	afx_msg void OnBnClickedRight();
	CComboBox m_cSFXList;
	afx_msg void OnBnClickedScaleDown();
	afx_msg void OnBnClickedScaleUp();
	afx_msg void OnBnClickedSfxCycleCheck();
	BOOL m_bCycleGIF;
	CButton m_cUpButton;
	CButton m_cDownButton;
	CButton m_cLeftButton;
	CButton m_cRightButton;
	BOOL m_bColorKeyed;
	afx_msg void OnBnClickedSfxColorKeyedCheck();
	BOOL m_bTranslucent;
	afx_msg void OnBnClickedSfxTranslucentCheck();
	CString m_szAlpha;
	afx_msg void OnEnChangeAlphaEdit();
	BOOL m_bLightSource;
	CString m_szLightSourceRange;
	CEdit m_cLightSourceRangeEdit;
	afx_msg void OnBnClickedLightSourceCheck();
	afx_msg void OnEnChangeLightRangeEdit();
	CString m_szInfoText1;
	CString m_szInfoText2;
	CEdit m_cRedEdit;
	CEdit m_cGreenEdit;
	CEdit m_cBlueEdit;
	CString m_szRed;
	CString m_szGreen;
	CString m_szBlue;
	BOOL m_bColorize;
	afx_msg void OnBnClickedSfxColorizeCheck();
	afx_msg void OnBnClickedDefaultActiveCheck();
	afx_msg void OnBnClickedDrawUnderCheck();
	BOOL m_bDefaultActivated;
	BOOL m_bDrawUnder;
};
