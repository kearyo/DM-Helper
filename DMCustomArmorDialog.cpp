// DMCustomArmorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMCustomArmorDialog.h"


// CDMCustomArmorDialog dialog

IMPLEMENT_DYNAMIC(CDMCustomArmorDialog, CDialog)

CDMCustomArmorDialog::CDMCustomArmorDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDMCustomArmorDialog::IDD, pParent)
	, m_szArmorName(_T(""))
	, m_szEncumbrance(_T("0"))
	, m_szCost(_T("0"))
	, m_szXP(_T("0"))
	, m_bThiefUsable(FALSE)
	, m_bMetalArmor(FALSE)
	, m_szMovement(_T("12"))
{
	m_pApp = (CDMHelperApp*)AfxGetApp();
}

CDMCustomArmorDialog::~CDMCustomArmorDialog()
{
}

void CDMCustomArmorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ARMOR_LIST, m_cArmorList);
	DDX_Text(pDX, IDC_ARMOR_NAME_EDIT, m_szArmorName);
	DDV_MaxChars(pDX, m_szArmorName, 32);
	DDX_Control(pDX, IDC_ARMOR_CLASS_COMBO, m_cArmorClassCombo);
	DDX_Control(pDX, IDC_COIN_COMBO, m_cCoinCombo);
	DDX_Text(pDX, IDC_ARMOR_ENCUMBRANCE_EDIT, m_szEncumbrance);
	DDV_MaxChars(pDX, m_szEncumbrance, 8);
	DDX_Text(pDX, IDC_ARMOR_COST_EDIT, m_szCost);
	DDV_MaxChars(pDX, m_szCost, 8);
	DDX_Text(pDX, IDC_ARMOR_XP_EDIT, m_szXP);
	DDV_MaxChars(pDX, m_szXP, 8);
	DDX_Check(pDX, IDC_THIEF_CHECK, m_bThiefUsable);
	DDX_Check(pDX, IDC_METAL_CHECK, m_bMetalArmor);
	DDX_Control(pDX, IDC_DELETE_ARMOR_BUTTON, m_cDeleteArmor);
	DDX_Control(pDX, IDC_MOVE_COMBO, m_cMoveCombo);
	DDX_CBString(pDX, IDC_MOVE_COMBO, m_szMovement);
	DDV_MaxChars(pDX, m_szMovement, 3);
	DDX_Control(pDX, IDC_SAVE_ARMOR_BUTTON, m_cSaveArmor);
}


BEGIN_MESSAGE_MAP(CDMCustomArmorDialog, CDialog)
	ON_EN_CHANGE(IDC_ARMOR_NAME_EDIT, &CDMCustomArmorDialog::OnEnChangeArmorNameEdit)
	ON_CBN_SELCHANGE(IDC_ARMOR_CLASS_COMBO, &CDMCustomArmorDialog::OnCbnSelchangeArmorClassCombo)
	ON_LBN_SELCHANGE(IDC_ARMOR_LIST, &CDMCustomArmorDialog::OnLbnSelchangeArmorList)
	ON_CBN_SELCHANGE(IDC_COIN_COMBO, &CDMCustomArmorDialog::OnCbnSelchangeCoinCombo)
	ON_EN_CHANGE(IDC_ARMOR_ENCUMBRANCE_EDIT, &CDMCustomArmorDialog::OnEnChangeArmorEncumbranceEdit)
	ON_EN_CHANGE(IDC_ARMOR_COST_EDIT, &CDMCustomArmorDialog::OnEnChangeArmorCostEdit)
	ON_BN_CLICKED(IDC_THIEF_CHECK, &CDMCustomArmorDialog::OnBnClickedThiefCheck)
	ON_BN_CLICKED(IDC_METAL_CHECK, &CDMCustomArmorDialog::OnBnClickedMetalCheck)
	ON_BN_CLICKED(IDC_NEW_ARMOR_BUTTON, &CDMCustomArmorDialog::OnBnClickedNewArmorButton)
	ON_BN_CLICKED(IDC_DELETE_ARMOR_BUTTON, &CDMCustomArmorDialog::OnBnClickedDeleteArmorButton)
	ON_BN_CLICKED(IDC_SAVE_ARMOR_BUTTON, &CDMCustomArmorDialog::OnBnClickedSaveArmorButton)
	ON_EN_CHANGE(IDC_ARMOR_XP_EDIT, &CDMCustomArmorDialog::OnEnChangeArmorXpEdit)
	ON_BN_CLICKED(IDCANCEL, &CDMCustomArmorDialog::OnBnClickedCancel)
