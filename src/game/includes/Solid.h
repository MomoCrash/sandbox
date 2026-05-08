#pragma once
#include "Material.h"

namespace US
{
	class Solid : public Material {
	protected:
		Solid();
	public:
		CellType GetType() override;
		uint64_t GetTypes() override;
	};

	class MovableSolid : public Solid {
	protected:
		MovableSolid();
	public:
		void Step(CellManager& manager, Cell const &srcCell, Cell &dstCell) override;
	};


	class ImovableSolid : public Solid {
	protected:
		ImovableSolid();

	public:
		void Step(CellManager &manager, const Cell &srcCell, Cell &dstCell) override;
	};
}