// DMHPEditDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMInventoryDialog.h"
#include "DMCharSpellsDialog.h"
#include "DMCharViewDialog.h"
#include "DMNPCViewDialog.h"
#include "DMHPEditDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DMHPEditDialog dialog


DMHPEditDialog::DMHPEditDialog(cDNDCharacter *_Character, CWnd* pParent /*=NULL*/)
	: CDialog(DMHPEditDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(DMHPEditDialog)
	m_szClass1Desc = _T("");
	m_szClass2Desc = _T("");
	m_szClass3Desc = _T("");
	for(int i = 0; i < 3; ++ i) for(int j = 0; j < 20; ++j) m_szHP[i][j] = _T("");
	for(int j = 0; j < 20; ++j) m_szLLabel[j] = _T("");
	//}}AFX_DATA_INIT

	m_pDMCharViewDialog = (CDMCharViewDialog *)pParent;

	m_pCharacter = _Character;

	Create(DMHPEditDialog::IDD, pParent);
}


void DMHPEditDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DMHPEditDialog)
	DDX_Text(pDX, IDC_LL_STATIC_1, m_szLLabel[0]);
	DDX_Text(pDX, IDC_LL_STATIC_2, m_szLLabel[1]);
	DDX_Text(pDX, IDC_LL_STATIC_3, m_szLLabel[2]);
	DDX_Text(pDX, IDC_LL_STATIC_4, m_szLLabel[3]);
	DDX_Text(pDX, IDC_LL_STATIC_5, m_szLLabel[4]);
	DDX_Text(pDX, IDC_LL_STATIC_6, m_szLLabel[5]);
	DDX_Text(pDX, IDC_LL_STATIC_7, m_szLLabel[6]);
	DDX_Text(pDX, IDC_LL_STATIC_8, m_szLLabel[7]);
	DDX_Text(pDX, IDC_LL_STATIC_9, m_szLLabel[8]);
	DDX_Text(pDX, IDC_LL_STATIC_10, m_szLLabel[9]);
	DDX_Text(pDX, IDC_LL_STATIC_11, m_szLLabel[10]);
	DDX_Text(pDX, IDC_LL_STATIC_12, m_szLLabel[11]);
	DDX_Text(pDX, IDC_LL_STATIC_13, m_szLLabel[12]);
	DDX_Text(pDX, IDC_LL_STATIC_14, m_szLLabel[13]);
	DDX_Text(pDX, IDC_LL_STATIC_15, m_szLLabel[14]);
	DDX_Text(pDX, IDC_LL_STATIC_16, m_szLLabel[15]);
	DDX_Text(pDX, IDC_LL_STATIC_17, m_szLLabel[16]);
	DDX_Text(pDX, IDC_LL_STATIC_18, m_szLLabel[17]);

	//////////////////////////////////////////////////

	DDX_Text(pDX, IDC_CLASS_1_STATIC, m_szClass1Desc);
	DDX_Text(pDX, IDC_CLASS_2_STATIC, m_szClass2Desc);
	DDX_Text(pDX, IDC_CLASS_3_STATIC, m_szClass3Desc);

	//////////////////////////////////////////////////

	DDX_Control(pDX, IDC_HP_EDIT_1_1, m_cHPEdit[0][0]);
	DDX_Control(pDX, IDC_HP_EDIT_1_2, m_cHPEdit[0][1]);
	DDX_Control(pDX, IDC_HP_EDIT_1_3, m_cHPEdit[0][2]);
	DDX_Control(pDX, IDC_HP_EDIT_1_4, m_cHPEdit[0][3]);
	DDX_Control(pDX, IDC_HP_EDIT_1_5, m_cHPEdit[0][4]);
	DDX_Control(pDX, IDC_HP_EDIT_1_6, m_cHPEdit[0][5]);
	DDX_Control(pDX, IDC_HP_EDIT_1_7, m_cHPEdit[0][6]);
	DDX_Control(pDX, IDC_HP_EDIT_1_8, m_cHPEdit[0][7]);
	DDX_Control(pDX, IDC_HP_EDIT_1_9, m_cHPEdit[0][8]);
	DDX_Control(pDX, IDC_HP_EDIT_1_10, m_cHPEdit[0][9]);
	DDX_Control(pDX, IDC_HP_EDIT_1_11, m_cHPEdit[0][10]);
	DDX_Control(pDX, IDC_HP_EDIT_1_12, m_cHPEdit[0][11]);
	DDX_Control(pDX, IDC_HP_EDIT_1_13, m_cHPEdit[0][12]);
	DDX_Control(pDX, IDC_HP_EDIT_1_14, m_cHPEdit[0][13]);
	DDX_Control(pDX, IDC_HP_EDIT_1_15, m_cHPEdit[0][14]);
	DDX_Control(pDX, IDC_HP_EDIT_1_16, m_cHPEdit[0][15]);
	DDX_Control(pDX, IDC_HP_EDIT_1_17, m_cHPEdit[0][16]);
	DDX_Control(pDX, IDC_HP_EDIT_1_18, m_cHPEdit[0][17]);
	DDX_Control(pDX, IDC_HP_EDIT_1_19, m_cHPEdit[0][18]);

	DDX_Control(pDX, IDC_HP_EDIT_2_1, m_cHPEdit[1][0]);
	DDX_Control(pDX, IDC_HP_EDIT_2_2, m_cHPEdit[1][1]);
	DDX_Control(pDX, IDC_HP_EDIT_2_3, m_cHPEdit[1][2]);
	DDX_Control(pDX, IDC_HP_EDIT_2_4, m_cHPEdit[1][3]);
	DDX_Control(pDX, IDC_HP_EDIT_2_5, m_cHPEdit[1][4]);
	DDX_Control(pDX, IDC_HP_EDIT_2_6, m_cHPEdit[1][5]);
	DDX_Control(pDX, IDC_HP_EDIT_2_7, m_cHPEdit[1][6]);
	DDX_Control(pDX, IDC_HP_EDIT_2_8, m_cHPEdit[1][7]);
	DDX_Control(pDX, IDC_HP_EDIT_2_9, m_cHPEdit[1][8]);
	DDX_Control(pDX, IDC_HP_EDIT_2_10, m_cHPEdit[1][9]);
	DDX_Control(pDX, IDC_HP_EDIT_2_11, m_cHPEdit[1][10]);
	DDX_Control(pDX, IDC_HP_EDIT_2_12, m_cHPEdit[1][11]);
	DDX_Control(pDX, IDC_HP_EDIT_2_13, m_cHPEdit[1][12]);
	DDX_Control(pDX, IDC_HP_EDIT_2_14, m_cHPEdit[1][13]);
	DDX_Control(pDX, IDC_HP_EDIT_2_15, m_cHPEdit[1][14]);
	DDX_Control(pDX, IDC_HP_EDIT_2_16, m_cHPEdit[1][15]);
	DDX_Control(pDX, IDC_HP_EDIT_2_17, m_cHPEdit[1][16]);
	DDX_Control(pDX, IDC_HP_EDIT_2_18, m_cHPEdit[1][17]);
	DDX_Control(pDX, IDC_HP_EDIT_2_19, m_cHPEdit[1][18]);

	DDX_Control(pDX, IDC_HP_EDIT_3_1, m_cHPEdit[2][0]);
	DDX_Control(pDX, IDC_HP_EDIT_3_2, m_cHPEdit[2][1]);
	DDX_Control(pDX, IDC_HP_EDIT_3_3, m_cHPEdit[2][2]);
	DDX_Control(pDX, IDC_HP_EDIT_3_4, m_cHPEdit[2][3]);
	DDX_Control(pDX, IDC_HP_EDIT_3_5, m_cHPEdit[2][4]);
	DDX_Control(pDX, IDC_HP_EDIT_3_6, m_cHPEdit[2][5]);
	DDX_Control(pDX, IDC_HP_EDIT_3_7, m_cHPEdit[2][6]);
	DDX_Control(pDX, IDC_HP_EDIT_3_8, m_cHPEdit[2][7]);
	DDX_Control(pDX, IDC_HP_EDIT_3_9, m_cHPEdit[2][8]);
	DDX_Control(pDX, IDC_HP_EDIT_3_10, m_cHPEdit[2][9]);
	DDX_Control(pDX, IDC_HP_EDIT_3_11, m_cHPEdit[2][10]);
	DDX_Control(pDX, IDC_HP_EDIT_3_12, m_cHPEdit[2][11]);
	DDX_Control(pDX, IDC_HP_EDIT_3_13, m_cHPEdit[2][12]);
	DDX_Control(pDX, IDC_HP_EDIT_3_14, m_cHPEdit[2][13]);
	DDX_Control(pDX, IDC_HP_EDIT_3_15, m_cHPEdit[2][14]);
	DDX_Control(pDX, IDC_HP_EDIT_3_16, m_cHPEdit[2][15]);
	DDX_Control(pDX, IDC_HP_EDIT_3_17, m_cHPEdit[2][16]);
	DDX_Control(pDX, IDC_HP_EDIT_3_18, m_cHPEdit[2][17]);
	DDX_Control(pDX, IDC_HP_EDIT_3_19, m_cHPEdit[2][18]);

	/////////////////////////////////////////////////////////

	DDX_Text(pDX, IDC_HP_EDIT_1_1, m_szHP[0][0]);
	DDX_Text(pDX, IDC_HP_EDIT_1_2, m_szHP[0][1]);
	DDX_Text(pDX, IDC_HP_EDIT_1_3, m_szHP[0][2]);
	DDX_Text(pDX, IDC_HP_EDIT_1_4, m_szHP[0][3]);
	DDX_Text(pDX, IDC_HP_EDIT_1_5, m_szHP[0][4]);
	DDX_Text(pDX, IDC_HP_EDIT_1_6, m_szHP[0][5]);
	DDX_Text(pDX, IDC_HP_EDIT_1_7, m_szHP[0][6]);
	DDX_Text(pDX, IDC_HP_EDIT_1_8, m_szHP[0][7]);
	DDX_Text(pDX, IDC_HP_EDIT_1_9, m_szHP[0][8]);
	DDX_Text(pDX, IDC_HP_EDIT_1_10, m_szHP[0][9]);
	DDX_Text(pDX, IDC_HP_EDIT_1_11, m_szHP[0][10]);
	DDX_Text(pDX, IDC_HP_EDIT_1_12, m_szHP[0][11]);
	DDX_Text(pDX, IDC_HP_EDIT_1_13, m_szHP[0][12]);
	DDX_Text(pDX, IDC_HP_EDIT_1_14, m_szHP[0][13]);
	DDX_Text(pDX, IDC_HP_EDIT_1_15, m_szHP[0][14]);
	DDX_Text(pDX, IDC_HP_EDIT_1_16, m_szHP[0][15]); 
	DDX_Text(pDX, IDC_HP_EDIT_1_17, m_szHP[0][16]);
	DDX_Text(pDX, IDC_HP_EDIT_1_18, m_szHP[0][17]);
	DDX_Text(pDX, IDC_HP_EDIT_1_19, m_szHP[0][18]);

	DDX_Text(pDX, IDC_HP_EDIT_2_1, m_szHP[1][0]);
	DDX_Text(pDX, IDC_HP_EDIT_2_2, m_szHP[1][1]);
	DDX_Text(pDX, IDC_HP_EDIT_2_3, m_szHP[1][2]);
	DDX_Text(pDX, IDC_HP_EDIT_2_4, m_szHP[1][3]);
	DDX_Text(pDX, IDC_HP_EDIT_2_5, m_szHP[1][4]);
	DDX_Text(pDX, IDC_HP_EDIT_2_6, m_szHP[1][5]);
	DDX_Text(pDX, IDC_HP_EDIT_2_7, m_szHP[1][6]);
	DDX_Text(pDX, IDC_HP_EDIT_2_8, m_szHP[1][7]);
	DDX_Text(pDX, IDC_HP_EDIT_2_9, m_szHP[1][8]);
	DDX_Text(pDX, IDC_HP_EDIT_2_10, m_szHP[1][9]);
	DDX_Text(pDX, IDC_HP_EDIT_2_11, m_szHP[1][10]);
	DDX_Text(pDX, IDC_HP_EDIT_2_12, m_szHP[1][11]);
	DDX_Text(pDX, IDC_HP_EDIT_2_13, m_szHP[1][12]);
	DDX_Text(pDX, IDC_HP_EDIT_2_14, m_szHP[1][13]);
	DDX_Text(pDX, IDC_HP_EDIT_2_15, m_szHP[1][14]);
	DDX_Text(pDX, IDC_HP_EDIT_2_16, m_szHP[1][15]); 
	DDX_Text(pDX, IDC_HP_EDIT_2_17, m_szHP[1][16]);
	DDX_Text(pDX, IDC_HP_EDIT_2_18, m_szHP[1][17]);
	DDX_Text(pDX, IDC_HP_EDIT_2_19, m_szHP[1][18]);

	DDX_Text(pDX, IDC_HP_EDIT_3_1, m_szHP[2][0]);
	DDX_Text(pDX, IDC_HP_EDIT_3_2, m_szHP[2][1]);
	DDX_Text(pDX, IDC_HP_EDIT_3_3, m_szHP[2][2]);
	DDX_Text(pDX, IDC_HP_EDIT_3_4, m_szHP[2][3]);
	DDX_Text(pDX, IDC_HP_EDIT_3_5, m_szHP[2][4]);
	DDX_Text(pDX, IDC_HP_EDIT_3_6, m_szHP[2][5]);
	DDX_Text(pDX, IDC_HP_EDIT_3_7, m_szHP[2][6]);
	DDX_Text(pDX, IDC_HP_EDIT_3_8, m_szHP[2][7]);
	DDX_Text(pDX, IDC_HP_EDIT_3_9, m_szHP[2][8]);
	DDX_Text(pDX, IDC_HP_EDIT_3_10, m_szHP[2][9]);
	DDX_Text(pDX, IDC_HP_EDIT_3_11, m_szHP[2][10]);
	DDX_Text(pDX, IDC_HP_EDIT_3_12, m_szHP[2][11]);
	DDX_Text(pDX, IDC_HP_EDIT_3_13, m_szHP[2][12]);
	DDX_Text(pDX, IDC_HP_EDIT_3_14, m_szHP[2][13]);
	DDX_Text(pDX, IDC_HP_EDIT_3_15, m_szHP[2][14]);
	DDX_Text(pDX, IDC_HP_EDIT_3_16, m_szHP[2][15]); 
	DDX_Text(pDX, IDC_HP_EDIT_3_17, m_szHP[2][16]);
	DDX_Text(pDX, IDC_HP_EDIT_3_18, m_szHP[2][17]);
	DDX_Text(pDX, IDC_HP_EDIT_3_19, m_szHP[2][18]);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(DMHPEditDialog, CDialog)
	//{{AFX_MSG_MAP(DMHPEditDialog)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &DMHPEditDialog::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DMHPEditDialog message handlers