END_MESSAGE_MAP()


void CDMCustomArmorDialog::ClearArmor()
{
	m_Armor.Init();
	//m_bContainer = FALSE;
	//m_bRations = FALSE;

	m_bChanged = FALSE;
}

void CDMCustomArmorDialog::CheckSaveChangedArmor()
{
	if(m_bChanged)
	{
		if(AfxMessageBox("Save changes to current armor ?", MB_YESNO) == IDYES)
		{
			OnBnClickedSaveArmorButton();
		}
	}
}

void CDMCustomArmorDialog::SaveCustomArmor()
{
	CString szTemp;

	BOOL bFirst = TRUE;
	
	szTemp.Format("%sData/tables/CustomArmor.dat", m_pApp->m_szEXEPath.GetBuffer(0));

	FILE *pOutFile = fopen(szTemp.GetBuffer(0), "wt");

	if(pOutFile != NULL)
	{
		fprintf(pOutFile, "# id, cost (in COPPER pieces), type, armor class, weight, move, XP\n");
		fprintf(pOutFile, "# id's MUST not be changed when adding to this list!\n");
		fprintf(pOutFile, "# 1XX armors can be worn be thieves without penalty, 2XX indicates metal armor\n");
		fprintf(pOutFile, "# File Version: (DO NOT CHANGE THE NEXT LINE !)\n");
		fprintf(pOutFile, "%ld\n", DMH_CURRENT_VERSION);

		for(int k = 0; k < m_pApp->m_CustomArmorsOrderedTypeArray.GetSize(); ++k)
		{
			cDNDArmor *pArmorBase = (cDNDArmor *)m_pApp->m_CustomArmorsOrderedTypeArray.GetAt(k);

			if(pArmorBase != NULL)
			{
				CString szName = pArmorBase->m_szType;
				szName.Replace(",", "_");

				if(bFirst)
				{
					bFirst = FALSE;
				}
				else
				{
					fprintf(pOutFile, "\n");
				}

				szTemp.Format("%d,%dc,%s,%d,%d,%d,%ld", pArmorBase->m_wTypeId, pArmorBase->m_nCost, szName, pArmorBase->m_nArmorClass, pArmorBase->m_nWeight, pArmorBase->m_nMove, pArmorBase->m_lXP);
				
				fprintf(pOutFile, szTemp.GetBuffer(0));
			}
		}

		fclose(pOutFile);
	}

}

// CDMCustomArmorDialog message handlers

