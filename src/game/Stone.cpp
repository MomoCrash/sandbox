#include "Stone.h"

#include "Cell.h"


namespace US
{
    Stone::Stone() {

    }

    CellType Stone::GetType() {
        return STONE;
    }

    uint64_t Stone::GetTypes()
    {
        return Solid::GetTypes() ^ GetType();
    }

    sf::Color& Stone::GetColor() {
        return Stone::Color;
    }
}
