// #if !defined(AFX_DMHELPERCOMMON_CPP__180AE6A1_F00E_4FB9_878D_216E96C10BB1__INCLUDED_)
// #define AFX_DMHELPERCOMMON_CPP__180AE6A1_F00E_4FB9_878D_216E96C10BB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "stdafx.h"
#include <math.h>
#include <string.h>
#include "DM Helper.h"
#include "DM Helper Common.h"
#include "DMInventoryDialog.h"
#include "DMCharSpellsDialog.h"
#include "DMCharViewDialog.h"
#include "DMNPCViewDialog.h"

BOOL g_bUseUnearthedArcana;
BOOL g_bUseDemiHumanLevelLimits;
BOOL g_bUseMaterialComponents;
BOOL g_bReRollOnesOnHitDie;
BOOL g_bReRollLessHalfOnHitDie;
BOOL g_bMaxHitPointsAtFirstLevel;
BOOL g_bUsed10Initiative;
BOOL g_bUseSoundEffects;


cDNDCustomClass _gCustomClass[12];	// yep global, so sue me

DWORD GetUniversalTime(void)
{
	// this function returns the current seconds since midnight (00:00:00), January 1, 1970
	//do not mess with this function unless you are SURE what you are doing

	static DWORD dwMinClock = 0L;

	time_t ltime;

	time(&ltime);

	if ((DWORD)ltime < dwMinClock)
		return dwMinClock;

	dwMinClock = (DWORD)ltime;

	return (DWORD)ltime;

} // end GetUniversalTime


DWORD dwLastUniqueID = 0L;

DWORD GetUniqueID(void)
{
	DWORD dwRetVal = GetUniversalTime();

	if (dwRetVal <= dwLastUniqueID)
	{
		dwRetVal = dwLastUniqueID + 1;
	}

	dwLastUniqueID = dwRetVal;

	return dwRetVal;

}


WORD GetMinMapKey(CMapWordToPtr *pMap)
{
	WORD wMinWord = 0xFFFF;

	for (POSITION pos = pMap->GetStartPosition(); pos != NULL; )
	{
		LPVOID pPtr = NULL;
		WORD wID = 0xFFFF;
		pMap->GetNextAssoc(pos,wID,pPtr);

		if(wID < wMinWord)
		{
			wMinWord = wID;
		}
	}
	return wMinWord;
}

WORD GetMaxMapKey(CMapWordToPtr *pMap)
{
	WORD wMaxWord = 0;

	for (POSITION pos = pMap->GetStartPosition(); pos != NULL; )
	{
		LPVOID pPtr = NULL;
		WORD wID = 0;
		pMap->GetNextAssoc(pos,wID,pPtr);

		if(wID > wMaxWord)
		{
			wMaxWord = wID;
		}
	}
	return wMaxWord;
}

float GetDistance(float x1, float y1, float x2, float y2)
{
	double dx = (x1 - x2)*100;
	double dy = (y1 - y2)*100;

	float fretval = (float)(sqrt( fabs(dx*dx + dy*dy) ));

	fretval /= 100;

	return fretval;
}


char *GetLogEventName(DND_LOG_EVENT_TYPES nEventType)
{
	switch(nEventType)
	{
		case DND_LOG_EVENT_TYPE_NEW_GAME_SESSION: return("New Game Session");
		case DND_LOG_EVENT_TYPE_GAME_SESSION_ENDED: return("Game Session Ended");

		case DND_LOG_EVENT_TYPE_CHARACTER_CAST_SPELL: return("Character Cast Spell");
		case DND_LOG_EVENT_TYPE_CHARACTER_GAINED_XP: return("Character Gained XP");
		case DND_LOG_EVENT_TYPE_CHARACTER_GAINED_XP_COMBAT: return("Character Gained XP (Combat)");
		case DND_LOG_EVENT_TYPE_CHARACTER_GAINED_XP_CLASS_SKILL: return("Character Gained XP (Class Skill)");
		case DND_LOG_EVENT_TYPE_CHARACTER_GAINED_XP_ROLEPLAY: return("Character Gained XP (Roleplaying)");
		case DND_LOG_EVENT_TYPE_CHARACTER_GAINED_XP_TREASURE: return("Character Gained XP (Treasure)");
		case DND_LOG_EVENT_TYPE_CHARACTER_LOST_XP: return("Character Lost XP");
		case DND_LOG_EVENT_TYPE_CHARACTER_TRAINED: return("Character Trained");
		case DND_LOG_EVENT_TYPE_CHARACTER_GAINED_LEVEL: return("Character Gained Level");
		case DND_LOG_EVENT_TYPE_CHARACTER_LOST_LEVEL: return ("Character Lost Level");
		case DND_LOG_EVENT_TYPE_CHARACTER_DIED: return("Character Died");
		case DND_LOG_EVENT_TYPE_CHARACTER_RAISED: return("Character Raised/Resurrected");
		case DND_LOG_EVENT_TYPE_CHARACTER_JOINED_PARTY: return("Character Joined Party");
		case DND_LOG_EVENT_TYPE_CHARACTER_LEFT_PARTY: return("Character Left Party");

		case DND_LOG_EVENT_TYPE_PARTY_GAINED_XP: return("Party Gained XP");
		case DND_LOG_EVENT_TYPE_PARTY_GAINED_XP_COMBAT: return("Party Gained XP (Combat)");
		case DND_LOG_EVENT_TYPE_PARTY_GAINED_XP_TREASURE: return("Party Gained XP (Treasure)");
		case DND_LOG_EVENT_TYPE_PARTY_LOST_XP: return("Party Lost XP");

		case DND_LOG_EVENT_TYPE_PARTY_FOUNDED: return("Party Founded");
		case DND_LOG_EVENT_TYPE_PARTY_BEGAN_JOURNEY: return("Party Began Journey");
		case DND_LOG_EVENT_TYPE_PARTY_TRAVELED: return("Party Traveled X Miles");
		case DND_LOG_EVENT_TYPE_PARTY_FINISHED_JOURNEY: return("Party Finished Journey");

		case DND_LOG_EVENT_TYPE_PARTY_RESTED: return("Party Rested");
		
		case DND_LOG_EVENT_TYPE_MISC: return("Misc Event");

		case DND_LOG_EVENT_TYPE_CHARACTER_FAILED_CAST_SPELL: return("Character Failed to Cast Spell");
	}

	return "Undefined";
}

char *GetLogEventDesc(DND_LOG_EVENT_TYPES nEventType)
{
	switch(nEventType)
	{
		case DND_LOG_EVENT_TYPE_NEW_GAME_SESSION: return("New Game Session $REALDATE");
		case DND_LOG_EVENT_TYPE_GAME_SESSION_ENDED: return("Game Session Ended $REALDATE");

		case DND_LOG_EVENT_TYPE_CHARACTER_CAST_SPELL: return("$CNAME cast a spell : ");
		case DND_LOG_EVENT_TYPE_CHARACTER_GAINED_XP: return("$CNAME gained $AMOUNT XP");
		case DND_LOG_EVENT_TYPE_CHARACTER_GAINED_XP_COMBAT: return("$CNAME gained $AMOUNT XP (Combat)");
		case DND_LOG_EVENT_TYPE_CHARACTER_GAINED_XP_CLASS_SKILL: return("$CNAME gained $AMOUNT XP (Class Skill)");
		case DND_LOG_EVENT_TYPE_CHARACTER_GAINED_XP_ROLEPLAY: return("$CNAME gained $AMOUNT XP (Roleplaying)");
		case DND_LOG_EVENT_TYPE_CHARACTER_GAINED_XP_TREASURE: return("$CNAME gained $AMOUNT XP (Treasure)");
		case DND_LOG_EVENT_TYPE_CHARACTER_LOST_XP: return("$CNAME lost $AMOUNT XP");

		case DND_LOG_EVENT_TYPE_CHARACTER_TRAINED: return("$CNAME trained");

		case DND_LOG_EVENT_TYPE_CHARACTER_GAINED_LEVEL: return("$CNAME gained a level");
		case DND_LOG_EVENT_TYPE_CHARACTER_LOST_LEVEL: return("$CNAME lost a level");

		case DND_LOG_EVENT_TYPE_CHARACTER_DIED: return("$CNAME died");
		case DND_LOG_EVENT_TYPE_CHARACTER_RAISED: return("$CNAME was raised/resurrected from the dead");
		case DND_LOG_EVENT_TYPE_CHARACTER_JOINED_PARTY: return("$CNAME joined the party");
		case DND_LOG_EVENT_TYPE_CHARACTER_LEFT_PARTY: return("$CNAME left the party");

		case DND_LOG_EVENT_TYPE_PARTY_GAINED_XP: return("The $PNAME gained $AMOUNT XP");
		case DND_LOG_EVENT_TYPE_PARTY_GAINED_XP_COMBAT: return("The $PNAME gained $AMOUNT XP (Combat)");
		case DND_LOG_EVENT_TYPE_PARTY_GAINED_XP_TREASURE: return("The $PNAME gained $AMOUNT XP (Treasure)");
		case DND_LOG_EVENT_TYPE_PARTY_LOST_XP: return("The $PNAME lost $AMOUNT XP");
		
		case DND_LOG_EVENT_TYPE_PARTY_FOUNDED: return("The $PNAME were founded");

		case DND_LOG_EVENT_TYPE_PARTY_BEGAN_JOURNEY: return("The $PNAME began a journey");
		case DND_LOG_EVENT_TYPE_PARTY_TRAVELED: return("The $PNAME traveled $AMOUNT miles");
		case DND_LOG_EVENT_TYPE_PARTY_FINISHED_JOURNEY: return("The $PNAME finished a journey");

		case DND_LOG_EVENT_TYPE_PARTY_RESTED: return("The $PNAME rested");
		
		case DND_LOG_EVENT_TYPE_MISC: return("");

		case DND_LOG_EVENT_TYPE_CHARACTER_FAILED_CAST_SPELL: return("$CNAME failed casting a spell : ");
	}

	return "Undefined";
}

int GetLanguageIndex(char *szLanguage)
{

	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	for (int i = 0; i < pApp->m_LanguageArray.GetSize(); ++i )
	{
		cDNDLanguage *pLanguage = (cDNDLanguage *)pApp->m_LanguageArray.GetAt(i);

		if(pLanguage != NULL)
		{
			if(pLanguage->m_nList)
			{
				if(strcmp(szLanguage, pLanguage->m_szLanguageName) == 0)
				{
					return i;
				}
			}
		}
	}

	return 0;
}

int GetRandomLanguageIndex()
{
	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	int nCount = pApp->m_LanguageArray.GetSize();

	return rand() % nCount;
}

POBJECTTYPE cDNDMagicTableItem::EnchantItem(int nMundane, int nMagicAdjustment, int nAmount)
{
	POBJECTTYPE pObj = NULL;

	if(nMundane < 0 || nMundane > 9 || nAmount == -1)
	{
		return pObj;
	}

	if(m_pMundaneObject[nMundane] != NULL)
	{
		pObj = m_pMundaneObject[nMundane]->Clone();

		if(nMagicAdjustment)
		{
			pObj->m_nMagicAdj = nMagicAdjustment;
		}
		else
		{
			pObj->m_nMagicAdj = m_nMagicAdjustmentLow;
		}

		pObj->m_lAmount = nAmount;

		pObj->m_lXP = m_lXPValue;
		pObj->m_nCost = m_lGPValue;

		if(pObj->IsScroll())
		{
			PopulateRandomScroll(pObj);
		}
		else if(pObj->IsMagicContainer())
		{
			PopulateRandomMagicItem(pObj);
		}
		else
		{
			ValidateMagicItemName(pObj);
		}
		
		if (m_nEncumbranceOverride)
		{
			pObj->m_nWeight = m_nEncumbranceOverride;
		}
	}

	//magic sword abilities add here !
	if(pObj != NULL && pObj->m_nMagicAdj >= 1 && pObj->IsSword())
	{
		PopulateMagicSword(pObj);
	}

	return pObj;

}

void cDNDMagicTableItem::PopulateMagicSword(POBJECTTYPE pObj)
{
	int nDieRoll = RollD100();

	//nDieRoll = 97;

	if(nDieRoll <= 75)
	{
		return;
	}

	pObj->m_Alignment = GetMagicSwordAlignment();

	int nPrimaryAbilities = 0;
	int nExtraordinaryPower = 0;

	if(nDieRoll >= 76 && nDieRoll <= 83)
	{
		pObj->m_nIntelligence = 12;
		pObj->m_nContentsCount[0] |= 0x0001;	// semi-empathy
		nPrimaryAbilities = 1;
	}
	else if(nDieRoll >= 84 && nDieRoll <= 89)
	{
		pObj->m_nIntelligence = 13;
		pObj->m_nContentsCount[0] |= 0x0002;	// empathy
		nPrimaryAbilities = 2;
	}
	else if(nDieRoll >= 90 && nDieRoll <= 94)
	{
		pObj->m_nIntelligence = 14;
		pObj->m_nContentsCount[0] |= 0x0004;	// speech
		nPrimaryAbilities = 2;
	}
	else if(nDieRoll >= 95 && nDieRoll <= 97)
	{
		pObj->m_nIntelligence = 15;
		pObj->m_nContentsCount[0] |= 0x0004;	// speech
		nPrimaryAbilities = 3;
	}
	else if(nDieRoll >= 98 && nDieRoll <= 99)
	{
		pObj->m_nIntelligence = 16;
		pObj->m_nContentsCount[0] |= 0x0004;	// speech
		nPrimaryAbilities = 3;
	}
	else if(nDieRoll == 100)
	{
		pObj->m_nIntelligence = 17;
		pObj->m_nContentsCount[0] |= 0x000C;	// speech and telepathy
		nPrimaryAbilities = 3;
		nExtraordinaryPower = 1;
	}

	if(pObj->m_nContentsCount[0] & 0x0004) // can speak, needs languages
	{
		int nLangs = 0;

		int nLangRoll = RollD100();
		if(nLangRoll <= 40)
			nLangs = 1; 
		else if(nLangRoll <= 70)
			nLangs = 2; 
		else if(nLangRoll <= 85)
			nLangs = 3; 
		else if(nLangRoll <= 95)
			nLangs = 4; 
		else if(nLangRoll <= 99)
			nLangs = 5; 
		else if(nLangRoll == 100)
			nLangs = 6; 

		int nFirstLang = 0;

		nLangRoll = RollD100();
		int nLanguage = 0;

		if(nLangRoll <= 20)
		{
			if(nLangRoll <= 5)
				nLanguage = GetLanguageIndex("Common");
			else if(nLangRoll <= 10)
				nLanguage = GetLanguageIndex("Dwarvish");
			else if(nLangRoll <= 15)
				nLanguage = GetLanguageIndex("Elvish");
			else if(nLangRoll <= 20)
				nLanguage = GetLanguageIndex("Halfling");

			pObj->m_nContents[0] = nLanguage;
			nFirstLang = 1;
			nLangs -= 1;
		}

		for(int j = nFirstLang; j < nLangs; ++j)
		{
			pObj->m_nContents[j] = GetRandomLanguageIndex();
		}

	}

	for(int i = 0; i < nPrimaryAbilities;++i)
	{
		strcpy(pObj->m_szPrimaryAbility[i], GetMagicSwordPrimaryAbility(0, 0));
	}

}

void cDNDMagicTableItem::PopulateRandomScroll(POBJECTTYPE pObj)
{
	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	DND_CHARACTER_CLASSES _MagicType = DND_CHARACTER_CLASS_UNDEF;

	int nFlags = pObj->m_nFlags;
	nFlags = ((nFlags / 100) % 100) % 10;

	BOOL bShortList = TRUE;

	switch(nFlags)
	{
		case 1:	_MagicType = DND_CHARACTER_CLASS_CLERIC; break;
		case 2:	_MagicType = DND_CHARACTER_CLASS_DRUID; break;
		case 3:	_MagicType = DND_CHARACTER_CLASS_ILLUSIONIST; break;
		case 4:	_MagicType = DND_CHARACTER_CLASS_MAGE; bShortList = FALSE; break;
	}

	cDNDSpellBook *pBook = pApp->m_SpellBooks.GetAt(_MagicType);

	if(pBook == NULL)
	{
		strcpy(pObj->m_szExtendedName, m_szDesc);
		return;
	}

	for(int nCount = 0; nCount < m_nAmountLow && nCount < MAX_OBJECT_CONTENTS; ++nCount)
	{
		int nListIndex = 0;

		if(bShortList && m_nLevelLow[1])
		{
			nListIndex = 1;
		}

		int nLevelDiff = (m_nLevelHigh[nListIndex] - m_nLevelLow[nListIndex] + 1);

		int nLevel = (rand()%nLevelDiff);
		nLevel += m_nLevelLow[nListIndex];

		int nSpells = max(pBook->m_nSpellsPerLevel[nLevel], 1);

		int nSpell = rand() % nSpells;

		int nSpellIndex = pBook->m_Spells[nLevel][nSpell].m_nSpellIdentifier;

		pObj->PushContents(nSpellIndex+1, 1); // yes plus 1, because spell zero is valid but contents zero is NOT
		
	}

	pObj->m_bEnchanted = FALSE;
	pObj->RenameScroll();

}

void cDNDMagicTableItem::PopulateRandomMagicItem(POBJECTTYPE pObj)
{
	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	for(int nCount = 0; nCount < MAX_OBJECT_CONTENTS && m_nSpellIndex[nCount]; ++nCount)
	{
		ASSERT(m_nSpellIndex[nCount] > 0);
		pObj->PushContents(m_nSpellIndex[nCount], m_nSpellUsageCharges[nCount]);
	}

	if(m_nChargesLow == 0) // && m_nChargesLow == 0)
	{
		m_nChargesLow = 0;
		m_nChargesHigh = 100;
	}

	int nChargesDiff = m_nChargesHigh - m_nChargesLow + 1;
	int nCharges = rand()%nChargesDiff;

	nCharges += m_nChargesLow;

	pObj->m_nCharges = nCharges;

	ValidateMagicItemName(pObj);

}

void cDNDMagicTableItem::ValidateMagicItemName(POBJECTTYPE pObj)
{
	BOOL bOverWrite = FALSE;

	CString szMagicName = m_szDesc;
	szMagicName.MakeLower();

	if(szMagicName.Find("+", 0) >= 0 && pObj->m_nMagicAdj > 0)
	{
		pObj->m_bEnchanted = TRUE;
		bOverWrite = TRUE;
	}
	if(szMagicName.Find("-", 0) >= 0 && pObj->m_nMagicAdj < 0)
	{
		pObj->m_bEnchanted = TRUE;
		bOverWrite = TRUE;
	}

	if(szMagicName.Find("sword", 0) >= 0) 
	{
		szMagicName.Replace("sword", pObj->m_szType);
		bOverWrite = TRUE;
	}
	else if(szMagicName.Find("crossbow", 0) >= 0) 
	{
		szMagicName.Replace("crossbow", pObj->m_szType);
		bOverWrite = TRUE;
	}
	else if(szMagicName.Find("bow", 0) >= 0) 
	{
		szMagicName.Replace("bow", pObj->m_szType);
		bOverWrite = TRUE;
	}
	else if(szMagicName.Find("in number", 0) >= 0) // "arrow +2, 2-16 in number"	
	{
		// do nothing, keep mundane name
		pObj->m_bEnchanted = FALSE;
		bOverWrite = FALSE;
	}
	else
	{
		bOverWrite = TRUE;
	}

	if(bOverWrite)
	{
		strcpy(pObj->m_szType, szMagicName.Left(31));
		pObj->m_szType[31] = 0;

		strcpy(pObj->m_szExtendedName, szMagicName.Left(127));
		pObj->m_szExtendedName[127] = 0;
	}

	//TRACE("GOT ITEM !\n");
}

void cDNDPartyLog::ValidateEvents()
{
	cDNDPartyLog *pCopyLog = new cDNDPartyLog();

	int nCount = 0;
	for(int i = 0; i < MAX_LOG_EVENTS; ++i)
	{
		if(m_Event[i].m_EventType != DND_LOG_EVENT_TYPE_UNDEFINED)
		{
			memcpy(&pCopyLog->m_Event[nCount], &m_Event[i], sizeof(cDNDPartyLogEvent));
			++nCount;
		}
	}

	memcpy(m_Event, pCopyLog->m_Event, sizeof(cDNDPartyLogEvent) * MAX_LOG_EVENTS);

	m_LogHeader.m_nEvents = nCount;

	delete pCopyLog;
}

void cDNDPartyLog::DeleteEvent(int nEvent)
{
	m_Event[nEvent].m_EventType = DND_LOG_EVENT_TYPE_UNDEFINED;
	
	ValidateEvents();
}

void cDNDParty::ValidatePartyRoster()
{
	DWORD	_dwPartyRoster[MAX_PARTY_MEMBERS];
	memset(_dwPartyRoster, 0, MAX_PARTY_MEMBERS * sizeof(DWORD)); 

	//memset(m_dwPartyRoster, 0, MAX_PARTY_MEMBERS * sizeof(DWORD)); 

	int nIndex = 0;

	for(int i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		if(m_dwPartyRoster[i] > 0)
		{
			_dwPartyRoster[nIndex] = m_dwPartyRoster[i];
			++nIndex;
		}
	}

	memcpy(m_dwPartyRoster, _dwPartyRoster, MAX_PARTY_MEMBERS * sizeof(DWORD));
	
	m_nNumPartyMembers = nIndex;
}


void cDNDParty::AddPartyMemberID(DWORD dwID)
{
	for(int i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		if(m_dwPartyRoster[i] == dwID)
		{
			return;
		}
	}

	m_dwPartyRoster[MAX_PARTY_MEMBERS-1] = dwID;

	ValidatePartyRoster();
}

void cDNDParty::RemovePartyMemberID(DWORD dwID)
{
	for(int i = 0; i < MAX_PARTY_MEMBERS; ++i)
	{
		if(m_dwPartyRoster[i] == dwID)
		{
			m_dwPartyRoster[i] = 0;
		}
	}

	ValidatePartyRoster();
}

//cDNDNonPlayerCharacter _NPCs[MAX_NPC_PARTY_MEMBERS];
void cDNDParty::ValidateNPCList() //eliminate gaps in the NPC list
{
	/* // prior to rev 1.0.037
	//m_NPCs[MAX_NPC_PARTY_MEMBERS];
	for(int i = 0; i < MAX_NPC_PARTY_MEMBERS-1; ++i)
	{
		if(m_NPCs[i].m_dwCharacterID == 0L)
		{
			for(int j = i; j < MAX_NPC_PARTY_MEMBERS; ++j)
			{
				if(m_NPCs[j].m_dwCharacterID != 0L)
				{
					memcpy(&m_NPCs[i], &m_NPCs[j], sizeof(cDNDNonPlayerCharacter));
					memset(&m_NPCs[j], 0, sizeof(cDNDNonPlayerCharacter));
					break;
				}
			}
		}
	}
	*/

	// rev 1.0.037
	cDNDNonPlayerCharacter *_NPCs = new cDNDNonPlayerCharacter[MAX_NPC_PARTY_MEMBERS];
	memset(_NPCs, 0, MAX_NPC_PARTY_MEMBERS * sizeof(cDNDNonPlayerCharacter));

	int nIndex = 0;
	for (int i = 0; i < MAX_NPC_PARTY_MEMBERS - 1; ++i)
	{
		if (m_NPCs[i].m_dwCharacterID != 0L)
		{
			memcpy(&_NPCs[nIndex], &m_NPCs[i], sizeof(cDNDNonPlayerCharacter));
			++nIndex;
		}
	}
	
	memcpy(&m_NPCs, _NPCs, MAX_NPC_PARTY_MEMBERS * sizeof(cDNDNonPlayerCharacter));

	delete []_NPCs;
}

cDNDNonPlayerCharacter* cDNDParty::GetOpenNPCSlot() 
{
	for(int i = 0; i < MAX_NPC_PARTY_MEMBERS; ++i)
	{
		if(m_NPCs[i].m_dwCharacterID == 0L)
		{
			memset(&m_NPCs[i], 0, sizeof(cDNDNonPlayerCharacter));
			return &m_NPCs[i];
		}
	}
	
	return NULL;
}

void cDNDParty::RemovePocketPCs()
{
	for(int i = 0; i < MAX_NPC_PARTY_MEMBERS; ++i)
	{
		if(m_NPCs[i].m_bPocketPC)
		{
			m_NPCs[i].m_bPocketPC = FALSE;
			m_NPCs[i].m_dwCharacterID = 0;
		}
	}

	ValidateNPCList();
}

void cDNDParty::ValidateSubParties()
{
	cDNDSubParty _SubParty[MAX_SUB_PARTIES];

	int nCount = 0;
	for(int i = 0; i < MAX_SUB_PARTIES; ++i)
	{
		if(m_SubParty[i].m_dwSubPartyID != 0)
		{
			memcpy(&_SubParty[nCount], &m_SubParty[i], sizeof(cDNDSubParty));
			++nCount;
		}
	}

	memcpy(m_SubParty, _SubParty, sizeof(cDNDSubParty) * MAX_SUB_PARTIES);
}

BOOL cDNDParty::CharacterIsPartyMember(cDNDCharacter *pCharacter)
{
	for(int i = 0; i < MAX_PARTY_MEMBERS && m_dwPartyRoster[i]; ++i)
	{
		if(m_dwPartyRoster[i] == pCharacter->m_dwCharacterID)
		{
			return TRUE;
		}
	}

	return FALSE;
}


BOOL cDNDParty::CharacterIsPartyMember(cDNDNonPlayerCharacter *pNPC)
{
	for(int i = 0; i < MAX_PARTY_MEMBERS && m_dwPartyRoster[i]; ++i)
	{
		if(m_dwPartyRoster[i] == pNPC->m_dwCharacterID)
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL cDNDParty::CharacterIsPartyMember(DWORD dwCharacterID)
{
	for(int i = 0; i < MAX_PARTY_MEMBERS && m_dwPartyRoster[i]; ++i)
	{
		if(m_dwPartyRoster[i] == dwCharacterID)
		{
			return TRUE;
		}
	}

	return FALSE;
}

void cDNDCalendar::Initialize()
{
	//calculate phase points in the moon cycles
	for(int i = 0; i < m_nMoons; ++i)
	{
		m_fMoonPhasePoint[i][0] = 0.0f;

		float fMoonDiv = m_fMoonCycle[i] / (float)m_nMoonPhases[i];

		for(int j = 1; j < m_nMoonPhases[i]; ++j)
		{
			m_fMoonPhasePoint[i][j] = m_fMoonPhasePoint[i][j-1]+fMoonDiv;
		}

		m_fMoonPhasePoint[i][0] = m_fMoonCycle[i];
	}

	//calculate total days in year
	m_nDaysInYear = 0;
	for (int i = 0; i < m_nMonthsInYear; ++i)
	{
		m_nDaysInYear += m_nDaysInMonth[i];
	}

}


float cDNDCalendar::GetMoonCycleDay(int nMoon, int nFindMonth, int nFindDayofMonth, int nFindYear, float fCycleStart)
{
	if(m_nMoons == 0 || nMoon >= m_nMoons)
	{
		return 0.0f;
	}

	int nCount = 0;
	int nDayOfMonth = 0;
	int nMonth = 0;
	int nYear = this->m_nBaseYear;
	float fMoonDay = 0.0f;

	if(fCycleStart == 999.9f)
	{
		fMoonDay = m_fMoonCycleBase[nMoon];
	}
	else
	{
		fMoonDay = fCycleStart;
	}

	do
	{
		if(nMonth == nFindMonth && nDayOfMonth == nFindDayofMonth && nYear == nFindYear)
		{
			//TRACE("RETURNED %s %d is a %s\n",this->m_szMonthNames[nMonth], nDayOfMonth, this->m_szDayNames[nDayOfWeek]);
			return fMoonDay;
		}

		int nLeapDay = 0;
		if(IsLeapYear(nYear) && nMonth == m_nLeapYearMonth)
		{
			nLeapDay = 1;
		}

		++nDayOfMonth;

		fMoonDay = fMoonDay + 1.0f;

		if(fMoonDay > m_fMoonCycle[nMoon])
		{
			fMoonDay = fMoonDay - m_fMoonCycle[nMoon];
		}
		
		if(nDayOfMonth >= m_nDaysInMonth[nMonth] + nLeapDay)
		{
			nDayOfMonth = 0;
			++nMonth;
		}

		if(nMonth >= m_nMonthsInYear)
		{
			++nYear;
			nDayOfMonth = 0;
			nMonth = 0;
		}

		++nCount;

	} while(nCount < 100000);

	return 0;
}

float cDNDCalendar::IncrementMoonDay(int nMoon, float fMoonDay)
{
	fMoonDay += 1.0f;

	if(fMoonDay > m_fMoonCycle[nMoon])
	{
		fMoonDay = fMoonDay - m_fMoonCycle[nMoon];
	}

	return fMoonDay;
}

int cDNDCalendar::GetMoonPhase(int nMoon, float fMoonDay)
{
	int nRetVal = -1;

	if(m_nMoons == 0)
	{
		return nRetVal;
	}

	float fMinDist = 1.0f;

	for(int i = 0; i <= m_nMoonPhases[nMoon]; ++i)
	{
		if(m_fMoonPhasePoint[nMoon][i] >= fMoonDay)
		{
			float fDist = m_fMoonPhasePoint[nMoon][i] - fMoonDay;

			if(fDist < fMinDist)
			{
				nRetVal = i; 

				if(nMoon == 1)
				{
					TRACE("FULL CELENE %f (%f %f)\n", fDist, m_fMoonPhasePoint[nMoon][i], fMoonDay);
				}

				break;
			}
		}

		/*
		fDist = fabs(m_fMoonPhasePoint[nMoon][i] - (m_fMoonCycle[nMoon]-fMoonDay)); // wrap around
		if(fDist < fMinDist)
		{
			nRetVal = i; 
			break;
		}
		*/
	}

	return nRetVal;
}

BOOL cDNDCalendar::IsLeapYear(int nFindYear)
{
	BOOL bRetVal = FALSE;

	//m_nLeapYearDivisor = 0;
	//m_nLeapYearExceptionDivisor = 0;
	//m_nLeapYearExceptionExceptionDivisor = 0;

	if(m_nLeapYearDivisor)
	{
		if(nFindYear % m_nLeapYearDivisor == 0) // divisible by 4 ?
		{
			bRetVal = TRUE;

			if(m_nLeapYearExceptionDivisor)
			{
				if(nFindYear % m_nLeapYearExceptionDivisor == 0) // divisible by 100 ?
				{
					bRetVal = FALSE;

					if(m_nLeapYearExceptionExceptionDivisor)
					{
						if(nFindYear % m_nLeapYearExceptionExceptionDivisor == 0) // divisible by 400 ?
						{
							bRetVal = TRUE;
						}
					}
				}
			}
		}
	}

	return bRetVal;
}

int cDNDCalendar::FindDayOfWeek(int nFindMonth, int nFindDayofMonth, int nFindYear)
{

	int nCount = 0;
	int nDayOfMonth = 0;
	int nDayOfWeek = this->m_nCalendarStartsOnDayOfWeek;
	int nMonth = 0;
	int nYear = this->m_nBaseYear;

	//TRACE("START %s %d is a %s\n",this->m_szMonthNames[nMonth], nDayOfMonth, this->m_szDayNames[nDayOfWeek]);
	//TRACE("LOOKING FOR %s(%d) %d\n",this->m_szMonthNames[nFindMonth], nFindMonth, nFindDayofMonth);

	do
	{
		//TRACE("%s %d is a %s\n",this->m_szMonthNames[nMonth], nDayOfMonth, this->m_szDayNames[nDayOfWeek]);
		if(nMonth == nFindMonth && nDayOfMonth == nFindDayofMonth && nYear == nFindYear)
		{
			//TRACE("RETURNED %s %d is a %s\n",this->m_szMonthNames[nMonth], nDayOfMonth, this->m_szDayNames[nDayOfWeek]);
			return nDayOfWeek;
		}

		int nLeapDay = 0;
		if(IsLeapYear(nYear) && nMonth == m_nLeapYearMonth)
		{
			nLeapDay = 1;
		}

		++nDayOfMonth;
		++nDayOfWeek;

		if(nDayOfWeek >= m_nDaysInWeek)
		{
			nDayOfWeek = 0;
		}

		if(nDayOfMonth >= m_nDaysInMonth[nMonth] + nLeapDay)
		{
			nDayOfMonth = 0;
			++nMonth;
		}

		if(nMonth >= m_nMonthsInYear)
		{
			++nYear;
			nDayOfMonth = 0;
			nMonth = 0;
		}

		++nCount;

	} while(nCount < 100000);

	return 0;
}

int cDNDCalendar::FindDayofYear(int nFindMonth, int nFindDayofMonth)
{
	int nRetDay = 0;
	for (int i = 0; i < nFindMonth; ++i)
	{
		nRetDay += m_nDaysInMonth[i];
	}

	nRetDay += nFindDayofMonth;

	nRetDay += 1;

	return nRetDay;
}

ULONG cDNDCalendar::FindTurnofYear(int nFindYear, int nFindMonth, int nFindDayofMonth, int nFindHour, int nFindTurn)
{
	ULONG lRetTurn = 0;
	
	lRetTurn = nFindYear * m_nDaysInYear * 144; // 144 turns in a day;
	lRetTurn += (ULONG)FindDayofYear(nFindMonth, nFindDayofMonth) * 144;  // 144 turns in a day;
	lRetTurn += 6 * nFindHour; // six turns in an hour
	lRetTurn += nFindTurn;

	return lRetTurn;
}

void cDNDParty::ModifyCalendar(DND_CALENDAR_MOD_TYPES nSet, DND_CALENDAR_MOD_TYPES nSetType, int nValue)
{

	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	if(pApp->m_pSelectedCalendar == NULL)
		return;

	int nIncrement = 0;
	BOOL bSet = FALSE;
	switch(nSetType)
	{
		case DND_CALENDAR_MOD_DECREMENT:
		{
			nIncrement = -1;
			break;
		}
		case DND_CALENDAR_MOD_INCREMENT:
		{
			nIncrement = 1;
			break;
		}
		case DND_CALENDAR_MOD_SET:
		{
			if(nValue == -9999)
				return;

			bSet = TRUE;

			break;
		}
	}

	/*
		m_dwCalendarID = 0;
		m_nSegment = 0;
		m_nRound = 0;
		m_nTurn = 0;
		m_nHour = 0;
		m_nDayofWeek = 0;
		m_nDayofMonth = 0;
		m_nMonth = 0;
		m_nYear = 0;
		*/

	switch(nSet)
	{
		case DND_CALENDAR_MOD_SECOND:
		{
			if(bSet)
				pApp->m_nCalendarSecond = nValue;
			else
				pApp->m_nCalendarSecond += nIncrement;

			break;
		}
		case DND_CALENDAR_MOD_SEGMENT:
		{
			if(bSet)
				m_nSegment = nValue;
			else
				m_nSegment += nIncrement;

			break;
		}
		case DND_CALENDAR_MOD_ROUND:
		{
			if(bSet)
				m_nRound = nValue;
			else
			{
				m_nRound += nIncrement;
				pApp->m_nGlobalRound += nIncrement;
			}

			break;
		}
		case DND_CALENDAR_MOD_TURN:
		{
			if(bSet)
			{
				m_nTurn = nValue;
					
			}
			else
			{
				m_nTurn += nIncrement;
				pApp->m_nGlobalRound += 10 * nIncrement;
			}
			break;
		}
		case DND_CALENDAR_MOD_HOUR:
		{
			if(bSet)
			{
				m_nHour = nValue;
					
			}
			else
			{
				m_nHour += nIncrement;
				pApp->m_nGlobalRound += 60 * nIncrement;
			}
			break;
		}
		case DND_CALENDAR_MOD_DAY:
		{
			if(bSet)
			{
				m_nDayofMonth = nValue;
					
			}
			else
			{
				m_nDayofMonth += nIncrement;
				pApp->m_nGlobalRound += 60 * 24 * nIncrement;
			}

			break;
		}
		case DND_CALENDAR_MOD_MONTH:
		{
			if(bSet)
				m_nMonth = nValue;
			else
			{
				m_nMonth += nIncrement;
				pApp->m_nGlobalRound += 60 * 24 * 30 * nIncrement;
			}

			break;
		}
		case DND_CALENDAR_MOD_YEAR:
		{
			if(bSet)
				m_nYear = nValue;
			else
			{
				m_nYear += nIncrement;
				pApp->m_nGlobalRound += 60 * 24 * 30 * 365 * nIncrement;
			}

			break;
		}

	}

	if(pApp->m_nCalendarSecond < 0)
	{
		pApp->m_nCalendarSecond = 0;
	}
	if(m_nSegment < 0)
	{
		m_nSegment = 9;
		--m_nRound;
	}
	if(m_nRound < 0)
	{
		m_nRound = 9;
		--m_nTurn;
	}
	if(m_nTurn < 0)
	{
		m_nTurn = 5;
		--m_nHour;
	}
	if(m_nHour < 0)
	{
		m_nHour = 23;
		--m_nDayofWeek;
		--m_nDayofMonth;
	}
	if(m_nDayofWeek < 0)
	{
		m_nDayofWeek = 0;
	}
	if(m_nDayofMonth < 0)
	{
		m_nDayofMonth = 0;
	}

	//////////////////////////////

	if(pApp->m_nCalendarSecond >= 6)
	{
		pApp->m_nCalendarSecond = 0;
		++m_nSegment;
	}
	if(m_nSegment >= 10)
	{
		m_nSegment = 0;
		++m_nRound;
		++pApp->m_nGlobalRound;
	}
	if(m_nRound >= 10)
	{
		m_nRound = 0;
		++m_nTurn;
	}
	if(m_nTurn >= 6)
	{
		m_nTurn = 0;
		++m_nHour;
	}
	if(m_nHour >= 24)
	{
		m_nHour = 0;
		++m_nDayofMonth;
	}
	if(m_nDayofMonth > pApp->m_pSelectedCalendar->m_nDaysInMonth[m_nMonth]-1)
	{
		m_nDayofMonth = 0;
		++m_nMonth;
	}
	if(m_nMonth > pApp->m_pSelectedCalendar->m_nMonthsInYear-1)
	{
		m_nMonth = 0;
		++m_nYear;
	}

	if(m_nYear < pApp->m_pSelectedCalendar->m_nBaseYear)
	{
		m_nYear = pApp->m_pSelectedCalendar->m_nBaseYear;
	}

	if(m_nYear > pApp->m_pSelectedCalendar->m_nBaseYear + 50)
	{
		m_nYear = pApp->m_pSelectedCalendar->m_nBaseYear + 50;
	}

	m_nDayofWeek = pApp->m_pSelectedCalendar->FindDayOfWeek(m_nMonth, m_nDayofMonth, m_nYear);

}

CString cDNDParty::GetCurrentMonthName()
{
	CString szRetVal = _T("");

	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	if (pApp->m_pSelectedCalendar != NULL)
	{
		if (m_nMonth >=0 && m_nMonth <= pApp->m_pSelectedCalendar->m_nMonthsInYear - 1)
		{
			szRetVal = pApp->m_pSelectedCalendar->m_szMonthNames[m_nMonth];
		}
	}

	return szRetVal;
}

CString cDNDParty::GetCurrentDayName()
{
	CString szRetVal = _T("");

	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	if (pApp->m_pSelectedCalendar != NULL)
	{
		m_nDayofWeek = pApp->m_pSelectedCalendar->FindDayOfWeek(m_nMonth, m_nDayofMonth, m_nYear);

		if (m_nDayofWeek >= 0 && m_nDayofWeek <= pApp->m_pSelectedCalendar->m_nDaysInWeek - 1)
		{
			szRetVal = pApp->m_pSelectedCalendar->m_szDayNames[m_nDayofWeek];
		}
	}

	return szRetVal;
}

void cDNDParty::CollapseRoute()
{
	cDNDWayPoint _WayPoints[MAX_PARTY_WAYPOINTS];

	int nCount = 1;

	for(int i=1; i < MAX_PARTY_WAYPOINTS; ++i)
	{
		if(!m_WayPoints[i].IsClear())
		{
			_WayPoints[nCount].Copy(&m_WayPoints[i]);
			++nCount;
		}
	}

	memcpy(m_WayPoints, _WayPoints, MAX_PARTY_WAYPOINTS * sizeof(cDNDWayPoint));

	m_WayPoints[0].m_fX = m_fPartyLocationX;
	m_WayPoints[0].m_fY = m_fPartyLocationY;

}

float cDNDParty::TravelRoute(float fDistanceToTravel)
{
	
	if(m_WayPoints[1].IsClear())
	{
		return 0.0f;
	}

	int nLoops = 0;
	float fDistanceTravelled = 0.0f;
	CollapseRoute();

	do
	{
		m_WayPoints[0].m_fX = m_fPartyLocationX;
		m_WayPoints[0].m_fY = m_fPartyLocationY;

		float fDistance = GetDistance(m_WayPoints[0].m_fX, m_WayPoints[0].m_fY, m_WayPoints[1].m_fX, m_WayPoints[1].m_fY);

		if(fDistance < 1.0f)
		{
			m_fPartyLocationX = m_WayPoints[1].m_fX;
			m_fPartyLocationY = m_WayPoints[1].m_fY;

			fDistanceTravelled += fDistance;
			m_WayPoints[1].Clear();
			CollapseRoute();
			
			if(m_WayPoints[1].IsClear())
			{
				return fDistanceTravelled;
			}
		}

		float dX = m_WayPoints[1].m_fX - m_WayPoints[0].m_fX;
		float dY = m_WayPoints[1].m_fY - m_WayPoints[0].m_fY;

		float mX = dX/fDistance;
		float mY = dY/fDistance;

		mX /= 100.0f;
		mY /= 100.0f;

		m_fPartyLocationX += mX;
		m_fPartyLocationY += mY;

		float fDistanceStep = GetDistance(m_fPartyLocationX, m_fPartyLocationY, m_WayPoints[0].m_fX, m_WayPoints[0].m_fY);

		fDistanceTravelled += fDistanceStep;

		m_WayPoints[0].m_fX = m_fPartyLocationX;
		m_WayPoints[0].m_fY = m_fPartyLocationY;

		fDistance = GetDistance(m_WayPoints[0].m_fX, m_WayPoints[0].m_fY, m_WayPoints[1].m_fX, m_WayPoints[1].m_fY);

		//TRACE("%d: Travelled %0.3f Total %0.3f Distance to WP %0.3f\n", nLoops, fDistanceStep, fDistanceTravelled, fDistance);

		++nLoops;

	} while(fDistanceTravelled < fDistanceToTravel);


	MarkChanged();

	return fDistanceTravelled;

}

BOOL cDNDParty::EatRations()
{
	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	BOOL bRetVal = TRUE;

	WORD wID;
	for (POSITION pos = pApp->m_CharacterViewMap.GetStartPosition(); pos != NULL; )
	{
		PDNDCHARVIEWDLG pCharDlg = NULL;
		pApp->m_CharacterViewMap.GetNextAssoc(pos,wID,pCharDlg);

		if(pCharDlg != NULL && pCharDlg->m_pCharacter != NULL && pCharDlg->m_pCharacter->m_dwCharacterID)
		{
			if(CharacterIsPartyMember(pCharDlg->m_pCharacter))
			{
				if(pCharDlg->m_pCharacter->EatRations() == FALSE)
				{
					bRetVal = FALSE;
				}
			}
		}
	}

	return bRetVal;
	
}

/////////////////////////////////////////////

void cDNDParty::MarkChanged()
{
	++m_lSaveFlag;
}

void cDNDParty::MarkSaved()
{
	m_lSaveFlag = 0L;
	m_lSavedFlag = 0L;

	for(int i = 0; i < MAX_NPC_PARTY_MEMBERS-1; ++i)
	{
		if(m_NPCs[i].m_dwCharacterID != 0L)
		{
			m_NPCs[i].MarkSaved();
		}
	}
}
	
BOOL cDNDParty::IsChanged()
{
	if(m_lSaveFlag != m_lSavedFlag)
		return TRUE;

	for(int i = 0; i < MAX_NPC_PARTY_MEMBERS-1; ++i)
	{
		if(m_NPCs[i].m_dwCharacterID != 0L)
		{
			if(m_NPCs[i].IsChanged())
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

/////////////////////////////////////////////

BOOL cDNDCharacter::IsAlive()
{
	if(m_nCurrentDamage < m_nHitPoints)
		return TRUE;

	return FALSE;
}

void cDNDCharacter::MarkChanged()
{
	++m_lSaveFlag;
}

void cDNDCharacter::MarkSaved()
{
	m_lSaveFlag = 0L;
	m_lSavedFlag = 0L;
}
	
BOOL cDNDCharacter::IsChanged()
{
	if(m_lSaveFlag != m_lSavedFlag)
		return TRUE;

	return FALSE;
}

void cDNDCharacter::ValidateInventory()
{

	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	cDNDObject _Inventory[MAX_CHARACTER_INVENTORY];

	int nCount = 0;

	for(int i = 0; i< MAX_CHARACTER_INVENTORY; ++i)
	{
		if(m_Inventory[i].m_wTypeId != 0)
		{
			if(m_Inventory[i].m_dwContainerID != 0)
			{
				cDNDObject * pContObject = FindObjectInInventory(m_Inventory[i].m_dwContainerID);
				if(pContObject == NULL)
				{
					m_Inventory[i].m_dwContainerID = 0;
				}
			}

			m_Inventory[i].CopyFull(&_Inventory[nCount]);

			if(m_Inventory[i].m_lAmount == 0)
			{
				m_Inventory[i].m_lAmount = 1; // can't have none of something right ?
			}

			++nCount;
		}
	}

	memcpy(m_Inventory, _Inventory, MAX_CHARACTER_INVENTORY * sizeof(cDNDObject));

	cDNDObject *pInvItem = FindObjectInInventory(m_SelectedAmmo.m_dwObjectID);
	if(pInvItem != NULL)
	{
		m_SelectedAmmo.m_lAmount = pInvItem->m_lAmount;
	}
}

void cDNDCharacter::StackInventory()
{
	ValidateInventory();

	for(int i = 0; i < MAX_CHARACTER_INVENTORY-1; ++i)
	{
		POBJECTTYPE pObj_1 = &m_Inventory[i];

		if(pObj_1->m_wTypeId == 0)
			continue;

		if(pObj_1->m_lAmount == 0)
			pObj_1->m_lAmount = 1;

		for(int j = i+1; j < MAX_CHARACTER_INVENTORY; ++j)
		{
			POBJECTTYPE pObj_2 = &m_Inventory[j];

			if(pObj_2->m_wTypeId == 0)
				continue;

			if(pObj_1->IsContainer() || pObj_2->IsContainer())
				continue;

			if(pObj_1->m_dwContainerID != pObj_2->m_dwContainerID)
				continue;

			if(pObj_2->m_wTypeId == pObj_1->m_wTypeId && pObj_2->m_nMagicAdj == pObj_1->m_nMagicAdj)
			{
				if(strcmp(pObj_1->m_szType, pObj_2->m_szType) != 0)
				{
					continue;
				}

				if(pObj_2->m_lAmount == 0)
					pObj_2->m_lAmount = 1;

				pObj_1->m_lAmount += pObj_2->m_lAmount;
				pObj_2->m_wTypeId = 0;
			}
		}

	}

	ValidateInventory();
}

void cDNDCharacter::ValidateInventoryFlags()
{
	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	for(int i = 0; i< MAX_CHARACTER_INVENTORY; ++i)
	{
		if(m_Inventory[i].m_wTypeId != 0)
		{
			POBJECTTYPE pObject = pApp->m_ObjectsIndexedTypeArray.GetAt(m_Inventory[i].m_wTypeId);

			if(pObject != NULL)
			{
				if(m_Inventory[i].m_nFlags != pObject->m_nFlags)
				{
					m_Inventory[i].m_nFlags = pObject->m_nFlags;
				}
			}
			else
			{
				TRACE("WARN: invalid object in ValidateInventoryFlags");
				m_Inventory[i].Init();
			}
		}
	}

	for(int i = 0; i < 5; ++i) // yes 5 !
	{	
		if(m_SelectedWeapons[i].m_wTypeId != 0)
		{
			PWEAPONTYPE pWeapon = pApp->m_WeaponsIndexedTypeArray.GetAt(m_SelectedWeapons[i].m_wTypeId);
			if(pWeapon != NULL)
			{
				memcpy(&m_SelectedWeapons[i].m_nWeaponVSAC, pWeapon->m_nWeaponVSAC, 11*sizeof(int));
			}
			else
			{
				if(IsAmmo(&m_SelectedWeapons[i]) == FALSE)
				{
					TRACE("WARN: invalid weapon in ValidateInventoryFlags");
					m_SelectedWeapons[i].Init();
				}
			}
		}
	}
}

void cDNDObject::RenameScroll()
{
	int nTopSpell = m_nContents[0];

	if(m_bEnchanted)
	{
		return;
	}

	if(!nTopSpell)
	{
		//strcpy(m_szType, "scroll : blank\0");
		int nFlags = m_nFlags;
		nFlags = ((nFlags / 100) % 100) % 10;

		switch(nFlags)
		{
			case 1:	strcpy(m_szType, "scroll, cleric\0"); break;
			case 2:	strcpy(m_szType, "scroll, druid\0"); break;
			case 3:	strcpy(m_szType, "scroll, illusionist\0"); break;
			case 4:	strcpy(m_szType, "scroll, magic-user\0"); break;
		}

		return;
	}

	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	cDNDSpell *pSpell = pApp->m_MasterSpellArray.GetAt(nTopSpell-1);

	if(pSpell != NULL)
	{
		CString szTemp;

		if(m_nContents[1])
		{
			szTemp.Format("scroll : %s ...", pSpell->m_szSpellName);
		}
		else
		{
			szTemp.Format("scroll : %s", pSpell->m_szSpellName);
		}

		strcpy(m_szType, szTemp.Left(31));
		m_szType[31] = 0;
	}
}

void cDNDCharacter::ValidateMagicContainerInventory()
{
	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	for(int i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
	{
		if(m_Inventory[i].IsScroll())
		{
			int nType = m_Inventory[i].m_nContents[0];
			m_Inventory[i].CollapseContents();
			//if(m_Inventory[i].m_nContents[0] != nType)
			{
				m_Inventory[i].RenameScroll();
			}
		}
	}
}

LONG cDNDCharacter::CountContainerCoins(DND_COIN_TYPES nCoinType)
{
	LONG lRetCoins = 0L;

	for(int i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
	{
		if(m_Inventory[i].IsContainer())
		{
			switch(nCoinType)
			{
				case PLATINUM_PIECES:	lRetCoins += m_Inventory[i].m_nContainerPlatinum; break;
				case GOLD_PIECES:		lRetCoins += m_Inventory[i].m_nContainerGold; break;
				case ELECTRUM_PIECES:	lRetCoins += m_Inventory[i].m_nContainerElectrum; break;
				case SILVER_PIECES:		lRetCoins += m_Inventory[i].m_nContainerSilver; break;
				case COPPER_PIECES:		lRetCoins += m_Inventory[i].m_nContainerCopper; break;
			}
		}
	}

	return lRetCoins;
}

void cDNDCharacter::BalanceContainerCoins(POBJECTTYPE pBaseContainer, DND_COIN_TYPES nCoinType)
{

	if(nCoinType == DND_COINS_ALL)
	{
		BalanceContainerCoins(pBaseContainer, PLATINUM_PIECES);
		BalanceContainerCoins(pBaseContainer, GOLD_PIECES);
		BalanceContainerCoins(pBaseContainer, ELECTRUM_PIECES);
		BalanceContainerCoins(pBaseContainer, SILVER_PIECES);
		BalanceContainerCoins(pBaseContainer, COPPER_PIECES);

		return;
	}

	m_lPlatinumCarried = max(m_lPlatinumCarried, 0);
	m_lGoldCarried = max(m_lGoldCarried, 0);
	m_lElectrumCarried = max(m_lElectrumCarried, 0);
	m_lSilverCarried = max(m_lSilverCarried, 0);
	m_lCopperCarried = max(m_lCopperCarried, 0);

	LONG lBaseAmount = 0;

	switch(nCoinType)
	{
		case PLATINUM_PIECES:	lBaseAmount = m_lPlatinumCarried; break;
		case GOLD_PIECES:		lBaseAmount = m_lGoldCarried; break;
		case ELECTRUM_PIECES:	lBaseAmount = m_lElectrumCarried; break;
		case SILVER_PIECES:		lBaseAmount = m_lSilverCarried; break;
		case COPPER_PIECES:		lBaseAmount = m_lCopperCarried; break;
	}


	LONG lCointainerCoins = CountContainerCoins(nCoinType);

	if(lCointainerCoins > lBaseAmount)
	{
		LONG lOverAmount = lCointainerCoins - lBaseAmount;

		BOOL bFoundSome = TRUE;

		while(lOverAmount > 0 && bFoundSome == TRUE)
		{
			bFoundSome = FALSE;

			for(int i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
			{
				m_Inventory[i].m_nContainerPlatinum = max(m_Inventory[i].m_nContainerPlatinum, 0);
				m_Inventory[i].m_nContainerGold = max(m_Inventory[i].m_nContainerGold, 0);
				m_Inventory[i].m_nContainerElectrum = max(m_Inventory[i].m_nContainerElectrum, 0);
				m_Inventory[i].m_nContainerSilver = max(m_Inventory[i].m_nContainerSilver, 0);
				m_Inventory[i].m_nContainerCopper = max(m_Inventory[i].m_nContainerCopper, 0);

				if(pBaseContainer == &m_Inventory[i])
					continue;

				if(m_Inventory[i].IsContainer())
				{
					int *pnCoins = NULL;
					switch(nCoinType)
					{
						case PLATINUM_PIECES:	pnCoins = &m_Inventory[i].m_nContainerPlatinum; break;
						case GOLD_PIECES:		pnCoins = &m_Inventory[i].m_nContainerGold; break;
						case ELECTRUM_PIECES:	pnCoins = &m_Inventory[i].m_nContainerElectrum; break;
						case SILVER_PIECES:		pnCoins = &m_Inventory[i].m_nContainerSilver; break;
						case COPPER_PIECES:		pnCoins = &m_Inventory[i].m_nContainerCopper; break;
					}

					if(*pnCoins > 0)
					{
						*pnCoins -= 1;
						lOverAmount -= 1;
						bFoundSome = TRUE;
					}
				}
			}
		}

		if(lOverAmount > 0 && pBaseContainer != NULL)
		{
			int *pnCoins = NULL;

			switch(nCoinType)
			{
				case PLATINUM_PIECES:	pnCoins = &pBaseContainer->m_nContainerPlatinum; break;
				case GOLD_PIECES:		pnCoins = &pBaseContainer->m_nContainerGold; break;
				case ELECTRUM_PIECES:	pnCoins = &pBaseContainer->m_nContainerElectrum; break;
				case SILVER_PIECES:		pnCoins = &pBaseContainer->m_nContainerSilver; break;
				case COPPER_PIECES:		pnCoins = &pBaseContainer->m_nContainerCopper; break;
			}

			if(pnCoins != NULL)
			{
				*pnCoins -= lOverAmount;
			}
		}

	}

}

BOOL cDNDCharacter::AddToInventory(cDNDObject *pNewObject, int *nReturnSlot)
{
	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	cDNDObject _LocalObject;
	pNewObject->CopyTo(&_LocalObject);
	_LocalObject.m_nMagicAdj = pNewObject->m_nMagicAdj;

	CString szCheck = _LocalObject.m_szType;

	int nAmountFinderLeft = szCheck.Find("(");

	if(nAmountFinderLeft >= 0) //this is a pre-stacked inventory item
	{
		CString szAmount = szCheck.Mid(nAmountFinderLeft+1);
		szAmount.Replace(")", "");

		LONG lAmount = atol(szAmount.GetBuffer(0));

		cDNDObject *pSwapObject = pApp->m_ObjectsIndexedTypeArray.GetAt(_LocalObject.m_nWeight);
		if(pSwapObject != NULL)
		{
			pSwapObject->CopyTo(&_LocalObject);
			_LocalObject.m_lAmount = lAmount;
		}	
	}


	for(int i = 0; i< MAX_CHARACTER_INVENTORY; ++i)
	{
		if(m_Inventory[i].m_wTypeId == 0)
		{
			_LocalObject.CopyTo(&m_Inventory[i]);
			m_Inventory[i].m_nMagicAdj = _LocalObject.m_nMagicAdj;
			
			if (m_Inventory[i].m_lAmount == 0)
			{
				m_Inventory[i].m_lAmount = 1;
			}

			if(m_Inventory[i].m_dwObjectID == 0)
			{
				m_Inventory[i].m_dwObjectID = GetUniqueID(); // now it's REAL

				if(nReturnSlot != NULL)
				{
					*nReturnSlot = i;
				}
			}
			
			ValidateInventory();

			return TRUE;
		}
	}

	return FALSE;
}

BOOL cDNDCharacter::AddToInventoryFull(cDNDObject *pNewObject) //add to inventory preserving all data, does not call validate inventory
{
	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	cDNDObject _LocalObject;
	pNewObject->CopyFull(&_LocalObject);

	CString szCheck = _LocalObject.m_szType;

	int nAmountFinderLeft = szCheck.Find("(");

	if(nAmountFinderLeft >= 0) //this is a pre-stacked inventory item
	{
		CString szAmount = szCheck.Mid(nAmountFinderLeft+1);
		szAmount.Replace(")", "");

		LONG lAmount = atol(szAmount.GetBuffer(0));

		cDNDObject *pSwapObject = pApp->m_ObjectsIndexedTypeArray.GetAt(_LocalObject.m_nWeight);
		if(pSwapObject != NULL)
		{
			pSwapObject->CopyFull(&_LocalObject);
			_LocalObject.m_lAmount = lAmount;
		}	
	}

	for(int i = 0; i< MAX_CHARACTER_INVENTORY; ++i)
	{
		if(m_Inventory[i].m_wTypeId == 0)
		{
			_LocalObject.CopyFull(&m_Inventory[i]);

			if(m_Inventory[i].m_dwObjectID == 0)
			{
				m_Inventory[i].m_dwObjectID = GetUniqueID(); // now it's REAL
			}

			return TRUE;
		}
	}

	return FALSE;
}

BOOL cDNDCharacter::BuyItem(int nCost)
{
	if(nCost == 0)
		return TRUE;

	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	float fInflation = ((float)pApp->m_Settings.m_nVendorPriceInflationFactor)/100.0f;
	nCost = (int)((float)nCost * fInflation + 0.5f);

	if(nCost <= 0)
		nCost = 1;

	int nTotalCoppers = 
		m_lCopperCarried +
		m_lSilverCarried * 10 +
		m_lElectrumCarried * 100 +
		m_lGoldCarried * 200 +
		m_lPlatinumCarried * 1000;


	if(nCost > nTotalCoppers)
		return FALSE;

	int nLoops = 10000;

	do
	{

		if(m_lCopperCarried >= nCost)
		{
			m_lCopperCarried -= nCost;

			BalanceContainerCoins(NULL, DND_COINS_ALL);

			MarkChanged();

			return TRUE;
		}

		if(m_lSilverCarried > 0)
		{
			--m_lSilverCarried;
			m_lCopperCarried += 10;
			continue;
		}

		if(m_lElectrumCarried > 0)
		{
			--m_lElectrumCarried;
			m_lSilverCarried += 10;
			continue;
		}

		if(m_lGoldCarried > 0)
		{
			--m_lGoldCarried;
			m_lElectrumCarried += 2;
			continue;
		}

		if(m_lPlatinumCarried > 0)
		{
			--m_lPlatinumCarried;
			m_lGoldCarried += 5;
			continue;
		}
		else
		{
			break;
		}

		--nLoops;  //just in case we fracked up

	} while(nLoops > 0);


	return FALSE;

}


void cDNDCharacter::ValidateProficiencies()
{
	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	cDNDObject _Proficiencies[MAX_WEAPON_PROFICIENCIES];

	int nCount = 0;

	int nProfPenalty = 0;
	int nProffs = CalculateWeaponProficiencies(this, &nProfPenalty);

	for(int i = 0; i< nProffs; ++i)
	{
		if(m_WeaponProficiencies[i].m_wTypeId != 0 && m_WeaponProficiencies[i].m_ObjectType == DND_OBJECT_TYPE_WEAPON)
		{
			m_WeaponProficiencies[i].CopyFull(&_Proficiencies[nCount]);
			++nCount;
		}
	}

	memcpy(m_WeaponProficiencies, _Proficiencies, MAX_WEAPON_PROFICIENCIES * sizeof(cDNDObject));
}

void cDNDCharacter::AddWeaponProficiency(PWEAPONTYPE pWeapon)
{
	for(int i = 0; i< MAX_WEAPON_PROFICIENCIES; ++i)
	{
		if(m_WeaponProficiencies[i].m_wTypeId == pWeapon->m_wTypeId)
			break;  // no duplicates

		if(m_WeaponProficiencies[i].m_wTypeId == 0)
		{
			POBJECTTYPE pObj = (POBJECTTYPE)pWeapon;
			pObj->CopyFull(&m_WeaponProficiencies[i]);
			break;
		}
	}
}

BOOL cDNDCharacter::IsProficientWithWeapon(PWEAPONTYPE pWeapon, int *nSpecializedToHitBonus, int *nSpecializedDamageBonus)
{
	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	PWEAPONTYPE pTestWeapon = pWeapon;

	*nSpecializedToHitBonus = 0;
	*nSpecializedDamageBonus = 0;

	if(pTestWeapon->m_nExceptionalStrength && IsMissileWeapon(pTestWeapon)) 
	{
		WORD wId = pTestWeapon->m_wTypeId - 1; //look one back in the list
		pTestWeapon = pApp->m_WeaponsIndexedTypeArray.GetAt(wId);
	}

	if(pTestWeapon != NULL)
	{
		for(int i = 0; i< MAX_WEAPON_PROFICIENCIES; ++i)
		{
			if(m_WeaponProficiencies[i].m_wTypeId == pTestWeapon->m_wTypeId)
			{
				if(GetWeaponSpecializationLevel())
				{
					if(m_WeaponProficiencies[i].m_nContents[0] == 1)
					{
						*nSpecializedToHitBonus = 1;
						*nSpecializedDamageBonus = 2;
					}
					else if(m_WeaponProficiencies[i].m_nContents[0] == 2)
					{
						*nSpecializedToHitBonus = 3;
						*nSpecializedDamageBonus = 3;
					}
				}

				return TRUE;
			}
		}
	}

	if (m_bIsNPC) // finally check here in case there were specialization bonuses
	{
		return TRUE;
	}

	return FALSE;
}

char *cDNDCharacter::GetUnarmedDamageDesc()
{
	static CString szRetVal = _T("");

	int nSTRDamAdj = 0;
	int nWeightAllow = 0;
	int nOpenDoors = 0;
	int nBendBars = 0;

	int nSTRToHitAdj = CalculateSTRAdjustments(this, &nSTRDamAdj, &nWeightAllow, &nOpenDoors, &nBendBars);

	if(m_Class[0] == DND_CHARACTER_CLASS_MONK)
	{
		CString szTemp;

		switch(m_nLevel[0])
		{
			default:
			case 1:		szTemp = "1-3"; break;
			case 2:		szTemp = "1-4"; break;
			case 3:		szTemp = "1-6"; break;
			case 4:		szTemp = "1-6"; break;
			case 5:		szTemp = "2-7"; break;
			case 6:		szTemp = "2-8"; break;
			case 7:		szTemp = "3-9"; break;
			case 8:		szTemp = "2-12"; break;
			case 9:		szTemp = "3-12"; break;
			case 10:	szTemp = "3-13"; break;
			case 11:	szTemp = "4-13"; break;
			case 12:	szTemp = "4-16"; break;
			case 13:	szTemp = "5-17"; break;
			case 14:	szTemp = "5-20"; break;
			case 15:	szTemp = "6-24"; break;
			case 16:	szTemp = "5-30"; break;
			case 17:	szTemp = "8-24"; break;
		}

		if(nSTRDamAdj)
			szRetVal.Format("%s +%d", szTemp, nSTRDamAdj);
		else
			szRetVal.Format("%s", szTemp);

		return szRetVal.GetBuffer(0);
	}

	if(nSTRDamAdj)
		szRetVal.Format("( 1-10 + %d) / 4", nSTRDamAdj);
	else
		szRetVal.Format("( 1-10 ) / 4");

	return szRetVal.GetBuffer(0);
}

void cDNDCharacter::LockAndLoad()
{
	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	if(IsThrowableWeapon(m_SelectedWeapons[0].m_nFlags))
	{
		m_SelectedWeapons[0].CopyTo(&m_SelectedAmmo);
		m_SelectedAmmo.m_dwObjectID = m_SelectedWeapons[0].m_dwObjectID;
		m_SelectedAmmo.m_nMagicAdj = m_SelectedWeapons[0].m_nMagicAdj;
		m_SelectedAmmo.m_lAmount = max(m_SelectedWeapons[0].m_lAmount, 1);
	}
	else if(IsMissileWeapon(&m_SelectedWeapons[0]))
	{
		for(int i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
		{
			if(m_Inventory[i].m_ObjectType == DND_OBJECT_TYPE_AMMO && IsValidAmmoForWeapon(&m_SelectedWeapons[0], &m_Inventory[i]))
			{
				PWEAPONTYPE pWeapon = pApp->m_AmmunitionIndexedTypeArray.GetAt(m_Inventory[i].m_wTypeId);

				if(pWeapon != NULL)
				{
					pWeapon->CopyTo(&m_SelectedAmmo);
					m_SelectedAmmo.m_dwObjectID = m_Inventory[i].m_dwObjectID;
					m_SelectedAmmo.m_nMagicAdj = m_Inventory[i].m_nMagicAdj;
					m_SelectedAmmo.m_lAmount = m_Inventory[i].m_lAmount;
				}
			}
		}
	}
	else
	{
		memset(&m_SelectedAmmo, 0, sizeof(cDNDWeapon));
	}
}

cDNDObject *cDNDCharacter::FindObjectInInventory(DWORD dwID)
{
	for(int i = 0; i< MAX_CHARACTER_INVENTORY; ++i)
	{
		if(m_Inventory[i].m_dwObjectID == dwID)
		{
			return &m_Inventory[i];
		}
	}

	return NULL;
}

cDNDObject *cDNDCharacter::FindObjectTypeInInventory(WORD wID, BOOL bSkipMagical)
{
	for (int i = 0; i< MAX_CHARACTER_INVENTORY; ++i)
	{
		if (m_Inventory[i].m_wTypeId == 0)
		{
			continue;
		}

		if (bSkipMagical && m_Inventory[i].m_nMagicAdj)
		{
			continue;
		}

		if (m_Inventory[i].m_wTypeId == wID)
		{
			return &m_Inventory[i];
		}
	}

	return NULL;
}

cDNDObject *cDNDCharacter::FindObjectClassificationInInventory(DND_OBJECT_TYPES nType, BOOL bSkipMagical)
{
	for (int i = 0; i< MAX_CHARACTER_INVENTORY; ++i)
	{
		if (m_Inventory[i].m_wTypeId == 0)
		{
			continue;
		}

		if (bSkipMagical && m_Inventory[i].m_nMagicAdj)
		{
			continue;
		}

		if (m_Inventory[i].m_ObjectType == nType)
		{
			return &m_Inventory[i];
		}
	}

	return NULL;
}

cDNDObject *cDNDCharacter::FindObjectNameInInventory(char *szName, BOOL bSkipMagical)
{
	for (int i = 0; i< MAX_CHARACTER_INVENTORY; ++i)
	{
		if (bSkipMagical && m_Inventory[i].m_nMagicAdj)
		{
			continue;
		}

		if (strcmp(m_Inventory[i].m_szType, szName) == 0)
		{
			return &m_Inventory[i];
		}
	}

	return NULL;
}

BOOL cDNDCharacter::ObjectIsInContainer(cDNDObject *pObj, DWORD dwParentContainerID, int nLevel) // returns true if pObj is stored in dwParentContainerID
{
	if(nLevel >= MAX_CHARACTER_INVENTORY)
	{
		return FALSE;		//don't go down the rabbit hole
	}

	if(pObj == NULL)
	{
		return FALSE;	// wtf ??
	}

	cDNDObject *pParentObj = FindObjectInInventory(pObj->m_dwContainerID);

	if(pParentObj == NULL)
	{
		return FALSE;
	}

	if(pParentObj->m_dwObjectID == dwParentContainerID)
	{
		return TRUE;
	}
	
	return ObjectIsInContainer(pParentObj, dwParentContainerID, nLevel+1);
}

int cDNDCharacter::CountObjectsInContainer(cDNDObject *pObj)
{
	if(pObj->IsContainer() == FALSE)
	{
		return 1;
	}

	int nCount = 0;

	for(int i = 0; i< MAX_CHARACTER_INVENTORY; ++i)
	{
		if(m_Inventory[i].m_dwObjectID == pObj->m_dwObjectID)
		{ 
			++nCount;
		}
		if(m_Inventory[i].m_dwContainerID)
		{
			if(ObjectIsInContainer(&m_Inventory[i], pObj->m_dwObjectID, 0))
			{
				++nCount;
			}
		}

	}

	return nCount;
}

BOOL cDNDCharacter::InventorySlotIsInBagOfHolding(POBJECTTYPE pObject, int nLevel)
{
	if(nLevel >= MAX_CHARACTER_INVENTORY)
	{
		return FALSE;		//don't go down the rabbit hole
	}

	if(pObject->IsMagicBag())
		return TRUE;

	if(pObject->m_dwContainerID)
	{
		POBJECTTYPE pNextObject = FindObjectInInventory(pObject->m_dwContainerID);

		if(pNextObject != NULL)
		{
			return InventorySlotIsInBagOfHolding(pNextObject, nLevel+1);
		}
		else
		{
			pObject->m_dwContainerID = 0; // error correct
		}
	}

	return FALSE;
}

int cDNDCharacter::GetInventoryOpenSlots()
{
	int nCount = 0;

	ValidateInventory();

	for(int i = 0; i< MAX_CHARACTER_INVENTORY; ++i)
	{
		if(m_Inventory[i].m_dwObjectID == 0)
		{
			++nCount;
		}
	}

	return nCount;
}

BOOL cDNDCharacter::IsEquippingObject(POBJECTTYPE pObject)
{
	if(m_HelmWorn.m_dwObjectID == pObject->m_dwObjectID)
		return TRUE;

	if(m_ArmorWorn.m_dwObjectID == pObject->m_dwObjectID)
		return TRUE;

	if(m_ShieldWorn.m_dwObjectID == pObject->m_dwObjectID)
		return TRUE;

	if(m_RingsWorn[0].m_dwObjectID == pObject->m_dwObjectID)
		return TRUE;

	if(m_RingsWorn[1].m_dwObjectID == pObject->m_dwObjectID)
		return TRUE;

	return FALSE;
}

/*
typedef enum
{
	DND_LANGUAGE_NONE =		 0,
	DND_LANGUAGE_COMMON	=	 1,

	DND_LANGUAGE_DWARVISH = 16,
	DND_LANGUAGE_ELVISH	=	17,
	DND_LANGUAGE_GOBLIN =	26,
	DND_LANGUAGE_GNOLL =	27,
	DND_LANGUAGE_GNOME =	28,
	DND_LANGUAGE_HALFLING = 29,
	DND_LANGUAGE_HOBGOBLIN = 30,
	DND_LANGUAGE_KOBOLD =	31,
	DND_LANGUAGE_ORCISH =	44,
	DND_LANGUAGE_UNDERCOMMON =	55

} DND_LANGUAGES;

*/

void cDNDCharacter::ValidateLanguages()
{
	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	for(int i = 0; i < MAX_CHARACTER_LANGUAGES; ++i)
	{
		if(m_nLanguages[i] < 0 || m_nLanguages[i] > pApp->m_nLastLanguageIndex)
		{
			m_nLanguages[i] = 0;
		}
	}

	if(m_nLanguages[0] == 0)
	{
		memset(m_nLanguages, 0, MAX_CHARACTER_LANGUAGES * sizeof(int));

		int nLang = 0;

		switch(m_nRace)
		{
			case DND_CHARACTER_RACE_HUMAN:
			{
				m_nLanguages[nLang++] =  DND_LANGUAGE_COMMON;

				break;
			}
			case DND_CHARACTER_RACE_ELF:
			{
				m_nLanguages[nLang++] =  DND_LANGUAGE_ELVISH;
				m_nLanguages[nLang++] =  DND_LANGUAGE_COMMON;

				m_nLanguages[nLang++] =  DND_LANGUAGE_GNOME;
				m_nLanguages[nLang++] =  DND_LANGUAGE_HALFLING;
				m_nLanguages[nLang++] =  DND_LANGUAGE_GOBLIN;
				m_nLanguages[nLang++] =  DND_LANGUAGE_HOBGOBLIN;
				m_nLanguages[nLang++] =  DND_LANGUAGE_ORCISH;
				m_nLanguages[nLang++] =  DND_LANGUAGE_GNOLL;

				break;
			}
			case DND_CHARACTER_RACE_HALF_ELF:
			{
				m_nLanguages[nLang++] =  DND_LANGUAGE_ELVISH;
				m_nLanguages[nLang++] =  DND_LANGUAGE_COMMON;

				m_nLanguages[nLang++] =  DND_LANGUAGE_GNOME;
				m_nLanguages[nLang++] =  DND_LANGUAGE_HALFLING;
				m_nLanguages[nLang++] =  DND_LANGUAGE_GOBLIN;
				m_nLanguages[nLang++] =  DND_LANGUAGE_HOBGOBLIN;
				m_nLanguages[nLang++] =  DND_LANGUAGE_ORCISH;
				m_nLanguages[nLang++] =  DND_LANGUAGE_GNOLL;

				break;
			}
			case DND_CHARACTER_RACE_DWARF:
			{
				m_nLanguages[nLang++] =  DND_LANGUAGE_DWARVISH;
				m_nLanguages[nLang++] =  DND_LANGUAGE_COMMON;
				m_nLanguages[nLang++] =  DND_LANGUAGE_GNOME;
				m_nLanguages[nLang++] =  DND_LANGUAGE_GOBLIN;
				m_nLanguages[nLang++] =  DND_LANGUAGE_KOBOLD;
				m_nLanguages[nLang++] =  DND_LANGUAGE_ORCISH;

				break;
			}
			case DND_CHARACTER_RACE_HALFLING:
			{
				m_nLanguages[nLang++] =  DND_LANGUAGE_HALFLING;
				m_nLanguages[nLang++] =  DND_LANGUAGE_COMMON;
				m_nLanguages[nLang++] =  DND_LANGUAGE_DWARVISH;
				m_nLanguages[nLang++] =  DND_LANGUAGE_ELVISH;
				m_nLanguages[nLang++] =  DND_LANGUAGE_GNOME;
				m_nLanguages[nLang++] =  DND_LANGUAGE_GOBLIN;
				m_nLanguages[nLang++] =  DND_LANGUAGE_ORCISH;

				break;
			}
			case DND_CHARACTER_RACE_GNOME:
			{
				m_nLanguages[nLang++] =  DND_LANGUAGE_GNOME;
				m_nLanguages[nLang++] =  DND_LANGUAGE_COMMON;
				m_nLanguages[nLang++] =  DND_LANGUAGE_DWARVISH;
				m_nLanguages[nLang++] =  DND_LANGUAGE_HALFLING;
				m_nLanguages[nLang++] =  DND_LANGUAGE_GOBLIN;
				m_nLanguages[nLang++] =  DND_LANGUAGE_KOBOLD;

				break;
			}
			case DND_CHARACTER_RACE_HALF_ORC:
			{
				m_nLanguages[nLang++] =  DND_LANGUAGE_COMMON;
				m_nLanguages[nLang++] =  DND_LANGUAGE_ORCISH;
				break;
			}
			case DND_CHARACTER_RACE_DROW:
			{
				m_nLanguages[nLang++] =  DND_LANGUAGE_COMMON;
				m_nLanguages[nLang++] =  DND_LANGUAGE_ELVISH;
				m_nLanguages[nLang++] =  DND_LANGUAGE_GNOME;
				m_nLanguages[nLang++] =  DND_LANGUAGE_UNDERCOMMON;

				break;
			}
		}
	}

	int nLanguages[MAX_CHARACTER_LANGUAGES];
	memset(nLanguages, 0, MAX_CHARACTER_LANGUAGES * sizeof(int));

	int nCount = 0;
	for(int i = 0; i < MAX_CHARACTER_LANGUAGES; ++i)
	{
		if(m_nLanguages[i])
		{
			nLanguages[nCount] = m_nLanguages[i];
			++nCount;
		}
	}

	memcpy(m_nLanguages, nLanguages, MAX_CHARACTER_LANGUAGES * sizeof(int));
}

BOOL cDNDCharacter::CanSpeakLanguage(DND_LANGUAGES nLanguage)
{
	for(int i = 0; i < MAX_CHARACTER_LANGUAGES; ++i)
	{
		if(m_nLanguages[i] == nLanguage)
		{
			return TRUE;
		}
	}

	return FALSE;
}

int cDNDCharacter::GetWeaponSpecializationLevel()
{
	if(g_bUseUnearthedArcana)
	{
		for(int i = 0; i < 4; ++i)
		{
			switch(m_Class[i])
			{
				case DND_CHARACTER_CLASS_FIGHTER:		
				case DND_CHARACTER_CLASS_RANGER:	
				{
					return m_nLevel[i];
				}
				case DND_CHARACTER_CLASS_CUSTOM_1:
				case DND_CHARACTER_CLASS_CUSTOM_2:
				case DND_CHARACTER_CLASS_CUSTOM_3:
				case DND_CHARACTER_CLASS_CUSTOM_4:
				case DND_CHARACTER_CLASS_CUSTOM_5:
				case DND_CHARACTER_CLASS_CUSTOM_6:
				case DND_CHARACTER_CLASS_CUSTOM_7:
				case DND_CHARACTER_CLASS_CUSTOM_8:
				case DND_CHARACTER_CLASS_CUSTOM_9:
				case DND_CHARACTER_CLASS_CUSTOM_10:
				case DND_CHARACTER_CLASS_CUSTOM_11:
				case DND_CHARACTER_CLASS_CUSTOM_12:
				{
					if (GetCustomClass(m_Class[i])->m_bWeaponSpecialization)
					{
						int nStartLevel = GetCustomClass(m_Class[i])->m_nWeaponSpecializationLevel;

						return (max((m_nLevel[i] - nStartLevel + 1), 0));
					}

					return 0;
				}
			}
		}
	}

	return 0;
}

BOOL cDNDCharacter::EquippedRegenerationRing()
{
	return m_RingsWorn[0].IsRegenerationRing() || m_RingsWorn[1].IsRegenerationRing();
}

DND_SPELL_MATERIAL_RETURN_CODES cDNDCharacter::CasterHasSpellMaterialComponents(PSPELL pSpell, int nMultiples, BOOL bCast, BOOL bGetInfo, std::vector<POBJECTINDEXER> *pSpellMaterialComponentsRequiredVector)
{
	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	DND_SPELL_MATERIAL_RETURN_CODES nRetCode = DND_SPELL_MATERIAL_RETURN_CANNOT_CAST;
	DND_SPELL_MATERIAL_RETURN_CODES nSuccessCode = DND_SPELL_MATERIAL_RETURN_CAN_CAST;
	DND_SPELL_MATERIAL_RETURN_CODES nVerifyCode = DND_SPELL_MATERIAL_RETURN_CAN_CAST;


	BOOL bValidateInventory = FALSE;

	pSpellMaterialComponentsRequiredVector->clear();

	int nComponentsRequired = 0;
	int nComponentsFound = 0;
	PDNDSPELLCOMPONENT pSpellComponents = NULL;
	if (pApp->m_SpellToMaterialComponentsMap.Lookup(pSpell->m_nSpellIdentifier, pSpellComponents) && pSpellComponents != NULL)
	{
		for (int i = 0; i < MAX_SPELL_COMPONENT_AND_COLUMNS; ++i)
		{
			pSpellMaterialComponentsRequiredVector[i].clear();
			if (pSpellComponents->m_RequiredComponents[i].size())
			{
				++nComponentsRequired;

				BOOL bFoundComponent = FALSE;
				std::vector<POBJECTINDEXER>::iterator iter;
				for (iter = pSpellComponents->m_RequiredComponents[i].begin(); iter != pSpellComponents->m_RequiredComponents[i].end(); iter++)
				{
					POBJECTINDEXER pIndexer = *iter;

					if (bGetInfo)
					{
						pSpellMaterialComponentsRequiredVector[i].push_back(pIndexer);
						continue;
					}

					//POBJECTTYPE pObjectExists = NULL;
					//pApp->m_ObjectsIndexedTypeArray.Lookup(pIndexer->m_wTypeID, pObjectExists);

					POBJECTTYPE pObject = FindObjectTypeInInventory(pIndexer->m_wTypeID, TRUE);
					if (pObject != NULL)
					{
						if (pObject->m_lAmount == 0)
						{
							pObject->m_lAmount = 1;
						}

						if (pObject->m_lAmount >= pIndexer->m_nAmount * nMultiples)
						{
							++nComponentsFound;
							bFoundComponent = TRUE;

							if (pSpell->m_ClassBook == DND_CHARACTER_CLASS_DRUID || pSpell->m_ClassBook == DND_CHARACTER_SPELL_CLASS_RANGER_DRUID)
							{
								DND_SPELL_MATERIAL_RETURN_CODES nTempCode = DND_SPELL_MATERIAL_RETURN_CAN_CAST;

								CString szName = pObject->m_szType;

								if(szName.Find("mistletoe, lesser") >= 0)
								{
									nTempCode = DND_SPELL_MATERIAL_RETURN_CAN_CAST_100_75_100;
								}
								else if (szName.Find("mistletoe, borrowed") >= 0)
								{
									nTempCode = DND_SPELL_MATERIAL_RETURN_CAN_CAST_75_50_100;
								}
								else if (szName == "holly")
								{
									nTempCode = DND_SPELL_MATERIAL_RETURN_CAN_CAST_75_50_75;
								}
								else if (szName.Find("oak leaves") >= 0)
								{
									nTempCode = DND_SPELL_MATERIAL_RETURN_CAN_CAST_50_50_50;
								}

								if (nTempCode < nSuccessCode)
								{
									nSuccessCode = nTempCode;
								}
							}

							pSpellMaterialComponentsRequiredVector[i].clear();

							int nFlags = (pIndexer->m_nFlags % 10);
							if (bCast && nFlags == 1)  // remove consumable components if we are casting the spell
							{
								pObject->m_lAmount -= pIndexer->m_nAmount;
								if (pObject->m_lAmount <= 0)
								{
									cDNDObject _BlankObj;
									_BlankObj.CopyFull(pObject);
									bValidateInventory = TRUE;
								}
							}

							break;
						}
					}
					else if(bCast == FALSE)
					{
						pSpellMaterialComponentsRequiredVector[i].push_back(pIndexer);

						if (pIndexer->m_wTypeID == 0)
						{
							if (nSuccessCode == DND_SPELL_MATERIAL_RETURN_CAN_CAST)
							{
								nVerifyCode = DND_SPELL_MATERIAL_RETURN_CAN_CAST_WITH_VERIFICATION; // this is an oddball, like verifying we have a sacrificial creature
								++nComponentsFound;
								break;
							}
						}
					}
				}
			}
		}

		if (nComponentsFound >= nComponentsRequired)
		{
			nRetCode = nSuccessCode;
		}
	}
	else
	{
		nRetCode = DND_SPELL_MATERIAL_RETURN_CAN_CAST;  // no material components required
	}

	if (bValidateInventory)
	{
		ValidateInventory();
		MarkChanged();
	}

	if (nVerifyCode == DND_SPELL_MATERIAL_RETURN_CAN_CAST_WITH_VERIFICATION && nRetCode == DND_SPELL_MATERIAL_RETURN_CAN_CAST)
	{
		nRetCode = nVerifyCode;
	}

	if (bCast == TRUE)
	{
		nRetCode = nSuccessCode;
	}

	return nRetCode;
}

BOOL cDNDCharacter::BuySpellComponents(PSPELL pSpell, int nMultiples, BOOL bNoCost, std::vector<POBJECTINDEXER> *pSpellMaterialComponentsRequiredVector)
{
	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	BOOL bRetVal = TRUE;

	// have we got a bag ??
	POBJECTTYPE pBagObject = FindObjectClassificationInInventory(DND_OBJECT_TYPE_MATERIAL_COMPONENT_CONTAINER, FALSE);
	DWORD dwContainerID = 0;

	if (pBagObject == NULL)
	{
		pBagObject = pApp->m_ObjectsIndexedTypeArray.GetAt(10114);

		if (pBagObject != NULL)
		{
			int nSlot = -1;
			AddToInventory(pBagObject, &nSlot);

			if (nSlot >= 0 && nSlot < MAX_CHARACTER_INVENTORY)
			{
				pBagObject = &m_Inventory[nSlot];
				strcpy(pBagObject->m_szType, "bag, spell component");
				pBagObject->m_ObjectType = DND_OBJECT_TYPE_MATERIAL_COMPONENT_CONTAINER;
			}
		}
	}

	if (pBagObject != NULL)
	{
		dwContainerID = pBagObject->m_dwObjectID;
	}

	for (int i = 0; i < MAX_SPELL_COMPONENT_AND_COLUMNS; ++i)
	{
		if (pSpellMaterialComponentsRequiredVector[i].size())
		{
			cDNDObject *pBuyObject = NULL;
			int nAmountNeeded = 0;

			std::vector<POBJECTINDEXER>::iterator iter;
			for (iter = pSpellMaterialComponentsRequiredVector[i].begin(); iter != pSpellMaterialComponentsRequiredVector[i].end(); iter++)
			{
				POBJECTINDEXER pIndexer = *iter;

				nAmountNeeded = pIndexer->m_nAmount * nMultiples;

				int nFlags = (pIndexer->m_nFlags % 10);

				if (nFlags == 0) // if component is not consumed by the spell, we only need one of them
				{
					nAmountNeeded = 1;
				}

				cDNDObject *pCheckObject = pApp->m_ObjectsIndexedTypeArray.GetAt(pIndexer->m_wTypeID);

				POBJECTTYPE pOwnedObject = FindObjectTypeInInventory(pIndexer->m_wTypeID, TRUE);
				if (pOwnedObject != NULL)
				{
					pOwnedObject->m_lAmount = max(pOwnedObject->m_lAmount, 1);

					if (pOwnedObject->m_lAmount >= nAmountNeeded && nFlags == 0) // if this is not a consumable spell component, and we already have one, we don't need another
					{
						pBuyObject = NULL;
					}
					else
					{
						pBuyObject = pCheckObject;
					}
					//else
					//{
					//	nAmountNeeded -= pOwnedObject->m_lAmount;  // comment out - don't do this, we might need the same component for multiple spells
					//}

				}
				else
				{
					pBuyObject = pCheckObject;
				}
			}

			if (pBuyObject != NULL)
			{
				for (int i = 0; i < nAmountNeeded; ++i)
				{
					int nCost = pBuyObject->m_nCost;

					if (bNoCost)
					{
						nCost = 0;
					}

					if (BuyItem(nCost))
					{
						int nSlot = -1;
						AddToInventory(pBuyObject, &nSlot);
						m_Inventory[nSlot].m_dwContainerID = dwContainerID;
					}
					else
					{
						bRetVal = FALSE;
					}
				}

			}

		}
	}

	StackInventory();
	MarkChanged();

	return bRetVal;
}

BOOL cDNDCharacter::BuyAllSpellComponents(BOOL bNoCost)
{
	BOOL bRetVal = TRUE;

	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	std::vector<POBJECTINDEXER> _SpellMaterialComponentsRequiredVector[MAX_SPELL_COMPONENT_AND_COLUMNS];
	cDNDSpellBook *pSpellBook;

	for (int nSpellClass = 0; nSpellClass < 4; ++nSpellClass)
	{
		switch (m_SpellClasses[nSpellClass])
		{
			case DND_CHARACTER_CLASS_CLERIC:
			case DND_CHARACTER_SPELL_CLASS_PALADIN_CLERIC:
			{
				pSpellBook = pApp->m_SpellBooks.GetAt(DND_CHARACTER_CLASS_CLERIC);
				break;
			}
			case DND_CHARACTER_CLASS_DRUID:
			case DND_CHARACTER_SPELL_CLASS_RANGER_DRUID:
			{
				pSpellBook = pApp->m_SpellBooks.GetAt(DND_CHARACTER_CLASS_DRUID);
				break;
			}
			case DND_CHARACTER_CLASS_MAGE:
			case DND_CHARACTER_SPELL_CLASS_RANGER_MAGE:
			{
				pSpellBook = pApp->m_SpellBooks.GetAt(DND_CHARACTER_CLASS_MAGE);
				break;
			}
			case DND_CHARACTER_CLASS_ILLUSIONIST:
			{
				pSpellBook = pApp->m_SpellBooks.GetAt(DND_CHARACTER_CLASS_ILLUSIONIST);
				break;
			}
			default:
			{
				pSpellBook = NULL;
				continue;
			}
		}

		if (pSpellBook != NULL)
		{
			#if USE_CANTRIPS
			for (int nLevel = 0; nLevel <= MAX_SPELL_LEVELS; ++nLevel)
			#else
			for (int nLevel = 1; nLevel <= MAX_SPELL_LEVELS; ++nLevel)
			#endif
			{
				for (int nSpell = 0; nSpell < MAX_SPELLS_PER_LEVEL; ++nSpell)
				{
					if (m_nSpellsMemorized[nSpellClass][nLevel][nSpell])
					{
						cDNDSpell *pSpell = &pSpellBook->m_Spells[nLevel][nSpell];

						if (pSpell->m_bSpellValid == FALSE)
							continue;

						for (int i = 0; i < MAX_SPELL_COMPONENT_AND_COLUMNS; ++i)
						{
							_SpellMaterialComponentsRequiredVector[i].clear();
						}
						
						int nMultiples = m_nSpellsMemorized[nSpellClass][nLevel][nSpell];
						CasterHasSpellMaterialComponents(pSpell, nMultiples, FALSE, TRUE, _SpellMaterialComponentsRequiredVector);
						if (_SpellMaterialComponentsRequiredVector[0].size())
						{
							if (BuySpellComponents(pSpell, nMultiples, bNoCost, _SpellMaterialComponentsRequiredVector) == FALSE)
							{
								bRetVal = FALSE;
							}
						}
					}
				}
			}
		}
	}


	return bRetVal;

}
	

BOOL cDNDCharacter::CastSpell(cDNDSpellSlot *pSpellSlot, BOOL bFailedToCast)
{
	if(pSpellSlot == NULL || pSpellSlot->m_pSpell == NULL || pSpellSlot->m_pnMemorizedSlot == NULL)
		return FALSE;

	if(pSpellSlot->m_pnMemorizedSlot <= 0)
		return FALSE;

	*pSpellSlot->m_pnMemorizedSlot -= 1;

	ValidateMagicContainerInventory();

	MarkChanged();

	return TRUE;
}

BOOL cDNDCharacter::EatRations()
{
	for(int i = 0; i< MAX_CHARACTER_INVENTORY; ++i)
	{
		//if(m_Inventory[i].m_wTypeId == 10030 || m_Inventory[i].m_wTypeId == 10032)
		if(m_Inventory[i].IsRations())
		{
			--m_Inventory[i].m_lAmount;

			if(m_Inventory[i].m_lAmount <= 0)
			{
				cDNDObject _BlankObj;
				_BlankObj.CopyFull(&m_Inventory[i]);
				ValidateInventory();

				MarkChanged();
			}

			return TRUE;
		}
	}

	return FALSE;
}

int cDNDNonPlayerCharacter::CalculateCombatLevel()
{

	if(m_nHitDice == 0)
	{
		return 0;
	}

	if(m_nHitDice == 1)
	{
		if(m_nHitDicePlus < 0)
			return 1;
		else if(m_nHitDicePlus > 0)
			return 3;
		else
			return 2;
	}

	int nLevel = m_nHitDice / 2 + 3;

	if(nLevel > 11)
		nLevel = 11;


	return nLevel;
}

int cDNDNonPlayerCharacter::CalculateSaveLevel()
{
	if (m_nHitDice == 0)
	{
		return 0;
	}

	if (m_nHitDice == 1)
	{
		if (m_nHitDicePlus < 0)
			return 0;
	}

	int nLevel = m_nHitDice;

	if (m_nHitDicePlus)
	{
		nLevel += 1;
	}
	if (m_nHitDicePlus > 4)
	{
		nLevel += 1;
	}

	return nLevel;
}

BOOL cDNDNonPlayerCharacter::CalculateNPCHitDice(PDNDMONSTERMANUALENTRY pMonster)
{
	BOOL bRetVal = FALSE;

	m_bPresetHitpoints = FALSE;

	if(m_bRolled == FALSE)
	{
		m_nHitDice = 0;
		m_nHitDicePlus = 0;

		for(int j =0 ; j < 4; ++j)
		{
			for(int i = 0; i < 20; ++i)
			{
				m_nHitPointRolls[j][i] = RollD8();
			}
		}

	}

	CString szTemp;
	CString szHD = pMonster->m_szHD;

	//TODO elementals are borked at the moment

	if(szHD.FindOneOf("/") >= 0)
	{
		int nNum = 0;
		int nDiv = 0;

		CString sToken=_T("");
		int i = 0; // substring index to extract
		while (AfxExtractSubString(sToken, szHD, i,'/'))
		{   
			switch(i)
			{
				case 0:  	nNum = atoi(sToken.GetBuffer(0)); break;
				case 1:  	nDiv = atoi(sToken.GetBuffer(0)); break;
				default:
				{
					break;
				}

			}
			i++;
		}

		if(nNum && nDiv)
		{
			m_bPresetHitpoints = TRUE;

			m_nHitDice = nNum / nDiv;
			m_nHitDicePlus = 0;

			m_nHitPoints = max ((m_nHitPointRolls[0][0] * nNum) / nDiv , 1);
			
			bRetVal = TRUE;
		}
	}
	else if(szHD.FindOneOf("+") >= 0 && szHD.FindOneOf("-") >= 0) //cloud giant example
	{
		szHD.Replace("+", "*");
		szHD.Replace("-", "*");
		CString sToken=_T("");
		int i = 0; // substring index to extract
		int nBase = 0;
		int nLow = 0;
		int nHigh = 0;
		while (AfxExtractSubString(sToken, szHD, i,'*'))
		{   
			switch(i)
			{
				case 0:  	nBase = atoi(sToken.GetBuffer(0)); break;
				case 1:  	nLow = atoi(sToken.GetBuffer(0)); break;
				case 2:  	nHigh = atoi(sToken.GetBuffer(0)); break;
				default:
				{
					break;
				}

			}
			i++;
		}

		if(nLow && nHigh)
		{
			int nDiff = (nHigh - nLow) + 1;
			int nRoll = rand() % nDiff;

			m_nHitDice = max (nRoll+nLow , 1) + nBase;

			m_nHitDicePlus = 0;
		}

		bRetVal = TRUE;
	}
	else if(szHD.FindOneOf("to") >= 0 && szHD.FindOneOf("hp") >= 0) //beholder example
	{
		szHD.Replace("to", "*");
		szHD.Replace("hp", "");
		CString sToken=_T("");
		int i = 0; // substring index to extract
		int nLow = 0;
		int nHigh = 0;
		while (AfxExtractSubString(sToken, szHD, i,'*'))
		{   
			switch(i)
			{
				case 0:  	nLow = atoi(sToken.GetBuffer(0)); break;
				case 1:  	nHigh = atoi(sToken.GetBuffer(0)); break;
				default:
				{
					break;
				}

			}
			i++;
		}

		if(nLow && nHigh)
		{
			m_bPresetHitpoints = TRUE;

			int nDiff = (nHigh - nLow) + 1;

			int nRoll = rand() % nDiff;

			m_nHitPoints = max (nRoll+nLow , 1);

			m_nHitDice = m_nHitPoints / 8 + 1;
			m_nHitDicePlus = 0;
		}

		bRetVal = TRUE;
	}
	else if(szHD.FindOneOf("-") >= 0)
	{
		CString sToken=_T("");
		int i = 0; // substring index to extract

		int nLow = 0;
		int nHigh = 0;

		while (AfxExtractSubString(sToken, szHD, i,'-'))
		{   
			switch(i)
			{
				case 0:  	nLow = atoi(sToken.GetBuffer(0)); break;
				case 1:  	nHigh = atoi(sToken.GetBuffer(0)); break;
				default:
				{
					break;
				}

			}
			i++;
		}
		
		if(nHigh <= nLow) // 1 - 1 ?
		{
			m_nHitDice = nLow;
			m_nHitDicePlus = -nHigh;

			bRetVal = TRUE;
		}
		else if(szHD.FindOneOf("hp") >= 0) // 2-5 hp see leprechaun
		{
			m_bPresetHitpoints = TRUE;

			int nDiff = (nHigh - nLow) + 1;

			int nRoll = rand() % nDiff;

			m_nHitPoints = max (  nLow + nRoll, 1);

			m_nHitDice = 1;
			m_nHitDicePlus = (8-nHigh)*-1;
			
			bRetVal = TRUE;
		}
		else // 3 - 8 ?
		{
			int nDiff = (nHigh - nLow) + 1;

			int nRoll = rand() % nDiff;

			m_nHitDice = nLow + nRoll;
			m_nHitDicePlus = 0;

			bRetVal = TRUE;
		}
	
	}
	else if(szHD.FindOneOf("to") >= 0 || szHD.FindOneOf("or") >= 0)
	{
		szHD.Replace("to", "*");
		szHD.Replace("or", "*");
		CString sToken=_T("");
		int i = 0; // substring index to extract
		int nLow = 0;
		int nHigh = 0;
		while (AfxExtractSubString(sToken, szHD, i,'*'))
		{   
			switch(i)
			{
				case 0:  	nLow = atoi(sToken.GetBuffer(0)); break;
				case 1:  	nHigh = atoi(sToken.GetBuffer(0)); break;
				default:
				{
					break;
				}

			}
			i++;
		}

		if(nLow > nHigh)
		{
			int nSwap = nLow;
			nLow = nHigh;
			nHigh = nSwap;
		}

		if(nLow && nHigh)
		{
			int nDiff = (nHigh - nLow) + 1;

			int nRoll = rand() % nDiff;

			if(m_bRolled == FALSE)
			{
				m_nHitDice = nLow + nRoll;
				m_nHitDicePlus = 0;
			}
		}

		bRetVal = TRUE;
	}
	else if(szHD.FindOneOf("+") >= 0)
	{
		CString sToken=_T("");
		int i = 0; // substring index to extract
		while (AfxExtractSubString(sToken, szHD, i,'+'))
		{   
			switch(i)
			{
				case 0:  	m_nHitDice = atoi(sToken.GetBuffer(0)); break;
				case 1:  	m_nHitDicePlus = atoi(sToken.GetBuffer(0)); break;
				default:
				{
					break;
				}

			}
			i++;
		}

		bRetVal = TRUE;
	}
	else if(szHD.FindOneOf("hp") >= 0)
	{
		m_bPresetHitpoints = TRUE;

		szHD.Replace("hp", "");

		int nPos = szHD.FindOneOf("(");
		if( nPos >= 0)
		{
			szTemp = szHD.Left(nPos);
			szHD.Replace(szTemp, "");
			szHD.Replace("(","");
			szHD.Replace(")","");

			m_nHitDice = atoi(szTemp);
		}
		else
		{
			m_nHitDice = min ( 17, (atoi(szHD.GetBuffer(0)) / 8) );
		}
		m_nHitDicePlus = 0;

		memset(m_nHitPointRolls, 0, 4 * 20 * sizeof(int));
		//m_nHitPointRolls[0][0] = atoi(szHD.GetBuffer(0));
		//m_nHitPoints = m_nHitPointRolls[0][0];
		m_nHitPoints = atoi(szHD.GetBuffer(0));

		bRetVal = TRUE;
	}
	else
	{
		m_nHitDice = atoi(szHD.GetBuffer(0));
		bRetVal = TRUE;
	}

	if(!m_bPresetHitpoints)
	{
		m_nHitPoints = 0;
	}

	if(bRetVal && !m_bPresetHitpoints)
	{
		if(m_nHitDice)
		{
			for(int i = 0; i < m_nHitDice; ++i)
			{
				int nRow = i / 20;
				int nCol = i % 20;

				//int nVal = max(m_nHitPointRolls[nRow][nCol] + m_nHitDicePlus , 1);

				int nVal = 0;
				
				if (i == 0)
				{
					nVal = max(m_nHitPointRolls[nRow][nCol] + m_nHitDicePlus, 1);
				}
				else
				{
					nVal = max(m_nHitPointRolls[nRow][nCol], 1);
				}

				//special cases where Gary was smoking something
				if(pMonster->m_szType == _T("Dragon"))
				{
					if(m_nHitPointRolls[0][0] == 0)
					{
						m_nHitPointRolls[0][0] = RollD8();
					}

					if(m_nHitPointRolls[0][0] < m_nDungeonLevel)
					{
						m_nHitPointRolls[0][0] = min(8, m_nDungeonLevel);
					}

					nVal = m_nHitPointRolls[0][0];
				}
		
				m_nHitPoints += nVal;
			}
		}
		else
		{
			m_nHitPoints = m_nHitPointRolls[0][0];
		}
	}

	m_bRolled = TRUE;

	return bRetVal;
}

int RollD3()
{
	int nDie = (rand()%3)+1;

	return nDie;
}


int RollD4()
{
	int nDie = (rand()%4)+1;

	return nDie;
}


int RollD6()
{
	int nDie = (rand()%6)+1;

	return nDie;
}

int RollD8()
{
	int nDie = (rand()%8)+1;

	return nDie;
}

int RollStat()
{
	//roll 4d6 - drop lowest add add them up

	int nDie[4];
	
	nDie[0] = RollD6();
	nDie[1] = RollD6();
	nDie[2] = RollD6();
	nDie[3] = RollD6();

	int nLowest = -1;
	int nLow = 7;
	for(int i = 0; i < 4; ++i)
	{
		if(nDie[i] < nLow)
		{
			nLow = nDie[i];
			nLowest = i;
		}
	}

	nDie[nLowest] = 0;

	int nTotal = nDie[0] + nDie[1] + nDie[2] + nDie[3];

	return nTotal;
}



int RollD10()
{
	int nDie = (rand()%10)+0;

	return nDie;
}

int RollD12()
{
	int nDie = (rand()%12)+1;

	return nDie;
}

std::string fooBar()
{
	std::string something = "avalue";
	return something;
}

int RollD20()
{
	std::string result = fooBar();

	int nDie = (rand()%20)+1;

	return nDie;
}


int RollD00()
{
	int nDie1 = RollD10();
	int nDie2 = RollD10();

	int nDie = nDie1*10 + nDie2;

	return nDie;
}

int RollD100()
{
	int nDie1 = RollD10();
	int nDie2 = RollD10();

	int nDie = nDie1*10 + nDie2;

	if(nDie == 0)
	{
		nDie = 100;
	}

	return nDie;
}

int nRollDie(int nDieType)
{
	int nDie = (rand()%nDieType)+1;

	return nDie;
}


int GetMaxHitDiceByClass(DND_CHARACTER_CLASSES nClass, int *nAdditionalHP)
{
	switch(nClass)
	{
		case DND_CHARACTER_CLASS_NONE:			*nAdditionalHP = 0; return 1;
		case DND_CHARACTER_CLASS_FIGHTER:		*nAdditionalHP = 3; return 9;
		case DND_CHARACTER_CLASS_RANGER:		*nAdditionalHP = 2; return 10; // 11		
		case DND_CHARACTER_CLASS_CAVALIER:		*nAdditionalHP = 3; return 10;	
		case DND_CHARACTER_CLASS_PALADIN:		*nAdditionalHP = 3; return 9;	
		case DND_CHARACTER_CLASS_BARBARIAN:		*nAdditionalHP = 4; return 8;

		case DND_CHARACTER_CLASS_CLERIC:		*nAdditionalHP = 2; return 9;		
		case DND_CHARACTER_CLASS_DRUID:			*nAdditionalHP = 1; return 15;
		case DND_CHARACTER_CLASS_MONK:			*nAdditionalHP = 1; return 17; //18

		case DND_CHARACTER_CLASS_MAGE:			*nAdditionalHP = 1; return 11;
		case DND_CHARACTER_CLASS_ILLUSIONIST:	*nAdditionalHP = 1; return 10;	

		case DND_CHARACTER_CLASS_THIEF:			*nAdditionalHP = 2; return 10;
		case DND_CHARACTER_CLASS_THIEF_ACROBAT: *nAdditionalHP = 2; return 10;
		case DND_CHARACTER_CLASS_ASSASSIN:		*nAdditionalHP = 1; return 15;	

		#if CUSTOM_CLASSES
		case DND_CHARACTER_CLASS_CUSTOM_1:
		case DND_CHARACTER_CLASS_CUSTOM_2:
		case DND_CHARACTER_CLASS_CUSTOM_3:
		case DND_CHARACTER_CLASS_CUSTOM_4:
		case DND_CHARACTER_CLASS_CUSTOM_5:
		case DND_CHARACTER_CLASS_CUSTOM_6:
		case DND_CHARACTER_CLASS_CUSTOM_7:
		case DND_CHARACTER_CLASS_CUSTOM_8:
		case DND_CHARACTER_CLASS_CUSTOM_9:
		case DND_CHARACTER_CLASS_CUSTOM_10:
		case DND_CHARACTER_CLASS_CUSTOM_11:
		case DND_CHARACTER_CLASS_CUSTOM_12:
		{
																								
			*nAdditionalHP = GetCustomClass(nClass)->m_nMaxChartLevelHP; return GetCustomClass(nClass)->m_nMaxChartHPLevel;
		}
		#endif
	}

	return 1;
}

BOOL IsBaseClass(DND_CHARACTER_CLASSES nClass)
{
	switch(nClass)
	{
		case DND_CHARACTER_CLASS_FIGHTER:	
		case DND_CHARACTER_CLASS_CLERIC:		
		case DND_CHARACTER_CLASS_MAGE:			
		case DND_CHARACTER_CLASS_ILLUSIONIST:	
		case DND_CHARACTER_CLASS_THIEF:				
		case DND_CHARACTER_CLASS_ASSASSIN:	
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL ClassIsValidMultiClass(DND_CHARACTER_CLASSES nClass1, DND_CHARACTER_CLASSES nClass2)
{
	#if CUSTOM_CLASSES
	if (IsCustomClass(nClass1) || IsCustomClass(nClass2))
	{
		if (nClass1 == nClass2)
		{
			return FALSE;
		}

		if (IsDefinedClass((DND_CHARACTER_CLASSES)nClass2) == FALSE)
		{
			return FALSE;
		}

		return TRUE;
	}
	#endif

	switch(nClass1)
	{
		case DND_CHARACTER_CLASS_FIGHTER:
		{
			switch(nClass2)
			{
				case DND_CHARACTER_CLASS_CLERIC:
				case DND_CHARACTER_CLASS_MAGE:			
				case DND_CHARACTER_CLASS_ILLUSIONIST:	
				case DND_CHARACTER_CLASS_THIEF:				
				case DND_CHARACTER_CLASS_ASSASSIN:
				{
					return TRUE;
				}
			}

			return FALSE;
		}
		case DND_CHARACTER_CLASS_RANGER:
		{
			if(g_bUseUnearthedArcana)
			{
				switch(nClass2)
				{
					case DND_CHARACTER_CLASS_CLERIC:
					case DND_CHARACTER_CLASS_DRUID:			
					{
						return TRUE;
					}
				}
			}

			return FALSE;
		}
			

		case DND_CHARACTER_CLASS_CLERIC:
		{
			if(g_bUseUnearthedArcana)
			{
				switch(nClass2)
				{
					case DND_CHARACTER_CLASS_RANGER:	
					{
						return TRUE;
					}
				}
			}
			switch(nClass2)
			{
				case DND_CHARACTER_CLASS_FIGHTER:
				case DND_CHARACTER_CLASS_MAGE:			
				case DND_CHARACTER_CLASS_ILLUSIONIST:	
				case DND_CHARACTER_CLASS_THIEF:				
				case DND_CHARACTER_CLASS_ASSASSIN:
				{
					return TRUE;
				}
			}

			return FALSE;
		}
				
			

		case DND_CHARACTER_CLASS_MAGE:			
		case DND_CHARACTER_CLASS_ILLUSIONIST:
		{
			switch(nClass2)
			{
				case DND_CHARACTER_CLASS_FIGHTER:
				case DND_CHARACTER_CLASS_CLERIC:				
				case DND_CHARACTER_CLASS_THIEF:				
				case DND_CHARACTER_CLASS_ASSASSIN:
				{
					return TRUE;
				}
			}

			return FALSE;
		}

		case DND_CHARACTER_CLASS_THIEF:			
		case DND_CHARACTER_CLASS_ASSASSIN:	
		{
			switch(nClass2)
			{
				case DND_CHARACTER_CLASS_FIGHTER:
				case DND_CHARACTER_CLASS_CLERIC:
				case DND_CHARACTER_CLASS_MAGE:			
				case DND_CHARACTER_CLASS_ILLUSIONIST:
				{
					return TRUE;
				}
			}

			return FALSE;
		}

	}

	return FALSE;
}

BOOL ClassIsValidDualClass(DND_CHARACTER_CLASSES nClass1, DND_CHARACTER_CLASSES nClass2, DND_CHARACTER_CLASSES nClass3)
{
	
	if(nClass1 != DND_CHARACTER_CLASS_UNDEF)
	{
		if(nClass1 == nClass2)
			return FALSE;

		if(nClass1 == nClass3)
			return FALSE;
	}

	return TRUE;
}

int GetHitDieTypeByClass(DND_CHARACTER_CLASSES nClass)
{
	switch(nClass)
	{
		case DND_CHARACTER_CLASS_NONE:			return 6;
		case DND_CHARACTER_CLASS_FIGHTER:		return 10;
		case DND_CHARACTER_CLASS_RANGER:		return 8;
		case DND_CHARACTER_CLASS_CAVALIER:		return 10;
		case DND_CHARACTER_CLASS_PALADIN:		return 10;
		case DND_CHARACTER_CLASS_BARBARIAN:		return 12;

		case DND_CHARACTER_CLASS_CLERIC:		return 8;
		case DND_CHARACTER_CLASS_DRUID:			return 8;
		case DND_CHARACTER_CLASS_MONK:			return 4;

		case DND_CHARACTER_CLASS_MAGE:			return 4;
		case DND_CHARACTER_CLASS_ILLUSIONIST:	return 4;

		case DND_CHARACTER_CLASS_THIEF:			return 6;	
		case DND_CHARACTER_CLASS_THIEF_ACROBAT: return 6;
		case DND_CHARACTER_CLASS_ASSASSIN:		return 6;

		#if CUSTOM_CLASSES
		case DND_CHARACTER_CLASS_CUSTOM_1:
		case DND_CHARACTER_CLASS_CUSTOM_2:
		case DND_CHARACTER_CLASS_CUSTOM_3:
		case DND_CHARACTER_CLASS_CUSTOM_4:
		case DND_CHARACTER_CLASS_CUSTOM_5:
		case DND_CHARACTER_CLASS_CUSTOM_6:
		case DND_CHARACTER_CLASS_CUSTOM_7:
		case DND_CHARACTER_CLASS_CUSTOM_8:
		case DND_CHARACTER_CLASS_CUSTOM_9:
		case DND_CHARACTER_CLASS_CUSTOM_10:
		case DND_CHARACTER_CLASS_CUSTOM_11:
		case DND_CHARACTER_CLASS_CUSTOM_12:
		{
			return GetCustomClass(nClass)->m_nHitDieType;
		}
		#endif
	}

	return 6;
}

BOOL CharacterIsFighter(cDNDCharacter *pCharacter)
{
	#if CUSTOM_CLASSES
	for (int i = 0; i < 3; ++i)
	{
		switch (pCharacter->m_Class[i])
		{
			case DND_CHARACTER_CLASS_FIGHTER:		
			case DND_CHARACTER_CLASS_RANGER:		
			case DND_CHARACTER_CLASS_CAVALIER:	
			case DND_CHARACTER_CLASS_PALADIN:		
			case DND_CHARACTER_CLASS_BARBARIAN:
			case DND_CHARACTER_CLASS_MONSTER:
			{
				return TRUE;
			}
			case DND_CHARACTER_CLASS_CUSTOM_1:
			case DND_CHARACTER_CLASS_CUSTOM_2:
			case DND_CHARACTER_CLASS_CUSTOM_3:
			case DND_CHARACTER_CLASS_CUSTOM_4:
			case DND_CHARACTER_CLASS_CUSTOM_5:
			case DND_CHARACTER_CLASS_CUSTOM_6:
			case DND_CHARACTER_CLASS_CUSTOM_7:
			case DND_CHARACTER_CLASS_CUSTOM_8:
			case DND_CHARACTER_CLASS_CUSTOM_9:
			case DND_CHARACTER_CLASS_CUSTOM_10:
			case DND_CHARACTER_CLASS_CUSTOM_11:
			case DND_CHARACTER_CLASS_CUSTOM_12:
			{
				if(GetCustomClass(pCharacter->m_Class[i])->m_CombatClass == DND_CHARACTER_CLASS_FIGHTER)
				{
					return TRUE;
				}
			}
		}
	}
	#else
	switch(pCharacter->m_Class[0])
	{
		case DND_CHARACTER_CLASS_FIGHTER:		
		case DND_CHARACTER_CLASS_RANGER:		
		case DND_CHARACTER_CLASS_CAVALIER:	
		case DND_CHARACTER_CLASS_PALADIN:		
		case DND_CHARACTER_CLASS_BARBARIAN:
		case DND_CHARACTER_CLASS_MONSTER:
		{
			return TRUE;
		}
	}

	switch(pCharacter->m_Class[1])
	{
		case DND_CHARACTER_CLASS_FIGHTER:		
		case DND_CHARACTER_CLASS_RANGER:		
		case DND_CHARACTER_CLASS_CAVALIER:	
		case DND_CHARACTER_CLASS_PALADIN:		
		case DND_CHARACTER_CLASS_BARBARIAN:
		{
			return TRUE;
		}
	}

	switch(pCharacter->m_Class[2])
	{
		case DND_CHARACTER_CLASS_FIGHTER:		
		case DND_CHARACTER_CLASS_RANGER:		
		case DND_CHARACTER_CLASS_CAVALIER:	
		case DND_CHARACTER_CLASS_PALADIN:		
		case DND_CHARACTER_CLASS_BARBARIAN:
		{
			return TRUE;
		}
	}
	#endif


	return FALSE;
}


int GetFighterClass(cDNDCharacter *pCharacter)
{

	#if CUSTOM_CLASSES
	for(int i = 0; i < 3; ++i)
	{
		switch(pCharacter->m_Class[i])
		{
			case DND_CHARACTER_CLASS_NONE:
			case DND_CHARACTER_CLASS_FIGHTER:		
			case DND_CHARACTER_CLASS_RANGER:		
			case DND_CHARACTER_CLASS_CAVALIER:	
			case DND_CHARACTER_CLASS_PALADIN:		
			case DND_CHARACTER_CLASS_BARBARIAN:
			case DND_CHARACTER_CLASS_MONSTER:
			{
				return i;
			}
			case DND_CHARACTER_CLASS_CUSTOM_1:
			case DND_CHARACTER_CLASS_CUSTOM_2:
			case DND_CHARACTER_CLASS_CUSTOM_3:
			case DND_CHARACTER_CLASS_CUSTOM_4:
			case DND_CHARACTER_CLASS_CUSTOM_5:
			case DND_CHARACTER_CLASS_CUSTOM_6:
			case DND_CHARACTER_CLASS_CUSTOM_7:
			case DND_CHARACTER_CLASS_CUSTOM_8:
			case DND_CHARACTER_CLASS_CUSTOM_9:
			case DND_CHARACTER_CLASS_CUSTOM_10:
			case DND_CHARACTER_CLASS_CUSTOM_11:
			case DND_CHARACTER_CLASS_CUSTOM_12:
			{
				if(GetCustomClass(pCharacter->m_Class[i])->m_CombatClass == DND_CHARACTER_CLASS_FIGHTER)
				{
					return i;
				}
			}
		}

	}
	#else
	switch(pCharacter->m_Class[0])
	{
		case DND_CHARACTER_CLASS_FIGHTER:		
		case DND_CHARACTER_CLASS_RANGER:		
		case DND_CHARACTER_CLASS_CAVALIER:	
		case DND_CHARACTER_CLASS_PALADIN:		
		case DND_CHARACTER_CLASS_BARBARIAN:
		{
			return 0;
		}
	}

	switch(pCharacter->m_Class[1])
	{
		case DND_CHARACTER_CLASS_FIGHTER:		
		case DND_CHARACTER_CLASS_RANGER:		
		case DND_CHARACTER_CLASS_CAVALIER:	
		case DND_CHARACTER_CLASS_PALADIN:		
		case DND_CHARACTER_CLASS_BARBARIAN:
		{
			return 1;
		}
	}

	switch(pCharacter->m_Class[2])
	{
		case DND_CHARACTER_CLASS_FIGHTER:		
		case DND_CHARACTER_CLASS_RANGER:		
		case DND_CHARACTER_CLASS_CAVALIER:	
		case DND_CHARACTER_CLASS_PALADIN:		
		case DND_CHARACTER_CLASS_BARBARIAN:
		{
			return 2;
		}
	}
	#endif

	return -1;
}

BOOL CharacterIsMage(cDNDCharacter *pCharacter)
{
	for(int i = 0; i < 3; ++i)
	{
		switch(pCharacter->m_Class[i])
		{
			case DND_CHARACTER_CLASS_MAGE:		
			case DND_CHARACTER_CLASS_ILLUSIONIST:		
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CharacterIsCleric(cDNDCharacter *pCharacter, BOOL bStrict)
{
	if(bStrict)
	{
		for(int i = 0; i < 3; ++i)
		{
			if(pCharacter->m_Class[i] == DND_CHARACTER_CLASS_CLERIC)
			{
				return TRUE;
			}
		}

		return FALSE;
	}

	for(int i = 0; i < 3; ++i)
	{
		switch(pCharacter->m_Class[i])
		{
			case DND_CHARACTER_CLASS_CLERIC:		
			case DND_CHARACTER_CLASS_DRUID:		
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CharacterIsThief(cDNDCharacter *pCharacter)
{
	for(int i = 0; i < 3; ++i)
	{
		switch(pCharacter->m_Class[i])
		{
			case DND_CHARACTER_CLASS_THIEF:		
			case DND_CHARACTER_CLASS_ASSASSIN:		
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

char *GetRaceName(DND_CHARACTER_RACES nRace)
{

	switch(nRace)
	{
		case DND_CHARACTER_RACE_HUMAN:		return("Human");
		case DND_CHARACTER_RACE_ELF:		return("Elven");
		case DND_CHARACTER_RACE_HALF_ELF:	return("Half Elven");
		case DND_CHARACTER_RACE_DWARF:		return("Dwarven");
		case DND_CHARACTER_RACE_HALFLING:	return("Halfling");
		//case DND_CHARACTER_RACE_HALFLING:	return("Halfling (Hairfoot)");
		case DND_CHARACTER_RACE_GNOME:		return("Gnome");
		case DND_CHARACTER_RACE_HALF_ORC:	return("Half Orc");
		case DND_CHARACTER_RACE_DROW:		return("Drow");

		case DND_CHARACTER_RACE_HALFLING_TALLFELLOW: return("Halfling (Tallfellow)");
		case DND_CHARACTER_RACE_HALFLING_STOUT: return("Halfling (Stout)");
	}

	return "Undefined";
}

BOOL IsDefinedClass(DND_CHARACTER_CLASSES nClass)
{
	//_gCustomClass[0].m_bClassDefined = TRUE;

	switch (nClass)
	{
		case DND_CHARACTER_CLASS_NONE:
		case DND_CHARACTER_CLASS_FIGHTER:	
		case DND_CHARACTER_CLASS_RANGER:		
		case DND_CHARACTER_CLASS_CAVALIER:	
		case DND_CHARACTER_CLASS_PALADIN:	
		case DND_CHARACTER_CLASS_BARBARIAN:	

		case DND_CHARACTER_CLASS_CLERIC:	
		case DND_CHARACTER_CLASS_DRUID:		
		case DND_CHARACTER_CLASS_MONK:			

		case DND_CHARACTER_CLASS_MAGE:			
		case DND_CHARACTER_CLASS_ILLUSIONIST:	

		case DND_CHARACTER_CLASS_THIEF:			
		case DND_CHARACTER_CLASS_THIEF_ACROBAT:	
		case DND_CHARACTER_CLASS_ASSASSIN:
		{
			return TRUE;
		}

		#if CUSTOM_CLASSES
		case DND_CHARACTER_CLASS_CUSTOM_1:
		case DND_CHARACTER_CLASS_CUSTOM_2:
		case DND_CHARACTER_CLASS_CUSTOM_3:
		case DND_CHARACTER_CLASS_CUSTOM_4:
		case DND_CHARACTER_CLASS_CUSTOM_5:
		case DND_CHARACTER_CLASS_CUSTOM_6:
		case DND_CHARACTER_CLASS_CUSTOM_7:
		case DND_CHARACTER_CLASS_CUSTOM_8:
		case DND_CHARACTER_CLASS_CUSTOM_9:
		case DND_CHARACTER_CLASS_CUSTOM_10:
		case DND_CHARACTER_CLASS_CUSTOM_11:
		case DND_CHARACTER_CLASS_CUSTOM_12:
		{
			return GetCustomClass(nClass)->m_bClassDefined;
		}
		#endif
	}

	return FALSE;
}

char *GetUndefClassName(DND_CHARACTER_CLASSES nClass)
{
	#if CUSTOM_CLASSES
	switch (nClass)
	{
		case DND_CHARACTER_CLASS_CUSTOM_1:	return "CUSTOM CLASS 1";
		case DND_CHARACTER_CLASS_CUSTOM_2:	return "CUSTOM CLASS 2";
		case DND_CHARACTER_CLASS_CUSTOM_3:	return "CUSTOM CLASS 3";
		case DND_CHARACTER_CLASS_CUSTOM_4:	return "CUSTOM CLASS 4";
		case DND_CHARACTER_CLASS_CUSTOM_5:	return "CUSTOM CLASS 5";
		case DND_CHARACTER_CLASS_CUSTOM_6:	return "CUSTOM CLASS 6";
		case DND_CHARACTER_CLASS_CUSTOM_7:	return "CUSTOM CLASS 7";
		case DND_CHARACTER_CLASS_CUSTOM_8:	return "CUSTOM CLASS 8";
		case DND_CHARACTER_CLASS_CUSTOM_9:	return "CUSTOM CLASS 9";
		case DND_CHARACTER_CLASS_CUSTOM_10:	return "CUSTOM CLASS 10";
		case DND_CHARACTER_CLASS_CUSTOM_11:	return "CUSTOM CLASS 11";
		case DND_CHARACTER_CLASS_CUSTOM_12:	return "CUSTOM CLASS 12";
	}
	#endif

	return "UNDEF";
}

char *GetClassName(DND_CHARACTER_CLASSES nClass)
{
	//strcpy(_gCustomClass[0].m_szClassName, "Ninja");

	switch(nClass)
	{
		case DND_CHARACTER_CLASS_NONE:			return("None (0 Level)");
		case DND_CHARACTER_CLASS_FIGHTER:		return("Fighter");
		case DND_CHARACTER_CLASS_RANGER:		return("Ranger");
		case DND_CHARACTER_CLASS_CAVALIER:		return("Cavalier");
		case DND_CHARACTER_CLASS_PALADIN:		return("Paladin");
		case DND_CHARACTER_CLASS_BARBARIAN:		return("Barbarian");

		case DND_CHARACTER_CLASS_CLERIC:		return("Cleric");
		case DND_CHARACTER_CLASS_DRUID:			return("Druid");
		case DND_CHARACTER_CLASS_MONK:			return("Monk");

		case DND_CHARACTER_CLASS_MAGE:			return("Magic User");
		case DND_CHARACTER_CLASS_ILLUSIONIST:	return("Illusionist");

		case DND_CHARACTER_CLASS_THIEF:			return("Thief");
		case DND_CHARACTER_CLASS_THIEF_ACROBAT:	return("Thief Acrobat");
		case DND_CHARACTER_CLASS_ASSASSIN:		return("Assassin");

		#if CUSTOM_CLASSES
		case DND_CHARACTER_CLASS_CUSTOM_1:
		case DND_CHARACTER_CLASS_CUSTOM_2:
		case DND_CHARACTER_CLASS_CUSTOM_3:
		case DND_CHARACTER_CLASS_CUSTOM_4:
		case DND_CHARACTER_CLASS_CUSTOM_5:
		case DND_CHARACTER_CLASS_CUSTOM_6:
		case DND_CHARACTER_CLASS_CUSTOM_7:
		case DND_CHARACTER_CLASS_CUSTOM_8:
		case DND_CHARACTER_CLASS_CUSTOM_9:
		case DND_CHARACTER_CLASS_CUSTOM_10:
		case DND_CHARACTER_CLASS_CUSTOM_11:
		case DND_CHARACTER_CLASS_CUSTOM_12:
		{
			if (IsDefinedClass(nClass))
			{
				return GetCustomClass(nClass)->m_szClassName;
			}
			else
			{
				return GetUndefClassName(nClass);
			}
		}
		#endif
	}


	return "None";
}

char *GetAlignmentName(DND_CHARACTER_ALIGNMENTS nAlignment)
{
	switch(nAlignment)
	{
		case DND_CHARACTER_ALIGNMENT_LAWFUL_GOOD:		return("Lawful Good");
		case DND_CHARACTER_ALIGNMENT_NEUTRAL_GOOD:		return("Neutral Good");
		case DND_CHARACTER_ALIGNMENT_CHAOTIC_GOOD:		return("Chaotic Good");

		case DND_CHARACTER_ALIGNMENT_LAWFUL_NEUTRAL:	return("Lawful Neutral");
		case DND_CHARACTER_ALIGNMENT_NEUTRAL:			return("Neutral");
		case DND_CHARACTER_ALIGNMENT_CHAOTIC_NEUTRAL:	return("Chaotic Neutral");

		case DND_CHARACTER_ALIGNMENT_LAWFUL_EVIL:		return("Lawful Evil");
		case DND_CHARACTER_ALIGNMENT_NEUTRAL_EVIL:		return("Neutral Evil");
		case DND_CHARACTER_ALIGNMENT_CHAOTIC_EVIL:		return("Chaotic Evil");
	}

	return "Undefined";
}

char *GetShortAlignmentName(DND_CHARACTER_ALIGNMENTS nAlignment)
{
	switch (nAlignment)
	{
		case DND_CHARACTER_ALIGNMENT_LAWFUL_GOOD:		return("LG");
		case DND_CHARACTER_ALIGNMENT_NEUTRAL_GOOD:		return("NG");
		case DND_CHARACTER_ALIGNMENT_CHAOTIC_GOOD:		return("CG");

		case DND_CHARACTER_ALIGNMENT_LAWFUL_NEUTRAL:	return("LN");
		case DND_CHARACTER_ALIGNMENT_NEUTRAL:			return("N");
		case DND_CHARACTER_ALIGNMENT_CHAOTIC_NEUTRAL:	return("CN");

		case DND_CHARACTER_ALIGNMENT_LAWFUL_EVIL:		return("LE");
		case DND_CHARACTER_ALIGNMENT_NEUTRAL_EVIL:		return("NE");
		case DND_CHARACTER_ALIGNMENT_CHAOTIC_EVIL:		return("CE");
	}

	return "A?";
}

BOOL IsAlignmentGood(DND_CHARACTER_ALIGNMENTS nAlignment)
{
	switch(nAlignment)
	{
		case DND_CHARACTER_ALIGNMENT_LAWFUL_GOOD:		
		case DND_CHARACTER_ALIGNMENT_NEUTRAL_GOOD:		
		case DND_CHARACTER_ALIGNMENT_CHAOTIC_GOOD:	
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL IsAlignmentEvil(DND_CHARACTER_ALIGNMENTS nAlignment)
{
	switch(nAlignment)
	{
		case DND_CHARACTER_ALIGNMENT_LAWFUL_EVIL:		
		case DND_CHARACTER_ALIGNMENT_NEUTRAL_EVIL:		
		case DND_CHARACTER_ALIGNMENT_CHAOTIC_EVIL:	
		{
			return TRUE;
		}
	}

	return FALSE;
}


char *GetVisionTypeName(DND_CHARACTER_VISION_TYPES nVision)
{
	switch(nVision)
	{
		case DND_CHARACTER_VISION_UNDEF:		return "undef";
		case DND_CHARACTER_VISION_STANDARD:		return "";
		case DND_CHARACTER_VISION_30_INF:		return "30' inf";
		case DND_CHARACTER_VISION_60_INF:		return "60' inf";
		case DND_CHARACTER_VISION_120_INF:		return "120' inf";
		case DND_CHARACTER_VISION_120_INF_UV:	return "120' inf/uvis";
		case DND_CHARACTER_VISION_UV:			return "uvis";
		case DND_CHARACTER_VISION_SPECIAL:		return "special";
	}

	return "";
}

DND_CHARACTER_VISION_TYPES GetCharacterVision(cDNDCharacter *pCharacter)
{
	DND_CHARACTER_VISION_TYPES nRetVal = DND_CHARACTER_VISION_STANDARD;

	switch(pCharacter->m_nRace)
	{
		case DND_CHARACTER_RACE_ELF:
		case DND_CHARACTER_RACE_HALF_ELF:
		case DND_CHARACTER_RACE_DWARF:
		case DND_CHARACTER_RACE_HALFLING:
		case DND_CHARACTER_RACE_GNOME:
		case DND_CHARACTER_RACE_HALF_ORC:
		{
			nRetVal = DND_CHARACTER_VISION_60_INF;		
			break;
		}
		case DND_CHARACTER_RACE_DROW:
		{	
			nRetVal = DND_CHARACTER_VISION_120_INF;
			break;
		}
	}

	if(pCharacter->m_VisionType == DND_CHARACTER_VISION_UNDEF)
	{
		pCharacter->m_VisionType = nRetVal;
	}

	return nRetVal;
}

int GetCharacterListeningScore(cDNDCharacter *pCharacter)
{
	int nRetVal = 2;

	switch(pCharacter->m_nRace)
	{
		case DND_CHARACTER_RACE_ELF:		nRetVal = 3; break;
		case DND_CHARACTER_RACE_HALF_ELF:	nRetVal = 2; break;
		case DND_CHARACTER_RACE_DWARF:		nRetVal = 2; break;
		case DND_CHARACTER_RACE_HALFLING:	nRetVal = 3; break;
		case DND_CHARACTER_RACE_GNOME:		nRetVal = 4; break;
		case DND_CHARACTER_RACE_HALF_ORC:	nRetVal = 3; break;
		case DND_CHARACTER_RACE_DROW:		nRetVal = 3; break;
	}

	if(pCharacter->m_bKeenEar)
		nRetVal += 1;

	if(pCharacter->m_bVeryKeenEar)
		nRetVal += 1;

	return nRetVal;
}

void AddStatModifiersByRace(cDNDCharacter *pCharacter, int *nStatBonus)
{
	switch(pCharacter->m_nRace)
	{
		case DND_CHARACTER_RACE_DWARF:
		{
			nStatBonus[0] = 0;   //STR
			nStatBonus[1] = 0;   //INT
			nStatBonus[2] = 0;   //WIS
			nStatBonus[3] = 0;   //DEX
			nStatBonus[4] = 1;   //CON
			nStatBonus[5] = -1;	 //CHA
			nStatBonus[6] = 0;	 //COM

			break;
		}
		case DND_CHARACTER_RACE_ELF:
		{
			nStatBonus[0] = 0;   //STR
			nStatBonus[1] = 0;   //INT
			nStatBonus[2] = 0;   //WIS
			nStatBonus[3] = 1;   //DEX
			nStatBonus[4] = -1;  //CON
			nStatBonus[5] = 0;	 //CHA
			nStatBonus[6] = 0;	 //COM

			break;
		}
		case DND_CHARACTER_RACE_HALF_ORC:
		{
			nStatBonus[0] = 1;   //STR
			nStatBonus[1] = 0;   //INT
			nStatBonus[2] = 0;   //WIS
			nStatBonus[3] = 0;   //DEX
			nStatBonus[4] = 1;   //CON
			nStatBonus[5] = -2;	 //CHA
			nStatBonus[6] = 0;	 //COM

			break;
		}
		case DND_CHARACTER_RACE_HALFLING:
		{
			nStatBonus[0] = -1;   //STR
			nStatBonus[1] = 0;   //INT
			nStatBonus[2] = 0;   //WIS
			nStatBonus[3] = 1;   //DEX
			nStatBonus[4] = 0;   //CON
			nStatBonus[5] = 0;	 //CHA
			nStatBonus[6] = 0;	 //COM

			break;
		}
	}
}

void GetMinimumStatsByClass(cDNDCharacter *pCharacter, int *nMinStats)
{

	switch(pCharacter->m_Class[0])
	{
		case DND_CHARACTER_CLASS_NONE:
		{
			nMinStats[0] = 0;   //STR
			nMinStats[1] = 0;   //INT
			nMinStats[2] = 0;   //WIS
			nMinStats[3] = 0;   //DEX
			nMinStats[4] = 0;   //CON
			nMinStats[5] = 0;	//CHA
			nMinStats[6] = 0;	//COM

			break;
		}

		case DND_CHARACTER_CLASS_FIGHTER:
		{
			nMinStats[0] = 9;   //STR
			nMinStats[1] = 0;   //INT
			nMinStats[2] = 0;   //WIS
			nMinStats[3] = 0;   //DEX
			nMinStats[4] = 7;   //CON
			nMinStats[5] = 0;	//CHA
			nMinStats[6] = 0;	//COM

			break;
		}

		case DND_CHARACTER_CLASS_CAVALIER:
		{
			nMinStats[0] = 15;  //STR
			nMinStats[1] = 10;  //INT
			nMinStats[2] = 10;  //WIS
			nMinStats[3] = 15;  //DEX
			nMinStats[4] = 15;  //CON
			nMinStats[5] = 0;	//CHA
			nMinStats[6] = 0;	//COM

			break;
		}

		case DND_CHARACTER_CLASS_PALADIN:
		{
			if(g_bUseUnearthedArcana)
			{
				nMinStats[0] = 15;  //STR
				nMinStats[1] = 10;  //INT
				nMinStats[2] = 13;  //WIS
				nMinStats[3] = 15;  //DEX
				nMinStats[4] = 15;  //CON
				nMinStats[5] = 17;	//CHA
				nMinStats[6] = 0;	//COM
			}
			else
			{
				nMinStats[0] = 12;  //STR
				nMinStats[1] = 9;   //INT
				nMinStats[2] = 13;  //WIS
				nMinStats[3] = 0;   //DEX
				nMinStats[4] = 9;   //CON
				nMinStats[5] = 17;	//CHA
				nMinStats[6] = 0;	//COM
			}

			break;
		}

		case DND_CHARACTER_CLASS_RANGER:
		{
			nMinStats[0] = 13;   //STR
			nMinStats[1] = 13;   //INT
			nMinStats[2] = 14;   //WIS
			nMinStats[3] = 0;    //DEX
			nMinStats[4] = 14;   //CON
			nMinStats[5] = 0;	 //CHA
			nMinStats[6] = 0;	 //COM

			break;
		}

		case DND_CHARACTER_CLASS_BARBARIAN:
		{
			nMinStats[0] = 15;  //STR
			nMinStats[1] = 0;   //INT
			nMinStats[2] = -16; //WIS
			nMinStats[3] = 14;  //DEX
			nMinStats[4] = 15;  //CON
			nMinStats[5] = 0;	//CHA
			nMinStats[6] = 0;	//COM

			break;
		}

		case DND_CHARACTER_CLASS_CLERIC:
		{
			nMinStats[0] = 0;   //STR
			nMinStats[1] = 0;   //INT
			nMinStats[2] = 9;   //WIS
			nMinStats[3] = 0;   //DEX
			nMinStats[4] = 0;   //CON
			nMinStats[5] = 0;	//CHA
			nMinStats[6] = 0;	//COM

			break;
		}

		case DND_CHARACTER_CLASS_DRUID:
		{
			nMinStats[0] = 0;   //STR
			nMinStats[1] = 0;   //INT
			nMinStats[2] = 12;   //WIS
			nMinStats[3] = 0;   //DEX
			nMinStats[4] = 0;   //CON
			nMinStats[5] = 15;	//CHA
			nMinStats[6] = 0;	//COM

			break;
		}

		case DND_CHARACTER_CLASS_MAGE:
		{
			nMinStats[0] = 0;   //STR
			nMinStats[1] = 9;   //INT
			nMinStats[2] = 0;   //WIS
			nMinStats[3] = 6;   //DEX
			nMinStats[4] = 0;   //CON
			nMinStats[5] = 0;	//CHA
			nMinStats[6] = 0;	//COM

			break;
		}

		case DND_CHARACTER_CLASS_ILLUSIONIST:
		{
			nMinStats[0] = 0;   //STR
			nMinStats[1] = 15;   //INT
			nMinStats[2] = 0;   //WIS
			nMinStats[3] = 16;   //DEX
			nMinStats[4] = 0;   //CON
			nMinStats[5] = 0;	//CHA
			nMinStats[6] = 0;	//COM

			break;
		}


		case DND_CHARACTER_CLASS_THIEF:
		{
			nMinStats[0] = 0;   //STR
			nMinStats[1] = 0;   //INT
			nMinStats[2] = 0;   //WIS
			nMinStats[3] = 9;   //DEX
			nMinStats[4] = 0;   //CON
			nMinStats[5] = 0;	//CHA
			nMinStats[6] = 0;	//COM

			break;
		}

		case DND_CHARACTER_CLASS_ASSASSIN:
		{
			nMinStats[0] = 12;  //STR
			nMinStats[1] = 11;  //INT
			nMinStats[2] = 0;   //WIS
			nMinStats[3] = 12;  //DEX
			nMinStats[4] = 0;   //CON
			nMinStats[5] = 0;	//CHA
			nMinStats[6] = 0;	//COM

			break;
		}

		case DND_CHARACTER_CLASS_MONK:
		{
			nMinStats[0] = 15;  //STR
			nMinStats[1] = 0;   //INT
			nMinStats[2] = 15;  //WIS
			nMinStats[3] = 15;  //DEX
			nMinStats[4] = 11;  //CON
			nMinStats[5] = 0;	//CHA
			nMinStats[6] = 0;	//COM

			break;
		}
		#if CUSTOM_CLASSES
		case DND_CHARACTER_CLASS_CUSTOM_1:
		case DND_CHARACTER_CLASS_CUSTOM_2:
		case DND_CHARACTER_CLASS_CUSTOM_3:
		case DND_CHARACTER_CLASS_CUSTOM_4:
		case DND_CHARACTER_CLASS_CUSTOM_5:
		case DND_CHARACTER_CLASS_CUSTOM_6:
		case DND_CHARACTER_CLASS_CUSTOM_7:
		case DND_CHARACTER_CLASS_CUSTOM_8:
		case DND_CHARACTER_CLASS_CUSTOM_9:
		case DND_CHARACTER_CLASS_CUSTOM_10:
		case DND_CHARACTER_CLASS_CUSTOM_11:
		case DND_CHARACTER_CLASS_CUSTOM_12:
		{
			nMinStats[0] = GetCustomClass(pCharacter->m_Class[0])->m_nMinAttributes[0];  //STR
			nMinStats[1] = GetCustomClass(pCharacter->m_Class[0])->m_nMinAttributes[1];  //INT
			nMinStats[2] = GetCustomClass(pCharacter->m_Class[0])->m_nMinAttributes[2];  //WIS
			nMinStats[3] = GetCustomClass(pCharacter->m_Class[0])->m_nMinAttributes[3];  //DEX
			nMinStats[4] = GetCustomClass(pCharacter->m_Class[0])->m_nMinAttributes[4];  //CON
			nMinStats[5] = GetCustomClass(pCharacter->m_Class[0])->m_nMinAttributes[5];	 //CHA
			nMinStats[6] = 0;

			break;
		}
		#endif

	}

}


BOOL GrantCharacterEarnedExperience(cDNDCharacter *pCharacter)
{
	BOOL bLevelUp = FALSE;

	if(pCharacter->m_lEarnedXP <= 0L)
	{
		pCharacter->m_lEarnedXP = 0L;

		return bLevelUp;
	}

	pCharacter->m_lEarnedXP = min(pCharacter->m_lEarnedXP, 10000000L);

	//calculate bonuses


	int nClasses = 0;
	for(int i = 0; i < 3; ++i)
	{
		if(pCharacter->m_nRace == DND_CHARACTER_RACE_HUMAN && i == pCharacter->m_nDualClassClass)
		{
			nClasses = 1;
			break;
		}

		if(pCharacter->m_Class[i] != DND_CHARACTER_CLASS_UNDEF)
		{
			++nClasses;
		}
	}

	if(nClasses == 0)
	{
		return FALSE;		// something is borked
	}

	LONG lSplitXP = pCharacter->m_lEarnedXP / nClasses;

	for(int i = 0; i < 3; ++i)
	{
		if(pCharacter->m_Class[i] != DND_CHARACTER_CLASS_UNDEF)
		{
			int nExpBonus = CalculateExperienceBonus(pCharacter, i);

			if(nExpBonus)
			{
				float fExpPerc = ((float)nExpBonus) / 100000.0f;

				pCharacter->m_lExperience[i] += (LONG)((float)lSplitXP * fExpPerc);
			}
		}
	}

	int nCurrentLevel[3];
	nCurrentLevel[0] = pCharacter->m_nLevel[0];
	nCurrentLevel[1] = pCharacter->m_nLevel[1];
	nCurrentLevel[2] = pCharacter->m_nLevel[2];

	BOOL bLeveled[3] = {FALSE, FALSE, FALSE};

	
	for(LONG lXP = pCharacter->m_lEarnedXP; lXP > 0L && pCharacter->m_lEarnedXP > 0L; --lXP)
	{
		for(int nClass = 0; nClass < 3 && pCharacter->m_lEarnedXP > 0L; ++nClass)
		{
			if(pCharacter->m_nRace == DND_CHARACTER_RACE_HUMAN && nClass != pCharacter->m_nDualClassClass)
			{
				continue;
			}

			if(pCharacter->m_Class[nClass] == DND_CHARACTER_CLASS_UNDEF)
			{
				continue;
			}

			if(bLeveled[nClass])
			{
				pCharacter->m_lExperience[nClass] = GetExperiencePointsForLevelByClass(pCharacter->m_Class[nClass], pCharacter->m_nLevel[nClass]);
				continue;
			}

			pCharacter->m_lExperience[nClass] += 1L;
			pCharacter->m_lEarnedXP -= 1L;

			pCharacter->m_nLevel[nClass] = GetExperienceLevelByClass(pCharacter, nClass);

			if(pCharacter->m_nLevel[nClass] > nCurrentLevel[nClass])
			{
				nCurrentLevel[nClass] = pCharacter->m_nLevel[nClass];

				bLevelUp = TRUE;

				if(bLeveled[nClass])
				{
					//can't level twice
					pCharacter->m_nLevel[nClass] -= 1;
					pCharacter->m_lExperience[nClass] -= 1L;
					pCharacter->m_lEarnedXP += 1L;

					return bLevelUp; // ??? this doesn't look right
				}
				else
				{
					bLeveled[nClass] = TRUE;
				}
			}

		}

	}

	return bLevelUp;
}


int GetExperienceLevelByClass(cDNDCharacter *pCharacter, int nClass)
{
	int nRetVal = 0;

	LONG lXP = pCharacter->m_lExperience[nClass];

	for (int i = 1; i < 100; ++i)
	{
		LONG lThisLevel = GetExperiencePointsForLevelByClass(pCharacter->m_Class[nClass], i);

		if (lThisLevel > lXP)
		{
			nRetVal = i - 1;

			int nMaxLevel = max(0, CalculateLevelLimits(pCharacter, pCharacter->m_Class[nClass]));

			if (nRetVal > nMaxLevel)
				nRetVal = nMaxLevel;

			break;
		}
	}

	return nRetVal;
}


LONG GetExperiencePointsForLevelByClass(DND_CHARACTER_CLASSES nClass, int nLevel)
{

	LONG lRetVal = 0;

	if(nLevel == 0)
	{
		return 0L;
	}

	switch(nClass)
	{
		case DND_CHARACTER_CLASS_NONE:
		{
			lRetVal = 0;
			break;
		}
		case DND_CHARACTER_CLASS_FIGHTER:
		{
			switch(nLevel)
			{
				case 1:		lRetVal = 0;		break;
				case 2:		lRetVal = 2001;		break;
				case 3:		lRetVal = 4001;		break;
				case 4:		lRetVal = 8001;		break;
				case 5:		lRetVal = 18001;	break;
				case 6:		lRetVal = 35001;	break;
				case 7:		lRetVal = 70001;	break;
				case 8:		lRetVal = 125001;	break;
				case 9:		lRetVal = 250001;	break;
				case 10:	lRetVal = 500001;	break;
				case 11:	lRetVal = 750001;	break;
				default:
				{
					nLevel -= 11;

					lRetVal = 1000001L + nLevel * 250000L;

					break;
				}
			}

			break;
		}
		case DND_CHARACTER_CLASS_RANGER:
		{
			switch(nLevel)
			{
				case 1:		lRetVal = 0;		break;
				case 2:		lRetVal = 2351;		break;
				case 3:		lRetVal = 4501;		break;
				case 4:		lRetVal = 10001;	break;
				case 5:		lRetVal = 20001;	break;
				case 6:		lRetVal = 40001;	break;
				case 7:		lRetVal = 90001;	break;
				case 8:		lRetVal = 150001;	break;
				case 9:		lRetVal = 225001;	break;
				case 10:	lRetVal = 325001;	break;
				case 11:	lRetVal = 650001;	break;
				case 12:	lRetVal = 1300001;	break;
				default:
				{
					nLevel -= 12;

					lRetVal = 1300001 + nLevel * 325000L;

					break;
				}
			}

			break;
		}

		case DND_CHARACTER_CLASS_PALADIN:
		{
			switch(nLevel)
			{
				case 1:		lRetVal = 0;		break;
				case 2:		lRetVal = 2751;		break;
				case 3:		lRetVal = 5501;		break;
				case 4:		lRetVal = 12001;	break;
				case 5:		lRetVal = 24001;	break;
				case 6:		lRetVal = 45001;	break;
				case 7:		lRetVal = 95001;	break;
				case 8:		lRetVal = 175001;	break;
				case 9:		lRetVal = 350001;	break;
				case 10:	lRetVal = 700001;	break;
				case 11:	lRetVal = 1050001;	break;
				default:
				{
					nLevel -= 11;

					lRetVal = 1050001 + nLevel * 350000L;

					break;
				}
			}

			break;
		}

		case DND_CHARACTER_CLASS_CAVALIER:
		{
			switch(nLevel)
			{
				case 1:		lRetVal = 0;		break;
				case 2:		lRetVal = 2501;		break;
				case 3:		lRetVal = 5001;		break;
				case 4:		lRetVal = 10001;	break;
				case 5:		lRetVal = 18501;	break;
				case 6:		lRetVal = 37001;	break;
				case 7:		lRetVal = 85001;	break;
				case 8:		lRetVal = 140001;	break;
				case 9:		lRetVal = 220001;	break;
				case 10:	lRetVal = 300001;	break;
				case 11:	lRetVal = 600001;	break;
				case 12:	lRetVal = 900001;	break;
				default:
				{
					nLevel -= 12;

					lRetVal = 1050001 + nLevel * 300000L;

					break;
				}
			}

			break;
		}

		case DND_CHARACTER_CLASS_BARBARIAN:
		{
			switch(nLevel)
			{
				case 1:		lRetVal = 0;		break;
				case 2:		lRetVal = 6001;		break;
				case 3:		lRetVal = 12001;	break;
				case 4:		lRetVal = 24001;	break;
				case 5:		lRetVal = 48001;	break;
				case 6:		lRetVal = 80001;	break;
				case 7:		lRetVal = 150001;	break;
				case 8:		lRetVal = 275001;	break;
				case 9:		lRetVal = 500001;	break;
				case 10:	lRetVal = 1000001;	break;
				case 11:	lRetVal = 1500001;	break;
				default:
				{
					nLevel -= 11;

					lRetVal = 1500001 + nLevel * 500000L;

					break;
				}
			}

			break;
		}

		////////////////////////////////////

		case DND_CHARACTER_CLASS_CLERIC:
		{
			switch(nLevel)
			{
				case 1:		lRetVal = 0;		break;
				case 2:		lRetVal = 1501;		break;
				case 3:		lRetVal = 3001;		break;
				case 4:		lRetVal = 6001;		break;
				case 5:		lRetVal = 13001;	break;
				case 6:		lRetVal = 27501;	break;
				case 7:		lRetVal = 55001;	break;
				case 8:		lRetVal = 111001;	break;
				case 9:		lRetVal = 225001;	break;
				case 10:	lRetVal = 450001;	break;
				case 11:	lRetVal = 675001;	break;
				default:
				{
					nLevel -= 11;

					lRetVal = 675001 + nLevel * 225000L;

					break;
				}
			}

			break;
		}

		case DND_CHARACTER_CLASS_DRUID:
		{
			switch(nLevel)
			{
				case 1:		lRetVal = 0;		break;
				case 2:		lRetVal = 2001;		break;
				case 3:		lRetVal = 4001;		break;
				case 4:		lRetVal = 7501;		break;
				case 5:		lRetVal = 12501;	break;
				case 6:		lRetVal = 20001;	break;
				case 7:		lRetVal = 35001;	break;
				case 8:		lRetVal = 60001;	break;
				case 9:		lRetVal = 90001;	break;
				case 10:	lRetVal = 125001;	break;
				case 11:	lRetVal = 200001;	break;
				case 12:	lRetVal = 300001;	break;
				case 13:	lRetVal = 750001;	break;
				case 14:	lRetVal = 1500001;	break;
				default:
				{
					nLevel -= 14;

					lRetVal = 1500001 + nLevel * 1500000L;

					break;
				}
			}

			break;
		}

		////////////////////////////////////

		case DND_CHARACTER_CLASS_THIEF:
		{
			switch(nLevel)
			{
				case 1:		lRetVal = 0;		break;
				case 2:		lRetVal = 1251L;	break;
				case 3:		lRetVal = 2501L;	break;
				case 4:		lRetVal = 5001L;	break;
				case 5:		lRetVal = 10001L;	break;
				case 6:		lRetVal = 20001L;	break;
				case 7:		lRetVal = 42501L;	break;
				case 8:		lRetVal = 70001L;	break;
				case 9:		lRetVal = 110001L;	break;
				case 10:	lRetVal = 160001L;	break;
				case 11:	lRetVal = 220001L;	break;
				case 12:	lRetVal = 440001L;	break;
				default:
				{
					nLevel -= 12;

					lRetVal = 440001 + nLevel * 220000L;

					break;
				}
			}

			break;
		}

		case DND_CHARACTER_CLASS_THIEF_ACROBAT:
		{
			switch(nLevel)
			{
				case 1:		lRetVal = 0;		break;
				case 2:		lRetVal = 1251L;	break;
				case 3:		lRetVal = 2501L;	break;
				case 4:		lRetVal = 5001L;	break;
				case 5:		lRetVal = 10001L;	break;

				case 6:		lRetVal = 20001L;	break;
				case 7:		lRetVal = 45001L;	break;
				case 8:		lRetVal = 75001L;	break;
				case 9:		lRetVal = 125001L;	break;
				case 10:	lRetVal = 180001L;	break;
				case 11:	lRetVal = 250001L;	break;
				case 12:	lRetVal = 500001L;	break;
				default:
				{
					nLevel -= 12;

					lRetVal = 500001 + nLevel * 250000L;

					break;
				}
			}

			break;
		}

		case DND_CHARACTER_CLASS_ASSASSIN:
		{
			switch(nLevel)
			{
				case 1:		lRetVal = 0;		break;
				case 2:		lRetVal = 1501;		break;
				case 3:		lRetVal = 3001;		break;
				case 4:		lRetVal = 6001;		break;
				case 5:		lRetVal = 12001;	break;
				case 6:		lRetVal = 25001;	break;
				case 7:		lRetVal = 50001;	break;
				case 8:		lRetVal = 100001;	break;
				case 9:		lRetVal = 200001;	break;
				case 10:	lRetVal = 300001;	break;
				case 11:	lRetVal = 425001;	break;
				case 12:	lRetVal = 575001;	break;
				case 13:	lRetVal = 750001;	break;
				case 14:	lRetVal = 1000001;	break;
				case 15:	lRetVal = 1500001;	break;
				default:
				{
					nLevel -= 15;

					lRetVal = 1500001 + nLevel * 500000L;

					break;
				}
			}

			break;
		}

		case DND_CHARACTER_CLASS_MONK:
		{
			switch(nLevel)
			{
				case 1:		lRetVal = 0;		break;
				case 2:		lRetVal = 2251;		break;
				case 3:		lRetVal = 4751;		break;
				case 4:		lRetVal = 10001;	break;
				case 5:		lRetVal = 22501;	break;
				case 6:		lRetVal = 47501;	break;
				case 7:		lRetVal = 98001;	break;
				case 8:		lRetVal = 200001;	break;
				case 9:		lRetVal = 350001;	break;
				case 10:	lRetVal = 500001;	break;
				case 11:	lRetVal = 700001;	break;
				case 12:	lRetVal = 950001;	break;
				case 13:	lRetVal = 1250001;	break;
				case 14:	lRetVal = 1750001;	break;
				case 15:	lRetVal = 2250001;	break;
				case 16:	lRetVal = 2750001;	break;
				case 17:	lRetVal = 3250001;	break;
				default:
				{
					nLevel -= 17;

					lRetVal = 3250001 + nLevel * 500000L;

					break;
				}
			}

			break;
		}

		////////////////////////////////////

		case DND_CHARACTER_CLASS_MAGE:
		{
			switch(nLevel)
			{
				case 1:		lRetVal = 0;		break;
				case 2:		lRetVal = 2501L;	break;
				case 3:		lRetVal = 5001L;	break;
				case 4:		lRetVal = 10001L;	break;
				case 5:		lRetVal = 22501L;	break;
				case 6:		lRetVal = 40001L;	break;
				case 7:		lRetVal = 60001L;	break;
				case 8:		lRetVal = 90001L;	break;
				case 9:		lRetVal = 135001L;	break;
				case 10:	lRetVal = 250001L;	break;
				case 11:	lRetVal = 375001L;	break;
				case 12:	lRetVal = 750001L;	break;
				case 13:	lRetVal = 1125001L;	break;
				case 14:	lRetVal = 1500001L;	break;
				case 15:	lRetVal = 1875001L;	break;
				case 16:	lRetVal = 2250001L;	break;
				case 17:	lRetVal = 2625001L;	break;
				case 18:	lRetVal = 3000001L;	break;
				default:
				{
					nLevel -= 18;

					lRetVal = 3000001L + nLevel * 375000L;

					break;
				}
			}

			break;
		}

		case DND_CHARACTER_CLASS_ILLUSIONIST:
		{
			switch(nLevel)
			{
				case 1:		lRetVal = 0;		break;
				case 2:		lRetVal = 2251;		break;
				case 3:		lRetVal = 4501;		break;
				case 4:		lRetVal = 9001;		break;
				case 5:		lRetVal = 18001;	break;
				case 6:		lRetVal = 35001;	break;
				case 7:		lRetVal = 60001;	break;
				case 8:		lRetVal = 95001;	break;
				case 9:		lRetVal = 145001;	break;
				case 10:	lRetVal = 220001;	break;
				case 11:	lRetVal = 440001;	break;
				case 12:	lRetVal = 660001;	break;
				default:
				{
					nLevel -= 12;

					lRetVal = 660001L + nLevel * 220000L;

					break;
				}
			}

			break;
		}

		#if CUSTOM_CLASSES
		case DND_CHARACTER_CLASS_CUSTOM_1:
		case DND_CHARACTER_CLASS_CUSTOM_2:
		case DND_CHARACTER_CLASS_CUSTOM_3:
		case DND_CHARACTER_CLASS_CUSTOM_4:
		case DND_CHARACTER_CLASS_CUSTOM_5:
		case DND_CHARACTER_CLASS_CUSTOM_6:
		case DND_CHARACTER_CLASS_CUSTOM_7:
		case DND_CHARACTER_CLASS_CUSTOM_8:
		case DND_CHARACTER_CLASS_CUSTOM_9:
		case DND_CHARACTER_CLASS_CUSTOM_10:
		case DND_CHARACTER_CLASS_CUSTOM_11:
		case DND_CHARACTER_CLASS_CUSTOM_12:
		{
			int nLevelIndex = nLevel - 1; // need index because table starts at entry 0 for MAX XP for level

			if (nLevelIndex == 0)
			{
				lRetVal = 0;
				break;
			}

			if (nLevelIndex < GetCustomClass(nClass)->m_nMaxChartLevel)
			{
				lRetVal = GetCustomClass(nClass)->m_lXPLevel[max(nLevelIndex - 1, 0)];
			}
			else
			{
				nLevelIndex -= GetCustomClass(nClass)->m_nMaxChartLevel;
				lRetVal = GetCustomClass(nClass)->m_lXPLevel[max(nLevelIndex - 1, 0)] + nLevelIndex * GetCustomClass(nClass)->m_lMaxChartLevelXP;
			}

			break;
		}
		#endif
	}

	return lRetVal;
}

char *GetClassLevelTitle(DND_CHARACTER_CLASSES nClass, int nLevel)
{
	switch (nClass)
	{
		case DND_CHARACTER_CLASS_CLERIC:
		{
			switch (nLevel)
			{
				case 0:		return "";
				case 1:		return "Acolyte";
				case 2:		return "Adept";
				case 3:		return "Priest";
				case 4:		return "Curate";
				case 5:		return "Perfect";
				case 6:		return "Canon";
				case 7:		return "Lama";
				case 8:		return "Patriarch";
				default:	return "High Priest";
			}

			break;
		}

		case DND_CHARACTER_CLASS_DRUID:
		{
			switch (nLevel)
			{
				case 0:		return "";
				case 1:		return "Aspirant";
				case 2:		return "Ovate";
				case 3:		return "Initiate of the 1st Circle";
				case 4:		return "Initiate of the 2nd Circle";
				case 5:		return "Initiate of the 3rd Circle";
				case 6:		return "Initiate of the 4th Circle";
				case 7:		return "Initiate of the 5th Circle";
				case 8:		return "Initiate of the 6th Circle";
				case 9:		return "Initiate of the 7th Circle";
				case 10:	return "Initiate of the 8th Circle";
				case 11:	return "Initiate of the 9th Circle";
				case 12:	return "Druid";
				case 13:	return "Arch Druid";
				default:	return "Great Druid";
			}

			break;
		}

		case DND_CHARACTER_CLASS_FIGHTER:
		{
			switch (nLevel)
			{
				case 0:		return "";
				case 1:		return "Veteran";
				case 2:		return "Warrior";
				case 3:		return "Swordsman";
				case 4:		return "Hero";
				case 5:		return "Swashbuckler";
				case 6:		return "Myrmidon";
				case 7:		return "Champion";
				case 8:		return "Superhero";
				default:	return "Lord";
			}

			break;
		}

		case DND_CHARACTER_CLASS_PALADIN:
		{
			switch (nLevel)
			{
				case 0:		return "";
				case 1:		return "Gallant";
				case 2:		return "Keeper"; 
				case 3:		return "Protector"; 
				case 4:		return "Defender"; 
				case 5:		return "Warder"; 
				case 6:		return "Guardian"; 
				case 7:		return "Chevalier"; 
				case 8:		return "Justiciar"; 
				default:	return "Paladin"; 
			}

			break;
		}

		case DND_CHARACTER_CLASS_RANGER:
		{
			switch (nLevel)
			{
				case 0:		return "";
				case 1:		return "Runner";
				case 2:		return "Strider";
				case 3:		return "Scout";
				case 4:		return "Courser";
				case 5:		return "Tracker";
				case 6:		return "Guide";
				case 7:		return "Pathfinder";
				case 8:		return "Ranger";
				case 9:		return "Ranger Knight";
				default:	return "Ranger Lord";
			}

			break;
		}

		case DND_CHARACTER_CLASS_MAGE:
		{
			switch (nLevel)
			{
				case 0:		return "";
				case 1:		return "Prestidigitator";
				case 2:		return "Evoker";
				case 3:		return "Conjurer";
				case 4:		return "Theurgist";
				case 5:		return "Thaumaturgist";
				case 6:		return "Magician";
				case 7:		return "Warlock";
				case 8:		return "Sorcerer";
				case 9:		return "Necromancer";
				default:	return "Wizard";
			}

			break;
		}

		case DND_CHARACTER_CLASS_ILLUSIONIST:
		{
			switch (nLevel)
			{
				case 0:		return "";
				case 1:		return "Prestigitator";
				case 2:		return "Minor Trickster";
				case 3:		return "Trickster";
				case 4:		return "Master Trickster";
				case 5:		return "Cabolist";
				case 6:		return "Visionist";
				case 7:		return "Phantasmist";
				case 8:		return "Apparitionist";
				case 9:		return "Spellbinder";
				default:	return "Illusionist";
			}

			break;
		}

		case DND_CHARACTER_CLASS_THIEF:
		{
			switch (nLevel)
			{
				case 0:		return "";
				case 1:		return "Rogue";
				case 2:		return "Footpad";
				case 3:		return "Cutpurse";
				case 4:		return "Robber";
				case 5:		return "Burglar";
				case 6:		return "Filcher";
				case 7:		return "Sharper";
				case 8:		return "Magsman";
				case 9:		return "Thief";
				default:	return "Master Thief";
			}

			break;
		}

		case DND_CHARACTER_CLASS_ASSASSIN:
		{
			switch (nLevel)
			{
				case 0:		return	"";
				case 1:		return	"Bravo";
				case 2:		return	"Rutterkin";
				case 3:		return	"Waghalter";
				case 4:		return	"Murderer";
				case 5:		return	"Thug";
				case 6:		return	"Killer";
				case 7:		return	"Cutthroat";
				case 8:		return	"Executioner";
				case 9:		return	"Assassin";
				case 10:	return	"Expert Assassin";
				case 11:	return	"Senior Assassin";
				case 12:	return	"Chief Assassin";
				case 13:	return	"Prime Assassin";
				case 14:	return	"Guildmaster Assassin";
				case 15:	return	"Grandfather of Assassins";
			}

			break;
		}

		case DND_CHARACTER_CLASS_MONK:
		{
			switch (nLevel)
			{
				case 0:		return	"";
				case 1:		return	"Novice";
				case 2:		return	"Initiate";
				case 3:		return	"Brother";
				case 4:		return	"Disciple";
				case 5:		return	"Immaculate";
				case 6:		return	"Master";
				case 7:		return	"Superior Master";
				case 8:		return	"Master of Dragons";
				case 9:		return	"Master of the North Wind";
				case 10:	return	"Master of the West Wind";
				case 11:	return	"Master of the South Wind";
				case 12:	return	"Master of the East Wind";
				case 13:	return	"Master of Winter";
				case 14:	return	"Master of Autumn";
				case 15:	return	"Master of Summer";
				case 16:	return	"Master of Spring";
				case 17:	return	"Grand Master of Flowers";
			}

			break;
		}

	} // switch (nClass)

	return "";
}

int CalculateLevelLimits(cDNDCharacter *pCharacter, DND_CHARACTER_CLASSES nClass)
{

	if(!g_bUseDemiHumanLevelLimits)
	{
		return 99;
	}

	//DND_CHARACTER_RACES_UNDEF
	//DND_CHARACTER_RACE_HUMAN, 
	//DND_CHARACTER_RACE_ELF,
	//DND_CHARACTER_RACE_HALF_ELF,
	//DND_CHARACTER_RACE_DWARF,
	//DND_CHARACTER_RACE_HALFLING,
	//DND_CHARACTER_RACE_GNOME,
	//DND_CHARACTER_RACE_HALF_ORC,
	//DND_CHARACTER_RACE_DROW,

	int _LevelLimitTable[14][9]	=	
	{
			
		//											H
		//							H				a				H
		//	U						a				l				a
		//	n		H				l		D		f		G		l
		//	d		u				f		w		l		n		f		D
		//	d		m		E		E		a		i		o		O		r
		//	e		a		l		l		r		l		m		r		o
		//	f		n		f		f		f		g		e		c		w

			0,		990,	000,	000,	000,	000,	000,	000,	000,	// DND_CHARACTER_CLASS_UNDEF

			0,		990,	 72,	 84,	 91,	 65,	 63,	100,	 72,	// DND_CHARACTER_CLASS_FIGHTER
			0,		990,	000,	 84,	000,	000,	000,	000,	000,	// DND_CHARACTER_CLASS_RANGER
			0,		990,	000,	000,	000,	000,	000,	000,	000,	// DND_CHARACTER_CLASS_CAVALIER 
			0,		990,	000,	000,	000,	000,	000,	000,	000,	// DND_CHARACTER_CLASS_PALADIN
			0,		990,	000,	000,	000,	000,	000,	000,	000,	// DND_CHARACTER_CLASS_BARBARIAN

			0,		990,	-70,	 50,	-80,	000,	-70,	 40,	 70,	// DND_CHARACTER_CLASS_CLERIC
			0,		990,	000,	990,	000,	-60,	000,	000,	000,	// DND_CHARACTER_CLASS_DRUID
			0,		990,	000,	000,	000,	000,	000,	000,	000,	// DND_CHARACTER_CLASS_MONK

			0,		990,	990,	990,	990,	990,	990,	 89,	990,	// DND_CHARACTER_CLASS_THIEF
			0,		990,	990,	990,	990,	990,	990,	 89,	990,	// DND_CHARACTER_CLASS_THIEF_ACROBAT
			0,		990,	100,	110,	 90,	000,	 80,	990,	100,	// DND_CHARACTER_CLASS_ASSASSIN

			0,		990,	116,	 87,	000,	000,	000,	000,	116,	// DND_CHARACTER_CLASS_MAGE
			0,		990,	000,	000,	000,	000,	 78,	000,	000,	// DND_CHARACTER_CLASS_ILLUSIONIST
		};

	int nRaceIndex = 0;
	switch(pCharacter->m_nRace)
	{
		case DND_CHARACTER_RACE_HUMAN:		nRaceIndex = 1; break;
		case DND_CHARACTER_RACE_ELF:		nRaceIndex = 2; break;
		case DND_CHARACTER_RACE_HALF_ELF:	nRaceIndex = 3; break;
		case DND_CHARACTER_RACE_DWARF:		nRaceIndex = 4; break;
		case DND_CHARACTER_RACE_HALFLING:	nRaceIndex = 5; break;
		case DND_CHARACTER_RACE_GNOME:		nRaceIndex = 6; break;
		case DND_CHARACTER_RACE_HALF_ORC:	nRaceIndex = 7; break;
		case DND_CHARACTER_RACE_DROW:		nRaceIndex = 8; break;

		case DND_CHARACTER_RACE_HALFLING_TALLFELLOW:	nRaceIndex = 5; break;
		case DND_CHARACTER_RACE_HALFLING_STOUT:			nRaceIndex = 5; break;
	}

	int nClassIndex = 0;
	switch(nClass)
	{
		case DND_CHARACTER_CLASS_FIGHTER:		nClassIndex = 1; break;
		case DND_CHARACTER_CLASS_RANGER:		nClassIndex = 2; break;
		case DND_CHARACTER_CLASS_CAVALIER:		nClassIndex = 3; break;
		case DND_CHARACTER_CLASS_PALADIN:		nClassIndex = 4; break;
		case DND_CHARACTER_CLASS_BARBARIAN:		nClassIndex = 5; break;

		case DND_CHARACTER_CLASS_CLERIC:		nClassIndex = 6; break;
		case DND_CHARACTER_CLASS_DRUID:			nClassIndex = 7; break;
		case DND_CHARACTER_CLASS_MONK:			nClassIndex = 8; break;

		case DND_CHARACTER_CLASS_THIEF:			nClassIndex = 9; break;
		case DND_CHARACTER_CLASS_THIEF_ACROBAT:	nClassIndex = 10; break;
		case DND_CHARACTER_CLASS_ASSASSIN:		nClassIndex = 11; break;

		case DND_CHARACTER_CLASS_MAGE:			nClassIndex = 12; break;
		case DND_CHARACTER_CLASS_ILLUSIONIST:	nClassIndex = 13; break;
	}


	int nTableLevel = _LevelLimitTable[nClassIndex][nRaceIndex];
	int nMaxLevel = nTableLevel / 10;

	int nException = nTableLevel % 10;

	switch(nException)
	{
		case 1: // Dwarven fighters with less than 17 strength are limited to 7th level; those with 17 strength are limited to 8th level.
		{
			if(pCharacter->m_nDisplayStats[ATTRIB_STR] < 17)
				nMaxLevel = 7;
			else if(pCharacter->m_nDisplayStats[ATTRIB_STR] < 18)
				nMaxLevel = 8;

			break;
		}
		case 2: // Elven fighters with less than 17 strength are limited to 5th level; those with 17 strength are limited to 6th level
		{
			if(pCharacter->m_nDisplayStats[ATTRIB_STR] < 17)
				nMaxLevel = 5;
			else if(pCharacter->m_nDisplayStats[ATTRIB_STR] < 18)
				nMaxLevel = 6;

			break;
		}
		case 3: // Gnome fighters of less than 18 strength are limited to 5th level
		{
			if(pCharacter->m_nDisplayStats[ATTRIB_STR] < 18)
				nMaxLevel = 5;

			break;
		}
		case 4:	// Half-elven fighters of less than 17 strength are limited to 6th level; those of 17 strength are limited to 7th level.
		{
			if(pCharacter->m_nDisplayStats[ATTRIB_STR] < 17)
				nMaxLevel = 6;
			else if(pCharacter->m_nDisplayStats[ATTRIB_STR] < 18)
				nMaxLevel = 7;

			break;
		}
		case 5: //  Halfling fighters of hairfeet sub-race, as well as all other types of sub-races with strength of under 17, are limited to 4th level. Tallfellows of 17 strength and
		{		//  Stouts of 18 strength can work up to 5th level. Tallfellows that somehow obtain 18 strength can work up to 6th level.
			if(pCharacter->m_nDisplayStats[ATTRIB_STR] < 17)
				nMaxLevel = 4;
			else if(pCharacter->m_nDisplayStats[ATTRIB_STR] < 18)
				nMaxLevel = 5;

			break;
		}
		case 6:	// Elven magic-users with intelligence of less than 17 are limited to 9th level; those with intelligence of 17 are limited to 10th level.
		{
			if(pCharacter->m_nDisplayStats[ATTRIB_INT] < 17)
				nMaxLevel = 9;
			else if(pCharacter->m_nDisplayStats[ATTRIB_INT] < 18)
				nMaxLevel = 10;

			break;
		}
		case 7: // Half-elven magic-users with intelligence of less than 17 are limited to 6th level; those with intelligence of 17 are limited to 7th level.
		{
			if(pCharacter->m_nDisplayStats[ATTRIB_INT] < 17)
				nMaxLevel = 6;
			else if(pCharacter->m_nDisplayStats[ATTRIB_INT] < 18)
				nMaxLevel = 7;

			break;
		}
		case 8: // Gnome illusionists with intelligence or dexterity under 17 are limited to 5th level; those with both intelligence and dexterity of 17 are limited to 6th level.
		{
			if(pCharacter->m_nDisplayStats[ATTRIB_INT] < 17 || pCharacter->m_nDisplayStats[ATTRIB_DEX] < 17)
				nMaxLevel = 5;
			else if(pCharacter->m_nDisplayStats[ATTRIB_INT] < 18 || pCharacter->m_nDisplayStats[ATTRIB_DEX] < 18)
				nMaxLevel = 6;

			break;
		}
		case 9: // Half-Orc thieves with dexterity of less than 17 are limited to 6th level; those with dexterity of 17 are limited to 7th level.
		{
			if(pCharacter->m_nDisplayStats[ATTRIB_DEX] < 17)
				nMaxLevel = 6;
			else if(pCharacter->m_nDisplayStats[ATTRIB_DEX] < 18)
				nMaxLevel = 7;

			break;
		}
	}

	if(nMaxLevel < 99)
	{
		if(g_bUseUnearthedArcana)
		{
			if(pCharacter->m_Class[1] == DND_CHARACTER_CLASS_UNDEF) // single classed char gets +2 on level limits
			{
				nMaxLevel += 2; 
			}
		}
	}

	if(nMaxLevel < 0)
	{
		TRACE("OWCH!\n");
	}

	return nMaxLevel;

}


int CalculateSTRAdjustments(cDNDCharacter *pCharacter, int *pnDamAdj, int *pnWeightAllow, int *pnOpenDoors, int *pnBendBars)
{
	int nRetVal = 0;

	if(pCharacter->m_nDisplayStats[ATTRIB_STR] == 18 && CharacterIsFighter(pCharacter) && pCharacter->m_nDisplayStats[ATTRIB_EXSTR] > 0)
	{
		if(pCharacter->m_nDisplayStats[ATTRIB_EXSTR] <= 50)
		{
			nRetVal = 1;	*pnDamAdj = 3;	*pnWeightAllow = 1000; *pnOpenDoors = 3; *pnBendBars = 20;
		}
		else if(pCharacter->m_nDisplayStats[ATTRIB_EXSTR] <= 75)
		{
			nRetVal = 2;	*pnDamAdj = 3;	*pnWeightAllow = 1250; *pnOpenDoors = 4; *pnBendBars = 25;
		}
		else if(pCharacter->m_nDisplayStats[ATTRIB_EXSTR] <= 90)
		{
			nRetVal = 2;	*pnDamAdj = 4; *pnWeightAllow = 1500; *pnOpenDoors = 4; *pnBendBars = 30;
		}
		else if(pCharacter->m_nDisplayStats[ATTRIB_EXSTR] <= 99)
		{
			nRetVal = 2;	*pnDamAdj = 5; *pnWeightAllow = 2000; *pnOpenDoors = 4; *pnBendBars = 35;
		}
		else if(pCharacter->m_nDisplayStats[ATTRIB_EXSTR] == 100)
		{
			nRetVal = 3;	*pnDamAdj = 6; *pnWeightAllow = 3000; *pnOpenDoors = 5; *pnBendBars = 40;
		}

		return nRetVal;
	}

	switch(pCharacter->m_nDisplayStats[ATTRIB_STR])
	{
		case 0:
		case 1:
		case 2:
		case 3:		nRetVal = -3;	*pnDamAdj = -1;	*pnWeightAllow = -350; *pnOpenDoors = 1; *pnBendBars = 0;	break;

		case 4:		nRetVal = -2;	*pnDamAdj = -1;	*pnWeightAllow = -250; *pnOpenDoors = 1; *pnBendBars = 0;	break;
		case 5:		nRetVal = -1;	*pnDamAdj = 0;	*pnWeightAllow = -250; *pnOpenDoors = 1; *pnBendBars = 0;	break;
		case 6:		nRetVal = 0;	*pnDamAdj = 0;	*pnWeightAllow = -150; *pnOpenDoors = 1; *pnBendBars = 0;	break;
		case 7:		nRetVal = 0;	*pnDamAdj = 0;	*pnWeightAllow = -150; *pnOpenDoors = 1; *pnBendBars = 0;	break;
		case 8:		nRetVal = 0;	*pnDamAdj = 0;	*pnWeightAllow = 0;	   *pnOpenDoors = 2; *pnBendBars = 1;	break;
		case 9:		nRetVal = 0;	*pnDamAdj = 0;	*pnWeightAllow = 0;	   *pnOpenDoors = 2; *pnBendBars = 1;	break;
		case 10:	nRetVal = 0;	*pnDamAdj = 0;	*pnWeightAllow = 0;	   *pnOpenDoors = 2; *pnBendBars = 2;	break;
		case 11:	nRetVal = 0;	*pnDamAdj = 0;	*pnWeightAllow = 0;	   *pnOpenDoors = 2; *pnBendBars = 2;	break;
		case 12:	nRetVal = 0;	*pnDamAdj = 0;	*pnWeightAllow = 100;  *pnOpenDoors = 2; *pnBendBars = 4;	break;
		case 13:	nRetVal = 0;	*pnDamAdj = 0;	*pnWeightAllow = 100;  *pnOpenDoors = 2; *pnBendBars = 4;	break;
		case 14:	nRetVal = 0;	*pnDamAdj = 0;	*pnWeightAllow = 200;  *pnOpenDoors = 2; *pnBendBars = 7;	break;
		case 15:	nRetVal = 0;	*pnDamAdj = 0;	*pnWeightAllow = 200;  *pnOpenDoors = 2; *pnBendBars = 7;	break;

		case 16:	nRetVal = 0;	*pnDamAdj = 1;  *pnWeightAllow = 350;  *pnOpenDoors = 3; *pnBendBars = 10;	break;
		case 17:	nRetVal = 1;	*pnDamAdj = 1;	*pnWeightAllow = 500;  *pnOpenDoors = 3; *pnBendBars = 13;	break;
		case 18:	nRetVal = 1;	*pnDamAdj = 2;	*pnWeightAllow = 750;  *pnOpenDoors = 3; *pnBendBars = 16;	break;

		case 19:	nRetVal = 3;	*pnDamAdj = 7;	*pnWeightAllow = 4500;  *pnOpenDoors = 7; *pnBendBars = 50;	break;
		case 20:	nRetVal = 3;	*pnDamAdj = 8;  *pnWeightAllow = 5000;  *pnOpenDoors = 7; *pnBendBars = 60;	break;
		case 21:	nRetVal = 4;	*pnDamAdj = 9;	*pnWeightAllow = 6000;  *pnOpenDoors = 9; *pnBendBars = 70;	break;
		case 22:	nRetVal = 4;	*pnDamAdj = 10;	*pnWeightAllow = 7500;  *pnOpenDoors = 11; *pnBendBars = 80;	break;
		case 23:	nRetVal = 5;	*pnDamAdj = 11;	*pnWeightAllow = 9000;  *pnOpenDoors = 11; *pnBendBars = 90;	break;
		case 24:	nRetVal = 6;	*pnDamAdj = 12;	*pnWeightAllow = 12000;  *pnOpenDoors = 19; *pnBendBars = 100;	break;
		case 25:	nRetVal = 7;	*pnDamAdj = 14;	*pnWeightAllow = 15000;  *pnOpenDoors = 23; *pnBendBars = 100;	break;
	}

	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	int nMonkClass = IsMonkClass(pCharacter);

	if(pApp->m_Settings.m_bMonkStrengthAdjustments == FALSE)
	{
		if(nMonkClass >= 0)
		{
			nRetVal = 0;
			*pnDamAdj = 0;
		}
	}


	return nRetVal;
}

int CalculateINTAdjustments(cDNDCharacter *pCharacter, int *pnKnowSpell, int *pnMinSpells, int *pnMaxSpells)
{
	int nRetVal = 0;

	switch(pCharacter->m_nDisplayStats[ATTRIB_INT])
	{
		case 0:
		case 1:
		case 2:
		case 3:		nRetVal = 0;	*pnKnowSpell = 0;	*pnMinSpells = 0;	*pnMaxSpells = 0; break;

		case 4:		nRetVal = 0;	*pnKnowSpell = 0;	*pnMinSpells = 0;	*pnMaxSpells = 0;  break;
		case 5:		nRetVal = 0;	*pnKnowSpell = 0;	*pnMinSpells = 0;	*pnMaxSpells = 0;  break;
		case 6:		nRetVal = 0;	*pnKnowSpell = 0;	*pnMinSpells = 0;	*pnMaxSpells = 0;  break;

		case 7:		nRetVal = 0;	*pnKnowSpell = 0;	*pnMinSpells = 0;	*pnMaxSpells = 0;  break;
		case 8:		nRetVal = 1;	*pnKnowSpell = 0;	*pnMinSpells = 0;	*pnMaxSpells = 0;  break;
		case 9:		nRetVal = 1;	*pnKnowSpell = 35;	*pnMinSpells = 4;	*pnMaxSpells = 6;  break;
		case 10:	nRetVal = 2;	*pnKnowSpell = 45;	*pnMinSpells = 4;	*pnMaxSpells = 6;  break;
		case 11:	nRetVal = 2;	*pnKnowSpell = 45;	*pnMinSpells = 5;	*pnMaxSpells = 7;  break;
		case 12:	nRetVal = 3;	*pnKnowSpell = 45;	*pnMinSpells = 5;	*pnMaxSpells = 7;  break;
		case 13:	nRetVal = 3;	*pnKnowSpell = 55;	*pnMinSpells = 6;	*pnMaxSpells = 9;  break;
		case 14:	nRetVal = 4;	*pnKnowSpell = 55;	*pnMinSpells = 6;	*pnMaxSpells = 9;  break;

		case 15:	nRetVal = 4;	*pnKnowSpell = 65;	*pnMinSpells = 7;	*pnMaxSpells = 11; break;
		case 16:	nRetVal = 5;	*pnKnowSpell = 65;	*pnMinSpells = 7;	*pnMaxSpells = 11; break;
		case 17:	nRetVal = 6;	*pnKnowSpell = 75;	*pnMinSpells = 8; 	*pnMaxSpells = 14; break;
		case 18:	nRetVal = 7;	*pnKnowSpell = 85;	*pnMinSpells = 9;	*pnMaxSpells = 18; break;

		case 19:	nRetVal = 8;	*pnKnowSpell = 95;	*pnMinSpells = 10;	*pnMaxSpells = 9999;  break;
		case 20:	nRetVal = 9;	*pnKnowSpell = 96;	*pnMinSpells = 11;	*pnMaxSpells = 9999;  break;
		case 21:	nRetVal = 10;	*pnKnowSpell = 97;	*pnMinSpells = 12;	*pnMaxSpells = 9999;  break;
		case 22:	nRetVal = 11;	*pnKnowSpell = 98;	*pnMinSpells = 13;	*pnMaxSpells = 9999;  break;
		case 23:	nRetVal = 12;	*pnKnowSpell = 99;	*pnMinSpells = 14;	*pnMaxSpells = 9999;  break;
		case 24:	nRetVal = 13;	*pnKnowSpell = 100;	*pnMinSpells = 15;	*pnMaxSpells = 9999;  break;
		case 25:	nRetVal = 14;	*pnKnowSpell = 100;	*pnMinSpells = 16;	*pnMaxSpells = 9999;  break;
	}

	return nRetVal;
}

int CalculateWISAdjustments(cDNDCharacter *pCharacter,  char **pszSpellBonus, int *pnSpellFailure)
{
	int nRetVal = 0;

	static char szSpellBonuses[14][32] = 
	{
		{"-\0"},
		{"1 1st\0"},
		{"2 1st\0"},
		{"1 2nd\0"},
		{"2 2nd\0"},
		{"1 3rd\0"},
		{"1 4th\0"},

		{"2 4th\0"},	//19
		{"3 4th\0"},	//20
		{"1 5th\0"},	//21
		{"2 5th\0"},	//22
		{"4 5th\0"},	//23
		{"2 6th\0"},	//24
		{"1 7th\0"}		//25

	};

	*pszSpellBonus = szSpellBonuses[0];
	
	switch(pCharacter->m_nDisplayStats[ATTRIB_WIS])
	{
		case 0:
		case 1:
		case 2:
		case 3:		nRetVal = -3;	*pszSpellBonus = szSpellBonuses[0];		*pnSpellFailure = 100; break;
																			
		case 4:		nRetVal = -2;	*pszSpellBonus = szSpellBonuses[0];		*pnSpellFailure = 100;  break;
		case 5:		nRetVal = -1;	*pszSpellBonus = szSpellBonuses[0];		*pnSpellFailure = 100;  break;
		case 6:		nRetVal = -1;	*pszSpellBonus = szSpellBonuses[0];		*pnSpellFailure = 100;  break;
																			
		case 7:		nRetVal = -1;	*pszSpellBonus = szSpellBonuses[0];		*pnSpellFailure = 100;  break;
		case 8:		nRetVal = 0;	*pszSpellBonus = szSpellBonuses[0];		*pnSpellFailure = 100;  break;
		case 9:		nRetVal = 0;	*pszSpellBonus = szSpellBonuses[0];		*pnSpellFailure = 100;  break;
		case 10:	nRetVal = 0;	*pszSpellBonus = szSpellBonuses[0];		*pnSpellFailure = 20;  break;
		case 11:	nRetVal = 0;	*pszSpellBonus = szSpellBonuses[0];		*pnSpellFailure = 15;  break;
		case 12:	nRetVal = 0;	*pszSpellBonus = szSpellBonuses[0];		*pnSpellFailure = 10;  break;
		case 13:	nRetVal = 0;	*pszSpellBonus = szSpellBonuses[1];		*pnSpellFailure = 5;  break;
		case 14:	nRetVal = 0;	*pszSpellBonus = szSpellBonuses[2];		*pnSpellFailure = 0;  break;
																			
		case 15:	nRetVal = 1;	*pszSpellBonus = szSpellBonuses[3];		*pnSpellFailure = 0; break;
		case 16:	nRetVal = 2;	*pszSpellBonus = szSpellBonuses[4];		*pnSpellFailure = 0; break;
		case 17:	nRetVal = 3;	*pszSpellBonus = szSpellBonuses[5];		*pnSpellFailure = 0; break;
		case 18:	nRetVal = 4;	*pszSpellBonus = szSpellBonuses[6];		*pnSpellFailure = 0; break;

		case 19:	nRetVal = 5;	*pszSpellBonus = szSpellBonuses[7];		*pnSpellFailure = 0;  break;
		case 20:	nRetVal = 6;	*pszSpellBonus = szSpellBonuses[8];		*pnSpellFailure = 0;  break;
		case 21:	nRetVal = 7;	*pszSpellBonus = szSpellBonuses[9];		*pnSpellFailure = 0;  break;
		case 22:	nRetVal = 8;	*pszSpellBonus = szSpellBonuses[10];	*pnSpellFailure = 0;  break;
		case 23:	nRetVal = 9;	*pszSpellBonus = szSpellBonuses[11];	*pnSpellFailure = 0;  break;
		case 24:	nRetVal = 10;	*pszSpellBonus = szSpellBonuses[12];	*pnSpellFailure = 0;  break;
		case 25:	nRetVal = 11;	*pszSpellBonus = szSpellBonuses[13];	*pnSpellFailure = 0;  break;
	}
	
	//TODO fix stats for 18+ abilities

	return nRetVal;
}


int CalculateDEXAdjustments(cDNDCharacter *pCharacter, int *pnDefenseAdj)
{
	int nRetVal = 0;

	switch(pCharacter->m_nDisplayStats[ATTRIB_DEX])
	{
		case 0:
		case 1:
		case 2:
		case 3:		nRetVal = -3;	*pnDefenseAdj = 5;	break;

		case 4:		nRetVal = -2;	*pnDefenseAdj = 3;	break;
		case 5:		nRetVal = -1;	*pnDefenseAdj = 2;	break;
		case 6:		nRetVal = 0;	*pnDefenseAdj = 1;	break;

		case 7:		nRetVal = 0;	*pnDefenseAdj = 0;	break;
		case 8:		nRetVal = 0;	*pnDefenseAdj = 0;	break;
		case 9:		nRetVal = 0;	*pnDefenseAdj = 0;	break;
		case 10:	nRetVal = 0;	*pnDefenseAdj = 0;	break;
		case 11:	nRetVal = 0;	*pnDefenseAdj = 0;	break;
		case 12:	nRetVal = 0;	*pnDefenseAdj = 0;	break;
		case 13:	nRetVal = 0;	*pnDefenseAdj = 0;	break;
		case 14:	nRetVal = 0;	*pnDefenseAdj = 0;	break;

		case 15:	nRetVal = 0;	*pnDefenseAdj = -1;	break;
		case 16:	nRetVal = 1;	*pnDefenseAdj = -2;	break;
		case 17:	nRetVal = 2;	*pnDefenseAdj = -3;	break;
		case 18:	nRetVal = 3;	*pnDefenseAdj = -4;	break;

		case 19:	nRetVal = 3;	*pnDefenseAdj = -4;	break;	
		case 20:	nRetVal = 3;	*pnDefenseAdj = -4;	break;
		case 21:	nRetVal = 4;	*pnDefenseAdj = -5;	break;
		case 22:	nRetVal = 4;	*pnDefenseAdj = -5;	break;
		case 23:	nRetVal = 4;	*pnDefenseAdj = -5;	break;
		case 24:	nRetVal = 5;	*pnDefenseAdj = -6;	break;
		case 25:	nRetVal = 5;	*pnDefenseAdj = -6;	break;
	}

	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	if(pApp->m_Settings.m_bMonkDexderityAdjustments == FALSE)
	{
		if(IsMonkClass(pCharacter) >= 0)
		{
			*pnDefenseAdj = 0;
		}
	}

	return nRetVal;
}


int CalculateCONAdjustments(cDNDCharacter *pCharacter, int *pnSysShock, int *pnResSurvival)
{
	int nRetVal = 0;

	switch(pCharacter->m_nDisplayStats[ATTRIB_CON])
	{
		case 0:
		case 1:
		case 2:
		case 3:		nRetVal = -2;	*pnSysShock = 35;	*pnResSurvival = 40; break;

		case 4:		nRetVal = -1;	*pnSysShock = 40;	*pnResSurvival = 45; break;
		case 5:		nRetVal = -1;	*pnSysShock = 45;	*pnResSurvival = 50; break;
		case 6:		nRetVal = -1;	*pnSysShock = 50;	*pnResSurvival = 55; break;

		case 7:		nRetVal = 0;	*pnSysShock = 55;	*pnResSurvival = 60; break;
		case 8:		nRetVal = 0;	*pnSysShock = 60;	*pnResSurvival = 65; break;
		case 9:		nRetVal = 0;	*pnSysShock = 65;	*pnResSurvival = 70; break;
		case 10:	nRetVal = 0;	*pnSysShock = 70;	*pnResSurvival = 75; break;
		case 11:	nRetVal = 0;	*pnSysShock = 74;	*pnResSurvival = 80; break;
		case 12:	nRetVal = 0;	*pnSysShock = 80;	*pnResSurvival = 85; break;
		case 13:	nRetVal = 0;	*pnSysShock = 85;	*pnResSurvival = 90; break;
		case 14:	nRetVal = 0;	*pnSysShock = 88;	*pnResSurvival = 92; break;

		case 15:	nRetVal = 1;	*pnSysShock = 91;	*pnResSurvival = 94; break;
		case 16:	nRetVal = 2;	*pnSysShock = 95;	*pnResSurvival = 96; break;
		case 17:	nRetVal = 3;	*pnSysShock = 97;	*pnResSurvival = 98; break;
		case 18:	nRetVal = 4;	*pnSysShock = 99;	*pnResSurvival = 100; break;

		case 19:	nRetVal = 5;	*pnSysShock = 100;	*pnResSurvival = 100; break;	
		case 20:	nRetVal = 5;	*pnSysShock = 100;	*pnResSurvival = 100; break;
		case 21:	nRetVal = 6;	*pnSysShock = 100;	*pnResSurvival = 100; break;
		case 22:	nRetVal = 6;	*pnSysShock = 100;	*pnResSurvival = 100; break;
		case 23:	nRetVal = 6;	*pnSysShock = 100;	*pnResSurvival = 100; break;
		case 24:	nRetVal = 7;	*pnSysShock = 100;	*pnResSurvival = 100; break;
		case 25:	nRetVal = 7;	*pnSysShock = 100;	*pnResSurvival = 100; break;
	}

	if(nRetVal > 2 && !CharacterIsFighter(pCharacter))
	{
		nRetVal = 2;
	}
	else if(pCharacter->m_Class[0] == DND_CHARACTER_CLASS_BARBARIAN)
	{
		nRetVal *=2;
	}

	return nRetVal;
}

int CalculateCHAAdjustments(cDNDCharacter *pCharacter, int *pnLoyaltyBase, int *pnReactAdj)
{
	int nRetVal = 0;

	switch(pCharacter->m_nDisplayStats[ATTRIB_CHA])
	{
		case 0:
		case 1:
		case 2:
		case 3:		nRetVal = 1;	*pnLoyaltyBase = -30;	*pnReactAdj = -25; break;

		case 4:		nRetVal = 1;	*pnLoyaltyBase = -25;	*pnReactAdj = -20; break;
		case 5:		nRetVal = 2;	*pnLoyaltyBase = -20;	*pnReactAdj = -15; break;
		case 6:		nRetVal = 2;	*pnLoyaltyBase = -10;	*pnReactAdj = -10; break;

		case 7:		nRetVal = 3;	*pnLoyaltyBase = -5;	*pnReactAdj = -5; break;
		case 8:		nRetVal = 3;	*pnLoyaltyBase = 0;		*pnReactAdj = 0; break;
		case 9:		nRetVal = 4;	*pnLoyaltyBase = 0;		*pnReactAdj = 0; break;
		case 10:	nRetVal = 4;	*pnLoyaltyBase = 0;		*pnReactAdj = 0; break;
		case 11:	nRetVal = 4;	*pnLoyaltyBase = 0;		*pnReactAdj = 0; break;
		case 12:	nRetVal = 5;	*pnLoyaltyBase = 0;		*pnReactAdj = 0; break;
		case 13:	nRetVal = 5;	*pnLoyaltyBase = 0;		*pnReactAdj = 5; break;
		case 14:	nRetVal = 6;	*pnLoyaltyBase = 5;		*pnReactAdj = 10; break;

		case 15:	nRetVal = 7;	*pnLoyaltyBase = 15;	*pnReactAdj = 15; break;
		case 16:	nRetVal = 8;	*pnLoyaltyBase = 20;	*pnReactAdj = 25; break;
		case 17:	nRetVal = 10;	*pnLoyaltyBase = 30;	*pnReactAdj = 30; break;
		case 18:	nRetVal = 15;	*pnLoyaltyBase = 40;	*pnReactAdj = 35; break;	

		case 19:	nRetVal = 20;	*pnLoyaltyBase = 50;	*pnReactAdj = 40; break;
		case 20:	nRetVal = 25;	*pnLoyaltyBase = 60;	*pnReactAdj = 45; break;
		case 21:	nRetVal = 30;	*pnLoyaltyBase = 70;	*pnReactAdj = 50; break;
		case 22:	nRetVal = 35;	*pnLoyaltyBase = 80;	*pnReactAdj = 55; break;
		case 23:	nRetVal = 40;	*pnLoyaltyBase = 90;	*pnReactAdj = 60; break;
		case 24:	nRetVal = 45;	*pnLoyaltyBase = 100;	*pnReactAdj = 65; break;
		case 25:	nRetVal = 50;	*pnLoyaltyBase = 100;	*pnReactAdj = 70; break;
	}

	return nRetVal;
}



///////////////////////////////////
void GetToHitChart(cDNDCharacter *pCharacter, int *pnAttackMatrix)
{

	//TODO to hit calcs for multiclassed characters

	int _ClericMatrix[21][7] = 
	{
	// LV	1-3		4-6		7-9		10-12	13-15	16-18	19+
		{	25,		23,		21,		20,		20,		20,		19}, 	//-10
		{	24,		22,		20,		20,		20,		19,		18}, 	//-9
		{	23,		21,		20,		20,		20,		18,		17}, 	//-8
		{	22,		20,		20,		20,		19,		17,		16},	//-7
		{	21,		20,		20,		20,		18,		16,		15}, 	//-6
		{	20,		20,		20,		19,		17,		15,		14},	//-5
		{	20,		20,		20,		18,		16,		14,		13}, 	//-4
		{	20,		20,		19,		17,		15,		13,		12}, 	//-3
		{	20,		20,		18,		16,		14,		12,		11}, 	//-2
		{	20,		19,		17,		15,		13,		11,		10},  	//-1
		{	20,		18,		16,		14,		12,		10,		 9}, 	//0
		{	19,		17,		15,		13,		11,		 9,		 8},  	//1
		{	18,		16,		14,		12,		10,		 8,		 7},  	//2
		{	17,		15,		13,		11,		 9,		 7,		 6},  	//3
		{	16,		14,		12,		10,		 8,		 6,		 5},  	//4
		{	15,		13,		11,		 9,		 7,		 5,		 4}, 	//5
		{	14,		12,		10,		 8,		 6,		 4,		 3}, 	//6
		{	13,		11,		 9,		 7,		 5,		 3,		 2},  	//7
		{	12,		10,		 8,		 6,		 4,		 2,		 1},  	//8
		{	11,		 9,		 7,		 5,		 3,		 1,		 0}, 	//9
		{	10,		 8,		 6,		 4,		 2,		 0,		 -1}, 	//10
	};

	int _MagicUserMatrix[21][5] = 
	{
	// LV	1-5		6-10	11-15	16-20	21+ 
		{	26,		24,		21,		20,		20}, 	//-10
		{	25,		23,		20,		20,		20}, 	//-9
		{	24,		22,		20,		20,		19}, 	//-8
		{	23,		21,		20,		20,		18},	//-7
		{	22,		20,		20,		19,		17}, 	//-6
		{	21,		20,		20,		18,		16},	//-5
		{	20,		20,		20,		17,		15}, 	//-4
		{	20,		20,		19,		16,		14}, 	//-3
		{	20,		20,		18,		15,		13}, 	//-2
		{	20,		20,		17,		14,		12},  	//-1
		{	20,		19,		16,		13,		11}, 	//0
		{	20,		18,		15,		12,		10},  	//1
		{	19,		17,		14,		11,		 9},  	//2
		{	18,		16,		13,		10,		 8},  	//3
		{	17,		15,		12,		 9,		 7},  	//4
		{	16,		14,		11,		 8,		 6}, 	//5
		{	15,		13,		10,		 7,		 5}, 	//6
		{	14,		12,		 9,		 6,		 4},  	//7
		{	13,		11,		 8,		 5,		 3},  	//8
		{	12,		10,		 7,		 4,		 2}, 	//9
		{	11,		 9,		 6,		 3,		 1}, 	//10
	};

	int _FighterMatrix[21][10] = 
	{
	// LV	0	12	34	56	78	910	111213141516 17
		{	26,	25,	23,	21,	20, 20, 20, 18, 16, 14},	//-10
		{	25, 24, 22, 20, 20, 20, 19, 17, 15, 13},	//-9
		{	24, 23, 21, 20, 20, 20, 18, 16, 14, 12},	//-8
		{	23, 22, 20, 20, 20, 19, 17, 15, 13, 11},	//-7
		{	22, 21, 20, 20, 20, 18, 16, 14, 12, 10},	//-6
		{	21, 20, 20, 20, 19, 17, 15, 13, 11,  9},	//-5
		{	20, 20, 20, 20, 18, 16, 14, 12, 10,  8},	//-4
		{	20, 20, 20, 19, 17, 15, 13, 11,  9,  7},	//-3
		{	20, 20, 20, 18, 16, 14, 12, 10,  8,  6},	//-2
		{	20, 20, 19, 17, 15, 13, 11,  9,  7,  5},	//-1
		{	20, 20, 18, 16, 14, 12, 10,  8,  6,  4},	//0
		{	20, 19, 17, 15, 13, 11,  9,  7,  5,  3},	//1
		{	19, 18, 16, 14, 12, 10,  8,  6,  4,  2},	//2
		{	18, 17, 15, 13, 11,  9,  7,  5,  3,  1},	//3
		{	17, 16, 14, 12, 10,  8,  6,  4,  2,  0},	//4
		{	16, 15, 13, 11,  9,  7,  5,  3,  1, -1},	//5
		{	15, 14, 12, 10,  8,  6,  4,  2,  0, -2},	//6
		{	14, 13, 11,  9,  7,  5,  3,  1, -1, -3},	//7
		{	13, 12, 10,  8,  6,  4,  2,  0, -2, -4},	//8
		{	12, 11, 9,   7,  5,  3,  1, -1, -3, -5},	//9
		{	11, 10, 8,   6,  4,  2,  0, -2, -4, -6},	//10
	};

	int _ThiefMatrix[21][6] = 
	{
	// LV	1-4		5-8		9-12	13-16	17-20	21+
		{	26,		24,		21,		20,		20,		20}, 	//-10
		{	25,		23,		20,		20,		20,		19}, 	//-9
		{	24,		22,		20,		20,		20,		18}, 	//-8
		{	23,		21,		20,		20,		19,		17},	//-7
		{	22,		20,		20,		20,		18,		16}, 	//-6
		{	21,		20,		20,		19,		17,		15},	//-5
		{	20,		20,		20,		18,		16,		14}, 	//-4
		{	20,		20,		19,		17,		15,		13}, 	//-3
		{	20,		20,		18,		16,		14,		12}, 	//-2
		{	20,		20,		17,		15,		13,		11},  	//-1
		{	20,		19,		16,		14,		12,		10}, 	//0
		{	20,		18,		15,		13,		11,		 9},  	//1
		{	19,		17,		14,		12,		10,		 8},  	//2
		{	18,		16,		13,		11,		 9,		 7},  	//3
		{	17,		15,		12,		10,		 8,		 6},  	//4
		{	16,		14,		11,		 9,		 7,		 5}, 	//5
		{	15,		13,		10,		 8,		 6,		 4}, 	//6
		{	14,		12,		 9,		 7,		 5,		 3},  	//7
		{	13,		11,		 8,		 6,		 4,		 2},  	//8
		{	12,		10,		 7,		 5,		 3,		 1}, 	//9
		{	11,		 9,		 6,		 4,		 2,		 0}, 	//10
	};

	int _MonsterMatrix[21][12] = 
	{
	// HD  >1-1		1-1		1		1+		2-3+	 4-5+		6-7+	8-9+	10-11+	12-13+	14-15+	16+
		{   26,		25,		24,		23,		21,		 20,		20,		20,		20,		19,		18,		17  }, 	//-10									
		{   25,		24,		23,		22,		20,		 20,		20,		20,		19,		18,		17,		16  }, 	//-9									
		{   24,		23,		22,		21,		20,		 20,		20,		20,		18,		17,		16,		15  }, 	//-8									
		{   23,		22,		21,		20,		20,		 20,		20,		19,		17,		16,		15,		14  },	//-7									
		{   22,		21,		20,		20,		20,		 20,		19,		18,		16,		15,		14,		13  }, 	//-6									
		{   21,		20,		20,		20,		20,		 20,		18,		17,		15,		14,		13,		12  },	//-5									
		{   20,		20,		20,		20,		20,		 19,		17,		16,		14,		13,		12,		11  }, 	//-4									
		{   20,		20,		20,		20,		19,		 18,		16,		15,		13,		12,		11,		10  }, 	//-3									
		{   20,		20,		20,		20,		18,		 17,		15,		14,		12,		11,		10,		9	}, 	//-2									
		{   20,		20,		20,		19,		17,		 16,		14,		13,		11,		10,		9,		8	},  //-1									
		{   20,		20,		19,		18,		16,		 15,		13,		12,		10,		9,		8,		7	}, 	//0										
		{   20,		19,		18,		17,		15,		 14,		12,		11,		9,		8,		7,		6	},  //1										
		{   19,		18,		17,		16,		14,		 13,		11,		10,		8,		7,		6,		5	},  //2										
		{   18,		17,		16,		15,		13,		 12,		10,		9,		7,		6,		5,		4	},  //3										
		{   17,		16,		15,		14,		12,		 11,		9,		8,		6,		5,		4,		3	},  //4										
		{   16,		15,		14,		13,		11,		 10,		8,		7,		5,		4,		3,		2	}, 	//5										
		{   15,		14,		13,		12,		10,		 9,			7,		6,		4,		3,		2,		1	}, 	//6										
		{   14,		13,		12,		11,		9,		 8,			6,		5,		3,		2,		1,		0	},  //7										
		{   13,		12,		11,		10,		8,		 9,			5,		4,		2,		1,		0,		-1	},  //8										
		{   12,		11,		10,		9,		7,		 6,			4,		3,		1,		0,		-1,		-2	}, 	//9										
		{   11,		10,		9,		8,		6,		 5,			3,		2,		0,		-1,		-2,		-3	}, 	//10									
	};



	int nCol = 0;

	#if CUSTOM_CLASSES
	DND_CHARACTER_CLASSES nCombatClass = pCharacter->m_Class[0];

	if (IsCustomClass(nCombatClass))
	{
		nCombatClass = GetCustomClass(nCombatClass)->m_CombatClass;
	}

	switch (nCombatClass)
	#else
	switch(pCharacter->m_Class[0])
	#endif
	{
		case DND_CHARACTER_CLASS_CLERIC:
		case DND_CHARACTER_CLASS_DRUID:
		{
			nCol = min(pCharacter->m_nLevel[0]-1,19)/3;

			if(nCol < 0) nCol = 0;

			int nCount = 20;
			for(int i = 0; i < 21; ++i)
			{
				pnAttackMatrix[nCount--] = _ClericMatrix[i][nCol];
			}
			break;
		}


		case DND_CHARACTER_CLASS_MAGE:
		case DND_CHARACTER_CLASS_ILLUSIONIST:
		{
			nCol = min(pCharacter->m_nLevel[0]-1,21)/5;

			if(nCol < 0) nCol = 0;

			int nCount = 20;
			for(int i = 0; i < 21; ++i)
			{
				pnAttackMatrix[nCount--] = _MagicUserMatrix[i][nCol];
			}
			break;
		}

		case DND_CHARACTER_CLASS_THIEF:
		case DND_CHARACTER_CLASS_THIEF_ACROBAT:
		case DND_CHARACTER_CLASS_ASSASSIN:
		case DND_CHARACTER_CLASS_MONK:
		{
			nCol = min(pCharacter->m_nLevel[0]-1,21)/4; 

			if(nCol < 0) nCol = 0;

			int nCount = 20;
			for(int i = 0; i < 21; ++i)
			{
				pnAttackMatrix[nCount--] = _ThiefMatrix[i][nCol];
			}
			break;
		}

		case DND_CHARACTER_CLASS_MONSTER:
		{
			nCol = pCharacter->m_nLevel[0];

			if(nCol < 0) nCol = 0;
			if(nCol > 11) nCol = 11;

			int nCount = 20;
			for(int i = 0; i < 21; ++i)
			{
				pnAttackMatrix[nCount--] = _MonsterMatrix[i][nCol];
			}
			break;
		}

		default: //fighter chart is default
		{
			if(pCharacter->m_nLevel[0] > 0)
			{
				nCol = min(pCharacter->m_nLevel[0]-1,17)/2 +1;
			}

			if(nCol < 0) nCol = 0;

			int nCount = 20;
			for(int i = 0; i < 21; ++i)
			{
				pnAttackMatrix[nCount--] = _FighterMatrix[i][nCol];
			}
			break;
		}
	}

}

///////////////////////////////////
void GetSavingThrows(cDNDCharacter *pCharacter, int *pnSaveMatrix)
{

	int _ClericSaves[5][7] = 
	{
	// LV	1-3		4-6		7-9		10-12	13-15	16-18	19+
		{	10,		9,		 7,		 6,		 5,		 4,		2}, 	//Paralyzation, Poison, Death Magic
		{	13,		12,		10,		 9,		 8,		 7,		5}, 	//Petrification or Polymorph
		{	14,		13,		11,		10,		 9,		 8,		6}, 	//Rod, Staff or Wand
		{	16,		15,		13,		12,		11,		10,		8},		//Breath Weapon
		{	15,		14,		12,		11,		10,		 9,		7}, 	//Spell
	};

	int _FighterSaves[5][10] = 
	{
	// LV	 0		1-2		3-4		5-6		7-8		9-10	11-12	13-14	15-16	17+
		{	16,		14,		13,		11,		10,		 8,		7,		5,		4,		3}, 	//Paralyzation, Poison, Death Magic
		{	17,		15,		14,		12,		11,		 9,		8,		6,		5,		4}, 	//Petrification or Polymorph
		{	18,		16,		15,		13,		12,		10,		9,		7,		6,		5}, 	//Rod, Staff or Wand
		{	20,		17,		16,		13,		12,		 9,		8,		5,		4,		4},		//Breath Weapon
		{	19,		17,		16,		14,		13,		11,		10,		8,		7,		6}, 	//Spell
	};

	int _MagicUserSaves[5][5] = 
	{
	// LV	1-5		6-10	11-15	16-20	21+
		{	14,		13,		11,		10,		 8}, 	//Paralyzation, Poison, Death Magic
		{	13,		11,		 9,		 7,		 5}, 	//Petrification or Polymorph
		{	11,		 9,		 7,		 5,		 3}, 	//Rod, Staff or Wand
		{	15,		13,		11,		 9,		 7},	//Breath Weapon
		{	12,		10,		 8,		 6,		 4}, 	//Spell
	};

	int _ThiefSaves[5][6] = 
	{
	// LV	1-4		5-8		9-12	13-16	17-20	21+
		{	13,		12,		11,		10,		 9,		8}, 	//Paralyzation, Poison, Death Magic
		{	12,		11,		10,		 9,		 8,		7}, 	//Petrification or Polymorph
		{	14,		12,		10,		 8,		 6,		4}, 	//Rod, Staff or Wand
		{	16,		15,		14,		13,		12,		11},	//Breath Weapon
		{	15,		13,		11,		 9,		 7,		5}, 	//Spell
	};

	int nCol = 0;

	int nSaveAdjust = 0;

	switch(pCharacter->m_nRace)
	{
		case DND_CHARACTER_RACE_DWARF:
		case DND_CHARACTER_RACE_HALFLING:
		case DND_CHARACTER_RACE_GNOME:
		{
			nSaveAdjust = 4;

			break;
		}
	}

	if (pCharacter->m_nDualClassClass < 0 || pCharacter->m_nDualClassClass > 2)
	{
		pCharacter->m_nDualClassClass = 0;
	}

	int nClassLevel = pCharacter->m_nDualClassClass;

	#if CUSTOM_CLASSES

	DND_CHARACTER_CLASSES nSaveClass = pCharacter->m_Class[nClassLevel];

	if (IsCustomClass(nSaveClass))
	{
		nSaveClass = GetCustomClass(nSaveClass)->m_SavingThrowClass;
	}

	switch(nSaveClass)
	#else
	switch (pCharacter->m_Class[nClassLevel])
	#endif
	{
		case DND_CHARACTER_CLASS_CLERIC:
		case DND_CHARACTER_CLASS_DRUID:
		{
			nCol = min(pCharacter->m_nLevel[nClassLevel] - 1 + nSaveAdjust, 19) / 3;

			if(nCol < 0) nCol = 0;

			for(int i = 0; i < 5; ++i)
			{
				pnSaveMatrix[i] = _ClericSaves[i][nCol];
			}
			break;
		}


		case DND_CHARACTER_CLASS_MAGE:
		case DND_CHARACTER_CLASS_ILLUSIONIST:
		{
			nCol = min(pCharacter->m_nLevel[nClassLevel] - 1 + nSaveAdjust, 21) / 5;

			if(nCol < 0) nCol = 0;

			for(int i = 0; i < 5; ++i)
			{
				pnSaveMatrix[i] = _MagicUserSaves[i][nCol];
			}
			break;
		}

		case DND_CHARACTER_CLASS_THIEF:
		case DND_CHARACTER_CLASS_THIEF_ACROBAT:
		case DND_CHARACTER_CLASS_ASSASSIN:
		case DND_CHARACTER_CLASS_MONK:
		{
			nCol = min(pCharacter->m_nLevel[nClassLevel] - 1 + nSaveAdjust, 21) / 4;

			if(nCol < 0) nCol = 0;

			for(int i = 0; i < 5; ++i)
			{
				pnSaveMatrix[i] = _ThiefSaves[i][nCol];
			}
			break;
		}

		case DND_CHARACTER_CLASS_PALADIN:
		{
			if (pCharacter->m_nLevel[nClassLevel] > 0)
			{
				nCol = min(pCharacter->m_nLevel[nClassLevel] - 1 + nSaveAdjust, 17) / 2 + 1;
			}

			if (nCol < 0) nCol = 0;

			for (int i = 0; i < 5; ++i)
			{
				pnSaveMatrix[i] = max(_FighterSaves[i][nCol] - 2, 1);
			}
			break;
		}

		case DND_CHARACTER_CLASS_BARBARIAN:
		{
			if (pCharacter->m_nLevel[nClassLevel] > 0)
			{
				nCol = min(pCharacter->m_nLevel[nClassLevel] - 1 + nSaveAdjust, 17) / 2 + 1;
			}

			if (nCol < 0) nCol = 0;

			pnSaveMatrix[0] = max(_FighterSaves[0][nCol] - 4, 1);	//Paralyzation, Poison, Death Magic		+4 (yes fudged a little)
			pnSaveMatrix[1] = max(_FighterSaves[1][nCol] - 3, 1);	//Petrification or Polymorph	+3
			pnSaveMatrix[2] = max(_FighterSaves[2][nCol] - 2, 1);	//Rod, Staff or Wand	+2
			pnSaveMatrix[3] = max(_FighterSaves[3][nCol] - 2, 1);	//Breath Weapon	+2
			pnSaveMatrix[4] = max(_FighterSaves[4][nCol] - pCharacter->m_nLevel[nClassLevel] / 4, 1);	//Spell + nLevel / 4

			break;
		}

		case DND_CHARACTER_CLASS_FIGHTER:
		case DND_CHARACTER_CLASS_RANGER:
		{
			if (pCharacter->m_nLevel[nClassLevel] > 0)
			{
				nCol = min(pCharacter->m_nLevel[nClassLevel] - 1 + nSaveAdjust, 17) / 2 + 1;
			}

			if (nCol < 0) nCol = 0;

			for (int i = 0; i < 5; ++i)
			{
				pnSaveMatrix[i] = _FighterSaves[i][nCol];
			}
			break;
		}

		default: //fighter chart is default, this particular case is for monsters
		{
			for(int i = 0; i < 5; ++i)
			{
				if (pCharacter->m_nLevel[1] > 0)
				{
					int nLevel = pCharacter->m_nLevel[1]; // yes [1] - as this is where we store the saving throw level

					if (i && pCharacter->m_nDisplayStats[ATTRIB_INT] == 0) // see DMG page 79, 11. SAVING THROW MATRIX FOR MONSTERS part (D) for non-intelligent monsters
					{
						nLevel /= 2;
					}

					nCol = min(nLevel - 1 + nSaveAdjust, 17) / 2 + 1;
				}

				if (nCol < 0) nCol = 0;

				pnSaveMatrix[i] = _FighterSaves[i][nCol];
			}
			break;
		}
	}

	int nProtectionRing = 0;

	if(pCharacter->m_RingsWorn[0].IsProtectionRing())
	{
		nProtectionRing = pCharacter->m_RingsWorn[0].m_nMagicAdj;
	}
	if(pCharacter->m_RingsWorn[1].IsProtectionRing())
	{
		nProtectionRing = max(nProtectionRing, pCharacter->m_RingsWorn[0].m_nMagicAdj);
	}

	for(int i = 0; i < 5; ++i)
	{
		pnSaveMatrix[i] -= nProtectionRing;
	}

	// apply custom mods
	for (int i = 0; i < 5; ++i)
	{
		pnSaveMatrix[i] -= pCharacter->m_nSavingThrowModifiers[i];
	}
}

BOOL GetTurnUndeadMatrix(cDNDCharacter *pCharacter, int *pnTurnMatrix)
{
	memset(pnTurnMatrix, 0, 13*sizeof(int));

	int nTurnLevel = 0;

	for(int i = 0; i < 3; ++i)
	{
		if(pCharacter->m_Class[i] == DND_CHARACTER_CLASS_CLERIC)
		{
			nTurnLevel = pCharacter->m_nLevel[i];
			break;
		}
		if(pCharacter->m_Class[i] == DND_CHARACTER_CLASS_PALADIN)
		{
			nTurnLevel = pCharacter->m_nLevel[i] - 2;
			break;
		}
		#if CUSTOM_CLASSES
		if (IsCustomClass(pCharacter->m_Class[i]) && GetCustomClass(pCharacter->m_Class[i])->m_bTurnUndead)
		{
			nTurnLevel = pCharacter->m_nLevel[i] - (GetCustomClass(pCharacter->m_Class[i])->m_nTurnUndeadLevel - 1);
		}
		#endif
	}

	if(nTurnLevel <= 0)
	{
		return FALSE;
	}

	int _TurnMatrix[13][10] = 
	{
	// LV	1		2		3		4		5		6		7		8		9-13	14+
		{	10,		7,		4,		-1,		-1,		-2,		-2,		-2,		-2,		 -2}, 	// skeleton
		{	13,		10,		7,		-1,		-1,		-2,		-2,		-2,		-2,		 -2}, 	// zombie
		{	16,		13,		10,		 4,		-1,		-1,		-2,		-2,		-2,		 -2}, 	// ghoul
		{	19,		16,		13,		 7,		 4,		-1,		-1,		-2,		-2,		 -2},	// shadow
		{	20,		19,		16,		 10,	 7,		 4,		-1,		-1,		-2,		 -2}, 	// wight
		{	99,		20,		19,		 13,	 10,	 7,		4,		-1,		-1,		 -2}, 	// ghast
		{	99,		99,		20,		 16,	 13,	 10,	7,		4,		-1,		 -2}, 	// wraith
		{	99,		99,		99,		 20,	 16,	 13,	10,		7,		4,		 -1}, 	// mummy
		{	99,		99,		99,		 99,	 20,	 16,	13,		10,		7,		 -1},	// spectre
		{	99,		99,		99,		 99,	 99,	 20,	16,		13,		10,		  4}, 	// vampire
		{	99,		99,		99,		 99,	 99,	 99,	20,		16,		13,		  7}, 	// ghost
		{	99,		99,		99,		 99,	 99,	 99,	99,		19,		16,		 10}, 	// lich
		{	99,		99,		99,		 99,	 99,	 99,	99,		20,		19,		 13}, 	// special
	
	};

	nTurnLevel -=1;

	if(nTurnLevel >= 8 && nTurnLevel <= 12)
		nTurnLevel = 8;
	else if(nTurnLevel > 12)
		nTurnLevel = 9;

	for(int i = 0; i < 13; ++i)
	{
		pnTurnMatrix[i] = _TurnMatrix[i][nTurnLevel];
	}

	// apply custom mods
	for (i = 0; i < 13; ++i)
	{
		if (pnTurnMatrix[i] == 99 && pCharacter->m_nClericTurnModifiers[i] != 0)
		{
			pnTurnMatrix[i] = 20 - pCharacter->m_nClericTurnModifiers[i];
		}
		else
		{
			pnTurnMatrix[i] -= pCharacter->m_nClericTurnModifiers[i];
		}

		if (pnTurnMatrix[i] > 99)
		{
			pnTurnMatrix[i] = 99;
		}
		if (pnTurnMatrix[i] < -2)
		{
			pnTurnMatrix[i] = -2;
		}
	}


	return TRUE;

}

///////////////////////////////////
void GetThiefSkills(cDNDCharacter *pCharacter, float *pfThiefSkillMatrix)
{
	int nThiefLevel = 0;
	int nThiefClass = 0;
	int nThiefClassIndex = 0;

	for(int i = 0; i < 3; ++i)
	{
		switch(pCharacter->m_Class[i])
		{
			
			case DND_CHARACTER_CLASS_THIEF:
			case DND_CHARACTER_CLASS_THIEF_ACROBAT:		// TODO this probably isn't right for acrobats
			case DND_CHARACTER_CLASS_MONK:
			{
				nThiefClass = pCharacter->m_Class[i];
				nThiefLevel = pCharacter->m_nLevel[i];
				nThiefClassIndex = i;
				break;
			}

			case DND_CHARACTER_CLASS_ASSASSIN:
			{
				nThiefClass = pCharacter->m_Class[i];
				nThiefLevel = pCharacter->m_nLevel[i]-2;
				break;
			}

			#if CUSTOM_CLASSES
			case DND_CHARACTER_CLASS_CUSTOM_1:
			case DND_CHARACTER_CLASS_CUSTOM_2:
			case DND_CHARACTER_CLASS_CUSTOM_3:
			case DND_CHARACTER_CLASS_CUSTOM_4:
			case DND_CHARACTER_CLASS_CUSTOM_5:
			case DND_CHARACTER_CLASS_CUSTOM_6:
			case DND_CHARACTER_CLASS_CUSTOM_7:
			case DND_CHARACTER_CLASS_CUSTOM_8:
			case DND_CHARACTER_CLASS_CUSTOM_9:
			case DND_CHARACTER_CLASS_CUSTOM_10:
			case DND_CHARACTER_CLASS_CUSTOM_11:
			case DND_CHARACTER_CLASS_CUSTOM_12:
			{
				if (GetCustomClass(pCharacter->m_Class[i])->m_bThiefSkills)
				{
					nThiefClass = DND_CHARACTER_CLASS_THIEF;
					nThiefLevel = pCharacter->m_nLevel[i] - (GetCustomClass(pCharacter->m_Class[i])->m_nThiefLevel - 1);
				}

				break;
			}
			#endif
		}
	}

	if(nThiefLevel < 0) 
		nThiefLevel = 0;

	float _ThiefSkillMatrix[8][18] = 
	{
	// LV	0		1		2		3		4		5		6		7		8		9		10		11		12		13		14		15		16		17
		{	0,		30,		35,		40,		45,		50,		55,		60,		65,		70,		80,		90,		100,	105,	110,	115,	125,	125}, 		//Pick Pockets
		{	0,		25,		29,		33,		37,		42,		47,		52,		57,		62,		67,		72,		77,		82,		87,		92,		97,		 99}, 		//Open Locks
		{	0,		20,		25,		30,		35,		40,		45,		50,		55,		60,		65,		70,		75,		80,		85,		90,		95,		 99}, 		//Find/Remove Traps
		{	0,		15,		21,		27,		33,		40,		47,		55,		62,		70,		78,		86,		94,		99,		99,		99,		99,		 99},		//Move Silently
		{	0,		10,		15,		20,		25,		31,		37,		43,		49,		56,		63,		70,		77,		85,		93,		99,		99,		 99}, 		//Hide In Shadows
		{	0,		10,		10,		15,		15,		20,		20,		25,		25,		30,		30,		35,		35,		40,		40,		50,		50,		 55},		//Hear Noise
		{	0,		85,		86,		87,		88,		90,		92,		94,		96,		98,		99,		99.1f,	99.2f,	99.3f,	99.4f,	99.5f,	99.6f,	 99.7f}, 	//Climb Walls
		{	0,		 0,		0,		0,		20,		25,		30,		35,		40,		45,		50,		55,		60,		65,		70,		75,		80,		 80}, 		//Read Languages
																																			   
	};

	int nCol = min(nThiefLevel,17); 

	if(nCol < 0) 
		nCol = 0;

	if(pCharacter->m_nDisplayStats[ATTRIB_DEX] < 9)
		nCol = 0;

	int nCount = 20;
	for(i = 0; i < 8; ++i)
	{
		pfThiefSkillMatrix[i] = _ThiefSkillMatrix[i][nCol];
	}

	if(nCol > 0)
	{
		switch(pCharacter->m_nRace)
		{
			case DND_CHARACTER_RACE_DWARF:
			{
				pfThiefSkillMatrix[0] += 0;   //Pick Pockets     
				pfThiefSkillMatrix[1] += 10;   //Open Locks       
				pfThiefSkillMatrix[2] += 15;   //Find/Remove Traps
				pfThiefSkillMatrix[3] += 0;   //Move Silently    
				pfThiefSkillMatrix[4] += 0;   //Hide In Shadows  
				pfThiefSkillMatrix[5] += 0;   //Hear Noise       
				pfThiefSkillMatrix[6] += -10;   //Climb Walls      
				pfThiefSkillMatrix[7] += -5;   //Read Languages   
				break;
			}
			case DND_CHARACTER_RACE_ELF:
			{
				pfThiefSkillMatrix[0] += 5;   //Pick Pockets     
				pfThiefSkillMatrix[1] += -5;   //Open Locks       
				pfThiefSkillMatrix[2] += 0;   //Find/Remove Traps
				pfThiefSkillMatrix[3] += 5;   //Move Silently    
				pfThiefSkillMatrix[4] += 10;   //Hide In Shadows  
				pfThiefSkillMatrix[5] += 5;   //Hear Noise       
				pfThiefSkillMatrix[6] += 0;   //Climb Walls      
				pfThiefSkillMatrix[7] += 0;   //Read Languages   
				break;
			}
			case DND_CHARACTER_RACE_GNOME:
			{
				pfThiefSkillMatrix[0] += 0;   //Pick Pockets     
				pfThiefSkillMatrix[1] += 5;   //Open Locks       
				pfThiefSkillMatrix[2] += 10;   //Find/Remove Traps
				pfThiefSkillMatrix[3] += 5;   //Move Silently    
				pfThiefSkillMatrix[4] += 5;   //Hide In Shadows  
				pfThiefSkillMatrix[5] += 10;   //Hear Noise       
				pfThiefSkillMatrix[6] += -15;   //Climb Walls      
				pfThiefSkillMatrix[7] += 0;   //Read Languages   
				break;
			}
			case DND_CHARACTER_RACE_HALF_ELF:
				{
				pfThiefSkillMatrix[0] += 10;   //Pick Pockets     
				pfThiefSkillMatrix[1] += 0;   //Open Locks       
				pfThiefSkillMatrix[2] += 0;   //Find/Remove Traps
				pfThiefSkillMatrix[3] += 0;   //Move Silently    
				pfThiefSkillMatrix[4] += 5;   //Hide In Shadows  
				pfThiefSkillMatrix[5] += 0;   //Hear Noise       
				pfThiefSkillMatrix[6] += 0;   //Climb Walls      
				pfThiefSkillMatrix[7] += 0;   //Read Languages   
				break;
			}
			case DND_CHARACTER_RACE_HALFLING:
			{
				pfThiefSkillMatrix[0] += 5;   //Pick Pockets     
				pfThiefSkillMatrix[1] += 5;   //Open Locks       
				pfThiefSkillMatrix[2] += 5;   //Find/Remove Traps
				pfThiefSkillMatrix[3] += 10;   //Move Silently    
				pfThiefSkillMatrix[4] += 15;   //Hide In Shadows  
				pfThiefSkillMatrix[5] += 5;   //Hear Noise       
				pfThiefSkillMatrix[6] += -15;   //Climb Walls      
				pfThiefSkillMatrix[7] += -5;   //Read Languages   
				break;
			}
			case DND_CHARACTER_RACE_HALF_ORC:
			{
				pfThiefSkillMatrix[0] += -5;   //Pick Pockets     
				pfThiefSkillMatrix[1] += 5;   //Open Locks       
				pfThiefSkillMatrix[2] += 5;   //Find/Remove Traps
				pfThiefSkillMatrix[3] += 0;   //Move Silently    
				pfThiefSkillMatrix[4] += 0;   //Hide In Shadows  
				pfThiefSkillMatrix[5] += 5;   //Hear Noise       
				pfThiefSkillMatrix[6] += 5;   //Climb Walls      
				pfThiefSkillMatrix[7] += -10;   //Read Languages   
				break;
			}
		}

		switch(pCharacter->m_nDisplayStats[ATTRIB_DEX])
		{
			case 9:
			default:
			{
				pfThiefSkillMatrix[0] += -15;   //Pick Pockets     
				pfThiefSkillMatrix[1] += -10;   //Open Locks       
				pfThiefSkillMatrix[2] += -10;   //Find/Remove Traps
				pfThiefSkillMatrix[3] += -20;   //Move Silently    
				pfThiefSkillMatrix[4] += -10;   //Hide In Shadows    
				break;
			}
			case 10:
			{
				pfThiefSkillMatrix[0] += -10;   //Pick Pockets     
				pfThiefSkillMatrix[1] += -5;   //Open Locks       
				pfThiefSkillMatrix[2] += -10;   //Find/Remove Traps
				pfThiefSkillMatrix[3] += -15;   //Move Silently    
				pfThiefSkillMatrix[4] += -5;   //Hide In Shadows    
				break;
			}
			case 11:
			{
				pfThiefSkillMatrix[0] += -5;   //Pick Pockets     
				pfThiefSkillMatrix[1] += 0;   //Open Locks       
				pfThiefSkillMatrix[2] += -5;   //Find/Remove Traps
				pfThiefSkillMatrix[3] += -10;   //Move Silently    
				pfThiefSkillMatrix[4] += 0;   //Hide In Shadows  
 
				break;
			}
			case 12:
			{
				pfThiefSkillMatrix[0] += 0;   //Pick Pockets     
				pfThiefSkillMatrix[1] += 0;   //Open Locks       
				pfThiefSkillMatrix[2] += 0;   //Find/Remove Traps
				pfThiefSkillMatrix[3] += -5;   //Move Silently    
				pfThiefSkillMatrix[4] += 0;   //Hide In Shadows  
 
				break;
			}
			case 13:
			case 14:
			case 15:
			{
				break;
			}
			case 16:
			{
				pfThiefSkillMatrix[0] += 0;   //Pick Pockets     
				pfThiefSkillMatrix[1] += 5;   //Open Locks       
				pfThiefSkillMatrix[2] += 0;   //Find/Remove Traps
				pfThiefSkillMatrix[3] += 0;   //Move Silently    
				pfThiefSkillMatrix[4] += 0;   //Hide In Shadows  
 
				break;
			}
			case 17:
			{
				pfThiefSkillMatrix[0] += 5;   //Pick Pockets     
				pfThiefSkillMatrix[1] += 10;   //Open Locks       
				pfThiefSkillMatrix[2] += 0;   //Find/Remove Traps
				pfThiefSkillMatrix[3] += 5;   //Move Silently    
				pfThiefSkillMatrix[4] += 5;   //Hide In Shadows  
 
				break;
			}
			case 18:
			{
				pfThiefSkillMatrix[0] += 10;   //Pick Pockets     
				pfThiefSkillMatrix[1] += 15;   //Open Locks       
				pfThiefSkillMatrix[2] += 5;   //Find/Remove Traps
				pfThiefSkillMatrix[3] += 10;   //Move Silently    
				pfThiefSkillMatrix[4] += 10;   //Hide In Shadows  
 
				break;
			}

			case 19:
			{
				pfThiefSkillMatrix[0] += 15;   //Pick Pockets     
				pfThiefSkillMatrix[1] += 20;   //Open Locks       
				pfThiefSkillMatrix[2] += 10;   //Find/Remove Traps
				pfThiefSkillMatrix[3] += 12;   //Move Silently    
				pfThiefSkillMatrix[4] += 12;   //Hide In Shadows  
 
				break;
			}
			case 20:
			{
				pfThiefSkillMatrix[0] += 20;   //Pick Pockets     
				pfThiefSkillMatrix[1] += 25;   //Open Locks       
				pfThiefSkillMatrix[2] += 15;   //Find/Remove Traps
				pfThiefSkillMatrix[3] += 15;   //Move Silently    
				pfThiefSkillMatrix[4] += 15;   //Hide In Shadows  
 
				break;
			}
			case 21:
			{
				pfThiefSkillMatrix[0] += 25;   //Pick Pockets     
				pfThiefSkillMatrix[1] += 30;   //Open Locks       
				pfThiefSkillMatrix[2] += 20;   //Find/Remove Traps
				pfThiefSkillMatrix[3] += 18;   //Move Silently    
				pfThiefSkillMatrix[4] += 18;   //Hide In Shadows  
 
				break;
			}
			case 22:
			{
				pfThiefSkillMatrix[0] += 30;   //Pick Pockets     
				pfThiefSkillMatrix[1] += 35;   //Open Locks       
				pfThiefSkillMatrix[2] += 25;   //Find/Remove Traps
				pfThiefSkillMatrix[3] += 20;   //Move Silently    
				pfThiefSkillMatrix[4] += 20;   //Hide In Shadows  
 
				break;
			}
			case 23:
			{
				pfThiefSkillMatrix[0] += 35;   //Pick Pockets     
				pfThiefSkillMatrix[1] += 40;   //Open Locks       
				pfThiefSkillMatrix[2] += 30;   //Find/Remove Traps
				pfThiefSkillMatrix[3] += 23;   //Move Silently    
				pfThiefSkillMatrix[4] += 23;   //Hide In Shadows  
 
				break;
			}
			case 24:
			{
				pfThiefSkillMatrix[0] += 40;   //Pick Pockets     
				pfThiefSkillMatrix[1] += 45;   //Open Locks       
				pfThiefSkillMatrix[2] += 35;   //Find/Remove Traps
				pfThiefSkillMatrix[3] += 25;   //Move Silently    
				pfThiefSkillMatrix[4] += 25;   //Hide In Shadows  
 
				break;
			}
			case 25:
			{
				pfThiefSkillMatrix[0] += 45;   //Pick Pockets     
				pfThiefSkillMatrix[1] += 50;   //Open Locks       
				pfThiefSkillMatrix[2] += 40;   //Find/Remove Traps
				pfThiefSkillMatrix[3] += 30;   //Move Silently    
				pfThiefSkillMatrix[4] += 30;   //Hide In Shadows  
 
				break;
			}
		}

	
	}


	switch(pCharacter->m_Class[nThiefClassIndex])
	{
		case DND_CHARACTER_CLASS_MONK:
		{
			pfThiefSkillMatrix[0] = 0.0f; // monk cannot pick pockets
			pfThiefSkillMatrix[7] = 0.0f; // monk cannot read languages
			break;
		}
	}

	// apply custom mods
	for (i = 0; i < 8; ++i)
	{
		pfThiefSkillMatrix[i] += pCharacter->m_nThiefSkillModifiers[i];
	}

	//final sanity check
	for(i = 0; i < 8; ++i)
	{
		if(pfThiefSkillMatrix[i] < 0.0f)
			pfThiefSkillMatrix[i] = 0.0f;
	}

}

///////////////////////////////////
int GetAssassinationTable(cDNDCharacter *pCharacter, int *pnAssassinationMatrix)
{
	int nAssassinLevel = 0;
	int nAssassinClass = 0;

	for(int i = 0; i < 3; ++i)
	{
		switch(pCharacter->m_Class[i])
		{
			case DND_CHARACTER_CLASS_ASSASSIN:
			{
				nAssassinClass = pCharacter->m_Class[i];
				nAssassinLevel = pCharacter->m_nLevel[i];
				break;
			}
			#if CUSTOM_CLASSES
			case DND_CHARACTER_CLASS_CUSTOM_1:
			case DND_CHARACTER_CLASS_CUSTOM_2:
			case DND_CHARACTER_CLASS_CUSTOM_3:
			case DND_CHARACTER_CLASS_CUSTOM_4:
			case DND_CHARACTER_CLASS_CUSTOM_5:
			case DND_CHARACTER_CLASS_CUSTOM_6:
			case DND_CHARACTER_CLASS_CUSTOM_7:
			case DND_CHARACTER_CLASS_CUSTOM_8:
			case DND_CHARACTER_CLASS_CUSTOM_9:
			case DND_CHARACTER_CLASS_CUSTOM_10:
			case DND_CHARACTER_CLASS_CUSTOM_11:
			case DND_CHARACTER_CLASS_CUSTOM_12:
			{
				if (GetCustomClass(pCharacter->m_Class[i])->m_bAssassinSkills)
				{
					nAssassinClass = DND_CHARACTER_CLASS_ASSASSIN;
					nAssassinLevel = pCharacter->m_nLevel[i] - (GetCustomClass(pCharacter->m_Class[i])->m_nAssassinLevel - 1);
				}

				break;
			}
			#endif
		}
	}

	int _AssassinationMatrix[16][10] = 
	{
// VIC LV	0-1		2-3		4-5		6-7		8-9		10-11	12-13	14-15	16-17	18+		// ASS LVL
		{	0,		0,		0,		0,		0,		0,		0,		0,		0,		0 },	//	0		

		{	50,		45,		35,		25,		10,		1,		0,		0,		0,		0 },	//	1	
		{	55,		50,		40,		30,		15,		2,		0,		0,		0,		0 },	//	2	
		{	60,		55,		45,		35,		20,		5,		0,		0,		0,		0 },	//	3
		{	65,		60,		50,		40,		25,		10,		1,		0,		0,		0 },	//	4
		{	70,		65,		55,		45,		30,		15,		5,		0,		0,		0 },	//	5	
		{	75,		70,		60,		50,		35,		20,		10,		1,		0,		0 },	//	6
		{	80,		75,		65,		55,		40,		25,		15,		5,		0,		0 },	//	7		
		{	85,		80,		70,		60,		45,		30,		20,		10,		2,		0 },	//	8
		{	95,		90,		80,		70,		55,		40,		30,		20,		5,		0 },	//	9
		{	99,		95,		85,		75,		60,		45,		35,		25,		10,		1 },	//	10
		{	100,	99,		90,		80,		65,		50,		40,		30,		15,		5 },	//	11
		{	100,	100,	95,		85,		70,		55,		45,		35,		20,		10 },	//	12
		{	100,	100,	99,		95,		80,		65,		50,		40,		25,		15 },	//	13
		{	100,	100,	100,	99,		90,		75,		60,		50,		35,		25 },	//	14
		{	100,	100,	100,	100,	99,		85,		70,		60,		40,		30 }	//	15
	};																					 

	int nCol = min(nAssassinLevel,15); 

	if(nCol < 0) 
		nCol = 0;

	for(i = 0; i < 10; ++i)
	{
		pnAssassinationMatrix[i] = _AssassinationMatrix[nCol][i];
	}

	// apply custom mods
	for (i = 0; i < 10; ++i)
	{
		pnAssassinationMatrix[i] += pCharacter->m_nAssassinSkillModifiers[i];
	}
	
	return nAssassinLevel;
}

int GetScrollSpellFailureChance(cDNDCharacter *pCharacter, POBJECTTYPE pScroll, int _nSpellLevel, int *pnTotalFailure, int *pnReversed)
{
	int nRetPerc = 100;

	DND_CHARACTER_CLASSES _MagicType = DND_CHARACTER_CLASS_UNDEF;

	int nFlags = pScroll->m_nFlags;
	nFlags = ((nFlags / 100) % 100) % 10;

	switch(nFlags)
	{
		case 1:	_MagicType = DND_CHARACTER_CLASS_CLERIC; break;
		case 2:	_MagicType = DND_CHARACTER_CLASS_DRUID; break;
		case 3:	_MagicType = DND_CHARACTER_CLASS_ILLUSIONIST; break;
		case 4:	_MagicType = DND_CHARACTER_CLASS_MAGE; break;
	}


	int nCastingLevel = -1;
	int nSpellLevel = GetLevelCanCastSpell(pCharacter, _MagicType, _nSpellLevel, &nCastingLevel);

	if(nSpellLevel == 0)
		return nRetPerc;


	int nDiff = nSpellLevel - nCastingLevel;

	if(nDiff < 0)
		return 0;

	nRetPerc = nDiff * 5;
	
	*pnTotalFailure = 0;
	*pnReversed = 0;

	if(nDiff >=1 && nDiff <=3)
		*pnTotalFailure = 95;
	else if(nDiff >=4 && nDiff <=6)
		*pnTotalFailure = 85;
	else if(nDiff >=7 && nDiff <=9)
		*pnTotalFailure = 75;
	else if(nDiff >=10 && nDiff <=12)
		*pnTotalFailure = 65;
	else if(nDiff >=13 && nDiff <=15)
		*pnTotalFailure = 50;
	else if(nDiff >=16)
		*pnTotalFailure = 30;

	*pnReversed = 100 - *pnTotalFailure;

	return nRetPerc;
}

int GetLevelCanCastSpell(cDNDCharacter *pCharacter, DND_CHARACTER_CLASSES _nSpellClass, int _nSpellLevel, int *pnCastingLevel)
{
	int nRetLevel = 0;

	*pnCastingLevel = -1;

	for(int i = 0; i < 4; ++i)
	{
		for(int nLevel = 1; nLevel < 22; ++nLevel)
		{
			if(pCharacter->m_SpellClasses[i] == _nSpellClass)
			{
				nRetLevel = GetSpellLevels(pCharacter, _nSpellClass, nLevel, _nSpellLevel);

				if(nRetLevel)
				{
					*pnCastingLevel = pCharacter->m_nCastingLevels[i];
					return nLevel;
				}
			}
			else if(pCharacter->m_SpellClasses[i] == DND_CHARACTER_SPELL_CLASS_RANGER_DRUID && _nSpellClass == DND_CHARACTER_CLASS_DRUID)
			{
				nRetLevel = GetSpellLevels(pCharacter, DND_CHARACTER_SPELL_CLASS_RANGER_DRUID, nLevel, _nSpellLevel);

				if(nRetLevel)
				{
					*pnCastingLevel = pCharacter->m_nCastingLevels[i];
					return nLevel;
				}
			}
			else if(pCharacter->m_SpellClasses[i] == DND_CHARACTER_SPELL_CLASS_RANGER_MAGE && _nSpellClass == DND_CHARACTER_CLASS_MAGE)
			{
				nRetLevel = GetSpellLevels(pCharacter, DND_CHARACTER_SPELL_CLASS_RANGER_MAGE, nLevel, _nSpellLevel);

				if(nRetLevel)
				{
					*pnCastingLevel = pCharacter->m_nCastingLevels[i];
					return nLevel;
				}
			}
			else if(pCharacter->m_SpellClasses[i] == DND_CHARACTER_SPELL_CLASS_PALADIN_CLERIC && _nSpellClass == DND_CHARACTER_CLASS_CLERIC)
			{
				nRetLevel = GetSpellLevels(pCharacter, DND_CHARACTER_SPELL_CLASS_PALADIN_CLERIC, nLevel, _nSpellLevel);

				if(nRetLevel)
				{
					*pnCastingLevel = pCharacter->m_nCastingLevels[i];
					return nLevel;
				}
			}
		}
	}

	return nRetLevel;
}

int GetSpellLevels(cDNDCharacter *pCharacter, DND_CHARACTER_CLASSES _nSpellClass, int _nCastLevel, int _nSpellLevel)
{

	int nRetSpells = 0;

	int _ClericMatrix[30][8] = 
	{
	// SPLV	0		1		2		3		4	    5	    6 		 7   //LEVEL
		{	0,		0,		0,		0,		0,		0,		0,		0 },	//0
		{	0,		1,		0,		0,		0,		0,		0,		0 },	//1	
		{	0,		2,		0,		0,		0,		0,		0,		0 },	//2
		{	0,		2,		1,		0,		0,		0,		0,		0 }, 	//3
		{	0,		3,		2,		0,		0,		0,		0,		0 },	//4
		{	0,		3,		3,		1,		0,		0,		0,		0 }, 	//5
		{	0,		3,		3,		2,		0,		0,		0,		0 },	//6
		{	0,		3,		3,		2,		1,		0,		0,		0 }, 	//7
		{	0,		3,		3,		3,		2,		0,		0,		0 }, 	//8
		{	0,		4,		4,		3,		2,		1,		0,		0 }, 	//9
		{	0,		4,		4,		3,		3,		2,		0,		0 },  	//10
		{	0,		5,		4,		4,		3,		2,		1,		0 }, 	//11
		{	0,		6,		5,		5,		3,		2,		2,		0 },  	//12
		{	0,		6,		6,		6,		4,		2,		2,		0 },  	//13
		{	0,		6,		6,		6,		5,		3,		2,		0 },  	//14
		{	0,		7,		7,		7,		5,		4,		2,		0 },  	//15
		{	0,		7,		7,		7,		6,		5,		3,		1 }, 	//16
		{	0,		8,		8,		8,		6,		5,		3,		1 }, 	//17
		{	0,		8,		8,		8,		7,		6,		4,		1 },  	//18
		{	0,		9,		9,		9,		7,		6,		4,		2 },  	//19
		{	0,		9,		9,		9,		8,		7,		5,		2 }, 	//20
		{	0,		9,		9,		9,		9,		8,		6,		2 }, 	//21
		{	0,		9,		9,		9,		9,		9,		6,		3 }, 	//22
		{	0,		9,		9,		9,		9,		9,		7,		3 }, 	//23
		{	0,		9,		9,		9,		9,		9,		8,		3 }, 	//24
		{	0,		9,		9,		9,		9,		9,		8,		4 }, 	//25
		{	0,		9,		9,		9,		9,		9,		9,		4 }, 	//26
		{	0,		9,		9,		9,		9,		9,		9,		5 }, 	//27
		{	0,		9,		9,		9,		9,		9,		9,		6 }, 	//28
		{	0,		9,		9,		9,		9,		9,		9,		7 },    //29 	
	};

	int _DruidMatrix[15][8] = 
	{
	// SPLV	0		1		2		3		4	    5	    6 		 7   //LEVEL
		{	0,		0,		0,		0,		0,		0,		0,		0 },	//0
		{	0,		2,		0,		0,		0,		0,		0,		0 },	//1	
		{	0,		2,		1,		0,		0,		0,		0,		0 },	//2
		{	0,		3,		2,		1,		0,		0,		0,		0 }, 	//3
		{	0,		4,		2,		2,		0,		0,		0,		0 },	//4
		{	0,		4,		3,		2,		0,		0,		0,		0 }, 	//5
		{	0,		4,		3,		2,		1,		0,		0,		0 },	//6
		{	0,		4,		4,		3,		1,		0,		0,		0 }, 	//7
		{	0,		4,		4,		3,		2,		0,		0,		0 }, 	//8
		{	0,		5,		4,		3,		2,		1,		0,		0 }, 	//9
		{	0,		5,		4,		3,		3,		2,		0,		0 },  	//10
		{	0,		5,		5,		3,		3,		2,		1,		0 }, 	//11
		{	0,		5,		5,		4,		4,		3,		2,		1 },  	//12
		{	0,		6,		5,		5,		5,		4,		3,		2 },  	//13
		{	0,		6,		6,		6,		6,		5,		4,		3 },  	//14	
	};



	int _MagicUserMatrix[30][10] = 
	{
	// SPLV	0		1		2		3		4	    5	    6 		7		8 		9	//LEVEL
		{	0,		0,		0,		0,		0,		0,		0,		0, 		0,	 	0 },	//0
		{	0,		1,		0,		0,		0,		0,		0,		0, 		0,	 	0 },	//1	
		{	0,		2,		0,		0,		0,		0,		0,		0,		0, 		0 },	//2
		{	0,		2,		1,		0,		0,		0,		0,		0,		0, 		0 }, 	//3
		{	0,		3,		2,		0,		0,		0,		0,		0,		0, 		0 },	//4
		{	0,		4,		2,		1,		0,		0,		0,		0,		0, 		0 }, 	//5
		{	0,		4,		2,		2,		0,		0,		0,		0,		0, 		0 },	//6
		{	0,		4,		3,		2,		1,		0,		0,		0,		0, 		0 }, 	//7
		{	0,		4,		3,		3,		2,		0,		0,		0,		0, 		0 }, 	//8
		{	0,		4,		3,		3,		2,		1,		0,		0,		0, 		0 }, 	//9
		{	0,		4,		4,		3,		2,		2,		0,		0,		0, 		0 },  	//10
		{	0,		4,		4,		4,		3,		3,		0,		0,		0, 		0 }, 	//11
		{	0,		4,		4,		4,		4,		4,		1,		0,		0, 		0 },  	//12
		{	0,		5,		5,		5,		4,		4,		2,		0,		0, 		0 },  	//13
		{	0,		5,		5,		5,		4,		5,		2,		1,		0, 		0 },  	//14
		{	0,		5,		5,		5,		5,		5,		2,		1,		0, 		0 },  	//15
		{	0,		5,		5,		5,		5,		5,		3,		2,		1, 		0 }, 	//16
		{	0,		5,		5,		5,		5,		5,		3,		3,		2, 		0 }, 	//17
		{	0,		5,		5,		5,		5,		5,		3,		3,		2, 		1 },  	//18
		{	0,		5,		5,		5,		5,		5,		3,		3,		3, 		1 },  	//19
		{	0,		5,		5,		5,		5,		5,		4,		3,		3, 		2 }, 	//20
		{	0,		5,		5,		5,		5,		5,		4,		4,		4, 		2 }, 	//21
		{	0,		5,		5,		5,		5,		5,		5,		4,		4, 		3 }, 	//22
		{	0,		5,		5,		5,		5,		5,		5,		5,		5, 		3 }, 	//23
		{	0,		5,		5,		5,		5,		5,		5,		5,		5, 		4 }, 	//24
		{	0,		5,		5,		5,		5,		5,		5,		5,		5, 		5 }, 	//25
		{	0,		6,		6,		6,		6,		5,		5,		5,		5, 		5 }, 	//26
		{	0,		6,		6,		6,		6,		6,		6,		6,		5, 		5 }, 	//27
		{	0,		6,		6,		6,		6,		6,		6,		6,		6, 		6 }, 	//28
		{	0,		7,		7,		7,		7,		6,		6,		6,		6, 		6 },    //29 	
	};

	
	int _IllusionistMatrix[27][8] = 
	{
	// SPLV	0		1		2		3		4	    5	    6 		7 //LEVEL
		{	0,		0,		0,		0,		0,		0,		0,		0 },	//0
		{	0,		1,		0,		0,		0,		0,		0,		0 },	//1	
		{	0,		2,		0,		0,		0,		0,		0,		0 },	//2
		{	0,		2,		1,		0,		0,		0,		0,		0 }, 	//3
		{	0,		3,		2,		0,		0,		0,		0,		0 },	//4
		{	0,		4,		2,		1,		0,		0,		0,		0 }, 	//5
		{	0,		4,		3,		1,		0,		0,		0,		0 },	//6
		{	0,		4,		3,		2,		0,		0,		0,		0 }, 	//7
		{	0,		4,		3,		2,		1,		0,		0,		0 }, 	//8
		{	0,		5,		3,		3,		2,		0,		0,		0 }, 	//9
		{	0,		5,		4,		3,		2,		1,		0,		0 },  	//10
		{	0,		5,		4,		3,		3,		2,		0,		0 }, 	//11
		{	0,		5,		5,		4,		3,		2,		1,		0 },  	//12
		{	0,		5,		5,		4,		3,		2,		2,		0 },  	//13
		{	0,		5,		5,		4,		3,		2,		2,		1 },  	//14
		{	0,		5,		5,		4,		4,		2,		2,		2 },  	//15
		{	0,		5,		5,		5,		4,		3,		2,		2 }, 	//16
		{	0,		5,		5,		5,		5,		3,		2,		2 }, 	//17
		{	0,		5,		5,		5,		5,		3,		3,		2 },  	//18
		{	0,		5,		5,		5,		5,		4,		3,		2 },  	//19
		{	0,		5,		5,		5,		5,		4,		3,		3 }, 	//20
		{	0,		5,		5,		5,		5,		5,		4,		3 }, 	//21
		{	0,		5,		5,		5,		5,		5,		5,		4 }, 	//22
		{	0,		5,		5,		5,		5,		5,		5,		5 }, 	//23
		{	0,		6,		6,		6,		6,		5,		5,		5 }, 	//24
		{	0,		6,		6,		6,		6,		6,		6,		6 }, 	//25
		{	0,		7,		7,		7,		7,		6,		6,		6 }, 	//26	
	};
	

	int _RangerDruidMatrix[18][4] = 
	{
	// SPLV	0		1		2		3		//LEVEL
		{	0,		0,		0,		0 },	//0
		{	0,		0,		0,		0 },	//1	
		{	0,		0,		0,		0 },	//2
		{	0,		0,		0,		0 }, 	//3
		{	0,		0,		0,		0 },	//4
		{	0,		0,		0,		0 }, 	//5
		{	0,		0,		0,		0 },	//6
		{	0,		0,		0,		0 }, 	//7
		{	0,		1,		0,		0 }, 	//8
		{	0,		1,		0,		0 }, 	//9
		{	0,		2,		0,		0 },  	//10
		{	0,		2,		0,		0 }, 	//11
		{	0,		2,		1,		0 },  	//12
		{	0,		2,		1,		0 },  	//13
		{	0,		2,		2,		0 },  	//14
		{	0,		2,		2,		0 },  	//15
		{	0,		2,		2,		1 }, 	//16
		{	0,		2,		2,		2 }, 	//17
	
	};

	int _RangerMageMatrix[18][3] = 
	{
	// SPLV	0		1		2		//LEVEL
		{	0,		0,		0 },	//0
		{	0,		0,		0 },	//1	
		{	0,		0,		0 },	//2
		{	0,		0,		0 }, 	//3
		{	0,		0,		0 },	//4
		{	0,		0,		0 }, 	//5
		{	0,		0,		0 },	//6
		{	0,		0,		0 }, 	//7
		{	0,		0,		0 }, 	//8
		{	0,		1,		0 }, 	//9
		{	0,		1,		0 },  	//10
		{	0,		2,		0 }, 	//11
		{	0,		2,		0 },  	//12
		{	0,		2,		1 },  	//13
		{	0,		2,		1 },  	//14
		{	0,		2,		2 },  	//15
		{	0,		2,		2 }, 	//16
		{	0,		2,		2 }, 	//17
	
	};

	int _PaladinClericMatrix[21][5] = 
	{
	// SPLV	0		1		2		3		4		//LEVEL
		{	0,		0,		0,		0,		0 },	//0
		{	0,		0,		0,		0,		0 },	//1	
		{	0,		0,		0,		0,		0 },	//2
		{	0,		0,		0,		0,		0 }, 	//3
		{	0,		0,		0,		0,		0 },	//4
		{	0,		0,		0,		0,		0 }, 	//5
		{	0,		0,		0,		0,		0 },	//6
		{	0,		0,		0,		0,		0 }, 	//7
		{	0,		0,		0,		0,		0 }, 	//8
		{	0,		1,		0,		0,		0 }, 	//9
		{	0,		2,		0,		0,		0 },  	//10
		{	0,		2,		1,		0,		0 }, 	//11
		{	0,		2,		2,		0,		0 },  	//12
		{	0,		2,		2,		1,		0 },  	//13
		{	0,		3,		2,		1,		0 },  	//14
		{	0,		3,		2,		1,		1 },  	//15
		{	0,		3,		3,		1,		1 }, 	//16
		{	0,		3,		3,		2,		1 }, 	//17
		{	0,		3,		3,		3,		1 }, 	//18
		{	0,		3,		3,		3,		2 }, 	//19
		{	0,		3,		3,		3,		3 }, 	//20
	
	};


	switch(_nSpellClass)
	{
		case DND_CHARACTER_CLASS_CLERIC:
		{
			_nCastLevel = min(_nCastLevel, 29);

			if(_nSpellLevel == 0 && g_bUseUnearthedArcana)
			{
				#if USE_CANTRIPS
				nRetSpells = _ClericMatrix[_nCastLevel][1];

				int nTotalSpellsInLevel = CountSpellsInLevel(pCharacter, DND_CHARACTER_CLASS_CLERIC, 1);
				nRetSpells -= nTotalSpellsInLevel;

				nRetSpells *= 2;

				#else
				nRetSpells = _ClericMatrix[_nCastLevel][_nSpellLevel];
				#endif
			}
			else if(_nSpellLevel == 1 && g_bUseUnearthedArcana)
			{
				#if USE_CANTRIPS
				nRetSpells = _ClericMatrix[_nCastLevel][1];

				int nTotalSpellsInLevel = CountSpellsInLevel(pCharacter, DND_CHARACTER_CLASS_CLERIC, 0);

				if(nTotalSpellsInLevel)
				{
					if(nTotalSpellsInLevel % 2 == 0)
						nTotalSpellsInLevel = nTotalSpellsInLevel / 2;
					else
						nTotalSpellsInLevel = nTotalSpellsInLevel / 2 + 1;
				}
				
				nRetSpells -= nTotalSpellsInLevel;

				#else
				nRetSpells = _ClericMatrix[_nCastLevel][_nSpellLevel];
				#endif
			}
			else if(_nSpellLevel > 7)
			{
				nRetSpells = 0;
			}
			else
			{
				nRetSpells = _ClericMatrix[_nCastLevel][_nSpellLevel];
			}

			if(nRetSpells)
			{
				int nWisdom = pCharacter->m_nDisplayStats[ATTRIB_WIS];

				if(_nSpellLevel == 1 && nWisdom >= 13)
					++nRetSpells;
				if(_nSpellLevel == 1 && nWisdom >= 14)
					++nRetSpells;
				if(_nSpellLevel == 2 && nWisdom >= 15)
					++nRetSpells;
				if(_nSpellLevel == 2 && nWisdom >= 16)
					++nRetSpells;
				if(_nSpellLevel == 3 && nWisdom >= 17)
					++nRetSpells;
				if(_nSpellLevel == 4 && nWisdom >= 18)
					++nRetSpells;
			}  

			break;
		}
		case DND_CHARACTER_CLASS_DRUID:
		{
			_nCastLevel = min(_nCastLevel, 14);

			if(_nSpellLevel == 0 && g_bUseUnearthedArcana)
			{
				#if USE_CANTRIPS
				nRetSpells = _DruidMatrix[_nCastLevel][1];

				int nTotalSpellsInLevel = CountSpellsInLevel(pCharacter, DND_CHARACTER_CLASS_DRUID, 1);
				nRetSpells -= nTotalSpellsInLevel;

				nRetSpells *= 2;

				#else
				nRetSpells = _DruidMatrix[_nCastLevel][_nSpellLevel];
				#endif
			}
			else if(_nSpellLevel == 1 && g_bUseUnearthedArcana)
			{
				#if USE_CANTRIPS
				nRetSpells = _DruidMatrix[_nCastLevel][1];

				int nTotalSpellsInLevel = CountSpellsInLevel(pCharacter, DND_CHARACTER_CLASS_DRUID, 0);

				if(nTotalSpellsInLevel)
				{
					if(nTotalSpellsInLevel % 2 == 0)
						nTotalSpellsInLevel = nTotalSpellsInLevel / 2;
					else
						nTotalSpellsInLevel = nTotalSpellsInLevel / 2 + 1;
				}
				
				nRetSpells -= nTotalSpellsInLevel;

				#else
				nRetSpells = _DruidMatrix[_nCastLevel][_nSpellLevel];
				#endif
			}
			else if(_nSpellLevel > 7)
			{
				nRetSpells = 0;
			}
			else
			{
				nRetSpells = _DruidMatrix[_nCastLevel][_nSpellLevel];
			}

			if(nRetSpells)
			{
				int nWisdom = pCharacter->m_nDisplayStats[ATTRIB_WIS];

				if(_nSpellLevel == 1 && nWisdom >= 13)
					++nRetSpells;
				if(_nSpellLevel == 1 && nWisdom >= 14)
					++nRetSpells;
				if(_nSpellLevel == 2 && nWisdom >= 15)
					++nRetSpells;
				if(_nSpellLevel == 2 && nWisdom >= 16)
					++nRetSpells;
				if(_nSpellLevel == 3 && nWisdom >= 17)
					++nRetSpells;
				if(_nSpellLevel == 4 && nWisdom >= 18)
					++nRetSpells;
			}  

			break;
		}
		case DND_CHARACTER_CLASS_MAGE:
		{
			_nCastLevel = min(_nCastLevel, 29);

			if(_nSpellLevel == 0 && g_bUseUnearthedArcana)
			{
				#if USE_CANTRIPS
				nRetSpells = _MagicUserMatrix[_nCastLevel][1];

				int nTotalSpellsInLevel = CountSpellsInLevel(pCharacter, DND_CHARACTER_CLASS_MAGE, 1);
				nRetSpells -= nTotalSpellsInLevel;

				nRetSpells *= 4;

				#else
				nRetSpells = _MagicUserMatrix[_nCastLevel][_nSpellLevel];
				#endif
			}
			else if(_nSpellLevel == 1 && g_bUseUnearthedArcana)
			{
				#if USE_CANTRIPS
				nRetSpells = _MagicUserMatrix[_nCastLevel][1];

				int nTotalSpellsInLevel = CountSpellsInLevel(pCharacter, DND_CHARACTER_CLASS_MAGE, 0);

				if(nTotalSpellsInLevel)
				{
					if(nTotalSpellsInLevel % 4 == 0)
						nTotalSpellsInLevel = nTotalSpellsInLevel / 4;
					else
						nTotalSpellsInLevel = nTotalSpellsInLevel / 4 + 1;
				}
				
				nRetSpells -= nTotalSpellsInLevel;

				#else
				nRetSpells = _MagicUserMatrix[_nCastLevel][_nSpellLevel];
				#endif
			}
			else if(_nSpellLevel > 9)
			{
				nRetSpells = 0;
			}
			else
			{
				nRetSpells = _MagicUserMatrix[_nCastLevel][_nSpellLevel];
			}

			break;
		}
		case DND_CHARACTER_CLASS_ILLUSIONIST:
		{
			_nCastLevel = min(_nCastLevel, 26);

			if(_nSpellLevel == 0 && g_bUseUnearthedArcana)
			{
				#if USE_CANTRIPS
				nRetSpells = _IllusionistMatrix[_nCastLevel][1];

				int nTotalSpellsInLevel = CountSpellsInLevel(pCharacter, DND_CHARACTER_CLASS_ILLUSIONIST, 1);
				nRetSpells -= nTotalSpellsInLevel;

				nRetSpells *= 4;

				#else
				nRetSpells = _IllusionistMatrix[_nCastLevel][_nSpellLevel];
				#endif
			}
			else if(_nSpellLevel == 1 && g_bUseUnearthedArcana)
			{
				#if USE_CANTRIPS
				nRetSpells = _IllusionistMatrix[_nCastLevel][1];

				int nTotalSpellsInLevel = CountSpellsInLevel(pCharacter, DND_CHARACTER_CLASS_ILLUSIONIST, 0);

				if(nTotalSpellsInLevel)
				{
					if(nTotalSpellsInLevel % 4 == 0)
						nTotalSpellsInLevel = nTotalSpellsInLevel / 4;
					else
						nTotalSpellsInLevel = nTotalSpellsInLevel / 4 + 1;
				}
				
				nRetSpells -= nTotalSpellsInLevel;

				#else
				nRetSpells = _IllusionistMatrix[_nCastLevel][_nSpellLevel];
				#endif
			}
			else if(_nSpellLevel > 7)
			{
				nRetSpells = 0;
			}
			else
			{
				nRetSpells = _IllusionistMatrix[_nCastLevel][_nSpellLevel];
			}

			break;
		}


		case DND_CHARACTER_SPELL_CLASS_RANGER_DRUID:
		{
			_nCastLevel = min(_nCastLevel, 17);

			if(_nSpellLevel > 3)
				nRetSpells = 0;
			else
				nRetSpells = _RangerDruidMatrix[_nCastLevel][_nSpellLevel];

			break;
		}
		case DND_CHARACTER_SPELL_CLASS_RANGER_MAGE:
		{
			_nCastLevel = min(_nCastLevel, 17);

			if(_nSpellLevel > 3)
				nRetSpells = 0;
			else
				nRetSpells = _RangerMageMatrix[_nCastLevel][_nSpellLevel];

			break;
		}
		case DND_CHARACTER_SPELL_CLASS_PALADIN_CLERIC:
		{
			_nCastLevel = min(_nCastLevel, 20);

			if(_nSpellLevel > 4)
				nRetSpells = 0;
			else
				nRetSpells = _PaladinClericMatrix[_nCastLevel][_nSpellLevel];

			break;
		}
	}

	if(nRetSpells < 0)
	{
		nRetSpells = 0;
	}

	return nRetSpells;

}

int CalculateExperienceBonus(cDNDCharacter *pCharacter, int nClass, int *pnNumClasses)
{
	int nBonus = 0;
	int nClasses = 0;

	int nStartClass = 0;
	int nEndClass = 3;

	if(nClass >= 0)
	{
		nStartClass = nClass;
		nEndClass = nStartClass + 1;
	}

	for(int i = nStartClass; i < nEndClass; ++i)
	{
		switch(pCharacter->m_Class[i])
		{
			case DND_CHARACTER_CLASS_NONE:
			{
				++nClasses;
				break;
			}
			case DND_CHARACTER_CLASS_CLERIC:
			{
				++nClasses;
				if(pCharacter->m_nDisplayStats[ATTRIB_WIS] > 15)
					nBonus += 10000;

				break;
			}
			case DND_CHARACTER_CLASS_DRUID:
			{
				++nClasses;
				if(pCharacter->m_nDisplayStats[ATTRIB_WIS] > 15 && pCharacter->m_nDisplayStats[ATTRIB_CHA] > 15)
					nBonus += 10000;

				break;
			}
			case DND_CHARACTER_CLASS_FIGHTER:
			{
				++nClasses;
				if(pCharacter->m_nDisplayStats[ATTRIB_STR] > 15)
					nBonus += 10000;

				break;
			}
			case DND_CHARACTER_CLASS_PALADIN:
			{
				++nClasses;
				if(pCharacter->m_nDisplayStats[ATTRIB_STR] > 15 && pCharacter->m_nDisplayStats[ATTRIB_WIS] > 15)
					nBonus += 10000;

				break;
			}
			case DND_CHARACTER_CLASS_RANGER:
			{
				++nClasses;
				if(pCharacter->m_nDisplayStats[ATTRIB_STR] > 15 && pCharacter->m_nDisplayStats[ATTRIB_INT] > 15 && pCharacter->m_nDisplayStats[ATTRIB_WIS] > 15)
					nBonus += 10000;

				break;
			}
			case DND_CHARACTER_CLASS_MAGE:
			{
				++nClasses;
				if(pCharacter->m_nDisplayStats[ATTRIB_INT] > 15)
					nBonus += 10000;

				break;
			}
			case DND_CHARACTER_CLASS_ILLUSIONIST:
			{
				++nClasses;

				break;
			}
			case DND_CHARACTER_CLASS_THIEF:
			{
				++nClasses;
				if(pCharacter->m_nDisplayStats[ATTRIB_DEX] > 15)
					nBonus += 10000;

				break;
			}
			case DND_CHARACTER_CLASS_ASSASSIN:
			{
				++nClasses;
				
				break;
			}
			case DND_CHARACTER_CLASS_MONK:
			{
				++nClasses;
				
				break;
			}
			case DND_CHARACTER_CLASS_CAVALIER:
			case DND_CHARACTER_CLASS_BARBARIAN:
			case DND_CHARACTER_CLASS_THIEF_ACROBAT:
			{
				++nClasses;

				break;
			}
			#if CUSTOM_CLASSES
			case DND_CHARACTER_CLASS_CUSTOM_1:
			case DND_CHARACTER_CLASS_CUSTOM_2:
			case DND_CHARACTER_CLASS_CUSTOM_3:
			case DND_CHARACTER_CLASS_CUSTOM_4:
			case DND_CHARACTER_CLASS_CUSTOM_5:
			case DND_CHARACTER_CLASS_CUSTOM_6:
			case DND_CHARACTER_CLASS_CUSTOM_7:
			case DND_CHARACTER_CLASS_CUSTOM_8:
			case DND_CHARACTER_CLASS_CUSTOM_9:
			case DND_CHARACTER_CLASS_CUSTOM_10:
			case DND_CHARACTER_CLASS_CUSTOM_11:
			case DND_CHARACTER_CLASS_CUSTOM_12:
			{
				++nClasses;

				int nCountPrequisites = 0;
				for (int j = 0; j < 6; ++j)
				{
					if (GetCustomClass(pCharacter->m_Class[i])->m_bPrerequisiteAttrib[j] == TRUE)
					{
						++nCountPrequisites;
					}
				}

				int nPrequisiteCount = 0;
				for (int j = 0; j < 6; ++j)
				{
					if (GetCustomClass(pCharacter->m_Class[i])->m_bPrerequisiteAttrib[j] == TRUE)
					{
						if (pCharacter->m_nDisplayStats[j] > 15)
						{
							++nPrequisiteCount;

							if (nPrequisiteCount == nCountPrequisites)
							{
								nBonus += 10000;
								break;
							}
						}
					}
				}

				break;
			}
			#endif
		}
	}

	if(pnNumClasses != NULL)
	{
		*pnNumClasses = nClasses;
	}

	if(nClasses)
	{
		nBonus /= nClasses;
	}

	return nBonus;
}



int CalculateEncumbrance(cDNDCharacter *pCharacter, int *pnMaxWeight)
{

	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	int nEncumbrance = 0;
	int nNegativeEncumbrance = 0;

	int nMaxWeight = 350;

	int nSTRDamAdj = 0;
	int nWeightAllow = 0;
	int nOpenDoors = 0;
	int nBendBars = 0;

	int nSTRToHitAdj = CalculateSTRAdjustments(pCharacter, &nSTRDamAdj, &nWeightAllow, &nOpenDoors, &nBendBars);

	nMaxWeight += nWeightAllow;

	for (int i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
	{
		if (pCharacter->m_Inventory[i].m_wTypeId)
		{
			LONG lAmount = max(pCharacter->m_Inventory[i].m_lAmount, 1L);

			cDNDObject *pCheckObject = pApp->m_ObjectsIndexedTypeArray.GetAt(pCharacter->m_Inventory[i].m_wTypeId);

			if (pCheckObject != NULL) //sanity check for weight in case the table was changed
			{
				if (strcmp(pCheckObject->m_szType, pCharacter->m_Inventory[i].m_szType) == 0 && pCharacter->m_Inventory[i].m_nWeight != pCheckObject->m_nWeight)
				{
					if (pCheckObject->m_nWeight != pCharacter->m_Inventory[i].m_nWeight)
					{
						pCharacter->m_Inventory[i].m_nWeight = pCheckObject->m_nWeight;
					}
				}
#ifdef _DEBUG
				else
				{
					if (pCharacter->m_Inventory[i].m_nWeight != pCheckObject->m_nWeight)
					{
						// NOT an error, usually magic item overriding encumbrance of mundane item i.e. bracers of defense
						TRACE("ENC MISMATCH ! %s != %s\n", pCheckObject->m_szType, pCharacter->m_Inventory[i].m_szType);
					}
				}
#endif
			}

			int nWeight = pCharacter->m_Inventory[i].m_nWeight;

			if (pCharacter->m_Inventory[i].m_bIgnoreEncumbrance) // DM FIAT
			{
				nWeight = 0;
			}

			//check for exceptions
			switch(pCharacter->m_Inventory[i].m_ObjectType)
			{
				case DND_OBJECT_TYPE_ARMOR:
				{
					if(pCharacter->m_ArmorWorn.m_dwObjectID == pCharacter->m_Inventory[i].m_dwObjectID && (pCharacter->m_ArmorWorn.m_nMagicAdj || pCharacter->m_Inventory[i].m_bEnchanted)  ) // magical armor is weightless
					{
						if(lAmount > 1)
							lAmount -= 1;
						else
							nWeight = 0;
					}
					break;
				}
				case DND_OBJECT_TYPE_HELMET:
				{
					if(pCharacter->m_HelmWorn.m_dwObjectID == pCharacter->m_Inventory[i].m_dwObjectID && pCharacter->m_ArmorWorn.m_wTypeId) // helmets are weightless if worn with armor if not great helms
					{
						if(pCharacter->m_HelmWorn.m_nFlags % 10 == 0)
						{
							if(lAmount > 1)
								lAmount -= 1;
							else
								nWeight = 0;
						}
					}
					break;
				}
			}

			if(pCharacter->m_Inventory[i].IsContainer())
			{
				if(pCharacter->m_Inventory[i].IsMagicBag())
				{
					nNegativeEncumbrance += pCharacter->m_Inventory[i].m_nContainerPlatinum;
					nNegativeEncumbrance += pCharacter->m_Inventory[i].m_nContainerGold;
					nNegativeEncumbrance += pCharacter->m_Inventory[i].m_nContainerElectrum;
					nNegativeEncumbrance += pCharacter->m_Inventory[i].m_nContainerSilver;
					nNegativeEncumbrance += pCharacter->m_Inventory[i].m_nContainerCopper;
				}
				else if(pCharacter->InventorySlotIsInBagOfHolding(&pCharacter->m_Inventory[i], 0))
				{
					nWeight = 0;
					nNegativeEncumbrance += pCharacter->m_Inventory[i].m_nContainerPlatinum;
					nNegativeEncumbrance += pCharacter->m_Inventory[i].m_nContainerGold;
					nNegativeEncumbrance += pCharacter->m_Inventory[i].m_nContainerElectrum;
					nNegativeEncumbrance += pCharacter->m_Inventory[i].m_nContainerSilver;
					nNegativeEncumbrance += pCharacter->m_Inventory[i].m_nContainerCopper;
				}
			}

			nEncumbrance += nWeight * lAmount;
		}
	}

	//over 1400 (140 #) and you cannot move !  see page 101-102 in Player's Handbook

	int nCoinFudge = 1;
	if(pApp->m_Settings.m_nGPEncumbranceFactor > 0)
		nCoinFudge = pApp->m_Settings.m_nGPEncumbranceFactor;


	nEncumbrance += pCharacter->m_lCopperCarried / nCoinFudge;
	nEncumbrance += pCharacter->m_lSilverCarried / nCoinFudge;
	nEncumbrance += pCharacter->m_lElectrumCarried / nCoinFudge;
	nEncumbrance += pCharacter->m_lGoldCarried / nCoinFudge;
	nEncumbrance += pCharacter->m_lPlatinumCarried / nCoinFudge;

	if(nNegativeEncumbrance)
	{
		nNegativeEncumbrance /= nCoinFudge;

		nEncumbrance -= nNegativeEncumbrance;

		nEncumbrance = max(nEncumbrance, 0);
	}

	*pnMaxWeight = nMaxWeight;

	return nEncumbrance;
}

int IsMonkClass(cDNDCharacter *pCharacter)
{
	for(int i = 0; i < 3; ++i)
	{
		if(pCharacter->m_Class[i] == DND_CHARACTER_CLASS_MONK)
		{
			return i;
		}
	}

	return -1;
}

char * GetMonkWeaponDamageAdj(cDNDCharacter *pCharacter, int nWeaponSlot)
{
	static char szRetVal[10];
	memset(szRetVal, 0, 10*sizeof(char));

	if(pCharacter->m_SelectedWeapons[nWeaponSlot].m_wTypeId == 0)
	{
		return szRetVal;
	}

	if(IsMissileWeapon(&pCharacter->m_SelectedWeapons[nWeaponSlot]))
	{
		return szRetVal;
	}

	int nMonkClass = IsMonkClass(pCharacter);

	if(nMonkClass >= 0)
	{
		float fDamage = (float)pCharacter->m_nLevel[nMonkClass] / 2.0f;
		sprintf(szRetVal, "+%0.1f", fDamage);
	}

	return szRetVal;
}

int CalculateBaseArmorClass(cDNDCharacter *pCharacter)
{
	int nMonkClass = IsMonkClass(pCharacter);

	if(nMonkClass == -1)
	{
		return 10;
	}

	int nMonkLevel = pCharacter->m_nLevel[nMonkClass];

	switch(nMonkLevel)
	{
		case 1:		return	10;
		case 2:		return	9;
		case 3:		return	8;
		case 4:		return	7;
		case 5:		return	7;
		case 6:		return	6;
		case 7:		return	5;
		case 8:		return	4;
		case 9:		return	3;
		case 10:	return	3;
		case 11:	return	2;
		case 12:	return	1;
		case 13:	return	0;
		case 14:	return	-1;
		case 15:	return	-1;
		case 16:	return	-2;
		default:	return	-3;
	}

	return 10;
}

int CalculateBaseMovement(cDNDCharacter *pCharacter)
{
	int nMove = 12;

	for(int i = 0; i < 3; ++i)
	{
		if(pCharacter->m_Class[i] == DND_CHARACTER_CLASS_MONK)
		{
			int nLevel = pCharacter->m_nLevel[i];

			if(nLevel < 17)
			{
				nMove = 14 + nLevel;
			}
			else
			{
				nMove = 32;
			}

			break;
		}
	}

	return nMove;

}

char *CalculateAttacksPerRound(cDNDCharacter *pCharacter, BOOL bBase)
{
	static char szRetVal[20];

	int nNum = 1;
	int nDem = 1;

	int nBowNum = 2;	
	int nBowDem = 1;	

	int nLCrossBowNum = 1;
	int nLCrossBowDem = 2;

	int nHCrossBowNum = 1;
	int nHCrossBowDem = 2;

	int nSpecializedToHitBonus = 0;
	int nSpecializedDamageBonus = 0;
	int nSpecializationLevel = pCharacter->GetWeaponSpecializationLevel();

	if(bBase == FALSE && nSpecializationLevel && pCharacter->m_SelectedWeapons[0].m_dwObjectID && pCharacter->IsProficientWithWeapon(&pCharacter->m_SelectedWeapons[0], &nSpecializedToHitBonus, &nSpecializedDamageBonus) && nSpecializedToHitBonus != 0)
	{
		switch(nSpecializationLevel)
		{
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			{
				nNum = 3;	nBowNum = 2;	nLCrossBowNum = 1;	nHCrossBowNum = 1;
				nDem = 2;	nBowDem = 1;	nLCrossBowDem = 1;	nHCrossBowDem = 2;
				break;
			}
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
			{
				nNum = 2;	nBowNum = 3;	nLCrossBowNum = 3;	nHCrossBowNum = 1;
				nDem = 1;	nBowDem = 1;	nLCrossBowDem = 2;	nHCrossBowDem = 1;
				break;
			}
			default:
			{
				nNum = 5;	nBowNum = 4;	nLCrossBowNum = 2;	nHCrossBowNum = 3;
				nDem = 2;	nBowDem = 1;	nLCrossBowDem = 1;	nHCrossBowDem = 2;
				break;
			}

		}

		if(IsMissileWeapon(&pCharacter->m_WeaponProficiencies[0]))
		{
			char *szSpd = pCharacter->m_SelectedWeapons[0].m_szSpeed;

			if(strcmp(szSpd, "1/2") == 0)
			{
				nNum = nLCrossBowNum;
				nDem = nLCrossBowDem;
			} 
			else if(strcmp(szSpd, "1") == 0)
			{
				nNum = nHCrossBowNum;
				nDem = nHCrossBowDem;
			} 

			nNum = nBowNum;
			nDem = nBowDem;
		}

	}
	else
	{
		int nMonkClass = IsMonkClass(pCharacter);

		if(nMonkClass >= 0)
		{
			int nMonkLevel = pCharacter->m_nLevel[nMonkClass];

			switch(nMonkLevel)
			{
				case 4:		nNum = 5; nDem = 4; break;
				case 5:		nNum = 5; nDem = 4; break;
				case 6:		nNum = 3; nDem = 2; break;
				case 7:		nNum = 3; nDem = 2; break;
				case 8:		nNum = 3; nDem = 2; break;
				case 9:		nNum = 2; nDem = 1; break;
				case 10:	nNum = 2; nDem = 1; break;	
				case 11:	nNum = 5; nDem = 2; break;
				case 12:	nNum = 5; nDem = 2; break;
				case 13:	nNum = 5; nDem = 2; break;
				case 14:	nNum = 3; nDem = 1; break;
				case 15:	nNum = 3; nDem = 1; break;
				case 16:	nNum = 4; nDem = 1; break;
				case 17:	nNum = 4; nDem = 1; break;
			}
		}
		else if(CharacterIsFighter(pCharacter))
		{
			int nFighterClass = GetFighterClass(pCharacter);
			int nLevel = pCharacter->m_nLevel[nFighterClass];

			switch(pCharacter->m_Class[max(nFighterClass,0)])
			{
				case DND_CHARACTER_CLASS_FIGHTER:	// 1-6 1/1 7-12 3/2 13+ 2/1
				#if CUSTOM_CLASSES
				case DND_CHARACTER_CLASS_CUSTOM_1:
				case DND_CHARACTER_CLASS_CUSTOM_2:
				case DND_CHARACTER_CLASS_CUSTOM_3:
				case DND_CHARACTER_CLASS_CUSTOM_4:
				case DND_CHARACTER_CLASS_CUSTOM_5:
				case DND_CHARACTER_CLASS_CUSTOM_6:
				case DND_CHARACTER_CLASS_CUSTOM_7:
				case DND_CHARACTER_CLASS_CUSTOM_8:
				case DND_CHARACTER_CLASS_CUSTOM_9:
				case DND_CHARACTER_CLASS_CUSTOM_10:
				case DND_CHARACTER_CLASS_CUSTOM_11:
				case DND_CHARACTER_CLASS_CUSTOM_12:
				#endif
				{
					if(nLevel >= 13)
					{
						nNum = 2;
						nDem = 1;
					}
					else if(nLevel >= 7)
					{
						nNum = 3;
						nDem = 2;
					}
					else
					{
						nNum = 1;
						nDem = 1;
					}

					break;
				}
				case DND_CHARACTER_CLASS_RANGER: // 1-7 1/1 8-14 3/2 15+ 2/1
				{
					if(nLevel >= 15)
					{
						nNum = 2;
						nDem = 1;
					}
					else if(nLevel >= 8)
					{
						nNum = 3;
						nDem = 2;
					}
					else
					{
						nNum = 1;
						nDem = 1;
					}

					break;
				}
				case DND_CHARACTER_CLASS_CAVALIER:	// 1-6 1/1 7-12 3/2 13+ 2/1
				case DND_CHARACTER_CLASS_PALADIN:
				case DND_CHARACTER_CLASS_BARBARIAN:
				{
					if(nLevel >= 13)
					{
						nNum = 2;
						nDem = 1;
					}
					else if(nLevel >= 7)
					{
						nNum = 3;
						nDem = 2;
					}
					else
					{
						nNum = 1;
						nDem = 1;
					}

					break;
				}
			}
			
		}

		if(bBase == FALSE && IsMissileWeapon(&pCharacter->m_SelectedWeapons[0]))
		{
			char *szSpd = pCharacter->m_SelectedWeapons[0].m_szSpeed;

			if(strcmp(szSpd, "1/2") == 0)
			{
				nDem *= 2;
			} 
			else
			{
				int nVal = atoi(szSpd);
				nNum *= nVal;
			}
		}

	}

	if(nNum % nDem == 0)
	{
		nNum /= nDem;
		nDem /= nDem;
	}

	sprintf(szRetVal, "%d/%d", nNum, nDem);

	return szRetVal;

}

DND_CHARACTER_CLASSES GetCombatClass(cDNDCharacter *pCharacter, int *pnCombatLevel, int *pnClassIndex)
{

	DND_CHARACTER_CLASSES nRetClass = (DND_CHARACTER_CLASSES)999;

	for(int i=0; i< 3;++i)
	{
		DND_CHARACTER_CLASSES nClass = pCharacter->m_Class[i];

		#if CUSTOM_CLASSES
		if (IsCustomClass(nClass))
		{
			nClass = GetCustomClass(pCharacter->m_Class[i])->m_CombatClass;
		}
		#endif

		if (nClass && nClass < nRetClass)
		{
			*pnClassIndex = i;
			*pnCombatLevel = pCharacter->m_nLevel[i];
			nRetClass = pCharacter->m_Class[i];
		}
	}

	return nRetClass;

}

int CalculateWeaponProficiencies(cDNDCharacter *pCharacter, int *pnProfPenalty)
{
	int nRetVal = -5;

	int nCombatLevel = 0;
	int nClassIndex = 0;
	DND_CHARACTER_CLASSES nCombatClass = GetCombatClass(pCharacter, &nCombatLevel, &nClassIndex);

	switch(nCombatClass)
	{
		case DND_CHARACTER_CLASS_NONE:
		{
			nRetVal = 1;
			*pnProfPenalty = -4;
			break;
		}
		case DND_CHARACTER_CLASS_FIGHTER:
		case DND_CHARACTER_CLASS_BARBARIAN:
		{
			nRetVal = 4 + nCombatLevel / 3;
			*pnProfPenalty = -2;
			break;
		}
		case DND_CHARACTER_CLASS_RANGER:
		{
			nRetVal = 3 + nCombatLevel / 3;
			*pnProfPenalty = -2;
			break;
		}
		case DND_CHARACTER_CLASS_PALADIN:
		case DND_CHARACTER_CLASS_CAVALIER:
		{
			nRetVal = 3 + nCombatLevel / 3;
			*pnProfPenalty = -2;
			break;
		}
		

		case DND_CHARACTER_CLASS_CLERIC:
		{
			nRetVal = 2 + nCombatLevel / 4;
			*pnProfPenalty = -3;
			break;
		}
		case DND_CHARACTER_CLASS_DRUID:
		{
			nRetVal = 2 + nCombatLevel / 5;
			*pnProfPenalty = -4;
			break;
		}
		case DND_CHARACTER_CLASS_MONK:
		{
			nRetVal = 1 + nCombatLevel / 2;
			*pnProfPenalty = -3;
			break;
		}

		case DND_CHARACTER_CLASS_THIEF:
		case DND_CHARACTER_CLASS_THIEF_ACROBAT:
		{
			nRetVal = 2 + nCombatLevel / 4;
			*pnProfPenalty = -3;
			break;
		}

		case DND_CHARACTER_CLASS_ASSASSIN:
		{
			nRetVal = 3 + nCombatLevel / 4;
			*pnProfPenalty = -2;
			break;
		}

		case DND_CHARACTER_CLASS_MAGE:
		case DND_CHARACTER_CLASS_ILLUSIONIST:
		{
			nRetVal = 1 + nCombatLevel / 6;
			*pnProfPenalty = -5;
			break;
		}	
		#if CUSTOM_CLASSES
		case DND_CHARACTER_CLASS_CUSTOM_1:
		case DND_CHARACTER_CLASS_CUSTOM_2:
		case DND_CHARACTER_CLASS_CUSTOM_3:
		case DND_CHARACTER_CLASS_CUSTOM_4:
		case DND_CHARACTER_CLASS_CUSTOM_5:
		case DND_CHARACTER_CLASS_CUSTOM_6:
		case DND_CHARACTER_CLASS_CUSTOM_7:
		case DND_CHARACTER_CLASS_CUSTOM_8:
		case DND_CHARACTER_CLASS_CUSTOM_9:
		case DND_CHARACTER_CLASS_CUSTOM_10:
		case DND_CHARACTER_CLASS_CUSTOM_11:
		case DND_CHARACTER_CLASS_CUSTOM_12:
		{
			nRetVal = GetCustomClass(pCharacter->m_Class[nClassIndex])->m_nInitialWeaponProf + nCombatLevel / max(1, GetCustomClass(pCharacter->m_Class[nClassIndex])->m_nAddWeaponProfLevels);
			*pnProfPenalty = -GetCustomClass(pCharacter->m_Class[nClassIndex])->m_nNonWeaponProfPenalty;
			break;
		}
		#endif
	}

	return nRetVal;
}

void CalculateWeaponlessCombatMatrix(cDNDCharacter *pCharacter)
{

	int nBaseAC = 10;

	if(pCharacter->m_ArmorWorn.m_wTypeId)
	{
		nBaseAC = pCharacter->m_ArmorWorn.m_nArmorClass;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// PUMMELING
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//pummeling attack adjustments
	pCharacter->m_nWeaponlessCombatAdjustments[0][0] = 0;  

	pCharacter->m_nWeaponlessCombatAdjustments[0][0] += pCharacter->m_nDisplayStats[ATTRIB_DEX];
	pCharacter->m_nWeaponlessCombatAdjustments[0][0] += max(0, pCharacter->m_nDisplayStats[ATTRIB_STR] - 15);	// Attacker’s strength -per point over 15 = +1
	pCharacter->m_nWeaponlessCombatAdjustments[0][0] += nBaseAC;
	
	
	//pummeling damage adjustments
	pCharacter->m_nWeaponlessCombatAdjustments[1][0] = 0;  
	pCharacter->m_nWeaponlessCombatAdjustments[1][0] += max(0, pCharacter->m_nDisplayStats[ATTRIB_STR] - 12);	// Attacker’s strength -per point over 12 = +1

	if(pCharacter->m_nDisplayStats[ATTRIB_STR] > 18)
	{
		pCharacter->m_nWeaponlessCombatAdjustments[1][0] += 10; //assume str 19 += 100% exceptional strength
	}
	else
	{
		pCharacter->m_nWeaponlessCombatAdjustments[1][0] += (pCharacter->m_nDisplayStats[ATTRIB_EXSTR] / 10) *2;	// strength per 10% over 18 = +2
	}

	if(pCharacter->m_SelectedWeapons[0].m_wTypeId && !IsMissileWeapon(&pCharacter->m_SelectedWeapons[0])) // Using metal pommel
	{
		pCharacter->m_nWeaponlessCombatAdjustments[1][0] += 10;
	}
	else if(nBaseAC < 8) // Using wooden butt or mailed fist
	{
		pCharacter->m_nWeaponlessCombatAdjustments[1][0] += 5;
	}

	//pummeling defense adjustments
	pCharacter->m_nWeaponlessCombatAdjustments[2][0] = 0;  

	pCharacter->m_nWeaponlessCombatAdjustments[2][0] -= max(0, pCharacter->m_nDisplayStats[ATTRIB_DEX] - 14) * 2;	// Active defender - per point of dexterity over 14

	if(pCharacter->m_ShieldWorn.m_wTypeId)
	{
		pCharacter->m_nWeaponlessCombatAdjustments[2][0] -= 10;
	}

	switch(nBaseAC)
	{
		case 8:	pCharacter->m_nWeaponlessCombatAdjustments[2][0] -= 10; break; // leather or padded
		case 7:
		case 6:
		case 5: pCharacter->m_nWeaponlessCombatAdjustments[2][0] -= 20; break; // Chain, ring, scale, or studded mail
		case 4:
		case 3:
		case 2: pCharacter->m_nWeaponlessCombatAdjustments[2][0] -= 40; break; // Banded, plate, or splint mail
	}

	if(pCharacter->m_RingsWorn[0].m_nMagicAdj || pCharacter->m_RingsWorn[1].m_nMagicAdj) // Magical cloak or ring
	{
		pCharacter->m_nWeaponlessCombatAdjustments[2][0] -= 30; 
	}

	if(pCharacter->m_HelmWorn.m_wTypeId)
	{
		int nFlag = (pCharacter->m_HelmWorn.m_nFlags / 10)%10;

		if(nFlag == 2)
		{
			pCharacter->m_nWeaponlessCombatAdjustments[2][0] -= 20; 
		}
		else if(nFlag == 1)
		{
			pCharacter->m_nWeaponlessCombatAdjustments[2][0] -= 10; 
		}
		else
		{
			pCharacter->m_nWeaponlessCombatAdjustments[2][0] -= 5; 
		}
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// GRAPPELING
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//grappeling attack adjustments
	pCharacter->m_nWeaponlessCombatAdjustments[0][1] = 0;  

	pCharacter->m_nWeaponlessCombatAdjustments[0][1] += pCharacter->m_nDisplayStats[ATTRIB_DEX];
	pCharacter->m_nWeaponlessCombatAdjustments[0][1] += pCharacter->m_nDisplayStats[ATTRIB_STR];

	if(pCharacter->m_nDisplayStats[ATTRIB_STR] > 18)
	{
		pCharacter->m_nWeaponlessCombatAdjustments[0][1] += 10; //assume str 19 += 100% exceptional strength
	}
	else
	{
		pCharacter->m_nWeaponlessCombatAdjustments[0][1] += (pCharacter->m_nDisplayStats[ATTRIB_EXSTR] / 10);	// strength per 10% over 18 = +1
	}


	// there are no grappeling damage adjustmets
	pCharacter->m_nWeaponlessCombatAdjustments[1][1] = 0; 

	//grappeling defense adjustments
	pCharacter->m_nWeaponlessCombatAdjustments[2][1] = 0; 
	pCharacter->m_nWeaponlessCombatAdjustments[2][1] -= max(0, pCharacter->m_nDisplayStats[ATTRIB_DEX] - 14);
	pCharacter->m_nWeaponlessCombatAdjustments[2][1] -= max(0, pCharacter->m_nDisplayStats[ATTRIB_STR] - 12);

	if(pCharacter->m_nDisplayStats[ATTRIB_STR] > 18)
	{
		pCharacter->m_nWeaponlessCombatAdjustments[2][1] -= 10; //assume str 19 += 100% exceptional strength
	}
	else
	{
		pCharacter->m_nWeaponlessCombatAdjustments[2][1] -= (pCharacter->m_nDisplayStats[ATTRIB_EXSTR] / 10);	// strength per 10% over 18 = +1
	}

	if(nBaseAC < 5)
	{
		pCharacter->m_nWeaponlessCombatAdjustments[2][1] -= 10;
	}

	if(pCharacter->m_HelmWorn.m_wTypeId)
	{
		pCharacter->m_nWeaponlessCombatAdjustments[2][1] -= 10;
	}

	if(pCharacter->m_ShieldWorn.m_wTypeId)
	{
		pCharacter->m_nWeaponlessCombatAdjustments[2][1] -= 10;
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// OVERBEARING
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//overbearing attack adjustments
	pCharacter->m_nWeaponlessCombatAdjustments[0][2] = 0; 


	pCharacter->m_nWeaponlessCombatAdjustments[0][2] += pCharacter->m_nDisplayStats[ATTRIB_STR];

	if(pCharacter->m_nDisplayStats[ATTRIB_STR] > 18)
	{
		pCharacter->m_nWeaponlessCombatAdjustments[0][2] += 20; //assume str 19 += 100% exceptional strength
	}
	else
	{
		pCharacter->m_nWeaponlessCombatAdjustments[0][2] += (pCharacter->m_nDisplayStats[ATTRIB_EXSTR] / 10)*2;	// strength per 10% over 18 = +2
	}

	//there are no overbearing damage modifiers
	pCharacter->m_nWeaponlessCombatAdjustments[1][2] = 0;
	
	pCharacter->m_nWeaponlessCombatAdjustments[2][2] = 0;

	//overbearing defense adjustments
	pCharacter->m_nWeaponlessCombatAdjustments[2][2] -= max(0, pCharacter->m_nDisplayStats[ATTRIB_STR] - 14);

	if(pCharacter->m_nDisplayStats[ATTRIB_STR] > 18)
	{
		pCharacter->m_nWeaponlessCombatAdjustments[2][2] -= 20; //assume str 19 += 100% exceptional strength
	}
	else
	{
		pCharacter->m_nWeaponlessCombatAdjustments[2][2] -= (pCharacter->m_nDisplayStats[ATTRIB_EXSTR] / 10)*2;	// strength per 10% over 18 = +2
	}

	pCharacter->m_nWeaponlessCombatAdjustments[2][2] -= max(0, pCharacter->m_nDisplayStats[ATTRIB_DEX] - 14) * 2;


}

int GetPriceFromString(char *sPrice)
{
	//all prices are stored internally in copper pieces

	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	CString szPrice = sPrice;

	int nPriceMul = 200; // 200 copper = 1 gold

	int nFoundCoin = szPrice.Replace("c", "");
	if(nFoundCoin != 0)
	{
		nPriceMul = 1; //price is in c.p.
	}
	nFoundCoin = szPrice.Replace("s", "");
	if(nFoundCoin != 0)
	{
		nPriceMul = 10; //price is in s.p.
	}

	//int nInflation = pApp->m_Settings.m_nVendorPriceInflationFactor;
	//nPriceMul = (nPriceMul * nInflation) / 100;

	int nPrice = atoi(szPrice.GetBuffer(0));

	nPrice *= nPriceMul;

	return nPrice;
}

char *GetStringFromPrice(int nPrice)
{
	static CString szRetVal;

	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	float fInflation = ((float)pApp->m_Settings.m_nVendorPriceInflationFactor)/100.0f;
	nPrice = (int)((float)nPrice * fInflation + 0.5f);

	if(nPrice < 1)
		nPrice = 1;

	int nGP = 0;
	int nEP = 0;
	int nSP = 0;
	int nCP = 0;

	szRetVal = _T("");
	CString szTemp;
	do
	{
		if(nPrice >= 200)
		{
			nGP += nPrice / 200;
			nPrice = nPrice - (nGP * 200);

			szTemp.Format("%d g.p. ", nGP);
			szRetVal += szTemp;
		}
		/*
		if(nPrice >= 100)
		{
			nEP += nPrice / 100;
			nPrice = nPrice - (nEP * 100);

			szTemp.Format("%d e.p. ", nEP);
			szRetVal += szTemp;
		}
		*/
		if(nPrice >= 10)
		{
			nSP += nPrice / 10;
			nPrice = nPrice - (nSP * 10);

			szTemp.Format("%d s.p. ", nSP);
			szRetVal += szTemp;
		}
		if(nPrice > 0)
		{
			nCP += nPrice;
			nPrice = 0;

			szTemp.Format("%d c.p. ", nCP);
			szRetVal += szTemp;
		}

	} while (nPrice > 0);

	//szRetVal.Format("%d %s", nPrice, szCoin.GetBuffer(0));

	return szRetVal.GetBuffer(0);
}

CString GetGPPriceFromString(CString szPrice)
{
	CString szRetVal = "1c";

	int nStartPara = szPrice.Find("(");
	int nEndPara = szPrice.Find(")");

	if (nStartPara >= 0 && nEndPara >= 0 && szPrice.Find("g.p.") >= 0)
	{
		szPrice = szPrice.Mid(nStartPara, nEndPara);
		szPrice.Replace("(", "");
		szPrice.Replace(")", "");
		szPrice.Replace("g.p.", "");
		szPrice.Replace(" ", "");
		szPrice.Replace(",", "");

		int nPrice = atoi(szPrice.GetBuffer(0));
		szPrice.Format("%dg", nPrice);
		szRetVal = szPrice;
	}

	return szRetVal;
}

int GetAmountFromString(CString *pszString)
{
	int nReturnVal = 1;
	CString szReturnString = *pszString;
	
	int nFirstSpace = pszString->FindOneOf(" ");
	
	if (nFirstSpace > -1)
	{
		CString szNum = pszString->Left(nFirstSpace);

		int nVal = atoi(szNum);

		if (nVal)
		{
			nReturnVal = nVal;
			szReturnString = pszString->Mid(nFirstSpace, pszString->GetLength());
		}

		szReturnString.Trim();
	}


	*pszString = szReturnString;
	return nReturnVal;
}

BOOL IsThrowableWeapon(UINT nFlags)
{
	int nWeaponFlag = (nFlags / 10000) % 10;

	//if(nWeaponFlag == 8 || nWeaponFlag == 9)
	if (nWeaponFlag == 9)
		return TRUE;

	return FALSE;

}

BOOL IsMissileWeapon(POBJECTTYPE pObject)
{
	if(IsThrowableWeapon(pObject->m_nFlags))
		return FALSE;

	if((pObject->m_nFlags / 10000) % 10 != 0)
		return TRUE;

	return FALSE;
}

BOOL IsMissileWeapon(PWEAPONTYPE pWeapon)
{
	if(IsThrowableWeapon(pWeapon->m_nFlags))
		return FALSE;

	if((pWeapon->m_nFlags / 10000) % 10 != 0)
		return TRUE;

	return FALSE;
}

int GetObjectFlag(PWEAPONTYPE pWeapon, int nFlag)
{
	int nDiv = (int)pow((double)10, nFlag);

	int nRetVal = (pWeapon->m_nFlags / nDiv) % 10;

	return nRetVal;
}

void SetObjectFlag(PWEAPONTYPE pWeapon, int nFlag, int nValue)
{
	int nDiv = (int)pow((double)10, nFlag);

	int nOldFlag = GetObjectFlag(pWeapon, nFlag);

	pWeapon->m_nFlags -= nDiv * nOldFlag;

	pWeapon->m_nFlags += nDiv * nValue;
}

int GetAmmoTypeFlag(PWEAPONTYPE pWeapon)
{
	//type 9 is custom
	return (pWeapon->m_nFlags / 1000) % 10;
}

void SetAmmoTypeFlag(PWEAPONTYPE pWeapon, int nFlag, int nCustomAmmoType)
{
	int nOldFlag = GetAmmoTypeFlag(pWeapon);

	pWeapon->m_nFlags -= nOldFlag * 1000;

	if (nFlag >= 1000)
	{
		pWeapon->m_nFlags += nFlag;
	}
	else
	{
		pWeapon->m_nFlags += nFlag * 1000;
	}

	pWeapon->m_nCustomAmmoType = nCustomAmmoType;
}

int GetAmmoUsedFlag(PWEAPONTYPE pWeapon)
{
	return (pWeapon->m_nFlags / 10000) % 10;
}
void SetAmmoUsedFlag(PWEAPONTYPE pWeapon, int nFlag, int nCustomAmmoType)
{
	int nOldFlag = GetAmmoUsedFlag(pWeapon);

	pWeapon->m_nFlags -= nOldFlag * 10000;

	if (nFlag >= 10000)
	{
		pWeapon->m_nFlags += nFlag;
	}
	else
	{
		pWeapon->m_nFlags += nFlag * 10000;
	}

	pWeapon->m_nCustomAmmoType = nCustomAmmoType;
}

BOOL IsPoleArm(POBJECTTYPE pObject)
{
	switch(pObject->m_wTypeId)
	{
		case 1123:	// fork_military
		case 1124:	// glaive
		case 1125:	// glaive-guisarme
		case 1126:	// guisarme		
		case 1127:	// guisarme-volge	
		case 1128:	// halberd	
		case 1156:	// voulge
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL IsAmmo(PWEAPONTYPE pWeapon)
{
	if((pWeapon->m_nFlags / 1000) % 10 != 0)
		return TRUE;

	return FALSE;
}

BOOL IsValidAmmoForWeapon(PWEAPONTYPE pWeapon, PWEAPONTYPE pAmmo)
{
	int nWeaponFlag = (pWeapon->m_nFlags / 10000) % 10;
	int nAmmoFlag = (pAmmo->m_nFlags / 1000) % 10;
	
	if (nWeaponFlag == nAmmoFlag)
	{
		if (nAmmoFlag == 8) // custom ammo
		{
			if (pWeapon->m_nCustomAmmoType == pAmmo->m_wTypeId)
			{
				return TRUE;
			}
		}
		else
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL IsValidAmmoForWeapon(PWEAPONTYPE pWeapon, POBJECTTYPE pAmmo)
{
	if (pAmmo->m_ObjectType == DND_OBJECT_TYPE_WEAPON || pAmmo->m_ObjectType == DND_OBJECT_TYPE_AMMO)
	{
		return IsValidAmmoForWeapon(pWeapon, (PWEAPONTYPE)pAmmo);
	}

	return FALSE;
}

/*
BOOL IsValidAmmoForWeapon(int nWeaponFlags, int nAmmoFlags)
{
	int nWeaponFlag = (nWeaponFlags / 10000) % 10;
	int nAmmoFlag = (nAmmoFlags / 1000) % 10;

	if(nWeaponFlag == nAmmoFlag)
		return TRUE;

	return FALSE;
}
*/

//utility functions
void RemoveSpaces(char *szInString)
{
	CString szTemp = szInString;

	szTemp.Replace(" ", "");
	szTemp.Replace("\n", "");
	szTemp.Replace("\r", "");

	szTemp.TrimRight();

	strcpy(szInString, szTemp.GetBuffer(0));

}
void RemoveLineFeeds(char *szInString)
{
	CString szTemp = szInString;

	szTemp.Replace("\n", "");
	szTemp.Replace("\r", "");

	szTemp.TrimRight();

	strcpy(szInString, szTemp.GetBuffer(0));

}

char *GetNumberSuffix(int nNumber)
{
	nNumber = nNumber % 10;

	switch(nNumber)
	{
		case 1: return "st";
		case 2: return "nd";
		case 3: return "rd";
	}

	return "th";
}

CString szPlusOrMinus(int nVal)
{
	static CString szRetVal = _T("");

	if(nVal >= 0)
	{
		szRetVal.Format("+%d", nVal);
	}
	else
	{
		szRetVal.Format("%d", nVal);
	}

	return szRetVal;
}

CString GetStringFromInt(int nVal)
{
	CString szRetVal = _T("");
	szRetVal.Format("%d", nVal);

	return szRetVal;
}

CString GetCharacterSize(int nHeight)
{

	if (nHeight > 7 * 12)
		return "L";

	if (nHeight < 4 * 12)
		return "S";

	return "M";
}

CString GetCharacterBook(DND_CHARACTER_CLASSES nClass)
{
	switch (nClass)
	{
		case DND_CHARACTER_CLASS_NONE:			return "PH pg. 22";
		case DND_CHARACTER_CLASS_FIGHTER:		return "PH pg. 22";
		case DND_CHARACTER_CLASS_RANGER:		return "PH pg. 24";
		case DND_CHARACTER_CLASS_CAVALIER:		return "UA pg. 20";
		case DND_CHARACTER_CLASS_PALADIN:		return "PH pg. 22";
		case DND_CHARACTER_CLASS_BARBARIAN:		return "UA pg. 14";

		case DND_CHARACTER_CLASS_CLERIC:		return "PH pg. 20";
		case DND_CHARACTER_CLASS_DRUID:			return "PH pg. 20";
		case DND_CHARACTER_CLASS_MONK:			return "PH pg. 30";

		case DND_CHARACTER_CLASS_THIEF:			return "PH pg. 26";
		case DND_CHARACTER_CLASS_THIEF_ACROBAT:	return "UA pg. 23";
		case DND_CHARACTER_CLASS_ASSASSIN:		return "PH pg. 28";

		case DND_CHARACTER_CLASS_MAGE:			return "PH pg. 25";
		case DND_CHARACTER_CLASS_ILLUSIONIST:	return "PH pg. 26";
	}

	return "DMG p. 88";
}

CString GetNumberth(int nNumber)
{
	int nDigit = nNumber % 10;

	CString szEth = "";

	switch (nDigit)
	{
		case 0:	szEth = "th"; break;
		case 1:	szEth = "st"; break;
		case 2:	szEth = "nd"; break;
		case 3:	szEth = "rd"; break;
		case 4:	szEth = "th"; break;
		case 5:	szEth = "th"; break;
		case 6:	szEth = "th"; break;
		case 7:	szEth = "th"; break;
		case 8:	szEth = "th"; break;
		case 9:	szEth = "th"; break;
	}

	CString szRetVal;
	szRetVal.Format("%d", nNumber);
	szRetVal += szEth;

	return szRetVal;
}

BOOL GetSpellClasses(cDNDCharacter *pCharacter)
{
	BOOL bRetVal = FALSE;

	pCharacter->m_SpellClasses[0] = DND_CHARACTER_CLASS_UNDEF;
	pCharacter->m_SpellClasses[1] = DND_CHARACTER_CLASS_UNDEF;
	pCharacter->m_SpellClasses[2] = DND_CHARACTER_CLASS_UNDEF;
	pCharacter->m_SpellClasses[3] = DND_CHARACTER_CLASS_UNDEF;

	memset(pCharacter->m_nCastingLevels, 0, 4*sizeof(int));

	for(int i = 0; i < 3; ++ i)
	{
		switch(pCharacter->m_Class[i])
		{
			case DND_CHARACTER_CLASS_CLERIC:
			{
				pCharacter->m_SpellClasses[i] = DND_CHARACTER_CLASS_CLERIC;
				pCharacter->m_nCastingLevels[i] = pCharacter->m_nLevel[i];
				bRetVal = TRUE;
				break;
			}
			case DND_CHARACTER_CLASS_DRUID:
			{
				pCharacter->m_SpellClasses[i] = DND_CHARACTER_CLASS_DRUID;
				pCharacter->m_nCastingLevels[i] = pCharacter->m_nLevel[i];
				bRetVal = TRUE;
				break;
			}

			case DND_CHARACTER_CLASS_MAGE:
			{
				pCharacter->m_SpellClasses[i] = DND_CHARACTER_CLASS_MAGE;
				pCharacter->m_nCastingLevels[i] = pCharacter->m_nLevel[i];
				bRetVal = TRUE;
				break;
			}
			case DND_CHARACTER_CLASS_ILLUSIONIST:
			{
				pCharacter->m_SpellClasses[i] = DND_CHARACTER_CLASS_ILLUSIONIST;
				pCharacter->m_nCastingLevels[i] = pCharacter->m_nLevel[i];
				bRetVal = TRUE;
				break;
			}

			case DND_CHARACTER_CLASS_RANGER:
			{
				if(pCharacter->m_nLevel[i] > 7)
				{
					pCharacter->m_SpellClasses[i] = DND_CHARACTER_SPELL_CLASS_RANGER_DRUID;
					pCharacter->m_nCastingLevels[i] = pCharacter->m_nLevel[i];
					bRetVal = TRUE;
				}
				if(pCharacter->m_nLevel[i] > 8)
				{
					pCharacter->m_SpellClasses[3] = DND_CHARACTER_SPELL_CLASS_RANGER_MAGE;
					pCharacter->m_nCastingLevels[3] = pCharacter->m_nLevel[i];
					bRetVal = TRUE;
				}

				break;
			}
			case DND_CHARACTER_CLASS_PALADIN:
			{
				if(pCharacter->m_nLevel[i] > 8)
				{
					pCharacter->m_SpellClasses[i] = DND_CHARACTER_SPELL_CLASS_PALADIN_CLERIC;
					pCharacter->m_nCastingLevels[i] = pCharacter->m_nLevel[i];
					bRetVal = TRUE;
				}

				break;
			}
			#if CUSTOM_CLASSES
			case DND_CHARACTER_CLASS_CUSTOM_1:
			case DND_CHARACTER_CLASS_CUSTOM_2:
			case DND_CHARACTER_CLASS_CUSTOM_3:
			case DND_CHARACTER_CLASS_CUSTOM_4:
			case DND_CHARACTER_CLASS_CUSTOM_5:
			case DND_CHARACTER_CLASS_CUSTOM_6:
			case DND_CHARACTER_CLASS_CUSTOM_7:
			case DND_CHARACTER_CLASS_CUSTOM_8:
			case DND_CHARACTER_CLASS_CUSTOM_9:
			case DND_CHARACTER_CLASS_CUSTOM_10:
			case DND_CHARACTER_CLASS_CUSTOM_11:
			case DND_CHARACTER_CLASS_CUSTOM_12:
			{
				DND_CHARACTER_CLASSES nSpellClass = GetCustomClass(pCharacter->m_Class[i])->m_MagicUseClass; 

				if (nSpellClass != DND_CHARACTER_CLASS_UNDEF)
				{
					pCharacter->m_SpellClasses[i] = GetCustomClass(pCharacter->m_Class[i])->m_MagicUseClass;

					int nLevelOffset = GetCustomClass(pCharacter->m_Class[i])->m_nMagicUseLevel - 1;

					pCharacter->m_nCastingLevels[i] = max(pCharacter->m_nLevel[i] - nLevelOffset, 0);

					bRetVal = TRUE;
				}

				break;
			}
			#endif
		}
	}


	return bRetVal;
}

int CountSpellsInLevel(int *pnArray)
{
	//m_pCharacter->m_nSpellsMemorized[m_nSelectedSpellClass][pSpell->m_nSpellLevel]
	int nRetVal = 0;

	for(int i = 0; i< MAX_SPELLS_PER_LEVEL; ++i)
	{
		if(pnArray[i] < 0 || pnArray[i] > 10)
		{
			//obvious shit data
			pnArray[i] = 0;

		}

		if(pnArray[i])
		{
			nRetVal += pnArray[i];
		}
	}

	return nRetVal;
}

//CANTRIPS
int CountSpellsInLevel(cDNDCharacter *pCharacter, DND_CHARACTER_CLASSES _nSpellClass, int _nSpellLevel)
{
	if(_nSpellLevel < 0 || _nSpellLevel > 9)
		return 0;

	int nSpellIndex = -1;

	for(int i = 0; i < 4; ++i)
	{
		if(pCharacter->m_SpellClasses[i] == _nSpellClass)
		{
			nSpellIndex = i;
			break;
		}
	}

	if(nSpellIndex == -1)
	{
		return 0;
	}

	int nRetVal = 0;

	for(i = 0; i< MAX_SPELLS_PER_LEVEL; ++i)
	{
		if(pCharacter->m_nSpellsMemorized[nSpellIndex][_nSpellLevel][i] < 0 || pCharacter->m_nSpellsMemorized[nSpellIndex][_nSpellLevel][i] > 10)
		{
			//obvious shit data
			pCharacter->m_nSpellsMemorized[nSpellIndex][_nSpellLevel][i] = 0;
		}

		if(pCharacter->m_nSpellsMemorized[nSpellIndex][_nSpellLevel][i])
		{
			nRetVal += pCharacter->m_nSpellsMemorized[nSpellIndex][_nSpellLevel][i];
		}
	}

	return nRetVal;
}

void GenerateHeightWeight(cDNDCharacter *pCharacter)
{
	if(pCharacter->m_nRace == DND_CHARACTER_RACES_UNDEF)
		return;

	int nBaseHeight = 0;
	int nHeightMinusLow = 0;
	int nHeightMinusHigh = 0;
	int nHeightPlusLow = 0;
	int nHeightPlusHigh = 0;

	int nBaseWeight = 0;
	int nWeightMinusLow = 0;
	int nWeightMinusHigh = 0;
	int nWeightPlusLow = 0;
	int nWeightPlusHigh = 0;

	if(pCharacter->m_nSex == 0) // male
	{
		switch(pCharacter->m_nRace)
		{
			case DND_CHARACTER_RACE_ELF:
			case DND_CHARACTER_RACE_DROW:
			{
				nBaseHeight = 60;
				nHeightMinusLow = 1; nHeightMinusHigh = 4;
				nHeightPlusLow = 1; nHeightPlusHigh = 6;

				nBaseWeight = 100;
				nWeightMinusLow = 1; nWeightMinusHigh = 10;
				nWeightPlusLow = 1; nWeightPlusHigh = 20;

				break;
			}

			case DND_CHARACTER_RACE_HALF_ELF:
			{
				nBaseHeight = 66;
				nHeightMinusLow = 1; nHeightMinusHigh = 6;
				nHeightPlusLow = 1; nHeightPlusHigh = 6;

				nBaseWeight = 130;
				nWeightMinusLow = 1; nWeightMinusHigh = 20;
				nWeightPlusLow = 1; nWeightPlusHigh = 20;

				break;
			}

			case DND_CHARACTER_RACE_DWARF:
			{
				nBaseHeight = 48;
				nHeightMinusLow = 1; nHeightMinusHigh = 4;
				nHeightPlusLow = 1; nHeightPlusHigh = 6;

				nBaseWeight = 150;
				nWeightMinusLow = 2; nWeightMinusHigh = 16;
				nWeightPlusLow = 2; nWeightPlusHigh = 24;

				break;
			}

			case DND_CHARACTER_RACE_HALFLING:
			{
				nBaseHeight = 36;
				nHeightMinusLow = 1; nHeightMinusHigh = 3;
				nHeightPlusLow = 1; nHeightPlusHigh = 6;

				nBaseWeight = 60;
				nWeightMinusLow = 2; nWeightMinusHigh = 8;
				nWeightPlusLow = 2; nWeightPlusHigh = 12;

				break;
			}

			case DND_CHARACTER_RACE_GNOME:
			{
				nBaseHeight = 42;
				nHeightMinusLow = 1; nHeightMinusHigh = 3;
				nHeightPlusLow = 1; nHeightPlusHigh = 3;

				nBaseWeight = 80;
				nWeightMinusLow = 2; nWeightMinusHigh = 8;
				nWeightPlusLow = 2; nWeightPlusHigh = 12;

				break;
			}

			case DND_CHARACTER_RACE_HALF_ORC:
			{
				nBaseHeight = 66;
				nHeightMinusLow = 1; nHeightMinusHigh = 4;
				nHeightPlusLow = 1; nHeightPlusHigh = 4;

				nBaseWeight = 150;
				nWeightMinusLow = 2; nWeightMinusHigh = 16;
				nWeightPlusLow = 4; nWeightPlusHigh = 40;

				break;
			}

			case DND_CHARACTER_RACE_HUMAN:
			default:
			{
				nBaseHeight = 72;
				nHeightMinusLow = 1; nHeightMinusHigh = 12;
				nHeightPlusLow = 1; nHeightPlusHigh = 12;

				nBaseWeight = 175;
				nWeightMinusLow = 3; nWeightMinusHigh = 36;
				nWeightPlusLow = 5; nWeightPlusHigh = 60;

				break;
			}
		}
	}
	else // female
	{
		switch(pCharacter->m_nRace)
		{
			case DND_CHARACTER_RACE_ELF:
			case DND_CHARACTER_RACE_DROW:
			{
				nBaseHeight = 54;
				nHeightMinusLow = 1; nHeightMinusHigh = 4;
				nHeightPlusLow = 1; nHeightPlusHigh = 6;

				nBaseWeight = 80;
				nWeightMinusLow = 1; nWeightMinusHigh = 10;
				nWeightPlusLow = 2; nWeightPlusHigh = 12;

				break;
			}

			case DND_CHARACTER_RACE_HALF_ELF:
			{
				nBaseHeight = 62;
				nHeightMinusLow = 1; nHeightMinusHigh = 6;
				nHeightPlusLow = 1; nHeightPlusHigh = 6;

				nBaseWeight = 100;
				nWeightMinusLow = 1; nWeightMinusHigh = 12;
				nWeightPlusLow = 2; nWeightPlusHigh = 16;

				break;
			}

			case DND_CHARACTER_RACE_DWARF:
			{
				nBaseHeight = 46;
				nHeightMinusLow = 1; nHeightMinusHigh = 4;
				nHeightPlusLow = 1; nHeightPlusHigh = 4;

				nBaseWeight = 120;
				nWeightMinusLow = 2; nWeightMinusHigh = 16;
				nWeightPlusLow = 2; nWeightPlusHigh = 20;

				break;
			}

			case DND_CHARACTER_RACE_HALFLING:
			{
				nBaseHeight = 33;
				nHeightMinusLow = 1; nHeightMinusHigh = 3;
				nHeightPlusLow = 1; nHeightPlusHigh = 3;

				nBaseWeight = 50;
				nWeightMinusLow = 2; nWeightMinusHigh = 8;
				nWeightPlusLow = 2; nWeightPlusHigh = 8;

				break;
			}

			case DND_CHARACTER_RACE_GNOME:
			{
				nBaseHeight = 39;
				nHeightMinusLow = 1; nHeightMinusHigh = 3;
				nHeightPlusLow = 1; nHeightPlusHigh = 3;

				nBaseWeight = 75;
				nWeightMinusLow = 1; nWeightMinusHigh = 8;
				nWeightPlusLow = 1; nWeightPlusHigh = 8;

				break;
			}

			case DND_CHARACTER_RACE_HALF_ORC:
			{
				nBaseHeight = 62;
				nHeightMinusLow = 1; nHeightMinusHigh = 3;
				nHeightPlusLow = 1; nHeightPlusHigh = 3;

				nBaseWeight = 120;
				nWeightMinusLow = 3; nWeightMinusHigh = 18;
				nWeightPlusLow = 4; nWeightPlusHigh = 32;

				break;
			}

			case DND_CHARACTER_RACE_HUMAN:
			default:
			{
				nBaseHeight = 66;
				nHeightMinusLow = 1; nHeightMinusHigh = 6;
				nHeightPlusLow = 1; nHeightPlusHigh = 8;

				nBaseWeight = 130;
				nWeightMinusLow = 3; nWeightMinusHigh = 30;
				nWeightPlusLow = 4; nWeightPlusHigh = 48;

				break;
			}
		}
	}

	int nLowHigh = (rand()%6) + 1;

	if(nBaseHeight && nBaseWeight)
	{
		if(nLowHigh <= 3)
		{
			pCharacter->m_nHeight = nBaseHeight - ((rand()%nHeightMinusHigh) + nHeightMinusLow);
			pCharacter->m_nWeight = nBaseWeight - ((rand()%nWeightMinusHigh) + nWeightMinusLow);
		}
		else
		{
			pCharacter->m_nHeight = nBaseHeight + ((rand()%nHeightPlusHigh) + nHeightPlusLow);
			pCharacter->m_nWeight = nBaseWeight + ((rand()%nWeightPlusHigh) + nWeightPlusLow);
		}
	}
}

int GenerateCharacterAge(cDNDCharacter *pCharacter)
{
	// keary bookmark

	int nRetVal = 0;

	if(pCharacter->m_nRace == DND_CHARACTER_RACES_UNDEF)
		return nRetVal;

	if(pCharacter->m_Class[0] == DND_CHARACTER_CLASS_UNDEF)
		return nRetVal;

	int nBaseAge[4] = {0,0,0,0};
	int nDieAddLow[4] = {0,0,0,0};
	int nDieAddHigh[4] = {0,0,0,0};

	for(int i = 0; i < 4; ++i)
	{
		switch(pCharacter->m_nRace)
		{
			case DND_CHARACTER_RACE_DWARF:
			{
				switch(pCharacter->m_Class[i])
				{
					case DND_CHARACTER_CLASS_CLERIC:		
					case DND_CHARACTER_CLASS_DRUID:			
					case DND_CHARACTER_CLASS_MONK:			nBaseAge[i] = 250;	nDieAddLow[i] = 2;	nDieAddHigh[i] = 10;	break;

					case DND_CHARACTER_CLASS_FIGHTER:		
					case DND_CHARACTER_CLASS_RANGER:		
					case DND_CHARACTER_CLASS_PALADIN:		
					case DND_CHARACTER_CLASS_CAVALIER:		
					case DND_CHARACTER_CLASS_BARBARIAN:		nBaseAge[i] = 40;	nDieAddLow[i] = 5;	nDieAddHigh[i] = 4;	break;

					case DND_CHARACTER_CLASS_MAGE:			
					case DND_CHARACTER_CLASS_ILLUSIONIST:	nBaseAge[i] = 250;	nDieAddLow[i] = 2;	nDieAddHigh[i] = 10;	break;

					case DND_CHARACTER_CLASS_THIEF:			
					case DND_CHARACTER_CLASS_THIEF_ACROBAT:	
					case DND_CHARACTER_CLASS_ASSASSIN:		nBaseAge[i] = 75;	nDieAddLow[i] = 3;	nDieAddHigh[i] = 6;	break;
				}

				break;
			}

			case DND_CHARACTER_RACE_ELF:
			case DND_CHARACTER_RACE_DROW:
			{
				switch(pCharacter->m_Class[i])
				{
					case DND_CHARACTER_CLASS_CLERIC:		
					case DND_CHARACTER_CLASS_DRUID:			
					case DND_CHARACTER_CLASS_MONK:			nBaseAge[i] = 500;	nDieAddLow[i] = 1;	nDieAddHigh[i] = 10;	break;

					case DND_CHARACTER_CLASS_FIGHTER:		
					case DND_CHARACTER_CLASS_RANGER:		
					case DND_CHARACTER_CLASS_PALADIN:		
					case DND_CHARACTER_CLASS_CAVALIER:		
					case DND_CHARACTER_CLASS_BARBARIAN:		nBaseAge[i] = 130;	nDieAddLow[i] = 5;	nDieAddHigh[i] = 6;	break;

					case DND_CHARACTER_CLASS_MAGE:			
					case DND_CHARACTER_CLASS_ILLUSIONIST:	nBaseAge[i] = 150;	nDieAddLow[i] = 5;	nDieAddHigh[i] = 6;	break;

					case DND_CHARACTER_CLASS_THIEF:			
					case DND_CHARACTER_CLASS_THIEF_ACROBAT:
					case DND_CHARACTER_CLASS_ASSASSIN:		nBaseAge[i] = 100;	nDieAddLow[i] = 5;	nDieAddHigh[i] = 6;	break;	
				}

				break;
			}

			case DND_CHARACTER_RACE_GNOME:
			{
				switch(pCharacter->m_Class[i])
				{
					case DND_CHARACTER_CLASS_CLERIC:		
					case DND_CHARACTER_CLASS_DRUID:			
					case DND_CHARACTER_CLASS_MONK:			nBaseAge[i] = 300;	nDieAddLow[i] = 3;	nDieAddHigh[i] = 12;	break;

					case DND_CHARACTER_CLASS_FIGHTER:		
					case DND_CHARACTER_CLASS_RANGER:		
					case DND_CHARACTER_CLASS_PALADIN:		
					case DND_CHARACTER_CLASS_CAVALIER:		
					case DND_CHARACTER_CLASS_BARBARIAN:		nBaseAge[i] = 60;	nDieAddLow[i] = 5;	nDieAddHigh[i] = 4;	break;

					case DND_CHARACTER_CLASS_MAGE:			
					case DND_CHARACTER_CLASS_ILLUSIONIST:	nBaseAge[i] = 100;	nDieAddLow[i] = 2;	nDieAddHigh[i] = 12;	break;

					case DND_CHARACTER_CLASS_THIEF:			
					case DND_CHARACTER_CLASS_THIEF_ACROBAT:
					case DND_CHARACTER_CLASS_ASSASSIN:		nBaseAge[i] = 80;	nDieAddLow[i] = 5;	nDieAddHigh[i] = 4;	break;	
				}

				break;
			}

			case DND_CHARACTER_RACE_HALF_ELF:
			{
				switch(pCharacter->m_Class[i])
				{
					case DND_CHARACTER_CLASS_CLERIC:		
					case DND_CHARACTER_CLASS_DRUID:			
					case DND_CHARACTER_CLASS_MONK:			nBaseAge[i] = 40;	nDieAddLow[i] = 2;	nDieAddHigh[i] = 4;	break;

					case DND_CHARACTER_CLASS_FIGHTER:		
					case DND_CHARACTER_CLASS_RANGER:		
					case DND_CHARACTER_CLASS_PALADIN:		
					case DND_CHARACTER_CLASS_CAVALIER:		
					case DND_CHARACTER_CLASS_BARBARIAN:		nBaseAge[i] = 22;	nDieAddLow[i] = 3;	nDieAddHigh[i] = 4;	break;

					case DND_CHARACTER_CLASS_MAGE:			
					case DND_CHARACTER_CLASS_ILLUSIONIST:	nBaseAge[i] = 30;	nDieAddLow[i] = 2;	nDieAddHigh[i] = 8;	break;

					case DND_CHARACTER_CLASS_THIEF:			
					case DND_CHARACTER_CLASS_THIEF_ACROBAT:
					case DND_CHARACTER_CLASS_ASSASSIN:		nBaseAge[i] = 22;	nDieAddLow[i] = 3;	nDieAddHigh[i] = 8;	break;	
				}

				break;
			}

			case DND_CHARACTER_RACE_HALFLING:
			{
				switch(pCharacter->m_Class[i])
				{
					case DND_CHARACTER_CLASS_NONE:
					case DND_CHARACTER_CLASS_CLERIC:		
					case DND_CHARACTER_CLASS_DRUID:			
					case DND_CHARACTER_CLASS_MONK:			

					case DND_CHARACTER_CLASS_FIGHTER:		
					case DND_CHARACTER_CLASS_RANGER:		
					case DND_CHARACTER_CLASS_PALADIN:		
					case DND_CHARACTER_CLASS_CAVALIER:		
					case DND_CHARACTER_CLASS_BARBARIAN:		nBaseAge[i] = 20;	nDieAddLow[i] = 3;	nDieAddHigh[i] = 4;	break;

					case DND_CHARACTER_CLASS_MAGE:			
					case DND_CHARACTER_CLASS_ILLUSIONIST:	

					case DND_CHARACTER_CLASS_THIEF:			
					case DND_CHARACTER_CLASS_THIEF_ACROBAT:
					case DND_CHARACTER_CLASS_ASSASSIN:		nBaseAge[i] = 40;	nDieAddLow[i] = 2;	nDieAddHigh[i] = 4;	break;	
				}

				break;
			}

			case DND_CHARACTER_RACE_HALF_ORC:
			{
				switch(pCharacter->m_Class[i])
				{
					case DND_CHARACTER_CLASS_CLERIC:		
					case DND_CHARACTER_CLASS_DRUID:			
					case DND_CHARACTER_CLASS_MONK:			nBaseAge[i] = 20;	nDieAddLow[i] = 1;	nDieAddHigh[i] = 4;	break;

					case DND_CHARACTER_CLASS_FIGHTER:		
					case DND_CHARACTER_CLASS_RANGER:		
					case DND_CHARACTER_CLASS_PALADIN:		
					case DND_CHARACTER_CLASS_CAVALIER:		
					case DND_CHARACTER_CLASS_BARBARIAN:		nBaseAge[i] = 13;	nDieAddLow[i] = 1;	nDieAddHigh[i] = 4;	break;

					case DND_CHARACTER_CLASS_MAGE:			
					case DND_CHARACTER_CLASS_ILLUSIONIST:	

					case DND_CHARACTER_CLASS_THIEF:			
					case DND_CHARACTER_CLASS_THIEF_ACROBAT:
					case DND_CHARACTER_CLASS_ASSASSIN:		nBaseAge[i] = 20;	nDieAddLow[i] = 1;	nDieAddHigh[i] = 4;	break;	
				}

				break;
			}

			case DND_CHARACTER_RACE_HUMAN:
			default:
			{
				switch(pCharacter->m_Class[i])
				{
					case DND_CHARACTER_CLASS_NONE:			nBaseAge[i] = 15;	nDieAddLow[i] = 1;	nDieAddHigh[i] = 4;	break;
					case DND_CHARACTER_CLASS_FIGHTER:		nBaseAge[i] = 15;	nDieAddLow[i] = 1;	nDieAddHigh[i] = 4;	break;
					case DND_CHARACTER_CLASS_RANGER:		nBaseAge[i] = 20;	nDieAddLow[i] = 1;	nDieAddHigh[i] = 4;	break;
					case DND_CHARACTER_CLASS_PALADIN:		nBaseAge[i] = 17;	nDieAddLow[i] = 1;	nDieAddHigh[i] = 4;	break;
					case DND_CHARACTER_CLASS_CAVALIER:		nBaseAge[i] = 20;	nDieAddLow[i] = 1;	nDieAddHigh[i] = 4;	break;
					case DND_CHARACTER_CLASS_BARBARIAN:		nBaseAge[i] = 15;	nDieAddLow[i] = 1;	nDieAddHigh[i] = 4;	break;

					case DND_CHARACTER_CLASS_CLERIC:		nBaseAge[i] = 18;	nDieAddLow[i] = 1;	nDieAddHigh[i] = 4;	break;
					case DND_CHARACTER_CLASS_DRUID:			nBaseAge[i] = 18;	nDieAddLow[i] = 1;	nDieAddHigh[i] = 4;	break;
					case DND_CHARACTER_CLASS_MONK:			nBaseAge[i] = 21;	nDieAddLow[i] = 1;	nDieAddHigh[i] = 4;	break;

					case DND_CHARACTER_CLASS_THIEF:			nBaseAge[i] = 18;	nDieAddLow[i] = 1;	nDieAddHigh[i] = 4;	break;
					case DND_CHARACTER_CLASS_THIEF_ACROBAT:	nBaseAge[i] = 18;	nDieAddLow[i] = 1;	nDieAddHigh[i] = 4;	break;
					case DND_CHARACTER_CLASS_ASSASSIN:		nBaseAge[i] = 20;	nDieAddLow[i] = 1;	nDieAddHigh[i] = 4;	break;

					case DND_CHARACTER_CLASS_MAGE:			nBaseAge[i] = 24;	nDieAddLow[i] = 2;	nDieAddHigh[i] = 8;	break;
					case DND_CHARACTER_CLASS_ILLUSIONIST:	nBaseAge[i] = 30;	nDieAddLow[i] = 1;	nDieAddHigh[i] = 6;	break;
				}

				break;
			}
		}
	}

	int nMaxClass = 0;
	int nMaxVal = 0;
	BOOL bMultiClassed = FALSE;

	for(int j = 0; j < 4; ++j)
	{
		if(nBaseAge[j] > nMaxVal)
		{
			nMaxVal = nBaseAge[j];
			nMaxClass = j;
			if(j > 0)
			{
				bMultiClassed = TRUE;
			}
		}
	}

	if(bMultiClassed)
	{
		nRetVal = nBaseAge[nMaxClass] + nDieAddLow[nMaxClass] * nDieAddHigh[nMaxClass];
	}
	else
	{
		for(int k = 1;  k <= nDieAddLow[nMaxClass]; ++k)
		{
			if(nDieAddHigh[nMaxClass])
			{
				nRetVal += (rand() % nDieAddHigh[nMaxClass]) + 1;
			}
		}

		nRetVal += nBaseAge[nMaxClass];
	}

	return nRetVal;
}


int GetXPFromMonsterManual(PDNDMONSTERMANUALENTRY pMonster, int nHitDice, int nHitDicePlus, int nHitPoints)
{
	int nRetVal = 0;

	CString szXP = pMonster->m_szXP;

	int nLength = szXP.GetLength();

	if(nLength)
	{
		int nSpecial = 0;
		int nExceptional = 0;

		int nCalcLocation = szXP.Find("CALC", 0);

		if(nCalcLocation >= 0)
		{
			szXP.Replace("CALC", "");

			nLength = szXP.GetLength();

			int nSpcLocation = szXP.Find("Spc", 0);

			if(nSpcLocation >=0)
			{
				nSpecial = atoi(szXP.GetBuffer(0));
				
				int nExcLocation = szXP.Find("Exc", 0);

				if(nExcLocation >=0)
				{
					nSpcLocation += 3;
					CString szExc = szXP.Right(nLength - nSpcLocation);

					nExceptional = atoi(szExc.GetBuffer(0));
				}

			}

			nRetVal = CalculateXP(nHitDice, nHitDicePlus, nHitPoints, nSpecial, nExceptional);
		}
		else //assuming NNNNN+XX/hp from MonsterManual.dat
		{
			CString szXPBase;
			CString szHPAdd;

			int nPlusLocation = szXP.Find("+", 0);

			if(nPlusLocation >=0)
			{
				szXPBase = szXP.Left(nPlusLocation);

				szHPAdd = szXP.Right(nLength-nPlusLocation-1);
				szHPAdd.Replace("/hp", " ");

			}
			else
			{
				szXPBase = szXP;
				szHPAdd =  _T("0");
			}

			nRetVal = atoi(szXPBase.GetBuffer(0)) + atoi(szHPAdd.GetBuffer(0)) * nHitPoints;
		}
	}

	return nRetVal;
}

int GetXPValueForCharacter(cDNDCharacter *pCharacter, int *pnSpecialAbility, int *pnExceptionalAbility)
{
	int nReturnedXP = 0;
	int nXPHitDice = 0;

	float fLevels = 0.0f;
	float fLevelTotal = 0.0f;

	//calc average level
	BOOL bHasLevel = FALSE;
	for(int i = 0; i < 3; ++i)
	{
		if(pCharacter->m_nLevel[i] && pCharacter->m_Class[i] != DND_CHARACTER_CLASS_UNDEF)
		{
			fLevels += 1.0f;
			fLevelTotal += (float)pCharacter->m_nLevel[i];
			bHasLevel = TRUE;
		}
	}

	if(bHasLevel)
	{
		fLevelTotal /= fLevels;
		nXPHitDice = (int)(fLevelTotal + 0.5f);
	}

	for(int i = 0; i < 3; ++i)
	{
		switch(pCharacter->m_Class[i])
		{
			case DND_CHARACTER_CLASS_NONE:
			case DND_CHARACTER_CLASS_FIGHTER:
			{
				break;
			}
			case DND_CHARACTER_CLASS_RANGER:		
			{
				nXPHitDice += 1;
				*pnSpecialAbility += 1; // surprise bonus
				*pnSpecialAbility += 1; // giant class opponent bonus

				if(nXPHitDice >= 9)
					*pnExceptionalAbility += 1; // offensive spell use

				break;
			}
			case DND_CHARACTER_CLASS_CAVALIER:	
			{
				nXPHitDice += 1;
				break;
			}
			case DND_CHARACTER_CLASS_PALADIN:		
			{
				*pnSpecialAbility += 1; // paladin abilities bonus
				
				if(nXPHitDice >= 3)
					*pnSpecialAbility += 1; // turn undead

				if(nXPHitDice >= 9)
					*pnExceptionalAbility += 1; // offensive spell use

				break;
			}
			case DND_CHARACTER_CLASS_BARBARIAN:		
			{
				break;
			}
			case DND_CHARACTER_CLASS_CLERIC:			
			{
				*pnSpecialAbility += 1; // defensive spell use
				*pnSpecialAbility += 1; // turn undead
				*pnExceptionalAbility += 1; // offensive spell use
				break;
			}
			case DND_CHARACTER_CLASS_DRUID:			
			{
				*pnSpecialAbility += 1; // defensive spell use
				*pnExceptionalAbility += 1; // offensive spell use
				break;
			}
			case DND_CHARACTER_CLASS_MONK:			
			{
				nXPHitDice += 1;
				break;
			}
			case DND_CHARACTER_CLASS_MAGE:			
			{
				*pnSpecialAbility += 1; // defensive spell use
				*pnExceptionalAbility += 1; // offensive spell use
				break;
			}
			case DND_CHARACTER_CLASS_ILLUSIONIST:	
			{
				*pnSpecialAbility += 1; // defensive spell use
				*pnExceptionalAbility += 1; // offensive spell use
				break;
			}

			case DND_CHARACTER_CLASS_THIEF:			
			{
				*pnSpecialAbility += 1;		//backstab
				break;
			}
			case DND_CHARACTER_CLASS_THIEF_ACROBAT: 
			{
				*pnSpecialAbility += 1;		//backstab
				break;
			}
			case DND_CHARACTER_CLASS_ASSASSIN:		
			{
				*pnSpecialAbility += 1;		//assassination
				break;
			}
		}
	}

	if(pCharacter->m_nCurrentArmorClass <= 0)
	{
		*pnSpecialAbility += 1;
	}

	nReturnedXP = CalculateXP(nXPHitDice, 0, pCharacter->m_nHitPoints, *pnSpecialAbility, *pnExceptionalAbility);

	return nReturnedXP;
}

int CalculateXP(int nHitDice, int nHitDicePlus, int nHitPoints, int nSpecialAbility, int nExceptionalAbility)
{
	int nRetVal = 0;

	int _XPMatrix[17][4] = 
	{
		// BASEXP		XP/HP	SPCXP		EXCXP	//LEVEL or Hit Dice		nRow
		{	5,			1,		2,			25 },	// up to 1-1			0
		{	10,			1,		4,			35 },	// 1-1 to 1				1
		{	20,			2,		8,			45 },	// 1+1 to 2				2
		{	35,			3,		15,			55 },	// 2+1 to 3				3
		{	60,			4,		25,			65 },	// 3+1 to 4				4
		{	90,			5,		40,			75 },	// 4+1 to 5				5
		{	150,		6,		75,			125 },	// 5+1 to 6				6
		{	225,		8,		125,		175 },	// 6+1 to 7				7
		{	375,		10,		175,		275 },	// 7+1 to 8				8
		{	600,		12,		300,		400 },	// 8+1 to 9				9
		{	900,		14,		450,		600 },	// 9+1 to 10+			10
		{	1300,		16,		700,		850 },	// 11 to 12+			11
		{	1800,		18,		950,		1200 },	// 13 to 14+			12
		{	2400,		20,		1250,		1600 },	// 15 to 16+			13
		{	3000,		25,		1550,		2000 },	// 17 to 18+			14
		{	4000,		30,		2100,		2500 },	// 19 to 20+			15
		{	5000,		35,		2600,		3000 }	// 21+					16

	};

	int nRow = 0;

	if(nHitDice == 1 && nHitDicePlus <= 0)
		nRow = 1;

	if(nHitDice < 11)
	{
		nRow = nHitDice;
		if(nHitDicePlus > 0)
			nRow +=1;
	}
	else if(nHitDice >= 11 && nHitDice <= 20)
	{
		if(nHitDice <= 12)
			nRow = 11;
		else if(nHitDice < 14)
			nRow = 12;
		else if(nHitDice < 16)
			nRow = 13;
		else if(nHitDice < 18)
			nRow = 14;
		else if(nHitDice < 20)
			nRow = 15;
	}
	else if(nHitDice >= 21)
	{
		nRow = 16;
	}

	nRetVal = _XPMatrix[nRow][0] + _XPMatrix[nRow][1] * nHitPoints + _XPMatrix[nRow][2] * nSpecialAbility + _XPMatrix[nRow][3] * nExceptionalAbility;

	return nRetVal;

}

int GetAverageCharacterLevel(cDNDCharacter *pCharacter)
{
	int nRetVal = 0;

	float fLevels = 0.0f;
	float fLevelTotal = 0.0f;

	//calc average level
	BOOL bHasLevel = FALSE;
	for(int i = 0; i < 3; ++i)
	{
		if(pCharacter->m_nLevel[i] && pCharacter->m_Class[i] != DND_CHARACTER_CLASS_UNDEF)
		{
			fLevels += 1.0f;
			fLevelTotal += (float)pCharacter->m_nLevel[i];
			bHasLevel = TRUE;
		}
	}

	if(bHasLevel)
	{
		fLevelTotal /= fLevels;
	}

	nRetVal = (int)(fLevelTotal + 0.5f); // round up

	return nRetVal;
}

POBJECTTYPE FindDNDObjectByName(CString szItemName)
{
	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	for(int k = 0; k < pApp->m_ObjectsOrderedTypeArray.GetSize(); ++k)
	{
		POBJECTTYPE pObjBase = pApp->m_ObjectsOrderedTypeArray.GetAt(k);

		if(pObjBase != NULL)
		{
			CString szCompare = pObjBase->m_szType;
			if(szCompare == szItemName)
			{
				return pObjBase;
			}
		}
	}


	return NULL;
}

char *GetRandomJewelName()
{
	int nName = rand() % 8;

	switch(nName)
	{
		case 0: return "diamond";	
		case 1: return "emerald";
		case 2: return "sapphire";
		case 3: return "ruby";
		case 4: return "opal";
		case 5: return "pearl";
		case 6: return "jade";
		case 7: return "amber";
	}

	return "diamond";
}

char *GetRandomJewelryName(int *nValueDiv)
{
	static char szRetName[128];
	memset(szRetName, 0, 128 * sizeof(char));

	int nName = rand() % 12;

	switch(nName)
	{
		case 0: strcat(szRetName, "engraved");	break;	
		case 1: strcat(szRetName, "jewelled");	break;	
		case 2: strcat(szRetName, "jewelled");	break;
		case 3: strcat(szRetName, "jewelled");	break;
		case 4: strcat(szRetName, "plain");		*nValueDiv *= 2; break;	
		case 5: strcat(szRetName, "plain");		*nValueDiv *= 2; break;
		case 6: strcat(szRetName, "plain");		*nValueDiv *= 2; break;
		case 7: strcat(szRetName, "plain");		*nValueDiv *= 2; break;
		case 8: strcat(szRetName, "plain");		*nValueDiv *= 2; break;
		case 9: strcat(szRetName, "plated");	*nValueDiv *= 10; break;	
		case 10: strcat(szRetName, "plated");	*nValueDiv *= 10; break;
		case 11: strcat(szRetName, "plated");	*nValueDiv *= 10; break;
		
	}

	nName = rand() % 12;

	switch(nName)
	{
		case 0: strcat(szRetName, " copper");	*nValueDiv *= 10;	break;	
		case 1: strcat(szRetName, " silver");	*nValueDiv *= 5;		break;	
		case 2: strcat(szRetName, " electrum");	*nValueDiv *= 2;		break;	
		case 3: strcat(szRetName, " gold");		*nValueDiv *= 1;		break;	
		case 4: strcat(szRetName, " platinum");	*nValueDiv *= 1;		break;	
		case 5: strcat(szRetName, " brass");	*nValueDiv *= 20;	break;	
		case 6: strcat(szRetName, " brass");	*nValueDiv *= 20;	break;
		case 7: strcat(szRetName, " bronze");	*nValueDiv *= 20;	break;	
		case 8: strcat(szRetName, " bronze");	*nValueDiv *= 20;	break;	
		case 9: strcat(szRetName, " iron");		*nValueDiv *= 50;	break;	
		case 10: strcat(szRetName, " steel");	*nValueDiv *= 50;	break;	
		case 11: strcat(szRetName, " mithril");	*nValueDiv *= 1;		break;	
	}

	nName = rand() % 41;

	switch(nName)
	{
		case 0: strcat(szRetName, " amulet");		break;
		case 1: strcat(szRetName, " amulet");		break;
		case 2: strcat(szRetName, " amulet");		break;
		case 3: strcat(szRetName, " anklet");		break;
		case 4: strcat(szRetName, " armband");		break;
		case 5: strcat(szRetName, " bracelet");		break;
		case 6: strcat(szRetName, " bracelet");		break;
		case 7: strcat(szRetName, " bracelet");		break;
		case 8: strcat(szRetName, " bracelet");		break;
		case 9: strcat(szRetName, " brooch");		break;
		case 10: strcat(szRetName, " brooch");		break;
		case 11: strcat(szRetName, " brooch");		break;
		case 12: strcat(szRetName, " brooch");		break;
		case 13: strcat(szRetName, " crown");		break;
		case 14: strcat(szRetName, " earring");		break;
		case 15: strcat(szRetName, " earring");		break;
		case 16: strcat(szRetName, " earring");		break;
		case 17: strcat(szRetName, " earring");		break;
		case 18: strcat(szRetName, " earrings");	break;
		case 19: strcat(szRetName, " earrings");	break;
		case 20: strcat(szRetName, " earrings");	break;
		case 21: strcat(szRetName, " hairpin");		break;
		case 22: strcat(szRetName, " necklace");	break;
		case 23: strcat(szRetName, " necklace");	break;
		case 24: strcat(szRetName, " necklace");	break;
		case 25: strcat(szRetName, " necklace");	break;
		case 26: strcat(szRetName, " necklace");	break;
		case 27: strcat(szRetName, " necklace");	break;
		case 28: strcat(szRetName, " necklace");	break;
		case 29: strcat(szRetName, " nosering");	break;
		case 30: strcat(szRetName, " pendant");		break;
		case 31: strcat(szRetName, " pendant");		break;
		case 32: strcat(szRetName, " pendant");		break;
		case 33: strcat(szRetName, " ring");		break;
		case 34: strcat(szRetName, " ring");		break;
		case 35: strcat(szRetName, " ring");		break;
		case 36: strcat(szRetName, " ring");		break;
		case 37: strcat(szRetName, " ring");		break;
		case 38: strcat(szRetName, " tiara");		break;
		case 39: strcat(szRetName, " torc");		break;
		case 40: strcat(szRetName, " torc");		break;
	}

	szRetName[31] = 0;

	return szRetName;
}

int FindTreasureTypeMultiplier(CString szTreasureType, CString szType)
{
	int nRetVal = 1;

	int nStartPos = szTreasureType.Find(szType);
	if(nStartPos == -1)
	{
		return nRetVal;
	}

	char sComma = ',';

	int nLength =  szTreasureType.GetLength();
	//find the comma or end of the string
	for(int nComma = nStartPos; nComma < nLength; ++nComma)
	{
		if(szTreasureType.GetAt(nComma) == sComma)
		{
			break;
		}
	}

	int nSpan = nComma - nStartPos;
	CString szSection = szTreasureType.Mid(nStartPos, nSpan);

	szSection.Replace(szType, "");

	nRetVal = atoi(szSection.GetBuffer(0));

	return nRetVal;
}

void RollMonsterJewelTreasure(cDNDNonPlayerCharacter *pNPC, int nMultiplier, int nPercentage, int nAmount, int nMinValue, int nMaxValue)
{
	int nValueSpan = nMaxValue - nMinValue;
	int nSpans = nValueSpan / 10;

	for(int i = 0; i < nMultiplier; ++i)
	{
		if(RollD100() <= nPercentage)
		{
			POBJECTTYPE pObject = FindDNDObjectByName("gem, basic");

			if(pObject != NULL)
			{
				int nSlot = 0;
				pNPC->AddToInventory(pObject, &nSlot);

				int nValue = rand()%nSpans * 10 + nMinValue;

				sprintf(pNPC->m_Inventory[nSlot].m_szType, "%s\0", GetRandomJewelName());
				pNPC->m_Inventory[nSlot].m_lAmount = nAmount;
				pNPC->m_Inventory[nSlot].m_nItemValue = nValue;
			}
		}
	}
}

void RollMonsterJewelryTreasure(cDNDNonPlayerCharacter *pNPC, int nMultiplier, int nPercentage, int nAmount, int nMinValue, int nMaxValue)
{
	int nValueSpan = nMaxValue - nMinValue;
	int nSpans = nValueSpan / 10;

	for(int i = 0; i < nMultiplier; ++i)
	{
		if(RollD100() <= nPercentage)
		{
			POBJECTTYPE pObject = FindDNDObjectByName("brooch, plain");

			if(pObject != NULL)
			{
				int nSlot = 0;

				pNPC->AddToInventory(pObject, &nSlot);

				int nValue = rand()%nSpans * 10 + nMinValue;

				int nValueDiv = 1;
				sprintf(pNPC->m_Inventory[nSlot].m_szType, "%s\0", GetRandomJewelryName(&nValueDiv));

				nValue /= nValueDiv;
				nValue = max(nValue, 1);

				pNPC->m_Inventory[nSlot].m_nItemValue = nValue;
				pNPC->m_Inventory[nSlot].m_lAmount = nAmount;
			}
		}
	}
}

BOOL RollMonsterMagicTreasure(cDNDNonPlayerCharacter *pNPC, int nMultiplier, int nPercentage, UINT nTableFlags)
{
	if(nMultiplier > 1)
	{
		return RollMonsterMagicTreasure(pNPC, nMultiplier-1, nPercentage, nTableFlags);
	}

	BOOL bRetVal = FALSE;

	int nDieRoll = RollD100();

	if(nDieRoll > nPercentage)
	{
		return FALSE;
	}

	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	if(nTableFlags == MAGIC_ITEM_FLAG_ANY)
	{
		int nDieRoll = RollD100();

		if(nDieRoll <= 20)
			nTableFlags = MAGIC_ITEM_FLAG_POTIONS;
		else if(nDieRoll <= 35)
			nTableFlags = MAGIC_ITEM_FLAG_SCROLLS;
		else if(nDieRoll <= 40)
			nTableFlags = MAGIC_ITEM_FLAG_RINGS;
		else if(nDieRoll <= 45)
			nTableFlags = MAGIC_ITEM_FLAG_RODS_STAVES_WANDS;
		else if(nDieRoll <= 48)
			nTableFlags = MAGIC_ITEM_FLAG_MISC_MAGIC_1;
		else if(nDieRoll <= 51)
			nTableFlags = MAGIC_ITEM_FLAG_MISC_MAGIC_2;
		else if(nDieRoll <= 54)
			nTableFlags = MAGIC_ITEM_FLAG_MISC_MAGIC_3;
		else if(nDieRoll <= 57)
			nTableFlags = MAGIC_ITEM_FLAG_MISC_MAGIC_4;
		else if(nDieRoll <= 60)
			nTableFlags = MAGIC_ITEM_FLAG_MISC_MAGIC_5;
		else if(nDieRoll <= 75)
			nTableFlags = MAGIC_ITEM_FLAG_ARMOR;
		else if(nDieRoll <= 86)
			nTableFlags = MAGIC_ITEM_FLAG_SWORDS;
		else if(nDieRoll <= 100)
			nTableFlags = MAGIC_ITEM_FLAG_MISC_WEAPONS;

	}

	int nTableIndex[13];
	memset(nTableIndex, 0, 13*sizeof(int));

	int nTables = 0;

	if(nTableFlags & MAGIC_ITEM_FLAG_ARMOR)				{ nTableIndex[nTables++] = DND_MAGIC_ITEM_TABLE_ARMOR; }
	if(nTableFlags & MAGIC_ITEM_FLAG_SWORDS)			{ nTableIndex[nTables++] = DND_MAGIC_ITEM_TABLE_SWORDS; }	
	if(nTableFlags & MAGIC_ITEM_FLAG_RINGS)				{ nTableIndex[nTables++] = DND_MAGIC_ITEM_TABLE_RINGS; }	
	if(nTableFlags & MAGIC_ITEM_FLAG_POTIONS)			{ nTableIndex[nTables++] = DND_MAGIC_ITEM_TABLE_POTIONS; }		
	if(nTableFlags & MAGIC_ITEM_FLAG_MISC_WEAPONS)		{ nTableIndex[nTables++] = DND_MAGIC_ITEM_TABLE_MISC_WEAPONS; }		
	if(nTableFlags & MAGIC_ITEM_FLAG_RODS_STAVES_WANDS)	{ nTableIndex[nTables++] = DND_MAGIC_ITEM_TABLE_RODS_STAVES_WANDS;}
	if(nTableFlags & MAGIC_ITEM_FLAG_MISC_MAGIC_1)		{ nTableIndex[nTables++] = DND_MAGIC_ITEM_TABLE_MISC_MAGIC_1; }		
	if(nTableFlags & MAGIC_ITEM_FLAG_MISC_MAGIC_2)		{ nTableIndex[nTables++] = DND_MAGIC_ITEM_TABLE_MISC_MAGIC_2; }		
	if(nTableFlags & MAGIC_ITEM_FLAG_MISC_MAGIC_3)		{ nTableIndex[nTables++] = DND_MAGIC_ITEM_TABLE_MISC_MAGIC_3; }		
	if(nTableFlags & MAGIC_ITEM_FLAG_MISC_MAGIC_4)		{ nTableIndex[nTables++] = DND_MAGIC_ITEM_TABLE_MISC_MAGIC_4; }		
	if(nTableFlags & MAGIC_ITEM_FLAG_MISC_MAGIC_5)		{ nTableIndex[nTables++] = DND_MAGIC_ITEM_TABLE_MISC_MAGIC_5; }		
	if(nTableFlags & MAGIC_ITEM_FLAG_SCROLLS)			{ nTableIndex[nTables++] = DND_MAGIC_ITEM_TABLE_SCROLLS; }		
	if(nTableFlags & MAGIC_ITEM_FLAG_ARTIFACTS_RELICS)	{ nTableIndex[nTables++] = DND_MAGIC_ITEM_TABLE_MISC_MAGIC_SPECIAL; }		

	if(nTables)
	{
		nTables = rand()%nTables;
	}

	nDieRoll = RollD100();

	//nDieRoll = 75;
	//nTableIndex[nTables] = DND_MAGIC_ITEM_TABLE_RODS_STAVES_WANDS;

	if(nTableIndex[nTables] == DND_MAGIC_ITEM_TABLE_MISC_MAGIC_1 && nDieRoll == 17) 
	{
		nTableIndex[nTables] = DND_MAGIC_ITEM_TABLE_MISC_MAGIC_SPECIAL;
		nDieRoll = RollD100();
	}

	for (int i = 0; i < pApp->m_MagicTableItemOrderedTypeArray.GetSize(); ++i )
	{
		PMAGICTABLEITEM pMagicItem = pApp->m_MagicTableItemOrderedTypeArray.GetAt(i);

		if(pMagicItem != NULL && pMagicItem->m_nTable == nTableIndex[nTables])
		{
			if(nDieRoll >= pMagicItem->m_nRollLow && nDieRoll <= pMagicItem->m_nRollHigh)
			{
				int nMundaneItem = 0;
				int nMagicAdjustment = pMagicItem->m_nMagicAdjustmentLow;
				int nAmount = pMagicItem->m_nAmountLow;

				if(pMagicItem->m_nMundanes > 1)
				{
					nMundaneItem = rand() % pMagicItem->m_nMundanes;
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

				POBJECTTYPE pObject = pMagicItem->EnchantItem(nMundaneItem, nMagicAdjustment, nAmount);


				if(nTableIndex[nTables] == DND_MAGIC_ITEM_TABLE_RODS_STAVES_WANDS && pObject->m_nCharges == 0)
				{
					pObject->m_nCharges = (rand()%96);
					pObject->m_nCharges += 5;
				}
				if(nTableIndex[nTables] == DND_MAGIC_ITEM_TABLE_SCROLLS) 
				{
					TRACE("SCROLL ( %s )!\n", pObject->m_szType);
				}
				if(nTableIndex[nTables] == DND_MAGIC_ITEM_TABLE_POTIONS) 
				{
					TRACE("POTION ( %s )!\n", pObject->m_szType);
				}

				pNPC->AddToInventory(pObject);

				delete pObject;

				bRetVal = TRUE;
				
				break;
			}
		}
	}

	return bRetVal;
}

void RollMonsterCoins(LONG *pCoinSlot, int nMultiplier, int nPercentage, int nDieRoll, int nCoinStacks)
{
	for(int i = 0; i < nMultiplier; ++i)
	{
		if(RollD100() <= nPercentage)
		{
			*pCoinSlot += nDieRoll * nCoinStacks;
		}
	}
}

void RollMonsterTreasure(cDNDNonPlayerCharacter *pNPC, PDNDMONSTERMANUALENTRY pMonster, BOOL bLair, BOOL bAddCoins)
{
	/*
	Monster Manual Line
	1256 | Orc | Humanoid | Common | Low to Average (5-10) | 35% | L (C,O,Qx10,S) | 7 | 30-300 | 6 | 9 | 1 | 1 | 1-8 or by weapon | Nil | Nil | Standard | M (6'+) | Nil | Nil | 10+1/hp
	*/

	if(pNPC == NULL || pMonster == NULL)
	{
		return;
	}

	if(pMonster->m_szTreasureType.Find("Nil") > -1)
	{
		return;
	}

	if(bAddCoins == FALSE)
	{
		pNPC->m_lCopperCarried = 0L;
		pNPC->m_lSilverCarried = 0L;
		pNPC->m_lElectrumCarried = 0L;
		pNPC->m_lGoldCarried = 0L;
		pNPC->m_lPlatinumCarried = 0L;
	}

	int nMultiplier = 0;
	int nPercentage = 0;

	//individual treasure types
	if(pMonster->m_szTreasureType.Find("J") > -1 && bLair == FALSE)
	{
		pNPC->m_lCopperCarried += RollD8() + RollD8() + RollD8();
	}

	if(pMonster->m_szTreasureType.Find("K") > -1 && bLair == FALSE)
	{
		pNPC->m_lSilverCarried += RollD6() + RollD6() + RollD6();
	}

	if(pMonster->m_szTreasureType.Find("L") > -1 && bLair == FALSE)
	{
		pNPC->m_lElectrumCarried += RollD6() + RollD6();
	}

	if(pMonster->m_szTreasureType.Find("M") > -1 && bLair == FALSE)
	{
		pNPC->m_lGoldCarried += RollD4() + RollD4();
	}

	if(pMonster->m_szTreasureType.Find("N") > -1 && bLair == FALSE)
	{
		pNPC->m_lPlatinumCarried += RollD6();
	}

	////////////////////////////////////////

	//lair treasure types

	// bLair = TRUE; // DEBUG/DEVEL ONLY remove for release

	if(pMonster->m_szTreasureType.Find("A") > -1 && bLair == TRUE) 
	{
		nMultiplier = FindTreasureTypeMultiplier(pMonster->m_szTreasureType, "Ax");

		RollMonsterCoins(&pNPC->m_lCopperCarried, nMultiplier, 25, RollD6(), 1000);
		RollMonsterCoins(&pNPC->m_lSilverCarried, nMultiplier, 30, RollD6(), 1000);
		RollMonsterCoins(&pNPC->m_lElectrumCarried, nMultiplier, 35, RollD6(), 1000);
		RollMonsterCoins(&pNPC->m_lGoldCarried, nMultiplier, 40, RollD3(), 1000);
		RollMonsterCoins(&pNPC->m_lPlatinumCarried, nMultiplier, 25, RollD4(), 100);
		
		RollMonsterJewelTreasure(pNPC, nMultiplier, 60, RollD10()*4, 100, 500);
		RollMonsterJewelryTreasure(pNPC, nMultiplier, 50, RollD10()*3, 10, 500);

		RollMonsterMagicTreasure(pNPC, nMultiplier*3, 30, MAGIC_ITEM_FLAG_ANY); 
	}

	if(pMonster->m_szTreasureType.Find("B") > -1 && bLair == TRUE) 
	{
		nMultiplier = FindTreasureTypeMultiplier(pMonster->m_szTreasureType, "Bx");

		RollMonsterCoins(&pNPC->m_lCopperCarried, nMultiplier, 50, RollD8(), 1000);
		RollMonsterCoins(&pNPC->m_lSilverCarried, nMultiplier, 25, RollD6(), 1000);
		RollMonsterCoins(&pNPC->m_lElectrumCarried, nMultiplier, 25, RollD4(), 1000);
		RollMonsterCoins(&pNPC->m_lGoldCarried, nMultiplier, 25, RollD3(), 1000);
		
		RollMonsterJewelTreasure(pNPC, nMultiplier, 30, RollD8(), 100, 500);
		RollMonsterJewelryTreasure(pNPC, nMultiplier, 20, RollD4(), 10, 500);

		RollMonsterMagicTreasure(pNPC, nMultiplier, 10, MAGIC_ITEM_FLAG_SWORDS | MAGIC_ITEM_FLAG_ARMOR | MAGIC_ITEM_FLAG_MISC_WEAPONS); 
	}

	if(pMonster->m_szTreasureType.Find("C") > -1 && bLair == TRUE) 
	{
		nMultiplier = FindTreasureTypeMultiplier(pMonster->m_szTreasureType, "Cx");

		RollMonsterCoins(&pNPC->m_lCopperCarried, nMultiplier, 20, RollD12(), 1000);
		RollMonsterCoins(&pNPC->m_lSilverCarried, nMultiplier, 30, RollD6(), 1000);
		RollMonsterCoins(&pNPC->m_lElectrumCarried, nMultiplier, 10, RollD4(), 1000);
		
		RollMonsterJewelTreasure(pNPC, nMultiplier, 25, RollD6(), 100, 500);
		RollMonsterJewelryTreasure(pNPC, nMultiplier, 20, RollD3(), 10, 500);

		RollMonsterMagicTreasure(pNPC, nMultiplier*2, 10, MAGIC_ITEM_FLAG_ANY); 
	}

	if(pMonster->m_szTreasureType.Find("D") > -1 && bLair == TRUE) 
	{
		nMultiplier = FindTreasureTypeMultiplier(pMonster->m_szTreasureType, "Dx");

		RollMonsterCoins(&pNPC->m_lCopperCarried, nMultiplier, 10, RollD8(), 1000);
		RollMonsterCoins(&pNPC->m_lSilverCarried, nMultiplier, 15, RollD12(), 1000);
		RollMonsterCoins(&pNPC->m_lElectrumCarried, nMultiplier, 15, RollD8(), 1000);
		RollMonsterCoins(&pNPC->m_lGoldCarried, nMultiplier, 50, RollD6(), 1000);
		
		RollMonsterJewelTreasure(pNPC, nMultiplier, 30, RollD10(), 100, 500);
		RollMonsterJewelryTreasure(pNPC, nMultiplier, 25, RollD6(), 10, 500);

		if(RollMonsterMagicTreasure(pNPC, nMultiplier*2, 15, MAGIC_ITEM_FLAG_ANY))
		{
			RollMonsterMagicTreasure(pNPC, nMultiplier, 100, MAGIC_ITEM_FLAG_POTIONS); //yes 100% chance
		}
	}

	if(pMonster->m_szTreasureType.Find("E") > -1 && bLair == TRUE) 
	{
		nMultiplier = FindTreasureTypeMultiplier(pMonster->m_szTreasureType, "Ex");

		RollMonsterCoins(&pNPC->m_lCopperCarried, nMultiplier, 5, RollD10(), 1000);
		RollMonsterCoins(&pNPC->m_lSilverCarried, nMultiplier, 25, RollD12(), 1000);
		RollMonsterCoins(&pNPC->m_lElectrumCarried, nMultiplier, 25, RollD6(), 1000);
		RollMonsterCoins(&pNPC->m_lGoldCarried, nMultiplier, 25, RollD8(), 1000);
		
		RollMonsterJewelTreasure(pNPC, nMultiplier, 15, RollD12(), 100, 1000);
		RollMonsterJewelryTreasure(pNPC, nMultiplier, 10, RollD8(), 10, 1000);

		if(RollMonsterMagicTreasure(pNPC, nMultiplier*3, 25, MAGIC_ITEM_FLAG_ANY))
		{
			RollMonsterMagicTreasure(pNPC, nMultiplier, 100, MAGIC_ITEM_FLAG_SCROLLS); //yes 100% chance
		}
	}

	if(pMonster->m_szTreasureType.Find("F") > -1 && bLair == TRUE) 
	{
		nMultiplier = FindTreasureTypeMultiplier(pMonster->m_szTreasureType, "Fx");

		RollMonsterCoins(&pNPC->m_lSilverCarried, nMultiplier, 10, RollD20(), 1000);
		RollMonsterCoins(&pNPC->m_lElectrumCarried, nMultiplier, 15, RollD12(), 1000);
		RollMonsterCoins(&pNPC->m_lGoldCarried, nMultiplier, 40, RollD10(), 1000);
		RollMonsterCoins(&pNPC->m_lPlatinumCarried, nMultiplier, 35, RollD8(), 100);
		
		RollMonsterJewelTreasure(pNPC, nMultiplier, 30, RollD10()*3, 100, 1000);
		RollMonsterJewelryTreasure(pNPC, nMultiplier, 10, RollD10(), 10, 1000);

		if(RollMonsterMagicTreasure(pNPC, nMultiplier*3, 30, MAGIC_ITEM_FLAG_ARMOR | MAGIC_ITEM_FLAG_RINGS | MAGIC_ITEM_FLAG_POTIONS | MAGIC_ITEM_FLAG_RODS_STAVES_WANDS | MAGIC_ITEM_FLAG_MISC_MAGIC | MAGIC_ITEM_FLAG_SCROLLS))
		{
			RollMonsterMagicTreasure(pNPC, nMultiplier, 100, MAGIC_ITEM_FLAG_POTIONS); //yes 100% chance
			RollMonsterMagicTreasure(pNPC, nMultiplier, 100, MAGIC_ITEM_FLAG_SCROLLS); //yes 100% chance
		}
	}

	if(pMonster->m_szTreasureType.Find("G") > -1 && bLair == TRUE) 
	{
		nMultiplier = FindTreasureTypeMultiplier(pMonster->m_szTreasureType, "Gx");

		RollMonsterCoins(&pNPC->m_lGoldCarried, nMultiplier, 50, RollD4()*10, 1000);
		RollMonsterCoins(&pNPC->m_lPlatinumCarried, nMultiplier, 50, RollD20(), 100);
		
		RollMonsterJewelTreasure(pNPC, nMultiplier, 30, RollD4()*5, 100, 1000);
		RollMonsterJewelryTreasure(pNPC, nMultiplier, 25, RollD10(), 10, 1000);

		if(RollMonsterMagicTreasure(pNPC, nMultiplier*4, 30, MAGIC_ITEM_FLAG_ANY))
		{
			RollMonsterMagicTreasure(pNPC, nMultiplier, 100, MAGIC_ITEM_FLAG_SCROLLS); //yes 100% chance
		}
	}

	if(pMonster->m_szTreasureType.Find("H") > -1 && bLair == TRUE) 
	{
		nMultiplier = FindTreasureTypeMultiplier(pMonster->m_szTreasureType, "Hx");

		RollMonsterCoins(&pNPC->m_lCopperCarried, nMultiplier, 25, RollD6()*5, 1000);
		RollMonsterCoins(&pNPC->m_lSilverCarried, nMultiplier, 40, RollD100(), 1000);
		RollMonsterCoins(&pNPC->m_lElectrumCarried, nMultiplier, 40, RollD4()*10, 1000);
		RollMonsterCoins(&pNPC->m_lGoldCarried, nMultiplier, 55, RollD6()*10, 1000);
		RollMonsterCoins(&pNPC->m_lPlatinumCarried, nMultiplier, 25, RollD10()*5, 100);
		
		RollMonsterJewelTreasure(pNPC, nMultiplier, 50, RollD100(), 100, 1000);
		RollMonsterJewelryTreasure(pNPC, nMultiplier, 50, RollD4()*10, 10, 1000);

		if(RollMonsterMagicTreasure(pNPC, nMultiplier*4, 15, MAGIC_ITEM_FLAG_ANY))
		{
			RollMonsterMagicTreasure(pNPC, nMultiplier, 100, MAGIC_ITEM_FLAG_POTIONS); //yes 100% chance
			RollMonsterMagicTreasure(pNPC, nMultiplier, 100, MAGIC_ITEM_FLAG_SCROLLS); //yes 100% chance
		}
	}

	if(pMonster->m_szTreasureType.Find("I") > -1 && bLair == TRUE) 
	{
		nMultiplier = FindTreasureTypeMultiplier(pMonster->m_szTreasureType, "Ix");

		RollMonsterCoins(&pNPC->m_lPlatinumCarried, nMultiplier, 30, RollD6()*3, 100);
		
		RollMonsterJewelTreasure(pNPC, nMultiplier, 55, RollD10()*2, 100, 1000);
		RollMonsterJewelryTreasure(pNPC, nMultiplier, 50, RollD12(), 10, 1000);

		RollMonsterMagicTreasure(pNPC, nMultiplier, 15, MAGIC_ITEM_FLAG_ANY);
	}

	if(pMonster->m_szTreasureType.Find("Q") > -1 && bLair == TRUE) // 50% chance of 1d4 gems
	{
		nMultiplier = FindTreasureTypeMultiplier(pMonster->m_szTreasureType, "Qx");
		nPercentage = 50;

		RollMonsterJewelTreasure(pNPC, nMultiplier, nPercentage, 4, 10, 200);
	}

	if(pMonster->m_szTreasureType.Find("R") > -1 && bLair == TRUE) 
	{
		nMultiplier = FindTreasureTypeMultiplier(pMonster->m_szTreasureType, "Rx");

		RollMonsterCoins(&pNPC->m_lGoldCarried, nMultiplier, 40, RollD4()*2, 1000);
		RollMonsterCoins(&pNPC->m_lPlatinumCarried, nMultiplier, 50, RollD6()*10, 100);
		
		RollMonsterJewelTreasure(pNPC, nMultiplier, 55, RollD8()*4, 100, 1000);
		RollMonsterJewelryTreasure(pNPC, nMultiplier, 45, RollD4()*12, 10, 1000);
	}

	if(pMonster->m_szTreasureType.Find("S") > -1 && bLair == TRUE) 
	{
		nMultiplier = FindTreasureTypeMultiplier(pMonster->m_szTreasureType, "Sx");

		int nRoll = RollD4()*2;
		RollMonsterMagicTreasure(pNPC, nMultiplier*nRoll, 40, MAGIC_ITEM_FLAG_POTIONS);
	}

	if(pMonster->m_szTreasureType.Find("T") > -1 && bLair == TRUE) 
	{
		nMultiplier = FindTreasureTypeMultiplier(pMonster->m_szTreasureType, "Tx");

		int nRoll = RollD4()*2;
		RollMonsterMagicTreasure(pNPC, nMultiplier*nRoll, 50, MAGIC_ITEM_FLAG_SCROLLS);
	}

	if(pMonster->m_szTreasureType.Find("U") > -1 && bLair == TRUE) 
	{
		nMultiplier = FindTreasureTypeMultiplier(pMonster->m_szTreasureType, "Ux");

		RollMonsterJewelTreasure(pNPC, nMultiplier, 90, RollD8()*10, 100, 1000);
		RollMonsterJewelryTreasure(pNPC, nMultiplier, 80, RollD6()*5, 10, 1000);

		RollMonsterMagicTreasure(pNPC, nMultiplier, 70, MAGIC_ITEM_FLAG_ARMOR);
		RollMonsterMagicTreasure(pNPC, nMultiplier, 70, MAGIC_ITEM_FLAG_SWORDS);
		RollMonsterMagicTreasure(pNPC, nMultiplier, 70, MAGIC_ITEM_FLAG_RINGS);
		RollMonsterMagicTreasure(pNPC, nMultiplier, 70, MAGIC_ITEM_FLAG_MISC_WEAPONS);
		RollMonsterMagicTreasure(pNPC, nMultiplier, 70, MAGIC_ITEM_FLAG_RODS_STAVES_WANDS);
		RollMonsterMagicTreasure(pNPC, nMultiplier, 70, MAGIC_ITEM_FLAG_MISC_MAGIC_1);
		RollMonsterMagicTreasure(pNPC, nMultiplier, 70, MAGIC_ITEM_FLAG_MISC_MAGIC_2);
		RollMonsterMagicTreasure(pNPC, nMultiplier, 70, MAGIC_ITEM_FLAG_MISC_MAGIC_3);
		RollMonsterMagicTreasure(pNPC, nMultiplier, 70, MAGIC_ITEM_FLAG_MISC_MAGIC_4);
		RollMonsterMagicTreasure(pNPC, nMultiplier, 70, MAGIC_ITEM_FLAG_MISC_MAGIC_5);
	}

	if(pMonster->m_szTreasureType.Find("V") > -1 && bLair == TRUE) 
	{
		nMultiplier = FindTreasureTypeMultiplier(pMonster->m_szTreasureType, "Vx");

		RollMonsterMagicTreasure(pNPC, nMultiplier*2, 85, MAGIC_ITEM_FLAG_ARMOR);
		RollMonsterMagicTreasure(pNPC, nMultiplier*2, 85, MAGIC_ITEM_FLAG_SWORDS);
		RollMonsterMagicTreasure(pNPC, nMultiplier*2, 85, MAGIC_ITEM_FLAG_RINGS);
		RollMonsterMagicTreasure(pNPC, nMultiplier*2, 85, MAGIC_ITEM_FLAG_MISC_WEAPONS);
		RollMonsterMagicTreasure(pNPC, nMultiplier*2, 85, MAGIC_ITEM_FLAG_RODS_STAVES_WANDS);
		RollMonsterMagicTreasure(pNPC, nMultiplier*2, 85, MAGIC_ITEM_FLAG_MISC_MAGIC_1);
		RollMonsterMagicTreasure(pNPC, nMultiplier*2, 85, MAGIC_ITEM_FLAG_MISC_MAGIC_2);
		RollMonsterMagicTreasure(pNPC, nMultiplier*2, 85, MAGIC_ITEM_FLAG_MISC_MAGIC_3);
		RollMonsterMagicTreasure(pNPC, nMultiplier*2, 85, MAGIC_ITEM_FLAG_MISC_MAGIC_4);
		RollMonsterMagicTreasure(pNPC, nMultiplier*2, 85, MAGIC_ITEM_FLAG_MISC_MAGIC_5);
	}

	if(pMonster->m_szTreasureType.Find("W") > -1 && bLair == TRUE) 
	{
		nMultiplier = FindTreasureTypeMultiplier(pMonster->m_szTreasureType, "Wx");

		RollMonsterCoins(&pNPC->m_lGoldCarried, nMultiplier, 60, RollD6()*5, 1000);
		RollMonsterCoins(&pNPC->m_lPlatinumCarried, nMultiplier, 15, RollD8(), 100);
		
		RollMonsterJewelTreasure(pNPC, nMultiplier, 60, RollD8()*10, 100, 1000);
		RollMonsterJewelryTreasure(pNPC, nMultiplier, 50, RollD8()*5, 10, 1000);

		//need a map !
	}

	if(pMonster->m_szTreasureType.Find("X") > -1 && bLair == TRUE) 
	{
		nMultiplier = FindTreasureTypeMultiplier(pMonster->m_szTreasureType, "Xx");

		RollMonsterMagicTreasure(pNPC, nMultiplier, 60, MAGIC_ITEM_FLAG_MISC_MAGIC);
		RollMonsterMagicTreasure(pNPC, nMultiplier, 60, MAGIC_ITEM_FLAG_POTIONS);
	}

	if(pMonster->m_szTreasureType.Find("Y") > -1 && bLair == TRUE) 
	{
		nMultiplier = FindTreasureTypeMultiplier(pMonster->m_szTreasureType, "Yx");

		RollMonsterCoins(&pNPC->m_lGoldCarried, nMultiplier, 70, RollD6()*2, 1000);
	}

	if(pMonster->m_szTreasureType.Find("Z") > -1 && bLair == TRUE) 
	{
		nMultiplier = FindTreasureTypeMultiplier(pMonster->m_szTreasureType, "Zx");

		RollMonsterCoins(&pNPC->m_lCopperCarried, nMultiplier, 20, RollD3(), 1000);
		RollMonsterCoins(&pNPC->m_lSilverCarried, nMultiplier, 25, RollD4(), 1000);
		RollMonsterCoins(&pNPC->m_lElectrumCarried, nMultiplier, 25, RollD4(), 1000);
		RollMonsterCoins(&pNPC->m_lGoldCarried, nMultiplier, 30, RollD4(), 1000);
		RollMonsterCoins(&pNPC->m_lPlatinumCarried, nMultiplier, 30, RollD6(), 100);
		
		RollMonsterJewelTreasure(pNPC, nMultiplier, 55, RollD6()*10, 100, 1000);
		RollMonsterJewelryTreasure(pNPC, nMultiplier, 50, RollD6()*5, 10, 1000);

		RollMonsterMagicTreasure(pNPC, nMultiplier*3, 50, MAGIC_ITEM_FLAG_ANY);
	}

	//pNPC->StackInventory();
}

void RollNPCharacterEquipment(cDNDNonPlayerCharacter *pNPC)
{
	RollNPCharacterMagicEquipment(pNPC); 

	pNPC->StackInventory();
	NPCharacterEquipItems(pNPC);

	if(CharacterIsFighter(pNPC))
	{
		RollNPCharacterEquipment_Fighter(pNPC);
	}
	
	if(CharacterIsThief(pNPC))
	{
		RollNPCharacterEquipment_Thief(pNPC);
	}

	if(CharacterIsCleric(pNPC, FALSE))
	{
		RollNPCharacterEquipment_Cleric(pNPC);
	}

	pNPC->StackInventory();
	NPCharacterEquipItems(pNPC);

	RollNPCharacter_Helmets(pNPC);

	if(CharacterIsMage(pNPC))
	{
		RollNPCharacterEquipment_Mage(pNPC);
	}

	pNPC->StackInventory();

	NPCharacterEquipItems(pNPC);

	RollNPCharacterMundaneEquipment(pNPC);

	pNPC->StackInventory();

}

void RollNPCharacter_Helmets(cDNDNonPlayerCharacter *pNPC)
{
	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	//add a helmet if armored
	if(pNPC->m_ArmorWorn.m_wTypeId)
	{
		//char szDefaultHelmet[32] = {"helm\0"};
		switch(pNPC->m_ArmorWorn.m_nArmorClass)
		{
			case 10:
			case 9:
			{
				break;	//should never get in here
			}
			case 8:	//leather
			case 7:	//studded leather
			{
				pApp->AddItemToInventoryByName(pNPC, "helm,leather");
				break;
			}
			case 6:	// scale
			case 5: // chain
			{
				pApp->AddItemToInventoryByName(pNPC, "coif,chainmail");
				break;
			}
			default:
			{
				if(!pApp->AddItemToInventoryByName(pNPC, "helm,horned", NULL, 0, 10))
				{
					if(!pApp->AddItemToInventoryByName(pNPC, "helm,great", NULL, 0, 20))
					{
						if(!pApp->AddItemToInventoryByName(pNPC, "helm,open", NULL, 0, 10))
						{
							pApp->AddItemToInventoryByName(pNPC, "helm");
						}
					}
				}

				break;
			}
 		}
	}
}

void RollNPCharacterEquipment_Fighter(cDNDNonPlayerCharacter *pNPC)
{
	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	int nLevel = (pNPC->m_nLevel[0] + pNPC->m_nLevel[1]/2 + pNPC->m_nLevel[2]/2) - 1;
	nLevel = max(nLevel, 1);


	if(!pApp->AddItemToInventoryByName(pNPC, "plate mail", NULL, 0, nLevel * 10))
	{
		if(!pApp->AddItemToInventoryByName(pNPC, "chain mail", NULL, 0, nLevel * 10))
		{
			if(!pApp->AddItemToInventoryByName(pNPC, "scale mail", NULL, 0, nLevel * 10))
			{
				if(!pApp->AddItemToInventoryByName(pNPC, "ring mail", NULL, 0, nLevel * 10))
				{
					if(!pApp->AddItemToInventoryByName(pNPC, "splint mail", NULL, 0, nLevel * 10))
					{
						pApp->AddItemToInventoryByName(pNPC, "splint mail");
					}
				}
			}
		}
	}


	int nDefaultChance = 30;
	int nShortSwordChance = 10;
	int nBroadSwordChance = 20;
	int nBastardSwordChance = 20;
	int nTwoHandedSwordChance = 20;
	int nHammerChance = 20;
	char szDefaultWeapon[32] = {"sword, long\0"};

	int nLongBowChance = 30;
	int nShortBowChance = 30;
	int nHeavyCrossBowChance = 20;
	int nLightCrossBowChance = 20;

	BOOL bSmallShield = FALSE;
	BOOL bTwoHandsFree = TRUE;

	switch(pNPC->m_nRace)
	{
		case DND_CHARACTER_RACE_ELF:
		case DND_CHARACTER_RACE_HALF_ELF:
		{
			nDefaultChance = 80;
			nShortSwordChance = 10;
			nBroadSwordChance = 20;
			nBastardSwordChance = 0;
			nTwoHandedSwordChance = 0;
			nHammerChance = 0;

			nLongBowChance = 30;
			nShortBowChance = 30;
			nHeavyCrossBowChance = 0;
			nLightCrossBowChance = 0;

			break;
		}
		case DND_CHARACTER_RACE_DWARF:
		{
			strcpy(szDefaultWeapon, "axe, battle\0");
			nDefaultChance = 50;
			nShortSwordChance = 10;
			nBroadSwordChance = 20;
			nBastardSwordChance = 0;
			nTwoHandedSwordChance = 0;
			nHammerChance = 50;

			nLongBowChance = 0;
			nShortBowChance = 30;
			nHeavyCrossBowChance = 0;
			nLightCrossBowChance = 30;

			bSmallShield = TRUE;


			break;
		}
		case DND_CHARACTER_RACE_HALFLING:
		case DND_CHARACTER_RACE_GNOME:
		{
			strcpy(szDefaultWeapon, "sword, short\0");
			nDefaultChance = 50;
			nShortSwordChance = 10;
			nBroadSwordChance = 0;
			nBastardSwordChance = 0;
			nTwoHandedSwordChance = 0;
			nHammerChance = 0;

			nLongBowChance = 0;
			nShortBowChance = 30;
			nHeavyCrossBowChance = 0;
			nLightCrossBowChance = 30;

			bSmallShield = TRUE;

			break;
		}
	}

	if(!pApp->AddItemToInventoryByName(pNPC, szDefaultWeapon, NULL, 0, nDefaultChance))
	{
		if(!pApp->AddItemToInventoryByName(pNPC, "sword, short", NULL, 0, nShortSwordChance))
		{
			if(!pApp->AddItemToInventoryByName(pNPC, "sword, broad", NULL, 0, nBroadSwordChance))
			{
				if(!pApp->AddItemToInventoryByName(pNPC, "sword, bastard", NULL, 0, nBastardSwordChance))
				{
					if(!pApp->AddItemToInventoryByName(pNPC, "sword, two handed", NULL, 0, nTwoHandedSwordChance))
					{
						if(!pApp->AddItemToInventoryByName(pNPC, "hammer", NULL, 0, nHammerChance))
						{
							pApp->AddItemToInventoryByName(pNPC, szDefaultWeapon);
						}
					}
					else
					{
						bTwoHandsFree = FALSE;
					}
				}
			}
		}
	}


	int nAmount = (rand()%12)+12;

	if(pApp->AddItemToInventoryByName(pNPC, "bow, long", NULL, 0, nLongBowChance))
	{
		for(int i = 0; i < nAmount; ++i)
		{
			pApp->AddItemToInventoryByName(pNPC, "arrow");
		}

		bTwoHandsFree = FALSE;
	}
	else if(pApp->AddItemToInventoryByName(pNPC, "bow, short", NULL, 0, nShortBowChance))
	{
		for(int i = 0; i < nAmount; ++i)
		{
			pApp->AddItemToInventoryByName(pNPC, "arrow");
		}
	}
	else if(pApp->AddItemToInventoryByName(pNPC, "crossbow, light", NULL, 0, nLightCrossBowChance))
	{
		for(int i = 0; i < nAmount; ++i)
		{
			pApp->AddItemToInventoryByName(pNPC, "quarrel");
		}
	}
	else if(pApp->AddItemToInventoryByName(pNPC, "crossbow, heavy", NULL, 0, nHeavyCrossBowChance))
	{
		for(int i = 0; i < nAmount; ++i)
		{
			pApp->AddItemToInventoryByName(pNPC, "quarrel");
		}

		bTwoHandsFree = FALSE;
	}


	pApp->AddItemToInventoryByName(pNPC, "dagger", NULL, 0, 20);
	pApp->AddItemToInventoryByName(pNPC, "dagger", NULL, 0, 20);
	pApp->AddItemToInventoryByName(pNPC, "dagger", NULL, 0, 20);
	pApp->AddItemToInventoryByName(pNPC, "dagger", NULL, 0, 20);


	if(bTwoHandsFree == TRUE)
	{
		if(bSmallShield)
		{
			pApp->AddItemToInventoryByName(pNPC, "small shield", NULL, 0, 75);
		}
		else
		{
			pApp->AddItemToInventoryByName(pNPC, "large shield", NULL, 0, 75);
		}
	}

}

///////////////////////////////////////////////////////////////

void RollNPCharacterEquipment_Cleric(cDNDNonPlayerCharacter *pNPC)
{
	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	int nLevel = (pNPC->m_nLevel[0] + pNPC->m_nLevel[1]/2 + pNPC->m_nLevel[2]/2) - 1;
	nLevel = max(nLevel, 1);

	
	if(pNPC->m_Class[0] == DND_CHARACTER_CLASS_DRUID)
	{
		if(!pApp->AddItemToInventoryByName(pNPC, "leather", NULL, 0, 50))
		{
			if(!pApp->AddItemToInventoryByName(pNPC, "padded", NULL, 0, 50))
			{
				pApp->AddItemToInventoryByName(pNPC, "leather");
			}
		}
	}
	else
	{
		if(!pApp->AddItemToInventoryByName(pNPC, "plate mail", NULL, 0, nLevel * 10))
		{
			if(!pApp->AddItemToInventoryByName(pNPC, "chain mail", NULL, 0, nLevel * 10))
			{
				if(!pApp->AddItemToInventoryByName(pNPC, "scale mail", NULL, 0, nLevel * 10))
				{
					if(!pApp->AddItemToInventoryByName(pNPC, "ring mail", NULL, 0, nLevel * 10))
					{
						if(!pApp->AddItemToInventoryByName(pNPC, "splint mail", NULL, 0, nLevel * 10))
						{
							pApp->AddItemToInventoryByName(pNPC, "splint mail");
						}
					}
				}
			}
		}
	}

	int nDefaultChance = 20;
	char szDefaultWeapon[32] = {"mace,footman's\0"};

	if(pNPC->m_Class[0] == DND_CHARACTER_CLASS_DRUID)
	{
		nDefaultChance = 80;
		strcpy(szDefaultWeapon, "scimitar\0");

		pApp->AddItemToInventoryByName(pNPC, "symbol, holy, wooden");
	}
	else
	{
		if(!pApp->AddItemToInventoryByName(pNPC, "symbol, holy, iron", NULL, 0, 30))
		{
			if(!pApp->AddItemToInventoryByName(pNPC, "symbol, holy, silver", NULL, 0, 20))
			{
				pApp->AddItemToInventoryByName(pNPC, "symbol, holy, wooden");
			}
		}
	}

	if(!pApp->AddItemToInventoryByName(pNPC, szDefaultWeapon, NULL, 0, nDefaultChance))
	{
		if(!pApp->AddItemToInventoryByName(pNPC, "hammer", NULL, 0, 20))
		{
			if(!pApp->AddItemToInventoryByName(pNPC, "flail,footman's", NULL, 0, 20))
			{
				if(!pApp->AddItemToInventoryByName(pNPC, "club", NULL, 0, 20))
				{
					pApp->AddItemToInventoryByName(pNPC, szDefaultWeapon);	
				}
			}
		}
	}


	if(pNPC->m_Class[0] == DND_CHARACTER_CLASS_DRUID)
	{
		pApp->AddItemToInventoryByName(pNPC, "small wood shield", NULL, 0, 75);
	}
	else
	{
		pApp->AddItemToInventoryByName(pNPC, "small shield", NULL, 0, 75);
		pApp->AddItemToInventoryByName(pNPC, "large shield", NULL, 0, 75);
	}

}

void RollNPCharacterEquipment_Thief(cDNDNonPlayerCharacter *pNPC)
{
	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	int nLevel = (pNPC->m_nLevel[0] + pNPC->m_nLevel[1]/2 + pNPC->m_nLevel[2]/2) - 1;
	nLevel = max(nLevel, 1);


	if(!pApp->AddItemToInventoryByName(pNPC, "elfin mail", NULL, 0, nLevel))
	{
		if(!pApp->AddItemToInventoryByName(pNPC, "leather", NULL, 0, 30))
		{
			if(!pApp->AddItemToInventoryByName(pNPC, "padded", NULL, 0, 30))
			{
				if(!pApp->AddItemToInventoryByName(pNPC, "studded leather", NULL, 0, 10))
				{
					pApp->AddItemToInventoryByName(pNPC, "leather");	
				}
			}
		}
	}

	int nDefaultChance = 20;
	char szDefaultWeapon[32] = {"sword,short\0"};

	if(!pApp->AddItemToInventoryByName(pNPC, szDefaultWeapon, NULL, 0, nDefaultChance))
	{
		if(!pApp->AddItemToInventoryByName(pNPC, "sword,long", NULL, 0, 20))
		{
			if(!pApp->AddItemToInventoryByName(pNPC, "sword,broad", NULL, 0, 20))
			{
				if(!pApp->AddItemToInventoryByName(pNPC, "club", NULL, 0, 20))
				{
					pApp->AddItemToInventoryByName(pNPC, szDefaultWeapon);
				}
			}
		}
	}

	int nAmount = (rand()%12)+12;
	if(pApp->AddItemToInventoryByName(pNPC, "sling", NULL, 0, 30))
	{
		for(int i = 0; i < nAmount; ++i)
		{
			pApp->AddItemToInventoryByName(pNPC, "sling bullet");
		}
	}

	pApp->AddItemToInventoryByName(pNPC, "dagger", NULL, 0, 20);
	pApp->AddItemToInventoryByName(pNPC, "dagger", NULL, 0, 20);
	pApp->AddItemToInventoryByName(pNPC, "dagger", NULL, 0, 20);
	pApp->AddItemToInventoryByName(pNPC, "dagger", NULL, 0, 20);


	pApp->AddItemToInventoryByName(pNPC, "thieves' picks & tools", NULL, 0, 75);

}

void RollNPCharacterEquipment_Mage(cDNDNonPlayerCharacter *pNPC)
{
	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	int nLevel = (pNPC->m_nLevel[0] + pNPC->m_nLevel[1]/2 + pNPC->m_nLevel[2]/2) - 1;
	nLevel = max(nLevel, 1);

	int nDefaultChance = 50;
	char szDefaultWeapon[32] = {"staff,quarter\0"};

	if(!pApp->AddItemToInventoryByName(pNPC, szDefaultWeapon, NULL, 0, nDefaultChance))
	{
		if(!pApp->AddItemToInventoryByName(pNPC, "dagger", NULL, 0, 20))
		{
			if(!pApp->AddItemToInventoryByName(pNPC, "dart", NULL, 0, 20))
			{
				pApp->AddItemToInventoryByName(pNPC, szDefaultWeapon);
			}
		}
	}

	pApp->AddItemToInventoryByName(pNPC, "dagger", NULL, 0, 50);
	pApp->AddItemToInventoryByName(pNPC, "dagger", NULL, 0, 30);
	pApp->AddItemToInventoryByName(pNPC, "dagger", NULL, 0, 20);
	pApp->AddItemToInventoryByName(pNPC, "dagger", NULL, 0, 10);

	int nAmount = (rand()%5)+1;
	if(pApp->AddItemToInventoryByName(pNPC, "dart", NULL, 0, 30))
	{
		for(int i = 0; i < nAmount; ++i)
		{
			pApp->AddItemToInventoryByName(pNPC, "dart");
		}
	}

	pApp->AddItemToInventoryByName(pNPC, "thieves' picks & tools", "spellbook");

}

void RollNPCharacterMundaneEquipment(cDNDNonPlayerCharacter *pNPC)
{
	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	pApp->AddItemToInventoryByName(pNPC, "tinderbox, with flint & steel", NULL, 0, 50);

	int nUnderWearChance = 50;
	int nBoots = 6;

	switch(pNPC->m_nRace)
	{
		case DND_CHARACTER_RACE_HUMAN:
		{
			nUnderWearChance = 50;
			nBoots = 4;

			if(pNPC->m_nSex)
			{
				nBoots = 6;
			}

			if(pApp->AddItemToInventoryByName(pNPC, "lantern, bullseye", NULL, 0, 25))
			{
				pApp->AddItemToInventoryByName(pNPC, "oil, flask of");
			}
			else if(pApp->AddItemToInventoryByName(pNPC, "lantern, hooded", NULL, 0, 25))
			{
				pApp->AddItemToInventoryByName(pNPC, "oil, flask of");
			}
			else if(RollD100() < 50)
			{
				pApp->AddItemToInventoryByName(pNPC, "torch", NULL, 0, 100);
				pApp->AddItemToInventoryByName(pNPC, "torch", NULL, 0, 50);
				pApp->AddItemToInventoryByName(pNPC, "torch", NULL, 0, 50);
			}

			break;
		}
		case DND_CHARACTER_RACE_ELF:
		case DND_CHARACTER_RACE_HALF_ELF:
		{
			if(pNPC->m_nSex)
			{
				nUnderWearChance = 0;
			}

			break;
		}
		case DND_CHARACTER_RACE_HALF_ORC:
		{
			nUnderWearChance = 0;

			break;
		}
		case DND_CHARACTER_RACE_DWARF:
		{
			nUnderWearChance = 0;

			pApp->AddItemToInventoryByName(pNPC, "loincloth");

			break;
		}
		case DND_CHARACTER_RACE_HALFLING:
		{
			nBoots = -1;

			break;
		}
	}

	if(CharacterIsThief(pNPC) && nBoots > 0)
	{
		if(!pApp->AddItemToInventoryByName(pNPC, "boots, high, soft", NULL, 0, 50))
		{
			pApp->AddItemToInventoryByName(pNPC, "boots, low, soft");
		}

		nBoots = -1;
	}

	BOOL bClothes = TRUE;

	if(pNPC->m_ArmorWorn.m_wTypeId)
	{
		nBoots = min(nBoots, 4);
		bClothes = FALSE;
	}

	if(nBoots > 0)
	{
		int nBoot = rand()%nBoots;

		switch(nBoot)
		{
			case 0:	pApp->AddItemToInventoryByName(pNPC, "boots, high, hard"); break;
			case 1:	pApp->AddItemToInventoryByName(pNPC, "boots, high, soft"); break;
			case 2:	pApp->AddItemToInventoryByName(pNPC, "boots, low, hard"); break;
			case 3:	pApp->AddItemToInventoryByName(pNPC, "boots, low, soft"); break;
			case 4:	pApp->AddItemToInventoryByName(pNPC, "shoes"); break;
			case 5:	pApp->AddItemToInventoryByName(pNPC, "sandals"); break;
		}
	}


/*
breeches,			
jerkin,		
robe, common,	
blouse,			
dress
shirt,
trousers,	
skirt,	
tunic,	
hose/leggings,
vest, cloth,				
vest, fur,
cap
*/


	if(bClothes)
	{
		switch(pNPC->m_nSex)
		{
			case 0: //male
			{
				if(pApp->AddItemToInventoryByName(pNPC, "tunic", NULL, 0, 20))
				{
					pApp->AddItemToInventoryByName(pNPC, "hose/leggings");
				}
				else if(pApp->AddItemToInventoryByName(pNPC, "breeches", NULL, 0, 20))
				{
					pApp->AddItemToInventoryByName(pNPC, "jerkin");
				}
				else if(pApp->AddItemToInventoryByName(pNPC, "robe,common", NULL, 0, 20))
				{
					pApp->AddItemToInventoryByName(pNPC, "trousers", NULL, 0, 30);
				}
				else if(pApp->AddItemToInventoryByName(pNPC, "shirt"))
				{
					pApp->AddItemToInventoryByName(pNPC, "trousers");
				}

				break;
			}
			case 1: //female
			{
				if(pApp->AddItemToInventoryByName(pNPC, "tunic", NULL, 0, 20))
				{
					pApp->AddItemToInventoryByName(pNPC, "hose/leggings");
				}
				else if(pApp->AddItemToInventoryByName(pNPC, "skirt", NULL, 0, 10))
				{
					pApp->AddItemToInventoryByName(pNPC, "blouse");
				}
				else if(pApp->AddItemToInventoryByName(pNPC, "robe,common", NULL, 0, 20))
				{
					pApp->AddItemToInventoryByName(pNPC, "hose/leggings", NULL, 0, 30);
				}
				else if(pApp->AddItemToInventoryByName(pNPC, "trousers"))
				{
					pApp->AddItemToInventoryByName(pNPC, "blouse");
				}
				

				break;
			}
		}
	}


	pApp->AddItemToInventoryByName(pNPC, "cloak", NULL, 0, 20);
	pApp->AddItemToInventoryByName(pNPC, "backpack, leather", NULL, 0, 90);
	pApp->AddItemToInventoryByName(pNPC, "belt", NULL, 0, 90);

	if(!pApp->AddItemToInventoryByName(pNPC, "vest, cloth", NULL, 0, 20))
	{
		pApp->AddItemToInventoryByName(pNPC, "vest, fur", NULL, 0, 20);
	}
			
	pApp->AddItemToInventoryByName(pNPC, "smallclothes", NULL, 0, nUnderWearChance);

	//finally add some pocket change

	pNPC->m_lGoldCarried += pNPC->m_nLevel[0] * RollD100();
	pNPC->m_lSilverCarried += rand()%20;
	pNPC->m_lCopperCarried += rand()%10;

}

void RollNPCharacterMagicEquipment(cDNDNonPlayerCharacter *pNPC)
{
	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();


	int nChartChance[4] = {0, 0, 0, 0};
	int nChartCount[4] = {0, 0, 0, 0};

	//fudge it
	//nChartChance[3] = 100;
	//nChartCount[3] = 25;

	int nClassOverride = -1;		// no scrolls at -1
	int nClassSelect = DND_CHARACTER_CLASS_UNDEF;

	for(int i = 0; i < 3 && nClassOverride == -1; ++i)
	{
		switch(pNPC->m_Class[i])
		{
			case DND_CHARACTER_CLASS_CLERIC:		nClassSelect = DND_CHARACTER_CLASS_CLERIC;		nClassOverride = 1; break;
			case DND_CHARACTER_CLASS_DRUID:			nClassSelect = DND_CHARACTER_CLASS_DRUID;		nClassOverride = 2; break;
			case DND_CHARACTER_CLASS_ILLUSIONIST:	nClassSelect = DND_CHARACTER_CLASS_ILLUSIONIST;	nClassOverride = 3; break;
			case DND_CHARACTER_CLASS_MAGE:			nClassSelect = DND_CHARACTER_CLASS_MAGE;		nClassOverride = 4; break;

			case DND_CHARACTER_CLASS_FIGHTER:		nClassSelect = DND_CHARACTER_CLASS_FIGHTER;		break;
			case DND_CHARACTER_CLASS_RANGER:		nClassSelect = DND_CHARACTER_CLASS_FIGHTER;		break;
			case DND_CHARACTER_CLASS_CAVALIER:		nClassSelect = DND_CHARACTER_CLASS_FIGHTER;		break;
			case DND_CHARACTER_CLASS_PALADIN:		nClassSelect = DND_CHARACTER_CLASS_FIGHTER;		break;
			case DND_CHARACTER_CLASS_BARBARIAN:		nClassSelect = DND_CHARACTER_CLASS_FIGHTER;		break;

			case DND_CHARACTER_CLASS_THIEF:			nClassSelect = DND_CHARACTER_CLASS_THIEF;		break;
			case DND_CHARACTER_CLASS_THIEF_ACROBAT:	nClassSelect = DND_CHARACTER_CLASS_THIEF;		break;

			case DND_CHARACTER_CLASS_ASSASSIN:		nClassSelect = DND_CHARACTER_CLASS_ASSASSIN;	break;
		}
	}

	int nLevel = pNPC->m_nLevel[0] + pNPC->m_nLevel[1]/2 + pNPC->m_nLevel[2]/2;

	switch(nLevel)
	{
		//			CHART I										 CHART II									 CHART III								 CHART IV
		case 1:		nChartChance[0] = 10;	nChartCount[0] = 1;	 nChartChance[1] = 0;	nChartCount[1] = 0;  nChartChance[2] = 0;  nChartCount[2] = 0; nChartChance[3] =  0; nChartCount[3] = 0; break;
		case 2:		nChartChance[0] = 20;	nChartCount[0] = 2;	 nChartChance[1] = 0;	nChartCount[1] = 0;  nChartChance[2] = 0;  nChartCount[2] = 0; nChartChance[3] =  0; nChartCount[3] = 0; break;
		case 3:		nChartChance[0] = 30;	nChartCount[0] = 2;	 nChartChance[1] = 10;	nChartCount[1] = 1;  nChartChance[2] = 0;  nChartCount[2] = 0; nChartChance[3] =  0; nChartCount[3] = 0; break;
		case 4:		nChartChance[0] = 40;	nChartCount[0] = 2;  nChartChance[1] = 20;	nChartCount[1] = 1;  nChartChance[2] = 0;  nChartCount[2] = 0; nChartChance[3] =  0; nChartCount[3] = 0; break;
		case 5:		nChartChance[0] = 50;	nChartCount[0] = 2;  nChartChance[1] = 30;	nChartCount[1] = 1;  nChartChance[2] = 0;  nChartCount[2] = 0; nChartChance[3] =  0; nChartCount[3] = 0; break;
		case 6:		nChartChance[0] = 60;	nChartCount[0] = 3;  nChartChance[1] = 40;	nChartCount[1] = 2;  nChartChance[2] = 0;  nChartCount[2] = 0; nChartChance[3] =  0; nChartCount[3] = 0; break;
		case 7:		nChartChance[0] = 70;	nChartCount[0] = 3;  nChartChance[1] = 50;	nChartCount[1] = 2;  nChartChance[2] = 10; nChartCount[2] = 1; nChartChance[3] =  0; nChartCount[3] = 0; break;
		case 8:		nChartChance[0] = 80;	nChartCount[0] = 3;  nChartChance[1] = 60;	nChartCount[1] = 2;  nChartChance[2] = 20; nChartCount[2] = 1; nChartChance[3] =  0; nChartCount[3] = 0; break;
		case 9:		nChartChance[0] = 90;	nChartCount[0] = 3;  nChartChance[1] = 70;	nChartCount[1] = 2;  nChartChance[2] = 30; nChartCount[2] = 1; nChartChance[3] =  0; nChartCount[3] = 0; break;
		case 10:	nChartChance[0] = 100;	nChartCount[0] = 3;  nChartChance[1] = 80;	nChartCount[1] = 2;  nChartChance[2] = 40; nChartCount[2] = 1; nChartChance[3] =  0; nChartCount[3] = 0; break;
		case 11:	nChartChance[0] = 100;	nChartCount[0] = 3;  nChartChance[1] = 90;	nChartCount[1] = 2;  nChartChance[2] = 50; nChartCount[2] = 1; nChartChance[3] =  10; nChartCount[3] = 1; break;
		case 12:	nChartChance[0] = 100;	nChartCount[0] = 3;  nChartChance[1] = 100;	nChartCount[1] = 2;  nChartChance[2] = 60; nChartCount[2] = 1; nChartChance[3] =  20; nChartCount[3] = 1; break;
		
		default:
		{
			if(nLevel >= 13)
			{
				nChartChance[0] = 100; nChartCount[0] = 3;
				nChartChance[1] = 100; nChartCount[1] = 2;
				nChartChance[2] = 100; nChartCount[2] = 1;
				nChartChance[3] =  30; nChartCount[3] = 1;
			}
			else
			{
				return;
			}	

			break;
		}
	}

	

	// CHART 1
	int nChart = 0;
	int nDieRoll = RollD100();

	if(nDieRoll <= nChartChance[nChart])
	{
		for(int i = 0; i < nChartCount[nChart]; ++i)
		{
			BOOL bReroll = FALSE;
			int nLoops = 0;

			do
			{
				bReroll = FALSE;

				nDieRoll = RollD20();

				if(nDieRoll >= 8 && nDieRoll <= 10 && nClassOverride == -1)
				{
					bReroll = TRUE;
				}

				if(nClassSelect == DND_CHARACTER_CLASS_THIEF && nDieRoll >= 16)
				{
					bReroll = TRUE;
				}

				if(nClassSelect != DND_CHARACTER_CLASS_FIGHTER && (nDieRoll == 16 || nDieRoll == 17))
				{
					bReroll = TRUE;
				}
				
				++nLoops;

			} while(bReroll == TRUE && nLoops < 1000);

			if(bReroll == TRUE && nLoops >= 1000)
			{
				nDieRoll = 0;
			}

			switch(nDieRoll)
			{
				case 1:		pApp->AddItemToInventoryByName(pNPC, "potion of climbing");				pApp->AddItemToInventoryByName(pNPC, "potion of flying");			break;
				case 2:		pApp->AddItemToInventoryByName(pNPC, "potion of extra-healing");		pApp->AddItemToInventoryByName(pNPC, "potion of polymorph (self)"); break;
				case 3:		pApp->AddItemToInventoryByName(pNPC, "potion of fire resistance");		pApp->AddItemToInventoryByName(pNPC, "potion of speed");			break;
				case 4:		pApp->AddItemToInventoryByName(pNPC, "potion of healing");				pApp->AddItemToInventoryByName(pNPC, "potion of giant strength");	break;
				case 5:		pApp->AddItemToInventoryByName(pNPC, "potion of heroism");				pApp->AddItemToInventoryByName(pNPC, "potion of invulnerability");	break;
				case 6:		pApp->AddItemToInventoryByName(pNPC, "potion of human control");		pApp->AddItemToInventoryByName(pNPC, "potion of levitation");		break;
				case 7:		pApp->AddItemToInventoryByName(pNPC, "potion of super-heroism");		pApp->AddItemToInventoryByName(pNPC, "potion of animal control");	break;

				case 8:		pApp->AddItemToInventoryByName(pNPC, "Scroll : 1 spell, Levels 1-6",	NULL, 0, -99, nClassOverride); break;	
				case 9:		pApp->AddItemToInventoryByName(pNPC, "Scroll : 2 spells, Levels 1-4",	NULL, 0, -99, nClassOverride); break;	
				case 10:	pApp->AddItemToInventoryByName(pNPC, "Scroll : Protection-Magic",		NULL, 0, -99, nClassOverride); break;	

				case 11:	pApp->AddItemToInventoryByName(pNPC, "Ring of Mammal Control");			break;
				case 12:	pApp->AddItemToInventoryByName(pNPC, "Ring of Protection", NULL, 1);	break;
				case 13:	pApp->AddItemToInventoryByName(pNPC, "leather armor +1");				break;
				case 14:	pApp->AddItemToInventoryByName(pNPC, "shield +1");						break;
				case 15:	pApp->AddItemToInventoryByName(pNPC, "sword +1");						break;
				case 16:	pApp->AddItemToInventoryByName(pNPC, "Arrow +1, 2-24 in number");		pApp->AddItemToInventoryByName(pNPC, "bow, short");			break;
				case 17:	pApp->AddItemToInventoryByName(pNPC, "Bolt +2, 2-20 in number");		pApp->AddItemToInventoryByName(pNPC, "crossbow, light");	break;
				case 18:	pApp->AddItemToInventoryByName(pNPC, "dagger", NULL, 1);				break;
				case 19:	pApp->AddItemToInventoryByName(pNPC, "javelin +2");						break;
				case 20:	pApp->AddItemToInventoryByName(pNPC, "mace +1");						break;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// CHART 2
	nChart = 1;
	nDieRoll = RollD100();

	if(nDieRoll <= nChartChance[nChart])
	{
		for(int i = 0; i < nChartCount[nChart]; ++i)
		{
			BOOL bReroll = FALSE;
			int nLoops = 0;

			do
			{
				bReroll = FALSE;

				nDieRoll = nRollDie(16);

				if((nDieRoll == 1 || (nDieRoll >= 4 && nDieRoll <= 8) || nDieRoll == 11 ) && nClassOverride == -1)
				{
					bReroll = TRUE;
				}

				if(nClassSelect != DND_CHARACTER_CLASS_FIGHTER && nDieRoll == 12)
				{
					bReroll = TRUE;
				}

				++nLoops;

			} while(bReroll == TRUE && nLoops < 1000);

			if(bReroll == TRUE && nLoops >= 1000)
			{
				nDieRoll = 0;
			}

			switch(nDieRoll)
			{
				case 1:		pApp->AddItemToInventoryByName(pNPC, "Scroll : 3 spells, Levels 2-9",	NULL, 0, -99, nClassOverride); break;	

				case 2:		pApp->AddItemToInventoryByName(pNPC, "Ring of Fire Resistance");		pApp->AddItemToInventoryByName(pNPC, "Ring of Invisibility");		break;	
				case 3:		pApp->AddItemToInventoryByName(pNPC, "Ring of Protection", NULL, 3);	break;
				case 4:		pApp->AddItemToInventoryByName(pNPC, "Staff of Striking (C,M)");		break;
				case 5:		pApp->AddItemToInventoryByName(pNPC, "Wand of Illusion (M)");			break;
				case 6:		pApp->AddItemToInventoryByName(pNPC, "Wand of Negation (any)");			break;
				case 7:		pApp->AddItemToInventoryByName(pNPC, "bracers of defense");				break;
				case 8:		pApp->AddItemToInventoryByName(pNPC, "brooch of shielding");			break;
				case 9:		pApp->AddItemToInventoryByName(pNPC, "cloak of elvenkind");				break;
				case 10:	pApp->AddItemToInventoryByName(pNPC, "dust of appearance");				break;
				case 11:	pApp->AddItemToInventoryByName(pNPC, "Figurine of Wondrous Power");		break;
				case 12:	pApp->AddItemToInventoryByName(pNPC, "Javelin of Lightning (F)");		pApp->AddItemToInventoryByName(pNPC, "Javelin of Lightning (F)");	pApp->AddItemToInventoryByName(pNPC, "Javelin of Lightning (F)"); break;
				case 13:	pApp->AddItemToInventoryByName(pNPC, "chain mail +1");					pApp->AddItemToInventoryByName(pNPC, "shield +2");					break;
				case 14:	pApp->AddItemToInventoryByName(pNPC, "splint mail +4");					break;
				case 15:	pApp->AddItemToInventoryByName(pNPC, "sword +3");						break;				
				case 16:	pApp->AddItemToInventoryByName(pNPC, "crossbow of speed");				pApp->AddItemToInventoryByName(pNPC, "hammer +2");					break;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// CHART 3
	nChart = 2;
	nDieRoll = RollD100();

	if(nDieRoll <= nChartChance[nChart])
	{
		for(int i = 0; i < nChartCount[nChart]; ++i)
		{
			BOOL bReroll = FALSE;
			int nLoops = 0;

			do
			{
				bReroll = FALSE;

				nDieRoll = nRollDie(16);

				if( ( (nDieRoll >= 1 && nDieRoll <= 4) || nDieRoll == 10) && nClassOverride == -1)
				{
					bReroll = TRUE;			
				}

				++nLoops;

			} while(bReroll == TRUE && nLoops < 1000);

			if(bReroll == TRUE && nLoops >= 1000)
			{
				nDieRoll = 0;
			}

			switch(nDieRoll)
			{
				case 1:		pApp->AddItemToInventoryByName(pNPC, "Ring of Spell Storing");			break;
				case 2:		pApp->AddItemToInventoryByName(pNPC, "Rod of Cancellation (any)");		break;
				case 3:		pApp->AddItemToInventoryByName(pNPC, "Staff of the Serpent (C)");		break;
				case 4:		pApp->AddItemToInventoryByName(pNPC, "bag of tricks");					break;

				case 5:		pApp->AddItemToInventoryByName(pNPC, "boots of speed");					break;
				case 6:		pApp->AddItemToInventoryByName(pNPC, "boots of striding and springing");break;
				case 7:		pApp->AddItemToInventoryByName(pNPC, "cloak of displacement");			break;
				case 8:		pApp->AddItemToInventoryByName(pNPC, "gauntlets of ogre power");		break;
				case 9:		pApp->AddItemToInventoryByName(pNPC, "pipes of the sewers");			break;
				case 10:	pApp->AddItemToInventoryByName(pNPC, "robe of blending");				break;
				case 11:	pApp->AddItemToInventoryByName(pNPC, "Rope of Climbing");				pApp->AddItemToInventoryByName(pNPC, "Rope of Entanglement");	break;
				case 12:	pApp->AddItemToInventoryByName(pNPC, "plate mail +3");					pApp->AddItemToInventoryByName(pNPC, "shield +2");				break;
				case 13:	pApp->AddItemToInventoryByName(pNPC, "shield +5");						break;
				case 14:	pApp->AddItemToInventoryByName(pNPC, "Sword +4, Defender");				break;
				case 15:	pApp->AddItemToInventoryByName(pNPC, "mace,footman's", "mace", 3);		break;
				case 16:	pApp->AddItemToInventoryByName(pNPC, "spear +3");						break;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// CHART 4
	nChart = 3;
	nDieRoll = RollD100();

	if(nDieRoll <= nChartChance[nChart])
	{
		for(int i = 0; i < nChartCount[nChart]; ++i)
		{
			BOOL bReroll = FALSE;
			int nLoops = 0;

			do
			{
				bReroll = FALSE;

				nDieRoll = nRollDie(13);

				switch(nDieRoll)
				{
					case 3:		if(nClassSelect != DND_CHARACTER_CLASS_CLERIC && nClassSelect != DND_CHARACTER_CLASS_FIGHTER)	bReroll = TRUE; break;
					case 4:		if(nClassSelect != DND_CHARACTER_CLASS_MAGE)													bReroll = TRUE; break;
					case 6:		if(nClassSelect != DND_CHARACTER_CLASS_MAGE)													bReroll = TRUE; break;
					case 8:		if(nClassSelect != DND_CHARACTER_CLASS_CLERIC)													bReroll = TRUE; break;
					case 9:		if(nClassSelect != DND_CHARACTER_CLASS_CLERIC)													bReroll = TRUE; break;
					case 11:	if(nClassSelect != DND_CHARACTER_CLASS_CLERIC && nClassSelect != DND_CHARACTER_CLASS_FIGHTER)	bReroll = TRUE; break;
					case 12:	if(nClassSelect != DND_CHARACTER_CLASS_THIEF && nClassSelect != DND_CHARACTER_CLASS_FIGHTER)	bReroll = TRUE; break;
					case 13:	if(nClassSelect != DND_CHARACTER_CLASS_FIGHTER)													bReroll = TRUE; break;
				}

				++nLoops;

			} while(bReroll == TRUE && nLoops < 1000);

			if(bReroll == TRUE && nLoops >= 1000)
			{
				nDieRoll = 0;
			}


			switch(nDieRoll)
			{
				case 1:		pApp->AddItemToInventoryByName(pNPC, "Ring of Djinni Summoning");		break;
				case 2:		pApp->AddItemToInventoryByName(pNPC, "Ring of Spell Turning");			break;
				case 3:		pApp->AddItemToInventoryByName(pNPC, "Rod of Smiting (C,F)");			break;
				case 4:		pApp->AddItemToInventoryByName(pNPC, "Wand of Fire (M)");				break;
				case 5:		pApp->AddItemToInventoryByName(pNPC, "Cube of Force");					break;
				case 6:		pApp->AddItemToInventoryByName(pNPC, "Eyes of Charming (M)");			break;
				case 7:		pApp->AddItemToInventoryByName(pNPC, "Horn of Valhalla");				break;
				case 8:		pApp->AddItemToInventoryByName(pNPC, "Robe of Scintillating Colors");	break;
				case 9:		pApp->AddItemToInventoryByName(pNPC, "Talisman of Pure Good (C)");		break;
				case 10:	pApp->AddItemToInventoryByName(pNPC, "Talisman of Ultimate Evil (C)");	break;
				case 11:	pApp->AddItemToInventoryByName(pNPC, "plate mail +4");					pApp->AddItemToInventoryByName(pNPC, "shield +3");				break;
				case 12:	pApp->AddItemToInventoryByName(pNPC, "Sword of Wounding");				break;
				case 13:	pApp->AddItemToInventoryByName(pNPC, "Arrow of Slaying");				break;
			
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

}

void NPCharacterEquipItems(cDNDNonPlayerCharacter *pNPC)
{
	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	//first do armor and shields and helmets
	for(int i = 0; i < MAX_CHARACTER_INVENTORY && pNPC->m_Inventory[i].m_dwObjectID; ++i)
	{
		cDNDObject *pInvItem = &pNPC->m_Inventory[i];

		if(pInvItem != NULL)
		{
			cDNDArmor *pArmor = NULL;
			pApp->m_ArmorIndexedTypeMap.Lookup(pInvItem->m_wTypeId, pArmor);
			if(pArmor != NULL)
			{
				BOOL bSwap = FALSE;
				BOOL bDrop = FALSE;

				if(CharacterIsMage(pNPC) && !CharacterIsFighter(pNPC)) //might be multiclassed
				{
					bDrop = TRUE;
				}
				else if(CharacterIsThief(pNPC) && pArmor->IsMetalArmor())
				{
					bDrop = TRUE;
				}

				if(bDrop == FALSE)
				{
					if(pNPC->m_ArmorWorn.m_wTypeId == 0)
					{
						bSwap = TRUE;
					}

					if(!bSwap)
					{
						int nAC1 = pArmor->m_nArmorClass - pArmor->m_nMagicAdj;
						int nAC2 = pNPC->m_ArmorWorn.m_nArmorClass - pNPC->m_ArmorWorn.m_nMagicAdj;

						if(nAC1 < nAC2)
						{
							bSwap = TRUE;
						}
					}
				}

				if(bDrop)
				{
					pInvItem->m_wTypeId = 0;
				}
				else if(bSwap)
				{
					pArmor->CopyTo(&pNPC->m_ArmorWorn);
					pNPC->m_ArmorWorn.m_dwObjectID = pInvItem->m_dwObjectID;
					pNPC->m_ArmorWorn.m_nMagicAdj = pInvItem->m_nMagicAdj;

					pInvItem->m_dwContainerID = 0;
				}

			}
			/////////////////////////////////////////
			
			cDNDShield *pShield = NULL;
			if(pInvItem->m_wTypeId) //might have been dropped above
			{
				pApp->m_ShieldsIndexedTypeMap.Lookup(pInvItem->m_wTypeId, pShield);
			}
			if(pShield != NULL)
			{
				BOOL bSwap = FALSE;
				BOOL bDrop = FALSE;

				if(CharacterIsFighter(pNPC) || CharacterIsCleric(pNPC, TRUE))
				{
					bDrop = FALSE;
				}
				else 
				{
					bDrop = TRUE;
				}

				if(!bDrop)
				{
					if(pNPC->m_ShieldWorn.m_wTypeId == 0)
					{
						bSwap = TRUE;
					}

					if(!bSwap)
					{
						if(pShield->m_nMagicAdj > pNPC->m_ShieldWorn.m_nMagicAdj)
						{
							bSwap = TRUE;
						}
					}
				}

				if(bDrop)
				{
					pInvItem->m_wTypeId = 0;
				}
				else if(bSwap)
				{
					pShield->CopyTo(&pNPC->m_ShieldWorn);
					pNPC->m_ShieldWorn.m_dwObjectID = pInvItem->m_dwObjectID;
					pNPC->m_ShieldWorn.m_nMagicAdj = pInvItem->m_nMagicAdj;

					pInvItem->m_dwContainerID = 0;
				}
			}

			/////////////////////////////////////////
			
			cDNDObject *pHelmet = NULL;
			if(pInvItem->m_wTypeId && pInvItem->m_wTypeId < pApp->m_HelmetsIndexedTypeArray.GetCount()) //might have been dropped above
			{
				pHelmet = pApp->m_HelmetsIndexedTypeArray.GetAt(pInvItem->m_wTypeId);
			}
			if(pHelmet != NULL)
			{
				BOOL bSwap = FALSE;
				BOOL bDrop = FALSE;

				if(CharacterIsFighter(pNPC) || CharacterIsCleric(pNPC, TRUE) || CharacterIsThief(pNPC))
				{
					bDrop = FALSE;
				}
				else 
				{
					bDrop = TRUE;
				}

				if(!bDrop)
				{
					if(pNPC->m_HelmWorn.m_wTypeId == 0)
					{
						bSwap = TRUE;
					}

					if(!bSwap)
					{
						if(pHelmet->m_nMagicAdj > pNPC->m_HelmWorn.m_nMagicAdj)
						{
							bSwap = TRUE;
						}
					}
				}

				if(bDrop)
				{
					pInvItem->m_wTypeId = 0;
				}
				else if(bSwap)
				{
					pHelmet->CopyTo(&pNPC->m_HelmWorn);
					pNPC->m_HelmWorn.m_dwObjectID = pInvItem->m_dwObjectID;
					pNPC->m_HelmWorn.m_nMagicAdj = pInvItem->m_nMagicAdj;

					pInvItem->m_dwContainerID = 0;
				}
			}	
			
		}
	}

	pNPC->ValidateInventory();

	// do any rings that are worn
	for(int i = 0; i < MAX_CHARACTER_INVENTORY && pNPC->m_Inventory[i].m_dwObjectID; ++i)
	{
		cDNDObject *pInvItem = &pNPC->m_Inventory[i];

		if(pInvItem != NULL)
		{
			cDNDRing *pRing = NULL;
			pApp->m_RingsIndexedTypeMap.Lookup(pInvItem->m_wTypeId, pRing);
			if(pRing != NULL)
			{
				BOOL bSwap = FALSE;

				if(pNPC->m_RingsWorn[0].m_wTypeId == 0)
				{
					bSwap = TRUE;
				}

				if(!bSwap)
				{
					if(pRing->m_nMagicAdj > pNPC->m_RingsWorn[0].m_nMagicAdj)
					{
						bSwap = TRUE;
					}
				}

				if(bSwap)
				{
					pRing->CopyTo(&pNPC->m_RingsWorn[0]);
					pNPC->m_RingsWorn[0].m_dwObjectID = pInvItem->m_dwObjectID;
					pNPC->m_RingsWorn[0].m_nMagicAdj = pInvItem->m_nMagicAdj;

					pInvItem->m_dwContainerID = 0;
				}
				
			}
		}
	}
	
	// do any second rings that are worn
	for(int i = 0; i < MAX_CHARACTER_INVENTORY && pNPC->m_Inventory[i].m_dwObjectID; ++i)
	{
		cDNDObject *pInvItem = &pNPC->m_Inventory[i];

		if(pInvItem != NULL)
		{
			if(pInvItem->m_dwObjectID == pNPC->m_RingsWorn[0].m_dwObjectID)
				continue;	//we already have this ring on on the other hand

			cDNDRing *pRing = NULL;
			pApp->m_RingsIndexedTypeMap.Lookup(pInvItem->m_wTypeId, pRing);
			if(pRing != NULL)
			{
				BOOL bSwap = FALSE;

				if(pNPC->m_RingsWorn[1].m_wTypeId == 0)
				{
					bSwap = TRUE;
				}

				if(!bSwap)
				{
					if(pRing->m_nMagicAdj > pNPC->m_RingsWorn[1].m_nMagicAdj)
					{
						bSwap = TRUE;
					}
				}

				if(bSwap)
				{
					pRing->CopyTo(&pNPC->m_RingsWorn[1]);
					pNPC->m_RingsWorn[1].m_dwObjectID = pInvItem->m_dwObjectID;
					pNPC->m_RingsWorn[1].m_nMagicAdj = pInvItem->m_nMagicAdj;

					pInvItem->m_dwContainerID = 0;
				}
				
			}
		}
	}
	
	// equip primary weapon
	for(int i = 0; i < MAX_CHARACTER_INVENTORY && pNPC->m_Inventory[i].m_dwObjectID; ++i)
	{
		cDNDObject *pInvItem = &pNPC->m_Inventory[i];

		if(pInvItem != NULL)
		{
			int nCount = pApp->m_WeaponsIndexedTypeArray.GetCount();
			if(pInvItem->m_wTypeId > nCount)
				continue;

			cDNDWeapon *pWeapon = pApp->m_WeaponsIndexedTypeArray.GetAt(pInvItem->m_wTypeId);

			if(pWeapon != NULL)
			{
				if(IsAmmo(pWeapon))
					continue;

				BOOL bSwap = FALSE;
				BOOL bDrop = FALSE;

				if(pNPC->m_SelectedWeapons[0].m_wTypeId == 0)
				{
					bSwap = TRUE;
				}

				if(pInvItem->IsWeaponUsableByClass(pNPC->m_Class[0]) == FALSE && pInvItem->IsWeaponUsableByClass(pNPC->m_Class[1]) == FALSE && pInvItem->IsWeaponUsableByClass(pNPC->m_Class[2]) == FALSE)
				{
					bDrop = TRUE;
				}

				if(!bSwap && !bDrop)
				{
					int nDamage1 = pWeapon->GetMaximumDamage() + pWeapon->m_nMagicAdj;
					int nDamage2 = pNPC->m_SelectedWeapons[0].GetMaximumDamage() + pNPC->m_SelectedWeapons[0].m_nMagicAdj;

					if(nDamage1 > nDamage2)
					{
						bSwap = TRUE;
					}
				}

				if(bDrop)
				{
					pInvItem->m_wTypeId = 0;
				}
				else if(bSwap)
				{
					pWeapon->CopyTo(&pNPC->m_SelectedWeapons[0]);
					pNPC->m_SelectedWeapons[0].m_dwObjectID = pInvItem->m_dwObjectID;
					pNPC->m_SelectedWeapons[0].m_nMagicAdj = pInvItem->m_nMagicAdj;

					pInvItem->m_dwContainerID = 0;

					pNPC->AddWeaponProficiency(pWeapon);

					if(IsMissileWeapon(pWeapon))
					{
						GenerateAmmoForWeapons(pNPC, pWeapon);
					}
				}
				
			}
		}
	}

	// equip secondary weapon
	for(int i = 0; i < MAX_CHARACTER_INVENTORY && pNPC->m_Inventory[i].m_dwObjectID; ++i)
	{
		if(pNPC->m_Inventory[i].m_dwObjectID && pNPC->m_Inventory[i].m_dwObjectID == pNPC->m_SelectedWeapons[0].m_dwObjectID)
		{
			continue; // we have already equipped this weapon
		}

		cDNDObject *pInvItem = &pNPC->m_Inventory[i];

		if(pInvItem != NULL)
		{
			int nCount = pApp->m_WeaponsIndexedTypeArray.GetCount();
			if(pInvItem->m_wTypeId > nCount)
				continue;

			cDNDWeapon *pWeapon = pApp->m_WeaponsIndexedTypeArray.GetAt(pInvItem->m_wTypeId);

			if(pWeapon != NULL)
			{
				if(IsAmmo(pWeapon))
					continue;

				BOOL bSwap = FALSE;

				if(pNPC->m_SelectedWeapons[1].m_wTypeId == 0)
				{
					bSwap = TRUE;
				}

				if(!bSwap)
				{
					int nDamage1 = pWeapon->GetMaximumDamage() + pWeapon->m_nMagicAdj;
					int nDamage2 = pNPC->m_SelectedWeapons[1].GetMaximumDamage() + pNPC->m_SelectedWeapons[1].m_nMagicAdj;

					if(nDamage1 > nDamage2)
					{
						bSwap = TRUE;
					}
				}

				if(bSwap)
				{
					pWeapon->CopyTo(&pNPC->m_SelectedWeapons[1]);
					pNPC->m_SelectedWeapons[1].m_dwObjectID = pInvItem->m_dwObjectID;
					pNPC->m_SelectedWeapons[1].m_nMagicAdj = pInvItem->m_nMagicAdj;

					pInvItem->m_dwContainerID = 0;

					pNPC->AddWeaponProficiency(pWeapon);

					if(IsMissileWeapon(pWeapon))
					{
						GenerateAmmoForWeapons(pNPC, pWeapon);
					}
				}
				
			}
		}
	}

	pNPC->ValidateProficiencies();
	pNPC->ValidateInventory();

	NPCharacterDropUnusedItems(pNPC);
	
}

void NPCharacterDropUnusedItems(cDNDNonPlayerCharacter *pNPC)
{
	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	//first do armor and shields
	for(int i = 0; i < MAX_CHARACTER_INVENTORY && pNPC->m_Inventory[i].m_dwObjectID; ++i)
	{
		cDNDObject *pInvItem = &pNPC->m_Inventory[i];

		if(pInvItem->m_dwObjectID == pNPC->m_ArmorWorn.m_dwObjectID)
			continue;

		if(pInvItem->m_dwObjectID == pNPC->m_ShieldWorn.m_dwObjectID)
			continue;

		if(pInvItem != NULL)
		{
			cDNDArmor *pArmor = NULL;
			pApp->m_ArmorIndexedTypeMap.Lookup(pInvItem->m_wTypeId, pArmor);
			if(pArmor != NULL)
			{
				pInvItem->m_wTypeId = 0;
			}
		}

		/////////////////////////////////////////
			
		cDNDShield *pShield = NULL;
		if(pInvItem->m_wTypeId) //might have been dropped above
		{
			pApp->m_ShieldsIndexedTypeMap.Lookup(pInvItem->m_wTypeId, pShield);
		}
		if(pShield != NULL)
		{
			pInvItem->m_wTypeId = 0;
		}
	}

	pNPC->StackInventory();
	pNPC->ValidateInventory();

	for(int i = 0; i < MAX_CHARACTER_INVENTORY && pNPC->m_Inventory[i].m_dwObjectID; ++i)
	{
		cDNDObject *pInvItem1 = &pNPC->m_Inventory[i];
		for(int j = 0; j < MAX_CHARACTER_INVENTORY && pNPC->m_Inventory[j].m_dwObjectID; ++j)
		{
			if(i == j)
			{
				continue;
			}

			cDNDObject *pInvItem2 = &pNPC->m_Inventory[j];

			if(pInvItem2->m_wTypeId == pInvItem1->m_wTypeId && pInvItem2->m_nMagicAdj == 0)
			{
				pInvItem2->m_wTypeId = 0;
			}
		}
	}

	pNPC->ValidateInventory();

	pNPC->LockAndLoad();
}

void GenerateAmmoForWeapons(cDNDCharacter *pCharacter, PWEAPONTYPE pWeapon)
{
	CString szType = _T("");

	int nFlag = (pWeapon->m_nFlags / 10000) % 10;

	//check to see if we don't already have some
	for(int i = 0; i< MAX_CHARACTER_INVENTORY; ++i)
	{
		if(pCharacter->m_Inventory[i].m_wTypeId != 0 && pCharacter->m_Inventory[i].m_nMagicAdj == 0)
		{
			int nAmmoFlag = (pCharacter->m_Inventory[i].m_nFlags / 1000)%10;

			if(nAmmoFlag == nFlag)
			{
				return;  // adios, we are done here
			}
		}
	}

	switch(nFlag)
	{
		case 1:
		{
			szType = _T("arrow");
			break;
		}
		case 2:
		{
			szType = _T("quarrel");
			break;
		}
		case 3:
		{
			szType = _T("sling bullet");
			break;
		}
		default:
		{
			break;
		}
	}

	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	int nAmount = (rand()%23)+2;
	for(int i = 0; i < nAmount; ++i)
	{
		pApp->AddItemToInventoryByName((cDNDNonPlayerCharacter *)pCharacter, szType.GetBuffer(0));
	}

}

void NPCharacterGenerateSpellBook(cDNDCharacter *pCharacter, int nClassIndex, cDNDSpellBook *pSpellBook)
{
	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	int nKnowSpell = 0;
	int nMinSpells = 0;
	int nMaxSpells = 0;

	int nAddLang = CalculateINTAdjustments(pCharacter, &nKnowSpell, &nMinSpells, &nMaxSpells);

	for(int j = 1; j <= min(pCharacter->m_nCastingLevels[nClassIndex], 9); ++ j)	
	{
		int nSpells = pSpellBook->m_nSpellsPerLevel[j];
		int nSpellsAdded = 0;

		if(nSpells)
		{
			int nLoops = 0;
			do
			{
				int nSpell = rand() % nSpells;
				int nDieRoll = RollD100();

				if(pCharacter->m_nSpellBooks[nClassIndex][j][nSpell] == 0 && nDieRoll <= nKnowSpell)
				{
					pCharacter->m_nSpellBooks[nClassIndex][j][nSpell] = 1;
					++nSpellsAdded;
				}

				++nLoops;

			} while(nSpellsAdded < nMaxSpells && nSpellsAdded < nSpells && nLoops < 100);  
		}

	}
}

void NPCharacterGenerateSpells(cDNDCharacter *pCharacter)
{
	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	for(int i = 0; i < 4; ++i)
	{
		BOOL bNeedsSpellBook = FALSE;

		if(pCharacter->m_SpellClasses[i] != DND_CHARACTER_CLASS_UNDEF)
		{
			cDNDSpellBook *pSpellBook = NULL;

			switch(pCharacter->m_SpellClasses[i])
			{
				case DND_CHARACTER_CLASS_MAGE:
				case DND_CHARACTER_SPELL_CLASS_RANGER_MAGE:
				{
					//this character needs a spellbook
					pSpellBook = pApp->m_SpellBooks.GetAt(DND_CHARACTER_CLASS_MAGE);
					NPCharacterGenerateSpellBook(pCharacter, i, pSpellBook);

					bNeedsSpellBook = TRUE;

					break;
				}
				case DND_CHARACTER_CLASS_ILLUSIONIST:
				{
					//this character needs a spellbook
					pSpellBook = pApp->m_SpellBooks.GetAt(DND_CHARACTER_CLASS_ILLUSIONIST);
					NPCharacterGenerateSpellBook(pCharacter, i, pSpellBook);

					bNeedsSpellBook = TRUE;

					break;
				}

				case DND_CHARACTER_CLASS_CLERIC:
				case DND_CHARACTER_SPELL_CLASS_PALADIN_CLERIC:
				{
					pSpellBook = pApp->m_SpellBooks.GetAt(DND_CHARACTER_CLASS_CLERIC);
					NPCharacterGenerateSpellBook(pCharacter, i, pSpellBook);

					break;
				}

				case DND_CHARACTER_CLASS_DRUID:
				case DND_CHARACTER_SPELL_CLASS_RANGER_DRUID:
				{
					pSpellBook = pApp->m_SpellBooks.GetAt(DND_CHARACTER_CLASS_DRUID);
					NPCharacterGenerateSpellBook(pCharacter, i, pSpellBook);

					break;
				}

			}
			
			//memorize spells
			for(int j = 1; j <= min(pCharacter->m_nCastingLevels[i], 9); ++ j)	
			{
				int nSpells = pSpellBook->m_nSpellsPerLevel[j];
				if(nSpells)
				{
					int nSpellsMemorized = 0;

					int nMaxSpellsThisLevel = GetSpellLevels(pCharacter, pCharacter->m_SpellClasses[i],  pCharacter->m_nCastingLevels[i], j);

					int nLoops = 0;
					do
					{
						int nSpell = rand() % nSpells;

						if(pCharacter->m_nSpellBooks[i][j][nSpell] == 0 || bNeedsSpellBook == TRUE)
						{
							//walk forward in the list so we don't spin in here forever
							do
							{
								nSpell = (nSpell + 1) % nSpells;
								++nLoops;
							} while(pCharacter->m_nSpellBooks[i][j][nSpell] == 0 && nLoops < 1000);
						}


						if(pCharacter->m_nSpellBooks[i][j][nSpell] == 1 || bNeedsSpellBook == FALSE)
						{
							pCharacter->m_nSpellsMemorized[i][j][nSpell] += 1;
							++nSpellsMemorized;
						}

						++nLoops;

					} while(nSpellsMemorized < nMaxSpellsThisLevel && nSpellsMemorized < nSpells && nLoops < 1000);  
				}

			}

			pCharacter->BuyAllSpellComponents(TRUE);
		}
		
	}

}

DND_CHARACTER_ALIGNMENTS GetMagicSwordAlignment()
{
	
	DND_CHARACTER_ALIGNMENTS nAlignments[9] =
	{
		DND_CHARACTER_ALIGNMENT_CHAOTIC_GOOD,
		DND_CHARACTER_ALIGNMENT_CHAOTIC_NEUTRAL,
		DND_CHARACTER_ALIGNMENT_CHAOTIC_EVIL,

		DND_CHARACTER_ALIGNMENT_NEUTRAL_EVIL,
		DND_CHARACTER_ALIGNMENT_LAWFUL_EVIL,
		DND_CHARACTER_ALIGNMENT_LAWFUL_GOOD,

		DND_CHARACTER_ALIGNMENT_LAWFUL_NEUTRAL,
		DND_CHARACTER_ALIGNMENT_NEUTRAL,
		DND_CHARACTER_ALIGNMENT_NEUTRAL_GOOD

	};

	int nDice[9][2] = 
	{
		1,5,
		6,15,
		16,20,
		21,25,
		26,30,
		31,55,
		56,60,
		61-80,
		81,100
	};


	int nDieRoll = RollD100();

	for(int i = 0; i < 9; ++i)
	{
		if(nDieRoll >= nDice[i][0] && nDieRoll <= nDice[i][1])
		{
			return nAlignments[i];
		}
	}

	return DND_CHARACTER_ALIGNMENT_NEUTRAL;
}

char *GetMagicSwordPrimaryAbility(int nIndex, int nDieRoll)
{
	static CString szRetVal = _T("");
	char sQuote = 34;

	char szAbilities[12][128] =
	{
		"detect *elevator*/shifting rooms/walls in a 1* radius",
		"detect sloping passages in a 1* radius",
		"detect traps of large size in a 1* radius",
		"detect evil/good in a 1* radius",
		"detect precious metals, kind, and amount in a 2* radius",
		"detect gems, kind, and number in a 1/2* radius",
		"detect magic in a 1* radius",
		"detect secret doors in a 1/2* radius",
		"detect invisible objects in a 1* radius",
		"locate object in a 12* radius",
		"roll twice on this table ignoring scores of 93 to 00",
		"roll on the Extraordinary Power Table instead"
	};

	int nDice[12][2] = 
	{
		1,11,
		12,22,
		23,33,
		34,44,
		45,55,
		56,66,
		67,77,
		78,82,
		83,87,
		88,92,
		93,98,
		99,100
	};

	if(nDieRoll == 0)
	{
		nIndex = 0;

		nDieRoll = RollD100();

		for(int i = 0; i < 12; ++i)
		{
			if(nDieRoll >= nDice[i][0] && nDieRoll <= nDice[i][1])
			{
				nIndex = i;
				break;
			}
		}

		if(nIndex > 9)
		{
			return GetMagicSwordExtraordinaryPowers(0, 0);
		}

	}

	szRetVal = szAbilities[nIndex];

	szRetVal.Replace('*', sQuote);

	return szRetVal.GetBuffer(0);

}

char *GetMagicSwordExtraordinaryPowers(int nIndex, int nDieRoll)
{
	static CString szRetVal = _T("");
	char sQuote = 34;

	char szAbilities[17][128] =
	{
		"charm person on contact - 3 times/day",
		"clairoudience, 3* range-3 times/day 1 round per use",
		"clairvoyance, 3* range - 3 times/day, 1 round per use",
		"determine directions and depth - 2 times/day",
		"ESP, 3* range - 3 times/day 1 round per use",
		"flying, 12*/turn - 1 hour/day",
		"heal - 1 time/day",
		"illusion, 12* range - 2 times/day, as the wand",
		"levitation, 1 turn duration - 3 times/day ot 6th level of magic",
		"strength - 1 time/day (upon wielder only)",
		"telekinesis, 2,500 g.p. wt. maximum - 2 times/day, 1 round each use",
		"telepathy, 6* range - 2 times/day",
		"teleportation - 1 time/day 6,000 g.p. wt. maximum, 2 segments",
		"X-ray vision, 4* range - 2 times/day 1 turn per use",
		"roll twice on this table ignoring scores of 95-97",
		"character may choose 1 power from this table",
		"character may choose 1 power from this table, and then roll for a Special Purpose"
	};

	int nDice[17][2] = 
	{
		1,7,
		8,15,
		16,22,
		23,28,
		29,34,
		35,41,
		42,47,
		55,61,
		48,54,
		62,67,
		68,75,
		76,81,
		82,88,
		89,94,
		95,97,
		98,99,
		100,100
	};


	if(nDieRoll == 0)
	{
		nIndex = 0;

		nDieRoll = RollD100();

		for(int i = 0; i < 17; ++i)
		{
			if(nDieRoll >= nDice[i][0] && nDieRoll <= nDice[i][1])
			{
				nIndex = i;
				break;
			}
		}

	}

	szRetVal = szAbilities[nIndex];

	szRetVal.Replace('*', sQuote);

	return szRetVal.GetBuffer(0);

}


char *GetMagicSwordSpecialPurpose(int nIndex, int nDieRoll)
{
	static CString szRetVal = _T("");
	char sQuote = 34;

	char szAbilities[9][128] =
	{
		"defeat/slay diametrically opposed alignment",
		"kill clerics",
		"kill fighters",
		"kill magic-users",
		"kill thieves",
		"kill bards/monks",
		"overthrow law and/or chaos",
		"slay good and/or evil",
		"slay non-human monsters"
	};

	int nDice[9][2] = 
	{
		1,10,
		11,20,
		21,30,
		31,40,
		41,50,
		51,55,
		56,65,
		66,75,
		76,100		
	};


	if(nDieRoll == 0)
	{
		nIndex = 0;

		nDieRoll = RollD100();

		for(int i = 0; i < 9; ++i)
		{
			if(nDieRoll >= nDice[i][0] && nDieRoll <= nDice[i][1])
			{
				nIndex = i;
				break;
			}
		}

	}

	szRetVal = szAbilities[nIndex];

	szRetVal.Replace('*', sQuote);

	return szRetVal.GetBuffer(0);

}

char *GetMagicSwordSpecialPurposePower(int nIndex, int nDieRoll)
{
	static CString szRetVal = _T("");
	char sQuote = 34;

	char szAbilities[7][128] =
	{
		"blindness for 2-12 rounds",
		"confusion for 2-12 rounds",
		"disintegrate",
		"fear for 1-4 rounds",
		"insanity for 1-4 rounds",
		"paralysis for 1-4 rounds",
		"+2 on all saving throws, -1 on each die of damage sustained"
	};

	int nDice[7][2] = 
	{
		1,10,
		11,20,
		21,25,
		26,55,
		56,65,
		66,80,
		81,100			
	};


	if(nDieRoll == 0)
	{
		nIndex = 0;

		nDieRoll = RollD100();

		for(int i = 0; i < 7; ++i)
		{
			if(nDieRoll >= nDice[i][0] && nDieRoll <= nDice[i][1])
			{
				nIndex = i;
				break;
			}
		}

	}

	szRetVal = szAbilities[nIndex];

	szRetVal.Replace('*', sQuote);

	return szRetVal.GetBuffer(0);

}

cDNDCustomClass::cDNDCustomClass()
{
	Clear();
}

cDNDCustomClass::~cDNDCustomClass()
{
}

void cDNDCustomClass::Clear()
{
	memset(m_szClassName, 0, 32 * sizeof(char));
	memset(m_lXPLevel, 0, 12 * sizeof(LONG));

	m_bClassDefined = FALSE;
	m_Class = DND_CHARACTER_CLASS_UNDEF;

	m_nHitDieType = 6;
	m_lMaxChartLevelXP = 220000;
	m_nMaxChartLevel = 9;

	m_nMaxChartLevelHP = 2;
	m_nMaxChartHPLevel = 9;

	m_CombatClass = DND_CHARACTER_CLASS_CLERIC;
	m_SavingThrowClass = DND_CHARACTER_CLASS_CLERIC;
	m_MagicUseClass = DND_CHARACTER_CLASS_UNDEF;
	m_nMagicUseLevel = 1;

	m_bTurnUndead = FALSE;
	m_nTurnUndeadLevel = 1;

	m_bThiefSkills = FALSE;
	m_nThiefLevel = 1;

	m_bAssassinSkills = FALSE;
	m_nAssassinLevel = 1;

	m_nInitialWeaponProf = 1;
	m_nAddWeaponProf = 1;		// unused
	m_nAddWeaponProfLevels = 6;
	m_nNonWeaponProfPenalty = 5;

	for (int i = 0; i < 6; ++i)
	{
		m_nMinAttributes[i] = 3;
		m_bPrerequisiteAttrib[i] = FALSE;
	}

	m_nFirstLevelHD = 1;

	m_bWeaponSpecialization = FALSE;
	m_nWeaponSpecializationLevel = 0;

	memset(m_ExpansionBuffer, 0, CUSTOM_CLASSES_EXPAMSION_BUFFER_SIZE * sizeof(ULONG));
}

void cDNDCustomClass::PopulateXPTable()
{
	//m_lXPLevel[0]
}


cDNDCustomClass *GetCustomClass(DND_CHARACTER_CLASSES _Class)
{
	#if CUSTOM_CLASSES
	int nClassIndex = (int)_Class;

	nClassIndex -= DND_CHARACTER_CLASS_CUSTOM_1;

	if (nClassIndex < 0 || nClassIndex > DND_CHARACTER_CLASS_CUSTOM_12)
	{
		nClassIndex = 0;
	}
	#else
	int nClassIndex = 0;
	#endif

	return &_gCustomClass[nClassIndex];
}

BOOL IsCustomClass(DND_CHARACTER_CLASSES _Class)
{
	#if CUSTOM_CLASSES
	if (_Class >= DND_CHARACTER_CLASS_CUSTOM_1 && _Class <= DND_CHARACTER_CLASS_CUSTOM_12)
	{
		return TRUE;
	}
	#endif

	return FALSE;
}

int GetNumDefinedCustomClasses()
{
	#if CUSTOM_CLASSES
	int nCount = 0;
	for (int i = 0; i < MAX_CUSTOM_CLASSES; ++i)
	{
		if(_gCustomClass[i].m_bClassDefined)
		{
			++nCount;
		}
	}

	return nCount;
	#else
	return 0;
	#endif
}

int cDNDMonsterManualEntry::GetIntelligenceStat()
{
	int nRetVal = 0;

	if(m_szIntelligence.Find("(0)") >= 0)
	{
		return nRetVal;
	}

	int nLowVal = 0;
	int nHighVal = 0;

	CString szDigits = _T("");

	for (int i = 0; i < m_szIntelligence.GetLength(); ++i)
	{
		CString szChar = m_szIntelligence.GetAt(i);

		if ((szChar >= "0" && szChar <= "9") || szChar == "-")
		{
			szDigits += szChar;
		}
	}

	CString sToken = _T("");
	i = 0; // substring index to extract
	while (AfxExtractSubString(sToken, szDigits, i, '-'))
	{
		if (sToken != "")
		{
			switch (i)
			{
				case 0:	nLowVal = atoi(sToken.GetBuffer(0)); break;
				case 1:	nHighVal = atoi(sToken.GetBuffer(0)); break;
			}
		}

		++i;		
	}

	if (nHighVal < nLowVal)
	{
		nRetVal = nLowVal;
	}
	else
	{
		int nSpan = (nHighVal - nLowVal) + 1;

		nRetVal = nLowVal + rand() % nSpan;
	}

	return nRetVal;
}

void cDNDMapSFXMotionTracker::Init(float fStartPosX, float fStartPosY, float fTargetPosX, float fTargetPosY, float fSpeed, int nSprites)
{
	Clear();

	m_fPosX = fStartPosX;
	m_fPosY = fStartPosY;

	m_fTargetPosX = fTargetPosX;
	m_fTargetPosY = fTargetPosY;

	m_fDistance = GetTargetDistance();
	m_fLastDistance = 999999.99f;

	m_fMoveX = (m_fTargetPosX - m_fPosX) / m_fDistance / 10.0f;
	m_fMoveY = (m_fTargetPosY - m_fPosY) / m_fDistance / 10.0f;

	m_fSpeed = fSpeed;

	m_nMaxSprites = nSprites;

	m_dwFlightTime = GetUniversalTime();
}

float cDNDMapSFXMotionTracker::GetTargetDistance()
{
	return GetDistance(m_fPosX, m_fPosY, m_fTargetPosX, m_fTargetPosY);
}

BOOL cDNDMapSFXMotionTracker::Move(float fMapScale)
{
	if (m_bTargetHit)
	{
		return FALSE;
	}

	++m_nFrames;

	if (m_nFrames % 5 == 0)
	{
		++m_nSprites;
		if (m_nSprites > m_nMaxSprites)
		{
			m_nSprites = m_nMaxSprites;
		}
	}

	float fSpeed = m_fSpeed / fMapScale;

	m_fPosX += m_fMoveX * fSpeed;
	m_fPosY += m_fMoveY * fSpeed;

	m_fOffsetPosX += m_fMoveX * m_fSpeed;
	m_fOffsetPosY += m_fMoveY * m_fSpeed;

	m_fSpriteOffsetX = m_fMoveX * fSpeed;
	m_fSpriteOffsetY = m_fMoveY * fSpeed;

	m_fDistance = GetTargetDistance();

	if (m_fLastDistance < m_fDistance)
	{
		m_fSpeed = 0.0f;
		m_bTargetHit = TRUE;
	}
	else if (m_nFrames > 5000)
	{
		m_fSpeed = 0.0f;
		m_bTargetHit = TRUE;
	}

	m_fLastDistance = m_fDistance;

	if (m_bTargetHit)
	{
		DWORD dwFlightTime = GetUniversalTime() - m_dwFlightTime;

		TRACE("FLIGHT TIME %ld\n", m_dwFlightTime);
	}

	return TRUE;
}

BOOL cDNDMapSFXMotionTracker::CheckTimer()
{
	if (GetUniversalTime() - m_dwFlightTime > 5)
	{
		return TRUE;
	}

	return FALSE;
}



/*
Armorer
Bowyer/fletcher
Former/gardener
Fisher (netting)
Forester
Gambler
Hunter/fisher (hook and line)
Husbandman (animal husbandry)
Jeweler/lapidary
Leather worker/tanner
Limner/painter
Mason/carpenter
Miner
Navigator (fresh or salt water)
Sailor (fresh or salt)
Shipwright (boats or ships)
Tailor/weaver
Teamster/freighter
Trader/barterer
Trapper/furrier
Woodworker/cabinetmaker
*/