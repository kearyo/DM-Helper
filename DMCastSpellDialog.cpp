// DMCastSpellDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMInventoryDialog.h"
#include "DMCharSpellsDialog.h"
#include "DMCharViewDialog.h"
#include "cDMMaterialComponentsDialog.h"
#include "DMNPCViewDialog.h"
#include "DMPartyDialog.h"
#include "DMSpellDescDialog.h"
#include "DMInitiativeDialog.h"
#include "DMCharacterSelectorDialog.h"
#include "DMCastSpellDialog.h"
#include "DMReminderSFXDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DMCastSpellDialog dialog


DMCastSpellDialog::DMCastSpellDialog(DMPartyDialog *_pPartyDialog, CDMBaseCharViewDialog *_pBaseCharViewDialog, cDNDCharacter	*_pCharacter, CWnd* pParent /*=NULL*/)
	: CDialog(DMCastSpellDialog::IDD, pParent)
	, m_szSpellDesc(_T(""))
{
	//{{AFX_DATA_INIT(DMCastSpellDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_pPartyDialog = _pPartyDialog;
	m_pBaseCharViewDialog = _pBaseCharViewDialog;
	m_pCharacter = _pCharacter;
}


void DMCastSpellDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DMCastSpellDialog)
	DDX_Control(pDX, IDC_SPELL_LIST, m_cSpellList);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_cCastButton);
	DDX_Text(pDX, IDC_SPELL_DESC, m_szSpellDesc);
}


BEGIN_MESSAGE_MAP(DMCastSpellDialog, CDialog)
	//{{AFX_MSG_MAP(DMCastSpellDialog)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_SPELL_INFO, OnSpellInfo)
	//}}AFX_MSG_MAP
	ON_LBN_DBLCLK(IDC_SPELL_LIST, &DMCastSpellDialog::OnLbnDblclkSpellList)
	ON_LBN_SELCHANGE(IDC_SPELL_LIST, &DMCastSpellDialog::OnLbnSelchangeSpellList)
	ON_BN_CLICKED(ID_FAIL_CAST, &DMCastSpellDialog::OnBnClickedFailCast)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DMCastSpellDialog message handlers

