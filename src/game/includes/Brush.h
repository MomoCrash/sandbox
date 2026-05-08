#pragma once
#include "SFML/Graphics/Color.hpp"

namespace US {

    class CellManager;


    class Brush {

    public:
        Brush(CellManager* manager);

        virtual ~Brush() = default;
        
        void SetSize(int w, int h);
        void SetDensity(int density);
        virtual void ApplyAt(int x, int y) = 0;
        virtual sf::Color const& GetColor() = 0;

    protected:

        CellManager* mCellManager;
        int mWidth, mHeight;
        int mDensity;
        
        
    };
    
}
