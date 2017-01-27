#ifndef _DUNGEONGEN_H_
#define _DUNGEONGEN_H_
 
#define MAX_DG_XSIZE 60 //90
#define MAX_DG_YSIZE 60 //90

#define MIN_DG_XSIZE 10 
#define MIN_DG_YSIZE 10 


#define ATTEMPT_FEATURES 4000

#define DG_WALL			0
#define DG_FLOOR		1
#define DG_CORRIDOR		2
#define DG_DOOR			3
#define DG_FOUNTAIN		4
#define DG_TRAPDOOR		5

class CDungeonGen
{

public:

	int m_nSizeX;
	int m_nSizeY;

	int m_nTiles[MAX_DG_XSIZE][MAX_DG_YSIZE];

	CDungeonGen(int nSizeX, int nSizeY)
	{
		m_nSizeX = min(nSizeX, MAX_DG_XSIZE);
		m_nSizeY = min(nSizeY, MAX_DG_YSIZE);

		m_nSizeX = max(m_nSizeX, MIN_DG_XSIZE);
		m_nSizeY = max(m_nSizeY, MIN_DG_YSIZE);

		memset(m_nTiles, 0, MAX_DG_XSIZE*MAX_DG_YSIZE*sizeof(int));

		InitMaze();
		MazeGen(GetUniqueID());
	}

	 ~CDungeonGen()
	{

	}

	//Custom Functions Here
	void InitMaze(void);
	void MazeGen(unsigned int seed);

	void Fill(int x1, int y1, int x2, int y2, int val);
	BOOL IsOpenTile(int x, int y);
	BOOL IsOpenTile(int nTile);
	BOOL IsWall(int x, int y, int &dx, int &dy);
	BOOL CanAdd(int x1, int y1, int x2, int y2);
	int random(int num);
	int negpo(void);

	void Blur(int x1, int y1, int x2, int y2);


};

#endif