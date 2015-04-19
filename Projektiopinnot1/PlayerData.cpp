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
	grid[int(EDITOR_WIDTH / 2.0)][int(EDITOR_HEIGHT / 2.0)]->armor = 1;
	grid[int(EDITOR_WIDTH / 2.0)][int(EDITOR_HEIGHT / 2.0)]->core = true;
	grid[int(EDITOR_WIDTH / 2.0)][int(EDITOR_HEIGHT / 2.0)]->childUp = true;
	grid[int(EDITOR_WIDTH / 2.0)][int(EDITOR_HEIGHT / 2.0)]->childDown = true;
	grid[int(EDITOR_WIDTH / 2.0)][int(EDITOR_HEIGHT / 2.0)]->childLeft = true;
	grid[int(EDITOR_WIDTH / 2.0)][int(EDITOR_HEIGHT / 2.0)]->childRight = true;

	grid[int(EDITOR_WIDTH / 2.0)][int(EDITOR_HEIGHT / 2.0) - 1]->armor = 1;
	grid[int(EDITOR_WIDTH / 2.0)][int(EDITOR_HEIGHT / 2.0) + 1]->armor = 1;
	grid[int(EDITOR_WIDTH / 2.0) - 1][int(EDITOR_HEIGHT / 2.0)]->armor = 1;
	grid[int(EDITOR_WIDTH / 2.0) + 1][int(EDITOR_HEIGHT / 2.0)]->armor = 1;

	//End of Debug ship editor//
	///////////////////////////
}


