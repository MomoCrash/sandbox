#pragma once

#include "Brush.h"
#include "Material.h"

namespace US {

    template<typename M>
    requires (std::is_base_of_v<Material, M> && HasColor<M>)
    class RectangleBrush : public Brush {
        
    public:
        RectangleBrush(CellManager *manager);

        void ApplyAt(int x, int y) override;
        sf::Color const& GetColor();
        
    };
    
}

#include "RectangleBrush.hpp"