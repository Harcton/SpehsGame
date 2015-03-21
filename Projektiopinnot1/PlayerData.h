#ifndef PLAYERDATA_H
#define PLAYERDATA_H


class PlayerData
{
public:
	PlayerData();
	~PlayerData();
	friend class Player;

	GridData* grid[EDITOR_WIDTH][EDITOR_HEIGHT];

private:

};

#endif