#include "MineField.h"
#include<random>
#include "SpriteCodex.h"
void MineField::Tile::spawnMine()
{
	hasBomb = true;
}

void MineField::Tile::Draw(Vei2& screenPos,Graphics& gfx)
{
	switch (state) {
	
	case State::Hidden:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		break;
	case State::Revealed:
		if (hasMine()) {
			SpriteCodex::DrawTileBomb(screenPos, gfx);
			SpriteCodex::DrawTileButton(screenPos, gfx);
		}
		break;
	case State::Flagged:
		SpriteCodex::DrawTileButton(screenPos, gfx);
		SpriteCodex::DrawTileFlag(screenPos, gfx);
		break;
	}
}

bool MineField::Tile::hasMine()
{
	return hasBomb;
}

Vei2& MineField::Tile::screenToGrid(Vei2& screenPos)
{
	return screenPos / SpriteCodex::tileSize;
}

bool MineField::Tile::hasFlag()
{
	if (state == State::Flagged)
		return true;
	return false;
}

void MineField::Tile::toggleFlag()
{
	state = State::Flagged;
}

Vei2& MineField::gridToScreen(Vei2& gridPos)
{
	return gridPos * SpriteCodex::tileSize;
}



MineField::MineField()
{
	std::uniform_int_distribution<int> xDist(0, gridWidth - 1);
	std::uniform_int_distribution<int> yDist(0, gridHeight - 1);

	std::random_device rd;

	std::mt19937 rng(rd());

	for (int i = 0; i < nMines; i++) {
		Vei2 spawnPos;
		
		do{
			spawnPos = { xDist(rng),yDist(rng) };
		} while (tileAt(spawnPos).hasMine());

		tileAt(spawnPos).spawnMine();
	}
}

void MineField::Draw(Graphics& gfx, Vei2& screenPos)
{
	gfx.DrawRect(getRect(screenPos), SpriteCodex::baseColor);	//drawing gray rect behind
	
	Vei2 screenPosTemp;
	for (Vei2 gridPos= { 0,0 }; gridPos.x < gridWidth; gridPos.x++) {
		for (gridPos.y = 0; gridPos.y < gridHeight; gridPos.y++) {
			screenPosTemp = gridToScreen(gridPos);
			tileAt(gridPos).Draw(screenPosTemp, gfx);
			
		}

	}

}

MineField::Tile& MineField::tileAt(const Vei2& gridPos)
{
	return tiles[gridPos.x + gridWidth * gridPos.y];
}

RectI MineField::getRect(Vei2& topLeft)
{
	return RectI(topLeft, SpriteCodex::tileSize * gridWidth, SpriteCodex::tileSize * gridHeight);
}