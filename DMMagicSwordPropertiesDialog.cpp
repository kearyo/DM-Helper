// DMMagicSwordPropertiesDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMInventoryDialog.h"
#include "DMCharSpellsDialog.h"
#include "DMCharViewDialog.h"
#include "DMNPCViewDialog.h"
#include "DMModifyValueDialog.h"
#include "DMMagicSwordPropertiesDialog.h"
#include "DMLanguageSelectDialog.h"
#include "DMStringSelectorDialog.h"



// cDMMagicSwordPropertiesDialog dialog

IMPLEMENT_DYNAMIC(cDMMagicSwordPropertiesDialog, CDialog)

cDMMagicSwordPropertiesDialog::cDMMagicSwordPropertiesDialog(CWnd* pParent /*=NULL*/)
	: CDialog(cDMMagicSwordPropertiesDialog::IDD, pParent)
	, m_szIntelligenceEdit(_T(""))
	, m_szEgoEdit(_T(""))
	, m_szPrimaryAbility1(_T(""))
	, m_szPrimaryAbility2(_T(""))
	, m_szPrimaryAbility3(_T(""))
	, m_szExceptionalPower(_T(""))
	, m_szSpecialPurpose(_T(""))
	, m_szSpecialPurposePower(_T(""))
	, m_bSemiEmpath(FALSE)
	, m_bEmpath(FALSE)
	, m_bSpeech(FALSE)
	, m_bTelepath(FALSE)
	, m_szLanguages(_T(""))
{
	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_pParentWindow = (DMInventoryDialog *)pParent;

	m_pSelectedInventoryObject = NULL;

	Create(cDMMagicSwordPropertiesDialog::IDD, pParent);
}

cDMMagicSwordPropertiesDialog::~cDMMagicSwordPropertiesDialog()
{
}

void cDMMagicSwordPropertiesDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ALIGNMENT_COMBO, m_cAlignmentCombo);
	DDX_Text(pDX, IDC_INT_EDIT, m_szIntelligenceEdit);
	DDX_Text(pDX, IDC_EGO_EDIT, m_szEgoEdit);
	DDV_MaxChars(pDX, m_szIntelligenceEdit, 2);
	DDX_Text(pDX, IDC_PRIMARY_ABILITY_EDIT_1, m_szPrimaryAbility1);
	DDX_Text(pDX, IDC_PRIMARY_ABILITY_EDIT_2, m_szPrimaryAbility2);
	DDX_Text(pDX, IDC_PRIMARY_ABILITY_EDIT_3, m_szPrimaryAbility3);
	DDX_Text(pDX, IDC_EXCEPTIONAL_POWER_EDIT, m_szExceptionalPower);
	DDX_Text(pDX, IDC_SPECIAL_PURPOSE_EDIT, m_szSpecialPurpose);
	DDX_Text(pDX, IDC_SPECIAL_PURPOSE_POWER_EDIT, m_szSpecialPurposePower);
	DDX_Check(pDX, IDC_SEMI_EMPATH_CHECK, m_bSemiEmpath);
	DDX_Check(pDX, IDC_EMPATH_CHECK, m_bEmpath);
	DDX_Check(pDX, IDC_SPEECH_CHECK, m_bSpeech);
	DDX_Check(pDX, IDC_TELEPATH_CHECK, m_bTelepath);
	DDX_Control(pDX, IDOK, m_cOK);
	DDX_Control(pDX, IDC_INT_EDIT, m_cIntelligenceEdit);
	DDX_Text(pDX, IDC_LANGUAGES_EDIT, m_szLanguages);
	DDX_Control(pDX, IDC_LANGUAGES_EDIT, m_cLanguagesEdit);
}


