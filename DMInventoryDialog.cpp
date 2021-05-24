// DMInventoryDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DM HelperDlg.h"
#include "DMInventoryDialog.h"
#include "DMCharSpellsDialog.h"
#include "DMCharViewDialog.h"
#include "DMNPCViewDialog.h"
#include "DMPartyDialog.h"
#include "DMModifyValueDialog.h"
#include "DMCharacterSelectorDialog.h"
#include "DMPCDetailsDialog.h"
#include "DMScrollDetailsDialog.h"
#include "DMContainerDetailsDialog.h"
#include "DMMundaneTypeSelectorDialog.h"
#include "DMTreasureTypeDialog.h"
#include "DMMagicSwordPropertiesDialog.h"
#include "DMItemDescriptionDialog.h"
#include "cDMChartLookupDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DMInventoryDialog dialog

#define COLUMN_SIZE		24

#ifdef _DEBUG
#define _DEBUG_CONTS FALSE
#else
#define _DEBUG_CONTS FALSE // ALWAYS FALSE
#endif


DMInventoryDialog::DMInventoryDialog(CDMHelperDlg* pMainDialog, cDNDCharacter *_Character, CDMCharViewDialog *pPCVWindow, DMNPCViewDialog *pNPCVWindow, CWnd* pParent /*=NULL*/)
	: CDialog(DMInventoryDialog::IDD, pParent)
	, m_szCacheName(_T(""))
	, m_szItemXP(_T(""))
	, m_szCacheDetails(_T(""))
	, m_bGiveWithXPCharacterCheck(FALSE)
	, m_bGiveWithXPPartyCheck(FALSE)
	, m_bDropWithXPCharacterCheck(FALSE)
	, m_bDropWithXPPartyCheck(FALSE)
	, m_szCacheNameDesc(_T("Description:"))
	, m_bPotionsCheck(FALSE)
	, m_bMagicalArmorCheck(FALSE)
	, m_bMagicalSwordsCheck(FALSE)
	, m_bRodsStavesWandsCheck(FALSE)
	, m_bMiscMagic1Check(FALSE)
	, m_bMiscMagic2Check(FALSE)
	, m_bMiscMagic3Check(FALSE)
	, m_bMiscMagic4Check(FALSE)
	, m_bMiscMagic5Check(FALSE)
	, m_bMiscMagicSpecialCheck(FALSE)
	, m_bXPCheck(FALSE)
	, m_bScrollsCheck(FALSE)
	, m_bShowContainerContents(TRUE)
	, m_bItemIdentified(FALSE)
	, m_szInventoryLabel(_T(""))
	, m_bDoubleSpecialized(FALSE)
	, m_bSpecialized(FALSE)
	, m_bInLairCheck(FALSE)
	, m_bMaterialComponentCheck(FALSE)
	, m_szItemDesc(_T(""))
	, m_bIgnoreEncumbrance(FALSE)
{
	//{{AFX_DATA_INIT(DMInventoryDialog)
	m_szEncumbrance = _T("");
	m_szMaxEncumbrance = _T("");
	m_szGoldEdit = _T("");
	m_szCopperEdit = _T("");
	m_szPlatinumEdit = _T("");
	m_szSilverEdit = _T("");
	m_szElectrumEdit = _T("");
	m_bPricesCheck = FALSE;
	m_szTotalInvItems = _T("");
	m_szProffStatic = _T("");
	m_szNonProffStatic = _T("");
	m_szRenameItem = _T("");
	m_bRingsCheck = FALSE;
	m_bWeightsCheck = FALSE;
	//}}AFX_DATA_INIT

	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_pMainDialog = pMainDialog;

	m_pParent = pParent;

	m_bParentClosing = FALSE;

	m_pPCVWindow = pPCVWindow;
	m_pNPCVWindow = pNPCVWindow;

	m_pCharacter = _Character;

	m_pGiftCharacter = NULL;

	//m_nSelectedInventorySlot = -1;
	m_pSelectedInventorySlot = NULL;

	m_pPCDetailsDialog = NULL;
	m_pScrollDetailsDialog = NULL;
	m_pContainerDetailsDialog = NULL;
	m_pMagicSwordPropertiesDialog = NULL;

	m_bMagicItemSelect = FALSE;

	m_bDisablePrompts = FALSE;

	m_dwInventoryFlag = m_pApp->m_dwInventoryFlag;

	Create(DMInventoryDialog::IDD, pParent);
}


//

DMInventoryDialog::~DMInventoryDialog()
{
	CleanUp();
}

void DMInventoryDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DMInventoryDialog)
	DDX_Control(pDX, IDC_RENAME_ITEM_EDIT, m_cRenameEdit);
	DDX_Control(pDX, IDC_PROFF_LIST, m_cProffList);
	DDX_Control(pDX, IDC_GIVE_TO_COMBO, m_cGiveToCombo);
	DDX_Control(pDX, IDC_INVENTORY_LIST, m_cInventoryList);
	DDX_Control(pDX, IDC_STORE_LIST, m_cStoreList);
	DDX_Text(pDX, IDC_ENC_EDIT, m_szEncumbrance);
	DDX_Text(pDX, IDC_MAX_ENC_STATIC, m_szMaxEncumbrance);
	DDX_Text(pDX, IDC_GP_EDIT, m_szGoldEdit);
	DDX_Text(pDX, IDC_CP_EDIT, m_szCopperEdit);
	DDX_Text(pDX, IDC_PP_EDIT, m_szPlatinumEdit);
	DDX_Text(pDX, IDC_SP_EDIT, m_szSilverEdit);
	DDX_Text(pDX, IDC_EP_EDIT, m_szElectrumEdit);
	DDX_Check(pDX, IDC_PRICES_CHECK, m_bPricesCheck);
	DDX_Check(pDX, IDC_WEAPONS_CHECK, m_bWeaponsCheck);
	DDX_Check(pDX, IDC_ARMOR_CHECK, m_bArmorCheck);
	DDX_Check(pDX, IDC_SHIELDS_CHECK, m_bShieldsCheck);
	DDX_Check(pDX, IDC_MISC_WEAPONS_CHECK, m_bMiscWeaponsCheck);
	DDX_Check(pDX, IDC_EQUIP_CHECK, m_bEquipCheck);
	DDX_Text(pDX, IDC_TOTAL_STATIC, m_szTotalInvItems);
	DDX_Text(pDX, IDC_WEP_PROF_STATIC, m_szProffStatic);
	DDX_Text(pDX, IDC_NON_WEP_PROF_STATIC, m_szNonProffStatic);
	DDX_Text(pDX, IDC_RENAME_ITEM_EDIT, m_szRenameItem);
	DDX_Check(pDX, IDC_RINGS_CHECK, m_bRingsCheck);
	DDX_Check(pDX, IDC_WEIGHTS_CHECK, m_bWeightsCheck);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_INVENTORY_STATIC, m_cInventoryStatic);
	DDX_Control(pDX, IDC_WP_STATIC, m_cWPFrame);
	DDX_Text(pDX, IDC_CACHE_NAME_EDIT, m_szCacheName);
	DDX_Control(pDX, IDC_DETAILS_LOCATOR_BUTTON, m_cDetailsLocator);
	DDX_Control(pDX, IDC_ITEM_XP_EDIT, m_cItemXPEdit);
	DDX_Text(pDX, IDC_ITEM_XP_EDIT, m_szItemXP);
	DDX_Text(pDX, IDC_CACHE_DETAILS_EDIT, m_szCacheDetails);
	DDV_MaxChars(pDX, m_szCacheDetails, 512);
	DDX_Control(pDX, IDC_CACHE_STATIC, m_cCachePropertiesFrame);
	DDX_Control(pDX, IDC_GIVE_WITH_XP_CHARACTER_CHECK, m_cGiveWithXPCharacterCheck);
	DDX_Check(pDX, IDC_GIVE_WITH_XP_CHARACTER_CHECK, m_bGiveWithXPCharacterCheck);
	DDX_Control(pDX, IDC_GIVE_WITH_XP_PARTY_CHECK, m_cGiveWithXPPartyCheck);
	DDX_Check(pDX, IDC_GIVE_WITH_XP_PARTY_CHECK, m_bGiveWithXPPartyCheck);
	DDX_Control(pDX, IDC_DROP_WITH_XP_CHARACTER_CHECK, m_cDropWithXPCharacterCheck);
	DDX_Check(pDX, IDC_DROP_WITH_XP_CHARACTER_CHECK, m_bDropWithXPCharacterCheck);
	DDX_Control(pDX, IDC_DROP_WITH_XP_PARTY_CHECK, m_cDropWithXPPartyCheck);
	DDX_Check(pDX, IDC_DROP_WITH_XP_PARTY_CHECK, m_bDropWithXPPartyCheck);
	DDX_Text(pDX, IDC_CACHE_NAME_STATIC, m_szCacheNameDesc);
	DDX_Check(pDX, IDC_POTIONS_CHECK, m_bPotionsCheck);
	DDX_Check(pDX, IDC_MAGICAL_ARMOR_CHECK, m_bMagicalArmorCheck);
	DDX_Check(pDX, IDC_MAGICAL_SWORDS_CHECK, m_bMagicalSwordsCheck);
	DDX_Check(pDX, IDC_RODS_STAVES_WANDS_CHECK, m_bRodsStavesWandsCheck);
	DDX_Check(pDX, IDC_MISC_MAGIC_1_CHECK, m_bMiscMagic1Check);
	DDX_Check(pDX, IDC_MISC_MAGIC_2_CHECK, m_bMiscMagic2Check);
	DDX_Check(pDX, IDC_MISC_MAGIC_3_CHECK, m_bMiscMagic3Check);
	DDX_Check(pDX, IDC_MISC_MAGIC_4_CHECK, m_bMiscMagic4Check);
	DDX_Check(pDX, IDC_MISC_MAGIC_5_CHECK, m_bMiscMagic5Check);
	DDX_Check(pDX, IDC_MISC_MAGIC_SPECIAL_CHECK, m_bMiscMagicSpecialCheck);
	DDX_Check(pDX, IDC_XP_CHECK, m_bXPCheck);
	DDX_Check(pDX, IDC_SCROLLS_CHECK, m_bScrollsCheck);
	DDX_Check(pDX, IDC_CONTAINER_CONTENTS_CHECK, m_bShowContainerContents);
	DDX_Control(pDX, IDC_CACHE_DETAILS_EDIT, m_cCacheDetailsEdit);
	DDX_Check(pDX, IDC_ITEM_IDENTIFIED_CHECK, m_bItemIdentified);
	DDX_Control(pDX, IDC_ITEM_IDENTIFIED_CHECK, m_cItemIdentified);
	DDX_Text(pDX, IDC_INVENTORY_LABEL, m_szInventoryLabel);
	DDX_Control(pDX, IDC_TREASURE_TYPE_BUTTON, m_cTreasureTypeButton);
	DDX_Control(pDX, IDC_SPECIALIZED_CHECK, m_cSpecializedCheck);
	DDX_Control(pDX, IDC_DOUBLE_SPECIALIZED_CHECK, m_cDoubleSpecializedCheck);
	DDX_Check(pDX, IDC_DOUBLE_SPECIALIZED_CHECK, m_bDoubleSpecialized);
	DDX_Check(pDX, IDC_SPECIALIZED_CHECK, m_bSpecialized);
	DDX_Control(pDX, IDC_ITEM_INFO_BUTTON, m_cItemInfoButton);
	DDX_Control(pDX, IDC_GEN_TREASURE_BUTTON, m_cGenerateTreasureButton);
	DDX_Control(pDX, IDC_IN_LAIR_CHECK, m_cInLairCheck);
	DDX_Check(pDX, IDC_IN_LAIR_CHECK, m_bInLairCheck);
	DDX_Check(pDX, IDC_MAGIC_COMPONENTS_CHECK, m_bMaterialComponentCheck);
	DDX_Text(pDX, IDC_ITEM_DESC, m_szItemDesc);
	DDX_Control(pDX, IDC_ITEM_ENCUMBRANCE_CHECK, m_cIgnoreEncumbrance);
	DDX_Check(pDX, IDC_ITEM_ENCUMBRANCE_CHECK, m_bIgnoreEncumbrance);
}


