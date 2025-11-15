#pragma once

#include "Actor.h"
#include "Player.h"

class CollectibleItem : public Actor
{
public:
	CollectibleItem(float x, float y, Game* game);
	void update();
	void collect();
	bool collected = false;
	int points = 10;
};