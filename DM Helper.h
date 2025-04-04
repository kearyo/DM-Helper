// DM Helper.h : main header file for the DM HELPER application
//

/* To add OnInitDialog :

1. Select the dialog class you want to add OnInitDialog message in "Class View" window.

2. Display the Properties Window (View Menu->Other Windows->Properties Window, or ALT+ENTER)

3. You will see a lot of icons in the window, like Categorized, Alphabetical, Events (ligthning icon) and Overrides (method icon).

Select the Overrides icon and locate OnInitDialog.


To enable/disable Edit and Continue
	1.	On the Tools menu, click Options.
	2.	In the Options dialog box, open the Debugging node, and select the Edit and Continue category.
	3.	To enable, select the Enable Edit and Continue check box. To disable, clear the check box.
	4.	Click OK.

*/


// MAP GENERATOR AT http://donjon.bin.sh/adnd/dungeon/

#if !defined(AFX_DMHELPER_H__180AE6A1_F00E_4FB9_878D_216E96C10BB1__INCLUDED_)
#define AFX_DMHELPER_H__180AE6A1_F00E_4FB9_878D_216E96C10BB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

extern BOOL g_bGlobalShutDownFlag;

#define WORK_TESTING	FALSE

#define NEW_FILE_MANAGEMENT_WINDOW	TRUE

#define DND_WM_MESSAGE  (WM_APP + 1)
#define DND_WEATHER_MESSAGE (DND_WM_MESSAGE + 0x0001)
#define DND_DIRTY_WINDOW_MESSAGE (DND_WM_MESSAGE + 0x0002)


int GetSelectedListCtrlItem(CListCtrl *plctrl);
int GetLastSelectedListCtrlItem(CListCtrl *plctrl, int nCurrent, int *nOther);
void SetSelectedListCtrlItem(CListCtrl *plctrl, int nPos);
void ClearSelectedListCtrlItems(CListCtrl *plctrl);
void SetSelectedListCtrlItems(CListCtrl *plctrl, int *nArray);
void ClearSelectedListBoxItems(CListBox *plctrl);
void SetSelectedListBoxItems(CListBox *plctrl, int *nArray);
int GetSelectedListBoxItemsCount(CListBox *plctrl);
void CleanMemorizedSpellListBox(CListBox *plctrl);

UINT DMInstantMapSFXThreadProc(LPVOID pData);

typedef enum
{
	DND_EDIT_TYPE_NONE = 0,

	DND_EDIT_TYPE_COPY,
	DND_EDIT_TYPE_CUT,
	DND_EDIT_TYPE_PASTE,

} DND_EDIT_TYPES;

typedef enum
{
	DND_WEATHER_TYPE_RAIN = 0,

	DND_WEATHER_TYPE_SNOW,

} DND_WEATHER_TYPES;


typedef enum
{
	DND_VIEW_SPELL_MODE_UNDEF = 0,

	DND_VIEW_SPELL_MODE_COPY_TO_SPELLBOOK, 

	DND_VIEW_SPELL_MODE_PRAY,

	DND_VIEW_SPELL_MODE_MEMORIZE,

	DND_VIEW_SPELL_MODE_CAST,

} DND_VIEW_SPELL_MODE;



typedef enum
{
	DND_TAB_TYPE_UNDEF = 0,

	DND_TAB_TYPE_CAMPAIGN,

	DND_TAB_TYPE_PARTY,

	DND_TAB_TYPE_SUBPARTY,

	DND_TAB_TYPE_CHARACTER,

	DND_TAB_TYPE_NPC,

	DND_TAB_TYPE_CACHE,

	DND_TAB_TYPE_MAP,

	DND_TAB_TYPE_SOUNDBOARD,

	DND_TAB_TYPE_LAST_TAB_TYPE,

} DND_TAB_TYPES;

class cDMUpdateParams
{

public:

	CString m_szUpdateURL;
	int m_nUpdateRule;

	cDMUpdateParams()
	{
		m_szUpdateURL = _T("");
		m_nUpdateRule = 0;
	};


};

class DMLoadFileDescriptor
{

public:

