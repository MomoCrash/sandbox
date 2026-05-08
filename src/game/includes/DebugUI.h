#pragma once

#include <EllipseBrush.h>
#include <RectangleBrush.h>

namespace US {

    enum BrushTypes {
        ELLIPSE,
        RECTANGLE,

        ERASER,
    };
    
    class DebugUI {
        
        sf::RectangleShape      mUiRect;
        sf::CircleShape         mUiCircle;
        sf::Font                mUiFont;
        sf::Text                mUiText;
        
        std::vector<Brush*>     mEllipseBrushes;
        std::vector<Brush*>     mRectBrushes;
        Brush*                  mEraser;

        std::vector<int>        mSizes;
        std::vector<int>        mDisplaySizes;
        
        int             mCurrentBrushIndex = 0;
        int             mCurrentSizeIndex  = 0;
        BrushTypes      mCurrentType = ELLIPSE;

        CellManager*    mManager;

        int             mDrawWidth              = 10;
        int             mDrawHeight             = 10;
                            
        int             mDrawDensity            = 40;

        sf::Vector2f    mResetPositon           {1080, 0};
        sf::Vector2f    mFPSPositon             {1080, 20};
        sf::Vector2f    mEntityPosition         {1080, 50};
        sf::Vector2f    mParameters             {1080, 100};
        sf::Vector2f    mDrawEllipsePosition    {1080, 160};
        sf::Vector2f    mDrawRectanglePosition  {1130, 160};
        sf::Vector2f    mDensityPosition        {1080, 300};
        sf::Vector2f    mMaterialsPosition      {1080, 350};
        sf::Vector2f    mEraserPosition         {1140, 680};

        sf::Vector2f    mResetSize              {1080, 720};

        void DrawTextAt(sf::RenderWindow& win, std::string s, sf::Vector2f pos);
        void DrawRectAt(sf::RenderWindow& win, sf::Vector2f pos, sf::Vector2f size);

    public:
        DebugUI(CellManager *manager);

        ~DebugUI();

        Brush* getCurrentBrush();
        template<typename M>
        void addBrush(BrushTypes type);

        void draw(sf::RenderWindow &target);
    };

    template<typename M>
    void DebugUI::addBrush(BrushTypes type) {
        switch (type) {
            case ELLIPSE: {
                Brush* brush = new EllipseBrush<M>(mManager);
                brush->SetSize(mDrawWidth, mDrawHeight);
                brush->SetDensity(mDrawDensity);
                mEllipseBrushes.push_back(brush);
                break;
            }
            case RECTANGLE: {
                Brush* brush = new RectangleBrush<M>(mManager);
                brush->SetSize(mDrawWidth, mDrawHeight);
                brush->SetDensity(mDrawDensity);
                mRectBrushes.push_back(brush);
                break;
            }
        }
    }
} // US