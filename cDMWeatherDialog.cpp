// cDMWeatherDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DM HelperDlg.h"
#include "cDMWeatherDialog.h"
#include "cDMMapViewDialog.h"
#include "afxdialogex.h"


// cDMWeatherDialog dialog

IMPLEMENT_DYNAMIC(cDMWeatherDialog, CDialogEx)

cDMWeatherDialog::cDMWeatherDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(cDMWeatherDialog::IDD, pParent)
{
	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_pDMMapViewDialog = (cDMMapViewDialog *)pParent;

	m_bShuttingDown = FALSE;
	m_pRainParticleBitmap = NULL;
	m_pRainSplatParticleBitmap = NULL;
	m_pSnowParticleBitmap = NULL;

	m_WeatherType = DND_WEATHER_TYPE_RAIN;

	for (int i = 0; i < MAX_PARTICLES; i++)
	{
		m_Particle[i].m_fX = 0;
		m_Particle[i].m_fY = 0;
		m_Particle[i].m_fZ = MAX_PARTICLE_DEPTH * 2;
	}

	Create(cDMWeatherDialog::IDD, pParent);
}

cDMWeatherDialog::~cDMWeatherDialog()
{
}

void cDMWeatherDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(cDMWeatherDialog, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDCANCEL, &cDMWeatherDialog::OnBnClickedCancel)
	ON_WM_PAINT()
	ON_MESSAGE(DND_WEATHER_MESSAGE, OnSetWeatherType)
END_MESSAGE_MAP()


// cDMWeatherDialog message handlers


BOOL cDMWeatherDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetBackgroundColor(RGB(255, 0, 255));
	LONG ExtendedStyle = GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
	
	//SetWindowLong(m_hWnd, GWL_STYLE, WS_CHILD);
	//SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, (WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW));
	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, ExtendedStyle | WS_EX_LAYERED);
	::SetLayeredWindowAttributes(GetSafeHwnd(), RGB(255, 0, 255), 0, LWA_COLORKEY);
	
	m_pRainParticleBitmap = ResourceToBitmap(AfxGetInstanceHandle(), IDB_RAIN_BITMAP);
	m_pRainSplatParticleBitmap = ResourceToBitmap(AfxGetInstanceHandle(), IDB_RAIN_SPLAT_BITMAP);
	m_pSnowParticleBitmap = ResourceToBitmap(AfxGetInstanceHandle(), IDB_SNOW_BITMAP);

	//SetWindowLong(m_hWnd, GWL_STYLE, WS_CHILD);

	m_pParticleThread = AfxBeginThread(DMWeatherParticleThreadProc, this);

	ShowWindow(SW_SHOW);

	// ::InvalidateRect(m_pParentWnd->m_hWnd, NULL, FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


Bitmap *cDMWeatherDialog::ResourceToBitmap(const HINSTANCE hInstance, const int id)
{
	HBITMAP hBmp = ::LoadBitmap(hInstance, MAKEINTRESOURCE(id));
	if (hBmp == NULL) return NULL;
	return Bitmap::FromHBITMAP(hBmp, NULL);
}


UINT DMWeatherParticleThreadProc(LPVOID pData)
{
	cDMWeatherDialog *pWeatherDlg = (cDMWeatherDialog*)pData;

	Sleep(500);

	do
	{
		BOOL bVisible = ::IsWindowVisible(pWeatherDlg->m_pDMMapViewDialog->m_hWnd);

		if (pWeatherDlg->m_pDMMapViewDialog->m_bDetachedWindow == FALSE)
		{
			if (pWeatherDlg->m_pDMMapViewDialog->m_pApp->m_pMainWindow->m_pCurrentOpenMapWindow != pWeatherDlg->m_pDMMapViewDialog)
			{
				bVisible = FALSE;
			}
		}

		if (pWeatherDlg->m_pDMMapViewDialog->IsIconic())
		{
			bVisible = FALSE;
		}

		pWeatherDlg->m_pDMMapViewDialog->m_bWeatherThreadRunning = TRUE;
		pWeatherDlg->UpdateParticles(bVisible);
		Sleep(5);
	} while (pWeatherDlg->m_bShuttingDown == FALSE && pWeatherDlg->m_pDMMapViewDialog->m_bShuttingDown == FALSE);

	pWeatherDlg->m_pDMMapViewDialog->m_bWeatherThreadRunning = FALSE;

	return 0;
}