	CString m_szLabel;
	CString m_szInitialPath;
	CString m_szSubDirectory;
	CString m_szInitialFileName;
	CString m_szOriginalFileName;	//may contain subdirectory information
	CString m_szFileTypes;
	CString m_szReturnedPath;
	BOOL m_bSave;
	BOOL m_bSuccess;


	DMLoadFileDescriptor()
	{
		m_szLabel = _T("");
		m_szInitialPath = _T("");
		m_szSubDirectory = _T("");
		m_szInitialFileName = _T("");
		m_szOriginalFileName = _T("");
		m_szFileTypes = _T("");
		m_szReturnedPath = _T("");
		m_bSave = FALSE;
		m_bSuccess = FALSE;
	}

	~DMLoadFileDescriptor()
	{
	}

};


class cDNDDataPicker;

#define PDNDDATAPICKER cDNDDataPicker*
typedef CTypedPtrMap <CMapWordToPtr, WORD, PDNDDATAPICKER> PDATAPICKERMAP;

class cDNDDataPicker
{
public:

	CString m_szLabel;

	LPVOID m_pData1;
	LPVOID m_pData2;

	int m_nData1;
	int m_nData2;

	void Reset()
	{
		m_szLabel = _T("");
		m_pData1 = NULL;
		m_pData2 = NULL;

		m_nData1 = 0;
		m_nData2 = 0;
	}

	cDNDDataPicker()
	{
		Reset();
	}

	cDNDDataPicker(char * szLabel, LPVOID p1, LPVOID p2)
	{
		Reset();
		m_szLabel = szLabel;
		
		m_pData1 = p1;
		m_pData2 = p2;
	}

	cDNDDataPicker(CString szLabel, LPVOID p1, LPVOID p2)
	{
		Reset();
		m_szLabel = szLabel;
		
		m_pData1 = p1;
		m_pData2 = p2;
	}

	~cDNDDataPicker()
	{
	}

};


class cDNDDisplayTab
{
public:

	WORD m_wTabId;	
	WORD m_wSubTabId;	//actual index in the tab control
	DND_TAB_TYPES m_nTabType;
	BOOL m_bSorted;
	BOOL m_bHide;

	CWnd* m_pWindow;

	cDNDDisplayTab()
	{
		m_wTabId = 0;
		m_wSubTabId = 0;
		m_nTabType = DND_TAB_TYPE_UNDEF;
		m_pWindow = NULL;
		m_bSorted = FALSE;
		m_bHide = FALSE;
	}

	~cDNDDisplayTab()
	{
	}

};


class cDNDSoundEffect
{
public:

	char m_szDesc[64];
	char m_szFilePath[MAX_PATH];

	cDNDSoundEffect()
	{
		memset(m_szDesc, 0, 64*sizeof(char));
		memset(m_szFilePath, 0, MAX_PATH*sizeof(char));
	}

	~cDNDSoundEffect()
	{
	}

	void Clear()
	{
		memset(m_szDesc, 0, 64*sizeof(char));
		memset(m_szFilePath, 0, MAX_PATH*sizeof(char));
	}

};

class cDNDSoundEffectMapper
{
public:

	int m_nID;
	char m_szDesc[64];

	cDNDSoundEffectMapper()
	{
		m_nID = 0;
		memset(m_szDesc, 0, 64 * sizeof(char));
	}

	~cDNDSoundEffectMapper()
	{
	}

};

#define SOUNDBOARD_SOUNDS_PER_PAGE	80
#define MAX_SOUNDBOARD_PAGES 50

class cDNDSettings
{
public:
	UINT	m_Version;

	BOOL m_bUseUnearthedArcana;
	BOOL m_bUseLevelLimits;
	BOOL m_bUseRandomNames;

	BOOL m_bMonkStrengthAdjustments;
	BOOL m_bMonkDexderityAdjustments;

	BOOL m_bUseMonsterManualII;
	BOOL m_bUseFiendFolio;

	BOOL m_bUseWeapons_vs_AC_Chart;

	BOOL m_bUseSmallFont;

	BOOL m_bUsed10Initiative;

	BOOL m_bMagicUserINTSpellBonus;