BEGIN_MESSAGE_MAP(DMInventoryDialog, CDialog)
	//{{AFX_MSG_MAP(DMInventoryDialog)
	ON_BN_CLICKED(IDC_GIVE_BUTTON, OnGiveButton)
	ON_BN_CLICKED(IDC_BUY_BUTTON, OnBuyButton)
	ON_BN_CLICKED(IDC_GIVE_TO_BUTTON, OnGiveToButton)
	ON_BN_CLICKED(IDC_DROP_BUTTON, OnDropButton)
	ON_EN_CHANGE(IDC_GP_EDIT, OnChangeGpEdit)
	ON_EN_CHANGE(IDC_PP_EDIT, OnChangePpEdit)
	ON_EN_CHANGE(IDC_EP_EDIT, OnChangeEpEdit)
	ON_EN_CHANGE(IDC_SP_EDIT, OnChangeSpEdit)
	ON_EN_CHANGE(IDC_CP_EDIT, OnChangeCpEdit)
	ON_BN_CLICKED(IDC_STACK_COIN_BUTTON, OnStackCoinButton)
	ON_BN_CLICKED(ID_OK, OnOk)
	ON_LBN_SELCHANGE(IDC_INVENTORY_LIST, OnSelchangeInventoryList)
	ON_BN_CLICKED(IDC_RADIO_1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO_2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO_3, OnRadio3)
	ON_BN_CLICKED(IDC_RADIO_4, OnRadio4)
	ON_BN_CLICKED(IDC_RADIO_5, OnRadio5)
	ON_BN_CLICKED(IDC_DROP_ALL_BUTTON, OnDropAllButton)
	ON_BN_CLICKED(IDC_PRICES_CHECK, OnPricesCheck)
	ON_BN_CLICKED(IDC_WEAPONS_CHECK, OnWeaponsCheck)
	ON_BN_CLICKED(IDC_ARMOR_CHECK, OnArmorCheck)
	ON_BN_CLICKED(IDC_SHIELDS_CHECK, OnShieldsCheck)
	ON_BN_CLICKED(IDC_MISC_WEAPONS_CHECK, OnMiscWeaponsCheck)
	ON_BN_CLICKED(IDC_EQUIP_CHECK, OnEquipCheck)
	ON_BN_CLICKED(IDC_STACK_INV_BUTTON, OnStackInvButton)
	ON_BN_CLICKED(IDC_ADD_SUB_BUTTON, OnAddSubButton)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_ADD_CUSTOM_BUTTON, OnAddCustomButton)
	ON_BN_CLICKED(IDC_ADD_PROFF, OnAddProff)
	ON_BN_CLICKED(IDC_REMOVE_PROFF, OnRemoveProff)
	ON_WM_CLOSE()
	ON_LBN_SELCHANGE(IDC_STORE_LIST, OnSelchangeStoreList)
	ON_BN_CLICKED(IDC_RINGS_CHECK, OnRingsCheck)
	ON_EN_CHANGE(IDC_RENAME_ITEM_EDIT, OnChangeRenameItemEdit)
	ON_BN_CLICKED(IDC_WEIGHTS_CHECK, OnWeightsCheck)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_CACHE_NAME_EDIT, &DMInventoryDialog::OnEnChangeCacheNameEdit)
	ON_BN_CLICKED(IDC_MAGIC_ADJ_BUTTON, &DMInventoryDialog::OnBnClickedMagicAdjButton)
	ON_EN_CHANGE(IDC_CACHE_DETAILS_EDIT, &DMInventoryDialog::OnEnChangeCacheDetailsEdit)
	ON_EN_CHANGE(IDC_ITEM_XP_EDIT, &DMInventoryDialog::OnEnChangeItemXpEdit)
	ON_BN_CLICKED(IDC_GIVE_WITH_XP_CHARACTER_CHECK, &DMInventoryDialog::OnBnClickedGiveWithXpCharacterCheck)
	ON_BN_CLICKED(IDC_GIVE_WITH_XP_PARTY_CHECK, &DMInventoryDialog::OnBnClickedGiveWithXpPartyCheck)
	ON_BN_CLICKED(IDC_DROP_WITH_XP_CHARACTER_CHECK, &DMInventoryDialog::OnBnClickedDropWithXpCharacterCheck)
	ON_BN_CLICKED(IDC_DROP_WITH_XP_PARTY_CHECK, &DMInventoryDialog::OnBnClickedDropWithXpPartyCheck)
	ON_BN_CLICKED(IDC_POTIONS_CHECK, &DMInventoryDialog::OnBnClickedPotionsCheck)
	ON_BN_CLICKED(IDC_MAGICAL_ARMOR_CHECK, &DMInventoryDialog::OnBnClickedMagicalArmorCheck)
	ON_BN_CLICKED(IDC_MAGICAL_SWORDS_CHECK, &DMInventoryDialog::OnBnClickedMagicalSwordsCheck)
	ON_BN_CLICKED(IDC_RODS_STAVES_WANDS_CHECK, &DMInventoryDialog::OnBnClickedRodsStavesWandsCheck)
	ON_BN_CLICKED(IDC_MISC_MAGIC_1_CHECK, &DMInventoryDialog::OnBnClickedMiscMagic1Check)
	ON_BN_CLICKED(IDC_MISC_MAGIC_2_CHECK, &DMInventoryDialog::OnBnClickedMiscMagic2Check)
	ON_BN_CLICKED(IDC_MISC_MAGIC_3_CHECK, &DMInventoryDialog::OnBnClickedMiscMagic3Check)
	ON_BN_CLICKED(IDC_MISC_MAGIC_4_CHECK, &DMInventoryDialog::OnBnClickedMiscMagic4Check)
	ON_BN_CLICKED(IDC_MISC_MAGIC_5_CHECK, &DMInventoryDialog::OnBnClickedMiscMagic5Check)
	ON_BN_CLICKED(IDC_MISC_MAGIC_SPECIAL_CHECK, &DMInventoryDialog::OnBnClickedMiscMagicSpecialCheck)
	ON_BN_CLICKED(IDC_XP_CHECK, &DMInventoryDialog::OnBnClickedXpCheck)
	ON_BN_CLICKED(IDC_SCROLLS_CHECK, &DMInventoryDialog::OnBnClickedScrollsCheck)
	ON_BN_CLICKED(IDC_CONTAINER_CONTENTS_CHECK, &DMInventoryDialog::OnBnClickedContainerContentsCheck)
	ON_BN_CLICKED(IDC_ITEM_IDENTIFIED_CHECK, &DMInventoryDialog::OnBnClickedItemIdentifiedCheck)
	ON_BN_CLICKED(IDC_TREASURE_TYPE_BUTTON, &DMInventoryDialog::OnBnClickedTreasureTypeButton)
	ON_BN_CLICKED(IDC_SPECIALIZED_CHECK, &DMInventoryDialog::OnBnClickedSpecializedCheck)
	ON_BN_CLICKED(IDC_DOUBLE_SPECIALIZED_CHECK, &DMInventoryDialog::OnBnClickedDoubleSpecializedCheck)
	ON_LBN_SELCHANGE(IDC_PROFF_LIST, &DMInventoryDialog::OnLbnSelchangeProffList)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_ITEM_INFO_BUTTON, &DMInventoryDialog::OnBnClickedItemInfoButton)
	ON_LBN_DBLCLK(IDC_STORE_LIST, &DMInventoryDialog::OnLbnDblclkStoreList)
	ON_LBN_DBLCLK(IDC_INVENTORY_LIST, &DMInventoryDialog::OnLbnDblclkInventoryList)
	ON_BN_CLICKED(IDC_GEN_TREASURE_BUTTON, &DMInventoryDialog::OnBnClickedGenTreasureButton)
	ON_BN_CLICKED(IDC_IN_LAIR_CHECK, &DMInventoryDialog::OnBnClickedInLairCheck)
	ON_BN_CLICKED(IDC_MAGIC_COMPONENTS_CHECK, &DMInventoryDialog::OnBnClickedMagicComponentsCheck)
	ON_STN_DBLCLK(IDC_CACHE_NAME_STATIC, &DMInventoryDialog::OnStnDblclickCacheNameStatic)
	ON_BN_CLICKED(IDC_ITEM_ENCUMBRANCE_CHECK, &DMInventoryDialog::OnBnClickedItemEncumbranceCheck)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DMInventoryDialog message handlers


BOOL DMInventoryDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CFont CourierFont;

	CourierFont.CreatePointFont(90,"Courier New");
	LOGFONT lf; CourierFont.GetLogFont(&lf);
	lf.lfWeight = FW_BOLD;
	m_ListFont.CreateFontIndirect(&lf);

	m_cStoreList.SetFont(&m_ListFont);
	m_cCacheDetailsEdit.SetFont(&m_ListFont);

	int Tabstops[] = { 32, 64, 96, 128 };
	
	m_cStoreList.SetTabStops(4, Tabstops);
	m_cStoreList.SetHorizontalExtent(1000);

	m_cInventoryList.SetFont(&m_ListFont);
	m_cInventoryList.SetHorizontalExtent(1000);

	((CButton *)(GetDlgItem(IDC_GIVE_BUTTON)))->SetBitmap((::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_GIVE_BUTTON_BITMAP))));
	((CButton *)(GetDlgItem(IDC_BUY_BUTTON)))->SetBitmap((::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_BUY_BUTTON_BITMAP))));
	((CButton *)(GetDlgItem(IDC_GIVE_TO_BUTTON)))->SetBitmap((::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_GIVE_TO_BUTTON_BITMAP))));
	((CButton *)(GetDlgItem(IDC_DROP_BUTTON)))->SetBitmap((::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_DROP_BUTTON_BITMAP))));
	((CButton *)(GetDlgItem(IDC_DROP_ALL_BUTTON)))->SetBitmap((::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_DROP_ALL_BUTTON_BITMAP))));

	((CButton *)(GetDlgItem( IDC_WEAPONS_CHECK )))->SetCheck(1);
	((CButton *)(GetDlgItem( IDC_ARMOR_CHECK )))->SetCheck(1);
	((CButton *)(GetDlgItem( IDC_SHIELDS_CHECK )))->SetCheck(1);
	((CButton *)(GetDlgItem( IDC_EQUIP_CHECK )))->SetCheck(1);

	m_bWeaponsCheck = TRUE;
	m_bArmorCheck = TRUE;
	m_bShieldsCheck = TRUE;
	m_bEquipCheck = TRUE;
	m_bMaterialComponentCheck = FALSE;

	m_bRingsCheck = FALSE;
	m_bMiscWeaponsCheck = FALSE;

	m_cRenameEdit.EnableWindow(FALSE);
	m_cItemXPEdit.EnableWindow(FALSE);

	m_cGiveWithXPCharacterCheck.ShowWindow(SW_HIDE);
	m_cGiveWithXPPartyCheck.ShowWindow(SW_HIDE);

	m_cDropWithXPCharacterCheck.ShowWindow(SW_HIDE);
	m_cDropWithXPPartyCheck.ShowWindow(SW_HIDE);

	m_cItemInfoButton.EnableWindow(FALSE);

	RECT rect;
	m_cDetailsLocator.GetWindowRect(&rect);

	POINT p;
	p.x = rect.left;
	p.y = rect.top;

	ScreenToClient(&p);

	m_nDetailsPosX = p.x;
	m_nDetailsPosY = p.y;

	m_bMagicItemSelect = FALSE;

	if (m_pCharacter->m_bIsNPC == FALSE)
	{
		m_pPCDetailsDialog = new CDMPCDetailsDialog(this);
		m_cGenerateTreasureButton.ShowWindow(SW_HIDE);
		m_cInLairCheck.ShowWindow(SW_HIDE);	
	}

	if (m_pCharacter->m_bIsCache == TRUE)
	{
		m_cGenerateTreasureButton.ShowWindow(SW_HIDE);
		m_cInLairCheck.ShowWindow(SW_HIDE);
	}

	m_pCharacter->ValidateInventory();

	m_szInventoryLabel = _T("Carried Items:");

	InitDisplay();

	ShowWindow(SW_HIDE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void DMInventoryDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// Do not call CDialog::OnPaint() for painting messages

	if(m_pParent != NULL)
	{
		CRect rect;
		m_pParent->GetClientRect(&rect);

		rect.top+=20;

		SetWindowPos(NULL, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);
	}

	/*
	CRect rect;
	m_cDetailsLocator.GetWindowRect(&rect);

	m_nDetailsPosX = rect.left;
	m_nDetailsPosY = rect.top;
	*/

	if (m_dwInventoryFlag != m_pApp->m_dwInventoryFlag)
	{
		m_dwInventoryFlag = m_pApp->m_dwInventoryFlag;

		InitDisplay();
	}

}

void DMInventoryDialog::SelectMagicItems(BOOL bSelect)
{
	m_bMagicItemSelect = bSelect;

	if(m_bMagicItemSelect)
	{
		m_bWeaponsCheck = FALSE;
		m_bArmorCheck = FALSE;
		m_bShieldsCheck = FALSE;
		m_bEquipCheck = FALSE;
		m_bMaterialComponentCheck = FALSE;
	}
	else
	{
		m_bPotionsCheck = FALSE;
		m_bRingsCheck = FALSE;
		m_bScrollsCheck = FALSE;
		m_bRodsStavesWandsCheck = FALSE;
		m_bMiscMagic1Check = FALSE;
		m_bMiscMagic2Check = FALSE;
		m_bMiscMagic3Check = FALSE;
		m_bMiscMagic4Check = FALSE;
		m_bMiscMagic5Check = FALSE;
		m_bMiscMagicSpecialCheck = FALSE;
		m_bMagicalArmorCheck = FALSE;
		m_bMagicalSwordsCheck = FALSE;
		m_bMiscWeaponsCheck = FALSE;
		m_bMaterialComponentCheck = FALSE;
	}

}

