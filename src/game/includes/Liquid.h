#pragma once
#include "Material.h"
#include "CellManager.h"

namespace US
{

	class Liquid : public Material {
	protected:
		int mSpreadMaxDist;
		int mDirection;

		Liquid();

	public:
		void Step(CellManager &manager, const Cell &srcCell, Cell &dstCell) override;
		void Spread(const Cell &srcCell, Cell &dstCell);
		CellType GetType() override;
		uint64_t GetTypes() override;
	};

}