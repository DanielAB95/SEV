#pragma once

#include "Actor.h"
#include "Enemy.h"

class Projectile : public Actor
{
public:
	Projectile(float xTarget, float yTarget, float x, float y, Game* game);
	void update();
	float xTarget;
	float yTarget;
	int damage; // Daño que inflige el proyectil
};
