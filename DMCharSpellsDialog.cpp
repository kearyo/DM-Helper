// DMCharSpellsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DM HelperDlg.h"
#include "DMInventoryDialog.h"
#include "DMCharSpellsDialog.h"
#include "DMCharViewDialog.h"
#include "DMCharSheetDialog.h"
#include "DMPartyDialog.h"
#include "DMSpellDescDialog.h"
#include "DMCharacterSelectorDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DMCharSpellsDialog dialog


DMCharSpellsDialog::DMCharSpellsDialog(cDNDCharacter *_Character, CDMCharViewDialog *_SiblingWindow, CWnd* pParent /*=NULL*/)
	: CDialog(DMCharSpellsDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(DMCharSpellsDialog)
	m_szSpellsRemaining = _T("");
	m_szSpellsRemainingSpellbook = _T("");
	//}}AFX_DATA_INIT


	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_pParent = pParent;

	m_pSiblingWindow = _SiblingWindow;

	m_pCharacter = _Character;

	m_nBaseSpellLevel = -1;
	
	m_nSelectedSpellClass = 0;
	m_nSelectedSpellMode = 0;
	m_nSelectedSpellLevel = 1;

	m_ViewSpellMode = DND_VIEW_SPELL_MODE_UNDEF;
	m_pSelectedSpell = NULL;
	m_pCurrentSpell = NULL;
	m_pSpellBook = NULL;
	m_bMageDisplay = FALSE;
	m_bRefreshed = FALSE;

	m_nMultipleSpells = 1;

	m_nSpellChartListTopIndex = -1;


	ClearSpellList(m_nSelectedSpellLists[DND_SPELL_LIST]);
	ClearSpellList(m_nSelectedSpellLists[DND_SPELLBOOK_LIST]);
	ClearSpellList(m_nSelectedSpellLists[DND_SPELLCAST_LIST]);

	Create(DMCharSpellsDialog::IDD, pParent);
}


void DMCharSpellsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DMCharSpellsDialog)
	DDX_Control(pDX, IDC_SPELLBOOK_STATIC, m_cSpellbookStatic);
	DDX_Control(pDX, IDC_SPELLS_REMAINING_SPELLBOOK, m_cSpellsRemainSpellbook);
	DDX_Control(pDX, IDC_SPELLS_MEMORIZED_LIST, m_cSpellsMemorizedList);
	DDX_Control(pDX, IDC_SPELL_BOOK_LIST, m_cSpellBookList);
	DDX_Control(pDX, IDC_STORE_SPELLS, m_cSaveSpellsButton);
	DDX_Control(pDX, IDC_RESTORE_SPELLS, m_cRecoverSpellsButton);
	DDX_Control(pDX, IDC_SPELLS_MODE_TAB, m_cSpellsModeTab);
	DDX_Control(pDX, IDC_SPELLS_TOP_TAB, m_cSpellsTopTab);
	DDX_Control(pDX, IDC_SPELLS_LEVEL_TAB, m_cSpellsLevelTab);
	DDX_Control(pDX, IDC_SPELLS_STATIC, m_cSpellsStatic);
	DDX_Control(pDX, IDC_FORGET_SPELL, m_cForgetSpellButton);
	DDX_Control(pDX, IDC_MEMORIZE_SPELL, m_cMemorizeSpellButton);
	DDX_Control(pDX, IDC_SPELL_CHART_LIST, m_cSpellChartList);
	DDX_Text(pDX, IDC_SPELLS_REMAINING, m_szSpellsRemaining);
	DDX_Text(pDX, IDC_SPELLS_REMAINING_SPELLBOOK, m_szSpellsRemainingSpellbook);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_MATERIAL_COMPONENTS, m_cMaterialComponentsButton);
}


BEGIN_MESSAGE_MAP(DMCharSpellsDialog, CDialog)
	//{{AFX_MSG_MAP(DMCharSpellsDialog)
	ON_WM_PAINT()
	ON_NOTIFY(TCN_SELCHANGE, IDC_SPELLS_TOP_TAB, OnSelchangeSpellsTopTab)
	ON_NOTIFY(TCN_SELCHANGE, IDC_SPELLS_MODE_TAB, OnSelchangeSpellsModeTab)
	ON_NOTIFY(TCN_SELCHANGE, IDC_SPELLS_LEVEL_TAB, OnSelchangeSpellsLevelTab)
	ON_NOTIFY(NM_CLICK, IDC_SPELL_CHART_LIST, OnClickSpellChartList)
	ON_NOTIFY(NM_DBLCLK, IDC_SPELL_CHART_LIST, OnDblclkSpellChartList)
	ON_BN_CLICKED(IDC_MEMORIZE_SPELL, OnMemorizeSpell)
	ON_BN_CLICKED(IDC_FORGET_SPELL, OnForgetSpell)
	ON_BN_CLICKED(IDC_STORE_SPELLS, OnStoreSpells)
	ON_BN_CLICKED(IDC_RESTORE_SPELLS, OnRestoreSpells)
	ON_LBN_SELCHANGE(IDC_SPELL_BOOK_LIST, OnSelchangeSpellBookList)
	ON_LBN_SELCHANGE(IDC_SPELLS_MEMORIZED_LIST, OnSelchangeSpellsMemorizedList)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_PING_BUTTON, OnPingButton)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &DMCharSpellsDialog::OnBnClickedOk)
	ON_WM_SETFOCUS()
	ON_WM_UPDATEUISTATE()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_MATERIAL_COMPONENTS, &DMCharSpellsDialog::OnBnClickedMaterialComponents)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DMCharSpellsDialog message handlers