BEGIN_MESSAGE_MAP(cDMMagicSwordPropertiesDialog, CDialog)
	ON_BN_CLICKED(IDCANCEL, &cDMMagicSwordPropertiesDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &cDMMagicSwordPropertiesDialog::OnBnClickedOk)
	ON_EN_CHANGE(IDC_INT_EDIT, &cDMMagicSwordPropertiesDialog::OnEnChangeIntEdit)
	ON_CBN_SELCHANGE(IDC_ALIGNMENT_COMBO, &cDMMagicSwordPropertiesDialog::OnCbnSelchangeAlignmentCombo)
	ON_EN_SETFOCUS(IDC_LANGUAGES_EDIT, &cDMMagicSwordPropertiesDialog::OnEnSetfocusLanguagesEdit)
	ON_EN_CHANGE(IDC_LANGUAGES_EDIT, &cDMMagicSwordPropertiesDialog::OnEnChangeLanguagesEdit)
	ON_BN_CLICKED(IDC_SELECT_LANGUAGES_BUTTON, &cDMMagicSwordPropertiesDialog::OnBnClickedSelectLanguagesButton)
	ON_BN_CLICKED(IDC_PRIMARY_ABILITY_BUTTON_1, &cDMMagicSwordPropertiesDialog::OnBnClickedPrimaryAbilityButton1)
	ON_BN_CLICKED(IDC_PRIMARY_ABILITY_BUTTON_2, &cDMMagicSwordPropertiesDialog::OnBnClickedPrimaryAbilityButton2)
	ON_BN_CLICKED(IDC_PRIMARY_ABILITY_BUTTON_3, &cDMMagicSwordPropertiesDialog::OnBnClickedPrimaryAbilityButton3)
	ON_BN_CLICKED(IDC_EXCEPTIONAL_POWER_BUTTON, &cDMMagicSwordPropertiesDialog::OnBnClickedExceptionalPowerButton)
	ON_BN_CLICKED(IDC_SPECIAL_PURPOSE_BUTTON, &cDMMagicSwordPropertiesDialog::OnBnClickedSpecialPurposeButton)
	ON_BN_CLICKED(IDC_SPECIAL_PURPOSE_POWER_BUTTON, &cDMMagicSwordPropertiesDialog::OnBnClickedSpecialPurposePowerButton)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_PRIMARY_ABILITY_EDIT_1, &cDMMagicSwordPropertiesDialog::OnEnChangePrimaryAbilityEdit1)
	ON_EN_CHANGE(IDC_PRIMARY_ABILITY_EDIT_2, &cDMMagicSwordPropertiesDialog::OnEnChangePrimaryAbilityEdit2)
	ON_EN_CHANGE(IDC_PRIMARY_ABILITY_EDIT_3, &cDMMagicSwordPropertiesDialog::OnEnChangePrimaryAbilityEdit3)
	ON_EN_CHANGE(IDC_EXCEPTIONAL_POWER_EDIT, &cDMMagicSwordPropertiesDialog::OnEnChangeExceptionalPowerEdit)
	ON_EN_CHANGE(IDC_SPECIAL_PURPOSE_EDIT, &cDMMagicSwordPropertiesDialog::OnEnChangeSpecialPurposeEdit)
	ON_EN_CHANGE(IDC_SPECIAL_PURPOSE_POWER_EDIT, &cDMMagicSwordPropertiesDialog::OnEnChangeSpecialPurposePowerEdit)
	ON_BN_CLICKED(IDC_SEMI_EMPATH_CHECK, &cDMMagicSwordPropertiesDialog::OnBnClickedSemiEmpathCheck)
	ON_BN_CLICKED(IDC_EMPATH_CHECK, &cDMMagicSwordPropertiesDialog::OnBnClickedEmpathCheck)
	ON_BN_CLICKED(IDC_SPEECH_CHECK, &cDMMagicSwordPropertiesDialog::OnBnClickedSpeechCheck)
	ON_BN_CLICKED(IDC_TELEPATH_CHECK, &cDMMagicSwordPropertiesDialog::OnBnClickedTelepathCheck)
END_MESSAGE_MAP()


// cDMMagicSwordPropertiesDialog message handlers

BOOL cDMMagicSwordPropertiesDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	for(int i = 0; i < 10; ++i)
	{
		m_cAlignmentCombo.InsertString(i, GetAlignmentName((DND_CHARACTER_ALIGNMENTS)i));
	}

	int nOffset = 0;

	if(m_pParentWindow->m_pCharacter->m_bIsCache)
	{
		nOffset += 46;
	}

	SetWindowPos(NULL, m_pParentWindow->m_nDetailsPosX, m_pParentWindow->m_nDetailsPosY+nOffset, 0, 0, SW_SHOW);
	ShowWindow(SW_SHOW);

	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void cDMMagicSwordPropertiesDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	Refresh();
}


