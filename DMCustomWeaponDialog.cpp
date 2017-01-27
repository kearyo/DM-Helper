// DMCustomWeaponDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMCustomWeaponDialog.h"


#ifdef _DEBUG
#define _DEBUG_CUST_WEAPONS	TRUE
#else
#define _DEBUG_CUST_WEAPONS	FALSE  //DON"T CHANGE !!!
#endif


// CDMCustomWeaponDialog dialog

IMPLEMENT_DYNAMIC(CDMCustomWeaponDialog, CDialog)

CDMCustomWeaponDialog::CDMCustomWeaponDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDMCustomWeaponDialog::IDD, pParent)
	, m_szWeaponName(_T(""))
	, m_szEncumbrance(_T(""))
	, m_szCost(_T(""))
	, m_bMissile(FALSE)
	, m_bAmmo(FALSE)
	, m_bSword(FALSE)
	, m_szXP(_T(""))
	, m_bCleric(FALSE)
	, m_bMagicUser(FALSE)
	, m_bThief(FALSE)
	, m_szWeaponVsAC_0(_T("-"))
	, m_szWeaponVsAC_1(_T("-"))
	, m_szWeaponVsAC_2(_T("-"))
	, m_szWeaponVsAC_3(_T("-"))
	, m_szWeaponVsAC_4(_T("-"))
	, m_szWeaponVsAC_5(_T("-"))
	, m_szWeaponVsAC_6(_T("-"))
	, m_szWeaponVsAC_7(_T("-"))
	, m_szWeaponVsAC_8(_T("-"))
	, m_szWeaponVsAC_9(_T("-"))
	, m_szWeaponVsAC_10(_T("-"))
	, m_szDamageSmallLow(_T(""))
	, m_szDamageSmallHigh(_T(""))
	, m_szDamageLargeLow(_T(""))
	, m_szDamageLargeHigh(_T(""))
	, m_bExceptionalStrength(FALSE)
	, m_szSpaceRequired(_T(""))
	, m_szWeaponSpeed(_T(""))
	, m_szDebugString(_T(""))
{
	m_pApp = (CDMHelperApp*)AfxGetApp();

	m_bChanged = FALSE;

	m_Weapon.Init();
}

CDMCustomWeaponDialog::~CDMCustomWeaponDialog()
{
}

void CDMCustomWeaponDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COIN_COMBO, m_cCoinCombo);
	DDX_Text(pDX, IDC_WEAPON_NAME_EDIT, m_szWeaponName);
	DDV_MaxChars(pDX, m_szWeaponName, 64);
	DDX_Text(pDX, IDC_WEAPON_ENCUMBRANCE_EDIT, m_szEncumbrance);
	DDV_MaxChars(pDX, m_szEncumbrance, 8);
	DDX_Text(pDX, IDC_WEAPON_COST_EDIT, m_szCost);
	DDV_MaxChars(pDX, m_szCost, 8);
	DDX_Check(pDX, IDC_MISSILE_CHECK, m_bMissile);
	DDX_Check(pDX, IDC_AMMO_CHECK, m_bAmmo);
	DDX_Check(pDX, IDC_SWORD_CHECK, m_bSword);
	DDX_Control(pDX, IDC_AMMO_TYPE_STATIC, m_cAmmoLabel);
	DDX_Control(pDX, IDC_AMMO_COMBO, m_cAmmoCombo);
	DDX_Control(pDX, IDC_AMMO_CHECK, m_cAmmo);
	DDX_Control(pDX, IDC_SWORD_CHECK, m_cSword);
	DDX_Control(pDX, IDC_WEAPON_LIST, m_cWeaponList);
	DDX_Control(pDX, IDC_DEBUG_BUTTON, m_cDebugButton);
	DDX_Text(pDX, IDC_WEAPON_XP_EDIT, m_szXP);
	DDX_Check(pDX, IDC_CLERIC_CHECK, m_bCleric);
	DDX_Check(pDX, IDC_MAGICUSER_CHECK, m_bMagicUser);
	DDX_Check(pDX, IDC_THIEF_CHECK, m_bThief);
	DDX_Text(pDX, IDC_WEAPON_VS_AC_EDIT0, m_szWeaponVsAC_0);
	DDX_Text(pDX, IDC_WEAPON_VS_AC_EDIT1, m_szWeaponVsAC_1);
	DDX_Text(pDX, IDC_WEAPON_VS_AC_EDIT2, m_szWeaponVsAC_2);
	DDX_Text(pDX, IDC_WEAPON_VS_AC_EDIT3, m_szWeaponVsAC_3);
	DDX_Text(pDX, IDC_WEAPON_VS_AC_EDIT4, m_szWeaponVsAC_4);
	DDX_Text(pDX, IDC_WEAPON_VS_AC_EDIT5, m_szWeaponVsAC_5);
	DDX_Text(pDX, IDC_WEAPON_VS_AC_EDIT6, m_szWeaponVsAC_6);
	DDX_Text(pDX, IDC_WEAPON_VS_AC_EDIT7, m_szWeaponVsAC_7);
	DDX_Text(pDX, IDC_WEAPON_VS_AC_EDIT8, m_szWeaponVsAC_8);
	DDX_Text(pDX, IDC_WEAPON_VS_AC_EDIT9, m_szWeaponVsAC_9);
	DDX_Text(pDX, IDC_WEAPON_VS_AC_EDIT10, m_szWeaponVsAC_10);
	DDX_Text(pDX, IDC_DAMAGE_SMALL_LOW, m_szDamageSmallLow);
	DDV_MaxChars(pDX, m_szDamageSmallLow, 2);
	DDX_Text(pDX, IDC_DAMAGE_SMALL_HIGH, m_szDamageSmallHigh);
	DDV_MaxChars(pDX, m_szDamageSmallHigh, 2);
	DDX_Text(pDX, IDC_DAMAGE_LARGE_LOW, m_szDamageLargeLow);
	DDV_MaxChars(pDX, m_szDamageLargeLow, 2);
	DDX_Text(pDX, IDC_DAMAGE_LARGE_HIGH, m_szDamageLargeHigh);
	DDV_MaxChars(pDX, m_szDamageLargeHigh, 2);
	DDX_Control(pDX, IDC_EXCEPT_STR_CHECK, m_cExceptionalStrength);
	DDX_Check(pDX, IDC_EXCEPT_STR_CHECK, m_bExceptionalStrength);
	DDX_Text(pDX, IDC_SPACE_REQD, m_szSpaceRequired);
	DDX_Text(pDX, IDC_WEAPON_SPEED, m_szWeaponSpeed);
	DDV_MaxChars(pDX, m_szSpaceRequired, 3);
	DDV_MaxChars(pDX, m_szWeaponSpeed, 3);
	DDX_Control(pDX, IDC_DEBUG_BUTTON2, m_cDebug2Button);
	DDX_Control(pDX, IDC_DEBUG_BUTTON3, m_cDebug3Button);
	DDX_Text(pDX, IDC_DEBUG_STRING, m_szDebugString);
	DDX_Control(pDX, IDC_DELETE_WEAPON_BUTTON, m_cDeleteWeaponButton);
	DDX_Control(pDX, IDC_SAVE_WEAPON_BUTTON, m_cSaveWeaponButton);
	DDX_Control(pDX, IDC_DEBUG_BUTTON4, m_cDebug4Button);
	DDX_Control(pDX, IDC_CLERIC_CHECK, m_cCleric);
	DDX_Control(pDX, IDC_MAGICUSER_CHECK, m_cMagicUser);
	DDX_Control(pDX, IDC_THIEF_CHECK, m_cThief);
}


