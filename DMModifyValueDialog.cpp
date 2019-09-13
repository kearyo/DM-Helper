// DMModifyValueDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMModifyValueDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DMModifyValueDialog dialog


void ModifyValue(int *_pnModifyValue, char *szLabel, LONG lAllValue, BOOL bAllowNegative, BOOL bAddValue, BOOL *pbMultiplier)
{

	DMModifyValueDialog *pDlg = new DMModifyValueDialog(_pnModifyValue, szLabel, lAllValue, bAllowNegative, bAddValue, pbMultiplier);

	pDlg->DoModal();

	delete pDlg;

}


DMModifyValueDialog::DMModifyValueDialog(int *_pnModifyValue, char *szLabel, long lAllValue, BOOL bAllowNegative, BOOL bAddValue, BOOL *pbMultiplier, CWnd* pParent /*=NULL*/)
	: CDialog(DMModifyValueDialog::IDD, pParent)
	, m_bX1Check(TRUE)
	, m_bX2Check(FALSE)
	, m_bX3Check(FALSE)
{
	//{{AFX_DATA_INIT(DMModifyValueDialog)
	m_szAddEdit = _T("");
	//}}AFX_DATA_INIT

	m_pnModifyValue = _pnModifyValue;
	m_bAllowNegative = bAllowNegative;
	m_bAddValue = bAddValue;
	m_lAllValue = lAllValue;
	m_pbMultiplier = pbMultiplier;

	if(szLabel != NULL)
	{
		m_szLabel = szLabel;
	}
	else
	{
		m_szLabel = _T("");
	}
}


void DMModifyValueDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DMModifyValueDialog)
	DDX_Control(pDX, ID_ADD, m_cSubtractButton);
	DDX_Text(pDX, IDC_ADD_EDIT, m_szAddEdit);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDOK, m_cOKButton);
	DDX_Control(pDX, IDC_BUTTON_ALL, m_cAllButton);
	DDX_Control(pDX, IDC_X1_CHECK, m_cX1Check);
	DDX_Control(pDX, IDC_X2_CHECK, m_cX2Check);
	DDX_Control(pDX, IDC_X3_CHECK, m_cX3Check);
	DDX_Check(pDX, IDC_X1_CHECK, m_bX1Check);
	DDX_Check(pDX, IDC_X2_CHECK, m_bX2Check);
	DDX_Check(pDX, IDC_X3_CHECK, m_bX3Check);
	DDX_Control(pDX, IDC_SWORD_ICON, m_cSwordIcon);
}


BEGIN_MESSAGE_MAP(DMModifyValueDialog, CDialog)
	//{{AFX_MSG_MAP(DMModifyValueDialog)
	ON_BN_CLICKED(ID_ADD, OnChange)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnButton9)
	ON_BN_CLICKED(IDC_BUTTON10, OnButton10)
	ON_BN_CLICKED(IDC_BUTTON_BACK, OnButtonBack)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_ALL, &DMModifyValueDialog::OnBnClickedButtonAll)
	ON_BN_CLICKED(IDC_X1_CHECK, &DMModifyValueDialog::OnBnClickedX1Check)
	ON_BN_CLICKED(IDC_X2_CHECK, &DMModifyValueDialog::OnBnClickedX2Check)
	ON_BN_CLICKED(IDC_X3_CHECK, &DMModifyValueDialog::OnBnClickedX3Check)
	ON_BN_CLICKED(IDOK, &DMModifyValueDialog::OnOK)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DMModifyValueDialog message handlers