BOOL DMCharSpellsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int nCount = 0;
	m_cSpellChartList.InsertColumn( nCount++, "Spell", LVCFMT_LEFT, 128, -1 );
	m_cSpellChartList.InsertColumn( nCount++, "Components", LVCFMT_LEFT, 100, -1 );
	m_cSpellChartList.InsertColumn( nCount++, "Range", LVCFMT_LEFT, 50, -1 );
	m_cSpellChartList.InsertColumn( nCount++, "Casting Time", LVCFMT_LEFT, 100, -1 );
	m_cSpellChartList.InsertColumn( nCount++, "Duration", LVCFMT_LEFT, 100, -1 );
	m_cSpellChartList.InsertColumn( nCount++, "Save", LVCFMT_LEFT, 50, -1 );
	m_cSpellChartList.InsertColumn( nCount++, "Area of Effect", LVCFMT_LEFT, 100, -1 );


	ClearSelectedListCtrlItems(&m_cSpellChartList);

	m_dwMasterSpellListHash = m_pApp->m_dwMasterSpellListHash;

	if (g_bUseMaterialComponents == FALSE)
	{
		m_cMaterialComponentsButton.ShowWindow(SW_HIDE);
	}
	else
	{
		m_cMaterialComponentsButton.ShowWindow(SW_SHOW);
	}

	SetTimer(52249, 500, NULL);

	ShowWindow(SW_HIDE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DMCharSpellsDialog::OnOK() 
{
	if(m_pSiblingWindow != NULL)
	{
		m_pSiblingWindow->m_nPage = 0;
		ShowWindow(SW_HIDE);
		m_pSiblingWindow->ShowWindow(SW_SHOW);

		m_pSiblingWindow->InvalidateRect(NULL);
		m_pSiblingWindow->ProcessCharStats();
		m_pSiblingWindow->Refresh();
	}
	
	//CDialog::OnOK();
}

void DMCharSpellsDialog::OnCancel() 
{
	CDialog::OnCancel();
}

void DMCharSpellsDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if(m_pParent != NULL)
	{
		CRect rect;
		m_pParent->GetClientRect(&rect);

		rect.top+=20;

		SetWindowPos(NULL, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
	}
}



void DMCharSpellsDialog::InitDisplay()
{
	Refresh();
}

void DMCharSpellsDialog::Refresh()
{
	m_bRefreshed = TRUE;

	BOOL bIsSpellCaster = GetSpellClasses(m_pCharacter);

	if(!bIsSpellCaster)
	{
		m_pSpellBook = NULL;
		m_cSpellChartList.ShowWindow(SW_HIDE);
		m_cMemorizeSpellButton.ShowWindow(SW_HIDE);
		m_cForgetSpellButton.ShowWindow(SW_HIDE);
		m_cSpellsStatic.ShowWindow(SW_HIDE);
		m_cSpellsLevelTab.ShowWindow(SW_HIDE);
		m_cSpellsTopTab.ShowWindow(SW_HIDE);
		m_cSpellsModeTab.ShowWindow(SW_HIDE);
		m_cSaveSpellsButton.ShowWindow(SW_HIDE);
		m_cRecoverSpellsButton.ShowWindow(SW_HIDE);
		return;
	}
	
	m_cSpellChartList.ShowWindow(SW_SHOW);
	m_cMemorizeSpellButton.ShowWindow(SW_SHOW);
	m_cForgetSpellButton.ShowWindow(SW_SHOW);
	m_cSpellsStatic.ShowWindow(SW_SHOW);
	m_cSpellsLevelTab.ShowWindow(SW_SHOW);
	m_cSpellsTopTab.ShowWindow(SW_SHOW);
	//m_cSpellsModeTab.ShowWindow(SW_SHOW);   NO ! OBSOLETE
	m_cSaveSpellsButton.ShowWindow(SW_SHOW);
	m_cRecoverSpellsButton.ShowWindow(SW_SHOW);

	CString szTemp;

	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	m_cSpellsTopTab.DeleteAllItems();
	m_cSpellsModeTab.DeleteAllItems();

	int nTopTabs = 0;
	int nModeTabs = 0;
	int nMaxSpellLevels = 0;

	memset(m_nTabSpellIndexes, 0, 4 * sizeof(int));
	memset(m_nSpellTabIndexes, 0, 4 * sizeof(int));

	if(m_pSiblingWindow != NULL)
	{
		m_pSiblingWindow->m_nSelectedSpellViewClass = 0;
	}

	for(int i = 0; i < 4; ++i)
	{
		if(!m_pCharacter->m_SpellClasses[i] || m_pCharacter->m_SpellClasses[i] == DND_CHARACTER_CLASS_UNDEF)
		{
			continue;
		}

		switch(m_pCharacter->m_SpellClasses[i])
		{
			case DND_CHARACTER_CLASS_CLERIC:
			case DND_CHARACTER_SPELL_CLASS_PALADIN_CLERIC:
			{
				m_nTabSpellIndexes[nTopTabs] = i;
				m_nSpellTabIndexes[i] = nTopTabs;
				m_cSpellsTopTab.InsertItem(nTopTabs++, "Cleric");

				if(i == m_nTabSpellIndexes[m_nSelectedSpellClass])
				{
					nMaxSpellLevels = 7;

					m_cSpellsModeTab.InsertItem(nModeTabs++, "Spells");
					m_cSpellsModeTab.InsertItem(nModeTabs++, "Memorized");

					m_pSpellBook = pApp->m_SpellBooks.GetAt(DND_CHARACTER_CLASS_CLERIC);

					m_bMageDisplay = FALSE;
				}

				break;
			}
			case DND_CHARACTER_CLASS_DRUID:
			case DND_CHARACTER_SPELL_CLASS_RANGER_DRUID:
			{
				m_nTabSpellIndexes[nTopTabs] = i;
				m_nSpellTabIndexes[i] = nTopTabs;
				m_cSpellsTopTab.InsertItem(nTopTabs++, "Druid");
				
				if(i == m_nTabSpellIndexes[m_nSelectedSpellClass])
				{
					nMaxSpellLevels = 7;
					m_cSpellsModeTab.InsertItem(nModeTabs++, "Spells");
					m_cSpellsModeTab.InsertItem(nModeTabs++, "Memorized");

					m_pSpellBook = pApp->m_SpellBooks.GetAt(DND_CHARACTER_CLASS_DRUID);

					m_bMageDisplay = FALSE;
				}

				break;
			}
			case DND_CHARACTER_CLASS_MAGE:
			case DND_CHARACTER_SPELL_CLASS_RANGER_MAGE:
			{
				m_nTabSpellIndexes[nTopTabs] = i;
				m_nSpellTabIndexes[i] = nTopTabs;
				m_cSpellsTopTab.InsertItem(nTopTabs++, "Magic User");
				
				if(i == m_nTabSpellIndexes[m_nSelectedSpellClass])
				{
					nMaxSpellLevels = 9;
					m_cSpellsModeTab.InsertItem(nModeTabs++, "Spells");
					m_cSpellsModeTab.InsertItem(nModeTabs++, "Spell Book");
					m_cSpellsModeTab.InsertItem(nModeTabs++, "Memorized");

					m_pSpellBook = pApp->m_SpellBooks.GetAt(DND_CHARACTER_CLASS_MAGE);

					m_bMageDisplay = TRUE;
				}

				break;
			}
			case DND_CHARACTER_CLASS_ILLUSIONIST:
			{
				m_nTabSpellIndexes[nTopTabs] = i;
				m_nSpellTabIndexes[i] = nTopTabs;
				m_cSpellsTopTab.InsertItem(nTopTabs++, "Illusionist");
				
				if(i == m_nTabSpellIndexes[m_nSelectedSpellClass])
				{
					nMaxSpellLevels = 7;
					m_cSpellsModeTab.InsertItem(nModeTabs++, "Spells");
					m_cSpellsModeTab.InsertItem(nModeTabs++, "Spell Book");
					m_cSpellsModeTab.InsertItem(nModeTabs++, "Memorized");

					m_pSpellBook = pApp->m_SpellBooks.GetAt(DND_CHARACTER_CLASS_ILLUSIONIST);

					m_bMageDisplay = TRUE;
				}

				break;
			}
		}
	}

	if(m_ViewSpellMode == DND_VIEW_SPELL_MODE_UNDEF)
	{
		if(m_bMageDisplay)
			m_ViewSpellMode = DND_VIEW_SPELL_MODE_COPY_TO_SPELLBOOK;  // magic user
		else
			m_ViewSpellMode = DND_VIEW_SPELL_MODE_PRAY;  // cleric
	}

	m_nBaseSpellLevel = -1;
	int nTabs = 0;
	m_cSpellsLevelTab.DeleteAllItems();

	int nSpellsAllowed = GetSpellLevels(m_pCharacter, m_pCharacter->m_SpellClasses[m_nTabSpellIndexes[m_nSelectedSpellClass]],  m_pCharacter->m_nCastingLevels[m_nTabSpellIndexes[m_nSelectedSpellClass]], m_nSelectedSpellLevel);

	if(nSpellsAllowed == 0 && m_nSelectedSpellLevel > 0) //multiclass switch
	{
		do
		{
			m_nSelectedSpellLevel -= 1; 
			nSpellsAllowed = GetSpellLevels(m_pCharacter, m_pCharacter->m_SpellClasses[m_nTabSpellIndexes[m_nSelectedSpellClass]],  m_pCharacter->m_nCastingLevels[m_nTabSpellIndexes[m_nSelectedSpellClass]], m_nSelectedSpellLevel);
		} while(nSpellsAllowed == 0 && m_nSelectedSpellLevel > 1);
	}

	for(int kk = 0; kk <= nMaxSpellLevels; ++kk)
	{
		nSpellsAllowed = GetSpellLevels(m_pCharacter, m_pCharacter->m_SpellClasses[m_nTabSpellIndexes[m_nSelectedSpellClass]],  m_pCharacter->m_nCastingLevels[m_nTabSpellIndexes[m_nSelectedSpellClass]], kk);

		if(nSpellsAllowed)
		{
			if(m_nBaseSpellLevel == -1)
			{
				m_nBaseSpellLevel = kk;
			}
			szTemp.Format("L%d (%d)", kk, nSpellsAllowed);
			m_cSpellsLevelTab.InsertItem(nTabs++, szTemp);

			if(kk == m_nSelectedSpellLevel)
			{
				int nTotalSpellsInLevel = -1;

				if(m_bMageDisplay)  //spells in spellbook
				{
					int nKnowSpell = 0;
					int nMinSpells = 0;
					int nSpellsAllowedSpellbook = 0;

					CalculateINTAdjustments(m_pCharacter, &nKnowSpell, &nMinSpells, &nSpellsAllowedSpellbook);
					nTotalSpellsInLevel = CountSpellsInLevel(m_pCharacter->m_nSpellBooks[m_nTabSpellIndexes[m_nSelectedSpellClass]][m_nSelectedSpellLevel]);

					if(nTotalSpellsInLevel > -1)
					{
						m_szSpellsRemainingSpellbook.Format("Remaining: %d of %d", nSpellsAllowedSpellbook - nTotalSpellsInLevel, nSpellsAllowedSpellbook);
					}
					else
					{
						m_szSpellsRemainingSpellbook = _T("");
					}
				}
				else
				{
					m_szSpellsRemainingSpellbook = _T("");
				}

				nTotalSpellsInLevel = -1;
				nTotalSpellsInLevel = CountSpellsInLevel(m_pCharacter->m_nSpellsMemorized[m_nTabSpellIndexes[m_nSelectedSpellClass]][m_nSelectedSpellLevel]);
			
				if(nTotalSpellsInLevel > -1)
				{
					m_szSpellsRemaining.Format("Remaining: %d of %d", nSpellsAllowed - nTotalSpellsInLevel, nSpellsAllowed);
				}
				else
				{
					m_szSpellsRemaining = _T("");
				}
			
			}
		}
	}

	RefreshSpellControls();

	m_cSpellChartList.DeleteAllItems();
	m_cSpellBookList.ResetContent();

	//clean memorized spell data
	CleanMemorizedSpellListBox(&m_cSpellsMemorizedList);

	if(m_pSpellBook != NULL)
	{
		int nModeArray[4] = {DND_VIEW_SPELL_MODE_COPY_TO_SPELLBOOK, DND_VIEW_SPELL_MODE_MEMORIZE, DND_VIEW_SPELL_MODE_CAST, -1};

		if(!m_bMageDisplay)
		{
			 nModeArray[0] = DND_VIEW_SPELL_MODE_PRAY;
			 nModeArray[1] = DND_VIEW_SPELL_MODE_CAST;
			 nModeArray[2] = -1;
		}

		for(int nMode = 0; nMode < 4 && nModeArray[nMode] != -1; ++nMode)
		{
			int nRow = 0;
			
			BOOL bDone = FALSE;
			int nSpellCount = 0;	
			
			while(!bDone)
			{
				cDNDSpell *pSpell = NULL;
				int nMultipleSpells = 0;

				int _ViewSpellMode = nModeArray[nMode];
				
				switch(_ViewSpellMode)
				{
					case DND_VIEW_SPELL_MODE_COPY_TO_SPELLBOOK:
					case DND_VIEW_SPELL_MODE_PRAY:
					{
						pSpell = &m_pSpellBook->m_Spells[m_nSelectedSpellLevel][nRow];
						break;
					}

					case DND_VIEW_SPELL_MODE_MEMORIZE:
					{
						do
						{
							nMultipleSpells = m_pCharacter->m_nSpellBooks[m_nTabSpellIndexes[m_nSelectedSpellClass]][m_nSelectedSpellLevel][nSpellCount];
							if(nMultipleSpells > 0)
							{
								pSpell = &m_pSpellBook->m_Spells[m_nSelectedSpellLevel][nSpellCount];
							}

							++nSpellCount;

						} while(pSpell == NULL && nSpellCount < MAX_SPELLS_PER_LEVEL && bDone == FALSE);

						break;
					}

					case DND_VIEW_SPELL_MODE_CAST:
					{
						do
						{
							nMultipleSpells = m_pCharacter->m_nSpellsMemorized[m_nTabSpellIndexes[m_nSelectedSpellClass]][m_nSelectedSpellLevel][nSpellCount];
							if(nMultipleSpells > 0)
							{
								pSpell = &m_pSpellBook->m_Spells[m_nSelectedSpellLevel][nSpellCount];
								if(pSpell && !pSpell->m_bSpellValid)
								{
									m_pCharacter->m_nSpellsMemorized[m_nTabSpellIndexes[m_nSelectedSpellClass]][m_nSelectedSpellLevel][nSpellCount] = 0;
									pSpell = NULL;
								}
							}

							++nSpellCount;

						} while(pSpell == NULL && nSpellCount < MAX_SPELLS_PER_LEVEL && bDone == FALSE);
					}
				}

				if(bDone == FALSE && pSpell != NULL && pSpell->m_bSpellValid)
				{
					int nCol = 1;

					if(nMultipleSpells > 1)
					{
						szTemp.Format("%d x %s", nMultipleSpells, pSpell->m_szSpellName);
					}
					else
					{
						szTemp.Format("%s", pSpell->m_szSpellName);
					}

					switch(_ViewSpellMode)
					{
						case DND_VIEW_SPELL_MODE_COPY_TO_SPELLBOOK:
						case DND_VIEW_SPELL_MODE_PRAY:
						{
							m_cSpellChartList.InsertItem(nRow, szTemp);
							m_cSpellChartList.SetItemData(nRow, (ULONG)pSpell);

							m_cSpellChartList.SetItemText(nRow, nCol++, pSpell->m_szSpellComponents);
							m_cSpellChartList.SetItemText(nRow, nCol++, pSpell->m_szSpellRange);
							m_cSpellChartList.SetItemText(nRow, nCol++, pSpell->m_szSpellCastingTime);
							m_cSpellChartList.SetItemText(nRow, nCol++, pSpell->m_szSpellDuration);
							m_cSpellChartList.SetItemText(nRow, nCol++, pSpell->m_szSpellSavingThrow);
							m_cSpellChartList.SetItemText(nRow, nCol++, pSpell->m_szSpellAreaOfEffect);
							break;
						}
						case DND_VIEW_SPELL_MODE_CAST:
						{
							m_cSpellsMemorizedList.InsertString(nRow, szTemp);

							//hier
							PSPELLSLOT pSpellSlot = new cDNDSpellSlot(pSpell, &m_pCharacter->m_nSpellsMemorized[m_nTabSpellIndexes[m_nSelectedSpellClass]][m_nSelectedSpellLevel][pSpell->m_nSpellNumber], 1, m_pCharacter->m_nCastingLevels[m_nSelectedSpellClass], FALSE);
							m_cSpellsMemorizedList.SetItemData(nRow, (ULONG)pSpellSlot);

							break;
						}
						case DND_VIEW_SPELL_MODE_MEMORIZE:
						{
							m_cSpellBookList.InsertString(nRow, szTemp);
							m_cSpellBookList.SetItemData(nRow, (ULONG)pSpell);
							break;
						}
					}

					++nRow;
				}
				else
				{
					bDone = TRUE;
				}
			}
		};

	}

	m_cSpellsTopTab.SetCurSel(m_nSelectedSpellClass); 
	m_cSpellsModeTab.SetCurSel(m_nSelectedSpellMode);
	m_cSpellsLevelTab.SetCurSel(m_nSelectedSpellLevel - m_nBaseSpellLevel);

	SetSelectedListCtrlItems(&m_cSpellChartList, m_nSelectedSpellLists[DND_SPELL_LIST]);


	//SetSelectedListBoxItems(&m_cSpellBookList, m_nSelectedSpellLists[DND_SPELLBOOK_LIST]);
	for(i = 0; i < m_cSpellBookList.GetCount(); ++i)
	{
		cDNDSpell *pSpell = (cDNDSpell *)m_cSpellBookList.GetItemData(i);

		if(pSpell != NULL)
		{
			if(m_nSelectedSpellLists[DND_SPELLBOOK_LIST][pSpell->m_nSpellNumber])
				m_cSpellBookList.SetSel(i, TRUE);
			else
				m_cSpellBookList.SetSel(i, FALSE);
		}
	}


	//SetSelectedListBoxItems(&m_cSpellsMemorizedList, m_nSelectedSpellLists[DND_SPELLCAST_LIST]);
	for(i = 0; i < m_cSpellsMemorizedList.GetCount(); ++i)
	{
		cDNDSpellSlot *pSpellSlot = (cDNDSpellSlot *)m_cSpellsMemorizedList.GetItemData(i);

		if(pSpellSlot != NULL && pSpellSlot->m_pSpell != NULL)
		{
			if(m_nSelectedSpellLists[DND_SPELLCAST_LIST][pSpellSlot->m_pSpell->m_nSpellNumber])
				m_cSpellsMemorizedList.SetSel(i, TRUE);
			else
				m_cSpellsMemorizedList.SetSel(i, FALSE);
		}
	}


	if(m_nSpellChartListTopIndex != -1)
	{
		m_cSpellChartList.EnsureVisible(m_cSpellChartList.GetItemCount()-1, TRUE);
		m_cSpellChartList.EnsureVisible(m_nSpellChartListTopIndex, FALSE);
		m_nSpellChartListTopIndex = -1;
	}

	if(m_pSiblingWindow != NULL)
	{
		m_pSiblingWindow->m_nSelectedSpellViewClass = m_nTabSpellIndexes[m_nSelectedSpellClass];
	}

	UpdateData(FALSE);

}

void DMCharSpellsDialog::RefreshSpellControls()
{

	if(m_ViewSpellMode == DND_VIEW_SPELL_MODE_UNDEF)
	{
		if(m_bMageDisplay)
			m_ViewSpellMode = DND_VIEW_SPELL_MODE_COPY_TO_SPELLBOOK;  // magic user
		else
			m_ViewSpellMode = DND_VIEW_SPELL_MODE_PRAY;  // cleric
	}

	if(m_bMageDisplay)
	{
		m_cSpellbookStatic.ShowWindow(SW_SHOW);
		m_cSpellBookList.ShowWindow(SW_SHOW);
	}
	else
	{
		m_cSpellbookStatic.ShowWindow(SW_HIDE);
		m_cSpellBookList.ShowWindow(SW_HIDE);
	}


	m_cSaveSpellsButton.EnableWindow(FALSE);
	m_cRecoverSpellsButton.EnableWindow(FALSE);

	switch(m_ViewSpellMode)
	{
		case DND_VIEW_SPELL_MODE_COPY_TO_SPELLBOOK:
		{
			m_cMemorizeSpellButton.SetWindowText("Copy To Book");

			if(GetSelectedListCtrlItem(&m_cSpellChartList) >= 0)
			{
				m_cMemorizeSpellButton.EnableWindow(TRUE);
			}
			else
			{
				m_cMemorizeSpellButton.EnableWindow(FALSE);
			}

			m_cForgetSpellButton.SetWindowText(" ");
			m_cForgetSpellButton.EnableWindow(FALSE);
	
			break;
		}
		case DND_VIEW_SPELL_MODE_MEMORIZE:
		{
			m_cMemorizeSpellButton.SetWindowText("Memorize");

			if(GetSelectedListBoxItemsCount(&m_cSpellBookList))
			{
				m_cMemorizeSpellButton.EnableWindow(TRUE);
				m_cForgetSpellButton.EnableWindow(TRUE);
			}
			else
			{
				m_cMemorizeSpellButton.EnableWindow(FALSE);
				m_cForgetSpellButton.EnableWindow(FALSE);
			}

			m_cForgetSpellButton.SetWindowText("Erase");

			break;
		}
		case DND_VIEW_SPELL_MODE_PRAY:
		{
			m_cMemorizeSpellButton.SetWindowText("Pray");

			if(GetSelectedListCtrlItem(&m_cSpellChartList) >= 0)
			{
				m_cMemorizeSpellButton.EnableWindow(TRUE);
			}
			else
			{
				m_cMemorizeSpellButton.EnableWindow(FALSE);
			}

			m_cForgetSpellButton.SetWindowText(" ");
			m_cForgetSpellButton.EnableWindow(FALSE);
			
			break;
		}
		case DND_VIEW_SPELL_MODE_CAST:
		{
			m_cMemorizeSpellButton.SetWindowText("Cast");

			if(GetSelectedListBoxItemsCount(&m_cSpellsMemorizedList))
			{
				m_cMemorizeSpellButton.EnableWindow(TRUE);
				m_cForgetSpellButton.EnableWindow(TRUE);
			}
			else
			{
				m_cMemorizeSpellButton.EnableWindow(FALSE);
				m_cForgetSpellButton.EnableWindow(FALSE);
			}

			m_cForgetSpellButton.SetWindowText("Forget");

			m_cSaveSpellsButton.EnableWindow(TRUE);
			m_cRecoverSpellsButton.EnableWindow(TRUE);
			
			break;
		}
	}

	if (m_pCurrentSpell == NULL)
	{
		m_cMaterialComponentsButton.EnableWindow(FALSE);
	}
	else
	{
		m_cMaterialComponentsButton.EnableWindow(TRUE);
	}
}

void DMCharSpellsDialog::ClearSpellList(int * pnSpellList)
{
	memset(pnSpellList, 0, MAX_SPELLS_PER_LEVEL * sizeof(int));
}

void DMCharSpellsDialog::OnSelchangeSpellsTopTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ClearSelectedListCtrlItems(&m_cSpellChartList);
	m_cMemorizeSpellButton.EnableWindow(FALSE);
	
	*pResult = 0;

	ClearSpellList(m_nSelectedSpellLists[DND_SPELL_LIST]);
	ClearSpellList(m_nSelectedSpellLists[DND_SPELLBOOK_LIST]);
	ClearSpellList(m_nSelectedSpellLists[DND_SPELLCAST_LIST]);
	m_nSelectedSpellClass = m_cSpellsTopTab.GetCurSel(); 
	

	m_ViewSpellMode = DND_VIEW_SPELL_MODE_UNDEF;

	Refresh();

	if(m_pSiblingWindow != NULL)
	{
		if(m_pSiblingWindow->m_pCharSheetDialog != NULL)
		{
			m_pSiblingWindow->m_pCharSheetDialog->InvalidateRect(NULL);
		}
	}

}

