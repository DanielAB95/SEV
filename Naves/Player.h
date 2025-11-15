#pragma once

#include "Actor.h"
#include "Projectile.h"
#include "Audio.h"
#include "Animation.h"

class Player : public Actor
{
public:
	Player(float x, float y, Game* game);
	Projectile* shoot();
	void update();
	void moveX(float axis);
	void moveY(float axis);
	void draw(float scrollX = 0, float scrollY = 0) override;
	Animation* aMoving;
	Animation* animation;
	Audio* audioShoot;
	int shootCadence = 22;
	int shootTime = 0;
	int numberOfShoots = 20;
	int lives;
};
