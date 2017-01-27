// DM Helper.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <mmsystem.h>

#include "DM Helper.h"
#include "DM HelperDlg.h"
#include "DMInventoryDialog.h"
#include "DMCharSpellsDialog.h"
#include "DMCharViewDialog.h"
#include "DMNPCViewDialog.h"
#include "DMPartyDialog.h"
#include "DMSaveConfirmDialog.h"
#include "DMFileManagementDialog.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int GetSelectedListCtrlItem(CListCtrl *plctrl)
{
	POSITION pos = plctrl->GetFirstSelectedItemPosition();

	int selected=-1;

	if(pos != NULL)
	{
		while (pos)
		{
			int nItem = plctrl->GetNextSelectedItem(pos);
			selected = nItem;
		}
	}

 return selected;
}

int GetLastSelectedListCtrlItem(CListCtrl *plctrl, int nCurrent, int *nOther)
{
	POSITION pos = plctrl->GetFirstSelectedItemPosition();

	int selected = -1;

	int nLoops = 0;
	*nOther = nCurrent;

	int nSelections[2] = { -1, -1 };
	int nIndex = 0;

	if (pos != NULL)
	{
		while (pos)
		{
			int nItem = plctrl->GetNextSelectedItem(pos);
			
			nSelections[nIndex] = nItem;
			nIndex = 1;

			++nLoops;
		}
	}

	if (nSelections[0] == nCurrent || nSelections[1] == -1)
	{
		selected = nSelections[0];
		*nOther = nSelections[1];
	}
	else
	{
		selected = nSelections[1];
		*nOther = nSelections[0];
	}

	return selected;
}

void SetSelectedListCtrlItem(CListCtrl *plctrl, int nPos)
{
	for(int i = 0; i < plctrl->GetItemCount(); ++i)
	{
		if(i == nPos)
		{
			plctrl->SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);

			plctrl->EnsureVisible(i, FALSE);
		}
		else
		{
			plctrl->SetItemState(i, 0, LVIS_FOCUSED | LVIS_SELECTED);
		}
	}
}

void ClearSelectedListCtrlItems(CListCtrl *plctrl)
{
	POSITION pos = plctrl->GetFirstSelectedItemPosition();

	if(pos != NULL)
	{
		while (pos)
		{
			int nItem = plctrl->GetNextSelectedItem(pos);
			plctrl->SetItemState(nItem, 0, LVIS_FOCUSED | LVIS_SELECTED);	
			plctrl->Update(nItem);
		}
	}
}

void SetSelectedListCtrlItems(CListCtrl *plctrl, int *nArray)
{
	for(int i = 0; i < plctrl->GetItemCount(); ++i)
	{
		if(nArray[i])
		{
			plctrl->SetItemState(i, LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
		}
		else
		{
			plctrl->SetItemState(i, 0, LVIS_FOCUSED | LVIS_SELECTED);
		}
	}
}

void ClearSelectedListBoxItems(CListBox *plctrl)
{
	for(int i = 0; i < plctrl->GetCount(); ++i)
	{
		plctrl->SetSel(i, FALSE);
	}
}

void SetSelectedListBoxItems(CListBox *plctrl, int *nArray)
{
	for(int i = 0; i < plctrl->GetCount(); ++i)
	{
		if(nArray[i])
			plctrl->SetSel(i, TRUE);
		else
			plctrl->SetSel(i, FALSE);
	}
}

int GetSelectedListBoxItemsCount(CListBox *plctrl)
{
	int nCount = 0;
	for(int i = 0; i < plctrl->GetCount(); ++i)
	{
		
		if(plctrl->GetSel(i))
			++nCount;
	}

	return nCount;
}

void CleanMemorizedSpellListBox(CListBox *plctrl)
{
	for(int i = 0; i < plctrl->GetCount(); ++i)
	{
		PSPELLSLOT pSpellSlot = (PSPELLSLOT)plctrl->GetItemData(i);
		if(pSpellSlot != NULL)
		{
			delete pSpellSlot;
		}
	}
	plctrl->ResetContent();
}

/////////////////////////////////////////////////////////////////////////////
// CDMHelperApp

BEGIN_MESSAGE_MAP(CDMHelperApp, CWinApp)
	//{{AFX_MSG_MAP(CDMHelperApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDMHelperApp construction

CDMHelperApp::CDMHelperApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDMHelperApp object

DWORD GetUniversalTime(void)
{
	// this function returns the current seconds since midnight (00:00:00), January 1, 1970
	//do not mess with this function unless you are SURE what you are doing

	static DWORD dwMinClock = 0L;

	time_t ltime;

	time( &ltime );

	if((DWORD)ltime < dwMinClock)
		return dwMinClock;

	dwMinClock = (DWORD)ltime;

	return (DWORD) ltime;

} // end GetUniversalTime


DWORD dwLastUniqueID = 0L;

DWORD GetUniqueID(void)
{
	DWORD dwRetVal = GetUniversalTime();

	if(dwRetVal <= dwLastUniqueID)
	{
		dwRetVal = dwLastUniqueID + 1;
	}

	dwLastUniqueID = dwRetVal;

	return dwRetVal;

}

void TimeSpan(char *szLabel, DWORD *dwStartTime)
{
	if(*dwStartTime == 0)
	{
		*dwStartTime = GetTickCount();
	}

	TRACE("%s = %ld (%ld)\n", szLabel, GetTickCount() - *dwStartTime, GetTickCount());
}

void WaitExecute(BOOL *bWaitFlag)
{
	MSG msg;

	while (*bWaitFlag == FALSE) 
	{ 
		 while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE)) 
		   if(!AfxGetThread()->PumpMessage()) 
				::PostQuitMessage (0); 
		::Sleep(100); 

	} 
}

void WaitTime(DWORD dwMilliSeconds)
{
	DWORD dwStartTime = GetTickCount();

	MSG msg;

	while (GetTickCount() - dwStartTime < dwMilliSeconds) 
	{ 
		 while (::PeekMessage(&msg, NULL, NULL, NULL, PM_NOREMOVE)) 
		   if(!AfxGetThread()->PumpMessage()) 
				::PostQuitMessage (0); 
		::Sleep(1); 

	} 
}

int MakeParagraphText(CString *pText, int nLineLength) // add line feeds to a line of text to make a paragraph out of it
{
	int nLines = 1;

	CString szOriginalText = *pText;
	CString szNewText = _T("");
	CString szLineFeed = _T("\n");
	//CString szPercent = _T("0/0");

	int nCurrentLineChar = 0;
	for(int i = 0; i < szOriginalText.GetLength(); ++i)
	{
		CString szCurrent = szOriginalText.GetAt(i);

		if(szCurrent == _T(" "))
		{
			if(nCurrentLineChar >= nLineLength)
			{
				szNewText += szLineFeed;
				nCurrentLineChar = 0;

				++nLines;
			}
			else
			{
				szNewText += szCurrent;
				++nCurrentLineChar;
			}
		}
		else if(szCurrent == _T("%"))
		{
			szNewText += szCurrent;
			nCurrentLineChar += 1;
		}
		else
		{
			if(szCurrent == _T("\n"))
			{
				++nLines;
			}
			if(szCurrent == _T("\r"))
			{
				//++nLines;
			}

			szNewText += szCurrent;
			++nCurrentLineChar;
		}
	}

	pText->Format("%s", szNewText);

	return nLines;
}

CDMHelperApp theApp;


//OPENFILENAME    g_ofn;
//char g_szFilename[MAX_PATH];


/////////////////////////////////////////////////////////////////////////////
// CDMHelperApp initialization

#include <afxpriv.h>
#if _MSC_VER < 1300
#include <..\src\occimpl.h> 
#else
#include "afxocc.h"
#endif

class CFontOccManager : public COccManager
{
public:
    CFontOccManager() { }

 virtual const DLGTEMPLATE* PreCreateDialog(_AFX_OCC_DIALOG_INFO* pOccDialogInfo,
  const DLGTEMPLATE* pOrigTemplate);
};


void GetDisplayFont(LOGFONT &lf, WORD & wDefSize)
{
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);
	lf = ncm.lfMessageFont;

	HDC hDC = ::GetDC(NULL);
	if (lf.lfHeight < 0)
	lf.lfHeight = -lf.lfHeight;
	wDefSize = (WORD)MulDiv(lf.lfHeight, 72, GetDeviceCaps(hDC, LOGPIXELSY));
	::ReleaseDC(NULL, hDC);
}

const DLGTEMPLATE* CFontOccManager::PreCreateDialog(_AFX_OCC_DIALOG_INFO* pOccDialogInfo,
 const DLGTEMPLATE* pOrigTemplate)
{
	const DLGTEMPLATE *lpNewTemplate = COccManager::PreCreateDialog (pOccDialogInfo, pOrigTemplate);

	if ((BYTE)GetVersion() >= 6 && !pOccDialogInfo->m_pNewTemplate) 
	{
		CDialogTemplate temp(lpNewTemplate);

		#if 0
			LOGFONT lf;
			WORD wDefSize = 0;
			GetDisplayFont( lf, wDefSize );
			temp.SetFont(lf.lfFaceName, wDefSize);
		#else

		CDMHelperApp *pApp = (CDMHelperApp*)AfxGetApp();

		if(pApp->m_Settings.m_bUseSmallFont)
		{
			int nFontSize = 8;

			CFont ArielFont;
			ArielFont.CreatePointFont(nFontSize*10,"Arial Black");
			LOGFONT lf; 
			ArielFont.GetLogFont(&lf);
			lf.lfWeight = FW_BOLD;
			temp.SetFont(lf.lfFaceName, nFontSize);
		}

		#endif

		pOccDialogInfo->m_pNewTemplate = (DLGTEMPLATE*) temp.Detach();

		return pOccDialogInfo->m_pNewTemplate;
	}
	else
		return lpNewTemplate;
}

int FindIndex(int *nArray, int nFindMe, int nStart, int nEnd, int *nLoops)
{
	int nIndex = -1;

	*nLoops += 1;

	if (nStart > nEnd)
	{
		return -1;
	}
	else
	{
		int nMiddle = (nStart + nEnd) / 2;

		//TRACE("SEARCH %d (%d)\n", nMiddle, nArray[nMiddle]);

		if (nFindMe == nArray[nMiddle])
		{
			return nMiddle;
		}
		else
		{
			if (nFindMe < nArray[nMiddle])
			{
				nIndex = FindIndex(nArray, nFindMe, nStart, nMiddle-1, nLoops);
			}
			
			if(nIndex == -1) //inital branching guess was wrong, so try again
			{
				nIndex = FindIndex(nArray, nFindMe, nMiddle+1, nEnd, nLoops);
			}

			if(nIndex == -1) //that guess was was wrong too, so try try again
			{
				nIndex = FindIndex(nArray, nFindMe, nStart, nMiddle-1, nLoops);
			}
		}
	} 

	return nIndex;
}