void DMCharSpellsDialog::OnSelchangeSpellsModeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ClearSelectedListCtrlItems(&m_cSpellChartList);
	m_cMemorizeSpellButton.EnableWindow(FALSE);

	ClearSpellList(m_nSelectedSpellLists[DND_SPELL_LIST]);
	ClearSpellList(m_nSelectedSpellLists[DND_SPELLBOOK_LIST]);
	ClearSpellList(m_nSelectedSpellLists[DND_SPELLCAST_LIST]);
	
	int nTabs = m_cSpellsModeTab.GetItemCount();

	m_nSelectedSpellMode = m_cSpellsModeTab.GetCurSel();

	m_pSelectedSpell = NULL;
	m_pCurrentSpell = NULL;

	switch(m_ViewSpellMode)
	{
		case DND_VIEW_SPELL_MODE_COPY_TO_SPELLBOOK:
		{
			if(m_nSelectedSpellMode == 1)
			{
				m_ViewSpellMode = DND_VIEW_SPELL_MODE_MEMORIZE;
			}
			else if(m_nSelectedSpellMode == 2)
			{
				m_ViewSpellMode = DND_VIEW_SPELL_MODE_CAST;
			}
	
			break;
		}
		case DND_VIEW_SPELL_MODE_MEMORIZE:
		{
			if(m_nSelectedSpellMode == 0)
			{
				m_ViewSpellMode = DND_VIEW_SPELL_MODE_COPY_TO_SPELLBOOK;
			}
			else if(m_nSelectedSpellMode == 2)
			{
				m_ViewSpellMode = DND_VIEW_SPELL_MODE_CAST;
			}
	
			break;
		}
		case DND_VIEW_SPELL_MODE_PRAY:
		{
			if(m_nSelectedSpellMode == 1)
			{
				m_ViewSpellMode = DND_VIEW_SPELL_MODE_CAST;
			}
			
			break;
		}
		case DND_VIEW_SPELL_MODE_CAST:
		{
			if(m_nSelectedSpellMode == 0)
			{
				m_ViewSpellMode = DND_VIEW_SPELL_MODE_UNDEF;
			}
			else if(m_nSelectedSpellMode == 1) // can only be hit by a mage
			{
				m_ViewSpellMode = DND_VIEW_SPELL_MODE_MEMORIZE;
			}

			break;
		}
	}


	*pResult = 0;

	Refresh();

}

