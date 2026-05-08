#include "Cell.h"

namespace US {
    Cell::Cell() : Position(0, 0) {
    }

    Cell::Cell(uint32_t x, uint32_t y) : Position(x, y) {
    }
} // US