BEGIN_MESSAGE_MAP(CDMCustomWeaponDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CDMCustomWeaponDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDMCustomWeaponDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_MISSILE_CHECK, &CDMCustomWeaponDialog::OnBnClickedMissileCheck)
	ON_BN_CLICKED(IDC_AMMO_CHECK, &CDMCustomWeaponDialog::OnBnClickedAmmoCheck)
	ON_BN_CLICKED(IDC_SWORD_CHECK, &CDMCustomWeaponDialog::OnBnClickedSwordCheck)
	ON_BN_CLICKED(IDC_DEBUG_BUTTON, &CDMCustomWeaponDialog::OnBnClickedDebugButton)
	ON_BN_CLICKED(IDC_NEW_WEAPON_BUTTON, &CDMCustomWeaponDialog::OnBnClickedNewWeaponButton)
	ON_BN_CLICKED(IDC_DELETE_WEAPON_BUTTON, &CDMCustomWeaponDialog::OnBnClickedDeleteWeaponButton)
	ON_BN_CLICKED(IDC_SAVE_WEAPON_BUTTON, &CDMCustomWeaponDialog::OnBnClickedSaveWeaponButton)
	ON_LBN_SELCHANGE(IDC_WEAPON_LIST, &CDMCustomWeaponDialog::OnLbnSelchangeWeaponList)
	ON_BN_CLICKED(IDC_DEBUG_BUTTON2, &CDMCustomWeaponDialog::OnBnClickedDebugButton2)
	ON_CBN_SELCHANGE(IDC_AMMO_COMBO, &CDMCustomWeaponDialog::OnCbnSelchangeAmmoCombo)
	ON_BN_CLICKED(IDC_DEBUG_BUTTON3, &CDMCustomWeaponDialog::OnBnClickedDebugButton3)
	ON_EN_CHANGE(IDC_WEAPON_NAME_EDIT, &CDMCustomWeaponDialog::OnEnChangeWeaponNameEdit)
	ON_EN_CHANGE(IDC_WEAPON_ENCUMBRANCE_EDIT, &CDMCustomWeaponDialog::OnEnChangeWeaponEncumbranceEdit)
	ON_EN_CHANGE(IDC_WEAPON_COST_EDIT, &CDMCustomWeaponDialog::OnEnChangeWeaponCostEdit)
	ON_CBN_SELCHANGE(IDC_COIN_COMBO, &CDMCustomWeaponDialog::OnCbnSelchangeCoinCombo)
	ON_EN_CHANGE(IDC_WEAPON_XP_EDIT, &CDMCustomWeaponDialog::OnEnChangeWeaponXpEdit)
	ON_EN_CHANGE(IDC_WEAPON_VS_AC_EDIT0, &CDMCustomWeaponDialog::OnEnChangeWeaponVsAcEdit0)
	ON_EN_CHANGE(IDC_WEAPON_VS_AC_EDIT1, &CDMCustomWeaponDialog::OnEnChangeWeaponVsAcEdit1)
	ON_EN_CHANGE(IDC_WEAPON_VS_AC_EDIT2, &CDMCustomWeaponDialog::OnEnChangeWeaponVsAcEdit2)
	ON_EN_CHANGE(IDC_WEAPON_VS_AC_EDIT3, &CDMCustomWeaponDialog::OnEnChangeWeaponVsAcEdit3)
	ON_EN_CHANGE(IDC_WEAPON_VS_AC_EDIT4, &CDMCustomWeaponDialog::OnEnChangeWeaponVsAcEdit4)
	ON_EN_CHANGE(IDC_WEAPON_VS_AC_EDIT5, &CDMCustomWeaponDialog::OnEnChangeWeaponVsAcEdit5)
	ON_EN_CHANGE(IDC_WEAPON_VS_AC_EDIT6, &CDMCustomWeaponDialog::OnEnChangeWeaponVsAcEdit6)
	ON_EN_CHANGE(IDC_WEAPON_VS_AC_EDIT7, &CDMCustomWeaponDialog::OnEnChangeWeaponVsAcEdit7)
	ON_EN_CHANGE(IDC_WEAPON_VS_AC_EDIT8, &CDMCustomWeaponDialog::OnEnChangeWeaponVsAcEdit8)
	ON_EN_CHANGE(IDC_WEAPON_VS_AC_EDIT9, &CDMCustomWeaponDialog::OnEnChangeWeaponVsAcEdit9)
	ON_EN_CHANGE(IDC_WEAPON_VS_AC_EDIT10, &CDMCustomWeaponDialog::OnEnChangeWeaponVsAcEdit10)
	ON_EN_CHANGE(IDC_DAMAGE_SMALL_LOW, &CDMCustomWeaponDialog::OnEnChangeDamageSmallLow)
	ON_EN_CHANGE(IDC_DAMAGE_SMALL_HIGH, &CDMCustomWeaponDialog::OnEnChangeDamageSmallHigh)
	ON_EN_CHANGE(IDC_DAMAGE_LARGE_LOW, &CDMCustomWeaponDialog::OnEnChangeDamageLargeLow)
	ON_EN_CHANGE(IDC_DAMAGE_LARGE_HIGH, &CDMCustomWeaponDialog::OnEnChangeDamageLargeHigh)
	ON_EN_CHANGE(IDC_SPACE_REQD, &CDMCustomWeaponDialog::OnEnChangeSpaceReqd)
	ON_EN_CHANGE(IDC_WEAPON_SPEED, &CDMCustomWeaponDialog::OnEnChangeWeaponSpeed)
	ON_BN_CLICKED(IDC_CLERIC_CHECK, &CDMCustomWeaponDialog::OnBnClickedClericCheck)
	ON_BN_CLICKED(IDC_MAGICUSER_CHECK, &CDMCustomWeaponDialog::OnBnClickedMagicuserCheck)
	ON_BN_CLICKED(IDC_THIEF_CHECK, &CDMCustomWeaponDialog::OnBnClickedThiefCheck)
	ON_BN_CLICKED(IDC_EXCEPT_STR_CHECK, &CDMCustomWeaponDialog::OnBnClickedExceptStrCheck)
	ON_BN_CLICKED(IDC_DEBUG_BUTTON4, &CDMCustomWeaponDialog::OnBnClickedDebugButton4)
