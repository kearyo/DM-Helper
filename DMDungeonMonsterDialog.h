#pragma once


// CDMDungeonMonsterDialog dialog

class CDMDungeonMonsterDialog : public CDialog
{
	DECLARE_DYNAMIC(CDMDungeonMonsterDialog)

public:
	CDMDungeonMonsterDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDMDungeonMonsterDialog();

// Dialog Data
	enum { IDD = IDD_DUNGEON_MONSTER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