void DMCharSpellsDialog::OnSelchangeSpellsLevelTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	ClearSelectedListCtrlItems(&m_cSpellChartList);
	m_cMemorizeSpellButton.EnableWindow(FALSE);

	ClearSpellList(m_nSelectedSpellLists[DND_SPELL_LIST]);
	ClearSpellList(m_nSelectedSpellLists[DND_SPELLBOOK_LIST]);
	ClearSpellList(m_nSelectedSpellLists[DND_SPELLCAST_LIST]);

	m_nSelectedSpellLevel = m_cSpellsLevelTab.GetCurSel() + m_nBaseSpellLevel;

	*pResult = 0;

	Refresh();
}


void DMCharSpellsDialog::OnClickSpellChartList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//UpdateData(TRUE);

	int nCursor = GetSelectedListCtrlItem(&m_cSpellChartList);

	if(nCursor >= 0)
	{
		m_nMultipleSpells = 1;

		m_nSpellChartListTopIndex = m_cSpellChartList.GetTopIndex();

		switch(m_ViewSpellMode)
		{
			case DND_VIEW_SPELL_MODE_MEMORIZE: m_ViewSpellMode = DND_VIEW_SPELL_MODE_COPY_TO_SPELLBOOK; break;
			case DND_VIEW_SPELL_MODE_CAST: m_ViewSpellMode = DND_VIEW_SPELL_MODE_UNDEF; break;
		}

		ClearSpellList(m_nSelectedSpellLists[DND_SPELL_LIST]);
		ClearSpellList(m_nSelectedSpellLists[DND_SPELLBOOK_LIST]);
		ClearSpellList(m_nSelectedSpellLists[DND_SPELLCAST_LIST]);

		m_pSelectedSpell = (cDNDSpell *)m_cSpellChartList.GetItemData(nCursor);
		m_pCurrentSpell = m_pSelectedSpell;

		if(m_pSelectedSpell != NULL)
		{
			m_nSelectedSpellLists[DND_SPELL_LIST][m_pSelectedSpell->m_nSpellNumber] = 1;
		}	

		POSITION pos = m_cSpellChartList.GetFirstSelectedItemPosition();

		int selected=-1;

		if(pos != NULL)
		{
			while (pos)
			{
				int nItem = m_cSpellChartList.GetNextSelectedItem(pos);

				cDNDSpell *pSpell = (cDNDSpell *)m_cSpellChartList.GetItemData(nItem);

				if(pSpell != NULL)
				{
					m_nSelectedSpellLists[DND_SPELL_LIST][pSpell->m_nSpellNumber] = 1;
				}

				selected = nItem;
			}
		}

		Refresh();

		//m_cSpellChartList.SetItemState(nCursor, LVIS_SELECTED, LVIS_SELECTED);
	}
	else
	{
		m_nSpellChartListTopIndex = -1;
	}
	
	*pResult = 0;
}
 
