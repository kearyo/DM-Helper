// DMPCDetailsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMInventoryDialog.h"
#include "DMCharSpellsDialog.h"
#include "DMCharViewDialog.h"
#include "DMNPCViewDialog.h"
#include "DMPCDetailsDialog.h"
#include "DMCharacterNotesDialog.h"


// CDMPCDetailsDialog dialog

IMPLEMENT_DYNAMIC(CDMPCDetailsDialog, CDialog)

CDMPCDetailsDialog::CDMPCDetailsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDMPCDetailsDialog::IDD, pParent)
	, m_szPlayerWillTitle(_T(""))
	, m_szPlayerWill(_T(""))
	, m_szPlayerName(_T(""))
	, m_szDateCreated(_T(""))
	, m_szPatronDeity(_T(""))
	, m_szPlaceofOrigin(_T(""))
	, m_szReligion(_T(""))
	, m_szHeight(_T(""))
	, m_szWeight(_T(""))
	, m_szAge(_T(""))
	, m_szDOBMonth(_T(""))
	, m_szDOBDay(_T(""))
	, m_szDOBYear(_T(""))
	, m_szAgeAdjust(_T(""))
	, m_szHairColor(_T(""))
	, m_szEyeColor(_T(""))
{

	m_pParentWindow = (DMInventoryDialog *)pParent;
	m_pPC = m_pParentWindow->m_pCharacter;

	Create(CDMPCDetailsDialog::IDD, pParent);
}

CDMPCDetailsDialog::~CDMPCDetailsDialog()
{
}

void CDMPCDetailsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WILL_STATIC, m_szPlayerWillTitle);
	DDX_Text(pDX, IDC_PLAYER_WILL_EDIT, m_szPlayerWill);
	DDV_MaxChars(pDX, m_szPlayerWill, 256);
	DDX_Text(pDX, IDC_PLAYER_NAME_EDIT, m_szPlayerName);
	DDV_MaxChars(pDX, m_szPlayerName, 31);
	DDX_Text(pDX, IDC_CHAR_CREATE_DATE_EDIT, m_szDateCreated);
	DDV_MaxChars(pDX, m_szDateCreated, 15);
	DDX_Text(pDX, IDC_PLAYER_DEITY_EDIT, m_szPatronDeity);
	DDV_MaxChars(pDX, m_szPatronDeity, 31);
	DDX_Text(pDX, IDC_CHAR_ORIGIN_EDIT, m_szPlaceofOrigin);
	DDV_MaxChars(pDX, m_szPlaceofOrigin, 31);
	DDX_Text(pDX, IDC_PLAYER_RELIGION_EDIT, m_szReligion);
	DDV_MaxChars(pDX, m_szReligion, 31);
	DDX_Text(pDX, IDC_CHAR_HEIGHT_EDIT, m_szHeight);
	DDV_MaxChars(pDX, m_szHeight, 3);
	DDX_Text(pDX, IDC_CHAR_WEIGHT_EDIT, m_szWeight);
	DDV_MaxChars(pDX, m_szWeight, 4);
	DDX_Text(pDX, IDC_CHAR_AGE_EDIT, m_szAge);
	DDV_MaxChars(pDX, m_szAge, 4);
	DDX_Text(pDX, IDC_CHAR_DOB_MONTH_EDIT, m_szDOBMonth);
	DDV_MaxChars(pDX, m_szDOBMonth, 2);
	DDX_Text(pDX, IDC_CHAR_DOB_DAY_EDIT, m_szDOBDay);
	DDV_MaxChars(pDX, m_szDOBDay, 2);
	DDX_Text(pDX, IDC_CHAR_DOB_YEAR_EDIT, m_szDOBYear);
	DDV_MaxChars(pDX, m_szDOBYear, 5);
	DDX_Text(pDX, IDC_CHAR_AGE_ADJUST_EDIT, m_szAgeAdjust);
	DDV_MaxChars(pDX, m_szAgeAdjust, 4);
	DDX_Text(pDX, IDC_CHAR_HAIR_COLOR_EDIT, m_szHairColor);
	DDV_MaxChars(pDX, m_szHairColor, 15);
	DDX_Text(pDX, IDC_CHAR_EYE_COLOR_EDIT, m_szEyeColor);
	DDV_MaxChars(pDX, m_szEyeColor, 15);
}


