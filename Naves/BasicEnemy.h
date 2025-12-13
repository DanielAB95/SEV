#pragma once
#include "Enemy.h"
#include "Player.h"

class BasicEnemy : public Enemy
{
public:
	BasicEnemy(float x, float y, Game* game);
	void update() override;
	void moveTowardsPlayer(Player* player); // Método para perseguir al jugador
};