void DMCharSpellsDialog::OnDblclkSpellChartList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	UpdateData(TRUE);

	int nCursor = GetSelectedListCtrlItem(&m_cSpellChartList);

	if(nCursor != -1)
	{
		m_nMultipleSpells = 1;
		cDNDSpell *pSpell = (cDNDSpell *)m_cSpellChartList.GetItemData(nCursor);
		DMSpellDescDialog *pDlg = new DMSpellDescDialog(pSpell);
		pDlg->DoModal();
		delete pDlg;
	}
	
	*pResult = 0;
}

void DMCharSpellsDialog::OnSelchangeSpellBookList() 
{
	int nCursor = m_cSpellBookList.GetCurSel();

	if(nCursor >= 0)
	{
		ClearSpellList(m_nSelectedSpellLists[DND_SPELL_LIST]);
		ClearSpellList(m_nSelectedSpellLists[DND_SPELLCAST_LIST]);

		m_ViewSpellMode = DND_VIEW_SPELL_MODE_MEMORIZE;

		cDNDSpell *pSelectedSpell = (cDNDSpell *)m_cSpellBookList.GetItemData(nCursor);
		m_nMultipleSpells = 1;
		
		if(pSelectedSpell != NULL)
		{
			m_nSelectedSpellLists[DND_SPELLBOOK_LIST][pSelectedSpell->m_nSpellNumber] = 1 - m_nSelectedSpellLists[DND_SPELLBOOK_LIST][pSelectedSpell->m_nSpellNumber];
			m_pCurrentSpell = pSelectedSpell;
		}

		Refresh();
	}

	m_cSpellBookList.SetCurSel(-1);

}