void DMInventoryDialog::InitDisplay()
{
	CString szTemp;

	m_cStoreList.ResetContent();

	m_cItemInfoButton.EnableWindow(FALSE);

	//CString szTabs;
	//szTabs.Format("%c%c%c%c%c",9,9,9,9,9);
	//                                                                                      *                                                                                            
	CString szSpaces = "                                                                    ";

	int nCount1 = m_pApp->m_ObjectsOrderedTypeArray.GetSize();
	int nCount2 = m_pApp->m_ObjectsIndexedTypeArray.GetSize();

	if(m_pCharacter->m_bIsNPC)
	{
		(GetDlgItem( IDC_PROFF_LIST ))-> ShowWindow(SW_HIDE);
		(GetDlgItem( IDC_WEP_PROF_STATIC ))-> ShowWindow(SW_HIDE);
		(GetDlgItem( IDC_NON_WEP_PROF_STATIC ))-> ShowWindow(SW_HIDE);
		(GetDlgItem( IDC_ADD_PROFF ))-> ShowWindow(SW_HIDE);
		(GetDlgItem( IDC_REMOVE_PROFF ))-> ShowWindow(SW_HIDE);

		if(m_pCharacter->m_bIsCache)
		{
			m_cInventoryStatic.SetWindowText("Cache Inventory");
			m_cWPFrame.ShowWindow(SW_HIDE);
			m_cCachePropertiesFrame.ShowWindow(SW_SHOW);

			(GetDlgItem( IDC_BUY_BUTTON ))-> ShowWindow(SW_HIDE);
			(GetDlgItem( ID_OK ))-> ShowWindow(SW_HIDE);
			(GetDlgItem( IDC_MAX_ENC_STATIC ))-> ShowWindow(SW_HIDE);

			m_szCacheName.Format("%s", m_pCharacter->m_szCharacterName);
			m_szCacheDetails.Format("%s", m_pCharacter->m_szDetails);

			m_cDropWithXPCharacterCheck.ShowWindow(SW_HIDE);
			m_cDropWithXPPartyCheck.ShowWindow(SW_HIDE);
		}
		else
		{
			m_cInventoryStatic.SetWindowText("NPC Inventory");
			//(GetDlgItem( IDC_WP_STATIC ))-> ShowWindow(SW_HIDE);
			m_cWPFrame.SetWindowText("NPC Details");
		}
	}

	if(m_pCharacter->m_bIsCache == FALSE)
	{
		m_szInventoryLabel = _T("Carried Items:");

		(GetDlgItem( IDC_CACHE_NAME_EDIT ))->ShowWindow(SW_HIDE);
		if(!m_pCharacter->m_bIsNPC)
		{
			(GetDlgItem( IDC_CACHE_NAME_STATIC ))->ShowWindow(SW_HIDE); //also monster treasure type
		}
		(GetDlgItem( IDC_CACHE_DETAILS__STATIC ))->ShowWindow(SW_HIDE);
		(GetDlgItem( IDC_CACHE_DETAILS_EDIT ))->ShowWindow(SW_HIDE);
		
		m_cCachePropertiesFrame.ShowWindow(SW_HIDE);
		m_cTreasureTypeButton.ShowWindow(SW_HIDE);
	}
	else
	{
		m_szInventoryLabel = _T("Stored Items:");
		m_cTreasureTypeButton.ShowWindow(SW_SHOW);
	}

	// mundane items
	int nIndex = 0;
	for(int i = 0; i < m_pApp->m_ObjectsOrderedTypeArray.GetSize(); ++i)
	{
		POBJECTTYPE pObjBase = m_pApp->m_ObjectsOrderedTypeArray.GetAt(i);

		if(pObjBase != NULL)
		{
			if(pObjBase->IsMagicBag())
			{
				continue;
			}

			if(pObjBase->m_wTypeId == 12100)
			{
				TRACE("HOOHA!\n");
			}

			POBJECTTYPE pObj = m_pApp->m_ObjectsIndexedTypeArray.GetAt(pObjBase->m_wTypeId);

			ASSERT(pObj->m_wTypeId == pObjBase->m_wTypeId);

			if(pObj != NULL)
			{
				if(!m_bWeaponsCheck && (pObj->m_ObjectType == DND_OBJECT_TYPE_WEAPON || pObj->m_ObjectType == DND_OBJECT_TYPE_AMMO))
					continue;

				if(!m_bShieldsCheck && pObj->m_ObjectType == DND_OBJECT_TYPE_SHIELD)
					continue;

				if(!m_bArmorCheck && (pObj->m_ObjectType == DND_OBJECT_TYPE_ARMOR || pObj->m_ObjectType == DND_OBJECT_TYPE_HELMET))
					continue;

				if(!m_bMaterialComponentCheck && pObj->m_ObjectType == DND_OBJECT_TYPE_MATERIAL_COMPONENT)
					continue;


				if(!m_bEquipCheck && (pObj->m_ObjectType == DND_OBJECT_TYPE_EQUIPMENT || pObj->m_ObjectType == DND_OBJECT_TYPE_RING))
					continue;

				if(m_bEquipCheck && pObj->m_ObjectType == DND_OBJECT_TYPE_RING && pObj->m_nFlags % 10 == 1) // non-magical rings only
					continue;

				if(pObj->m_ObjectType == DND_OBJECT_TYPE_WEAPON &&  ((pObj->m_nFlags / 10) % 10 == 2) ||   ((pObj->m_nFlags / 10) % 10 == 3) ) // monk open hand, pummel attack
					continue;

				m_bXPCheck = FALSE;
	
				if(m_bPricesCheck || m_bWeightsCheck)
				{
					CString szLine;
					szTemp.Format("%s", pObj->m_szType); 

					szLine = szTemp.Left(COLUMN_SIZE);
					
					CString szPrice = _T("");

					if(m_bPricesCheck)
					{
						if(pObj->m_nCost)
						{
							szPrice.Format("%s", GetStringFromPrice(pObj->m_nCost));
						}
					}
					else
					{
						if(szLine.Find("(") == -1)
						{
							szPrice.Format("%d g.p.w.", pObj->m_nWeight);
						}
					}

					int nOrgLen = szLine.GetLength();
					int nLen = COLUMN_SIZE - szLine.GetLength();
					szLine += szSpaces.Left(nLen);
					
					szTemp.Format("%s%c%s",szLine.GetBuffer(0), 9, szPrice.GetBuffer(0)); //, nOrgLen);
				}
				else
				{
					szTemp.Format("%s",pObj->m_szType);
				}

				m_cStoreList.InsertString(nIndex, szTemp);
				m_cStoreList.SetItemDataPtr(nIndex, pObj);

				++nIndex;
			}
		}
	}

	// magical items
	if(nIndex == 0)
	{
		for(int i = 0; i < m_pApp->m_MagicTableItemOrderedTypeArray.GetSize(); ++i)
		{
			PMAGICTABLEITEM pMagicItem = m_pApp->m_MagicTableItemOrderedTypeArray.GetAt(i);

			if(pMagicItem != NULL)
			{
				if(pMagicItem->m_nAmountLow == 0)
					continue;

				if(!m_bPotionsCheck && pMagicItem->m_nTable == DND_MAGIC_ITEM_TABLE_POTIONS)
					continue;

				if(!m_bRodsStavesWandsCheck && pMagicItem->m_nTable == DND_MAGIC_ITEM_TABLE_RODS_STAVES_WANDS)
					continue;

				if(!m_bMiscMagic1Check && pMagicItem->m_nTable == DND_MAGIC_ITEM_TABLE_MISC_MAGIC_1)
					continue;

				if(!m_bMiscMagic2Check && pMagicItem->m_nTable == DND_MAGIC_ITEM_TABLE_MISC_MAGIC_2)
					continue;

				if(!m_bMiscMagic3Check && pMagicItem->m_nTable == DND_MAGIC_ITEM_TABLE_MISC_MAGIC_3)
					continue;

				if(!m_bMiscMagic4Check && pMagicItem->m_nTable == DND_MAGIC_ITEM_TABLE_MISC_MAGIC_4)
					continue;

				if(!m_bMiscMagic5Check && pMagicItem->m_nTable == DND_MAGIC_ITEM_TABLE_MISC_MAGIC_5)
					continue;
				
				if(!m_bMiscMagicSpecialCheck && pMagicItem->m_nTable == DND_MAGIC_ITEM_TABLE_MISC_MAGIC_SPECIAL)
					continue;

				if(!m_bScrollsCheck && pMagicItem->m_nTable == DND_MAGIC_ITEM_TABLE_SCROLLS)
					continue;

				if(!m_bRingsCheck && pMagicItem->m_nTable == DND_MAGIC_ITEM_TABLE_RINGS)
					continue;

				if(!m_bMagicalArmorCheck && pMagicItem->m_nTable == DND_MAGIC_ITEM_TABLE_ARMOR)
					continue;

				if(!m_bMagicalSwordsCheck && pMagicItem->m_nTable == DND_MAGIC_ITEM_TABLE_SWORDS)
					continue;
			
				if(!m_bMiscWeaponsCheck && pMagicItem->m_nTable == DND_MAGIC_ITEM_TABLE_MISC_WEAPONS)
					continue;

				if(m_bPricesCheck || m_bWeightsCheck || m_bXPCheck)
				{
					CString szLine;
					szTemp.Format("%s", pMagicItem->m_szDesc); 

					szLine = szTemp.Left(COLUMN_SIZE);
					
					CString szPrice = _T("");

					if(m_bPricesCheck)
					{
						if(pMagicItem->m_lGPValue)
						{
							szPrice.Format("%s", GetStringFromPrice(pMagicItem->m_lGPValue * 200L));
						}
					}
					else if(m_bWeightsCheck)
					{
						//if(szLine.Find("(") == -1) // this is ok in magic items list
						{
							szPrice.Format("%d g.p.w.", pMagicItem->m_pMundaneObject[0]->m_nWeight);
						}
					}
					else if(m_bXPCheck)
					{
						szPrice.Format("%ld x.p.", pMagicItem->m_lXPValue);
					}

					int nOrgLen = szLine.GetLength();
					int nLen = COLUMN_SIZE - szLine.GetLength();
					szLine += szSpaces.Left(nLen);
					
					szTemp.Format("%s%c%s",szLine.GetBuffer(0), 9, szPrice.GetBuffer(0)); //, nOrgLen);
				}
				else
				{
					szTemp.Format("%s",pMagicItem->m_szDesc);
				}

				m_cStoreList.InsertString(nIndex, szTemp);
				m_cStoreList.SetItemDataPtr(nIndex, pMagicItem);

				++nIndex;
			}
		}
	}

	if(m_pPCDetailsDialog != NULL)
	{
		m_pPCDetailsDialog->Init();
	}

	Refresh();
}

void DMInventoryDialog::RefreshAll()
{
	Refresh();

	if(m_pPCDetailsDialog != NULL)
	{
		m_pPCDetailsDialog->Refresh();
	}

	if(m_pScrollDetailsDialog != NULL)
	{
		m_pScrollDetailsDialog->Refresh();
	}

	if(m_pContainerDetailsDialog != NULL && m_pSelectedInventorySlot != NULL && m_pSelectedInventorySlot->IsContainer())
	{
		m_pContainerDetailsDialog->Refresh(m_pSelectedInventorySlot->m_dwObjectID);
	}
}

