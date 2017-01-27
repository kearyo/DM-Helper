// DMScrollDetailsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMInventoryDialog.h"
#include "DMCharSpellsDialog.h"
#include "DMCharViewDialog.h"
#include "DMNPCViewDialog.h"
#include "DMScrollDetailsDialog.h"
#include "DMSpellSelectorDialog.h"


// CDMScrollDetailsDialog dialog

IMPLEMENT_DYNAMIC(CDMScrollDetailsDialog, CDialog)

CDMScrollDetailsDialog::CDMScrollDetailsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDMScrollDetailsDialog::IDD, pParent)
	, m_szChargesEdit(_T(""))
{
	m_pParentWindow = (DMInventoryDialog *)pParent;

	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_nSelectedSpellIndex = 0;

	Create(CDMScrollDetailsDialog::IDD, pParent);
}

CDMScrollDetailsDialog::~CDMScrollDetailsDialog()
{
}

void CDMScrollDetailsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCROLL_SPELL_LIST, m_cScrollSpellList);
	DDX_Text(pDX, IDC_CHARGES_EDIT, m_szChargesEdit);
	DDV_MaxChars(pDX, m_szChargesEdit, 3);
}


BEGIN_MESSAGE_MAP(CDMScrollDetailsDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CDMScrollDetailsDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDMScrollDetailsDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_ADD_SPELL_BUTTON, &CDMScrollDetailsDialog::OnBnClickedAddSpellButton)
	ON_LBN_SELCHANGE(IDC_SCROLL_SPELL_LIST, &CDMScrollDetailsDialog::OnLbnSelchangeScrollSpellList)
	ON_BN_CLICKED(IDC_DELETE_SPELL_BUTTON, &CDMScrollDetailsDialog::OnBnClickedDeleteSpellButton)
	ON_BN_CLICKED(IDC_CAST_BUTTON, &CDMScrollDetailsDialog::OnBnClickedCastButton)
	ON_BN_CLICKED(IDC_COPY_SPELL_BUTTON, &CDMScrollDetailsDialog::OnBnClickedCopySpellButton)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_CHARGES_EDIT, &CDMScrollDetailsDialog::OnEnChangeChargesEdit)
END_MESSAGE_MAP()


// CDMScrollDetailsDialog message handlers


BOOL CDMScrollDetailsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	int nOffset = 0;

	if(m_pParentWindow->m_pCharacter->m_bIsCache)
	{
		nOffset = 46;
	}

	SetWindowPos(NULL, m_pParentWindow->m_nDetailsPosX, m_pParentWindow->m_nDetailsPosY+nOffset, 0, 0, SW_SHOW);
	ShowWindow(SW_SHOW);

	m_cScrollSpellList.SetHorizontalExtent(1000);

	Refresh();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDMScrollDetailsDialog::UpdateButtons()
{
	if(m_nSelectedSpellIndex)
	{
		(GetDlgItem( IDC_CAST_BUTTON ))-> EnableWindow(TRUE);
		(GetDlgItem( IDC_COPY_SPELL_BUTTON ))-> EnableWindow(TRUE);
		(GetDlgItem( IDC_DELETE_SPELL_BUTTON ))-> EnableWindow(TRUE);
	}
	else
	{
		(GetDlgItem( IDC_CAST_BUTTON ))-> EnableWindow(FALSE);
		(GetDlgItem( IDC_COPY_SPELL_BUTTON ))-> EnableWindow(FALSE);
		(GetDlgItem( IDC_DELETE_SPELL_BUTTON ))-> EnableWindow(FALSE);
	}


	if(m_pParentWindow == NULL || m_pParentWindow->m_pSelectedInventorySlot == NULL)
	{
		return;
	}

	DND_CHARACTER_CLASSES _MagicType = DND_CHARACTER_CLASS_UNDEF;

	int nFlags = m_pParentWindow->m_pSelectedInventorySlot->m_nFlags;
	nFlags = ((nFlags / 100) % 100) % 10;

	switch(nFlags)
	{
		case 1:	_MagicType = DND_CHARACTER_CLASS_CLERIC; break;
		case 2:	_MagicType = DND_CHARACTER_CLASS_DRUID; break;
		case 3:	_MagicType = DND_CHARACTER_CLASS_ILLUSIONIST; break;
		case 4:	_MagicType = DND_CHARACTER_CLASS_MAGE; break;
	}

	switch(_MagicType)
	{
		case DND_CHARACTER_CLASS_CLERIC:
		{
			(GetDlgItem( IDC_COPY_SPELL_BUTTON ))-> ShowWindow(SW_HIDE);
			break;
		}
		case DND_CHARACTER_CLASS_DRUID:
		{
			(GetDlgItem( IDC_COPY_SPELL_BUTTON ))-> ShowWindow(SW_HIDE);
			break;
		}
		case DND_CHARACTER_CLASS_ILLUSIONIST:
		{
			(GetDlgItem( IDC_COPY_SPELL_BUTTON ))-> ShowWindow(SW_SHOW);
			break;
		}
		case DND_CHARACTER_CLASS_MAGE:
		{
			(GetDlgItem( IDC_COPY_SPELL_BUTTON ))-> ShowWindow(SW_SHOW);
			break;
		}
	}

	if(!m_pParentWindow->m_pSelectedInventorySlot->IsScroll())
	{
		(GetDlgItem( IDC_COPY_SPELL_BUTTON ))-> ShowWindow(SW_HIDE);
	}

	if(m_pParentWindow->m_pCharacter->m_bIsCache)
	{
		(GetDlgItem( IDC_CAST_BUTTON ))-> ShowWindow(SW_HIDE);
		(GetDlgItem( IDC_COPY_SPELL_BUTTON ))-> ShowWindow(SW_HIDE);
	}

}

