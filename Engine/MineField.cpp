#include "MineField.h"
#include<random>
#include "SpriteCodex.h"
#include <assert.h>
void MineField::Tile::spawnMine()
{
	hasBomb = true;
}

void MineField::Tile::Draw(Vei2& screenPos,Graphics& gfx,bool gameIsOver)
{
	 
	if (!gameIsOver) {
		switch (state)
		{

		case State::Hidden:

			SpriteCodex::DrawTileButton(screenPos, gfx);
			break;
		case State::Revealed:
			if (!hasMine()) {
				SpriteCodex::drawTileNumber(gfx, screenPos, nNeighbourMines);


			}
			else {
				SpriteCodex::DrawTileBomb(screenPos, gfx);

			}
			break;
		case State::Flagged:
			SpriteCodex::DrawTileButton(screenPos, gfx);
			SpriteCodex::DrawTileFlag(screenPos, gfx);
			break;
		}
	}
	
	
	
}

bool MineField::Tile::hasMine()
{
	return hasBomb;
}

void MineField::Tile::setNeighbourMineCount(int mineCount)
{
	assert(nNeighbourMines == -1);
	nNeighbourMines = mineCount;

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

void MineField::Tile::toggleFlag(MineField& field)
{
	if (!field.gameIsOver) {
		if (state == State::Flagged) {
			state = State::Hidden;
		}
		else if (state == State::Hidden) {
			state = State::Flagged;
		}
	}
}


void MineField::Tile::Reveal(MineField& field)
{
	if (!field.gameIsOver) {
		if (!hasFlag()) {
			if (hasMine())
				field.gameIsOver = true;
			state = State::Revealed;
		}
	}
}





int MineField::countNeighbourMines(Vei2& gridPos)
{
	int xStart = std::max(0, gridPos.x - 1);
	int xEnd = std::min(gridWidth - 1, gridPos.x + 1);
	int count = 0;
	int yStart = std::max(0, gridPos.y - 1);
	int yEnd = std::min(gridHeight - 1, gridPos.y + 1);

	for (Vei2 gvect = { xStart,yStart }; gvect.x <= xEnd; gvect.x++) {
		for (gvect.y = yStart; gvect.y <= yEnd; gvect.y++) {
			if (tileAt(gvect).hasMine()) {
				count++;
			}
		}
	
	}
	return count;
}

Vei2& MineField::gridToScreen(Vei2& gridPos)
{
	return gridPos * SpriteCodex::tileSize;
}

Vei2& MineField::screenToGrid(Vei2& screenPos)
{
	return screenPos / SpriteCodex::tileSize;
}

void MineField::onRevealClick(Vei2& screenPos)
{
	if (tileAt(screenToGrid(screenPos)).hasMine()) {
		gameIsOver = true;
	}
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
	
	for (Vei2 gridPos = { 0,0 }; gridPos.y < gridHeight; gridPos.y++) {
		for (gridPos.x = 0; gridPos.x < gridWidth; gridPos.x++) {
			tileAt(gridPos).setNeighbourMineCount(countNeighbourMines(gridPos));
		}
	}

}

void MineField::Draw(Graphics& gfx, Vei2& screenPos)
{
	gfx.DrawRect(getRect(screenPos), SpriteCodex::baseColor);	
	
	Vei2 screenPosTemp;
	for (Vei2 gridPos= { 0,0 }; gridPos.x < gridWidth; gridPos.x++) {
		for (gridPos.y = 0; gridPos.y < gridHeight; gridPos.y++) {
			screenPosTemp = gridToScreen(gridPos);
			tileAt(gridPos).Draw(screenPosTemp, gfx,gameIsOver);
			
		}

	}

}

MineField::Tile& MineField::tileAt(const Vei2& gridPos)
{
	return tiles[gridPos.x + gridWidth * gridPos.y];
}

MineField::Tile& MineField::tileAtMouse(Vei2& screenPos)
{
	Vei2 gridPos = screenToGrid(screenPos);

	return tiles[gridPos.x + gridWidth * gridPos.y];
}

RectI MineField::getRect(Vei2& topLeft)
{
	return RectI(topLeft, SpriteCodex::tileSize * gridWidth, SpriteCodex::tileSize * gridHeight);
}