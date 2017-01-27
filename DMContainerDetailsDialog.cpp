// DMContainerDetailsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMInventoryDialog.h"
#include "DMCharSpellsDialog.h"
#include "DMCharViewDialog.h"
#include "DMNPCViewDialog.h"
#include "DMContainerDetailsDialog.h"
#include "DMModifyValueDialog.h"


// DMContainerDetailsDialog dialog

IMPLEMENT_DYNAMIC(CDMContainerDetailsDialog, CDialog)

CDMContainerDetailsDialog::CDMContainerDetailsDialog(DWORD dwContainerID, CWnd* pParent /*=NULL*/)
	: CDialog(CDMContainerDetailsDialog::IDD, pParent)
{
	m_pParentWindow = (DMInventoryDialog *)pParent;

	m_dwContainerID = dwContainerID;

	m_pSelectedInventoryObject = NULL;
	m_pSelectedContainerObject = NULL;

	Create(CDMContainerDetailsDialog::IDD, pParent);
}

CDMContainerDetailsDialog::~CDMContainerDetailsDialog()
{
}

void CDMContainerDetailsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PUT_IN_BUTTON, m_cPutInButton);
	DDX_Control(pDX, IDC_TAKE_OUT_BUTTON, m_cTakeOutButton);
	DDX_Control(pDX, IDC_INVENTORY_LIST, m_cInventoryList);
	DDX_Control(pDX, IDC_CONTAINER_LIST, m_cContainerList);
	DDX_Control(pDX, IDC_PP_BUTTON, m_cPPButton);
	DDX_Control(pDX, IDC_GP_BUTTON, m_cGPButton);
	DDX_Control(pDX, IDC_EP_BUTTON, m_cEPButton);
	DDX_Control(pDX, IDC_SP_BUTTON, m_cSPButton);
	DDX_Control(pDX, IDC_CP_BUTTON, m_cCPButton);
	DDX_Control(pDX, IDC_CONTAINER_DETAILS_STATIC, m_cContainDetailsStatic);
}


BEGIN_MESSAGE_MAP(CDMContainerDetailsDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CDMContainerDetailsDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDMContainerDetailsDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_PUT_IN_BUTTON, &CDMContainerDetailsDialog::OnBnClickedPutInButton)
	ON_BN_CLICKED(IDC_TAKE_OUT_BUTTON, &CDMContainerDetailsDialog::OnBnClickedTakeOutButton)
	ON_LBN_SELCHANGE(IDC_INVENTORY_LIST, &CDMContainerDetailsDialog::OnLbnSelchangeInventoryList)
	ON_LBN_SELCHANGE(IDC_CONTAINER_LIST, &CDMContainerDetailsDialog::OnLbnSelchangeContainerList)
	ON_BN_CLICKED(IDC_PP_BUTTON, &CDMContainerDetailsDialog::OnBnClickedPpButton)
	ON_BN_CLICKED(IDC_GP_BUTTON, &CDMContainerDetailsDialog::OnBnClickedGpButton)
	ON_BN_CLICKED(IDC_EP_BUTTON, &CDMContainerDetailsDialog::OnBnClickedEpButton)
	ON_BN_CLICKED(IDC_SP_BUTTON, &CDMContainerDetailsDialog::OnBnClickedSpButton)
	ON_BN_CLICKED(IDC_CP_BUTTON, &CDMContainerDetailsDialog::OnBnClickedCpButton)
	ON_BN_CLICKED(IDC_ALL_COINS_BUTTON, &CDMContainerDetailsDialog::OnBnClickedAllCoinsButton)
END_MESSAGE_MAP()


// DMContainerDetailsDialog message handlers

