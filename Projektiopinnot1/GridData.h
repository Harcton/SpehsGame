#ifndef GRIDDATA_H
#define GRIDDATA_H


class GridData
{
public:
	~GridData();
	GridData();
	friend class PlayerData;

private:
	bool skeleton;
	int turret;
	int engine;
};

#endif