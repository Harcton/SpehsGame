#include "Main.h"
#include "GridData.h"


GridData::~GridData()
{
}
GridData::GridData()
{
	turretFire.inputType = mouseInput;
	turretFire.mouseButton = sf::Mouse::Left;
}


