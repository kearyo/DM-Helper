// cDMChartLookupDialog.cpp : implementation file
//

#include "stdafx.h"
#include "DM Helper.h"
#include "cDMChartLookupDialog.h"
#include "afxdialogex.h"


// cDMChartLookupDialog dialog

IMPLEMENT_DYNAMIC(cDMChartLookupDialog, CDialog)

cDMChartLookupDialog::cDMChartLookupDialog(DND_CHART_TYPES _ChartType, CWnd* pParent /*=NULL*/)
	: CDialog(cDMChartLookupDialog::IDD, pParent)
{

	m_ChartType = _ChartType;

	m_bRotateHeader = FALSE;
	m_bSmallFont = FALSE;

	m_nHeaderX = 0;
	m_nHeaderY = 0;
	m_nShiftHeaderX = 0;

	m_nLabelX = 0;
	m_nLabelY = 0;
	m_nShiftLabelY = 0;

	m_nDataX = 0;

	for (int i = 0; i < 30; ++i)
	{
		for (int j = 0; j < 20; ++j)
		{
			m_szMatrix[i][j] = _T("");
		}
	}
}

cDMChartLookupDialog::~cDMChartLookupDialog()
{
}

void cDMChartLookupDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(cDMChartLookupDialog, CDialog)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// cDMChartLookupDialog message handlers