void DMCharSpellsDialog::OnSelchangeSpellsMemorizedList() 
{
	int nCursor = m_cSpellsMemorizedList.GetCurSel();

	if (nCursor >= 0)
	{
		ClearSpellList(m_nSelectedSpellLists[DND_SPELL_LIST]);
		ClearSpellList(m_nSelectedSpellLists[DND_SPELLBOOK_LIST]);

		m_ViewSpellMode = DND_VIEW_SPELL_MODE_CAST;
	
		cDNDSpellSlot *pSelectedSpellSlot = (cDNDSpellSlot *)m_cSpellsMemorizedList.GetItemData(nCursor);
		
		if(pSelectedSpellSlot)
		{
			cDNDSpell *pSelectedSpell = pSelectedSpellSlot->m_pSpell;

			if(pSelectedSpell != NULL)
			{
				m_nSelectedSpellLists[DND_SPELLCAST_LIST][pSelectedSpell->m_nSpellNumber] = 1 - m_nSelectedSpellLists[DND_SPELLCAST_LIST][pSelectedSpell->m_nSpellNumber];

				UpdateData(TRUE);

				if (m_cSpellsMemorizedList.GetSel(nCursor))
				{
					m_pCurrentSpell = pSelectedSpell;

					if (pSelectedSpellSlot->m_pnMemorizedSlot != NULL)
					{
						m_nMultipleSpells = *pSelectedSpellSlot->m_pnMemorizedSlot;
					}
					else
					{
						m_nMultipleSpells = 1;
					}
				}
				else
				{
					m_pCurrentSpell = NULL;
				}
			}
		}

		Refresh();
	}
	else
	{
		m_pCurrentSpell = NULL;
		Refresh();
	}

	m_cSpellsMemorizedList.SetCurSel(-1);
}


