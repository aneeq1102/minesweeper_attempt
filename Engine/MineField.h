#pragma once
#include "Vei2.h"
#include"graphics.h"

class MineField
{
private:
	class Tile {
		public:
			enum class State{
				Hidden,
				Flagged,
				Revealed
		
			};

		public:
			void spawnMine();
			void Draw(Vei2& screenPos,Graphics& gfx);
			bool hasMine();
			void setNeighbourMineCount(int mineCount);
			Vei2& screenToGrid(Vei2& screenPos);
			bool hasFlag();
			void toggleFlag();
			void Reveal();
			
		private:
			State state = State::Hidden;
			int nNeighbourMines = -1;
			bool isRevealed = false;
			bool hasBomb = false;
			
	
	};

public:
	MineField();
	void Draw(Graphics& gfx, Vei2& screenPos);
	Tile& tileAt(const Vei2& gridPos);
	Tile& tileAtMouse(Vei2& screenPos);
	RectI getRect(Vei2& topLeft);
	int countNeighbourMines(Vei2& gridPos);
	Vei2& gridToScreen(Vei2& gridPos);
	Vei2& screenToGrid(Vei2& screenPos);
private:
	int static constexpr gridWidth = 10;
	int static constexpr gridHeight = 10;
	Tile tiles[gridWidth * gridHeight];
	
	int static constexpr nMines = 10;
};