END_MESSAGE_MAP()


// CDMCustomWeaponDialog message handlers

void CDMCustomWeaponDialog::ClearWeapon()
{
	m_Weapon.Init();

	m_bChanged = FALSE;
}

void CDMCustomWeaponDialog::CheckSaveChangedWeapon()
{
	if (m_bChanged)
	{
		if (AfxMessageBox("Save changes to current weapon ?", MB_YESNO) == IDYES)
		{
			OnBnClickedSaveWeaponButton();
		}
	}
}

void CDMCustomWeaponDialog::SaveCustomWeapon()
{
#if 1

	CString szTemp;

	CWaitCursor myWaitCursor;

	BOOL bFirst = TRUE;

	szTemp.Format("%sData/tables/CustomWeapons.dat", m_pApp->m_szEXEPath.GetBuffer(0));

	FILE *pOutFile = fopen(szTemp.GetBuffer(0), "wt");

	if (pOutFile != NULL)
	{
		fprintf(pOutFile, "# id, type,  weight, speed/fire rate, space required/range, damage small/med, damage large, execption strength added to to hit/damage, flags\n");
		fprintf(pOutFile, "# DO NOT modify this file directly, use the custom weapons editor!\n");
		fprintf(pOutFile, "# flags are one digit ammo type required, then ammunition type,then usable by clerics, thieves, then magic-users (111 indicates can be used by all)\n");
		fprintf(pOutFile, "# weapons are listed in the order that they will be presented in the program\n");
		fprintf(pOutFile, "#ID,	TYPE,			COST,	WGHT,	SPACE,		SPD,	DMGS,	DMGL,	EXCP,	FLAGS	,WvAC-0,WvAC-1,WvAC-2,WvAC-3,WvAC-4,WvAC-5,WvAC-6,WvAC-7,WvAC-8,WvAC-9,WvAC-10, CUST AMMO TYPE, CUST FLAGS\n");

		fprintf(pOutFile, "# File Version: (DO NOT CHANGE THE NEXT LINE !)\n");
		fprintf(pOutFile, "%ld\n", DMH_CURRENT_VERSION);

		
		//sort the list
		int n1 = m_pApp->m_CustomWeaponsOrderedTypeArray.GetSize();
		int n2 = m_pApp->m_CustomWeaponsOrderedTypeArray.GetCount();

		POBJECTTYPEARRAY _LocalArray;

		for (int i = 0; i < m_pApp->m_CustomWeaponsOrderedTypeArray.GetSize(); ++i)
		{
			cDNDWeapon *pWeapon = (cDNDWeapon *)m_pApp->m_CustomWeaponsOrderedTypeArray.GetAt(i);

			if (pWeapon == NULL)
				continue;

			_LocalArray.InsertAt(pWeapon->m_wTypeId, pWeapon);
		}

		int p = _LocalArray.GetSize();

		m_pApp->m_CustomWeaponsOrderedTypeArray.RemoveAll();

		for (int i = 0; i < _LocalArray.GetSize(); ++i)
		{
			cDNDWeapon *pWeapon = (cDNDWeapon *)_LocalArray.GetAt(i);

			if (pWeapon == NULL)
				continue;

			m_pApp->m_CustomWeaponsOrderedTypeArray.InsertAt(pWeapon->m_wTypeId, pWeapon);
		}

		_LocalArray.RemoveAll();

		for (int k = 0; k < m_pApp->m_CustomWeaponsOrderedTypeArray.GetSize(); ++k)
		{
			cDNDWeapon *pWeaponBase = (cDNDWeapon *)m_pApp->m_CustomWeaponsOrderedTypeArray.GetAt(k);

			if (pWeaponBase != NULL)
			{
				CString szName = pWeaponBase->m_szType;
				szName.Replace(",", "_");
				szName.Replace("(", "{");
				szName.Replace(")", "}");


				if (bFirst)
				{
					bFirst = FALSE;
				}
				else
				{
					fprintf(pOutFile, "\n");
				}

				CString szExceptionalStrength = _T("Y");
				if (IsMissileWeapon(pWeaponBase) && !pWeaponBase->m_nExceptionalStrength)
				{
					szExceptionalStrength = _T("N");
				}
				if (IsAmmo(pWeaponBase))
				{
					szExceptionalStrength = _T("N");
				}

				CString szFlags = _T("");
				szFlags.Format("%05d", pWeaponBase->m_nFlags);

				//"#ID,	TYPE,			COST,	WGHT,	SPACE,		SPD,	DMGS,	DMGL,	EXCP,	FLAGS	,WvAC-0,WvAC-1,WvAC-2,WvAC-3,WvAC-4,WvAC-5,WvAC-6,WvAC-7,WvAC-8,WvAC-9,WvAC-10\n");
				szTemp.Format("%d,%s,%dc,%d,%s,%s,%s,%s,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%u", pWeaponBase->m_wTypeId, szName, pWeaponBase->m_nCost, pWeaponBase->m_nWeight, pWeaponBase->m_szSpeed, pWeaponBase->m_szSpaceRange, pWeaponBase->m_szDamageSmall, pWeaponBase->m_szDamageLarge, szExceptionalStrength, szFlags,
					pWeaponBase->m_nWeaponVSAC[10],
					pWeaponBase->m_nWeaponVSAC[9],
					pWeaponBase->m_nWeaponVSAC[8],
					pWeaponBase->m_nWeaponVSAC[7],
					pWeaponBase->m_nWeaponVSAC[6],
					pWeaponBase->m_nWeaponVSAC[5],
					pWeaponBase->m_nWeaponVSAC[4],
					pWeaponBase->m_nWeaponVSAC[3],
					pWeaponBase->m_nWeaponVSAC[2],
					pWeaponBase->m_nWeaponVSAC[1],
					pWeaponBase->m_nWeaponVSAC[0],
					pWeaponBase->m_nCustomAmmoType,
					pWeaponBase->m_nCustomFlags);

				fprintf(pOutFile, szTemp.GetBuffer(0));
			}
		}

		fclose(pOutFile);

		m_pApp->InitializeObjectTables();
	}

	PopulateWeaponList();

#endif
}

BOOL CDMCustomWeaponDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

#if _DEBUG_CUST_WEAPONS
	m_cDebugButton.ShowWindow(SW_SHOW);
	m_cDebug2Button.ShowWindow(SW_SHOW);
	m_cDebug3Button.ShowWindow(SW_SHOW);
	m_cDebug4Button.ShowWindow(SW_SHOW);
