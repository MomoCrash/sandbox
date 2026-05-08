#pragma once

#include "Brush.h"
#include "Material.h"
#include "CellManager.h"

namespace US {

    template<typename M>
    requires (std::is_base_of_v<Material, M> && HasColor<M>)
    class EllipseBrush : public Brush {
        
    public:
        EllipseBrush(CellManager *manager);

        void ApplyAt(int x, int y) override;
        sf::Color const& GetColor();
        
    };
    
}

#include "EllipseBrush.hpp"