void CDMScrollDetailsDialog::Refresh()
{

	if(m_pParentWindow == NULL || m_pParentWindow->m_pSelectedInventorySlot == NULL)
	{
		return;
	}

	BOOL bCalcFailure = FALSE;

	CString szTemp;

	if(m_pParentWindow->m_pSelectedInventorySlot->IsMagicRod())
	{
		szTemp = _T("Rod Details");

		(GetDlgItem( IDC_CHARGES_STATIC ))-> ShowWindow(SW_SHOW);
		(GetDlgItem( IDC_CHARGES_EDIT ))-> ShowWindow(SW_SHOW);
	}
	else if(m_pParentWindow->m_pSelectedInventorySlot->IsMagicStaff())
	{
		szTemp = _T("Staff Details");

		(GetDlgItem( IDC_CHARGES_STATIC ))-> ShowWindow(SW_SHOW);
		(GetDlgItem( IDC_CHARGES_EDIT ))-> ShowWindow(SW_SHOW);
	}
	else if(m_pParentWindow->m_pSelectedInventorySlot->IsMagicWand())
	{
		szTemp = _T("Wand Details");

		(GetDlgItem( IDC_CHARGES_STATIC ))-> ShowWindow(SW_SHOW);
		(GetDlgItem( IDC_CHARGES_EDIT ))-> ShowWindow(SW_SHOW);
	}
	else if(m_pParentWindow->m_pSelectedInventorySlot->IsMagicRing())
	{
		szTemp = _T("Ring Details");

		(GetDlgItem( IDC_CHARGES_STATIC ))-> ShowWindow(SW_SHOW);
		(GetDlgItem( IDC_CHARGES_EDIT ))-> ShowWindow(SW_SHOW);
	}
	else
	{
		szTemp = _T("Scroll Details");
		
		(GetDlgItem( IDC_CHARGES_STATIC ))-> ShowWindow(SW_HIDE);
		(GetDlgItem( IDC_CHARGES_EDIT ))-> ShowWindow(SW_HIDE);
		bCalcFailure = TRUE;
	}

	(GetDlgItem( IDC_SCROLL_DETAILS_STATIC ))-> SetWindowText(szTemp);

	m_szChargesEdit.Format("%d", m_pParentWindow->m_pSelectedInventorySlot->m_nCharges);

	m_cScrollSpellList.ResetContent();
	for(int i = 0; i < MAX_OBJECT_CONTENTS; ++i)
	{
		if(m_pParentWindow->m_pSelectedInventorySlot->m_nContents[i])
		{
			int nSpellIndex = m_pParentWindow->m_pSelectedInventorySlot->m_nContents[i] - 1;
			cDNDSpell *pSpell = m_pApp->m_MasterSpellArray.GetAt(nSpellIndex);

			if(pSpell != NULL)
			{
				if(m_pParentWindow->m_pSelectedInventorySlot->m_nContentsCount[i] > 1)
				{
					szTemp.Format("%d x %s [L%d]", m_pParentWindow->m_pSelectedInventorySlot->m_nContentsCount[i], pSpell->m_szSpellName, pSpell->m_nSpellLevel);
				}
				else
				{
					szTemp.Format("%s [L%d]", pSpell->m_szSpellName, pSpell->m_nSpellLevel);
				}

				if(bCalcFailure == TRUE && m_pParentWindow->m_pCharacter->m_bIsCache == FALSE)
				{
					int nTotalFailure = 0;
					int nReversed = 0;

					int nSpellFailure = GetScrollSpellFailureChance(m_pParentWindow->m_pCharacter, m_pParentWindow->m_pSelectedInventorySlot, pSpell->m_nSpellLevel, &nTotalFailure, &nReversed);

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
				}

				m_cScrollSpellList.InsertString(i, szTemp);
				m_cScrollSpellList.SetItemData(i, m_pParentWindow->m_pSelectedInventorySlot->m_nContents[i]);
			}

		}
	}

	UpdateButtons();

	UpdateData(FALSE);
}

