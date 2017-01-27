// DMLanguageSelectDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DM HelperDlg.h"
#include "DMLoadFileDialog.h"
#include "DMInventoryDialog.h"
#include "DMCharSpellsDialog.h"
#include "DMCharViewDialog.h"
#include "DMLanguageSelectDialog.h"


// CDMLanguageSelectDialog dialog

IMPLEMENT_DYNAMIC(CDMLanguageSelectDialog, CDialog)

CDMLanguageSelectDialog::CDMLanguageSelectDialog(cDNDObject* pObject, CWnd* pParent /*=NULL*/)
	: CDialog(CDMLanguageSelectDialog::IDD, pParent)
{
	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_pObject = pObject;
	m_pParentCharView = (CDMCharViewDialog *)pParent;
}

CDMLanguageSelectDialog::~CDMLanguageSelectDialog()
{
}

void CDMLanguageSelectDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LANGUAGE_LIST, m_cLanguageList);
}


BEGIN_MESSAGE_MAP(CDMLanguageSelectDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CDMLanguageSelectDialog::OnBnClickedOk)
END_MESSAGE_MAP()


// CDMLanguageSelectDialog message handlers

BOOL CDMLanguageSelectDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	Refresh();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDMLanguageSelectDialog::OnBnClickedOk()
{
	UpdateData(TRUE);

	if(m_pParentCharView != NULL)
	{
		memset(m_pParentCharView->m_pCharacter->m_nLanguages, 0, MAX_CHARACTER_LANGUAGES * sizeof(int));
	}
	if(m_pObject != NULL)
	{
		memset(m_pObject->m_nContents, 0, MAX_CHARACTER_LANGUAGES * sizeof(int));
	}

	int nCount = 0;

	for(int i = 0; i < m_cLanguageList.GetCount() && nCount < MAX_CHARACTER_LANGUAGES; ++ i)
	{
		if(m_cLanguageList.GetSel(i))
		{
			cDNDLanguage *pLanguage = (cDNDLanguage *)m_cLanguageList.GetItemData(i);

			if(m_pParentCharView != NULL)
			{
				m_pParentCharView->m_pCharacter->m_nLanguages[nCount++] = pLanguage->m_nIndex;
			}
			if(m_pObject != NULL)
			{
				m_pObject->m_nContents[nCount++] = pLanguage->m_nIndex;
			}
		}
	}

	if(m_pParentCharView != NULL)
	{
		m_pParentCharView->m_pCharacter->MarkChanged();
	}

	OnOK();
}

void CDMLanguageSelectDialog::Refresh()
{
	m_cLanguageList.ResetContent();

	int nCount = 0;
	for (int i = 0; i < m_pApp->m_LanguageArray.GetSize(); ++i )
	{
		cDNDLanguage *pLanguage = (cDNDLanguage *)m_pApp->m_LanguageArray.GetAt(i);

		if(pLanguage != NULL)
		{
			if(pLanguage->m_nList)
			{
				m_cLanguageList.InsertString(nCount, pLanguage->m_szLanguageName);
				m_cLanguageList.SetItemData(nCount, (DWORD_PTR)pLanguage);

				if(m_pParentCharView != NULL)
				{
					if(m_pParentCharView->m_pCharacter->CanSpeakLanguage((DND_LANGUAGES)pLanguage->m_nIndex))
					{
						m_cLanguageList.SetSel(nCount, 1);
					}
				}
				if(m_pObject != NULL)
				{
					if(m_pObject->CanSpeakLanguage(pLanguage->m_nIndex))
					{
						m_cLanguageList.SetSel(nCount, 1);
					}
				}

				++nCount;
			}
		}
	}


	UpdateData(FALSE);
}
