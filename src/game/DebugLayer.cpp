#include "DebugLayer.h"

#include <iostream>

#include "CellManager.h"
#include "Sand.h"
#include "Stone.h"

namespace US {
    DebugLayer::DebugLayer(CellManager* manager, unsigned int width, unsigned int height) : mDebugUI(manager), mHasUpdate(false),
        brush(manager) {
        mWindow = sf::RenderWindow(sf::VideoMode({width+100, height}), "Ultimate Simulator (V.1)");

        mCellManager = manager;

        mWidth = width;
        mHeight = height;
        
        mTexture = sf::Texture(sf::Vector2u(width, height));
        mImage = sf::Image(sf::Vector2u(width, height));

        brush.SetSize(100, 100);
        brush.SetDensity(100);

        mpCameraState = new RenderState({ mWidth, mHeight }, &mTexture);
    }

    DebugLayer::~DebugLayer() {
        
        delete mpCameraState;
        
    }

    void DebugLayer::UpdateInputs()
    {

        // Process events
        while (const std::optional event = mWindow.pollEvent())
        {
            // Close window: exit
            if (event->is<sf::Event::Closed>())
                mWindow.close();
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            sf::Vector2i cellPosition = mCellManager->ScreenSpaceToWorldSpace(sf::Mouse::getPosition(mWindow));
            sf::Vector2u worldPosition = mpCameraState->WorldSpaceToCellSpace(cellPosition);
            if (worldPosition.x < mWidth && worldPosition.y < mHeight && worldPosition.x > 0 && worldPosition.y > 0) {
                mDebugUI.getCurrentBrush()->ApplyAt(worldPosition.x, worldPosition.y);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
        {
            mpCameraState->IncrementZoom(-mCellManager->GetClock().GetDeltaTime());
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
        {
            mpCameraState->IncrementZoom(mCellManager->GetClock().GetDeltaTime());
        }

        float CameraSpeed = 200;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z))
        {
            mpCameraState->MoveCamera(0, -mCellManager->GetClock().GetDeltaTime() * CameraSpeed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            mpCameraState->MoveCamera(0, mCellManager->GetClock().GetDeltaTime() * CameraSpeed);
        }        
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
        {
            mpCameraState->MoveCamera(-mCellManager->GetClock().GetDeltaTime() * CameraSpeed, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
        {
            mpCameraState->MoveCamera(mCellManager->GetClock().GetDeltaTime() * CameraSpeed, 0);
        }
    }

    void DebugLayer::draw() {

        if (!mWindow.isOpen()) mCellManager->Stop();
        
        // Clear screen
        mWindow.clear();

        mTexture.update(mCellManager->GetCellsColor());
        mWindow.draw(*mpCameraState);
        mDebugUI.draw(mWindow);

        // Update the window
        mWindow.display();
    }
} // US