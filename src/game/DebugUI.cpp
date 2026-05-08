#include "DebugUI.h"

#include <CellManager.h>
#include <Stone.h>

#include "Generator.h"
#include "RedSand.h"
#include "Sand.h"
#include "Water.h"
#include "Eraser.h"

namespace US {

    void DebugUI::DrawTextAt(sf::RenderWindow& win, std::string s, sf::Vector2f pos) {
        mUiText.setString(s);
        mUiText.setPosition(pos);
        win.draw(mUiText);
    }

    void DebugUI::DrawRectAt(sf::RenderWindow &win, sf::Vector2f pos, sf::Vector2f size) {
        mUiRect.setPosition(pos);
        mUiRect.setSize(size);
        win.draw(mUiRect);
    }

    DebugUI::DebugUI(CellManager* manager) : mUiFont(sf::Font("resources/font.otf")), mUiText(mUiFont), mManager(manager) {
        
        mUiRect = sf::RectangleShape(sf::Vector2f(100, 100));

        addBrush<Sand>(ELLIPSE);
        addBrush<RedSand>(ELLIPSE);
        addBrush<Stone>(ELLIPSE);
        addBrush<Water>(ELLIPSE);
        addBrush<Generator>(ELLIPSE);
        
        addBrush<Sand>(RECTANGLE);
        addBrush<RedSand>(RECTANGLE);
        addBrush<Stone>(RECTANGLE);
        addBrush<Water>(RECTANGLE);
        addBrush<Generator>(RECTANGLE);

        mEraser = new Eraser(manager);
        
        mUiText.setFillColor(sf::Color::White);
        mUiCircle.setFillColor(sf::Color::White);

        mSizes = {
            50,
            100,
            1000
        };

        mDisplaySizes = {
            5,
            10,
            15
        };
        
    }

    DebugUI::~DebugUI() {
        for (auto brush : mEllipseBrushes) {
            delete brush;
        }
        for (auto brush : mRectBrushes) {
            delete brush;
        }
        
    }

    Brush * DebugUI::getCurrentBrush() {
        Brush* br;
        switch (mCurrentType) {
            case ELLIPSE: 
                br = mEllipseBrushes[mCurrentBrushIndex];
                break;
            case RECTANGLE:
                br = mRectBrushes[mCurrentBrushIndex];
                break;
                default:
                br = mEllipseBrushes[0];
            case ERASER:
                br = mEraser;
        }
        
        br->SetSize(mDrawWidth, mDrawHeight);
        br->SetDensity(mDrawDensity);
        return br;
    }
    
    void DebugUI::draw(sf::RenderWindow &target) {

        mUiRect.setFillColor(sf::Color::Black);
        mUiRect.setSize(mResetSize);
        mUiRect.setPosition(mResetPositon);
        target.draw(mUiRect);
        
        DrawTextAt(target, "FPS :" + std::to_string(mManager->GetClock().GetFPS()), mFPSPositon);
        
        DrawTextAt(target, "Entities :" + std::to_string(mManager->GetMaterialCount()), mEntityPosition);
        
        DrawTextAt(target, "Parameters", mParameters);
        
        mUiRect.setFillColor(sf::Color::White);

        sf::Vector2i cellPosition = sf::Mouse::getPosition(target);
        sf::Vector2f screenPosition = sf::Vector2f(cellPosition.x, cellPosition.y);
        float lastDisplaySize = 0;
        for (int i = 0; i < mSizes.size(); i++) {

            if (i != mCurrentSizeIndex) {
                mUiCircle.setFillColor(sf::Color::White);
                mUiRect.setFillColor(sf::Color::White);
            } else {
                switch (mCurrentType) {
                    case ELLIPSE: {
                        mUiCircle.setFillColor(sf::Color::Red);
                        break;
                    }
                    case RECTANGLE: {
                        mUiRect.setFillColor(sf::Color::Red);
                        break;
                    }
                }
            }
            
            mUiCircle.setPosition(mDrawEllipsePosition + sf::Vector2f(0.0f, lastDisplaySize + mDisplaySizes[i]*2 + 10));
            mUiCircle.setRadius(mDisplaySizes[i]);
            target.draw(mUiCircle);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && mUiCircle.getGlobalBounds().contains(screenPosition)) {
                mDrawWidth = mSizes[i];
                mDrawHeight = mSizes[i];
                mCurrentType = ELLIPSE;
                mCurrentSizeIndex = i;
            }

            mUiRect.setPosition(mDrawRectanglePosition + sf::Vector2f(0.0f,  lastDisplaySize + mDisplaySizes[i]*2 + 10));
            mUiRect.setSize(sf::Vector2f(mDisplaySizes[i]*2.0f, mDisplaySizes[i]*2.0f));
            target.draw(mUiRect);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && mUiRect.getGlobalBounds().contains(screenPosition)) {
                mDrawWidth = mSizes[i];
                mDrawHeight = mSizes[i];
                mCurrentType = RECTANGLE;
                mCurrentSizeIndex = i;
            }

            lastDisplaySize += mDisplaySizes[i]*2.0f;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            mDrawDensity -= 20;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
            mDrawDensity += 20;
        }
        DrawTextAt(target, "Density " + std::to_string(mDrawDensity), mDensityPosition);
        
        for (int i = 0; i < mEllipseBrushes.size(); i++) {
            Brush* br = mEllipseBrushes[i];
            mUiRect.setFillColor(br->GetColor());
            DrawRectAt(target, mMaterialsPosition + sf::Vector2f(30 * (i%3), 30 * (i/3)), sf::Vector2f(20, 20));
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && mUiRect.getGlobalBounds().contains(screenPosition)) {
                mCurrentBrushIndex = i;
            }
        }

        DrawRectAt(target, mEraserPosition, sf::Vector2f(20, 20));
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left) && mUiRect.getGlobalBounds().contains(screenPosition)) {
            mCurrentType = ERASER;
        }
    }
} // US
