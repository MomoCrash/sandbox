#pragma once

#include "RectangleBrush.h"

namespace US {
    template<typename M> requires (std::is_base_of_v<Material, M> && HasColor<M>)
    RectangleBrush<M>::RectangleBrush(CellManager *manager) : Brush(manager) {
    }

    // ==== Square Brush ====//
    template<typename M> requires (std::is_base_of_v<Material, M> && HasColor<M>)
    void RectangleBrush<M>::ApplyAt(int x, int y) {

        for (int i = 0; i < this->mDensity; i++) {

            float randX = (float)(rand()%10000) / 10000.0f - 0.5f;
            float randY = (float)(rand()%10000) / 10000.0f - 0.5f;

            int dx = randX * this->mWidth ;
            int dy = randY * this->mHeight;

            this->mCellManager->SpawnCellAt<M>( x + dx, y + dy);
            
        }
        
    }

    template<typename M> requires (std::is_base_of_v<Material, M> && HasColor<M>)
    sf::Color const & RectangleBrush<M>::GetColor() {
        return M::Color;
    }
}
