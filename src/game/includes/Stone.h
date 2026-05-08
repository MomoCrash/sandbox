#pragma once
#include "Solid.h"

namespace US {

    class Stone : public ImovableSolid {

    public:
        static inline sf::Color Color = { 100, 100, 100 };

        CellType GetType() override;
        uint64_t GetTypes() override;
        sf::Color & GetColor() override;

        Stone();
    };

}
