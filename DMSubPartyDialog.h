#pragma once


// CDMSubPartyDialog dialog

class CDMSubPartyDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMSubPartyDialog)

public:
	CDMSubPartyDialog(cDNDSubParty *pSubParty, CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMSubPartyDialog();

	cDNDSubParty *m_pSubParty;
	DMPartyDialog *m_pParentPartyDialog;

	DWORD m_dwSubPartyID;

	void SyncParent();
	void Initialize();
	void UpdateSelections();
	void Refresh();

// Dialog Data
	enum { IDD = IDD_SUBPARTY_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
