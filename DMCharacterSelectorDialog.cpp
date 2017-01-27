// DMCharacterSelectorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMInventoryDialog.h"
#include "DMCharSpellsDialog.h"
#include "DMCharViewDialog.h"
#include "DMNPCViewDialog.h"
#include "cDMMapViewDialog.h"
#include "DMCharacterSelectorDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DMCharacterSelectorDialog dialog


DMCharacterSelectorDialog::DMCharacterSelectorDialog(DWORD *pdwReturnedID, DWORD _dwParentPartyID, DND_SELECTOR_TYPES _SelectorType, CWnd* pParent /*=NULL*/)
	: CDialog(DMCharacterSelectorDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(DMCharacterSelectorDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_dwParentPartyID = _dwParentPartyID;

	m_SelectorType = _SelectorType;

	m_pdwReturnedID = pdwReturnedID;
}


void DMCharacterSelectorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DMCharacterSelectorDialog)
	DDX_Control(pDX, IDC_CHARACTER_LIST, m_cCharacterList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DMCharacterSelectorDialog, CDialog)
	//{{AFX_MSG_MAP(DMCharacterSelectorDialog)
	ON_LBN_DBLCLK(IDC_CHARACTER_LIST, OnDblclkCharacterList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DMCharacterSelectorDialog message handlers

void DMCharacterSelectorDialog::OnOK() 
{
	// TODO: Add extra validation here

	UpdateData(FALSE);

	int nCursor = m_cCharacterList.GetCurSel();

	if(nCursor != -1)
	{
		*m_pdwReturnedID = m_cCharacterList.GetItemData(nCursor);
	}
	
	CDialog::OnOK();
}

void DMCharacterSelectorDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL DMCharacterSelectorDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	int nRow = 0;
	WORD wID;

	switch(m_SelectorType)
	{
		case DND_SELECTOR_CHARACTER:
		{
			SetWindowText("Select Character:");
			for (POSITION pos = m_pApp->m_CharacterViewMap.GetStartPosition(); pos != NULL; )
			{
				PDNDCHARVIEWDLG pCharDlg = NULL;
			
				m_pApp->m_CharacterViewMap.GetNextAssoc(pos,wID,pCharDlg);

				if(pCharDlg != NULL && pCharDlg->m_pCharacter != NULL && pCharDlg->m_pCharacter->m_dwCharacterID)
				{
					m_cCharacterList.InsertString(nRow, pCharDlg->m_pCharacter->m_szCharacterName);
					m_cCharacterList.SetItemData(nRow, pCharDlg->m_pCharacter->m_dwCharacterID);
					++nRow;
				}
			}

			for (pos = m_pApp->m_NPCViewMap.GetStartPosition(); pos != NULL; )
			{
				PDNDNPCVIEWDLG pNPCDlg = NULL;

				m_pApp->m_NPCViewMap.GetNextAssoc(pos,wID,pNPCDlg);

				if(pNPCDlg != NULL && pNPCDlg->m_pNPC != NULL && pNPCDlg->m_pNPC->m_dwCharacterID && !pNPCDlg->m_pNPC->m_bIsCache)
				{
					for(int i = 0; i < m_cCharacterList.GetCount(); ++i)
					{
						if(m_cCharacterList.GetItemData(i) == pNPCDlg->m_pNPC->m_dwCharacterID)
						{
							pNPCDlg = NULL;
							break;
						}
					}

					if(pNPCDlg != NULL)
					{
						m_cCharacterList.InsertString(nRow, pNPCDlg->m_pNPC->m_szCharacterName);
						m_cCharacterList.SetItemData(nRow, pNPCDlg->m_pNPC->m_dwCharacterID);
						++nRow;
					}
				}
			}

			break;
		}
		case DND_SELECTOR_MAP:
		{
			SetWindowText("Select Map:");
			for (POSITION pos = m_pApp->m_MapViewMap.GetStartPosition(); pos != NULL; )
			{
				PDNDMAPVIEWDLG pMapDlg = NULL;
				m_pApp->m_MapViewMap.GetNextAssoc(pos,wID,pMapDlg);

				if(pMapDlg != NULL && pMapDlg->m_pDNDMap != NULL && pMapDlg->m_pDNDMap->m_dwMapID)
				{
					m_cCharacterList.InsertString(nRow, pMapDlg->m_pDNDMap->m_szMapName);
					m_cCharacterList.SetItemData(nRow, pMapDlg->m_pDNDMap->m_dwMapID);
					++nRow;
				}
			}

			break;
		}
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DMCharacterSelectorDialog::OnDblclkCharacterList() 
{
	// TODO: Add your control notification handler code here
	
	OnOK();	
}