	BOOL m_bFreecastCantrips;

	BOOL m_bReserved[116];

	int m_nGPEncumbranceFactor;
	int m_nVendorPriceInflationFactor;
	int m_nMonitorDPMIFactor;

	BOOL m_bUseMaterialComponents;

	int m_nReserved[126];

	ULONG m_lReserved[128];
	float m_fReserved[128];

	int m_nReservedBlock[4092];
	
	cDNDSoundEffect m_SoundFX[MAX_SOUNDBOARD_PAGES][SOUNDBOARD_SOUNDS_PER_PAGE];

	char m_szSoundBoardTabLabels[MAX_SOUNDBOARD_PAGES][64];

	cDNDSoundEffectMapper m_WeaponSFX[MAX_WEAPONS_DEFINED][4];
	cDNDSoundEffectMapper m_SpellSFX[5*MAX_SPELL_LEVELS*MAX_SPELLS_PER_LEVEL];

	cDNDSettings()
	{
		m_Version = DMH_CURRENT_VERSION;
		
		m_bUseUnearthedArcana = TRUE;
		m_bUseLevelLimits = TRUE;
		m_bUseRandomNames = TRUE;

		m_bMonkStrengthAdjustments = FALSE;
		m_bMonkDexderityAdjustments = FALSE;

		m_bUseMonsterManualII = FALSE;
		m_bUseFiendFolio = FALSE;

		m_bUseWeapons_vs_AC_Chart = FALSE;

		m_bUseSmallFont = FALSE;

		m_bUsed10Initiative = FALSE;

		m_nGPEncumbranceFactor = 1;
		m_nVendorPriceInflationFactor = 100;

		m_nMonitorDPMIFactor = 96;
		
		memset(m_bReserved, 0, 116*sizeof(BOOL));

		memset(m_nReserved, 0, 126*sizeof(int));

		memset(m_lReserved, 0, 128*sizeof(ULONG));
		memset(m_fReserved, 0, 128*sizeof(float));

		memset(m_nReservedBlock, 0, 4092*sizeof(int));

		memset(m_szSoundBoardTabLabels, 0, MAX_SOUNDBOARD_PAGES*64*sizeof(char));

	}

	void SanityCheck()
	{
		if(m_nGPEncumbranceFactor < 1 || m_nGPEncumbranceFactor > 1000)
		{
			m_nGPEncumbranceFactor = 1;
		}
	}

	~cDNDSettings()
	{
	}

};


class cDMMapViewDialog;

class cDNDMapScaler
{
public:

	DWORD m_dwParentMapID;
	DWORD m_dwChildMapID;

	cDMMapViewDialog *m_pParentMapView; 
	cDMMapViewDialog *m_pChildMapView;

	int m_nParentMapX1;
	int m_nParentMapY1;

	int m_nParentMapX2;
	int m_nParentMapY2;

	int m_nChildMapX1;
	int m_nChildMapY1;

	int m_nChildMapX2;
	int m_nChildMapY2;

	void Reset()
	{
		m_dwParentMapID = 0;
		m_dwChildMapID = 0;

		m_pParentMapView = NULL;
		m_pChildMapView  = NULL;

		m_nParentMapX1 = 0;
		m_nParentMapY1 = 0;

		m_nParentMapX2 = 0;
		m_nParentMapY2 = 0;

		m_nChildMapX1 = 0;
		m_nChildMapY1 = 0;

		m_nChildMapX2 = 0;
		m_nChildMapY2 = 0;
	}
	
	cDNDMapScaler()
	{
		Reset();
	}

	~cDNDMapScaler()
	{
	}

};



#define PDNDTABTYPE cDNDDisplayTab*
typedef CTypedPtrArray <CPtrArray, PDNDTABTYPE> PDNDTABTYPEARRAY;

void TimeSpan(char *szLabel, DWORD *dwStartTime);
void WaitExecute(BOOL *bWaitFlag);
void WaitTime(DWORD dwMilliSeconds);

int MakeParagraphText(CString *pText, int nLineLength);

/////////////////////////////////////////////////////////////////////////////
// CDMHelperApp:
// See DM Helper.cpp for the implementation of this class