#endif

	m_cCoinCombo.InsertString(0, "g.p.");
	m_cCoinCombo.InsertString(1, "s.p.");
	m_cCoinCombo.InsertString(2, "c.p.");

	m_cCoinCombo.SetCurSel(0);

	PopulateWeaponList();

	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDMCustomWeaponDialog::PopulateWeaponList()
{
	m_cWeaponList.ResetContent();

	int nIndex = 0;

	for (int i = 0; i < m_pApp->m_CustomWeaponsOrderedTypeArray.GetSize(); ++i)
	{
		PWEAPONTYPE pWeapon = (PWEAPONTYPE)m_pApp->m_CustomWeaponsOrderedTypeArray.GetAt(i);

		if (pWeapon != NULL)
		{
			CString szTemp;
			szTemp.Format("%s", pWeapon->m_szType);

			#if _DEBUG_CUST_WEAPONS
			CString szDebug;
			szDebug.Format(" { %d %d %d }", GetAmmoUsedFlag(pWeapon), GetAmmoTypeFlag(pWeapon), pWeapon->m_nCustomAmmoType);
			szTemp += szDebug;
			#endif

			if (-1 == szTemp.Find("(")) // no ammo stacks wanted !
			{
				m_cWeaponList.InsertString(nIndex, szTemp);
				m_cWeaponList.SetItemData(nIndex++, pWeapon->m_wTypeId);
			}
		}
	}
}

void CDMCustomWeaponDialog::PopulateAmmoList()
{
	m_cAmmoCombo.ResetContent();
	int nIndex = 0;

	m_cAmmoCombo.InsertString(nIndex, "thrown");
	m_cAmmoCombo.SetItemData(nIndex++, 9);

	m_cAmmoCombo.InsertString(nIndex, "arrow");
	m_cAmmoCombo.SetItemData(nIndex++, 1);

	m_cAmmoCombo.InsertString(nIndex, "quarrel");
	m_cAmmoCombo.SetItemData(nIndex++, 2);

	m_cAmmoCombo.InsertString(nIndex, "bullet");
	m_cAmmoCombo.SetItemData(nIndex++, 3);

	for (int i = 0; i < m_pApp->m_AmmunitionOrderedTypeArray.GetSize(); ++i)
	{
		PWEAPONTYPE pWeapon = (PWEAPONTYPE)m_pApp->m_AmmunitionOrderedTypeArray.GetAt(i);

		if (pWeapon != NULL)
		{
			if (!IsAmmo(pWeapon))
				continue;

			CString szTemp;
			szTemp.Format("%s", pWeapon->m_szType);

			if (GetAmmoTypeFlag(pWeapon) == 8) 
			{
				m_cAmmoCombo.InsertString(nIndex, pWeapon->m_szType);
				m_cAmmoCombo.SetItemData(nIndex++, pWeapon->m_wTypeId);
			}
		}
	}

	UpdateData(FALSE);

}
char * CDMCustomWeaponDialog::PlusMinus(int nVal)
{
	static CString szRetVal = _T("-");

	if (nVal < 0)
		szRetVal.Format("%d", nVal);
	else if (nVal > 0)
		szRetVal.Format("+%d", nVal);
	else
		szRetVal = _T("-");

	return szRetVal.GetBuffer(0);
}

void CDMCustomWeaponDialog::InitView()
{
	m_szWeaponName = m_Weapon.m_szType;
	m_szEncumbrance.Format("%d", m_Weapon.m_nWeight);

	int nCopperCost = m_Weapon.m_nCost;

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
	m_szXP.Format("%d", m_Weapon.m_lXP);

	m_szWeaponVsAC_0.Format("%s", PlusMinus(m_Weapon.m_nWeaponVSAC[10]));
	m_szWeaponVsAC_1.Format("%s", PlusMinus(m_Weapon.m_nWeaponVSAC[9]));
	m_szWeaponVsAC_2.Format("%s", PlusMinus(m_Weapon.m_nWeaponVSAC[8]));
	m_szWeaponVsAC_3.Format("%s", PlusMinus(m_Weapon.m_nWeaponVSAC[7]));
	m_szWeaponVsAC_4.Format("%s", PlusMinus(m_Weapon.m_nWeaponVSAC[6]));
	m_szWeaponVsAC_5.Format("%s", PlusMinus(m_Weapon.m_nWeaponVSAC[5]));
	m_szWeaponVsAC_6.Format("%s", PlusMinus(m_Weapon.m_nWeaponVSAC[4]));
	m_szWeaponVsAC_7.Format("%s", PlusMinus(m_Weapon.m_nWeaponVSAC[3]));
	m_szWeaponVsAC_8.Format("%s", PlusMinus(m_Weapon.m_nWeaponVSAC[2]));
	m_szWeaponVsAC_9.Format("%s", PlusMinus(m_Weapon.m_nWeaponVSAC[1]));
	m_szWeaponVsAC_10.Format("%s", PlusMinus(m_Weapon.m_nWeaponVSAC[0]));

	m_szDamageSmallLow = _T("");
	m_szDamageSmallHigh = _T("");
	CString szDamage = m_Weapon.m_szDamageSmall;

	CString szToken = _T("");
	int i = 0; // substring index to extract
	while (AfxExtractSubString(szToken, szDamage, i, '-'))
	{
		if (i == 0)
			m_szDamageSmallLow = szToken;
		else 
			m_szDamageSmallHigh = szToken;
		i++;
	}

	
	m_szDamageLargeLow = _T("");
	m_szDamageLargeHigh = _T(""); 
	szDamage = m_Weapon.m_szDamageLarge;

	i = 0; // substring index to extract
	while (AfxExtractSubString(szToken, szDamage, i, '-'))
	{
		if (i == 0)
			m_szDamageLargeLow = szToken;
		else
			m_szDamageLargeHigh = szToken;
		i++;
	}

	m_szSpaceRequired = m_Weapon.m_szSpaceRange;
	m_szWeaponSpeed = m_Weapon.m_szSpeed;

	if (GetAmmoUsedFlag(&m_Weapon))
	{
		m_bMissile = TRUE;
		PopulateAmmoList();

		m_bExceptionalStrength = m_Weapon.m_nExceptionalStrength;
	}
	else
	{
		m_bMissile = FALSE;
	}

	if (m_Weapon.m_ObjectType == DND_OBJECT_TYPE_AMMO)
		m_bAmmo = TRUE;
	else
		m_bAmmo = FALSE;

	if(m_Weapon.m_nCustomFlags & 0x0001)
		m_bSword = TRUE;
	else
		m_bSword = FALSE;

	if(GetObjectFlag(&m_Weapon, 2))
	{
		m_bCleric = TRUE;
	}
	else
	{
		m_bCleric = FALSE;
	}

	if (GetObjectFlag(&m_Weapon, 1))
	{
		m_bThief = TRUE;
	}
	else
	{
		m_bThief = FALSE;
	}

	if (GetObjectFlag(&m_Weapon, 0))
	{
		m_bMagicUser = TRUE;
	}
	else
	{
		m_bMagicUser = FALSE;
	}

	UpdateData(FALSE);

}