BOOL DMCastSpellDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	if(m_pPartyDialog != NULL && m_pPartyDialog->m_pInitiativeDialog != NULL && m_pBaseCharViewDialog->m_nCastSpellCursorPos == -1)
	{
		m_cCastButton.SetWindowText("Start Cast");
	}
	else
	{
		m_cCastButton.SetWindowText("Cast");
	}

	m_cSpellList.SetHorizontalExtent(1000);

	Refresh();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DMCastSpellDialog::Refresh()
{
	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	CleanMemorizedSpellListBox(&m_cSpellList);

	cDNDSpellBook *pSpellBook;

	int nSpellCount = 0;

	CString szTemp;

	for(int nSpellClass = 0; nSpellClass < 4 ; ++nSpellClass)
	{
		switch(m_pCharacter->m_SpellClasses[nSpellClass])
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

		if(pSpellBook != NULL)
		{
			#if USE_CANTRIPS
			for(int nLevel = 0;  nLevel <= MAX_SPELL_LEVELS; ++nLevel)
			#else
			for(int nLevel = 1;  nLevel <= MAX_SPELL_LEVELS; ++nLevel)
			#endif
			{
				for(int nSpell = 0; nSpell < MAX_SPELLS_PER_LEVEL; ++nSpell)
				{
					if(m_pCharacter->m_nSpellsMemorized[nSpellClass][nLevel][nSpell])
					{
						cDNDSpell *pSpell = &pSpellBook->m_Spells[nLevel][nSpell];

						if(m_pCharacter->m_nSpellsMemorized[nSpellClass][nLevel][nSpell] > 1)
							szTemp.Format("%d x %s", m_pCharacter->m_nSpellsMemorized[nSpellClass][nLevel][nSpell], pSpell->m_szSpellName);
						else
							szTemp = pSpell->m_szSpellName;

						CString szComponents = "  ( * )";
						szComponents.Replace("*", pSpell->m_szSpellComponents);
						szTemp += szComponents;

						m_cSpellList.InsertString(nSpellCount, szTemp);

						PSPELLSLOT pSpellSlot = new cDNDSpellSlot(pSpell, &m_pCharacter->m_nSpellsMemorized[nSpellClass][nLevel][nSpell], 1, m_pCharacter->m_nCastingLevels[nSpellClass], FALSE);
						m_cSpellList.SetItemData(nSpellCount, (ULONG)pSpellSlot);

						++nSpellCount;
					}
				}
			}
		}
	}

	//add scrolls, ring, wands etc

	for(int i = 0; i< MAX_CHARACTER_INVENTORY; ++i)
	{
		if( (m_pCharacter->m_Inventory[i].IsScroll() && m_pCharacter->m_Inventory[i].m_nContentsCount[0]) || (m_pCharacter->m_Inventory[i].UsesMagicCharges() && m_pCharacter->m_Inventory[i].m_nCharges >= 1) )
		{
			for(int j = 0; j < MAX_OBJECT_CONTENTS; ++j)
			{
				if(m_pCharacter->m_Inventory[i].m_nContents[j])
				{
					int nSpellIndex = m_pCharacter->m_Inventory[i].m_nContents[j] - 1;

					cDNDSpell *pSpell = m_pApp->m_MasterSpellArray.GetAt(nSpellIndex);

					if(pSpell != NULL)
					{
						int *pnCharges = NULL;
						int nChargesExpended = 1;
						BOOL bCastFromDevice = FALSE;

						if(m_pCharacter->m_Inventory[i].IsScroll())
						{
							if(m_pCharacter->m_Inventory[i].m_nContentsCount[j] > 1)
							{
								szTemp.Format("scroll : %s (x%d)", pSpell->m_szSpellName, m_pCharacter->m_Inventory[i].m_nContentsCount[j]);
							}
							else
							{
								szTemp.Format("scroll : %s", pSpell->m_szSpellName);
							}

							int nTotalFailure = 0;
							int nReversed = 0;

							int nSpellFailure = GetScrollSpellFailureChance(m_pCharacter, &m_pCharacter->m_Inventory[i], pSpell->m_nSpellLevel, &nTotalFailure, &nReversed);

							CString szFail = _T("");
							if(nSpellFailure >= 100)
							{
								szFail = " [CANNOT CAST]";
							}
							else if(nSpellFailure > 0)
							{
								szFail.Format(" [Fail: %d%c Total Fail: %d%c Reverse: %d%c]", nSpellFailure, 37, nTotalFailure, 37, nReversed, 37);
							}

							szTemp += szFail;

							pnCharges = &m_pCharacter->m_Inventory[i].m_nContentsCount[j];
						}
						else if(m_pCharacter->m_Inventory[i].IsMagicRod())
						{
							szTemp.Format("rod : %s (%d/%d)", pSpell->m_szSpellName, m_pCharacter->m_Inventory[i].m_nContentsCount[j], m_pCharacter->m_Inventory[i].m_nCharges);

							pnCharges = &m_pCharacter->m_Inventory[i].m_nCharges;
							nChargesExpended = max(m_pCharacter->m_Inventory[i].m_nContentsCount[j], 1); 
							bCastFromDevice = TRUE;
						}
						else if(m_pCharacter->m_Inventory[i].IsMagicStaff())
						{
							szTemp.Format("staff : %s (%d/%d)", pSpell->m_szSpellName, m_pCharacter->m_Inventory[i].m_nContentsCount[j], m_pCharacter->m_Inventory[i].m_nCharges);

							pnCharges = &m_pCharacter->m_Inventory[i].m_nCharges;
							nChargesExpended = max(m_pCharacter->m_Inventory[i].m_nContentsCount[j], 1); 
							bCastFromDevice = TRUE;
						}
						else if(m_pCharacter->m_Inventory[i].IsMagicWand())
						{
							szTemp.Format("wand : %s (%d/%d)", pSpell->m_szSpellName, m_pCharacter->m_Inventory[i].m_nContentsCount[j], m_pCharacter->m_Inventory[i].m_nCharges);

							pnCharges = &m_pCharacter->m_Inventory[i].m_nCharges;
							nChargesExpended = max(m_pCharacter->m_Inventory[i].m_nContentsCount[j], 1); 
							bCastFromDevice = TRUE;
						}
						else if(m_pCharacter->m_Inventory[i].IsMagicRing())
						{
							szTemp.Format("ring : %s (%d/%d)", pSpell->m_szSpellName, m_pCharacter->m_Inventory[i].m_nContentsCount[j], m_pCharacter->m_Inventory[i].m_nCharges);

							pnCharges = &m_pCharacter->m_Inventory[i].m_nCharges;
							nChargesExpended = max(m_pCharacter->m_Inventory[i].m_nContentsCount[j], 1); 
							bCastFromDevice = TRUE;
						}

						m_cSpellList.InsertString(nSpellCount, szTemp);

						PSPELLSLOT pSpellSlot = new cDNDSpellSlot(pSpell, pnCharges, nChargesExpended, 7, bCastFromDevice);
						m_cSpellList.SetItemData(nSpellCount, (ULONG)pSpellSlot);

						++nSpellCount;
					}
				}
			}
		}
	}


	if(m_pBaseCharViewDialog->m_nCastSpellCursorPos > -1)
	{
		m_cSpellList.SetCurSel(m_pBaseCharViewDialog->m_nCastSpellCursorPos);
	}

}