//

class CDMHelperDlg;
class DMPartyDialog;
class CDMCharViewDialog;
class CDMBaseCharViewDialog;
//class DMNPCViewDialog;
class cDMBaseNPCViewDialog;
class cDMMapViewDialog;
class cDMPDFViewDialog;
class CDMReminderSFXDialog;


#define PDNDPARTYVIEWDLG DMPartyDialog*
typedef CTypedPtrMap <CMapWordToPtr, WORD, PDNDPARTYVIEWDLG> PPARTYVIEWMAP;

#define PDNDBASECHARVIEWDLG	CDMBaseCharViewDialog*
#define PDNDCHARVIEWDLG CDMCharViewDialog*
#define PDNDNPCVIEWDLG cDMBaseNPCViewDialog*
#define PDNDPDFVIEWDLG cDMPDFViewDialog*

typedef CTypedPtrMap <CMapWordToPtr, WORD, PDNDBASECHARVIEWDLG> PBASECHARVIEWMAP;
typedef CTypedPtrMap <CMapWordToPtr, WORD, PDNDCHARVIEWDLG> PCHARVIEWMAP;
typedef CTypedPtrMap <CMapWordToPtr, WORD, PDNDNPCVIEWDLG> PNPCVIEWMAP;
typedef CTypedPtrMap <CMapWordToPtr, WORD, PDNDPDFVIEWDLG> PPDFVIEWMAP;

#define PDNDMAPVIEWDLG cDMMapViewDialog*
typedef CTypedPtrMap <CMapWordToPtr, WORD, PDNDMAPVIEWDLG> PMAPVIEWMAP;

typedef CTypedPtrMap <CMapStringToString, CString, CString> PSTRINGTOSTRINGMAP;


class CDMHelperApp : public CWinApp
{
public:
	CDMHelperApp();

	cDNDSettings m_Settings;

	ULONG_PTR m_gdiplusToken;

	BOOL m_bInitialResize;

	CDMHelperDlg *m_pMainWindow;

	int m_nTotalSpells;

	CString m_szEXEPath;
	CString m_szLoggedError;

	cDMUpdateParams m_UpdateParams;
	
	PDNDTABTYPEARRAY m_TabArray;
	PDNDTABTYPEARRAY m_SubTabArray;

	PCALENDARARRAY	m_CalendarArray;

	PARMORTYPEMAP m_ArmorOrderedTypeMap;
	PARMORTYPEMAP m_ArmorIndexedTypeMap;

	POBJECTTYPEARRAY m_HelmetsOrderedTypeArray;
	POBJECTTYPEARRAY m_HelmetsIndexedTypeArray;

	PSHIELDTYPEMAP m_ShieldsOrderedTypeMap;
	PSHIELDTYPEMAP m_ShieldsIndexedTypeMap;

	PRINGTYPEMAP	m_RingsOrderedTypeMap;
	PRINGTYPEMAP	m_RingsIndexedTypeMap;

	PWEAPONTYPEARRAY m_WeaponsOrderedTypeArray;
	PWEAPONTYPEARRAY m_WeaponsIndexedTypeArray;

	PWEAPONTYPEARRAY m_AmmunitionOrderedTypeArray;
	PWEAPONTYPEARRAY m_AmmunitionIndexedTypeArray;

	POBJECTTYPEARRAY m_EquipmentOrderedTypeArray;
	POBJECTTYPEARRAY m_EquipmentIndexedTypeArray;

	POBJECTTYPEARRAY m_LightSourceOrderedTypeArray;
	POBJECTTYPEARRAY m_LightSourceIndexedTypeArray;

	POBJECTTYPEARRAY m_MaterialComponentOrderedTypeArray;
	POBJECTTYPEARRAY m_MaterialComponentIndexedTypeArray;

	PSPELLARRAY		m_MasterSpellArray;
	PSPELLARRAY		m_CustomSpellArray;
	PSPELLBOOKARRAY	m_SpellBooks;

	PSPELLTOMATERIALCOMPONENTSMAP m_SpellToMaterialComponentsMap;

	PCSTRINGMAP	m_SpellTypeMap;
	
