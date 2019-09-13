#include "stdafx.h"
#include "CppUnitTest.h"
#include "resource.h"
#include "../DM Helper.h"
#include "../DM HelperDlg.h"
#include "../DMPartyDialog.h"
#include "../DMInventoryDialog.h"
#include "../DMCharSpellsDialog.h"
#include "../DMCharViewDialog.h"
#include "../cDMStrikeOrThrowDialog.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DMHelper_tests_0
{		
	TEST_CLASS(DMHelper_Unit_Tests)
	{
	public:
		
		TEST_METHOD(BaseTestMethod)
		{
			Assert::AreNotEqual(1,2);
		}

		TEST_METHOD(cDNDCharacterTest)
		{
			CDMHelperApp *pApp = (CDMHelperApp *)malloc(sizeof(CDMHelperApp));

			CDMHelperDlg* pMainDialog = new CDMHelperDlg();
			cDNDCharacter *pCharacter = new cDNDCharacter();

			Assert::AreEqual(pCharacter->m_dwCharacterID, (DWORD)0);

			CDMCharViewDialog *pCharViewDialog = new CDMCharViewDialog(pMainDialog, pCharacter);

			pCharViewDialog->m_pCharacter = pCharacter;
			pCharacter->m_Class[0] = DND_CHARACTER_CLASS_PALADIN;
			pCharacter->m_nLevel[0] = 1;
			pCharacter->m_nRace = DND_CHARACTER_RACE_HUMAN;

			pCharViewDialog->ExternalRollPCStats();

			bool bSuccess = (pCharacter->m_nBaseStats[ATTRIB_CHA] >= 17);  // Paladins have a minimum charisma  of 17

			Assert::IsTrue(bSuccess);

			delete pCharViewDialog;
			delete pCharacter;
			free(pApp);

		}

		TEST_METHOD(cDNDNonPlayerCharacterTest)
		{
			CDMHelperApp *pApp = (CDMHelperApp *)malloc(sizeof(CDMHelperApp));
			memset(pApp, 0, sizeof(CDMHelperApp));

			cDNDNonPlayerCharacter *pNPC = new cDNDNonPlayerCharacter();
			Assert::AreEqual(pNPC->m_dwCharacterID, (DWORD)0);

			PARMORTYPEMAP _ArmorOrderedTypeMap;
			PARMORTYPEMAP _ArmorIndexedTypeMap;
			POBJECTTYPEARRAY _ObjectsOrderedTypeArray;
			POBJECTTYPEARRAY _ObjectsIndexedTypeArray;
			POBJECTTYPEARRAY _HelmetsOrderedTypeArray;
			POBJECTTYPEARRAY _HelmetsIndexedTypeArray;

			//fake out the memory structures
			memcpy(&pApp->m_ArmorOrderedTypeMap, &_ArmorOrderedTypeMap, sizeof(PARMORTYPEMAP));
			memcpy(&pApp->m_ArmorIndexedTypeMap, &_ArmorIndexedTypeMap, sizeof(PARMORTYPEMAP));

			memcpy(&pApp->m_HelmetsOrderedTypeArray, &_HelmetsOrderedTypeArray, sizeof(PARMORTYPEMAP));
			memcpy(&pApp->m_HelmetsIndexedTypeArray, &_HelmetsIndexedTypeArray, sizeof(PARMORTYPEMAP));

			memcpy(&pApp->m_ObjectsOrderedTypeArray, &_ObjectsOrderedTypeArray, sizeof(PARMORTYPEMAP));
			memcpy(&pApp->m_ObjectsIndexedTypeArray, &_ObjectsIndexedTypeArray, sizeof(PARMORTYPEMAP));
			//pApp->m_nDefinedObjectCount = 0;

			pApp->LoadArmorTable("..\\RELEASE\\Data\\tables\\armor.dat");
			pApp->LoadHelmetsTable("..\\RELEASE\\Data\\tables\\helmets.dat");

			BOOL bSuccess = pApp->AddItemToInventoryByName(pNPC, "helm,leather");

			Assert::IsTrue(bSuccess);

			bSuccess = (pNPC->m_Inventory[0].m_ObjectType == DND_OBJECT_TYPE_HELMET);
			
			Assert::IsTrue(bSuccess);
			
			free(pApp);
		}

		TEST_METHOD(cDNDPartyLogTest)
		{
			CDMHelperApp *pApp = (CDMHelperApp *)malloc(sizeof(CDMHelperApp));
			memset(pApp, 0, sizeof(CDMHelperApp));

			DMPartyDialog *pPartyDialog = (DMPartyDialog *)malloc(sizeof(DMPartyDialog));
			memset(pPartyDialog, 0, sizeof(DMPartyDialog));

			pPartyDialog->m_pApp = pApp;
			pPartyDialog->m_pParty = new cDNDParty();
			pPartyDialog->m_pPartyLog = new cDNDPartyLog();

			PCHARVIEWMAP _CharacterViewMap;
			memcpy(&pApp->m_CharacterViewMap, &_CharacterViewMap, sizeof(PCHARVIEWMAP));

			cDNDCharacter *pCharacter = new cDNDCharacter();
			pCharacter->m_dwCharacterID = 1024;
			strcpy(pCharacter->m_szCharacterName, "Bardon");
			
			char szComment[128];
			memset(szComment, 0, 128 * sizeof(char));

			for (int i = 0; i < 3000; ++i)
			{
				pPartyDialog->m_pParty->m_nYear = i;
				
				int nVal = i % DND_LOG_EVENT_TYPE_LAST_EVENT_TYPE;
				DND_LOG_EVENT_TYPES nEvent = (DND_LOG_EVENT_TYPES)nVal;

				pPartyDialog->LogPartyEvent(FALSE, APPEND_TO_LOG, nEvent, "Bardon", 0, 0L, szComment);
			}

			Assert::AreEqual(pPartyDialog->m_pPartyLog->m_LogHeader.m_nEvents, 2048);

			pPartyDialog->m_pPartyLog->DeleteEvent(1024);

			// this is 1971 because a bunch of DND_LOG_EVENT_TYPE_UNDEFINED are deleted by ValidateEvents
			Assert::AreEqual(pPartyDialog->m_pPartyLog->m_LogHeader.m_nEvents, 1971);

			BOOL bSuccess = (pPartyDialog->m_pPartyLog->m_Event[1024].m_EventType == DND_LOG_EVENT_TYPE_CHARACTER_LOST_LEVEL);

			Assert::IsTrue(bSuccess);
		
		}
	};

	


}