void DMCastSpellDialog::OnOK() 
{
	BOOL bCloseWindow = TRUE;

	UpdateData(TRUE);

	int nCursor = m_cSpellList.GetCurSel();

	if(nCursor >= 0)
	{
		PSPELLSLOT pSpellSlot = (PSPELLSLOT)m_cSpellList.GetItemData(nCursor);
		if(pSpellSlot != NULL)
		{
			if(m_pPartyDialog != NULL && m_pPartyDialog->m_pInitiativeDialog != NULL)
			{
				if(m_pBaseCharViewDialog->m_bInitiativeCasting == FALSE)
				{
					m_pBaseCharViewDialog->m_nCastSpellCursorPos = nCursor;

					int nCastingTime = m_pApp->GetCastingTime(pSpellSlot->m_pSpell);

					if(nCastingTime > 1)
					{
						m_pBaseCharViewDialog->m_nInitiativeRoll += (nCastingTime - 1);
						m_pBaseCharViewDialog->m_bInitiativeCasting = TRUE;
						m_pBaseCharViewDialog->m_nCastingSpellSegments = nCastingTime;
					}
					else
					{
						m_pBaseCharViewDialog->m_szInitiativeAction.Format("cast : %s", pSpellSlot->m_pSpell->m_szSpellName);
						m_pPartyDialog->m_pInitiativeDialog->Refresh();
					}
				}
				else
				{
					m_pBaseCharViewDialog->m_szInitiativeAction.Format("cast : %s", pSpellSlot->m_pSpell->m_szSpellName);
					m_pBaseCharViewDialog->m_bInitiativeCasting = FALSE;
				}
			}

			if(m_pBaseCharViewDialog->m_bInitiativeCasting)
			{
				m_pBaseCharViewDialog->m_szInitiativeAction.Format("casting : %s", pSpellSlot->m_pSpell->m_szSpellName);
				if(m_pPartyDialog->m_pInitiativeDialog != NULL)
				{
					m_pPartyDialog->m_pInitiativeDialog->RefreshAndSelect();
				}
			}
			else
			{
				DND_SPELL_MATERIAL_RETURN_CODES nRetCode = m_pBaseCharViewDialog->CharacterCanCastSpell(pSpellSlot->m_pSpell, 1, FALSE, FALSE, FALSE);

				if (nRetCode != DND_SPELL_MATERIAL_RETURN_CANNOT_CAST)
				{
					m_pBaseCharViewDialog->CharacterCanCastSpell(pSpellSlot->m_pSpell, 1, TRUE, FALSE, FALSE);
					m_pCharacter->CastSpell(pSpellSlot);

					CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

					if (pApp->SpellIsHealingSpell(pSpellSlot->m_pSpell))
					{
						DWORD dwCharacterID = 0;
						DMCharacterSelectorDialog *pDlg = new DMCharacterSelectorDialog(&dwCharacterID, 0, DND_SELECTOR_CHARACTER);
						pDlg->DoModal();

						if (dwCharacterID)
						{
							m_pApp->HealCharacter(dwCharacterID);
						}
					}

					// delayed blast
					BOOL bPlaySounds = TRUE;
					#if GAMETABLE_BUILD
					if (pApp->m_pInstantMapSFXPlacer == NULL)
					{
						if (pApp->m_bSpellFXOnMaps)
						{
							int nSoundRepeats = pApp->GetSpellRepeats(pSpellSlot);
							pApp->m_pInstantMapSFXPlacer = new cDNDInstantMapSFXPlacer(m_pBaseCharViewDialog->m_szCharacterFirstName, m_pCharacter->m_dwCharacterID, pSpellSlot->m_pSpell, pSpellSlot->m_bCastFromDevice, nSoundRepeats);
							AfxBeginThread(DMInstantMapSFXThreadProc, (LPVOID)pApp);
							if (pApp->m_pDMReminderSFXDialog == NULL)
							{
								pApp->m_pDMReminderSFXDialog = new CDMReminderSFXDialog(AfxGetMainWnd());
							}

							bPlaySounds = FALSE;
						}
					}
					#endif

					if (bPlaySounds)
					{
						if (pSpellSlot->m_bCastFromDevice == FALSE && pSpellSlot->m_pSpell->HasVerbalComponent())
						{
							pApp->PlayPCSoundFX("* PC Cast Spell", m_pBaseCharViewDialog->m_szCharacterFirstName, "NADA", FALSE, pSpellSlot->m_pSpell->m_nSpellIdentifier);
						}

						int nSoundRepeats = pApp->GetSpellRepeats(pSpellSlot);
						pApp->PlaySpellSFX(pSpellSlot->m_pSpell->m_nSpellIdentifier, nSoundRepeats);
					}

					DMPartyDialog *pPartyDlg = pApp->FindCharacterPartyDialog(m_pCharacter);
					if (pPartyDlg != NULL)
					{
						pPartyDlg->LogPartyEvent(FALSE, APPEND_TO_LOG, DND_LOG_EVENT_TYPE_CHARACTER_CAST_SPELL, m_pCharacter->m_szCharacterName, pPartyDlg->m_pParty->m_dwPartyID, 0L, pSpellSlot->m_pSpell->m_szSpellName);
					}

					m_pBaseCharViewDialog->m_nCastSpellCursorPos = -1;

					if (m_pPartyDialog->m_pInitiativeDialog != NULL)
					{
						m_pPartyDialog->m_pInitiativeDialog->Refresh();
					}

					if (m_pBaseCharViewDialog->m_pInventoryDialog != NULL)
					{
						m_pBaseCharViewDialog->m_pInventoryDialog->RefreshAll();
					}
				}
				else
				{
					bCloseWindow = FALSE;
				}
			}
		}
	}
	
	if (bCloseWindow)
	{
		CDialog::OnOK();
	}
}

