// DMCastSpellDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMInventoryDialog.h"
#include "DMCharSpellsDialog.h"
#include "DMCharViewDialog.h"
#include "DMNPCViewDialog.h"
#include "DMPartyDialog.h"
#include "DMSpellDescDialog.h"
#include "DMInitiativeDialog.h"
#include "DMCastSpellDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DMCastSpellDialog dialog


DMCastSpellDialog::DMCastSpellDialog(DMPartyDialog *_pPartyDialog, CDMBaseCharViewDialog *_pBaseCharViewDialog, cDNDCharacter	*_pCharacter, CWnd* pParent /*=NULL*/)
	: CDialog(DMCastSpellDialog::IDD, pParent)
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
}


BEGIN_MESSAGE_MAP(DMCastSpellDialog, CDialog)
	//{{AFX_MSG_MAP(DMCastSpellDialog)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	ON_BN_CLICKED(IDC_SPELL_INFO, OnSpellInfo)
	//}}AFX_MSG_MAP
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

						m_cSpellList.InsertString(nSpellCount, szTemp);

						PSPELLSLOT pSpellSlot = new cDNDSpellSlot(pSpell, &m_pCharacter->m_nSpellsMemorized[nSpellClass][nLevel][nSpell], 1);
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
						}
						else if(m_pCharacter->m_Inventory[i].IsMagicStaff())
						{
							szTemp.Format("staff : %s (%d/%d)", pSpell->m_szSpellName, m_pCharacter->m_Inventory[i].m_nContentsCount[j], m_pCharacter->m_Inventory[i].m_nCharges);

							pnCharges = &m_pCharacter->m_Inventory[i].m_nCharges;
							nChargesExpended = max(m_pCharacter->m_Inventory[i].m_nContentsCount[j], 1); 
						}
						else if(m_pCharacter->m_Inventory[i].IsMagicWand())
						{
							szTemp.Format("wand : %s (%d/%d)", pSpell->m_szSpellName, m_pCharacter->m_Inventory[i].m_nContentsCount[j], m_pCharacter->m_Inventory[i].m_nCharges);

							pnCharges = &m_pCharacter->m_Inventory[i].m_nCharges;
							nChargesExpended = max(m_pCharacter->m_Inventory[i].m_nContentsCount[j], 1); 
						}
						else if(m_pCharacter->m_Inventory[i].IsMagicRing())
						{
							szTemp.Format("ring : %s (%d/%d)", pSpell->m_szSpellName, m_pCharacter->m_Inventory[i].m_nContentsCount[j], m_pCharacter->m_Inventory[i].m_nCharges);

							pnCharges = &m_pCharacter->m_Inventory[i].m_nCharges;
							nChargesExpended = max(m_pCharacter->m_Inventory[i].m_nContentsCount[j], 1); 
						}

						m_cSpellList.InsertString(nSpellCount, szTemp);

						PSPELLSLOT pSpellSlot = new cDNDSpellSlot(pSpell, pnCharges, nChargesExpended);
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
				m_pCharacter->CastSpell(pSpellSlot);

				CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

				pApp->PlayPCSoundFX("* Cast Spell", m_pBaseCharViewDialog->m_szCharacterFirstName, "NADA", FALSE);
				pApp->PlaySpellSFX(pSpellSlot->m_pSpell->m_nSpellIdentifier);

				DMPartyDialog *pPartyDlg = pApp->FindCharacterPartyDialog(m_pCharacter);
				if(pPartyDlg != NULL)
				{
					pPartyDlg->LogPartyEvent(FALSE, APPEND_TO_LOG, DND_LOG_EVENT_TYPE_CHARACTER_CAST_SPELL, m_pCharacter->m_szCharacterName, pPartyDlg->m_pParty->m_dwPartyID, 0L, pSpellSlot->m_pSpell->m_szSpellName);
				}

				m_pBaseCharViewDialog->m_nCastSpellCursorPos = -1;
			
				if(m_pPartyDialog->m_pInitiativeDialog != NULL)
				{
					m_pPartyDialog->m_pInitiativeDialog->Refresh();
				}

				if(m_pBaseCharViewDialog->m_pInventoryDialog != NULL)
				{
					m_pBaseCharViewDialog->m_pInventoryDialog->RefreshAll();
				}
			}
		}
	}
	
	CDialog::OnOK();
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