void DMCharSpellsDialog::OnMemorizeSpell() 
{

	int nSpellsAllowedForLevel = GetSpellLevels(m_pCharacter, m_pCharacter->m_SpellClasses[m_nTabSpellIndexes[m_nSelectedSpellClass]],  m_pCharacter->m_nCastingLevels[m_nTabSpellIndexes[m_nSelectedSpellClass]], m_nSelectedSpellLevel);

	switch(m_ViewSpellMode)
	{
		case DND_VIEW_SPELL_MODE_PRAY:
		{
			int nTotalMemorizedSpellsInLevel = CountSpellsInLevel(m_pCharacter->m_nSpellsMemorized[m_nTabSpellIndexes[m_nSelectedSpellClass]][m_nSelectedSpellLevel]);

			for(int i = 0; i < MAX_SPELLS_PER_LEVEL && nTotalMemorizedSpellsInLevel < nSpellsAllowedForLevel; ++i)
			{
				if(m_nSelectedSpellLists[DND_SPELL_LIST][i] == 1)
				{
					m_pCharacter->m_nSpellsMemorized[m_nTabSpellIndexes[m_nSelectedSpellClass]][m_nSelectedSpellLevel][i] += 1;
					nTotalMemorizedSpellsInLevel = CountSpellsInLevel(m_pCharacter->m_nSpellsMemorized[m_nTabSpellIndexes[m_nSelectedSpellClass]][m_nSelectedSpellLevel]);	
				}
			}

			break;
		}
		case DND_VIEW_SPELL_MODE_MEMORIZE:
		{
			int nTotalMemorizedSpellsInLevel = CountSpellsInLevel(m_pCharacter->m_nSpellsMemorized[m_nTabSpellIndexes[m_nSelectedSpellClass]][m_nSelectedSpellLevel]);

			for(int i = 0; i < MAX_SPELLS_PER_LEVEL && nTotalMemorizedSpellsInLevel < nSpellsAllowedForLevel; ++i)
			{
				if(m_nSelectedSpellLists[DND_SPELLBOOK_LIST][i] == 1)
				{
					m_pCharacter->m_nSpellsMemorized[m_nTabSpellIndexes[m_nSelectedSpellClass]][m_nSelectedSpellLevel][i] += 1;
					nTotalMemorizedSpellsInLevel = CountSpellsInLevel(m_pCharacter->m_nSpellsMemorized[m_nTabSpellIndexes[m_nSelectedSpellClass]][m_nSelectedSpellLevel]);	
				}
			}
			break;
		}
		case DND_VIEW_SPELL_MODE_COPY_TO_SPELLBOOK:
		{
			int nKnowSpell = 0;
			int nMinSpells = 0;
			int nMaxSpells = 0;

			int nAddLang = CalculateINTAdjustments(m_pCharacter, &nKnowSpell, &nMinSpells, &nMaxSpells);

			int nTotalBookSpellsInLevel = CountSpellsInLevel(m_pCharacter->m_nSpellBooks[m_nTabSpellIndexes[m_nSelectedSpellClass]][m_nSelectedSpellLevel]);

			for(int i = 0; i < MAX_SPELLS_PER_LEVEL && nTotalBookSpellsInLevel < nMaxSpells; ++i)
			{
				if(m_nSelectedSpellLists[DND_SPELL_LIST][i] == 1)
				{
					m_pCharacter->m_nSpellBooks[m_nTabSpellIndexes[m_nSelectedSpellClass]][m_nSelectedSpellLevel][i] = 1;
					nTotalBookSpellsInLevel = CountSpellsInLevel(m_pCharacter->m_nSpellBooks[m_nTabSpellIndexes[m_nSelectedSpellClass]][m_nSelectedSpellLevel]);
				}
			}

			break;
		}
		case DND_VIEW_SPELL_MODE_CAST:
		{
			/*
			for(int i = 0; i < MAX_SPELLS_PER_LEVEL; ++i)
			{
				if(m_nSelectedSpellLists[DND_SPELLCAST_LIST][i] == 1)
				{
					//m_pSpellBook;
					TRACE("WAKKA WAKKA !\n");

					int nCursor = m_cSpellsMemorizedList.GetCurSel();

					if(nCursor >= 0)
					{
						ClearSpellList(m_nSelectedSpellLists[DND_SPELL_LIST]);
						ClearSpellList(m_nSelectedSpellLists[DND_SPELLBOOK_LIST]);

						m_ViewSpellMode = DND_VIEW_SPELL_MODE_CAST;
					
						
					}

					//CAST ME !!!!
				
					m_pCharacter->m_nSpellsMemorized[m_nSelectedSpellClass][m_nSelectedSpellLevel][i] -= 1;
					if(m_pCharacter->m_nSpellsMemorized[m_nSelectedSpellClass][m_nSelectedSpellLevel][i] < 0)
					{
						m_pCharacter->m_nSpellsMemorized[m_nSelectedSpellClass][m_nSelectedSpellLevel][i] = 0;
					}
				}
			}
			*/
			
			for(int i = 0; i < m_cSpellsMemorizedList.GetCount(); ++i)
			{
				if(m_cSpellsMemorizedList.GetSel(i))
				{
					cDNDSpellSlot *pSelectedSpellSlot = (cDNDSpellSlot *)m_cSpellsMemorizedList.GetItemData(i);

					DND_SPELL_MATERIAL_RETURN_CODES nRetCode = m_pSiblingWindow->CharacterCanCastSpell(pSelectedSpellSlot->m_pSpell, 1, FALSE, FALSE, FALSE);

					if (nRetCode != DND_SPELL_MATERIAL_RETURN_CANNOT_CAST)
					{
						m_pSiblingWindow->CharacterCanCastSpell(pSelectedSpellSlot->m_pSpell, 1, TRUE, FALSE, FALSE);
						m_pCharacter->CastSpell(pSelectedSpellSlot);

						if (m_pApp->SpellIsHealingSpell(pSelectedSpellSlot->m_pSpell))
						{
							DWORD dwCharacterID = 0;
							DMCharacterSelectorDialog *pDlg = new DMCharacterSelectorDialog(&dwCharacterID, 0, DND_SELECTOR_CHARACTER);
							pDlg->DoModal();

							if (dwCharacterID)
							{
								m_pApp->HealCharacter(dwCharacterID);
							}
						}

						if (pSelectedSpellSlot->m_pSpell->HasVerbalComponent())
						{
							m_pApp->PlayPCSoundFX("* PC Cast Spell", m_pSiblingWindow->m_szCharacterFirstName, "NADA", FALSE, pSelectedSpellSlot->m_pSpell->m_nSpellIdentifier);
						}

						int nSoundRepeats = m_pApp->GetSpellRepeats(pSelectedSpellSlot);
						m_pApp->PlaySpellSFX(pSelectedSpellSlot->m_pSpell->m_nSpellIdentifier, nSoundRepeats);

						DMPartyDialog *pPartyDlg = m_pApp->FindCharacterPartyDialog(m_pCharacter);
						if (pPartyDlg != NULL)
						{
							pPartyDlg->LogPartyEvent(FALSE, APPEND_TO_LOG, DND_LOG_EVENT_TYPE_CHARACTER_CAST_SPELL, m_pCharacter->m_szCharacterName, pPartyDlg->m_pParty->m_dwPartyID, 0L, pSelectedSpellSlot->m_pSpell->m_szSpellName);
						}
					}
				}
			}

			break;
		}

	}

	Refresh();

	m_pCharacter->MarkChanged();

}