void DMCastSpellDialog::OnBnClickedFailCast()
{
	BOOL bCloseWindow = TRUE;

	UpdateData(TRUE);

	int nCursor = m_cSpellList.GetCurSel();

	if (nCursor >= 0)
	{
		PSPELLSLOT pSpellSlot = (PSPELLSLOT)m_cSpellList.GetItemData(nCursor);
		if (pSpellSlot != NULL)
		{
			if (m_pPartyDialog != NULL && m_pPartyDialog->m_pInitiativeDialog != NULL)
			{
				m_pCharacter->CastSpell(pSpellSlot);

				CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

				if (pSpellSlot->m_bCastFromDevice == FALSE && pSpellSlot->m_pSpell->HasVerbalComponent())
				{
					pApp->PlayPCSoundFX("* PC Cast Spell", m_pBaseCharViewDialog->m_szCharacterFirstName, "NADA", FALSE, pSpellSlot->m_pSpell->m_nSpellIdentifier);
				}
				m_pApp->PlaySoundFX("SPELL FAILURE");

				m_pBaseCharViewDialog->m_szInitiativeAction.Format("failed to cast : %s", pSpellSlot->m_pSpell->m_szSpellName);

				DMPartyDialog *pPartyDlg = pApp->FindCharacterPartyDialog(m_pCharacter);
				if (pPartyDlg != NULL)
				{
					pPartyDlg->LogPartyEvent(FALSE, APPEND_TO_LOG, DND_LOG_EVENT_TYPE_CHARACTER_FAILED_CAST_SPELL, m_pCharacter->m_szCharacterName, pPartyDlg->m_pParty->m_dwPartyID, 0L, pSpellSlot->m_pSpell->m_szSpellName);
				}

				m_pBaseCharViewDialog->m_nCastSpellCursorPos = -1;

				if (m_pPartyDialog->m_pInitiativeDialog != NULL)
				{
					m_pPartyDialog->m_pInitiativeDialog->Refresh();
				}

				if (m_pBaseCharViewDialog->m_pInventoryDialog != NULL)
				{
					m_pBaseCharViewDialog->m_pInventoryDialog->RefreshAll();
				}
			}
		}
	}

	if (bCloseWindow)
	{
		CDialog::OnOK();
	}
}



