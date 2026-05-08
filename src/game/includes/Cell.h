#pragma once

#include <cstdint>

#include "defines.h"
#include "SFML/System/Vector2.hpp"
#include "Material.h"

namespace US {

    struct Cell {

        Cell();
        Cell(uint32_t x, uint32_t y);

        sf::Vector2u Position;
        Material* Material = nullptr;

        // Around Cells
        Cell* UP        = nullptr;
        Cell* DOWN      = nullptr;
        Cell* RIGHT     = nullptr;
        Cell* LEFT      = nullptr;

        Cell* UP_RIGHT  = nullptr;
        Cell* UP_LEFT   = nullptr;
        Cell* DOWN_RIGHT= nullptr;
        Cell* DOWN_LEFT = nullptr;
    
        bool IsEmpty() const { return Material == nullptr; }
    };

} // US