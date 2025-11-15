#include "LifesPowerUp.h"

LifesPowerUp::LifesPowerUp(float x, float y, Game* game)
	: PowerUp("res/corazon.png", x, y, 44, 36, game) {

	vx = -2;
}

void LifesPowerUp::effect(Player* player) {
	player->lives++;
}
