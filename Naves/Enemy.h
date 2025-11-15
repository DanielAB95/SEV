#pragma once

#include "Actor.h"
#include "Animation.h"

class Enemy : public Actor
{
public:
	Enemy(float x, float y, Game* game);
	void draw(float scrollX = 0, float scrollY = 0) override;
	virtual void update();
	Animation* aMoving;
	Animation* animation;
	int lives;
};
