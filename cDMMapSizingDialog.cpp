// cDMMapSizingDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "cDMMapSizingDialog.h"
#include "afxdialogex.h"


// cDMMapSizingDialog dialog

IMPLEMENT_DYNAMIC(cDMMapSizingDialog, CDialog)

cDMMapSizingDialog::cDMMapSizingDialog(cDNDMap *pDNDMap, CWnd* pParent /*=NULL*/)
	: CDialog(cDMMapSizingDialog::IDD, pParent)
{

	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_pDNDMap = pDNDMap;
	m_pOriginalBitmap = m_pDNDMap->m_Cells[0][0].m_pBitmap;
}

cDMMapSizingDialog::~cDMMapSizingDialog()
{
}

void cDMMapSizingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_cProgressBar);
}


BEGIN_MESSAGE_MAP(cDMMapSizingDialog, CDialog)
	ON_BN_CLICKED(IDCANCEL, &cDMMapSizingDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &cDMMapSizingDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RESIZE_CELL_BUTTON, &cDMMapSizingDialog::OnBnClickedResizeCellButton)
	ON_BN_CLICKED(IDC_RESIZE_MAP_BUTTON, &cDMMapSizingDialog::OnBnClickedResizeMapButton)
END_MESSAGE_MAP()


// cDMMapSizingDialog message handlers

BOOL cDMMapSizingDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cProgressBar.ShowWindow(SW_HIDE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}



void cDMMapSizingDialog::OnBnClickedCancel()
{
	
	CDialog::OnCancel();
}


void cDMMapSizingDialog::OnBnClickedOk()
{
	
	CDialog::OnOK();
}


void cDMMapSizingDialog::OnBnClickedResizeCellButton()
{
	int nBitmapWidth = m_pOriginalBitmap->GetWidth();
	int nBitmapHeight = m_pOriginalBitmap->GetHeight();

	m_pDNDMap->m_nPixelSizeX = nBitmapWidth;
	m_pDNDMap->m_nPixelSizeY = nBitmapHeight;

	OnBnClickedOk();
}


void cDMMapSizingDialog::OnBnClickedResizeMapButton()
{
	CWaitCursor myWaitCursor;

	int nBitmapWidth = m_pOriginalBitmap->GetWidth();
	int nBitmapHeight = m_pOriginalBitmap->GetHeight();

	if (m_pDNDMap->m_nPixelSizeX < 512)
	{
		m_pDNDMap->m_nPixelSizeX = 512;
	}

	if (m_pDNDMap->m_nPixelSizeY < 512)
	{
		m_pDNDMap->m_nPixelSizeY = 512;
	}

	
	if (nBitmapWidth < m_pDNDMap->m_nPixelSizeX)
	{
		nBitmapWidth = m_pDNDMap->m_nPixelSizeX;
	}

	if (nBitmapHeight < m_pDNDMap->m_nPixelSizeY)
	{
		nBitmapHeight = m_pDNDMap->m_nPixelSizeY;
	}
	

	m_pDNDMap->m_nRows = nBitmapHeight / m_pDNDMap->m_nPixelSizeY;
	m_pDNDMap->m_nColumns = nBitmapWidth / m_pDNDMap->m_nPixelSizeX;

	if (nBitmapHeight % m_pDNDMap->m_nPixelSizeY)
	{
		m_pDNDMap->m_nRows += 1;
	}

	if (nBitmapWidth % m_pDNDMap->m_nPixelSizeX)
	{
		m_pDNDMap->m_nColumns += 1;
	}

	m_cProgressBar.SetRange(0, m_pDNDMap->m_nRows*m_pDNDMap->m_nColumns);

	m_cProgressBar.ShowWindow(SW_SHOW);

	int nCellsProcessed = 0;

	// C:\source\DM Helper\Release\Data\maps\bitmaps
	CString szDirPath = m_pApp->m_szEXEPath + "data\\maps\\bitmaps\\" + m_pDNDMap->m_szMapName;
	CreateDirectory(szDirPath, NULL);

	Color blackColor(255, 0, 0, 0);

	// Save bitmap (as a png)
	const CLSID pngEncoderClsId = { 0x557cf406, 0x1a04, 0x11d3, { 0x9a, 0x73, 0x00, 0x00, 0xf8, 0x1e, 0xf3, 0x2e } };

	//char szOutFile[MAX_PATH];

	BOOL bFailed = FALSE;
	int nDrawnWidth = 0;
	for (int nColumn = 0; nColumn < m_pDNDMap->m_nColumns; ++nColumn)
	{
		int nDrawnHeight = 0;
		for (int nRow = 0; nRow < m_pDNDMap->m_nRows; ++nRow)
		{
			int nX = nColumn *  m_pDNDMap->m_nPixelSizeX;
			int nY = nRow *  m_pDNDMap->m_nPixelSizeY;

			RectF bmpRect(0, 0, m_pDNDMap->m_nPixelSizeX - 1, m_pDNDMap->m_nPixelSizeY - 1);

			m_pDNDMap->m_Cells[nColumn][nRow].m_pBitmap = m_pOriginalBitmap->Clone(0, 0, m_pDNDMap->m_nPixelSizeX - 1, m_pDNDMap->m_nPixelSizeY - 1, m_pOriginalBitmap->GetPixelFormat());

			if (m_pDNDMap->m_Cells[nColumn][nRow].m_pBitmap == NULL)
			{
				bFailed = TRUE;
				continue;
			}
			
			Graphics *pGraphics = Graphics::FromImage(m_pDNDMap->m_Cells[nColumn][nRow].m_pBitmap);

			int nSourceWidth = m_pDNDMap->m_nPixelSizeX - 1;
			int nSourceHeight = m_pDNDMap->m_nPixelSizeY - 1;

			int nWidthRemaining = nBitmapWidth - nDrawnWidth;
			int nHeightRemaining = nBitmapHeight - nDrawnHeight;

			nSourceWidth = min(nWidthRemaining - 1, nSourceWidth);
			nSourceHeight = min(nHeightRemaining - 1, nSourceHeight);

			pGraphics->Clear(blackColor);
			pGraphics->DrawImage(m_pOriginalBitmap, 0, 0, nX, nY, nSourceWidth, nSourceHeight, UnitPixel);

			CString szFileName;
			szFileName.Format("\\%s_%02d_%02d.PNG", m_pDNDMap->m_szMapName, nColumn, nRow);

			CString szPath = szDirPath + szFileName;
			CStringW wPath = szPath;

			m_pDNDMap->m_Cells[nColumn][nRow].m_pBitmap->Save(wPath, &pngEncoderClsId, NULL);

			szPath.Replace(m_pApp->m_szEXEPath, "<$DMAPATH>");
			strcpy(m_pDNDMap->m_Cells[nColumn][nRow].m_szBitmapPath, szPath.GetBuffer(0));
			
			nDrawnHeight += nSourceHeight;

			++nCellsProcessed;

			m_cProgressBar.SetPos(nCellsProcessed);
		}

		nDrawnWidth += m_pDNDMap->m_nPixelSizeX;

	}

	delete m_pOriginalBitmap;

	if (bFailed)
	{
		AfxMessageBox("Failed to create images ! (Out of Memory ?)", MB_OK);
	}

	OnBnClickedOk();
}


