// DMSFXMapperDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMSFXMapperDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void CDMListBox::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default

	CWnd *pParent = GetParent();

	pParent->PostMessage(WM_VSCROLL, (UINT)m_hWnd, nPos);
	
	CListBox::OnVScroll(nSBCode, nPos, pScrollBar);
}

BEGIN_MESSAGE_MAP(CDMListBox, CListBox)
	//{{AFX_MSG_MAP(CDMListBox)
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DMSFXMapperDialog dialog


DMSFXMapperDialog::DMSFXMapperDialog(BOOL bSpells, CWnd* pParent /*=NULL*/)
	: CDialog(DMSFXMapperDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(DMSFXMapperDialog)
	m_szTypeLabel = _T("Weapons:");
	m_bSwingHit = FALSE;
	m_bSwingMiss = FALSE;
	m_bRangedHit = FALSE;
	m_bRangedMiss = FALSE;
	m_bShowAllCheck = FALSE;
	m_bDruidSpellCheck = FALSE;
	m_bClericSpellCheck = TRUE;
	m_bIllusionistSpellCheck = FALSE;
	m_bMagicUserSpellCheck = FALSE;
	//}}AFX_DATA_INIT

	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_bSpells = bSpells;
}


void DMSFXMapperDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DMSFXMapperDialog)
	DDX_Control(pDX, IDC_MAGICUSER_SPELL_CHECK, m_cMagicUserSpellCheck);
	DDX_Control(pDX, IDC_ILLUSIONIST_SPELL_CHECK, m_cIllusionistSpellCheck);
	DDX_Control(pDX, IDC_DRUID_SPELL_CHECK, m_cDruidSpellCheck);
	DDX_Control(pDX, IDC_CLERIC_SPELL_CHECK, m_cClericSpellCheck);
	DDX_Control(pDX, IDC_INFO_TEXT, m_cInfoText);
	DDX_Control(pDX, IDC_RANGED_MISS_CHECK, m_cRangedMiss);
	DDX_Control(pDX, IDC_RANGED_HIT_CHECK, m_cRangedHit);
	DDX_Control(pDX, IDC_SWING_MISS_CHECK, m_cSwingMiss);
	DDX_Control(pDX, IDC_SWING_HIT_CHECK, m_cSwingHit);
	DDX_Control(pDX, IDC_SFX_MAP_4, m_cSFXMap4);
	DDX_Control(pDX, IDC_SFX_MAP_3, m_cSFXMap3);
	DDX_Control(pDX, IDC_SFX_MAP_2, m_cSFXMap2);
	DDX_Control(pDX, IDC_SFX_MAP_1, m_cSFXMap1);
	DDX_Control(pDX, IDC_SFX_LIST, m_cSFXList);
	DDX_Control(pDX, IDC_EVENT_LIST, m_cEventList);
	DDX_Text(pDX, IDC_TYPE_LABEL, m_szTypeLabel);
	DDX_Check(pDX, IDC_SWING_HIT_CHECK, m_bSwingHit);
	DDX_Check(pDX, IDC_SWING_MISS_CHECK, m_bSwingMiss);
	DDX_Check(pDX, IDC_RANGED_HIT_CHECK, m_bRangedHit);
	DDX_Check(pDX, IDC_RANGED_MISS_CHECK, m_bRangedMiss);
	DDX_Check(pDX, IDC_SHOW_ALL_CHECK, m_bShowAllCheck);
	DDX_Check(pDX, IDC_DRUID_SPELL_CHECK, m_bDruidSpellCheck);
	DDX_Check(pDX, IDC_CLERIC_SPELL_CHECK, m_bClericSpellCheck);
	DDX_Check(pDX, IDC_ILLUSIONIST_SPELL_CHECK, m_bIllusionistSpellCheck);
	DDX_Check(pDX, IDC_MAGICUSER_SPELL_CHECK, m_bMagicUserSpellCheck);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DMSFXMapperDialog, CDialog)
	//{{AFX_MSG_MAP(DMSFXMapperDialog)
	ON_LBN_SELCHANGE(IDC_EVENT_LIST, OnSelchangeEventList)
	ON_BN_CLICKED(IDC_MAP_SFX, OnMapSfx)
	ON_WM_PAINT()
	ON_WM_DRAWITEM()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_CLEAR_SFX, OnClearSfx)
	ON_BN_CLICKED(IDC_SWING_HIT_CHECK, OnSwingHitCheck)
	ON_BN_CLICKED(IDC_SWING_MISS_CHECK, OnSwingMissCheck)
	ON_BN_CLICKED(IDC_RANGED_HIT_CHECK, OnRangedHitCheck)
	ON_BN_CLICKED(IDC_RANGED_MISS_CHECK, OnRangedMissCheck)
	ON_BN_CLICKED(IDC_SHOW_ALL_CHECK, OnShowAllCheck)
	ON_BN_CLICKED(IDC_CLERIC_SPELL_CHECK, OnClericSpellCheck)
	ON_BN_CLICKED(IDC_MAGICUSER_SPELL_CHECK, OnMagicuserSpellCheck)
	ON_BN_CLICKED(IDC_DRUID_SPELL_CHECK, OnDruidSpellCheck)
	ON_BN_CLICKED(IDC_ILLUSIONIST_SPELL_CHECK, OnIllusionistSpellCheck)
	ON_LBN_SELCHANGE(IDC_SFX_MAP_1, OnSelchangeSfxMap1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DMSFXMapperDialog message handlers

BOOL DMSFXMapperDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(m_bSpells)
	{
		m_szTypeLabel = _T("Spells:");

		m_cRangedMiss.ShowWindow(SW_HIDE);
		m_cRangedHit.ShowWindow(SW_HIDE);
		m_cSwingMiss.ShowWindow(SW_HIDE);
		m_cSwingHit.ShowWindow(SW_HIDE);

		m_cSFXMap2.ShowWindow(SW_HIDE);
		m_cSFXMap3.ShowWindow(SW_HIDE);
		m_cSFXMap4.ShowWindow(SW_HIDE);

		m_cInfoText.ShowWindow(SW_HIDE);
	}
	else
	{
		m_cMagicUserSpellCheck.ShowWindow(SW_HIDE);
		m_cIllusionistSpellCheck.ShowWindow(SW_HIDE);
		m_cDruidSpellCheck.ShowWindow(SW_HIDE);
		m_cClericSpellCheck.ShowWindow(SW_HIDE);
	}

	m_nBaseEvent = -1;

	PopulateSFXList();

	UpdateData(FALSE);

	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DMSFXMapperDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	Refresh();
}

