#include "Generator.h"

#include "CellManager.h"
#include "Sand.h"

namespace US {
    
    CellType Generator::GetType()
    {
        return GENERATOR;
    }

    uint64_t Generator::GetTypes()
    {
        return Solid::GetTypes() ^ GetType();
    }

    sf::Color& Generator::GetColor()
    {
        return Generator::Color;
    }

    void Generator::Step(CellManager &manager, Cell const &srcCell, Cell &dstCell) {
        ImovableSolid::Step(manager, srcCell, dstCell);
		int choice = rand() % 10;
        if (choice == 0) {
            if (dstCell.DOWN == nullptr) return;
            manager.SpawnCellAt<Sand>(dstCell.DOWN->Position.x, dstCell.DOWN->Position.y);
        } else if (choice == 1) {
            if (dstCell.LEFT == nullptr) return;
            manager.SpawnCellAt<Sand>(dstCell.LEFT->Position.x, dstCell.LEFT->Position.y);
        } else if (choice == 2) {
            if (dstCell.RIGHT == nullptr) return;
            manager.SpawnCellAt<Sand>(dstCell.RIGHT->Position.x, dstCell.RIGHT->Position.y);
        }
        
    }

    Generator::Generator() {
    }
}