void DMCastSpellDialog::OnSpellInfo() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	int nCursor = m_cSpellList.GetCurSel();

	if(nCursor != -1)
	{
		PSPELLSLOT pSpellSlot = (PSPELLSLOT)m_cSpellList.GetItemData(nCursor);
		DMSpellDescDialog *pDlg = new DMSpellDescDialog(pSpellSlot->m_pSpell);
		pDlg->DoModal();
		delete pDlg;
	}
	
}


void DMCastSpellDialog::OnCancel() 
{
	if (m_pBaseCharViewDialog != NULL)
	{
		m_pBaseCharViewDialog->m_szInitiativeAction = _T("");
		m_pBaseCharViewDialog->m_bInitiativeCasting = FALSE;
		m_pBaseCharViewDialog->m_nCastSpellCursorPos = -1;
		m_pBaseCharViewDialog->m_nCastingSpellSegments = 0;
	}
	
	CDialog::OnCancel();
}

void DMCastSpellDialog::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

void DMCastSpellDialog::OnDestroy() 
{
	//clean memorized spell data
	CleanMemorizedSpellListBox(&m_cSpellList);

	CDialog::OnDestroy();
	
}

void DMCastSpellDialog::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonDblClk(nFlags, point);
}



void DMCastSpellDialog::OnLbnDblclkSpellList()
{
	OnOK();
}


void DMCastSpellDialog::OnLbnSelchangeSpellList()
{
	UpdateData(TRUE);

	m_szSpellDesc = _T("");

	int nCursor = m_cSpellList.GetCurSel();

	if (nCursor >= 0)
	{
		if (g_bUseMaterialComponents)
		{
			PSPELLSLOT pSpellSlot = (PSPELLSLOT)m_cSpellList.GetItemData(nCursor);
			if (pSpellSlot != NULL && pSpellSlot->m_pSpell != NULL)
			{
				//DND_SPELL_MATERIAL_RETURN_CODES nRetCode = m_pBaseCharViewDialog->CharacterCanCastSpell(pSpellSlot->m_pSpell, 1, FALSE, FALSE, FALSE);
				DND_SPELL_MATERIAL_RETURN_CODES nRetCode = m_pCharacter->CasterHasSpellMaterialComponents(pSpellSlot->m_pSpell, 1, FALSE, FALSE, m_pBaseCharViewDialog->m_SpellMaterialComponentsRequiredVector);
				
				if (nRetCode != DND_SPELL_MATERIAL_RETURN_CAN_CAST)
				{
					switch (nRetCode)
					{
						case DND_SPELL_MATERIAL_RETURN_CANNOT_CAST:
						{
							m_szSpellDesc = _T("INSUFFICIENT MATERIAL COMPONENTS !");
							break;
						}
						case DND_SPELL_MATERIAL_RETURN_CAN_CAST_50_50_50:
						{
							m_szSpellDesc = " RANGE     DURATION     AREA of EFFECT\n 50%          50%               50%";
							break;
						}
						case DND_SPELL_MATERIAL_RETURN_CAN_CAST_75_50_75:
						{
							m_szSpellDesc = " RANGE     DURATION     AREA of EFFECT\n 75%          50%               75%";
							break;
						}
						case DND_SPELL_MATERIAL_RETURN_CAN_CAST_75_50_100:
						{
							m_szSpellDesc = " RANGE     DURATION     AREA of EFFECT\n 75%          50%               100%";
							break;
						}
						case DND_SPELL_MATERIAL_RETURN_CAN_CAST_100_75_100:
						{
							m_szSpellDesc = " RANGE     DURATION     AREA of EFFECT\n 100%        75%               100%";
							break;
						}
					}
				}
			}
		}
	}


	UpdateData(FALSE);
}


