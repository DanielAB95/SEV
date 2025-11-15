#pragma once

#include "Tile.h"

class DestructibleTile : public Tile
{
public:
	DestructibleTile(string filename, float x, float y, Game* game);
	void update();
	void destroy();
	int lives = 3;
	bool destroyed = false;
};