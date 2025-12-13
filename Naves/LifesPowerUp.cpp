#include "LifesPowerUp.h"
#include <iostream>
using namespace std;

LifesPowerUp::LifesPowerUp(float x, float y, Game* game)
	: PowerUp("res/corazon.png", x, y, 44, 36, game) {

}

void LifesPowerUp::effect(Player* player) {
	// Curar un 30% de la vida máxima (mínimo 10 de vida)
	int healAmount = max(10, static_cast<int>(player->maxLives * 0.3f));
	player->lives = min(player->maxLives, player->lives + healAmount);
	cout << "¡Power-up de vida recogido! Curado +" << healAmount << " HP. Vida actual: " << player->lives << "/" << player->maxLives << endl;
}