	int m_nLastLanguageIndex;
	PDNDLANGUAGEARRAY m_LanguageArray;

	PDNDRANDOMNAMETABLEMAP	m_RandomNameMap;


	PCALENDAR	m_pSelectedCalendar;

	PPARTYVIEWMAP m_PartyViewMap;
	PCHARVIEWMAP m_CharacterViewMap;
	PNPCVIEWMAP	m_NPCViewMap;
	PMAPVIEWMAP m_MapViewMap;
	PMAPVIEWMAP m_DetachedMapViewMap;
	PPDFVIEWMAP m_PDFViewMap;

	int m_nTotalMonsterCount;
	PMONSTERMANUALARRAY m_MonsterManualOrderedArray;
	PMONSTERMANUALMAP m_MonsterManualIndexedMap;
	PMONSTERMANUALMAP m_PCMonsterManualIndexedMap;
	PMONSTERMANUALMAP m_CustomMonsterManualIndexedMap;

	PCSTRINGMAP	m_MonsterDescMap;
	PCSTRINGMAP	m_MonsterIntMap;
	PCSTRINGMAP	m_MagicResistanceMap;
	PCSTRINGMAP m_MonsterPageMap;

	cDNDMapScaler m_MapScaler;

	int m_nDefinedObjectCount;
	int m_nPummelWeaponIndex;
	int m_nMonkWeaponIndex;

	POBJECTTYPEARRAY m_ObjectsOrderedTypeArray;
	POBJECTTYPEARRAY m_ObjectsIndexedTypeArray;
	PSTRINGOBJECTMAP m_NamedObjectLookupMap;

	POBJECTTYPEARRAY m_CustomObjectsOrderedTypeArray;
	// POBJECTTYPEARRAY m_CustomObjectsIndexedTypeArray; //unnecessary and unused
	POBJECTTYPEARRAY m_CustomArmorsOrderedTypeArray;
	POBJECTTYPEARRAY m_CustomWeaponsOrderedTypeArray;

	PMAGICTABLEITEMARRAY	m_MagicTableItemOrderedTypeArray;
	PMAGICTABLEITEMARRAY	m_MagicTableItemIndexedTypeArray;

	PMAGICITEMDESCRIPTIONMAP	m_MagicItemDescriptionMap;

	CWnd* m_pBaseCopyCharacterDialog;
	BOOL m_bCharacterCopyBufferIsNPC;

	PDNDRANDOMENCOUNTERINDEX	m_pRandomEncounterIndex[16][20];

	PRANOMENCOUNTERTABLEARRAY m_RandomEncounterTables;

	BOOL m_bSaveAllParties;
	BOOL m_bSaveAllMaps;
	BOOL m_bSaveAllCharacters;
	BOOL m_bRollPCsInitiative;

	int m_nMaxTiles;
	Bitmap* m_pDungeonTilesBitmap;
	Bitmap* m_pIsometricDungeonTilesBitmap;
	
	int m_nCalendarSecond;		// for sub-segment time tracking
	int m_nGlobalRound;			// for spell durations

	DWORD m_dwInventoryFlag;	// flag for validity of object tables

	DWORD m_dwMasterSpellListHash;

	DND_EDIT_TYPES	m_SoundFXCutPasteType;
	cDNDSoundEffect *m_pSoundFXCutPasteBuffer;

	char *m_pEncryptedSoundBuffer;

	int m_nInitiativeCurrentAttackNumber;
	DWORD m_dwInitiativeCurrentAttackerID;

	CDMReminderSFXDialog *m_pDMReminderSFXDialog;

	double m_dMonitorScaleFactorX;
	double m_dMonitorScaleFactorY;

	#if GAMETABLE_BUILD
	CWinThread *m_pGameServerUpdateThread;
	MiniUpdateMap m_MiniUpdateMap;
	DWORD m_dwMiniUpdateFlag;
	BOOL GetServerMiniPosition(char *szMiniName, float *pfX, float *pFY);
	cDNDSpell m_UtilitySpell;		// used for dragon breat weapons etc.
	#endif

	#if USE_DX_SOUND
	SoundClass *m_pDXSound;

