// DMEventLoggerDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "DM HelperDlg.h"
#include "DMLoadFileDialog.h"
#include "DMCharSpellsDialog.h"
#include "DMInventoryDialog.h"
#include "DMCharViewDialog.h"
#include "DMNPCViewDialog.h"
#include "cDMMapViewDialog.h"
#include "DMCharacterSelectorDialog.h"
#include "DMCastSpellDialog.h"
#include "DMModifyValueDialog.h"
#include "DMTimeEditDialog.h"
#include "DMPartyDialog.h"
#include "DMEventLoggerDialog.h"


// CDMEventLoggerDialog dialog

IMPLEMENT_DYNAMIC(CDMEventLoggerDialog, CDialog)

CDMEventLoggerDialog::CDMEventLoggerDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDMEventLoggerDialog::IDD, pParent)
	, m_szComment(_T(""))
	, m_szEventTarget(_T(""))
	, m_szEventValueLabel(_T(""))
	, m_szEventValue(_T(""))
{

	m_pApp = (CDMHelperApp *)AfxGetApp();

	m_pParentWindow = (DMPartyDialog *)pParent;

	m_nEventType = DND_LOG_EVENT_TYPE_UNDEFINED;
	
	m_bNeedCharList = FALSE;
	m_bNeedEventValue = FALSE;

	m_dwSelectedCharacterID = 0;
	m_pSelectedCharacter = NULL;
	m_lAmount = 0L;

	m_nInsertPosition = APPEND_TO_LOG;


	Create(CDMEventLoggerDialog::IDD);
}

CDMEventLoggerDialog::~CDMEventLoggerDialog()
{
}

void CDMEventLoggerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EVENT_TYPE_COMBO, m_cEventTypeCombo);
	DDX_Text(pDX, IDC_COMMENT_EDIT, m_szComment);
	DDX_Control(pDX, IDOK, m_cLogEventButton);
	DDX_Control(pDX, IDC_EVENT_TARGET_COMBO, m_cEventTargetCombo);
	DDX_Text(pDX, IDC_EVENT_TARGET_STATIC, m_szEventTarget);
	DDX_Control(pDX, IDC_EVENT_VALUE_LABEL, m_cEventValueLabel);
	DDX_Text(pDX, IDC_EVENT_VALUE_LABEL, m_szEventValueLabel);
	DDX_Control(pDX, IDC_XP_EDIT, m_cEventValueEdit);
	DDX_Text(pDX, IDC_XP_EDIT, m_szEventValue);
	DDV_MaxChars(pDX, m_szComment, 127);
}


BEGIN_MESSAGE_MAP(CDMEventLoggerDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CDMEventLoggerDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDMEventLoggerDialog::OnBnClickedCancel)
	ON_WM_CLOSE()
	ON_CBN_SELCHANGE(IDC_EVENT_TYPE_COMBO, &CDMEventLoggerDialog::OnCbnSelchangeEventTypeCombo)
	ON_CBN_SELCHANGE(IDC_EVENT_TARGET_COMBO, &CDMEventLoggerDialog::OnCbnSelchangeEventTargetCombo)
END_MESSAGE_MAP()


// CDMEventLoggerDialog message handlers


