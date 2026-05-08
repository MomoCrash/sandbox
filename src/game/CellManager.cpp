#include "CellManager.h"
#include "DebugLayer.h"

namespace US {
    CellManager::CellManager(int width, int height)
        : GameManager(), 
        mDebugLayer(new DebugLayer(this, width, height)), 
        mThreadNumber(0), 
        mHasUpdate(false), 
        mSwappingBuffers(false),
        mCellUpdateProcess(US::MakeFunction(ColumnUpdateProcess)),
        mCellPresentProcess(US::MakeFunction(ColumnPresentProcess))
    {
        mWidth = width * INV_CELL_SIZE;
        mHeight = height * INV_CELL_SIZE;

        mSize = mWidth * mHeight;
        mBufferedCells = new Cell[mSize];
        mPresentCells = new Cell[mSize];

        mCellsInfo = sf::Image(sf::Vector2u(width, height));
        mImageWidth = width;
        mImageHeight = height;

        for (int i = 0; i < mSize; i++) {
            mBufferedCells[i] = Cell(i % mWidth, i / mWidth);
            mPresentCells[i] = Cell(i % mWidth, i / mWidth);
        }

        for (int x = 0; x < mWidth; x++) {
            for (int y = 0; y < mHeight; y++) {
                bool isNotUpperBorder = y > 0;
                bool isNotDownBorder = y < mHeight - 1;
                bool isNotRightBorder = x < mWidth - 1;
                bool isNotLeftBorder = x > 0;

                Cell &writeCell = mPresentCells[y * mWidth + x];
                if (isNotUpperBorder)
                    writeCell.UP = &mPresentCells[(y - 1) * mWidth + x];
                if (isNotDownBorder)
                    writeCell.DOWN = &mPresentCells[(y + 1) * mWidth + x];
                if (isNotRightBorder)
                    writeCell.RIGHT = &mPresentCells[y * mWidth + (x + 1)];
                if (isNotLeftBorder)
                    writeCell.LEFT = &mPresentCells[y * mWidth + (x - 1)];

                if (isNotUpperBorder && isNotRightBorder)
                    writeCell.UP_RIGHT = &mPresentCells[(y - 1) * mWidth + (x + 1)];
                if (isNotUpperBorder && isNotLeftBorder)
                    writeCell.UP_LEFT = &mPresentCells[(y - 1) * mWidth + (x - 1)];
                if (isNotDownBorder && isNotRightBorder)
                    writeCell.DOWN_RIGHT = &mPresentCells[(y + 1) * mWidth + (x + 1)];
                if (isNotDownBorder && isNotLeftBorder)
                    writeCell.DOWN_LEFT = &mPresentCells[(y + 1) * mWidth + (x - 1)];

                Cell &readCell = mBufferedCells[y * mWidth + x];
                if (isNotUpperBorder)
                    readCell.UP = &mBufferedCells[(y - 1) * mWidth + x];
                if (isNotDownBorder)
                    readCell.DOWN = &mBufferedCells[(y + 1) * mWidth + x];
                if (isNotRightBorder)
                    readCell.RIGHT = &mBufferedCells[y * mWidth + (x + 1)];
                if (isNotLeftBorder)
                    readCell.LEFT = &mBufferedCells[y * mWidth + (x - 1)];

                if (isNotUpperBorder && isNotRightBorder)
                    readCell.UP_RIGHT = &mBufferedCells[(y - 1) * mWidth + (x + 1)];
                if (isNotUpperBorder && isNotLeftBorder)
                    readCell.UP_LEFT = &mBufferedCells[(y - 1) * mWidth + (x - 1)];
                if (isNotDownBorder && isNotRightBorder)
                    readCell.DOWN_RIGHT = &mBufferedCells[(y + 1) * mWidth + (x + 1)];
                if (isNotDownBorder && isNotLeftBorder)
                    readCell.DOWN_LEFT = &mBufferedCells[(y + 1) * mWidth + (x - 1)];
            }
        }
    }

    CellManager::~CellManager() {
        
        for (int i = 0; i < mMaterials.Size(); ++i) {
            delete mMaterials[i];
        }

        delete mCellUpdateScheduler;
        delete mCellPresentScheduler;
        
        delete[] mBufferedCells;
        delete[] mPresentCells;
        
    }

