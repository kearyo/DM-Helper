// DMMundaneTypeSelectorDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMMundaneTypeSelectorDialog.h"


// cDMMundaneTypeSelectorDialog dialog

int nPickButtons[] = 
{
	IDC_TYPE_RADIO1,
	IDC_TYPE_RADIO2,
	IDC_TYPE_RADIO3,
	IDC_TYPE_RADIO4, 
	IDC_TYPE_RADIO5, 
	IDC_TYPE_RADIO6, 
	IDC_TYPE_RADIO7, 
	IDC_TYPE_RADIO8,
	IDC_TYPE_RADIO9, 
	IDC_TYPE_RADIO10
};

IMPLEMENT_DYNAMIC(cDMMundaneTypeSelectorDialog, CDialog)

cDMMundaneTypeSelectorDialog::cDMMundaneTypeSelectorDialog(PMAGICTABLEITEM pMagicItem, int *pnSelected, int *pnMagicAdjustment, int *pnAmount, CWnd* pParent /*=NULL*/)
	: CDialog(cDMMundaneTypeSelectorDialog::IDD, pParent)
	, m_szAmountEdit(_T("1"))
{

	m_pMagicItem = pMagicItem;

	m_pnSelected = pnSelected;

	m_pnMagicAdjustment = pnMagicAdjustment;

	m_pnAmount = pnAmount;
		
}

cDMMundaneTypeSelectorDialog::~cDMMundaneTypeSelectorDialog()
{
}

void cDMMundaneTypeSelectorDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_AMOUNT_EDIT, m_szAmountEdit);
	DDV_MaxChars(pDX, m_szAmountEdit, 3);
}


BEGIN_MESSAGE_MAP(cDMMundaneTypeSelectorDialog, CDialog)
	ON_BN_CLICKED(IDOK, &cDMMundaneTypeSelectorDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &cDMMundaneTypeSelectorDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_TYPE_RADIO1, &cDMMundaneTypeSelectorDialog::OnBnClickedTypeRadio1)
	ON_BN_CLICKED(IDC_TYPE_RADIO2, &cDMMundaneTypeSelectorDialog::OnBnClickedTypeRadio2)
	ON_BN_CLICKED(IDC_TYPE_RADIO3, &cDMMundaneTypeSelectorDialog::OnBnClickedTypeRadio3)
	ON_BN_CLICKED(IDC_TYPE_RADIO4, &cDMMundaneTypeSelectorDialog::OnBnClickedTypeRadio4)
	ON_BN_CLICKED(IDC_TYPE_RADIO5, &cDMMundaneTypeSelectorDialog::OnBnClickedTypeRadio5)
	ON_BN_CLICKED(IDC_TYPE_RADIO6, &cDMMundaneTypeSelectorDialog::OnBnClickedTypeRadio6)
	ON_BN_CLICKED(IDC_TYPE_RADIO7, &cDMMundaneTypeSelectorDialog::OnBnClickedTypeRadio7)
	ON_BN_CLICKED(IDC_TYPE_RADIO8, &cDMMundaneTypeSelectorDialog::OnBnClickedTypeRadio8)
	ON_BN_CLICKED(IDC_TYPE_RADIO9, &cDMMundaneTypeSelectorDialog::OnBnClickedTypeRadio9)
	ON_BN_CLICKED(IDC_TYPE_RADIO10, &cDMMundaneTypeSelectorDialog::OnBnClickedTypeRadio10)
	ON_BN_CLICKED(IDC_ROLL_BUTTON, &cDMMundaneTypeSelectorDialog::OnBnClickedRollButton)
END_MESSAGE_MAP()


// cDMMundaneTypeSelectorDialog message handlers

BOOL cDMMundaneTypeSelectorDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString szTemp = _T("");

	if(m_pnAmount != NULL)
	{
		SetWindowText("Enter amount of magic item:");
		for(int i = 0; i < 10; ++i)
		{
			(GetDlgItem( nPickButtons[i] ))-> ShowWindow(SW_HIDE);
		}
	}
	else
	{
		(GetDlgItem( IDC_AMOUNT_EDIT ))-> ShowWindow(SW_HIDE);
		(GetDlgItem( IDC_ROLL_BUTTON ))-> ShowWindow(SW_HIDE);

		for(int i = 0; i < 10; ++i)
		{
			if(m_pnMagicAdjustment != NULL)
			{
				if(i < m_pMagicItem->m_nMagicAdjustmentHigh)
				{
					szTemp.Format("%s +%d", m_pMagicItem->m_szDesc, i+1);
					(GetDlgItem( nPickButtons[i] ))-> SetWindowText(szTemp);
				}
				else
				{
					(GetDlgItem( nPickButtons[i] ))-> ShowWindow(SW_HIDE);
				}
			}
			else if(m_pnSelected != NULL)
			{
				if(m_pMagicItem->m_pMundaneObject[i] != NULL)
				{
					CString szAdd = _T("");
					CString szType = m_pMagicItem->m_pMundaneObject[i]->m_szType;
					szType.MakeLower();

					if (szType.Find("leather") >= 0)			szAdd = _T(" (AC 8)");
					if (szType.Find("studded leather") >= 0)	szAdd = _T(" (AC 7)");
					if (szType.Find("scale mail") >= 0)			szAdd = _T(" (AC 6)");
					if (szType.Find("chain mail") >= 0)			szAdd = _T(" (AC 5)");
					if (szType.Find("splint mail") >= 0)		szAdd = _T(" (AC 4)");
					if (szType.Find("plate mail") >= 0)			szAdd = _T(" (AC 3)");
					if (szType.Find("field plate") >= 0)		szAdd = _T(" (AC 2)");

					szType += szAdd;

					(GetDlgItem(nPickButtons[i]))->SetWindowText(szType);
				}
				else
				{
					(GetDlgItem( nPickButtons[i] ))-> ShowWindow(SW_HIDE);
				}
			}
			else
			{
				(GetDlgItem( nPickButtons[i] ))-> ShowWindow(SW_HIDE);
			}
		}
	}

	//ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void cDMMundaneTypeSelectorDialog::OnBnClickedOk()
{
	UpdateData(TRUE);

	if(m_pnAmount != NULL)
	{
		*m_pnAmount = atoi(m_szAmountEdit.GetBuffer(0));
	}

	OnOK();
}

