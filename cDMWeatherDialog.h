#pragma once

#include <afxcontrolbars.h>


#define MAX_PARTICLES	512
#define MAX_PARTICLE_DEPTH	256

class CDMParticle
{
public:

	float m_fX;
	float m_fY;
	float m_fZ;
	float m_fSize;

	CDMParticle()
	{
		m_fX = 0;
		m_fY = 0;
		m_fZ = 0;
		m_fSize = 8.0f;
	};

};


// cDMWeatherDialog dialog

class cDMWeatherDialog : public CDialogEx
{
	DECLARE_DYNAMIC(cDMWeatherDialog)

public:
	cDMWeatherDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~cDMWeatherDialog();

	Bitmap * ResourceToBitmap(const HINSTANCE hInstance, const int id);
	void UpdateParticles(BOOL bVisible);
	void Cleanup();

	CDMHelperApp *m_pApp;

	cDMMapViewDialog *m_pDMMapViewDialog;

	Bitmap* m_pRainParticleBitmap;
	Bitmap* m_pRainSplatParticleBitmap;
	Bitmap* m_pSnowParticleBitmap;

	CWinThread *m_pParticleThread;

	CDMParticle m_Particle[MAX_PARTICLES];

	BOOL m_bShuttingDown;

	DND_WEATHER_TYPES m_WeatherType;

// Dialog Data
	enum { IDD = IDD_WEATHER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBnClickedCancel();
	virtual void PostNcDestroy();
	afx_msg void OnPaint();
	afx_msg LRESULT OnSetWeatherType(UINT wParam, LONG lParam);
};

UINT DMWeatherParticleThreadProc(LPVOID pData);
