#pragma once

#include <DebugUI.h>
#include <Sand.h>

#include "SFML/Graphics/Image.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Shader.hpp"
#include "SFML/Graphics/Texture.hpp"

#include "EllipseBrush.h"

#include "RenderState.h"

namespace US {
    class CellManager;

    class DebugLayer {
        
        DebugUI mDebugUI;
        
        sf::RenderWindow mWindow;

        sf::Shader mShader;
        sf::Texture mTexture;
        sf::Image mImage;

        RenderState* mpCameraState;

        uint32_t mWidth;
        uint32_t mHeight;

        bool     mHasUpdate;
        
        CellManager* mCellManager;
        sf::Vector2u mLastMiddleClick = { 0, 0 };

        EllipseBrush<Sand> brush;
        
    public:
        DebugLayer(CellManager* manager, unsigned int width, unsigned int height);
        ~DebugLayer();
        
        void UpdateInputs();
        void draw();
    };
} // US