BEGIN_MESSAGE_MAP(CDMPCDetailsDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CDMPCDetailsDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDMPCDetailsDialog::OnBnClickedCancel)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_CHAR_AGE_EDIT, &CDMPCDetailsDialog::OnEnChangeCharAgeEdit)
	ON_EN_CHANGE(IDC_PLAYER_NAME_EDIT, &CDMPCDetailsDialog::OnEnChangePlayerNameEdit)
	ON_EN_CHANGE(IDC_CHAR_CREATE_DATE_EDIT, &CDMPCDetailsDialog::OnEnChangeCharCreateDateEdit)
	ON_EN_CHANGE(IDC_PLAYER_DEITY_EDIT, &CDMPCDetailsDialog::OnEnChangePlayerDeityEdit)
	ON_EN_CHANGE(IDC_CHAR_ORIGIN_EDIT, &CDMPCDetailsDialog::OnEnChangeCharOriginEdit)
	ON_EN_CHANGE(IDC_PLAYER_RELIGION_EDIT, &CDMPCDetailsDialog::OnEnChangePlayerReligionEdit)
	ON_EN_CHANGE(IDC_CHAR_HEIGHT_EDIT, &CDMPCDetailsDialog::OnEnChangeCharHeightEdit)
	ON_EN_CHANGE(IDC_CHAR_WEIGHT_EDIT, &CDMPCDetailsDialog::OnEnChangeCharWeightEdit)
	ON_EN_CHANGE(IDC_CHAR_DOB_MONTH_EDIT, &CDMPCDetailsDialog::OnEnChangeCharDobMonthEdit)
	ON_EN_CHANGE(IDC_CHAR_DOB_DAY_EDIT, &CDMPCDetailsDialog::OnEnChangeCharDobDayEdit)
	ON_EN_CHANGE(IDC_CHAR_AGE_ADJUST_EDIT, &CDMPCDetailsDialog::OnEnChangeCharAgeAdjustEdit)
	ON_EN_CHANGE(IDC_CHAR_HAIR_COLOR_EDIT, &CDMPCDetailsDialog::OnEnChangeCharHairColorEdit)
	ON_EN_CHANGE(IDC_CHAR_EYE_COLOR_EDIT, &CDMPCDetailsDialog::OnEnChangeCharEyeColorEdit)
	ON_EN_CHANGE(IDC_PLAYER_WILL_EDIT, &CDMPCDetailsDialog::OnEnChangePlayerWillEdit)
	ON_EN_CHANGE(IDC_CHAR_DOB_YEAR_EDIT, &CDMPCDetailsDialog::OnEnChangeCharDobYearEdit)
	ON_BN_CLICKED(IDC_NOTES_BUTTON, &CDMPCDetailsDialog::OnBnClickedNotesButton)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CDMPCDetailsDialog message handlers

BOOL CDMPCDetailsDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowPos(NULL, m_pParentWindow->m_nDetailsPosX, m_pParentWindow->m_nDetailsPosY, 0, 0, SW_SHOW);
	ShowWindow(SW_SHOW);

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDMPCDetailsDialog::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CDMPCDetailsDialog::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CDMPCDetailsDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting	
}

void CDMPCDetailsDialog::Init(BOOL bAgeEdit)
{
	m_szPlayerName = m_pPC->m_szPlayerName;
	m_szDateCreated = m_pPC->m_szDateCreated;
	m_szPatronDeity = m_pPC->m_szPatronDeity;
	m_szPlaceofOrigin = m_pPC->m_szPlaceofOrigin;
	m_szReligion = m_pPC->m_szReligion;

	m_szHeight.Format("%d", m_pPC->m_nHeight);
	m_szWeight.Format("%d", m_pPC->m_nWeight);

	m_szAge.Format("%d", m_pPC->m_nAge);

	m_szDOBMonth.Format("%d", m_pPC->m_nDOBMonth);
	m_szDOBDay.Format("%d", m_pPC->m_nDOBDay);

	if(bAgeEdit == FALSE)
	{
		if(m_pPC->m_nDOBYear == 0)
			m_szDOBYear = _T("?");
		else
			m_szDOBYear.Format("%d", m_pPC->m_nDOBYear);
	}

	m_szAgeAdjust.Format("%d", m_pPC->m_nAgeAdj);

	m_szHairColor = m_pPC->m_szHairColor;
	m_szEyeColor = m_pPC->m_szEyeColor;

	m_szPlayerWill = m_pPC->m_szWill;

	m_szPlayerWillTitle.Format("I %s do hereby:", m_pParentWindow->m_pCharacter->m_szCharacterName);
	//give all of my stuff to the rest of my party unless they can use it to resurrect me

	UpdateData(FALSE);


}

