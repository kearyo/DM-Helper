// DMCustomItemsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMCustomItemsDialog.h"


// CDMCustomItemsDialog dialog

IMPLEMENT_DYNAMIC(CDMCustomItemsDialog, CDialog)

CDMCustomItemsDialog::CDMCustomItemsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDMCustomItemsDialog::IDD, pParent)
	, m_szItemName(_T(""))
	, m_szEncumbrance(_T(""))
	, m_szCost(_T(""))
	, m_bContainer(FALSE)
	, m_bRations(FALSE)
	, m_szXP(_T(""))
{
	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_bChanged = FALSE;
}

CDMCustomItemsDialog::~CDMCustomItemsDialog()
{
}

void CDMCustomItemsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ITEM_LIST, m_cItemList);
	DDX_Text(pDX, IDC_ITEM_NAME_EDIT, m_szItemName);
	DDV_MaxChars(pDX, m_szItemName, 127);
	DDX_Control(pDX, IDC_COIN_COMBO, m_cCoinCombo);
	DDX_Text(pDX, IDC_ITEM_ENCUMBRANCE_EDIT, m_szEncumbrance);
	DDX_Text(pDX, IDC_ITEM_COST_EDIT, m_szCost);
	DDX_Check(pDX, IDC_CONTAINER_CHECK, m_bContainer);
	DDX_Control(pDX, IDC_DELETE_ITEM_BUTTON, m_cDeleteItem);
	DDX_Check(pDX, IDC_RATIONS_CHECK, m_bRations);
	DDX_Text(pDX, IDC_ITEM_XP_EDIT, m_szXP);
	DDX_Control(pDX, IDC_SAVE_ITEM_BUTTON, m_cSaveButton);
}


BEGIN_MESSAGE_MAP(CDMCustomItemsDialog, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CDMCustomItemsDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_NEW_ITEM_BUTTON, &CDMCustomItemsDialog::OnBnClickedNewItemButton)
	ON_BN_CLICKED(IDC_SAVE_ITEM_BUTTON, &CDMCustomItemsDialog::OnBnClickedSaveItemButton)
	ON_LBN_SELCHANGE(IDC_ITEM_LIST, &CDMCustomItemsDialog::OnLbnSelchangeItemList)
	ON_BN_CLICKED(IDC_CONTAINER_CHECK, &CDMCustomItemsDialog::OnBnClickedContainerCheck)
	ON_BN_CLICKED(IDC_DELETE_ITEM_BUTTON, &CDMCustomItemsDialog::OnBnClickedDeleteItemButton)
	ON_BN_CLICKED(IDC_RATIONS_CHECK, &CDMCustomItemsDialog::OnBnClickedRationsCheck)
	ON_EN_CHANGE(IDC_ITEM_NAME_EDIT, &CDMCustomItemsDialog::OnEnChangeItemNameEdit)
	ON_EN_CHANGE(IDC_ITEM_ENCUMBRANCE_EDIT, &CDMCustomItemsDialog::OnEnChangeItemEncumbranceEdit)
	ON_EN_CHANGE(IDC_ITEM_COST_EDIT, &CDMCustomItemsDialog::OnEnChangeItemCostEdit)
	ON_EN_CHANGE(IDC_ITEM_XP_EDIT, &CDMCustomItemsDialog::OnEnChangeItemXpEdit)
	ON_CBN_SELCHANGE(IDC_COIN_COMBO, &CDMCustomItemsDialog::OnCbnSelchangeCoinCombo)
END_MESSAGE_MAP()


void CDMCustomItemsDialog::ClearObject()
{
	m_Object.Init();
	m_bContainer = FALSE;
	m_bRations = FALSE;

	m_bChanged = FALSE;
}

void CDMCustomItemsDialog::CheckSaveChangedObject()
{
	if(m_bChanged)
	{
		if(AfxMessageBox("Save changes to current object ?", MB_YESNO) == IDYES)
		{
			OnBnClickedSaveItemButton();
		}
	}
}