BOOL DMModifyValueDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();

	if(m_szLabel != _T(""))
	{
		SetWindowText(m_szLabel);
	}

	m_cSubtractButton.EnableWindow(m_bAllowNegative);


	if(m_bAddValue == FALSE)
	{
		m_cOKButton.SetWindowText("Enter");
		m_cSubtractButton.ShowWindow(SW_HIDE);
	}

	if(m_lAllValue == 0L)
	{
		m_cAllButton.ShowWindow(SW_HIDE);
	}

	if (m_pbMultiplier == NULL)
	{
		m_cX1Check.ShowWindow(SW_HIDE);
		m_cX2Check.ShowWindow(SW_HIDE);
		m_cX3Check.ShowWindow(SW_HIDE);
		m_cSwordIcon.ShowWindow(SW_SHOW);
	}
	else
	{
		m_cSwordIcon.ShowWindow(SW_HIDE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DMModifyValueDialog::OnOK() //add the values
{
	UpdateData(TRUE);

	int nAdd = atoi(m_szAddEdit.GetBuffer(0));

	if(m_bAllowNegative == FALSE && nAdd < 0)
	{
		nAdd = -nAdd;
	}

	if (m_bX2Check)
	{
		nAdd *= 2;
		*m_pbMultiplier = TRUE;
	}
	else if (m_bX3Check)
	{
		nAdd *= 3;
		*m_pbMultiplier = TRUE;
	}

	*m_pnModifyValue += nAdd;
	
	CDialog::OnOK();
}

void DMModifyValueDialog::OnChange() //subtract the values
{
	UpdateData(TRUE);

	int nSubtract = atoi(m_szAddEdit.GetBuffer(0));

	*m_pnModifyValue -= nSubtract;

	PostMessage(WM_CLOSE);
}


void DMModifyValueDialog::OnCancel() 
{
	CDialog::OnCancel();
}


void DMModifyValueDialog::OnButton1() 
{
	m_szAddEdit += "1";
	UpdateData(FALSE);
}

void DMModifyValueDialog::OnButton2() 
{
	m_szAddEdit += "2";
	UpdateData(FALSE);
	
}

void DMModifyValueDialog::OnButton3() 
{

	m_szAddEdit += "3";
	UpdateData(FALSE);
}

void DMModifyValueDialog::OnButton4() 
{
	m_szAddEdit += "4";
	UpdateData(FALSE);
}

void DMModifyValueDialog::OnButton5() 
{
	m_szAddEdit += "5";
	UpdateData(FALSE);
}

void DMModifyValueDialog::OnButton6() 
{

	m_szAddEdit += "6";
	UpdateData(FALSE);
}

void DMModifyValueDialog::OnButton7() 
{
	m_szAddEdit += "7";
	UpdateData(FALSE);

}

void DMModifyValueDialog::OnButton8() 
{
	m_szAddEdit += "8";
	UpdateData(FALSE);
}

void DMModifyValueDialog::OnButton9() 
{
	m_szAddEdit += "9";
	UpdateData(FALSE);
}

void DMModifyValueDialog::OnButton10() 
{
	m_szAddEdit += "0";
	UpdateData(FALSE);
}

void DMModifyValueDialog::OnButtonBack() 
{
	
	UpdateData(TRUE);

	CString szTemp = m_szAddEdit;

	int nLen = szTemp.GetLength();

	nLen -= 1;

	if(nLen > 0)
		m_szAddEdit = szTemp.Left(nLen);
	else
		m_szAddEdit = _T("");

	UpdateData(FALSE);
	
}

void DMModifyValueDialog::OnBnClickedButtonAll()
{
	m_szAddEdit.Format("%ld", m_lAllValue);

	UpdateData(FALSE);

	OnOK();
}


void DMModifyValueDialog::OnBnClickedX1Check()
{
	m_bX1Check = TRUE;
	m_bX2Check = FALSE;
	m_bX3Check = FALSE;

	UpdateData(FALSE);
}


void DMModifyValueDialog::OnBnClickedX2Check()
{
	m_bX1Check = FALSE;
	m_bX2Check = TRUE;
	m_bX3Check = FALSE;

	UpdateData(FALSE);
}


void DMModifyValueDialog::OnBnClickedX3Check()
{
	m_bX1Check = FALSE;
	m_bX2Check = FALSE;
	m_bX3Check = TRUE;

	UpdateData(FALSE);
}