BOOL cDMChartLookupDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	switch (m_ChartType)
	{
		case DND_CHART_ITEM_SAVING_THROWS:
		{	
			SetWindowText("Saving Throw Matrix for Magical and Non-Magical Items");
			m_bRotateHeader = TRUE;

			m_nHeaderX = 170;
			m_nHeaderY = 160;
			m_nShiftHeaderX = 34;

			m_nShiftLabelY = 20;
			m_nLabelX = 10;
			m_nLabelY = m_nHeaderY + 8;

			m_nDataX = m_nHeaderX;
			
			m_szMatrix[0][0] = "acid"; m_szMatrix[0][1] = "crushing blow"; m_szMatrix[0][2] = "normal blow"; m_szMatrix[0][3] = "disintegrate"; m_szMatrix[0][4] = "fall"; m_szMatrix[0][5] = "fireball"; m_szMatrix[0][6] = "magical fire"; m_szMatrix[0][7] = "normal fire"; m_szMatrix[0][8] = "frost"; m_szMatrix[0][9] = "lightning"; m_szMatrix[0][10] = "electricity";

			m_szMatrix[1][0] = "Bone or Ivory";
			m_szMatrix[2][0] = "Ceramic";
			m_szMatrix[3][0] = "Cloth";
			m_szMatrix[4][0] = "Crystal or Vial";
			m_szMatrix[5][0] = "Glass";
			m_szMatrix[6][0] = "Leather or Book";
			m_szMatrix[7][0] = "Liquid*";
			m_szMatrix[8][0] = "Metal, hard";
			m_szMatrix[9][0] = "Metal, soft or Jewelry**";
			m_szMatrix[10][0] = "Mirror***";
			m_szMatrix[11][0] = "Parchment or Paper";
			m_szMatrix[12][0] = "Stone, small or Gem";
			m_szMatrix[13][0] = "Wood or Rope, thin";
			m_szMatrix[14][0] = "Wood or Rope, thick";

			m_szMatrix[1][1] = "11_16_10_20_ 6_17_ 9_ 3_ 2_ 8_ 1";
			m_szMatrix[2][1] = " 4_18_12_19_11_ 5_ 3_ 2_ 4_ 2_ 1";
			m_szMatrix[3][1] = "12_ 6_ 3_20_ 2_20_16_13_ 1_18_ 1";
			m_szMatrix[4][1] = " 6_19_14_20_13_10_ 6_ 3_ 7_15_ 5";
			m_szMatrix[5][1] = " 5_20_15_20_14_11_ 7_ 4_ 6_17_ 1";
			m_szMatrix[6][1] = "10_ 4_ 2_20_ 1_13_ 6_ 4_ 3_13_ 1";
			m_szMatrix[7][1] = "15_ 0_ 0_20_ 0_15_14_13_12_16_15";
			m_szMatrix[8][1] = " 7_ 6_ 2_17_ 2_ 6 _2_ 1_1*_11_ 1";
			m_szMatrix[9][1] = "13_14_ 9_19_ 4_18_13_ 5_ 1_16_ 1";
			m_szMatrix[10][1] = "12_20_15_20_13_14_ 9_ 5_ 6_18_ 1";
			m_szMatrix[11][1] = "16_11_ 6_20_ 0_25_21_18_ 2_20_ 1";
			m_szMatrix[12][1] = " 3_17_ 7_18_ 4_ 7_ 3_ 2_ 1_14_ 2";
			m_szMatrix[13][1] = " 9_13_ 6_20_ 2_15_11_ 9_ 1_10_ 1";
			m_szMatrix[14][1] = " 8_10_ 3_19_ 1_11_ 7_ 5_ 1_12_ 1";
			m_szMatrix[15][0] = " ";
			m_szMatrix[16][0] = "* Potions, magical oils, poisons, acids while container remains intact.";
			m_szMatrix[17][0] = "** Includes pearls of any sort.";
			m_szMatrix[18][0] = "*** Silvered glass. Treat silver mirror as 'Metal, soft, steel as 'Metal, hard.'";
			m_szMatrix[19][0] = "* If exposed to extreme cold then struck against a very hard surface with force, saving throw is -10 on die!";

			break;
		}

		case DND_CHART_POTION_MISCIBITY:
		{
			SetWindowText("Potion Miscibility Table");

			m_nHeaderX = 20;
			m_nHeaderY = 60;
			m_nShiftHeaderX = 128;

			m_nShiftLabelY = 20;
			m_nLabelX = 20;
			m_nLabelY = m_nHeaderY + 16;

			m_nDataX = m_nHeaderX;

			m_szMatrix[0][0] = "Dice Score"; m_szMatrix[0][1] = "Result"; 

			m_szMatrix[1][0] = "01_EXPLOSION! Internal damage is 6-60 h.p., those within\na 5\" radius take 1 - 10 h.p. if mixed externally, all in\na 10\" radius take 4 - 24 hit points, no save.";
			m_szMatrix[2][0] = "02-03_Lethal poison results, and imbiber is dead; if externally\nmixed, a poison gas cloud of 10\" diameter results, and\nall within it must save versus poison or die.";
			m_szMatrix[3][0] = "04-08_Mild poison which causes nausea and loss of 1 point each\nof strength and dexterity for 5 - 20 rounds, no saving\nthrow possible; one potion is cancelled, the other is at\nhalf strength and duration. (Use random determination\nfor which is cancelled and which is at half efficacy.)";
			m_szMatrix[4][0] = "09-15_Immiscible. Both potions totally destroyed, as one cancelled the other.";
			m_szMatrix[5][0] = "16-25_Immiscible.One potion cancelled, but the other remains\nnormal(random selection).";
			m_szMatrix[6][0] = "26-35_Immiscible result which causes both potions to be at half\nnormal efficacy when consumed.";
			m_szMatrix[7][0] = "36-90_Miscible. Potions work normally unless their effects are\ncontradictory, e.g.diminution and growth, which will\nsimply cancel each other.";
			m_szMatrix[8][0] = "91-99_Compatible result which causes one potion (randomly\ndetermined) to have 150 % normal efficacy. (You must\ndetermine if both effect and duration are permissible,\nor if only the duration should be extended.)";
			m_szMatrix[9][0] = "00_DISCOVERY! The admixture of the two potions has\ncaused a special formula which will cause one of the\ntwo potions only to function, but its effects will be permanent upon the imbiber.\n(Note that some harmful side effects could well result from this. . .)";
			
			break;
		}

		case DND_CHART_TREASURE_TYPES:
		{
			SetWindowText("Treasure Types");
			m_bSmallFont = TRUE;

			int nSizeX = 760;
			int nSizeY = 820;

			SetWindowPos(NULL, nSizeX / 2, 100, nSizeX, nSizeY, 0);

			m_nHeaderX = 20;
			m_nHeaderY = 20;
			m_nShiftHeaderX = 76;

			m_nShiftLabelY = 12;
			m_nLabelX = 20;
			m_nLabelY = m_nHeaderY + 30;

			m_nDataX = m_nHeaderX;

			m_szMatrix[0][0] = "Treasure\nType"; m_szMatrix[0][1] = "1000's\nof\nCopper"; m_szMatrix[0][2] = "1000's\nof\nSilver";  m_szMatrix[0][3] = "1000's\nof\nElectrum"; m_szMatrix[0][4] = "1000's\nof\nGold"; m_szMatrix[0][5] = "100's\nof\nPlatinum"; m_szMatrix[0][6] = "Gems"; m_szMatrix[0][7] = "Jewelry"; m_szMatrix[0][8] = "Maps\nor\nMagic";
			m_szMatrix[1][0] = "A_1-6 :25%_1-6 :30%_1-6 :35%_1-10 :40%_1-4 :25%_4-40 :60%_3-50 :50%_Any 3: 30%";
			m_szMatrix[2][0] = "B_1-8 :50%_1-6 :25%_1-4 :25%_1-3 :25%_nil_1-8 :30%_1-4 :20%_Sword, armor,\nor misc.\nweapon: 10%";
			m_szMatrix[3][0] = "C_1-12 :20%_1-6 :30%_1-4 :10%_nil_nil_1-6 :25%_1-3 :20%_Any 2: 10%"; 
			m_szMatrix[4][0] = "D_1-8 :10%_1-12 :15%_1-8 :15%_1-6 :50%_nil_1-10 :30%_1-6 :25%_Any 2 plus 1\npotion: 15%";
			m_szMatrix[5][0] = "E_1-10 :5%_1-12 :25%_1-6 :25%_1-8 :25%_nil_1-12 :15%_1-8 :10%_Any 3 plus 1\nscroll: 25%";
			m_szMatrix[6][0] = "F_nil_1-20 :10%_1-12 :15%_1-10 :40%_1-8 :35%_3-30 :20%_1-10 :10%_Any 3 except\nswords or misc.\nweapons, plus 1\npotion & 1\nscroll: 30%";
			m_szMatrix[7][0] = "G_nil_nil_nil_10-40 :50%_1-20 :50%_5-20 :30%_1-10 :25%_Any 4 plus 1\nscroll: 35%";
			m_szMatrix[8][0] = "H_5-30 :25%_1-100 :40%_10-40 :40%_10-60 :55%_5-50 :25%_1-100 :50%_10-40 :50%_Any 4 plus 1\npotion & 1\nscroll: 15% ";
			m_szMatrix[9][0] = "I_nil_nil_nil_nil_3-18 :30%_2-20 :55%_1-12 :50%_Any 1: 15% ";
			m_szMatrix[10][0] = "J_3-24 per\nindividual_nil_nil_nil_nil_nil_nil_nil";
			m_szMatrix[11][0] = "K_nil_3-18 per\nindividual_nil_nil_nil_nil_nil_nil";
			m_szMatrix[12][0] = "L_nil_nil_2-12 per\nindividual_nil_nil_nil_nil_nil";
			m_szMatrix[13][0] = "M_nil_nil_nil_2-8 per\nindividual_nil_nil_nil_nil";
			m_szMatrix[14][0] = "N_nil_nil_nil_nil_1-6 per\nindividual_nil_nil_nil";
			m_szMatrix[15][0] = "O_1-4 :25%_1-3 :20%_nil_nil_nil_nil_nil_nil";
			m_szMatrix[16][0] = "P_nil_1-6 :30%_1-2 :25%_nil_nil_nil_nil_nil";
			m_szMatrix[17][0] = "Q_nil_nil_nil_nil_nil_1-4 :50%_nil_nil";
			m_szMatrix[18][0] = "R_nil_nil_nil_2-8 :40%_10-60 :50%_4-32 :55%_1-12 :45%_nil";
			m_szMatrix[19][0] = "S_nil_nil_nil_nil_nil_nil_nil_2-8 potions:\n40% ";
			m_szMatrix[20][0] = "T_nil_nil_nil_nil_nil_nil_nil_1-4 scrolls: 50%";
			m_szMatrix[21][0] = "U_nil_nil_nil_nil_nil_10-80 :90%_5-30 :80%_1 of each magic\nexcluding\npotions &\nscrolls: 70% ";
			m_szMatrix[22][0] = "V_nil_nil_nil_nil_nil_nil_nil_2 of each magic\nexcluding\npotions &\nscrolls: 85% ";
			m_szMatrix[23][0] = "W_nil_nil_nil_5-30 :60%_1-8 :15%_10-80 :60%_5-40 :50%_1 map: 55%";
			m_szMatrix[24][0] = "X_nil_nil_nil_nil_nil_nil_nil_1 misc. magic\nplus 1 potion:\n60%";
			m_szMatrix[25][0] = "Y_nil_nil_nil_2-12 :70%_nil_nil_nil_nil";
			m_szMatrix[26][0] = "Z_1-3 :20%_1-4 :25%_1-4 :25%_1-4 :30%_1-6 :30%_10-60 :55%_5-30 :50%_Any 3 magic:\n50%";

			break;
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void cDMChartLookupDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CDC* pDC;
	pDC = &dc;

	CFont *OFont = NULL;

	if (m_bSmallFont)
	{
		CFont smallFont;
		CFont CourierFont;

		CourierFont.CreatePointFont(80, "Courier New");
		LOGFONT lf; CourierFont.GetLogFont(&lf);
		lf.lfWeight = FW_BOLD;
		smallFont.CreateFontIndirect(&lf);

		OFont = (CFont *)pDC->SelectObject(&smallFont);
	}

	SetBkMode(dc, TRANSPARENT);
	
	int nLineBreakY = 0;
	if (m_bRotateHeader)
	{
		CFont verFont;
		verFont.CreateFont(14, 10, 900, 0, FALSE, FALSE, FALSE, 0, ARABIC_CHARSET, OUT_TT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Fixedsys"));

		OFont = (CFont *)pDC->SelectObject(&verFont);
		
		for (int i = 0; i < 20 && m_szMatrix[0][i] != _T(""); ++i)
		{
			pDC->TextOut(m_nHeaderX + i*m_nShiftHeaderX, m_nHeaderY, m_szMatrix[0][i]);
		}
	
		pDC->SelectObject(OFont);
	}
	else
	{
		for (int i = 0; i < 30 && m_szMatrix[0][i] != _T(""); ++i)
		{
			//pDC->TextOut(m_nHeaderX + i*m_nShiftHeaderX, m_nHeaderY, m_szMatrix[0][i]);
			CString sSubString = _T("");
			int k = 0; // substring index to extract
			while (AfxExtractSubString(sSubString, m_szMatrix[0][i], k, '\n'))
			{
				int nLineY = m_nHeaderY + (k - 1)*m_nShiftLabelY;
				pDC->TextOut(m_nHeaderX + i*m_nShiftHeaderX, nLineY, sSubString);
				++k;
			}
		}
	}

	for (int i = 1; i < 30 && m_szMatrix[i][0] != _T(""); ++i)
	{
		//pDC->TextOut(m_nLabelX, m_nLabelY + (i-1)*m_nShiftLabelY, m_szMatrix[i][0]);
		CString sSubString = _T("");
		int k = 0; // substring index to extract
		int nStartY = nLineBreakY;
		while (AfxExtractSubString(sSubString, m_szMatrix[i][0], k, '_'))
		{
			CString sSubSubString = _T("");
			int l = 0; // substring index to extract
			while (AfxExtractSubString(sSubSubString, sSubString, l, '\n'))
			{
				if (l) 
				{
					nLineBreakY += m_nShiftLabelY;
				}

				pDC->TextOut(m_nLabelX + k * m_nShiftHeaderX, m_nLabelY + (i - 1)*m_nShiftLabelY + l*m_nShiftLabelY + nStartY, sSubSubString);

				++l;
			}

			++k;
		}

		for (int j = 1; j < 20 && m_szMatrix[i][j] != _T(""); ++j)
		{
			if (m_szMatrix[i][j] == _T(""))
				continue;
			
			sSubString = _T("");
			k = 0; // substring index to extract
			while (AfxExtractSubString(sSubString, m_szMatrix[i][j], k, '_'))
			{
				pDC->TextOut(m_nDataX + k * m_nShiftHeaderX, m_nLabelY + (i - 1)*m_nShiftLabelY, sSubString);
				++k;
			}
		}

		if (m_bSmallFont)
		{
			nLineBreakY += 4;
		}
	}

	if (m_bSmallFont && OFont != NULL)
	{
		pDC->SelectObject(OFont);
	}

}