void CDMScrollDetailsDialog::OnBnClickedOk()
{
	OnOK();
}

void CDMScrollDetailsDialog::OnBnClickedCancel()
{
	OnCancel();
}

void CDMScrollDetailsDialog::OnBnClickedAddSpellButton()
{

	DND_CHARACTER_CLASSES _MagicType = DND_CHARACTER_CLASS_UNDEF;

	int nFlags = m_pParentWindow->m_pSelectedInventorySlot->m_nFlags;
	nFlags = ((nFlags / 100) % 100) % 10;

	switch(nFlags)
	{
		case 1:	_MagicType = DND_CHARACTER_CLASS_CLERIC; break;
		case 2:	_MagicType = DND_CHARACTER_CLASS_DRUID; break;
		case 3:	_MagicType = DND_CHARACTER_CLASS_ILLUSIONIST; break;
		case 4:	_MagicType = DND_CHARACTER_CLASS_MAGE; break;
	}


	cDNDSpell *pSpell = NULL;

	int nCount = 1;

	int *pnCount = &nCount;

	if(m_pParentWindow->m_pSelectedInventorySlot->IsScroll() == FALSE)
	{
		pnCount = NULL;
	}

	CDMSpellSelectorDialog *pDlg = new CDMSpellSelectorDialog(&pSpell, _MagicType, pnCount);

	pDlg->DoModal();

	delete pDlg;

	if(pSpell != NULL)
	{
		if(m_pParentWindow->m_pSelectedInventorySlot != NULL)
		{
			m_pParentWindow->m_pSelectedInventorySlot->PushContents(pSpell->m_nSpellIdentifier+1, nCount); // zero is "bless", so we have to fudge it +1

			if(m_pParentWindow->m_pSelectedInventorySlot->UsesMagicCharges())
			{
				m_pParentWindow->m_pSelectedInventorySlot->m_nCharges += 1;
			}

			m_pParentWindow->m_pSelectedInventorySlot->m_bEnchanted = FALSE;
			m_pParentWindow->m_pCharacter->ValidateMagicContainerInventory();
			m_pParentWindow->Refresh();
			Refresh();

			if(m_pParentWindow->m_pCharacter != NULL)
			{
				m_pParentWindow->m_pCharacter->MarkChanged();
			}
		}
	}
}

void CDMScrollDetailsDialog::OnLbnSelchangeScrollSpellList()
{
	UpdateData(TRUE);

	int nCursor = m_cScrollSpellList.GetCurSel();

	if(nCursor > -1)
	{
		m_nSelectedSpellIndex = m_cScrollSpellList.GetItemData(nCursor);
	}
	else
	{
		m_nSelectedSpellIndex = 0;
	}

	UpdateButtons();
}

void CDMScrollDetailsDialog::OnBnClickedDeleteSpellButton()
{
	if(m_nSelectedSpellIndex)
	{
		m_pParentWindow->m_pSelectedInventorySlot->m_bEnchanted = FALSE;
		m_pParentWindow->m_pSelectedInventorySlot->PopContents(m_nSelectedSpellIndex);
			
		if(m_pParentWindow->m_pCharacter != NULL)
		{
			m_pParentWindow->m_pCharacter->ValidateMagicContainerInventory();
			m_pParentWindow->m_pCharacter->MarkChanged();
		}

		m_pParentWindow->Refresh();
		Refresh();

		OnLbnSelchangeScrollSpellList();

	}
}