void cDMMagicSwordPropertiesDialog::Refresh()
{
	m_pSelectedInventoryObject = m_pParentWindow->m_pSelectedInventorySlot;


	if(m_pSelectedInventoryObject == NULL)
		return;

	m_szIntelligenceEdit.Format("%d", m_pSelectedInventoryObject->m_nIntelligence);

	m_cAlignmentCombo.SetCurSel((int)m_pSelectedInventoryObject->m_Alignment);

	BOOL bFirst = TRUE;
	m_szLanguages = _T("");

	float fEgo = 0.0;

	int nCount = 0;
	for (int i = 0; i < m_pApp->m_LanguageArray.GetSize(); ++i )
	{
		cDNDLanguage *pLanguage = (cDNDLanguage *)m_pApp->m_LanguageArray.GetAt(i);

		if(pLanguage != NULL)
		{
			if(pLanguage->m_nList)
			{
				if(m_pSelectedInventoryObject->CanSpeakLanguage(pLanguage->m_nIndex))
				{
					if(bFirst == FALSE)
					{
						m_szLanguages += _T(", ");
					}
					bFirst = FALSE;

					m_szLanguages += pLanguage->m_szLanguageName;

					fEgo += 0.5f;
				}
			}
		}
	}


	if(m_pSelectedInventoryObject->m_nContentsCount[0] & 0x0001)
	{
		m_bSemiEmpath = TRUE;
	}
	else
	{
		m_bSemiEmpath = FALSE;
	}
	if(m_pSelectedInventoryObject->m_nContentsCount[0] & 0x0002)
	{
		m_bEmpath = TRUE;
	}
	else
	{
		m_bEmpath = FALSE;
	}
	if(m_pSelectedInventoryObject->m_nContentsCount[0] & 0x0004)
	{
		m_bSpeech = TRUE;
	}
	else
	{
		m_bSpeech = FALSE;
	}
	if(m_pSelectedInventoryObject->m_nContentsCount[0] & 0x0008)
	{
		m_bTelepath = TRUE;
	}
	else
	{
		m_bTelepath = FALSE;
	}

	m_szPrimaryAbility1.Format("%s", m_pSelectedInventoryObject->m_szPrimaryAbility[0]);
	m_szPrimaryAbility2.Format("%s", m_pSelectedInventoryObject->m_szPrimaryAbility[1]);
	m_szPrimaryAbility3.Format("%s", m_pSelectedInventoryObject->m_szPrimaryAbility[2]);
	m_szExceptionalPower.Format("%s", m_pSelectedInventoryObject->m_szExtraordinaryPower);
	m_szSpecialPurpose.Format("%s", m_pSelectedInventoryObject->m_szSpecialPurpose);
	m_szSpecialPurposePower.Format("%s", m_pSelectedInventoryObject->m_szSpecialPurposePower);


	//calculate the sword's ego

	fEgo += abs(m_pSelectedInventoryObject->m_nMagicAdj);

	if(m_szPrimaryAbility1.GetLength() > 0) fEgo += 1.0f;
	if(m_szPrimaryAbility2.GetLength() > 0) fEgo += 1.0f;
	if(m_szPrimaryAbility3.GetLength() > 0) fEgo += 1.0f;
	if(m_szExceptionalPower.GetLength() > 0) fEgo += 2.0f;
	if(m_szSpecialPurpose.GetLength() > 0) fEgo += 5.0f;

	if(m_bTelepath) fEgo += 2.0f; //telepathy
	if(m_pSelectedInventoryObject->m_nIntelligence >= 16) fEgo += 1.0f; //read languages
	if(m_pSelectedInventoryObject->m_nIntelligence >= 17) fEgo += 2.0f; //read magic

	int nEgo = (int)(fEgo + 0.5f);

	// calculate bearer's personality strength

	int nPersonalityStrength = 0;
	if(!m_pParentWindow->m_pCharacter->m_bIsCache)
	{
		nPersonalityStrength = m_pParentWindow->m_pCharacter->m_nDisplayStats[ATTRIB_INT] + m_pParentWindow->m_pCharacter->m_nDisplayStats[ATTRIB_CHA];

		int nLevel = GetAverageCharacterLevel(m_pParentWindow->m_pCharacter);

		nPersonalityStrength += nLevel;

		if(nLevel)
		{
			float fHPByLevel = (float)m_pParentWindow->m_pCharacter->m_nHitPoints / (float)nLevel;

			int nTemp = (int)(fHPByLevel + 0.5f);

			fHPByLevel = (float)nTemp;

			float fStrengthLoss = (float)m_pParentWindow->m_pCharacter->m_nCurrentDamage / fHPByLevel;

			nPersonalityStrength -= (int)(fStrengthLoss + 0.5f);
		}
	}

	m_szEgoEdit.Format("%d / %d", nEgo, nPersonalityStrength);

	UpdateData(FALSE);
}

