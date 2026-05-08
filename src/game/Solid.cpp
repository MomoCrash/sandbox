#include "Solid.h"

#include "Cell.h"
#include "CellManager.h"

namespace US
{
	/* === SOLID === */
	Solid::Solid() {

	}

	CellType Solid::GetType()
	{
		return SOLID;
	}

	uint64_t Solid::GetTypes()
	{
		return SOLID;
	}

	/* === MOVABLE SOLID === */
	MovableSolid::MovableSolid() {

	}

	void MovableSolid::Step(CellManager& manager, Cell const &srcCell, Cell &dstCell)
	{
		const Cell* pDown = srcCell.DOWN;

		// Touching Bottom of screen
		if (pDown == nullptr) {
			manager.GoTo(srcCell, dstCell);
			return;
		}

		if (pDown->IsEmpty()) {
			manager.GoTo(srcCell, *dstCell.DOWN);
			return;
		}

		if (pDown->Material->CheckHasType(LIQUID)) {
			manager.SwapCellAt(srcCell, *dstCell.DOWN);
			return;
		}

		if (pDown->Material->CheckHasType(SOLID)) {
			Cell* pDiagonal = LookDiagonally(manager, srcCell, dstCell);
			if (pDiagonal != nullptr) {
				manager.GoTo(srcCell, *pDiagonal);
			}
			else {
				manager.GoTo(srcCell, dstCell);
			}
			return;
		}

	}

	/* === IMOVABLE SOLID === */
	ImovableSolid::ImovableSolid() {
		mAffectedByPhysics = false;
	}

	void ImovableSolid::Step(CellManager &manager, const Cell &srcCell, Cell &dstCell) {
		manager.GoTo(srcCell, dstCell);
	}
}