BOOL CDMEventLoggerDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	int nCount = 0;
	for(int i = DND_LOG_EVENT_TYPE_UNDEFINED+1; i < DND_LOG_EVENT_TYPE_LAST_EVENT_TYPE; ++i)
	{
		m_cEventTypeCombo.InsertString(nCount, GetLogEventName( (DND_LOG_EVENT_TYPES)i));
		m_cEventTypeCombo.SetItemData(nCount++, i);
	}

	m_cEventTargetCombo.EnableWindow(FALSE);
	m_cEventValueEdit.EnableWindow(FALSE);

	ShowWindow(SW_SHOW);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDMEventLoggerDialog::OnBnClickedOk()
{
	UpdateData(TRUE);

	CString szName = _T("");

	//validate our pointers
	if(m_dwSelectedCharacterID)
	{
		PDNDCHARVIEWDLG pCharDlg = NULL;
		m_pApp->m_CharacterViewMap.Lookup((WORD)m_dwSelectedCharacterID, pCharDlg);
		if(pCharDlg == NULL)
		{
			PDNDNPCVIEWDLG pNPCDlg =  NULL;
			m_pApp->m_NPCViewMap.Lookup((WORD)m_dwSelectedCharacterID, pNPCDlg);

			if(pNPCDlg == NULL)
			{
				//our character must have went away !
				Init();
				return;
			}
		}
	}

	if(m_pSelectedCharacter != NULL)
	{
		szName = m_pSelectedCharacter->m_szCharacterName;
	}

	m_lAmount = atol(m_szEventValue.GetBuffer(0));

	m_pParentWindow->LogPartyEvent(TRUE, m_nInsertPosition, m_nEventType, szName.GetBuffer(0), m_dwSelectedCharacterID, m_lAmount, m_szComment.GetBuffer(0));

	m_nInsertPosition = APPEND_TO_LOG;

	Init();

}

void CDMEventLoggerDialog::OnBnClickedCancel()
{
	OnCancel();

	m_pParentWindow->m_pEventLoggerDialog = NULL;

	delete this;
}

void CDMEventLoggerDialog::OnClose()
{
	CDialog::OnClose();

	m_pParentWindow->m_pEventLoggerDialog = NULL;

	delete this;
}

void CDMEventLoggerDialog::OnCbnSelchangeEventTypeCombo()
{
	Init();
}

void CDMEventLoggerDialog::OnCbnSelchangeEventTargetCombo()
{
	UpdateData(TRUE);

	int nCursor = m_cEventTargetCombo.GetCurSel();

	if(nCursor > -1)
	{
		if(m_bNeedCharList)
		{
			m_pSelectedCharacter = (cDNDCharacter *)m_cEventTargetCombo.GetItemData(nCursor);
			m_dwSelectedCharacterID = m_pSelectedCharacter->m_dwCharacterID;
			Refresh();
			return;
		}
	}
}


void CDMEventLoggerDialog::Init()
{
	UpdateData(TRUE);

	if(m_nInsertPosition == APPEND_TO_LOG)
	{
		m_cLogEventButton.SetWindowText("Log Event");
	}
	else
	{
		m_cLogEventButton.SetWindowText("Insert Event");
	}

	m_nEventType = DND_LOG_EVENT_TYPE_UNDEFINED;

	m_pSelectedCharacter = NULL;
	m_dwSelectedCharacterID = 0;

	m_bNeedCharList = FALSE;
	m_bNeedEventValue = FALSE;

	m_cEventTargetCombo.EnableWindow(FALSE);
	m_cEventValueEdit.EnableWindow(FALSE);

	m_szEventTarget = _T("");
	m_szEventValueLabel = _T("");
	m_szEventValue = _T("");
	m_szComment = _T("");

	int nCursor = m_cEventTypeCombo.GetCurSel();

	if(nCursor > -1)
	{
		m_nEventType = (DND_LOG_EVENT_TYPES)m_cEventTypeCombo.GetItemData(nCursor);

		switch(m_nEventType)
		{
			case DND_LOG_EVENT_TYPE_CHARACTER_CAST_SPELL:
			case DND_LOG_EVENT_TYPE_CHARACTER_DIED:
			case DND_LOG_EVENT_TYPE_CHARACTER_RAISED:
			case DND_LOG_EVENT_TYPE_CHARACTER_JOINED_PARTY:
			case DND_LOG_EVENT_TYPE_CHARACTER_LEFT_PARTY:
			case DND_LOG_EVENT_TYPE_CHARACTER_GAINED_LEVEL:
			case DND_LOG_EVENT_TYPE_CHARACTER_LOST_LEVEL:
			{
				m_bNeedCharList = TRUE;
				break;
			}
			
			case DND_LOG_EVENT_TYPE_CHARACTER_GAINED_XP:
			case DND_LOG_EVENT_TYPE_CHARACTER_GAINED_XP_COMBAT:
			case DND_LOG_EVENT_TYPE_CHARACTER_GAINED_XP_CLASS_SKILL:
			case DND_LOG_EVENT_TYPE_CHARACTER_GAINED_XP_ROLEPLAY:
			case DND_LOG_EVENT_TYPE_CHARACTER_GAINED_XP_TREASURE:
			case DND_LOG_EVENT_TYPE_CHARACTER_LOST_XP:
			{
				m_szEventValueLabel = _T("XP:");
				m_bNeedCharList = TRUE;
				m_bNeedEventValue = TRUE;
				break;
			}

			case DND_LOG_EVENT_TYPE_PARTY_GAINED_XP:
			case DND_LOG_EVENT_TYPE_PARTY_GAINED_XP_COMBAT:
			case DND_LOG_EVENT_TYPE_PARTY_GAINED_XP_TREASURE:
			case DND_LOG_EVENT_TYPE_PARTY_LOST_XP:
			{
				m_szEventValueLabel = _T("XP:");
				m_bNeedEventValue = TRUE;
				break;
			}

			case DND_LOG_EVENT_TYPE_PARTY_TRAVELED:
			{
				m_szEventValueLabel = _T("Miles:");
				m_bNeedEventValue = TRUE;
				break;
			}
		}

		if(m_bNeedCharList)
		{
			m_szEventTarget = _T("Character:");
			m_cEventTargetCombo.EnableWindow(TRUE);
			PopulateCharacterList();
		}
		if(m_bNeedEventValue)
		{
			m_cEventValueEdit.EnableWindow(TRUE);
		}
	}

	Refresh();
}

