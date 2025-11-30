#include "ShootPowerUp.h"

ShootPowerUp::ShootPowerUp(float x, float y, Game* game)
	: PowerUp("res/icono_recolectable.png", x, y, 40, 40, game) {

}

void ShootPowerUp::effect(Player* player) {
	player->numberOfShoots = player->numberOfShoots + 10;
}
