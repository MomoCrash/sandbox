#pragma once
#include "Solid.h"

namespace US
{
	class Sand : public MovableSolid
	{
	public:
		static inline sf::Color Color = { 226, 202, 118 };
		
		CellType GetType() override;
		uint64_t GetTypes() override;
		sf::Color& GetColor() override;

		Sand();
	};
}