void CDMCustomWeaponDialog::Refresh()
{
	if (m_bMissile)
	{
		m_cAmmoCombo.ShowWindow(SW_SHOW);
		m_cAmmoLabel.ShowWindow(SW_SHOW);
		m_cExceptionalStrength.ShowWindow(SW_SHOW);

		m_cAmmo.ShowWindow(SW_HIDE);
		m_cSword.ShowWindow(SW_HIDE);

		if (GetAmmoUsedFlag(&m_Weapon) == 9)
		{
			m_cAmmoCombo.SetCurSel(0);
		}
		else
		{
			for (int i = 1; i < m_cAmmoCombo.GetCount(); ++i)
			{
				if (GetAmmoUsedFlag(&m_Weapon) == m_cAmmoCombo.GetItemData(i))
				{
					m_cAmmoCombo.SetCurSel(i);
					break;
				}
				if (m_Weapon.m_nCustomAmmoType == m_cAmmoCombo.GetItemData(i))
				{
					m_cAmmoCombo.SetCurSel(i);
					break;
				}
			}
		}

	}
	else
	{
		m_cAmmoCombo.ShowWindow(SW_HIDE);
		m_cAmmoLabel.ShowWindow(SW_HIDE);
		m_cExceptionalStrength.ShowWindow(SW_HIDE);

		m_cAmmo.ShowWindow(SW_SHOW);
		m_cSword.ShowWindow(SW_SHOW);
	}

	m_cThief.EnableWindow(TRUE);
	m_cCleric.EnableWindow(TRUE);
	m_cMagicUser.EnableWindow(TRUE);

	if (m_bAmmo)
	{
		m_cThief.EnableWindow(FALSE);
		m_cCleric.EnableWindow(FALSE);
		m_cMagicUser.EnableWindow(FALSE);

		m_bThief = FALSE;
		m_bCleric = FALSE;
		m_bMagicUser = FALSE;

		m_bExceptionalStrength = FALSE;

		m_Weapon.m_ObjectType = DND_OBJECT_TYPE_AMMO;
		SetAmmoTypeFlag(&m_Weapon, 8, 0);
	}
	else if (m_bMissile)
	{
		m_Weapon.m_ObjectType = DND_OBJECT_TYPE_WEAPON;
	}
	else if (m_bSword)
	{
		m_bExceptionalStrength = FALSE;

		m_Weapon.m_ObjectType = DND_OBJECT_TYPE_WEAPON;
		SetAmmoTypeFlag(&m_Weapon, 0, 0);

		m_Weapon.m_nCustomFlags |= 0x0001;
	}
	else
	{
		m_bExceptionalStrength = FALSE;

		m_Weapon.m_ObjectType = DND_OBJECT_TYPE_WEAPON;
		SetAmmoTypeFlag(&m_Weapon, 0, 0);
	}

	BOOL bFoundSel = FALSE;
	for (int i = 0; i < m_cWeaponList.GetCount(); ++i)
	{
		if (m_cWeaponList.GetItemData(i) == m_Weapon.m_wTypeId)
		{
			m_cWeaponList.SetCurSel(i);
			bFoundSel = TRUE;
			break;
		}
	}
	if (!bFoundSel)
	{
		m_cWeaponList.SetCurSel(-1);
		m_cDeleteWeaponButton.EnableWindow(FALSE);
	}
	else
	{
		m_cDeleteWeaponButton.EnableWindow(TRUE);
	}

	if (m_bChanged && m_szWeaponName.GetLength() >= 1)
	{
		m_cSaveWeaponButton.EnableWindow(TRUE);
	}
	else
	{
		m_cSaveWeaponButton.EnableWindow(FALSE);
	}

	#if _DEBUG_CUST_WEAPONS
	m_szDebugString.Format("( %d %d %d )", GetAmmoUsedFlag(&m_Weapon), GetAmmoTypeFlag(&m_Weapon), m_Weapon.m_nCustomAmmoType);
	#endif

	UpdateData(FALSE);
}

void CDMCustomWeaponDialog::OnBnClickedOk()
{
	CDialog::OnOK();
}


void CDMCustomWeaponDialog::OnBnClickedCancel()
{
	CheckSaveChangedWeapon();

	CDialog::OnCancel();
}

void CDMCustomWeaponDialog::OnLbnSelchangeWeaponList()
{
	CheckSaveChangedWeapon();

	UpdateData(TRUE);

	int nCursor = m_cWeaponList.GetCurSel();

	if (nCursor > -1)
	{
		WORD wID = (WORD)m_cWeaponList.GetItemData(nCursor);

		for (int k = 0; k < m_pApp->m_CustomWeaponsOrderedTypeArray.GetSize(); ++k)
		{
			cDNDWeapon *pWeaponBase = (cDNDWeapon *)m_pApp->m_CustomWeaponsOrderedTypeArray.GetAt(k);

			if (pWeaponBase != NULL)
			{
				if (pWeaponBase->m_wTypeId == wID)
				{
					pWeaponBase->CopyFull(&m_Weapon);

					InitView();

					m_bChanged = FALSE;

					Refresh();

					return;
				}

			}
		}
	}

}


void CDMCustomWeaponDialog::OnBnClickedMissileCheck()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	if (m_bMissile)
	{
		m_bAmmo = FALSE;
		m_bSword = FALSE;

		SetAmmoUsedFlag(&m_Weapon, 9, 0);
		SetObjectFlag(&m_Weapon, 4, 1);

		PopulateAmmoList();
	}
	else
	{
		SetAmmoUsedFlag(&m_Weapon, 0, 0);
		SetObjectFlag(&m_Weapon, 4, 0);
	}

	Refresh();
}


void CDMCustomWeaponDialog::OnBnClickedAmmoCheck()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	if (m_bAmmo)
	{
		m_bMissile = FALSE;
		SetObjectFlag(&m_Weapon, 4, 0);
		m_bSword = FALSE;
	}

	Refresh();
}


void CDMCustomWeaponDialog::OnBnClickedSwordCheck()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	if (m_bSword)
	{
		m_bMissile = FALSE;
		SetObjectFlag(&m_Weapon, 4, 0);
		m_bAmmo = FALSE;
	}

	Refresh();
}


