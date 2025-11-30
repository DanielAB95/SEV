#include "LifesPowerUp.h"
#include <iostream>
using namespace std;

LifesPowerUp::LifesPowerUp(float x, float y, Game* game)
	: PowerUp("res/corazon.png", x, y, 44, 36, game) {

}

void LifesPowerUp::effect(Player* player) {
	player->lives++;
	cout << "¡Power-up de vida recogido! Vidas: " << player->lives << endl;
}
