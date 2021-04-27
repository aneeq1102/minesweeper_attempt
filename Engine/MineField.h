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
			Vei2& screenToGrid(Vei2& screenPos);
			bool hasFlag();
			void toggleFlag();
			
		private:
			State state = State::Hidden;
			bool isRevealed = false;
			bool hasBomb = false;
			
	
	};

public:
	MineField();
	void Draw(Graphics& gfx, Vei2& screenPos);
	Tile& tileAt(const Vei2& gridPos);
	RectI getRect(Vei2& topLeft);
	Vei2& gridToScreen(Vei2& gridPos);
private:
	int static constexpr gridWidth = 10;
	int static constexpr gridHeight = 10;
	Tile tiles[gridWidth * gridHeight];
	
	int static constexpr nMines = 10;
};