void CDMCustomWeaponDialog::OnBnClickedDebugButton()
{
	m_Weapon.Init();

	m_szWeaponName = _T("katana");
	m_szEncumbrance = _T("50");
	m_szCost = _T("1000");
	m_szXP = _T("20000");

	m_szDamageSmallLow = _T("1");
	m_szDamageSmallHigh = _T("10");
	m_szDamageLargeLow = _T("1");
	m_szDamageLargeHigh = _T("6");

	m_szSpaceRequired = _T("3");
	m_szWeaponSpeed = _T("4");

	m_szWeaponVsAC_0 = _T("-4"); // this is correct for a long sword
	m_szWeaponVsAC_1 = _T("3");
	m_szWeaponVsAC_2 = _T("-2");
	m_szWeaponVsAC_3 = _T("-1");
	m_szWeaponVsAC_4 = _T("-");
	m_szWeaponVsAC_5 = _T("-");
	m_szWeaponVsAC_6 = _T("-");
	m_szWeaponVsAC_7 = _T("-");
	m_szWeaponVsAC_8 = _T("-");
	m_szWeaponVsAC_9 = _T("1");
	m_szWeaponVsAC_10 = _T("2");

	m_bMissile = FALSE;
	m_bAmmo = FALSE;
	m_bSword = TRUE;

	m_bChanged = TRUE;

	Refresh();
}


void CDMCustomWeaponDialog::OnBnClickedDebugButton2()
{
	m_Weapon.Init();

	m_szWeaponName = _T("crossbow, hand");
	m_szEncumbrance = _T("50");
	m_szCost = _T("250");
	m_szXP = _T("10000");

	m_szDamageSmallLow = _T("1");
	m_szDamageSmallHigh = _T("4");
	m_szDamageLargeLow = _T("1");
	m_szDamageLargeHigh = _T("2");

	m_szSpaceRequired = _T("1");
	m_szWeaponSpeed = _T("2");

	m_szWeaponVsAC_0 = _T("-1"); 
	m_szWeaponVsAC_1 = _T("-1");
	m_szWeaponVsAC_2 = _T("-");
	m_szWeaponVsAC_3 = _T("-");
	m_szWeaponVsAC_4 = _T("-");
	m_szWeaponVsAC_5 = _T("-");
	m_szWeaponVsAC_6 = _T("-");
	m_szWeaponVsAC_7 = _T("-");
	m_szWeaponVsAC_8 = _T("1");
	m_szWeaponVsAC_9 = _T("2");
	m_szWeaponVsAC_10 = _T("3");

	m_bMissile = TRUE;
	m_bAmmo = FALSE;
	m_bSword = FALSE;

	SetObjectFlag(&m_Weapon, 4, 1);

	PopulateAmmoList();

	m_bChanged = TRUE;

	Refresh();
}

void CDMCustomWeaponDialog::OnBnClickedDebugButton3()
{
	m_Weapon.Init();

	m_szWeaponName = _T("quarrel, mini");
	m_szEncumbrance = _T("1");
	m_szCost = _T("5");
	m_szXP = _T("5");

	m_szDamageSmallLow = _T("1");
	m_szDamageSmallHigh = _T("4");
	m_szDamageLargeLow = _T("1");
	m_szDamageLargeHigh = _T("2");

	m_szSpaceRequired = _T("1");
	m_szWeaponSpeed = _T("2");

	m_szWeaponVsAC_0 = _T("-1");
	m_szWeaponVsAC_1 = _T("-1");
	m_szWeaponVsAC_2 = _T("-");
	m_szWeaponVsAC_3 = _T("-");
	m_szWeaponVsAC_4 = _T("-");
	m_szWeaponVsAC_5 = _T("-");
	m_szWeaponVsAC_6 = _T("-");
	m_szWeaponVsAC_7 = _T("-");
	m_szWeaponVsAC_8 = _T("1");
	m_szWeaponVsAC_9 = _T("2");
	m_szWeaponVsAC_10 = _T("3");

	m_bMissile = FALSE;
	m_bAmmo = TRUE;
	m_bSword = FALSE;

	m_bChanged = TRUE;

	Refresh();
}

void CDMCustomWeaponDialog::OnBnClickedDebugButton4()
{
	m_Weapon.Init();

	m_szWeaponName = _T("shuriken");
	m_szEncumbrance = _T("1");
	m_szCost = _T("1");
	m_szXP = _T("1");

	m_szDamageSmallLow = _T("1");
	m_szDamageSmallHigh = _T("2");
	m_szDamageLargeLow = _T("1");
	m_szDamageLargeHigh = _T("2");

	m_szSpaceRequired = _T("1");
	m_szWeaponSpeed = _T("1");

	m_szWeaponVsAC_0 = _T("-4");
	m_szWeaponVsAC_1 = _T("-3");
	m_szWeaponVsAC_2 = _T("-2");
	m_szWeaponVsAC_3 = _T("-1");
	m_szWeaponVsAC_4 = _T("-");
	m_szWeaponVsAC_5 = _T("-");
	m_szWeaponVsAC_6 = _T("-");
	m_szWeaponVsAC_7 = _T("-");
	m_szWeaponVsAC_8 = _T("1");
	m_szWeaponVsAC_9 = _T("2");
	m_szWeaponVsAC_10 = _T("3");

	m_bMissile = TRUE;
	m_bAmmo = FALSE;
	m_bSword = FALSE;

	SetAmmoUsedFlag(&m_Weapon, 9, 0);

	PopulateAmmoList();

	m_bChanged = TRUE;

	Refresh();
}

void CDMCustomWeaponDialog::OnBnClickedNewWeaponButton()
{
	m_Weapon.Init();

	m_bChanged = FALSE;

	InitView();

	Refresh();
}


void CDMCustomWeaponDialog::OnBnClickedDeleteWeaponButton()
{
	UpdateData(TRUE);

	int nCursor = m_cWeaponList.GetCurSel();

	if (nCursor > -1)
	{
		if (AfxMessageBox("Weapon will be deleted completely !\nAre you sure ?", MB_YESNO) != IDYES)
			return;

		WORD wID = (WORD)m_cWeaponList.GetItemData(nCursor);

		cDNDWeapon *pWeapon = (cDNDWeapon *)m_pApp->m_ObjectsIndexedTypeArray.GetAt(wID);

		if (pWeapon != NULL)
		{
			for (int k = 0; k < m_pApp->m_CustomWeaponsOrderedTypeArray.GetSize(); ++k)
			{
				cDNDWeapon *pWeaponBase = (cDNDWeapon *)m_pApp->m_CustomWeaponsOrderedTypeArray.GetAt(k);

				if (pWeaponBase != NULL)
				{
					if (pWeaponBase->m_wTypeId == pWeapon->m_wTypeId)
					{
						m_pApp->m_CustomWeaponsOrderedTypeArray.SetAt(k, NULL);
					}
				}
			}

			cDNDWeapon *pIndexedWeapon = (cDNDWeapon *)m_pApp->m_ObjectsIndexedTypeArray.GetAt(pWeapon->m_wTypeId);
			if (pIndexedWeapon != NULL)
			{
				m_pApp->m_ObjectsIndexedTypeArray.RemoveAt(pIndexedWeapon->m_wTypeId);
				//delete pIndexedWeapon; // // don't delete it here !
			}

			//find it and delete it from this array
			for (int i = 0; i < m_pApp->m_ObjectsOrderedTypeArray.GetSize(); ++i)
			{
				cDNDWeapon *pOrderedWeapon = (cDNDWeapon *)m_pApp->m_ObjectsOrderedTypeArray.GetAt(i);
				if (pOrderedWeapon != NULL && pOrderedWeapon->m_wTypeId == pWeapon->m_wTypeId)
				{
					m_pApp->m_ObjectsOrderedTypeArray.RemoveAt(i);
					delete pOrderedWeapon;
					break;
				}
			}

		}

		m_bChanged = FALSE;

		ClearWeapon();

		SaveCustomWeapon();

		InitView();

		Refresh();
		
	}
}


