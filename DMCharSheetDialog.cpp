// DMCharSheetDialog.cpp : implementation file
//

#include "stdafx.h"


#include "DM Helper.h"
#include "DMInventoryDialog.h"
#include "DMCharSpellsDialog.h"
#include "DMCharViewDialog.h"
#include "DMNPCViewDialog.h"
#include "DMCharSheetDialog.h"
#include "cDMCharacterPortraitDialog.h"


// #ifdef _DEBUG
// #define new DEBUG_NEW
// #undef THIS_FILE
// static char THIS_FILE[] = __FILE__;
// #endif

#define MAX_CS_PAGES	3

static OPENFILENAME    g_ofn;
char g_CS_szFilename[MAX_PATH];

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	using namespace Gdiplus;
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if(size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if(pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for(UINT j = 0; j < num; ++j)
	{
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}    
	}

	free(pImageCodecInfo);
	return 0;
}

//size of the char sheet is 841x1088
int nMaxSizeX = 841;
int nMaxSizeY = 1088;

/////////////////////////////////////////////////////////////////////////////
// DMCharSheetDialog dialog


DMCharSheetDialog::DMCharSheetDialog(cDNDCharacter *_Character, CDMCharViewDialog *_pCharViewDialog, CWnd* pParent /*=NULL*/)
	: CDialog(DMCharSheetDialog::IDD, pParent)
	, m_nSliderOffset(0)
{
	//{{AFX_DATA_INIT(DMCharSheetDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_pCharacter = _Character;
	m_pPortraitBitmap = NULL;
	m_bSaveSheet = FALSE;
	m_bPrintSheet = FALSE;
	m_pCharViewDialog = _pCharViewDialog;
	m_nPage = 0;

	BOOL bSmallFontSize = m_pApp->m_Settings.m_bUseSmallFont;
	m_pApp->m_Settings.m_bUseSmallFont = FALSE;
	Create(DMCharSheetDialog::IDD, pParent);
	m_pApp->m_Settings.m_bUseSmallFont = bSmallFontSize;
}


void DMCharSheetDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DMCharSheetDialog)
	DDX_Control(pDX, IDC_PORTRAIT_BUTTON, m_cPortraitButton);
	DDX_Control(pDX, IDC_SAVE_BUTTON, m_cSaveButton);
	//}}AFX_DATA_MAP
	DDX_Slider(pDX, IDC_CHARSHEET_SLIDER, m_nSliderOffset);
	DDV_MinMaxInt(pDX, m_nSliderOffset, 0, 1087);
	DDX_Control(pDX, IDC_CHARSHEET_SLIDER, m_cSliderOffset);
}


BEGIN_MESSAGE_MAP(DMCharSheetDialog, CDialog)
	//{{AFX_MSG_MAP(DMCharSheetDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_SAVE_BUTTON, OnSaveButton)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_PORTRAIT_BUTTON, OnPortraitButton)
	ON_BN_CLICKED(IDC_PRINT_BUTTON, OnPrintButton)
	ON_BN_CLICKED(IDC_FLIP_PAGE_BUTTON, OnFlipPageButton)
	ON_BN_CLICKED(IDC_PAGE_BACK_BUTTON, OnPageBackButton)
	//}}AFX_MSG_MAP
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DMCharSheetDialog message handlers

BOOL DMCharSheetDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_cSliderOffset.SetRange(0, 1087);

	if(m_pCharacter != NULL)
	{
		SetWindowText(m_pCharacter->m_szCharacterName);
	}

	ShowWindow(SW_SHOW);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void DMCharSheetDialog::OnOK() 
{
	// TODO: Add extra validation here
	
	//CDialog::OnOK();
}

void DMCharSheetDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}


void DMCharSheetDialog::DrawContainerCoins(POBJECTTYPE pObj, int *pnItemCount, int *pnStartX, int *pnStartY, CDC *pmemdc, int nLevel, int nShiftY)
{
	CString szCoins = _T("");
	CString szTabs = _T("");

	for(int t = 0; t < nLevel; ++t)
	{
		szTabs += _T("   ");
	}

	szCoins += _T(" [");

	BOOL bFirstCoin = TRUE;
	CString szCoin = _T("");
	if(pObj->m_nContainerPlatinum)
	{
		if(!bFirstCoin) {szCoins += _T(", ");} bFirstCoin = FALSE;
		szCoin.Format("%ld p.p.", pObj->m_nContainerPlatinum);
		szCoins += szCoin;
	}
	if(pObj->m_nContainerGold)
	{
		if(!bFirstCoin) {szCoins += _T(", ");} bFirstCoin = FALSE;
		szCoin.Format("%ld g.p.", pObj->m_nContainerGold);
		szCoins += szCoin;
	}
	if(pObj->m_nContainerElectrum)
	{
		if(!bFirstCoin) {szCoins += _T(", ");} bFirstCoin = FALSE;
		szCoin.Format("%ld e.p.", pObj->m_nContainerElectrum);
		szCoins += szCoin;
	}
	if(pObj->m_nContainerSilver)
	{
		if(!bFirstCoin) {szCoins += _T(", ");} bFirstCoin = FALSE;
		szCoin.Format("%ld s.p.", pObj->m_nContainerSilver);
		szCoins += szCoin;
	}
	if(pObj->m_nContainerCopper)
	{
		if(!bFirstCoin) {szCoins += _T(", ");} bFirstCoin = FALSE;
		szCoin.Format("%ld c.p.", pObj->m_nContainerCopper);
		szCoins += szCoin;
	}

	szCoins += _T("]");

	CString szFinal;
	szFinal.Format("%s%s", szTabs, szCoins);

	*pnItemCount += 1;
			
	
	DrawCSText(szFinal.GetBuffer(0), *pnStartX, *pnStartY, pmemdc);

	*pnStartY += 16;

	if(*pnStartY > 320 + nShiftY)
	{
		*pnStartY = 106 + nShiftY;
		*pnStartX += 246;
	}

}

void DMCharSheetDialog::DrawSubInventory(int *pnItemCount, DWORD dwContainerID, int *pnStartX, int *pnStartY, CDC *pmemdc, int nLevel, int nShiftY)  // we can go recursive with this sumbitch
{

	if(nLevel > MAX_CHARACTER_INVENTORY)
		return;

	CString szTemp;
	CString szWeight;

	CString szTabs = _T("");

	for(int t = 0; t < nLevel; ++t)
	{
		szTabs += _T("   ");
	}

	for(int i = 0; i < MAX_CHARACTER_INVENTORY && *pnItemCount < 42; ++i)
	{
		POBJECTTYPE pObj = &m_pCharacter->m_Inventory[i];

		if(pObj != NULL && pObj->m_wTypeId)
		{
			if(pObj->m_dwContainerID != dwContainerID)
				continue;

			if(pObj->m_nMagicAdj && !pObj->m_bEnchanted)
			{
				if(pObj->m_bMysteryItem)
					szTemp.Format("%s%s +?", szTabs, pObj->m_szType);
				else if(pObj->m_nMagicAdj > 0)
					szTemp.Format("%s%s +%d", szTabs, pObj->m_szType, pObj->m_nMagicAdj);
				else
					szTemp.Format("%s%s %d", szTabs, pObj->m_szType, pObj->m_nMagicAdj);
			}
			else
			{
				if(pObj->m_bMysteryItem)
				{
					if(pObj->IsScroll())
					{
						szTemp.Format("%sunknown scroll", szTabs);
					}
					else
					{
						szTemp.Format("%sunknown item", szTabs);
					}
				}
				else
				{
					szTemp.Format("%s%s", szTabs, pObj->m_szType);
				}
			}

			CString szFinal;

			if(pObj->m_lAmount > 1)
			{
				szFinal.Format("%s (%ld)", szTemp, pObj->m_lAmount);
			}
			else
			{
				szFinal = szTemp;
			}

			if(*pnItemCount == 41)
			{
				szFinal = "more ....";
			}
			else
			{
				int nAdjWeight = pObj->m_nWeight;
				LONG lAmount = max(1L, pObj->m_lAmount);

				if (pObj->m_bIgnoreEncumbrance)
				{
					nAdjWeight = 0;  // DM FIAT
				}

				//check for exceptions
				switch(pObj->m_ObjectType)
				{
					case DND_OBJECT_TYPE_ARMOR:
					{
						if(m_pCharacter->m_ArmorWorn.m_dwObjectID == pObj->m_dwObjectID && (m_pCharacter->m_ArmorWorn.m_nMagicAdj || pObj->m_bEnchanted) ) // magical armor is weightless
						{
							if(lAmount > 1)
								lAmount -= 1;
							else
								nAdjWeight = 0;
						}
						break;
					}
					case DND_OBJECT_TYPE_HELMET:
					{
						if(m_pCharacter->m_HelmWorn.m_dwObjectID == pObj->m_dwObjectID && m_pCharacter->m_ArmorWorn.m_wTypeId) // helmets are weightless if worn with armor if not great helms
						{
							if(m_pCharacter->m_HelmWorn.m_nFlags % 10 == 0)
							{
								if(lAmount > 1)
									lAmount -= 1;
								else
									nAdjWeight = 0;
							}
						}
						break;
					}
				}

				int nWeight = nAdjWeight * lAmount;

				if(pObj->IsMagicBag())
				{
					//count the bag but not the coins
				}
				else if(m_pCharacter->InventorySlotIsInBagOfHolding(pObj, 0))
				{
					nWeight = 0;
				}
				else if(pObj->IsCoinContainer())
				{
					int nCoinFudge = 1;
					if(m_pApp->m_Settings.m_nGPEncumbranceFactor > 0)
						nCoinFudge = m_pApp->m_Settings.m_nGPEncumbranceFactor;

					LONG lCoinWeight = pObj->GetCoinWeight() / nCoinFudge;

					nWeight += lCoinWeight;
				}

				if(nWeight)
				{
					szWeight.Format("%d", nWeight);
					DrawCSText(szWeight.GetBuffer(0), *pnStartX+200, *pnStartY, pmemdc);
				}
				else
				{
					DrawCSText("-", *pnStartX+200, *pnStartY, pmemdc);
				}

				*pnItemCount += 1;
			}
			
			DrawCSText(szFinal.GetBuffer(0), *pnStartX, *pnStartY, pmemdc);

			*pnStartY += 16;

			if (*pnStartY > 320 + nShiftY)
			{
				*pnStartY = 106 + nShiftY;
				*pnStartX += 246;
			}

			if(pObj->IsCoinContainer())
			{
				DrawContainerCoins(pObj, pnItemCount, pnStartX, pnStartY, pmemdc, nLevel, nShiftY);
			}

			if(pObj->IsContainer())
			{
				DrawSubInventory(pnItemCount, pObj->m_dwObjectID, pnStartX, pnStartY, pmemdc, nLevel + 1, nShiftY);
			}
		}
	}
}

void DMCharSheetDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// Do not call CDialog::OnPaint() for painting messages

	CString szTemp;
	
	int nPrintPages = 1;

	CBitmap bmp;		// shatner
	//CBitmap base_bmp;		// shatner
	CBitmap *poldbmp;
	CDC memdc;
	CDC* pDC;
	pDC = &dc;


	nMaxSizeY = 1088;
	if (m_nPage == 0)
	{
		nMaxSizeY = 2176;
	}

	int nDisplayWeapons = 4;
	int nAdjustWeaponLabelX = 0;
	int nAdjustWeaponTableX = 0;
	int nAdjustWeaponTableY = 0;

	int nToHitCellSizeX = 18;
	int nToHitCellSizeY = 15;
	int nToHitDamageBoxOffset = 0;

	int nAdjustWeaponSpeedX = 0;
	int nAdjustWeaponSpaceRange = 0;


	BOOL bDrawThiefSkills = FALSE;

	//CBrush brush;
    //CBrush* pOldBr;

	if(m_pCharacter->m_szPortraitPath[0] != 0 && m_pPortraitBitmap == NULL)
	{
		szTemp.Format("%s", m_pCharacter->m_szPortraitPath);

		szTemp.Replace("<$DMAPATH>", m_pApp->m_szEXEPath);

		LPWSTR wcsFile = szTemp.AllocSysString();
		m_pPortraitBitmap = new Bitmap(wcsFile , FALSE);
	}
   
	// Load the bitmap resource

	int nBitMapResource = 0;

	if(m_nPage == 3)
	{
		bmp.LoadBitmap( IDB_SCROLL_BITMAP );
	}
	else if(m_nPage == 2)
	{
		bmp.LoadBitmap( IDB_SCROLL_BITMAP );
	}
	else if(m_nPage == 1)
	{
		bmp.LoadBitmap( IDB_SPELLBOOK_BITMAP );
	}
	//else if(m_nPage == 1)
	//{
	//	bmp.LoadBitmap( IDB_INVENTORY_BITMAP );
	//}
	#if CUSTOM_CLASSES
	else if (m_pCharacter->m_Class[1] || IsCustomClass(m_pCharacter->m_Class[0]))
	#else
	else if(m_pCharacter->m_Class[1])
	#endif
	{
		bmp.LoadBitmap( IDB_MULTICLASS_BITMAP );
		nBitMapResource = IDB_MULTICLASS_BITMAP;

		nDisplayWeapons = 3;
		nAdjustWeaponLabelX = -15;

		nAdjustWeaponTableX = 0;
		nAdjustWeaponTableY = -8;

		nToHitCellSizeX = 20;

		nToHitDamageBoxOffset = 35;

		nAdjustWeaponSpeedX = 20;

		bDrawThiefSkills = TRUE;
	}
	else
	{
		switch((m_pCharacter->m_Class[0]))
		{
			case DND_CHARACTER_CLASS_CLERIC:	
			case DND_CHARACTER_CLASS_DRUID:			
			{
				bmp.LoadBitmap( IDB_CLERIC_BITMAP );
				nBitMapResource = IDB_CLERIC_BITMAP;

				nAdjustWeaponLabelX = -4;

				nAdjustWeaponTableX = 16;
				nAdjustWeaponTableY = -12;

				nToHitCellSizeX = 20;
				nToHitCellSizeY = 17;

				nToHitDamageBoxOffset = 15;
				nAdjustWeaponSpeedX = 17;

				break;
			}

			case DND_CHARACTER_CLASS_MAGE:		
			case DND_CHARACTER_CLASS_ILLUSIONIST:	
			{
				bmp.LoadBitmap( IDB_MAGICUSER_BITMAP );
				nBitMapResource = IDB_MAGICUSER_BITMAP;

				nDisplayWeapons = 3;

				nAdjustWeaponLabelX = -10;

				nAdjustWeaponTableX = 2;
				nAdjustWeaponTableY = -2;

				nToHitCellSizeX = 19;
				nToHitCellSizeY = 16;

				nToHitDamageBoxOffset = 15;

				nAdjustWeaponSpeedX = 20;
				nAdjustWeaponSpaceRange = -10;

				break;
			}

			case DND_CHARACTER_CLASS_THIEF:			
			case DND_CHARACTER_CLASS_THIEF_ACROBAT:	
			case DND_CHARACTER_CLASS_ASSASSIN:	
			case DND_CHARACTER_CLASS_MONK:
			{
				bmp.LoadBitmap( IDB_THIEF_BITMAP );
				nBitMapResource = IDB_THIEF_BITMAP;

				nAdjustWeaponLabelX = -8;

				nAdjustWeaponTableX = -20;
				nAdjustWeaponTableY = +6;

				nToHitCellSizeY = 16;
				nToHitDamageBoxOffset = 10;

				nAdjustWeaponSpeedX = 17;

				bDrawThiefSkills = TRUE;

				break;
			}
			default:
			{
				bmp.LoadBitmap( IDB_FIGHTER_BITMAP );
				nBitMapResource = IDB_FIGHTER_BITMAP;
				break;
			}
		}
		
	}

	//base_bmp.LoadBitmap(IDB_CHAR_SHEET_BASE_BITMAP);

	// Create a compatible memory DC
	memdc.CreateCompatibleDC( pDC );

	// Select the bitmap into the DC
	poldbmp = memdc.SelectObject(&bmp);

	CFont newfont;
	CFont* oldfont;

	int nSize = 17;   // 12 point font
	BOOL success = newfont.CreatePointFont(nSize * 10, "Cloister Black", pDC);

	oldfont = memdc.SelectObject(&newfont);

	SetBkMode(memdc, TRANSPARENT);

	if(m_nPage == 0)
	{
		nPrintPages = 2;

		if(m_nSliderOffset > 0)
		{
			m_cPortraitButton.ShowWindow(SW_HIDE);
		}
		else
		{
			m_cPortraitButton.ShowWindow(SW_SHOW);
		}

		Graphics graphics(memdc);

		if(m_pCharacter->m_szPortraitPath[0] && m_pPortraitBitmap != NULL)
		{
			try
			{
				graphics.DrawImage(m_pPortraitBitmap, 512+2, 150+2, 192-4, 193-4);
			}
			catch(...)
			{
				memset(m_pCharacter->m_szPortraitPath, 0, MAX_PATH * sizeof(char));
				delete m_pPortraitBitmap;
			}
		}
		
		DrawCSText(m_pCharacter->m_szCharacterName, 146, 155, &memdc);

		int nDisplayedStats = 6;

		if (g_bUseUnearthedArcana) // if displaying comeliness, it is displayed in a smaller font below
		{
			nDisplayedStats = 5;
		}

		for (int i = 0; i < nDisplayedStats; ++i)
		{
			DrawCSText(m_pCharacter->m_nDisplayStats[i], 98, 352+i*33, &memdc);
		}

		if(m_pCharacter->m_nDisplayStats[ATTRIB_EXSTR])
		{
			szTemp.Format("%02d", m_pCharacter->m_nDisplayStats[ATTRIB_EXSTR]);
			DrawCSText(szTemp.GetBuffer(0), 152, 352, &memdc);
		}

		CFont newfont2;
		nSize = 12;   // 12 point font
		success = newfont2.CreatePointFont(nSize * 10, "HamletOrNot", pDC);
		memdc.SelectObject(&newfont2);

		szTemp = " ";
		CString szLevel;
		if(m_pCharacter->m_Class[2])
		{
			szTemp.Format("%s/%s/%s", GetClassName(m_pCharacter->m_Class[0]), GetClassName(m_pCharacter->m_Class[1]), GetClassName(m_pCharacter->m_Class[2]));
			szLevel.Format("%d/%d/%d", m_pCharacter->m_nLevel[0], m_pCharacter->m_nLevel[1], m_pCharacter->m_nLevel[2]);
		}
		else if(m_pCharacter->m_Class[1])
		{
			szTemp.Format("%s/%s", GetClassName(m_pCharacter->m_Class[0]), GetClassName(m_pCharacter->m_Class[1]));
			szLevel.Format("%d/%d", m_pCharacter->m_nLevel[0], m_pCharacter->m_nLevel[1]);
		}
		else
		{
			szTemp.Format("%s", GetClassName(m_pCharacter->m_Class[0]));
			szLevel.Format("%d", m_pCharacter->m_nLevel[0]);
		}

		DrawCSText(szTemp.GetBuffer(0), 171, 192, &memdc);

		DrawCSText(GetRaceName(m_pCharacter->m_nRace), 171, 211, &memdc);

		DrawCSText(m_pCharacter->m_szSecondarySkill, 175, 295, &memdc);
		DrawCSText(GetVisionTypeName(m_pCharacter->m_VisionType), 290, 295, &memdc);

		szTemp.Format("%d in 20", GetCharacterListeningScore(m_pCharacter));
		DrawCSText(szTemp.GetBuffer(0), 360, 295, &memdc);

		//DrawCSText(m_pCharacter->m_nLevel[0], 391, 192, &memdc);
		DrawCSText(szLevel.GetBuffer(0), 391, 192, &memdc);
		DrawCSText(GetAlignmentName(m_pCharacter->m_nAlignment), 396, 211, &memdc);

		DrawCSText(m_pCharacter->m_szPatronDeity, 210, 229, &memdc);
		DrawCSText(m_pCharacter->m_szReligion, 390, 229, &memdc);
		DrawCSText(m_pCharacter->m_szPlaceofOrigin, 220, 249, &memdc);


		//movement rates
		szTemp.Format("%d", m_pCharViewDialog->m_nCharacterMovementRate);
		DrawCSText(szTemp.GetBuffer(0), 125, 282, &memdc);

		if(m_pCharacter->m_nConcealedMove)
		{
			szTemp.Format("%d", m_pCharacter->m_nConcealedMove);
			DrawCSText(szTemp.GetBuffer(0), 215, 270, &memdc);
		}

		if(m_pCharacter->m_nClimbingMove)
		{
			szTemp.Format("%d", m_pCharacter->m_nClimbingMove);
			DrawCSText(szTemp.GetBuffer(0), 295, 270, &memdc);
		}

		if(m_pCharacter->m_nSpecialMove)
		{
			szTemp.Format("%d", m_pCharacter->m_nSpecialMove);
			DrawCSText(szTemp.GetBuffer(0), 380, 270, &memdc);
		}

		int nSTRDamAdj = 0;
		int nWeightAllow = 0;
		int nOpenDoors = 0;
		int nBendBars = 0;

		int nSTRToHitAdj = CalculateSTRAdjustments(m_pCharacter, &nSTRDamAdj, &nWeightAllow, &nOpenDoors, &nBendBars);

		DrawCSText(nSTRToHitAdj, 196, 352, &memdc, TRUE);
		DrawCSText(nSTRDamAdj, 237, 352, &memdc, TRUE);
		szTemp.Format("1-%d", nOpenDoors);
		DrawCSText(szTemp.GetBuffer(0), 294, 352, &memdc);
		szTemp.Format("%d", nBendBars);
		DrawCSText(szTemp.GetBuffer(0), 349, 352, &memdc);

		int nKnowSpell = 0;
		int nMinSpells = 0;
		int nMaxSpells = 0;

		int nAddLang = CalculateINTAdjustments(m_pCharacter, &nKnowSpell, &nMinSpells, &nMaxSpells);
		DrawCSText(nAddLang, 180, 384, &memdc, FALSE);
		DrawCSText(nKnowSpell, 242, 384, &memdc, FALSE);
		DrawCSText(nMinSpells, 297, 384, &memdc, FALSE);

		if(nMaxSpells == 9999)
			DrawCSText("all", 347, 387, &memdc);
		else
			DrawCSText(nMaxSpells, 350, 384, &memdc, FALSE);


		int nSpellFailure = 100;
		char *szSpellBonus;
		int nMagicAttackAdj = CalculateWISAdjustments(m_pCharacter,  &szSpellBonus, &nSpellFailure);

		DrawCSText(nMagicAttackAdj, 192, 417, &memdc, TRUE);
		DrawCSText(szSpellBonus, 257, 417, &memdc);
		DrawCSText(nSpellFailure, 332, 417, &memdc, FALSE);


		int nDefenseAdj = 0;
		int nReactAttack = CalculateDEXAdjustments(m_pCharacter, &nDefenseAdj);

		DrawCSText(nReactAttack, 192, 450, &memdc, TRUE);
		DrawCSText(nDefenseAdj, 335, 450, &memdc, TRUE);
		DrawCSText(nReactAttack, 257, 450, &memdc, TRUE); //attack adj is same as raection adj

		int nSysShock = 0;
		int nResSurvival = 0;
		int nConBonus = CalculateCONAdjustments(m_pCharacter, &nSysShock, &nResSurvival);

		DrawCSText(nConBonus, 193, 483, &memdc, TRUE);
		DrawCSText(nSysShock, 265, 483, &memdc);
		DrawCSText(nResSurvival, 339, 483, &memdc);

		int nLoyaltyBase = 0;
		int nReactAdj = 0;

		int nMaxHenchmen = CalculateCHAAdjustments(m_pCharacter, &nLoyaltyBase, &nReactAdj);
		DrawCSText(nMaxHenchmen, 200, 516, &memdc, FALSE);
		DrawCSText(nLoyaltyBase, 265, 516, &memdc, TRUE);
		DrawCSText(nReactAdj, 340, 516, &memdc, TRUE);


		/////////////////////////////////////////////////////////

		//shitter
		if (g_bUseUnearthedArcana) // if displaying comeliness, it is displayed in a smaller font here with charisma
		{
			DrawCSText(m_pCharacter->m_nDisplayStats[5], 98, 347 + 5 * 33, &memdc);
			DrawCSText(m_pCharacter->m_nDisplayStats[6], 106, 357 + 5 * 33, &memdc);
		}


		if(m_pCharacter->m_nCurrentArmorClass < 0)
			DrawCSText(m_pCharacter->m_nCurrentArmorClass, 110, 588, &memdc);
		else
			DrawCSText(m_pCharacter->m_nCurrentArmorClass, 114, 588, &memdc);

		DrawCSText(m_pCharacter->m_nHitPoints, 114, 630, &memdc);

		CFont newfont3;
		nSize = 10;   // 10 point font
		success = newfont3.CreatePointFont(nSize * 10, "HamletOrNot", pDC);
		memdc.SelectObject(&newfont3);

		DrawCSText(m_pCharacter->m_szPlayerName, 100, 69, &memdc);
		DrawCSText(m_pCharacter->m_szDateCreated, 100, 94, &memdc);


		if(m_pCharacter->m_ArmorWorn.m_nMagicAdj == 0 || m_pCharacter->m_ArmorWorn.m_bEnchanted)
			szTemp.Format("%s", m_pCharacter->m_ArmorWorn.m_szType);
		else if(m_pCharacter->m_ArmorWorn.m_nMagicAdj > 0)
			szTemp.Format("%s (+%d)", m_pCharacter->m_ArmorWorn.m_szType, m_pCharacter->m_ArmorWorn.m_nMagicAdj);
		else 
			szTemp.Format("%s (%d)", m_pCharacter->m_ArmorWorn.m_szType, m_pCharacter->m_ArmorWorn.m_nMagicAdj);


		DrawCSText(szTemp.GetBuffer(0), 145, 580, &memdc);

		DrawCSText(m_pCharacter->m_ArmorWorn.m_nArmorClass, 245, 580, &memdc);  // base AC

		DrawCSText(nDefenseAdj * -1, 168, 600, &memdc);

		int nACMagicAdj = m_pCharacter->m_ArmorWorn.m_nMagicAdj +  m_pCharacter->m_ShieldWorn.m_nMagicAdj;  // TODO add in rings of protection etc
		DrawCSText(nACMagicAdj, 225, 600, &memdc);

		DrawCSText(m_pCharacter->m_nUnshieldedArmorClass, 304, 600, &memdc);

		DrawCSText(m_pCharacter->m_nRearArmorClass, 382, 600, &memdc);

		DrawCSText(nConBonus, 168, 622, &memdc, TRUE);


		int nHitDie = GetHitDieTypeByClass(m_pCharacter->m_Class[0]);
		szTemp.Format("d%d", nHitDie);
		DrawCSText(szTemp.GetBuffer(0), 240, 622, &memdc);

		if(m_pCharacter->m_Class[2]) //three classes
		{
			szTemp.Format("+ d%d + d%d / 3", GetHitDieTypeByClass(m_pCharacter->m_Class[1]), GetHitDieTypeByClass(m_pCharacter->m_Class[2]));
			DrawCSText(szTemp.GetBuffer(0), 300, 622, &memdc);
		}
		else if(m_pCharacter->m_Class[1]) //two classes
		{
			szTemp.Format("+ d%d / 2", GetHitDieTypeByClass(m_pCharacter->m_Class[1]));
			DrawCSText(szTemp.GetBuffer(0), 300, 622, &memdc);
		}

		int nSavesMatrix[5];
		GetSavingThrows(m_pCharacter, nSavesMatrix);

		for(i=0; i< 5; ++i)
		{
			DrawCSText(nSavesMatrix[i], 772, 372+31*i, &memdc, FALSE);
		}

		//Languages
		szTemp = _T("");
		CString szTemp2 = _T("");
		int nLine = 0;
		int nLen = 0;
		BOOL bFirst = TRUE;
		for(int i = 0; i < MAX_CHARACTER_LANGUAGES && m_pCharacter->m_nLanguages[i] && nLine < 2; ++i)
		{
			//if(m_pCharacter->m_nLanguages[i] > 0)
			{
				cDNDLanguage *pLanguage = (cDNDLanguage *)m_pApp->m_LanguageArray.GetAt(m_pCharacter->m_nLanguages[i]);

				if(bFirst)
				{
					nLen += pLanguage->m_szLanguageName.GetLength();
					bFirst = FALSE;
				}
				else
				{
					nLen += (pLanguage->m_szLanguageName.GetLength() + 2);

					if(nLine == 0)
					{
						szTemp += _T(", ");
					}
					else if(nLine == 1)
					{
						szTemp2 += _T(", ");
					}
				}

				if(nLen > 42)
				{
					++nLine;
					nLen = 0;
					//bFirst = TRUE;
				}

				if(nLine == 0)
				{
					szTemp += pLanguage->m_szLanguageName;
					
				}
				else if(nLine == 1)
				{
					szTemp2 += pLanguage->m_szLanguageName;	
				}
				else if(nLine == 2)
				{
					szTemp2 += _T("more ...");
				}

			} 
		}

		DrawCSText(szTemp.GetBuffer(0), 452, 475, &memdc);
		DrawCSText(szTemp2.GetBuffer(0), 390, 493, &memdc);


		//////////////////////////////
		//To Hit Chart

		
		int nAttackMatrix[21];
		GetToHitChart(m_pCharacter, nAttackMatrix);

		DrawCSText("BASE", 275+nAdjustWeaponTableX+nAdjustWeaponLabelX, 723+nAdjustWeaponTableY, &memdc);

		for(i=0; i< 9; ++i)
		{
			int nWeaponVsACAdj = 0;

			szTemp.Format("%d",nAttackMatrix[i] - nSTRToHitAdj - nWeaponVsACAdj);
			DrawCSText(szTemp.GetBuffer(0), (523+nAdjustWeaponTableX)+nToHitCellSizeX*i, 721+nAdjustWeaponTableY, &memdc);
		}


		int nLineY = 721+nAdjustWeaponTableY+nToHitCellSizeY*2;

		// open handed attacks
		DrawCSText("open hand", 275+nAdjustWeaponTableX+nAdjustWeaponLabelX, 723+nAdjustWeaponTableY+nToHitCellSizeY, &memdc);

		int nNoWeapon[11] = {-9,-8,-7,-5,-3,-1,0,0,2,0,4};

		for(i=0; i< 9; ++i)
		{
			int nWeaponVsACAdj = 0;

			if(m_pApp->m_Settings.m_bUseWeapons_vs_AC_Chart)
			{
				int nIndex = 10-i;
				nWeaponVsACAdj = nNoWeapon[nIndex];
			}

			szTemp.Format("%d",nAttackMatrix[i] - nSTRToHitAdj - nWeaponVsACAdj);
			DrawCSText(szTemp.GetBuffer(0), (523+nAdjustWeaponTableX)+nToHitCellSizeX*i, 721+nAdjustWeaponTableY+nToHitCellSizeY, &memdc);
		}

		szTemp.Format("%s", m_pCharacter->GetUnarmedDamageDesc());
		DrawCSText(szTemp.GetBuffer(0), 690+nAdjustWeaponTableX+nAdjustWeaponLabelX+nToHitDamageBoxOffset, nLineY + -1*nToHitCellSizeY, &memdc);

		// -----------------------------------------

		for(int j = 0; j < nDisplayWeapons; ++j)
		{
			if(!m_pCharacter->m_SelectedWeapons[j].m_wTypeId)
				continue;

			if(m_pCharacter->m_SelectedWeapons[j].m_bMysteryItem)
				continue;

			DrawCSText(m_pCharacter->m_SelectedWeapons[j].m_szType, 275+nAdjustWeaponTableX+nAdjustWeaponLabelX, nLineY + j*nToHitCellSizeY, &memdc);

			if(j == 0) // weapon in hand
			{
				int nIppyY = 46;

				if(nBitMapResource == IDB_MAGICUSER_BITMAP) // yeah I got lazy here
				{
					nIppyY = 42;
				}
				else if(nBitMapResource == IDB_THIEF_BITMAP) 
				{
					nIppyY = 48;
				}

				DrawCSText(m_pCharacter->m_SelectedWeapons[j].m_szType, 275+nAdjustWeaponTableX+nAdjustWeaponLabelX-100, nLineY + j*nToHitCellSizeY-nIppyY, &memdc);
			}

			int nToHitAdj = 0;
			int nDamageAdj =0;

			if(m_pCharacter->m_SelectedWeapons[j].m_nExceptionalStrength)
			{
				nToHitAdj += nSTRToHitAdj;
				nDamageAdj += nSTRDamAdj;
			}

			cDNDWeapon *pDamageWeapon = &m_pCharacter->m_SelectedWeapons[j];

			if(IsMissileWeapon(&m_pCharacter->m_SelectedWeapons[j]) && IsValidAmmoForWeapon(&m_pCharacter->m_SelectedWeapons[j], &m_pCharacter->m_SelectedAmmo))
			{
				nToHitAdj += m_pCharacter->m_SelectedAmmo.m_nMagicAdj;
				nDamageAdj += m_pCharacter->m_SelectedAmmo.m_nMagicAdj;

				if(m_pCharacter->m_SelectedAmmo.m_szDamageSmall[0] != '-')  // for sling stone weirdness
				{
					pDamageWeapon = &m_pCharacter->m_SelectedAmmo;
				}
			}

			int nSpecializedToHitBonus = 0;
			int nSpecializedDamageBonus = 0;
			if(m_pCharacter->IsProficientWithWeapon(&m_pCharacter->m_SelectedWeapons[j], &nSpecializedToHitBonus, &nSpecializedDamageBonus) == FALSE)
			{
				int nProfPenalty = 0;
				int nNumProfs = CalculateWeaponProficiencies(m_pCharacter, &nProfPenalty);
				nToHitAdj += nProfPenalty;
				nDamageAdj += nProfPenalty;
			}

			nToHitAdj += nSpecializedToHitBonus;
			nDamageAdj += nSpecializedDamageBonus;

			if(m_pCharacter->m_SelectedWeapons[j].m_nMagicAdj)
			{
				DrawCSText(m_pCharacter->m_SelectedWeapons[j].m_nMagicAdj, 365+nAdjustWeaponTableX+nAdjustWeaponLabelX, nLineY + j*nToHitCellSizeY, &memdc, TRUE);
			}

			DrawCSText(m_pCharacter->m_SelectedWeapons[j].m_szSpaceRange, 415+nAdjustWeaponTableX+nAdjustWeaponLabelX+nAdjustWeaponSpaceRange, nLineY + j*nToHitCellSizeY, &memdc);
			DrawCSText(m_pCharacter->m_SelectedWeapons[j].m_szSpeed, 485+nAdjustWeaponTableX+nAdjustWeaponLabelX +nAdjustWeaponSpeedX, nLineY + j*nToHitCellSizeY, &memdc);

			nToHitAdj += m_pCharacter->m_SelectedWeapons[j].m_nMagicAdj;
			nDamageAdj += m_pCharacter->m_SelectedWeapons[j].m_nMagicAdj;

			for(i=0; i < 9; ++i)
			{
				int nWeaponVsACAdj = 0;

				if(m_pApp->m_Settings.m_bUseWeapons_vs_AC_Chart)
				{
					int nIndex = 10-i;
					nWeaponVsACAdj = m_pCharacter->m_SelectedWeapons[j].m_nWeaponVSAC[nIndex];
				}

				szTemp.Format("%d", nAttackMatrix[i] - nToHitAdj - nWeaponVsACAdj);
				DrawCSText(szTemp.GetBuffer(0), (523+nAdjustWeaponTableX)+nToHitCellSizeX*i, nLineY + j*nToHitCellSizeY, &memdc);
			}

			if(nDamageAdj)
			{
				szTemp.Format("%s/%s +%d %s", pDamageWeapon->m_szDamageSmall, pDamageWeapon->m_szDamageLarge, nDamageAdj, GetMonkWeaponDamageAdj(m_pCharacter, j));
			}
			else
			{
				szTemp.Format("%s/%s %s", pDamageWeapon->m_szDamageSmall, pDamageWeapon->m_szDamageLarge, GetMonkWeaponDamageAdj(m_pCharacter, j));
			}
			DrawCSText(szTemp.GetBuffer(0), 690+nAdjustWeaponTableX+nAdjustWeaponLabelX+nToHitDamageBoxOffset, nLineY + j*nToHitCellSizeY, &memdc);
		}

		//attacks per round / weaponless combat / weapons of proficiency

		CalculateWeaponlessCombatMatrix(m_pCharacter);

		int nWCX = 0;
		int nWCY = 0;
		int nWCSizeX = 0;
		int nWCSizeY = 0;
		int nProfX = 578;
		int nProfY = 577;

		if(nBitMapResource == IDB_FIGHTER_BITMAP)
		{
			szTemp.Format("%s", CalculateAttacksPerRound(m_pCharacter, TRUE));
			DrawCSText(szTemp.GetBuffer(0), 236, 827, &memdc);

			nWCX = 123;
			nWCY = 753;
			nWCSizeX = 52;
			nWCSizeY = 23;

			nProfX = 578;
			nProfY = 577;

			DrawTurnUndead(&memdc, 20, 0, 50);
		}
		else if(nBitMapResource == IDB_MULTICLASS_BITMAP)
		{
			szTemp.Format("%s", CalculateAttacksPerRound(m_pCharacter, TRUE));
			DrawCSText(szTemp.GetBuffer(0), 330, 805, &memdc);

			nWCX = 120;
			nWCY = 746;
			nWCSizeX = 52;
			nWCSizeY = 23;

			nProfX = 578;
			nProfY = 577;

			DrawTurnUndead(&memdc, 4, -44, 55);
		}
		else if(nBitMapResource == IDB_CLERIC_BITMAP)
		{
			nWCX = 124;
			nWCY = 746;
			nWCSizeX = 60;
			nWCSizeY = 23;

			nProfY = 575;

			DrawTurnUndead(&memdc, 0, 0, 55);
		}
		else if(nBitMapResource == IDB_THIEF_BITMAP)
		{
			nWCX = 124;
			nWCY = 780;
			nWCSizeX = 43;
			nWCSizeY = 23;
		}
		else if(nBitMapResource == IDB_MAGICUSER_BITMAP)
		{
			nWCX = 124;
			nWCY = 752;
			nWCSizeX = 52;
			nWCSizeY = 23;
		}

		for(int i=0; i < 3; ++i)
		{
			for(int j = 0; j < 3; ++j)
			{
				szTemp.Format("%d", m_pCharacter->m_nWeaponlessCombatAdjustments[i][j]);
				DrawCSText(szTemp.GetBuffer(0), nWCX+i*nWCSizeX, nWCY+j*nWCSizeY, &memdc);
			}
		}

		int nProfPenalty = 0;
		int nProfs = CalculateWeaponProficiencies(m_pCharacter, &nProfPenalty);

		szTemp.Format("%d           %d", nProfs, nProfPenalty);
		DrawCSText(szTemp.GetBuffer(0), nProfX, nProfY, &memdc);

		//////////////////////////////
		if(bDrawThiefSkills)
		{
			float fThiefSkillMatrix[8];
			GetThiefSkills(m_pCharacter, fThiefSkillMatrix);

			int nTSColX[8] = {148, 230, 325, 412, 506, 578, 652, 752};

			for(int ii = 0; ii < 8; ++ii)
			{
				if(ii == 6)
					szTemp.Format("%2.1f", fThiefSkillMatrix[ii]);
				else
					szTemp.Format("%2.0f", fThiefSkillMatrix[ii]);

				DrawCSText(szTemp.GetBuffer(0), nTSColX[ii], 986, &memdc);
			}
		}

		//////////////////////////////

		CFont newfont4;
		nSize = 7;   // 7 point font
		success = newfont4.CreatePointFont(nSize * 10, "SmallFonts", pDC);
		memdc.SelectObject(&newfont4);

		//spells memorized
		if(m_pCharViewDialog->m_pSpellsDialog != NULL && m_pCharViewDialog->m_pSpellsDialog->m_pSpellBook != NULL)
		{
			DrawSpellList(nBitMapResource, &memdc);
		}

		//////////////////////////////

	//} // m_nPage == 0
	//else if(m_nPage == 1)
	//{
		//m_cPortraitButton.ShowWindow(SW_HIDE);

		int nShiftY = 1088;

		CFont newfont5;
		nSize = 10;   // 10 point font
		success = newfont5.CreatePointFont(nSize * 10, "HamletOrNot", pDC);
		memdc.SelectObject(&newfont5);

		int nItemCount = 0;

		int nStartX = 80;
		int nStartY = 106 + nShiftY;

		CString szTemp;
		CString szWeight;

		for(int i = 0; i < MAX_CHARACTER_INVENTORY && nItemCount < 42; ++i)
		{
			POBJECTTYPE pObj = &m_pCharacter->m_Inventory[i];

			if(pObj != NULL && pObj->m_wTypeId)
			{
				if(pObj->m_dwContainerID)
					continue;

				if(pObj->m_nMagicAdj && !pObj->m_bEnchanted)
				{
					if(pObj->m_bMysteryItem)
						szTemp.Format("%s +?", pObj->m_szType);
					else if(pObj->m_nMagicAdj > 0)
						szTemp.Format("%s +%d", pObj->m_szType, pObj->m_nMagicAdj);
					else
						szTemp.Format("%s %d", pObj->m_szType, pObj->m_nMagicAdj);
				}
				else
				{
					if(pObj->m_bMysteryItem)
					{
						if(pObj->IsScroll())
						{
							szTemp.Format("unknown scroll");
						}
						else
						{
							szTemp.Format("unknown item");
						}
					}
					else 
					{
						szTemp.Format("%s", pObj->m_szType);
					}
				}

				CString szFinal;

				if(pObj->m_lAmount > 1)
				{
					szFinal.Format("%s (%ld)", szTemp, pObj->m_lAmount);
				}
				else
				{
					szFinal = szTemp;
				}

				if(nItemCount == 41)
				{
					szFinal = "more ....";
				}
				else
				{
					int nAdjWeight = pObj->m_nWeight;
					LONG lAmount = max(1L, pObj->m_lAmount);

					if (pObj->m_bIgnoreEncumbrance)
					{
						nAdjWeight = 0;  // DM FIAT
					}

					//check for exceptions
					switch(pObj->m_ObjectType)
					{
						case DND_OBJECT_TYPE_ARMOR:
						{
							if(m_pCharacter->m_ArmorWorn.m_dwObjectID == pObj->m_dwObjectID && (m_pCharacter->m_ArmorWorn.m_nMagicAdj || pObj->m_bEnchanted) ) // magical armor is weightless
							{
								if(lAmount > 1)
									lAmount -= 1;
								else
									nAdjWeight = 0;
							}
							break;
						}
						case DND_OBJECT_TYPE_HELMET:
						{
							if(m_pCharacter->m_HelmWorn.m_dwObjectID == pObj->m_dwObjectID && m_pCharacter->m_ArmorWorn.m_wTypeId) // helmets are weightless if worn with armor if not great helms
							{
								if(m_pCharacter->m_HelmWorn.m_nFlags % 10 == 0)
								{
									if(lAmount > 1)
										lAmount -= 1;
									else
										nAdjWeight = 0;
								}
							}
							break;
						}
					}

					int nWeight = nAdjWeight * lAmount;

					if(pObj->IsMagicBag())
					{
						//count the bag but not the coins
					}
					else if(m_pCharacter->InventorySlotIsInBagOfHolding(pObj, 0))
					{
						nWeight = 0;
					}
					else if(pObj->IsCoinContainer())
					{
						int nCoinFudge = 1;
						if(m_pApp->m_Settings.m_nGPEncumbranceFactor > 0)
							nCoinFudge = m_pApp->m_Settings.m_nGPEncumbranceFactor;

						LONG lCoinWeight = pObj->GetCoinWeight() / nCoinFudge;

						nWeight += lCoinWeight;
					}

					if(nWeight)
					{
						szWeight.Format("%d", nWeight);
						DrawCSText(szWeight.GetBuffer(0), nStartX+200, nStartY, &memdc);
					}
					else
					{
						DrawCSText("-", nStartX+200, nStartY, &memdc);
					}
				}
				
				DrawCSText(szFinal.GetBuffer(0), nStartX, nStartY, &memdc);

				nItemCount += 1;

				nStartY += 16;

				if (nStartY > 320 + nShiftY)
				{
					nStartY = 106 + nShiftY;
					nStartX += 246;
				}

				if(pObj->IsCoinContainer())
				{
					DrawContainerCoins(pObj, &nItemCount, &nStartX, &nStartY, &memdc, 0, nShiftY);
				}

				if(pObj->IsContainer())
				{
					DrawSubInventory(&nItemCount, pObj->m_dwObjectID, &nStartX, &nStartY, &memdc, 1, nShiftY);
				}
			}
		}

		int nMaxWeight = 0;
		int nEncumbrance = CalculateEncumbrance(m_pCharacter, &nMaxWeight);

		DrawCSText(nMaxWeight - 350, 180, 338 + nShiftY, &memdc);

		//nMaxWeight, nMaxWeight+350, nMaxWeight+700, nMaxWeight+1050, nMaxWeight+1400
		DrawCSText(nMaxWeight, 320, 338 + nShiftY, &memdc);
		DrawCSText(nMaxWeight + 350, 380, 338 + nShiftY, &memdc);
		DrawCSText(nMaxWeight + 700, 445, 338 + nShiftY, &memdc);
		DrawCSText(nMaxWeight + 1050, 520, 338 + nShiftY, &memdc);

		szTemp.Format("%d", nEncumbrance);
		DrawCSText(szTemp.GetBuffer(0), 720, 342 + nShiftY, &memdc);

		//Experience
		int nNumClasses = 1;
		int nExpBonus = CalculateExperienceBonus(m_pCharacter, -1, &nNumClasses);
		float fExpBonus = ((float)nExpBonus) / 1000.0f;
		szTemp.Format("%0.0f", fExpBonus);

		DrawCSText(szTemp.GetBuffer(0), 755, 509 + nShiftY, &memdc);

		LONG lNextXP = GetExperiencePointsForLevelByClass(m_pCharacter->m_Class[m_pCharacter->m_nDualClassClass], m_pCharacter->m_nLevel[m_pCharacter->m_nDualClassClass] + 1);
		DrawCSText(lNextXP, 710, 496 + nShiftY, &memdc);

		for(int i = 0; i < 3; ++i)
		{
			if(m_pCharacter->m_Class[i] != DND_CHARACTER_CLASS_UNDEF)
			{
				DrawCSText(m_pCharacter->m_lExperience[i], 484, 522 + i * 15 + nShiftY, &memdc);

				if(m_pCharacter->m_lEarnedXP)
				{
					DrawCSText(m_pCharacter->m_lEarnedXP / nNumClasses, 570, 522 + i * 15 + nShiftY, &memdc);
				}

				DrawCSText(m_pCharacter->m_lExperience[i] + m_pCharacter->m_lEarnedXP / nNumClasses, 720, 522 + i * 15 + nShiftY, &memdc);
			}

		}
		
		//Money money money
		DrawCSText(m_pCharacter->m_lCopperCarried, 130, 515 + nShiftY, &memdc);
		DrawCSText(m_pCharacter->m_lSilverCarried, 130, 535 + nShiftY, &memdc);
		DrawCSText(m_pCharacter->m_lElectrumCarried, 145, 555 + nShiftY, &memdc);
		DrawCSText(m_pCharacter->m_lGoldCarried, 120, 575 + nShiftY, &memdc);
		DrawCSText(m_pCharacter->m_lPlatinumCarried, 140, 595 + nShiftY, &memdc);

		DrawCSText(m_pCharacter->m_lCopperCarried + m_pCharacter->m_lSilverCarried + m_pCharacter->m_lElectrumCarried + m_pCharacter->m_lGoldCarried + m_pCharacter->m_lPlatinumCarried, 260, 494 + nShiftY, &memdc);


		if(m_pCharacter->m_nSex == 0)
			szTemp = "male";
		else
			szTemp = "female";

		DrawCSText(szTemp.GetBuffer(0), 80, 708 + nShiftY, &memdc);

		if(m_pCharacter->m_nWeight)
		{
			szTemp.Format("%d lbs", m_pCharacter->m_nWeight);
			DrawCSText(szTemp.GetBuffer(0), 80, 730 + nShiftY, &memdc);
		}

		if(m_pCharacter->m_nHeight)
		{
			int nFeet = m_pCharacter->m_nHeight / 12;
			int nInches = m_pCharacter->m_nHeight % 12;

			szTemp.Format("%d ft %d in", nFeet, nInches);
			DrawCSText(szTemp.GetBuffer(0), 80, 754 + nShiftY, &memdc);
		}

		szTemp.Format("%d", m_pCharacter->m_nAge);
		DrawCSText(szTemp.GetBuffer(0), 280, 688 + nShiftY, &memdc);

		szTemp.Format("%d", m_pCharacter->m_nAge + m_pCharacter->m_nAgeAdj);
		DrawCSText(szTemp.GetBuffer(0), 360, 683 + nShiftY, &memdc);

		szTemp.Format("%d years", m_pCharacter->m_nAgeAdj);
		DrawCSText(szTemp.GetBuffer(0), 445, 683 + nShiftY, &memdc);

		DrawCSText(m_pCharacter->m_szHairColor, 610, 686 + nShiftY, &memdc);
		DrawCSText(m_pCharacter->m_szEyeColor, 715, 686 + nShiftY, &memdc);


		CFont newfont6;
		nSize = 8;   // 7 point font
		//success = newfont4.CreatePointFont(nSize * 10, "SmallFonts", pDC);
		success = newfont6.CreatePointFont(nSize * 10, "HamletOrNot", pDC);
		memdc.SelectObject(&newfont6);

		//birthday
		if(m_pApp->m_pSelectedCalendar != NULL)
		{
			if(m_pCharacter->m_nDOBYear)
			{
				int nBirthMonth = abs(m_pCharacter->m_nDOBMonth-1) % m_pApp->m_pSelectedCalendar->m_nMonthsInYear;
				szTemp.Format("Born the %d%s day of %s in the year %d", m_pCharacter->m_nDOBDay, GetNumberSuffix(m_pCharacter->m_nDOBDay), m_pApp->m_pSelectedCalendar->m_szMonthNames[nBirthMonth], m_pCharacter->m_nDOBYear);

				DrawCSText(szTemp.GetBuffer(0), 116, 965 + nShiftY, &memdc);
			}
		}

		TRACE("OBJECT SIZE = %d %d\n", sizeof(cDNDObject), sizeof(unsigned long));

		//last will and testament
		DrawCSText(m_pCharacter->m_szCharacterName, 140, 1015 + nShiftY, &memdc);

		DrawCSText(m_pCharacter->m_szWill, 294, 1015 + nShiftY, &memdc);

	}
	else if(m_nPage == 1)
	{
		m_cPortraitButton.ShowWindow(SW_HIDE);

		cDNDSpellBook *pSpellBook = NULL;

		BOOL bClericDisplay = FALSE;

		int nMaxLevel = 9;

		if(m_pCharViewDialog->m_pSpellsDialog != NULL)
		{
			switch(m_pCharacter->m_SpellClasses[m_pCharViewDialog->m_nSelectedSpellViewClass])
			{
				case DND_CHARACTER_CLASS_CLERIC:
				case DND_CHARACTER_SPELL_CLASS_PALADIN_CLERIC:
				{
					pSpellBook = m_pApp->m_SpellBooks.GetAt(DND_CHARACTER_CLASS_CLERIC);
					bClericDisplay = TRUE;
					nMaxLevel = 7;
					break;
				}
				case DND_CHARACTER_CLASS_DRUID:
				case DND_CHARACTER_SPELL_CLASS_RANGER_DRUID:
				{
					pSpellBook = m_pApp->m_SpellBooks.GetAt(DND_CHARACTER_CLASS_DRUID);
					bClericDisplay = TRUE;
					nMaxLevel = 7;
					break;
				}
				case DND_CHARACTER_CLASS_MAGE:
				case DND_CHARACTER_SPELL_CLASS_RANGER_MAGE:
				{
					pSpellBook = m_pApp->m_SpellBooks.GetAt(DND_CHARACTER_CLASS_MAGE);
					break;
				}
				case DND_CHARACTER_CLASS_ILLUSIONIST:
				{
					pSpellBook = m_pApp->m_SpellBooks.GetAt(DND_CHARACTER_CLASS_ILLUSIONIST);
					break;
				}
			}
		}

		int nStartLevel = 1;
		if(g_bUseUnearthedArcana && m_pCharacter->m_SpellClasses[m_pCharViewDialog->m_nSelectedSpellViewClass] <= DND_CHARACTER_CLASS_ILLUSIONIST)
		{
			nStartLevel = 0;

			if(nMaxLevel == 9)
			{
				for(int xx = nMaxLevel; xx >= 1; --xx)
				{
					int nSpellsAllowedForLevel = GetSpellLevels(m_pCharacter, m_pCharacter->m_SpellClasses[m_pCharViewDialog->m_nSelectedSpellViewClass],  m_pCharacter->m_nCastingLevels[m_pCharViewDialog->m_nSelectedSpellViewClass], xx);

					if(nSpellsAllowedForLevel == 0)
					{
						--nMaxLevel;
					}
					else
					{
						break;
					}
				}

				if(nMaxLevel == 9)
				{
					nStartLevel = 1;
				}
			}
		}

		if(pSpellBook != NULL)
		{
			CFont verFont;
			verFont.CreateFont(20,10,2700,0,FALSE,FALSE,FALSE,0,ARABIC_CHARSET,OUT_TT_PRECIS,
					CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_ROMAN,_T("HamletOrNot"));

			if(bClericDisplay)
			{
				szTemp.Format("Prayerbook of %s", m_pCharacter->m_szCharacterName);
			}
			else
			{
				szTemp.Format("Spellbook of %s", m_pCharacter->m_szCharacterName);
				
			}

			CFont *OFont =(CFont *)memdc.SelectObject(&verFont);
			memdc.TextOut(780,100,szTemp.GetBuffer(0));


			int nSpellPos[10][2];
			int nSpellLines[10];

			memset(nSpellPos, 0, 10*2*sizeof(int));
			memset(nSpellLines, 0, 10*sizeof(int));

			int sX = 730;
			int sY = 100;
			int sizeX = 150;
			int j = 1;

			for(int i = nStartLevel; i <= nMaxLevel; ++i)
			{
				//TRACE("DRAW %d at %d\n", i, sX, sY);

				if(i == 0 && bClericDisplay)
				{
					szTemp = _T("Orisons");
				}
				else if(i == 0)
				{
					szTemp = _T("Cantrips");
				}
				else
				{
					szTemp.Format("Level: %d", i);
				}

				nSpellPos[i][0] = sX;
				nSpellPos[i][1] = sY;

				nSpellLines[i] = sX;

				memdc.TextOut(sX,sY,szTemp.GetBuffer(0));
				sX -= sizeX;

				if(sY == 100 && sX < 730 - 3*sizeX)
				{
					sY = 600;
					sX = 780;
				}
			}

			CFont verFont2;
			verFont2.CreateFont(10,5,2700,0,FALSE,FALSE,FALSE,0,ARABIC_CHARSET,OUT_TT_PRECIS,
					CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_ROMAN,_T("HamletOrNot"));

			CFont *OFont2 =(CFont *)memdc.SelectObject(&verFont2);
			
			for(int nLevel = nStartLevel; nLevel <= nMaxLevel; ++nLevel)
			{
				//m_nSpellBooks[4][MAX_SPELL_LEVELS][MAX_SPELLS_PER_LEVEL];
				for(int nSpell = 0; nSpell < MAX_SPELLS_PER_LEVEL; ++nSpell)
				{
					if(m_pCharacter->m_nSpellBooks[m_pCharViewDialog->m_nSelectedSpellViewClass][nLevel][nSpell] == 1 || bClericDisplay)// || nLevel > 0)
					{
						cDNDSpell *pSpell = &pSpellBook->m_Spells[nLevel][nSpell];

						if(pSpell != NULL)
						{
							memdc.TextOut(nSpellPos[nLevel][0]-20,nSpellPos[nLevel][1],pSpell->m_szSpellName);
							nSpellPos[nLevel][0] -= 12;

							if(nSpellPos[nLevel][0] < nSpellLines[nLevel] - 9*12)
							{
								nSpellPos[nLevel][0] = nSpellLines[nLevel];
								nSpellPos[nLevel][1] += sizeX;
							}
						}
					}
				}
			}

		} //if pSpellBook != NULL


	}
	else if(m_nPage == 2) // scrolls
	{
		m_cPortraitButton.ShowWindow(SW_HIDE);

		CFont verFont;
		verFont.CreateFont(20,10,2700,0,FALSE,FALSE,FALSE,0,ARABIC_CHARSET,OUT_TT_PRECIS,
				CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_ROMAN,_T("HamletOrNot"));
	
		szTemp.Format("Scrolls of %s", m_pCharacter->m_szCharacterName);

		CFont *OFont =(CFont *)memdc.SelectObject(&verFont);
		memdc.TextOut(820,60,szTemp.GetBuffer(0));

		CFont verFont2;
		verFont2.CreateFont(15,10, 2700,0,FALSE,FALSE,FALSE,0,ARABIC_CHARSET,OUT_TT_PRECIS,
				CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_ROMAN,_T("HamletOrNot"));

		CFont *OFont2 =(CFont *)memdc.SelectObject(&verFont2);

		//memdc.TextOut(750,60,szTemp.GetBuffer(0));

		int nX = 750;
		int nY = 60;

		for(int i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
		{
			if(m_pCharacter->m_Inventory[i].IsScroll())
			{
				for(int j = 0; j < MAX_OBJECT_CONTENTS; ++j)
				{
					if(m_pCharacter->m_Inventory[i].m_nContents[j] && m_pCharacter->m_Inventory[i].m_nContentsCount[j])
					{
						int nSpellIndex = m_pCharacter->m_Inventory[i].m_nContents[j] - 1;
						cDNDSpell *pSpell = m_pApp->m_MasterSpellArray.GetAt(nSpellIndex);

						if(pSpell != NULL)
						{
							if(m_pCharacter->m_Inventory[i].m_bMysteryItem)
							{
								szTemp = _T("Unidentified Spell");
							}
							else if(m_pCharacter->m_Inventory[i].m_nContentsCount[j] > 1)
							{
								szTemp.Format("%s x %d [L%d]", pSpell->m_szSpellName, m_pCharacter->m_Inventory[i].m_nContentsCount[j], pSpell->m_nSpellLevel);
							}
							else
							{
								szTemp.Format("%s [L%d]", pSpell->m_szSpellName, pSpell->m_nSpellLevel);
							}

							memdc.TextOut(nX, nY,szTemp.GetBuffer(0));

							nX -= 20;

							if(nX < 100)
							{
								nX = 750;
								nY += 400;
							}
						}
					}
				}
			}
		}

	} // end page 2, scrolls
	else if(m_nPage == 3) // notes
	{
		m_cPortraitButton.ShowWindow(SW_HIDE);

		szTemp.Format("Character Notes: %s", m_pCharacter->m_szCharacterName);

		DrawCSText(szTemp.GetBuffer(0), 70, 40, &memdc);

		CFont newfont3;
		nSize = 10;   // 10 point font
		//success = newfont3.CreatePointFont(nSize * 10, "HamletOrNot", pDC);
		success = newfont3.CreatePointFont(nSize * 10, "Cloister Black", pDC);
		memdc.SelectObject(&newfont3);

		szTemp.Format("%s", m_pCharacter->m_szDetails);

		int nLines = MakeParagraphText(&szTemp, 125);

		DrawLFText(szTemp.GetBuffer(0), 70, 100, &memdc);

		int nY = 100+nLines*20;
		
		nY += 20;
		//szTemp = _T("This is the next line *****");
		//DrawLFText(szTemp.GetBuffer(0), 70, nY, &memdc);
		
		BOOL bFirst = TRUE;
		for(int i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
		{
			POBJECTTYPE pObj = &m_pCharacter->m_Inventory[i];

			if(pObj != NULL && pObj->m_wTypeId)
			{
				if(pObj->m_nMagicAdj && pObj->m_nIntelligence && pObj->m_bMysteryItem == FALSE)
				{
					if(bFirst)
					{
						DrawLFText("Magic Swords:", 70, nY, &memdc); 
						nY+=30;
					}
					bFirst = FALSE;

					if(pObj->m_nMagicAdj > 0)
					{
						szTemp.Format("%s +%d", pObj->GetExtendedName(), pObj->m_nMagicAdj);
					}
					else
					{
						szTemp.Format("%s %d", pObj->GetExtendedName(), pObj->m_nMagicAdj);
					}

					CString szAlignment = _T("");
					szAlignment.Format(", alignment %s", GetAlignmentName(pObj->m_Alignment));
					szTemp += szAlignment;

					if(pObj->m_nContentsCount[0] & 0x0001)
					{
						szTemp += _T(", semi-empathic");
					}
					if(pObj->m_nContentsCount[0] & 0x0002)
					{
						szTemp += _T(", empathic");
					}
					if(pObj->m_nContentsCount[0] & 0x0004)
					{
						szTemp += _T(", can speak");
					}
					if(pObj->m_nContentsCount[0] & 0x0008)
					{
						szTemp += _T(", telepathic");
					}

					DrawLFText(szTemp.GetBuffer(0), 70, nY, &memdc); 
					nY+=20;

					szTemp = _T("");
					BOOL bFirstLanguage = TRUE;
					for (int i = 0; i < m_pApp->m_LanguageArray.GetSize(); ++i )
					{
						cDNDLanguage *pLanguage = (cDNDLanguage *)m_pApp->m_LanguageArray.GetAt(i);

						if(pLanguage != NULL)
						{
							if(pLanguage->m_nList)
							{
								if(pObj->CanSpeakLanguage(pLanguage->m_nIndex))
								{
									if(bFirstLanguage == TRUE)
									{
										szTemp += _T("Languages: ");
									}
									else
									{
										szTemp += _T(", ");
									}
									bFirstLanguage = FALSE;

									szTemp += pLanguage->m_szLanguageName;
								}
							}
						}
					}

					if(bFirstLanguage == FALSE)
					{
						DrawLFText(szTemp.GetBuffer(0), 70, nY, &memdc); 
						nY+=20;
					}
					
					for(int i = 0; i < 3; ++i)
					{
						if(strlen(pObj->m_szPrimaryAbility[i]) > 0)
						{
							szTemp.Format("Primary Ability: %s", pObj->m_szPrimaryAbility[i]);
							DrawLFText(szTemp.GetBuffer(0), 70, nY, &memdc); 
							nY+=20;
						}
					}
					if(strlen(pObj->m_szExtraordinaryPower) > 0)
					{
						szTemp.Format("Extraordinary Power: %s", pObj->m_szExtraordinaryPower);
						DrawLFText(szTemp.GetBuffer(0), 70, nY, &memdc); 
						nY+=20;
					}
					if(strlen(pObj->m_szSpecialPurpose) > 0)
					{
						szTemp.Format("Special Purpose: %s", pObj->m_szSpecialPurpose);
						DrawLFText(szTemp.GetBuffer(0), 70, nY, &memdc); 
						nY+=20;
					}
					if(strlen(pObj->m_szSpecialPurposePower) > 0)
					{
						szTemp.Format("Special Purpose Power: %s", pObj->m_szSpecialPurposePower);
						DrawLFText(szTemp.GetBuffer(0), 70, nY, &memdc); 
						nY+=20;
					}

					nY+=20;

				}
			}
		}

	} // end page 3, notes

	//offset here !
	pDC->BitBlt( 0, -m_nSliderOffset, nMaxSizeX+1, nMaxSizeY, &memdc, 0, 0, SRCCOPY );


	//pDC->SelectObject(pOldBr);
	pDC->SelectObject(oldfont); 
	pDC->SelectObject( poldbmp );
	//memdc.SelectObject( poldbmp );

	
	if(m_bPrintSheet)
	{
		m_bPrintSheet = FALSE;
		PrintBitmap(&memdc, &bmp, nPrintPages);		// shatner
	}

	if(m_bSaveSheet)
	{
		m_bSaveSheet = FALSE;

		Gdiplus::Bitmap bitmap(bmp, NULL);
		CLSID clsid;
		GetEncoderClsid(L"image/bmp", &clsid);

		LPWSTR wcsFile = m_szFileName.AllocSysString();
		bitmap.Save(wcsFile, &clsid);
	}

	/*
	CRect Rect;
	GetWindowRect (Rect);

	szTemp.Format("SIZE %d %d\n", Rect.bottom-Rect.top, Rect.right-Rect.left);

	AfxMessageBox(szTemp, MB_OK);
	*/
	
}

void DMCharSheetDialog::DrawTurnUndead(CDC *memdc, int nOffX, int nOffY, int nCellX)
{
	CString szTemp = _T("");
	int nTurnMatrix[21];

	if(GetTurnUndeadMatrix(m_pCharacter, nTurnMatrix))
	{
		for(int i = 0; i < 13; ++i)
		{
			switch(nTurnMatrix[i])
			{
				case 99:	szTemp = _T("-"); break;
				case -1:	szTemp = _T("turn"); break;
				case -2:	szTemp = _T("damn"); break;
				default:	szTemp.Format("%d", nTurnMatrix[i]); break;
			}

			if(i < 6)
				DrawCSText(szTemp.GetBuffer(0), 483+i*nCellX+nOffX, 970+nOffY, memdc);
			else
				DrawCSText(szTemp.GetBuffer(0), 426+(i-6)*nCellX+nOffX, 992+nOffY, memdc);
		}
	}
}

void DMCharSheetDialog::DrawSpellList(int nBitMapResource, CDC *memdc)
{
	if(m_pCharViewDialog->m_pSpellsDialog->m_pSpellBook == NULL)
		return;

	CString szTemp;

	int nSpellListX = 425;
	int nSpellListY = 855;
	int nMaxRows = 6;
	int nMaxColumns = 3;
	int nRowSize = 16;
	int nColumnSize = 105;

	int nSpellLevelX = 458;
	int nSpellLevelY = 830;
	int nSpellLevelColumnSize = 49;

	//cleric is default
	switch(nBitMapResource)
	{
		case IDB_MAGICUSER_BITMAP:
		{
			nSpellListX = 402;
			nSpellListY = 849;

			nMaxRows = 10;

			nRowSize = 16;
			nColumnSize = 105;

			nSpellLevelX = 487;
			nSpellLevelY = 822;
			nSpellLevelColumnSize = 32;
					
			break;
		}
		case IDB_MULTICLASS_BITMAP:
		{
			nSpellListX = 400;
			nSpellListY = 838;

			nMaxRows = 5;

			nRowSize = 16;
			nColumnSize = 105;

			nSpellLevelX = 454;
			nSpellLevelY = 812;
			nSpellLevelColumnSize = 32;
		
			break;
		}
		case IDB_FIGHTER_BITMAP:
		{
			nSpellListX = 412;
			nSpellListY = 896;

			nMaxRows = 5;

			nRowSize = 16;
			nColumnSize = 105;

			nSpellLevelX = 523;
			nSpellLevelY = 873;
			nSpellLevelColumnSize = 32;
		
			break;
		}
	}


	int nStartSpellListY = nSpellListY;
	int nRow = 0;
	int nColumn = 0;

	BOOL bComplete = FALSE;

	int nMaxSpellsPerLevel[4][10];
	memset(nMaxSpellsPerLevel, 0, 4*10*sizeof(int));

	for(int nSpellClass = 0; nSpellClass < 4 && !bComplete; ++nSpellClass)
	{
		if(m_pCharacter->m_SpellClasses[nSpellClass] == 0)
			continue;

		cDNDSpellBook *pSpellBook = NULL;
		switch(m_pCharacter->m_SpellClasses[nSpellClass])
		{
			case DND_CHARACTER_CLASS_CLERIC:
			case DND_CHARACTER_SPELL_CLASS_PALADIN_CLERIC:
			{		
				pSpellBook = m_pApp->m_SpellBooks.GetAt(DND_CHARACTER_CLASS_CLERIC);
				break;
			}
			case DND_CHARACTER_CLASS_DRUID:
			case DND_CHARACTER_SPELL_CLASS_RANGER_DRUID:
			{
				pSpellBook = m_pApp->m_SpellBooks.GetAt(DND_CHARACTER_CLASS_DRUID);
				break;
			}
			case DND_CHARACTER_CLASS_MAGE:
			case DND_CHARACTER_SPELL_CLASS_RANGER_MAGE:
			{
				pSpellBook = m_pApp->m_SpellBooks.GetAt(DND_CHARACTER_CLASS_MAGE);
				break;
			}
			case DND_CHARACTER_CLASS_ILLUSIONIST:
			{
				pSpellBook = m_pApp->m_SpellBooks.GetAt(DND_CHARACTER_CLASS_ILLUSIONIST);
				break;
			}
		} //end switch

		if(pSpellBook == NULL)
			continue;

		for(int nSpellLevel = 0; nSpellLevel < 10 && !bComplete; ++nSpellLevel)
		{
			BOOL bDone = FALSE;
			int nSpellCount = 0;
			
			while(!bDone)
			{
				cDNDSpell *pSpell = NULL;
				int nMultipleSpells = 0;
			
				do
				{
					nMultipleSpells = m_pCharacter->m_nSpellsMemorized[nSpellClass][nSpellLevel][nSpellCount];
					if(nMultipleSpells > 0)
					{
						pSpell = &pSpellBook->m_Spells[nSpellLevel][nSpellCount];
						if(pSpell && !pSpell->m_bSpellValid)
						{
							m_pCharacter->m_nSpellsMemorized[nSpellClass][nSpellLevel][nSpellCount] = 0;
							pSpell = NULL;
						}
					}

					++nSpellCount;

				} while(pSpell == NULL && nSpellCount < MAX_SPELLS_PER_LEVEL && bDone == FALSE && bComplete == FALSE);

				if(bDone == FALSE && pSpell != NULL && pSpell->m_bSpellValid && bComplete == FALSE)
				{
					if(nMultipleSpells > 1)
					{
						szTemp.Format("%d x %s", nMultipleSpells, pSpell->m_szSpellName);
					}
					else
					{
						szTemp.Format("%s", pSpell->m_szSpellName);
					}

					//draw szTemp
					DrawCSText(nSpellLevel, nSpellListX-17, nSpellListY-5, memdc);

					CString szLeft = szTemp.Left(16);
					DrawCSText(szLeft.GetBuffer(0), nSpellListX, nSpellListY, memdc);
					nSpellListY += nRowSize;

					++nRow;

					if(nRow > nMaxRows)
					{
						nRow = 0;

						nSpellListY = nStartSpellListY;
						nSpellListX += nColumnSize;

						++nColumn;

						if(nColumn > nMaxColumns)
						{
							bComplete = TRUE;
						}
					}

				}
				else
				{
					bDone = TRUE;
				}
			};

		} //for(int nSpellLevel = 0; nSpellLevel < 10; ++nSpellLevel)

	} // for(int nSpellClass = 0; nSpellClass < 10; ++nSpellClass)

	for(int nSpellLevel = 0; nSpellLevel < 10; ++nSpellLevel)
	{
		CString szMaxSpellsPerLevel = _T("");

		for(int nSpellClass = 0; nSpellClass <= 3; ++nSpellClass)
		{
			int nSpellsAllowed = GetSpellLevels(m_pCharacter, m_pCharacter->m_SpellClasses[nSpellClass],  m_pCharacter->m_nCastingLevels[nSpellClass], nSpellLevel);

			if(nSpellsAllowed)
			{
				if(szMaxSpellsPerLevel == _T(""))
				{
					szMaxSpellsPerLevel.Format("%d",nSpellsAllowed);
				}
				else
				{
					CString szSlasher;
					szSlasher.Format("/%d", nSpellsAllowed);
					szMaxSpellsPerLevel += szSlasher;
				}
			}
		}
		
		DrawCSText(szMaxSpellsPerLevel.GetBuffer(0), nSpellLevelX, nSpellLevelY, memdc);
		nSpellLevelX += nSpellLevelColumnSize;
	}
	
}

void DMCharSheetDialog::DrawCSText(char * szData, int x, int y, CDC *memdc)
{

	CRect rect;
	GetClientRect(&rect);
	
	rect.right = nMaxSizeX;
	rect.bottom = nMaxSizeY;

	rect.left = x;
	rect.top = y;

	memdc->DrawText(szData, &rect, DT_SINGLELINE | DT_LEFT );

}

void DMCharSheetDialog::DrawLFText(char * szData, int x, int y, CDC *memdc)
{

	CRect rect;
	GetClientRect(&rect);
	
	rect.right = nMaxSizeX;
	rect.bottom = nMaxSizeY;

	rect.left = x;
	rect.top = y;

	memdc->DrawText(szData, &rect, DT_LEFT );

}


void DMCharSheetDialog::DrawCSText(int val, int x, int y, CDC *memdc, BOOL bPlus)
{

	CRect rect;
	GetClientRect(&rect);
	
	rect.right = nMaxSizeX-1;
	rect.bottom = nMaxSizeY-1;

	rect.left = x;
	rect.top = y;

	CString szVal;

	if(val <= 0)
		bPlus = FALSE;

	if(val < 10)
	{
		if(bPlus)
			szVal.Format(" +%d", val);
		else
			szVal.Format(" %d", val);
	}
	else
	{
		if(bPlus)
			szVal.Format("+%d", val);
		else
			szVal.Format("%d", val);
	}

	memdc->DrawText(szVal, &rect, DT_SINGLELINE | DT_LEFT );

}

void DMCharSheetDialog::OnSaveButton() 
{
	ZeroMemory(&g_ofn, sizeof(OPENFILENAME));
	g_ofn.lStructSize = sizeof(OPENFILENAME);
	g_ofn.nMaxFile = MAX_PATH;
	g_ofn.nMaxFileTitle = MAX_PATH;
	g_ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	g_ofn.hwndOwner   = m_hWnd;
	g_ofn.hInstance   = m_pApp->m_hInstance;
    g_ofn.lpstrFile   = g_CS_szFilename;
	g_ofn.lpstrInitialDir = "C:/";
    g_ofn.lpstrTitle  = "Save Character Sheet BMP";
    g_ofn.lpstrFilter = "Character Sheet BMP Files (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0\0";
    g_ofn.lpstrDefExt = "bmp";

    // Get action template file name
    if(GetSaveFileName(&g_ofn))
	{
		m_szFileName = g_CS_szFilename;
		m_bSaveSheet = TRUE;
		InvalidateRect(NULL);
	}

}

void DMCharSheetDialog::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class

	m_pCharViewDialog->m_pCharSheetDialog = NULL;

	delete this;
	
	CDialog::PostNcDestroy();
}