void DMCharSpellsDialog::OnForgetSpell() 
{
	switch(m_ViewSpellMode)
	{
		case DND_VIEW_SPELL_MODE_MEMORIZE:
		{
			if(AfxMessageBox("Are you sure you wish erase these spells from the spellbook ?", MB_ICONQUESTION|MB_OKCANCEL) == IDOK)
			{
				for(int i = 0; i < MAX_SPELLS_PER_LEVEL; ++i)
				{
					if(m_nSelectedSpellLists[DND_SPELLBOOK_LIST][i] == 1)
					{
						m_pCharacter->m_nSpellBooks[m_nTabSpellIndexes[m_nSelectedSpellClass]][m_nSelectedSpellLevel][i] -= 1;
						if(m_pCharacter->m_nSpellBooks[m_nTabSpellIndexes[m_nSelectedSpellClass]][m_nSelectedSpellLevel][i] < 0)
						{
							m_pCharacter->m_nSpellBooks[m_nTabSpellIndexes[m_nSelectedSpellClass]][m_nSelectedSpellLevel][i] = 0;
						}
					}
				}

				m_pCharacter->MarkChanged();
			}
			break;
		}
		case DND_VIEW_SPELL_MODE_CAST:
		{
			for(int i = 0; i < MAX_SPELLS_PER_LEVEL; ++i)
			{
				if(m_nSelectedSpellLists[DND_SPELLCAST_LIST][i] == 1)
				{
					m_pCharacter->m_nSpellsMemorized[m_nTabSpellIndexes[m_nSelectedSpellClass]][m_nSelectedSpellLevel][i] -= 1;
					if(m_pCharacter->m_nSpellsMemorized[m_nTabSpellIndexes[m_nSelectedSpellClass]][m_nSelectedSpellLevel][i] < 0)
					{
						m_pCharacter->m_nSpellsMemorized[m_nTabSpellIndexes[m_nSelectedSpellClass]][m_nSelectedSpellLevel][i] = 0;
					}

					m_nMultipleSpells = m_pCharacter->m_nSpellsMemorized[m_nTabSpellIndexes[m_nSelectedSpellClass]][m_nSelectedSpellLevel][i];

					m_pCharacter->MarkChanged();
				}
			}

			break;
		}

	}

	Refresh();
	
}

void DMCharSpellsDialog::OnStoreSpells() 
{
	for(int i = 0; i < MAX_SPELL_LEVELS; ++ i)
	{
		for(int j = 0; j < MAX_SPELLS_PER_LEVEL; ++j)
		{
			m_pCharacter->m_nLastSpellsMemorized[m_nTabSpellIndexes[m_nSelectedSpellClass]][i][j] = m_pCharacter->m_nSpellsMemorized[m_nTabSpellIndexes[m_nSelectedSpellClass]][i][j];
		}
	}

	AfxMessageBox("Spell selection saved for later recovery !", MB_ICONEXCLAMATION|MB_OK);

	m_pCharacter->MarkChanged();
}

void DMCharSpellsDialog::OnRestoreSpells() 
{
	for(int i = 0; i < MAX_SPELL_LEVELS; ++ i)
	{
		for(int j = 0; j < MAX_SPELLS_PER_LEVEL; ++j)
		{
			m_pCharacter->m_nSpellsMemorized[m_nTabSpellIndexes[m_nSelectedSpellClass]][i][j] = m_pCharacter->m_nLastSpellsMemorized[m_nTabSpellIndexes[m_nSelectedSpellClass]][i][j];
		}
	}

	AfxMessageBox("Spells recovered from previous selection !", MB_ICONEXCLAMATION|MB_OK);

	Refresh();

	m_pCharacter->MarkChanged();
}

void DMCharSpellsDialog::OnBnClickedMaterialComponents()
{
	if (m_pCurrentSpell != NULL)
	{
		m_pSiblingWindow->CharacterCanCastSpell(m_pCurrentSpell, m_nMultipleSpells, FALSE, TRUE, TRUE);
	}
}


/*

void CDMCharViewDialog::OnCustomdrawSpellList ( NMHDR* pNMHDR, LRESULT* pResult )
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

    // Take the default processing unless we 
    // set this to something else below.
    *pResult = CDRF_DODEFAULT;

    // First thing - check the draw stage. If it's the control's prepaint
    // stage, then tell Windows we want messages for every item.

    if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
    {
		*pResult = CDRF_NOTIFYITEMDRAW;
    }
    else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
    {
		// This is the prepaint stage for an item. Here's where we set the
		// item's text color. Our return value will tell Windows to draw the
		// item itself, but it will use the new color we set here.
		// We'll cycle the colors through red, green, and light blue.

		COLORREF crText;

		crText = RGB(0,0,0);
		switch(m_ViewSpellMode)
		{
			case DND_VIEW_SPELL_MODE_MEMORIZE:
			{
				crText = RGB(200,0,0);
				break;
			}
			case DND_VIEW_SPELL_MODE_CAST:
			{
				crText = RGB(0,200,0);
				break;
			}
		}

		// Store the color back in the NMLVCUSTOMDRAW struct.
		pLVCD->clrText = crText;

		// Tell Windows to paint the control itself.
		*pResult = CDRF_DODEFAULT;
    }

}

  */


void DMCharSpellsDialog::OnClose() 
{
	//clean memorized spell data
	CleanMemorizedSpellListBox(&m_cSpellsMemorizedList);

	if(m_pSiblingWindow != NULL)
	{
		m_pSiblingWindow->m_bFirstSpellViewRefresh = TRUE;
	}
	
	KillTimer(52249);

	//CDialog::OnClose();

	delete this;
}

void DMCharSpellsDialog::PostNcDestroy() 
{

	CDialog::PostNcDestroy();

	//delete this;
}

void DMCharSpellsDialog::OnPingButton() 
{
	
	
}


void DMCharSpellsDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	CleanMemorizedSpellListBox(&m_cSpellsMemorizedList);
}

void DMCharSpellsDialog::OnBnClickedOk()
{
	if(m_pSiblingWindow != NULL)
	{
		m_pSiblingWindow->m_pSpellsDialog = NULL;
	}

	PostMessage(WM_CLOSE);

	OnOK();
}

void DMCharSpellsDialog::OnSetFocus(CWnd* pOldWnd)
{
	CDialog::OnSetFocus(pOldWnd);

	// TODO: Add your message handler code here
}

HBRUSH DMCharSpellsDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here

	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void DMCharSpellsDialog::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 52249)
	{
		// TRACE("PING 52249 %s\n", m_pCharacter->m_szCharacterName);
		if(m_dwMasterSpellListHash != m_pApp->m_dwMasterSpellListHash)
		{
			m_dwMasterSpellListHash = m_pApp->m_dwMasterSpellListHash;
			Refresh();
		}
	}

	CDialog::OnTimer(nIDEvent);
}