void CDMCustomWeaponDialog::OnBnClickedSaveWeaponButton()
{
	UpdateData(TRUE);

	strcpy(m_Weapon.m_szType, m_szWeaponName.GetBuffer(0));

	m_Weapon.m_nWeight = atoi(m_szEncumbrance.GetBuffer(0));
	m_Weapon.m_nCost = atoi(m_szCost.GetBuffer(0));
	m_Weapon.m_lXP = atoi(m_szXP.GetBuffer(0));

	int nCoin = m_cCoinCombo.GetCurSel();

	switch (nCoin)
	{
	case 0:	m_Weapon.m_nCost *= 200; break;
	case 1:	m_Weapon.m_nCost *= 10; break;
	}

	strcpy(m_Weapon.m_szSpeed, m_szWeaponSpeed);
	strcpy(m_Weapon.m_szSpaceRange, m_szSpaceRequired);

	sprintf(m_Weapon.m_szDamageSmall, "%s-%s", m_szDamageSmallLow.Left(2), m_szDamageSmallHigh.Left(2));
	sprintf(m_Weapon.m_szDamageLarge, "%s-%s", m_szDamageLargeLow.Left(2), m_szDamageLargeHigh.Left(2));

	m_Weapon.m_nWeaponVSAC[10] = atoi(m_szWeaponVsAC_0.GetBuffer(0));
	m_Weapon.m_nWeaponVSAC[9] = atoi(m_szWeaponVsAC_1.GetBuffer(0));
	m_Weapon.m_nWeaponVSAC[8] = atoi(m_szWeaponVsAC_2.GetBuffer(0));
	m_Weapon.m_nWeaponVSAC[7] = atoi(m_szWeaponVsAC_3.GetBuffer(0));
	m_Weapon.m_nWeaponVSAC[6] = atoi(m_szWeaponVsAC_4.GetBuffer(0));
	m_Weapon.m_nWeaponVSAC[5] = atoi(m_szWeaponVsAC_5.GetBuffer(0));
	m_Weapon.m_nWeaponVSAC[4] = atoi(m_szWeaponVsAC_6.GetBuffer(0));
	m_Weapon.m_nWeaponVSAC[3] = atoi(m_szWeaponVsAC_7.GetBuffer(0));
	m_Weapon.m_nWeaponVSAC[2] = atoi(m_szWeaponVsAC_8.GetBuffer(0));
	m_Weapon.m_nWeaponVSAC[1] = atoi(m_szWeaponVsAC_9.GetBuffer(0));
	m_Weapon.m_nWeaponVSAC[0] = atoi(m_szWeaponVsAC_10.GetBuffer(0));

#if 1
	cDNDWeapon *pWeapon = NULL;
	if (m_Weapon.m_wTypeId == 0)
	{
		pWeapon = new cDNDWeapon();

		m_Weapon.CopyTo(pWeapon);

		pWeapon->m_wTypeId = m_pApp->GetNextCustomWeaponIndex();

		if (pWeapon->m_ObjectType == DND_OBJECT_TYPE_AMMO)
		{
			pWeapon->m_nCustomAmmoType = pWeapon->m_wTypeId;
			m_Weapon.m_nCustomAmmoType = pWeapon->m_wTypeId;
		}

		m_pApp->m_CustomWeaponsOrderedTypeArray.InsertAt(pWeapon->m_wTypeId, pWeapon, 1);

		int nDefinedObjectCount = m_pApp->m_ObjectsOrderedTypeArray.GetCount();
		m_pApp->m_ObjectsOrderedTypeArray.InsertAt(nDefinedObjectCount, pWeapon->Clone());
		m_pApp->m_ObjectsIndexedTypeArray.InsertAt(pWeapon->m_wTypeId, pWeapon);

		if (pWeapon->m_ObjectType == DND_OBJECT_TYPE_AMMO)
		{
			int nDefinedAmmoCount = m_pApp->m_AmmunitionOrderedTypeArray.GetCount();
			m_pApp->m_AmmunitionOrderedTypeArray.InsertAt(nDefinedAmmoCount, pWeapon);
			m_pApp->m_AmmunitionIndexedTypeArray.InsertAt(pWeapon->m_wTypeId, pWeapon);
		}
		else
		{
			m_pApp->m_WeaponsOrderedTypeArray.InsertAt(nDefinedObjectCount, pWeapon);
			m_pApp->m_WeaponsIndexedTypeArray.InsertAt(pWeapon->m_wTypeId, pWeapon);
		}

		m_Weapon.m_wTypeId = pWeapon->m_wTypeId;
	}
	else
	{
		pWeapon = (cDNDWeapon*)m_pApp->m_ObjectsIndexedTypeArray.GetAt(m_Weapon.m_wTypeId);

		if (pWeapon != NULL)
		{
			m_Weapon.CopyTo(pWeapon);

			if (pWeapon->m_ObjectType == DND_OBJECT_TYPE_WEAPON)
			{
				//remove it from the ammo array if it was ever in there
				for (int i = 0; i < m_pApp->m_AmmunitionOrderedTypeArray.GetSize(); ++i)
				{
					cDNDWeapon *pFormerAmmo = m_pApp->m_AmmunitionOrderedTypeArray.GetAt(i);

					if (pFormerAmmo == pWeapon)
					{
						m_pApp->m_AmmunitionOrderedTypeArray.RemoveAt(i);
						m_pApp->m_AmmunitionIndexedTypeArray.RemoveAt(pWeapon->m_wTypeId);

						//add it to the weapon arrays
						int nDefinedWeaponCount = m_pApp->m_WeaponsOrderedTypeArray.GetCount();
						m_pApp->m_WeaponsOrderedTypeArray.InsertAt(nDefinedWeaponCount, pWeapon);
						m_pApp->m_WeaponsIndexedTypeArray.InsertAt(pWeapon->m_wTypeId, pWeapon);
					}
				}

				//find it and copy it to this array
				for (int i = 0; i < m_pApp->m_WeaponsOrderedTypeArray.GetSize(); ++i)
				{
					cDNDWeapon *pOrderedWeapon = NULL;
					pOrderedWeapon = m_pApp->m_WeaponsOrderedTypeArray.GetAt(i);
					if (pOrderedWeapon != NULL && pOrderedWeapon->m_wTypeId == pWeapon->m_wTypeId)
					{
						m_Weapon.CopyTo(pOrderedWeapon);
						break;
					}
				}
			}
			else if (pWeapon->m_ObjectType == DND_OBJECT_TYPE_AMMO)
			{
				//remove it from the weapon array if it was ever in there
				for (int i = 0; i < m_pApp->m_WeaponsOrderedTypeArray.GetSize(); ++i)
				{
					cDNDWeapon *pFormerWeapon = m_pApp->m_WeaponsOrderedTypeArray.GetAt(i);

					if (pFormerWeapon == pWeapon)
					{
						m_pApp->m_WeaponsOrderedTypeArray.RemoveAt(i);
						m_pApp->m_WeaponsIndexedTypeArray.RemoveAt(pFormerWeapon->m_wTypeId);

						//add it to the ammo array now
						int nDefinedAmmoCount = m_pApp->m_AmmunitionOrderedTypeArray.GetCount();
						m_pApp->m_AmmunitionOrderedTypeArray.InsertAt(nDefinedAmmoCount, pWeapon);
						m_pApp->m_AmmunitionIndexedTypeArray.InsertAt(pWeapon->m_wTypeId, pWeapon);
					}
				}

				//find it and copy it to this array
				for (int i = 0; i < m_pApp->m_AmmunitionOrderedTypeArray.GetSize(); ++i)
				{
					cDNDWeapon *pOrderedAmmo = NULL;
					pOrderedAmmo = m_pApp->m_AmmunitionOrderedTypeArray.GetAt(i);
					if (pOrderedAmmo != NULL && pOrderedAmmo->m_wTypeId == pWeapon->m_wTypeId)
					{
						m_Weapon.CopyTo(pOrderedAmmo);
						break;
					}
				}
			}
		}
	}

	m_pApp->AlphabetizeObjectList();

#endif

	m_bChanged = FALSE;

	SaveCustomWeapon();
	 
	Refresh();
}





