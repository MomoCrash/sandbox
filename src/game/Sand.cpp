#include "Sand.h"

#include "Cell.h"
#include "CellManager.h"

namespace US
{
	CellType Sand::GetType()
	{
		return SAND;
	}

	uint64_t Sand::GetTypes()
	{
		return Solid::GetTypes() ^ GetType();
	}

	sf::Color& Sand::GetColor()
	{
		return Sand::Color;
	}

	Sand::Sand()
	{

	}

}
