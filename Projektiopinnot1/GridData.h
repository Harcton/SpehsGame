#ifndef GRIDDATA_H
#define GRIDDATA_H


class GridData
{
public:
	~GridData();
	GridData();
	friend class PlayerData;
	friend class Player;
	friend class ShipEditor;

private:
	int turret = 0;
	int engine = 0;
	int armor = 0;

	bool childUp = false;
	bool childRight = false;
	bool childDown = false;
	bool childLeft = false;

	bool core = false;

};

#endif