void CDMCustomWeaponDialog::OnCbnSelchangeAmmoCombo()
{
	UpdateData(TRUE);

	int nCursor = m_cAmmoCombo.GetCurSel();

	if (nCursor > -1)
	{
		int nFlag = m_cAmmoCombo.GetItemData(nCursor);

		SetAmmoTypeFlag(&m_Weapon, 0, 0);
		SetAmmoUsedFlag(&m_Weapon, 0, 0);

		switch (nFlag)
		{
			case 9:
			{
				SetAmmoUsedFlag(&m_Weapon, 90000, 0);
				break;
			}
			case 1:
			case 2:
			case 3:
			{
				SetAmmoUsedFlag(&m_Weapon, nFlag, 0);
				break;
			}
			default: // custom ammo type
			{
				SetAmmoUsedFlag(&m_Weapon, 8, nFlag);
				break;
			}
		}

		m_bChanged = TRUE;

		Refresh();

	}
}




void CDMCustomWeaponDialog::OnEnChangeWeaponNameEdit()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	Refresh();
}


void CDMCustomWeaponDialog::OnEnChangeWeaponEncumbranceEdit()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	Refresh();
}


void CDMCustomWeaponDialog::OnEnChangeWeaponCostEdit()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	Refresh();
}


void CDMCustomWeaponDialog::OnCbnSelchangeCoinCombo()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	Refresh();
}


void CDMCustomWeaponDialog::OnEnChangeWeaponXpEdit()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	Refresh();
}


void CDMCustomWeaponDialog::OnEnChangeWeaponVsAcEdit0()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	Refresh();
}


void CDMCustomWeaponDialog::OnEnChangeWeaponVsAcEdit1()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	Refresh();
}


void CDMCustomWeaponDialog::OnEnChangeWeaponVsAcEdit2()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	Refresh();
}


void CDMCustomWeaponDialog::OnEnChangeWeaponVsAcEdit3()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	Refresh();
}


void CDMCustomWeaponDialog::OnEnChangeWeaponVsAcEdit4()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	Refresh();
}


void CDMCustomWeaponDialog::OnEnChangeWeaponVsAcEdit5()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	Refresh();
}


void CDMCustomWeaponDialog::OnEnChangeWeaponVsAcEdit6()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	Refresh();
}


void CDMCustomWeaponDialog::OnEnChangeWeaponVsAcEdit7()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	Refresh();
}


void CDMCustomWeaponDialog::OnEnChangeWeaponVsAcEdit8()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	Refresh();
}


void CDMCustomWeaponDialog::OnEnChangeWeaponVsAcEdit9()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	Refresh();
}


void CDMCustomWeaponDialog::OnEnChangeWeaponVsAcEdit10()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	Refresh();
}


void CDMCustomWeaponDialog::OnEnChangeDamageSmallLow()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	Refresh();
}


void CDMCustomWeaponDialog::OnEnChangeDamageSmallHigh()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	Refresh();
}


void CDMCustomWeaponDialog::OnEnChangeDamageLargeLow()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	Refresh();
}


void CDMCustomWeaponDialog::OnEnChangeDamageLargeHigh()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	Refresh();
}


void CDMCustomWeaponDialog::OnEnChangeSpaceReqd()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	Refresh();
}


void CDMCustomWeaponDialog::OnEnChangeWeaponSpeed()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	Refresh();
}


void CDMCustomWeaponDialog::OnBnClickedClericCheck()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	if(m_bCleric)
		SetObjectFlag(&m_Weapon, 2, 1);
	else
		SetObjectFlag(&m_Weapon, 2, 0);

	Refresh();
}


void CDMCustomWeaponDialog::OnBnClickedMagicuserCheck()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	if (m_bMagicUser)
		SetObjectFlag(&m_Weapon, 0, 1);
	else
		SetObjectFlag(&m_Weapon, 0, 0);

	Refresh();
}


void CDMCustomWeaponDialog::OnBnClickedThiefCheck()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	if (m_bThief)
		SetObjectFlag(&m_Weapon, 1, 1);
	else
		SetObjectFlag(&m_Weapon, 1, 0);

	Refresh();
}


void CDMCustomWeaponDialog::OnBnClickedExceptStrCheck()
{
	UpdateData(TRUE);

	m_bChanged = TRUE;

	if (m_bExceptionalStrength)
		m_Weapon.m_nExceptionalStrength = 1;
	else
		m_Weapon.m_nExceptionalStrength = 0;

	Refresh();
}


