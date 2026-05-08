#include "Liquid.h"

#include "CellManager.h"

namespace US
{
	Liquid::Liquid()
	{
		mSpreadMaxDist = 3;
		mDirection = 1;
	}

	void Liquid::Step(CellManager &manager, const Cell &srcCell, Cell &dstCell) {

		mDirection = rand() % 2 * 2 - 1;
		if (srcCell.DOWN != nullptr && srcCell.DOWN->IsEmpty()) {
			manager.GoTo(srcCell, *dstCell.DOWN);
			return;
		}
		
		for (int i = 0; i < mSpreadMaxDist; i++) {

			if (!manager.IsInWorld(srcCell.Position.x + (i+1) * mDirection, srcCell.Position.y)) {
				manager.GoTo(srcCell, dstCell);
				return;
			}
			
			Cell const& nextCell = manager.ReadCellAt(srcCell.Position.x + (i+1) * mDirection, srcCell.Position.y);
			if (!nextCell.IsEmpty() || mSpreadMaxDist-1 == i) {

				if (nextCell.DOWN != nullptr && nextCell.DOWN->IsEmpty()) {

					Cell& newWater = manager.WriteCellAt(nextCell.DOWN->Position.x - mDirection, nextCell.DOWN->Position.y);
					manager.GoTo(srcCell, newWater);
					return;
				} else {
					Cell& newWater = manager.WriteCellAt(nextCell.Position.x - mDirection, nextCell.Position.y);
					manager.GoTo(srcCell, newWater);
					return;
				}
				
			}
			
		}
		
	}

	CellType Liquid::GetType()
	{
		return LIQUID;
	}

	uint64_t Liquid::GetTypes()
	{
		return SOLID;
	}
}
