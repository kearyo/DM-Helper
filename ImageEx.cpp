// GDIPlusHelper.cpp: implementation of the CGDIPlusHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImageEx.h"
#include <process.h>
#include "DM Helper.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
// #define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	ImageEx::ImageEx
// 
// DESCRIPTION:	Constructor for constructing images from a resource 
// 
// RETURNS:	
// 
// NOTES:		
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       29012002	1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
ImageEx::ImageEx(LPCTSTR sResourceType, LPCTSTR sResource)
{
	Initialize();

	if (Load(sResourceType, sResource))
	{

	   nativeImage = NULL;
	   
	   lastResult = DllExports::GdipLoadImageFromStreamICM(m_pStream, &nativeImage);
	  
	   TestForAnimatedGIF();
    }
}



////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	ImageEx::ImageEx
// 
// DESCRIPTION:	Constructor for constructing images from a file
// 
// RETURNS:	
// 
// NOTES:		
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       29012002	1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
ImageEx::ImageEx(const WCHAR* filename, BOOL useEmbeddedColorManagement) : Image(filename, useEmbeddedColorManagement)
{
	Initialize();

	m_bIsInitialized = true;

	m_bCycled = FALSE;

	TestForAnimatedGIF();
}


////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	ImageEx::~ImageEx
// 
// DESCRIPTION:	Free up fresources
// 
// RETURNS:	
// 
// NOTES:		
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       29012002	1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
ImageEx::~ImageEx()
{
	Destroy();
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	InitAnimation
// 
// DESCRIPTION:	Prepare animated GIF
// 
// RETURNS:	
// 
// NOTES:		
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       29012002	1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
bool ImageEx::InitAnimation(HWND hWnd, CPoint pt, float fScreenScale, float fSpriteScale, BOOL bCycleAnimation, BOOL *pbRenderingFlag, int nCycles, BOOL bColorKeyed, BOOL bTranslucent, BOOL bDrawUnder, float fAlpha, BOOL bColorize, float fRed, float fGreen, float fBlue)
{

	m_hWnd = hWnd;
	m_pt = pt;

	m_fScreenScale = fScreenScale;
	m_fSpriteScale = fSpriteScale;

	m_bCycleAnimation = bCycleAnimation;

	m_pbRenderingFlag = pbRenderingFlag;

	m_nCycles = nCycles;

	m_bColorKeyed = bColorKeyed;
	m_bTranslucent = bTranslucent;
	m_fAlpha = fAlpha;
	m_bDrawUnder = bDrawUnder;

	m_bColorize = bColorize;
	m_fRed = fRed;
	m_fGreen = fGreen;
	m_fBlue = fBlue;

	if (!m_bIsInitialized)
	{
		TRACE(_T("GIF not initialized\n"));
		return false;
	};

	if (IsAnimatedGIF())
	{
		if (m_hThread == NULL)
		{
		
			unsigned int nTID = 0;

			m_hThread = (HANDLE) _beginthreadex( NULL, 0, _ThreadAnimationProc, this, CREATE_SUSPENDED,&nTID);
			
			if (!m_hThread)
			{
				TRACE(_T("Couldn't start a GIF animation thread\n"));
				return true;
			} 
			else
			{
				ResumeThread(m_hThread);
			}
		}
	} 

	return false;	

}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	LoadFromBuffer
// 
// DESCRIPTION:	Helper function to copy phyical memory from buffer a IStream
// 
// RETURNS:	
// 
// NOTES:		
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       29012002	1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
bool ImageEx::LoadFromBuffer(BYTE* pBuff, int nSize)
{
	bool bResult = false;

	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nSize);
	if (hGlobal)
	{
		void* pData = GlobalLock(hGlobal);
		if (pData)
			memcpy(pData, pBuff, nSize);
		
		GlobalUnlock(hGlobal);

		if (CreateStreamOnHGlobal(hGlobal, TRUE, &m_pStream) == S_OK)
			bResult = true;

	}


	return bResult;
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	GetResource
// 
// DESCRIPTION:	Helper function to lock down resource
// 
// RETURNS:	
// 
// NOTES:		
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       29012002	1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
bool ImageEx::GetResource(LPCTSTR lpName, LPCTSTR lpType, void* pResource, int& nBufSize)
{ 
	HRSRC		hResInfo;
	HANDLE		hRes;
	LPSTR		lpRes	= NULL; 
	int			nLen	= 0;
	bool		bResult	= FALSE;

	// Find the resource

	hResInfo = FindResource(m_hInst , lpName, lpType);
	if (hResInfo == NULL) 
	{
		DWORD dwErr = GetLastError();
		return false;
	}

	// Load the resource
	hRes = LoadResource(m_hInst , hResInfo);

	if (hRes == NULL) 
		return false;

	// Lock the resource
	lpRes = (char*)LockResource(hRes);

	if (lpRes != NULL)
	{ 
		if (pResource == NULL)
		{
			nBufSize = SizeofResource(m_hInst , hResInfo);
			bResult = true;
		}
		else
		{
			if (nBufSize >= (int)SizeofResource(m_hInst , hResInfo))
			{
				memcpy(pResource, lpRes, nBufSize);
				bResult = true;
			}
		} 

		UnlockResource(hRes);  
	}

	// Free the resource
	FreeResource(hRes);

	return bResult;
}


////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	Load
// 
// DESCRIPTION:	Helper function to load resource from memory
// 
// RETURNS:	
// 
// NOTES:		
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       29012002	1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
bool ImageEx::Load(CString sResourceType, CString sResource)
{
	bool bResult = false;


	BYTE*	pBuff = NULL;
	int		nSize = 0;
	if (GetResource(sResource.GetBuffer(0), sResourceType.GetBuffer(0), pBuff, nSize))
	{
		if (nSize > 0)
		{
			pBuff = new BYTE[nSize];

			if (GetResource(sResource, sResourceType.GetBuffer(0), pBuff, nSize))
			{
				if (LoadFromBuffer(pBuff, nSize))
				{

					bResult = true;
				}
			}

			delete []pBuff;
		}
	}


	m_bIsInitialized = bResult;

	return bResult;
}


////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	GetSize
// 
// DESCRIPTION:	Returns Width and Height object
// 
// RETURNS:	
// 
// NOTES:		
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       29012002	1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
CSize ImageEx::GetSize()
{
	return CSize(GetWidth(), GetHeight());
}


////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	TestForAnimatedGIF
// 
// DESCRIPTION:	Check GIF/Image for avialability of animation
// 
// RETURNS:	
// 
// NOTES:		
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       29012002	1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
bool ImageEx::TestForAnimatedGIF()
{
   UINT count = 0;
   count = GetFrameDimensionsCount();
   GUID* pDimensionIDs = new GUID[count];

   // Get the list of frame dimensions from the Image object.
   GetFrameDimensionsList(pDimensionIDs, count);

   // Get the number of frames in the first dimension.
   m_nFrameCount = GetFrameCount(&pDimensionIDs[0]);

	// Assume that the image has a property item of type PropertyItemEquipMake.
	// Get the size of that property item.
   int nSize = GetPropertyItemSize(PropertyTagFrameDelay);

   // Allocate a buffer to receive the property item.
   m_pPropertyItem = (PropertyItem*) malloc(nSize);

   GetPropertyItem(PropertyTagFrameDelay, nSize, m_pPropertyItem);

   
   delete  pDimensionIDs;

   return m_nFrameCount > 1;
}


////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	ImageEx::Initialize
// 
// DESCRIPTION:	Common function called from Constructors
// 
// RETURNS:	
// 
// NOTES:		
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       29012002	1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
void ImageEx::Initialize()
{
	m_pStream = NULL;
	m_nFramePosition = 0;
	m_nFrameCount = 0;
	m_pStream = NULL;
	lastResult = InvalidParameter;
	m_hThread = NULL;
	m_bIsInitialized = false;
	m_pPropertyItem = NULL;
	m_bColorKeyed = FALSE;
	m_bTranslucent = FALSE;
	
#ifdef INDIGO_CTRL_PROJECT
	m_hInst = _Module.GetResourceInstance();
#else
	m_hInst = AfxGetResourceHandle();
#endif


	m_bPause = false;

	m_hExitEvent = CreateEvent(NULL,TRUE,FALSE,NULL);

	m_hPause = CreateEvent(NULL,TRUE,TRUE,NULL);

	m_bFirstDraw = TRUE;

	m_pBackGroundBitmap = NULL;

	#if USE_GIF_BUFFER
	m_pBufferBitmap = NULL;
	#endif

	m_fSpriteScale = 1.0f;

	m_fScreenScale = 1.0f;

}


void ImageEx::Position(int nX, int nY, float fScreenScale, float fSpriteScale)
{
	m_pt.x = nX;
	m_pt.y = nY;

	m_fScreenScale = fScreenScale;

	m_fSpriteScale = fSpriteScale;
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	ImageEx::_ThreadAnimationProc
// 
// DESCRIPTION:	Thread to draw animated gifs
// 
// RETURNS:	
// 
// NOTES:		
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       29012002	1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
UINT WINAPI ImageEx::_ThreadAnimationProc(LPVOID pParam)
{
	ASSERT(pParam);
	ImageEx *pImage = reinterpret_cast<ImageEx *> (pParam);
	pImage->ThreadAnimation();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	ImageEx::ThreadAnimation
// 
// DESCRIPTION:	Helper function
// 
// RETURNS:	
// 
// NOTES:		
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       29012002	1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
void ImageEx::ThreadAnimation()
{
	m_nFramePosition = 0;

	bool bExit = false;
	while (bExit == false)
	{
		bExit = (DrawFrameGIF() || g_bGlobalShutDownFlag);

		#ifdef _DEBUG
		if (bExit)
		{
			TRACE("OUT!\n");
		}
		#endif
	}
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	ImageEx::DrawFrameGIF
// 
// DESCRIPTION:	
// 
// RETURNS:	
// 
// NOTES:		
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       29012002	1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
bool ImageEx::DrawFrameGIF()
{

	::WaitForSingleObject(m_hPause, INFINITE);

	GUID   pageGuid = FrameDimensionTime;

	long hmWidth = GetWidth();
	long hmHeight = GetHeight();

	float fScale = m_fSpriteScale * m_fScreenScale;

	hmWidth *= fScale;
	hmHeight *= fScale;

	POINT _pt = m_pt;

	_pt.x = _pt.x - hmWidth / 2;
	_pt.y = _pt.y - hmHeight / 2;

	HDC hDC = GetDC(m_hWnd);
	if (hDC)
	{
		Graphics graphics(hDC);

		int nBackWidth = hmWidth; //*2;
		int nBackHeight = hmHeight; //*2;

		if (*m_pbRenderingFlag == FALSE)
		{
			if (m_bFirstDraw && IsAnimatedGIF() && m_pBackGroundBitmap == NULL)
			{
				HDC scrdc, memdc;
				HBITMAP membit;
				scrdc = ::GetDC(0);

				POINT pointScreen;
				pointScreen.x = _pt.x;
				pointScreen.y = _pt.y;

				ClientToScreen(m_hWnd, &pointScreen);

				memdc = CreateCompatibleDC(scrdc);
				membit = CreateCompatibleBitmap(scrdc, nBackWidth, nBackHeight);
				HBITMAP hOldBitmap = (HBITMAP)SelectObject(memdc, membit);
				BitBlt(memdc, 0, 0, nBackWidth, nBackHeight, scrdc, pointScreen.x, pointScreen.y, SRCCOPY);

				//Gdiplus::Bitmap bitmap(membit, NULL);
				//graphics.DrawImage(&bitmap, m_pt.x, m_pt.y, hmWidth, hmHeight);

				m_pBackGroundBitmap = new Bitmap(membit, NULL);

				#if USE_GIF_BUFFER
				HBITMAP membitB = CreateCompatibleBitmap(scrdc, nBackWidth, nBackHeight);;
				m_pBufferBitmap = new Bitmap(membitB, NULL);
				DeleteObject(membitB);
				#endif

				SelectObject(memdc, hOldBitmap);

				DeleteObject(memdc);
				DeleteObject(membit);
				::ReleaseDC(0, scrdc);
			}

			if (m_pBackGroundBitmap != NULL)
			{
				#if USE_GIF_BUFFER
				Graphics g(m_pBufferBitmap);
				g.DrawImage(m_pBackGroundBitmap, 0, 0, nBackWidth, nBackHeight);
				#else
				graphics.DrawImage(m_pBackGroundBitmap, _pt.x, _pt.y, nBackWidth, nBackHeight);
				#endif
			}

			if (m_bCycleAnimation == TRUE || m_bCycled == FALSE)
			{
				if (m_bTranslucent)
				{
					#if 0
					ColorMatrix ClrMatrix = {
						1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
						0.0f, 0.0f, 0.0f, m_fAlpha, 0.0f,
						0.0f, 0.0f, 0.0f, 0.0f, 1.0f
					};
					#else
					float fRed = 1.0f;
					float fGreen = 1.0f;
					float fBlue = 1.0f;

					if (m_bColorize)
					{
						fRed = m_fRed;
						fGreen = m_fGreen;
						fBlue = m_fBlue;
					}

					ColorMatrix ClrMatrix = {
						fRed, 0.0f, 0.0f, 0.0f, 0.0f,
						0.0f, fGreen, 0.0f, 0.0f, 0.0f,
						0.0f, 0.0f, fBlue, 0.0f, 0.0f,
						0.0f, 0.0f, 0.0f, m_fAlpha, 0.0f,
						0.0f, 0.0f, 0.0f, 0.0f, 1.0f
					};

					#endif

					ImageAttributes ImgAttr;

					ImgAttr.SetColorMatrix(&ClrMatrix, ColorMatrixFlagsDefault,
						ColorAdjustTypeBitmap);

					ImgAttr.SetColorKey(Color(0, 0, 0, 0), Color(0, 128, 128, 128), ColorAdjustTypeBitmap);
					
					RectF dst;
					dst.X = (Gdiplus::REAL) _pt.x;
					dst.Y = (Gdiplus::REAL) _pt.y;
					dst.Width = (Gdiplus::REAL)hmWidth;
					dst.Height = (Gdiplus::REAL)hmHeight;

					#if USE_GIF_BUFFER
					RectF dstB;
					dstB.X = 0;
					dstB.Y = 0;
					dstB.Width = nBackWidth;
					dstB.Height = nBackHeight;

					Graphics g(m_pBufferBitmap);
					g.DrawImage(this, dstB, 0, 0, GetWidth(), GetHeight(), Gdiplus::UnitPixel, &ImgAttr);

					if (m_bDrawUnder) // draw under image
					{
						if (m_pBackGroundBitmap != NULL)
						{
							#if USE_GIF_BUFFER
							Graphics g(m_pBufferBitmap);

							ImageAttributes imAttr;
							imAttr.SetColorKey(Color(0, 0, 0), Color(0, 0, 0), ColorAdjustTypeBitmap);

							Rect destRect(0, 0, nBackWidth, nBackHeight);
							g.DrawImage(m_pBackGroundBitmap, destRect, 0, 0, nBackWidth, nBackHeight, Gdiplus::UnitPixel, &imAttr);

							//g.DrawImage(m_pBackGroundBitmap, 0, 0, nBackWidth, nBackHeight);
							#else
							graphics.DrawImage(m_pBackGroundBitmap, _pt.x, _pt.y, nBackWidth, nBackHeight);
							#endif
						}
					}

					graphics.DrawImage(m_pBufferBitmap, dst, 0, 0, nBackWidth, nBackHeight, Gdiplus::UnitPixel, NULL); // draw it on the screen
					#else
					graphics.DrawImage(this, dst, 0, 0, GetWidth(), GetHeight(), Gdiplus::UnitPixel, &ImgAttr);
					#endif

					
				}
				else if (m_bColorKeyed)
				{
					ImageAttributes ImgAttr;
					ImgAttr.SetColorKey(Color(0, 0, 0, 0), Color(0, 128, 128, 128), ColorAdjustTypeBitmap);
					RectF dst;

					dst.X = (Gdiplus::REAL) _pt.x;
					dst.Y = (Gdiplus::REAL) _pt.y;
					dst.Width = (Gdiplus::REAL)hmWidth;
					dst.Height = (Gdiplus::REAL)hmHeight;

					#if USE_GIF_BUFFER
					RectF dstB;
					dstB.X = 0;
					dstB.Y = 0;
					dstB.Width = nBackWidth;
					dstB.Height = nBackHeight;

					Graphics g(m_pBufferBitmap);
					g.DrawImage(this, dstB, 0, 0, GetWidth(), GetHeight(), Gdiplus::UnitPixel, &ImgAttr);

					if (m_bDrawUnder) // draw under image
					{
						if (m_pBackGroundBitmap != NULL)
						{
							#if USE_GIF_BUFFER
							Graphics g(m_pBufferBitmap);

							ImageAttributes imAttr;
							imAttr.SetColorKey(Color(0, 0, 0), Color(0, 0, 0), ColorAdjustTypeBitmap);

							Rect destRect(0, 0, nBackWidth, nBackHeight);
							g.DrawImage(m_pBackGroundBitmap, destRect, 0, 0, nBackWidth, nBackHeight, Gdiplus::UnitPixel, &imAttr);

							//g.DrawImage(m_pBackGroundBitmap, 0, 0, nBackWidth, nBackHeight);
							#else
							graphics.DrawImage(m_pBackGroundBitmap, _pt.x, _pt.y, nBackWidth, nBackHeight);
							#endif
						}
					}

					graphics.DrawImage(m_pBufferBitmap, dst, 0, 0, nBackWidth, nBackHeight, Gdiplus::UnitPixel, NULL);  // draw it on the screen
					#else
					graphics.DrawImage(this, dst, 0, 0, GetWidth(), GetHeight(), Gdiplus::UnitPixel, &ImgAttr);
					#endif
				}
				else
				{
					if (m_pBackGroundBitmap != NULL && m_bDrawUnder)
					{
						#if USE_GIF_BUFFER
						Graphics g(m_pBufferBitmap);

						g.DrawImage(this, 0,0, hmWidth, hmHeight);

						ImageAttributes imAttr;
						imAttr.SetColorKey(Color(0, 0, 0), Color(0, 0, 0), ColorAdjustTypeBitmap);

						Rect destRect(0, 0, nBackWidth, nBackHeight);
						g.DrawImage(m_pBackGroundBitmap, destRect, 0, 0, nBackWidth, nBackHeight, Gdiplus::UnitPixel, &imAttr);

						//g.DrawImage(m_pBackGroundBitmap, 0, 0, nBackWidth, nBackHeight);
						#else
						graphics.DrawImage(m_pBackGroundBitmap, _pt.x, _pt.y, nBackWidth, nBackHeight);
						#endif

						RectF dst;

						dst.X = (Gdiplus::REAL) _pt.x;
						dst.Y = (Gdiplus::REAL) _pt.y;
						dst.Width = (Gdiplus::REAL)hmWidth;
						dst.Height = (Gdiplus::REAL)hmHeight;

						graphics.DrawImage(m_pBufferBitmap, dst, 0, 0, nBackWidth, nBackHeight, Gdiplus::UnitPixel, NULL);  // draw it on the screen

					}
					else
					{
						graphics.DrawImage(this, _pt.x, _pt.y, hmWidth, hmHeight);
					}
				}
				
			}
		}

		ReleaseDC(m_hWnd, hDC);
	}

	SelectActiveFrame(&pageGuid, m_nFramePosition++);		
	
	if (m_nFramePosition == m_nFrameCount)
	{
		if (m_bCycleAnimation == FALSE && m_bCycled == FALSE)
		{
			RECT rRect;
			rRect.left = _pt.x;
			rRect.top = _pt.y;
			rRect.right = _pt.x + hmWidth;
			rRect.bottom = _pt.y + hmHeight;

			InvalidateRect(m_hWnd, &rRect, TRUE);
		}

		m_nFramePosition = 0;

		if (m_nCycles <= 0)
		{
			m_bCycled = TRUE;
		}
		else
		{
			--m_nCycles;
		}
	}


	long lPause = ((long*) m_pPropertyItem->value)[m_nFramePosition] * 10;

	DWORD dwErr = WaitForSingleObject(m_hExitEvent, lPause);

	return dwErr == WAIT_OBJECT_0;


}



////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	ImageEx::SetPause
// 
// DESCRIPTION:	Toggle Pause state of GIF
// 
// RETURNS:		
// 
// NOTES:		
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       29012002	1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
void ImageEx::SetPause(bool bPause)
{
	if (!IsAnimatedGIF())
		return;

	if (bPause && !m_bPause)
	{
		::ResetEvent(m_hPause);
	}
	else
	{

		if (m_bPause && !bPause)
		{
			::SetEvent(m_hPause);
		}
	}

	m_bPause = bPause;
}


void ImageEx::ResetBackground()
{
	if (NULL != m_pBackGroundBitmap)
	{
		delete m_pBackGroundBitmap;
		m_pBackGroundBitmap = NULL;
	}
	#if USE_GIF_BUFFER
	if (NULL != m_pBufferBitmap)
	{
		delete m_pBufferBitmap;
		m_pBufferBitmap = NULL;
	}
	#endif

	m_bFirstDraw = TRUE;
}

void ImageEx::Destroy()
{
	
	if (m_hThread)
	{
		// If pause un pause
		SetPause(false);

		SetEvent(m_hExitEvent);
		WaitForSingleObject(m_hThread, INFINITE);
	}

	if (m_hThread != NULL)
	{
		CloseHandle(m_hThread);
	}
	if (m_hExitEvent != NULL)
	{
		CloseHandle(m_hExitEvent);
	}
	if(m_hPause != NULL)
	{
		CloseHandle(m_hPause);
	}
	if (m_pPropertyItem != NULL)
	{
		free(m_pPropertyItem);
	}

	m_pPropertyItem = NULL;
	m_hThread = NULL;
	m_hExitEvent = NULL;
	m_hPause = NULL;

	if (NULL != m_pBackGroundBitmap)
	{
		delete m_pBackGroundBitmap;
		m_pBackGroundBitmap = NULL;
	}

	#if USE_GIF_BUFFER
	if (NULL != m_pBufferBitmap)
	{
		delete m_pBufferBitmap;
		m_pBufferBitmap = NULL;
	}
	#endif

	if (m_pStream)
	{
		m_pStream->Release();
		m_pStream = NULL;
	}

}