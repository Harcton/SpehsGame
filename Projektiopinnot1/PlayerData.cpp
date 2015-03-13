#include "Main.h"
#include "GridData.h"
#include "PlayerData.h"


PlayerData::~PlayerData()
{
	for (unsigned int ex = 0; ex < EDITOR_WIDTH; ex++)
		for (unsigned int ey = 0; ey < EDITOR_HEIGHT; ey++)
			delete grid[ex][ey];
}
PlayerData::PlayerData()
{
	for (unsigned int ex = 0; ex < EDITOR_WIDTH; ex++)
		for (unsigned int ey = 0; ey < EDITOR_HEIGHT; ey++)
		{
			grid[ex][ey] = new GridData;
		}

	grid[5][5]->core = true;
	grid[5][5]->skeleton = true;
	grid[5][5]->childRight = true;

	grid[6][5]->skeleton = true;
	grid[6][5]->childUp = true;
	grid[6][5]->childDown = true;

	grid[6][4]->skeleton = true;
	grid[6][4]->childUp = true;

	grid[6][6]->skeleton = true;
	grid[6][6]->childDown = true;

	grid[6][3]->skeleton = true;
	grid[6][7]->skeleton = true;


	grid[6][3]->turret = 1;
	grid[6][4]->turret = 1;
	grid[6][5]->turret = 1;
	grid[6][6]->turret = 1;
	grid[6][7]->turret = 1;
}