BOOL DMHPEditDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	for(int j = 0; j < 3; ++j)
	{
		for(int i = 0; i <= 18; ++i)
		{
			m_szHP[j][i].Format("%d", m_pCharacter->m_nHitPointRolls[j][i]);
			m_cHPEdit[j][i].ShowWindow(SW_HIDE);

			if(!m_pCharacter->m_Class[j])
				continue;

			m_szClass1Desc.Format("%s", GetClassName(m_pCharacter->m_Class[0]));

			if(j == 1)
				m_szClass2Desc.Format("%s", GetClassName(m_pCharacter->m_Class[1]));

			if(j == 2)
				m_szClass3Desc.Format("%s", GetClassName(m_pCharacter->m_Class[2]));

			if(i == 0)
			{
				switch(m_pCharacter->m_Class[j])
				{
					case DND_CHARACTER_CLASS_RANGER:
					case DND_CHARACTER_CLASS_MONK:
					//case DND_CHARACTER_CLASS_BARBARIAN:
					{
						break;
					}
					#if CUSTOM_CLASSES
					case DND_CHARACTER_CLASS_CUSTOM_1:
					case DND_CHARACTER_CLASS_CUSTOM_2:
					case DND_CHARACTER_CLASS_CUSTOM_3:
					case DND_CHARACTER_CLASS_CUSTOM_4:
					case DND_CHARACTER_CLASS_CUSTOM_5:
					case DND_CHARACTER_CLASS_CUSTOM_6:
					case DND_CHARACTER_CLASS_CUSTOM_7:
					case DND_CHARACTER_CLASS_CUSTOM_8:
					case DND_CHARACTER_CLASS_CUSTOM_9:
					case DND_CHARACTER_CLASS_CUSTOM_10:
					case DND_CHARACTER_CLASS_CUSTOM_11:
					case DND_CHARACTER_CLASS_CUSTOM_12:
					{
						if (GetCustomClass(m_pCharacter->m_Class[j])->m_nFirstLevelHD > 1)
						{
							break;
						}
						continue;
					}
					#endif
					default:
					{
						continue;
					}
				}
			}

			int nAdditionalHP = 0;
			int nLevel = min(m_pCharacter->m_nLevel[j], GetMaxHitDiceByClass(m_pCharacter->m_Class[j], &nAdditionalHP));

			if(i >= nLevel+1)  // +1 because of monk and ranger weirdness
				continue;

			m_cHPEdit[j][i].ShowWindow(SW_SHOW);

			if(i)
			{
				m_szLLabel[i-1].Format("%d",i);
			}
		}
	}

	CString szTemp;
	szTemp.Format("Hitpoints per level - %s", m_pCharacter->m_szCharacterName);

	SetWindowText(szTemp);

	UpdateData(FALSE);

	ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DMHPEditDialog::OnBnClickedOk()
{
	UpdateData(TRUE);

	for (int j = 0; j < 3; ++j)
	{
		for (int i = 0; i < 19; ++i)
		{
			m_pCharacter->m_nHitPointRolls[j][i] = atoi(m_szHP[j][i].GetBuffer(0));
		}
	}

	if (m_pDMCharViewDialog != NULL)
	{
		m_pDMCharViewDialog->ProcessCharStats();
		m_pDMCharViewDialog->Refresh();
		m_pDMCharViewDialog->m_pHPEditDialog = NULL;
	}

	CDialog::OnOK();
}


void DMHPEditDialog::OnCancel() 
{
	
	if(m_pDMCharViewDialog != NULL)
	{
		m_pDMCharViewDialog->m_pHPEditDialog = NULL;
	}
	
	CDialog::OnCancel();
}

void DMHPEditDialog::OnClose() 
{

	if(m_pDMCharViewDialog != NULL)
	{
		m_pDMCharViewDialog->m_pHPEditDialog = NULL;
	}
	
	CDialog::OnClose();
}


