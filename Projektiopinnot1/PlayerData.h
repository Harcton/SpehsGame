#ifndef PLAYERDATA_H
#define PLAYERDATA_H


class PlayerData
{
public:
	PlayerData();
	~PlayerData();
	friend class Player;

	GridData* grid[11][11];

private:

};

#endif