void DMCharSheetDialog::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();

	if(m_pCharViewDialog != NULL)
	{
		m_pCharViewDialog->m_pCharSheetDialog = NULL;
	}

	delete this;
}

void DMCharSheetDialog::OnPortraitButton() 
{
	CString szPath;

	CString szDirectory;
	szDirectory.Format("%s//Data//characters//portraits", m_pApp->m_szEXEPath.GetBuffer(0));

	ZeroMemory(&g_ofn, sizeof(OPENFILENAME));
	g_ofn.lStructSize = sizeof(OPENFILENAME);
	g_ofn.nMaxFile = MAX_PATH;
	g_ofn.nMaxFileTitle = MAX_PATH;
	g_ofn.Flags = OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	g_ofn.hwndOwner   = m_hWnd;
	g_ofn.hInstance   = m_pApp->m_hInstance;
    g_ofn.lpstrFile   = g_CS_szFilename;
	g_ofn.lpstrInitialDir = szDirectory.GetBuffer(0);
    g_ofn.lpstrTitle  = "Load map bitmap file";
    //g_ofn.lpstrFilter = "Graphics Files (*.bmp)\0*.bmp\0(*.gif)\0*.gif\0(*.jpg)\0*.jpg\0All Files (*.*)\0*.*\0\0";
	g_ofn.lpstrFilter = "Graphics Files (*.bmp)\0*.bmp\0(*.gif)\0*.gif\0(*.jpg)\0*.jpg\0(*.png)\0*.png\0All Files (*.*)\0*.*\0\0";
    g_ofn.lpstrDefExt = "bmp";

    // Get action template file name
    if(GetOpenFileName(&g_ofn))
	{
		szPath = g_CS_szFilename;

		szPath.MakeUpper();
		szPath.Replace(m_pApp->m_szEXEPath, "<$DMAPATH>");

		strcpy(m_pCharacter->m_szPortraitPath, szPath.GetBuffer(0));

		if(m_pPortraitBitmap != NULL)
		{
			delete m_pPortraitBitmap;
			m_pPortraitBitmap = NULL;
		}

		InvalidateRect(NULL);

		m_pCharacter->MarkChanged();
		
	}
	
}


