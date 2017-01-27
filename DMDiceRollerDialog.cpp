// DMDiceRollerDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DM HelperDlg.h"
#include "DMModifyValueDialog.h"
#include "DMDiceRollerDialog.h"


// CDMDiceRollerDialog dialog

IMPLEMENT_DYNAMIC(CDMDiceRollerDialog, CDialog)

CDMDiceRollerDialog::CDMDiceRollerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDMDiceRollerDialog::IDD, pParent)
	, m_szNumDiceEdit(_T("1"))
	, m_szResultStatic(_T("=    "))
{
	Create(CDMDiceRollerDialog::IDD);

	m_pDMHelperDlg = (CDMHelperDlg *)pParent;
}

CDMDiceRollerDialog::~CDMDiceRollerDialog()
{
}

void CDMDiceRollerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_D20_BUTTON, m_cD20Button);
	DDX_Text(pDX, IDC_NUM_DICE_EDIT, m_szNumDiceEdit);
	DDX_Text(pDX, IDC_RESULT_STATIC, m_szResultStatic);
	DDX_Control(pDX, IDC_D12_BUTTON, m_cD12Button);
	DDX_Control(pDX, IDC_D10_BUTTON, m_cD10Button);
	DDX_Control(pDX, IDC_D6_BUTTON, m_cD6Button);
	DDX_Control(pDX, IDC_D4_BUTTON2, m_cD4Button);
	DDX_Control(pDX, IDC_D100_BUTTON, m_cD100Button);
	DDX_Control(pDX, IDC_D8_BUTTON, m_cD8Button);
	DDX_Control(pDX, IDC_DX_BUTTON, m_cDXButton);
}


BEGIN_MESSAGE_MAP(CDMDiceRollerDialog, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, &CDMDiceRollerDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDMDiceRollerDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_D20_BUTTON, &CDMDiceRollerDialog::OnBnClickedD20Button)
	ON_BN_CLICKED(IDC_D12_BUTTON, &CDMDiceRollerDialog::OnBnClickedD12Button)
	ON_BN_CLICKED(IDC_D10_BUTTON, &CDMDiceRollerDialog::OnBnClickedD10Button)
	ON_BN_CLICKED(IDC_D6_BUTTON, &CDMDiceRollerDialog::OnBnClickedD6Button)
	ON_BN_CLICKED(IDC_D4_BUTTON2, &CDMDiceRollerDialog::OnBnClickedD4Button2)
	ON_BN_CLICKED(IDC_D100_BUTTON, &CDMDiceRollerDialog::OnBnClickedD100Button)
	ON_BN_CLICKED(IDC_D8_BUTTON, &CDMDiceRollerDialog::OnBnClickedD8Button)
	ON_BN_CLICKED(IDC_DX_BUTTON, &CDMDiceRollerDialog::OnBnClickedDxButton)
END_MESSAGE_MAP()


// CDMDiceRollerDialog message handlers

BOOL CDMDiceRollerDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cD20Button.SetBitmap((::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_D20_BITMAP))));
	m_cD12Button.SetBitmap((::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_D12_BITMAP))));
	m_cD10Button.SetBitmap((::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_D10_BITMAP))));
	m_cD8Button.SetBitmap((::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_D8_BITMAP))));
	m_cD6Button.SetBitmap((::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_D6_BITMAP))));
	m_cD4Button.SetBitmap((::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_D4_BITMAP))));
	m_cD100Button.SetBitmap((::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_D100_BITMAP))));
	m_cDXButton.SetBitmap((::LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_DX_BITMAP))));

	ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDMDiceRollerDialog::PostNcDestroy()
{
	CDialog::PostNcDestroy();

	m_pDMHelperDlg->m_pDiceRollerDialog = NULL;

	delete this;
}

void CDMDiceRollerDialog::OnClose()
{
	//CDialog::OnClose();

	m_pDMHelperDlg->m_pDiceRollerDialog = NULL;

	//DestroyWindow();

	delete this;

}

void CDMDiceRollerDialog::OnBnClickedOk()
{
	m_pDMHelperDlg->m_pDiceRollerDialog = NULL;

	OnOK();
}

void CDMDiceRollerDialog::OnBnClickedCancel()
{
	m_pDMHelperDlg->m_pDiceRollerDialog = NULL;

	OnCancel();
}

void CDMDiceRollerDialog::RollDice(int nDieType)
{
	UpdateData(TRUE);

	int nNumDice = atoi(m_szNumDiceEdit.GetBuffer(0));

	if(nNumDice < 1)
		nNumDice = 1;

	if(nNumDice > 100)
		nNumDice = 100;

	int nResult = 0;

	for(int i = 0; i < nNumDice; ++i)
	{
		int nDieRoll = (rand() % nDieType) + 1;

		nResult += nDieRoll;
	}

	m_szResultStatic.Format("= %d", nResult);

	m_szNumDiceEdit.Format("%d", nNumDice);

	UpdateData(FALSE);

}

void CDMDiceRollerDialog::OnBnClickedD20Button()
{
	RollDice(20);
}

void CDMDiceRollerDialog::OnBnClickedD12Button()
{
	RollDice(12);
}

void CDMDiceRollerDialog::OnBnClickedD10Button()
{
	RollDice(10);
}

void CDMDiceRollerDialog::OnBnClickedD8Button()
{
	RollDice(8);
}

void CDMDiceRollerDialog::OnBnClickedD6Button()
{
	RollDice(6);
}

void CDMDiceRollerDialog::OnBnClickedD4Button2()
{
	RollDice(4);
}

void CDMDiceRollerDialog::OnBnClickedD100Button()
{
	RollDice(100);
}



void CDMDiceRollerDialog::OnBnClickedDxButton()
{
	int nDieValue = 0;
	ModifyValue((int *)&nDieValue, "Maximum Roll of This Die:", FALSE, FALSE);


	if(nDieValue < 1)
		nDieValue = 1;

	if(nDieValue > 32766)
		nDieValue = 32766;

	RollDice(nDieValue);

}
