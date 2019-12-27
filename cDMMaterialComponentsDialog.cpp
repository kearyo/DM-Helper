// cDMMaterialComponentsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "cDMMaterialComponentsDialog.h"
#include "afxdialogex.h"


// cDMMaterialComponentsDialog dialog

IMPLEMENT_DYNAMIC(cDMMaterialComponentsDialog, CDialog)
cDMMaterialComponentsDialog::cDMMaterialComponentsDialog(PDNDCHARACTER pCharacter, PSPELL pSpell, int nMultiples, std::vector<POBJECTINDEXER> *pSpellMaterialComponentsRequiredVector, BOOL bCheckComponents, BOOL bGetInfo, DND_SPELL_MATERIAL_RETURN_CODES *pnReturnCode, CWnd* pParent /*=NULL*/)
	: CDialog(cDMMaterialComponentsDialog::IDD, pParent)
	, m_szComponents(_T(""))
	, m_szComponentsLine(_T(""))
{
	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_pCharacter = pCharacter;
	m_pSpell = pSpell;
	m_nMultiples = nMultiples;
	m_pSpellMaterialComponentsRequiredVector = pSpellMaterialComponentsRequiredVector;
	m_bCheckComponents = bCheckComponents;
	m_bGetInfo = bGetInfo;
	m_pnReturnCode = pnReturnCode;
}

cDMMaterialComponentsDialog::~cDMMaterialComponentsDialog()
{
}

void cDMMaterialComponentsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_COMPONENTS_LIST, m_szComponents);
	DDX_Text(pDX, IDC_COMPONENTS_LINE, m_szComponentsLine);
	DDX_Control(pDX, IDOK, m_cCastButton);
	DDX_Control(pDX, IDCANCEL, m_cCancelButton);
	DDX_Control(pDX, IDC_BUY_COMPONENTS_BUTTON, m_cBuyComponentsButton);
	DDX_Control(pDX, IDC_GIVE_COMPONENTS_BUTTON, m_cGiveComponentsButton);
	DDX_Control(pDX, IDC_GIVE_ALL_COMPONENTS_BUTTON, m_cGiveAllComponentsButton);
	DDX_Control(pDX, IDC_BUY_ALL_COMPONENTS_BUTTON, m_cBuyAllComponentsButton);
}


BEGIN_MESSAGE_MAP(cDMMaterialComponentsDialog, CDialog)
	ON_BN_CLICKED(IDOK, &cDMMaterialComponentsDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &cDMMaterialComponentsDialog::OnBnClickedCancel)
	ON_STN_CLICKED(IDC_COMPONENTS_LIST, &cDMMaterialComponentsDialog::OnStnClickedComponentsList)
	ON_BN_CLICKED(IDC_BUY_COMPONENTS_BUTTON, &cDMMaterialComponentsDialog::OnBnClickedBuyComponentsButton)
	ON_BN_CLICKED(IDC_GIVE_COMPONENTS_BUTTON, &cDMMaterialComponentsDialog::OnBnClickedGiveComponentsButton)
	ON_BN_CLICKED(IDC_BUY_ALL_COMPONENTS_BUTTON, &cDMMaterialComponentsDialog::OnBnClickedBuyAllComponentsButton)
	ON_BN_CLICKED(IDC_GIVE_ALL_COMPONENTS_BUTTON, &cDMMaterialComponentsDialog::OnBnClickedGiveAllComponentsButton)
END_MESSAGE_MAP()


// cDMMaterialComponentsDialog message handlers


void cDMMaterialComponentsDialog::OnBnClickedOk()
{
	*m_pnReturnCode = DND_SPELL_MATERIAL_RETURN_CAN_CAST;
	CDialog::OnOK();
}


void cDMMaterialComponentsDialog::OnBnClickedCancel()
{
	*m_pnReturnCode = DND_SPELL_MATERIAL_RETURN_CANNOT_CAST;
	CDialog::OnCancel();
}


BOOL cDMMaterialComponentsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString szTitle;
	szTitle.Format("Material Components - %s", m_pSpell->m_szSpellName);

	#ifdef _DEBUG
	CString szDebug;
	szDebug.Format(" ( %d )", m_pSpell->m_nSpellIdentifier);
	szTitle += szDebug;
	#endif

	SetWindowText(szTitle);

	if (m_bGetInfo)
	{
		m_szComponentsLine = _T("This spell requires the following material components:");
		m_cCastButton.ShowWindow(SW_HIDE);
	}
	else
	{
		m_szComponentsLine = _T("Verify the following material components for this spell:");
		m_cBuyComponentsButton.ShowWindow(SW_HIDE);
		m_cGiveComponentsButton.ShowWindow(SW_HIDE);
		m_cBuyAllComponentsButton.ShowWindow(SW_HIDE);
		m_cGiveAllComponentsButton.ShowWindow(SW_HIDE);
	}

	CString szTemp = _T("");
	BOOL bFirstRow = TRUE;
	for (int i = 0; i < MAX_SPELL_COMPONENT_AND_COLUMNS; ++i)
	{
		if (m_pSpellMaterialComponentsRequiredVector[i].size())
		{
			if (bFirstRow == FALSE)
			{
				m_szComponents += "\nAND\n";
			}

			bFirstRow = FALSE;

			BOOL bFirstColumn = TRUE;
			std::vector<POBJECTINDEXER>::iterator iter;
			for (iter = m_pSpellMaterialComponentsRequiredVector[i].begin(); iter != m_pSpellMaterialComponentsRequiredVector[i].end(); iter++)
			{
				POBJECTINDEXER pIndexer = *iter;

				if (bFirstColumn == FALSE)
				{
					m_szComponents += " OR ";
				}

				if (pIndexer->m_nAmount > 1)
				{
					szTemp.Format("%d x ", pIndexer->m_nAmount);
					m_szComponents += szTemp;
				}

				m_szComponents += pIndexer->m_szName;

				bFirstColumn = FALSE;
			}
			
		}
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void cDMMaterialComponentsDialog::OnStnClickedComponentsList()
{
	// TODO: Add your control notification handler code here
}


void cDMMaterialComponentsDialog::OnBnClickedBuyComponentsButton()
{
	BOOL bBoughtCompents = m_pCharacter->BuySpellComponents(m_pSpell, m_nMultiples, FALSE, m_pSpellMaterialComponentsRequiredVector);

	if (bBoughtCompents == FALSE)
	{
		AfxMessageBox("INSUFFICIENT FUNDS TO BUY ALL COMPONENTS !", MB_OK);
	}
	else
	{
		m_pApp->PlaySoundFX("Purchase");
		OnOK();
	}
}


void cDMMaterialComponentsDialog::OnBnClickedGiveComponentsButton()
{
	m_pCharacter->BuySpellComponents(m_pSpell, m_nMultiples, TRUE, m_pSpellMaterialComponentsRequiredVector);

	OnOK();
}


void cDMMaterialComponentsDialog::OnBnClickedBuyAllComponentsButton()
{
	BOOL bBoughtCompents = m_pCharacter->BuyAllSpellComponents(FALSE);

	if (bBoughtCompents == FALSE)
	{
		AfxMessageBox("INSUFFICIENT FUNDS TO BUY ALL COMPONENTS !", MB_OK);
	}
	else
	{
		m_pApp->PlaySoundFX("Purchase");
		OnOK();
	}
}


void cDMMaterialComponentsDialog::OnBnClickedGiveAllComponentsButton()
{
	m_pCharacter->BuyAllSpellComponents(TRUE);
	OnOK();
}