void DMCharSheetDialog::PrintBitmap(CDC *memDC, CBitmap *_bitmap, int nPrintPages)
{
	CString szFileName; // = "c:/dword.bmp";

	szFileName.Format("%sData/temp/%u.bmp", m_pApp->m_szEXEPath, GetUniqueID());

	DWORD dwFlags = PD_ALLPAGES | PD_USEDEVMODECOPIES | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE | PD_NOSELECTION;

	if (nPrintPages == 2)
	{
		dwFlags = PD_ALLPAGES | PD_PAGENUMS | PD_NOSELECTION | PD_HIDEPRINTTOFILE;
	}

	CPrintDialog printDlg(FALSE, dwFlags);

	if (nPrintPages == 2)
	{
		printDlg.m_pd.nMinPage = 1;
		printDlg.m_pd.nMaxPage = 2;
		printDlg.m_pd.nFromPage = 1;
		printDlg.m_pd.nToPage = 2;
	}
	else
	{
		printDlg.m_pd.nMinPage = 1;
		printDlg.m_pd.nMaxPage = 1;
		printDlg.m_pd.nFromPage = 1;
		printDlg.m_pd.nToPage = 1;
	}

	 //printDlg.GetDefaults(); 
	 // Or get from user:
	  if (printDlg.DoModal() == IDCANCEL)   
			 return; 
	 CDC dc;
	 if (!dc.Attach(printDlg.GetPrinterDC())) {
	  AfxMessageBox(_T("No printer found!")); return;
	 } 

	Gdiplus::Bitmap bitmap(*_bitmap, NULL);
	CLSID clsid;
	GetEncoderClsid(L"image/bmp", &clsid);

	LPWSTR wcsFile = szFileName.AllocSysString();
	bitmap.Save(wcsFile, &clsid);

 
	 dc.m_bPrinting = TRUE; 
	 DOCINFO di;    
	 // Initialise print document details
	 ::ZeroMemory (&di, sizeof (DOCINFO));
	 di.cbSize = sizeof (DOCINFO);
	 di.lpszDocName = szFileName; 
	 BOOL bPrintingOK = dc.StartDoc(&di); // Begin a new print job 
	 // Get the printing extents
	 // and store in the m_rectDraw field of a 
	 // CPrintInfo object
	 CPrintInfo Info;

	 //Info.SetMinPage(1);
	 Info.SetMaxPage(printDlg.m_pd.nToPage); // just one page 

	 int maxw = dc.GetDeviceCaps(HORZRES);
	 int maxh = dc.GetDeviceCaps(VERTRES); 
	 Info.m_rectDraw.SetRect(0, 0, maxw, maxh); 
	 for (UINT page = Info.GetMinPage(); page <= 
		  Info.GetMaxPage() && bPrintingOK; page++) {
	  dc.StartPage();    // begin new page
	  Info.m_nCurPage = page;
	  CBitmap bitmap;
	  // LoadImage does the trick here, it creates a DIB section
	  // You can also use a resource here
	  // by using MAKEINTRESOURCE() ... etc. 
	  if(!bitmap.Attach(::LoadImage(
	   ::GetModuleHandle(NULL), szFileName, IMAGE_BITMAP, 0, 0, 
	   LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE))) {
		AfxMessageBox(_T("Error loading bitmap!")); return;
	   } 
	   BITMAP bm;
	   bitmap.GetBitmap(&bm);
	   int w = bm.bmWidth; 
	   int h = bm.bmHeight; 
	   // create memory device context
	   CDC memDC; 
	   memDC.CreateCompatibleDC(&dc);
	   CBitmap *pBmp = memDC.SelectObject(&bitmap);
	   memDC.SetMapMode(dc.GetMapMode());
	   dc.SetStretchBltMode(HALFTONE);
	   // now stretchblt to maximum width on page

	   if (nPrintPages == 1)
	   {
		   dc.StretchBlt(0, 0, maxw, maxh, &memDC, 0, 0, w, h, SRCCOPY);
	   }
	   else
	   {
		   if (page == 1)
		   {
			   dc.StretchBlt(0, 0, maxw, maxh, &memDC, 0, 0, w, h/2, SRCCOPY);
		   }
		   else  if (page == 2)
		   {
			   dc.StretchBlt(0, 0, maxw, maxh, &memDC, 0, h/2, w, h / 2, SRCCOPY);
		   }
	   }


	   // clean up
	   memDC.SelectObject(pBmp);
	   bPrintingOK = (dc.EndPage() > 0);   // end page
	 } 
	 if (bPrintingOK)
	   dc.EndDoc(); // end a print job
	 else dc.AbortDoc();           // abort job. 

	DeleteFile(szFileName); 

}


