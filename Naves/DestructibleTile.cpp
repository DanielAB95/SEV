#include "DestructibleTile.h"

DestructibleTile::DestructibleTile(string filename, float x, float y, Game* game)
	: Tile(filename, x, y, game) {
	lives = 3;
	destroyed = false;
}

void DestructibleTile::update() {
	if (lives <= 0 && !destroyed) {
		destroy();
	}
}

void DestructibleTile::destroy() {
	destroyed = true;
}