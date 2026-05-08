#pragma once
#include "Brush.h"

namespace US {

	class Eraser : public Brush {
	public:
		Eraser(CellManager* manager);
		~Eraser() override;

		void ApplyAt(int x, int y) override;
		sf::Color const &GetColor() override;
	};
}