void CDMPCDetailsDialog::Refresh(BOOL bCharacterChanged, BOOL bClosing)
{
	UpdateData(TRUE);

	strcpy(m_pPC->m_szPlayerName, m_szPlayerName.Left(31));
	strcpy(m_pPC->m_szDateCreated, m_szDateCreated.Left(15));
	strcpy(m_pPC->m_szPatronDeity, m_szPatronDeity.Left(31));
	strcpy(m_pPC->m_szPlaceofOrigin, m_szPlaceofOrigin.Left(31));
	strcpy(m_pPC->m_szReligion, m_szReligion.Left(31));

	m_pPC->m_nHeight = atoi(m_szHeight.GetBuffer(0));
	m_pPC->m_nWeight = atoi(m_szWeight.GetBuffer(0));

	m_pPC->m_nAge = atoi(m_szAge.GetBuffer(0));

	m_pPC->m_nDOBMonth = atoi(m_szDOBMonth.GetBuffer(0));
	m_pPC->m_nDOBDay = atoi(m_szDOBDay.GetBuffer(0));
	m_pPC->m_nDOBYear = atoi(m_szDOBYear.GetBuffer(0));

	m_pPC->m_nAgeAdj = atoi(m_szAgeAdjust.GetBuffer(0));

	strcpy(m_pPC->m_szHairColor, m_szHairColor.Left(15));
	strcpy(m_pPC->m_szEyeColor, m_szEyeColor.Left(15));

	strcpy(m_pPC->m_szWill, m_szPlayerWill.Left(255));

	if(bClosing == FALSE)
	{
		if(m_pParentWindow->m_pPCVWindow != NULL)
		{
			m_pParentWindow->m_pPCVWindow->InvalidateRect(NULL);
			m_pParentWindow->m_pPCVWindow->ProcessCharStats();
			m_pParentWindow->m_pPCVWindow->Refresh();
		}
	}

	if(bCharacterChanged)
	{
		m_pPC->MarkChanged();
	}

}

void CDMPCDetailsDialog::OnEnChangeCharAgeEdit()
{

	UpdateData(TRUE);

	m_pPC->m_nAge = atoi(m_szAge.GetBuffer(0));

	if(m_pParentWindow->m_pPCVWindow != NULL && m_pParentWindow->m_pPCVWindow->m_nYear != 0)
	{
		m_pPC->m_nDOBYear = m_pParentWindow->m_pPCVWindow->m_nYear - m_pPC->m_nAge;

		//not past birthday yet ?
		if(m_pParentWindow->m_pPCVWindow->m_nMonth < m_pPC->m_nDOBMonth || (m_pParentWindow->m_pPCVWindow->m_nMonth == m_pPC->m_nDOBMonth && m_pParentWindow->m_pPCVWindow->m_nDayofMonth < m_pPC->m_nDOBDay))
		{
			m_pPC->m_nDOBYear -= 1;
		}

		Init();

		Refresh();
	}

}

void CDMPCDetailsDialog::OnEnChangePlayerNameEdit()
{
	Refresh();
}

void CDMPCDetailsDialog::OnEnChangeCharCreateDateEdit()
{
	Refresh();
}

void CDMPCDetailsDialog::OnEnChangePlayerDeityEdit()
{
	Refresh();
}

void CDMPCDetailsDialog::OnEnChangeCharOriginEdit()
{
	Refresh();
}

void CDMPCDetailsDialog::OnEnChangePlayerReligionEdit()
{
	Refresh();
}

void CDMPCDetailsDialog::OnEnChangeCharHeightEdit()
{
	Refresh();
}

void CDMPCDetailsDialog::OnEnChangeCharWeightEdit()
{
	Refresh();
}

void CDMPCDetailsDialog::OnEnChangeCharDobMonthEdit()
{
	Refresh();
}

void CDMPCDetailsDialog::OnEnChangeCharDobDayEdit()
{
	Refresh();
}

void CDMPCDetailsDialog::OnEnChangeCharAgeAdjustEdit()
{
	Refresh();
}
void CDMPCDetailsDialog::OnEnChangeCharHairColorEdit()
{
	Refresh();
}

void CDMPCDetailsDialog::OnEnChangeCharEyeColorEdit()
{
	Refresh();
}

void CDMPCDetailsDialog::OnEnChangePlayerWillEdit()
{
	Refresh();
}

void CDMPCDetailsDialog::OnEnChangeCharDobYearEdit()
{
	Refresh();
}

void CDMPCDetailsDialog::OnBnClickedNotesButton()
{
	CDMCharacterNotesDialog *pDlg = new CDMCharacterNotesDialog(m_pPC);
	pDlg->DoModal();

	delete pDlg;
}

void CDMPCDetailsDialog::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnClose();
}
