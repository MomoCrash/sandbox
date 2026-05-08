#pragma once
#include "Solid.h"

namespace US
{
	class RedSand : public MovableSolid
	{
	public:
		static inline sf::Color Color = { 194, 99, 75 };
		
		CellType GetType() override;
		uint64_t GetTypes() override;
		sf::Color& GetColor() override;

		RedSand();
	};
}