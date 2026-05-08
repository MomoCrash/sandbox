#pragma once
#include "SFML/Graphics.hpp"

namespace US 
{
	class Cell;
	class CellManager;

	enum CellType : uint64_t {

		EMPTY		= 0x0000000000000000,

		SOLID		= 0x0000000000000001,
		LIQUID		= 0x0000000000000010,
		GAS			= 0x0000000000000100,

		SAND		= 0x0000000000001000,
		RED_SAND	= 0x0000000000002000,
		STONE		= 0x0000000000010000,
		WATER		= 0x0000000000100000,
		ACID 		= 0x0000000000200000,
		GENERATOR	= 0x1000000000000000,

	};

	template<typename M>
	concept HasColor = requires(M material) { M::Color; };

	class Material {
	protected:
		/* === CONST DATA === */
		float mHeat;
		float mResistance;
		bool mAffectedByPhysics;

		/* === DYNAMIC DATA === */
		bool mIsFreeFalling;
		float mVelocityX;
		float mVelocityY;

		Material();
		
		static Cell* LookDiagonally(CellManager& manager, Cell const& srcCell, Cell& dstCell);
		static Cell const* RayCast(CellManager& manager, Cell& cell, int dx, int dy, int maxDistance = 100);
	public:
		virtual ~Material() = default;
		
		virtual CellType GetType();
		virtual uint64_t GetTypes();
		bool CheckHasType(CellType type);
		bool CheckIsType(CellType type);

		virtual sf::Color& GetColor();

		virtual void Step(CellManager& manager, Cell const &srcCell, Cell &dstCell) = 0;
	};
}