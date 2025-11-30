#pragma once
#include "Actor.h"

class EnemyProjectile : public Actor
{
public:
	EnemyProjectile(float x, float y, float targetX, float targetY, Game* game);
	void update();
	float speed;
};
