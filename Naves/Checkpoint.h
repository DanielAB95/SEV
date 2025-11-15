#pragma once

#include "Actor.h"
#include "Player.h"

class Checkpoint : public Actor
{
public:
	Checkpoint(float x, float y, Game* game);
	void update();
	void activate();
	bool active = false;
	bool used = false;
};