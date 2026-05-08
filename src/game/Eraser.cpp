#pragma once
#include "Eraser.h"
#include "CellManager.h"

namespace US 
{
	Eraser::Eraser(CellManager* manager) : Brush(manager) {}

	Eraser::~Eraser() { }

	void Eraser::ApplyAt(int x, int y)
	{
		for (int i = 0; i < mDensity; ++i) {
			float angle = ((float)(rand() % 10000) / 10000.0f) * 2 * sf::priv::pi;
			float radius = (float)(rand() % 10000) / 10000.0f;

			float dx = cos(angle) * mWidth  * radius;
			float dy = sin(angle) * mHeight * radius;
			
			mCellManager->CleanCell(x + dx, y + dy);
		}

	}

	sf::Color const & Eraser::GetColor() {
		return sf::Color::Black;
	}
}
