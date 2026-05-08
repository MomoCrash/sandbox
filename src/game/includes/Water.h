#pragma once
#include "Liquid.h"

namespace US {
    class Water : public Liquid {
    public:
        static inline sf::Color Color = { 15, 94, 156 };
		
        CellType GetType() override;
        uint64_t GetTypes() override;
        sf::Color& GetColor() override;

        Water();
    };
}