void CDMCustomItemsDialog::InitView()
{
	m_szItemName.Format("%s", m_Object.m_szType);
	m_szEncumbrance.Format("%d", m_Object.m_nWeight);
	m_szXP.Format("%ld", m_Object.m_lXP);

	int nCopperCost = m_Object.m_nCost;

	if (nCopperCost > 200 && (nCopperCost % 200 == 0))
	{
		m_cCoinCombo.SetCurSel(0);
		nCopperCost /= 200;
	}
	else if (nCopperCost > 10 && (nCopperCost % 10 == 0))
	{
		m_cCoinCombo.SetCurSel(1);
		nCopperCost /= 10;
	}
	else
	{
		m_cCoinCombo.SetCurSel(2);
	}

	m_szCost.Format("%d", nCopperCost);

	int nCount = 0;
	m_cItemList.ResetContent();
	for (int k = 0; k < m_pApp->m_CustomObjectsOrderedTypeArray.GetSize(); ++k)
	{
		POBJECTTYPE pObjBase = m_pApp->m_CustomObjectsOrderedTypeArray.GetAt(k);

		if (pObjBase != NULL)
		{
			m_cItemList.InsertString(nCount, pObjBase->m_szType);
			m_cItemList.SetItemData(nCount, (DWORD_PTR)pObjBase);

			++nCount;
		}
	}

	if (m_Object.m_nFlags == 100000)
	{
		m_bContainer = TRUE;
	}
	else
	{
		m_bContainer = FALSE;
	}

	if (m_Object.m_nFlags == 50000)
	{
		m_bRations = TRUE;
	}
	else
	{
		m_bRations = FALSE;
	}

	UpdateData(FALSE);
}

void CDMCustomItemsDialog::Refresh()
{
	UpdateData(TRUE);

	m_cDeleteItem.EnableWindow(FALSE);

	for (int i = 0; i < m_cItemList.GetCount(); ++i)
	{
		POBJECTTYPE pObjBase = (POBJECTTYPE)m_cItemList.GetItemData(i);

		if (pObjBase != NULL && pObjBase->m_wTypeId == m_Object.m_wTypeId)
		{
			m_cItemList.SetCurSel(i);
			m_cDeleteItem.EnableWindow(TRUE);
		}
	}

	if(m_Object.m_nFlags / 100000 == 1)
	{
		m_bContainer = TRUE;
	}
	else
	{
		m_bContainer = FALSE;
	}

	if(m_Object.m_nFlags / 10000 == 5)
	{
		m_bRations = TRUE;
	}
	else
	{
		m_bRations = FALSE;
	}


	if (m_bChanged && m_szItemName.GetLength() > 0)
	{
		m_cSaveButton.EnableWindow(TRUE);
	}
	else
	{
		m_cSaveButton.EnableWindow(FALSE);
	}

}

void CDMCustomItemsDialog::SaveCustomItems()
{
	CString szTemp;
	
	szTemp.Format("%sData/tables/CustomItems.dat", m_pApp->m_szEXEPath.GetBuffer(0));

	FILE *pOutFile = fopen(szTemp.GetBuffer(0), "wt");

	if(pOutFile != NULL)
	{
		fprintf(pOutFile, "#id, cost (in COPPER pieces), type, weight, XP, flags ( 50000 is rations, 100000 is a container, 200000 magic bag, 300000 is a scroll, 400000 rod, 500000 staff, 600000 wand, 700000 magic storing ring)\n");
		fprintf(pOutFile, "# id's must not be changed when adding to this list! It is not recommended that you modify this file manually - use the custom object editor in the program !\n");
		fprintf(pOutFile, "# do not use parentheses in any item name '(' or ')' unless it is a stack of similar items\n");
		fprintf(pOutFile, "# File Version: (DO NOT CHANGE THE NEXT LINE !)\n");
		fprintf(pOutFile, "%ld\n", DMH_CURRENT_VERSION);

		for(int k = 0; k < m_pApp->m_CustomObjectsOrderedTypeArray.GetSize(); ++k)
		{
			POBJECTTYPE pObjBase = m_pApp->m_CustomObjectsOrderedTypeArray.GetAt(k);

			if(pObjBase != NULL)
			{
				CString szName = pObjBase->m_szType;
				szName.Replace(",", "_");

				szTemp.Format("%d,%dc,%s,%d,%d,%06d\n", pObjBase->m_wTypeId, pObjBase->m_nCost, szName, pObjBase->m_nWeight, pObjBase->m_lXP, pObjBase->m_nFlags);
				
				fprintf(pOutFile, szTemp.GetBuffer(0));
			}
		}

		fclose(pOutFile);
	}

}