BOOL CDMCustomArmorDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_bChanged = FALSE;

	int nCount = 0;
	int nArmorClass = 10;

	m_cArmorClassCombo.InsertString(nCount, "10");
	m_cArmorClassCombo.SetItemData(nCount++, (DWORD_PTR)nArmorClass--);

	m_cArmorClassCombo.InsertString(nCount, "9");
	m_cArmorClassCombo.SetItemData(nCount++, (DWORD_PTR)nArmorClass--);

	m_cArmorClassCombo.InsertString(nCount, "8");
	m_cArmorClassCombo.SetItemData(nCount++, (DWORD_PTR)nArmorClass--);

	m_cArmorClassCombo.InsertString(nCount, "7");
	m_cArmorClassCombo.SetItemData(nCount++, (DWORD_PTR)nArmorClass--);

	m_cArmorClassCombo.InsertString(nCount, "6");
	m_cArmorClassCombo.SetItemData(nCount++, (DWORD_PTR)nArmorClass--);

	m_cArmorClassCombo.InsertString(nCount, "5");
	m_cArmorClassCombo.SetItemData(nCount++, (DWORD_PTR)nArmorClass--);

	m_cArmorClassCombo.InsertString(nCount, "4");
	m_cArmorClassCombo.SetItemData(nCount++, (DWORD_PTR)nArmorClass--);

	m_cArmorClassCombo.InsertString(nCount, "3");
	m_cArmorClassCombo.SetItemData(nCount++, (DWORD_PTR)nArmorClass--);

	m_cArmorClassCombo.InsertString(nCount, "2");
	m_cArmorClassCombo.SetItemData(nCount++, (DWORD_PTR)nArmorClass--);

	m_cArmorClassCombo.InsertString(nCount, "1");
	m_cArmorClassCombo.SetItemData(nCount++, (DWORD_PTR)nArmorClass--);

	m_cArmorClassCombo.InsertString(nCount, "0");
	m_cArmorClassCombo.SetItemData(nCount++, (DWORD_PTR)nArmorClass--);

	m_cCoinCombo.InsertString(0, "g.p.");
	m_cCoinCombo.InsertString(1, "s.p.");
	m_cCoinCombo.InsertString(2, "c.p.");

	m_cCoinCombo.SetCurSel(0);

	nCount = 0;
	m_cMoveCombo.InsertString(nCount++, "12");
	m_cMoveCombo.InsertString(nCount++, "9");
	m_cMoveCombo.InsertString(nCount++, "6");
	m_cMoveCombo.InsertString(nCount++, "3");
	
	m_cMoveCombo.SetCurSel(0);


	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDMCustomArmorDialog::InitView()
{
	
}

void CDMCustomArmorDialog::Refresh()
{
	int nCopperCost = m_Armor.m_nCost;

	if(nCopperCost > 200 && (nCopperCost % 200 == 0))
	{
		m_cCoinCombo.SetCurSel(0);
		nCopperCost /= 200;
	}
	else if(nCopperCost > 10 && (nCopperCost % 10 == 0))
	{
		m_cCoinCombo.SetCurSel(1);
		nCopperCost /= 10;
	}
	else
	{
		m_cCoinCombo.SetCurSel(2);
	}

	m_szCost.Format("%d", nCopperCost);
	m_szMovement.Format("%d", m_Armor.m_nMove);
	m_szXP.Format("%d", m_Armor.m_lXP);
	m_szEncumbrance.Format("%d", m_Armor.m_nWeight);
	

	m_szArmorName.Format("%s", m_Armor.m_szType);

	for(int i = 0; i < m_cArmorClassCombo.GetCount(); ++i)
	{
		int nAC = (int)m_cArmorClassCombo.GetItemData(i);

		if(nAC == m_Armor.m_nArmorClass)
		{
			m_cArmorClassCombo.SetCurSel(i);
			break;
		}
	}

	int nCount = 0;
	m_cArmorList.ResetContent();
	for(int k = 0; k < m_pApp->m_CustomArmorsOrderedTypeArray.GetSize(); ++k)
	{
		cDNDArmor *pArmorBase = (cDNDArmor*)m_pApp->m_CustomArmorsOrderedTypeArray.GetAt(k);

		if(pArmorBase != NULL)
		{
			m_cArmorList.InsertString(nCount, pArmorBase->m_szType);
			m_cArmorList.SetItemData(nCount, (DWORD_PTR)pArmorBase);

			if (pArmorBase->m_wTypeId == m_Armor.m_wTypeId)
			{
				m_cArmorList.SetCurSel(nCount);
			}

			++nCount;
		}
	}

	m_cDeleteArmor.EnableWindow(FALSE);

	if (m_bChanged)
	{
		m_cSaveArmor.EnableWindow(TRUE);
	}
	else
	{
		m_cSaveArmor.EnableWindow(FALSE);
	}

	UpdateData(FALSE);
}