void cDMWeatherDialog::UpdateParticles(BOOL bVisible)
{
	
	RECT rect;
	GetClientRect(&rect);

	int nSizeX = max(rect.right-rect.left, 1);
	int nSizeY = max(rect.bottom-rect.top, 1);

	HDC hDC = ::GetDC(m_hWnd);
	if (hDC)
	{
		Graphics graphics(hDC);

		Color color(255, 255, 0, 255);

		Rect rectangle(rect.left, rect.top, rect.right, rect.bottom);
		SolidBrush solidBrush(color);
		graphics.FillRectangle(&solidBrush, rectangle);

		if (bVisible)
		{
			int halfWidth = nSizeX / 2;
			int halfHeight = nSizeY / 2;

			float fDecrease = 0.2f;
			float fSpeed = 5.0f;
			float fMaxSize = 8.0f;

			int nMaxParticles = MAX_PARTICLES;

			switch (m_WeatherType)
			{
				case DND_WEATHER_TYPE_RAIN:
				{
					fSpeed = 10.0f;
					fDecrease = 1.0f;
					nMaxParticles = MAX_PARTICLES/2;
					fMaxSize = 11.0f;
					break;
				}
				case DND_WEATHER_TYPE_SNOW:
				{
					fSpeed = 5.0f;
					fDecrease = 0.5f;
					nMaxParticles = MAX_PARTICLES;
					fMaxSize = 8.0f;
					break;
				}
			}

			ImageAttributes attr;
			attr.SetColorKey(Color(255, 0, 255), Color(255, 0, 255), ColorAdjustTypeBitmap);

			for (int i = 0; i < nMaxParticles && m_bShuttingDown == FALSE; i++)
			{
				m_Particle[i].m_fZ += fSpeed;

				if (m_Particle[i].m_fZ > MAX_PARTICLE_DEPTH)
				{
					/*
					switch (m_WeatherType)
					{
						case DND_WEATHER_TYPE_RAIN:
						{
							float k = 128.0 / m_Particle[i].m_fZ;

							float px = m_Particle[i].m_fX * k + halfWidth;
							float py = m_Particle[i].m_fY  * k + halfHeight;

							RectF dst;
							dst.X = (int)(px + 0)* 1.0f - 16;
							dst.Y = (int)(py + 0)* 1.0f - 9;
							dst.Width = 32;
							dst.Height = 18;

							graphics.DrawImage(m_pRainSplatParticleBitmap, dst, 0, 0, 32, 18, UnitPixel, &attr); //, UnitPixel);

							break;
						}
						case DND_WEATHER_TYPE_SNOW:
						{
							break;
						}
					}
					*/

					m_Particle[i].m_fX = (rand() % nSizeX) - halfWidth;
					m_Particle[i].m_fY = (rand() % nSizeY) - halfHeight;
					m_Particle[i].m_fZ = (rand() % MAX_PARTICLE_DEPTH);
					m_Particle[i].m_fSize = fMaxSize;
				}

				m_Particle[i].m_fSize -= fDecrease;

				float k = 128.0 / m_Particle[i].m_fZ;

				switch (m_WeatherType)
				{
					case DND_WEATHER_TYPE_RAIN:
					{
						break;
					}
					case DND_WEATHER_TYPE_SNOW:
					{
						m_Particle[i].m_fX += (rand() % 10) - 5;
						m_Particle[i].m_fY += (rand() % 10) - 5;
						break;
					}
				}

				float px = m_Particle[i].m_fX * k + halfWidth;
				float py = m_Particle[i].m_fY  * k + halfHeight;

				RectF dst;
				dst.X = (int)(px + 0)* 1.0f;
				dst.Y = (int)(py + 0)* 1.0f;
				dst.Width = 12;
				dst.Height = 12;
				//dst.Width = max((int)m_Particle[i].m_fSize, 1);
				//dst.Height = max((int)m_Particle[i].m_fSize, 1);

				int nFrame = (int)(m_Particle[i].m_fSize + 0.5f);
				if (nFrame > 11)
					nFrame = 11;
				if (nFrame < 0)
					nFrame = 0;
				nFrame = 11 - nFrame;


				switch (m_WeatherType)
				{
					case DND_WEATHER_TYPE_RAIN:
					{
						graphics.DrawImage(m_pRainParticleBitmap, dst, nFrame*32+1, 1, 12, 12, UnitPixel, &attr); //, UnitPixel);
						break;
					}
					case DND_WEATHER_TYPE_SNOW:
					{
						graphics.DrawImage(m_pSnowParticleBitmap, dst, nFrame * 32 + 1, 1, 12, 12, UnitPixel, &attr); //, UnitPixel);
						break;
					}
				}

			}
		}
	}
}


void cDMWeatherDialog::Cleanup()
{
	if (m_bShuttingDown == FALSE)
	{
		m_bShuttingDown = TRUE;

		Sleep(100);
	}

	if (m_pDMMapViewDialog != NULL && m_pDMMapViewDialog->m_bWeatherThreadRunning)
	{
		int nCount = 0;
		do
		{
			Sleep(100);
			nCount++;
		} while (m_pDMMapViewDialog->m_bWeatherThreadRunning && nCount < 100);
	}

	if (m_pRainParticleBitmap != NULL)
	{
		delete m_pRainParticleBitmap;
		m_pRainParticleBitmap = NULL;
	}
	
	if (m_pRainSplatParticleBitmap != NULL)
	{
		delete m_pRainSplatParticleBitmap;
		m_pRainSplatParticleBitmap = NULL;
	}

	if (m_pSnowParticleBitmap != NULL)
	{
		delete m_pSnowParticleBitmap;
		m_pSnowParticleBitmap = NULL;
	}

	m_pDMMapViewDialog = NULL;
}

void cDMWeatherDialog::OnClose()
{
	Cleanup();

	CDialogEx::OnClose();
}


void cDMWeatherDialog::OnBnClickedCancel()
{
	Cleanup();

	CDialogEx::OnCancel();
}


void cDMWeatherDialog::PostNcDestroy()
{
	Cleanup();

	CDialogEx::PostNcDestroy();
}


void cDMWeatherDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	/*
	RECT rect;
	m_pDMMapViewDialog->GetWindowRect(&rect);
	SetWindowPos(NULL, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
	*/
}

LRESULT cDMWeatherDialog::OnSetWeatherType(UINT wParam, LONG lParam)
{
	m_WeatherType = (DND_WEATHER_TYPES)wParam;

	return 0;
}