void DMSFXMapperDialog::PopulateSFXList()
{
	CString szTemp;

	int nCount = 0;
	m_cEventList.ResetContent();

	if(m_bSpells)
	{
		m_nBaseEvent = -1;

		for(int i = 0; i < m_pApp->m_MasterSpellArray.GetSize(); ++i)
		{
			PSPELL pSpell = m_pApp->m_MasterSpellArray.GetAt(i);

			if(pSpell == NULL)
				continue;

			if(pSpell->m_ClassBook == DND_CHARACTER_CLASS_CLERIC && !m_bClericSpellCheck)
				continue;

			if(pSpell->m_ClassBook == DND_CHARACTER_CLASS_DRUID && !m_bDruidSpellCheck)
				continue;

			if(pSpell->m_ClassBook == DND_CHARACTER_CLASS_MAGE && !m_bMagicUserSpellCheck)
				continue;

			if(pSpell->m_ClassBook == DND_CHARACTER_CLASS_ILLUSIONIST && !m_bIllusionistSpellCheck)
				continue;

			if(m_nBaseEvent == -1)
			{
				m_nBaseEvent = i;
			}

			szTemp.Format("%s", pSpell->m_szSpellName);

			m_cEventList.InsertString(nCount, szTemp);
			m_cEventList.SetItemData(nCount, pSpell->m_nSpellIdentifier);
			++nCount;
		}

	}
	else
	{
		for(int i = 0; i < m_pApp->m_ObjectsOrderedTypeArray.GetSize(); ++i)
		{
			POBJECTTYPE pObj= m_pApp->m_ObjectsOrderedTypeArray.GetAt(i);

			if(pObj->m_ObjectType != DND_OBJECT_TYPE_WEAPON)
				continue;

			szTemp.Format("%s", pObj->m_szType);
			szTemp.Replace("ZZZ", "");

			m_cEventList.InsertString(nCount, szTemp);
			m_cEventList.SetItemData(nCount, pObj->m_wTypeId);
			++nCount;
		}
	}


	nCount = 0;
	m_cSFXList.ResetContent();

	for(int i = 0; i < MAX_SOUNDBOARD_PAGES; ++ i)
	{
		for(int j = 0; j < SOUNDBOARD_SOUNDS_PER_PAGE; ++j)
		{
			if(m_pApp->m_Settings.m_SoundFX[i][j].m_szDesc[0])
			{
				if(!m_bShowAllCheck)
				{
					if(m_bSpells && i != 1)
						continue;
					
					if(!m_bSpells && i != 0)
						continue;
				}

				m_cSFXList.InsertString(nCount, m_pApp->m_Settings.m_SoundFX[i][j].m_szDesc);
				++nCount;
			}
		}
	}

	UpdateData(FALSE);
}

