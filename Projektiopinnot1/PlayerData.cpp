#include "Main.h"
#include "GridData.h"
#include "PlayerData.h"


PlayerData::~PlayerData()
{
	std::cout << "\nDeallocating all player data memory..";
	//free grid element memory
	for (unsigned int ex = 0; ex < EDITOR_WIDTH; ex++)
		for (unsigned int ey = 0; ey < EDITOR_HEIGHT; ey++)
			delete grid[ex][ey];
}
PlayerData::PlayerData()
{
	//Initialize grid elements
	for (unsigned int ex = 0; ex < EDITOR_WIDTH; ex++)
		for (unsigned int ey = 0; ey < EDITOR_HEIGHT; ey++)
		{
			grid[ex][ey] = new GridData;
		}

	/////////////////////
	//Debug ship editor:
	grid[5][5]->armor = 1;
	grid[5][5]->core = true;
	grid[5][5]->childRight = true;

	grid[6][5]->armor = 1;
	grid[6][5]->childUp = true;
	grid[6][5]->childDown = true;

	grid[6][4]->armor = 1;
	grid[6][4]->childUp = true;

	grid[6][6]->armor = 1;
	grid[6][6]->childDown = true;


	grid[6][3]->armor = 1;
	grid[6][3]->childRight = true;
	grid[6][3]->childLeft = true;

	grid[5][3]->armor = 1;
	grid[7][3]->armor = 1;
	grid[6][7]->armor = 1;

	//grid[6][3]->turret = 1;
	//grid[6][4]->turret = 1;
	//grid[6][5]->turret = 1;
	//grid[6][6]->turret = 1;
	//grid[6][7]->turret = 1;
	//End of Debug ship editor//
	///////////////////////////
}