void cDMMagicSwordPropertiesDialog::MarkChanged()
{
	if(m_pParentWindow->m_pNPCVWindow != NULL)
	{
		if(m_pParentWindow->m_pNPCVWindow->m_pNPC->m_bIsCache)
		{
			if(m_pParentWindow->m_pNPCVWindow->m_pCacheInventory != NULL)
			{
				m_pParentWindow->m_pNPCVWindow->m_pCacheInventory->MarkChanged();
			}
		}
	}
	if(m_pParentWindow->m_pCharacter != NULL)
	{
		m_pParentWindow->m_pCharacter->MarkChanged();
	}
}

void cDMMagicSwordPropertiesDialog::OnBnClickedCancel()
{
	OnCancel();
}

void cDMMagicSwordPropertiesDialog::OnBnClickedOk()
{
	OnOK();
}

void cDMMagicSwordPropertiesDialog::OnEnChangeIntEdit()
{
	UpdateData(TRUE);

	if(m_pSelectedInventoryObject != NULL)
	{
		m_pSelectedInventoryObject->m_nIntelligence = atoi(m_szIntelligenceEdit.GetBuffer(0));
		m_pParentWindow->m_pCharacter->MarkChanged();
		MarkChanged();
	}
}

void cDMMagicSwordPropertiesDialog::OnCbnSelchangeAlignmentCombo()
{
	UpdateData(TRUE);

	if(m_pSelectedInventoryObject != NULL)
	{
		m_pSelectedInventoryObject->m_Alignment = (DND_CHARACTER_ALIGNMENTS)m_cAlignmentCombo.GetCurSel();
		m_pParentWindow->m_pCharacter->MarkChanged();
		MarkChanged();
	}

}

void cDMMagicSwordPropertiesDialog::OnEnSetfocusLanguagesEdit()
{
	TRACE("WEE\n");
}

void cDMMagicSwordPropertiesDialog::OnEnChangeLanguagesEdit()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void cDMMagicSwordPropertiesDialog::OnBnClickedSelectLanguagesButton()
{
	CDMLanguageSelectDialog *pDlg = new CDMLanguageSelectDialog(m_pSelectedInventoryObject, NULL);

	pDlg->DoModal();

	delete pDlg;

	Refresh();

	MarkChanged();
}

void cDMMagicSwordPropertiesDialog::OnBnClickedPrimaryAbilityButton1()
{
	CDMStringSelectorDialog *pDlg = new CDMStringSelectorDialog(m_pSelectedInventoryObject->m_szPrimaryAbility[0], 128, this);

	for(int i = 0; i < 10; ++i)
	{
		pDlg->InsertString(i, GetMagicSwordPrimaryAbility(i, -1));
	}

	MarkChanged();
}

void cDMMagicSwordPropertiesDialog::OnBnClickedPrimaryAbilityButton2()
{
	CDMStringSelectorDialog *pDlg = new CDMStringSelectorDialog(m_pSelectedInventoryObject->m_szPrimaryAbility[1], 128, this);

	for(int i = 0; i < 10; ++i)
	{
		pDlg->InsertString(i, GetMagicSwordPrimaryAbility(i, -1));
	}

	MarkChanged();
}

void cDMMagicSwordPropertiesDialog::OnBnClickedPrimaryAbilityButton3()
{
	CDMStringSelectorDialog *pDlg = new CDMStringSelectorDialog(m_pSelectedInventoryObject->m_szPrimaryAbility[2], 128, this);

	for(int i = 0; i < 10; ++i)
	{
		pDlg->InsertString(i, GetMagicSwordPrimaryAbility(i, -1));
	}

	MarkChanged();
}

void cDMMagicSwordPropertiesDialog::OnBnClickedExceptionalPowerButton()
{
	CDMStringSelectorDialog *pDlg = new CDMStringSelectorDialog(m_pSelectedInventoryObject->m_szExtraordinaryPower, 128, this);

	for(int i = 0; i < 14; ++i)
	{
		pDlg->InsertString(i, GetMagicSwordExtraordinaryPowers(i, -1));
	}

	MarkChanged();
}

void cDMMagicSwordPropertiesDialog::OnBnClickedSpecialPurposeButton()
{
	CDMStringSelectorDialog *pDlg = new CDMStringSelectorDialog(m_pSelectedInventoryObject->m_szSpecialPurpose, 128, this);

	for(int i = 0; i < 9; ++i)
	{
		pDlg->InsertString(i, GetMagicSwordSpecialPurpose(i, -1));
	}

	MarkChanged();
}