#if 0
void DMCharSheetDialog::PrintBitmap(CDC *memDC, CBitmap *bitmap)
{
	int bmpWidth = 1600;
	int bmpHeight = 500;

	CBitmap * pOldBitmap = (CBitmap *) memDC->SelectObject( bitmap );
	if (pOldBitmap == NULL) // if bitmap is very big, better check this !
	{
		memDC->DeleteDC();
		AfxMessageBox("Not enough resource for the bitmap. Either reduce the bitmap dimension or switch to lower screen setting (e.g. 256-color mode), and try again.");
		return;
	}
	
	CDC prtDC;
	CPrintInfo printInfo;
	CSize size;
	DOCINFO di;
	CString szPortName, szAppName, szPrintError;
	szAppName.LoadString(AFX_IDS_APP_TITLE);
	szPrintError = "";

	CSize paper_size;  //printer paper size in mm
	int xLogPPI = 0;
	int yLogPPI = 0;

	if( AfxGetApp()->GetPrinterDeviceDefaults(&printInfo.m_pPD->m_pd) )
	{
		HDC hDC = printInfo.m_pPD->m_pd.hDC;
		if (hDC == NULL)
			hDC = printInfo.m_pPD->CreatePrinterDC();
		if(hDC !=NULL)
		{
			prtDC.Attach(hDC);
			paper_size.cx = prtDC.GetDeviceCaps(HORZSIZE);
			paper_size.cy = prtDC.GetDeviceCaps(VERTSIZE);
			xLogPPI = prtDC.GetDeviceCaps(LOGPIXELSX);
			yLogPPI = prtDC.GetDeviceCaps(LOGPIXELSY);
		}
		else 
		{
			AfxMessageBox("Can not find printer. Please check installed/default printers.");
			return;
		}
	}
	int scr_xLogPPI = memDC->GetDeviceCaps(LOGPIXELSX);
	int scr_yLogPPI = memDC->GetDeviceCaps(LOGPIXELSY);
	int paper_width = (int) ((double) paper_size.cx * (double) xLogPPI / 25.4);   //width of a printed page in pixels
	int paper_height = (int) ((double) paper_size.cy * (double) yLogPPI / 25.4);
	double ratio_x = (double) xLogPPI / (double) scr_xLogPPI;
	double ratio_y = (double) yLogPPI / (double) scr_yLogPPI;

	CString strPageNumber = "";
				
	int page_info_left = (int) ( (double) paper_width * 0.9 );
	int page_info_right = paper_width;
	int page_info_top = (int) ( (double) paper_height * 0.99);
	int page_info_bottom = paper_height;
	CRect page_info_rect = CRect(page_info_left, page_info_top, 
 	                             page_info_right,page_info_bottom );
	int printed_pages = 0;
	int total_print_pages = 0;
	BOOL bAbort_print = FALSE;

	// calculate pages
    int total_pages = (int)(bmpWidth * ratio_x + paper_width - 1 ) / paper_width;
	//pop up printer dialog
	CPrintDialog prtDlg(FALSE, PD_PAGENUMS);
	
	prtDlg.m_pd.nMinPage = 1;
	prtDlg.m_pd.nMaxPage = total_pages;
	prtDlg.m_pd.nFromPage = 1;
	prtDlg.m_pd.nToPage = total_pages;
		
	if(prtDlg.DoModal() == IDOK )
	{
		memset(&di, 0, sizeof(DOCINFO));
		di.cbSize = sizeof(DOCINFO);
		di.lpszDocName = szAppName;
		szPortName = prtDlg.GetPortName();
		di.lpszOutput = szPortName;
		prtDC.m_bPrinting = TRUE;
	}
	else
		return;  //Cancel button pressed, don't forget this!

	if(total_pages != 1 && AfxMessageBox("WARNING:  Cannot print to a single page !", MB_OKCANCEL) == IDCANCEL)
	{
		prtDC.Detach();
		prtDC.DeleteDC();
		return;
	}

	if(prtDC.StartDoc(&di) == -1)
	{
		AfxMessageBox("Printing error occured. Unable to find printer.");
		prtDC.Detach();
		prtDC.DeleteDC();
		return;
	}
				
	prtDC.SetMapMode(MM_TEXT);
		
	int i = 0;
	for(i = 0; i < total_pages; i++)
	{
		prtDC.StartPage();
		strPageNumber.Format("Page:%d of %d", ++printed_pages, total_print_pages );
		
		if ( i == (total_pages - 1) && total_pages > 1 ) //last page
		{
			int last_bmpWidth = (int)(bmpWidth - paper_width / ratio_x) * i;
			prtDC.StretchBlt(0, 0, (int)(last_bmpWidth * ratio_x), (int)(bmpHeight* ratio_y), memDC,
					    (int)(paper_width * i / ratio_x), 0, last_bmpWidth, bmpHeight, SRCCOPY);
		}
		else
			prtDC.StretchBlt(0, 0, paper_width, (int)(bmpHeight* ratio_y), memDC,
					    (int)(paper_width * i / ratio_x), 0, (int)(paper_width / ratio_x) , bmpHeight, SRCCOPY);
		prtDC.TextOut(page_info_rect.left, page_info_rect.top, strPageNumber );
		
		prtDC.EndPage();
	}
	memDC->SelectObject(pOldBitmap);
	//delete bitmap;
	memDC->DeleteDC();
		
	prtDC.EndDoc();
	prtDC.Detach();
	prtDC.DeleteDC();
}
#endif


