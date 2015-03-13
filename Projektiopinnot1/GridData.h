#ifndef GRIDDATA_H
#define GRIDDATA_H


class GridData
{
public:
	~GridData();
	GridData();
	friend class PlayerData;
	friend class Player;

private:
	bool skeleton = false;
	int turret = 0;
	int engine = 0;

	bool childUp = false;
	bool childRight = false;
	bool childDown = false;
	bool childLeft = false;

	bool core = false;

};

#endif