BOOL CDMHelperApp::InitInstance()
{
	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

#if 0
	AfxEnableControlContainer();
#else
	AfxEnableControlContainer(new CFontOccManager());
#endif
	
	srand( (unsigned)time( NULL ) );

	m_dwInventoryFlag = 0;

	m_nTotalMonsterCount = 0;

	g_bUseSoundEffects = TRUE;

#if 0
	//int nArray[8] = {4, 5, 6, 7, 8, 1, 2, 3};
	int nArray[8] = {3, 4, 5, 6, 7, 8, 1, 2};

	for(int nFindMe = 0; nFindMe <= 9; ++nFindMe)
	{
		int nLoops = 0;
		int nFoundIndex = FindIndex(nArray, nFindMe, 0, 7, &nLoops);

		if(nFoundIndex == -1)
		{
			TRACE("Didn't Find %d after %d loops\n", nFindMe, nLoops);
		}
		else
		{
			TRACE("Found %d at %d after %d loops\n", nFindMe, nFoundIndex, nLoops);
		}
	}

	TRACE("OK\n");
#endif

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	TRACE("MAX_PATH = %d\n", MAX_PATH);
	TRACE("sizes %d %d %d ( %d )\n", sizeof(int), sizeof(char), sizeof(DND_CHARACTER_ALIGNMENTS), sizeof(cDNDObject));

	ASSERT(sizeof(cDNDObject) == 4164); // pass this assertion or you are screwed !
	/*
	//64 99 C9 60 09 
	char szWhut[10];
	szWhut[0] = 0x64;
	szWhut[1] = 0x99;
	szWhut[2] = 0xC9;
	szWhut[3] = 0x9;
	szWhut[4] = 0;
	szWhut[5] = 0;
	*/

	TCHAR buff[MAX_PATH];
    memset(buff, 0, MAX_PATH);
    ::GetModuleFileName(NULL,buff,sizeof(buff));    
	m_szEXEPath = buff;
    m_szEXEPath = m_szEXEPath.Left(m_szEXEPath.ReverseFind(_T('\\'))+1);    

#ifdef _DEBUG
	m_szEXEPath.Replace("Debug", "Release");
#endif

	m_szEXEPath.MakeUpper();

	CString szTemp;
	CString szTemp2;

	szTemp = m_szEXEPath + "data\\monster_pictures";
	CreateDirectory(szTemp, NULL);

	szTemp += "\\icons";
	CreateDirectory(szTemp, NULL);

	szTemp = m_szEXEPath + "data\\characters\\icons";
	CreateDirectory(szTemp, NULL);

	szTemp.Format("%sdata\\fonts\\CloisterBlack.ttf", m_szEXEPath);
	AddFontResource(szTemp); 

	szTemp.Format("%sdata\\fonts\\HamletOrNot.ttf", m_szEXEPath);
	AddFontResource(szTemp); 

	TRACE("SIZEOF CHARACTER = %d = %d\n", sizeof(cDNDNonPlayerCharacter), sizeof(cDNDNonPlayerCharacter) * 128);

	TRACE("SIZEOF MAP CELL = %d\n", sizeof(cDNDMapCell));

	LoadSettings();


#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL);

	m_bInitialResize = FALSE;

	m_pSelectedCalendar = NULL;

	m_nDefinedObjectCount = 0;

	m_nPummelWeaponIndex = 0;
	m_nMonkWeaponIndex = 0;

	m_nLastLanguageIndex = 0;


	szTemp.Format("%sDMUpdater.dat", m_szEXEPath.GetBuffer(0));
	LoadUpdateParams(szTemp.GetBuffer(0));

	szTemp.Format("%sData\\tables\\calendar_data.dat", m_szEXEPath.GetBuffer(0));
	LoadCalendars(szTemp.GetBuffer(0));

	InitializeSpells();

	InitializeObjectTables();

	szTemp.Format("%sData\\tables\\MagicItemDescriptions.dat", m_szEXEPath.GetBuffer(0));
	LoadMagicItemDescriptions(szTemp.GetBuffer(0));

	szTemp.Format("%sData/tables/languages.dat", m_szEXEPath.GetBuffer(0));
	LoadLanguages(szTemp.GetBuffer(0));

	//szTemp.Format("%s/Data/tables/BaseMonsterManual.dat", m_szEXEPath.GetBuffer(0));
	//szTemp2.Format("%s/Data/tables/MonsterManual.dat", m_szEXEPath.GetBuffer(0));
	//PrescanMonsterManual(szTemp.GetBuffer(0), szTemp2.GetBuffer(0));

	InitializeMonsters();

	szTemp.Format("%sData/tables/RandomEncounterIndex.dat", m_szEXEPath.GetBuffer(0));
	LoadRandomEncounterIndexTable(szTemp.GetBuffer(0));

	szTemp.Format("%sData/tables/RandomEncounterTables.dat", m_szEXEPath.GetBuffer(0));
	LoadRandomEncounterTables(szTemp.GetBuffer(0));

	szTemp.Format("%sData/tables/names/random_names.dat", m_szEXEPath.GetBuffer(0));
	LoadRandomNames(szTemp.GetBuffer(0));

	//CString szTestName = GetRandomName(DND_CHARACTER_RACE_HUMAN, 1);
	//szTestName = GetRandomName((DND_CHARACTER_RACES)(DND_CHARACTER_RACE_HUMAN+0), 1);

	m_bSaveAllParties = FALSE;
	m_bSaveAllMaps = FALSE;
	m_bSaveAllCharacters = FALSE;
	m_bRollPCsInitiative = TRUE;

	m_pBaseCopyCharacterDialog = NULL;
	m_bCharacterCopyBufferIsNPC = FALSE;

	m_pDungeonTilesBitmap = NULL;
	m_pIsometricDungeonTilesBitmap = NULL;

	m_nCalendarSecond = 0;

	CDMHelperDlg dlg;
	m_pMainWnd = &dlg;

	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CDMHelperApp::ExitInstance() 
{
	POSITION pos;
	WORD wID;

	for (int i = 0; i < m_CalendarArray.GetSize(); ++i )
	{
		cDNDCalendar *pCalendar = (cDNDCalendar *)m_CalendarArray.GetAt(i);

		if(pCalendar != NULL)
		{
			delete pCalendar;
		}
	}

	for (pos = m_SpellTypeMap.GetStartPosition(); pos != NULL; )
	{
		CString *pString = NULL;
		m_SpellTypeMap.GetNextAssoc(pos,wID,pString);

		if(pString != NULL)
		{
			delete pString;
		}
	}
	m_SpellTypeMap.RemoveAll();

	for (i = 0; i < m_MasterSpellArray.GetSize(); ++i )
	{
		PSPELL pSpell = (PSPELL)m_MasterSpellArray.GetAt(i);

		if(pSpell != NULL)
		{
			delete pSpell;
		}
	}

	for (i = 0; i < m_SpellBooks.GetSize(); ++i )
	{
		PSPELLBOOK pSpellBook = (PSPELLBOOK)m_SpellBooks.GetAt(i);

		if(pSpellBook != NULL)
		{
			delete pSpellBook;
		}
	}

	CleanUpObjectTables();

	for (pos = m_MagicItemDescriptionMap.GetStartPosition(); pos != NULL;)
	{
		CString szJunk; 

		PMAGICITEMDESCRIPTION pMagicItemDescription = NULL;
		m_MagicItemDescriptionMap.GetNextAssoc(pos, szJunk, pMagicItemDescription);

		if (pMagicItemDescription != NULL)
		{
			delete pMagicItemDescription;
		}
	}
	m_MagicItemDescriptionMap.RemoveAll();

	for (int i = 0; i < m_LanguageArray.GetSize(); ++i )
	{
		cDNDLanguage *pLanguage = (cDNDLanguage *)m_LanguageArray.GetAt(i);

		if(pLanguage != NULL)
		{
			delete pLanguage;
		}
	}

	for (pos = m_MonsterDescMap.GetStartPosition(); pos != NULL; )
	{
		CString *pString = NULL;
		m_MonsterDescMap.GetNextAssoc(pos,wID,pString);

		if(pString != NULL)
		{
			delete pString;
		}
	}
	m_MonsterDescMap.RemoveAll();

	for (pos = m_MonsterIntMap.GetStartPosition(); pos != NULL; )
	{
		CString *pString = NULL;
		m_MonsterIntMap.GetNextAssoc(pos,wID,pString);

		if(pString != NULL)
		{
			delete pString;
		}
	}
	m_MonsterIntMap.RemoveAll();

	for (pos = m_MagicResistanceMap.GetStartPosition(); pos != NULL; )
	{
		CString *pString = NULL;
		m_MagicResistanceMap.GetNextAssoc(pos,wID,pString);

		if(pString != NULL)
		{
			delete pString;
		}
	}
	m_MagicResistanceMap.RemoveAll();
	

	for (pos = m_MonsterManualIndexedMap.GetStartPosition(); pos != NULL; )
	{
		PDNDMONSTERMANUALENTRY pMonster = NULL;
		m_MonsterManualIndexedMap.GetNextAssoc(pos,wID,pMonster);

		if(pMonster != NULL)
		{
			delete pMonster;
		}
	}

	for (pos = m_CustomMonsterManualIndexedMap.GetStartPosition(); pos != NULL; )
	{
		PDNDMONSTERMANUALENTRY pMonster = NULL;
		m_CustomMonsterManualIndexedMap.GetNextAssoc(pos,wID,pMonster);

		if(pMonster != NULL)
		{
			delete pMonster;
		}
	}
	
	for (pos = m_RandomNameMap.GetStartPosition(); pos != NULL; )
	{
		PDNDRANDOMNAMETABLE pNameTable = NULL;
		m_RandomNameMap.GetNextAssoc(pos,wID,pNameTable);

		if(pNameTable != NULL)
		{
			delete pNameTable;
		}
	}


	for(int i = 0; i < 16; ++ i)
	{
		for(int j = 0; j < 20; ++j)
		{
			delete m_pRandomEncounterIndex[i][j];
		}
	}

	cDNDRandomEncounterTable *pTable = NULL;
	cDNDRandomEncounterTableEntry *pEntry = NULL;

	for(int i = 0; i < m_RandomEncounterTables.GetCount(); ++i)
	{
		cDNDRandomEncounterTable *pTable = m_RandomEncounterTables.GetAt(i);

		delete pTable;
	}

	if(m_pDungeonTilesBitmap != NULL)
	{
		delete m_pDungeonTilesBitmap;
		m_pDungeonTilesBitmap = NULL;
	}
	if (m_pIsometricDungeonTilesBitmap != NULL)
	{
		delete m_pIsometricDungeonTilesBitmap;
		m_pIsometricDungeonTilesBitmap = NULL;
	}


	SaveSettings();

	Gdiplus::GdiplusShutdown(m_gdiplusToken);

	CString szSourceFile;
	CString szDestFile;

	szSourceFile.Format("%s_DM Update.exe", m_szEXEPath.GetBuffer(0));
	szDestFile.Format("%sDM Update.exe", m_szEXEPath.GetBuffer(0));

	CopyFile( (LPCTSTR) szSourceFile, (LPCTSTR) szDestFile, FALSE);
	
	return CWinApp::ExitInstance();
}

BOOL CDMHelperApp::LoadUpdateParams(char *path)
{
	FILE *pInfile = fopen(path, "rt");

	char szInLine[256];
	int nLines = 0;

	if(pInfile != NULL)
	{
		while (!feof(pInfile))
		{
			fgets(szInLine, 255, pInfile);
			CString szTemp = szInLine;
			
			if(szTemp.FindOneOf("#") >= 0)
				continue;

			switch(nLines)
			{
				case 0:	m_UpdateParams.m_szUpdateURL = szTemp; break;
				case 1:	m_UpdateParams.m_nUpdateRule = atoi(szTemp.GetBuffer(0)); break;
			}

			++nLines;
		}

		fclose(pInfile);

		m_UpdateParams.m_szUpdateURL.Replace("\r","");
		m_UpdateParams.m_szUpdateURL.Replace("\n","");

		return TRUE;
	}
	else
	{
		AfxMessageBox("CRITICAL ERROR: DMUpdater.dat NOT FOUND !", MB_OK);
		return FALSE;
	}

	AfxMessageBox("CRITICAL ERROR: Unable to load DMUpdater.dat !", MB_OK);

	return FALSE;
}

BOOL CDMHelperApp::SaveUpdateParams()
{
	CString szPath;
	szPath.Format("%sDMUpdater.dat", m_szEXEPath.GetBuffer(0));

	FILE *pOutfile = fopen(szPath.GetBuffer(0), "wt");

	/*
	# location of Dungeo Maestro update repository
	http://50.116.10.164/dnd/updates
	# flag for update behavior: 0-manual updates only 1-check update everytime program is started
	1 
	*/

	if(pOutfile != NULL)
	{
		fprintf(pOutfile, "# location of Dungeo Maestro update repository\n");
		fprintf(pOutfile, "%s\n", m_UpdateParams.m_szUpdateURL.GetBuffer(0));
		fprintf(pOutfile, "# flag for update behavior: 0-manual updates only 1-check update everytime program is started\n");
		fprintf(pOutfile, "%d\n", m_UpdateParams.m_nUpdateRule);

		fclose(pOutfile);
	}


	return FALSE;
}

int CDMHelperApp::GetNextCustomObjectIndex()
{
	int nRetVal = 11000;
	int nLastVal = nRetVal-1;

	for(int k = 0; k < m_CustomObjectsOrderedTypeArray.GetSize(); ++k)
	{
		POBJECTTYPE pObjBase = m_CustomObjectsOrderedTypeArray.GetAt(k);

		if(pObjBase != NULL)
		{
			if(pObjBase->m_wTypeId - nLastVal > 1)
			{
				return pObjBase->m_wTypeId-1;
			}

			if(pObjBase->m_wTypeId+1 > nRetVal)
			{
				nRetVal = pObjBase->m_wTypeId+1;
			}

			nLastVal = pObjBase->m_wTypeId;
		}
	}

	return nRetVal;	
}

int CDMHelperApp::GetNextCustomArmorIndex(BOOL bMetalArmor)
{
	int nRetVal = 12100;

	if(bMetalArmor)
		nRetVal = 12200;

	int nLastVal = nRetVal-1;

	for(int k = 0; k < m_CustomArmorsOrderedTypeArray.GetSize(); ++k)
	{
		cDNDArmor *pArmorBase = (cDNDArmor *)m_CustomArmorsOrderedTypeArray.GetAt(k);

		if(pArmorBase != NULL)
		{
			if(bMetalArmor && pArmorBase->m_wTypeId < 12200)
			{
				continue;
			}
			else  if(pArmorBase->m_wTypeId >= 12200)
			{
				continue;
			}

			if(pArmorBase->m_wTypeId - nLastVal > 1)
			{
				return pArmorBase->m_wTypeId-1;
			}

			if(pArmorBase->m_wTypeId+1 > nRetVal)
			{
				nRetVal = pArmorBase->m_wTypeId+1;
			}

			nLastVal = pArmorBase->m_wTypeId;
		}
	}

	return nRetVal;	
}

int CDMHelperApp::GetNextCustomWeaponIndex()
{
	int nRetVal = 13000;

	int nLastVal = nRetVal - 1;

	for (int k = 0; k < m_CustomWeaponsOrderedTypeArray.GetSize(); ++k)
	{
		cDNDWeapon *pWeaponBase = (cDNDWeapon *)m_CustomWeaponsOrderedTypeArray.GetAt(k);

		if (pWeaponBase != NULL)
		{
			if (pWeaponBase->m_wTypeId - nLastVal > 1)
			{
				return pWeaponBase->m_wTypeId - 1;
			}

			if (pWeaponBase->m_wTypeId + 1 > nRetVal)
			{
				nRetVal = pWeaponBase->m_wTypeId + 1;
			}

			nLastVal = pWeaponBase->m_wTypeId;
		}
	}

	return nRetVal;
}


int CDMHelperApp::LoadDungeonTiles()
{
	int nNumTiles = 0;

	if(m_pDungeonTilesBitmap == NULL)
	{
		Bitmap* pDungeonTilesBitmap = NULL;

		CString szFileName;
		szFileName.Format("%sData/maps/bitmaps/dungeon_tiles.png", m_szEXEPath.GetBuffer(0));

		BSTR b = szFileName.AllocSysString(); 
		pDungeonTilesBitmap  = Bitmap::FromFile(b);
		//m_pDungeonTilesBitmap = Bitmap::FromResource(AfxGetApp()->m_hInstance, MAKEINTRESOURCEW(IDB_MOON_CELLS_BITMAP));

		m_pDungeonTilesBitmap = pDungeonTilesBitmap->Clone(0,0,400,600,PixelFormatDontCare); // make copy in memory
		delete pDungeonTilesBitmap;

		SysFreeString(b); 
	}

	int nHeight = m_pDungeonTilesBitmap->GetHeight();
	nNumTiles = nHeight / 40 * 10;

	return nNumTiles;
}

int CDMHelperApp::LoadIsometricDungeonTiles()
{
	int nNumTiles = 0;

	if (m_pIsometricDungeonTilesBitmap == NULL)
	{
		Bitmap* pDungeonTilesBitmap = NULL;

		CString szFileName;
		szFileName.Format("%sData/maps/bitmaps/iso_dungeon_tiles.png", m_szEXEPath.GetBuffer(0));

		BSTR b = szFileName.AllocSysString();
		pDungeonTilesBitmap = Bitmap::FromFile(b);
		//m_pDungeonTilesBitmap = Bitmap::FromResource(AfxGetApp()->m_hInstance, MAKEINTRESOURCEW(IDB_MOON_CELLS_BITMAP));

		m_pIsometricDungeonTilesBitmap = pDungeonTilesBitmap->Clone(0, 0, 810, 600, PixelFormatDontCare); // make copy in memory
		delete pDungeonTilesBitmap;

		SysFreeString(b);
	}

	int nHeight = m_pIsometricDungeonTilesBitmap->GetHeight();
	nNumTiles = nHeight / 80 * 10;

	return nNumTiles;
}

void CDMHelperApp::LoadCalendars(char *path)
{
	FILE *pInfile = fopen(path, "rt");

	char szInLine[256];
	int nCalendarCount = 0;

	cDNDCalendar *pCalendar = NULL;

	if(pInfile != NULL)
	{
		int nCalendarCount = 0;
		int nLineRead = 0;
		int nSubIndex = 0;
		int nHolidaysRead = 0;
		int nMoonsRead = 0;
		int nMoonPhasesRead = 0;

		while (!feof(pInfile))
		{
			if(pCalendar == NULL)
			{
				pCalendar = new cDNDCalendar();
			}

			fgets(szInLine, 255, pInfile);
			CString szTemp = szInLine;
			
			if(szTemp.FindOneOf("#") >= 0)
				continue;

			szTemp.Replace("\r", "");
			szTemp.Replace("\n", "");

			switch(nLineRead)
			{
				case 0:
				{
					pCalendar->m_dwCalendarID = atoi(szTemp.GetBuffer(0));
					++nLineRead;
					break;
				}
				case 1:
				{
					strcpy(pCalendar->m_szCalendarName, szTemp.GetBuffer(0));
					++nLineRead;
					break;
				}
				case 2:
				{
					pCalendar->m_nBaseYear = atoi(szTemp.GetBuffer(0));
					++nLineRead;
					break;
				}
				case 3:
				{
					pCalendar->m_nDaysInWeek = atoi(szTemp.GetBuffer(0));
					++nLineRead;
					break;
				}
				case 4:
				{
					pCalendar->m_nMonthsInYear = atoi(szTemp.GetBuffer(0));
					++nLineRead;
					break;
				}
				case 5:
				{
					strcpy(pCalendar->m_szDayNames[nSubIndex++], szTemp.GetBuffer(0));

					if(nSubIndex >= pCalendar->m_nDaysInWeek)
					{
						nSubIndex = 0;
						++nLineRead;
					}

					break;
				}
				case 6:
				{
					CString sToken=_T("");
					int i = 0; // substring index to extract
					while (AfxExtractSubString(sToken, szTemp, i,','))
					{   
						sToken.Replace("	", "");	//remove tabs

						switch(i)
						{
							case 0:	
							{
								strcpy(pCalendar->m_szMonthNames[nSubIndex], sToken.GetBuffer(0)); 
								break;
							}
							case 1: 
							{
								pCalendar->m_nDaysInMonth[nSubIndex] = atoi(sToken.GetBuffer(0));

								++nSubIndex;

								if(nSubIndex >= pCalendar->m_nMonthsInYear)
								{
									nSubIndex = 0;
									++nLineRead;

									break;
								}
								
							}
						}

						i++;
					}

					break;
					
				}
				case 7:
				{
					//example 2/4/100/400 for Gregorian calendar
					CString sToken=_T("");
					int i = 0; // substring index to extract
					while (AfxExtractSubString(sToken, szTemp, i,'/'))
					{   
						sToken.Replace("	", "");	//remove tabs

						switch(i)
						{
							case 0:	
							{
								pCalendar->m_nLeapYearMonth = atoi(sToken.GetBuffer(0)); 
								break;
							}
							case 1: 
							{
								pCalendar->m_nLeapYearDivisor = atoi(sToken.GetBuffer(0));
								break;
							}
							case 2: 
							{
								pCalendar->m_nLeapYearExceptionDivisor = atoi(sToken.GetBuffer(0));
								break;
							}
							case 3: 
							{
								pCalendar->m_nLeapYearExceptionExceptionDivisor = atoi(sToken.GetBuffer(0));
								break;
							}
						}

						i++;
					}

					++nLineRead;
					break;
				}
				case 8:
				{
					pCalendar->m_nCalendarStartsOnDayOfWeek = atoi(szTemp.GetBuffer(0));

					++nLineRead;

					break;
				}
				case 9: // number of holidays
				{
					pCalendar->m_nNumHolidays = atoi(szTemp.GetBuffer(0));

					++nLineRead;

					break;
				}
				case 10:
				{
					//parse holiday line here

					//example 12/25/Christmas for Gregorian calendar Christmas
					CString sToken=_T("");
					int i = 0; // substring index to extract
					while (AfxExtractSubString(sToken, szTemp, i,'/'))
					{   
						sToken.Replace("	", "");	//remove tabs

						switch(i)
						{
							case 0:	
							{
								pCalendar->m_Holidays[nHolidaysRead].m_nMonth = atoi(sToken.GetBuffer(0)); 
								break;
							}
							case 1: 
							{
								pCalendar->m_Holidays[nHolidaysRead].m_nDay = atoi(sToken.GetBuffer(0)); 
								break;
							}
							case 2: 
							{
								strcpy(pCalendar->m_Holidays[nHolidaysRead].m_szDesc, sToken.GetBuffer(0)); 
								break;
							}
						}

						i++;
					}

					++nHolidaysRead;

					if(nHolidaysRead >= pCalendar->m_nNumHolidays)
					{
						/*
						m_CalendarArray.InsertAt(nCalendarCount++, pCalendar);

						//next calendar
						pCalendar = NULL;
						nSubIndex = 0;
						nLineRead = 0;
						nHolidaysRead = 0;
						*/

						++nLineRead;
					}

					break;
				} // end case 10
				
				case 11: // moons
				{
					pCalendar->m_nMoons = atoi(szTemp.GetBuffer(0));

					++nLineRead;

					break;
				}
				case 12: // moon name
				{
					strcpy(pCalendar->m_szMoonName[nMoonsRead], szTemp.GetBuffer(0)); 
					pCalendar->m_szMoonName[nMoonsRead][31] = 0;
					
					++nLineRead;

					break;
				}
				case 13: // moon phases
				{
					pCalendar->m_nMoonPhases[nMoonsRead] = atoi(szTemp.GetBuffer(0));

					++nLineRead;

					break;
				}
				case 14: // moon phase names + GFX ID
				{
					//parse phase name line here

					//example New moon,1 for Gregorian calendar Christmas
					CString sToken=_T("");
					int i = 0; // substring index to extract
					while (AfxExtractSubString(sToken, szTemp, i,','))
					{   
						sToken.Replace("	", "");	//remove tabs

						switch(i)
						{
							case 0:	
							{
								strcpy(pCalendar->m_szMoonPhaseName[nMoonsRead][nMoonPhasesRead], sToken.GetBuffer(0)); 
								pCalendar->m_szMoonPhaseName[nMoonsRead][nMoonPhasesRead][31] = 0;

								i++;

								break;
							}
							case 1: 
							{
								pCalendar->m_nMoonPhaseSymbol[nMoonsRead][nMoonPhasesRead] = atoi(sToken.GetBuffer(0)); 
								
								++nMoonPhasesRead;

								if(nMoonPhasesRead >= pCalendar->m_nMoonPhases[nMoonsRead])
								{
									++nLineRead;
								}

								i++;

								break;
							}
						}
					}

					break;
				} //end case 14

				case 15: //moon cycle
				{
					pCalendar->m_fMoonCycle[nMoonsRead] = (float)atof(szTemp.GetBuffer(0));

					++nLineRead;

					break;
				}

				case 16: //moon cycle base
				{
					pCalendar->m_fMoonCycleBase[nMoonsRead] = (float)atof(szTemp.GetBuffer(0));

					++nMoonsRead;

					if(nMoonsRead >= pCalendar->m_nMoons) 
					{
						pCalendar->Initialize();
						m_CalendarArray.InsertAt(nCalendarCount++, pCalendar);

						//next calendar
						pCalendar = NULL;
						nSubIndex = 0;
						nLineRead = 0;
						nHolidaysRead = 0;
						nMoonsRead = 0;
						nMoonPhasesRead = 0;
					}
					else
					{
						nLineRead = 12;
						nMoonPhasesRead = 0;
					}

					break;
				}


			}

		}

		if(pCalendar != NULL) // probably an error if we get here
		{
			if(pCalendar->m_szCalendarName[0] != 0)
			{
				CString szTemp;
				CString szCalError = _T("");

				if(nHolidaysRead != pCalendar->m_nNumHolidays)
					szCalError += _T("Holidays: Number Holidays != Holiday Names\n\r");

				szTemp.Format("Calendar %s not loaded due to errors !", pCalendar->m_szCalendarName);
				szCalError += szTemp;

				szTemp.Format("\n\r\n\rMonths In Year: %d\n\rDays In Week: %d\n\rNum Holidays: %d\n\rNum Moons: %d",
				pCalendar->m_nMonthsInYear,
				pCalendar->m_nDaysInWeek,
				pCalendar->m_nNumHolidays,
				pCalendar->m_nMoons);

				szCalError += szTemp;

				AfxMessageBox(szCalError, MB_OK);
			}

			delete pCalendar;
		}

		fclose(pInfile);
	}
	else
	{
		CString szTemp;
		szTemp.Format("CRITICAL ERROR: FILE %s not found !", path);
		AfxMessageBox(szTemp, MB_OK);
	}
}

void CDMHelperApp::LoadArmorTable(char *path)
{
	FILE *pInfile = fopen(path, "rt");

	char szInLine[256];
	int nArmorCount = 0;

	if(pInfile != NULL)
	{
		while (!feof(pInfile))
		{
			fgets(szInLine, 255, pInfile);
			CString szTemp = szInLine;
			
			if(szTemp.FindOneOf("#") >= 0)
				continue;

			cDNDArmor *pArmor = new cDNDArmor();

			//id, type, armor class, weight, move
			CString sToken=_T("");
			int i = 0; // substring index to extract
			while (AfxExtractSubString(sToken, szTemp, i,','))
			{   
				sToken.Replace("	", "");	//remove tabs

				switch(i)
				{
					case 0: pArmor->m_wTypeId = atoi(sToken.GetBuffer(0)); break;
					case 1: pArmor->m_nCost = GetPriceFromString(sToken.GetBuffer(0)); break;
					case 2: strcpy(pArmor->m_szType, sToken.GetBuffer(0)); break;
					case 3: pArmor->m_nArmorClass = atoi(sToken.GetBuffer(0)); break;
					case 4: pArmor->m_nWeight = atoi(sToken.GetBuffer(0)); break;
					case 5: pArmor->m_nMove = atoi(sToken.GetBuffer(0)); break;
				}
				i++;
			}

			if(pArmor->m_szType[0] == 0) //bad data
			{
				delete pArmor;
				continue;
			}

			//TRACE("ADD ARMOR %s AT %d (%d)\n", pArmor->m_szType, nArmorCount, pArmor->m_szType[0]);

			m_ArmorOrderedTypeMap.SetAt(nArmorCount, pArmor);
			m_ArmorIndexedTypeMap.SetAt(pArmor->m_wTypeId, pArmor);
			++nArmorCount;

			if(pArmor->m_wTypeId)
			{
				m_ObjectsOrderedTypeArray.InsertAt(m_nDefinedObjectCount, pArmor->Clone());
				m_ObjectsIndexedTypeArray.InsertAt(pArmor->m_wTypeId, pArmor);
				++m_nDefinedObjectCount;
			}

		}

		fclose(pInfile);

		int nCountArmor = m_ObjectsOrderedTypeArray.GetSize();
	}
}

void CDMHelperApp::LoadCustomArmorsTable(char *path)
{
	FILE *pInfile = fopen(path, "rt");

	char szInLine[256];
	int nArmorCount = m_ArmorOrderedTypeMap.GetCount();

	LONG lFileVersion = 0L;

	if(pInfile != NULL)
	{
		while (!feof(pInfile))
		{
			fgets(szInLine, 255, pInfile);
			CString szTemp = szInLine;
			
			if(szTemp.FindOneOf("#") >= 0)
				continue;

			//First real data line MUST be file version
			if(lFileVersion == 0L)
			{
				lFileVersion = atol(szTemp.GetBuffer(0));
				continue;
			}

			cDNDArmor *pArmor = new cDNDArmor();

			//id, type, armor class, weight, move
			CString sToken=_T("");
			int i = 0; // substring index to extract
			while (AfxExtractSubString(sToken, szTemp, i,','))
			{   
				sToken.Replace("	", "");	//remove tabs

				switch(i)
				{
					case 0: pArmor->m_wTypeId = atoi(sToken.GetBuffer(0)); break;
					case 1: pArmor->m_nCost = GetPriceFromString(sToken.GetBuffer(0)); break;
					case 2: strcpy(pArmor->m_szType, sToken.GetBuffer(0)); break;
					case 3: pArmor->m_nArmorClass = atoi(sToken.GetBuffer(0)); break;
					case 4: pArmor->m_nWeight = atoi(sToken.GetBuffer(0)); break;
					case 5: pArmor->m_nMove = atoi(sToken.GetBuffer(0)); break;
					case 6: pArmor->m_lXP = atol(sToken.GetBuffer(0)); break;
				}
				i++;
			}

			if(pArmor->m_szType[0] == 0) //bad data
			{
				delete pArmor;
				continue;
			}

			//TRACE("ADD ARMOR %s AT %d (%d)\n", pArmor->m_szType, nArmorCount, pArmor->m_szType[0]);

			m_CustomArmorsOrderedTypeArray.InsertAt(pArmor->m_wTypeId, pArmor, 1);

			m_ArmorOrderedTypeMap.SetAt(nArmorCount, pArmor);
			m_ArmorIndexedTypeMap.SetAt(pArmor->m_wTypeId, pArmor);
			++nArmorCount;

			if(pArmor->m_wTypeId)
			{
				m_ObjectsOrderedTypeArray.InsertAt(m_nDefinedObjectCount, pArmor->Clone());
				m_ObjectsIndexedTypeArray.InsertAt(pArmor->m_wTypeId, pArmor);
				++m_nDefinedObjectCount;

				POBJECTTYPE pObj = m_ObjectsIndexedTypeArray.GetAt(pArmor->m_wTypeId);
				TRACE("pObj >> %x\n", pObj);
			}

		}

		fclose(pInfile);

		int nCountArmor = m_ObjectsOrderedTypeArray.GetSize();
	}
}

void CDMHelperApp::LoadCustomWeaponsTable(char *path)
{
	FILE *pInfile = fopen(path, "rt");

	char szInLine[256];
	int nWeaponCount = m_WeaponsOrderedTypeArray.GetCount();
	int nAmmoCount = m_AmmunitionOrderedTypeArray.GetCount();
	int nCustomWeaponCount = 0;

	CString szTemp;

	if (pInfile != NULL)
	{
		while (!feof(pInfile))
		{
			fgets(szInLine, 255, pInfile);
			CString szTemp = szInLine;

			if (szTemp.FindOneOf("#") >= 0)
				continue;

			cDNDWeapon *pWeapon = new cDNDWeapon();

			//id, type, weight
			CString sToken = _T("");
			int i = 0; // substring index to extract
			while (AfxExtractSubString(sToken, szTemp, i, ','))
			{
				sToken.Replace("	", "");	//remove tabs

				switch (i)
				{
					//#ID	TYPE	COST		WGHT	SPACE		SPD	DMGS	DMGL	EXCP	FLAGS	
					case 0: pWeapon->m_wTypeId = atoi(sToken.GetBuffer(0)); break;

					case 1:
					{
						sToken.Replace("_", ",");	//remove tabs
						strcpy(pWeapon->m_szType, sToken.GetBuffer(0));
						break;
					}

					case 2: //cost
					{
						pWeapon->m_nCost = GetPriceFromString(sToken.GetBuffer(0));
						break;
					}

					case 3: pWeapon->m_nWeight = atoi(sToken.GetBuffer(0)); break;

					case 4: strcpy(pWeapon->m_szSpaceRange, sToken.GetBuffer(0)); break;
					case 5: strcpy(pWeapon->m_szSpeed, sToken.GetBuffer(0)); break;
					case 6: strcpy(pWeapon->m_szDamageSmall, sToken.GetBuffer(0)); break;
					case 7: strcpy(pWeapon->m_szDamageLarge, sToken.GetBuffer(0)); break;

					case 8:
					{
						if (sToken.FindOneOf("Y") >= 0)
						{
							pWeapon->m_nExceptionalStrength = 1;
						}
						break;
					}

					case 9: pWeapon->m_nFlags = atoi(sToken.GetBuffer(0)); break;

						//weapons vs. AC
					case 10:
					case 11:
					case 12:
					case 13:
					case 14:
					case 15:
					case 16:
					case 17:
					case 18:
					case 19:
					case 20:
					{
						pWeapon->m_nWeaponVSAC[i - 10] = atoi(sToken.GetBuffer(0));
						break;
					}
					case 21:
					{
						pWeapon->m_nCustomAmmoType = atoi(sToken.GetBuffer(0));
						break;
					}
					case 22:
					{
						pWeapon->m_nCustomFlags = atoi(sToken.GetBuffer(0));
						break;
					}
				}
				i++;
			}

			if (pWeapon->m_szType[0] == 0) //bad data
			{
				delete pWeapon;
				continue;
			}

			if (pWeapon->m_wTypeId)
			{
				if (IsAmmo(pWeapon)) // this is ammo
				{
					pWeapon->m_ObjectType = DND_OBJECT_TYPE_AMMO;
					m_AmmunitionOrderedTypeArray.InsertAt(nAmmoCount, pWeapon);
					m_AmmunitionIndexedTypeArray.InsertAt(pWeapon->m_wTypeId, pWeapon);
					++nAmmoCount;
				}
				else
				{
					m_WeaponsOrderedTypeArray.InsertAt(nWeaponCount, pWeapon);
					m_WeaponsIndexedTypeArray.InsertAt(pWeapon->m_wTypeId, pWeapon);
					++nWeaponCount;
				}

				m_CustomWeaponsOrderedTypeArray.InsertAt(nCustomWeaponCount++, pWeapon);

				m_ObjectsOrderedTypeArray.InsertAt(m_nDefinedObjectCount, pWeapon->Clone());
				m_ObjectsIndexedTypeArray.InsertAt(pWeapon->m_wTypeId, pWeapon);
				++m_nDefinedObjectCount;
			}

		}

		fclose(pInfile);
	}
}

void CDMHelperApp::LoadHelmetsTable(char *path)
{
	FILE *pInfile = fopen(path, "rt");

	char szInLine[256];
	int nHelmetCount = 0;

	if(pInfile != NULL)
	{
		while (!feof(pInfile))
		{
			fgets(szInLine, 255, pInfile);
			CString szTemp = szInLine;
			
			if(szTemp.FindOneOf("#") >= 0)
				continue;

			cDNDObject *pHelmet = new cDNDObject();

			//id, type, weight
			CString sToken=_T("");
			int i = 0; // substring index to extract
			while (AfxExtractSubString(sToken, szTemp, i,','))
			{   
				sToken.Replace("	", "");	//remove tabs

				switch(i)
				{
					case 0: pHelmet->m_wTypeId = atoi(sToken.GetBuffer(0)); break;
					case 1: pHelmet->m_nCost = GetPriceFromString(sToken.GetBuffer(0)); break; //cost
					case 2: 
					{
						sToken.Replace("_", ",");
						strcpy(pHelmet->m_szType, sToken.GetBuffer(0)); 
						break;
					}
					case 3: pHelmet->m_nWeight = atoi(sToken.GetBuffer(0)); break;
					case 4: pHelmet->m_nFlags = atoi(sToken.GetBuffer(0)); break;
				}
				i++;
			}

			if(pHelmet->m_wTypeId == 0 || pHelmet->m_szType[0] == 0) //bad data
			{
				delete pHelmet;
				continue;
			}

			pHelmet->m_ObjectType = DND_OBJECT_TYPE_HELMET;

			m_HelmetsOrderedTypeArray.InsertAt(nHelmetCount, pHelmet);
			m_HelmetsIndexedTypeArray.InsertAt(pHelmet->m_wTypeId, pHelmet);
			++nHelmetCount;

			if(pHelmet->m_wTypeId)
			{
				m_ObjectsOrderedTypeArray.InsertAt(m_nDefinedObjectCount, pHelmet->Clone());
				m_ObjectsIndexedTypeArray.InsertAt(pHelmet->m_wTypeId, pHelmet);
				++m_nDefinedObjectCount;
			}

		}

		fclose(pInfile);
	}

}

void CDMHelperApp::LoadShieldsTable(char *path)
{
	FILE *pInfile = fopen(path, "rt");

	char szInLine[256];
	int nShieldCount = 0;

	if(pInfile != NULL)
	{
		while (!feof(pInfile))
		{
			fgets(szInLine, 255, pInfile);
			CString szTemp = szInLine;
			
			if(szTemp.FindOneOf("#") >= 0)
				continue;

			cDNDShield *pShield = new cDNDShield();

			//id, type, weight
			CString sToken=_T("");
			int i = 0; // substring index to extract
			while (AfxExtractSubString(sToken, szTemp, i,','))
			{   
				sToken.Replace("	", "");	//remove tabs

				switch(i)
				{
					case 0: pShield->m_wTypeId = atoi(sToken.GetBuffer(0)); break;
					case 1: pShield->m_nCost = GetPriceFromString(sToken.GetBuffer(0)); break; //cost
					case 2: strcpy(pShield->m_szType, sToken.GetBuffer(0)); break;
					case 3: pShield->m_nWeight = atoi(sToken.GetBuffer(0)); break;
				}
				i++;
			}

			if(pShield->m_szType[0] == 0) //bad data
			{
				delete pShield;
				continue;
			}

			//TRACE("ADD ARMOR %s AT %d (%d)\n", pArmor->m_szType, nArmorCount, pArmor->m_szType[0]);

			m_ShieldsOrderedTypeMap.SetAt(nShieldCount, pShield);
			m_ShieldsIndexedTypeMap.SetAt(pShield->m_wTypeId, pShield);
			++nShieldCount;

			if(pShield->m_wTypeId)
			{
				m_ObjectsOrderedTypeArray.InsertAt(m_nDefinedObjectCount, pShield->Clone());
				m_ObjectsIndexedTypeArray.InsertAt(pShield->m_wTypeId, pShield);
				++m_nDefinedObjectCount;
			}

		}

		fclose(pInfile);
	}
}

void CDMHelperApp::LoadWeaponsTable(char *path)
{
	FILE *pInfile = fopen(path, "rt");

	char szInLine[256];
	int nWeaponCount = 0;
	int nAmmoCount = 0;
	CString szTemp;

	if(pInfile != NULL)
	{
		while (!feof(pInfile))
		{
			fgets(szInLine, 255, pInfile);
			CString szTemp = szInLine;
			
			if(szTemp.FindOneOf("#") >= 0)
				continue;

			cDNDWeapon *pWeapon = new cDNDWeapon();

			//id, type, weight
			CString sToken=_T("");
			int i = 0; // substring index to extract
			while (AfxExtractSubString(sToken, szTemp, i,','))
			{   
			   sToken.Replace("	", "");	//remove tabs

			   switch(i)
			   {
				    //#ID	TYPE	COST		WGHT	SPACE		SPD	DMGS	DMGL	EXCP	FLAGS	
					case 0: pWeapon->m_wTypeId = atoi(sToken.GetBuffer(0)); break;

					case 1:
					{
						sToken.Replace("_", ", ");	//remove tabs
						strcpy(pWeapon->m_szType, sToken.GetBuffer(0)); 
						break;
					}

					case 2: //cost
					{
						pWeapon->m_nCost = GetPriceFromString(sToken.GetBuffer(0));
						break;
					}

					case 3: pWeapon->m_nWeight = atoi(sToken.GetBuffer(0)); break;

					case 4: strcpy(pWeapon->m_szSpaceRange, sToken.GetBuffer(0)); break;
					case 5: strcpy(pWeapon->m_szSpeed, sToken.GetBuffer(0)); break;
					case 6: strcpy(pWeapon->m_szDamageSmall, sToken.GetBuffer(0)); break;
					case 7: strcpy(pWeapon->m_szDamageLarge, sToken.GetBuffer(0)); break;

					case 8: 
					{
						if(sToken.FindOneOf("Y") >= 0)
						{
							pWeapon->m_nExceptionalStrength = 1;
						}
						break;
					}

					case 9: pWeapon->m_nFlags = atoi(sToken.GetBuffer(0)); break;

					//weapons vs. AC
					case 10:
					case 11:
					case 12:
					case 13:
					case 14:
					case 15:
					case 16:
					case 17:
					case 18:
					case 19:
					case 20:
					{
						pWeapon->m_nWeaponVSAC[i-10] = atoi(sToken.GetBuffer(0)); 
						break;
					}
			   }
			   i++;
			}

			if(pWeapon->m_szType[0] == 0) //bad data
			{
				delete pWeapon;
				continue;
			}

			//TRACE("ADD ARMOR %s AT %d (%d)\n", pArmor->m_szType, nArmorCount, pArmor->m_szType[0]);

			if(IsAmmo(pWeapon)) // this is ammo
			{
				pWeapon->m_ObjectType = DND_OBJECT_TYPE_AMMO;
				m_AmmunitionOrderedTypeArray.InsertAt(nAmmoCount, pWeapon);
				m_AmmunitionIndexedTypeArray.InsertAt(pWeapon->m_wTypeId, pWeapon);
				++nAmmoCount;
			}
			else
			{
				m_WeaponsOrderedTypeArray.InsertAt(nWeaponCount, pWeapon);
				m_WeaponsIndexedTypeArray.InsertAt(pWeapon->m_wTypeId, pWeapon);
				++nWeaponCount;
			}

			if(pWeapon->m_wTypeId)
			{
				if(pWeapon->m_wTypeId == 1160)
				{
					TRACE("HIYA\n");
				}

				if((pWeapon->m_nFlags / 10) % 10 == 2) //  pummel attack
					m_nPummelWeaponIndex = pWeapon->m_wTypeId;

				if((pWeapon->m_nFlags / 10) % 10 == 3) // monk open hand attack
					m_nMonkWeaponIndex = pWeapon->m_wTypeId;
				
				m_ObjectsOrderedTypeArray.InsertAt(m_nDefinedObjectCount, pWeapon->Clone());
				m_ObjectsIndexedTypeArray.InsertAt(pWeapon->m_wTypeId, pWeapon);
				++m_nDefinedObjectCount;
			}

		}

		fclose(pInfile);
	}
}

void CDMHelperApp::LoadRingsTable(char *path)
{
	FILE *pInfile = fopen(path, "rt");

	char szInLine[256];
	int nRingCount = 0;

	if(pInfile != NULL)
	{
		while (!feof(pInfile))
		{
			fgets(szInLine, 255, pInfile);
			CString szTemp = szInLine;
			
			if(szTemp.FindOneOf("#") >= 0)
				continue;

			cDNDRing *pRing = new cDNDRing();

			//id, type, weight
			CString sToken=_T("");
			int i = 0; // substring index to extract
			while (AfxExtractSubString(sToken, szTemp, i,','))
			{   
				sToken.Replace("	", "");	//remove tabs

				switch(i)
				{
					case 0: pRing->m_wTypeId = atoi(sToken.GetBuffer(0)); break;
					case 1: pRing->m_nCost = GetPriceFromString(sToken.GetBuffer(0)); break; //cost
					case 2:
					{
						sToken.Replace("_",",");
						strcpy(pRing->m_szType, sToken.GetBuffer(0)); 
						break;
					}
					case 3: pRing->m_nWeight = atoi(sToken.GetBuffer(0)); break;
					case 4: pRing->m_nFlags = atoi(sToken.GetBuffer(0)); break;
				}
				i++;
			}

			if(pRing->m_szType[0] == 0) //bad data
			{
				delete pRing;
				continue;
			}

			m_RingsOrderedTypeMap.SetAt(nRingCount, pRing);
			m_RingsIndexedTypeMap.SetAt(pRing->m_wTypeId, pRing);
			++nRingCount;

			if(pRing->m_wTypeId)
			{
				m_ObjectsOrderedTypeArray.InsertAt(m_nDefinedObjectCount, pRing->Clone());
				m_ObjectsIndexedTypeArray.InsertAt(pRing->m_wTypeId, pRing);
				++m_nDefinedObjectCount;
			}

		}

		fclose(pInfile);
	}
}



void CDMHelperApp::LoadEquipmentTable(char *path)
{
	FILE *pInfile = fopen(path, "rt");

	char szInLine[256];
	int nObjectCount = 0;

	if(pInfile != NULL)
	{
		while (!feof(pInfile))
		{
			fgets(szInLine, 255, pInfile);
			CString szTemp = szInLine;
			
			if(szTemp.FindOneOf("#") >= 0)
				continue;

			cDNDObject *pObject = new cDNDObject();

			//id, type, weight
			CString sToken=_T("");
			int i = 0; // substring index to extract
			while (AfxExtractSubString(sToken, szTemp, i,','))
			{   
				sToken.Replace("	", "");	//remove tabs

				switch(i)
				{
					case 0: pObject->m_wTypeId = atoi(sToken.GetBuffer(0)); break;
					case 1: pObject->m_nCost = GetPriceFromString(sToken.GetBuffer(0)); break; //cost
					case 2: 
					{
						sToken.Replace("_", ",");
						strcpy(pObject->m_szType, sToken.GetBuffer(0)); 
						break;
					}
					case 3: pObject->m_nWeight = atoi(sToken.GetBuffer(0)); break;
					case 4: pObject->m_nFlags = atoi(sToken.GetBuffer(0)); break;
				}
				i++;
			}

			if(pObject->m_wTypeId == 0 || pObject->m_szType[0] == 0) //bad data
			{
				delete pObject;
				continue;
			}

			pObject->m_ObjectType = DND_OBJECT_TYPE_EQUIPMENT;

			m_EquipmentOrderedTypeArray.InsertAt(nObjectCount, pObject);
			m_EquipmentIndexedTypeArray.InsertAt(pObject->m_wTypeId, pObject);
			++nObjectCount;

			if(pObject->m_wTypeId)
			{
				m_ObjectsOrderedTypeArray.InsertAt(m_nDefinedObjectCount, pObject->Clone());
				m_ObjectsIndexedTypeArray.InsertAt(pObject->m_wTypeId, pObject);
				++m_nDefinedObjectCount;
			}

		}

		fclose(pInfile);
	}

}

void CDMHelperApp::LoadCustomItemsTable(char *path)
{
	FILE *pInfile = fopen(path, "rt");

	LONG lFileVersion = 0L;

	char szInLine[256];
	int nObjectCount = 0;
	WORD wLastTypeID = 0;

	char sTab[2];
	sprintf(sTab,"%c\0",9);

	if(pInfile != NULL)
	{
		while (!feof(pInfile))
		{
			fgets(szInLine, 255, pInfile);
			CString szTemp = szInLine;
			
			if(szTemp.FindOneOf("#") >= 0)
				continue;

			//First real data line MUST be file version
			if(lFileVersion == 0L)
			{
				lFileVersion = atol(szTemp.GetBuffer(0));
				continue;
			}

			POBJECTTYPE pTestObj = m_ObjectsIndexedTypeArray.GetAt(12100);

			cDNDObject *pObject = new cDNDObject();

			//id, type, weight
			CString sToken=_T("");
			int i = 0; // substring index to extract
			while (AfxExtractSubString(sToken, szTemp, i,','))
			{   
				sToken.Replace("	", "");	//remove tabs
				sToken.Replace(sTab, "");	//remove tabs

				switch(i)
				{
					case 0: pObject->m_wTypeId = atoi(sToken.GetBuffer(0)); break;
					case 1: pObject->m_nCost = GetPriceFromString(sToken.GetBuffer(0)); break; //cost
					case 2: 
					{
						sToken.Replace("_", ",");
						strcpy(pObject->m_szType, sToken.GetBuffer(0)); 
						break;
					}
					case 3: pObject->m_nWeight = atoi(sToken.GetBuffer(0)); break;
					case 4: pObject->m_lXP = atoi(sToken.GetBuffer(0)); break;
					case 5: pObject->m_nFlags = atoi(sToken.GetBuffer(0)); break;
				}
				i++;
			}

			if(pObject->m_wTypeId == 0 || pObject->m_szType[0] == 0) //bad data
			{
				delete pObject;
				continue;
			}

			pTestObj = m_ObjectsIndexedTypeArray.GetAt(12100); 

			pObject->m_ObjectType = DND_OBJECT_TYPE_EQUIPMENT;

			m_EquipmentOrderedTypeArray.InsertAt(nObjectCount, pObject);
			m_EquipmentIndexedTypeArray.InsertAt(pObject->m_wTypeId, pObject);
			++nObjectCount;

			if(pObject->m_wTypeId && wLastTypeID != pObject->m_wTypeId)
			{
				pTestObj = m_ObjectsIndexedTypeArray.GetAt(12100);

				m_ObjectsOrderedTypeArray.InsertAt(m_nDefinedObjectCount, pObject->Clone());
				
				pTestObj = m_ObjectsIndexedTypeArray.GetAt(12100);
				
				m_ObjectsIndexedTypeArray.InsertAt(pObject->m_wTypeId, pObject);

				pTestObj = m_ObjectsIndexedTypeArray.GetAt(12100);

				m_CustomObjectsOrderedTypeArray.InsertAt(pObject->m_wTypeId, pObject, 1);

				pTestObj = m_ObjectsIndexedTypeArray.GetAt(12100);

				++m_nDefinedObjectCount;

				wLastTypeID = pObject->m_wTypeId;
			}

		}

		fclose(pInfile);
	}

}


void CDMHelperApp::LoadMagicItemTable(char *path)
{
	FILE *pInfile = fopen(path, "rt");

	char szInLine[256];
	int nObjectCount = 0;

	if(pInfile != NULL)
	{
		while (!feof(pInfile))
		{
			fgets(szInLine, 255, pInfile);
			CString szTemp = szInLine;
			
			if(szTemp.FindOneOf("#") >= 0)
				continue;

			cDNDMagicTableItem *pMagicItem = new cDNDMagicTableItem();

			//id, type, weight
			CString sToken=_T("");
			int i = 0; // substring index to extract
			while (AfxExtractSubString(sToken, szTemp, i,'|'))
			{   
				sToken.Replace("	", "");	//remove tabs

				switch(i)
				{
					case 0:
					{
						if(sToken == _T("potions"))
							pMagicItem->m_nTable = DND_MAGIC_ITEM_TABLE_POTIONS;
						else if(sToken == _T("rings"))
							pMagicItem->m_nTable = DND_MAGIC_ITEM_TABLE_RINGS;
						else if(sToken == _T("scrolls"))
							pMagicItem->m_nTable = DND_MAGIC_ITEM_TABLE_SCROLLS;
						else if(sToken == _T("rods staves wands"))
							pMagicItem->m_nTable = DND_MAGIC_ITEM_TABLE_RODS_STAVES_WANDS;
						else if(sToken == _T("misc magic 1"))
							pMagicItem->m_nTable = DND_MAGIC_ITEM_TABLE_MISC_MAGIC_1;
						else if(sToken == _T("misc magic 2"))
							pMagicItem->m_nTable = DND_MAGIC_ITEM_TABLE_MISC_MAGIC_2;
						else if(sToken == _T("misc magic 3"))
							pMagicItem->m_nTable = DND_MAGIC_ITEM_TABLE_MISC_MAGIC_3;
						else if(sToken == _T("misc magic 4"))
							pMagicItem->m_nTable = DND_MAGIC_ITEM_TABLE_MISC_MAGIC_4;
						else if(sToken == _T("misc magic 5"))
							pMagicItem->m_nTable = DND_MAGIC_ITEM_TABLE_MISC_MAGIC_5;
						else if(sToken == _T("artifacts relics"))
							pMagicItem->m_nTable = DND_MAGIC_ITEM_TABLE_MISC_MAGIC_SPECIAL;
						else if(sToken == _T("armor"))
							pMagicItem->m_nTable = DND_MAGIC_ITEM_TABLE_ARMOR;
						else if(sToken == _T("swords"))
							pMagicItem->m_nTable = DND_MAGIC_ITEM_TABLE_SWORDS;
						else if(sToken == _T("misc weapons"))
							pMagicItem->m_nTable = DND_MAGIC_ITEM_TABLE_MISC_WEAPONS;
						
						break;
					}
					case 1: // table die roll
					{
						CString sSubToken=_T("");
						int j = 0; // substring index to extract
						while (AfxExtractSubString(sSubToken, sToken, j,'-'))
						{   
							switch(j)
							{
								case 0:	pMagicItem->m_nRollLow = atoi(sSubToken.GetBuffer(0)); break;
								case 1:	pMagicItem->m_nRollHigh = atoi(sSubToken.GetBuffer(0)); break;
							}
							++j;
						}
					
						break;
					}
					case 2:
					{
						sToken.MakeLower();
						sToken.Trim();
						strcpy(pMagicItem->m_szDesc, sToken.Left(127));
						pMagicItem->m_szDesc[127] = 0;

						break;
					}
					case 3: pMagicItem->m_lXPValue = atol(sToken.GetBuffer(0)); break;
					case 4: pMagicItem->m_lGPValue = atol(sToken.GetBuffer(0)); break;
					
					case 5: // table amount
					{
						CString sSubToken=_T("");
						int j = 0; // substring index to extract
						while (AfxExtractSubString(sSubToken, sToken, j,'-'))
						{   
							switch(j)
							{
								case 0:	pMagicItem->m_nAmountLow = atoi(sSubToken.GetBuffer(0)); pMagicItem->m_nAmountHigh = pMagicItem->m_nAmountLow; break;
								case 1:	pMagicItem->m_nAmountHigh = atoi(sSubToken.GetBuffer(0)); break;
							}
							++j;
						}
					
						break;
					}


					case 6: // search and assign mundanes
					{
						CString sSubToken=_T("");
						int j = 0; // substring index to extract
						while (AfxExtractSubString(sSubToken, sToken, j,'*'))
						{   
							switch(j)
							{
								case 0:	
								case 1:	
								case 2:	
								case 3:	
								case 4:	
								case 5:	
								case 6:	
								case 7:	
								case 8:	
								case 9:	
								{
									if (sSubToken.Find("encumbrance", 0) >= 0)
									{
										TRACE("UNCLE IKE !\n");

										sSubToken.Replace("encumbrance", "");
										pMagicItem->m_nEncumbranceOverride = atoi(sSubToken.GetBuffer(0));
									}
									else
									{
										for (int k = 0; k < m_ObjectsOrderedTypeArray.GetSize(); ++k)
										{
											POBJECTTYPE pObjBase = m_ObjectsOrderedTypeArray.GetAt(k);

											if (pObjBase != NULL)
											{
												CString szCompare = pObjBase->m_szType;

												#if 0
												if (szCompare.Find("x-ray", 0) >= 0)
												{
													TRACE("SUPERMAN !\n");
												}
												#endif

												if (szCompare == sSubToken)
												{
													pMagicItem->m_pMundaneObject[j] = pObjBase;
													++pMagicItem->m_nMundanes;
												}
											}
										}
									}

									break;
								}
							}
							++j;
						}
					
						break;
					}
					case 7: // magic adjustment
					{
						switch(pMagicItem->m_nTable)
						{
							case DND_MAGIC_ITEM_TABLE_SCROLLS:
							{
								break;
							}

							default:
							{
								CString sSubToken=_T("");
								int j = 0; // substring index to extract
								while (AfxExtractSubString(sSubToken, sToken, j,'_'))
								{   
									switch(j)
									{
										case 0:	pMagicItem->m_nMagicAdjustmentLow = atoi(sSubToken.GetBuffer(0)); pMagicItem->m_nMagicAdjustmentHigh = pMagicItem->m_nMagicAdjustmentLow; break;
										case 1:	pMagicItem->m_nMagicAdjustmentHigh = atoi(sSubToken.GetBuffer(0)); break;
									}
									++j;
								}

								break;
							}

						} //switch(pMagicItem->m_nTable)
					
						break;
					} // end case 7: // magic adjustment

					case 8: // levels (scrolls)
					{
						CString sSubToken=_T("");
						int j = 0; // substring index to extract
						while (AfxExtractSubString(sSubToken, sToken, j,'*'))
						{   
							CString sSubSubToken=_T("");
							int k = 0; // substring index to extract
							while (AfxExtractSubString(sSubSubToken, sSubToken, k,'_'))
							{
								//if(j && k)
								//{
								//	TRACE("SHAZAM!\n");
								//}

								switch(k)
								{
									case 0:	pMagicItem->m_nLevelLow[j] = atoi(sSubSubToken.GetBuffer(0)); pMagicItem->m_nLevelHigh[j] = pMagicItem->m_nLevelLow[j]; break;
									case 1:	pMagicItem->m_nLevelHigh[j] = atoi(sSubSubToken.GetBuffer(0)); break;
								}

								++k;
							}

							++j;
						}

						break;
					} // end case 8: // levels (scrolls)

					case 9: // spells contained in object
					{
						CString sSubToken=_T("");
						int j = 0; // substring index to extract
						while (AfxExtractSubString(sSubToken, sToken, j,'*'))
						{   
							if(sSubToken != "N/A")
							{
								sSubToken.MakeLower();
								for(int nSpellIndex = 0; nSpellIndex < m_MasterSpellArray.GetCount(); ++nSpellIndex)
								{
									cDNDSpell *pSpell = m_MasterSpellArray.GetAt(nSpellIndex);

									if(pSpell != NULL)
									{
										CString szCheckName = pSpell->m_szSpellName;
										szCheckName.MakeLower();

										if(sSubToken == szCheckName)
										{
											pMagicItem->m_nSpellIndex[j%MAX_OBJECT_CONTENTS] = pSpell->m_nSpellIdentifier + 1;
										}
									}
								}
							}

							++j;
						}

						break;
					} //end case 9: // contained in object

					case 10: // charges per spell cast from the object
					{
						CString sSubToken=_T("");
						int j = 0; // substring index to extract
						while (AfxExtractSubString(sSubToken, sToken, j,'*'))
						{   
							
							pMagicItem->m_nSpellUsageCharges[j%MAX_OBJECT_CONTENTS] = atoi(sSubToken.GetBuffer(0));

							++j;
						}
						break;
					} //end case 10: // charges per spell

					case 11: // charges
					{
						CString sSubToken=_T("");
						int j = 0; // substring index to extract
						while (AfxExtractSubString(sSubToken, sToken, j,'_'))
						{   
							switch(j)
							{
								case 0:	pMagicItem->m_nChargesLow = atoi(sSubToken.GetBuffer(0)); pMagicItem->m_nChargesHigh = pMagicItem->m_nChargesLow; break;
								case 1:	pMagicItem->m_nChargesHigh = atoi(sSubToken.GetBuffer(0)); break;
							}
							++j;
						}

						//if(pMagicItem->m_nChargesHigh)
						//{
						//	TRACE("WOOT!\n");
						//}

						break;
					} //end case 11: // charges
					
				}

				i++;
			}

			if(pMagicItem->m_nMundanes == 0 || pMagicItem->m_pMundaneObject[0] == NULL) //bad data
			{
				delete pMagicItem;
				continue;
			}

			m_MagicTableItemOrderedTypeArray.InsertAt(nObjectCount, pMagicItem);
			//m_MagicTableItemIndexedTypeArray.InsertAt(pObject->m_wTypeId, pObject);
			++nObjectCount;

		}

		fclose(pInfile);
	}

}

void CDMHelperApp::LoadMagicItemDescriptions(char *path)
{
	FILE *pInfile = fopen(path, "rt");

	char szInLine[1024];
	int nFileVersion = 0;
	int nObjectCount = 0;
	int nState = 0;

	CString szDelimit = _T("<d>\n");

	PMAGICITEMDESCRIPTION pMagicItemDescription = NULL;

	CString szDesc = _T("");

	if (pInfile != NULL)
	{
		fgets(szInLine, 10, pInfile);

		while (!feof(pInfile) && nState < 99)
		{
			fgets(szInLine, 1024, pInfile);
			CString szTemp = szInLine;

			CString szFirst = szTemp.Left(2);

			if (szFirst.FindOneOf("#") >= 0)
				continue;

			switch (nState)
			{
				case 0:		//read file version
				{
					nFileVersion = atoi(szInLine);

					if (nFileVersion < 10025)
					{
						nState = 99;
					}
					else
					{
						nState = 1;
					}

					break;
				}
				case 1: // read description name
				{
					int nBloog = szTemp.GetLength();
					int nBlarg = szDelimit.GetLength();

					if (szTemp == szDelimit)
					{
						break;
					}

					if (pMagicItemDescription == NULL)
					{
						pMagicItemDescription = new cDNDMagicItemDescription();
					}

					pMagicItemDescription->m_szMagicItemName = szTemp;
					pMagicItemDescription->m_szMagicItemName.MakeLower();

					pMagicItemDescription->m_szMagicItemName.Replace("\n", "");
					pMagicItemDescription->m_szMagicItemName.Replace("\r", "");

					nState = 2;

					break;
				}
				case 2: // parse out description
				{
					if (szTemp == szDelimit)
					{
						//this one is done
						m_MagicItemDescriptionMap.SetAt(pMagicItemDescription->m_szMagicItemName, pMagicItemDescription);
						
						//int nLen = pMagicItemDescription->m_szMagicItemName.GetLength();
						//cDNDMagicItemDescription *pPook = NULL;
						//m_MagicItemDescriptionMap.Lookup(pMagicItemDescription->m_szMagicItemName, pPook);

						pMagicItemDescription = NULL;
						nState = 1;
						break;
					}

					pMagicItemDescription->m_szDescription += szTemp;
				}
			}
		}

		fclose(pInfile);
	}
}

void CDMHelperApp::InitializeMonsters()
{
	CString szTemp = _T("");
	POSITION pos;
	WORD wID;

	for (pos = m_MonsterDescMap.GetStartPosition(); pos != NULL; )
	{
		CString *pString = NULL;
		m_MonsterDescMap.GetNextAssoc(pos,wID,pString);

		if(pString != NULL)
		{
			delete pString;
		}
	}
	m_MonsterDescMap.RemoveAll();

	for (pos = m_MonsterIntMap.GetStartPosition(); pos != NULL; )
	{
		CString *pString = NULL;
		m_MonsterIntMap.GetNextAssoc(pos,wID,pString);

		if(pString != NULL)
		{
			delete pString;
		}
	}
	m_MonsterIntMap.RemoveAll();

	for (pos = m_MagicResistanceMap.GetStartPosition(); pos != NULL; )
	{
		CString *pString = NULL;
		m_MagicResistanceMap.GetNextAssoc(pos,wID,pString);

		if(pString != NULL)
		{
			delete pString;
		}
	}
	m_MagicResistanceMap.RemoveAll();

	for (pos = m_MonsterManualIndexedMap.GetStartPosition(); pos != NULL; )
	{
		PDNDMONSTERMANUALENTRY pMonster = NULL;
		m_MonsterManualIndexedMap.GetNextAssoc(pos,wID,pMonster);

		if(pMonster != NULL)
		{
			delete pMonster;
		}
	}
	m_MonsterManualIndexedMap.RemoveAll();

	for (pos = m_CustomMonsterManualIndexedMap.GetStartPosition(); pos != NULL; )
	{
		PDNDMONSTERMANUALENTRY pMonster = NULL;
		m_CustomMonsterManualIndexedMap.GetNextAssoc(pos,wID,pMonster);

		if(pMonster != NULL)
		{
			delete pMonster;
		}
	}
	m_CustomMonsterManualIndexedMap.RemoveAll();
	m_MonsterManualOrderedArray.RemoveAll();
	m_PCMonsterManualIndexedMap.RemoveAll();

		
	szTemp.Format("%sData/tables/MonsterManual.dat", m_szEXEPath.GetBuffer(0));
	LoadMonsterManual(szTemp.GetBuffer(0), FALSE);

	szTemp.Format("%sData/tables/MonsterManual_II.dat", m_szEXEPath.GetBuffer(0));
	LoadMonsterManual(szTemp.GetBuffer(0), FALSE);

	szTemp.Format("%sData/tables/FiendFolio.dat", m_szEXEPath.GetBuffer(0));
	LoadMonsterManual(szTemp.GetBuffer(0), FALSE);

	szTemp.Format("%sData/tables/CustomMonsters.dat", m_szEXEPath.GetBuffer(0));
	LoadMonsterManual(szTemp.GetBuffer(0), TRUE);
}

void CDMHelperApp::PrescanMonsterManual(char *path, char *path2)
{
	FILE *pInfile = fopen(path, "rt");
	FILE *pOutfile = fopen(path2, "wt");

	char szInLine[512];
	int nCount = 0;
	int nProcessed = 0;

	CString szName;
	CString szData[50];

	if(pInfile != NULL)
	{
		while (!feof(pInfile))
		{
			BOOL bProcess = FALSE;
			fgets(szInLine, 512, pInfile);
			CString szTemp = szInLine;
			
			if(szTemp.FindOneOf("#") >= 0)
				continue;

			CString sToken=_T("");
			int i = 0; // substring index to extract
			while (AfxExtractSubString(sToken, szTemp, i,'|'))
			{   
				sToken.Replace("	", "");	//remove tabs

				switch(i)
				{
					case 0: szName = sToken; break;
					case 11:
					{
						if(sToken == "LG")
							szData[i] = "1";
						else if(sToken == "CG")
							szData[i] = "3";
						else if(sToken == "N")
							szData[i] = "5";
						else if(sToken == "LE")
							szData[i] = "7";
						else if(sToken == "CE")
							szData[i] = "9";

						else if(sToken == "C")
							szData[i] = "6";
						else if(sToken == "E")
							szData[i] = "8";
						else if(sToken == "LG(N)")
							szData[i] = "1";
						else if(sToken == "N(G)")
							szData[i] = "5";
						else if(sToken == "CG(N)")
							szData[i] = "3";
						else if(sToken == "NG/CG")
							szData[i] = "2";
						else if(sToken == "N(LE)")
							szData[i] = "7";
						else if(sToken == "N(E)")
							szData[i] = "8";
						else if(sToken == "N(CE)")
							szData[i] = "9";
						else if(sToken == "N(G or E)")
							szData[i] = "5";
						else if(sToken == "N to LG")
							szData[i] = "0";
						else if(sToken == "Special")
							szData[i] = "0";			
						else
						{
							TRACE("WTF IS %s\n", sToken.GetBuffer(0));
						}

						break;
					}
					case 28:
					{
						if(nCount == 1273)
						{
							TRACE("ORC ?\n");
						}

						if(sToken == "Monster Manual (First Edition)")
						{
							TRACE("GOT %s %s\n", szName.GetBuffer(0), sToken.GetBuffer(0));
							bProcess = TRUE;
						}
						break;
					}
					case 31:
					{
						if(bProcess)
						{
							TRACE("WTF IS %s\n", szName);
						}
						break;
					}
					default:
					{
						szData[i] = sToken;
						break;
					}

				}
				i++;
			}

			++nCount;

			if(bProcess)
			{
				fprintf(pOutfile, "%04d | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s | %s\n",
					nProcessed + DND_CHARACTER_FIRST_MONSTER,
					szName.GetBuffer(0),
					szData[1].GetBuffer(0),
					szData[4].GetBuffer(0),
					szData[8].GetBuffer(0),
					szData[9].GetBuffer(0),
					szData[10].GetBuffer(0),
					szData[11].GetBuffer(0),
					szData[12].GetBuffer(0),
					szData[13].GetBuffer(0),
					szData[14].GetBuffer(0),
					szData[15].GetBuffer(0),
					szData[17].GetBuffer(0),
					szData[18].GetBuffer(0),
					szData[19].GetBuffer(0),
					szData[20].GetBuffer(0),
					szData[21].GetBuffer(0),
					szData[22].GetBuffer(0),
					szData[24].GetBuffer(0),
					szData[25].GetBuffer(0));

				++nProcessed;
			}

		}

		fclose(pInfile);
		fclose(pOutfile);
	}
}

void CDMHelperApp::LoadLanguages(char *path)
{
	FILE *pInfile = fopen(path, "rt");

	char szInLine[512];
	int nCount = 0;
	int nIndex = 1;

	cDNDLanguage *pLanguage = new cDNDLanguage();

	pLanguage->m_szLanguageName = _T("None");

	m_LanguageArray.InsertAt(nCount, pLanguage);
	++nCount;

	if(pInfile != NULL)
	{
		while (!feof(pInfile))
		{
			BOOL bProcess = FALSE;
			fgets(szInLine, 512, pInfile);
			CString szTemp = szInLine;
			
			if(szTemp.FindOneOf("#") >= 0)
				continue;

			szTemp.Replace(" | ", "|");	//remove extra spaces
			szTemp.Replace(" |", "|");	//remove extra spaces

			szTemp.Replace("\n", "");	//remove line feed
			szTemp.Replace("\r", "");	//remove carriage return

			CString sToken=_T("");
			int i = 0; // substring index to extract
			while (AfxExtractSubString(sToken, szTemp, i,'|'))
			{   
				sToken.Replace("	", "");	//remove tabs

				switch(i)
				{
					case 0:
					{
						pLanguage = new cDNDLanguage();

						CString sSubToken=_T("");
						int j = 0; // substring index to extract
						while (AfxExtractSubString(sSubToken, sToken, j,'-'))
						{   
							switch(j)
							{
								case 0:	pLanguage->m_nDieRollLow = atoi(sSubToken.GetBuffer(0)); pLanguage->m_nDieRollHigh = pLanguage->m_nDieRollLow; break;
								case 1:	pLanguage->m_nDieRollHigh = atoi(sSubToken.GetBuffer(0)); break;
							}
							++j;
						}

						break;
					}
					case 1:
					{
						pLanguage->m_szLanguageName = sToken;
						break;
					}
					case 2:
					{
						pLanguage->m_nList = atoi(sToken.GetBuffer(0));

						pLanguage->m_nIndex = nIndex;
						m_nLastLanguageIndex = nIndex;
						++nIndex;
						
						m_LanguageArray.InsertAt(nCount, pLanguage);

						++nCount;

						break;
					}
				}

				i++;
			}

		}

		fclose(pInfile);
	}
}

void CDMHelperApp::AddMonsterDescToMap(CString szInitialDesc)
{
	CString szDesc = szInitialDesc;

	szDesc.Replace("\n", "");
	szDesc.Replace("\r", "");

	szDesc.TrimRight(" ");
	szDesc.TrimLeft(" ");

	for (POSITION pos = m_MonsterDescMap.GetStartPosition(); pos != NULL; )
	{
		WORD wID;
		CString *pCompare = NULL;
		m_MonsterDescMap.GetNextAssoc(pos,wID,pCompare);

		if(pCompare != NULL)
		{
			if(*pCompare == szDesc)
			{ 
				return;
			}
		}
	}

	CString *pString = new CString();
	int nCount = m_MonsterDescMap.GetCount();

	*pString = szDesc;

	m_MonsterDescMap.SetAt(nCount, pString);	
}

	
void CDMHelperApp::AddMonsterIntToMap(CString szInitialInt)
{
	CString szInt = szInitialInt;

	szInt.Replace("\n", "");
	szInt.Replace("\r", "");

	szInt.TrimRight(" ");
	szInt.TrimLeft(" ");

	for (POSITION pos = m_MonsterIntMap.GetStartPosition(); pos != NULL; )
	{
		WORD wID;
		CString *pCompare = NULL;
		m_MonsterIntMap.GetNextAssoc(pos,wID,pCompare);

		if(pCompare != NULL)
		{
			if(*pCompare == szInt)
			{ 
				return;
			}
		}
	}

	CString *pString = new CString();
	int nCount = m_MonsterIntMap.GetCount();

	*pString = szInt;

	m_MonsterIntMap.SetAt(nCount, pString);	
}

void CDMHelperApp::AddMonsterMagicResistanceToMap(CString szInitialMR)
{
	CString szMR = szInitialMR;

	szMR.Replace("\n", "");
	szMR.Replace("\r", "");

	szMR.TrimRight(" ");
	szMR.TrimLeft(" ");

	for (POSITION pos = m_MagicResistanceMap.GetStartPosition(); pos != NULL; )
	{
		WORD wID;
		CString *pCompare = NULL;
		m_MagicResistanceMap.GetNextAssoc(pos,wID,pCompare);

		if(pCompare != NULL)
		{
			if(*pCompare == szMR)
			{ 
				return;
			}
		}
	}

	CString *pString = new CString();
	int nCount = m_MagicResistanceMap.GetCount();

	*pString = szMR;

	m_MagicResistanceMap.SetAt(nCount, pString);	
}



/*

0		Name,            	 Orc,
1		Type,            	 Humanoid,
2		Climate,         	 Any,
3		Terrain,         	 N/A,
4		Frequency,       	 Common,
5		Organization,    	 N/A,
6		Activity Cycle,  	 N/A,
7		Diet,            	 N/A,
8		Intelligence,    	 Low to Average (5-10),
9		% in Lair,       	 0.35,
10		Treasure,        	 "L (C,O,Qx10,S)",
11		Alignment,       	 LE,
12		No. Appearing,   	 30-300,
13		AC,              	 6,
14		Movement,        	 9,
15		HD,              	 1,
16		Thac0,           	 N/A,
17		No. Attacks,     	 1,
18		Damage,          	 1-8 or by weapon,
19		Special Attack,  	 Nil,
20		Special Defence, 	 Nil,
21		MR,              	 Standard,
22		Size,            	 M (6'+),
23		Morale,          	 N/A,
24		Psionic Ability, 	 Nil,
25		A/D Modes,       	 Nil,
26		Level,           	 N/A,
27		XP,              	 N/A,
28		Source,          	 Monster Manual (First Edition),
29		Edition          	 1e
*/

/*

0		Name,            	 Orc,
1		Type,            	 Humanoid,
4		Frequency,       	 Common,
8		Intelligence,    	 Low to Average (5-10),
9		% in Lair,       	 0.35,
10		Treasure,        	 "L (C,O,Qx10,S)",
11		Alignment,       	 LE,
12		No. Appearing,   	 30-300,
13		AC,              	 6,
14		Movement,        	 9,
15		HD,              	 1,
16		No. Attacks,     	 1,
17		Damage,          	 1-8 or by weapon,
18		Special Attack,  	 Nil,
19		Special Defence, 	 Nil,
20		MR,              	 Standard,
21		Size,            	 M (6'+),
22		Psionic Ability, 	 Nil,
23		A/D Modes,       	 Nil,

# Name | Type | Frequency |  Intelligence | % in Lair | Treasure | Alignment | No. Appearing | AC | Movement |  HD | No. Attacks | Damage | Special Attack | Special Defense | Magic Resist | Size | Psionic Ability | A/D Modes | 

*/

void CDMHelperApp::LoadMonsterManual(char *path, BOOL bCustom)
{
	#if 0
	#ifdef _DEBUG

	CString szPath;

	szPath.Format("%sData/tables/NUMBERS.CSV", m_szEXEPath.GetBuffer(0));

	FILE *pOutFile = fopen(szPath.GetBuffer(0), "wt");

	for(int i=3000; i <= 3350; ++i)
	{
		fprintf(pOutFile, "%d,%d\n", i, i);
	}

	fclose(pOutFile);

	#endif
	#endif

	FILE *pInfile = fopen(path, "rt");

	char szInLine[512];
	int nCount = m_nTotalMonsterCount;

	if(pInfile != NULL)
	{
		while (!feof(pInfile))
		{
			BOOL bProcess = FALSE;
			fgets(szInLine, 512, pInfile);
			CString szTemp = szInLine;
			
			if(szTemp.FindOneOf("#") >= 0)
				continue;

			cDNDMonsterManualEntry *pMonster = new cDNDMonsterManualEntry();

			szTemp.Replace(" | ", "|");	//remove extra spaces
			szTemp.Replace(" |", "|");	//remove extra spaces

			szTemp.Replace("\n", "");	//remove line feed
			szTemp.Replace("\r", "");	//remove carriage return

			CString sToken=_T("");
			int i = 0; // substring index to extract
			while (AfxExtractSubString(sToken, szTemp, i,'|'))
			{   
				sToken.Replace("	", "");	//remove tabs

				switch(i)
				{
					case 0:  	pMonster->m_nMonsterIndex		= atoi(sToken.GetBuffer(0)); break;
					case 1:  	pMonster->m_szMMName			= sToken; break;
					case 2:  	pMonster->m_szType        		= sToken; break; 
					case 3:  	pMonster->m_szFrequency			= sToken; break; 
					case 4:  	pMonster->m_szIntelligence		= sToken; break; 
					case 5:  	pMonster->m_szPercentinLair		= sToken; break; 
					case 6:  	pMonster->m_szTreasureType		= sToken; break; 
					case 7:  	pMonster->m_nAlignment			= (DND_CHARACTER_ALIGNMENTS)atoi(sToken.GetBuffer(0)); break;     	
					case 8:  	pMonster->m_szNumAppearing		= sToken; break; 
					case 9:
					{
						if(sToken == _T("Special"))  //assuming this means "by armor type"
						{
							pMonster->m_nBaseAC = 10;
						}
						else if(sToken == _T("N/A"))  
						{
							pMonster->m_nBaseAC = 10;
						}
						else
						{
							pMonster->m_nBaseAC	= atoi(sToken.GetBuffer(0)); 
						}

						/*
						if(sToken != _T("0") && pMonster->m_nBaseAC == 0)
						{
							TRACE("crap\n");
						}
						*/

						break; 
					}
					case 10: 	pMonster->m_szMovement			= sToken; break; 
					case 11: 	pMonster->m_szHD				= sToken; break;  
					case 12: 	pMonster->m_szNumAttacks		= sToken; break; 
					case 13: 	pMonster->m_szDamage			= sToken; break; 
					case 14: 	pMonster->m_szSpecialAttack		= sToken; break; 
					case 15: 	pMonster->m_szSpecialDefense	= sToken; break; 
					case 16: 	pMonster->m_szMagicResist		= sToken; break; 
					case 17: 	pMonster->m_szSize				= sToken; break; 
					case 18: 	pMonster->m_szPsionicAbility	= sToken; break; 
					case 19: 	pMonster->m_szAttDefModes		= sToken; break; 
					case 20: 	pMonster->m_szXP				= sToken; break; 

					case 21:	 // default armor
					{
						if(sToken.Find("Nil",0) < 0)
						{
							pMonster->m_szArmors = sToken;
						}

						break; 
					}
					case 22:	// default weapon
					{
						if(sToken.Find("Nil",0) < 0)
						{
							pMonster->m_szWeapons = sToken;
						}

						break;  
					}

					default:
					{
						break;
					}

				}
				i++;
			}

			if(pMonster->m_nMonsterIndex == 0)
			{
				TRACE("FAWK!\n");
				delete pMonster;
				continue;
			}

			pMonster->m_nMonsterID = nCount;

			m_MonsterManualOrderedArray.InsertAt(pMonster->m_nMonsterID, pMonster);
			m_MonsterManualIndexedMap.SetAt(pMonster->m_nMonsterIndex, pMonster);

			AddMonsterDescToMap(pMonster->m_szType);
			AddMonsterIntToMap(pMonster->m_szIntelligence);
			AddMonsterMagicResistanceToMap(pMonster->m_szMagicResist);

			if(bCustom)
			{
				cDNDMonsterManualEntry *pCustomMonster = new cDNDMonsterManualEntry();
				pCustomMonster->CopyFrom(pMonster);
				m_CustomMonsterManualIndexedMap.SetAt(pCustomMonster->m_nMonsterIndex, pCustomMonster);
			}

			if(pMonster->m_szMMName == "Elf")
			{
				m_PCMonsterManualIndexedMap.SetAt(DND_CHARACTER_RACE_ELF, pMonster);
			}
			else if(pMonster->m_szMMName == "Dwarf")
			{
				m_PCMonsterManualIndexedMap.SetAt(DND_CHARACTER_RACE_DWARF, pMonster);
			}
			else if(pMonster->m_szMMName == "Halfling")
			{
				m_PCMonsterManualIndexedMap.SetAt(DND_CHARACTER_RACE_HALFLING, pMonster);
			}
			else if(pMonster->m_szMMName == "Gnome")
			{
				m_PCMonsterManualIndexedMap.SetAt(DND_CHARACTER_RACE_GNOME, pMonster);
			}
		
			++nCount;
			++m_nTotalMonsterCount;

		}

		fclose(pInfile);
	}
}

cDNDMonsterManualEntry * CDMHelperApp::GetMonsterManualEntryFromName(CString szMonsterName)
{

	if(szMonsterName == "Cleric"
		|| szMonsterName == "Druid"
		|| szMonsterName == "Magic-User"
		|| szMonsterName == "Illusionist"
		|| szMonsterName == "Fighter"
		|| szMonsterName == "Ranger"
		|| szMonsterName == "Paladin"
		|| szMonsterName == "Thief"
		|| szMonsterName == "Assassin"
		|| szMonsterName == "Monk")
	{
		cDNDMonsterManualEntry *pMonster = m_MonsterManualOrderedArray.GetAt(0);
		return pMonster;
	}

	if(szMonsterName == "Hydra - Pyro")
	{
		szMonsterName = "Hydra";
	}

	if(szMonsterName == "Mezzodaemon")
	{
		szMonsterName = "Demon - Type III - Glabrezu";
	}
	if(szMonsterName == "Nycadaemon")
	{
		szMonsterName = "Demon - Type III - Glabrezu";
	}

	if(szMonsterName == "Titan - Lesser" 
	   || szMonsterName == "Titan - Minor"
	   || szMonsterName == "Titan - Elder")
	{
		szMonsterName = "Titan";
	}

	if(szMonsterName == "REROLL")
	{
		szMonsterName = "Demon - Type III - Glabrezu";
	}


	for(int i = 0; i < m_MonsterManualOrderedArray.GetCount(); ++i)
	{
		cDNDMonsterManualEntry *pMonster = m_MonsterManualOrderedArray.GetAt(i);

		if(pMonster == NULL)
			continue;

		if(pMonster->m_szMMName == szMonsterName)
		{
			return pMonster;
		}
	}

	return NULL;
}


void CDMHelperApp::LoadRandomEncounterIndexTable(char *path)
{
	FILE *pInfile = fopen(path, "rt");

	char szInLine[512];
	int nDieRoll = 0;

	if(pInfile != NULL)
	{
		while (!feof(pInfile))
		{
			BOOL bProcess = FALSE;
			fgets(szInLine, 512, pInfile);
			CString szTemp = szInLine;
			
			if(szTemp.FindOneOf("#") >= 0)
				continue;

			szTemp.Replace(" | ", "|");	//remove extra spaces
			szTemp.Replace(" |", "|");	//remove extra spaces

			szTemp.Replace("\n", "");	//remove line feed
			szTemp.Replace("\r", "");	//remove carriage return

			CString sToken=_T("");
			int i = 0; // substring index to extract
			while (AfxExtractSubString(sToken, szTemp, i,'|'))
			{   
				sToken.Replace("	", "");	//remove tabs

				switch(i)
				{
					case 0:
					{
						nDieRoll = atoi(sToken.GetBuffer(0));

						break;
					}
					default:
					{
						int nCol = (i-1)%16;
						int nRow = (nDieRoll-1)%20;

						m_pRandomEncounterIndex[nCol][nRow] = new cDNDRandomEncounterIndex();

						m_pRandomEncounterIndex[nCol][nRow]->m_nLevel = i%16;
						m_pRandomEncounterIndex[nCol][nRow]->m_nDieRoll = nDieRoll; // 1 to 20 or boom 
						m_pRandomEncounterIndex[nCol][nRow]->m_szTableName = sToken;

						break;
					}
				}

				i++;
			}

		}

		fclose(pInfile);
	}
}


void CDMHelperApp::LoadRandomEncounterTables(char *path)
{
	FILE *pInfile = fopen(path, "rt");

	char szInLine[512];
	int nDieRoll = 0;

	//m_RandomEncounterTables
	cDNDRandomEncounterTable *pTable = NULL;
	cDNDRandomEncounterTableEntry *pEntry = NULL;

	int nTable = -1;
	int nLastTable = -1;

	if(pInfile != NULL)
	{
		int nTableEntry = 0;

		while (!feof(pInfile))
		{			
			fgets(szInLine, 512, pInfile);
			CString szTemp = szInLine;
			
			if(szTemp.FindOneOf("#") >= 0)
				continue;

			szTemp.Replace(" | ", "|");	//remove extra spaces
			szTemp.Replace(" |", "|");	//remove extra spaces

			szTemp.Replace("\n", "");	//remove line feed
			szTemp.Replace("\r", "");	//remove carriage return

			

			CString sToken=_T("");
			int i = 0; // substring index to extract
			while (AfxExtractSubString(sToken, szTemp, i,'|'))
			{   
				sToken.Replace("	", "");	//remove tabs

				switch(i)
				{
					case 0:
					{
						nTable = atoi(sToken.GetBuffer(0));

						if(nTable != nLastTable)
						{
							if(pTable != NULL)
							{
								pTable->m_nEntries = nTableEntry;
								m_RandomEncounterTables.Add(pTable);
							}

							nLastTable = nTable;
							pTable = new cDNDRandomEncounterTable();

							nTableEntry = 0;
						}

						break;
					}

					case 1:
					{
						pTable->m_nLevel = atoi(sToken.GetBuffer(0));

						break;
					}
					case 2:
					{
						pTable->m_szTableName = sToken;

						break;
					}

					case 3: //new monster, die roll low/high
					{
						pEntry = new cDNDRandomEncounterTableEntry();

						CString sSubToken=_T("");
						int j = 0; // substring index to extract
						while (AfxExtractSubString(sSubToken, sToken, j,'-'))
						{   
							switch(j)
							{
								case 0:	pEntry->m_nDieRollLow[0] = atoi(sSubToken.GetBuffer(0)); break;
								case 1:	pEntry->m_nDieRollHigh[0] = atoi(sSubToken.GetBuffer(0)); break;
							}
							++j;
						}

						if(pEntry->m_nDieRollLow[0])
						{
							pTable->m_bValidColumn[0] = TRUE;
							if(pEntry->m_nDieRollHigh[0] == 0)
							{
								pEntry->m_nDieRollHigh[0] = pEntry->m_nDieRollLow[0];
							}
						}
					
						break;
					}

					case 4:	// die roll low/high for terrain type columns
					case 5:
					case 6:
					case 7:
					case 8:
					case 9:
					case 10:
					{
						CString sSubToken=_T("");
						int j = 0; // substring index to extract
						while (AfxExtractSubString(sSubToken, sToken, j,'-'))
						{   
							switch(j)
							{
								case 0:	pEntry->m_nDieRollLow[i-3] = atoi(sSubToken.GetBuffer(0)); break;
								case 1:	pEntry->m_nDieRollHigh[i-3] = atoi(sSubToken.GetBuffer(0)); break;
							}
							++j;
						}

						if(pEntry->m_nDieRollLow[i-3])
						{
							pTable->m_bValidColumn[i-3] = TRUE;
							if(pEntry->m_nDieRollHigh[i-3] == 0)
							{
								pEntry->m_nDieRollHigh[i-3] = pEntry->m_nDieRollLow[i-3];
							}
						}

						break;
					}

					case 11:
					{
						sToken.Replace("–", "-"); //spreadsheet weirdness

						pEntry->m_szMonsterName = sToken;

						if(pEntry->m_szMonsterName == _T(""))
						{
							TRACE("INIT READ FUBAR\n");
						}

						break;
					}

					case 12:
					{
						CString sSubToken=_T("");
						int j = 0; // substring index to extract
						while (AfxExtractSubString(sSubToken, sToken, j,'-'))
						{   
							switch(j)
							{
								case 0:	pEntry->m_nNumAppearingLow = atoi(sSubToken.GetBuffer(0)); pEntry->m_nNumAppearingHigh = pEntry->m_nNumAppearingLow; break;
								case 1:	pEntry->m_nNumAppearingHigh = atoi(sSubToken.GetBuffer(0)); break;
							}
							++j;
						}

						break;
					}

					case 13:
					{
						pEntry->m_szData = sToken;

						pTable->m_EncounterArray.Add(pEntry);

						++nTableEntry;

						break;
					}

					default:
					{
						break;
					}
				}

				i++;
			}

		}

		if(pTable != NULL) //add the last one we read
		{
			pTable->m_nEntries = nTableEntry;
			m_RandomEncounterTables.Add(pTable);
		}

		fclose(pInfile);
	}

	// validation and interlinking

	for(int i = 0; i < m_RandomEncounterTables.GetCount(); ++ i)
	{
		pTable = m_RandomEncounterTables.GetAt(i);

		for(int j = 0; j < pTable->m_EncounterArray.GetCount(); ++ j)
		{
			pEntry =  pTable->m_EncounterArray.GetAt(j);

			//subtable check
			for(int k = 0; k < m_RandomEncounterTables.GetCount(); ++ k)
			{
				cDNDRandomEncounterTable *pSubTable =  m_RandomEncounterTables.GetAt(k);

				if(pEntry != NULL && pEntry->m_szMonsterName == pSubTable->m_szTableName)
				{
					pEntry->m_pSubTable = pSubTable;
					break;
				}
			}

			if(pEntry->m_pSubTable == NULL)
			{
				pEntry->m_pMonsterManualEntry = GetMonsterManualEntryFromName(pEntry->m_szMonsterName);

				if(pEntry->m_pMonsterManualEntry == NULL && pEntry->m_szMonsterName.Find(" or ") >= 0)
				{
					int nDieRoll = RollD6();

					#if 0
					#ifdef _DEBUG
					nDieRoll = 6;
					#endif
					#endif

					TRACE("SPLIT NAME %s\n", pEntry->m_szMonsterName);

					pEntry->m_szMonsterName.Replace(" or ", "|");

					CString sToken=_T("");
					int i = 0; // substring index to extract
					while (AfxExtractSubString(sToken, pEntry->m_szMonsterName, i,'|') && pEntry->m_pMonsterManualEntry == NULL)
					{ 
						if(nDieRoll <= 3 || i != 0)
						{
							pEntry->m_pMonsterManualEntry = GetMonsterManualEntryFromName(sToken);
						}

						++i;
					}
				}

				if(pEntry->m_pMonsterManualEntry == NULL)
				{
					//TRACE("ERROR !\n");
					CString szError;
					szError.Format("ERROR: Encounter Table/Monster Manual Mismatch (%s) not found !", pEntry->m_szMonsterName);
					AfxMessageBox(szError, MB_OK);
				}
				else
				{
					//TRACE("FOUND %s\n", pEntry->m_pMonsterManualEntry->m_szMMName);
				}
			}
		}

	}

}

DND_CHARACTER_RACES CDMHelperApp::GetCharacterRaceFromName(char *szRaceName)
{
	CString szTemp = szRaceName;
	
	return GetCharacterRaceFromName(szTemp);
}

DND_CHARACTER_RACES CDMHelperApp::GetCharacterRaceFromName(CString szRaceName)
{

	if(szRaceName.Left(5) == _T("Human")) return DND_CHARACTER_RACE_HUMAN;
	else if(szRaceName == _T("Elven")) return DND_CHARACTER_RACE_ELF;
	else if(szRaceName == _T("Elf")) return DND_CHARACTER_RACE_ELF;
	else if(szRaceName == _T("Half Elven")) return DND_CHARACTER_RACE_HALF_ELF;
	else if(szRaceName == _T("Half Elf")) return DND_CHARACTER_RACE_HALF_ELF;
	else if(szRaceName == _T("Dwarven")) return DND_CHARACTER_RACE_DWARF;
	else if(szRaceName == _T("Dwarf")) return DND_CHARACTER_RACE_DWARF;
	else if(szRaceName == _T("Halfling")) return DND_CHARACTER_RACE_HALFLING;
	else if(szRaceName == _T("Gnome")) return DND_CHARACTER_RACE_GNOME;
	else if(szRaceName == _T("Half Orc")) return DND_CHARACTER_RACE_HALF_ORC;
	else if(szRaceName == _T("Drow")) return DND_CHARACTER_RACE_DROW;

	PDNDMONSTERMANUALENTRY pMonsterManualEntry = GetMonsterManualEntryFromName(szRaceName);

	if(pMonsterManualEntry != NULL)
	{
		return (DND_CHARACTER_RACES)pMonsterManualEntry->m_nMonsterIndex;
	}

	return DND_CHARACTER_RACES_UNDEF;
}

void CDMHelperApp::LoadRandomNames(char *path)
{
	FILE *pInfile = fopen(path, "rt");

	char szInLine[512];

	CString szNameDataFileName = _T("");
	DND_CHARACTER_RACES nCurrentRace = DND_CHARACTER_RACES_UNDEF;

	if(pInfile != NULL)
	{
		while (!feof(pInfile))
		{
			fgets(szInLine, 512, pInfile);
			CString szTemp = szInLine;
			
			if(szTemp.FindOneOf("#") >= 0)
				continue;
			
			szTemp.Replace("\n", "");	//remove line feed
			szTemp.Replace("\r", "");	//remove carriage return

			szNameDataFileName = _T("");
			nCurrentRace = DND_CHARACTER_RACES_UNDEF;

			CString sToken=_T("");
			int i = 0; // substring index to extract
			while (AfxExtractSubString(sToken, szTemp, i,'|'))
			{   
				switch(i)
				{
					case 0:
					{
						szNameDataFileName = sToken;

						break;
					}
					default:
					{

						if(i%2 == 1) // this is the RACE
						{
							TRACE("PROCESS RACE %s\n", sToken);
							nCurrentRace = GetCharacterRaceFromName(sToken);
						}
						else // this is the GENDER
						{
							if(nCurrentRace != DND_CHARACTER_RACES_UNDEF)
							{
								sToken.MakeUpper();
								int nGender = 0;
								if(sToken == _T("FEMALE"))
								{
									nGender = 1;
								}

								LoadRandomNameDataTable(szNameDataFileName, nCurrentRace, nGender);
							}
						}

						break;
					}
				}

				i++;
			}

		}

		fclose(pInfile);
	}
}

void CDMHelperApp::LoadRandomNameDataTable(CString szNameDataFileName, DND_CHARACTER_RACES nCurrentRace, int nGender)
{
	CString szPath;
	szPath.Format("%sData/tables/names/%s", m_szEXEPath.GetBuffer(0), szNameDataFileName.GetBuffer(0));

	FILE *pInfile = fopen(szPath.GetBuffer(0), "rt");

	if(pInfile != NULL)
	{
		char szInLine[512];

		PDNDRANDOMNAMETABLE pNameTable = NULL;
		m_RandomNameMap.Lookup(nCurrentRace, pNameTable);

		if(pNameTable == NULL)
		{
			pNameTable = new cDNDRandomNameTable(nCurrentRace);
			m_RandomNameMap.SetAt(nCurrentRace, pNameTable);
		}

		PDNDRANDOMNAMEDATATABLE pNameDataTable = NULL;

		int nSize = pNameTable->m_NameDataArray[nGender].GetSize();
		if(pNameTable->m_nNameDataSize[nGender] < nSize)
		{
			pNameDataTable = pNameTable->m_NameDataArray[nGender].GetAt(pNameTable->m_nNameDataSize[nGender]);
		}

		if(pNameDataTable == NULL)
		{
			pNameDataTable = new cDNDRandomNameDataTable();
			pNameTable->m_NameDataArray[nGender].InsertAt(pNameTable->m_nNameDataSize[nGender], pNameDataTable);
			++pNameTable->m_nNameDataSize[nGender];
		}

		int nNameHalf = 0;
		while (!feof(pInfile))
		{
			fgets(szInLine, 512, pInfile);
			CString szTemp = szInLine;
			
			if(szTemp.FindOneOf("#") >= 0)
			{
				continue;
			}
			if(szTemp.FindOneOf("|") >= 0)
			{
				++nNameHalf;
				continue;
			}
			
			szTemp.Replace("\n", "");	//remove line feed
			szTemp.Replace("\r", "");	//remove carriage return

			if(szTemp.GetLength() == 0)
			{
				continue;
			}

			switch(nNameHalf)
			{
				case 0:
				{
					pNameDataTable->m_szFirstNameFirstSyllable[pNameDataTable->m_nSyllables[0]] = szTemp;
					pNameDataTable->m_nSyllables[0] = (pNameDataTable->m_nSyllables[0]+1) % MAX_RANDOM_NAME_TABLE_ENTRIES;
					break;
				}
				case 1:
				{
					pNameDataTable->m_szFirstNameLastSyllable[pNameDataTable->m_nSyllables[1]] = szTemp;
					pNameDataTable->m_nSyllables[1] = (pNameDataTable->m_nSyllables[1]+1) % MAX_RANDOM_NAME_TABLE_ENTRIES;
					break;
				}
				case 2:
				{
					pNameDataTable->m_szLastNameFirstSyllable[pNameDataTable->m_nSyllables[2]] = szTemp;
					pNameDataTable->m_nSyllables[2] = (pNameDataTable->m_nSyllables[2]+1) % MAX_RANDOM_NAME_TABLE_ENTRIES;
					break;
				}
				case 3:
				{
					pNameDataTable->m_szLastNameLastSyllable[pNameDataTable->m_nSyllables[3]] = szTemp;
					pNameDataTable->m_nSyllables[3] = (pNameDataTable->m_nSyllables[3]+1) % MAX_RANDOM_NAME_TABLE_ENTRIES;
					break;
				}
				case 4:
				{
					pNameDataTable->m_szFinishNameSyllable[pNameDataTable->m_nSyllables[4]] = szTemp;
					pNameDataTable->m_nSyllables[4] = (pNameDataTable->m_nSyllables[4]+1) % MAX_RANDOM_NAME_TABLE_ENTRIES;
					break;
				}
			}
			
		}

		fclose(pInfile);
	}
}

CString CDMHelperApp::Capitalize(CString szInString)
{
	CString szRetVal =  _T("");

	int nLength = szInString.GetLength();

	if(nLength < 1 )
	{
		return szRetVal;
	}

	CString szLetter = szInString.Left(1);
	szLetter.MakeUpper();

	CString szRemainder = szInString.Right(nLength-1);

	szRetVal = szLetter;
	szRetVal += szRemainder;

	return szRetVal;

}

CString CDMHelperApp::GetRandomName(DND_CHARACTER_RACES nRace, int nGender)
{
	CString szRetVal = _T("");

	if(nGender < 0 || nGender > 1)
	{
		return szRetVal;
	}

	//nRace = DND_CHARACTER_RACE_GNOME;

	PDNDRANDOMNAMETABLE pNameTable = NULL;
	m_RandomNameMap.Lookup(nRace, pNameTable);

	if(pNameTable != NULL)
	{
		if(pNameTable->m_nNameDataSize[nGender] > 0)
		{
			int nPick = rand()%pNameTable->m_nNameDataSize[nGender];

			PDNDRANDOMNAMEDATATABLE pNameDataTable = pNameTable->m_NameDataArray[nGender].GetAt(nPick);

			if(pNameDataTable != NULL && pNameDataTable->m_nSyllables[0] > 0)
			{
				szRetVal = pNameDataTable->m_szFirstNameFirstSyllable[ rand()%pNameDataTable->m_nSyllables[0] ];

				if(pNameDataTable->m_nSyllables[1] > 0)
				{
					szRetVal += pNameDataTable->m_szFirstNameLastSyllable[ rand()%pNameDataTable->m_nSyllables[1] ];
				}

				if(pNameDataTable->m_nSyllables[2] > 0)
				{
					szRetVal += _T(" ");

					CString szTemp = Capitalize(pNameDataTable->m_szLastNameFirstSyllable[ rand()%pNameDataTable->m_nSyllables[2] ]);
					szRetVal += szTemp; 
				}

				if(pNameDataTable->m_nSyllables[3] > 0)
				{
					szRetVal += pNameDataTable->m_szLastNameLastSyllable[ rand()%pNameDataTable->m_nSyllables[3] ];
				}
				if(pNameDataTable->m_nSyllables[4] > 0)
				{
					szRetVal += pNameDataTable->m_szFinishNameSyllable[ rand()%pNameDataTable->m_nSyllables[4] ];
				}

			}
			
		}
	}

	szRetVal.Replace(" _", "");  //for oddballs that don't have a firstname/lastname structure

	return szRetVal;
}

void CDMHelperApp::CleanUpObjectTables()
{
	int i;
	POSITION pos;
	WORD wID;

	for (i = 0; i < m_WeaponsOrderedTypeArray.GetSize(); ++i)
	{
		PWEAPONTYPE pWeapon = (PWEAPONTYPE)m_WeaponsOrderedTypeArray.GetAt(i);

		if (pWeapon != NULL)
		{
			delete pWeapon;
		}
	}
	m_WeaponsOrderedTypeArray.RemoveAll();
	m_WeaponsIndexedTypeArray.RemoveAll();
	m_CustomWeaponsOrderedTypeArray.RemoveAll();

	for (i = 0; i < m_AmmunitionOrderedTypeArray.GetSize(); ++i)
	{
		PWEAPONTYPE pWeapon = (PWEAPONTYPE)m_AmmunitionOrderedTypeArray.GetAt(i);

		if (pWeapon != NULL)
		{
			delete pWeapon;
		}
	}
	m_AmmunitionOrderedTypeArray.RemoveAll();
	m_AmmunitionIndexedTypeArray.RemoveAll();

	for (i = 0; i < m_EquipmentOrderedTypeArray.GetSize(); ++i)
	{
		cDNDObject *pObject = (cDNDObject *)m_EquipmentOrderedTypeArray.GetAt(i);

		if (pObject != NULL)
		{
			delete pObject;
		}
	}
	m_EquipmentOrderedTypeArray.RemoveAll();
	m_EquipmentIndexedTypeArray.RemoveAll();

	for (i = 0; i < m_HelmetsOrderedTypeArray.GetSize(); ++i)
	{
		cDNDObject *pHelmet = (cDNDObject *)m_HelmetsOrderedTypeArray.GetAt(i);

		if (pHelmet != NULL)
		{
			delete pHelmet;
		}
	}
	m_HelmetsOrderedTypeArray.RemoveAll();
	m_HelmetsIndexedTypeArray.RemoveAll();

	//kill the clones !
	for (i = 0; i < m_ObjectsOrderedTypeArray.GetSize() - 1; ++i)
	{
		cDNDObject *pObject = m_ObjectsOrderedTypeArray.GetAt(i);
		delete pObject;
	}
	m_ObjectsOrderedTypeArray.RemoveAll();
	m_ObjectsIndexedTypeArray.RemoveAll();
	m_CustomObjectsOrderedTypeArray.RemoveAll();

	for (i = 0; i < m_MagicTableItemOrderedTypeArray.GetSize(); ++i)
	{
		PMAGICTABLEITEM pMagicItem = m_MagicTableItemOrderedTypeArray.GetAt(i);

		if (pMagicItem != NULL)
		{
			delete pMagicItem;
		}
	}
	m_MagicTableItemOrderedTypeArray.RemoveAll();
	m_MagicTableItemIndexedTypeArray.RemoveAll();

	for (pos = m_ShieldsOrderedTypeMap.GetStartPosition(); pos != NULL;)
	{
		cDNDShield *pShield = NULL;
		m_ShieldsOrderedTypeMap.GetNextAssoc(pos, wID, pShield);

		if (pShield != NULL)
		{
			delete pShield;
		}
	}
	m_ShieldsOrderedTypeMap.RemoveAll();
	m_ShieldsIndexedTypeMap.RemoveAll();

	for (pos = m_ArmorOrderedTypeMap.GetStartPosition(); pos != NULL;)
	{
		cDNDArmor *pArmor = NULL;
		m_ArmorOrderedTypeMap.GetNextAssoc(pos, wID, pArmor);

		if (pArmor != NULL)
		{
			delete pArmor;
		}
	}
	m_ArmorOrderedTypeMap.RemoveAll();
	m_ArmorIndexedTypeMap.RemoveAll();
	m_CustomArmorsOrderedTypeArray.RemoveAll();

	for (pos = m_RingsOrderedTypeMap.GetStartPosition(); pos != NULL;)
	{
		cDNDRing *pRing = NULL;
		m_RingsOrderedTypeMap.GetNextAssoc(pos, wID, pRing);

		if (pRing != NULL)
		{
			delete pRing;
		}
	}
	m_RingsOrderedTypeMap.RemoveAll();
	m_RingsIndexedTypeMap.RemoveAll();

}

void CDMHelperApp::InitializeObjectTables()
{
	CString szTemp = _T("");

	CleanUpObjectTables();

	// these have to be loaded in the order of their type id's, or expansion of the array will kick your ass
	szTemp.Format("%sData/tables/armor.dat", m_szEXEPath.GetBuffer(0));
	LoadArmorTable(szTemp.GetBuffer(0));

	szTemp.Format("%sData/tables/shields.dat", m_szEXEPath.GetBuffer(0));
	LoadShieldsTable(szTemp.GetBuffer(0));

	szTemp.Format("%sData/tables/helmets.dat", m_szEXEPath.GetBuffer(0));
	LoadHelmetsTable(szTemp.GetBuffer(0));

	szTemp.Format("%sData/tables/weapons.dat", m_szEXEPath.GetBuffer(0));
	LoadWeaponsTable(szTemp.GetBuffer(0));

	szTemp.Format("%sData/tables/rings.dat", m_szEXEPath.GetBuffer(0));
	LoadRingsTable(szTemp.GetBuffer(0));

	szTemp.Format("%sData/tables/equipment.dat", m_szEXEPath.GetBuffer(0));
	LoadEquipmentTable(szTemp.GetBuffer(0));

	szTemp.Format("%sData/tables/CustomItems.dat", m_szEXEPath.GetBuffer(0));
	LoadCustomItemsTable(szTemp.GetBuffer(0));

	szTemp.Format("%sData/tables/CustomArmor.dat", m_szEXEPath.GetBuffer(0));
	LoadCustomArmorsTable(szTemp.GetBuffer(0));

	szTemp.Format("%sData/tables/CustomWeapons.dat", m_szEXEPath.GetBuffer(0));
	LoadCustomWeaponsTable(szTemp.GetBuffer(0));

	AlphabetizeObjectList();

	szTemp.Format("%sData/tables/magic_items.dat", m_szEXEPath.GetBuffer(0));		// some magic items are dependant on spell definitions, so must be loaded after last spellbook
	LoadMagicItemTable(szTemp.GetBuffer(0));

	m_dwInventoryFlag = GetUniversalTime();
}

void CDMHelperApp::InitializeSpells()
{
	CString szTemp;

	for (POSITION pos = m_SpellTypeMap.GetStartPosition(); pos != NULL; )
	{
		CString *pString = NULL;
		WORD wID;
		m_SpellTypeMap.GetNextAssoc(pos,wID,pString);

		if(pString != NULL)
		{
			delete pString;
		}
	}
	m_SpellTypeMap.RemoveAll();

	for (int i = 0; i < m_MasterSpellArray.GetSize(); ++i )
	{
		PSPELL pSpell = (PSPELL)m_MasterSpellArray.GetAt(i);

		if(pSpell != NULL)
		{
			delete pSpell;
		}
	}

	m_MasterSpellArray.RemoveAll();

	for (i = 0; i < m_SpellBooks.GetSize(); ++i )
	{
		PSPELLBOOK pSpellBook = (PSPELLBOOK)m_SpellBooks.GetAt(i);

		if(pSpellBook != NULL)
		{
			delete pSpellBook;
		}
	}

	m_SpellBooks.RemoveAll();

	m_CustomSpellArray.RemoveAll();

	m_nTotalSpells = 0;

	szTemp.Format("%sData/tables/cleric_spells.dat", m_szEXEPath.GetBuffer(0));
	LoadSpellBook(DND_CHARACTER_CLASS_CLERIC, szTemp.GetBuffer(0));

	szTemp.Format("%sData/tables/druid_spells.dat", m_szEXEPath.GetBuffer(0));
	LoadSpellBook(DND_CHARACTER_CLASS_DRUID, szTemp.GetBuffer(0));

	szTemp.Format("%sData/tables/magicuser_spells.dat", m_szEXEPath.GetBuffer(0));
	LoadSpellBook(DND_CHARACTER_CLASS_MAGE, szTemp.GetBuffer(0));

	szTemp.Format("%sData/tables/illusionist_spells.dat", m_szEXEPath.GetBuffer(0));
	LoadSpellBook(DND_CHARACTER_CLASS_ILLUSIONIST, szTemp.GetBuffer(0));

	szTemp.Format("%sData/tables/CustomSpells.dat", m_szEXEPath.GetBuffer(0));
	LoadCustomSpells(szTemp.GetBuffer(0));

	m_dwMasterSpellListHash = GetUniversalTime();
}

cDNDSpellBook *CDMHelperApp::LoadSpellBook(DND_CHARACTER_CLASSES nClassBook, char *path)
{
	cDNDSpellBook *pBook = new cDNDSpellBook();

	FILE *pInfile = fopen(path, "rt");

	char szInLine[2048];
	char szSpellStats[256];

	if(pInfile != NULL)
	{
		int nSpellNum = 0;
		int nSpellLevel = 0;
		int nOldSpellLevel = 0;

		CString szLastGoodSpell = _T("-");
		CString szLastGoodDesc = _T("-");
		int nLastGoodLevel = 0;

		while (!feof(pInfile))
		{
			memset(szSpellStats, 0, 256*sizeof(char));

			cDNDSpell *pSpell = new cDNDSpell();
			fgets(pSpell->m_szSpellName, 64, pInfile);
			fgets(pSpell->m_szSpellType, 64, pInfile);
			fgets(szSpellStats, 256, pInfile);
			fgets(pSpell->m_szSpellDesc, MAX_SPELL_DESC_SIZE, pInfile);
			fgets(szInLine, MAX_SPELL_DESC_SIZE, pInfile);

			RemoveLineFeeds(pSpell->m_szSpellName);

			//parse out level descriptor's etc.

			CString szTemp = szSpellStats;

			szTemp.Replace("        ", "*");
			szTemp.Replace("       ", "*");
			szTemp.Replace("      ", "*");
			szTemp.Replace("     ", "*");
			szTemp.Replace("    ", "*");
			szTemp.Replace("   ", "*");
			szTemp.Replace("  ", "*");

			//id, type, weight
			CString sToken=_T("");
			int i = 0; // substring index to extract
			while (AfxExtractSubString(sToken, szTemp, i,'*'))
			{   
			   sToken.Replace("	", "");	//remove tabs
			   sToken.TrimLeft(" ");
			   sToken.TrimRight(" ");
			   switch(i)
			   {
					case 0: 
					{
						//TRACE("TOKEN 0 IS %s\n", sToken.GetBuffer(0));
						sToken.Replace("Level:", "");
						nSpellLevel = atoi(sToken.GetBuffer(0));
						pSpell->m_nSpellLevel = nSpellLevel;

						if(pSpell->m_nSpellLevel == 99)
						{
							pSpell->m_nSpellLevel = 0;
						}

						if(pSpell->m_nSpellLevel > pBook->m_nMaxLevel)
						{
							pBook->m_nMaxLevel = pSpell->m_nSpellLevel;
						}

						if(nSpellLevel == 0)
						{
							int nBadDesc = szLastGoodDesc.GetLength();
							CString szError;
							szError.Format("ERROR loading spell data, last correctly loaded spell was level: %d spell %s", nLastGoodLevel, szLastGoodSpell.GetBuffer(0));
							AfxMessageBox(szError, MB_OK);
							ASSERT(nSpellLevel != 0);
						}

						break;
					}
					case 1:
					{
						//TRACE("TOKEN 1 IS %s\n", sToken.GetBuffer(0));
						sToken.Replace("Components:", "");
						sToken.TrimLeft(" ");
						strcpy(pSpell->m_szSpellComponents, sToken.GetBuffer(0));
						break;
					}
					case 2:
					{
						//TRACE("TOKEN 2 IS %s\n", sToken.GetBuffer(0));
						sToken.Replace("Range:", "");
						strcpy(pSpell->m_szSpellRange, sToken.GetBuffer(0));
						break;
					}
					case 3:
					{
						//TRACE("TOKEN 2 IS %s\n", sToken.GetBuffer(0));
						sToken.Replace("Casting Time:", "");
						strcpy(pSpell->m_szSpellCastingTime, sToken.GetBuffer(0));
						break;
					}
					case 4:
					{
						//TRACE("TOKEN 3 IS %s\n", sToken.GetBuffer(0));
						sToken.Replace("Duration:", "");
						strcpy(pSpell->m_szSpellDuration, sToken.GetBuffer(0));
						break;
					}
					case 5: 
					{
						//TRACE("TOKEN 4 IS %s\n", sToken.GetBuffer(0));
						sToken.Replace("Saving Throw:", "");
						strcpy(pSpell->m_szSpellSavingThrow, sToken.GetBuffer(0));
						break;
					}
					case 6: 
					{
						//TRACE("TOKEN 5 IS %s\n", sToken.GetBuffer(0));
						sToken.Replace("Area of Effect:", "");
						strcpy(pSpell->m_szSpellAreaOfEffect, sToken.GetBuffer(0));
						RemoveLineFeeds(pSpell->m_szSpellAreaOfEffect);
						break;
					} 
					case 7: 
					case 8: 
					case 9: 
					{
						break;
					}
			   };

			   i++;

			} //end while loop

			if (strlen(pSpell->m_szSpellAreaOfEffect) == 0)
			{
				TRACE("SPELL %s is FRAKKED ! (%s)\n", pSpell->m_szSpellName, path);
				TRACE("\n");
			}

			if(nOldSpellLevel != nSpellLevel)
			{
				nOldSpellLevel = nSpellLevel;
				nSpellNum = 0;
			}

			pSpell->m_ClassBook = nClassBook;
			pSpell->m_nSpellNumber = nSpellNum;
			pSpell->m_bSpellValid = TRUE;
			pSpell->m_nSpellIdentifier = m_nTotalSpells;

			if(nSpellLevel == 99)
			{
				nSpellLevel = 0;
			}

			++pBook->m_nSpellsPerLevel[nSpellLevel];

			//TRACE("PARSED: %s Level: %d\n", pSpell->m_szSpellName, pSpell->m_nSpellLevel);

			if(strcmp(pSpell->m_szSpellCastingTime, " Special") != 0)
			{
				int nCastingTime = GetCastingTime(pSpell);

				/*
				if(nCastingTime == 0)
				{
					TRACE("SPELL 0 !\n");
				}
				*/
			}

			memcpy(&pBook->m_Spells[pSpell->m_nSpellLevel][nSpellNum], pSpell, sizeof(cDNDSpell));
			
			szLastGoodSpell = pSpell->m_szSpellName;
			szLastGoodDesc = pSpell->m_szSpellDesc;
			nLastGoodLevel = nSpellLevel;

			m_MasterSpellArray.InsertAt(m_nTotalSpells++, pSpell);

			AddSpellTypeToMap(pSpell->m_szSpellType);

			++nSpellNum;
		}

		fclose(pInfile);
	}

	pBook->m_ClassBook = nClassBook;
	m_SpellBooks.InsertAt((DWORD)nClassBook, pBook);

	return pBook;
}

void CDMHelperApp::LoadCustomSpells(char *path)
{
	cDNDSpellBook *pBook = new cDNDSpellBook();

	FILE *pInfile = fopen(path, "rt");

	char szInLine[2048];
	char szSpellStats[512];

	if(pInfile != NULL)
	{
		int nSpellNum = 0;
		int nSpellLevel = 0;
		int nOldSpellLevel = 0;

		CString szLastGoodSpell = _T("-");
		CString szLastGoodDesc = _T("-");
		int nLastGoodLevel = 0;

		LONG lFileVersion = 0L;
		while (!feof(pInfile) && lFileVersion == 0L)
		{
			fgets(szInLine, 255, pInfile);
			CString szTemp = szInLine;
			
			if(szTemp.FindOneOf("#") >= 0)
				continue;

			lFileVersion = atol(szInLine);
		}

		while (!feof(pInfile))
		{
			fgets(szInLine, 255, pInfile);
			CString szTemp = szInLine;
			
			if(szTemp.FindOneOf("#") >= 0)
				continue;

			DND_CHARACTER_CLASSES nClassBook = DND_CHARACTER_CLASS_UNDEF;

			memset(szSpellStats, 0, 512*sizeof(char));

			cDNDSpell *pSpell = new cDNDSpell();

			strcpy(pSpell->m_szSpellName, szTemp.Left(63));

			fgets(pSpell->m_szSpellType, 64, pInfile);
			fgets(szSpellStats, 512, pInfile);
			fgets(pSpell->m_szSpellDesc, MAX_SPELL_DESC_SIZE, pInfile);
			fgets(szInLine, MAX_SPELL_DESC_SIZE, pInfile);

			RemoveLineFeeds(pSpell->m_szSpellName);
			RemoveLineFeeds(pSpell->m_szSpellType);
			RemoveLineFeeds(pSpell->m_szSpellDesc);

			//parse out level descriptor's etc.

			szTemp = szSpellStats;

			szTemp.Replace("\n","");
			szTemp.Replace("\r","");

			CString sToken=_T("");
			int i = 0; // substring index to extract
			while (AfxExtractSubString(sToken, szTemp, i,'|'))
			{   
			   sToken.Replace("	", "");	//remove tabs

			   switch(i)
			   {
					case 0: 
					{	
						sToken.Replace("Class:", "");

						if(sToken == _T("Cleric")) 
							nClassBook = DND_CHARACTER_CLASS_CLERIC;
						else if(sToken == _T("Druid")) 
							nClassBook = DND_CHARACTER_CLASS_DRUID;
						else if(sToken == _T("Magic_User")) 
							nClassBook = DND_CHARACTER_CLASS_MAGE;
						else if(sToken == _T("Illusionist")) 
							nClassBook = DND_CHARACTER_CLASS_ILLUSIONIST;
						else
						{
							ASSERT(1==2);
							nClassBook = DND_CHARACTER_CLASS_CLERIC;
						}

						break;
					}
					case 1: 
					{
						//TRACE("TOKEN 0 IS %s\n", sToken.GetBuffer(0));
						sToken.Replace("Level:", "");
						nSpellLevel = atoi(sToken.GetBuffer(0));
						pSpell->m_nSpellLevel = nSpellLevel;

						if(pSpell->m_nSpellLevel == 99)
						{
							pSpell->m_nSpellLevel = 0;
						}

						if(pSpell->m_nSpellLevel > pBook->m_nMaxLevel)
						{
							pBook->m_nMaxLevel = pSpell->m_nSpellLevel;
						}

						if(nSpellLevel == 0)
						{
							int nBadDesc = szLastGoodDesc.GetLength();
							CString szError;
							szError.Format("ERROR loading spell data, last correctly loaded spell was level: %d spell %s", nLastGoodLevel, szLastGoodSpell.GetBuffer(0));
							AfxMessageBox(szError, MB_OK);
							ASSERT(nSpellLevel != 0);
						}

						break;
					}
					case 2:
					{
						//TRACE("TOKEN 1 IS %s\n", sToken.GetBuffer(0));
						sToken.Replace("Components:", "");
						strcpy(pSpell->m_szSpellComponents, sToken.GetBuffer(0));
						break;
					}
					case 3:
					{
						//TRACE("TOKEN 2 IS %s\n", sToken.GetBuffer(0));
						sToken.Replace("Range:", "");
						strcpy(pSpell->m_szSpellRange, sToken.GetBuffer(0));
						break;
					}
					case 4:
					{
						//TRACE("TOKEN 2 IS %s\n", sToken.GetBuffer(0));
						sToken.Replace("Casting Time:", "");
						strcpy(pSpell->m_szSpellCastingTime, sToken.GetBuffer(0));
						break;
					}
					case 5:
					{
						//TRACE("TOKEN 3 IS %s\n", sToken.GetBuffer(0));
						sToken.Replace("Duration:", "");
						strcpy(pSpell->m_szSpellDuration, sToken.GetBuffer(0));
						break;
					}
					case 6: 
					{
						//TRACE("TOKEN 4 IS %s\n", sToken.GetBuffer(0));
						sToken.Replace("Saving Throw:", "");
						strcpy(pSpell->m_szSpellSavingThrow, sToken.GetBuffer(0));
						break;
					}
					case 7: 
					{
						//TRACE("TOKEN 5 IS %s\n", sToken.GetBuffer(0));
						sToken.Replace("Area of Effect:", "");
						strcpy(pSpell->m_szSpellAreaOfEffect, sToken.GetBuffer(0));
						RemoveLineFeeds(pSpell->m_szSpellAreaOfEffect);
						break;
					} 
					case 8: 
					case 9: 
					case 10: 
					{
						break;
					}
			   };

			   i++;

			} //end while loop

			
			pSpell->m_ClassBook = nClassBook;
			pSpell->m_nSpellNumber = nSpellNum;
			pSpell->m_bSpellValid = TRUE;
			pSpell->m_nSpellIdentifier = m_nTotalSpells;

			//TRACE("PARSED: %s Level: %d\n", pSpell->m_szSpellName, pSpell->m_nSpellLevel);

			if(strcmp(pSpell->m_szSpellCastingTime, " Special") != 0)
			{
				int nCastingTime = GetCastingTime(pSpell);

				/*
				if(nCastingTime == 0)
				{
					TRACE("SPELL 0 !\n");
				}
				*/
			}

		
			szLastGoodSpell = pSpell->m_szSpellName;
			szLastGoodDesc = pSpell->m_szSpellDesc;
			nLastGoodLevel = nSpellLevel;

			cDNDSpellBook *pSpellBook = m_SpellBooks.GetAt(nClassBook);

			if(pSpellBook == NULL)
				continue;

			pSpell->m_nSpellNumber = pSpellBook->m_nSpellsPerLevel[pSpell->m_nSpellLevel];
			pSpell->m_nSpellIdentifier = m_nTotalSpells;
			pSpell->m_bSpellValid = TRUE;

			pSpellBook->m_nSpellsPerLevel[pSpell->m_nSpellLevel] = (pSpellBook->m_nSpellsPerLevel[pSpell->m_nSpellLevel] + 1) % MAX_SPELLS_PER_LEVEL;
			
			pSpellBook->m_Spells[pSpell->m_nSpellLevel][pSpell->m_nSpellNumber-1];

			memcpy(&pSpellBook->m_Spells[pSpell->m_nSpellLevel][pSpell->m_nSpellNumber], pSpell, sizeof(cDNDSpell));

			m_CustomSpellArray.InsertAt(m_nTotalSpells, pSpell);
			m_MasterSpellArray.InsertAt(m_nTotalSpells++, pSpell);
			
		}

		fclose(pInfile);
	}

	delete pBook;

}

void CDMHelperApp::AddSpellTypeToMap(char *szInitialType)
{
	CString szType = szInitialType;

	szType.Replace(" ,Reversible", "");
	szType.Replace(",Reversible", "");
	szType.Replace(" Reversible", "");
	szType.Replace("Reversible", "");
	szType.Replace("(", "");
	szType.Replace(")", "");
	szType.Replace("}", "");
	szType.Replace("\n", "");
	szType.Replace("\r", "");
	szType.Replace(",", "/");
	szType.Replace("-", "/");
	szType.Replace("Conj.", "Conjuration");

	szType.Replace("; druidical only", "");
	szType.Replace("; clerical only", "");

	szType.TrimRight(" ");
	szType.TrimLeft(" ");

	for (POSITION pos = m_SpellTypeMap.GetStartPosition(); pos != NULL; )
	{
		WORD wID;
		CString *pCompare = NULL;
		m_SpellTypeMap.GetNextAssoc(pos,wID,pCompare);

		if(pCompare != NULL)
		{
			if(*pCompare == szType)
			{ 
				return;
			}
		}
	}

	CString *pString = new CString();
	int nCount = m_SpellTypeMap.GetCount();

	*pString = szType;

	m_SpellTypeMap.SetAt(nCount, pString);	
}

void CDMHelperApp::AlphabetizeObjectList()
{
	//alphabetize the ordered object list 
	//return;

	int nCount = m_ObjectsOrderedTypeArray.GetSize();

	for(int i = 0; i < nCount - 1; ++i)
	{
		cDNDObject *pObject1 = m_ObjectsOrderedTypeArray.GetAt(i);
		CString szString1 = pObject1->m_szType;
		szString1.Replace("*", "");
		szString1.Replace(",", "");
		szString1.Replace(" ", "");

		for(int j = i + 1; j < nCount; ++j)
		{
			cDNDObject *pObject2 = m_ObjectsOrderedTypeArray.GetAt(j);
			CString szString2 = pObject2->m_szType;
			szString2.Replace("*", "");
			szString2.Replace(",", "");
			szString2.Replace(" ", "");

			if(szString2.CompareNoCase(szString1) < 0)
			{
				CString szJunk = szString1;
				szString1 = szString2;
				szString2 = szJunk;
				
				cDNDObject _BlankObj;
				pObject1->CopyFull(&_BlankObj);
				pObject2->CopyFull(pObject1);
				_BlankObj.CopyFull(pObject2);
			}
		}
	}

#if 0 
	TRACE("OBJ DUMP START\n");
	for (int i = 0; i < nCount; ++i)
	{
		cDNDObject *pObject = m_ObjectsOrderedTypeArray.GetAt(i);

		TRACE("> %05d %s\n", pObject->m_wTypeId, pObject->m_szType);
	}
	TRACE("OBJ DUMP END\n");
#endif

}

void CDMHelperApp::LoadSettings()
{
	CString szTemp;
	szTemp.Format("%s/Data/DMSettings.dat", m_szEXEPath.GetBuffer(0));

	FILE *pInFile = fopen(szTemp.GetBuffer(0), "rb");

	if(pInFile != NULL)
	{
		fread(&m_Settings, sizeof(cDNDSettings), 1, pInFile);

		fclose(pInFile);

		m_Settings.SanityCheck();

		g_bUseUnearthedArcana = m_Settings.m_bUseUnearthedArcana;
		g_bUseDemiHumanLevelLimits = m_Settings.m_bUseLevelLimits;			
	}
}

void CDMHelperApp::SaveSettings()
{
	CString szTemp;
	szTemp.Format("%s/Data/DMSettings.dat", m_szEXEPath.GetBuffer(0));

	FILE *pOutFile = fopen(szTemp.GetBuffer(0), "wb");

	if(pOutFile != NULL)
	{
		m_Settings.m_bUseUnearthedArcana = g_bUseUnearthedArcana;
		m_Settings.m_bUseLevelLimits = g_bUseDemiHumanLevelLimits;	

		fwrite(&m_Settings, sizeof(cDNDSettings), 1, pOutFile);

		fclose(pOutFile);
	}
}

void CDMHelperApp::PlayWeaponSFX(int nWeaponID, int nSoundType)
{
	if(g_bUseSoundEffects == FALSE)
		return;

	int nSoundIndex = -1;
	for(int i = 0; i < MAX_WEAPONS_DEFINED; ++i)
	{
		if(m_Settings.m_WeaponSFX[i][nSoundType].m_nID == nWeaponID)
		{
			nSoundIndex = i;
			break;
		}
	}

	if(nSoundIndex == -1)
		return;

	for(i = 0; i < MAX_SOUNDBOARD_PAGES; ++ i)
	{
		for(int j = 0; j < SOUNDBOARD_SOUNDS_PER_PAGE; ++j)
		{
			if(m_Settings.m_SoundFX[i][j].m_szDesc[0])
			{
				if(strcmp(m_Settings.m_SoundFX[i][j].m_szDesc, m_Settings.m_WeaponSFX[nSoundIndex][nSoundType].m_szDesc) == 0)
				{
					CString szPath = m_Settings.m_SoundFX[i][j].m_szFilePath;
					szPath.Replace("<$DMAPATH>", m_szEXEPath);
					PlaySound ((LPCSTR) szPath.GetBuffer(0), AfxGetInstanceHandle() , SND_ASYNC | SND_FILENAME | SND_NODEFAULT);
					return;
				}
			}
		}
	}
}

void CDMHelperApp::PlaySpellSFX(int nSpellID)
{

	if(g_bUseSoundEffects == FALSE)
		return;

	int nSoundIndex = -1;
	for(int i = 0; i < 5*MAX_SPELL_LEVELS*MAX_SPELLS_PER_LEVEL; ++i)
	{
		if(m_Settings.m_SpellSFX[i].m_nID == nSpellID)
		{
			nSoundIndex = i;
			break;
		}
	}

	for(i = 0; i < MAX_SOUNDBOARD_PAGES; ++ i)
	{
		for(int j = 0; j < SOUNDBOARD_SOUNDS_PER_PAGE; ++j)
		{
			if(m_Settings.m_SoundFX[i][j].m_szDesc[0])
			{
				if(strcmp(m_Settings.m_SoundFX[i][j].m_szDesc, m_Settings.m_SpellSFX[nSoundIndex].m_szDesc) == 0)
				{
					CString szPath = m_Settings.m_SoundFX[i][j].m_szFilePath;
					szPath.Replace("<$DMAPATH>", m_szEXEPath);
					PlaySound ((LPCSTR) szPath.GetBuffer(0), AfxGetInstanceHandle() , SND_ASYNC | SND_FILENAME | SND_NODEFAULT);
					return;
				}
			}
		}
	}
}


BOOL CDMHelperApp::SaveConfirmCharacter(cDNDCharacter *pCharacter, BOOL bMultiple)
{

	CString szConfirmMsg;
	szConfirmMsg.Format("Save character %s ?", pCharacter->m_szCharacterName);

	BOOL *pbMultiple = NULL;

	if(bMultiple)
	{
		pbMultiple = &m_bSaveAllCharacters;
	}

	BOOL bSave = FALSE;
	CDMSaveConfirmDialog *pDlg = new CDMSaveConfirmDialog(szConfirmMsg.GetBuffer(0), &bSave, pbMultiple);

	pDlg->DoModal();

	delete pDlg;

	return bSave;
}

BOOL CDMHelperApp::SaveConfirmParty(cDNDParty *pParty, BOOL bMultiple)
{

	CString szConfirmMsg;
	szConfirmMsg.Format("Save party %s ?", pParty->m_szPartyName);

	BOOL *pbMultiple = NULL;

	if(bMultiple)
	{
		pbMultiple = &m_bSaveAllParties;
	}

	BOOL bSave = FALSE;
	CDMSaveConfirmDialog *pDlg = new CDMSaveConfirmDialog(szConfirmMsg.GetBuffer(0), &bSave, pbMultiple);

	pDlg->DoModal();

	delete pDlg;

	return bSave;
}

BOOL CDMHelperApp::SaveConfirmMap(cDNDMap *pMap, BOOL bMultiple)
{

	CString szConfirmMsg;
	szConfirmMsg.Format("Save map %s ?", pMap->m_szMapName);

	BOOL *pbMultiple = NULL;

	if(bMultiple)
	{
		pbMultiple = &m_bSaveAllParties;
	}

	BOOL bSave = FALSE;
	CDMSaveConfirmDialog *pDlg = new CDMSaveConfirmDialog(szConfirmMsg.GetBuffer(0), &bSave, pbMultiple);

	pDlg->DoModal();

	delete pDlg;

	return bSave;
}

DMPartyDialog *CDMHelperApp::FindCharacterPartyDialog(cDNDCharacter *pCharacter)
{
	if(pCharacter == NULL)
		return NULL;

	for (POSITION pos = m_PartyViewMap.GetStartPosition(); pos != NULL; )
	{
		WORD wID;
		PDNDPARTYVIEWDLG pPartyDlg = NULL;
		m_PartyViewMap.GetNextAssoc(pos,wID,pPartyDlg);

		if(pPartyDlg != NULL && pPartyDlg->m_pParty != NULL)
		{
			if(pPartyDlg->m_pParty->CharacterIsPartyMember(pCharacter))
			{
				return pPartyDlg;
			}
		}
	}

	return NULL;
}

void CDMHelperApp::ValidateNPCViewMap()
{
	WORD wID;
	for (POSITION pos = m_NPCViewMap.GetStartPosition(); pos != NULL; )
	{
		PDNDNPCVIEWDLG pNPCDlg = NULL;

		m_NPCViewMap.GetNextAssoc(pos,wID,pNPCDlg);

		if(pNPCDlg != NULL)
		{
			return;
		}
	}

	m_NPCViewMap.RemoveAll();

	TRACE("CLEANED m_NPCViewMap !\n");

}

void CDMHelperApp::ResetDataPicker(PDATAPICKERMAP *pPickerMap)
{
	POSITION pos;
	WORD wID;
	for (pos = pPickerMap->GetStartPosition(); pos != NULL; )
	{
		PDNDDATAPICKER pPicker = NULL;
		pPickerMap->GetNextAssoc(pos,wID,pPicker);

		if(pPicker != NULL)
		{
			delete pPicker;
		}
	}

	pPickerMap->RemoveAll();
}

int CDMHelperApp::GetCastingTime(cDNDSpell* pSpell) // in segments
{
	int nSegments = 0;

	CString szTemp = pSpell->m_szSpellCastingTime;

	//int nPunk = szTemp.Replace("round", "");

	if(szTemp.Replace("turns", "") > 0 || szTemp.Replace("turn", "") > 0)
	{
		nSegments = atoi(szTemp.GetBuffer(0)) * 60;
	}

	if(szTemp.Replace("rounds", "") > 0 || szTemp.Replace("round", "") > 0)
	{
		nSegments = atoi(szTemp.GetBuffer(0)) * 10;
	}


	if(szTemp.Replace("segments", "") > 0 || szTemp.Replace("segment", "") > 0)
	{
		nSegments = atoi(szTemp.GetBuffer(0));
	}

	return nSegments;
}

cDNDRandomEncounterTable *CDMHelperApp::FindEncounterTableByName(CString szTableName)
{

	for(int i = 0; i < m_RandomEncounterTables.GetCount(); ++i)
	{
		cDNDRandomEncounterTable *pTable = m_RandomEncounterTables.GetAt(i);

		if(pTable != NULL)
		{
			if(pTable->m_szTableName == szTableName)
			{
				return pTable;
			}
		}
	}

	return NULL;
}

CString CDMHelperApp::GenerateRandomParty(CString szTableName, cDNDParty *pParentParty, cDNDParty *pParty, DWORD dwSubPartyID, int nTerrain, int nLevel, int nOverrideDieRoll, RECT *rRoomWalls, PDATAPICKERMAP *pMonsterNameIndexer, PDNDMONSTERMANUALENTRY *pMonsterManualEntry)
{
	TRACE("ENTER GenerateRandomParty\n");

	CString szRetVal = _T("");

	BOOL bPartyGood = FALSE;
	BOOL bPartyEvil = FALSE;

	int nDieRoll = RollD100();

	if(nDieRoll <= 50)
	{
		bPartyGood = TRUE;
	}
	else
	{
		bPartyEvil = TRUE;
	}


	if(nLevel > 16)
	{
		nLevel = 16;
	}

	int nEncounterLevel = 0;

	if(szTableName == _T("DUNGEON")) // this is a dungeon random encounter, so use the dungeon index chart to get the final table
	{
		nDieRoll = RollD20(); // pick the table

		cDNDRandomEncounterIndex *pIndex = m_pRandomEncounterIndex[nLevel-1][nDieRoll-1];

		if(pIndex == NULL)
		{
			//report error
			return szRetVal;
		}

		szTableName = pIndex->m_szTableName;

		if(m_Settings.m_bUseMonsterManualII || m_Settings.m_bUseFiendFolio)
		{
			szTableName += _T(" MMII+FF (roll 2-20)");
		}
	}

	cDNDRandomEncounterTable *pTable = FindEncounterTableByName(szTableName);

	if(pTable == NULL)
	{
		//report error
		return szRetVal;
	}

	nEncounterLevel = max(nLevel, pTable->m_nLevel);

	cDNDRandomEncounterTableEntry *pEntry = NULL;
	BOOL bReady = TRUE;

	int nLoops = 0;
	do
	{
		++nLoops;

		bReady = TRUE;

		if(nOverrideDieRoll)
		{
			nDieRoll = nOverrideDieRoll;
		}
		else
		{
			if(szTableName.Find("(roll 2-20)") >= 0)
			{
				nDieRoll = RollD12() + RollD8();
			}
			else
			{
				nDieRoll = RollD100(); // pick the monster
			}
		}

		pEntry = pTable->GetEntryFromDieRoll(nDieRoll, nTerrain);

		if(pEntry == NULL)
		{
			//report error
			return szRetVal;
		}

		if(pEntry->m_pMonsterManualEntry == NULL)
		{
			//report error
			return szRetVal;
		}

		if(nOverrideDieRoll == 0)
		{
			if(m_Settings.m_bUseMonsterManualII == FALSE && (pEntry->m_pMonsterManualEntry->m_nMonsterIndex) / 1000 == 2)
				bReady = FALSE;

			if(m_Settings.m_bUseFiendFolio == FALSE && (pEntry->m_pMonsterManualEntry->m_nMonsterIndex / 1000) == 3)
				bReady = FALSE;
		}

	} while(bReady == FALSE && nLoops < 1000);

	if(pEntry == NULL)
	{
		//report error
		return szRetVal;
	}

	int nNumAppearing = 1;

	if(pEntry->m_nNumAppearingLow == 0) // no number appearing from encounter table, so use Monster Manual values
	{
		int nNumAppearingLow = 0;
		int nNumAppearingHigh = 0;

		sscanf(pEntry->m_pMonsterManualEntry->m_szNumAppearing.GetBuffer(0), "%d-%d", &nNumAppearingLow, &nNumAppearingHigh);

		int nRange = (nNumAppearingHigh - nNumAppearingLow + 1);

		if(nRange > 1)
		{
			nNumAppearing = rand() % nRange;
			nNumAppearing += nNumAppearingLow;
		}

		if(nNumAppearingHigh == nNumAppearingLow)
		{
			nNumAppearing = nNumAppearingHigh;
		}

		if(nNumAppearing == 0)
		{
			TRACE("WTF?\n");
		}
	}
	else if(pEntry->m_nNumAppearingHigh != pEntry->m_nNumAppearingLow)
	{
		int nRange = (pEntry->m_nNumAppearingHigh - pEntry->m_nNumAppearingLow + 1);
		nNumAppearing = rand() % nRange;
		nNumAppearing += pEntry->m_nNumAppearingLow;
	}

	if(nNumAppearing > 127)
	{
		nNumAppearing = 127;
	}

	int nSlot = 0;
	for(int i = 0; i < MAX_NPC_PARTY_MEMBERS; ++i)
	{
		if(pParty->m_NPCs[i].m_dwCharacterID == 0)
		{
			nSlot = i;
			break;
		}
	}

	if(nSlot >= MAX_NPC_PARTY_MEMBERS)
	{
		//report error
		szRetVal = _T("Party Full !");
		return szRetVal;
	}

	BOOL bAddFemales = FALSE;
	int nInLair = atoi(pEntry->m_pMonsterManualEntry->m_szPercentinLair.GetBuffer(0));

	if(RollD100() < nInLair)
	{
		bAddFemales = TRUE;
	}

	if(pEntry->m_pMonsterManualEntry->m_nMonsterIndex == 999 && nEncounterLevel == 99) // // this is a character NPC party , not a monster party
	{
		nEncounterLevel = RollD12();
		nTerrain = 0;
	}

	CString szTemp;
	for(int i = 0; i < nNumAppearing; ++i)
	{
		pParty->m_NPCs[nSlot].m_dwCharacterID = GetUniqueID();

		if(rRoomWalls != NULL)
		{
			int nSizeX = rRoomWalls->right - rRoomWalls->left;
			int nSizeY = rRoomWalls->bottom - rRoomWalls->top;

			if(nSizeX > 0 && nSizeY > 0)
			{
				pParty->m_NPCs[nSlot].m_fLocalLocationX = (float)( rand()%nSizeX + rRoomWalls->left);
				pParty->m_NPCs[nSlot].m_fLocalLocationY = (float)( rand()%nSizeY + rRoomWalls->top);

				pParty->m_NPCs[nSlot].m_fMapLocationX = pParty->m_NPCs[nSlot].m_fLocalLocationX;
				pParty->m_NPCs[nSlot].m_fMapLocationY = pParty->m_NPCs[nSlot].m_fLocalLocationY;
			}
		}

		if(pEntry->m_pMonsterManualEntry->m_nMonsterIndex == 999) // this is a character NPC, not a monster
		{
			GenerateNPCharacter(&pParty->m_NPCs[nSlot], nTerrain, nEncounterLevel, &bPartyGood, &bPartyEvil);
			pParty->m_NPCs[nSlot].m_dwSubPartyID = dwSubPartyID;

			pParty->AddPartyMemberID(pParty->m_NPCs[nSlot].m_dwCharacterID);

			// why were we doing this ?
			//sprintf(pParty->m_szPartyName, "[%s]\0", "Character Party");
		}
		else
		{
			pParty->m_NPCs[nSlot].m_nMonsterIndex = pEntry->m_pMonsterManualEntry->m_nMonsterIndex;
			pParty->m_NPCs[nSlot].m_nDungeonLevel = nEncounterLevel;

			int nNameIndex = nSlot+1;

			if(pMonsterNameIndexer != NULL)
			{
				PDNDDATAPICKER pPicker = NULL;

				pMonsterNameIndexer->Lookup(pEntry->m_pMonsterManualEntry->m_nMonsterIndex, pPicker);

				if(pPicker != NULL)
				{
					++pPicker->m_nData2;
					nNameIndex = +pPicker->m_nData2;
				}
				else
				{
					pPicker = new cDNDDataPicker();
					pPicker->m_nData1 = pEntry->m_pMonsterManualEntry->m_nMonsterIndex;
					++pPicker->m_nData2;
					nNameIndex = +pPicker->m_nData2;

					pMonsterNameIndexer->SetAt(pEntry->m_pMonsterManualEntry->m_nMonsterIndex, pPicker);
				}

			}

			// why were we doing this ?
			//sprintf(pParty->m_szPartyName, "[%s]\0", pEntry->m_szMonsterName);

			if(pMonsterManualEntry != NULL)
			{
				*pMonsterManualEntry = pEntry->m_pMonsterManualEntry;
			}

			CString szRandomName = _T("");

			DND_CHARACTER_RACES nRace = GetCharacterRaceFromName( pEntry->m_pMonsterManualEntry->m_szMMName);

			if(bAddFemales)
			{
				if(RollD100() <= 50)
				{
					pParty->m_NPCs[nSlot].m_nSex = 1;  // girls are number 1 !
				}
			}
				
			if(m_Settings.m_bUseRandomNames)
			{
				if(nRace != DND_CHARACTER_RACES_UNDEF)
				{
					szRandomName = GetRandomName(nRace, pParty->m_NPCs[nSlot].m_nSex);
				}
				else
				{
					szRandomName = GetRandomName((DND_CHARACTER_RACES)pEntry->m_pMonsterManualEntry->m_nMonsterIndex, pParty->m_NPCs[nSlot].m_nSex);
				}
			}

			if(szRandomName == _T(""))
			{
				szTemp.Format("%s %d", pEntry->m_pMonsterManualEntry->m_szMMName, nNameIndex);
			}
			else
			{
				szTemp.Format("%s [%s %d]\0", szRandomName, pEntry->m_pMonsterManualEntry->m_szMMName, nNameIndex);
			}

			strcpy(pParty->m_NPCs[nSlot].m_szCharacterName, szTemp.GetBuffer(0));

			pParty->m_NPCs[nSlot].m_nAlignment = pEntry->m_pMonsterManualEntry->m_nAlignment;
			pParty->m_NPCs[nSlot].m_dwSubPartyID = dwSubPartyID;

			BOOL bEquip = GenerateRandomMonsterWeapons(&pParty->m_NPCs[nSlot], pEntry->m_pMonsterManualEntry);
			bEquip |= GenerateRandomMonsterArmor(&pParty->m_NPCs[nSlot], pEntry->m_pMonsterManualEntry);

			if(bEquip)
			{
				NPCharacterEquipItems(&pParty->m_NPCs[nSlot]);
			}

			TRACE("ROLLED %s\n", szTemp);

		}

		nSlot = (nSlot + 1) % MAX_NPC_PARTY_MEMBERS;
	}

	szRetVal.Format("Rolled %d on %s\n%s", nDieRoll, szTableName, pParty->m_szPartyName);

	pParty->MarkChanged();

	TRACE("EXIT GenerateRandomParty nNumAppearing = %d\n", nNumAppearing);

	return szRetVal;

}

/*
DND_CHARACTER_CLASS_FIGHTER = 100, 
DND_CHARACTER_CLASS_RANGER = 110,
DND_CHARACTER_CLASS_CAVALIER = 120,
DND_CHARACTER_CLASS_PALADIN = 121,
DND_CHARACTER_CLASS_BARBARIAN = 130,

DND_CHARACTER_CLASS_CLERIC = 200,
DND_CHARACTER_CLASS_DRUID = 210,
DND_CHARACTER_CLASS_MONK = 220,

DND_CHARACTER_CLASS_THIEF = 300,
DND_CHARACTER_CLASS_THIEF_ACROBAT = 301,
DND_CHARACTER_CLASS_ASSASSIN = 310,

DND_CHARACTER_CLASS_MAGE = 400,
DND_CHARACTER_CLASS_ILLUSIONIST = 410,
*/

void CDMHelperApp::GenerateNPCharacter(cDNDNonPlayerCharacter *pNPC, int nTerrain, int nLevel, BOOL *pbPartyGood, BOOL *pbPartyEvil)
{
	cDNDRandomEncounterTable *pTable = FindEncounterTableByName("CHARACTER SUBTABLE");

	if(pTable == NULL)
	{
		return;
	}

	int nDieRoll = RollD100();

	cDNDRandomEncounterTableEntry *pEntry = pTable->GetEntryFromDieRoll(nDieRoll, nTerrain);

	if(pEntry == NULL)
	{
		return;
	}

	int nLevelShift = (rand() % 5) - 2;
	nLevel += nLevelShift;

	if(nLevel < 1)
	{
		nLevel = 1;
	}

	DND_CHARACTER_ALIGNMENTS nPossibleAlignments[9] = {
	DND_CHARACTER_ALIGNMENT_LAWFUL_GOOD,
	DND_CHARACTER_ALIGNMENT_NEUTRAL_GOOD,
	DND_CHARACTER_ALIGNMENT_CHAOTIC_GOOD,

	DND_CHARACTER_ALIGNMENT_LAWFUL_NEUTRAL,
	DND_CHARACTER_ALIGNMENT_NEUTRAL,
	DND_CHARACTER_ALIGNMENT_CHAOTIC_NEUTRAL,

	DND_CHARACTER_ALIGNMENT_LAWFUL_EVIL,
	DND_CHARACTER_ALIGNMENT_NEUTRAL_EVIL,
	DND_CHARACTER_ALIGNMENT_CHAOTIC_EVIL };

	int nNumPossibleAlignMents = 9;

	if(*pbPartyGood == TRUE)
	{
		nNumPossibleAlignMents = 6;
		nPossibleAlignments[0] = DND_CHARACTER_ALIGNMENT_LAWFUL_GOOD;
		nPossibleAlignments[1] = DND_CHARACTER_ALIGNMENT_NEUTRAL_GOOD;
		nPossibleAlignments[2] = DND_CHARACTER_ALIGNMENT_CHAOTIC_GOOD;
		nPossibleAlignments[3] = DND_CHARACTER_ALIGNMENT_LAWFUL_NEUTRAL;
		nPossibleAlignments[4] = DND_CHARACTER_ALIGNMENT_NEUTRAL;
		nPossibleAlignments[5] = DND_CHARACTER_ALIGNMENT_CHAOTIC_NEUTRAL;
	}
	else
	{
		nNumPossibleAlignMents = 6;
		nPossibleAlignments[0] = DND_CHARACTER_ALIGNMENT_LAWFUL_NEUTRAL;
		nPossibleAlignments[1] = DND_CHARACTER_ALIGNMENT_NEUTRAL;
		nPossibleAlignments[2] = DND_CHARACTER_ALIGNMENT_CHAOTIC_NEUTRAL;
		nPossibleAlignments[3] = DND_CHARACTER_ALIGNMENT_NEUTRAL_EVIL;
		nPossibleAlignments[4] = DND_CHARACTER_ALIGNMENT_CHAOTIC_EVIL;
		nPossibleAlignments[5] = DND_CHARACTER_ALIGNMENT_LAWFUL_EVIL;
	}


	DND_CHARACTER_RACES nPossibleRaces[6] = { 
	DND_CHARACTER_RACE_DWARF,
	DND_CHARACTER_RACE_ELF,
	DND_CHARACTER_RACE_GNOME,
	DND_CHARACTER_RACE_HALF_ELF,
	DND_CHARACTER_RACE_HALFLING,
	DND_CHARACTER_RACE_HALF_ORC };

	int nNumPossibleRaces = 6;
	int nFemaleChance = 0;

	CString szMonsterName = pEntry->m_szMonsterName;

	if(*pbPartyEvil == TRUE)
	{
		if(szMonsterName == _T("Ranger") || szMonsterName == _T("Paladin"))
		{
			szMonsterName = _T("Fighter");
		}
	}

	if(*pbPartyGood == TRUE)
	{
		if(szMonsterName == _T("Assassin"))
		{
			szMonsterName = _T("Thief");
		}
	}

	if(szMonsterName == _T("Fighter"))
	{
		pNPC->m_Class[0] = DND_CHARACTER_CLASS_FIGHTER;
		pNPC->m_nMonsterIndex = DND_CHARACTER_CLASS_FIGHTER;
		nFemaleChance = 20;
	}
	else if(szMonsterName == _T("Ranger"))
	{
		pNPC->m_Class[0] = DND_CHARACTER_CLASS_RANGER;
		pNPC->m_nMonsterIndex = DND_CHARACTER_CLASS_RANGER;
		nFemaleChance = 20;
		nNumPossibleAlignMents = 3;

		nNumPossibleRaces = 1;
		nPossibleRaces[0] = DND_CHARACTER_RACE_HALF_ELF;

		*pbPartyGood = TRUE;

	}
	else if(szMonsterName == _T("Paladin"))
	{
		pNPC->m_Class[0] = DND_CHARACTER_CLASS_PALADIN;
		pNPC->m_nMonsterIndex = DND_CHARACTER_CLASS_PALADIN;
		nFemaleChance = 10;
		nNumPossibleAlignMents = 1;

		nNumPossibleRaces = 1;
		nPossibleRaces[0] = DND_CHARACTER_RACE_HUMAN;

		*pbPartyGood = TRUE;
	}
	else if(szMonsterName == _T("Cleric"))
	{
		pNPC->m_Class[0] = DND_CHARACTER_CLASS_CLERIC;
		pNPC->m_nMonsterIndex = DND_CHARACTER_CLASS_CLERIC;
		nFemaleChance = 30;

		nNumPossibleRaces = 2;
		nPossibleRaces[0] = DND_CHARACTER_RACE_HALF_ELF;
		nPossibleRaces[1] = DND_CHARACTER_RACE_HALF_ORC;

		if(*pbPartyGood == TRUE)
		{
			nNumPossibleAlignMents = 5;
			nPossibleAlignments[0] = DND_CHARACTER_ALIGNMENT_LAWFUL_GOOD;
			nPossibleAlignments[1] = DND_CHARACTER_ALIGNMENT_NEUTRAL_GOOD;
			nPossibleAlignments[2] = DND_CHARACTER_ALIGNMENT_CHAOTIC_GOOD;
			nPossibleAlignments[3] = DND_CHARACTER_ALIGNMENT_LAWFUL_NEUTRAL;
			nPossibleAlignments[4] = DND_CHARACTER_ALIGNMENT_CHAOTIC_NEUTRAL;
		}
		else
		{
			nNumPossibleAlignMents = 5;
			nPossibleAlignments[0] = DND_CHARACTER_ALIGNMENT_LAWFUL_NEUTRAL;
			nPossibleAlignments[1] = DND_CHARACTER_ALIGNMENT_CHAOTIC_NEUTRAL;
			nPossibleAlignments[2] = DND_CHARACTER_ALIGNMENT_NEUTRAL_EVIL;
			nPossibleAlignments[3] = DND_CHARACTER_ALIGNMENT_CHAOTIC_EVIL;
			nPossibleAlignments[4] = DND_CHARACTER_ALIGNMENT_LAWFUL_EVIL;
		}
	}
	else if(szMonsterName == _T("Druid"))
	{
		pNPC->m_Class[0] = DND_CHARACTER_CLASS_DRUID;
		pNPC->m_nMonsterIndex = DND_CHARACTER_CLASS_DRUID;
		nFemaleChance = 30;
		nNumPossibleAlignMents = 1;
		nPossibleAlignments[0] = DND_CHARACTER_ALIGNMENT_NEUTRAL;

		nNumPossibleRaces = 1;
		nPossibleRaces[0] = DND_CHARACTER_RACE_HUMAN;

	}
	else if(szMonsterName == _T("Monk"))
	{
		pNPC->m_Class[0] = DND_CHARACTER_CLASS_MONK;
		pNPC->m_nMonsterIndex = DND_CHARACTER_CLASS_MONK;
		nFemaleChance = 10;
		nNumPossibleAlignMents = 3;
		nPossibleAlignments[0] = DND_CHARACTER_ALIGNMENT_LAWFUL_GOOD;
		nPossibleAlignments[1] = DND_CHARACTER_ALIGNMENT_LAWFUL_NEUTRAL;
		nPossibleAlignments[2] = DND_CHARACTER_ALIGNMENT_LAWFUL_EVIL;

		nNumPossibleRaces = 1;
		nPossibleRaces[0] = DND_CHARACTER_RACE_HUMAN;
	}
	else if(szMonsterName == _T("Thief"))
	{
		pNPC->m_Class[0] = DND_CHARACTER_CLASS_THIEF;
		pNPC->m_nMonsterIndex = DND_CHARACTER_CLASS_THIEF;
		nFemaleChance = 50;

		if(*pbPartyGood == TRUE)
		{
			nNumPossibleAlignMents = 4;
			nPossibleAlignments[0] = DND_CHARACTER_ALIGNMENT_NEUTRAL_GOOD;
			nPossibleAlignments[1] = DND_CHARACTER_ALIGNMENT_CHAOTIC_GOOD;
			nPossibleAlignments[2] = DND_CHARACTER_ALIGNMENT_NEUTRAL;
			nPossibleAlignments[3] = DND_CHARACTER_ALIGNMENT_CHAOTIC_NEUTRAL;
		}
		else
		{
			nNumPossibleAlignMents = 4;
			nPossibleAlignments[0] = DND_CHARACTER_ALIGNMENT_NEUTRAL;
			nPossibleAlignments[1] = DND_CHARACTER_ALIGNMENT_CHAOTIC_NEUTRAL;
			nPossibleAlignments[2] = DND_CHARACTER_ALIGNMENT_NEUTRAL_EVIL;
			nPossibleAlignments[3] = DND_CHARACTER_ALIGNMENT_CHAOTIC_EVIL;
		}

	}
	else if(szMonsterName == _T("Assassin"))
	{
		pNPC->m_Class[0] = DND_CHARACTER_CLASS_ASSASSIN;
		pNPC->m_nMonsterIndex = DND_CHARACTER_CLASS_ASSASSIN;
		nFemaleChance = 50;

		nNumPossibleAlignMents = 3;
		nPossibleAlignments[0] = DND_CHARACTER_ALIGNMENT_LAWFUL_EVIL;
		nPossibleAlignments[1] = DND_CHARACTER_ALIGNMENT_NEUTRAL_EVIL;
		nPossibleAlignments[2] = DND_CHARACTER_ALIGNMENT_CHAOTIC_EVIL;

		*pbPartyEvil = TRUE;
	}
	else if(szMonsterName == _T("Magic-User"))
	{
		pNPC->m_Class[0] = DND_CHARACTER_CLASS_MAGE;
		pNPC->m_nMonsterIndex = DND_CHARACTER_CLASS_MAGE;
		nFemaleChance = 50;

		nNumPossibleRaces = 2;
		nPossibleRaces[0] = DND_CHARACTER_RACE_ELF;
		nPossibleRaces[1] = DND_CHARACTER_RACE_HALF_ELF;
	}
	else if(szMonsterName == _T("Illusionist"))
	{
		pNPC->m_Class[0] = DND_CHARACTER_CLASS_ILLUSIONIST;
		pNPC->m_nMonsterIndex = DND_CHARACTER_CLASS_ILLUSIONIST;
		nFemaleChance = 50;

		nNumPossibleRaces = 3;
		nPossibleRaces[0] = DND_CHARACTER_RACE_ELF;
		nPossibleRaces[1] = DND_CHARACTER_RACE_ELF;
		nPossibleRaces[2] = DND_CHARACTER_RACE_HALF_ELF;
	}

	int nSex = RollD100();

	if(nSex <= nFemaleChance)
	{
		pNPC->m_nSex = 1;
	}
	else
	{
		pNPC->m_nSex = 0;
	}

	if(nNumPossibleAlignMents == 1)
	{
		pNPC->m_nAlignment = nPossibleAlignments[0];
	}
	else
	{
		BOOL bOK = FALSE;

		int nCount = 0;
		do
		{
			int nAlign = rand()%nNumPossibleAlignMents;
			pNPC->m_nAlignment = nPossibleAlignments[nAlign];

			if(*pbPartyEvil == TRUE && IsAlignmentGood(pNPC->m_nAlignment) == TRUE)
				bOK = FALSE;
			else if(*pbPartyGood == TRUE && IsAlignmentEvil(pNPC->m_nAlignment) == TRUE)
				bOK = FALSE;
			else bOK = TRUE;

			++nCount;

		} while (bOK == FALSE && nCount < 1000);
	}

	/*
	if(*pbPartyGood == FALSE && *pbPartyEvil == FALSE)
	{
		if(IsAlignmentEvil(pNPC->m_nAlignment))
		{
			*pbPartyEvil = TRUE;
		}
		if(IsAlignmentGood(pNPC->m_nAlignment))
		{
			*pbPartyGood = TRUE;
		}
	}
	*/

	pNPC->m_nLevel[0] = nLevel;

	nDieRoll = RollD100();

	if(nDieRoll > 20 || nNumPossibleRaces == 1)
	{
		pNPC->m_nRace = DND_CHARACTER_RACE_HUMAN;
	}
	else
	{
		int nRace = rand()%nNumPossibleRaces;

		pNPC->m_nRace = nPossibleRaces[nRace];

		nDieRoll = RollD100();

		if(nDieRoll <= 50)
		{
			GenerateMultiClassCharacter(pNPC, 1, *pbPartyGood, *pbPartyEvil);

			nDieRoll = RollD100();

			if(nDieRoll <= 25)
			{
				GenerateMultiClassCharacter(pNPC, 2, *pbPartyGood, *pbPartyEvil);
			}

			int nMultiLevels = 0;
			int nRemainder = 0;
			int nClasses = 0;

			if(pNPC->m_Class[2])
			{
				nMultiLevels = max(nLevel / 3, 1);
				nRemainder = nLevel % 3;

				pNPC->m_nLevel[0] = nMultiLevels;
				pNPC->m_nLevel[1] = nMultiLevels;
				pNPC->m_nLevel[2] = nMultiLevels;

				nClasses = 3;
			}
			else if(pNPC->m_Class[1])
			{
				nMultiLevels = max(nLevel / 2, 1);
				nRemainder = nLevel % 2;

				pNPC->m_nLevel[0] = nMultiLevels;
				pNPC->m_nLevel[1] = nMultiLevels;

				nClasses = 2;
			}

			int nCount = 0;
			for(int i = 0;  i < nRemainder; ++i)
			{
				pNPC->m_nLevel[nCount]+=1;

				++nCount;

				if(nCount >= nClasses)
					nCount = 0;

			}

		}
		
	}

	//check for level limits
	for(int i = 0; i < 3; ++ i)
	{
		if(pNPC->m_nLevel[i])
		{
			int nMaxLevel = CalculateLevelLimits(pNPC, pNPC->m_Class[i]);

			if(pNPC->m_nLevel[i] > nMaxLevel)
			{
				pNPC->m_nLevel[i] = nMaxLevel;
			}
		}
	}

	CString szRandomName = _T("");

	if(m_Settings.m_bUseRandomNames)
	{	
		szRandomName = GetRandomName(pNPC->m_nRace, pNPC->m_nSex);
	}

	if(szRandomName == _T(""))
	{
		strcpy(pNPC->m_szCharacterName, szMonsterName);
	}
	else
	{
		sprintf(pNPC->m_szCharacterName,"%s [%s]\0", szRandomName, szMonsterName);
	}

	RollNPCharacterEquipment(pNPC);

	pNPC->m_bIsNPC = TRUE;
	pNPC->m_bPocketPC = TRUE;
	pNPC->MarkSaved();

}

int CDMHelperApp::GetClassOrderingScore(DND_CHARACTER_CLASSES nClass)
{
	switch(nClass)
	{
		case DND_CHARACTER_CLASS_FIGHTER: return 0;
		case DND_CHARACTER_CLASS_RANGER: return 1;
		case DND_CHARACTER_CLASS_CLERIC: return 2;
		case DND_CHARACTER_CLASS_DRUID: return 3;
		case DND_CHARACTER_CLASS_MAGE: return 4;
		case DND_CHARACTER_CLASS_THIEF: return 5;
		case DND_CHARACTER_CLASS_ASSASSIN: return 6;
		case DND_CHARACTER_CLASS_ILLUSIONIST: return 7;
	}

	return 0;
}

void CDMHelperApp::SortCharacterClasses(cDNDNonPlayerCharacter *pNPC)
{
	int nSlot = 0;

	for(int i = 0; i < 4; ++i)
	{
		if(pNPC->m_Class[i] != DND_CHARACTER_CLASS_UNDEF)
		{
			++nSlot;
		}
	}

	if(nSlot < 2)
	{
		return;
	}

	//if(nSlot == 3)
	//{
	//	TRACE("GOT TRIP !\n");
	//}

	//sort the classes
	for(int i = 0; i < nSlot-1; ++i)
	{
		for(int j = i; j < nSlot; ++j)
		{
			if(GetClassOrderingScore(pNPC->m_Class[i]) > GetClassOrderingScore(pNPC->m_Class[j]))
			{
				DND_CHARACTER_CLASSES nJunk =  pNPC->m_Class[i];
				pNPC->m_Class[i] =  pNPC->m_Class[j];
				pNPC->m_Class[j] = nJunk;
			}
		}
	}

	//if(nSlot == 3)
	//{
	//	TRACE("ALL BE SORTED!\n");
	//}
}

void CDMHelperApp::GenerateMultiClassCharacter(cDNDNonPlayerCharacter *pNPC, int nSlot, BOOL bPartyGood, BOOL bPartyEvil)
{
	DND_CHARACTER_CLASSES nAvailableClasses[8] = { 
	DND_CHARACTER_CLASS_FIGHTER,
	DND_CHARACTER_CLASS_RANGER,
	DND_CHARACTER_CLASS_CLERIC,
	DND_CHARACTER_CLASS_DRUID,
	DND_CHARACTER_CLASS_THIEF ,
	DND_CHARACTER_CLASS_ASSASSIN,
	DND_CHARACTER_CLASS_MAGE,
	DND_CHARACTER_CLASS_ILLUSIONIST };

	int nNumAvailableClasses = 8;

	memset(nAvailableClasses, 0, 8 *sizeof(int)); // yes really

	switch(pNPC->m_nRace)
	{
		case DND_CHARACTER_RACE_DWARF:
		{
			if(bPartyGood)
			{
				nAvailableClasses[0] = DND_CHARACTER_CLASS_FIGHTER;
				nAvailableClasses[1] = DND_CHARACTER_CLASS_THIEF;
	
				nNumAvailableClasses = 2;
			}
			else
			{
				nAvailableClasses[0] = DND_CHARACTER_CLASS_FIGHTER;
				nAvailableClasses[1] = DND_CHARACTER_CLASS_THIEF;
				nAvailableClasses[2] = DND_CHARACTER_CLASS_ASSASSIN;

				nNumAvailableClasses = 3;
			}

			break;
		}
		case DND_CHARACTER_RACE_ELF:
		{
			if(bPartyGood)
			{
				nAvailableClasses[0] = DND_CHARACTER_CLASS_FIGHTER;
				nAvailableClasses[1] = DND_CHARACTER_CLASS_THIEF;
				nAvailableClasses[2] = DND_CHARACTER_CLASS_MAGE;
				nAvailableClasses[3] = DND_CHARACTER_CLASS_ILLUSIONIST;

				nNumAvailableClasses = 4;
			}
			else
			{
				nAvailableClasses[0] = DND_CHARACTER_CLASS_FIGHTER;
				nAvailableClasses[1] = DND_CHARACTER_CLASS_THIEF;
				nAvailableClasses[2] = DND_CHARACTER_CLASS_ASSASSIN;
				nAvailableClasses[3] = DND_CHARACTER_CLASS_MAGE;
				nAvailableClasses[4] = DND_CHARACTER_CLASS_ILLUSIONIST;

				nNumAvailableClasses = 5;
			}

			break;
		}

		case DND_CHARACTER_RACE_GNOME:
		{
			if(bPartyGood)
			{
				nAvailableClasses[0] = DND_CHARACTER_CLASS_FIGHTER;
				nAvailableClasses[1] = DND_CHARACTER_CLASS_THIEF;
				nAvailableClasses[2] = DND_CHARACTER_CLASS_ILLUSIONIST;

				nNumAvailableClasses = 3;
			}
			else
			{
				nAvailableClasses[0] = DND_CHARACTER_CLASS_FIGHTER;
				nAvailableClasses[1] = DND_CHARACTER_CLASS_THIEF;
				nAvailableClasses[2] = DND_CHARACTER_CLASS_ASSASSIN;
				nAvailableClasses[3] = DND_CHARACTER_CLASS_ILLUSIONIST;

				nNumAvailableClasses = 4;
			}

			break;
		}
		case DND_CHARACTER_RACE_HALF_ELF:
		{
			if(bPartyGood)
			{
				nAvailableClasses[0] = DND_CHARACTER_CLASS_FIGHTER;
				nAvailableClasses[1] = DND_CHARACTER_CLASS_RANGER;
				nAvailableClasses[2] = DND_CHARACTER_CLASS_THIEF;
				nAvailableClasses[3] = DND_CHARACTER_CLASS_MAGE;
				nAvailableClasses[4] = DND_CHARACTER_CLASS_ILLUSIONIST;

				nNumAvailableClasses = 5;
			}
			else
			{
				nAvailableClasses[0] = DND_CHARACTER_CLASS_FIGHTER;
				nAvailableClasses[1] = DND_CHARACTER_CLASS_RANGER;
				nAvailableClasses[2] = DND_CHARACTER_CLASS_THIEF;
				nAvailableClasses[3] = DND_CHARACTER_CLASS_ASSASSIN;
				nAvailableClasses[4] = DND_CHARACTER_CLASS_MAGE;
				nAvailableClasses[5] = DND_CHARACTER_CLASS_ILLUSIONIST;

				nNumAvailableClasses = 6;
			}

			break;
		}
		case DND_CHARACTER_RACE_HALFLING:
		{
			if(bPartyGood)
			{
				nAvailableClasses[0] = DND_CHARACTER_CLASS_FIGHTER;
				nAvailableClasses[1] = DND_CHARACTER_CLASS_THIEF;

				nNumAvailableClasses = 2;
			}
			else
			{
				nAvailableClasses[0] = DND_CHARACTER_CLASS_FIGHTER;
				nAvailableClasses[1] = DND_CHARACTER_CLASS_THIEF;
				nAvailableClasses[2] = DND_CHARACTER_CLASS_ASSASSIN;

				nNumAvailableClasses = 3;
			}

			break;
		}
		case DND_CHARACTER_RACE_HALF_ORC:
		{
			if(bPartyGood)
			{
				nAvailableClasses[0] = DND_CHARACTER_CLASS_FIGHTER;
				nAvailableClasses[1] = DND_CHARACTER_CLASS_CLERIC;
				nAvailableClasses[2] = DND_CHARACTER_CLASS_THIEF;

				nNumAvailableClasses = 3;
			}
			else
			{
				nAvailableClasses[0] = DND_CHARACTER_CLASS_FIGHTER;
				nAvailableClasses[1] = DND_CHARACTER_CLASS_CLERIC;
				nAvailableClasses[2] = DND_CHARACTER_CLASS_THIEF;
				nAvailableClasses[3] = DND_CHARACTER_CLASS_ASSASSIN;

				nNumAvailableClasses = 4;
			}

			break;
		}
	}

	int nCount = 0;

	DND_CHARACTER_CLASSES nPickedClasses[8];

	memset(nPickedClasses, 0, 8 *sizeof(int));

	for(int i = 0;  i < nNumAvailableClasses; ++i)
	{
		if((pNPC->m_Class[0] / 100 != nAvailableClasses[i] / 100) && (pNPC->m_Class[nSlot-1] / 100 != nAvailableClasses[i] / 100)) // not 100% confident this works
		{
			nPickedClasses[nCount] = nAvailableClasses[i];
			++nCount;
		}
	}


	if(nCount)
	{
		int nDieRoll = rand()%nCount;
		pNPC->m_Class[nSlot] = nPickedClasses[nDieRoll];
	}


	SortCharacterClasses(pNPC);

}

BOOL CDMHelperApp::AddItemToInventoryByName(cDNDNonPlayerCharacter *pNPC, char *szBaseName, char *szOverrideName, int nMaxMagicAdj, int nPercChance, int nOverrideMundane)
{
	BOOL bRetVal = FALSE;

	CString szItemName = szBaseName;

	if(szItemName.Find("&", 0) >= 0 && nOverrideMundane == -1)
	{
		CString sToken=_T("");
		int i = 0; // substring index to extract
		while (AfxExtractSubString(sToken, szItemName, i,'&'))
		{ 
			bRetVal |= AddItemToInventoryByName(pNPC, sToken.GetBuffer(0), szOverrideName, nMaxMagicAdj, nPercChance, nOverrideMundane);

			++i;
		};

		return bRetVal;
	}

	if(nPercChance != -99)
	{
		int nDieRoll = RollD100();

		if(nDieRoll >= nPercChance)
		{
			return bRetVal;
		}
	}

	szItemName.MakeLower();
	szItemName.Replace(" ", "");

	if(szItemName == _T("none"))
	{
		return TRUE; // yes it's TRUE, we have randomly selected nothing
	}

	if(nOverrideMundane == -1)
	{
		if(szItemName == _T("shield"))
		{
			switch(rand() % 2)
			{
				case 0: szItemName = _T("smallshield"); break;
				case 1: szItemName = _T("largeshield"); break;
			}
		}
		else if(szItemName == _T("sword"))
		{
			switch(rand() % 4)
			{
				case 0: szItemName = _T("sword,short"); break;
				case 1: szItemName = _T("sword,long"); break;
				case 2: szItemName = _T("sword,broad"); break;
				case 3: szItemName = _T("sword,bastard"); break;
			}
		}
		else if(szItemName == _T("crossbow"))
		{
			switch(rand() % 2)
			{
				case 0: szItemName = _T("crossbow,light"); break;
				case 1: szItemName = _T("crossbow,heavy"); break;
			}
		}
		else if(szItemName == _T("bow"))
		{
			switch(rand() % 2)
			{
				case 0: szItemName = _T("bow,short"); break;
				case 1: szItemName = _T("bow,long"); break;
			}
		}
		else if(szItemName == _T("polearm"))
		{
			switch(rand() % 6)
			{
				case 0: szItemName = _T("glaive"); break;
				case 1: szItemName = _T("glaive-guisarme"); break;
				case 2: szItemName = _T("guisarme"); break;
				case 3: szItemName = _T("guisarme-volge"); break;
				case 4: szItemName = _T("halberd"); break;
				case 5: szItemName = _T("voulge"); break;
			}
		}
		else if(szItemName == _T("lance"))
		{
			switch(rand() % 4)
			{
				case 0: szItemName = _T("lance,heavyhorse"); break;
				case 1: szItemName = _T("lance,mediumhorse"); break;
				case 2: szItemName = _T("lance,lighthorse"); break;
				case 3: szItemName = _T("guisarme-volge"); break;
			}
		}
		else if(szItemName == _T("mace"))
		{
			szItemName = _T("mace,footman's");
		}
		else if(szItemName == _T("flail"))
		{
			szItemName = _T("flail,footman's");
		}
		else if(szItemName == _T("pick"))
		{
			szItemName = _T("pick,footman's");
		}
		else if(szItemName == _T("axe"))
		{
			szItemName = _T("axe,hand");
		}
		else if(szItemName == _T("robe"))
		{
			szItemName = _T("robe,common");
		}
	}

	POBJECTTYPE pObject = NULL;

	for (int i = 0; i < m_MagicTableItemOrderedTypeArray.GetSize(); ++i )
	{
		PMAGICTABLEITEM pMagicItem = m_MagicTableItemOrderedTypeArray.GetAt(i);

		if(pMagicItem != NULL)
		{
			CString szCompare = pMagicItem->m_szDesc;
			szCompare.MakeLower();
			szCompare.Replace(" ", "");

			if(szCompare == szItemName)
			{
				int nMundaneItem = 0;
				int nMagicAdjustment = pMagicItem->m_nMagicAdjustmentLow;
				int nAmount = pMagicItem->m_nAmountLow;

				if(pMagicItem->m_nMundanes > 1)
				{
					if(nOverrideMundane)
					{
						nMundaneItem = (nOverrideMundane-1)%4;	// in general, so we can force cleric scrolls to clerics, mage scrolls to mages etc.
					}
					else
					{
						nMundaneItem = rand() % pMagicItem->m_nMundanes;
					}
				}
				if(pMagicItem->m_nMagicAdjustmentHigh > pMagicItem->m_nMagicAdjustmentLow)
				{
					int nAdj = pMagicItem->m_nMagicAdjustmentHigh - pMagicItem->m_nMagicAdjustmentLow;
					nMagicAdjustment = pMagicItem->m_nMagicAdjustmentLow + rand()%nAdj;
				}
				if(pMagicItem->m_nAmountHigh > pMagicItem->m_nAmountLow)
				{
					int nAdj = pMagicItem->m_nAmountHigh - pMagicItem->m_nAmountLow;
					nAmount = pMagicItem->m_nAmountLow + rand()%nAdj;
				}

				if(nMaxMagicAdj)
				{
					nMagicAdjustment = nMaxMagicAdj;
				}

				pObject = pMagicItem->EnchantItem(nMundaneItem, nMagicAdjustment, nAmount);

				if(szOverrideName != NULL)
				{
					strcpy(pObject->m_szType, szOverrideName);
				}

				bRetVal = TRUE;
				break;
			}
		}
	}
	////////////////////


	if(pObject == NULL)
	{
		for(int k = 0; k < m_ObjectsOrderedTypeArray.GetSize(); ++k)
		{
			POBJECTTYPE pObjBase = m_ObjectsOrderedTypeArray.GetAt(k);

			if(pObjBase != NULL)
			{
				CString szCompare = pObjBase->m_szType;
				szCompare.MakeLower();
				szCompare.Replace(" ", "");

				//if(szCompare.Find("mace") >= 0)
				//{
				//	TRACE("schlager!\n");
				//}

				if(szCompare == szItemName)
				{
					pObject = new cDNDObject();
					pObjBase->CopyFull(pObject);

					if(nMaxMagicAdj)
					{
						pObject->m_nMagicAdj = nMaxMagicAdj;
					}

					if(szOverrideName != NULL)
					{
						strcpy(pObject->m_szType, szOverrideName);
					}

					bRetVal = TRUE;
					break;
				}
			}
		}
	}

	if(pObject != NULL)
	{
		pNPC->AddToInventory(pObject);
		delete pObject;
	}
	else
	{
		TRACE("WTF is %s %s\n", szBaseName, szItemName);
	}

	return bRetVal;
}

BOOL CDMHelperApp::GenerateRandomMonsterWeapons(cDNDNonPlayerCharacter *pNPC, PDNDMONSTERMANUALENTRY pMonsterManualEntry)
{
	BOOL bRetVal = FALSE;

	if(pMonsterManualEntry->m_szWeapons == _T(""))
	{
		return bRetVal;
	}

	int nDieRoll = RollD100();
	int nTargetRoll = 0;

	CString sToken=_T("");
	int i = 0; // substring index to extract
	while (AfxExtractSubString(sToken, pMonsterManualEntry->m_szWeapons, i,'/') && bRetVal == FALSE)
	{
		CString sSubToken=_T("");
		CString szName=_T("");
		int j = 0; // substring index to extract
		while (AfxExtractSubString(sSubToken, sToken, j,'='))
		{
			switch(j)
			{
				case 0:
				{
					szName = sSubToken;
					break;
				}
				case 1:
				{
					int nPercChance = atoi(sSubToken.GetBuffer(0));

					nTargetRoll += nPercChance;

					if(nDieRoll <= nTargetRoll)
					{
						bRetVal |= AddItemToInventoryByName(pNPC, szName.GetBuffer(0), NULL, 0, -99, -1);
					}

					break;
				}
			}
			++j;
		}
		++i;
	}

	return bRetVal;

}

BOOL CDMHelperApp::GenerateRandomMonsterArmor(cDNDNonPlayerCharacter *pNPC, PDNDMONSTERMANUALENTRY pMonsterManualEntry)
{
	BOOL bRetVal = FALSE;

	if(pMonsterManualEntry->m_szArmors == _T(""))
	{
		return bRetVal;
	}

	int nDieRoll = RollD100();
	int nTargetRoll = 0;

	CString sToken=_T("");
	int i = 0; // substring index to extract
	while (AfxExtractSubString(sToken, pMonsterManualEntry->m_szArmors, i,'/') && bRetVal == FALSE)
	{
		CString sSubToken=_T("");
		CString szName=_T("");
		int j = 0; // substring index to extract
		while (AfxExtractSubString(sSubToken, sToken, j,'='))
		{
			switch(j)
			{
				case 0:
				{
					szName = sSubToken;
					break;
				}
				case 1:
				{
					int nPercChance = atoi(sSubToken.GetBuffer(0));

					nTargetRoll += nPercChance;

					if(nDieRoll <= nTargetRoll)
					{
						bRetVal |= AddItemToInventoryByName(pNPC, szName.GetBuffer(0), NULL, 0, -99, -1);
					}

					break;
				}
			}
			++j;
		}
		++i;
	}

	return bRetVal;

}

void CDMHelperApp::CallFileManagementDialog(DMLoadFileDescriptor *pFileDesc)
{
	CDMFileManagementDialog *pDlg = new CDMFileManagementDialog(pFileDesc);

	pDlg->DoModal();
	delete pDlg;
}