void DMSFXMapperDialog::Refresh()
{
	UpdateData(TRUE);

	int nStart = m_cEventList.GetTopIndex();
	int nEnd = min(nStart + 22, m_cEventList.GetCount());

	int nCount = 0;

	m_cSFXMap1.ResetContent();
	m_cSFXMap2.ResetContent();
	m_cSFXMap3.ResetContent();
	m_cSFXMap4.ResetContent();

	TRACE("REFRESH %d to %d\n", nStart, nEnd);

	for(int i = nStart; i < nEnd; ++i)
	{
		if(m_bSpells)
		{
			m_cSFXMap1.InsertString(nCount, m_pApp->m_Settings.m_SpellSFX[i+m_nBaseEvent].m_szDesc);

			if(m_cEventList.GetSel(i))
			{
				m_cSFXMap1.SetSel(nCount);
			}
		}
		else
		{
			m_cSFXMap1.InsertString(nCount, m_pApp->m_Settings.m_WeaponSFX[i][0].m_szDesc);
			m_cSFXMap2.InsertString(nCount, m_pApp->m_Settings.m_WeaponSFX[i][1].m_szDesc);
			m_cSFXMap3.InsertString(nCount, m_pApp->m_Settings.m_WeaponSFX[i][2].m_szDesc);
			m_cSFXMap4.InsertString(nCount, m_pApp->m_Settings.m_WeaponSFX[i][3].m_szDesc);

			if(m_cEventList.GetSel(i))
			{
				if(m_bSwingHit)
				{
					m_cSFXMap1.SetSel(nCount);
				}
				if(m_bSwingMiss)
				{
					m_cSFXMap2.SetSel(nCount);
				}
				if(m_bRangedHit)
				{
					m_cSFXMap3.SetSel(nCount);
				}
				if(m_bRangedMiss)
				{
					m_cSFXMap4.SetSel(nCount);
				}
			}
		}

		++nCount;
	}

	UpdateData(FALSE);
}

void DMSFXMapperDialog::OnOK() 
{
	// TODO: Add extra validation here

	UpdateData(TRUE);
	
	CDialog::OnOK();
}

void DMSFXMapperDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
	UpdateData(TRUE);

	CDialog::OnCancel();
}

void DMSFXMapperDialog::OnSelchangeEventList() 
{
	Refresh();
}

void DMSFXMapperDialog::OnMapSfx() 
{
	UpdateData(TRUE);

	int nCursor = m_cSFXList.GetCurSel();

	if(nCursor > -1)
	{
		int nCount = 0;

		for(int i = 0; i < m_cEventList.GetCount(); ++i)
		{
			if(m_cEventList.GetSel(i))
			{
				char szSFX[64];
				memset(szSFX, 0, 64*sizeof(char));
				
				m_cSFXList.GetText(nCursor, szSFX);
				int nID = m_cEventList.GetItemData(i);

				if(m_bSpells)
				{
					m_pApp->m_Settings.m_SpellSFX[i+m_nBaseEvent].m_nID = nID;
					strcpy(m_pApp->m_Settings.m_SpellSFX[i+m_nBaseEvent].m_szDesc, szSFX);	
				}
				else
				{
					if(m_bSwingHit)
					{
						m_pApp->m_Settings.m_WeaponSFX[i][0].m_nID = nID;
						strcpy(m_pApp->m_Settings.m_WeaponSFX[i][0].m_szDesc, szSFX);
					}
					if(m_bSwingMiss)
					{
						m_pApp->m_Settings.m_WeaponSFX[i][1].m_nID = nID;
						strcpy(m_pApp->m_Settings.m_WeaponSFX[i][1].m_szDesc, szSFX);
					}
					if(m_bRangedHit)
					{
						m_pApp->m_Settings.m_WeaponSFX[i][2].m_nID = nID;
						strcpy(m_pApp->m_Settings.m_WeaponSFX[i][2].m_szDesc, szSFX);
					}
					if(m_bRangedMiss)
					{
						m_pApp->m_Settings.m_WeaponSFX[i][3].m_nID = nID;
						strcpy(m_pApp->m_Settings.m_WeaponSFX[i][3].m_szDesc, szSFX);
					}
				}
			}

			++nCount;
		}

		Refresh();
	}
	
}