void DMInventoryDialog::AddContainerItemToList(DWORD dwContainerID, int *pnTotalItems, int nLevel) // this function can go recursive
{
	CString szTemp;

	#if _DEBUG_CONTS
	CString szConts;
	#endif

	if(nLevel > MAX_CHARACTER_INVENTORY)
	{
		return;
	}

	CString szTabs = _T("");

	for(int t=0; t < nLevel; ++t)
	{
		szTabs += _T("  ");
	}

	for(int i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
	{
		POBJECTTYPE pObj = &m_pCharacter->m_Inventory[i];

		if(pObj != NULL && pObj->m_wTypeId && pObj->m_dwContainerID == dwContainerID)
		{
			if(pObj->m_nMagicAdj && !pObj->m_bEnchanted)
			{
				if(pObj->m_nMagicAdj > 0)
					szTemp.Format("%s%s +%d", szTabs, pObj->m_szType, pObj->m_nMagicAdj);
				else
					szTemp.Format("%s%s %d", szTabs, pObj->m_szType, pObj->m_nMagicAdj);
			}
			else
			{
				szTemp.Format("%s%s", szTabs, pObj->m_szType);
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

			#if _DEBUG_CONTS
			szConts.Format(" [%ld %ld]", pObj->m_dwObjectID, pObj->m_dwContainerID);
			szFinal += szConts;
			#endif

			CString szCoins = _T("");
			if(pObj->IsCoinContainer())
			{
				szCoins += _T(" [");

				BOOL bFirstCoin = TRUE;

				CString szCoin = _T("");
				if(pObj->m_nContainerPlatinum)
				{
					if(!bFirstCoin) {szCoins += _T(", ");} bFirstCoin = FALSE;
					szCoin.Format("%ld p.p.", pObj->m_nContainerPlatinum);
					szCoins += szCoin;
				}
				if(pObj->m_nContainerGold)
				{
					if(!bFirstCoin) {szCoins += _T(", ");} bFirstCoin = FALSE;
					szCoin.Format("%ld g.p.", pObj->m_nContainerGold);
					szCoins += szCoin;
				}
				if(pObj->m_nContainerElectrum)
				{
					if(!bFirstCoin) {szCoins += _T(", ");} bFirstCoin = FALSE;
					szCoin.Format("%ld e.p.", pObj->m_nContainerElectrum);
					szCoins += szCoin;
				}
				if(pObj->m_nContainerSilver)
				{
					if(!bFirstCoin) {szCoins += _T(", ");} bFirstCoin = FALSE;
					szCoin.Format("%ld s.p.", pObj->m_nContainerSilver);
					szCoins += szCoin;
				}
				if(pObj->m_nContainerCopper)
				{
					if(!bFirstCoin) {szCoins += _T(", ");} bFirstCoin = FALSE;
					szCoin.Format("%ld c.p.", pObj->m_nContainerCopper);
					szCoins += szCoin;
				}

				szCoins += _T("]");

				szFinal += szCoins;
			}

			if(pObj->m_nItemValue)
			{
				CString szValue;
				szValue.Format(" [%d g.p.]", pObj->m_nItemValue);
				szFinal += szValue;
			}

			m_cInventoryList.InsertString(*pnTotalItems, szFinal);
			m_cInventoryList.SetItemDataPtr(*pnTotalItems, pObj);

			if(pObj == m_pSelectedInventorySlot)
			{
				m_cInventoryList.SetCurSel(*pnTotalItems);
				m_cItemIdentified.ShowWindow(SW_SHOW);
				m_cIgnoreEncumbrance.ShowWindow(SW_SHOW);
			}

			++*pnTotalItems;

			if(pObj->IsContainer())
			{
				AddContainerItemToList(pObj->m_dwObjectID, pnTotalItems, nLevel+1);
			}
		}
	}
}

void DMInventoryDialog::CleanUp()
{
	if(m_pPCDetailsDialog != NULL)
	{
		if(m_pPCDetailsDialog->m_hWnd != NULL)
		{
			if(m_bParentClosing == FALSE)
			{
				m_pPCDetailsDialog->Refresh(FALSE, TRUE);
			}
			m_pPCDetailsDialog->PostMessage(WM_CLOSE, 0, 0);
		}
		delete m_pPCDetailsDialog;
		m_pPCDetailsDialog = NULL;
	}

	
	if(m_pScrollDetailsDialog != NULL)
	{
		if(m_pScrollDetailsDialog->m_hWnd != NULL)
		{
			m_pScrollDetailsDialog->PostMessage(WM_CLOSE, 0, 0);
		}
		delete m_pScrollDetailsDialog;
		m_pScrollDetailsDialog = NULL;
	}

	if(m_pContainerDetailsDialog != NULL)
	{
		if(m_pContainerDetailsDialog->m_hWnd != NULL)
		{
			m_pContainerDetailsDialog->PostMessage(WM_CLOSE, 0, 0);
		}
		delete m_pContainerDetailsDialog;
		m_pContainerDetailsDialog = NULL;
	}

	if(m_pMagicSwordPropertiesDialog != NULL)
	{
		if(m_pMagicSwordPropertiesDialog->m_hWnd != NULL)
		{
			m_pMagicSwordPropertiesDialog->PostMessage(WM_CLOSE, 0, 0);
		}
		delete m_pMagicSwordPropertiesDialog;
		m_pMagicSwordPropertiesDialog = NULL;
	}

}

void DMInventoryDialog::Refresh()
{

	if (m_pCharacter->m_bIsCache && m_pNPCVWindow != NULL)
	{
		if (m_pNPCVWindow->m_pCacheInventory != NULL)
		{
			//sync here
			m_pNPCVWindow->m_pCacheInventory->m_lCopperCarried = m_pCharacter->m_lCopperCarried;
			m_pNPCVWindow->m_pCacheInventory->m_lSilverCarried = m_pCharacter->m_lSilverCarried;
			m_pNPCVWindow->m_pCacheInventory->m_lElectrumCarried = m_pCharacter->m_lElectrumCarried;
			m_pNPCVWindow->m_pCacheInventory->m_lGoldCarried = m_pCharacter->m_lGoldCarried;
			m_pNPCVWindow->m_pCacheInventory->m_lPlatinumCarried = m_pCharacter->m_lPlatinumCarried;

			m_pNPCVWindow->m_pCacheInventory->m_fMapLocationX = m_pCharacter->m_fMapLocationX;
			m_pNPCVWindow->m_pCacheInventory->m_fMapLocationY = m_pCharacter->m_fMapLocationY;

			m_pNPCVWindow->m_pCacheInventory->m_fLocalLocationX = m_pCharacter->m_fLocalLocationX;
			m_pNPCVWindow->m_pCacheInventory->m_fLocalLocationY = m_pCharacter->m_fLocalLocationY;

			m_pNPCVWindow->m_pCacheInventory->m_lSaveFlag = m_pCharacter->m_lSaveFlag;

			memcpy(m_pNPCVWindow->m_pCacheInventory->m_szCacheDesc, m_pCharacter->m_szCharacterName, 128 * sizeof(char));
			memcpy(m_pNPCVWindow->m_pCacheInventory->m_szCacheDetails, m_pCharacter->m_szDetails, 512 * sizeof(char));
			memcpy(m_pNPCVWindow->m_pCacheInventory->m_Inventory, m_pCharacter->m_Inventory, MAX_CHARACTER_INVENTORY * sizeof(cDNDObject));
		}
	}

	m_szCopperEdit.Format("%ld", m_pCharacter->m_lCopperCarried);
	m_szSilverEdit.Format("%ld", m_pCharacter->m_lSilverCarried);
	m_szElectrumEdit.Format("%ld", m_pCharacter->m_lElectrumCarried);
	m_szGoldEdit.Format("%ld", m_pCharacter->m_lGoldCarried);
	m_szPlatinumEdit.Format("%ld", m_pCharacter->m_lPlatinumCarried);

	CString szTemp;

	#if _DEBUG_CONTS
	CString szConts;
	#endif

	m_cSpecializedCheck.ShowWindow(SW_HIDE);
	m_cDoubleSpecializedCheck.ShowWindow(SW_HIDE);

	m_cInventoryList.ResetContent();
	m_cItemIdentified.ShowWindow(SW_HIDE);
	m_cIgnoreEncumbrance.ShowWindow(SW_HIDE);

	int nTotalItems = 0;

	for(int i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
	{
		POBJECTTYPE pObj = &m_pCharacter->m_Inventory[i];

		if(pObj != NULL && pObj->m_wTypeId && pObj->m_dwContainerID == 0)
		{
			if(pObj->m_nMagicAdj && !pObj->m_bEnchanted)
			{
				if(pObj->m_nMagicAdj > 0)
					szTemp.Format("%s +%d", pObj->m_szType, pObj->m_nMagicAdj);
				else
					szTemp.Format("%s %d", pObj->m_szType, pObj->m_nMagicAdj);
			}
			else
			{
				szTemp.Format("%s", pObj->GetExtendedName());
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

			#if _DEBUG_CONTS
			szConts.Format(" [%ld %ld]", pObj->m_dwObjectID, pObj->m_dwContainerID);
			szFinal += szConts;
			#endif

			CString szCoins = _T("");
			if(pObj->IsCoinContainer())
			{
				szCoins += _T(" [");

				BOOL bFirstCoin = TRUE;
				CString szCoin = _T("");
				if(pObj->m_nContainerPlatinum)
				{
					if(!bFirstCoin) {szCoins += _T(", ");} bFirstCoin = FALSE;
					szCoin.Format("%ld p.p.", pObj->m_nContainerPlatinum);
					szCoins += szCoin;
				}
				if(pObj->m_nContainerGold)
				{
					if(!bFirstCoin) {szCoins += _T(", ");} bFirstCoin = FALSE;
					szCoin.Format("%ld g.p.", pObj->m_nContainerGold);
					szCoins += szCoin;
				}
				if(pObj->m_nContainerElectrum)
				{
					if(!bFirstCoin) {szCoins += _T(", ");} bFirstCoin = FALSE;
					szCoin.Format("%ld e.p.", pObj->m_nContainerElectrum);
					szCoins += szCoin;
				}
				if(pObj->m_nContainerSilver)
				{
					if(!bFirstCoin) {szCoins += _T(", ");} bFirstCoin = FALSE;
					szCoin.Format("%ld s.p.", pObj->m_nContainerSilver);
					szCoins += szCoin;
				}
				if(pObj->m_nContainerCopper)
				{
					if(!bFirstCoin) {szCoins += _T(", ");} bFirstCoin = FALSE;
					szCoin.Format("%ld c.p.", pObj->m_nContainerCopper);
					szCoins += szCoin;
				}

				szCoins += _T("]");

				szFinal += szCoins;
			} // if(pObj->IsCoinContainer())

			if(pObj->m_nItemValue)
			{
				CString szValue;
				szValue.Format(" [%d g.p.]", pObj->m_nItemValue);
				szFinal += szValue;
			}

			m_cInventoryList.InsertString(nTotalItems, szFinal);
			m_cInventoryList.SetItemDataPtr(nTotalItems, pObj);

			if(pObj == m_pSelectedInventorySlot)
			{
				m_cInventoryList.SetCurSel(nTotalItems);
				m_cItemIdentified.ShowWindow(SW_SHOW);
				m_cIgnoreEncumbrance.ShowWindow(SW_SHOW);
			}

			++nTotalItems;

			if(pObj->IsContainer())
			{
				AddContainerItemToList(pObj->m_dwObjectID, &nTotalItems, 1);
			}
		}
	}

	m_szTotalInvItems.Format("Items: %d", nTotalItems);

	int nMaxWeight = 0;
	int nEncumbrance = CalculateEncumbrance(m_pCharacter, &nMaxWeight);

	m_szEncumbrance.Format("%d g.p.", nEncumbrance);

	m_szMaxEncumbrance.Format("of %d/%d/%d/%d", nMaxWeight, nMaxWeight+350, nMaxWeight+700, nMaxWeight+1050, nMaxWeight+1400);

	int nSpecializationLevel = m_pCharacter->GetWeaponSpecializationLevel();

	int nTotalProfs = 0;

	int nCurrProff = m_cProffList.GetCurSel();

	if(nSpecializationLevel >= 1 && nCurrProff >= 0)
	{
		m_cSpecializedCheck.ShowWindow(SW_SHOW);

		if(m_pCharacter->m_WeaponProficiencies[nCurrProff].m_nContents[0])
		{
			m_bSpecialized = TRUE;

			if(IsMissileWeapon(&m_pCharacter->m_WeaponProficiencies[nCurrProff]) || m_pCharacter->m_WeaponProficiencies[nCurrProff].m_wTypeId == 1154 || IsPoleArm(&m_pCharacter->m_WeaponProficiencies[nCurrProff]) )
			{
				m_pCharacter->m_WeaponProficiencies[nCurrProff].m_nContents[0] = 1;
				m_bDoubleSpecialized = FALSE;
			}
			else
			{
				m_cDoubleSpecializedCheck.ShowWindow(SW_SHOW);

				if(m_pCharacter->m_WeaponProficiencies[nCurrProff].m_nContents[0] > 1)
				{
					m_bDoubleSpecialized = TRUE;
				}
				else
				{
					m_bDoubleSpecialized = FALSE;
				}
			}
		}
		else
		{
			m_bSpecialized = FALSE;
		}
	}

	m_cProffList.ResetContent();

	for(i = 0; i < MAX_WEAPON_PROFICIENCIES; ++i)
	{
		POBJECTTYPE pObj = &m_pCharacter->m_WeaponProficiencies[i];

		if(pObj != NULL && pObj->m_wTypeId)
		{
			m_cProffList.InsertString(i, m_pCharacter->m_WeaponProficiencies[i].m_szType);
			m_cProffList.SetItemDataPtr(i, pObj);
			++nTotalProfs;

			if(nSpecializationLevel)
			{
				nTotalProfs += m_pCharacter->m_WeaponProficiencies[i].m_nContents[0];

				if(m_pCharacter->m_WeaponProficiencies[i].m_nContents[0])
				{
					//bows other than light crossbow requires another profficiency slot
					if(IsMissileWeapon(&m_pCharacter->m_WeaponProficiencies[i]) && m_pCharacter->m_WeaponProficiencies[i].m_wTypeId != 1116)
					{
						++nTotalProfs;
					}
				}
			}
		}
	}

	int nProfPenalty = 0;
	int nNumProfs = CalculateWeaponProficiencies(m_pCharacter, &nProfPenalty);

	m_szProffStatic.Format("Weapon Proficiencies : %d/%d", nTotalProfs, nNumProfs);
	m_szNonProffStatic.Format("Non-proficiency penalty: %d", nProfPenalty);

	m_cProffList.SetCurSel(nCurrProff);

	m_cGiveToCombo.ResetContent();

	int nRow = 0;
	WORD wID;
	for (POSITION pos = m_pApp->m_CharacterViewMap.GetStartPosition(); pos != NULL; )
	{
		PDNDCHARVIEWDLG pCharDlg = NULL;
		m_pApp->m_CharacterViewMap.GetNextAssoc(pos,wID,pCharDlg);

		if(pCharDlg != NULL && pCharDlg->m_pCharacter != NULL && pCharDlg->m_pCharacter->m_dwCharacterID)
		{
			if(pCharDlg->m_pCharacter == m_pCharacter)
				continue;

			m_cGiveToCombo.InsertString(nRow, pCharDlg->m_pCharacter->m_szCharacterName);
			m_cGiveToCombo.SetItemData(nRow, (ULONG)pCharDlg->m_pCharacter);

			if(m_pGiftCharacter == pCharDlg->m_pCharacter)
			{
				m_cGiveToCombo.SetCurSel(nRow);
			}

			++nRow;
		}
	}

	for (pos = m_pApp->m_NPCViewMap.GetStartPosition(); pos != NULL; )
	{
		PDNDNPCVIEWDLG pNPCDlg = NULL;
		m_pApp->m_NPCViewMap.GetNextAssoc(pos,wID,pNPCDlg);

		if(pNPCDlg != NULL && pNPCDlg->m_pNPC != NULL && pNPCDlg->m_pNPC->m_dwCharacterID)
		{
			if(pNPCDlg->m_pNPC == m_pCharacter)
				continue;

			m_cGiveToCombo.InsertString(nRow, pNPCDlg->m_pNPC->m_szCharacterName);
			m_cGiveToCombo.SetItemData(nRow, (ULONG)pNPCDlg->m_pNPC);

			if(m_pGiftCharacter == pNPCDlg->m_pNPC)
			{
				m_cGiveToCombo.SetCurSel(nRow);
			}

			++nRow;
		}
	}

	

	UpdateData(FALSE);

	if(m_pPCVWindow != NULL)
	{
		m_pPCVWindow->Refresh();
	}
	if(m_pNPCVWindow != NULL)
	{
		PDNDMONSTERMANUALENTRY pMonster = NULL;
	
		m_pApp->m_MonsterManualIndexedMap.Lookup(m_pNPCVWindow->m_pNPC->m_nMonsterIndex, pMonster);

		if(pMonster != NULL && m_pNPCVWindow->m_pNPC->m_bIsCache == FALSE)
		{
			m_szCacheNameDesc.Format("Treasure Types: %s", pMonster->m_szTreasureType);
			(GetDlgItem( IDC_CACHE_NAME_STATIC ))->ShowWindow(SW_SHOW); //also monster treasure type
		}

		m_pNPCVWindow->Refresh();
	}
}


void DMInventoryDialog::MarkCacheChanged(cDNDCharacter *pCacheCharacter)
{
	if(m_pNPCVWindow != NULL)
	{
		if(m_pNPCVWindow->m_pNPC->m_bIsCache)
		{
			if(m_pNPCVWindow->m_pCacheInventory != NULL)
			{
				m_pNPCVWindow->m_pCacheInventory->MarkChanged();
			}
		}
	}
	
	if(pCacheCharacter != NULL && pCacheCharacter->m_bIsCache)
	{
		PDNDNPCVIEWDLG pNPCDlg = NULL;
		m_pApp->m_NPCViewMap.Lookup((WORD)pCacheCharacter->m_dwCharacterID, pNPCDlg);

		if(pNPCDlg != NULL && pNPCDlg->m_pNPC != NULL && pNPCDlg->m_pNPC->m_bIsCache && pNPCDlg->m_pCacheInventory != NULL)
		{
			pNPCDlg->m_pNPC->MarkChanged();
			pNPCDlg->m_pCacheInventory->MarkChanged();
			//pNPCDlg->Refresh();
		}
	}
}


void DMInventoryDialog::OnOK() 
{
	//CDialog::OnOK();  //Don't want return
}

void DMInventoryDialog::OnOk() 
{
	if(m_pPCDetailsDialog != NULL)
	{
		m_pPCDetailsDialog->Refresh(FALSE);
	}

	if(m_pPCVWindow != NULL)
	{
		m_pPCVWindow->m_pInventoryDialog = NULL;

		m_pPCVWindow->m_nPage = 0;
		ShowWindow(SW_HIDE);
		m_pPCVWindow->ShowWindow(SW_SHOW);

		m_pPCVWindow->InvalidateRect(NULL);
		m_pPCVWindow->ProcessCharStats();
		m_pPCVWindow->Refresh();
	}

	if(m_pNPCVWindow != NULL)
	{
		m_pNPCVWindow->m_pInventoryDialog = NULL;

		m_pNPCVWindow->m_nPage = 0;
		ShowWindow(SW_HIDE);
		m_pNPCVWindow->ShowWindow(SW_SHOW);

		m_pNPCVWindow->InvalidateRect(NULL);
		m_pNPCVWindow->ProcessCharStats();
		m_pNPCVWindow->Refresh();
	}

	PostMessage(WM_CLOSE);
}


void DMInventoryDialog::OnCancel() 
{
	CDialog::OnCancel();
}

void DMInventoryDialog::CheckItemSelect()
{
	UpdateData(TRUE);

	m_cItemInfoButton.EnableWindow(FALSE);

	m_szItemDesc = _T("");

	CString szItemName = _T("");

	int nCursor = m_cStoreList.GetCurSel();

	if (nCursor >= 0)
	{
		if (m_bMagicItemSelect)
		{
			PMAGICTABLEITEM pMagicItem = (PMAGICTABLEITEM)m_cStoreList.GetItemData(nCursor);

			szItemName.Format("%s", pMagicItem->m_szDesc);
			m_szItemDesc.Format("DESC: %s", pMagicItem->m_szDesc);
		}
		else
		{
			POBJECTTYPE pObj = (POBJECTTYPE)m_cStoreList.GetItemData(nCursor);

			if (pObj->m_szExtendedName[0])
			{
				m_szItemDesc.Format("DESC: %s", pObj->m_szExtendedName);
			}
		}

		if (szItemName != _T(""))
		{
			cDNDMagicItemDescription *pMagicItemDescription = NULL;

			//szItemName = "potion of animal control";
			//int nLen = szItemName.GetLength();

			BOOL bFound = m_pApp->m_MagicItemDescriptionMap.Lookup(szItemName, pMagicItemDescription);

			if (bFound && pMagicItemDescription != NULL)
			{
				m_cItemInfoButton.EnableWindow(TRUE);
			}
		}

	}
}

void DMInventoryDialog::OnGiveButton() 
{
	UpdateData(TRUE);

	int nCursor = m_cStoreList.GetCurSel();

	if(nCursor >= 0)
	{
		POBJECTTYPE pObj = NULL;

		if(m_bMagicItemSelect)
		{
			PMAGICTABLEITEM pMagicItem = (PMAGICTABLEITEM)m_cStoreList.GetItemData(nCursor);

			int nMundaneItem = 0;
			int nMagicAdjustment = 0;
			int nAmount = 0;

			if(pMagicItem->m_nMundanes > 1)
			{
				cDMMundaneTypeSelectorDialog *pDlg = new cDMMundaneTypeSelectorDialog(pMagicItem, &nMundaneItem, NULL, NULL);
				pDlg->DoModal();
				delete pDlg;
			}
			if(pMagicItem->m_nMagicAdjustmentHigh > pMagicItem->m_nMagicAdjustmentLow)
			{
				cDMMundaneTypeSelectorDialog *pDlg = new cDMMundaneTypeSelectorDialog(pMagicItem, NULL, &nMagicAdjustment, NULL);
				pDlg->DoModal();
				delete pDlg;
			}
			if(pMagicItem->m_nAmountHigh > pMagicItem->m_nAmountLow)
			{
				cDMMundaneTypeSelectorDialog *pDlg = new cDMMundaneTypeSelectorDialog(pMagicItem, NULL, NULL, &nAmount);
				pDlg->DoModal();
				delete pDlg;
			}

			pObj = pMagicItem->EnchantItem(nMundaneItem, nMagicAdjustment, nAmount);
		}
		else
		{
			pObj = (POBJECTTYPE)m_cStoreList.GetItemData(nCursor);
			pObj = pObj->Clone();
		}

		if(pObj != NULL)
		{
			m_pCharacter->AddToInventory(pObj);

			m_pCharacter->MarkChanged();

			MarkCacheChanged(NULL);

			RefreshAll();

			delete pObj;
		}
	}
	
}

void DMInventoryDialog::OnBuyButton() 
{
	UpdateData(TRUE);

	int nCursor = m_cStoreList.GetCurSel();

	TRACE("PICKED %d\n", nCursor);

	if(nCursor >= 0)
	{
		POBJECTTYPE pObj = (POBJECTTYPE)m_cStoreList.GetItemData(nCursor);

		if(m_pCharacter->BuyItem(pObj->m_nCost))
		{
			m_pCharacter->AddToInventory(pObj);

			m_pCharacter->MarkChanged();

			MarkCacheChanged(NULL);

			RefreshAll();

			m_pApp->PlaySoundFX("Purchase");
		}
	}
	
}

void DMInventoryDialog::OnGiveToButton() 
{
	UpdateData(TRUE);

	CString szLine;
	szLine.Format("OUTPUT : %c %c %c\n", 175, 175, 175);
	TRACE(szLine);

	int nCursor = m_cGiveToCombo.GetCurSel();

	if(nCursor > -1)
	{
		m_pGiftCharacter = (cDNDCharacter *)m_cGiveToCombo.GetItemData(nCursor);

		if(m_pGiftCharacter == NULL)
			return;

		m_pCharacter->MarkChanged();
		m_pGiftCharacter->MarkChanged();

		BOOL bNeedAmount = FALSE;
		LONG lAmount = 0;

		LONG *_SourceLong = NULL;
		LONG *_DestLong = NULL;
		LONG lSourceAvailable = 0L;

		LONG lXP = 0L;
		float fXPDivider = 1.0f;
		BOOL bCoins = TRUE;

		if(((CButton *)(GetDlgItem( IDC_RADIO_1 )))->GetCheck())
		{
			bNeedAmount = TRUE;
			_SourceLong = &m_pCharacter->m_lPlatinumCarried;
			_DestLong = &m_pGiftCharacter->m_lPlatinumCarried;
			lSourceAvailable = *_SourceLong;

			fXPDivider = 0.20f;
		}
		else if(((CButton *)(GetDlgItem( IDC_RADIO_2 )))->GetCheck())
		{
			bNeedAmount = TRUE;
			_SourceLong = &m_pCharacter->m_lGoldCarried;
			_DestLong = &m_pGiftCharacter->m_lGoldCarried;
			lSourceAvailable = *_SourceLong;

			fXPDivider = 1.0f;
		}
		else if(((CButton *)(GetDlgItem( IDC_RADIO_3 )))->GetCheck())
		{
			bNeedAmount = TRUE;
			_SourceLong = &m_pCharacter->m_lElectrumCarried;
			_DestLong = &m_pGiftCharacter->m_lElectrumCarried;
			lSourceAvailable = *_SourceLong;

			fXPDivider = 2.0f;
		}
		else if(((CButton *)(GetDlgItem( IDC_RADIO_4 )))->GetCheck())
		{
			bNeedAmount = TRUE;
			_SourceLong = &m_pCharacter->m_lSilverCarried;
			_DestLong = &m_pGiftCharacter->m_lSilverCarried;
			lSourceAvailable = *_SourceLong;

			fXPDivider = 20.0f;
		}
		else if(((CButton *)(GetDlgItem( IDC_RADIO_5 )))->GetCheck())
		{
			bNeedAmount = TRUE;
			_SourceLong = &m_pCharacter->m_lCopperCarried;
			_DestLong = &m_pGiftCharacter->m_lCopperCarried;
			lSourceAvailable = *_SourceLong;

			fXPDivider = 200.0f;
		}
		else if(m_pSelectedInventorySlot != NULL)
		{
			lXP = m_pSelectedInventorySlot->m_lXP;

			if(m_pSelectedInventorySlot->m_lAmount > 1)
			{
				bNeedAmount = TRUE;
				lSourceAvailable = m_pSelectedInventorySlot->m_lAmount;
			}
		}
		
		if(bNeedAmount)
		{
			ModifyValue((int *)&lAmount, "Transfer Amount:", lSourceAvailable, FALSE);

			if(lAmount > lSourceAvailable)
			{
				lAmount = lSourceAvailable;
			}
		}
		else
		{
			lAmount = 1;
		}
		
		if(_SourceLong != NULL && _DestLong != NULL)
		{
			if(*_SourceLong < lAmount)
				lAmount = *_SourceLong;

			*_SourceLong -= lAmount;
			*_DestLong += lAmount;

			Refresh();

			if(bCoins)
			{
				lXP = (long) (( (float)lAmount / fXPDivider) + 0.5f);
			}

			if(lXP)
			{
				CString szComment;
				szComment.Format("from %s", m_pCharacter->m_szCharacterName);
				DMPartyDialog *pPartyDlg = m_pApp->FindCharacterPartyDialog(m_pGiftCharacter);
				if(m_bGiveWithXPCharacterCheck)
				{	
					m_pGiftCharacter->m_lEarnedXP += lXP;

					if(pPartyDlg != NULL)
					{
						szComment.Format("from %s", m_pCharacter->m_szCharacterName);
						pPartyDlg->LogPartyEvent(FALSE, APPEND_TO_LOG, DND_LOG_EVENT_TYPE_CHARACTER_GAINED_XP_TREASURE, m_pGiftCharacter->m_szCharacterName,m_pGiftCharacter->m_dwCharacterID, lXP, szComment.GetBuffer(0));
					}
				}
				else if(m_bGiveWithXPPartyCheck && pPartyDlg != NULL)
				{
					szComment.Format("from %s, taken by %s", m_pCharacter->m_szCharacterName, m_pGiftCharacter->m_szCharacterName);
					pPartyDlg->m_pParty->m_lXP += lXP;
					pPartyDlg->LogPartyEvent(FALSE, APPEND_TO_LOG, DND_LOG_EVENT_TYPE_PARTY_GAINED_XP_TREASURE, pPartyDlg->m_pParty->m_szPartyName, pPartyDlg->m_pParty->m_dwPartyID, lXP, szComment.GetBuffer(0));
				}
			}

			m_pCharacter->MarkChanged();
			m_pGiftCharacter->MarkChanged();
			MarkCacheChanged(m_pGiftCharacter);

			return;
		}
		else if(m_pSelectedInventorySlot != NULL)
		{
			int nGiftSize = m_pCharacter->CountObjectsInContainer(m_pSelectedInventorySlot);
			int nReceiveSize = m_pGiftCharacter->GetInventoryOpenSlots();

			if(nGiftSize > nReceiveSize)
			{
				AfxMessageBox("The selected item will not fit in the destination inventory !", MB_OK);
				return;
			}

			cDNDObject _LocalObject;

			m_pSelectedInventorySlot->CopyTo(&_LocalObject);
			_LocalObject.m_lAmount = lAmount;
			_LocalObject.m_nMagicAdj = m_pSelectedInventorySlot->m_nMagicAdj;
			_LocalObject.m_dwContainerID = 0;
			

			if(nGiftSize > 1) // gift of a container
			{
				_LocalObject.m_dwObjectID = m_pSelectedInventorySlot->m_dwObjectID;

				m_pGiftCharacter->AddToInventoryFull(&_LocalObject);

				DWORD dwContainerID = m_pSelectedInventorySlot->m_dwObjectID;
				
				for(int i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
				{
					POBJECTTYPE pObj = &m_pCharacter->m_Inventory[i];

					if(pObj != NULL && pObj->m_wTypeId && pObj->m_dwContainerID)
					{
						if(m_pCharacter->ObjectIsInContainer(pObj, dwContainerID, 0))
						{
							m_pCharacter->m_Inventory[i].CopyFull(&_LocalObject);
							m_pGiftCharacter->AddToInventoryFull(&_LocalObject);

							m_pGiftCharacter->m_lPlatinumCarried += _LocalObject.m_nContainerPlatinum;
							m_pGiftCharacter->m_lGoldCarried += _LocalObject.m_nContainerGold;
							m_pGiftCharacter->m_lElectrumCarried += _LocalObject.m_nContainerElectrum;
							m_pGiftCharacter->m_lSilverCarried += _LocalObject.m_nContainerSilver;
							m_pGiftCharacter->m_lCopperCarried += _LocalObject.m_nContainerCopper;

							m_pCharacter->m_Inventory[i].m_wTypeId = 0;
						}
					}
				}

				cDNDObject _BlankObject;
				_BlankObject.CopyFull(m_pSelectedInventorySlot);

				m_pCharacter->ValidateInventory();
				m_pGiftCharacter->ValidateInventory();

				m_pGiftCharacter->BalanceContainerCoins(NULL, DND_COINS_ALL);

				m_pSelectedInventorySlot = NULL;

				DisplaySubWindows();

			}
			else
			{
				m_pGiftCharacter->AddToInventory(&_LocalObject);

				m_pGiftCharacter->m_lPlatinumCarried += _LocalObject.m_nContainerPlatinum;
				m_pGiftCharacter->m_lGoldCarried += _LocalObject.m_nContainerGold;
				m_pGiftCharacter->m_lElectrumCarried += _LocalObject.m_nContainerElectrum;
				m_pGiftCharacter->m_lSilverCarried += _LocalObject.m_nContainerSilver;
				m_pGiftCharacter->m_lCopperCarried += _LocalObject.m_nContainerCopper;

				m_pGiftCharacter->BalanceContainerCoins(NULL, DND_COINS_ALL);

				for(int i = 0; i < lAmount; ++i)
				{		
					m_bDisablePrompts = TRUE;
					OnDropButton();
					m_bDisablePrompts = FALSE;
				}
			}

			m_pCharacter->MarkChanged();
			m_pGiftCharacter->MarkChanged();
			MarkCacheChanged(m_pGiftCharacter);

			RefreshAll();
		}

	}
	
}

void DMInventoryDialog::OnDropButton() 
{
	if(m_pSelectedInventorySlot == NULL)
	{
		return;
	}

	cDNDObject _BlankObj;

	LONG lXP = m_pSelectedInventorySlot->m_lXP;

	if(lXP)
	{
		CString szComment;
		DMPartyDialog *pPartyDlg = m_pApp->FindCharacterPartyDialog(m_pCharacter);
		if(m_bDropWithXPCharacterCheck)
		{	
			m_pCharacter->m_lEarnedXP += lXP;

			if(pPartyDlg != NULL)
			{
				szComment.Format("from a %s", m_pSelectedInventorySlot->m_szType);
				pPartyDlg->LogPartyEvent(FALSE, APPEND_TO_LOG, DND_LOG_EVENT_TYPE_CHARACTER_GAINED_XP_TREASURE, m_pCharacter->m_szCharacterName,m_pCharacter->m_dwCharacterID, lXP, szComment.GetBuffer(0));
			}
		}
		else if(m_bDropWithXPPartyCheck && pPartyDlg != NULL)
		{
			szComment.Format("from a %s, sold by %s", m_pSelectedInventorySlot->m_szType, m_pCharacter->m_szCharacterName);
			pPartyDlg->m_pParty->m_lXP += lXP;
			pPartyDlg->LogPartyEvent(FALSE, APPEND_TO_LOG, DND_LOG_EVENT_TYPE_PARTY_GAINED_XP_TREASURE, pPartyDlg->m_pParty->m_szPartyName, pPartyDlg->m_pParty->m_dwPartyID, lXP, szComment.GetBuffer(0));
		}
	}

	if(m_pSelectedInventorySlot->IsContainer())
	{
		int nContainerContents = m_pCharacter->CountObjectsInContainer(m_pSelectedInventorySlot);

		if(nContainerContents > 1 || m_pSelectedInventorySlot->GetCoinWeight() && m_bDisablePrompts == FALSE)
		{
			if(AfxMessageBox("This will delete the container and ALL its contents !\nAre you sure you wish to do that ?", MB_ICONQUESTION|MB_OKCANCEL) == IDCANCEL)
			{
				return;
			}
		}

		DWORD dwContainerID = m_pSelectedInventorySlot->m_dwObjectID;
				
		for(int i = 0; i < MAX_CHARACTER_INVENTORY; ++i)
		{
			POBJECTTYPE pObj = &m_pCharacter->m_Inventory[i];

			if(pObj != NULL && pObj->m_wTypeId && pObj->m_dwContainerID)
			{
				if(m_pCharacter->ObjectIsInContainer(pObj, dwContainerID, 0))
				{
					m_pCharacter->m_Inventory[i].m_wTypeId = 0;

					m_pCharacter->m_lPlatinumCarried -= m_pCharacter->m_Inventory[i].m_nContainerPlatinum;
					m_pCharacter->m_lGoldCarried -= m_pCharacter->m_Inventory[i].m_nContainerGold;
					m_pCharacter->m_lElectrumCarried -= m_pCharacter->m_Inventory[i].m_nContainerElectrum;
					m_pCharacter->m_lSilverCarried -= m_pCharacter->m_Inventory[i].m_nContainerSilver;
					m_pCharacter->m_lCopperCarried -= m_pCharacter->m_Inventory[i].m_nContainerCopper;
				}
			}
		}

		m_pCharacter->m_lPlatinumCarried -= m_pSelectedInventorySlot->m_nContainerPlatinum;
		m_pCharacter->m_lGoldCarried -= m_pSelectedInventorySlot->m_nContainerGold;
		m_pCharacter->m_lElectrumCarried -= m_pSelectedInventorySlot->m_nContainerElectrum;
		m_pCharacter->m_lSilverCarried -= m_pSelectedInventorySlot->m_nContainerSilver;
		m_pCharacter->m_lCopperCarried -= m_pSelectedInventorySlot->m_nContainerCopper;

		cDNDObject _BlankObject;
		_BlankObject.CopyFull(m_pSelectedInventorySlot);

		m_pCharacter->ValidateInventory();

		m_pCharacter->BalanceContainerCoins(NULL, DND_COINS_ALL);

		m_pSelectedInventorySlot = NULL;

		DisplaySubWindows();
	}
	else
	{
		--m_pSelectedInventorySlot->m_lAmount;
		
		if(m_pSelectedInventorySlot->m_lAmount <= 0)
		{
			_BlankObj.CopyFull(m_pSelectedInventorySlot);

			m_pSelectedInventorySlot = NULL;
			m_cInventoryList.SetCurSel(-1);

			DisplaySubWindows();

			m_pCharacter->ValidateInventory();
		}
	}

	m_pCharacter->MarkChanged();
	MarkCacheChanged(NULL);

	RefreshAll();

}



void DMInventoryDialog::OnChangePpEdit() 
{
	UpdateData(TRUE);

	((CButton *)(GetDlgItem( IDC_RADIO_1 )))->SetCheck(0);
	((CButton *)(GetDlgItem( IDC_RADIO_2 )))->SetCheck(0);
	((CButton *)(GetDlgItem( IDC_RADIO_3 )))->SetCheck(0);
	((CButton *)(GetDlgItem( IDC_RADIO_4 )))->SetCheck(0);
	((CButton *)(GetDlgItem( IDC_RADIO_5 )))->SetCheck(0);

	((CButton *)(GetDlgItem( IDC_RADIO_1 )))->SetCheck(1);
	m_pCharacter->m_lPlatinumCarried = atoi(m_szPlatinumEdit.GetBuffer(0));

	m_cGiveWithXPCharacterCheck.ShowWindow(SW_SHOW);
	m_cGiveWithXPPartyCheck.ShowWindow(SW_SHOW);

	m_cDropWithXPCharacterCheck.ShowWindow(SW_HIDE);
	m_cDropWithXPPartyCheck.ShowWindow(SW_HIDE);

	m_pCharacter->BalanceContainerCoins(NULL, PLATINUM_PIECES);

	RefreshAll();

	m_pCharacter->MarkChanged();
	MarkCacheChanged(NULL);
}


void DMInventoryDialog::OnChangeGpEdit() 
{
	UpdateData(TRUE);

	((CButton *)(GetDlgItem( IDC_RADIO_1 )))->SetCheck(0);
	((CButton *)(GetDlgItem( IDC_RADIO_2 )))->SetCheck(0);
	((CButton *)(GetDlgItem( IDC_RADIO_3 )))->SetCheck(0);
	((CButton *)(GetDlgItem( IDC_RADIO_4 )))->SetCheck(0);
	((CButton *)(GetDlgItem( IDC_RADIO_5 )))->SetCheck(0);

	((CButton *)(GetDlgItem( IDC_RADIO_2 )))->SetCheck(1);
	m_pCharacter->m_lGoldCarried = atoi(m_szGoldEdit.GetBuffer(0));

	m_cGiveWithXPCharacterCheck.ShowWindow(SW_SHOW);
	m_cGiveWithXPPartyCheck.ShowWindow(SW_SHOW);

	m_cDropWithXPCharacterCheck.ShowWindow(SW_HIDE);
	m_cDropWithXPPartyCheck.ShowWindow(SW_HIDE);

	m_pCharacter->BalanceContainerCoins(NULL, GOLD_PIECES);

	RefreshAll();

	m_pCharacter->MarkChanged();
	MarkCacheChanged(NULL);
	
}

void DMInventoryDialog::OnChangeEpEdit() 
{
	UpdateData(TRUE);

	((CButton *)(GetDlgItem( IDC_RADIO_1 )))->SetCheck(0);
	((CButton *)(GetDlgItem( IDC_RADIO_2 )))->SetCheck(0);
	((CButton *)(GetDlgItem( IDC_RADIO_3 )))->SetCheck(0);
	((CButton *)(GetDlgItem( IDC_RADIO_4 )))->SetCheck(0);
	((CButton *)(GetDlgItem( IDC_RADIO_5 )))->SetCheck(0);

	((CButton *)(GetDlgItem( IDC_RADIO_3 )))->SetCheck(1);
	m_pCharacter->m_lElectrumCarried = atoi(m_szElectrumEdit.GetBuffer(0));

	m_cGiveWithXPCharacterCheck.ShowWindow(SW_SHOW);
	m_cGiveWithXPPartyCheck.ShowWindow(SW_SHOW);

	m_cDropWithXPCharacterCheck.ShowWindow(SW_HIDE);
	m_cDropWithXPPartyCheck.ShowWindow(SW_HIDE);

	m_pCharacter->BalanceContainerCoins(NULL, ELECTRUM_PIECES);

	RefreshAll();

	m_pCharacter->MarkChanged();
	MarkCacheChanged(NULL);
	
}

void DMInventoryDialog::OnChangeSpEdit() 
{
	UpdateData(TRUE);

	((CButton *)(GetDlgItem( IDC_RADIO_1 )))->SetCheck(0);
	((CButton *)(GetDlgItem( IDC_RADIO_2 )))->SetCheck(0);
	((CButton *)(GetDlgItem( IDC_RADIO_3 )))->SetCheck(0);
	((CButton *)(GetDlgItem( IDC_RADIO_4 )))->SetCheck(0);
	((CButton *)(GetDlgItem( IDC_RADIO_5 )))->SetCheck(0);

	((CButton *)(GetDlgItem( IDC_RADIO_4 )))->SetCheck(1);
	m_pCharacter->m_lSilverCarried = atoi(m_szSilverEdit.GetBuffer(0));

	m_cGiveWithXPCharacterCheck.ShowWindow(SW_SHOW);
	m_cGiveWithXPPartyCheck.ShowWindow(SW_SHOW);

	m_cDropWithXPCharacterCheck.ShowWindow(SW_HIDE);
	m_cDropWithXPPartyCheck.ShowWindow(SW_HIDE);

	m_pCharacter->BalanceContainerCoins(NULL, SILVER_PIECES);

	RefreshAll();

	m_pCharacter->MarkChanged();
	MarkCacheChanged(NULL);
	
}

void DMInventoryDialog::OnChangeCpEdit() 
{
	UpdateData(TRUE);

	((CButton *)(GetDlgItem( IDC_RADIO_1 )))->SetCheck(0);
	((CButton *)(GetDlgItem( IDC_RADIO_2 )))->SetCheck(0);
	((CButton *)(GetDlgItem( IDC_RADIO_3 )))->SetCheck(0);
	((CButton *)(GetDlgItem( IDC_RADIO_4 )))->SetCheck(0);
	((CButton *)(GetDlgItem( IDC_RADIO_5 )))->SetCheck(0);

	((CButton *)(GetDlgItem( IDC_RADIO_5 )))->SetCheck(1);
	m_pCharacter->m_lCopperCarried = atoi(m_szCopperEdit.GetBuffer(0));

	m_cGiveWithXPCharacterCheck.ShowWindow(SW_SHOW);
	m_cGiveWithXPPartyCheck.ShowWindow(SW_SHOW);

	m_cDropWithXPCharacterCheck.ShowWindow(SW_HIDE);
	m_cDropWithXPPartyCheck.ShowWindow(SW_HIDE);

	m_pCharacter->BalanceContainerCoins(NULL, COPPER_PIECES);

	RefreshAll();

	m_pCharacter->MarkChanged();
	MarkCacheChanged(NULL);
	
}

void DMInventoryDialog::OnStackCoinButton() 
{
	LONG lCP = m_pCharacter->m_lCopperCarried;
	LONG lSP = m_pCharacter->m_lSilverCarried;
	LONG lEP = m_pCharacter->m_lElectrumCarried;
	LONG lGP = m_pCharacter->m_lGoldCarried;
	LONG lPP = m_pCharacter->m_lPlatinumCarried;

	int nStackLevel = 0;
	if( ((CButton *)(GetDlgItem( IDC_RADIO_1 )))->GetState())
		nStackLevel = 4;
	else if( ((CButton *)(GetDlgItem( IDC_RADIO_2 )))->GetState())
		nStackLevel = 3;
	else if( ((CButton *)(GetDlgItem( IDC_RADIO_3 )))->GetState())
		nStackLevel = 2;
	else if( ((CButton *)(GetDlgItem( IDC_RADIO_4 )))->GetState())
		nStackLevel = 1;


	if(nStackLevel > 0)
	{
		lSP += lCP / 10L;
		lCP = lCP % 10L;
	}

	if(nStackLevel > 1)
	{
		lEP += lSP / 10L;
		lSP = lSP % 10L;
	}

	if(nStackLevel > 2)
	{
		lGP += lEP / 2L;
		lEP = lEP % 2L;
	}

	if(nStackLevel > 3)
	{
		lPP += lGP / 5L;
		lGP = lGP % 5L;
	}

	m_pCharacter->m_lCopperCarried =	lCP;
	m_pCharacter->m_lSilverCarried =	lSP;
	m_pCharacter->m_lElectrumCarried =	lEP;
	m_pCharacter->m_lGoldCarried =		lGP;
	m_pCharacter->m_lPlatinumCarried =	lPP;

	TRACE("STACK LEVEL %d\n", nStackLevel, m_pCharacter->m_lPlatinumCarried);

	m_pCharacter->BalanceContainerCoins(NULL, DND_COINS_ALL);

	InitDisplay();

	RefreshAll();

	m_pCharacter->MarkChanged();
	MarkCacheChanged(NULL);
	
}

void DMInventoryDialog::DisplaySubWindows()
{
	if(m_pSelectedInventorySlot != NULL)
	{
		if(m_pSelectedInventorySlot->IsContainer())
		{
			if(m_pContainerDetailsDialog == NULL)
			{
				m_pContainerDetailsDialog = new CDMContainerDetailsDialog(m_pSelectedInventorySlot->m_dwObjectID, this);
			}
			if(m_pContainerDetailsDialog != NULL)
			{
				m_pContainerDetailsDialog->ShowWindow(SW_SHOW);
				m_pContainerDetailsDialog->Refresh(m_pSelectedInventorySlot->m_dwObjectID);
			}
			if(m_pPCDetailsDialog != NULL)
			{
				m_pPCDetailsDialog->ShowWindow(SW_HIDE);
			}
			if(m_pScrollDetailsDialog != NULL)
			{
				m_pScrollDetailsDialog->ShowWindow(SW_HIDE);
			}
			if(m_pMagicSwordPropertiesDialog != NULL)
			{
				m_pMagicSwordPropertiesDialog->ShowWindow(SW_HIDE);
			}
		}
		else if(m_pSelectedInventorySlot->IsMagicContainer())
		{
			if(m_pScrollDetailsDialog == NULL)
			{
				m_pScrollDetailsDialog = new CDMScrollDetailsDialog(this);
			}
			if(m_pScrollDetailsDialog != NULL)
			{
				m_pScrollDetailsDialog->ShowWindow(SW_SHOW);
				m_pScrollDetailsDialog->Refresh();
			}
			if(m_pPCDetailsDialog != NULL)
			{
				m_pPCDetailsDialog->ShowWindow(SW_HIDE);
			}
			if(m_pContainerDetailsDialog != NULL)
			{
				m_pContainerDetailsDialog->ShowWindow(SW_HIDE);
			}
			if(m_pMagicSwordPropertiesDialog != NULL)
			{
				m_pMagicSwordPropertiesDialog->ShowWindow(SW_HIDE);
			}
		}
		else if(m_pSelectedInventorySlot->m_nMagicAdj && m_pSelectedInventorySlot->IsSword())
		{
			if(m_pMagicSwordPropertiesDialog == NULL)
			{
				m_pMagicSwordPropertiesDialog = new cDMMagicSwordPropertiesDialog(this);
			}
			if(m_pMagicSwordPropertiesDialog != NULL)
			{
				m_pMagicSwordPropertiesDialog->ShowWindow(SW_SHOW);
				m_pMagicSwordPropertiesDialog->Refresh();
			}
			if(m_pScrollDetailsDialog != NULL)
			{
				m_pScrollDetailsDialog->ShowWindow(SW_HIDE);
			}
			if(m_pContainerDetailsDialog != NULL)
			{
				m_pContainerDetailsDialog->ShowWindow(SW_HIDE);
			}
			if(m_pPCDetailsDialog != NULL)
			{
				m_pPCDetailsDialog->ShowWindow(SW_HIDE);
			}
			if(m_pContainerDetailsDialog != NULL)
			{
				m_pContainerDetailsDialog->ShowWindow(SW_HIDE);
			}
		}
		else
		{
			if(m_pScrollDetailsDialog != NULL)
			{
				m_pScrollDetailsDialog->ShowWindow(SW_HIDE);
			}
			if(m_pContainerDetailsDialog != NULL)
			{
				m_pContainerDetailsDialog->ShowWindow(SW_HIDE);
			}
			if(m_pMagicSwordPropertiesDialog != NULL)
			{
				m_pMagicSwordPropertiesDialog->ShowWindow(SW_HIDE);
			}
			if(m_pPCDetailsDialog != NULL)
			{
				m_pPCDetailsDialog->ShowWindow(SW_SHOW);
			}
		}
	}
	else
	{
		if(m_pScrollDetailsDialog != NULL)
		{
			m_pScrollDetailsDialog->ShowWindow(SW_HIDE);
		}
		if(m_pContainerDetailsDialog != NULL)
		{
			m_pContainerDetailsDialog->ShowWindow(SW_HIDE);
		}
		if(m_pMagicSwordPropertiesDialog != NULL)
		{
			m_pMagicSwordPropertiesDialog->ShowWindow(SW_HIDE);
		}
		if(m_pPCDetailsDialog != NULL)
		{
			m_pPCDetailsDialog->ShowWindow(SW_SHOW);
		}
	}
	
}

void DMInventoryDialog::OnSelchangeInventoryList() 
{
	UpdateData(TRUE);

	m_cItemInfoButton.EnableWindow(FALSE);

	((CButton *)(GetDlgItem( IDC_RADIO_1 )))->SetCheck(0);
	((CButton *)(GetDlgItem( IDC_RADIO_2 )))->SetCheck(0);
	((CButton *)(GetDlgItem( IDC_RADIO_3 )))->SetCheck(0);
	((CButton *)(GetDlgItem( IDC_RADIO_4 )))->SetCheck(0);
	((CButton *)(GetDlgItem( IDC_RADIO_5 )))->SetCheck(0);

	int nCursor = m_cInventoryList.GetCurSel();

	if(nCursor > -1)
	{
		m_pSelectedInventorySlot = (POBJECTTYPE)m_cInventoryList.GetItemData(nCursor);
	}
	else
	{
		m_pSelectedInventorySlot = NULL;
	}

	if(m_pSelectedInventorySlot != NULL)
	{
		m_cItemIdentified.ShowWindow(SW_SHOW);
		m_cIgnoreEncumbrance.ShowWindow(SW_SHOW);

		m_bItemIdentified = !m_pSelectedInventorySlot->m_bMysteryItem;
		m_bIgnoreEncumbrance = m_pSelectedInventorySlot->m_bIgnoreEncumbrance;

		m_szRenameItem = m_pSelectedInventorySlot->m_szType;
		m_cRenameEdit.EnableWindow(TRUE);
		m_szItemXP.Format("%ld", m_pSelectedInventorySlot->m_lXP);
		m_cItemXPEdit.EnableWindow(TRUE);

		m_cGiveWithXPCharacterCheck.ShowWindow(SW_HIDE);
		m_cGiveWithXPPartyCheck.ShowWindow(SW_HIDE);

		if(m_pCharacter->m_bIsCache == FALSE)
		{
			m_cDropWithXPCharacterCheck.ShowWindow(SW_SHOW);
			m_cDropWithXPPartyCheck.ShowWindow(SW_SHOW);
		}

		CString szItemName = m_pSelectedInventorySlot->m_szType;

		if (szItemName != _T(""))
		{
			cDNDMagicItemDescription *pMagicItemDescription = NULL;

			//szItemName = "potion of animal control";
			//int nLen = szItemName.GetLength();

			BOOL bFound = m_pApp->m_MagicItemDescriptionMap.Lookup(szItemName, pMagicItemDescription);

			if (bFound && pMagicItemDescription != NULL)
			{
				m_cItemInfoButton.EnableWindow(TRUE);
			}
		}
	}
	else
	{
		m_cItemIdentified.ShowWindow(SW_HIDE);
		m_cIgnoreEncumbrance.ShowWindow(SW_HIDE);

		m_szRenameItem = _T("");
		m_cRenameEdit.EnableWindow(FALSE);
		m_szItemXP = _T("");
		m_cItemXPEdit.EnableWindow(FALSE);

		m_cGiveWithXPCharacterCheck.ShowWindow(SW_HIDE);
		m_cGiveWithXPPartyCheck.ShowWindow(SW_HIDE);

		m_cDropWithXPCharacterCheck.ShowWindow(SW_HIDE);
		m_cDropWithXPPartyCheck.ShowWindow(SW_HIDE);

		m_pSelectedInventorySlot = NULL;
	}

	m_cStoreList.SetCurSel(-1);

	DisplaySubWindows();

	UpdateData(FALSE);
	
}

void DMInventoryDialog::OnChangeRenameItemEdit() 
{
	UpdateData(TRUE);

	if(m_pSelectedInventorySlot != NULL)
	{
		CString szTemp = m_szRenameItem.Left(31);

		szTemp.Replace("(", "");
		szTemp.Replace(")", "");

		strcpy(m_pSelectedInventorySlot->m_szType, szTemp.GetBuffer(0));

		m_pSelectedInventorySlot->m_szType[31] = 0;

		m_pSelectedInventorySlot->m_bEnchanted = FALSE;

		Refresh();

		m_pCharacter->MarkChanged();
		MarkCacheChanged(NULL);
	}
}

void DMInventoryDialog::OnEnChangeItemXpEdit()
{
	UpdateData(TRUE);

	if(m_pSelectedInventorySlot != NULL)
	{
		CString szTemp = m_szItemXP.Left(10);

		m_pSelectedInventorySlot->m_lXP = atol(szTemp.GetBuffer(0));

		Refresh();

		m_pCharacter->MarkChanged();
		MarkCacheChanged(NULL);
	}	
}


void DMInventoryDialog::OnSelchangeStoreList() 
{
	m_szRenameItem = _T("");
	m_cRenameEdit.EnableWindow(FALSE);

	m_szItemXP  = _T("");
	m_cItemXPEdit.EnableWindow(FALSE);

	m_cInventoryList.SetCurSel(-1);
	m_pSelectedInventorySlot = NULL;

	if(m_pContainerDetailsDialog != NULL)
	{
		m_pContainerDetailsDialog->ShowWindow(SW_HIDE);
	}
	if(m_pScrollDetailsDialog != NULL)
	{
		m_pScrollDetailsDialog->ShowWindow(SW_HIDE);
	}
	if(m_pPCDetailsDialog != NULL)
	{
		m_pPCDetailsDialog->ShowWindow(SW_SHOW);
	}

	CheckItemSelect();

	Refresh();

	UpdateData(FALSE);
}

void DMInventoryDialog::OnLbnDblclkStoreList()
{
	OnBnClickedItemInfoButton(); 
}


void DMInventoryDialog::OnLbnDblclkInventoryList()
{
	OnBnClickedItemInfoButton();
}


void DMInventoryDialog::OnRadio1() 
{
	m_szRenameItem = _T("");
	m_cRenameEdit.EnableWindow(FALSE);

	m_szItemXP  = _T("");
	m_cItemXPEdit.EnableWindow(FALSE);

	m_cInventoryList.SetCurSel(-1);
	m_pSelectedInventorySlot = NULL;

	m_cInventoryList.SetCurSel(-1);

	m_cGiveWithXPCharacterCheck.ShowWindow(SW_SHOW);
	m_cGiveWithXPPartyCheck.ShowWindow(SW_SHOW);

	m_cDropWithXPCharacterCheck.ShowWindow(SW_HIDE);
	m_cDropWithXPPartyCheck.ShowWindow(SW_HIDE);

	UpdateData(FALSE);
	
}

void DMInventoryDialog::OnRadio2() 
{
	m_szRenameItem = _T("");
	m_cRenameEdit.EnableWindow(FALSE);

	m_szItemXP  = _T("");
	m_cItemXPEdit.EnableWindow(FALSE);

	m_cInventoryList.SetCurSel(-1);
	m_pSelectedInventorySlot = NULL;
	
	m_cInventoryList.SetCurSel(-1);

	m_cGiveWithXPCharacterCheck.ShowWindow(SW_SHOW);
	m_cGiveWithXPPartyCheck.ShowWindow(SW_SHOW);

	m_cDropWithXPCharacterCheck.ShowWindow(SW_HIDE);
	m_cDropWithXPPartyCheck.ShowWindow(SW_HIDE);

	UpdateData(FALSE);
}

void DMInventoryDialog::OnRadio3() 
{
	m_szRenameItem = _T("");
	m_cRenameEdit.EnableWindow(FALSE);

	m_szItemXP  = _T("");
	m_cItemXPEdit.EnableWindow(FALSE);

	m_cInventoryList.SetCurSel(-1);
	m_pSelectedInventorySlot = NULL;
	
	m_cInventoryList.SetCurSel(-1);

	m_cGiveWithXPCharacterCheck.ShowWindow(SW_SHOW);
	m_cGiveWithXPPartyCheck.ShowWindow(SW_SHOW);

	m_cDropWithXPCharacterCheck.ShowWindow(SW_HIDE);
	m_cDropWithXPPartyCheck.ShowWindow(SW_HIDE);

	UpdateData(FALSE);
}

void DMInventoryDialog::OnRadio4() 
{
	m_szRenameItem = _T("");
	m_cRenameEdit.EnableWindow(FALSE);

	m_szItemXP  = _T("");
	m_cItemXPEdit.EnableWindow(FALSE);

	m_cInventoryList.SetCurSel(-1);
	m_pSelectedInventorySlot = NULL;
	
	m_cInventoryList.SetCurSel(-1);

	m_cGiveWithXPCharacterCheck.ShowWindow(SW_SHOW);
	m_cGiveWithXPPartyCheck.ShowWindow(SW_SHOW);

	m_cDropWithXPCharacterCheck.ShowWindow(SW_HIDE);
	m_cDropWithXPPartyCheck.ShowWindow(SW_HIDE);

	UpdateData(FALSE);
}

void DMInventoryDialog::OnRadio5() 
{
	m_szRenameItem = _T("");
	m_cRenameEdit.EnableWindow(FALSE);

	m_szItemXP  = _T("");
	m_cItemXPEdit.EnableWindow(FALSE);

	m_cInventoryList.SetCurSel(-1);
	m_pSelectedInventorySlot = NULL;
	
	m_cInventoryList.SetCurSel(-1);

	m_cGiveWithXPCharacterCheck.ShowWindow(SW_SHOW);
	m_cGiveWithXPPartyCheck.ShowWindow(SW_SHOW);

	m_cDropWithXPCharacterCheck.ShowWindow(SW_HIDE);
	m_cDropWithXPPartyCheck.ShowWindow(SW_HIDE);

	UpdateData(FALSE);
}

void DMInventoryDialog::OnDropAllButton() 
{
	if(AfxMessageBox("Are you sure you wish to erase the character's entire inventory ?!", MB_ICONQUESTION|MB_OKCANCEL) == IDCANCEL)
	{
		return;
	}

	memset(m_pCharacter->m_Inventory, 0, MAX_CHARACTER_INVENTORY * sizeof(cDNDObject));

	m_szRenameItem = _T("");
	m_cRenameEdit.EnableWindow(FALSE);

	m_szItemXP  = _T("");
	m_cItemXPEdit.EnableWindow(FALSE);

	m_cInventoryList.SetCurSel(-1);
	m_pSelectedInventorySlot = NULL;

	m_cInventoryList.SetCurSel(-1);

	Refresh();

	m_pCharacter->MarkChanged();
	MarkCacheChanged(NULL);
	
}

void DMInventoryDialog::OnPricesCheck() 
{
	UpdateData(TRUE);

	m_bWeightsCheck = FALSE;
	m_bXPCheck = FALSE;

	InitDisplay();

	Refresh();	
}

void DMInventoryDialog::OnWeightsCheck() 
{
	UpdateData(TRUE);

	m_bPricesCheck = FALSE;
	m_bXPCheck = FALSE;

	InitDisplay();

	Refresh();		
}


void DMInventoryDialog::OnBnClickedXpCheck()
{
	UpdateData(TRUE);

	m_bWeightsCheck = FALSE;
	m_bPricesCheck = FALSE;

	InitDisplay();

	Refresh();	
}


void DMInventoryDialog::OnWeaponsCheck() 
{	
	UpdateData(TRUE);

	if(m_bWeaponsCheck)
	{
		SelectMagicItems(FALSE);
	}

	InitDisplay();

	Refresh();
}

void DMInventoryDialog::OnArmorCheck() 
{	
	UpdateData(TRUE);

	if(m_bArmorCheck)
	{
		SelectMagicItems(FALSE);
	}

	InitDisplay();

	Refresh();
}

void DMInventoryDialog::OnShieldsCheck() 
{	
	UpdateData(TRUE);

	if(m_bShieldsCheck)
	{
		SelectMagicItems(FALSE);
	}

	InitDisplay();

	Refresh();
}

void DMInventoryDialog::OnEquipCheck() 
{
	UpdateData(TRUE);

	if(m_bEquipCheck)
	{
		SelectMagicItems(FALSE);
	}

	InitDisplay();

	Refresh();
}


void DMInventoryDialog::OnBnClickedMagicComponentsCheck()
{
	UpdateData(TRUE);

	if (m_bMaterialComponentCheck)
	{
		SelectMagicItems(FALSE);
		
		m_bWeaponsCheck = FALSE;
		m_bArmorCheck = FALSE;
		m_bShieldsCheck = FALSE;
		m_bEquipCheck = FALSE;
		m_bMaterialComponentCheck = TRUE;
	}

	InitDisplay();

	Refresh();
}


void DMInventoryDialog::OnRingsCheck() 
{
	UpdateData(TRUE);

	if(m_bRingsCheck)
	{
		SelectMagicItems(TRUE);
	}

	InitDisplay();

	Refresh();
}

void DMInventoryDialog::OnBnClickedScrollsCheck()
{
	UpdateData(TRUE);

	if(m_bScrollsCheck)
	{
		SelectMagicItems(TRUE);
	}

	InitDisplay();

	Refresh();
}


void DMInventoryDialog::OnBnClickedPotionsCheck()
{
	UpdateData(TRUE);

	if(m_bPotionsCheck)
	{
		SelectMagicItems(TRUE);
	}

	InitDisplay();

	Refresh();
}


void DMInventoryDialog::OnBnClickedRodsStavesWandsCheck()
{
	UpdateData(TRUE);

	if(m_bRodsStavesWandsCheck)
	{
		SelectMagicItems(TRUE);
	}

	InitDisplay();

	Refresh();
}



void DMInventoryDialog::OnBnClickedMiscMagic1Check()
{
	UpdateData(TRUE);

	if(m_bMiscMagic1Check)
	{
		SelectMagicItems(TRUE);
	}

	InitDisplay();

	Refresh();
}


void DMInventoryDialog::OnBnClickedMiscMagic2Check()
{
	UpdateData(TRUE);

	if(m_bMiscMagic2Check)
	{
		SelectMagicItems(TRUE);
	}

	InitDisplay();

	Refresh();
}


void DMInventoryDialog::OnBnClickedMiscMagic3Check()
{
	UpdateData(TRUE);

	if(m_bMiscMagic3Check)
	{
		SelectMagicItems(TRUE);
	}

	InitDisplay();

	Refresh();
}

void DMInventoryDialog::OnBnClickedMiscMagic4Check()
{
	UpdateData(TRUE);

	if(m_bMiscMagic4Check)
	{
		SelectMagicItems(TRUE);
	}

	InitDisplay();

	Refresh();
}

void DMInventoryDialog::OnBnClickedMiscMagic5Check()
{
	UpdateData(TRUE);

	if(m_bMiscMagic5Check)
	{
		SelectMagicItems(TRUE);
	}

	InitDisplay();

	Refresh();
}


void DMInventoryDialog::OnBnClickedMiscMagicSpecialCheck()
{
	UpdateData(TRUE);

	if(m_bMiscMagicSpecialCheck)
	{
		SelectMagicItems(TRUE);
	}

	InitDisplay();

	Refresh();
}




void DMInventoryDialog::OnBnClickedMagicalArmorCheck()
{
	UpdateData(TRUE);

	if(m_bMagicalArmorCheck)
	{
		SelectMagicItems(TRUE);
	}

	InitDisplay();

	Refresh();
}

void DMInventoryDialog::OnBnClickedMagicalSwordsCheck()
{
	UpdateData(TRUE);

	if(m_bMagicalSwordsCheck)
	{
		SelectMagicItems(TRUE);
	}

	InitDisplay();

	Refresh();
}


void DMInventoryDialog::OnMiscWeaponsCheck() 
{	
	UpdateData(TRUE);

	if(m_bMiscWeaponsCheck)
	{
		SelectMagicItems(TRUE);
	}

	InitDisplay();

	Refresh();
}




void DMInventoryDialog::OnStackInvButton() 
{

	if(AfxMessageBox("Are you sure you wish to stack the character's inventory ?", MB_ICONQUESTION|MB_OKCANCEL) == IDCANCEL)
	{
		return;
	}

	m_pCharacter->StackInventory();

	/*
	m_pCharacter->ValidateInventory();

	for(int i = 0; i < MAX_CHARACTER_INVENTORY-1; ++i)
	{
		POBJECTTYPE pObj_1 = &m_pCharacter->m_Inventory[i];

		if(pObj_1->m_wTypeId == 0)
			continue;

		if(pObj_1->m_lAmount == 0)
			pObj_1->m_lAmount = 1;

		for(int j = i+1; j < MAX_CHARACTER_INVENTORY; ++j)
		{
			POBJECTTYPE pObj_2 = &m_pCharacter->m_Inventory[j];

			if(pObj_2->m_wTypeId == 0)
				continue;

			if(pObj_1->IsContainer() || pObj_2->IsContainer())
				continue;

			if(pObj_1->m_dwContainerID != pObj_2->m_dwContainerID)
				continue;

			if(pObj_2->m_wTypeId == pObj_1->m_wTypeId && pObj_2->m_nMagicAdj == pObj_1->m_nMagicAdj)
			{
				if(strcmp(pObj_1->m_szType, pObj_2->m_szType) != 0)
				{
					continue;
				}

				if(pObj_2->m_lAmount == 0)
					pObj_2->m_lAmount = 1;

				pObj_1->m_lAmount += pObj_2->m_lAmount;
				pObj_2->m_wTypeId = 0;
			}
		}

	}


	m_pCharacter->ValidateInventory();
	*/

	m_pCharacter->MarkChanged();

	MarkCacheChanged(NULL);

	RefreshAll();
	
}

void DMInventoryDialog::OnAddSubButton() 
{

	UpdateData(TRUE);

	int nCursor = m_cInventoryList.GetCurSel();

	int *pVal = NULL;

	BOOL bCheckInv = FALSE;
	if(nCursor >= 0 && m_pSelectedInventorySlot != NULL)
	{
		pVal = (int *)&m_pSelectedInventorySlot->m_lAmount;

		bCheckInv = TRUE;
	}
	else
	{
		if( ((CButton *)(GetDlgItem( IDC_RADIO_1 )))->GetState())
			pVal = (int *)&m_pCharacter->m_lPlatinumCarried;
		else if( ((CButton *)(GetDlgItem( IDC_RADIO_2 )))->GetState())
			pVal = (int *)&m_pCharacter->m_lGoldCarried;
		else if( ((CButton *)(GetDlgItem( IDC_RADIO_3 )))->GetState())
			pVal = (int *)&m_pCharacter->m_lElectrumCarried;
		else if( ((CButton *)(GetDlgItem( IDC_RADIO_4 )))->GetState())
			pVal = (int *)&m_pCharacter->m_lSilverCarried;
		else if( ((CButton *)(GetDlgItem( IDC_RADIO_5 )))->GetState())
			pVal = (int *)&m_pCharacter->m_lCopperCarried;

	}

	if(pVal != NULL)
	{
		ModifyValue(pVal, "", 0);

		if(*pVal < 0)
			*pVal = 0;

		
		if(bCheckInv && m_pSelectedInventorySlot != NULL)
		{
			if(m_pSelectedInventorySlot->m_lAmount == 0)
			{
				m_pSelectedInventorySlot->m_wTypeId = 0;
			}
		}

		m_pCharacter->MarkChanged();
		MarkCacheChanged(NULL);

		m_pCharacter->ValidateInventory();

		Refresh();

		if(bCheckInv)
		{
			m_cInventoryList.SetCurSel(nCursor);
		}
	}
	
}

void DMInventoryDialog::OnBnClickedMagicAdjButton()
{
	UpdateData(TRUE);

	int nCursor = m_cInventoryList.GetCurSel();

	if(nCursor < 0)
		return;

	if(m_pSelectedInventorySlot == NULL)
		return;
	
	int *pVal = (int *)&m_pSelectedInventorySlot->m_nMagicAdj;

	if(pVal != NULL)
	{
		ModifyValue(pVal,"",0);

		m_pSelectedInventorySlot->m_bEnchanted = FALSE;

		m_pCharacter->ValidateInventory();

		m_pCharacter->MarkChanged();
		MarkCacheChanged(NULL);

		Refresh();

		m_cInventoryList.SetCurSel(nCursor);

		OnSelchangeInventoryList();
	}
}


void DMInventoryDialog::OnAddCustomButton() 
{
	// TODO: Add your control notification handler code here
	
}

void DMInventoryDialog::OnAddProff() 
{
	UpdateData(TRUE);

	POBJECTTYPE pObj = NULL;

	int nCursor1 = m_cInventoryList.GetCurSel();
	int nCursor2 = m_cStoreList.GetCurSel();

	//TRACE("PROFF %d %d\n", nCursor1, nCursor2);

	if(nCursor1 >= 0 && m_bMagicItemSelect == FALSE)
	{
		pObj = (POBJECTTYPE)m_cInventoryList.GetItemData(nCursor1);

		if(pObj == NULL || pObj->m_ObjectType != DND_OBJECT_TYPE_WEAPON)
		{
			pObj = NULL;
		}
	}

	if(nCursor2 >= 0 && pObj == NULL)
	{
		pObj = (POBJECTTYPE)m_cStoreList.GetItemData(nCursor2);

		if(pObj == NULL || pObj->m_ObjectType != DND_OBJECT_TYPE_WEAPON)
		{
			pObj = NULL;
		}
	}

	if(pObj != NULL)
	{
		PWEAPONTYPE pTestWeapon = m_pApp->m_WeaponsIndexedTypeArray.GetAt(pObj->m_wTypeId);

		if(pTestWeapon->m_nExceptionalStrength && IsMissileWeapon(pTestWeapon)) 
		{
			WORD wId = pTestWeapon->m_wTypeId - 1; //look one back in the list for non-exceptional bow
			pObj = m_pApp->m_ObjectsIndexedTypeArray.GetAt(wId);

			if(pObj == NULL)
				return;
		}

		for(int i = 0; i< MAX_WEAPON_PROFICIENCIES; ++i)
		{
			if(m_pCharacter->m_WeaponProficiencies[i].m_wTypeId == pObj->m_wTypeId)
				break;  // no duplicates

			if(m_pCharacter->m_WeaponProficiencies[i].m_wTypeId == 0)
			{
				pObj->CopyFull(&m_pCharacter->m_WeaponProficiencies[i]);
				break;
			}
		}

		m_pCharacter->ValidateProficiencies();

		m_pCharacter->MarkChanged();
		MarkCacheChanged(NULL);

		Refresh();

		if(m_pPCVWindow != NULL)
		{
			m_pPCVWindow->InvalidateRect(NULL);
			m_pPCVWindow->ProcessCharStats();
			m_pPCVWindow->Refresh();
		}
		if(m_pNPCVWindow != NULL)
		{
			m_pNPCVWindow->InvalidateRect(NULL);
			m_pNPCVWindow->ProcessCharStats();
			m_pNPCVWindow->Refresh();
		}
	}
	
	
}

void DMInventoryDialog::OnRemoveProff() 
{
	UpdateData(TRUE);

	int nCursor = m_cProffList.GetCurSel();

	if(nCursor >= 0)
	{
		TRACE("PROFF OFF%d\n", nCursor);

		POBJECTTYPE pObj = (POBJECTTYPE)m_cProffList.GetItemData(nCursor);

		for(int i = 0; i< MAX_WEAPON_PROFICIENCIES; ++i)
		{
			if(m_pCharacter->m_WeaponProficiencies[i].m_wTypeId == pObj->m_wTypeId)
				m_pCharacter->m_WeaponProficiencies[i].m_wTypeId = 0;
		}
	}

	m_pCharacter->MarkChanged();
	MarkCacheChanged(NULL);

	m_pCharacter->ValidateProficiencies();

	Refresh();

	if(m_pPCVWindow != NULL)
	{
		m_pPCVWindow->InvalidateRect(NULL);
		m_pPCVWindow->ProcessCharStats();
		m_pPCVWindow->Refresh();
	}

	if(m_pNPCVWindow != NULL)
	{
		m_pNPCVWindow->InvalidateRect(NULL);
		m_pNPCVWindow->ProcessCharStats();
		m_pNPCVWindow->Refresh();
	}
	
}

void DMInventoryDialog::OnClose() 
{
	CleanUp();

	CDialog::OnClose();

	//delete this;
}

void DMInventoryDialog::PostNcDestroy() 
{
	CDialog::PostNcDestroy();
	
	//delete this;
}


void DMInventoryDialog::OnEnChangeCacheNameEdit()
{
	UpdateData(TRUE);

	strcpy(m_pCharacter->m_szCharacterName, m_szCacheName.Left(127));

	m_pCharacter->MarkChanged();
	MarkCacheChanged(NULL);

	if(m_pNPCVWindow != NULL)
	{
		if(m_pNPCVWindow->m_pNPC->m_bIsCache)
		{
			if(m_pNPCVWindow->m_pCacheInventory != NULL)
			{
				memcpy(m_pNPCVWindow->m_pCacheInventory->m_szCacheDesc,m_pNPCVWindow-> m_pNPC->m_szCharacterName, 128 * sizeof(char));
			}
		}

		m_pNPCVWindow->SetWindowText(m_pCharacter->m_szCharacterName);
		m_pMainDialog->InvalidateRect(NULL); 
	}
}


void DMInventoryDialog::OnEnChangeCacheDetailsEdit()
{
	UpdateData(TRUE);

	strcpy(m_pCharacter->m_szDetails, m_szCacheDetails.Left(511));

	m_pCharacter->MarkChanged();
	MarkCacheChanged(NULL);

	if(m_pNPCVWindow != NULL)
	{
		if(m_pNPCVWindow->m_pNPC->m_bIsCache)
		{
			if(m_pNPCVWindow->m_pCacheInventory != NULL)
			{
				memcpy(m_pNPCVWindow->m_pCacheInventory->m_szCacheDetails, m_pNPCVWindow->m_pNPC->m_szDetails, 512 * sizeof(char));
			}
		}
	}
	
}


/*
m_bGiveWithXPCharacterCheck(FALSE)
	, m_bGiveWithXPPartyCheck(FALSE)
	, m_bDropWithXPCharacterCheck(FALSE)
	, m_bDropWithXPPartyCheck(FALSE)
*/

void DMInventoryDialog::OnBnClickedGiveWithXpCharacterCheck()
{
	UpdateData(TRUE);

	if(m_bGiveWithXPCharacterCheck)
		m_bGiveWithXPPartyCheck = FALSE;

	UpdateData(FALSE);

}

void DMInventoryDialog::OnBnClickedGiveWithXpPartyCheck()
{
	UpdateData(TRUE);

	if(m_bGiveWithXPPartyCheck)
		m_bGiveWithXPCharacterCheck = FALSE;

	UpdateData(FALSE);
}

void DMInventoryDialog::OnBnClickedDropWithXpCharacterCheck()
{
	UpdateData(TRUE);

	if(m_bDropWithXPCharacterCheck)
		m_bDropWithXPPartyCheck = FALSE;

	UpdateData(FALSE);
}

void DMInventoryDialog::OnBnClickedDropWithXpPartyCheck()
{
	UpdateData(TRUE);

	if(m_bDropWithXPPartyCheck)
		m_bDropWithXPCharacterCheck = FALSE;

	UpdateData(FALSE);
}


void DMInventoryDialog::OnBnClickedContainerContentsCheck()
{
	UpdateData(TRUE);

	RefreshAll();
}

void DMInventoryDialog::OnBnClickedItemIdentifiedCheck()
{
	UpdateData(TRUE);

	if(m_pSelectedInventorySlot != NULL)
	{
		m_pSelectedInventorySlot->m_bMysteryItem = !m_bItemIdentified;
		m_pCharacter->MarkChanged();
	}

	RefreshAll();
}

void DMInventoryDialog::OnBnClickedItemEncumbranceCheck()
{
	UpdateData(TRUE);

	if (m_pSelectedInventorySlot != NULL)
	{
		m_pSelectedInventorySlot->m_bIgnoreEncumbrance = m_bIgnoreEncumbrance;  
		m_pCharacter->MarkChanged();
	}

	RefreshAll();
}


void DMInventoryDialog::OnBnClickedTreasureTypeButton()
{

	CString szTreasureType = _T("");
	BOOL bInLair = FALSE;

	if (m_pNPCVWindow != NULL && m_pNPCVWindow->m_pNPC != NULL && m_pNPCVWindow->m_pNPC->m_bIsCache == TRUE)
	{
		bInLair = TRUE;
	}
	
	cDMTreasureTypeDialog *pDlg = new cDMTreasureTypeDialog(&szTreasureType, &bInLair);
	pDlg->DoModal();
	delete pDlg;


	if(szTreasureType != _T(""))
	{
		PDNDMONSTERMANUALENTRY pMonster = new cDNDMonsterManualEntry();
		pMonster->m_szTreasureType = szTreasureType;

		cDNDNonPlayerCharacter *pNPC = (cDNDNonPlayerCharacter *)m_pCharacter;
		RollMonsterTreasure(pNPC, pMonster, bInLair, TRUE);

		delete(pMonster);

		Refresh();
	}

}

void DMInventoryDialog::OnBnClickedSpecializedCheck()
{
	UpdateData(TRUE);

	int nCurrProff = m_cProffList.GetCurSel();

	if(nCurrProff >= 0)
	{
		POBJECTTYPE pObj = &m_pCharacter->m_WeaponProficiencies[nCurrProff];

		if(m_bSpecialized)
		{
			pObj->m_nContents[0] = 1;
		}
		else
		{
			pObj->m_nContents[0] = 0;
		}

		Refresh();
		
		if(m_pPCVWindow != NULL)
		{
			m_pPCVWindow->ProcessCharStats();
		}
	}

}

void DMInventoryDialog::OnBnClickedDoubleSpecializedCheck()
{
	UpdateData(TRUE);

	int nCurrProff = m_cProffList.GetCurSel();

	if(nCurrProff >= 0)
	{
		POBJECTTYPE pObj = &m_pCharacter->m_WeaponProficiencies[nCurrProff];

		if(m_bDoubleSpecialized)
		{
			pObj->m_nContents[0] = 2;
		}
		else
		{
			pObj->m_nContents[0] = 1;
		}

		Refresh();

		if(m_pPCVWindow != NULL)
		{
			m_pPCVWindow->ProcessCharStats();
		}
		
	}
}

void DMInventoryDialog::OnLbnSelchangeProffList()
{
	if(m_pCharacter->GetWeaponSpecializationLevel())
	{
		UpdateData(TRUE);
		Refresh();
	}
}


void DMInventoryDialog::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_dwInventoryFlag != m_pApp->m_dwInventoryFlag)
	{
		m_dwInventoryFlag = m_pApp->m_dwInventoryFlag;

		InitDisplay();
	}

	CDialog::OnMouseMove(nFlags, point);
}


void DMInventoryDialog::OnBnClickedItemInfoButton()
{
	UpdateData(TRUE);

	CString szItemName = _T("");

	int nCursor = m_cStoreList.GetCurSel();

	if (nCursor >= 0)
	{
		if (m_bMagicItemSelect)
		{
			PMAGICTABLEITEM pMagicItem = (PMAGICTABLEITEM)m_cStoreList.GetItemData(nCursor);

			szItemName.Format("%s", pMagicItem->m_szDesc);
		}
		else
		{
			POBJECTTYPE pObj = (POBJECTTYPE)m_cStoreList.GetItemData(nCursor);
		}
	}
	else if (m_pSelectedInventorySlot != NULL)
	{
		szItemName = m_pSelectedInventorySlot->m_szExtendedName;
	}

	if (szItemName != _T(""))
	{
		cDNDMagicItemDescription *pMagicItemDescription = NULL;

		BOOL bFound = m_pApp->m_MagicItemDescriptionMap.Lookup(szItemName, pMagicItemDescription);

		if (bFound && pMagicItemDescription != NULL)
		{
			CDMItemDescriptionDialog *pDlg = new CDMItemDescriptionDialog(pMagicItemDescription);
			pDlg->DoModal();
			delete pDlg;
		}
	}
}





void DMInventoryDialog::OnBnClickedGenTreasureButton()
{
	// RollMonsterTreasure(cDNDNonPlayerCharacter *pNPC, PDNDMONSTERMANUALENTRY pMonster, BOOL bLair, BOOL bAddCoins)

	if (m_pNPCVWindow != NULL)
	{
		PDNDMONSTERMANUALENTRY pMonster = NULL;

		m_pApp->m_MonsterManualIndexedMap.Lookup(m_pNPCVWindow->m_pNPC->m_nMonsterIndex, pMonster);

		if (pMonster != NULL && m_pNPCVWindow->m_pNPC->m_bIsCache == FALSE)
		{
			RollMonsterTreasure(m_pNPCVWindow->m_pNPC, pMonster, m_bInLairCheck, TRUE);

			Refresh();
		}	
	}
}

void DMInventoryDialog::OnBnClickedInLairCheck()
{
	UpdateData(TRUE);
}

void DMInventoryDialog::OnStnDblclickCacheNameStatic()
{
	if (m_szCacheNameDesc.Find("Treasure Types") >= 0)
	{
		cDMChartLookupDialog *pDlg = new cDMChartLookupDialog(DND_CHART_TREASURE_TYPES);
		pDlg->DoModal();
		delete pDlg;
	}
}