void cDMMundaneTypeSelectorDialog::OnBnClickedCancel()
{
	if(m_pnSelected != NULL)
	{
		*m_pnSelected = -1;
	}

	if(m_pnMagicAdjustment != NULL)
	{
		*m_pnMagicAdjustment = 0;
	}

	if(m_pnAmount != NULL)
	{
		*m_pnAmount = -1;
	}

	OnCancel();
}

void cDMMundaneTypeSelectorDialog::OnBnClickedTypeRadio1()
{
	if(m_pnSelected != NULL)
	{
		*m_pnSelected = 0;
	}

	if(m_pnMagicAdjustment != NULL)
	{
		*m_pnMagicAdjustment = 1;
	}
}

void cDMMundaneTypeSelectorDialog::OnBnClickedTypeRadio2()
{
	if(m_pnSelected != NULL)
	{
		*m_pnSelected = 1;
	}

	if(m_pnMagicAdjustment != NULL)
	{
		*m_pnMagicAdjustment = 2;
	}
}

void cDMMundaneTypeSelectorDialog::OnBnClickedTypeRadio3()
{
	if(m_pnSelected != NULL)
	{
		*m_pnSelected = 2;
	}

	if(m_pnMagicAdjustment != NULL)
	{
		*m_pnMagicAdjustment = 3;
	}
}

void cDMMundaneTypeSelectorDialog::OnBnClickedTypeRadio4()
{
	if(m_pnSelected != NULL)
	{
		*m_pnSelected = 3;
	}

	if(m_pnMagicAdjustment != NULL)
	{
		*m_pnMagicAdjustment = 4;
	}
}

void cDMMundaneTypeSelectorDialog::OnBnClickedTypeRadio5()
{
	if(m_pnSelected != NULL)
	{
		*m_pnSelected = 4;
	}

	if(m_pnMagicAdjustment != NULL)
	{
		*m_pnMagicAdjustment = 5;
	}
}

void cDMMundaneTypeSelectorDialog::OnBnClickedTypeRadio6()
{
	if(m_pnSelected != NULL)
	{
		*m_pnSelected = 5;
	}

	if(m_pnMagicAdjustment != NULL)
	{
		*m_pnMagicAdjustment = 6;
	}
}

void cDMMundaneTypeSelectorDialog::OnBnClickedTypeRadio7()
{
	if(m_pnSelected != NULL)
	{
		*m_pnSelected = 6;
	}

	if(m_pnMagicAdjustment != NULL)
	{
		*m_pnMagicAdjustment = 7;
	}
}

void cDMMundaneTypeSelectorDialog::OnBnClickedTypeRadio8()
{
	if(m_pnSelected != NULL)
	{
		*m_pnSelected = 7;
	}

	if(m_pnMagicAdjustment != NULL)
	{
		*m_pnMagicAdjustment = 8;
	}
}

void cDMMundaneTypeSelectorDialog::OnBnClickedTypeRadio9()
{
	if(m_pnSelected != NULL)
	{
		*m_pnSelected = 8;
	}

	if(m_pnMagicAdjustment != NULL)
	{
		*m_pnMagicAdjustment = 9;
	}
}

void cDMMundaneTypeSelectorDialog::OnBnClickedTypeRadio10()
{
	if(m_pnSelected != NULL)
	{
		*m_pnSelected = 9;
	}

	if(m_pnMagicAdjustment != NULL)
	{
		*m_pnMagicAdjustment = 10;
	}
}

void cDMMundaneTypeSelectorDialog::OnBnClickedRollButton()
{
	if(m_pMagicItem->m_nAmountHigh > m_pMagicItem->m_nAmountLow && m_pnAmount != NULL)
	{
		int nDiff = (m_pMagicItem->m_nAmountHigh - m_pMagicItem->m_nAmountLow) + 1;

		int nRoll = rand() % nDiff;
		nRoll += m_pMagicItem->m_nAmountLow;

		*m_pnAmount = nRoll;

		m_szAmountEdit.Format("%d", *m_pnAmount);

		UpdateData(FALSE);
	}
}