void CDMEventLoggerDialog::PopulateCharacterList()
{
	m_cEventTargetCombo.ResetContent();

	int nRow = 0;
	WORD wID;

	for (POSITION pos = m_pApp->m_CharacterViewMap.GetStartPosition(); pos != NULL; )
	{
		PDNDCHARVIEWDLG pCharDlg = NULL;
	
		m_pApp->m_CharacterViewMap.GetNextAssoc(pos,wID,pCharDlg);

		if(pCharDlg != NULL && pCharDlg->m_pCharacter != NULL && pCharDlg->m_pCharacter->m_dwCharacterID)
		{
			m_cEventTargetCombo.InsertString(nRow, pCharDlg->m_pCharacter->m_szCharacterName);
			m_cEventTargetCombo.SetItemData(nRow, (DWORD)pCharDlg->m_pCharacter);
			++nRow;
		}
	}

	for (pos = m_pApp->m_NPCViewMap.GetStartPosition(); pos != NULL; )
	{
		PDNDNPCVIEWDLG pNPCDlg = NULL;

		m_pApp->m_NPCViewMap.GetNextAssoc(pos,wID,pNPCDlg);

		if(pNPCDlg != NULL && pNPCDlg->m_pNPC != NULL && pNPCDlg->m_pNPC->m_dwCharacterID && pNPCDlg->m_pNPC->m_bIsCache == FALSE)
		{
			for(int i = 0; i < m_cEventTargetCombo.GetCount(); ++i)
			{
				if(m_cEventTargetCombo.GetItemData(i) == pNPCDlg->m_pNPC->m_dwCharacterID)
				{
					pNPCDlg = NULL;
					break;
				}
			}

			if(pNPCDlg != NULL)
			{
				m_cEventTargetCombo.InsertString(nRow, pNPCDlg->m_pNPC->m_szCharacterName);
				m_cEventTargetCombo.SetItemData(nRow, (DWORD)pNPCDlg->m_pNPC);
				++nRow;
			}
		}
	}

}

void CDMEventLoggerDialog::Refresh()
{
	BOOL bReady = TRUE;

	if(m_nEventType == DND_LOG_EVENT_TYPE_UNDEFINED)
	{
		bReady = FALSE;
	}

	if(m_bNeedCharList && m_pSelectedCharacter == NULL)
	{
		bReady = FALSE;
	}


	if(bReady)
	{
		m_cLogEventButton.EnableWindow(TRUE);
	}
	else
	{
		m_cLogEventButton.EnableWindow(FALSE);
	}

	UpdateData(FALSE);

}