void DMCharSheetDialog::OnPrintButton() 
{
	// TODO: Add your control notification handler code here

	m_bPrintSheet = TRUE;

	InvalidateRect(NULL);
	
}

void DMCharSheetDialog::OnFlipPageButton() 
{
	// TODO: Add your control notification handler code here

	m_nPage = m_nPage + 1;
	if(m_nPage > MAX_CS_PAGES)
		m_nPage = 0;

	InvalidateRect(NULL);
	
}

void DMCharSheetDialog::OnPageBackButton() 
{
	// TODO: Add your control notification handler code here
	
	m_nPage = m_nPage - 1;
	if(m_nPage < 0)
		m_nPage = MAX_CS_PAGES;

	InvalidateRect(NULL);
}


void DMCharSheetDialog::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	UpdateData(TRUE);

	TRACE("NPOS = %d %d\n", nPos, m_nSliderOffset);

	InvalidateRect(NULL);

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}



BOOL DMCharSheetDialog::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	m_nSliderOffset -= zDelta;

	if (m_nSliderOffset < 0)
		m_nSliderOffset = 0;

	if (m_nSliderOffset > 1440)
		m_nSliderOffset = 1440;

	InvalidateRect(NULL);

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


void DMCharSheetDialog::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (point.x >= 514 && point.x <= 700 && point.y >= 170 && point.y <= 340)
	{
		if (NULL != m_pPortraitBitmap)
		{
			cDMCharacterPortraitDialog *pDlg = new cDMCharacterPortraitDialog(m_pPortraitBitmap);
			pDlg->DoModal();
			delete pDlg;
		}
	}

	CDialog::OnLButtonDblClk(nFlags, point);
}
