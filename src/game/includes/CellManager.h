#pragma once

#include "Cell.h"
#include <vector>

#include "GameManager.h"
#include "Scheduler.h"

#include "UnorderedArray.h"
#include "SFML/Graphics/Image.hpp"

namespace US {
    class DebugLayer;

    struct ThreadColumnInfo {

        int     WorkStartX;
        int     WorkStartY;
        int     WorkMaxX;
        int     WorkMaxY;
        
        CellManager* Manager;
        
    };

    class CellManager final : public GameManager {
        DebugLayer*         mDebugLayer;
        
        Cell*               mPresentCells   ;
        Cell*               mBufferedCells  ;
        sf::Image           mCellsInfo;

        uint32_t        mSize;
        uint32_t        mWidth;
        uint32_t        mHeight;

        uint32_t        mImageWidth;
        uint32_t        mImageHeight;
        
        uint32_t        mThreadNumber;

        bool            mHasUpdate;
        bool            mSwappingBuffers;

        UnorderedArray<Material*>               mMaterials;
        
        US::CustomFunction<bool, ThreadInfo<ThreadColumnInfo>*>     mCellUpdateProcess      = nullptr;
        Scheduler<ThreadColumnInfo>*                                mCellUpdateScheduler    = nullptr;

        US::CustomFunction<bool, ThreadInfo<ThreadColumnInfo>*>     mCellPresentProcess     = nullptr;
        Scheduler<ThreadColumnInfo>*                                mCellPresentScheduler   = nullptr;
        
        // ===== Multithreading ==== //
        static bool ColumnUpdateProcess(ThreadInfo<ThreadColumnInfo> * pThreadInfo);
        static bool ColumnPresentProcess(ThreadInfo<ThreadColumnInfo> * pThreadInfo);

        void SwapBuffers();

    public:
        CellManager(int width, int height);
        ~CellManager();

        void StartScheduler();
        void SetThreadCount(uint8_t threadCount);

        void Start() override;
        void Update() override;
        void End() override;

        Cell const&     ReadCellAt (uint32_t x, uint32_t y);
        Cell const&     ReadCellAt (uint32_t index);

        std::vector<Cell const*> RayCastAll(int x, int y, int dx, int dy, int maxDistance);

        Cell&           WriteCellAt (uint32_t x, uint32_t y);
        Cell&           WriteCellAt (uint32_t index);

        template<typename M> requires (std::is_base_of_v<Material, M> && HasColor<M>)
        void            SpawnCellAt(uint32_t x, uint32_t y);
        void            CleanCell(uint32_t x, uint32_t y);

        void            SwapCellAt(uint32_t xa, uint32_t ya, uint32_t xb, uint32_t yb);
        void            SwapCellAt(Cell const& cellA, Cell const& cellB);
        void            GoTo(Cell const& srcCell, Cell& dstCell);

        sf::Vector2i    ScreenSpaceToWorldSpace(sf::Vector2i const& world);
        void            DrawCell(uint32_t originX, uint32_t originY, sf::Color cellColor);
        sf::Color       GetCellColorAt(uint32_t x, uint32_t y);

        bool            IsInScreen(uint32_t x, uint32_t y) const;
        bool            IsInWorld(int x, int y) const;

        void            CreateThreadInfos(ThreadInfo<ThreadColumnInfo> *info, int layer, uint32_t startX, uint32_t endX);


        // ===== GET / SET ===== //
        Cell const*     GetPresentCells();
        Cell const*     GetBufferedCells();
        sf::Image&      GetCellsColor ();
        
        uint32_t        GetSize() const { return mSize; }
        uint32_t        GetMaterialCount() { return mMaterials.Size(); }
        
    };

    template<typename M> requires (std::is_base_of_v<Material, M>&& HasColor<M>)
    inline void CellManager::SpawnCellAt(uint32_t x, uint32_t y)
    {
        if (!IsInWorld(x, y)) {
            return;
        }

        Cell const& srcCell = ReadCellAt(x, y);
        if (!srcCell.IsEmpty()) {
            return;
        }

        Cell& cell = WriteCellAt(x, y);
        M* pMat = new M();
        cell.Material = pMat;

        mMaterials.Add(pMat);
    }
}