void CDMScrollDetailsDialog::OnBnClickedCastButton()
{
	UpdateData(TRUE);

	int nCursor = m_cScrollSpellList.GetCurSel();

	if(nCursor < 0 || nCursor > MAX_OBJECT_CONTENTS-1)
		return;


	if(m_nSelectedSpellIndex)
	{
		BOOL bCast = FALSE;

		if(m_pParentWindow->m_pSelectedInventorySlot->IsScroll())
		{
			bCast = m_pParentWindow->m_pSelectedInventorySlot->PopContents(m_nSelectedSpellIndex);
		}
		else
		{
			if(m_pParentWindow->m_pSelectedInventorySlot->m_nCharges > 0)
			{
				bCast = TRUE;
				m_pParentWindow->m_pSelectedInventorySlot->m_nCharges -= max(1, m_pParentWindow->m_pSelectedInventorySlot->m_nContentsCount[nCursor]);
			}
		}

		if(bCast)
		{
			m_pApp->PlaySpellSFX(m_nSelectedSpellIndex-1);
		}

		m_pParentWindow->m_pSelectedInventorySlot->m_bEnchanted = FALSE;

		if(m_pParentWindow->m_pCharacter != NULL)
		{
			m_pParentWindow->m_pCharacter->ValidateMagicContainerInventory();
			m_pParentWindow->m_pCharacter->MarkChanged();
		}
		
		m_pParentWindow->Refresh();
		Refresh();

		OnLbnSelchangeScrollSpellList();

	}
}

void CDMScrollDetailsDialog::OnBnClickedCopySpellButton()
{
	if(m_nSelectedSpellIndex < 1) 
	{
		return;
	}

	if(m_pParentWindow->m_pCharacter->m_bIsCache)
	{
		return;
	}

	DND_CHARACTER_CLASSES _MagicType = DND_CHARACTER_CLASS_UNDEF;

	int nFlags = m_pParentWindow->m_pSelectedInventorySlot->m_nFlags;
	nFlags = ((nFlags / 100) % 100) % 10;

	switch(nFlags)
	{
		case 1:	_MagicType = DND_CHARACTER_CLASS_CLERIC; return; //how the hell did we get in here ?
		case 2:	_MagicType = DND_CHARACTER_CLASS_DRUID; return;	//how the hell did we get in here ?
		case 3:	_MagicType = DND_CHARACTER_CLASS_ILLUSIONIST; break;
		case 4:	_MagicType = DND_CHARACTER_CLASS_MAGE; break;
	}

	int nSelectedSpellClass = -1;

	for(int i = 0; i < 4; ++i)
	{
		if(m_pParentWindow->m_pCharacter->m_SpellClasses[i] == _MagicType)
		{
			nSelectedSpellClass = i;
			break;
		}

		if(m_pParentWindow->m_pCharacter->m_SpellClasses[i] == DND_CHARACTER_SPELL_CLASS_RANGER_MAGE && _MagicType == DND_CHARACTER_CLASS_MAGE)
		{
			nSelectedSpellClass = i;
			break;
		}
	}

	if(nSelectedSpellClass == -1)
	{
		return; // something is borked
	}

	cDNDSpell *pSpell = m_pApp->m_MasterSpellArray.GetAt(m_nSelectedSpellIndex);
	
	if(pSpell != NULL)
	{
		int nSelectedSpellLevel = pSpell->m_nSpellLevel;

		int nKnowSpell = 0;
		int nMinSpells = 0;
		int nMaxSpells = 0;

		int nAddLang = CalculateINTAdjustments(m_pParentWindow->m_pCharacter, &nKnowSpell, &nMinSpells, &nMaxSpells);

		int nTotalBookSpellsInLevel = CountSpellsInLevel(m_pParentWindow->m_pCharacter->m_nSpellBooks[nSelectedSpellClass][nSelectedSpellLevel]);

		if(nTotalBookSpellsInLevel < nMaxSpells)
		{
			m_pParentWindow->m_pCharacter->m_nSpellBooks[nSelectedSpellClass][nSelectedSpellLevel][pSpell->m_nSpellNumber-1] = 1;

			m_pParentWindow->m_pSelectedInventorySlot->m_bEnchanted = FALSE;
			m_pParentWindow->m_pSelectedInventorySlot->PopContents(m_nSelectedSpellIndex);

			if(m_pParentWindow->m_pCharacter != NULL)
			{
				m_pParentWindow->m_pCharacter->ValidateMagicContainerInventory();
				m_pParentWindow->m_pCharacter->MarkChanged();
			}

			m_pParentWindow->Refresh();
			Refresh();

			OnLbnSelchangeScrollSpellList();

		}
		else
		{
			AfxMessageBox("This character cannot know more spells at this level !", MB_OK);
		}
	}
	
}

void CDMScrollDetailsDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
}

void CDMScrollDetailsDialog::OnEnChangeChargesEdit()
{
	UpdateData(TRUE);

	m_pParentWindow->m_pSelectedInventorySlot->m_nCharges = atoi(m_szChargesEdit.GetBuffer(0));

	if(m_pParentWindow->m_pCharacter != NULL)
	{
		m_pParentWindow->m_pCharacter->MarkChanged();
	}

	//Refresh();
	
}
