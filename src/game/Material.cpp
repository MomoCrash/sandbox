#include "Material.h"

#include "Cell.h"
#include "CellManager.h"

namespace US
{
	Material::Material()
	{
		mVelocityX			= 0.0f;
		mVelocityY			= 0.0f;
		mHeat				= 0.0f;
		mResistance			= 0.0f;
		mAffectedByPhysics	= true;
	}

	Cell* Material::LookDiagonally(CellManager& manager, Cell const& srcCell, Cell& dstCell)
	{
		int choice = rand() % 2;
		if (choice == 0 && srcCell.DOWN_LEFT != nullptr && srcCell.DOWN_LEFT->IsEmpty()) return dstCell.DOWN_LEFT;
		if (choice == 1 && srcCell.DOWN_RIGHT != nullptr && srcCell.DOWN_RIGHT->IsEmpty()) return dstCell.DOWN_RIGHT;
		return nullptr;
	}

	Cell const* Material::RayCast(CellManager& manager, Cell& cell, int dx, int dy, int maxDistance)
	{
		float slope = static_cast<float>(dy) / static_cast<float>(abs(dx));
		
		int sign = (dx > 0 ? 1 : -1);

		for (int i = 0; i < maxDistance; ++i) {

			int offset_x = sign * i;
			int offset_y = slope * i;

			if (manager.IsInWorld(cell.Position.x + offset_x, cell.Position.y + offset_y) == false) {
				return nullptr;
			}

			Cell const& current = manager.ReadCellAt(cell.Position.x + offset_x, cell.Position.y + offset_y);

			if (manager.ReadCellAt(cell.Position.x + offset_x, cell.Position.y + offset_y).Material != nullptr) {
				return &current;
			}
		}

		return nullptr;
	
	}

	CellType Material::GetType()
	{
		return EMPTY;
	}

	uint64_t Material::GetTypes()
	{
		return EMPTY;
	}

	bool Material::CheckHasType(CellType type)
	{
		return (type & GetTypes()) == type;
	}

	bool Material::CheckIsType(CellType type)
	{
		return GetType() == type;
	}

	sf::Color& Material::GetColor()
	{
		static sf::Color DEFAULT_COLOR = { 0, 0, 0 };
		return DEFAULT_COLOR;
	}

}