// CDMCustomItemsDialog message handlers

BOOL CDMCustomItemsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cCoinCombo.InsertString(0, "g.p.");
	m_cCoinCombo.InsertString(1, "s.p.");
	m_cCoinCombo.InsertString(2, "c.p.");

	m_cCoinCombo.SetCurSel(0);

	m_cSaveButton.EnableWindow(FALSE);

	InitView();

	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDMCustomItemsDialog::OnBnClickedCancel()
{
	CheckSaveChangedObject();

	OnCancel();
}

void CDMCustomItemsDialog::OnBnClickedNewItemButton()
{
	CheckSaveChangedObject();

	ClearObject();

	m_bChanged = FALSE;

	InitView();

	Refresh();

}

void CDMCustomItemsDialog::OnBnClickedSaveItemButton()
{
	UpdateData(TRUE);

	strcpy(m_Object.m_szType, m_szItemName.GetBuffer(0));

	m_Object.m_nWeight = atoi(m_szEncumbrance.GetBuffer(0));
	m_Object.m_nCost = atoi(m_szCost.GetBuffer(0));
	m_Object.m_lXP = atoi(m_szXP.GetBuffer(0));

	int nCoin = m_cCoinCombo.GetCurSel();

	switch(nCoin)
	{
		case 0:	m_Object.m_nCost *= 200; break;
		case 1:	m_Object.m_nCost *= 10; break;
	}

	POBJECTTYPE pObject = NULL;
	if(m_Object.m_wTypeId == 0)
	{
		pObject = new cDNDObject();

		m_Object.CopyFull(pObject);

		pObject->m_ObjectType = DND_OBJECT_TYPE_EQUIPMENT;

		pObject->m_wTypeId = m_pApp->GetNextCustomObjectIndex();

		m_pApp->m_CustomObjectsOrderedTypeArray.InsertAt(pObject->m_wTypeId, pObject, 1);

		int nDefinedObjectCount = m_pApp->m_ObjectsOrderedTypeArray.GetCount();
		m_pApp->m_ObjectsOrderedTypeArray.InsertAt(nDefinedObjectCount, pObject->Clone());
		m_pApp->m_ObjectsIndexedTypeArray.InsertAt(pObject->m_wTypeId, pObject);

		m_pApp->m_EquipmentOrderedTypeArray.InsertAt(nDefinedObjectCount, pObject);
		m_pApp->m_EquipmentIndexedTypeArray.InsertAt(pObject->m_wTypeId, pObject);
	}
	else
	{
		pObject = m_pApp->m_ObjectsIndexedTypeArray.GetAt(m_Object.m_wTypeId);

		if(pObject != NULL)
		{
			m_Object.CopyFull(pObject);

			//find it and copy it to this array
			for(int i = 0; i < m_pApp->m_ObjectsOrderedTypeArray.GetSize(); ++i)
			{
				cDNDObject *pOrderedObject = m_pApp->m_ObjectsOrderedTypeArray.GetAt(i);
				if(pOrderedObject != NULL && pOrderedObject->m_wTypeId == pObject->m_wTypeId)
				{
					m_Object.CopyFull(pOrderedObject);
					break;
				}
			}

		}
	}

	m_pApp->AlphabetizeObjectList();

	m_pApp->m_dwInventoryFlag = GetUniversalTime();

	SaveCustomItems();

	m_bChanged = FALSE;

	InitView();

	Refresh();
}

void CDMCustomItemsDialog::OnLbnSelchangeItemList()
{

	UpdateData(TRUE);

	int nCursor = m_cItemList.GetCurSel();

	CheckSaveChangedObject();

	m_cItemList.SetCurSel(nCursor);

	if(nCursor > -1)
	{
		POBJECTTYPE pObject = (POBJECTTYPE)m_cItemList.GetItemData(nCursor);

		if(pObject != NULL)
		{
			pObject->CopyFull(&m_Object);

			InitView();

			m_cDeleteItem.EnableWindow(TRUE);

			m_cItemList.SetCurSel(nCursor);

			Refresh();

		}
	}

}

