#include "Brush.h"

namespace US {
    
    Brush::Brush(CellManager *manager) : mWidth(0), mHeight(0), mDensity(0) {
        mCellManager = manager;
    }
    
    void Brush::SetSize(int w, int h) {
        mWidth = w;
        mHeight = h;
    }
    
    void Brush::SetDensity(int density) {
        mDensity = density;
    }
    
}