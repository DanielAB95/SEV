#pragma once
#include "Enemy.h"
class BasicEnemy : public Enemy
{
public:
	BasicEnemy(float x, float y, Game* game);
	void update() override;
};

