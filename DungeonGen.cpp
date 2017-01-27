/*   */

#include "stdafx.h"
#include <math.h>

#include <string>
#include <cstdlib>
#include <cstdio>

#include "DM Helper.h"
#include "DungeonGen.h"


void CDungeonGen::MazeGen(unsigned int seed)
{
	srand(seed);
	srand( (unsigned)time( NULL ) );
	int x;
	int y;
	int sx;
	int sy;
	int px;
	int py;
	int dx;
	int dy;

	//Create initial room
	int nNX = m_nSizeX * 2 / 3;
	int nNY = m_nSizeY * 2 / 3;

	int nStart = random(5);

	switch(nStart)
	{
		case 1:
		{
			nNX = m_nSizeX / 3;
			nNY = m_nSizeY / 3;
			break;
		}
		case 2:
		{
			nNX = m_nSizeX / 3;
			break;
		}
		case 3:
		{
			nNY = m_nSizeY / 3;
			break;
		}
		case 4:
		{
			nNX = m_nSizeX / 2;
			nNY = m_nSizeY / 2;
		}
	}

	Fill(nNX,nNY,nNX+6,nNY+6,DG_FLOOR + 1000);
	m_nTiles[nNX][nNY] = DG_FLOOR;

	m_nTiles[nNX][nNY] += 1001000;

	int nRoomNumber = 1;

	//Fill(47,47,53,53,DG_FLOOR);

	for(int i = 0; i < ATTEMPT_FEATURES; i++)
	{
		int nLoops = 0;
		for(;;)
		{
			++nLoops; // no INFINITY
			//find insertion point
			x = random(m_nSizeX);
			y = random(m_nSizeY);
			if(IsWall(x,y,dx,dy)) break;
			if(nLoops > 30000) break;
		}
		
		sx = random(3)+3;
		sy = random(3)+3;

		//make corridor
		if(random(10) < 5)
		{
			if(random(2) == 0)
			{
				sx = 1; sy *= 2;
			}
			else
			{
				sy = 1; sx *= 2;
			}
		}
		
		if(dx == 0)
		{
			px = x-(sx/2);
			if(dy > 0)
			{
				py = y +dy;
			}
			else
			{
				py = y - sy ;
			}
		}
		else
		{
			py = y-(sy/2);
			if(dx > 0)
			{
				px = x+dx ;
			}
			else
			{
				px = x - sx ;
			}
		}


		if(CanAdd(px-1,py-1,px+sx+1,py+sy+1) == TRUE)
		{
			m_nTiles[x][y] = DG_FLOOR;

			//if(random(10) == 2) 
			if(random(100) <= 70) 
			{
				m_nTiles[x][y] = DG_DOOR;
			}

			int nTile = DG_CORRIDOR;
			if(sx > 1 && sy > 1)
			{
				//this is a room
				nTile = DG_FLOOR;

				++nRoomNumber;
				nTile += 1000 * nRoomNumber;
			}

			Fill(px,py,px+sx,py+sy, nTile);
			
			if(nTile % 1000 == DG_FLOOR) // debug - mark corner of room
			{
				m_nTiles[px][py] = DG_FLOOR + 1000 * nRoomNumber;
				m_nTiles[px][py] += 1000000; //set room anchor flag
			}

		//	if(sx > 1 && sy > 1) Blur(px-2,py-2,px+sx+2,py+sy+2);
		}
	}


}

void CDungeonGen::InitMaze(void)
{
	for(int x = 0; x < m_nSizeX; x++)
	{
		for(int y = 0; y < m_nSizeY; y++)
		{
			m_nTiles[x][y] = DG_WALL;
		}
	}
}


void CDungeonGen::Fill(int x1, int y1, int x2, int y2, int val)
{
	for(int x = x1; x < x2; x++)
	{
		for(int y = y1; y < y2; y++)
		{
			m_nTiles[x][y] = val;
		}
	}

}

BOOL CDungeonGen::IsOpenTile(int x, int y)
{
	return IsOpenTile(m_nTiles[x][y]);
}

BOOL CDungeonGen::IsOpenTile(int nTile)
{
	nTile = nTile % 1000; // remove flags

	switch(nTile)
	{
		case DG_FLOOR:
		case DG_CORRIDOR:
		case DG_FOUNTAIN:
		case DG_TRAPDOOR:
		{
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CDungeonGen::IsWall(int x, int y, int &dx, int &dy)
{
	int spaces = 0;

	if(m_nTiles[x][y] != DG_WALL) return FALSE;

	if(x <= 0 || x >= m_nSizeX) return FALSE;
	if(y <= 0 || y >= m_nSizeY) return FALSE;

	if( IsOpenTile(m_nTiles[x+1][y]) ) dx = -1, dy = 0, spaces++;
	if( IsOpenTile(m_nTiles[x-1][y]) ) dx = 1, dy = 0,spaces++;
	if( IsOpenTile(m_nTiles[x][y+1]) ) dx = 0, dy = -1,spaces++;
	if( IsOpenTile(m_nTiles[x][y-1]) ) dx = 0, dy = 1,spaces++;

	if(spaces == 1) return TRUE;

	return FALSE;

}
BOOL CDungeonGen::CanAdd(int x1, int y1, int x2, int y2)
{
	for(int x = x1; x < x2; x++)
	{
		for(int y = y1; y < y2; y++)
		{
			if(x <= 0 || x >= m_nSizeX) return FALSE;
			if(y <= 0 || y >= m_nSizeY) return FALSE;	

			if(m_nTiles[x][y] != DG_WALL) return FALSE;
		}
	}

	return TRUE;
}
int CDungeonGen::random(int num)
{
	return int(rand()%num);

}
int CDungeonGen::negpo(void)
{
	if(random(2) == 0)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

void CDungeonGen::Blur(int x1, int y1, int x2, int y2)
{
	int c,x,y;
	for(int i = 0; i < 30; i++)
	{
		x = random(x2-x1)+x1;
		y = random(y2-y1)+y1;

		if(IsWall(x,y,c,c) == TRUE) m_nTiles[x][y] = DG_FLOOR;
	}

	return;
}