void cDMMagicSwordPropertiesDialog::OnBnClickedSpecialPurposePowerButton()
{
	CDMStringSelectorDialog *pDlg = new CDMStringSelectorDialog(m_pSelectedInventoryObject->m_szSpecialPurposePower, 128, this);

	for(int i = 0; i < 7; ++i)
	{
		pDlg->InsertString(i, GetMagicSwordSpecialPurposePower(i, -1));
	}

	MarkChanged();
}


void cDMMagicSwordPropertiesDialog::OnEnChangePrimaryAbilityEdit1()
{
	UpdateData(TRUE);
	sprintf(m_pSelectedInventoryObject->m_szPrimaryAbility[0], "%s", m_szPrimaryAbility1.Left(127));
	m_pSelectedInventoryObject->m_szPrimaryAbility[0][127] = 0;

	MarkChanged();
}

void cDMMagicSwordPropertiesDialog::OnEnChangePrimaryAbilityEdit2()
{
	UpdateData(TRUE);
	sprintf(m_pSelectedInventoryObject->m_szPrimaryAbility[1], "%s", m_szPrimaryAbility2.Left(127));
	m_pSelectedInventoryObject->m_szPrimaryAbility[1][127] = 0;

	MarkChanged();
}

void cDMMagicSwordPropertiesDialog::OnEnChangePrimaryAbilityEdit3()
{
	UpdateData(TRUE);
	sprintf(m_pSelectedInventoryObject->m_szPrimaryAbility[2], "%s", m_szPrimaryAbility3.Left(127));
	m_pSelectedInventoryObject->m_szPrimaryAbility[2][127] = 0;

	MarkChanged();
}

void cDMMagicSwordPropertiesDialog::OnEnChangeExceptionalPowerEdit()
{
	UpdateData(TRUE);
	sprintf(m_pSelectedInventoryObject->m_szExtraordinaryPower, "%s", m_szExceptionalPower.Left(127));
	m_pSelectedInventoryObject->m_szExtraordinaryPower[127] = 0;

	MarkChanged();
}

void cDMMagicSwordPropertiesDialog::OnEnChangeSpecialPurposeEdit()
{
	UpdateData(TRUE);
	sprintf(m_pSelectedInventoryObject->m_szSpecialPurpose, "%s", m_szSpecialPurpose.Left(127));
	m_pSelectedInventoryObject->m_szSpecialPurpose[127] = 0;

	MarkChanged();
}

void cDMMagicSwordPropertiesDialog::OnEnChangeSpecialPurposePowerEdit()
{
	UpdateData(TRUE);
	sprintf(m_pSelectedInventoryObject->m_szSpecialPurposePower, "%s", m_szSpecialPurposePower.Left(127));
	m_pSelectedInventoryObject->m_szSpecialPurposePower[127] = 0;

	MarkChanged();
}

void cDMMagicSwordPropertiesDialog::OnBnClickedSemiEmpathCheck()
{
	UpdateData(TRUE);

	if(m_bSemiEmpath)
	{
		m_pSelectedInventoryObject->m_nContentsCount[0] |= 0x0001;
	}
	else
	{
		m_pSelectedInventoryObject->m_nContentsCount[0] ^= 0x0001;
	}

	MarkChanged();

}

void cDMMagicSwordPropertiesDialog::OnBnClickedEmpathCheck()
{
	UpdateData(TRUE);

	if(m_bEmpath)
	{
		m_pSelectedInventoryObject->m_nContentsCount[0] |= 0x0002;
	}
	else
	{
		m_pSelectedInventoryObject->m_nContentsCount[0] ^= 0x0002;
	}

	MarkChanged();
}

void cDMMagicSwordPropertiesDialog::OnBnClickedSpeechCheck()
{
	UpdateData(TRUE);

	if(m_bSpeech)
	{
		m_pSelectedInventoryObject->m_nContentsCount[0] |= 0x0004;
	}
	else
	{
		m_pSelectedInventoryObject->m_nContentsCount[0] ^= 0x0004;
	}

	MarkChanged();
}

void cDMMagicSwordPropertiesDialog::OnBnClickedTelepathCheck()
{
	UpdateData(TRUE);

	if(m_bTelepath)
	{
		m_pSelectedInventoryObject->m_nContentsCount[0] |= 0x0008;
	}
	else
	{
		m_pSelectedInventoryObject->m_nContentsCount[0] ^= 0x0008;
	}

	MarkChanged();
}
