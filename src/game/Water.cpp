#include "Water.h"

US::CellType US::Water::GetType() {
    return WATER;
}

uint64_t US::Water::GetTypes() {
    return Liquid::GetTypes() | GetType(); 
}

sf::Color & US::Water::GetColor() {
    return Color;
}

US::Water::Water() {
}