BOOL CDMContainerDetailsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CFont CourierFont;

	CourierFont.CreatePointFont(90,"Courier New");
	LOGFONT lf; CourierFont.GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	m_ListFont.CreateFontIndirect(&lf);

	m_cInventoryList.SetFont(&m_ListFont);
	m_cContainerList.SetFont(&m_ListFont);

	m_cInventoryList.SetHorizontalExtent(1000);
	m_cContainerList.SetHorizontalExtent(1000);

	m_cPutInButton.LoadBitmaps(IDB_LARGE_RIGHT_BITMAP, IDB_LARGE_LEFT_BITMAP);
	m_cTakeOutButton.LoadBitmaps(IDB_LARGE_LEFT_BITMAP, IDB_LARGE_RIGHT_BITMAP);

	int nOffset = 0;

	if(m_pParentWindow->m_pCharacter->m_bIsCache)
	{
		nOffset = 46;
	}

	SetWindowPos(NULL, m_pParentWindow->m_nDetailsPosX, m_pParentWindow->m_nDetailsPosY+nOffset, 0, 0, SW_SHOW);
	ShowWindow(SW_SHOW);

	Refresh(m_dwContainerID);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDMContainerDetailsDialog::Refresh(DWORD dwContainerID)
{
	if(m_dwContainerID != dwContainerID)
	{
		m_dwContainerID = dwContainerID;

		m_pSelectedInventoryObject = NULL;
		m_pSelectedContainerObject = NULL;
	}

	CString szTemp;

	szTemp.Format("Container Details: %s", m_pParentWindow->m_pSelectedInventorySlot->m_szType);
	m_cContainDetailsStatic.SetWindowText(szTemp);

	szTemp.Format("%ld p.p.", m_pParentWindow->m_pSelectedInventorySlot->m_nContainerPlatinum);
	m_cPPButton.SetWindowText(szTemp);

	szTemp.Format("%ld g.p.", m_pParentWindow->m_pSelectedInventorySlot->m_nContainerGold);
	m_cGPButton.SetWindowText(szTemp);

	szTemp.Format("%ld e.p.", m_pParentWindow->m_pSelectedInventorySlot->m_nContainerElectrum);
	m_cEPButton.SetWindowText(szTemp);

	szTemp.Format("%ld s.p.", m_pParentWindow->m_pSelectedInventorySlot->m_nContainerSilver);
	m_cSPButton.SetWindowText(szTemp);

	szTemp.Format("%ld c.p.", m_pParentWindow->m_pSelectedInventorySlot->m_nContainerCopper);
	m_cCPButton.SetWindowText(szTemp);

	cDNDCharacter *pCharacter = m_pParentWindow->m_pCharacter;

	m_cInventoryList.ResetContent();

	int nTotalItems = 0;

	for(int i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
	{
		POBJECTTYPE pObj = &pCharacter->m_Inventory[i];

		if(pObj != NULL && pObj->m_wTypeId)
		{
			if(pObj->m_dwContainerID)
				continue;

			if(pObj->m_dwObjectID == m_dwContainerID)		// don't show the selected container in this list
				continue;

			if(pObj->IsContainer())
			{
				if(pCharacter->ObjectIsInContainer(m_pParentWindow->m_pSelectedInventorySlot, pObj->m_dwObjectID, 0))
					continue;
			}

			if(pCharacter->IsEquippingObject(pObj))
				continue;

			if(pObj->m_nMagicAdj && !pObj->m_bEnchanted)
			{
				if(pObj->m_nMagicAdj > 0)
					szTemp.Format("%s +%d", pObj->m_szType, pObj->m_nMagicAdj);
				else
					szTemp.Format("%s %d", pObj->m_szType, pObj->m_nMagicAdj);
			}
			else
			{
				szTemp.Format("%s", pObj->m_szType);
			}

			CString szFinal;

			if(pObj->m_lAmount > 1)
			{
				szFinal.Format("%s (%ld)", szTemp, pObj->m_lAmount);
			}
			else
			{
				szFinal = szTemp;
			}

			m_cInventoryList.InsertString(nTotalItems, szFinal);
			m_cInventoryList.SetItemDataPtr(nTotalItems, pObj);

			if(pObj == m_pSelectedInventoryObject)
			{
				m_cInventoryList.SetCurSel(nTotalItems);
			}

			++nTotalItems;
		}
	}

	///////CONTAINER CONTENTS//////////////////////////////////////////////////////////////////

	m_cContainerList.ResetContent();

	nTotalItems = 0;

	for(int i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
	{
		POBJECTTYPE pObj = &pCharacter->m_Inventory[i];

		if(pObj != NULL && pObj->m_wTypeId)
		{
			if(pObj->m_dwObjectID == m_dwContainerID)		// don't show the selected container in this list
				continue;

			if(pObj->m_dwContainerID != m_dwContainerID)	// only show items that are already in this container
				continue;

			if(pObj->m_nMagicAdj && !pObj->m_bEnchanted)
			{
				if(pObj->m_nMagicAdj > 0)
					szTemp.Format("%s +%d", pObj->m_szType, pObj->m_nMagicAdj);
				else
					szTemp.Format("%s %d", pObj->m_szType, pObj->m_nMagicAdj);
			}
			else
			{
				szTemp.Format("%s", pObj->m_szType);
			}

			CString szFinal;

			if(pObj->m_lAmount > 1)
			{
				szFinal.Format("%s (%ld)", szTemp, pObj->m_lAmount);
			}
			else
			{
				szFinal = szTemp;
			}

			m_cContainerList.InsertString(nTotalItems, szFinal);
			m_cContainerList.SetItemDataPtr(nTotalItems, pObj);

			if(pObj == m_pSelectedContainerObject)
			{
				m_cContainerList.SetCurSel(nTotalItems);
			}

			++nTotalItems;
		}
	}
}

void CDMContainerDetailsDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CDMContainerDetailsDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CDMContainerDetailsDialog::OnBnClickedPutInButton()
{
	if(m_pSelectedInventoryObject != NULL)
	{
		m_pSelectedInventoryObject->m_dwContainerID = m_dwContainerID;

		m_pSelectedInventoryObject = NULL;

		m_pParentWindow->Refresh();

		Refresh(m_dwContainerID);

		m_pParentWindow->m_pCharacter->MarkChanged();
	}
}

void CDMContainerDetailsDialog::OnBnClickedTakeOutButton()
{
	if(m_pSelectedContainerObject != NULL)
	{
		m_pSelectedContainerObject->m_dwContainerID = 0;

		m_pSelectedContainerObject = NULL;

		m_pParentWindow->Refresh();

		Refresh(m_dwContainerID);

		m_pParentWindow->m_pCharacter->MarkChanged();
	}
}

void CDMContainerDetailsDialog::OnLbnSelchangeInventoryList()
{
	UpdateData(TRUE);

	int nCursor = m_cInventoryList.GetCurSel();

	if(nCursor > -1)
	{
		m_pSelectedInventoryObject = (POBJECTTYPE)m_cInventoryList.GetItemData(nCursor);
	}
	else
	{
		m_pSelectedInventoryObject = NULL;
	}

	Refresh(m_dwContainerID);
}

void CDMContainerDetailsDialog::OnLbnSelchangeContainerList()
{
	UpdateData(TRUE);

	int nCursor = m_cContainerList.GetCurSel();

	if(nCursor > -1)
	{
		m_pSelectedContainerObject = (POBJECTTYPE)m_cContainerList.GetItemData(nCursor);
	}
	else
	{
		m_pSelectedContainerObject = NULL;
	}

	Refresh(m_dwContainerID);
}

void CDMContainerDetailsDialog::OnBnClickedPpButton()
{
	cDNDCharacter *pCharacter = m_pParentWindow->m_pCharacter;
	LONG lAmount = pCharacter->m_lPlatinumCarried;

	ModifyValue((int *)&lAmount, "Add/Subtract Container p.p.", pCharacter->m_lPlatinumCarried);

	lAmount -= pCharacter->m_lPlatinumCarried;

	m_pParentWindow->m_pSelectedInventorySlot->m_nContainerPlatinum += lAmount;

	pCharacter->BalanceContainerCoins(m_pParentWindow->m_pSelectedInventorySlot, PLATINUM_PIECES);

	m_pParentWindow->Refresh();
	Refresh(m_dwContainerID);

	pCharacter->MarkChanged();
}

void CDMContainerDetailsDialog::OnBnClickedGpButton()
{
	cDNDCharacter *pCharacter = m_pParentWindow->m_pCharacter;
	LONG lAmount = pCharacter->m_lGoldCarried;

	ModifyValue((int *)&lAmount, "Add/Subtract Container g.p.", pCharacter->m_lGoldCarried);

	lAmount -= pCharacter->m_lGoldCarried;

	m_pParentWindow->m_pSelectedInventorySlot->m_nContainerGold += lAmount;

	pCharacter->BalanceContainerCoins(m_pParentWindow->m_pSelectedInventorySlot, GOLD_PIECES);

	m_pParentWindow->Refresh();
	Refresh(m_dwContainerID);

	pCharacter->MarkChanged();

}

void CDMContainerDetailsDialog::OnBnClickedEpButton()
{
	cDNDCharacter *pCharacter = m_pParentWindow->m_pCharacter;
	LONG lAmount = pCharacter->m_lElectrumCarried;

	ModifyValue((int *)&lAmount, "Add/Subtract Container e.p.", pCharacter->m_lElectrumCarried);

	lAmount -= pCharacter->m_lElectrumCarried;

	m_pParentWindow->m_pSelectedInventorySlot->m_nContainerElectrum += lAmount;

	pCharacter->BalanceContainerCoins(m_pParentWindow->m_pSelectedInventorySlot, ELECTRUM_PIECES);

	m_pParentWindow->Refresh();
	Refresh(m_dwContainerID);

	pCharacter->MarkChanged();
}

void CDMContainerDetailsDialog::OnBnClickedSpButton()
{
	cDNDCharacter *pCharacter = m_pParentWindow->m_pCharacter;
	LONG lAmount = pCharacter->m_lSilverCarried;

	ModifyValue((int *)&lAmount, "Add/Subtract Container s.p.", pCharacter->m_lSilverCarried);

	lAmount -= pCharacter->m_lSilverCarried;

	m_pParentWindow->m_pSelectedInventorySlot->m_nContainerSilver += lAmount;

	pCharacter->BalanceContainerCoins(m_pParentWindow->m_pSelectedInventorySlot, SILVER_PIECES);

	m_pParentWindow->Refresh();
	Refresh(m_dwContainerID);

	pCharacter->MarkChanged();
}

void CDMContainerDetailsDialog::OnBnClickedCpButton()
{
	cDNDCharacter *pCharacter = m_pParentWindow->m_pCharacter;
	LONG lAmount = pCharacter->m_lCopperCarried;

	ModifyValue((int *)&lAmount, "Add/Subtract Container c.p.", pCharacter->m_lCopperCarried);

	lAmount -= pCharacter->m_lCopperCarried;

	m_pParentWindow->m_pSelectedInventorySlot->m_nContainerCopper += lAmount;

	pCharacter->BalanceContainerCoins(m_pParentWindow->m_pSelectedInventorySlot, COPPER_PIECES);

	m_pParentWindow->Refresh();
	Refresh(m_dwContainerID);

	pCharacter->MarkChanged();
}

void CDMContainerDetailsDialog::OnBnClickedAllCoinsButton()
{
	cDNDCharacter *pCharacter = m_pParentWindow->m_pCharacter;

	m_pParentWindow->m_pSelectedInventorySlot->m_nContainerPlatinum = pCharacter->m_lPlatinumCarried;
	m_pParentWindow->m_pSelectedInventorySlot->m_nContainerGold = pCharacter->m_lGoldCarried;
	m_pParentWindow->m_pSelectedInventorySlot->m_nContainerElectrum = pCharacter->m_lElectrumCarried;
	m_pParentWindow->m_pSelectedInventorySlot->m_nContainerSilver = pCharacter->m_lSilverCarried;
	m_pParentWindow->m_pSelectedInventorySlot->m_nContainerCopper = pCharacter->m_lCopperCarried;

	pCharacter->BalanceContainerCoins(m_pParentWindow->m_pSelectedInventorySlot, DND_COINS_ALL);

	m_pParentWindow->Refresh();
	Refresh(m_dwContainerID);

	pCharacter->MarkChanged();
}