void CDMCustomArmorDialog::OnEnChangeArmorNameEdit()
{
	UpdateData(TRUE);

	sprintf(m_Armor.m_szType, "%s", m_szArmorName.Left(31));
	m_Armor.m_szType[31] = 0;

	m_bChanged = TRUE;

	Refresh();
}

void CDMCustomArmorDialog::OnCbnSelchangeArmorClassCombo()
{
	UpdateData(TRUE);

	int nCursor = m_cArmorClassCombo.GetCurSel();

	if(nCursor > -1)
	{
		int nAC = (int)m_cArmorClassCombo.GetItemData(nCursor);

		m_Armor.m_nArmorClass = nAC;

		m_bChanged = TRUE;

		Refresh();
	}
}

void CDMCustomArmorDialog::OnLbnSelchangeArmorList()
{
	UpdateData(TRUE);

	int nCursor = m_cArmorList.GetCurSel();

	CheckSaveChangedArmor();

	m_cArmorList.SetCurSel(nCursor);

	if(nCursor > -1)
	{
		cDNDArmor *pArmor = (cDNDArmor *)m_cArmorList.GetItemData(nCursor);

		if(pArmor != NULL)
		{
			m_bChanged = FALSE;

			pArmor->CopyTo(&m_Armor);
			Refresh();

			m_cDeleteArmor.EnableWindow(TRUE);

			m_cArmorList.SetCurSel(nCursor);

		}
	}
}

void CDMCustomArmorDialog::OnCbnSelchangeCoinCombo()
{
	m_bChanged = TRUE;

	Refresh();
}

void CDMCustomArmorDialog::OnEnChangeArmorEncumbranceEdit()
{
	UpdateData(TRUE);

	m_Armor.m_nWeight = atoi(m_szEncumbrance.GetBuffer(0));

	m_bChanged = TRUE;

	Refresh();
}

void CDMCustomArmorDialog::OnEnChangeArmorCostEdit()
{
	UpdateData(TRUE);

	m_Armor.m_nWeight = atoi(m_szEncumbrance.GetBuffer(0));

	m_bChanged = TRUE;

	Refresh();
}
void CDMCustomArmorDialog::OnEnChangeArmorXpEdit()
{
	UpdateData(TRUE);

	m_Armor.m_lXP = atoi(m_szXP.GetBuffer(0));

	m_bChanged = TRUE;

	Refresh();
}

void CDMCustomArmorDialog::OnBnClickedThiefCheck()
{
	
}

void CDMCustomArmorDialog::OnBnClickedMetalCheck()
{
	
}

void CDMCustomArmorDialog::OnBnClickedNewArmorButton()
{
	CheckSaveChangedArmor();

	ClearArmor();

	Refresh();
}

void CDMCustomArmorDialog::OnBnClickedDeleteArmorButton()
{
	UpdateData(TRUE);

	int nCursor = m_cArmorList.GetCurSel();

	if(nCursor > -1)
	{
		if(AfxMessageBox("Armor will be deleted completely !\nAre you sure ?", MB_YESNO) != IDYES)
			return;

		cDNDArmor *pArmor = (cDNDArmor *)m_cArmorList.GetItemData(nCursor);

		if(pArmor != NULL)
		{
			for(int k = 0; k < m_pApp->m_CustomArmorsOrderedTypeArray.GetSize(); ++k)
			{
				cDNDArmor *pArmorBase = (cDNDArmor *)m_pApp->m_CustomArmorsOrderedTypeArray.GetAt(k);

				if(pArmorBase != NULL)
				{
					if(pArmorBase->m_wTypeId == pArmor->m_wTypeId)
					{
						m_pApp->m_CustomArmorsOrderedTypeArray.SetAt(k, NULL);
					}
				}
			}
		
			cDNDArmor *pIndexedArmor = (cDNDArmor *)m_pApp->m_ObjectsIndexedTypeArray.GetAt(pArmor->m_wTypeId);
			if(pIndexedArmor != NULL)
			{
				m_pApp->m_ObjectsIndexedTypeArray.RemoveAt(pIndexedArmor->m_wTypeId);
				//delete pIndexedArmor; // // don't delete it here !
			}

			//find it and delete it from this array
			for(int i = 0; i < m_pApp->m_ObjectsOrderedTypeArray.GetSize(); ++i)
			{
				cDNDArmor *pOrderedArmor = (cDNDArmor *)m_pApp->m_ObjectsOrderedTypeArray.GetAt(i);
				if(pOrderedArmor != NULL && pOrderedArmor->m_wTypeId == pArmor->m_wTypeId)
				{
					m_pApp->m_ObjectsOrderedTypeArray.RemoveAt(i);
					delete pOrderedArmor; 
					break;
				}
			}

			/*
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
			*/
		}

		ClearArmor();

		SaveCustomArmor();

		Refresh();
	}
}