	void InitDXSoundFX();
	void ShutDownDXSoundFX();

	#endif

	BOOL m_bWaitOnDungeonMaster;
	cDNDInstantMapSFXPlacer *m_pInstantMapSFXPlacer;

	CMutex m_GameMapMutex;
	CSingleLock *m_pGameMapSingleLock;

	BOOL m_bSpellFXOnMaps;
	BOOL m_bSpellFXOnMapsMagicMissile;
	BOOL m_bEnableGameWatcher;

	BOOL PlayDXSFX(CString szWAVPath);

	BOOL LoadUpdateParams(char *path);
	BOOL SaveUpdateParams();

	int GetNextCustomObjectIndex();
	int GetNextCustomArmorIndex(BOOL bMetalArmor);
	int GetNextCustomWeaponIndex();

	int LoadDungeonTiles();
	int LoadIsometricDungeonTiles();
	void LoadCalendars(char *path);
	void LoadArmorTable(char *path);
	void LoadHelmetsTable(char *path);
	void LoadShieldsTable(char *path);
	void LoadWeaponsTable(char *path);
	void LoadRingsTable(char *path);
	void LoadEquipmentTable(char *path);
	void LoadCustomItemsTable(char *path);
	void LoadCustomArmorsTable(char *path);
	void LoadCustomWeaponsTable(char *path);
	void LoadMagicItemTable(char *path);
	void LoadMagicItemDescriptions(char *path);
	void LoadLightSourceTable(char *path);
	void LoadLanguages(char *path);


	void InitializeMonsters();

	void PrescanMonsterManual(char *path, char *path2);
	void AddMonsterDescToMap(CString szInitialDesc);
	void AddMonsterIntToMap(CString szInitialInt);
	void AddMonsterMagicResistanceToMap(CString szInitialMR);

	void LoadMonsterPageIndexer(char *path);
	void DeleteMonsterPageIndexer();
	void LoadMonsterManual(char *path, char *szBook, BOOL bCustom);

	cDNDMonsterManualEntry *GetMonsterManualEntryFromName(CString szMonsterName);

	void LoadRandomEncounterIndexTable(char *path);
	void LoadRandomEncounterTables(char *path);

	DND_CHARACTER_RACES GetCharacterRaceFromName(char *szRaceName);
	DND_CHARACTER_RACES GetCharacterRaceFromName(CString szRaceName);

	void LoadRandomNames(char *path);
	void LoadRandomNameDataTable(CString szNameDataFileName, DND_CHARACTER_RACES nCurrentRace, int nGender);
	CString Capitalize(CString szInString);
	CString GetRandomName(DND_CHARACTER_RACES nRace, int nGender);

	void LoadCustomClasses(char *path);

	void CleanUpObjectTables();

	void InitializeObjectTables();

	void InitializeSpells();

	void InitializeSpellMaterialComponentsTable();
	void InitializeSpellMaterialComponents(CString szPath, BOOL bCustomComponents);
	void CreateSpellMaterialComponentsList();

	cDNDSpellBook *LoadSpellBook(DND_CHARACTER_CLASSES nClassBook, char *path);
	void LoadCustomSpells(char *path);
	void AddSpellTypeToMap(char *szInitialType);
	void AlphabetizeObjectList();

	void LoadSettings();
	void SaveSettings();
	void CheckForSoundBoardUpdates();

	BOOL StopSound();
	BOOL PlayLicensedSound(CString szFile, BOOL bAsync);
	BOOL PlaySoundFXFromFile(CString szFile, BOOL bAsync = TRUE, int nOverrideNum = 0);
	BOOL PlaySoundFX(CString szDesc, BOOL bAsync = TRUE, int nOverrideNum = 0);
	BOOL PlayPCSoundFX(CString szDesc, CString szName, CString szDefault, BOOL bAsync = TRUE, int nOverrideNum = 0);
	BOOL PlayEquipItemSFX(CString szDesc, CString szAlternate, BOOL bAsync = TRUE);
	void PlayWeaponSFX(int nWeaponID, int nIndex, CString szMagicWeaponName = _T(""), BOOL bAsync = TRUE);
	void PlaySpellSFX(int nSpellID, int nRepeats);
	int GetSpellRepeats(PSPELLSLOT pSpellSlot);


