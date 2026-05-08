#include "RedSand.h"

#include "Cell.h"
#include "CellManager.h"

namespace US
{
	CellType RedSand::GetType()
	{
		return RED_SAND;
	}

	uint64_t RedSand::GetTypes()
	{
		return Solid::GetTypes() ^ GetType();
	}

	sf::Color& RedSand::GetColor()
	{
		return RedSand::Color;
	}

	RedSand::RedSand()
	{

	}

}