void DMSFXMapperDialog::OnClearSfx() 
{
	UpdateData(FALSE);

	int nCount = 0;

	for(int i = 0; i < m_cEventList.GetCount(); ++i)
	{
		if(m_cEventList.GetSel(i))
		{
			if(m_bSpells)
			{
				m_pApp->m_Settings.m_SpellSFX[i+m_nBaseEvent].m_nID = 0;
				memset(m_pApp->m_Settings.m_SpellSFX[i+m_nBaseEvent].m_szDesc, 0, 64 * sizeof(int));
			}
			else
			{
				if(m_bSwingHit)
				{
					m_pApp->m_Settings.m_WeaponSFX[i][0].m_nID = 0;
					memset(m_pApp->m_Settings.m_WeaponSFX[i][0].m_szDesc, 0, 64 * sizeof(int));
				}
				if(m_bSwingMiss)
				{
					m_pApp->m_Settings.m_WeaponSFX[i][1].m_nID = 0;
					memset(m_pApp->m_Settings.m_WeaponSFX[i][0].m_szDesc, 0, 64 * sizeof(int));
				}
				if(m_bRangedHit)
				{
					m_pApp->m_Settings.m_WeaponSFX[i][2].m_nID = 0;
					memset(m_pApp->m_Settings.m_WeaponSFX[i][0].m_szDesc, 0, 64 * sizeof(int));
				}
				if(m_bRangedMiss)
				{
					m_pApp->m_Settings.m_WeaponSFX[i][3].m_nID = 0;
					memset(m_pApp->m_Settings.m_WeaponSFX[i][0].m_szDesc, 0, 64 * sizeof(int));
				}
			}
		}

		++nCount;
	}

	Refresh();
	
}

BOOL DMSFXMapperDialog::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->hwnd == m_cSFXMap1.m_hWnd ||
	   pMsg->hwnd == m_cSFXMap2.m_hWnd ||
	   pMsg->hwnd == m_cSFXMap3.m_hWnd ||
	   pMsg->hwnd == m_cSFXMap4.m_hWnd)
	{
		if(pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_LBUTTONUP)
		{
			//TRACE("TRAP >> %d 0x%x\n", pMsg->hwnd, pMsg->message);
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL DMSFXMapperDialog::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	

	return CDialog::OnChildNotify(message, wParam, lParam, pLResult);
}

void DMSFXMapperDialog::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

BOOL DMSFXMapperDialog::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	/*
	TRACE(">> %d 0x%x\n", nID, nCode);

	switch(nID)
	{
		case IDC_SFX_MAP_1:
		case IDC_SFX_MAP_2:
		case IDC_SFX_MAP_3:
		case IDC_SFX_MAP_4:
		{
			TRACE("TRAP >> %d 0x%x\n", nID, nCode);
			
			//if(nCode == 0x4 || nCode == 0x5)
				return TRUE;

			break;
		}
	}
	*/

	return CDialog::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void DMSFXMapperDialog::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	// TODO: Add your message handler code here and/or call default

	Refresh();
	
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


void DMSFXMapperDialog::OnSwingHitCheck() 
{
	Refresh();
}

void DMSFXMapperDialog::OnSwingMissCheck() 
{
	Refresh();	
}

void DMSFXMapperDialog::OnRangedHitCheck() 
{
	Refresh();
}

void DMSFXMapperDialog::OnRangedMissCheck() 
{
	Refresh();
}

void DMSFXMapperDialog::OnShowAllCheck() 
{
	UpdateData(TRUE);

	PopulateSFXList();
	Refresh();
}

void DMSFXMapperDialog::OnClericSpellCheck() 
{
	UpdateData(TRUE);

	//m_bClericSpellCheck = FALSE;
	m_bDruidSpellCheck = FALSE;
	m_bIllusionistSpellCheck = FALSE;
	m_bMagicUserSpellCheck = FALSE;

	PopulateSFXList();
	Refresh();
	
}

void DMSFXMapperDialog::OnMagicuserSpellCheck() 
{
	UpdateData(TRUE);

	m_bClericSpellCheck = FALSE;
	m_bDruidSpellCheck = FALSE;
	m_bIllusionistSpellCheck = FALSE;
	//m_bMagicUserSpellCheck = FALSE;

	PopulateSFXList();
	Refresh();
	
}

void DMSFXMapperDialog::OnDruidSpellCheck() 
{
	UpdateData(TRUE);

	m_bClericSpellCheck = FALSE;
	//m_bDruidSpellCheck = FALSE;
	m_bIllusionistSpellCheck = FALSE;
	m_bMagicUserSpellCheck = FALSE;

	PopulateSFXList();
	Refresh();
	
}

void DMSFXMapperDialog::OnIllusionistSpellCheck() 
{
	UpdateData(TRUE);

	m_bClericSpellCheck = FALSE;
	m_bDruidSpellCheck = FALSE;
	//m_bIllusionistSpellCheck = FALSE;
	m_bMagicUserSpellCheck = FALSE;

	PopulateSFXList();
	Refresh();
	
}

void DMSFXMapperDialog::OnSelchangeSfxMap1() 
{
	// TODO: Add your control notification handler code here
	

	TRACE("GOT IN !\n");
}