void CDMCustomArmorDialog::OnBnClickedSaveArmorButton()
{
	UpdateData(TRUE);

	strcpy(m_Armor.m_szType, m_szArmorName.GetBuffer(0));

	m_Armor.m_nWeight = atoi(m_szEncumbrance.GetBuffer(0));
	m_Armor.m_nCost = atoi(m_szCost.GetBuffer(0));
	m_Armor.m_lXP = atoi(m_szXP.GetBuffer(0));
	m_Armor.m_nMove = atoi(m_szMovement.GetBuffer(0));

	int nCoin = m_cCoinCombo.GetCurSel();

	switch(nCoin)
	{
		case 0:	m_Armor.m_nCost *= 200; break;
		case 1:	m_Armor.m_nCost *= 10; break;
	}

	cDNDArmor *pArmor = NULL;
	if(m_Armor.m_wTypeId == 0)
	{
		pArmor = new cDNDArmor();

		m_Armor.CopyTo(pArmor);

		pArmor->m_ObjectType = DND_OBJECT_TYPE_ARMOR;

		pArmor->m_wTypeId = m_pApp->GetNextCustomArmorIndex(m_bMetalArmor);

		m_pApp->m_CustomArmorsOrderedTypeArray.InsertAt(pArmor->m_wTypeId, pArmor, 1);

		int nDefinedObjectCount = m_pApp->m_ObjectsOrderedTypeArray.GetCount();
		m_pApp->m_ObjectsOrderedTypeArray.InsertAt(nDefinedObjectCount, pArmor->Clone());
		m_pApp->m_ObjectsIndexedTypeArray.InsertAt(pArmor->m_wTypeId, pArmor);

		m_pApp->m_ArmorOrderedTypeMap.SetAt(nDefinedObjectCount, pArmor);
		m_pApp->m_ArmorIndexedTypeMap.SetAt(pArmor->m_wTypeId, pArmor);

		m_Armor.m_wTypeId = pArmor->m_wTypeId;
	}
	else
	{
		m_pApp->m_ArmorIndexedTypeMap.Lookup(m_Armor.m_wTypeId, pArmor);

		if(pArmor != NULL)
		{
			m_Armor.CopyTo(pArmor);

			//find it and copy it to this array
			for(int i = 0; i < m_pApp->m_ArmorOrderedTypeMap.GetSize(); ++i)
			{
				cDNDArmor *pOrderedArmor = NULL;
				m_pApp->m_ArmorOrderedTypeMap.Lookup(i, pOrderedArmor);
				if(pOrderedArmor != NULL && pOrderedArmor->m_wTypeId == pArmor->m_wTypeId)
				{
					m_Armor.CopyTo(pOrderedArmor);
					break;
				}
			}
		}
	}

	m_pApp->AlphabetizeObjectList();

	m_bChanged = FALSE;

	SaveCustomArmor();

	Refresh();
}




void CDMCustomArmorDialog::OnBnClickedCancel()
{
	CheckSaveChangedArmor();

	CDialog::OnCancel();
}