    void CellManager::StartScheduler() {
        
        if (mCellUpdateScheduler != nullptr) return;
        if (mCellPresentScheduler != nullptr) return;
        
        int columnWidth = mWidth / mThreadNumber;

        std::vector<ThreadInfo<ThreadColumnInfo>> threadUpdate;
        std::vector<ThreadInfo<ThreadColumnInfo>> threadpresent;
        
        int currentLayer = 0;
        int x = 0;
        for (; mWidth-x > columnWidth; x+=columnWidth) {
            ThreadInfo<ThreadColumnInfo> infoUpdate;
            ThreadInfo<ThreadColumnInfo> infoPresent;
            CreateThreadInfos(&infoUpdate, currentLayer, x, x+columnWidth);
            CreateThreadInfos(&infoPresent, 0, x, x+columnWidth);

            ++currentLayer%=2;
            threadUpdate.push_back(infoUpdate);
            threadpresent.push_back(infoPresent);
        }

        int leftSpace = mWidth - x;
        if (leftSpace > 0) {
            ThreadInfo<ThreadColumnInfo> infoUpdate;
            ThreadInfo<ThreadColumnInfo> infoPresent;
            CreateThreadInfos(&infoUpdate, currentLayer, x, mWidth);
            CreateThreadInfos(&infoPresent, 0, x, mWidth);

            ++currentLayer %= 2;
            threadUpdate.push_back(infoUpdate);
            threadpresent.push_back(infoPresent);
        }

        mCellUpdateScheduler = new Scheduler(&mCellUpdateProcess, threadUpdate);
        mCellPresentScheduler = new Scheduler(&mCellPresentProcess, threadpresent);
    }

    void CellManager::SetThreadCount(uint8_t threadCount) {
        mThreadNumber = threadCount;
    }

    void CellManager::Start() {
        
        StartScheduler();
        
    }

    void CellManager::Update() {
        mCellUpdateScheduler->Update();
        mDebugLayer->UpdateInputs();

        //Swap layers and buffer
        SwapBuffers();
        
        mCellPresentScheduler->Update();

        mDebugLayer->draw();
    }

    void CellManager::End() {
        
        mCellUpdateScheduler->TerminateWait();
        mCellPresentScheduler->TerminateWait();

        delete mDebugLayer;
        
    }

    bool CellManager::ColumnUpdateProcess(ThreadInfo<ThreadColumnInfo> *pThreadInfo) {

        ThreadColumnInfo* pInfos = pThreadInfo->pData;

        for (int x = pInfos->WorkStartX; x < pInfos->WorkMaxX; ++x) {
            
            for (int y = pInfos->WorkStartY; y < pInfos->WorkMaxY; ++y) {

                Cell const& srcCell     = pInfos->Manager->ReadCellAt(x, y);
                Cell&       dstCell     = pInfos->Manager->WriteCellAt(x, y);

                if (srcCell.Material != nullptr)
                    srcCell.Material->Step(*pInfos->Manager, srcCell, dstCell);
                
            }
            
        }

        return true;
        
    }

    bool CellManager::ColumnPresentProcess(ThreadInfo<ThreadColumnInfo>* pThreadInfo)
    {
        ThreadColumnInfo* pInfos = pThreadInfo->pData;

        for (int x = pInfos->WorkStartX; x < pInfos->WorkMaxX; ++x) {

            for (int y = pInfos->WorkStartY; y < pInfos->WorkMaxY; ++y) {

                Cell const& srcCell = pInfos->Manager->ReadCellAt(x, y);
                Cell& dstCell = pInfos->Manager->WriteCellAt(x, y);

                // draw present
                if (srcCell.Material != nullptr) {
                    pInfos->Manager->DrawCell(srcCell.Position.x, srcCell.Position.y, srcCell.Material->GetColor());
                }
                else {
                    pInfos->Manager->DrawCell(srcCell.Position.x, srcCell.Position.y, sf::Color::Black);
                }

                // Clean Buffered
                dstCell.Material = nullptr;
            }

        }

        return true;
    }

    void CellManager::SwapBuffers() {
        
        Cell* presentAddr = mPresentCells;
        mPresentCells   = mBufferedCells;
        mBufferedCells  = presentAddr;
    }

    Cell const * CellManager::GetPresentCells() {
        return mPresentCells;
    }

    Cell const * CellManager::GetBufferedCells() {
        return mBufferedCells;
    }

    sf::Image & CellManager::GetCellsColor() {
        return mCellsInfo;
    }

