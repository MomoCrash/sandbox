#pragma once

#include "EllipseBrush.h"

namespace US {
    template<typename M> requires (std::is_base_of_v<Material, M> && HasColor<M>)
    EllipseBrush<M>::EllipseBrush(CellManager *manager) : Brush(manager) {
    }

    // ==== Ellipse Brush ====//
    template<typename M> requires (std::is_base_of_v<Material, M> && HasColor<M>)
    void EllipseBrush<M>::ApplyAt(int x, int y) {

        for (int i = 0; i < this->mDensity; i++) {

            float theta = (float)(rand()%10000) / 10000.0f * 2.0f*sf::priv::pi;
            float radius = (float)(rand()%10000) / 10000.0f;

            int dx = cos(theta) * this->mWidth * radius;
            int dy = sin(theta) * this->mHeight * radius;

            this->mCellManager->SpawnCellAt<M>( x + dx, y + dy);
            
        }
        
    }

    template<typename M> requires (std::is_base_of_v<Material, M> && HasColor<M>)
    sf::Color const & EllipseBrush<M>::GetColor() {
        return M::Color;
    }

}
