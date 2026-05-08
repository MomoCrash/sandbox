#pragma once

#include "Solid.h"

namespace US {
    
    class Generator : public ImovableSolid {

    public:
        static inline sf::Color Color = { 128, 128, 128 };
		
        CellType GetType() override;
        uint64_t GetTypes() override;
        sf::Color& GetColor() override;

        void Step(CellManager &manager, Cell const &srcCell, Cell &dstCell) override;

        Generator();
        
    };
    
}