    sf::Vector2i CellManager::ScreenSpaceToWorldSpace(sf::Vector2i const& world) {
        return {static_cast<int>(static_cast<float>(world.x) * INV_CELL_SIZE), static_cast<int>(static_cast<float>(world.y) * INV_CELL_SIZE)};
    }

    Cell const& CellManager::ReadCellAt(uint32_t x, uint32_t y) {
        return mPresentCells[y * mWidth + x];
    }

    Cell const & CellManager::ReadCellAt(uint32_t index) {
        return mPresentCells[index];
    }

    std::vector<Cell const*> CellManager::RayCastAll(int x, int y, int dx, int dy, int maxDistance)
    {

        std::vector<Cell const*> result;

        Cell const& origin = ReadCellAt(x, y);

        float slope = static_cast<float>(dy) / static_cast<float>(abs(dx));
        int sign = (dx > 0 ? 1 : -1);

        for (int i = 0; i < maxDistance; ++i) {

            int offset_x = sign * i;
            int offset_y = slope * i;

            if (IsInWorld(origin.Position.x + offset_x, origin.Position.y + offset_y) == false) {
                return result;
            }

            Cell const& current = ReadCellAt(origin.Position.x + offset_x, origin.Position.y + offset_y);

            if (ReadCellAt(origin.Position.x + offset_x, origin.Position.y + offset_y).IsEmpty() == false) {
                return result;
            }
            else {
                result.push_back(&current);
            }
        }

        return result;
    }

    Cell & CellManager::WriteCellAt(uint32_t x, uint32_t y) {
        return mBufferedCells[y  * mWidth + x];
    }

    Cell & CellManager::WriteCellAt(uint32_t index) {
        return mBufferedCells[index];
    }

    void CellManager::CleanCell(uint32_t x, uint32_t y)
    {
        if (!IsInWorld(x, y)) {
            return;
        }

        Cell const& srcCell = ReadCellAt(x, y);
        Cell & dstCell = WriteCellAt(x, y);
        if (!srcCell.IsEmpty()) {
            dstCell.Material = nullptr;
        }
    }

    void CellManager::SwapCellAt(uint32_t xa, uint32_t ya, uint32_t xb, uint32_t yb) {

        Cell const& cellA = ReadCellAt(xa, ya);
        Cell const& cellB = ReadCellAt(xb, yb);
        
        Cell& newCellA = WriteCellAt(xa, ya);
        newCellA.Material = cellB.Material;

        Cell& newCellB = WriteCellAt(xb, yb);
        newCellB.Material = cellA.Material;

    }

    void CellManager::SwapCellAt(Cell const&cellA, Cell const&cellB) {
        SwapCellAt(cellA.Position.x, cellA.Position.y, cellB.Position.x, cellB.Position.y);
    }

    void CellManager::GoTo(Cell const &srcCell, Cell &dstCell) {
        if(dstCell.IsEmpty())
		    dstCell.Material = srcCell.Material;
    }

    void CellManager::DrawCell(uint32_t originX, uint32_t originY, sf::Color cellColor) {

        originX *= CELL_SIZE;
        originY *= CELL_SIZE;

        for (uint32_t x = originX; x < originX + CELL_SIZE; x++) {
            for (uint32_t y = originY; y < originY + CELL_SIZE; y++) {
                if (!IsInScreen(x, y)) continue;
                mCellsInfo.setPixel(sf::Vector2u(x, y), cellColor);
            }
        }
        
    }

    sf::Color CellManager::GetCellColorAt(uint32_t x, uint32_t y) {
        return mCellsInfo.getPixel(sf::Vector2u(x * CELL_SIZE, y * CELL_SIZE));
    }

    bool CellManager::IsInScreen(uint32_t x, uint32_t y) const {
        return (x < mImageWidth && y < mImageHeight);
    }

    bool CellManager::IsInWorld(int x, int y) const {
        return (x >= 0 && x < mWidth && y >= 0 && y < mHeight);
    }

    void CellManager::CreateThreadInfos(ThreadInfo<ThreadColumnInfo> *info, int layer, uint32_t startX, uint32_t endX) {

        info->pData = new ThreadColumnInfo();
        info->pData->WorkStartX = startX;
        info->pData->WorkStartY = 0;
        info->pData->WorkMaxX = endX;
        info->pData->WorkMaxY = mHeight;
        info->pData->Manager = this;
        
        info->Layer = layer;
        
    }
    
} // US