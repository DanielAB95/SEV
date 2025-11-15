#pragma once

#include "Actor.h"

class Tile : public Actor
{
public:
	Tile(string filename, float x, float y, Game* game);
	void draw(float scrollX , float scrollY) override; // Va a re-definir draw
};