void CDMCustomItemsDialog::OnBnClickedContainerCheck()
{
	UpdateData(TRUE);

	if(m_bContainer)
	{
		m_Object.m_nFlags = 100000;
	}
	else
	{
		m_Object.m_nFlags = 0;
	}

	m_bRations = FALSE;

	m_bChanged = TRUE;

	Refresh();
}

void CDMCustomItemsDialog::OnBnClickedRationsCheck()
{
	UpdateData(TRUE);

	if(m_bRations)
	{
		m_Object.m_nFlags = 50000;
	}
	else
	{
		m_Object.m_nFlags = 0;
	}

	m_bContainer = FALSE;

	m_bChanged = TRUE;

	Refresh();
}


void CDMCustomItemsDialog::OnBnClickedDeleteItemButton()
{
	UpdateData(TRUE);

	int nCursor = m_cItemList.GetCurSel();

	if(nCursor > -1)
	{
		if(AfxMessageBox("Item will be deleted completely !\nAre you sure ?", MB_YESNO) != IDYES)
			return;

		POBJECTTYPE pObject = (POBJECTTYPE)m_cItemList.GetItemData(nCursor);

		if(pObject != NULL)
		{
			for(int k = 0; k < m_pApp->m_CustomObjectsOrderedTypeArray.GetSize(); ++k)
			{
				POBJECTTYPE pObjBase = m_pApp->m_CustomObjectsOrderedTypeArray.GetAt(k);

				if(pObjBase != NULL)
				{
					if(pObjBase->m_wTypeId == pObject->m_wTypeId)
					{
						m_pApp->m_CustomObjectsOrderedTypeArray.SetAt(k, NULL);
					}
				}
			}
		
			cDNDObject *pIndexedObject = m_pApp->m_ObjectsIndexedTypeArray.GetAt(pObject->m_wTypeId);
			if(pIndexedObject != NULL)
			{
				m_pApp->m_ObjectsIndexedTypeArray.RemoveAt(pObject->m_wTypeId);
				//delete pIndexedObject; // // don't delete it here !
			}

			//find it and delete it from this array
			for(int i = 0; i < m_pApp->m_ObjectsOrderedTypeArray.GetSize(); ++i)
			{
				cDNDObject *pOrderedObject = m_pApp->m_ObjectsOrderedTypeArray.GetAt(i);
				if(pOrderedObject != NULL && pOrderedObject->m_wTypeId == pObject->m_wTypeId)
				{
					m_pApp->m_ObjectsOrderedTypeArray.RemoveAt(i);
					delete pOrderedObject; 
					break;
				}
			}

			for(int i = 0; i < m_pApp->m_EquipmentOrderedTypeArray.GetSize(); ++i)
			{
				cDNDObject *pOrderedObject = m_pApp->m_EquipmentOrderedTypeArray.GetAt(i);
				if(pOrderedObject != NULL && pOrderedObject->m_wTypeId == pObject->m_wTypeId)
				{
					m_pApp->m_EquipmentOrderedTypeArray.RemoveAt(i);
					delete pOrderedObject;
					break;
				}
			}
		}

		ClearObject();

		SaveCustomItems();

		m_bChanged = FALSE;

		InitView();

		Refresh();
	}
}


void CDMCustomItemsDialog::OnEnChangeItemNameEdit()
{
	m_bChanged = TRUE;

	Refresh();
}

void CDMCustomItemsDialog::OnEnChangeItemEncumbranceEdit()
{
	m_bChanged = TRUE;

	Refresh();
}

void CDMCustomItemsDialog::OnEnChangeItemCostEdit()
{
	m_bChanged = TRUE;

	Refresh();
}

void CDMCustomItemsDialog::OnEnChangeItemXpEdit()
{
	m_bChanged = TRUE;

	Refresh();
}

void CDMCustomItemsDialog::OnCbnSelchangeCoinCombo()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	Refresh();
}
