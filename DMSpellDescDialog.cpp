// DMSpellDescDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DMSpellDescDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DMSpellDescDialog dialog


DMSpellDescDialog::DMSpellDescDialog(cDNDSpell *_pSpell, CWnd* pParent /*=NULL*/)
	: CDialog(DMSpellDescDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(DMSpellDescDialog)
	m_szSpellDesc = _T("");
	m_szDesc1 = _T("");
	m_szDesc2 = _T("");
	m_szDesc3 = _T("");
	m_szDesc4 = _T("");
	m_szDesc5 = _T("");
	m_szDesc6 = _T("");
	//}}AFX_DATA_INIT

	m_pSpell = _pSpell;
}


void DMSpellDescDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DMSpellDescDialog)
	DDX_Control(pDX, IDC_FAKE_EDIT, m_cFakeEdit);
	DDX_Control(pDX, IDC_SPELL_DESC_EDIT, m_cSpellEditBox);
	DDX_Text(pDX, IDC_SPELL_DESC_EDIT, m_szSpellDesc);
	DDX_Text(pDX, IDC_DESC_1, m_szDesc1);
	DDX_Text(pDX, IDC_DESC_2, m_szDesc2);
	DDX_Text(pDX, IDC_DESC_3, m_szDesc3);
	DDX_Text(pDX, IDC_DESC_4, m_szDesc4);
	DDX_Text(pDX, IDC_DESC_5, m_szDesc5);
	DDX_Text(pDX, IDC_DESC_6, m_szDesc6);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DMSpellDescDialog, CDialog)
	//{{AFX_MSG_MAP(DMSpellDescDialog)
	ON_EN_CHANGE(IDC_SPELL_DESC_EDIT, OnChangeSpellDescEdit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DMSpellDescDialog message handlers

BOOL DMSpellDescDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString szTemp;

	szTemp.Format("Spell: %s", m_pSpell->m_szSpellName);

	SetWindowText(szTemp);

	m_szSpellDesc = m_pSpell->m_szSpellDesc;
	
	m_szSpellDesc.Replace("\r", "");
	m_szSpellDesc.Replace("\n", "");

	m_szSpellDesc.Replace("^", "\r\n");

	FindReferencedSpell();

	m_szDesc1.Format("Components: %s", m_pSpell->m_szSpellComponents);
	m_szDesc2.Format("Range: %s", m_pSpell->m_szSpellRange);
	m_szDesc3.Format("Casting Time: %s", m_pSpell->m_szSpellCastingTime);
	m_szDesc4.Format("Duration: %s", m_pSpell->m_szSpellDuration);
	m_szDesc5.Format("Save: %s", m_pSpell->m_szSpellSavingThrow);
	m_szDesc6.Format("Area of Effect: %s", m_pSpell->m_szSpellAreaOfEffect);

	m_cFakeEdit.SetFocus();

	UpdateData(FALSE);

	m_cFakeEdit.SetWindowPos(NULL, 995, 995, 10, 10, SWP_SHOWWINDOW); 
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void DMSpellDescDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void DMSpellDescDialog::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void DMSpellDescDialog::OnChangeSpellDescEdit() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	
	
}

void DMSpellDescDialog::FindReferencedSpell() //finds spells that the Player Handbook description is "this spell is the same as that spell"  argghhhh
{
	if(m_szSpellDesc.Find("same as the") == -1)
		return;

	CDMHelperApp *pApp = (CDMHelperApp *)AfxGetApp();

	CString szSpellName = m_pSpell->m_szSpellName;

	CString szCurrentDesc = _T("");

	BOOL bFoundOne = FALSE;

	for (int i = 0; i < pApp->m_SpellBooks.GetSize() && bFoundOne == FALSE; ++i )
	{
		PSPELLBOOK pSpellBook = (PSPELLBOOK)pApp->m_SpellBooks.GetAt(i);

		if(pSpellBook != NULL)
		{
			for(int nLevel = 1; nLevel <= pSpellBook->m_nMaxLevel && bFoundOne == FALSE; ++nLevel)
			{
				for(int nSpell = 0; nSpell < MAX_SPELLS_PER_LEVEL && bFoundOne == FALSE; ++nSpell)
				{
					if(pSpellBook->m_Spells[nLevel][nSpell].m_nSpellIdentifier == m_pSpell->m_nSpellIdentifier)
						break;

					CString szSpellCheck = pSpellBook->m_Spells[nLevel][nSpell].m_szSpellName;

					if(szSpellCheck == szSpellName)
					{
						szCurrentDesc = pSpellBook->m_Spells[nLevel][nSpell].m_szSpellDesc;

						szCurrentDesc.Replace("\r", "");
						szCurrentDesc.Replace("\n", "");

						szCurrentDesc.Replace("^", "\r\n");

						if(szCurrentDesc.Find("same as the") >= 0)
							continue;

						bFoundOne = TRUE;
						break;
					}
					
				}
			}
			
		}
	}

	if(bFoundOne)
	{
		m_szSpellDesc += _T("\r\n\r\nReferenced spell:\r\n");
		m_szSpellDesc += szCurrentDesc;
	}

}