	BOOL SaveConfirmCharacter(cDNDCharacter *pCharacter, BOOL bMultiple);
	BOOL SaveConfirmParty(cDNDParty *pParty, BOOL bMultiple);
	BOOL SaveConfirmMap(cDNDMap *pMap, BOOL bMultiple);

	DMPartyDialog *FindCharacterPartyDialog(cDNDCharacter *pCharacter);

	void ValidateNPCViewMap();

	void ResetDataPicker(PDATAPICKERMAP *pPickerMap);

	int GetCastingTime(cDNDSpell* pSpell);

	BOOL SpellIsDirectDamageSpell(cDNDSpell* pSpell);
	CString GetCharacterNameFromID(DWORD dwCharacterID);
	LONG GetCharacterXPFromID(DWORD dwCharacterID);
	BOOL WoundCharacter(DWORD dwCharacterID, int *nRetDamage);
	BOOL SpellIsHealingSpell(cDNDSpell* pSpell);
	BOOL HealCharacter(DWORD dwCharacterID);
	#if GAMETABLE_BUILD
	void DragonBreathWeaponSFX(cDMBaseNPCViewDialog *pNPCDialog);
	#endif
	void RefreshAllMapViews();

	cDNDRandomEncounterTable *FindEncounterTableByName(CString szTableName);
	CString GenerateRandomParty(CString szTableName, cDNDParty *pParentParty, cDNDParty *pParty, DWORD dwSubPartyID, int nTerrain, int nLevel, int nOverrideDieRoll, RECT *rRoomWalls, PDATAPICKERMAP *pMonsterNameIndexer, PDNDMONSTERMANUALENTRY *pMonsterManualEntry);
	int GetClassOrderingScore(DND_CHARACTER_CLASSES nClass);
	void SortCharacterClasses(cDNDNonPlayerCharacter *pNPC);
	void GenerateNPCharacter(cDNDNonPlayerCharacter *pNPC, int nTerrain, int nLevel, BOOL *pbPartyGood, BOOL *pbPartyEvil);
	void GenerateMultiClassCharacter(cDNDNonPlayerCharacter *pNPC, int nSlot, BOOL bPartyGood, BOOL bPartyEvil);
	BOOL AddItemToInventoryByName(cDNDNonPlayerCharacter *pNPC, char *szBaseName, char *szOverrideName = NULL, int nMaxMagicAdj = 0, int nPercChance = -99, int nOverrideMundane = 0); // nOverrideMundane is indexed + 1
	BOOL GenerateRandomMonsterWeapons(cDNDNonPlayerCharacter *pNPC, PDNDMONSTERMANUALENTRY pMonsterManualEntry);
	BOOL GenerateRandomMonsterArmor(cDNDNonPlayerCharacter *pNPC, PDNDMONSTERMANUALENTRY pMonsterManualEntry);

	void CollapseWeaponArray(PWEAPONTYPEARRAY pWeaponArray, BOOL bIndexed);

	void CallFileManagementDialog(DMLoadFileDescriptor *pFileDesc);

	HWND CreateWeatherDialog(cDMMapViewDialog *pParent, DND_WEATHER_TYPES nWeather);

	void ImportSoundBoards(CString szFileName, BOOL bSave);

	BOOL EncryptRandomSounds(CString szFileName, int *pnNumEncrypts);
	BOOL EncryptWAVFile(CString szFileName);
	BOOL SendRemoteMusicCommand(CString szCommand, int nValue = -999);

	#ifdef _DEBUG
	#if KEARY_BUILD
	#if 0
	void WriteMaterialComponentsTable();
	#endif
	#endif
	#endif

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDMHelperApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CDMHelperApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

UINT DMGameServerUpdateThreadProc(LPVOID pData);
void ProcessMiniDataUpdate(std::string sBuffer);

#endif // !defined(AFX_DMHELPER_H__180AE6A1_F00E_4FB9_878D_216E96C10BB1__INCLUDED_)
